#ifndef  _PROTOCALSYS_H_
#define  _PROTOCALSYS_H_

#include "GSProtocol.pb.h"
#include "ConndMsg.h"

class ProtocalSys
{
    public:
        ProtocalSys();
        ~ProtocalSys();
        int SendLoginInfo(ConndMsg &stMsg);
    private:
        Connd::Protocol::GSPkg m_stPkg;
};



#endif  
