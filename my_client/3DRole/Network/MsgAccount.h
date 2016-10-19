
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAccount.h: interface for the CMsgAccount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGACCOUNT_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_)
#define AFX_MSGACCOUNT_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgAccount : public CNetMsg
{
public:
    CMsgAccount();
    virtual ~CMsgAccount();

    BOOL	Create	(const char* pszAccount, const char* pszPsw, const char* pszServer);

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

private:
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

#endif // !defined(AFX_MSGACCOUNT_H__5D73358C_1E18_4877_BB14_969CC7CBAAF5__INCLUDED_)
