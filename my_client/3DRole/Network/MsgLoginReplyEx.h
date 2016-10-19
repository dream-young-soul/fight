
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgLoginReplyEx.h
// Create by: Huang Yuhang
// Create on: 2004/11/12 15:50
//--------------------------------------------------------------------------------------
#ifndef _MSG_LOGINREPLYEX_H
#define _MSG_LOGINREPLYEX_H
//--------------------------------------------------------------------------------------
#include "NetMsg.h"

class CMsgLoginReplyEx : public CNetMsg
{
public:
    CMsgLoginReplyEx();
    virtual ~CMsgLoginReplyEx();

    BOOL	Create	(OBJID idAccount, DWORD dwData, const char* pszInfo);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;
        OBJID			idAccount;
        DWORD			dwData;
        int				nServePort;
        unsigned  int	dwUin;
        char			szInfo[_MAX_NAMESIZE_CLI];
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif
