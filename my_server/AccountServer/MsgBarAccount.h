
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Íø°ÉÕÊºÅµÇÂ¼ÏûÏ¢
// ÏÉ½£ÐÞ£¬2002.2.26
//////////////////////////////////////////////////////////////////////

#ifndef MSGBARACCOUNT_H
#define MSGBARACCOUNT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgBarAccount : public CNetMsg
{
public:
    CMsgBarAccount();
    virtual ~CMsgBarAccount();

    BOOL	Create	(LPCTSTR pszAccount, LPCTSTR pszPsw, LPCTSTR pszServer, LPCTSTR pszFeeAccount, LPCTSTR pszFeePsw);

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

        char			szBarAccount[_MAX_NAMESIZE];
        char			szBarPassword[_MAX_NAMESIZE];
        int				nCount;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // MSGBARACCOUNT_H
