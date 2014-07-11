#ifndef  _MSGDISPATCHER_H_
#define  _MSGDISPATCHER_H_

#include "ConndMsg.h"

typedef int (*CallBackFun) (ConndMsg& stMsg);

class MsgDispatcher
{
    public:
        MsgDispatcher();
        ~MsgDispatcher();
        int Dispatch(ConndMsg& stMsg);
    private:
        CallBackFun m_vCallBackFuns[MAX_CONND_MSG_ID];
};

#endif

