
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgDice.h: interface for the CMsgDice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGDICE_H__B3F539EB_1871_4A23_A7BD_72211205A127__INCLUDED_)
#define AFX_MSGDICE_H__B3F539EB_1871_4A23_A7BD_72211205A127__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
enum
{
    _ACTION_CHIPIN				= 0,		//下注  to server, to client
    //个人下注通过这个Action广播同步
    //新加入赌局的人也通过这个Action取得投注列表
    //dwData为投了多少钱，ucType为投哪种注
    // exclude self, 自己通过_ACTION_CHIPIN 返回
    _ACTION_CHIPIN_CONFIRM		= 1,		//下注确认// only to client
    _ACTION_CANCELCHIP			= 2,		//取消下注to server, to client
    //dwMoney为投了多少钱，ucType为投哪种注
    // exclude self, 自己通过_ACTION_CHIPIN 返回
    _ACTION_CANCELCHIP_CONFIRM	= 3,		//退出下注确认// only to client
    _ACTION_BEGINCHIP			= 4,		//开始下注 // to client 倒计时30s
    _ACTION_ENDCHIP				= 5,		//买定离手 // to client 倒计时5S
    _ACTION_DICE				= 6,		//骰子消息 // to client 倒计时5S // MsgName广播赢钱消息
};
class CMsgDice : public CNetMsg
{
public:
    CMsgDice();
    virtual ~CMsgDice();
    BOOL	Create	();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
    BOOL			Create		(OBJID idDice, int nAction, DWORD dwMoney, int nType = 0);
private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;
        unsigned char	ucAction;
        union
        {
            unsigned char	ucTime;		// 允许下注开始了多久
            unsigned char	ucAmount;
        };
        OBJID			idDice;
        struct
        {
            unsigned char	ucType;
            union
            {
                DWORD			dwData;
                unsigned char	ucDice[4];
            };
        } Data[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGDICE_H__B3F539EB_1871_4A23_A7BD_72211205A127__INCLUDED_)
