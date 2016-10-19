
//**********************************************************
// 代码编辑器
//**********************************************************

// UserList.h: interface for the CUserList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERLIST_H__419FFB53_E120_437C_9BF8_B8839D75C9E1__INCLUDED_)
#define AFX_USERLIST_H__419FFB53_E120_437C_9BF8_B8839D75C9E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "AllMsg.h"
#include "define.h"
#include "windows.h"
#include "NetworkDef.h"
#include "TimeOut.h"
#include "ClientSocket.h"
#include "AutoPtr.h"
#include "GameObjSet.h"
#include <time.h>
#include <vector>
using namespace std;
using namespace world_kernel;

class CSupermanList;
struct CPlayer
{
    OBJID		idUser;
    NAMESTR		szName;
    PROCESS_ID	idProcess;

    NAMESTR		szAccount;				// 仅用于无帐号服务器登录
    SOCKET_ID	idSocket;			// idSocket == SOCKET_NONE : is agent

    CPlayer() { idUser = ID_NONE; m_tPoint = 0; idSocket = SOCKET_NONE; }
    OBJID	GetID()					{ return idUser; }
    void	ReleaseByOwner()		{ delete this; }
    OBJID	GetProcessID()			{ return idProcess; }
    bool	SendMsg(CNetMsg* pMsg);
    bool	IsAgent()				{ return idSocket == SOCKET_NONE; }
    bool	IsGM()					{ return (strstr(szName, "[GM]") || strstr(szName, "[PM]")); }
    bool	IsPM()					{ return (strstr(szName, "[PM]") != NULL); }

public: // point fee
    void	OnTimer(time_t tCurr);
public:
    int		m_tPoint;
    CTimeOut	m_tTick;
    OBJID	m_idAccount;
    int 	m_nLevel;

    NAMESTR		szNotify;
};

enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };

class IDatabase;
class CUserList
{
public:
    CUserList();
    virtual ~CUserList();
    static CUserList*	CreateNew()		{ return new CUserList; }
    bool	Create(ISocket* pPort, IDatabase* pDb);
    ULONG	Release()			{ delete this; return 0; }

public:
    void DelAllAgent();
    bool	LoginUser(LPCTSTR szAccount, LPCTSTR szPassword, SOCKET_ID idSocket);	// 仅用于非帐号服务器启动
    int		LoginUser(OBJID idAccount, SOCKET_ID idSocket, LPCTSTR szInfo);			// enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };
    bool	LoginAgent(OBJID idAgent);
    bool	LogoutUser(SOCKET_ID idSocket);		// return false: no this user
    bool	KickOutAccount(OBJID idAccount, LPCTSTR szText);		// return false: 不在线
    bool	KickOutSocket(SOCKET_ID idSocket);		// return false: 不在线
    void	OnTimer(time_t tCurr);
    void	LogoutAllUser();
    bool	CreateUser(PROCESS_ID idProcess, OBJID idUser, LPCTSTR szName, OBJID idAccount, SOCKET_ID idSocket, int nLevel, LPCTSTR szNotify);
    bool	CreateAgent(PROCESS_ID idProcess, OBJID idAgent, LPCTSTR szName);
    CPlayer*	GetPlayer(LPCTSTR szName);
    CPlayer*	GetPlayer(OBJID idUser);
    CPlayer*	GetPlayerBySocket(SOCKET_ID idSocket)	{ if (idSocket >= 0 && idSocket < m_setUser.size()) { return m_setUser[idSocket]; } return NULL; }
    CPlayer*	GetPlayerByAccount(LPCTSTR szAccount);
    CPlayer*	GetPlayerByAccountID(OBJID idAccount);
    CPlayer*	GetGM()		{ if (m_setGm.size() <= 0) { return NULL; } else { return this->GetPlayer(m_setGm.front()); } }
    bool	CreateNewPlayer(LPCTSTR szAccount, LPCTSTR szName, LPCTSTR szPassword,
                            int unLook, int unData, OBJID idAccount, char cLength, char cFat);
    void	ChangeProcessID(SOCKET_ID idSocket, PROCESS_ID idProcess);
    bool	IsLoginMapGroup(SOCKET_ID idSocket)			{ return (idSocket >= 0 && idSocket < m_setUser.size() && m_setUser[idSocket] && m_setUser[idSocket]->idProcess >= MSGPORT_MAPGROUP_FIRST); }
    void	LoginToMapGroup(CPlayer* pPlayer);
    int		GetUserAmount()								{ return m_nPlayerAmount; }
    void	SendTimeToSocket(SOCKET_ID idSocket);
    CSupermanList*	QuerySupermanList()					{ ASSERT(m_pSupermanSet); return m_pSupermanSet; }

    void	BroadcastMsg(CNetMsg* pMsg, CPlayer* pExcludeSender = NULL);			// 全世界广播
    void	BroadcastMsg(LPCTSTR szMsg, CPlayer* pExcludeSender = NULL, LPCTSTR pszEmotion = NULL, DWORD dwColor = 0xffffff, int nAttribute = _TXTATR_NORMAL);			// 全世界广播

protected:
    int m_nHour;
    CTimeOut m_tHour;
    typedef vector<CPlayer*>	USERLIST_SET;
    USERLIST_SET	m_setUser;
    typedef	CGameObjSet<CPlayer>	AGENT_SET;
    CAutoPtr<AGENT_SET>		m_setAgent;
    vector<OBJID>	m_setGm;

    ISocket*		m_pSocket;

    CAutoPtr<CSupermanList>		m_pSupermanSet;
    int				m_nPlayerAmount;
    int				m_nMaxPlayerAmount;

public: // ctrl
    MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(AFX_USERLIST_H__419FFB53_E120_437C_9BF8_B8839D75C9E1__INCLUDED_)
