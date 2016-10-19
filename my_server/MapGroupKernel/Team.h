
//**********************************************************
// 代码编辑器
//**********************************************************

// Team.h: interface for the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_)
#define AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include <windows.h>
#include "Myheap.h"

#include <vector>
#include <algorithm>
using namespace std;

#define		_MAX_TEAMAMOUNT			5
const int	_RANGE_EXPSHARE			= 32;
const int	_RANGE_TEAM_STATUS		= 12;			// 结界状态影响的范围半径
const int	MAX_TEAM_EXP_TIMES		= 360;			// 组队经验值的等级最大倍数
//---jinggy---帮派系统---begin
//const int	TEAMLEADER_WITHNEWBIE_REPUTEINC = 100;		//带低级的人练级1小时声望增加100
//---jinggy---帮派系统---end

enum
{
    CHANGETEAM_ADD = 1,
    CHANGETEAM_DEL,
    CHANGETEAM_DISMISS,
    CHANGETEAM_INFO,
    CHANGETEAM_FLY,
};

struct TeamInfoStruct
{
    OBJID	id;
    OBJID	idLeader;
    OBJID	setMember[_MAX_TEAMAMOUNT];
    BOOL	bCloseMoney;
    BOOL	bCloseItem;
};

class CNetMsg;
class IRole;
class CTeam
{
public:
    CTeam(PROCESS_ID idProcess);
    virtual ~CTeam();

public:
    static	CTeam*		CreateNew		(PROCESS_ID idProcess, OBJID m_idTeam, OBJID idLeader);
    static	CTeam*		CreateNew		(PROCESS_ID idProcess, TeamInfoStruct* pInfo);
    ULONG	ReleaseByOwner()	{ delete this; return 0; }
    OBJID	GetID()				{ return m_id; }
    void	GetInfo(TeamInfoStruct* pInfo);
    void	Destroy				();

public:
    BOOL	AddMember			(OBJID idMember);
    BOOL	DelMember			(OBJID idMember);
    void	Dismiss				(OBJID idLeader);

    void	Open				();
    void	Close				();
    BOOL	IsClosed			();
    void	OpenMoney			();
    void	CloseMoney			();
    BOOL	IsCloseMoney		()			{ return m_bCloseMoney; }
    void	SetCloseMoney		(bool flag)	{ m_bCloseMoney = flag; }
    void	OpenItem			();
    void	CloseItem			();
    BOOL	IsCloseItem			()			{ return m_bCloseItem; }
    void	SetCloseItem		(bool flag)	{ m_bCloseItem = flag; }
    void	OpenGemAccess		();
    void	CloseGemAccess		();
    BOOL	IsCloseGemAccess	()			{ return m_bCloseGemAccess; }
    void	SetCloseGemAccess	(bool flag)	{ m_bCloseGemAccess = flag; }

    OBJID	GetMemberByIndex	(int nIndex);
    int		GetMemberAmount		();

    BOOL	IsTeamMember		(OBJID idMember);
    BOOL	IsTeamWithNewbie	(OBJID idKiller, IRole* pTarget);
    BOOL	IsTeamWithTutor		(OBJID idKiller, IRole* pTarget);

    void	SetLeader			(OBJID idLeader);
    OBJID	GetLeader			();

    void	AwardMemberExp		(OBJID idKiller, IRole* pTarget, int nAddExp);

    // 用于结界系统的
    void	AttachMemberStatus	(int nStatus, int nPower, int nSecs, int nTimes, OBJID idExclude);
    void	AdjustMemberStatus	(int nOldAmount, int nNewAmount);
    int		ShareDamage			(int nDamage, OBJID idExclude);

    void	BroadcastTeamMsg	(CNetMsg* pMsg, CUser* pSender = NULL);
    void	BroadcastMemberLife	(CUser* pMember, bool bMaxLife = false);

public: // call by transmit from other mapgroup
    //	void DealTeamLeaderInfo();
    void	AddMemberOnly		(OBJID idMember);
    void	DelMemberOnly		(OBJID idMember);

private:
    BOOL	Create				(OBJID idLeader);
    void	Init				();
    CTeam*	CloneNew();

protected:
    typedef	vector<OBJID>		USERIDSET;
    USERIDSET					m_setIdUser;

private:
    OBJID						m_id;
    OBJID						m_idLeader;
    PROCESS_ID					m_idProcess;
    BOOL						m_fClosed;
    BOOL						m_bCloseMoney;
    BOOL						m_bCloseItem;
    BOOL						m_bCloseGemAccess;

    CTimeOut					m_tGuidingTime;  //带练时间

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_)
