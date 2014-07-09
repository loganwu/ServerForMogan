#ifndef __SESSION_KEY_H__
#define __SESSION_KEY_H__
#include <string.h>

struct SessionKey
{
	SessionKey()
	{
		memset(this,0,sizeof(*this));
	}
	uint32_t uIP;
	uint32_t uPort;
 	bool operator<(const SessionKey & stSKey) const
 	{
 		if (this->uIP < stSKey.uIP ||
 			(this->uIP == stSKey.uIP && this->uPort < stSKey.uPort))
 		{
 			return true;
 		}
 		return false;
 	}

};

class SKeyCompare
{ 
public:
	bool operator()(const SessionKey &x,const SessionKey &y)
	{
		if (x.uIP < y.uIP ||
			(x.uIP == y.uIP && x.uPort < y.uPort))
		{
			return true;
		}
		return false;
	}
};

#endif
