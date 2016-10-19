
//**********************************************************
// 代码编辑器
//**********************************************************

// RoleManager.h: interface for the CRoleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROLEMANAGER_H__275BD196_A44A_426C_931A_1697009F510D__INCLUDED_)
#define AFX_ROLEMANAGER_H__275BD196_A44A_426C_931A_1697009F510D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "GameObjSet.h"
#include "I_Role.h"
#include "T_SingleMap.h"
#include "MyHeap.h"

typedef	CSingleMap<IRole>		IRoleSet;
typedef	CSingleMap<IRole>		CRoleSet;
//typedef	IGameObjSet<IRole>		IRoleSet;
//typedef	CGameObjSet<IRole>		CRoleSet;

class CRoleManager;
class IRoleManager
{
public:
    virtual ~IRoleManager()	{}
    virtual void	Release()									PURE_VIRTUAL_FUNCTION
    virtual void	OnTimer(time_t tCurr)						PURE_VIRTUAL_FUNCTION
public:
    virtual IRole*		QueryRole(OBJID idRole)					PURE_VIRTUAL_FUNCTION_0
    virtual IRole*		QueryRole(CNetMsg* pMsg, OBJID idRole = ID_NONE)				PURE_VIRTUAL_FUNCTION_0
    virtual CRoleSet*	QuerySet()								PURE_VIRTUAL_FUNCTION_0
    virtual CNpc*		QueryDynNpcByRecordID(OBJID idNpcRecord)	PURE_VIRTUAL_FUNCTION_0
public:
    virtual void	BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL)					PURE_VIRTUAL_FUNCTION
    virtual void	BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender = NULL)					PURE_VIRTUAL_FUNCTION
    virtual void	BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL)			PURE_VIRTUAL_FUNCTION
    virtual void	BroadcastMapGroupMsg(LPCTSTR szMsg, CUser* pExcludeSender = NULL)			PURE_VIRTUAL_FUNCTION
    virtual void	SaveAll()									PURE_VIRTUAL_FUNCTION
    virtual bool	LoadAllPet()								PURE_VIRTUAL_FUNCTION_0
public:
    virtual int		CountAllNpc(OBJID idMap)								PURE_VIRTUAL_FUNCTION_0
    virtual int		CountFurnitureNpc(OBJID idMap)							PURE_VIRTUAL_FUNCTION_0
    virtual int		CountNpcByName(OBJID idMap, LPCTSTR szName)				PURE_VIRTUAL_FUNCTION_0
    virtual int		CountNpcByType(OBJID idMap, int nType)					PURE_VIRTUAL_FUNCTION_0
    virtual int		CountMonsterByName(OBJID idMap, LPCTSTR szName)			PURE_VIRTUAL_FUNCTION_0
    virtual int		CountMonsterByGenID(OBJID idMap, OBJID idGen)			PURE_VIRTUAL_FUNCTION_0
};

class ISocket;
class IDatabase;
class CRoleManager : IRoleManager
{
public:
    CRoleManager(PROCESS_ID idProcess);
    virtual ~CRoleManager();
    IRoleManager*	GetInterface() { return (IRoleManager*)this; }

public: // interface
    virtual IRole*		QueryRole(OBJID idRole)		{ return QuerySet()->GetObj(idRole); }
    virtual IRole*		QueryRole(CNetMsg* pMsg, OBJID idRole = ID_NONE);
    virtual CRoleSet*	QuerySet()		{ ASSERT(m_setRole); return m_setRole; }		//@ 删除玩家时，请注意UserManager中的玩家指针
    virtual CNpc*		QueryDynNpcByRecordID(OBJID idNpcRecord);

public:
    virtual bool	Create();
    virtual void	Release()			{ delete this; }
    virtual void	OnTimer(time_t tCurr);
    virtual void	SaveAll();
    virtual bool	LoadAllPet();

public: // count
    virtual int		CountAllNpc(OBJID idMap);
    virtual int		CountFurnitureNpc(OBJID idMap);
    virtual int		CountNpcByName(OBJID idMap, LPCTSTR szName);
    virtual int		CountNpcByType(OBJID idMap, int nType);
    virtual int		CountMonsterByName(OBJID idMap, LPCTSTR szName);
    virtual int		CountMonsterByGenID(OBJID idMap, OBJID idGen);

public:
    virtual void	BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL);			// 全世界广播
    virtual void	BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender = NULL);			// 全世界广播
public: // mapgroup
    virtual void	BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL);			// 本地图组广播
    virtual void	BroadcastMapGroupMsg(LPCTSTR szMsg, CUser* pExcludeSender = NULL);			// 全世界广播

protected:
    CRoleSet*		m_setRole;

    PROCESS_ID	m_idProcess;

public:
    MYHEAP_DECLARATION(s_heap);
};


inline bool IsNpcID(OBJID idRole)		{ return (idRole >= NPCSERVERID_FIRST && idRole <= NPCSERVERID_LAST); }
inline bool IsSceneID(OBJID idRole)		{ return (idRole >= SCENEID_FIRST && idRole <= SCENEID_LAST); }
inline bool IsTrapID(OBJID idRole)		{ return (idRole >= TRAPID_FIRST && idRole <= TRAPID_LAST); }


#endif // !defined(AFX_ROLEMANAGER_H__275BD196_A44A_426C_931A_1697009F510D__INCLUDED_)
