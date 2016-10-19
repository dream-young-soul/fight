
//**********************************************************
// 代码编辑器
//**********************************************************

// UserManager.h: interface for the CMapGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPGROUP_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
#define AFX_MAPGROUP_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	warning(disable:4786)
#include "define.h"
#include "RoleManager.h"
#include "UserManager.h"
#include "MapManager.h"
#include "NpcManager.h"
#include "SynManager.h"
#include "LeaveWord.h"
#include "autoptr.h"
#include "GameAction.h"
#include "EventPack.h"

#include "DropRuleMap.h"	//add by zlong 2003-11-15
#include "MercenaryTask.h"
#include "Announce.h"

#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////
class IIntraMsg
{
public:
    //	virtual ~IIntraMsg() {}

    virtual bool	SendClientMsg		(SOCKET_ID idSocket, CNetMsg* pMsg)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	SendNpcMsg			(OBJID idNpc, CNetMsg* pMsg)						PURE_VIRTUAL_FUNCTION_0
    virtual bool	SendBroadcastMsg	(CNetMsg* pNetMsg)									PURE_VIRTUAL_FUNCTION_0
    virtual bool	TransmitMsg			(CNetMsg* pNetMsg, SOCKET_ID idSocket, OBJID idNpc)									PURE_VIRTUAL_FUNCTION_0
    virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg)									PURE_VIRTUAL_FUNCTION_0
    virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, LPCTSTR szName)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, OBJID idUser)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	TransmitWorldMsg	(SOCKET_ID idSocket, CNetMsg* pMsg)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	SetNpcProcessID		(OBJID idNpc, bool bAddNew)							PURE_VIRTUAL_FUNCTION_0
    virtual bool	SendSocketUserInfo	(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	SendObjInfo			(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoMsgID, void* pInfo, int nSize)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	ChangeMapGroup		(PROCESS_ID idProcess, OBJID idUser, OBJID idMap, int nPosX, int nPosY)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	PrintText			(LPCTSTR szText)									PURE_VIRTUAL_FUNCTION_0
    virtual bool	CloseSocket			(SOCKET_ID idSocket)								PURE_VIRTUAL_FUNCTION_0
    virtual bool	CloseMapGroup		(SOCKET_ID idGmSocket)								PURE_VIRTUAL_FUNCTION_0
    virtual bool	ChangeTeam			(int nAction, OBJID idTeam, OBJID idUser, int nData = 0)			PURE_VIRTUAL_FUNCTION_0
    virtual void	RemoteCall			(REMOTE_CALL0* pInfo, bool bSendToLocal = false)								PURE_VIRTUAL_FUNCTION
    virtual void	LevelUp				(OBJID idUser, int nLevel)							PURE_VIRTUAL_FUNCTION
    virtual void	QueryFee			(OBJID idAccount)									PURE_VIRTUAL_FUNCTION
    virtual void	ModifyNpc			(OBJID idNpc, LPCTSTR szField, LPCTSTR szData)		PURE_VIRTUAL_FUNCTION
    virtual void	DelTransNpc			(OBJID idMaster, bool bBroadcast = true)				PURE_VIRTUAL_FUNCTION
    virtual void	ChangeCode			(SOCKET_ID idSocket, DWORD dwCode)					PURE_VIRTUAL_FUNCTION
    virtual void	SendRpc				(const CEventPack& pPack)							PURE_VIRTUAL_FUNCTION
};

//////////////////////////////////////////////////////////////////////
class IDataDefault
{
public:
    virtual IRecordset*		GetMagicData()												PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetUserWeaponSkill()										PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetGameItemData()											PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetUserData()												PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetGameMapData()											PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetSynData()												PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetSynAttrData()											PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetPetData()												PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetDynaNpc()												PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetDynaMap()												PURE_VIRTUAL_FUNCTION_0
    virtual IRecordset*		GetEudemonData()											PURE_VIRTUAL_FUNCTION_0
};

class ISynMessage
{
public:
    virtual bool	CreateSyndicate		(const CreateSyndicateInfo* pInfo)				PURE_VIRTUAL_FUNCTION_0
    virtual bool	DestroySyndicate	(OBJID idSyn, OBJID idTarget = ID_NONE)			PURE_VIRTUAL_FUNCTION_0
    virtual bool	CombineSyndicate	(OBJID idSyn, OBJID idTarget)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	ChangeSyndicate		(const SynFuncInfo0* pFuncInfo)					PURE_VIRTUAL_FUNCTION_0
    virtual void	SetMapSynID			(OBJID idMap, OBJID idSyn)						PURE_VIRTUAL_FUNCTION
};

//////////////////////////////////////////////////////////////////////
class ISocket;
class IDatabase;
class IMessagePort;
class CMapGroup : IIntraMsg, IDataDefault, ISynMessage
{
public:
    CMapGroup();
    virtual ~CMapGroup() {}
    PROCESS_ID		GetID() { return m_idProcess; }
    int		GetMapGroup()	{ return m_idProcess - MSGPORT_MAPGROUP_FIRST; }

public: // create
    bool	Create(PROCESS_ID idProcess, ISocket* pSocket, IDatabase* pDb, IMessagePort* pMsgPort);
    void	Destroy();
    void	OnTimer(time_t tCurr);
    IUserManager*	CreateNewUserManager();
    IMapManager*	CreateNewMapManager();
    INpcManager*	CreateNewNpcManager();

private:// action
    CTimeOut	m_tmEvent;

    void	ProcessEvent	(void);

public: // interface
    IRoleManager*	GetRoleManager()	{ ASSERT(m_pRoleManager); return m_pRoleManager; }
    IUserManager*	GetUserManager()	{ ASSERT(m_pUserManager); return m_pUserManager; }
    IMapManager*	GetMapManager()		{ ASSERT(m_pMapManager); return m_pMapManager; }
    INpcManager*	GetNpcManager()		{ ASSERT(m_pNpcManager); return m_pNpcManager; }
    IDatabase*		GetDatabase()		{ ASSERT(m_pDb); return m_pDb; }
    CSynManager*	GetSynManager()		{ ASSERT(m_pSynManager); return m_pSynManager; }
    IIntraMsg*		QueryIntraMsg()		{ return (IIntraMsg*)this; }
    IDataDefault*	QueryDataDefault()	{ return (IDataDefault*)this; }
    ISynMessage*	QuerySynMessage()	{ return (ISynMessage*)this; }
    CLeaveWord*		GetLeaveWord()		{ ASSERT(m_pLeaveWord); return m_pLeaveWord; }
    CGameAction*	GetGameAction()		{ ASSERT(m_pGameAction); return m_pGameAction; }
    CMercenaryTask*	GetMercenaryTask()	{ ASSERT(m_pMercenaryTask); return m_pMercenaryTask; }
    IMessagePort*	QueryMsgPort()		{ CHECKF(m_pMsgPort); return m_pMsgPort; }
    CAnnounce*      GetAnnounce()          { CHECKF(m_pAnnounce); return m_pAnnounce;}

public: // const
    int				GetMapGroupAmount();

protected: // IIntraMsg

    virtual bool	SendClientMsg		(SOCKET_ID idSocket, CNetMsg* pMsg);			// to network
    virtual bool	SendNpcMsg			(OBJID idNpc, CNetMsg* pMsg);					// to network
    virtual bool	SendBroadcastMsg	(CNetMsg* pNetMsg);
    virtual bool	TransmitMsg			(CNetMsg* pNetMsg, SOCKET_ID idSocket, OBJID idNpc);		// 广播给其它地图组核心处理
    virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg);									// 广播给世界核心处理
    virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, LPCTSTR szName);					// 通过世界核心转发给玩家
    virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, OBJID idUser);					// 通过世界核心转发给玩家
    virtual bool	TransmitWorldMsg	(SOCKET_ID idSocket, CNetMsg* pMsg);

    virtual bool	SendSocketUserInfo	(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo);
    virtual bool	SendObjInfo			(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoMsgID, void* pInfo, int nSize);
    virtual bool	SetNpcProcessID		(OBJID idNpc, bool bAddNew);
    virtual bool	ChangeMapGroup		(PROCESS_ID idProcess, OBJID idUser, OBJID idMap, int nPosX, int nPosY);
    virtual bool	PrintText			(LPCTSTR szText);
    virtual bool	CloseSocket			(SOCKET_ID idSocket)			{ return m_pSocket->CloseSocket(idSocket); }
    virtual bool	CloseMapGroup		(SOCKET_ID idGmSocket);		// 关闭所有socket(除GM)，禁止登录
    virtual bool	ChangeTeam			(int nAction, OBJID idTeam, OBJID idUser, int nData);			// 组队相关
    virtual void	RemoteCall			(REMOTE_CALL0* pInfo, bool bSendToLocal = false);
    virtual void	LevelUp				(OBJID idUser, int nLevel);
    virtual void	QueryFee			(OBJID idAccount);
    virtual void	ModifyNpc			(OBJID idNpc, LPCTSTR szField, LPCTSTR szData);
    virtual void	DelTransNpc			(OBJID idMaster, bool bBroadcast = true);
    virtual void	ChangeCode			(SOCKET_ID idSocket, DWORD dwCode);
    virtual void	SendRpc				(const CEventPack& pPack);

protected:
    PROCESS_ID		m_idProcess;
    ISocket*		m_pSocket;
    IDatabase*		m_pDb;
    IMessagePort*	m_pMsgPort;

    IRoleManager*	m_pRoleManager;
    IUserManager*	m_pUserManager;
    IMapManager*	m_pMapManager;
    INpcManager*	m_pNpcManager;
    CSynManager*	m_pSynManager;
    CLeaveWord*		m_pLeaveWord;
    CGameAction*	m_pGameAction;
    CMercenaryTask*	m_pMercenaryTask;
    CAnnounce*      m_pAnnounce;
    //	CAutoPtr<CGameAction>		m_ptrGameAction;

protected: // GameDataDefault
    IRecordset*		GetMagicData()			{ ASSERT(m_pMagicDataDefault); return m_pMagicDataDefault; }
    IRecordset*		GetUserWeaponSkill()	{ ASSERT(m_pUserWeaponSkillDefault); return m_pUserWeaponSkillDefault; }
    IRecordset*		GetGameItemData()		{ ASSERT(m_pGameItemDataDefault); return m_pGameItemDataDefault; }
    IRecordset*		GetUserData()			{ ASSERT(m_pUserDataDefault); return m_pUserDataDefault; }
    IRecordset*		GetGameMapData()		{ ASSERT(m_pGameMapDataDefault); return m_pGameMapDataDefault; }
    IRecordset*		GetSynData()			{ ASSERT(m_pSynDataDefault); return m_pSynDataDefault; }
    IRecordset*		GetSynAttrData()		{ ASSERT(m_pSynAttrDataDefault); return m_pSynAttrDataDefault; }
    IRecordset*		GetPetData()			{ ASSERT(m_pPetDataDefault); return m_pPetDataDefault; }
    IRecordset*		GetDynaNpc()			{ ASSERT(m_pDynaNpcDataDefault); return m_pDynaNpcDataDefault; }
    IRecordset*		GetDynaMap()			{ ASSERT(m_pDynaMapDataDefault); return m_pDynaMapDataDefault; }
    IRecordset*		GetEudemonData()		{ ASSERT(m_pEudemonDataDefault); return m_pEudemonDataDefault; }
protected:
    IRecordset*			m_pMagicDataDefault;
    IRecordset*			m_pUserWeaponSkillDefault;
    IRecordset*			m_pGameItemDataDefault;
    IRecordset*			m_pUserDataDefault;
    IRecordset*			m_pGameMapDataDefault;
    IRecordset*			m_pSynDataDefault;
    IRecordset*			m_pSynAttrDataDefault;
    IRecordset*			m_pPetDataDefault;
    IRecordset*			m_pDynaNpcDataDefault;
    IRecordset*			m_pDynaMapDataDefault;
    IRecordset*			m_pEudemonDataDefault;
protected: // Syndicate
    virtual bool	CreateSyndicate		(const CreateSyndicateInfo* pInfo);
    virtual bool	DestroySyndicate	(OBJID idSyn, OBJID idTarget = ID_NONE);
    virtual bool	CombineSyndicate	(OBJID idSyn, OBJID idTarget);
    virtual bool	ChangeSyndicate		(const SynFuncInfo0* pFuncInfo);
    virtual void	SetMapSynID			(OBJID idMap, OBJID idSyn);

public: // static
    static void	AddMapGroup(PROCESS_ID idProcess)
    { if (idProcess >= m_setMapGroup.size()) { m_setMapGroup.resize(idProcess + 1); } }
    static int	IsValidMapGroupProcessID(PROCESS_ID idProcess)
    { return idProcess >= MSGPORT_MAPGROUP_FIRST && idProcess < m_setMapGroup.size(); }

    static BOOL	CheckTime(const DWORD dwType, const char* pszParam);

private: // static
    typedef	vector<CMapGroup>	MAPGROUP_SET;
    static MAPGROUP_SET		m_setMapGroup;
    friend CMapGroup*	MapGroup(PROCESS_ID idProcess);		// 可存取以上静态对象

public:

    //	MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////
#define		PID	m_idProcess
extern CMapGroup*	MapGroup(PROCESS_ID idProcess);
inline CMapGroup*	MapGroup(PROCESS_ID idProcess) { CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess)); return &CMapGroup::m_setMapGroup[idProcess]; }
// 用法：MapGroup(PID)->UserManager()->GetUser(idUser);

//////////////////////////////////////////////////////////////////////
#define	Database()			(::MapGroup(PID)->GetDatabase())
#define	RoleManager()		(::MapGroup(PID)->GetRoleManager())
#define	UserManager()		(::MapGroup(PID)->GetUserManager())
#define	MapManager()		(::MapGroup(PID)->GetMapManager())
#define	NpcManager()		(::MapGroup(PID)->GetNpcManager())
#define	GameDataDefault()	(::MapGroup(PID)->QueryDataDefault())
#define	SynManager()		(::MapGroup(PID)->GetSynManager())
#define	LeaveWord()			(::MapGroup(PID)->GetLeaveWord())
#define	GameAction()		(::MapGroup(PID)->GetGameAction())
#define MercenaryTask()		(::MapGroup(PID)->GetMercenaryTask())
#define Announce()          (::MapGroup(PID)->GetAnnounce())

//////////////////////////////////////////////////////////////////////
inline IDatabase*		_Database(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetDatabase(); }
inline IRoleManager*	_RoleManager(PROCESS_ID PID)		{ return ::MapGroup(PID)->GetRoleManager(); }
inline IUserManager*	_UserManager(PROCESS_ID PID)		{ return ::MapGroup(PID)->GetUserManager(); }
inline IMapManager*		_MapManager(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetMapManager(); }
inline INpcManager*		_NpcManager(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetNpcManager(); }
inline IDataDefault*	_GameDataDefault(PROCESS_ID PID)	{ return ::MapGroup(PID)->QueryDataDefault(); }
inline CSynManager*		_SynManager(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetSynManager(); }
inline CLeaveWord*		_LeaveWord(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetLeaveWord(); }
inline CAnnounce*       _Announce(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetAnnounce();}
inline CGameAction*		_GameAction(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetGameAction(); }
inline CMercenaryTask*	_MercenaryTask(PROCESS_ID PID)		{ return ::MapGroup(PID)->GetMercenaryTask(); }

//////////////////////////////////////////////////////////////////////
inline OBJID			GetServerID()			{ extern DWORD	g_dwServerID; return g_dwServerID; }
inline CItemType*		ItemType()				{ extern CItemType* g_pItemType; CHECKF(g_pItemType); return g_pItemType; }

// add by zlong 2003-11-15
inline CDropRuleMap*	DropRuleMap()			{ extern CDropRuleMap* g_pDropRuleMap; CHECKF(g_pDropRuleMap); return g_pDropRuleMap; }

inline ITaskSet*		TaskSet()				{ extern ITaskSet* g_setTask; CHECKF(g_setTask); return g_setTask; }
inline IActionSet*		ActionSet()				{ extern IActionSet* g_setAction; CHECKF(g_setAction); return g_setAction; }
inline ILevupexpSet*	LevupexpSet()			{ extern ILevupexpSet* g_setLevupexp; CHECKF(g_setLevupexp); return g_setLevupexp; }
inline IAddPointSet*	AddPointSet()			{ extern IAddPointSet* g_setAddPoint; CHECKF(g_setAddPoint); return g_setAddPoint; }
inline IMagicTypeSet*	MagicTypeSet()			{ extern IMagicTypeSet* g_setMagicType; CHECKF(g_setMagicType); return g_setMagicType; }
inline IMagicTypeSet*	AutoMagicTypeSet()		{ extern IMagicTypeSet* g_setAutoMagicType; CHECKF(g_setAutoMagicType); return g_setAutoMagicType; }
inline IMagicTrackSet*	MagicTrackSet()			{ extern IMagicTrackSet* g_setMagicTrack; CHECKF(g_setMagicTrack); return g_setMagicTrack; }
inline ITrapTypeSet*	TrapTypeSet()			{ extern ITrapTypeSet* g_setTrapType; CHECKF(g_setTrapType); return g_setTrapType; }
inline IMonsterTypeSet*	MonsterType()			{ extern IMonsterTypeSet* g_setMonsterType; CHECKF(g_setMonsterType); return g_setMonsterType; }
inline IRebirthSet*		RebirthSet()			{ extern IRebirthSet* g_setRebirth; CHECKF(g_setRebirth); return g_setRebirth; }
inline CItemAddition*	ItemAddition()			{ extern CItemAddition* g_pItemAddition; CHECKF(g_pItemAddition); return g_pItemAddition; }

//////////////////////////////////////////////////////////////////////
#define	g_db			(*Database())			// 兼容于旧代码
#define	g_UserManager	(*UserManager())		// 兼容于旧代码



#endif // !defined(AFX_MAPGROUP_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
