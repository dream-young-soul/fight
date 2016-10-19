
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgBetOutside.h: interface for the CMsgAtk class.
// 仙剑修，2002.1.8
//////////////////////////////////////////////////////////////////////

#ifndef	MSGBETOUTSIDE_H
#define MSGBETOUTSIDE_H

#include "NetMsg.h"

enum
{
    _ACTION_CHIPIN				= 0,		//下注  to server, to client
    //个人下注通过这个Action广播同步
    //新加入赌局的人也通过这个Action取得投注列表
    //dwData为投了多少钱，ucType为投哪种注
    // exclude self, 自己通过_ACTION_CHIPIN_CONFIRM 返回
    _ACTION_CHIPIN_CONFIRM		= 1,		//下注确认// only to client
    _ACTION_CANCELCHIP			= 2,		//取消下注to server, to client
    //dwMoney为投了多少钱，ucType为投哪种注
    // exclude self, 自己通过_ACTION_CANCELCHIP_CONFIRM 返回
    _ACTION_CANCELCHIP_CONFIRM	= 3,		//退出下注确认// only to client
    _ACTION_BEGINCHIP			= 4,		//开始下注 // to client 倒计时	// ucTime is leave secs
    _ACTION_ENDCHIP				= 5,		//买定离手 // to client 倒计时5S
    _ACTION_DICE				= 6,		//骰子消息 // to client 倒计时5S // MsgName广播赢钱消息
};

class CMsgBODice : public CNetMsg
{
public:
    CMsgBODice();
    virtual ~CMsgBODice();

    BOOL	Create (int nAction, OBJID idNpc, int nTime = 0);
    BOOL	Append (int nType, DWORD dwData);
    BOOL	Create (int nAction, OBJID idNpc, char aDice[4]);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        unsigned char	ucAction;
        union
        {
            unsigned char	ucTime;		// 允许下注开始了多久 // use for _ACTION_BEGINCHIP
            unsigned char	ucAmount;
        };
        OBJID			idDiceNpc;
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






#endif // MSGBETOUTSIDE_H
