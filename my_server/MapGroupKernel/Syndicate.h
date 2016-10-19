
//**********************************************************
// 代码编辑器
//**********************************************************

// Syndicate.h: interface for the CSyndicate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_)
#define AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "NetWorkDef.h"
#include "protocol.h"
#include "GameData.h"
#include "GameObj.h"
#include <vector>
using namespace std;

const int	SYN_MONEY_BASELINE		= 0;					// 帮派基金少于该值，会限制帮派功能

//////////////////////////////////////////////////////////////////////
enum
{
    SYNFUNC_DEMISE = 1,
    SYNFUNC_SETTENET,
    SYNFUNC_SETLEADERTITLE,
    SYNFUNC_SETMEMBERTITLE,
    SYNFUNC_ADDDATA,
    SYNFUNC_SETDATA,
    SYNFUNC_PRESENT,
    SYNFUNC_ADD_MEMBER,
    SYNFUNC_DEL_MEMBER,
    SYNFUNC_UP_LEVEL,
    SYNFUNC_SETSYNNAME,
};

//////////////////////////////////////////////////////////////////////
const int	MAX_SYNENEMYSIZE		= 5;		// 一共5个敌对帮派空间
const int	MAX_SYNALLYSIZE			= 5;		// 一共5个结盟帮派空间
enum SYNDATA
{
    SYNDATA_ID = 0,			// 仅用于INSERT

    SYNDATA_NAME = 1,			// 帮派名
    SYNDATA_TENET,			// 帮派宗旨
    SYNDATA_ANNOUNCE,		// 帮派公告
    SYNDATA_MEMBERTITLE,	// 帮派成员称号
    SYNDATA_LEADERID,		// 帮主ID
    SYNDATA_LEADERNAME,		// 帮主名字
    SYNDATA_MONEY,			// 帮派基金
    SYNDATA_FEALTY,			// Master ID
    SYNDATA_DEL_FLAG,
    SYNDATA_AMOUNT,			// 帮众数量
    SYNDATA_ENEMY0,			// 敌对帮派
    SYNDATA_ENEMY1,
    SYNDATA_ENEMY2,
    SYNDATA_ENEMY3,
    SYNDATA_ENEMY4,
    SYNDATA_ALLY0,			// 同盟帮派
    SYNDATA_ALLY1,
    SYNDATA_ALLY2,
    SYNDATA_ALLY3,
    SYNDATA_ALLY4,
    SYNDATA_RANK,			// 骑士团等级  1 ~ 5
    SYNDATA_SAINT,			// 圣骑士团    (0: NULL，1：圣)
    SYNDATA_MANTLE_FALG,	// 披风标志    add huang 2004.1.1
    SYNDATA_DISMISS_TIME,	// 自动解散开始时间   //add huang 2004.1.5
    SYNDATA_REPUTE,			// 帮派声望
    SYNDATA_PUBLISHTIME,	// 公告发布时间，格式：yyyymmdd
};
char	szSynTable[];
typedef	CGameData<SYNDATA, szSynTable, szID>	CSynBase;

const int   DISMISSSYN_MONEY_LIMIT  = 100000;           // 金钱少于此数，自动解散。//add huang 2004.1.5
const int	DISMISSSYN_AMOUNT_LIMIT	= 20;				// 帮众少于此数，自动解散
const int   DISMISSSYN_TIME_LIMIT   = 24 * 60 * 60;     // 自动解散帮派时限 24小时


//////////////////////////////////////////////////////////////////////
class ISynModify
{
public:
    virtual bool	Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave = true)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetTenet(LPCTSTR szTenet, bool bSave = true)													PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave = true)										PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave = true)										PURE_VIRTUAL_FUNCTION_0
    virtual bool	AddData(SYNDATA idx, int nData, bool bSave = true)											PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetData(SYNDATA idx, int nData, bool bSave = true)											PURE_VIRTUAL_FUNCTION_0
    virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave = true)										PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetSynName(LPCTSTR szSynName, bool bSave = true)												PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
// 所有成员函数为原子操作。
class CSyndicate : public CGameObj, ISynModify
{
protected:
    CSyndicate();
    virtual ~CSyndicate();
public:
    static CSyndicate* CreateNew() { ASSERT(!"CSyndicate::CreateNew()"); return NULL; }				// 用于CSynPtr
    static CSyndicate* CreateNew(PROCESS_ID idProcess)	{ CSyndicate* ptr = new CSyndicate; if (ptr) { ptr->m_idProcess = idProcess; } return ptr; }
    virtual ULONG	ReleaseByOwner()				{ delete this; return 0; }
    bool	Create(IRecordset* pRes);
    bool	Create(const CreateSyndicateInfo* pInfo, bool bSave);
    ISynModify*		QueryModify()	{ return (ISynModify*)&m_obj; }
    bool			ChangeSyndicate(const SynFuncInfo0* pFuncInfo);
    bool			ReloadData();

public: // const
    OBJID	GetID()							{ return m_pData->GetKey(); }
    int		GetInt(SYNDATA idx);
    LPCTSTR	GetStr(SYNDATA idx)				{ return m_pData->GetStr(idx); }
    void	BroadcastSynMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL);
    void	BroadcastSubSynMsg(SOCKET_ID idSocket, OBJID idNpc, LPCTSTR szMsg, CUser* pExcludeSender = NULL, LPCTSTR pszEmotion = NULL, DWORD dwColor = 0xffffff, int nAttribute = _TXTATR_SYNDICATE);
    LPCTSTR	GetFealtyName();
    bool	IsNearSyn(OBJID idSyn);
    //	OBJID	GetMasterSynID()				{ if (GetInt(SYNDATA_FEALTY) != ID_NONE) return GetInt(SYNDATA_FEALTY); return GetID(); }
    CSyndicate*		GetMasterSyn();
    bool	IsFriendly(OBJID idSyn, BOOL bNest = true);		// mast be true
    bool	IsHostile(OBJID idSyn, BOOL bNest = true);		// mast be true
    bool	IsNeutral(OBJID idSyn);
    bool	IsWhiteSynMapOwner();
    bool	IsBlackSynMapOwner();
    int		GetMemberAmount(bool bIncludeSub = true);
    int		GetSynAmount();
    bool    AutoDismissSyn();
protected:
    bool    CheckDismissTime();    // add huang 2004.1.7
    bool    CheckDismissSyn();
    void	BroadcastLocalSynMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL);

public: // application
    void	SendInfoToClient(CUser* pUser);
    void	SetNewSubName(LPCTSTR szName);		// 临时代码

    bool	ClearAllySyn(CSyndicate* pTargetSyn);
    bool	ClearEnemySyn(CSyndicate* pTargetSyn);

public: // link -------------------------------------
    CAutoLink<CSyndicate>&	QueryLink	()					{ return m_link; }
protected:
    CAutoLink<CSyndicate>	m_link;

public: // syn war -------------------------------------
    void	AddSynWarScore(OBJID idNpc, int nScore);
    int		GetSynWarScore(OBJID idNpc);
    void	ClearSynWarScore(OBJID idNpc);
protected:
    struct	SynWarInfo
    {
        OBJID	idNpc;
        int		nScore;
    };
    typedef	vector<SynWarInfo>	SYNWAR_SET;
    SYNWAR_SET		m_setSynWarScore;
protected: // modify, mast be atom operator. test & set
    // bSave 必须为true，否则不会存盘
    virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
    virtual bool	Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave);
    virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
    virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
    virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
    virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
    virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
    virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
public:
    int GetMoney();

private:
    CSynBase*	m_pData;

protected: // ctrl
    PROCESS_ID		m_idProcess;
    MYHEAP_DECLARATION(s_heap)
protected:
    class	XSynModify : public ISynModify
    {
    public:
        virtual bool	Demise(OBJID idOldLeader, int nOldSynMemberLevel, OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave);
        virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
        virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
        virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
        virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
        virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
        virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
        virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
    protected:
        CSyndicate*		This()		{ return m_pOwner; }
        CSyndicate*		m_pOwner;
        friend class CSyndicate;
    } m_obj;
public:
    void	SynchroInfo();

    OBJID	GetMasterSynID();
    bool	FallLevel();
    bool	IsFallLevel();
    bool	IsSubSubSyn();
    bool	IsSubSyn();
    int		GetSubSubSynNum();
    bool	IfCanCreateSubSubSyn();
    bool	IsMasterSyn()				{ return (GetInt(SYNDATA_FEALTY) == ID_NONE); }
    //帮派每周会在统一的地方发放本周军团利润的一部分作为成员所得
    bool	AllotBonusForMember(CUserPtr pUser, DWORD dwBonus);
    bool	ApplyKickoutMember(CUserPtr pLeader, CUserPtr pTarget);
    int		GetSubSynNum();
    friend class CSyndicate::XSynModify;
};

#endif // !defined(AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_)
