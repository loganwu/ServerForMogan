#ifndef _SESSIONINFO_H_
#define _SESSIONINFO_H_

#pragma pack(1)

struct SessionInfo
{
	uint32_t uSessionId;
	uint64_t ullRoleId;
	uint32_t uiIP;
};

#pragma pack()

#endif