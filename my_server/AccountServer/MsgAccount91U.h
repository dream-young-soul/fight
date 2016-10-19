
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAccount.h: interface for the CMsgAccount91U class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGACCOUNT91U_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_)
#define AFX_MSGACCOUNT91U_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgAccount91U : public CNetMsg
{
public:
    CMsgAccount91U();
    virtual ~CMsgAccount91U();

    BOOL	Create	(OBJID id91U, const char* pszAccount, const char* pszPsw, const char* pszServer, const char* szIPAddr);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
//	void			Process		(CGameSocket* pSocket);

//private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        OBJID			id91U;
        char			szAccount[_MAX_NAMESIZE];
        char			szPassword[_MAX_NAMESIZE];
        char			szServer[_MAX_NAMESIZE];
        char			szIPAddr[_MAX_NAMESIZE];
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGACCOUNT91U_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_)
