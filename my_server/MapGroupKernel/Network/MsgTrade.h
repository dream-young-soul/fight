
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgTrade.h: interface for the CMsgTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_)
#define AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

const unsigned short _TRADE_APPLY			= 1;		// 请求交易		to client/server
const unsigned short _TRADE_QUIT			= 2;		// 取消交易		to server
const unsigned short _TRADE_OPEN			= 3;		// 打开交易窗	to client
const unsigned short _TRADE_SUCCESS			= 4;		// 交易成功(同时关闭)		to client
const unsigned short _TRADE_FALSE			= 5;		// 交易失败(同时关闭)		to client
const unsigned short _TRADE_ADDITEM			= 6;		// 加物品		to client/server(对方收到MSGITEMINFO消息)
const unsigned short _TRADE_ADDMONEY		= 7;		// 加钱			to server
const unsigned short _TRADE_MONEYALL		= 8;		// 对方钱总数	to client(对方收到金钱总数)
const unsigned short _TRADE_SELFMONEYALL	= 9;		// 自己钱总钱	to client(金钱总数)
const unsigned short _TRADE_OK				= 10;		// 同意交易		to client/server
const unsigned short _TRADE_ADDITEMFAIL		= 11;		// 加物品失败	to client

class CMsgTrade : public CNetMsg
{
public:
    CMsgTrade();
    virtual ~CMsgTrade();

public:
    BOOL	Create		(unsigned short usAction, OBJID id);

    BOOL	Create		(char* pbufMsg, DWORD dwMsgSize);
    void	Process		(CGameSocket* pSocket);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        union
        {
            OBJID			id;
            DWORD			nData;
        };
        unsigned short	usAction;
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_)
