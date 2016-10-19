
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ²éÑ¯·ÑÓÃÀà
// ÏÉ½£ÐÞ£¬2001.11.22
//////////////////////////////////////////////////////////////////////

#ifndef	MSGQUERYFEE_H
#define MSGQUERYFEE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "OnlineTable.h"

class CMsgQueryFee : public CNetMsg
{
public:
    CMsgQueryFee();
    virtual ~CMsgQueryFee();

    BOOL	Create	(OBJID idAccount, unsigned char ucType, DWORD dwData, int nTime);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
//	void			Process		(CGameSocket* pSocket);

//private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;
        OBJID			idAccount;
        DWORD			dwData;
        int				nTime;
        unsigned char	ucType;			// enum { c_typeNone = 0, c_typePoint, ..., c_typeAll };
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // MSGQUERYFEE_H
