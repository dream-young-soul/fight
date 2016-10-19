
//**********************************************************
// 代码编辑器
//**********************************************************

// OnlineTable.cpp: implementation of the COnlineTable class.
//
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "OnlineTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const DWORD	INVALID_SERIAL			= 1234567890;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// COnlineAccount

COnlineAccount::COnlineAccount()
{
    Init();
}

COnlineAccount::~COnlineAccount()
{
}

void COnlineAccount::Init()
{
    m_nState			= c_stateOnlineNone;
    m_idAccount			= 0;			// 主键
    m_szLoginName[0]	= 0;
    m_szPassword[0]		= 0;
//? 游戏服务器未上传此属性	m_idRole			= 0;
    m_nAuthenID			= 0;
    m_szClientIP[0]		= 0;
    m_szServerName[0]	= 0;
    m_nFeeType			= 0;
    m_idFeeAccount		= ID_NONE;
    m_szFeeAccount[0]	= 0;
    m_tStartPoint		= 0;
    m_tLastPoint		= 0;
    m_tHeartbeat		= 0;
    m_tTimeWait			= 0;
    m_tEndPoint			= 0;
    m_nEndType			= c_endNone;
    m_nPointCount		= 0;
    m_dwSerial			= INVALID_SERIAL;
}

void COnlineAccount::Init(int nState,
                          OBJID		idAccount,
                          DWORD		nAuthenID,
                          LPCTSTR	pszLoginName,
                          LPCTSTR	pszPassword,
                          LPCTSTR	pszClientIP,
                          LPCTSTR	pszServerName,
                          int		nFeeType,
                          OBJID		idFeeAccount,
                          LPCTSTR	szFeeAccount)
{
    ASSERT(nState);
    ASSERT(idAccount);
// 目前数据库中有空串帐号	ASSERT(pszLoginName[0]);

    m_nState			= nState;

    m_idAccount			= idAccount;
    m_nAuthenID			= nAuthenID;
    SafeCopy(m_szLoginName, pszLoginName, _MAX_NAMESIZE);
    SafeCopy(m_szPassword, pszPassword, _MAX_NAMESIZE);

    SafeCopy(m_szClientIP, pszClientIP, IPSTRSIZE);
    SafeCopy(m_szServerName, pszServerName, _MAX_NAMESIZE);

    m_nFeeType			= nFeeType;
    m_idFeeAccount		= idFeeAccount;
    SafeCopy(m_szFeeAccount, szFeeAccount, _MAX_NAMESIZE);
    m_tStartPoint		= 0;
    m_tLastPoint		= 0;
    m_tEndPoint			= 0;

    m_nEndType			= c_endNone;
    m_nPointCount		= 0;
    m_tHeartbeat		= clock();
    m_tTimeWait			= 0;

    m_dwSerial			= INVALID_SERIAL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// COnlineTable
COnlineTable::COnlineTable(int nSize)
{
    LOCKTHREAD;

    m_pAccount = new COnlineAccount[nSize];

    m_nCount = 0;

//	for(int i = 0; i < MAXGAMESERVERS; i++)
//		m_nCountList[i] = 0;
}

COnlineTable::~COnlineTable()
{
    LOCKTHREAD;

    LOGMSG("在线表析购时剩余[%d]个元素", m_nCount);
#ifdef	TEST_INDEX
    ASSERT(m_idxID.Size() == m_nCount);
    ASSERT(m_idxName.Size() == m_nCount);
#endif
    for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)		// 1: 从1开始
    {
        if (m_pAccount[nIndex].m_nState)
        {
            m_pAccount[nIndex].m_tEndPoint	= time(NULL);
            LogFee(nIndex, COnlineAccount::c_endAccountCloseKick);
            Remove_0(nIndex);
        }
    }
#ifdef	TEST_INDEX
    ASSERT(m_idxID.Size() == m_nCount);
    ASSERT(m_idxName.Size() == m_nCount);
#endif

    LOGMSG("在线表清空后剩余[%d]个元素", m_nCount);

    if (m_nCount)
    {
        LOGWARNING("~COnlineTable() 异常，m_nCount不为0[%d]", m_nCount);
    }
#ifdef	TEST_INDEX
    if (m_idxName.Size())
    {
        LOGWARNING("idxName.Size() : %d", m_idxName.Size());
        for(IndexName_t::iterator j = m_idxName.Begin(); j != m_idxName.End(); j++)
        {
            LOGWARNING("---> name[%s], index[%d]", m_idxName.Key(j), m_idxName.Data(j));
        }
    }
    if (m_idxID.Size())
    {
        LOGWARNING("idxID.Size() : %d", m_idxID.Size());
        for(IndexID_t::iterator i = m_idxID.Begin(); i != m_idxID.End(); i++)
        {
            LOGWARNING("---> id[%d], index[%d]", m_idxID.Key(i), m_idxID.Data(i));
        }
    }
#endif

    m_aPlayerCount.ClearAll();
    if (STAT_SERIAL)
    {
        m_aSerialCount.ClearAll();
    }
    delete [] m_pAccount;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// COnlineTable
bool	COnlineTable::GetAttr(const char* szAccount, OBJID& idAccount, char* szPassword,
                              DWORD& nAuthenID, char* szServerName, char* szFeeAccount, OBJID& idFeeAccount, char* szOldIP)
{
    LOCKTHREAD;

    int nIndex = FindIndex(szAccount);		// return 0: ERROR
    if (nIndex && m_pAccount[nIndex].m_nState)
    {
        idAccount		= m_pAccount[nIndex].m_idAccount;
        SafeCopy(szPassword, m_pAccount[nIndex].m_szPassword, _MAX_NAMESIZE);
        nAuthenID		= m_pAccount[nIndex].m_nAuthenID;
        SafeCopy(szServerName, m_pAccount[nIndex].m_szServerName, _MAX_NAMESIZE);
        SafeCopy(szFeeAccount, m_pAccount[nIndex].m_szFeeAccount, _MAX_NAMESIZE);
        idFeeAccount	= m_pAccount[nIndex].m_idFeeAccount;
        SafeCopy(szOldIP, m_pAccount[nIndex].m_szClientIP, _MAX_NAMESIZE);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	COnlineTable::GetAttr2(OBJID idAccount, int& nFeeType, char* szServerName,
                               OBJID& idFeeAccount, char* szFeeAccount)
{
    LOCKTHREAD;

    int nIndex = FindIndex(idAccount);		// return 0: ERROR
    if (nIndex && m_pAccount[nIndex].m_nState)
    {
        nFeeType		= m_pAccount[nIndex].m_nFeeType;
        SafeCopy(szServerName, m_pAccount[nIndex].m_szServerName, _MAX_NAMESIZE);
        idFeeAccount	= m_pAccount[nIndex].m_idFeeAccount;
        SafeCopy(szFeeAccount, m_pAccount[nIndex].m_szFeeAccount, _MAX_NAMESIZE);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
#define		IDXASSERT(x,y,z)	{if (!(x)) LOGERROR("ASSERT(" #x ") -> [%s][%d] in %d line of %s", y, z, __LINE__, __FILE__); }
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	COnlineTable::AddNew(OBJID idAccount, DWORD nAuthenID, const char* pClientIP, const char* szLoginName
                             , const char* szPassword, int nPointType, const char* szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount)			// return 0: error
{
    LOCKTHREAD;

    int	nFreeIndex = FindFree(idAccount);		// return 0: 溢出或已有此帐号
    if (nFreeIndex)
    {
        m_nCount++;
        m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] + 1);
        m_pAccount[nFreeIndex].Init(COnlineAccount::c_stateOnlineNormal,
                                    idAccount,
                                    nAuthenID,
                                    szLoginName,
                                    szPassword,
                                    pClientIP,
                                    szServerName,
                                    nPointType,
                                    idFeeAccount,
                                    szFeeAccount);
#ifdef	TEST_INDEX
        int	nOldIDCount = m_idxID.Size();
        int	nOldNameCount = m_idxName.Size();
        IDXASSERT(!m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(!m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
        m_idxID.Add(idAccount, nFreeIndex);
        m_idxName.Add(szLoginName, nFreeIndex);
#ifdef	TEST_INDEX
        IDXASSERT(m_idxID.Size() == nOldIDCount + 1, szLoginName, idAccount);
        IDXASSERT(m_idxName.Size() == nOldNameCount + 1, szLoginName, idAccount);
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

        return true;
    }
    else
    {
        LOGWARNING("ADDNEW()时共享冲突，帐号[%s]可能会重复登录。改为REJOIN", szLoginName);

        // 复盖旧内容
        int		nIndex = FindIndex(szLoginName);
        ASSERT(nIndex == FindIndex(idAccount));
        ASSERT(nIndex);

        // 检查交叉登录
        if (strcmp(szServerName, m_pAccount[nIndex].m_szServerName) != 0
                || strcmp(pClientIP, m_pAccount[nIndex].m_szClientIP) != 0
                || strcmp(szFeeAccount, m_pAccount[nIndex].m_szFeeAccount) != 0)	// 上次是用网吧帐号或ISP上的
        {
            return false;
        }

        if (m_pAccount[nIndex].m_tStartPoint)
        {
            m_pAccount[nIndex].m_tHeartbeat = clock();
            m_pAccount[nIndex].m_tEndPoint	= time(NULL);
            LogFee(nIndex, COnlineAccount::c_endAccountRejoinKick);
        }

        ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
        ASSERT(strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0);
        m_pAccount[nIndex].Init(COnlineAccount::c_stateOnlineRejoin,
                                idAccount,
                                nAuthenID,
                                szLoginName,
                                szPassword,
                                pClientIP,
                                szServerName,
                                nPointType,
                                idFeeAccount,
                                szFeeAccount);
#ifdef	TEST_INDEX
        int	nOldIDCount = m_idxID.Size();
        int	nOldNameCount = m_idxName.Size();
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
        ASSERT(m_idxID.IsIn(idAccount));
        ASSERT(m_idxName.IsIn(szLoginName));
        m_idxID.Add(idAccount, nIndex);
        m_idxName.Add(szLoginName, nIndex);
#ifdef	TEST_INDEX
        IDXASSERT(m_idxID.Size() == nOldIDCount, szLoginName, idAccount);
        IDXASSERT(m_idxName.Size() == nOldNameCount, szLoginName, idAccount);
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

        return true;
    }
}

// 补填一个帐号
bool	COnlineTable::AppendNew(OBJID idAccount, DWORD nAuthenID, const char* pClientIP, const char* szLoginName
                                , const char* szPassword, int nPointType, const char* szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount)	// return 0: error
{
    LOCKTHREAD;

//改为PUBLIC	RemoveOvertime();	//????? 当没有新玩家登录时，永远不会检查超时。超时的目的只是回收在线表资源，延迟检测关系不大。
    int	nFreeIndex = FindFree(idAccount);		// return 0: 溢出或已有此帐号
    if (nFreeIndex)
    {
        m_nCount++;
        m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] + 1);
        m_pAccount[nFreeIndex].Init(COnlineAccount::c_stateOnlineRejoin,
                                    idAccount,
                                    nAuthenID,
                                    szLoginName,
                                    szPassword,
                                    pClientIP,
                                    szServerName,
                                    nPointType,
                                    idFeeAccount,
                                    szFeeAccount);
#ifdef	TEST_INDEX
        int	nOldIDCount = m_idxID.Size();
        int	nOldNameCount = m_idxName.Size();
        IDXASSERT(!m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(!m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
        m_idxID.Add(idAccount, nFreeIndex);
        m_idxName.Add(szLoginName, nFreeIndex);
#ifdef	TEST_INDEX
        IDXASSERT(m_idxID.Size() == nOldIDCount + 1, szLoginName, idAccount);
        IDXASSERT(m_idxName.Size() == nOldNameCount + 1, szLoginName, idAccount);
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

        return true;
    }
    else
    {
        LOGWARNING("APPENDNEW()时共享冲突。帐号[%s]改用REJOIN", szLoginName);

        // 复盖旧内容
        int		nIndex = FindIndex(szLoginName);
        ASSERT(nIndex == FindIndex(idAccount));
        ASSERT(nIndex);
        // 检查交叉登录
        if (strcmp(szServerName, m_pAccount[nIndex].m_szServerName) != 0
                || strcmp(pClientIP, m_pAccount[nIndex].m_szClientIP) != 0
                || strcmp(szFeeAccount, m_pAccount[nIndex].m_szFeeAccount) != 0)	// 上次是用网吧帐号或ISP上的
        {
            return false;
        }

        if (m_pAccount[nIndex].m_tStartPoint)
        {
            m_pAccount[nIndex].m_tHeartbeat = clock();
            m_pAccount[nIndex].m_tEndPoint	= time(NULL);
            LogFee(nIndex, COnlineAccount::c_endAccountRejoinKick);
        }

        ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
        ASSERT(strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0);
        m_pAccount[nIndex].Init(COnlineAccount::c_stateOnlineRejoin,
                                idAccount,
                                m_pAccount[nIndex].m_nAuthenID,
                                szLoginName,
                                m_pAccount[nIndex].m_szPassword,
                                m_pAccount[nIndex].m_szClientIP,
                                szServerName,
                                nPointType,
                                idFeeAccount,
                                szFeeAccount);
#ifdef	TEST_INDEX
        int	nOldIDCount = m_idxID.Size();
        int	nOldNameCount = m_idxName.Size();
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
        m_idxID.Add(idAccount, nIndex);
        m_idxName.Add(szLoginName, nIndex);
#ifdef	TEST_INDEX
        IDXASSERT(m_idxID.Size() == nOldIDCount, szLoginName, idAccount);
        IDXASSERT(m_idxName.Size() == nOldNameCount, szLoginName, idAccount);
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

        return true;
    }
}

bool	COnlineTable::Rejoin(OBJID idAccount, DWORD nAuthenID, const char* pClientIP, const char* szLoginName
                             , const char* szPassword, int nPointType, const char* szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount)			// return 0: error
{
    LOCKTHREAD;

    int		nIndex = FindIndex(szLoginName);
    ASSERT(nIndex == FindIndex(idAccount));
    if (nIndex)
    {
        // 检查交叉登录
        if (strcmp(szServerName, m_pAccount[nIndex].m_szServerName) != 0
                || strcmp(pClientIP, m_pAccount[nIndex].m_szClientIP) != 0
                || strcmp(szFeeAccount, m_pAccount[nIndex].m_szFeeAccount) != 0)	// 上次是用网吧帐号或ISP上的
        {
            return false;
        }

        if (m_pAccount[nIndex].m_tStartPoint)
        {
            m_pAccount[nIndex].m_tHeartbeat = clock();
            m_pAccount[nIndex].m_tEndPoint	= time(NULL);
            LogFee(nIndex, COnlineAccount::c_endAccountRejoinKick);
        }

        ASSERT(strcmp(m_pAccount[nIndex].m_szPassword, szPassword) == 0);

        ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
        ASSERT(strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0);
        m_pAccount[nIndex].Init(COnlineAccount::c_stateOnlineRejoin,
                                idAccount,
                                nAuthenID,
                                szLoginName,
                                szPassword,
                                pClientIP,
                                szServerName,
                                nPointType,
                                idFeeAccount,
                                szFeeAccount);
#ifdef	TEST_INDEX
        int	nOldIDCount = m_idxID.Size();
        int	nOldNameCount = m_idxName.Size();
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
        m_idxID.Add(idAccount, nIndex);
        m_idxName.Add(szLoginName, nIndex);
#ifdef	TEST_INDEX
        IDXASSERT(m_idxID.Size() == nOldIDCount, szLoginName, idAccount);
        IDXASSERT(m_idxName.Size() == nOldNameCount, szLoginName, idAccount);
        IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
        IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

        return true;
    }
    else
    {
        LOGWARNING("REJOIN()时共享冲突。玩家[%s]改为ADDNEW", szLoginName);

        int	nFreeIndex = FindFree(idAccount);		// return 0: 溢出或已有此帐号
        if (nFreeIndex)
        {
            m_nCount++;
            m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] + 1);
            m_pAccount[nFreeIndex].Init(COnlineAccount::c_stateOnlineNormal,
                                        idAccount,
                                        nAuthenID,
                                        szLoginName,
                                        szPassword,
                                        pClientIP,
                                        szServerName,
                                        nPointType,
                                        idFeeAccount,
                                        szFeeAccount);
#ifdef	TEST_INDEX
            int	nOldIDCount = m_idxID.Size();
            int	nOldNameCount = m_idxName.Size();
            IDXASSERT(!m_idxID.IsIn(idAccount), szLoginName, idAccount);
            IDXASSERT(!m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
            m_idxID.Add(idAccount, nFreeIndex);
            m_idxName.Add(szLoginName, nFreeIndex);
#ifdef	TEST_INDEX
            IDXASSERT(m_idxID.Size() == nOldIDCount + 1, szLoginName, idAccount);
            IDXASSERT(m_idxName.Size() == nOldNameCount + 1, szLoginName, idAccount);
            IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
            IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

            return true;
        }
        else
        {
            LOGERROR("Rejoin()内部异常。在线表数量[%d/%d]", GetPlayerCount(), ONLINETABLESIZE);
            return false;
        }
    }
}

//?? 隐患：调用前必须在线。计点线程改为多线程时有BUG
int  COnlineTable::StartFee(OBJID idAccount, const char* pszServerName)		// return -n: 服务器不匹配
{
    LOCKTHREAD;

    int nIndex = FindIndex(idAccount);		// return 0: ERROR
    ASSERT(nIndex);
    if (!nIndex)
    {
        return -1;
    }

    // 检查双重登录
    ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
    if (strcmp(m_pAccount[nIndex].m_szServerName, pszServerName))
    {
        LOGERROR("错误的STARTFEE。玩家[%s][%d]登录了[%s]，但接收到[%s]的STARTFEE消息",
                 m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount,
                 m_pAccount[nIndex].m_szServerName, pszServerName);
        return -1;
    }

    ASSERT(m_pAccount[nIndex].m_nState != COnlineAccount::c_stateOnlineNone);
    m_pAccount[nIndex].m_tHeartbeat = clock();
    if (m_pAccount[nIndex].m_tStartPoint)
    {
#ifdef	DUMP
        LOGERROR("重复接收到MSG_STARTFEE消息。游戏服务器为[%s], 玩家帐号为[%s]",
                 m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName);
#endif
    }
    else
    {
        if (m_pAccount[nIndex].m_nPointCount)
        {
            LOGERROR("程序异常。[%s]玩家接收到MSG_STARTFEE消息时在线表中的点数未清空[%d]。",
                     m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_nPointCount);
            m_pAccount[nIndex].m_nPointCount = 0;
        }
        m_pAccount[nIndex].m_tStartPoint	= time(NULL);
    }

    return 0;
}

//?? 隐患：调用前必须在线。计点线程改为多线程时有BUG
int  COnlineTable::EndFee(OBJID idAccount, const char* pszServerName, bool bOffline)		// return -n: 服务器不匹配
{
    LOCKTHREAD;

    int nIndex = FindIndex(idAccount);
    if (!nIndex)
    {
        if (g_bEnableLogin && !bOffline)
        {
            LOGWARNING("多余的ENDFEE, 游戏服务器[%s]帐号ID[%d]", pszServerName, idAccount);
        }
        return -1;
    }

    // 过滤掉非记录服务器的END_FEE
    ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
    if (strcmp(pszServerName, m_pAccount[nIndex].m_szServerName))
    {
        if (!bOffline)
            LOGERROR("错误的END_FEE，忽略。玩家[%s][%d]在[%s]，但接收到[%s]的消息。",
                     m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount,
                     m_pAccount[nIndex].m_szServerName, pszServerName);
        return -1;
    }

//不能清空，由下面清	m_pAccount[nIndex].m_nState			= COnlineAccount::c_stateOnlineNone;	// 冗余
    m_pAccount[nIndex].m_tHeartbeat = clock();
    m_pAccount[nIndex].m_tEndPoint		= time(NULL);

    if (m_pAccount[nIndex].m_tStartPoint)
    {
        if (bOffline)
        {
            LogFee(nIndex, COnlineAccount::c_endGameServerLose);
        }
        else if (m_pAccount[nIndex].m_nState == COnlineAccount::c_stateOnlineNormal)
        {
            LogFee(nIndex, COnlineAccount::c_endNormal);
        }
        else if (m_pAccount[nIndex].m_nState == COnlineAccount::c_stateOnlineRejoin)
        {
            LogFee(nIndex, COnlineAccount::c_endRejoin);
        }
        else
        {
            LOGERROR("COnlineTable::EndFee(OBJID idAccount) 内部异常");
        }
    }
    else
    {
        LogFee(nIndex, COnlineAccount::c_endGameServerKick);
    }

    int nCount = (m_pAccount[nIndex].m_tEndPoint - m_pAccount[nIndex].m_tStartPoint) / (POINTFEEINTERVALSECS * 1000);
    if (m_pAccount[nIndex].m_tStartPoint && nCount - m_pAccount[nIndex].m_nPointCount > 0)		//? 3: 误差率
    {
        char szStartTime[20];
        char szPointTime[20];
        char szEndTime[20];
        char szStamp[15];
        DateTime(szStartTime, m_pAccount[nIndex].m_tStartPoint);
        DateTime(szPointTime, m_pAccount[nIndex].m_tLastPoint);
        DateTime(szEndTime, m_pAccount[nIndex].m_tEndPoint);
        time_t	tCurr = time(NULL);
        tm*	pTm = localtime(&tCurr);
        sprintf(szStamp, "%04d%02d%02d%02d%02d%02d", pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
        ASSERT(c_typeAll == 7);
        ASSERT(COnlineAccount::c_endAll == 8);
        const char* 	aFeeType[] = { "未知", "计点", "月卡", "网吧计点", "网吧月卡", "ISP", "免费", "Error", "Error"  };
        const char* 	aEndType[] = { "未知", "正常退出", "延时退出", "游戏服务器踢", "重复登录",
                                       "超时退出", "帐号服务器关闭", "登录信息丢失", "Error", "Error"
                                  };
        const char* 	pFeeType = aFeeType[0];
        const char* 	pEndType = aEndType[0];
        if (m_pAccount[nIndex].m_nFeeType >= c_typeNone && m_pAccount[nIndex].m_nFeeType < c_typeAll)
        {
            pFeeType = aFeeType[m_pAccount[nIndex].m_nFeeType];
        }
        if (m_pAccount[nIndex].m_nEndType > COnlineAccount::c_endNone && m_pAccount[nIndex].m_nEndType < COnlineAccount::c_endAll)
        {
            pEndType = aEndType[m_pAccount[nIndex].m_nEndType];
        }
        LOGERROR("计费误差过大：[%s]上传[%d]的结束计费消息\n"
                 "开始时间[%s]，最后计点[%s]，结束时间[%s]，\n"
                 "计费类型[%s], 计点总数[%d], 结束类型[%s]",
                 m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_idAccount,
                 szStartTime, szPointTime, szEndTime,
                 pFeeType, m_pAccount[nIndex].m_nPointCount, pEndType);
    }

    // 同时删除成员
    ASSERT(m_pAccount[nIndex].m_nState);
    if (m_pAccount[nIndex].m_nState == COnlineAccount::c_stateOnlineNormal || bOffline || REJOINTIMEWAITSECS == 0)
    {
        Remove_0(nIndex);
    }
    else
    {
        m_pAccount[nIndex].m_tTimeWait = clock() + REJOINTIMEWAITSECS * CLOCKS_PER_SEC;
    }

    return 0;
}

//?? 隐患：调用前必须在线。计点线程改为多线程时有BUG
int COnlineTable::PointFee(OBJID idAccount, const char* pszServerName)		// return -n: 服务器不匹配
{
    LOCKTHREAD;

    int nIndex = FindIndex(idAccount);
    if (!nIndex)
    {
        LOGERROR("COnlineTable::PointFee(OBJID idAccount) 内部异常");
        return	-1;		// 没有该玩家的在线表(内部异常)
    }

    // 检查错误的POINT_FEE
    ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
    if (strcmp(m_pAccount[nIndex].m_szServerName, pszServerName))
    {
        LOGERROR("错误的POINT_FEE。玩家[%s][%d]登录了[%s]，但接收到[%s]的消息。IP地址[%s]",
                 m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount,
                 m_pAccount[nIndex].m_szServerName, pszServerName,
                 m_pAccount[nIndex].m_szClientIP);
        return -1;		// -1: 错误
    }

    ASSERT(m_pAccount[nIndex].m_nState != COnlineAccount::c_stateOnlineNone);
    m_pAccount[nIndex].m_tHeartbeat = clock();
    if (!m_pAccount[nIndex].m_tStartPoint)
    {
#ifdef	DUMP
        LOGWARNING("接收到POINTFEE消息之前没有收到STARTFEE消息，帐号ID[%d]。可能是补填的帐号", m_pAccount[nIndex].m_idAccount);
#endif
        m_pAccount[nIndex].m_tStartPoint	= time(NULL);
    }
    m_pAccount[nIndex].m_tLastPoint	= time(NULL);
    m_pAccount[nIndex].m_nPointCount++;
//	LOGPOINT("玩家[%d]的本次计点数已累计到[%d]", m_pAccount[nIndex].m_idAccount, m_pAccount[nIndex].m_nPointCount);

    int nRet = m_pAccount[nIndex].m_nPointCount;
    if (nRet > 0)
    {
        return nRet;
    }
    else
    {
        return 0;
    }
}

// 检查并删除所有超时的在线表
bool	COnlineTable::RemoveOvertime()
{
    LOCKTHREAD;

    bool	ret = false;
    for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)	// 1: 0号单元保留
    {
        // 检查TIME_WAIT
        if (m_pAccount[nIndex].m_nState && m_pAccount[nIndex].m_tTimeWait && clock() > m_pAccount[nIndex].m_tTimeWait)
        {
            Remove_0(nIndex);
            ret = true;
        }

        // 检查超时
        if (m_pAccount[nIndex].m_nState
                && clock() > m_pAccount[nIndex].m_tHeartbeat + ONLINEHEARTBEATSECS * CLOCKS_PER_SEC)
        {
            m_pAccount[nIndex].m_tEndPoint	= time(NULL);
            LogFee(nIndex, COnlineAccount::c_endAccountOvertimeKick);
            Remove_0(nIndex);
            ret = true;
        }
    }
    return ret;
}

bool	COnlineTable::GetServerName(OBJID idAccount, char* bufServer)
{
    LOCKTHREAD;

    int		nIndex = FindIndex(idAccount);
    if (nIndex)
    {
        SafeCopy(bufServer, m_pAccount[nIndex].m_szServerName, _MAX_NAMESIZE);
        return true;
    }
    else
    {
        return false;
    }
}

//? 可优化
int	COnlineTable::GetFeeCount(OBJID idFeeAccount)
{
    LOCKTHREAD;

    int 	nCount = 0;
    for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)	// 1: 0号单元保留
    {
        if (m_pAccount[nIndex].m_nState && m_pAccount[nIndex].m_idFeeAccount == idFeeAccount)
        {
            nCount++;
        }
    }
    return nCount;
}

bool COnlineTable::CheckUniqueIP(LPCTSTR pClientIP, OBJID idAccount)
{
    LOCKTHREAD;

    ASSERT(!( pClientIP == 0 || *pClientIP == 0 ));
    if (pClientIP == 0 || *pClientIP == 0)
    {
        return false;
    }

    for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)	// 1: 0号单元保留
    {
        if (m_pAccount[nIndex].m_nFeeType == c_typeISP
                && strcmp(m_pAccount[nIndex].m_szClientIP, pClientIP) == 0
                && m_pAccount[nIndex].m_idAccount != idAccount)
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// private

//?? 该函数需要优化
int		COnlineTable::FindIndex(const char* szLoginName)		// return 0: ERROR
{
// 内部函数	LOCKTHREAD;

#ifdef	INDEXOPTIMIZE
    int	nIndex = m_idxName[szLoginName];
    if (!nIndex || strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0)
    {
        return nIndex;
    }
    LOGERROR("在线表索引不同步，帐号名[%s]的索引为[%d]，但索引[%d]的帐号名为[%s]。索引被删除。★",
             szLoginName, nIndex, nIndex, m_pAccount[nIndex].m_szLoginName);
    m_idxName.Del(szLoginName);
    return 0;
#endif

    for(int i = 1; i < ONLINETABLESIZE; i++)	// 1: 0号单元保留
    {
        if (m_pAccount[i].m_nState && strcmp(m_pAccount[i].m_szLoginName, szLoginName) == 0)
        {
            if (i != m_idxName[szLoginName])
            {
                LOGERROR("TEST: 登录名[%s]索引不同步[%d][%d]，内部异常", szLoginName, i, m_idxName[szLoginName]);
            }

            return i;
        }
    }

    if (m_idxName[szLoginName])
    {
        LOGERROR("TEST: 登录名[%s]索引不同步[%d]，内部异常.", szLoginName, m_idxName[szLoginName]);
    }

    return 0;
}

//?? 该函数需要优化
int		COnlineTable::FindIndex(OBJID idAccount)		// return 0: ERROR
{
// 内部函数	LOCKTHREAD;

#ifdef	INDEXOPTIMIZE
    int	nIndex = m_idxID[idAccount];
    if (!nIndex || m_pAccount[nIndex].m_idAccount == idAccount)
    {
        return nIndex;
    }
    LOGERROR("在线表索引不同步，帐号ID[%d]的索引为[%d]，但索引[%d]的帐号ID为[%d]。索引被删除。★",
             idAccount, nIndex, nIndex, m_pAccount[nIndex].m_idAccount);
    m_idxID.Del(idAccount);
    return 0;
#endif

    for(int i = 1; i < ONLINETABLESIZE; i++)	// 1: 0号单元保留
    {
        if (m_pAccount[i].m_nState && m_pAccount[i].m_idAccount == idAccount)
        {
            if (i != m_idxID[idAccount])
            {
                LOGERROR("TEST: 帐号ID[%d]索引不同步[%d][%d]，内部异常", idAccount, i, m_idxID[idAccount]);
            }

            return i;
        }
    }

    if (m_idxID[idAccount])
    {
        LOGERROR("TEST: 帐号ID[%d]索引不同步[%d]，内部异常.", idAccount, m_idxID[idAccount]);
    }

    return 0;
}

//?? 该函数需要优化
int		COnlineTable::FindFree(OBJID idAccount)		// return 0: 溢出或已有此帐号
{
// 内部函数	LOCKTHREAD;

    int	nFreeIndex = 0;
    for(int i = 1; i < ONLINETABLESIZE; i++)	// 1: 0号单元保留
    {
        if (m_pAccount[i].m_nState && m_pAccount[i].m_idAccount == idAccount)
        {
            LOGERROR("企图重复添加在线玩家表[%d]，共享冲突。", idAccount);
            return 0;
        }
        else if (!m_pAccount[i].m_nState && !nFreeIndex)
        {
            nFreeIndex = i;
//??? 不返回，继续检查重复添加			break;
        }
    }

    if (nFreeIndex)
    {
        return nFreeIndex;
    }
    else
    {
        LOGERROR("在线玩家表溢出[%d]，请修改config.ini，并重新启动程序。", m_nCount);
        return 0;
    }
}

// 私有的
void	COnlineTable::Remove_0(int nIndex)
{
    if (nIndex && m_pAccount[nIndex].m_nState)
    {
        LPCTSTR	szServerName = m_pAccount[nIndex].m_szServerName;
        ASSERT(m_aPlayerCount.IsIn(szServerName));
        if (m_aPlayerCount[szServerName] > 0)
        {
            m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] - 1);
        }
        if (m_nCount > 0)
        {
            m_nCount--;
        }
        if (STAT_SERIAL)
        {
            DWORD dwSerial = m_pAccount[nIndex].m_dwSerial;
            if (dwSerial != INVALID_SERIAL && m_aSerialCount[dwSerial] > 0)
            {
                m_aSerialCount.Add(dwSerial, m_aSerialCount[dwSerial] - 1);
            }
        }

#ifdef	TEST_INDEX
        int	nOldIDCount = m_idxID.Size();
        int	nOldNameCount = m_idxName.Size();
        IDXASSERT(m_idxID.IsIn(m_pAccount[nIndex].m_idAccount), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
        IDXASSERT(m_idxName.IsIn(m_pAccount[nIndex].m_szLoginName), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
#endif
        m_idxID.Del(m_pAccount[nIndex].m_idAccount);
        m_idxName.Del(m_pAccount[nIndex].m_szLoginName);
#ifdef	TEST_INDEX
        IDXASSERT(m_idxID.Size() == nOldIDCount - 1, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
        IDXASSERT(m_idxName.Size() == nOldNameCount - 1, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
        IDXASSERT(!m_idxID.IsIn(m_pAccount[nIndex].m_idAccount), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
        IDXASSERT(!m_idxName.IsIn(m_pAccount[nIndex].m_szLoginName), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
#endif
        m_pAccount[nIndex].Init();
    }
    else
    {
        LOGERROR("Remove_0()内部异常，Index[%d], State[%d]", nIndex, m_pAccount[nIndex].m_nState);
    }
}

// 私有的
void	COnlineTable::LogFee(int nIndex, enum COnlineAccount::EEndType nEndType)
{
    m_pAccount[nIndex].m_nEndType	= nEndType;
    char szStartTime[20];
    char szPointTime[20];
    char szEndTime[20];
    char szStamp[255];
    DateTime(szStartTime, m_pAccount[nIndex].m_tStartPoint);
    DateTime(szPointTime, m_pAccount[nIndex].m_tLastPoint);
    DateTime(szEndTime, m_pAccount[nIndex].m_tEndPoint);
    time_t	tCurr = time(NULL);
    tm*	pTm = localtime(&tCurr);
    sprintf(szStamp, "%04d%02d%02d%02d%02d%02d", pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
    ASSERT(c_typeAll == 7);
    ASSERT(COnlineAccount::c_endAll == 8);
    const char* 	aFeeType[] = { "未知", "计点", "月卡", "网吧计点", "网吧月卡", "ISP", "免费", "Error", "Error" };
    const char* 	aEndType[] = { "未知", "正常退出", "延时退出", "游戏服务器踢", "重复登录",
                                   "超时退出", "帐号服务器关闭", "登录信息丢失", "Error", "Error"
                              };
    const char* 	pFeeType = aFeeType[0];
    const char* 	pEndType = aEndType[0];
    if (m_pAccount[nIndex].m_nFeeType >= c_typeNone && m_pAccount[nIndex].m_nFeeType < c_typeAll)
    {
        pFeeType = aFeeType[m_pAccount[nIndex].m_nFeeType];
    }
    if (m_pAccount[nIndex].m_nEndType > COnlineAccount::c_endNone && m_pAccount[nIndex].m_nEndType < COnlineAccount::c_endAll)
    {
        pEndType = aEndType[m_pAccount[nIndex].m_nEndType];
    }
    if (m_pAccount[nIndex].m_nPointCount)
    {
        LOGPOINT("[%s]的[%s][%d]退出。开始时间[%s]，最后计点[%s]，结束时间[%s]，"
                 "计费帐号[%s][%d], 计费类型[%s], 计点总数[%d], 结束类型[%s], IP地址[%s]\n"
                 "INSERT DELAYED fee VALUES ('%s','%s','%s',   %d,'%s','%s','%s',   '%s',%d,%d,   %d,%d,'%s');"
                 , m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount
                 , szStartTime, szPointTime, szEndTime
                 , m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount
                 , pFeeType, m_pAccount[nIndex].m_nPointCount, pEndType, m_pAccount[nIndex].m_szClientIP
                 , szStamp, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName
                 , m_pAccount[nIndex].m_idAccount, szStartTime, szPointTime, szEndTime
                 , m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount, m_pAccount[nIndex].m_nFeeType
                 , m_pAccount[nIndex].m_nPointCount, m_pAccount[nIndex].m_nEndType, m_pAccount[nIndex].m_szClientIP
                );
        if (m_pAccount[nIndex].m_nFeeType == c_typeISP)
        {
            LOGISPPOINT(m_pAccount[nIndex].m_szFeeAccount, "[%s]的[%s][%d]退出。开始时间[%s]，最后计点[%s]，结束时间[%s]，"
                        "计费帐号[%s][%d], 计费类型[%s], 计点总数[%d], 结束类型[%s], IP地址[%s]\n"
                        "INSERT DELAYED fee VALUES ('%s','%s','%s',   %d,'%s','%s','%s',   '%s',%d,%d,   %d,%d,'%s');"
                        , m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount
                        , szStartTime, szPointTime, szEndTime
                        , m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount
                        , pFeeType, m_pAccount[nIndex].m_nPointCount, pEndType, m_pAccount[nIndex].m_szClientIP
                        , szStamp, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName
                        , m_pAccount[nIndex].m_idAccount, szStartTime, szPointTime, szEndTime
                        , m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount, m_pAccount[nIndex].m_nFeeType
                        , m_pAccount[nIndex].m_nPointCount, m_pAccount[nIndex].m_nEndType, m_pAccount[nIndex].m_szClientIP
                       );
        }
    }
    else
    {
        LOGPOINT("[%s]的[%s]退出。帐号ID[%d], 开始时间[%s]，最后计点[%s]，结束时间[%s]，"
                 "计费帐号[%s], 计费类型[%s], 计点总数[%d], 结束类型[%s], IP地址[%s]"
                 , m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount
                 , szStartTime, szPointTime, szEndTime
                 , m_pAccount[nIndex].m_szFeeAccount, pFeeType, m_pAccount[nIndex].m_nPointCount
                 , pEndType, m_pAccount[nIndex].m_szClientIP
                );
    }

    char szSQL[1024];
    sprintf(szSQL, "INSERT DELAYED fee VALUES ('%s','%s','%s',   %d,'%s','%s','%s',   '%s',%d,%d,   %d,%d,'%s');"
            , szStamp, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName
            , m_pAccount[nIndex].m_idAccount, szStartTime, szPointTime, szEndTime
            , m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount, m_pAccount[nIndex].m_nFeeType
            , m_pAccount[nIndex].m_nPointCount, m_pAccount[nIndex].m_nEndType, m_pAccount[nIndex].m_szClientIP
           );
    g_db.ExecuteSQL(szSQL);
}




