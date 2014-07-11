#ifndef  _BUSIENTERGAME_H_
#define  _BUSIENTERGAME_H_

#include "ConndMsg.h"

class BusiEnterGame
{
    public:
        BusiEnterGame(){};
        ~BusiEnterGame(){};
		int OnLogin(ConndMsg& stMsg);
		int OnEnterZone(ConndMsg& stMsg);
    private:
		int FinishLogin(/*RoleInfo* pstRoleInfo*/);
        int CheckAvgLoad();

};

#endif  

