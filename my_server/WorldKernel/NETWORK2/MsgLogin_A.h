
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgLogin.h: interface for the CMsgLogin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGLOGIN_H__63122645_0BCE_11D5_8D37_0010B13A12DB__INCLUDED_A)
#define AFX_MSGLOGIN_H__63122645_0BCE_11D5_8D37_0010B13A12DB__INCLUDED_A

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgLogin_A : public CNetMsg
{
public:
    CMsgLogin_A();
    virtual ~CMsgLogin_A();

    BOOL	Create(OBJID idAccount, DWORD dwCheckData, char* pszName, USHORT usVersion);

public:
    virtual BOOL			Create	(char* pbufMsg, DWORD dwMsgSize);
    //	virtual void			Process		(void *pInfo);

    virtual unsigned short	GetStdSize	(void) {return sizeof(MSG_Info);}

    //private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        OBJID			idAccount;
        DWORD			dwCheckData;
        unsigned short 	usVersion;
        char			szUserName[_MAX_NAMESIZE];
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGLOGIN_H__63122645_0BCE_11D5_8D37_0010B13A12DB__INCLUDED_A)
