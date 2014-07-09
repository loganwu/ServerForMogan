/********************************************
FileName:           ImAgentMgr.cpp
Author:             neil
Date:               2012-3-16
Description:        处理ImAgent的消息
Version:            1.0
History:
<author>  <date>   <version>  <desc>
neil      12/3/16  1.0        create
*********************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>

#include "Tracer.h"
#include "RouteSvr.h"
#include "ImAgentMgr.h"
#include "Monitor.h"

using namespace std;
using namespace tencent::im;
using namespace tencent::im::common;
using namespace tencent::im::imagent;
using namespace tencent::im::notify;
using namespace tencent::im::msg;

// 即通给出，不需修改
#define WANG_APP_ID         1006    //wang chao app id
#define APP_ID  WANG_APP_ID
#define KEY_TYPE 27
typedef uint32_t __LenType;
static size_t LEN_SIZE = sizeof(__LenType);
#define ByteSwap    htonl

// 接收ImAgent消息的端口，跟rockdeng确认
#define IM_AGENT_LIS_PORT   10999

ImAgentMgr::~ImAgentMgr()
{
    google::protobuf::ShutdownProtobufLibrary();
}

int ImAgentMgr::InitSock(const int idx, const char *szIp, const char *szImAgentIp, const int iImAgentPort)
{
    if ((NULL == szIp) || (NULL == szImAgentIp) || (iImAgentPort <= 0))
    {
        return -1;
    }
    if(idx>=IM_AGENT_SVR_NUM)
    {
        return -2;
    }

    MAKE_UDPP_LOG_STREAM(LOG);
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if(0 == idx )
    {
        m_iSockFd = socket(AF_INET, SOCK_DGRAM, 0);

        if (m_iSockFd < 0)
        {
            ERROR << "ImAgent socket init fail,error " << strerror(errno) << endl;
            return -1;
        }

        struct sockaddr_in  m_stAddress; //绑定本地端口10999
        memset(&m_stAddress, 0, sizeof(m_stAddress));

        m_stAddress.sin_family = AF_INET;
        m_stAddress.sin_port   = htons(IM_AGENT_LIS_PORT);
        m_stAddress.sin_addr.s_addr = inet_addr(szIp);

        int iReuseAddr = 1;
        setsockopt(m_iSockFd, SOL_SOCKET, SO_REUSEADDR, &iReuseAddr, sizeof(iReuseAddr));

        if (bind(m_iSockFd, (struct sockaddr *)&m_stAddress, sizeof(m_stAddress)) < 0)
        {
            ERROR << "ImAgent socket bind fail,error " << strerror(errno) << endl;
            return -1;
        }
    }
    
    memset(&m_stImAgentAddr[idx], 0, sizeof(m_stImAgentAddr[idx])); //向ImAgent发消息的地址
    m_stImAgentAddr[idx].sin_family = AF_INET;
    m_stImAgentAddr[idx].sin_port   = htons(iImAgentPort);
    m_stImAgentAddr[idx].sin_addr.s_addr = inet_addr(szImAgentIp);

    TRACE << "ImAgent socket init ok." << endl;

    return 0;
}

int ImAgentMgr::OnMsg(int iImAgentSock)
{
    MAKE_UDPP_LOG_STREAM(LOG);

    memset(szRecvBuff, 0, IM_AGENT_BUFF_MAX);
    socklen_t iSockLen = sizeof(struct sockaddr);

    int iLen = recvfrom(iImAgentSock, szRecvBuff, IM_AGENT_BUFF_MAX, 0, &m_stServerAddr, &iSockLen);
    if (iLen < 0)
    {
        if (errno != EINTR)
        {
            ERROR << "ImAgent socket recv fail,error " << strerror(errno) << endl;
        }
        return -1;
    }

    ProcMsg(iImAgentSock,szRecvBuff,iLen,false);
    return 0;
}


int ImAgentMgr::ProcMsg(int iImAgentSock, char *szBuff, const int iMsgLen, bool bRoute)
{
    MAKE_UDPP_LOG_STREAM(LOG);
    if (iMsgLen <= 0 || szBuff == NULL)
    {
        return -1;
    }
    
    ImAgentPackage pkg;

    string szRecvMsg(szBuff, iMsgLen);
    if (!UnserializeMessage(szRecvMsg, pkg))
    {
        ERROR << "UnserializeMessage failed\n";
        return -1;
    }

    if (!pkg.has_head() || !pkg.head().has_req_user())
    {
        ERROR << "protoc no ImAgentHead or no User, pkglen:" << iMsgLen << endl;
        return -1;
    }

    const ImAgentHead &stImMsgHead = pkg.head();
    const User &stImMsgUser = stImMsgHead.req_user();

    //主动发起的消息都是IMAGENT_GROUPMSG_SEND类型，如果是主动从ImAgent发过来的，带包体；
    //如果是从游戏侧发过去的，ImAgent回包没有包体，只有Head
    char szLog[200];
    snprintf(szLog, sizeof(szLog),
             "recv imagent pkg, head info: uin:%u, appid:%d, instsid:%d, clientip:%u, cmd:%d, seq:%u,result %d\n",
             (uint32_t)stImMsgUser.uin(), stImMsgUser.app_id(), stImMsgUser.instance_id(),
             stImMsgUser.client_ip(), stImMsgHead.command(), stImMsgHead.seq(), stImMsgHead.result());
    INFO << szLog;

    ImAgentPackage stRespPkg; //回包

    //非本服务器消息，转发至其他中转服务器，判断转发次数
    unsigned int uiBindSvrId = 0;
    unsigned int uiBindUnionId = 0;
    unsigned int uiGroupId = GetGroupIdFromMsg(pkg);
    if(uiGroupId)
    {
        //没找到绑定群
        int iRet = SingletonHolder<SvrDataMgr>::Instance()->IsGroupBind(uiGroupId, uiBindSvrId, uiBindUnionId);
        if (iRet <= 0)
        {
            //丢弃
            if(bRoute)
            {
                INFO << "Discard Msg From Other RouteSvr, GroupId:" << uiGroupId << endl;                
                SingletonHolder<Monitor>::Instance()->AddCounter(Monitor::MONI_DISCARD_MSG, iMsgLen);
                //unbind???
                return -1;
            }
            else
            //转发至其他RouteSvr
            {
                int iRouteSock = SingletonHolder<RouteSvr>::Instance()->GetRouteSvrSock();
                sockaddr_in stAddr = SingletonHolder<RouteSvr>::Instance()->GetRouteSvrAddr();
                int iLen = sendto(iRouteSock, szBuff, iMsgLen, 0, (struct sockaddr *)&stAddr, sizeof(struct sockaddr));
                SingletonHolder<Monitor>::Instance()->AddCounter(Monitor::MONI_IMNOTIFY_SEND, iLen);
                INFO << "Send To Other RouteSvr Of GroupId:" << uiGroupId << "Len=" << iMsgLen << 
                     "ip=" << stAddr.sin_addr.s_addr << "port=" << htons(stAddr.sin_port) << endl;
                stRespPkg.mutable_head()->CopyFrom(stImMsgHead);
                stRespPkg.mutable_head()->set_result(0);
                stRespPkg.mutable_msg_send_resp();
            }
        }
        else
        {
            if (stImMsgHead.command() == imagent::IMAGENT_GROUP_NOTIFY) // 群通知
            {
                SingletonHolder<Monitor>::Instance()->AddCounter(Monitor::MONI_IMNOTIFY_RECV, iMsgLen);
                if (pkg.has_group_notify_req())
                {
                    DoNotify(pkg.group_notify_req());
                    stRespPkg.mutable_head()->CopyFrom(stImMsgHead); 
                    stRespPkg.mutable_head()->set_result(0);
                    stRespPkg.mutable_group_notify_resp();
                }
                else
                {
                    ERROR << "invalid protoc buf, cmd: %d" << stImMsgHead.command() << endl;
                    return 0;
                }
            }
            else if (stImMsgHead.command() == imagent::IMAGENT_GROUPMSG_SEND) // ImAgent的群消息
            {
                SingletonHolder<Monitor>::Instance()->AddCounter(Monitor::MONI_IM_CHAT_RECV, iMsgLen);
                if (pkg.has_msg_send_req() && pkg.msg_send_req().has_msg())
                {
                    DoImMsg(pkg);
                    stRespPkg.mutable_head()->CopyFrom(stImMsgHead);
                    stRespPkg.mutable_head()->set_result(0);
                    stRespPkg.mutable_msg_send_resp();
                }
                else
                {
                    //发送过去的每条消息都会收到一个33Byte的回包，不处理
                    if (0 != stImMsgHead.result())
                    {
                        ERROR << "ImAgentRsp result " << stImMsgHead.result() << ":" << stImMsgHead.err() << endl;
                    }

                    return 0;
                }
            }
            else
            {
                ERROR << "Ignore ImAgent cmd " << stImMsgHead.command() << endl;
                return -1;
            }
        }
    }
    else
    {
        INFO << "Ignore ImAgent cmd " << stImMsgHead.command() << endl;
        return -1;
    }

    if(!bRoute)
    {
        // 对ImAgent消息回包
        string strBuf;
        if (!SerializeMessage(stRespPkg, strBuf))
        {
            return -1;
        }
        int iLen = sendto(iImAgentSock, &strBuf[0], strBuf.size(), 0, &m_stServerAddr, sizeof(struct sockaddr));
        SingletonHolder<Monitor>::Instance()->AddCounter(Monitor::MONI_IMNOTIFY_SEND, iLen);
    }
    return 0;
}

int ImAgentMgr::DoNotify(const GroupNotifyReq &stNotify)
{
    int iRet = SingletonHolder<RouteSvr>::Instance()->SendImNotify(stNotify.group_code(),
                                                                   stNotify.notify_op(), stNotify.notify_uin());

    MAKE_UDPP_LOG_STREAM(LOG);
    char szLog[200];
    snprintf(szLog, sizeof(szLog),
             "receive group notify info:groupcode:%u,op:%d,uin:%u,RouteSvr ret %d.\n",
             stNotify.group_code(),
             stNotify.notify_op(),
             stNotify.notify_uin(),
             iRet);
    TRACE << szLog;

    return 0;
}

int ImAgentMgr::DoImMsg(ImAgentPackage &stImMsg)
{
    MAKE_UDPP_LOG_STREAM(LOG);

    const Msg &stMsg = stImMsg.msg_send_req().msg();

    if (!stMsg.has_head() || !stMsg.head().has_routing_head())
    {
        ERROR << "ImAgent protoc msg have no routing head !" << endl;
        return -1;
    }

    const RoutingHead &stRout = stMsg.head().routing_head();

    if (stRout.has_group()) // 群消息
    {
        // 来源信息和群信息
        const Group &stGroup = stRout.group();

        if (!stGroup.has_sender() || !stGroup.has_group_info())
        {
            ERROR << "pkg format error: missing group info or sender." << endl;;
            return -21;
        }
        
        return DealMsg(stMsg, stGroup.sender().uin(), stGroup.group_info().group_id());
    }
    else
    {
        ERROR << "Unknow ImAgent msg type!" << endl;
        return -2;
    }

    return 0;
}

int ImAgentMgr::DealMsg(const Msg &stMsg, const unsigned int uiUin, const unsigned int uiGrpCode)
{
    MAKE_UDPP_LOG_STREAM(LOG);
    char szLog[200];

    // 消息是否分包(暂时不需要，可以删除该if)
    if (stMsg.head().has_content_head())
    {
        const ContentHead &stCtHead = stMsg.head().content_head();

        snprintf(szLog, sizeof(szLog), "total pkg:%d, cur:%d, seq:%d\n",
                 stCtHead.pkg_num(), stCtHead.pkg_index(), stCtHead.seq());
        INFO << szLog;
    }

    // 消息内容
    if (!stMsg.has_body() || !stMsg.body().has_rich_text())
    {
        ERROR << "have no msg content !" << endl;
        return -32;
    }

    const RichText &stRich = stMsg.body().rich_text();

    snprintf(szLog, sizeof(szLog), "From uin %u, grp %u, group msg have %d elem\n",
             uiUin, uiGrpCode, stRich.elems_size());
    INFO << szLog;

    string strMsg;
    for (int i = 0; i < stRich.elems_size(); i++)
    {
        const Elem &stElem = stRich.elems(i);

        if (stElem.has_text())
        {
            const Text &stTxt = stElem.text();

            snprintf(szLog, sizeof(szLog), "text msg:%s, link:%s\n",
                     (stTxt.has_str() ? stTxt.str().c_str() : "null"),
                     (stTxt.has_link() ? stTxt.link().c_str() : "null"));
            INFO << szLog;

            strMsg += stTxt.str();
        }

        if (stElem.has_face())
        {
            const Face &stFace = stElem.face();
            snprintf(szLog, sizeof(szLog), "system face index:%u\n", stFace.index());
            INFO << szLog;

            strMsg += "[琛ㄦ儏]"; //"[表情]"UTF8在GBK下表示
        }
    }

    //全部发送，在GameSvr处分包
    if(strMsg.size() > 0)
    {
        SingletonHolder<RouteSvr>::Instance()->SendImMsg(uiUin, uiGrpCode, strMsg.c_str(), strMsg.size());
    }
    return 0;
}

//copy from ImAgent Demo prog,do not modify
bool ImAgentMgr::SerializeMessage(const ::google::protobuf::Message& pkg, string& buf)
{
    MAKE_UDPP_LOG_STREAM(LOG);
    ostringstream oss;

    for (size_t i = 0; i < LEN_SIZE; ++i)
    {
        oss << ' ';
    }

    oss << "{{";

    if (!pkg.SerializeToOstream(&oss))
    {
        ERROR << "SerializeToOstream() failed for pkg\n";
        return false;
    }

    oss << "}}";
    buf = oss.str();

    if (buf.size() > 0xFFFF)
    {
        ERROR << "pkg is too long\n";
        return false;
    }

    __LenType *total_len = reinterpret_cast<__LenType *>(&buf[0]);
    *total_len = ByteSwap(buf.size());
    return true;
}

//copy from ImAgent Demo prog,do not modify
bool ImAgentMgr::UnserializeMessage(const string &buf, ::google::protobuf::Message &pkg)
{
    MAKE_UDPP_LOG_STREAM(LOG);
    const __LenType *p = reinterpret_cast<const __LenType *>(&buf[0]);
    __LenType total_len = ByteSwap(*p);

    if (total_len != buf.size() || total_len < 4 + LEN_SIZE)
    {
        ERROR << "totallen error:" << total_len << ",buf size " << buf.size() << ",len size " << LEN_SIZE << "\n";
        return false;
    }

    if (0 != memcmp(&buf[LEN_SIZE], "{{", 2)
        || 0 != memcmp(&buf[total_len - 2], "}}", 2))
    {
        ERROR << "buf format error\n";
        return false;
    }

    if (!pkg.ParseFromArray(&buf[LEN_SIZE + 2], total_len - LEN_SIZE - 4))
    {
        ERROR << "parse pkg from buf failed\n";
        return false;
    }

    return true;
}

int ImAgentMgr::SendChatMsg(const unsigned int uiSenderUin, const unsigned int uiGrpId,
                            const char *pszSvrIp, const char *pszMsg, const int iMsgLen,
                            const char* pszKey, const int iKeyLen)
{
    MAKE_UDPP_LOG_STREAM(LOG);

    if ((0 == uiSenderUin) || (0 == uiGrpId) || (NULL == pszSvrIp)
        || (NULL == pszMsg) || (iMsgLen <= 0)) //iKeyLen
    {
        return -1;
    }

    //1. pack imagent msg(copy form Demo)
    RichText rich_text;
    Text *text = rich_text.add_elems()->mutable_text();

    if (NULL == text)
    {
        return -1;
    }

    text->set_str(pszMsg);

    static uint32_t pkg_seq = 0; //每个消息唯一
    //set head
    ImAgentPackage req_pkg;
    req_pkg.mutable_head()->set_command(IMAGENT_GROUPMSG_SEND);   //send msg,game2imagent
    req_pkg.mutable_head()->set_seq(++pkg_seq); //溢出为0,可以不处理

    //set key
    req_pkg.mutable_head()->mutable_signature()->set_key_type(KEY_TYPE); 
    req_pkg.mutable_head()->mutable_signature()->set_session_app_id(WCBY_KEY_APPID);
    req_pkg.mutable_head()->mutable_signature()->set_session_key(pszKey);

    req_pkg.mutable_head()->mutable_req_user()->set_uin(uiSenderUin);
    req_pkg.mutable_head()->mutable_req_user()->set_app_id(APP_ID);  //sender's app id
    req_pkg.mutable_head()->mutable_req_user()->set_instance_id(1);//sender's instance id
    req_pkg.mutable_head()->mutable_req_user()->set_app_type(1);   //sender's app type
    req_pkg.mutable_head()->mutable_req_user()->set_client_ip(inet_addr(pszSvrIp));

    //set msg
    Msg *msg = req_pkg.mutable_msg_send_req()->mutable_msg();

    if (NULL == msg)
    {
        return -1;
    }

    msg->mutable_head()->mutable_routing_head()->mutable_group()->mutable_sender()->CopyFrom(req_pkg.mutable_head()->req_user());
    msg->mutable_head()->mutable_routing_head()->mutable_group()->mutable_group_info()->set_group_id(uiGrpId);
    msg->mutable_head()->mutable_routing_head()->mutable_group()->mutable_group_info()->set_group_type(GROUP);

    msg->mutable_head()->mutable_content_head()->set_seq(pkg_seq); //neilwu mod
    msg->mutable_head()->mutable_content_head()->set_date_time(::time(0));  //msg time
    msg->mutable_body()->mutable_rich_text()->CopyFrom(rich_text);

    //serialize
    string strBuf;

    if (!SerializeMessage(req_pkg, strBuf))
    {
        return -1;
    }

    //2. send to imagent
    struct sockaddr_in stAddr;
    CmlbClientApi* pCmlbClientApi=SingletonHolder<RouteSvr>::Instance()->GetCmlbClientApi();
    int iRet = CMLB_V2_GetOneIntf(pCmlbClientApi, IMAGENT_APPID,&stAddr,sizeof(stAddr));
    if (iRet != 0) 
    {
        TRACE << "CMLB_V2_GetOneIntf Failed." << endl;
        return 0;
    }
    stAddr.sin_port=ntohs(PROTOBUF_PORT);//protobuf协议端口18000
    
    //int idx = SingletonHolder<RouteSvr>::Instance()->GetRandomNum(0,2);
    int iSendLen = sendto(m_iSockFd, &strBuf[0], strBuf.size(), 0, (const sockaddr *)&stAddr, sizeof(stAddr));
    if (0 > iSendLen)
    {
        ERROR << "send cmd failed, " << strerror(errno) << endl;
        return -1;
    }
    SingletonHolder<Monitor>::Instance()->AddCounter(Monitor::MONI_GAME_CHAT_SEND, iSendLen);

#ifdef DEBUG
    char szLog[500];
    snprintf(szLog, sizeof(szLog),
             "SendChatMsg2Im,svr ip %s,sender %u,group %u,sendlen %d,msg:%s,key %s, keylen %d.",
             pszSvrIp, uiSenderUin, uiGrpId, iSendLen, pszMsg, pszKey, iKeyLen);
    INFO << szLog << endl;
#endif

    return 0;
}

unsigned int ImAgentMgr::GetGroupIdFromMsg(ImAgentPackage &pkg)
{
    if (!pkg.has_head() || !pkg.head().has_req_user())
    {
        return 0;
    }

    const ImAgentHead &stImMsgHead = pkg.head();
    if (stImMsgHead.command() == imagent::IMAGENT_GROUP_NOTIFY) // 群通知
    {
        if (pkg.has_group_notify_req())
            return pkg.group_notify_req().group_code();
    }
    
    if (stImMsgHead.command() == imagent::IMAGENT_GROUPMSG_SEND) // ImAgent的群消息
    {
        if (pkg.has_msg_send_req() && pkg.msg_send_req().has_msg())
        {
            const Msg &stMsg = pkg.msg_send_req().msg();
            if (!stMsg.has_head() || !stMsg.head().has_routing_head())
            {
                return 0;
            }

            const RoutingHead &stRout = stMsg.head().routing_head();
            if (stRout.has_group())
            {
                const Group &stGroup = stRout.group();
                if (!stGroup.has_sender() || !stGroup.has_group_info())
                {
                    return 0;
                }
                return stGroup.group_info().group_id();
            }
        }
    }
    
    return 0;
}


