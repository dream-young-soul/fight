
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAccount.h: interface for the CMsgAccount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGACCOUNT_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_A)
#define AFX_MSGACCOUNT_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_A

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgAccount_A : public CNetMsg
{
public:
    CMsgAccount_A();
    virtual ~CMsgAccount_A();

    BOOL	Create	(const char* pszAccount, const char* pszPsw, const char* pszServer);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    //	void			Process		(CGameSocket* pSocket);

    //private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        char			szAccount[_MAX_NAMESIZE];
        char			szPassword[_MAX_NAMESIZE];
        char			szServer[_MAX_NAMESIZE];
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGACCOUNT_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_A)
