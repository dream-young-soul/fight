
//**********************************************************
// 代码编辑器
//**********************************************************

// Hero.h: interface for the CHero class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HERO_H__E1E1F17C_8E9A_439A_B54A_E7A6D3213111__INCLUDED_)
#define AFX_HERO_H__E1E1F17C_8E9A_439A_B54A_E7A6D3213111__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetworkDef.h"
#include "Player.h"
#include "Pet.h"
#include "SchoolMember.h"
#include "Friend.h"
#include "Enemy.h"
#include "Item.h"
#include "AutoAllot.h"
#include "Trade.h"
#include "Package.h"
#include "Bank.h"
#include "XpCounterDown.h"
#include "WeaponSkill.h"
#include "TaskDialog.h"
#include "Team.h"
#include "Professional.h"
#include "Magic.h"
#include "MyTimer.h"
#include "ImageString.h"
#include "Booth.h"
#include "BoothManager.h"
#include "Bulletin.h"
#include "Dummy.h"
#include "DiceManager.h"
#include "Check.h"
#include "ItemPackage.h"
// constant ...
const int _MAGIC_DECDELAY_PER_LEVEL		= 100;
const int _MAGIC_DELAY					= 600;

const int _MAX_FRIEND_AMOUNT	= 50;
const int _MAX_HEROITEM			= 40;
const int _MAX_HEROEGG			= 6;
const int _MAX_HEROPET			= 3;
const int _MAX_HEROSOUL         = 40;
const int _POWERCOST_JUMP		= 10;

const int MaxPhysicalForce		= 100;

// for teacher annouce
enum
{
    _ANNOUNCE_ISSUE = 0, //发布公告to server
    _ANNOUNCE_CHECK, //查看公告to client
    _ANNOUNCE_SLEF_CHECK, //查看自己的公告to client
    _ANNOUNCE_USER_INFO,//to server/client在发布时，客户端要用到的信息。 返回客户端的:等级，//，导师等级 信息结构中char szBuf[1]的内容为：姓名，职业
    _ANNOUNCE_CANCEL,//取消公告 要idAnnounce
};

typedef struct
{
    OBJID idAnnounce; // 公告ID
    char szTitle[32]; // 公告标题
} ST_ANNOUNCE_TITLE;

enum
{
    CHECK_BY_INDEX,
    CHECK_BY_ID,
    CHECK_BY_OWNER,
};

typedef deque<ST_ANNOUNCE_TITLE*> DEQUE_ANNOUNCE;

typedef struct
{
    int				usType;
    OBJID			idAnnounce;
    OBJID			user_id;
    int				level;// 发布公告人的等级
    int				teacher_level;// 导师等级
    char			szBuf[1];//to client 姓名，职业，公告标题，公告内容
} ST_ANNOUNCE_INFO;

typedef struct
{
    int				usType;
    OBJID			idAnnounce;
    OBJID			user_id;
    int				level;
    int				teacher_level;// 导师等级
    int				profession;
    string			szName;
    string			szTitle; //公告标题
    string			szDesc;//公告内容
} AnnounceInfo;

// player task
enum
{
    _WANT_PK		= 0,	// 悬赏PK
    _WANT_ITEM		= 1,	// 悬赏物品
    _WANT_EUDEMON	= 2,	// 悬赏幻兽
};

enum
{
    RELATION_TEACHER = 1, // 导师
    RELATION_STUDENT,	// 徒弟
    RELATION_STUDENTOFSTUDENT, // 徒弟的徒弟(徒孙)
};

const int _MAX_PRIZE = 5;

enum
{
    TASKLIST_NONE = 0,
    TASKLIST_UNACCEPTED, // 未接任务
    TASKLIST_ACCEPTED, // 已接任务
    TASKLIST_COMPLETED, // 已完成任务
    TASKLIST_MYTASK, // 我的任务
};

typedef struct
{
    OBJID idTask; // 任务ID
    UCHAR ucReqLev; // 需要佣兵等级
    char szTitle[32]; // 任务标题
} ST_TASK_TITLE;

typedef deque<ST_TASK_TITLE*> DEQUE_PLAYERTASK;

typedef struct
{
    union
    {
        OBJID idItem; // 物品ID // to server的时候
        struct   // to client的时候
        {
            OBJID idItemType; // 物品类型
            USHORT usDurability; // 耐久
            UCHAR ucAddition; // 追加 // 幻兽成长
            UCHAR ucDegree; // 等级 // 幻兽等级
        };
    };
} ST_PLAYERTASK_ITEM;


typedef struct
{
    OBJID idTask;
    // 如果是物品 需要物品的附加属性
    DWORD dwDurability;	// 耐久
    UCHAR ucAddition;	// 追加 // 幻兽成长
    UCHAR ucDegree;		// 等级 // 幻兽等级

    DWORD	dwMoney;
    UCHAR	ucType;		// 悬赏类型
    UCHAR	ucRankReq; // 需要的佣兵等级

    ST_PLAYERTASK_ITEM infoItem[_MAX_PRIZE];
    string strName;
    string strDesc;
    string strWant;
} TaskInfo;

//是否自动分配点数
enum {_NOT_AUTOALLOT = 0, _AUTOALLOT = 1};
enum {_DEL_PT = false, _DEL_OBJ = true};

enum {PKMODE_FREE = 0, PKMODE_MONSTERONLY, PKMODE_TMEXCLUDE, PKMODE_ARRESTMENT, PKMODE_SYNDICATE};
// 自由PK 安全PK 组队 捕杀 骑士团

enum {REJECT_FRIEND = 0, REJECT_TRADE = 1, REJECT_TEAM = 2, REJECT_TEACHER = 3 , REJECT_STUDENT = 4};
enum {PACKAGE_BAG = 0, PACKAGE_PET = 1, PACKAGE_EGG = 2, PACKAGE_SOUL = 3};

#include <deque>
typedef deque<char*> DEQUE_STRING;

#include <string>
typedef struct
{
    vector<string> m_setStr;
    int			   nHelpIndex;
    bool	       bActived;
} UserHelpTip;

typedef struct
{
    UCHAR   ucLevel; // 等级
    BOOL	bOnLine;
    DWORD   dwSynID;
    int		nProffer; // 贡献度
    UCHAR	ucRank; // 职位
    UCHAR	ucProfession; // 职业
    UCHAR	ucNobilityRank; // 爵位
    SHORT	sPk; // PK值
    DWORD	dwLookFace; // FACE
    char	szName[_MAX_NAMESIZE];
    char	szRank[_MAX_NAMESIZE];
    char szMate[_MAX_NAMESIZE]; // 配偶名字
} SynMemberInfo;
typedef deque<SynMemberInfo*> DEQUE_SYNMEMBER;

typedef struct
{
    OBJID		idSyn;
    OBJID		idFealty;
    DWORD		dwSyndicateFund;			// 帮派基金
    DWORD		dwSyndicatePopulation;		// 帮派人口
    UCHAR		ucRank;						// 等级
    UCHAR		ucSaint;
    char		szName[_MAX_NAMESIZE];		//
    char		szLeader[_MAX_NAMESIZE];	// 帮主
} SyndicateInfo;

class CHero : public CPlayer
{
public:
    CHero();
    virtual ~CHero();

    void	Init	(void);
    void	Reset	(void);

public:
    void	Walk			(int nPosX, int nPosY);
    void	Run				(int nPosX, int nPosY);
    void	Jump			(int nPosX, int nPosY);
    void	Direction		(int nPosX, int nPosY);

    void	InTone			(DWORD dwMagicTYpe, CPlayer* pTarget = NULL);
    void	TeamFollow		();

    void	Emotion			(int nEmotionType, CPlayer* pTarget = NULL);
    void	Stop			(void);

    void	Poison			(CPlayer* pTarget, OBJID idItem);
    void	Dart			(CPlayer* pTarget, OBJID idItem);
    void	Steal			(CPlayer* pTarget);

    void	DeleteRole		(void);

    void	SetAlignPos(int nPosX, int nPosY);
    CMyPos	GetAlignPos(void)	{return m_posAlign;}

    // override
    DWORD	SetCommand		(CCommand* pCommand);
    BOOL	ProcessCommand	();

    OBJID	m_idAccount;

public:
    BOOL	ProcessChgMap	(void);	// return true means to chg

    void	Process(void* pInfo);
    void	Show	(void* pInfo);
    void	ShowXp	(int talk_width);

public:
    void	Talk			(CPlayer* pTarget, const char* pszWords, const char* pszEmotion = NULL, DWORD dwColor = 0xffffff, unsigned short unAttribute = _TXTATR_NORMAL, unsigned short unStyle = _TXT_NORMAL);
    void	Talk			(const char* pszTarget, const char* pszWords, const char* pszEmotion = NULL, DWORD dwColor = 0xffffff, unsigned short unAttribute = _TXTATR_NORMAL, unsigned short unStyle = _TXT_NORMAL);

private:
    DWORD	m_dwLastTalk;

    // mine
public:
    void	Mine	(void);
    BOOL	IsMiner	(void);

    // item improve
public:
    void	ImproveItem(OBJID idItem, OBJID idTreasure);

    void	EmbedGem	(OBJID idItem, OBJID idGem, int nPos);
    void	TakeOutGem	(OBJID idItem, int nPos);

    BOOL	SplitItem	(OBJID idItem, int nAmount);
    BOOL	CombinItem	(OBJID idItemSource, OBJID idItemTarget);

    // point allot
public:
    void	AllotPoint(int nForce, int nSpeed, int nHealth, int nSoul);
    void	DoLevelUp();
    DWORD	GetNextLevelExp();

private:
    CMyPos	m_posAlign;
    DWORD	m_dwMoney;

    DWORD	m_dwAutoAllot;
    DWORD	m_dwKoXpAdd;

    DWORD	m_dwLifeForeCast;
    DWORD	m_dwLastLifeSync;
    OBJID	m_idTaskDialog;
    BOOL	m_bShowHpBar;
    //CAutoAllot m_objAllot;
public:
    void	OpenHpBar() {m_bShowHpBar = true;}
    void	CloseHpBar() {m_bShowHpBar = false;}
    BOOL	IsShowLife() {return m_bShowHpBar;}
    // ko Order
public:
    DWORD	GetKoXpAdd() {return m_dwKoXpAdd;}
    void	SetKoXpAdd(DWORD dwKoXpAdd) {m_dwKoXpAdd = dwKoXpAdd;}
    // normal attack
public:
    void	Attack			(CPlayer* pTarget, BOOL bForced = false);
    void	Shoot			(CPlayer* pTarget, OBJID idBullet, BOOL bForced = false);

    void	LockAttack		(CPlayer* pTarget, BOOL bForced = false, BOOL bMove = true);
    void	RushAttack		(CPlayer* pTarget, BOOL bForced = false);
    void	SkillAttack		(CPlayer* pTarget, OBJID idSkillType, BOOL bForced = false);

    void	SetLifeForeCast	(DWORD dwLife) {m_dwLifeForeCast = dwLife; m_dwLastLifeSync = ::TimeGet();}
    DWORD	GetLifeForeCast	() {return m_dwLifeForeCast;}
    void	ProcessLifeForeCast();
    void	SetData(DWORD dwDataType, DWORD dwData);

    BOOL	m_bFirstAtk;

    // xp
public:
    enum {XPFULL_TIME = 60000, KILLAWARD_XPTIME = 0};

    void	BurstXp();
    void	ResetXpFullTime(DWORD dwFullTime);
    void	ProcessXp	(void);

    DWORD	m_dwXpFullTime;
    DWORD   m_dwXpStartTime;

    void	AddKoCount	();	//杀怪数量
    DWORD	m_dwKoCount;
    DWORD	m_dwKoScale;

    /*/ super soldier
    public:
    	enum {TICKS_SPSOLDIER = 20, INTERVAL_SPSOLDIER = 1000, TICKS_SPSOLDIER_INIT = 20, KILL_AWARD_SPSOLDIER = 800, KILL_AWARD_TORNADO = 1000};

    	DWORD	m_dwSuperSoldierBegin;
    	DWORD	m_dwSuperSoldierBeginCopy;
    */


    // reborn
public:
    void	ProcessReborn	(void);
    DWORD	m_timeDie, m_timeGhost;

    //for userhelpinfo
public:
    bool    m_bCanSetHelpTips;
    bool	IsFlyHelpShow();
    void	ResetFlyHelp();
    void    ResetFlyHelpIndex(int nIndex) {m_nResetIndex = nIndex;}
    int		GetHelpTipAmount(int nIndex);
    const char* GetHelpTipLineByIndex(int nIndex, int nLine);

    int		GetFlyHelpAmount();
    void	SetHelpTip(OBJID IdItemType = 0, OBJID IdMagic	= 0, int nMagicLevel = 0, BOOL bLevelUp = false);
    void	ShowHelpTips(int posShowX, int posShowY, int nWidth = 420, DWORD dwColor = 0xffffff, DWORD dwBkColor = 0x1E000080);

    void	ActivedHelpTips(int nAmount);
    void	ClearHelpTip();

    void	SetTips();
    const char*	GetTipsByLine(int nLine);
    int		GetTipsAmount();

    void	ClearTips();
protected:
    int	  m_nLoopTime;
    bool  m_bShowFlyHelp;
    DWORD m_dwLoginTime;
    DWORD m_dwStayTime;
    DWORD m_dwStartTime;
    BOOL  m_bSetHelpTip;
    int	  m_nShowIndex;
    int   m_nResetIndex;

    vector<UserHelpTip*> m_setHelpTip;
    vector<string> m_setTips;

    bool  CheckHelpTip(int nIndex);
    const char* GetFlyHelpByIndex(int nIndex);
    // for syndicate
private:
    DWORD	m_dwTimeSynWarReportLastFlash;
    vector<string> m_setSynKickDoc;
public:
    OBJID    IsSubSynLeader  ();
    OBJID    IsSubSubSynLeader();
    void	SetSynName(int naction, char* szName);
    void	MoveSynMember(char* szName, OBJID idTarget);
    void	QuerySynKickDoc();
    int		GetSynKickDocAmount();
    void	AddSynKickDoc(char* pszName);
    void	TakeInSynKickDoc(char* pszName);
    void	AgreeSynKickDoc(int nIndex, BOOL bAgree);
    const char*   GetSynKickDocByIndex(int nIndex);
    void	ClearSynKickDoc() {m_setSynKickDoc.clear();}
    void	JoinSynMember	(CPlayer* pTarget);
    void	AcceptSynMember	(CPlayer* pTarget);
    void	TitleSynMember	(CPlayer* pTarget, const char* pszTitle);
    void	KickoutSynMember(CPlayer* pTarget);
    void	KickoutSynMember(char* pszTargetName);
    void	QuitSyn			(void);
    void	SynContribute	(DWORD dwMoney);
    int		GetSynWarReportAmount ();
    char*	GetSynWarReportLine(int nIndex);
    void	ClearSynWarReport();
    void	AddSynWarReport(char* pszLine);
    void	ShowSynWarReport();
    void	SetSynLeader(int nAction, char* pszTargetName);
    void	UniteSubSyn(OBJID idSource, OBJID idTarget);
    void	UniteSyn(OBJID idSource, OBJID idTarget);
    void	QuerySynMember(char* pszName);
    void	AddAllySyn(OBJID idTarget);
    void	ClearAllySyn(char* pszName);
    void	AddAntagonizeSyn(char* pszName);
    void	ClearAntagonizeSyn(char* pszName);
private:
    DEQUE_STRING m_dequeSynWarReport;

    // for marriage
public:
    void	Court			(CPlayer* pTarget);
    void	Marriage		(CPlayer* pTarget);
    void	Divorce			(CPlayer* pTarget);

    // for items
public:
    IItemPackage*	QueryPackageBag() {return m_pPackageNormal;}
    IItemPackage*	QueryPackageEgg() {return m_pPackageEgg;}
    IItemPackage*	QueryPackageSoul() {return m_pPackageSoul;}
    IItemPackage*	QueryPackagePet() {return m_pPackagePet;}



    CItem*	GetItem			(OBJID id, BOOL bExcludeEquipment = true);
    BOOL	AddItem			(CItem* pItem);

    int     GetGhostGemAmount	(int nSoulType);
    int     GetPackageLimit (int nPackageType = PACKAGE_BAG);
    int		GetItemAmount	(int nPackageType = PACKAGE_BAG);
    CItem*	GetItemByIndex	(UINT uIndex, int nPackageType = PACKAGE_BAG);
    CItem*	SearchItem		(OBJID id);
    BOOL	IsPackageFull   (CItem* pItem);

    BOOL	DelItem			(OBJID id, BOOL bDelete = _DEL_PT);
    void	ClearItem		(void);

    // interface
    BOOL	TryItem			(CItem* pItem);
    BOOL	TryItemTarget   (CItem* pItem, OBJID idTarget);
    void	UseItem			(OBJID id, OBJID idTarget = ID_NONE);
    void	RepairItem		(OBJID id);
    void	SellItem		(OBJID id);
    void	BuyItem			(OBJID idType);
    BOOL	DropItem		(OBJID id, int x, int y);
    void	PickUpItem		(OBJID id, int x, int y);

    DWORD	GetMoney() {return m_dwMoney;}
    void	SetMoney(DWORD dwMoney) {m_dwMoney = dwMoney;}

    DWORD	GetCarryingCapacity();
    DWORD	GetItemTotalWeight();

private:
    //DEQUE_ITEM	m_dequeItem;
    IItemPackage*	m_pPackageNormal;	//	普通袋子
    IItemPackage*	m_pPackagePet;		//	宠物袋子
    IItemPackage*	m_pPackageSoul;		//	魔魂袋子
    IItemPackage*	m_pPackageEgg;		//	蛋蛋袋子

    // equipment
public:
    CItem*	GetEquipment(int nPosition)	{ MYASSERT(nPosition - 1 >= 0 && nPosition - 1 < _MAX_EQUIPMENT); return m_pEquipment[nPosition-1]; }
    void	SetEquipment(int nPosition, CItem* pItem);
    BOOL	CanEquip	(int nPosition, CItem* pItem) const;

    BOOL	EquipItem	(int nPosition, OBJID idItem);
    BOOL	UnequipItem	(int nPosition);

    CItem*	SearchEquipment	(OBJID idItem);

private:
    CItem*	m_pEquipment[_MAX_EQUIPMENT];

    // own info
public:
    DWORD	GetAdditionAtk();
    DWORD	GetAdditionDef();
    DWORD	GetAdditionMAtk();
    DWORD	GetAdditionMDef();
    DWORD	GetMaxAtk();
    DWORD	GetMinAtk();
    DWORD	GetMaxMagicAtk();
    DWORD	GetMinMagicAtk();

    DWORD	GetDef();
    DWORD	GetMagicDef();

    DWORD	GetDex();

    DWORD	GetMaxLife();
    DWORD	GetMaxMana();
    DWORD	GetMaxXP();

    void	SetAutoAllot(DWORD dwAutoAllot) {m_dwAutoAllot = dwAutoAllot;}
    DWORD	GetAutoAllot(void) {return m_dwAutoAllot;}

    DWORD	m_dwNobility;
    DWORD   m_dwMetempsychosis;

    // override
    int		GetAtkRange	(void);

    // frineds
private:
    DEQUE_FRIEND		m_setFriend;
    OBJID				m_idFriendQuery;
    DWORD				m_dwFriendBeginQuery;
    BOOL				m_bFriendQueryBusy;
    BOOL				m_bFriendQueryReturn;
    OBJID				m_idFriendLastApply;
    char				m_szFriendLastApplyInfo[256];
    char				m_szFriendLastAcceptInfo[256];
    char				m_szNameFriendLastAccept[16];
public:
    BOOL	AddFriend		(CFriend* pFriend);
    BOOL	DelFriend		(OBJID id);
    void	QueryFriend	(OBJID id);
    void	ProcessFriend();
    void	QueryFriendReturn() {m_bFriendQueryReturn = true;}
    OBJID	GetCurrentQueryFriendID() {return m_idFriendQuery;}
    void ClearFriendList();

    BOOL MakeFriend		(OBJID id);
    BOOL BreakOffFriend	(OBJID id);

    int  GetFriendAmount();
    CFriend* GetFriend	(OBJID id);
    CFriend* GetFriendByIndex	(int nIndex);
    CFriend* GetFriendByName	(const char* pszName);
    void LeaveWords2Friend (OBJID idFriend, char* szWords);

    OBJID	GetIdFriendLastApply();
    void	SetIdFriendLastApply(OBJID id);
    const	char*	GetNameFriendLastApply() {return m_szNameFriendLastAccept;}
    void	SetNameFriendLastApply(char* pszName)
    {if (pszName && strlen(pszName) < _MAX_NAMESIZE) { strcpy(m_szNameFriendLastAccept, pszName); }}

    const   char*	GetFriendLastApplyInfo();
    void	SetFriendLastApplyInfo(const char* pszInfo);
    const	char*	GetFriendLastAcceptInfo();
    void	SetFriendLastAcceptInfo(const char* pszInfo);


    // enemies
private:
    DEQUE_ENEMY			m_setEnemy;
    OBJID				m_idEnemyQuery;
    DWORD				m_dwEnemyBeginQuery;
    BOOL				m_bEnemyQueryBusy;
    BOOL				m_bEnemyQueryReturn;
public:
    BOOL	AddEnemy		(CEnemy* pEnemy);
    BOOL	DelEnemy		(OBJID id, BOOL bSendMsg = true);
    void	QueryEnemy		(OBJID id);
    void	ProcessEnemy	();
    void	QueryEnemyReturn() {m_bEnemyQueryReturn = true;}
    OBJID	GetCurrentQueryEnemyID() {return m_idEnemyQuery;}
    void	ClearEnemyList();

    int			GetEnemyAmount();
    CEnemy*	GetEnemy(OBJID id);
    CEnemy*	GetEnemyByIndex	(int nIndex);
    CEnemy*	GetEnemyByName	(const char* pszName);

    // trade
public:
    // 交易的开与关
    void	CloseTrade(BOOL	bSuc);
    void	OpenTrade(OBJID idPlayer);
    BOOL	IsTradeActive();

    // 取交易栏需要信息
    DWORD	GetHeroTradeMoney();
    DWORD	GetPlayerTradeMoney();
    CItem*	GetHeroTradeItemByIndex(int nIndex);
    CItem*	GetPlayerTradeItemByIndex(int nIndex);
    int		GetHeroTradeItemAmount();
    int		GetPlayerTradeItemAmount();
    char*	GetPlayerTradeName();
    char*	GetApplyTradeName();

    // 网络消息中使用
    void	SetPlayerTradeMoney(DWORD dwMoney);
    void	SetHeroTradeMoney(DWORD dwMoney);
    void	AddHeroTradeMoney(DWORD dwMoney);
    void	AddHeroTradeItem(CItem* pItem);
    void	AddPlayerTradeItem(CItem* pItem);
    void	TradePlayerAgree();
    void	AddApply(OBJID idPlayer);
    void	AddItemFalse(OBJID idItem);

    // 申请，接受，同意，退出
    void	ApplyTrade(CPlayer* pPlayer);
    void	AcceptTrade();
    void	AgreeTrade();
    void	QuitTrade();

    // auction 拍卖
    void	AuctionBid(OBJID idNpc, OBJID idItem, int dwValue);	//出价格
    void	AuctionAddItem(OBJID idNpc, OBJID idItem, int dwValue); //加入拍卖物品
    void	AuctionItemInfo(OBJID idNpc, OBJID idItem); //查询当前拍卖物品信息
    CItem*	GetAuctionItem(); //获得当前拍卖物品
    void	SetAuctionItemId(OBJID idItem) { m_AuctionItemId = idItem;}
    OBJID   GetAuctionItemId() {return m_AuctionItemId;}
    void    SetAuctionItemMoney(int nMoney) {m_AuctionItemMoney = nMoney;}
    int     GetAuctionItemMoney() {return m_AuctionItemMoney;}
    void	SetAuctionOwnerName(char* szName) { strcpy(m_AuctionOwnerName, szName);}
    char*	GetAuctionOwnerName() {return m_AuctionOwnerName;}
    void	SetAuctionName(char* szName) { strcpy(m_AuctionName, szName);}
    char*   GetAuctionName() { return m_AuctionName;}
    void	SetAuctionItem(CItem* pItem);
    void	DelAuctionItem();
private:
    CItem*  m_AuctionItem;	//拍卖物品
    OBJID   m_AuctionItemId;
    int		m_AuctionItemMoney; //拍卖价格
    char    m_AuctionName[_MAX_NAMESIZE]; //加价人
    char	m_AuctionOwnerName[_MAX_NAMESIZE]; //物品主人
    // package
public:
    void	OpenPackage(OBJID idPackage, int nType);
    void	ClosePackage();
    int		GetPackageItemAmount();
    CItem*	GetPackageItemByIndex(int nIndex);
    void	AddPackageItem(CItem* pItem);
    int		GetPackageMaxItemAmount();
    void	QueryPackage();
    void	CheckInItem(OBJID idItem);
    void	CheckOutItem(OBJID idItem);
    int		GetPackageType();
    void	ResetPackage();
    void	DelPackageItem(OBJID idItem);

    // bank
public:
    void	DrawMoney		(DWORD dwMoney);
    void	SaveMoney		(DWORD dwMoney);
    DWORD	GetSavedMoney	();
    void	SetSavedMoney	(DWORD dwMoney);
    void	QueryMoney		();
    void	OpenBank		(OBJID idBank);
    void	CloseBank		();
    BOOL	DropMoney		(DWORD dwMoney, int x, int y);

    // task dialog
public:
    void			DestroyTaskDialog();
    int				GetTaskDialogLineAmount();
    TaskDialogLine*	GetTaskDialogLineByIndex(int nIndex);
    void			AddTaskDialogLine(TaskDialogLine* pLine);
    void			AddTaskDialogLine(int nType, DWORD dwData, int nIndex, char* szStr, CMyPos posLine);
    void			AnswerTaskDialog(int nIndex, char* pszEdit);
    void			CreateTaskDialog();

    void			ActiveNpc(OBJID idNpc);
    void			ActiveAction(OBJID idAction, char* pszStr = NULL);
    // for npc dialog open
private:
    OBJID	m_idDialogPara;

public:
    void	SetLastDialogPara(OBJID idDialog) {m_idDialogPara = idDialog;}
    OBJID	GetLastDialogPara() {return m_idDialogPara;}
    // status
public:
    virtual void	ReplaceStatus	(__int64 dwStatus);

private:
    CMyTimer		m_tmNameAlert;

    // pk mode
private:
    int		m_nPkMode;

public:
    void	SetPkMode(int nPkMode, BOOL bSubmit = true);
    int	GetPkMode() {return m_nPkMode;}
private:
    BOOL	CanPk	(CPlayer* pTarget);

private:
    CProfessional		m_objProfessional;
    CTeam				m_objTeam;
    OBJID				m_dwFlashTeamMemberID;
    DWORD				m_dwLastQueryTeamLeader;
    DWORD				m_dwLastQueryTeam;

    CTrade				m_objTrade;
    CPackage			m_objPackage;
    CBank				m_objBank;
    CTaskDialog			m_objTaskDialog;
    DEQUE_WEAPONSKILL	m_setSingleHandWeaponSkill;
    DEQUE_WEAPONSKILL	m_setDoubleHandWeaponSkill;
    DEQUE_WEAPONSKILL	m_setOtherWeaponSkill;
    DWORD				m_setLevelExp[_MAX_LEVEL*2];

    VECTOR_MAGIC		m_setMagic;

    DWORD				m_dwMagicAttackBegin;
    DWORD				m_dwMagicAttackBusy;
    BOOL				m_bMagicAttackBusy;
private:
    void			DestroyWeaponSkill();
    CWeaponSkill*	GetWeaponSkillByType(DEQUE_WEAPONSKILL& setSkill, int nType);
    CWeaponSkill*	GetWeaponSkill(DEQUE_WEAPONSKILL& setSkill, int nIndex);
    void			AddNewWeaponSkill(DEQUE_WEAPONSKILL& setSkill, int nSkillType, DWORD dwLevel, DWORD dwExp);

    void			CreateLevelExp();
public:
    CWeaponSkill*	GetWeaponSkill(int nWeaponSkillClass, int nIndex);
    CWeaponSkill*	GetWeaponSkill(int nWeaponSkillType);
    int				GetWeaponSkillAmount(int nWeaponSkillClass);
    void			AddNewWeaponSkill(int nSkillType, DWORD dwLevel, DWORD dwExp);

public:
    // teammember operation
    void			TeamCreate();
    void			TeamApplyJion(OBJID idTarget);
    void			TeamLeave();
    void			TeamAcceptInvite();
    void			SetTeamLeaderPos(CMyPos pos)	{ m_objTeam.m_posLeader = pos, m_dwLastQueryTeamLeader = ::TimeGet();}
    DWORD			GetTeamLeaderFlashTime()		{ return m_dwLastQueryTeamLeader;}
    CMyPos			GetTeamLeaderPos(void)			{ return m_objTeam.m_posLeader; }
    void			QueryTeamMemberPos(OBJID idTarget);
    OBJID			GetTeamMemberFlashID();
    void			SetTeamMoneyAccess(BOOL bAccess);
    void			SetTeamItemAccess(BOOL bAccess);
    void			SetTeamGemAccess(BOOL bAccess);
    BOOL			IsTeamClosed() {return m_objTeam.IsTeamClosed();}


    // teamleader operation
    void			TeamInvite(OBJID idTarget);
    void			TeamKickOut(OBJID idTarget);
    void			TeamDismiss();
    void			TeamClose();
    void			TeamOpen();
    void			TeamAgreeApply();
    char*			GetTeamApplyerName();

    // system operation
    void			ClearTeamMember();
    void			SetTeamMemberHp(OBJID idMember, int nHp);
    void			SetTeamMemberMaxHp(OBJID idMember, int nMaxHp);
    int				GetTeamMemberAmount();
    TeamMemberInfo* GetTeamMemberInfo(int nIndex);
    TeamMemberInfo* GetTeamMemberInfoByID(OBJID idMember);
    BOOL			SetTeamLastApply(OBJID idPlayer);
    void			AddTeamMember(TeamMemberInfo* pInfo);
    void			DelTeamMember(OBJID id);
    void			CloseTeam();
    void			OpenTeam();
    BOOL			IsTeamMember(OBJID idPlayer);
    BOOL			IsTeamMember(char* pszPlayer);

    BOOL			IsTeamLeader();
    void			ShowTeam(CMyPos posShow);
    void			SetTeamMemberFace(OBJID idMember, int nFace);

    // profession
public:
    char*			GetProfessionalName(DWORD dwIndex);

    // jump?
private:
    int			 m_nHeightCanJump;
    BOOL		 CanJump(CMyPos posTarGet);
    DWORD		 m_dwBeginUseTransformMagicTime;

    DWORD		 m_dwFlyBegin;
    DWORD		 m_dwFlyDuration;
    void		 ShowFly();

    DWORD		 m_dwTransformBegin;
    DWORD		 m_dwTransformDuration;
    BOOL		 m_bShowAbortTransformDialog;
    void		 ShowTransform();

public:
    void		 BeginFly(DWORD dwMagicType, DWORD dwLevel);
    BOOL		 IsTransform();
    void		 BeginTransForm(DWORD dwMagicType, DWORD dwLevel);
    void		 ProcessAbortTransformDialog();
    void		 AbortTransform();
public:
    void		DestroyMagicSet();
    void		AddMagic(OBJID idMagic, DWORD dwLevel, DWORD dwExp);
    void		StartMagicBusy(DWORD dwDuration);
    void		EndMagicBusy();
    void		ProcessMagicBusy();
    int			GetMagicAmount();
    int			GetTeamMagicAmount();
    CMagic*		GetTeamMagicByIndex(int nIndex);
    CMagic*		GetMagicByIndex(int nIndex);
    CMagic*		GetMagic(OBJID idType);

    BOOL		IsXpSkillEnable() {return m_bXpSkillEnable;}
    int			GetXpSkillAmount();
    CMagic*		GetXpSkillByIndex(int nIndex);

    int			GetKongfuAmount();
    CMagic*		GetKongfuByIndex(int nIndex);

    void		MagicAttack(OBJID idMagicType, OBJID idPlayer, BOOL bForce = false);
    void		MagicAttack(OBJID idMagicType, CMyPos posTarget);
    DWORD		GetMagicDelay() {return _MAGIC_DELAY - _MAGIC_DECDELAY_PER_LEVEL * this->GetProfessionLevel();}
    void		AbortIntone();
    void		ProcessMagic();
private:
    BOOL        m_bXpSkillEnable;
    DWORD		m_dwLastBump;

private:
    OBJID		m_idLastSyndicateApply;
    OBJID		m_idLastSyndicate;
    char		m_szSyndicateAnnounce[_MAX_STRING];
    char		m_szSyndicateAnnounceTime[64];
    char		m_szActionMessageBox[_MAX_STRING];
    int			m_nSynListIndex;
    DEQUE_SYNMEMBER m_setSyndicateMember;
    SynMemberInfo m_objSynMemberInfo;

    SyndicateInfo m_QuerySynInfo;

    OBJID		m_idLastSynAlly;
    char		m_szLastSynAllyName[_MAX_STRING];

public:
    SynMemberInfo* GetReturnSynMemberInfo() {return &m_objSynMemberInfo;}

    void		QueryOtherSynicateInfo(char* szSynName);
    SyndicateInfo* GetReturnSyndicateInfo() {return &m_QuerySynInfo;}

    void		QuerySynicateInfo();
    void		SetSynListIndex(int nIndex) {m_nSynListIndex = nIndex;}
    void		SetSyndicateRank(DWORD dwRank);
    void		SetLastSyndicateApplyID(OBJID idApply);
    void		SetLastSyndicateID(OBJID idSynDicate);
    OBJID		GetLastSyndicateID();
    OBJID		GetLastSyndicateApplyID()	{ return m_idLastSyndicateApply; }
    void		SendSyndicateAnnounce(char* pszStr);
    void		SyndicateApplyJoin(OBJID idTarget);
    void		SynDicateAcceptJoin();
    void		SyndicateApplyInvite(OBJID idTarget);
    void		SyndicateAcceptInvite();
    void		LeaveSyndicate();
    void		SetSyndicateAnnounce(char* pszWord);
    char*		GetSyndicateAnnounce();
    void		SetSyndicateAnnounceTime(DWORD dwTime);
    char*       GetSyndicateAnnounceTime();

    void		QuerySpecifSynMember(OBJID idSyn, int nIndex);

    void		RenameSynName(char* szNowName);
    void		QuerySynMemberList(int nIndex);
    int			GetSynMemberAmount();
    SynMemberInfo*	GetSynMemberByIndex(int nIndex);
    void		ClearSynMember();
    void		AddSynMember(char* pszMember);
    BOOL		IsAllEquipmentNonsuch();
    DWORD		GetBowShootRange();
    void		SetActionMessage(const char* pszStr);
    char*		GetActionMessage();

    void		SelectMedal(DWORD dwMedalSelect);
    void		SelectHonorTitle(DWORD dwSelectedTitle);

    OBJID		GetLastSynAlly() {return m_idLastSynAlly;}
    void		SetLastSynAlly(OBJID idSyn) {m_idLastSynAlly = idSyn;}	//发出请求的player id

    const char*	GetLastSynAllyName() {return m_szLastSynAllyName;}
    void		SetLastSynAllyName(const char* pszName)
    {if (pszName && strlen(pszName) < _MAX_NAMESIZE) { strcpy(m_szLastSynAllyName, pszName); }}
    //ghost and so on ...
public:
    void Relife();
    void Body2Ghost();
    void ReBorn();
    BOOL IsRebornEnable()	{ return (this->IsGhost() && (::TimeGet() - m_timeGhost) > 20000); }

    // dummy and so on ...
public:
    CDummy  m_objDummy;
    CDummy  m_objPlayerDummy;

public:
    void CreateStatuty();
    // booth and so on ...
public:
    CBooth m_objBooth;
    CBoothManager m_objBoothManager;

    // bulletin and so on ...
public:
    CBulletin m_objBulletin;
    void LockBooth(CPlayer* pTarget);
    BOOL CanBooth(CPlayer* pTarget);

    //changeface
public:
    void ChangeFace(int nFace);

    // mouse player and so on ...
private:
    BOOL m_bMousePlayerFixed;
    BOOL n_bMousePlayerTerrain;
public:
    void ProcessMousePlayer();
    void FixMousePlayer();
    void CreateMousePlayer(int nLookType, BOOL bTerrain = false);
    void SetMousePlayerDir(int nDir);
    void PutMousePlayer();
    void DelMousePlayer();
    // void DelNpc
public:
    void DelNpc(OBJID idNpc);

    // wanted
public:
    char m_szWanted[_MAX_NAMESIZE];
    char m_szPoliceWanted[_MAX_NAMESIZE];
    // dice
public:
    DiceManager m_objDiceManager;
public:
    void ApplyLanding();

    // charge up
private:
    BOOL	m_bChargeUp;
    DWORD	m_dwTimeChargeUp;
    DWORD	m_dwChargeUpTotalTime;

public:
    void BeginChangeUp(OBJID idMagicType, DWORD dwDelay);
    void StopChargeUp();
    void ShowChargeUp();
    void AbortChargeUp();

    // user help info
public:
    int		GetUserHelpInfoLineAmount();
    const char*	GetUserHelpInfoLineByIndex(int nIndex);

    // arrow amount
private:
    BOOL	m_bShowArrow;
public:
    CCheck  m_objCheck;
public:
    void	ShowArrow(int nOffset);

    // for pet ai
private:
    OBJID	m_idWhoAttackMe;
    OBJID	m_idMyTarget;
    DWORD   m_dwTimeWhoAttackMe;
    OBJID	m_idSpecialTarget;
    DWORD	m_dwTimeSpecialTarget;
    vector<OBJID>	m_setPetInfo;
public:
    void	SetIDWhoAttackMe(OBJID id) {m_idWhoAttackMe = id; m_dwTimeWhoAttackMe = ::TimeGet();}
    void	SetIDMyTarget(OBJID id);
    OBJID	GetIDWhoAttackMe() {return m_idWhoAttackMe;}
    OBJID	GetIDMyTarget() {return m_idMyTarget;}
    void	SetSpecialTarget(OBJID idTarget);
    OBJID	GetSpecialTarget();
    void	ProcessSpecialTarget();
    DWORD	GetTimeWhoAttackme() {return m_dwTimeWhoAttackMe;}
    void	AddPet(OBJID idPet);
    void	DelPet(OBJID idPet);
    int		GetCallPetAmount() {return m_setPetInfo.size();}
    void	ClearPet();
    BOOL	CheckPet(OBJID idPet);
    CPet*   GetPetByID(OBJID idPet);
    CPet*	GetPetByItemID(OBJID idItem);
    CPet*   GetPetByIndex(int nIndex);
    BOOL	TryItem(OBJID idItem, int nPosition);
    DWORD	GetMonsterNameColor(int nMonsterLevel);
    void	IdentItem(OBJID idItem);
public:
    DWORD	GetSoul();
    DWORD	GetOriginalSoul();
    DWORD	GetAdditionalSoul();
    BOOL	IsMagicAttackBusy() {return m_bMagicAttackBusy || this->IsMagicLock();}

    void	Reject(int nType);

    void	SetTaskDialogID(OBJID idDialog) {m_idTaskDialog = idDialog;}
    OBJID	GetTaskDialogID() {return m_idTaskDialog;}

    // for teacher board
private:
    DEQUE_ANNOUNCE m_setAnnounceList;
public:
    AnnounceInfo m_infoAnnounce;

    void	QueryMyTeacherBoard();
    void	QueryTeacherBoardList(int nIndex);
    void	QueryTeacherBoardInfo(OBJID idTBoard);

    void	UpTeacherBoard(char* pszTitle, char* pszInfo);
    void	CancelTeacherBoard(OBJID idAnnounce);
    void	RequestTeacherBoard(OBJID idUser);

    ST_ANNOUNCE_TITLE* GetAnnounceByIndex(int nIndex);
    int				   GetAnnounceAmount();
    void			   AddAnnounceList(OBJID idAnnounce, const char* szTitle);
    void			   ClearAnnounceList();
    // for school ...
private:
    DEQUE_SCHOOLMEMBER m_setSchoolMember;
    vector<string>	m_setSecSchoolMember;
    BOOL	m_bQuerySchoolMember;
    OBJID	m_idLastTeacherApply;
    OBJID	m_idLastStudentApply;
    char	m_szLastTeacherApply[64];
    char	m_szLastStudentApply[64];
public:
    void	QuerySecSchoolMember(OBJID IdMember);
    int		GetSecSchoolMemberAmount();
    const char*	GetSecSchoolMemberByIndex(int nIndex);
    void	AddSecSchoolMember(char* strName);

    void	QuerySchoolMember();
    void    SetQuerySchoolMember(bool bFlag);
    BOOL	AddSchoolMember(CSchoolMember* pSchoolMember);
    BOOL	DelSchoolMember(OBJID id);
    BOOL	UpdataSchoolMember(OBJID id, UCHAR ucStatus, char* pszName, UCHAR ucRelation, char* pszMate, UCHAR ucLevel, UCHAR ucProfession, USHORT usPkValue, UCHAR ucNobilityRank, DWORD dwSynID_Rank);
    BOOL	IsHaveTeacher();
    BOOL	IsHaveStudent();
    int		GetSchoolMemberAmount();
    CSchoolMember* GetSchoolMember(OBJID id);
    CSchoolMember* GetSchoolMemberByIndex	(int nIndex);
    CSchoolMember* GetSchoolMemberByName	(const char* pszName);

    void		ClearSchoolMemberList();

    BOOL	TeacherApply(OBJID idPlayer); // 拜师
    BOOL	StudentApply(OBJID idPlayer); // 招徒
    BOOL    DismissStudent(OBJID idPlayer); //开除徒弟
    BOOL    LeaveTeacher();	//脱离师门
    BOOL	AgreeTeacherApply(); // 同意拜师请求
    BOOL	AgreeStudentApply(); // 同意招徒请求

    void	SetLastTeacherApply(OBJID idPlayer);
    void	SetLastStudentApply(OBJID idPlayer);
    const	char*	GetLastTeacherApply();	// 拜师人名字
    const	char*	GetLastStudentApply();  // 招徒人名字

    // 佣兵任务
private:
    DEQUE_PLAYERTASK m_setPlayerTask;
public:
    TaskInfo	m_infoTask;
    int			m_PlayerTaskType;
    BOOL		m_bMagicLock;
public:
    void    ClearPlayerTaskList();
    void	SetPlayerTaskListType(USHORT usType) {m_PlayerTaskType = usType ; }
    void	AddPlayerTaskList(OBJID idTask, const char* szTitle , UCHAR ucReqLev );
    DWORD	GetPlayerTaskAmout();
    ST_TASK_TITLE*	GetPlayerTaskIndex(int nIndex);

    void	QueryPlayerTaskAcceptedList(OBJID idLast);
    void	QueryPlayerTaskUnacceptedList(OBJID idLast);
    void	QueryPlayerTaskCompletedList(OBJID idLast);
    void	QueryPlayerMyTaskList(OBJID idLast);
    void	QueryPlayerTaskDetial(OBJID idTask);

    void	AcceptPlayerTask(OBJID idTask);
    void	CompletePlayerTask(OBJID idTask);
    void	CompletePlayerTaskForItem(OBJID idTask, OBJID idItem);
    void	CancelPlayerTask(OBJID idTask);

    void	WantPk(const char* pszName, const char* pszTitle, const char* pszDesc,
                   DWORD dwMoney, UCHAR ucRankReq, OBJID idItem0,
                   OBJID idItem1, OBJID idItem2, OBJID idItem3,
                   OBJID idItem4);
    void	WantItem(const char* pszName,  const char* pszTitle, const char* pszDesc,
                     UCHAR ucType,
                     DWORD dwDurability, UCHAR ucAddition, UCHAR ucDegree,
                     WORD dwMoney, UCHAR ucRankReq, OBJID idItem0,
                     OBJID idItem1, OBJID idItem2, OBJID idItem3,
                     OBJID idItem4);

    // 物品提升
    void	UpLevelItem(OBJID idItem, OBJID idTreesure);
    void	ImproveQuality(OBJID idItem, OBJID idGem);
    void	ImproveSoulDegree(OBJID idItem, OBJID idGem);
    void	ImproveMagic(OBJID idItem, OBJID idGem);
    void	MagicLock() {m_bMagicLock = true;}
    void	UnMagicLock() {m_bMagicLock = false;}
    BOOL	IsMagicLock() {return m_bMagicLock;}
private:
    OBJID   m_idAttchPet;	//当前合体的幻兽itemid
    int		m_nMaxPetCall;	//最大幻兽召唤数量

    DWORD   m_dwPetCallStartTime;
    DWORD   m_bPetCall;
public:
    // 幻兽相关
    void	SetMaxPetCall(int nMax) {m_nMaxPetCall = nMax;}
    int		GetMaxPetCall() {return m_nMaxPetCall;}
    void    SetAttachPetId( OBJID idPetItem) { m_idAttchPet = idPetItem;}
    OBJID   GetAttachPetId( ) {return m_idAttchPet;} //当前合体的幻兽itemid
    void    AttachEudemon(OBJID idPetItem); //幻兽合体 ,幻兽的itemid
    void    UnAttachEudemon(OBJID idPetItem); //幻兽解体
    void	EvolveEudemon(OBJID idPetItem, int nMode);// 幻兽进化, 幻兽itemID 进化类型
    void	ReEvolveEudemon(OBJID idPetItem, int nMode); //幻兽二次进化 ,幻兽itemid,进化类型
    void	ReboneEudemon(OBJID idPetItem, OBJID idGem);// 幻兽复活，幻兽itemID 宝石ID
    void	EnhanceEudemon(OBJID idPetItem, OBJID idGem);// 幻兽强化，幻兽itemID, 宝石ID
    void	CallEudenmon(OBJID idPetItem);
    void	KillEudenmon(OBJID idPetItem);
    void	RenameEudenmon(OBJID idPetItem, char* petName); //幻兽改名

    // collect resource
public:
    void	CollectRes(OBJID idRes);// 挖矿 伐木 采石
private:
    typedef struct
    {
        int x, y, cx, cy;
        std::string strMusic;
    } RangeMusicInfo;
    typedef	struct
    {
        std::vector<RangeMusicInfo> setRangeMusicInfo;
        std::string strBgMusic;
    } MusicInfo;
    MusicInfo m_setMusicInfo;
    std::string	m_strCurrentMusic;
    std::string	m_strCurrentMusicPlay;
    DWORD		m_dwLastChangeMusicTimer;
    int			m_nMusicVolume;
public:
    DWORD GetPetLevelExp(int nLev);
    void	LoadMusicInfo();
    void	ProcessMusic();
    void	SetMusicVolume(int nVolume);
public:// 急行相关
    int		m_nFastRunSetp;
public:
    int		m_nLastMagicDir;

public:
    char	m_szServerName[128];
    DWORD	m_dwServerPort;

    void	Set91ID(OBJID id) {m_dw91Id = id;}
    OBJID	Get91ID() {return m_dw91Id;}
private:
    DWORD		m_dw91Id;
};

extern CHero	g_objHero;
const int Statuary1037[7][3] =
{
    {187, 191, 100},
    {184, 198, 100},
    {194, 188, 100},
    {185, 210, 100},
    {200, 210, 100},
    {206, 204, 100},
    {206, 189, 100}
};
const int Statuary1038[7][3] =
{
    {379, 384, 100},
    {373, 387, 100},
    {382, 378, 100},
    {380, 394, 100},
    {382, 399, 100},
    {389, 385, 100},
    {394, 387, 100}
};

#endif // !defined(AFX_HERO_H__E1E1F17C_8E9A_439A_B54A_E7A6D3213111__INCLUDED_)
