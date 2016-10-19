
//**********************************************************
// 代码编辑器
//**********************************************************

// 场外赌博
// 仙剑修，2002.1.7
//////////////////////////////////////////////////////////////////////

#ifndef	BETOUTSIDE_H
#define BETOUTSIDE_H

#include "windows.h"
#include "MultiObjSet.h"
#include "Npc.h"
#include "AutoLinkSet.h"
#include "AutoPtr.h"
#include "TimeOut.h"

class	CUser;

const int	BOSTAT_SECS = 1800;		// 每半小时统计一个庄家赢的钱，并写LOG



class	CBet
{
public:
    enum
    {
        DICE_VALUE_SMALL	= 0,
        DICE_VALUE_BIG		= 1,
        DICE_VALUE_111		= 2,
        DICE_VALUE_222		= 3,
        DICE_VALUE_333		= 4,
        DICE_VALUE_444		= 5,
        DICE_VALUE_555		= 6,
        DICE_VALUE_666		= 7,
        DICE_VALUE_4		= 8,
        DICE_VALUE_5		= 9,
        DICE_VALUE_6		= 10,
        DICE_VALUE_7		= 11,
        DICE_VALUE_8		= 12,
        DICE_VALUE_9		= 13,
        DICE_VALUE_10		= 14,
        DICE_VALUE_11		= 15,
        DICE_VALUE_12		= 16,
        DICE_VALUE_13		= 17,
        DICE_VALUE_14		= 18,
        DICE_VALUE_15		= 19,
        DICE_VALUE_16		= 20,
        DICE_VALUE_17		= 21,
        DICE_VALUE_ALL		= 22,
        DICE_VALUE_END		= 22,
    };

public:
    CBet				(OBJID idUser, OBJID idType, int nMoney) { m_idUser = idUser; m_idType = idType; m_nMoney = nMoney; }
    void	ReleaseByOwner()	{ delete this; }
    OBJID	GetID		() { return m_idUser; }
    OBJID	GetObjType	() { return m_idType; }
    int		GetMoney	() { return m_nMoney; }
    void	SetMoney	(int nMoney) { m_nMoney = nMoney; }

protected:
    OBJID	m_idUser;
    OBJID	m_idType;
    int		m_nMoney;

public: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

class	CNpcBet
{
protected:
    CNpcBet();
    ~CNpcBet();
public:
    static CNpcBet*	CreateNew(PROCESS_ID idProcess, CNpc* pNpc);
    void	Release()	{ delete this; }
public:
    void ProcessNew();
    void ProcessOpen();
    void ProcessRun();
    void BroadcastListMsg(CNetMsg* pMsg, CUser* pExcludeUser);
    bool	JoinBet(CUser* pUser);
    bool	LeaveBet(CUser* pUser);
    bool	ChipIn(CUser* pUser, int nType, int nMoney);
    void	OnTimer();
protected:
    enum	{ c_betNone = 0, c_betWait, c_betRun }	m_nState;
    typedef	CAutoLinkSet<CUser>	PLAYER_SET;
    PLAYER_SET	m_setPlayer;
    typedef	IMultiObjSet<CBet>	IBetSet;
    typedef	CMultiObjSet<CBet>	CBetSet;
    CAutoPtr<IBetSet>	m_setBet;	// 部分玩家参与注意与玩家表同步
    int		m_nCountDown;
    char	m_aDice[4];

protected: // ctrl
    PROCESS_ID		m_idProcess;
    CNpc*			m_pOwner;

    ///////////////////////////////////////////
private:
    //	bool	FlushState(CUser * pUser);
    int		m_nSysGain;		// 用于统计
    int		m_nSysGainAll;
    time_t	m_tGain;
    CTimeOut	m_tClock;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // BETOUTSIDE_H
