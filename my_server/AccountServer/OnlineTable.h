
//**********************************************************
// 代码编辑器
//**********************************************************

// OnlineTable.h: interface for the COnlineTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINETABLE_H__81A9486E_D360_4B8A_9028_CF9E67FF50EF__INCLUDED_)
#define AFX_ONLINETABLE_H__81A9486E_D360_4B8A_9028_CF9E67FF50EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "T_Index.h"
#include "string"

using namespace std;

enum { c_typeNone = 0, c_typePoint, c_typeTime, c_typeNetBarPoint, c_typeNetBarTime, c_typeISP, c_typeFree, c_typeAll };

class COnlineAccount
{
public:
    COnlineAccount();
    virtual ~COnlineAccount();
public:
    // 无。由友元操作
protected:
    void	Init();
    void	Init(int nState,
                 OBJID idAccount,
                 DWORD nAuthenID,
                 LPCTSTR	pszLoginName,
                 LPCTSTR	pszPassword,
                 LPCTSTR	pszClientIP,
                 LPCTSTR	pszServerName,
                 int		nPointType,
                 OBJID		idFeeAccount,
                 LPCTSTR	szFeeAccount);
protected:
    enum { c_stateOnlineNone = 0, c_stateOnlineNormal, c_stateOnlineRejoin };
    int		m_nState;				// 非0表示在线

    OBJID	m_idAccount;			// 主键
    DWORD	m_nAuthenID;
    char 	m_szLoginName[IPSTRSIZE];
    char 	m_szPassword[IPSTRSIZE];
//? 游戏服务器未上传此属性	OBJID	m_idRole;

    char	m_szClientIP[IPSTRSIZE];
    char	m_szServerName[SERVERNAMESIZE];

    int		m_nFeeType;			// ★补登的表项类型为c_typeNone。enum { c_typeNone = 0, c_typePoint, ..., c_typeAll };
    OBJID	m_idFeeAccount;						// 计费帐号。如果在网吧登录，为网吧帐号名；否则同玩家帐号名。
    char	m_szFeeAccount[IPSTRSIZE];			// 计费帐号。如果在网吧登录，为网吧帐号名；否则同玩家帐号名。
    time_t	m_tStartPoint;
    time_t	m_tLastPoint;
    time_t	m_tEndPoint;

    enum EEndType { c_endNone = 0, c_endNormal, c_endRejoin, c_endGameServerKick, c_endAccountRejoinKick,
                    c_endAccountOvertimeKick, c_endAccountCloseKick, c_endGameServerLose, c_endAll
                  } m_nEndType;
    int		m_nPointCount;
    time_t	m_tHeartbeat;
    time_t	m_tTimeWait;

    DWORD	m_dwSerial;

    friend	class COnlineTable;
};

class COnlineTable
{
public:
    COnlineTable(int nSize);
    ~COnlineTable();
public:
//	COnlineAccount &	operator [](int nIndex);
    typedef	CIndex<DWORD, int>	SERIAL_INDEX;

    // 返回OnlineIndex
    bool	GetAttr(const char* szAccount, OBJID& idAccount, char* szPassword,
                    DWORD& nAuthenID, char* szServerName, char* szFeeAccount, OBJID& idFeeAccount, char* szOldIP);
    bool	GetAttr2(OBJID idAccount, int& nFeeType, char* szServerName,
                     OBJID& idFeeAccount, char* szFeeAccount);
    bool	AddNew(OBJID idAccount, DWORD nAuthenID, const char* pClientIP, const char* szLoginName
                   , const char* szPassword, int nPointType, const char* szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount);		// return 0: error
    bool	AppendNew(OBJID idAccount, DWORD nAuthenID, const char* pClientIP, const char* szLoginName
                      , const char* szPassword, int nPointType, const char* szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount);			// return 0: error
    bool	Rejoin(OBJID idAccount, DWORD nAuthenID, const char* pClientIP, const char* szLoginName
                   , const char* szPassword, int nPointType, const char* szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount);			// return 0: error
    int		StartFee(OBJID idAccount, const char* pszServerName);		// return -n: 服务器不匹配
    int 	EndFee(OBJID idAccount, const char* pszServerName, bool bOffline);			//? 同时删除成员	// return -n: 服务器不匹配
    int		PointFee(OBJID idAccount, const char* pszServerName);		// return -n: 服务器不匹配
    int		GetPlayerCount(LPCTSTR pszServerName = NULL)
    {
        LOCKTHREAD;
        if (pszServerName == NULL) { return m_nCount; }
        else { return m_aPlayerCount[pszServerName]; }
    }
    SERIAL_INDEX*	GetSerialSet()			{ return &m_aSerialCount; }
    void	SetPassword(OBJID idAccount, const char* szPassword)		// 用于补登的玩家重登录时添加口令
    {
        LOCKTHREAD;
        int	nIndex = FindIndex(idAccount);
        if (nIndex && strlen(szPassword) < _MAX_PSWSIZE)
        {
            strcpy(m_pAccount[nIndex].m_szPassword, szPassword);
        }
        else
        {
            LOGERROR("SetPassword([%s]) 内部异常", szPassword);
        }
    }
    void	SetSerial(OBJID idAccount, DWORD dwSerial)		// 用于补登序列号
    {
        LOCKTHREAD;
        int	nIndex = FindIndex(idAccount);
        if (nIndex && dwSerial < 10000)				// serial 不大于9999
        {
            m_pAccount[nIndex].m_dwSerial	= dwSerial;
            m_aSerialCount.Add(dwSerial, m_aSerialCount[dwSerial] + 1);
        }
        else
        {
            LOGERROR("SetSerial([%d]) 异常", dwSerial);
        }
    }
    bool	IsOnline(OBJID idAccount) { LOCKTHREAD; return FindIndex(idAccount) != 0; }
    bool	IsTimeWait(OBJID idAccount)
    {
        LOCKTHREAD;
        int nIndex = FindIndex(idAccount);
        return nIndex && m_pAccount[nIndex].m_tTimeWait;
    }
    bool	RemoveOvertime();
    bool	GetServerName(OBJID idAccount, char* bufServer);
    int		GetFeeCount(OBJID idFeeAccount);
    bool	CheckUniqueIP(LPCTSTR pClientIP, OBJID idAccount);
protected:
    COnlineAccount* 	m_pAccount;
    typedef	CIndex<OBJID, int>	IndexID_t;
    IndexID_t		m_idxID;		// 三个索引注意与在线表同步
    typedef	CIndex<string, int>	IndexName_t;
    IndexName_t		m_idxName;
    int				m_nCount;
    CIndex<string, int>	m_aPlayerCount;
    SERIAL_INDEX	m_aSerialCount;
    CCriticalSection	m_xCtrl;
private:
    int		FindIndex(const char* szLoginName);		// return 0: ERROR
    int		FindIndex(OBJID idAccount);					// return 0: ERROR
    int		FindFree(OBJID idAccount);					// return 0: ERROR
    void	Remove_0(int nIndex);
    void	LogFee(int nIndex, enum COnlineAccount::EEndType nEndType);
};

#endif // !defined(AFX_ONLINETABLE_H__81A9486E_D360_4B8A_9028_CF9E67FF50EF__INCLUDED_)
