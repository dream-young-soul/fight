
//**********************************************************
// 代码编辑器
//**********************************************************

// SynAttr.h: interface for the CSynAttr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNATTR_H__7205A42E_F451_4A87_9BBD_37397D6578E2__INCLUDED_)
#define AFX_SYNATTR_H__7205A42E_F451_4A87_9BBD_37397D6578E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "GameData.h"
#include "GameObj.h"
#include "user.h"
#include "Myheap.h"
#include "AutoLink.h"
#include "Syndicate.h"

#define	DEFAULT_TENET				""
#define	DEFAULT_LEADERTITLE			STR_SYNDICATE_LEADER
#define	DEFAULT_MEMBERTITLE			STR_SYNDICATE_MEMBER
//---jinggy---2004-12-7---define---begin
#define TIMEONLINE_FIRSTJOIN		10*60	//首次加入给在线时间设置10*60分钟
//---jinggy---2004-12-7---define---end
//////////////////////////////////////////////////////////////////////
const int	DEFAULT_PROFFER			= 0;				// 新人的贡献
const int	CHANGELEADER_DAYS		= 30;				// 帮主30天不上线，可能被替换
const int	CHANGELEADER_MONEY		= 100000;			// 替换帮主要捐多少钱。
const int	SYNMONEY_LIMIT			= 2000000000;		// 基金的限制额
const int	SYNPROFFER_LIMIT		= 1000000000;		// 贡献的限制额
const int	MAX_ASSISTANTSIZE		= 5;				// 最多有多少个副帮主，暂定
const int	SYN_MEMBER_LIMIT		= 800;				// 限制最多招收多少个帮众

//---jinggy---2004-12-7---基金---begin
const int	MONEY_PER_MEMBER		= 10000;			// 招人增加的基金
const int	MONEY_PER_MEMBER_OUT	= 30000;		// 主动脱离团队减少的基金
const int	MONEY_KICKOUT			= 20000;			// 开除人员扣除的基金
//---jinggy---2004-12-7---基金---end

//---jinggy---2004-12-7---声望---begin
const int	REPUTE_PER_MEMBER		= 1000;			// 招人增加的声望1000
const int	REPUTE_PER_MEMBER_OUT	= 3000;			// 主动退出减少军团声望3000
const int	REPUTE_KICKOUT			= 1000;			// 开除人员扣除的声望1000
//---jinggy---2004-12-7---声望---end
//---jinggy---贡献度---begin

//---jinggy---贡献度---end

//////////////////////////////////////////////////////////////////////
struct SynAttrInfoStruct
{
    OBJID		id;
    OBJID		idSyn;
    USHORT		usMantle;
    UCHAR		ucRank;
    UCHAR		ucRankShow;
    int 		nProffer;
    int			nProfferInc;
    int			nTitle;
    UCHAR		ucLevel;
};

//////////////////////////////////////////////////////////////////////
enum SYNATTRDATA
{
    SYNATTRDATA_ID = 0,		   // 用于INSERT
    SYNATTRDATA_SYNID = 1,
    SYNATTRDATA_RANK,
    SYNATTRDATA_PROFFER,
    //---jinggy---2004-12-7---begin
    SYNATTRDATA_TIMEONLINE,		//成员的在线时间，若2个月没有达到10个小时则为背叛帮派
    SYNATTRDATA_TIMEDETECTED,	//用来和当前时间进行比较，看是否到了2个月的时间间隔
    SYNATTRDATA_PROFFERINC,		//记录成员从上次帮派发放奖金后得到的Proffer累加值
    SYNATTRDATA_MEMBERLEVEL,	//成员级别
    //---jinggy---2004-12-7---end
    SYNATTRDATA_TITLE,				// 军衔
};
char	szSynAttrTable[];
typedef	CGameData<SYNATTRDATA, szSynAttrTable, szID>		CSynAttrData;

//////////////////////////////////////////////////////////////////////
class CSynAttr
{
protected:
    CSynAttr();
    virtual ~CSynAttr();

public:
    static CSynAttr* CreateNew(PROCESS_ID idProcess, CUser* pUser);
    ULONG	ReleaseByOwner()						{ delete this; return 0; }
    //	void	OnTimer(DWORD tCurr);

public: // login
    bool	Create();
    bool	SendInfoToClient();

public: // MapGroup
    bool	SaveInfo();
    bool	GetInfo(SynAttrInfoStruct* pInfo);
    bool	AppendSynAttr(SynAttrInfoStruct* pInfo);

public: // const
    OBJID	GetSynID()								{ if (m_pData) { return m_pData->GetInt(SYNATTRDATA_SYNID); } return ID_NONE; }
    int		GetInt(SYNATTRDATA idx)					{ CHECKF(m_pData); ASSERT(idx != SYNATTRDATA_SYNID); return m_pData->GetInt(idx); }
    int		GetAssistantCount();
    DWORD	GetSynRankShow		();
    LPCTSTR	GetRankTitle();
    //	int		SumSynMember();
    int		GetProffer(void)	const				{ if (m_pData) { return m_pData->GetInt(SYNATTRDATA_PROFFER); } return 0; }
    //---jinggy---得到贡献值的增量
    int		GetProfferInc(void)	const				{ if (m_pData) { return m_pData->GetInt(SYNATTRDATA_PROFFERINC); } return 0; }

public: // modify
    bool	CreateSyndicate(LPCTSTR szName, int nLevel, int nMoney, int nMoneyLeave);
    //	bool	CreateSubSyn(LPCTSTR szName, int nLevel,int nProffer, int nMoney, int nMoneyLeave);
    OBJID	CreateSubSyn(OBJID idFealty, const char* pszName);
    bool	DestroySyndicate();
    bool	JoinSyn(OBJID idSyn, int nRank = RANK_NEWBIE, int nProffer = 0);
    bool	AddMemberList(OBJID idSyn, LPCTSTR szName, int nLevel);
    bool	LeaveSyn(bool bKickout = false, bool bDeleteRecord = true, bool bSynchro = true);
    bool	DelMemberList(OBJID idOldSyn, LPCTSTR szName);
    bool	DonateMoney(int nMoney);
    bool	SetRank(int nRank, bool bLeader = false);
    bool	ChangeLeader(LPCTSTR szSyn, int nLevel);
    bool	SetIdRank(OBJID idSyn, int nRank, bool bSynchro = true);
    bool	AddProffer(int nData);
    bool	DecProffer(int nData);
    void	UpLevel(int nLevel);			// synchro world kernel member list
    void	SetProffer(int nProffer);


public:
    void	SynchroInfo(BOOL bAnnounce = false);				// 修改后，广播自己的属性
protected:
    bool	ChangeLeader();				// 检查是否换帮主
    void	AddProfferToLeader(int nProffer);

public: // static
    int		GetSynRank();
    OBJID	CreateSubSubSyn(OBJID idFealty, const char  szName[]);
    bool	CreateSubSubSyn(LPCTSTR szName, int nLevel, int nProffer, int nMoney, int nMoneyLeave);
    int		GetSynMemberLevel();
    bool	SetSynMemberLevel(int nLevel);
    void	ResetSynProfferInc();
    void	Betray();
    bool	DetectIsBetrySyn();
    void	IncTimeOnline(unsigned short usTimeInc);

    //---jinggy---2004-12-7---获得帮派成员的连续在线时间
    unsigned int GetTimeOnline();

    CSyndicate* QuerySyndicate();
    bool	ChangeSyn(CSyndicate* pSyn, CSyndicate* pTarget);
    bool	KickoutMember(LPCTSTR szTarget);

protected: // data
    CSynAttrData*	m_pData;			//? 可能为0
    int m_nProfferInc;
protected: // ctrl
    PROCESS_ID		m_idProcess;
    CUser*			m_pUser;
    CAutoLink<CSyndicate>	m_linkSyn;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SYNATTR_H__7205A42E_F451_4A87_9BBD_37397D6578E2__INCLUDED_)
