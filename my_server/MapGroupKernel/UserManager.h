
//**********************************************************
// 代码编辑器
//**********************************************************

// UserManager.h: interface for the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMANAGER_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
#define AFX_USERMANAGER_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	warning(disable:4786)
#include "define.h"
#include "User.h"
#include "GameBlock.h"
#include "team.h"
#include "NetworkDef.h"
#include "roleManager.h"
#include "index.h"
#include <vector>
using namespace std;

class IForMapGroup
{
public:
    virtual void	BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL)				PURE_VIRTUAL_FUNCTION
public: // user change mapgroup
    virtual void	JoinMapGroup(SOCKET_ID idSocket, UserInfoStruct* pInfo)				PURE_VIRTUAL_FUNCTION
};

typedef	IGameObjSet<CTeam>		ITeamSet;
typedef	CGameObjSet<CTeam>		CTeamSet;

class CAgent;
class CUserManager;
/*class IUserManager
{
public:
	virtual IForMapGroup*	GetForMapGroup()			PURE_VIRTUAL_FUNCTION_0
public:
	virtual ~IUserManager()	{}
	virtual void	Release()										PURE_VIRTUAL_FUNCTION
	virtual void	OnTimer(time_t tCurr)							PURE_VIRTUAL_FUNCTION
public:
	virtual CUser*	CreateUser(SOCKET_ID idSocket, OBJID idUser)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	LoginUser(SOCKET_ID idSocket, CUser* pUser)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	LogoutUser(SOCKET_ID idSocket, BOOL bOffline=true)			PURE_VIRTUAL_FUNCTION_0
	virtual CAgent*	CreateAgent(OBJID idAgent)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	LoginAgent(CAgent* pAgent)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	LogoutAgent(OBJID idAgent, BOOL bOffline=true)			PURE_VIRTUAL_FUNCTION_0
	virtual void	LogoutAllUser()							PURE_VIRTUAL_FUNCTION
	virtual bool	KickOutSocket(SOCKET_ID idSocket, LPCTSTR szReason)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsLoginMapGroup(SOCKET_ID idSocket)		PURE_VIRTUAL_FUNCTION_0
public:
	virtual CUser*	GetUser(OBJID idUser)					PURE_VIRTUAL_FUNCTION_0		// 请勿使用，改用GetRole
	virtual CUser*	GetUser(LPCTSTR szName)					PURE_VIRTUAL_FUNCTION_0
	virtual CUser*	GetUser(CNetMsg* pMsg)					PURE_VIRTUAL_FUNCTION_0
	virtual CUser*	GetUserBySocketID(SOCKET_ID idSocket)	PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetSocketAmount()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMapUserAmount(OBJID idMap)			PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetUserAmount()							PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMaxUser()							PURE_VIRTUAL_FUNCTION_0
	virtual bool	FindNextSynMember(OBJID idSyn, int& nIter)	PURE_VIRTUAL_FUNCTION_0
public:
	virtual void	BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL)					PURE_VIRTUAL_FUNCTION
	virtual void	BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender=NULL, LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, int nAttribute=_TXTATR_NORMAL)					PURE_VIRTUAL_FUNCTION
	virtual void	BroadcastMapMsg(OBJID idMap, CNetMsg* pMsg, CUser* pExcludeSender=NULL)				PURE_VIRTUAL_FUNCTION
public:	// --------- 队伍管理 ---------------
	virtual CTeam*			GetTeamByLeaderId		(OBJID idLeader)					PURE_VIRTUAL_FUNCTION_0
	virtual CTeam*			GetTeamByIndex			(int nIndex)						PURE_VIRTUAL_FUNCTION_0
	virtual int				GetTeamAmount			()									PURE_VIRTUAL_FUNCTION_0
	virtual BOOL			IsValidTeam				(CTeam* pTeam)						PURE_VIRTUAL_FUNCTION_0
	virtual	BOOL			AddTeam					(CTeam* pTeam)						PURE_VIRTUAL_FUNCTION_0
	virtual void			DelTeam					(CTeam* pTeam)						PURE_VIRTUAL_FUNCTION
	virtual OBJID			SpawnNewTeamID			()									PURE_VIRTUAL_FUNCTION_0
	CUserManager::Iterator	NewEnum					()									PURE_VIRTUAL_FUNCTION_(CUserManager::Iterator)
};*/

class ISocket;
class IDatabase;
class CUserManager;
typedef CUserManager IUserManager;
class CUserManager : IForMapGroup
{
public://---------------------------------------------
    class Iterator
    {
    private: // create by master
        Iterator(IRoleSet* pSet) : m_pSet(pSet), m_pIter(pSet->NewEnum()), m_pUser(NULL) {}
        friend class CUserManager;
    public:
        bool	Next()							{ while(m_pIter.Next()) { if (m_pIter->QueryObj(OBJ_USER, IPP_OF(m_pUser))) { return true; } } m_pUser = NULL; return false; }
        operator CUser*()						{ return m_pUser; }
        CUser* operator->()						{ ASSERT(m_pUser); return m_pUser; }
    protected:
        IRoleSet*			m_pSet;
        CRoleSet::Iterator	m_pIter;
        CUser*				m_pUser;
    };
    Iterator	NewEnum();
    //---------------------------------------------
public:
    CUserManager(PROCESS_ID idProcess) : m_idProcess(idProcess), m_dwMaxUser(0) {}
    virtual ~CUserManager() {}
    IUserManager*	GetInterface() { return (IUserManager*)this; }
    virtual IForMapGroup*	GetForMapGroup() { return (IForMapGroup*)this; }

public:
    virtual bool	Create();
    virtual void	Release();
    virtual void	OnTimer(time_t tCurr);
public: // login/logout
    virtual CUser*	CreateUser(SOCKET_ID idSocket, OBJID idUser);			// login第一步
    virtual bool	LoginUser(SOCKET_ID idSocket, CUser* pUser);			// login第二步
    virtual bool	LogoutUser(SOCKET_ID idSocket, BOOL bOffline = true);
    virtual CAgent*	CreateAgent(OBJID idAgent);
    virtual bool	LoginAgent(CAgent* pAgent);
    virtual bool	LogoutAgent(CUser* pAgent, BOOL bOffline = true);
    virtual bool	KickOutSocket(SOCKET_ID idSocket, LPCTSTR szReason = NULL);
    virtual void	LogoutAllUser();
    virtual bool	IsLoginMapGroup(SOCKET_ID idSocket)			{ return (idSocket >= 0 && idSocket < (SOCKET_ID)m_setUser.size() && m_setUser[idSocket]); }		// && m_setUser[m_setUser]->idProcess >= MSGPORT_MAPGROUP_FIRST

public: // get user
    virtual CUser*	GetUser(OBJID idUser);
    virtual CUser*	GetUser(LPCTSTR szName);
    virtual CUser*	GetUser(CNetMsg* pMsg);
    virtual CUser*	GetUserBySocketID(SOCKET_ID idSocket)	{ if (idSocket >= 0 && idSocket < m_setUser.size()) { return m_setUser[idSocket]; } return NULL; }
    //can't use this func	virtual DWORD	GetSocketAmount()						{ return m_setUser.size(); }
    virtual DWORD	GetMapUserAmount(OBJID idMap);
    virtual DWORD	GetUserAmount();
    virtual DWORD	GetMaxUser()	{return m_dwMaxUser;}
    DWORD	m_dwMaxUser;
    virtual bool	FindNextSynMember(OBJID idSyn, int& nIter);
    virtual CUser*	GetGM()	{ if (m_setGm.size() <= 0) { return NULL; } else { return this->GetUser(m_setGm.front()); } }

public:
    virtual void	BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL);			// 全世界广播
    virtual void	BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender = NULL, LPCTSTR pszEmotion = NULL, DWORD dwColor = 0xffffff, int nAttribute = _TXTATR_NORMAL);			// 全世界广播
    virtual void	BroadcastMapMsg(OBJID idMap, CNetMsg* pMsg, CUser* pExcludeSender = NULL);			// 本地图广播
public: // mapgroup
    virtual void	BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender = NULL);			// 本地图组广播
    virtual void	JoinMapGroup(SOCKET_ID idSocket, UserInfoStruct* pInfo);

public:	// --------- 队伍管理 ---------------
    CTeam*			GetTeamByLeaderId		(OBJID idLeader);
    CTeam*			GetTeamByIndex			(int nIndex);
    int				GetTeamAmount			();
    BOOL			IsValidTeam				(CTeam* pTeam);
    BOOL			AddTeam					(CTeam* pTeam);
    void			DelTeam					(CTeam* pTeam);
    OBJID			SpawnNewTeamID			();

public: // cheat ---------------------------------------------
    bool	AddCheat(OBJID idUser, int* pnAmount, int* pnTimes);
    int		GetCheatCount(OBJID idUser)				{ return m_setCheatIDTimes.IsIn(idUser) ? m_setCheatIDTimes.At(idUser) : 0; }
    int		GetCheatAmount()						{ return m_setCheatIDTimes.Size(); }
protected:
    Index<OBJID, int>	m_setCheatIDTimes;

protected:
    typedef	vector<CUser*> USER_SET;
    USER_SET	m_setUser;					// 以SOCKET_ID为索引。reference point set

    ITeamSet*	m_pTeamSet;
    OBJID		m_idLastTeam;

    vector<OBJID>	m_setGm;

    PROCESS_ID	m_idProcess;

public:
    MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(AFX_USERMANAGER_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
