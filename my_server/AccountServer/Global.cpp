
//**********************************************************
// 代码编辑器
//**********************************************************

// 全局变量
// 仙剑修，2001.11.19


#include "AllHeads.h"

HANDLE	g_xDatabase;

/////////////////////////////////////////////////////////////////////////////
// 用于统计

long	s_nSocketCount = 0;
long	s_nLoginCount = 0;
long	s_nMaxServerTime = 0;
long	s_nAvgServerTime = 0;
long	s_nPointCount = 0;
long	s_nLoginAccept = 0;
long	s_nPointFee = 0;
long	s_nPointSum = 0;
long	s_nAllTimeSum = 0;
long	s_nDatabaseTimeSum = 0;

CCriticalSection	s_xCtrl;
char	s_bufText[4096] = "";
char	s_bufState[MAXGAMESERVERS] = "";
char	s_bufStateBackup[MAXGAMESERVERS] = "";
void	PrintText(const char* szFormat, ...)
{
    CSingleLock xLock(&s_xCtrl, true);
    ASSERT(xLock.IsLocked());

    va_list argptr;
    va_start( argptr, szFormat);     /* Initialize variable arguments. */

    char	buf[4096] = "";					/* ?????空间可能不足 */
    int		ret = vsprintf(buf, szFormat, argptr);
    ASSERT(ret < 4096 - 2);						/* ?????空间可能不足 */

    va_end( argptr );              /* Reset variable arguments.      */

    char* 	pNew = strchr(s_bufText, '\n');
    if (pNew)
    {
        static int	nCount = 0;
        if (nCount < 11)
        {
            nCount++;
        }
        else
        {
            strcpy(s_bufText, pNew + 1);
        }
    }

    strcat(s_bufText, "【");
    char szCurrTime[20];
    DateTime(szCurrTime, time(NULL));
    if (strlen(szCurrTime) >= 8)
    {
        strcat(s_bufText, szCurrTime);    //  + 8过滤掉年份月份
    }
    strcat(s_bufText, ": ");
    strcat(s_bufText, buf);
    strcat(s_bufText, "】\r\n");
}

bool	LockedGetText(char* buf)
{
    CSingleLock xLock(&s_xCtrl, true);
    ASSERT(xLock.IsLocked());

    if (strlen(s_bufText))
    {
        strcpy(buf, s_bufText);
//		s_bufText[0] = 0;
        return true;
    }
    else
    {
        return false;
    }
}

void	SetServerState(int nIndex, int nState)
{
    CSingleLock xLock(&s_xCtrl, true);
    ASSERT(xLock.IsLocked());

    if (nIndex >= 0 && nIndex < MAXGAMESERVERS)
    {
        s_bufState[nIndex] = nState;
    }

    if (nIndex >= 0 && nIndex < MAXGAMESERVERS && nState <= c_flagBaseMax)
    {
        s_bufStateBackup[nIndex] = nState;
    }
}

void	GetServerState(char* buf)
{
    CSingleLock xLock(&s_xCtrl, true);
    ASSERT(xLock.IsLocked());

    //首次初始化
    if (s_bufState[0] == 0)
    {
        memset(s_bufState, c_flagNone, MAXGAMESERVERS);
        memset(s_bufStateBackup, c_flagNone, MAXGAMESERVERS);
    }

    memcpy(buf, s_bufState, MAXGAMESERVERS);
    memcpy(s_bufState, s_bufStateBackup, MAXGAMESERVERS);
}

/////////////////////////////////////////////////////////////////////////////

time_t		g_tStartServerTime = 0;

/////////////////////////////////////////////////////////////////////////////
// 全局对象

CPointThread* 	g_pPointThread = NULL;		//?? 必须先定义(POINTLISTENPORT);
CLoginThread* 	g_pLoginThread = NULL;		//?? 要调用上一个类(LOGINLISTENPORT);
COnlineTable* 	g_pOnlineTable = NULL;		// 在线玩家表
CAccount		g_cDatabase;
bool			g_bEnableLogin = false;		//?? 可改为SHARE共享变量

CServerAccount	g_aServerAccount[MAXGAMESERVERS];	// 游戏服务器列表
int				g_nServerAccount;					// 游戏服务器数量
char			g_szCountHeadLine[4096];			// 游戏服务器列表的标题行
ISP_SET			g_setISPList;						// IPS的IP地址列表


//////////////////////////////////////////////////////////////////
// 全局函数

DWORD	NewAuthenID(unsigned long nClientIP)
{
    // 生成认证ID号
    srand(clock() * nClientIP);
    return (OBJID)((rand() + 1) * (rand() + 1));
}

// return false: 非法。或源串超长(成功拷贝部分串)
bool	SafeCopy(char* pTarget, const char* pSource, int nBufLen /*= 0*/)
{
    try
    {
        ASSERT(pTarget);
        if (pTarget)
        {
            pTarget[0] = 0;
            ASSERT(pSource);
            if (pSource)
            {
                if (nBufLen && strlen(pSource) >= nBufLen)
                {
                    strncpy(pTarget, pSource, nBufLen - 1);
                    pTarget[nBufLen-1] = 0;
                    return false;
                }
                else
                {
                    strcpy(pTarget, pSource);
                    return true;
                }
            }
        }
    }
    catch(...)
    {
        LOGCATCH("SafeCopy(). invalid access.");
    }

    return false;
}

// 添尾0，删尾空
bool	FormatStr(char* pszBuf, int nBufLen /*= 0*/)
{
    ASSERT(pszBuf);

    try
    {
        if (nBufLen)
        {
            pszBuf[nBufLen-1] = 0;
        }

        // 删除尾空格
        char* ptr = pszBuf + strlen(pszBuf) - 1;
        while(ptr >= pszBuf && *ptr == ' ')
        {
            *(ptr--) = 0;
        }
    }
    catch(...)
    {
        LOGCATCH("FormatStr(). invalid access.");
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// szPassword="" 表示不检查密码
bool	GetAccount(LPCTSTR szAccount, LPCTSTR szPassword,
                   OBJID& idAccount, int& nType, int& nPoint, DWORD& nPointTime, int& nBlock)			// 返回值
{
    idAccount	= ID_NONE;
    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
    if (g_cDatabase.Create(szAccount, szPassword))
    {
        // VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
        idAccount		= g_cDatabase.GetID();
        nBlock			= g_cDatabase.GetOnline();			// 查ONLINE
        nPoint			= g_cDatabase.GetPoint();
        nType			= g_cDatabase.GetType();
        nPointTime		= g_cDatabase.GetPointTime();
        g_cDatabase.Destroy();
        // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    }
    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
    return	(idAccount != ID_NONE);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	IsFreeServer(LPCTSTR szServer)
{
    for(int i = 0; i < g_nServerAccount; i++)
    {
        if (g_aServerAccount[i].m_bFree && strcmp(g_aServerAccount[i].m_szServerName, szServer) == 0)
        {
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CheckISP(OBJID idAccount, unsigned long nClientIP, LPCTSTR pClientIP,
                 OBJID& idFeeAccount, char* szFeeAccount, int& nPoint, DWORD& nPointTime, char* szNotifyMsg)	// 返回值
{
    bool	bIsISP = false;

    unsigned char ip1, ip2, ip3, ip4;
    ip1 = *( ((unsigned char*)&nClientIP) + 0);
    ip2 = *( ((unsigned char*)&nClientIP) + 1);
    ip3 = *( ((unsigned char*)&nClientIP) + 2);
    ip4 = *( ((unsigned char*)&nClientIP) + 3);

    for(int i = 0; i < g_setISPList.size(); i++)
    {
        int	fi1, fi2, fi3, fi4;
        if (sscanf(g_setISPList[i].szFirstIP, "%d.%d.%d.%d", &fi1, &fi2, &fi3, &fi4) != 4)
        {
            LOGERROR("invalud first ip format [%s]", g_setISPList[i].szFirstIP);
            continue;
        }

        int	la1, la2, la3, la4;
        if (sscanf(g_setISPList[i].szLastIP, "%d.%d.%d.%d", &la1, &la2, &la3, &la4) != 4)
        {
            LOGERROR("invalud last ip format [%s]", g_setISPList[i].szLastIP);
            continue;
        }

        if (ip1 >= fi1 && ip1 <= la1
                && ip2 >= fi2 && ip2 <= la2
                && ip3 >= fi3 && ip3 <= la3
                && ip4 >= fi4 && ip4 <= la4 )
        {
            if (pClientIP && g_pOnlineTable->CheckUniqueIP(pClientIP, idAccount))
            {
                ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                if (g_cDatabase.Create(g_setISPList[i].szAccount, g_setISPList[i].szPassword))
                {
                    // VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    if (g_cDatabase.GetPoint() > 0)
                    {
                        SafeCopy(szFeeAccount, g_cDatabase.GetName(), _MAX_NAMESIZE);
                        idFeeAccount	= g_cDatabase.GetID();
                        nPoint			= g_cDatabase.GetPoint();
                        bIsISP	= true;
                        nPointTime		= g_cDatabase.GetPointTime();
                    }
                    else
                    {
                        // ISP转普通帐号
                        sprintf(szNotifyMsg, FLAG_ISP_TO_NORMAL);
                    }
                    g_cDatabase.Destroy();
                    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                }
                ::ReleaseMutex(g_xDatabase);	//------------------------------------------
            }
            else
            {
                // ISP转普通帐号
                sprintf(szNotifyMsg, FLAG_ISP_TO_NORMAL);
            }

            return bIsISP;	// found it!
        }
    } // for

    if (pClientIP)
    {
        ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
        if (g_cDatabase.Create(c_typeISP, pClientIP))
        {
            // VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
            SafeCopy(szFeeAccount, g_cDatabase.GetName(), _MAX_NAMESIZE);
            idFeeAccount	= g_cDatabase.GetID();
            nPoint			= g_cDatabase.GetPoint();
            bIsISP	= true;
            nPointTime		= g_cDatabase.GetPointTime();
            g_cDatabase.Destroy();
            // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        }
        ::ReleaseMutex(g_xDatabase);	//------------------------------------------
    }

    return bIsISP;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	Check7DaysNodify(DWORD nPointTime, char* szNotifyMsg)		// return szNotifyMsg
{
    time_t	tCurr = time(NULL);
    struct tm	tmCurr;
    tmCurr = *localtime(&tCurr);
    tmCurr.tm_mday	+= ACCOUNT_AHEAD_DAYS;
    time_t	tAhead	= mktime(&tmCurr);
    if (tAhead != (time_t) - 1)
    {
        tm*		pTm	= localtime(&tAhead);
        int		nAhead	= (pTm->tm_year + 1900) * 10000 + (pTm->tm_mon + 1) * 100 + pTm->tm_mday;
        if (nAhead > nPointTime)
        {
            nPointTime %= 100000000;	// 防止szNotifyMsg溢出
            sprintf(szNotifyMsg, "%d年%d月%d日", 			// ★注意：第一字节必须为数字。以便游戏服务器同步。
                    (nPointTime / 10000) % 10000, (nPointTime / 100) % 100, nPointTime % 100);
            return true;
        }
    }
    return false;
}

