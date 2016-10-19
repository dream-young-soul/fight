
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgConnect.h: interface for the CMsgConnect91U class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCONNECT91U_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_)
#define AFX_MSGCONNECT91U_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgConnect91U : public CNetMsg
{
public:
    CMsgConnect91U();
    virtual ~CMsgConnect91U();

    BOOL	Create	(OBJID id91U, OBJID idAccount, DWORD dwData, char* pszInfo);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
//	void			Process		(CGameSocket* pSocket);

//protected:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        OBJID			id91U;
        OBJID			idAccount;
        DWORD			dwData;
        char			szInfo[_MAX_NAMESIZE];
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGCONNECT91U_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_)
