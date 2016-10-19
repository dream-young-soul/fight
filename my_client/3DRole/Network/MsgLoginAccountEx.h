
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgLoginAccountEx.h
// Create by: Huang Yuhang
// Create on: 2004/11/12 15:54
//--------------------------------------------------------------------------------------
#ifndef _MSG_LOGINACCOUNTEX_H
#define _MSG_LOGINACCOUNTEX_H

#include "NetMsg.h"

class CMsgLoginAccountEx : public CNetMsg
{
public:
    CMsgLoginAccountEx();
    virtual ~CMsgLoginAccountEx();

    BOOL	Create	(unsigned long uType, const char* pszAccount, const char* pszPsw, const char* pszServer, const char* szInfo);

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;
        unsigned long	unType;
        char			szAccount[_MAX_NAMESIZE_CLI];
        char			szPassword[_MAX_NAMESIZE_CLI];
        char			szServer[_MAX_NAMESIZE_CLI];
        char			szInfo[_MAX_NAMESIZE_CLI];		// 光盘标志
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif
