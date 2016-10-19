
//**********************************************************
// 代码编辑器
//**********************************************************

// 登录线程类
// 仙剑修，2001.11.20

//////////////////////////////////////////////////////////////////////
#include "AllHeads.h"
#include "LoginThread.h"
#include "MsgAccount.h"
#include "MsgConnect.h"
#include "MsgLogin.h"
#include "MsgQueryFee.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
#undef	LOCKTHREAD		// 登录线程不需要互斥★
#define	LOCKTHREAD
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoginThread::CLoginThread(u_short nPort)
    : CThreadBase(), m_cListenSocket(nPort)
{
    LOCKTHREAD;

    m_pBanIPs = new CBanIP[MAXBANIPS];
}

CLoginThread::~CLoginThread()
{
    LOCKTHREAD;

    delete m_pBanIPs;
}

//#define	CRITSECT	//	CSingleLock(&m_xCtrl, true);

//#define	LOCK	//?	{CSingleLock(&m_xCtrl, true);
//#define	UNLOCK	//?	}

// 该线程类没有外部消息进入，没有共享冲突
void	CLoginThread::OnInit()
{
    LOCKTHREAD;

    try
    {
        m_cRc5.Rc5InitKey(RC5PASSWORD_KEY);
        m_cRc5NetBar.Rc5InitKey(RC5BARPASSWORD_KEY);

        m_cListenSocket.Open();
        LOGMSG("登录线程正常启动");

    }
    catch(...) { LOGCATCH("登录线程初始化异常退出"); }
}

bool	CLoginThread::OnProcess()
{
    LOCKTHREAD;

    try
    {
        time_t	tStart = clock();
        long	nUsed = 0;

        if (g_bEnableLogin)		// 允许登录了：)
        {
            // select
            fd_set	readmask;
            FD_ZERO(&readmask);
            if (!m_cListenSocket.IsOpen())
            {
                m_cListenSocket.Rebuild();
            }
            if (m_cListenSocket.IsOpen())
            {
                FD_SET(m_cListenSocket.Socket(), &readmask);
            }
            for(int i = 0; i < MAXCONNECTS; i++)
            {
                if (m_aServerSocket[i].IsOpen())
                {
                    FD_SET(m_aServerSocket[i].Socket(), &readmask);
                }
            }
            struct timeval	timeout = {0, 0};
            int ret = select(FD_SETSIZE, &readmask, (fd_set*) 0, (fd_set*) 0, &timeout);

            // 检查是否接收到新连接
            if (ret == -1)	// error
            {
                //			m_cListenSocket.Close();
                LOGERROR("登录线程 select 错误[%d]", WSAGetLastError());
                //			PrintText("登录线程出错，SOCKET被关闭。%d 秒后将会自动重建", REBUILDLISTENDELAYSEC);
            }
            else if (ret > 0 && FD_ISSET(m_cListenSocket.Socket(), &readmask))
            {
                FD_CLR(m_cListenSocket.Socket(), &readmask);
                ret--;
                u_long	nClientIP;
                SOCKET sockNew = m_cListenSocket.Accept(nClientIP);
                if (sockNew != INVALID_SOCKET)
                {
                    InterlockedIncrement(&s_nLoginAccept);

                    // 取对方IP。查BAN表
                    bool	bBan = false;
                    //				sockaddr_in	inAddr;
                    //				memset(&inAddr, 0, sizeof(inAddr));
                    //				int		nLen = sizeof(inAddr);
                    //				if (getpeername(sockNew, (sockaddr *)&inAddr, &nLen) == 0)		// 可以直接由ACCEPT返回
                    {
                        //					unsigned long	nClientIP = inAddr.sin_addr.S_un.S_addr;
                        for(int i = 0; i < MAXBANIPS; i++)
                        {
                            if (m_pBanIPs[i].ClientIP() == nClientIP && m_pBanIPs[i].IsBan())
                            {
                                bBan = true;		// 不让该IP登录
                                break;
                            }
                        }
                    }

                    // 添加在线表
                    if (bBan)
                    {
                        // 关闭SOCKET
                        //	修改SOCKET属性成立即关闭型
                        struct linger ling;
                        ling.l_onoff = 1;
                        ling.l_linger = 0;
                        setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
                        closesocket(sockNew);		// ★主动关闭SOCKET，必须设置成立即关闭
                        InterlockedDecrement(&s_nSocketCount);
                    }
                    else
                    {
                        // 找空位置
                        bool	bSuccess = false;
                        int i = 0;
                        for(; i < MAXCONNECTS; i++)
                        {
                            if (!m_aServerSocket[i].IsOpen())
                            {
                                bSuccess = true;
                                break;	// 成功
                            }
                        }

                        // 添加连接
                        if (bSuccess)
                        {
                            m_aServerSocket[i].Open(sockNew, nClientIP);
                            m_aServerSocket[i].GetPeerIP();		// 立即取IP
                        }
                        else
                        {
                            /*
                            LOGWARNING("已接收到一个新连接，但连接表已满。请修改MAXCONNECTS，并重新编译程序。");

                            //	修改SOCKET属性成立即关闭型
                            struct linger ling;
                            ling.l_onoff = 1;
                            ling.l_linger = 0;
                            setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));
                            closesocket(sockNew);
                            InterlockedDecrement(&s_nSocketCount);
                            //*/
                            //*else
                            // 抢占旧有的连接，以对抗“大量连接攻击”。可优化为找出攻击者，加入BAN表。??
                            srand((unsigned int) clock());
                            int		nPos = rand() % MAXCONNECTS;
                            char	bufTemp[IPSTRSIZE];
                            ASSERT(m_aServerSocket[nPos].IsOpen());
                            SafeCopy(bufTemp, m_aServerSocket[nPos].GetPeerIP(), IPSTRSIZE);
                            if (FD_ISSET(m_aServerSocket[nPos].Socket(), &readmask))
                            {
                                FD_CLR(m_aServerSocket[nPos].Socket(), &readmask);
                                ret--;
                            }
                            m_aServerSocket[nPos].Close(true);
                            // 由新SOCKET使用
                            m_aServerSocket[nPos].Open(sockNew, nClientIP);
                            LOGWARNING("连接表已满，MAXCONNECTS参数太小。[%s]抢占了[%s]的连接表。",
                                       m_aServerSocket[nPos].GetPeerIP(), bufTemp);
                            //*/
                        }
                    }
                }
            }	// 新连接

            // 接收到登录消息包。需要优化??
            for( int i = 0; i < MAXCONNECTS; i++)
            {
                if (ret <= 0)
                {
                    break;    // 没连接有数据了
                }

                if (m_aServerSocket[i].IsOpen() && FD_ISSET(m_aServerSocket[i].Socket(), &readmask))
                {
                    FD_CLR(m_aServerSocket[i].Socket(), &readmask);
                    ret--;

                    char	bufMsg[_MAX_MSGSIZE];
                    int		nMsgLen = 0;
                    if (m_aServerSocket[i].Recv(bufMsg, nMsgLen))
                    {
                        if (nMsgLen)
                        {
                            ProcessMsg(i, bufMsg, nMsgLen);
                        }
                    }
                }
            }
            nUsed = clock() - tStart;
            InterlockedExchangeAdd(&s_nAllTimeSum, nUsed);
            InterlockedIncrement(&s_nLoginCount);
            InterlockedExchangeAdd(&s_nAvgServerTime, nUsed);
            if (nUsed > InterlockedExchangeAdd(&s_nMaxServerTime, 0))
            {
                InterlockedExchange(&s_nMaxServerTime, nUsed);
            }
        }	// if (g_bEnableLogin)

        long	nRemain = LOGINLOOPDELAY - nUsed;
        if (nRemain < 0 || nRemain > LOGINLOOPDELAY)
        {
            nRemain = 0;    //? 切换到计点线程。计费线程优先
        }
        Sleep(nRemain);

        return true;

    }
    catch(...)
    {
        LOGCATCH("登录线程主循环异常，仍坚持运行中。");
        PrintText("登录线程主循环出错，仍坚持运行中...");
        return true;
    }
}

void	CLoginThread::OnDestroy()
{
    LOCKTHREAD;

    try
    {
        m_cListenSocket.Close();
        for(int i = 0; i < MAXCONNECTS; i++)
        {
//			if (m_aServerSocket[i].IsOpen())
            m_aServerSocket[i].Close();
        }

        LOGMSG("登录线程正常关闭");

    }
    catch(...) { LOGCATCH("登录线程关闭时异常退出"); }
}

///////////////////////////////////////////////////////////////////////////////
// 内部函数
///////////////////////////////////////////////////////////////////////////////
bool	CLoginThread::ProcessMsg(int nIndex, char* pBuf, int nLen)
{
    unsigned short unMsgType = CNetMsg::GetType(pBuf, nLen);
    int	nMsgLen = CNetMsg::GetSize(pBuf, nLen);

    if (unMsgType == _MSG_ACCOUNT && nMsgLen == nLen )
    {
        return ProcessMsgAccount(nIndex, pBuf, nLen);
    }
    else if (unMsgType == _MSG_BARACCOUNT && nMsgLen == nLen )
    {
        return ProcessMsgBarAccount(nIndex, pBuf, nLen);
    }
    else if (unMsgType == _MSG_CONNECT && nMsgLen == nLen)
    {
        if (STAT_SERIAL)
        {
            return ProcessMsgConnect(nIndex, pBuf, nLen);
        }
        return true;
    }
    else
    {
        char* 	pClientIP	= m_aServerSocket[nIndex].GetPeerIP();
        unsigned long nClientIP	= inet_addr(pClientIP);
        AddBan(nClientIP, pClientIP, "未知");
        if (nLen >= 20)
        {
            pBuf[19] = 0;
            LOGWARNING("一个客户端[%s]上传了非法登录消息，消息头[%d][%d]，长度[%d]，可能是[%s]",
                       pClientIP, nMsgLen, unMsgType, nLen, (pBuf + 4));
        }
        else
            LOGWARNING("一个客户端[%s]上传了非法登录消息，消息头[%d][%d]，长度[%d]",
                       pClientIP, nMsgLen, unMsgType, nLen);
        m_aServerSocket[nIndex].Close(true);			// 立即关闭
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 私有函数
/////////////////////////////////////////////////////////////////////////////////////////////
bool	AppendPassword(OBJID idAccount, LPCTSTR szAccount, LPCTSTR szPassword)
{
    bool	bUpdate = false;
    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
    if (g_cDatabase.Create(szAccount, szPassword))		// 有帐号
    {
        // VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
        g_cDatabase.Destroy();
        // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        g_pOnlineTable->SetPassword(idAccount, szPassword);
        bUpdate = true;
    }
    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
    return bUpdate;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	GetFeeType(LPCTSTR szAccount, LPCTSTR szPassword, int& nFeeType, int& nPoint)		// return nType
{
    nFeeType	= c_typeNone;
    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
    if (g_cDatabase.Create(szAccount, szPassword))		// 有帐号
    {
        // VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
        nFeeType	= g_cDatabase.GetType();
        nPoint		= g_cDatabase.GetPoint();
        g_cDatabase.Destroy();
        // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    }
    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
    return (nFeeType != c_typeNone);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	GetLicence(LPCTSTR szBarAccount, LPCTSTR szBarPassword,
                   OBJID& idFeeAccount, int& nPoint, DWORD& nPointTime, 						// 返回值
                   int& nLicenceType, int& nLicence, char* szNetBarIP, char* szIPMask)			// 返回值
{
    idFeeAccount	= ID_NONE;
    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
    if (g_cDatabase.Create(szBarAccount, szBarPassword))		// 有帐号
    {
        // VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
        idFeeAccount	= g_cDatabase.GetID();
        nPoint			= g_cDatabase.GetPoint();
        nPointTime		= g_cDatabase.GetPointTime();
        nLicenceType	= g_cDatabase.GetType();
        nLicence		= g_cDatabase.GetLicence();
        SafeCopy(szNetBarIP, g_cDatabase.GetNetBarIP(), _MAX_NAMESIZE);
        SafeCopy(szIPMask, g_cDatabase.GetIPMask(), _MAX_NAMESIZE);
        g_cDatabase.Destroy();
        // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    }
    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
    return (idFeeAccount != ID_NONE);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CLoginThread::ProcessMsgAccount(int nIndex, char* pBuf, int nLen)
{
    CMsgAccount	cMsg;
    cMsg.Create(pBuf, nLen);
#ifdef	RC5ENCRYPT
    m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
#endif
    FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// 添尾0，删尾空

    if (cMsg.m_pInfo->szAccount[0] == 0 || cMsg.m_pInfo->szPassword[0] == 0 || cMsg.m_pInfo->szServer[0] == 0)
    {
        return false;
    }

    char* 	pClientIP						= m_aServerSocket[nIndex].GetPeerIP();
    unsigned long nClientIP					= inet_addr(pClientIP);
    DWORD	nAuthenID						= NewAuthenID(nClientIP);
    char* 	pErrMsg							= "登录失败";
    char	szNotifyMsg[_MAX_NAMESIZE]		= "";
    OBJID	idAccount						= ID_NONE;
    OBJID	idFeeAccount					= ID_NONE;
    char	szFeeAccount[_MAX_NAMESIZE]		= "";
    int		nFeeType						= c_typeFree;
    int		nPoint							= 0;
    DWORD	nPointTime						= 0;
    int		nBlock							= false;
    bool	bRejoin							= false;
    char	szOldServerName[SERVERNAMESIZE]	= "";
    char	szOldFeeAccount[_MAX_NAMESIZE]	= "";
    char 	szPassword[_MAX_NAMESIZE]	= "";		// 无用变量
    OBJID	idOldFeeAccount				= ID_NONE;	// 无用变量
    char	szOldIP[_MAX_NAMESIZE]		= "";		// 无用变量

    if (g_pOnlineTable->GetAttr(cMsg.m_pInfo->szAccount, idAccount, szPassword, nAuthenID,			// 仅nAuthenID、szOldServerName、szOldFeeAccount 有意义
                                szOldServerName, szOldFeeAccount, idOldFeeAccount, szOldIP))		//??? 在线。该函数可能引起ADDNEW的非原子性操作
    {
        bRejoin	= true;
    }

    if (!GetAccount(cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                    idAccount, nFeeType, nPoint, nPointTime, nBlock))			// 返回值
    {

        if (idAccount == ID_NONE)
        {
            AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errPassword, "帐号名或口令错");
			LOGACCOUNT("玩家[%s]口令[%s]登录。帐号名或口令错, IP地址[%s]",
			           cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, pClientIP);
			return false;
        }
    }

    // 查封号
    if (nBlock)
    {
        RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBan, "该帐号被封号");
        return false;
    }

    // 检查非法登录
    if (nFeeType == c_typeNetBarPoint || nFeeType == c_typeNetBarTime || nFeeType == c_typeISP)
    {
        RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "该帐号不能登录");
        return false;
    }

    if (IsFreeServer(cMsg.m_pInfo->szServer))
    {
        // 免费服务器
        nFeeType		= c_typeFree;
//		SafeCopy(szNotifyMsg, "欢迎参加测试", _MAX_NAMESIZE);
    }
    else if (CheckISP(idAccount, nClientIP, pClientIP,
                      idFeeAccount, szFeeAccount, nPoint, nPointTime, szNotifyMsg))		// 返回值
    {
        // ISP类型
        nFeeType		= c_typeISP;
    }
    else
    {
        idFeeAccount	= idAccount;
        SafeCopy(szFeeAccount, cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);
    }

    if (bRejoin)
    {
        // 不允许同时登录到另一台服务器。查服务器名和计费帐号名是否相同
        if (strcmp(cMsg.m_pInfo->szServer, szOldServerName) != 0
                || strcmp(szFeeAccount, szOldFeeAccount) != 0)	// 上次是用网吧帐号或ISP上的
        {
            if (!g_pOnlineTable->IsTimeWait(idAccount))
            {
                g_pPointThread->Kickout(idAccount);    // 通知游戏服务器踢人
            }

            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errOnline, "请稍后重新登录");
            return false;
        }
    }

    // 分类检查过期
    switch(nFeeType)
    {
    case c_typePoint:	// 没有处理计点卡的时限(点卡也应该有时限)
    case c_typeISP:
        if (nPoint <= 0)
        {
            if (ENABLE_LOGIN_NO_POINT)
            {
                SafeCopy(szNotifyMsg, FLAG_NO_POINT, _MAX_NAMESIZE);
            }
            else
            {
                RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotPoint, "小时数已用尽");
                return false;
            }
        }
        break;
    case c_typeTime:
        {
            time_t	tCurr = time(NULL);
            tm* 	pTm = localtime(&tCurr);
            DWORD	nCurrTime = (pTm->tm_year + 1900) * 10000 + (pTm->tm_mon + 1) * 100 + pTm->tm_mday;
            if (nCurrTime > nPointTime)
            {
                if (MONTH_CARD_TO_POINT_CARD && nPoint > 0)
                {
                    nFeeType	= c_typePoint;
                }
                else if (MONTH_CARD_TO_POINT_CARD && ENABLE_LOGIN_NO_POINT)
                {
                    nFeeType	= c_typePoint;
                    SafeCopy(szNotifyMsg, FLAG_NO_POINT, _MAX_NAMESIZE);
                }
                else
                {
                    RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotTime, "帐号已过期");
                    return false;
                }
            }
            else
            {
                // 检查是否还剩7天
                if (szNotifyMsg[0] == 0)		// 没有通知消息时
                {
                    Check7DaysNodify(nPointTime, szNotifyMsg);		// return szNotifyMsg
                }
            }
        }
        break;
    case c_typeFree:
        break;
    default:
        RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknown, "数据库错误");
        LOGERROR("玩家[%s]计费类型错误[%d]。★★★★★★★★★", cMsg.m_pInfo->szAccount, nFeeType);
        return false;
    }

    // 登录
    ASSERT(idAccount);

    // 通知游戏服务器
    int nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer);
    switch(nRet)
    {
    case CPointThread::ERR_NO_SERVER:
        RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "服务器未启动");
        return false;
    case CPointThread::ERR_BUSY:
        RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerBusy, "服务器忙请稍候");
        return false;
    case CPointThread::ERR_FULL:
        RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerFull, "服务器人数已满");
        return false;
    case CPointThread::ERR_NONE:
        {
            if (bRejoin)
            {
                // 重新进入游戏，修改一些属性
                g_pOnlineTable->Rejoin(idAccount, nAuthenID, pClientIP,
                                       cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                       nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount);
                AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
                LOGACCOUNT("同意玩家[%s][%d]按[%d]类型重新登录[%s]。认证ID[%08X]，IP[%s]，计费帐号[%s]，通知[%s]",
                           cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
                           nAuthenID, pClientIP, szFeeAccount, szNotifyMsg);
            }
            else
            {
                // 添加在线表
                g_pOnlineTable->AddNew(idAccount, nAuthenID, pClientIP,
                                       cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                       nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount);			// return 0: error
                AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
                LOGACCOUNT("同意玩家[%s][%d]按[%d]类型登录[%s]。认证ID[%08X]，IP[%s]，计费帐号[%s]，通知[%s]",
                           cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
                           nAuthenID, pClientIP, szFeeAccount, szNotifyMsg);
            }

            // dump to database
            char	szStamp[255];
            time_t	tCurr = time(NULL);
            tm*	pTm = localtime(&tCurr);
            sprintf(szStamp, "%04d%02d%02d%02d%02d%02d", pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
            char szSQL[1024];
            sprintf(szSQL, "INSERT DELAYED logon VALUES ('%s', '%s',%d,%d,'%s',   %u,'%s','%s','%s',%d);",
                    szStamp, cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
                    nAuthenID, pClientIP, szFeeAccount, szNotifyMsg, bRejoin);
            g_db.ExecuteSQL(szSQL);
            return true;
        }
        break;
    default:
        ASSERT(!"NewLogin");
        return false;
    }
}

bool	CLoginThread::ProcessMsgBarAccount(int nIndex, char* pBuf, int nLen)
{
    CMsgBarAccount	cMsg;
    cMsg.Create(pBuf, nLen);
#ifdef	RC5ENCRYPT
    m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
    m_cRc5NetBar.Rc5Decrypt(cMsg.m_pInfo->szBarPassword, _MAX_NAMESIZE);
//?	m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szBarPassword, _MAX_NAMESIZE);
#endif
    FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szBarAccount, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szBarPassword, _MAX_NAMESIZE);		// 添尾0，删尾空

    OBJID	idAccount		= ID_NONE;
    OBJID	idFeeAccount	= ID_NONE;
    char 	szPassword[_MAX_NAMESIZE]	= "";
    char* 	pClientIP	= m_aServerSocket[nIndex].GetPeerIP();
    unsigned long nClientIP	= inet_addr(pClientIP);
    DWORD	nAuthenID	= NewAuthenID(nClientIP);
    char	szServerName[SERVERNAMESIZE]	= "";
    char	szBarAccount[_MAX_NAMESIZE]		= "";
    char	szNotifyMsg[_MAX_NAMESIZE]	= "";
    char	szOldIP[_MAX_NAMESIZE]			= "";
    char* 	pErrMsg = "登录失败";

    if (IsFreeServer(cMsg.m_pInfo->szServer))
    {
        RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "请使用普通帐号");
        return false;
    }

    // 登录
    if (g_pOnlineTable->GetAttr(cMsg.m_pInfo->szAccount, idAccount, szPassword, nAuthenID,
                                szServerName, szBarAccount, idFeeAccount, szOldIP))		//??? 在线。该函数可能引起ADDNEW的非原子性操作
    {
        // 补登录的玩家无口令
        if (szPassword[0] == 0)
        {
            if (AppendPassword(idAccount, cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword))
            {
                SafeCopy(szPassword, cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
            }
        }

        // 查口令
        if (strcmp(cMsg.m_pInfo->szPassword, szPassword))		// 口令错
        {
            LOGACCOUNT("玩家[%s]再次登录，口令[%s]错。IP地址[%s]",
                       cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, pClientIP);
            AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errPassword, "帐号名或口令错3");
            return false;
        }

        // 帐号、口令正确
        // 查点数
        //? 不必查，下线时在线表会自动删除

        // 取LICENCETYPE(可优化，将取LICENCETYPE放在ONLINE表中)
        int		nLicenceType = c_typeNone;
        int		nPoint		= 0;	// 无意义
        GetFeeType(cMsg.m_pInfo->szBarAccount, cMsg.m_pInfo->szBarPassword, nLicenceType, nPoint);		// return nLicenceType

        // 不允许同时登录到另一台服务器，查服务器名和计费帐号名是否相同
        if (nLicenceType == c_typeNone || strcmp(cMsg.m_pInfo->szServer, szServerName) != 0
                || strcmp(cMsg.m_pInfo->szBarAccount, szBarAccount) != 0
                || strcmp(pClientIP, szOldIP) != 0  )	// 比较上次登录的网吧帐号是否相同
        {
            if (!g_pOnlineTable->IsTimeWait(idAccount))
            {
                g_pPointThread->Kickout(idAccount);    // 通知游戏服务器踢人
            }

            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errOnline, "请稍后重新登录");
            return false;
        }

        // 不使用新认证ID					nAuthenID	= NewAuthenID(nClientIP);

        // 通知游戏服务器
        int nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer);
        switch(nRet)
        {
        case CPointThread::ERR_NO_SERVER:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "服务器未启动");
            return false;
        case CPointThread::ERR_BUSY:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerBusy, "服务器忙请稍候");
            return false;
        case CPointThread::ERR_FULL:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerFull, "服务器人数已满");
            return false;
        case CPointThread::ERR_NONE:
            // 重新进入游戏，修改一些属性
            g_pOnlineTable->Rejoin(idAccount, nAuthenID, pClientIP,
                                   cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                   nLicenceType, cMsg.m_pInfo->szServer, idFeeAccount, cMsg.m_pInfo->szBarAccount);
            AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
            LOGACCOUNT("同意玩家[%s][%d]用网吧帐号[%s][%d]再次登录[%s]。认证ID[%08X]，IP[%s]，计费帐号[%s]",
                       cMsg.m_pInfo->szAccount, idAccount,
                       cMsg.m_pInfo->szBarAccount, idFeeAccount, cMsg.m_pInfo->szServer,
                       nAuthenID, pClientIP, cMsg.m_pInfo->szBarAccount);
            return true;
        default:
            ASSERT(!"NewLogin");
            return false;
        }
    }
    else	// 没在线
    {
        // 检查LICENCE帐号//////////////////////////////////////////////////////////////////////
        ASSERT(idFeeAccount	== ID_NONE);
        int		nPoint			= 0;
        DWORD	nPointTime		= 0;
        int		nLicenceType	= c_typeNone;
        int		nLicence		= 0;
        char	szNetBarIP[IPSTRSIZE]	= "127.0.0.1";
        char	szIPMask[IPSTRSIZE]		= "255.255.255.255";
        if (!GetLicence(cMsg.m_pInfo->szBarAccount, cMsg.m_pInfo->szBarPassword,
                        idFeeAccount, nPoint, nPointTime, nLicenceType, nLicence, szNetBarIP, szIPMask))			// 返回值
        {
            LOGACCOUNT("许可证帐号[%s]或口令[%s]错。IP地址[%s]",
                       cMsg.m_pInfo->szBarAccount, cMsg.m_pInfo->szBarPassword, pClientIP);
            AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "许可证密码错误");
            return false;
        }

        // 检查TYPE和许可人数
        switch(nLicenceType)
        {
        case c_typePoint:
        case c_typeTime:
        case c_typeISP:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "许可证类型错误");
            return false;
        case c_typeNetBarPoint:
            if (nPoint <= 0)
            {
                RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotTime, "许可证小时耗尽");
                return false;
            }
            break;
        case c_typeNetBarTime:
            {
                // 检查过期
                time_t	tCurr = time(NULL);
                tm* 	pTm = localtime(&tCurr);
                DWORD	nCurrTime = (pTm->tm_year + 1900) * 10000 + (pTm->tm_mon + 1) * 100 + pTm->tm_mday;
                if (nCurrTime > nPointTime)
                {
                    RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotTime, "许可证过期");
                    return false;
                }

                // 检查人数
                int nCount = g_pOnlineTable->GetFeeCount(idFeeAccount);
                if (nCount >= nLicence)
                {
//					LOGACCOUNT("许可证[%s]人数已满[%d/%d]。IP地址[%s]",
//								cMsg.m_pInfo->szBarAccount, nCount, nLicence, pClientIP);
                    RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarOverflow, "许可证人数已满");
                    return false;
                }

                // 检查是否还剩7天
                if (szNotifyMsg[0] == 0)		// 没有通知消息时
                {
                    Check7DaysNodify(nPointTime, szNotifyMsg);		// return szNotifyMsg
                }
            }
            break;
        default:
            ASSERT(!"switch(nLicenceType)");
            return false;
        }

        // 检查IP
        DWORD	dwNetBarIP	= inet_addr(szNetBarIP);
        DWORD	dwIPMask	= inet_addr(szIPMask);
        ASSERT(dwNetBarIP != INADDR_NONE);
//		ASSERT(dwIPMask != INADDR_NONE);
        if ((nClientIP & dwIPMask) != (dwNetBarIP & dwIPMask))
        {
            LOGACCOUNT("许可证[%s]的登录IP超出范围[%s][%s]。IP地址[%s]★",
                       cMsg.m_pInfo->szBarAccount, szNetBarIP, szIPMask, pClientIP);
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "许可证IP错误");
            return false;
        }

        ASSERT(idFeeAccount != ID_NONE);

        // 检查玩家的帐号//////////////////////////////////////////////////////////////////////
        ASSERT(idAccount == ID_NONE);		// 冗余
        int		nAccountType	= c_typeNone;
        int		nBlock			= false;
        int		nUserPoint;					// 无用
        DWORD	nUserPointTime;				// 无用
        if (!GetAccount(cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                        idAccount, nAccountType, nUserPoint, nUserPointTime, nBlock))		// 返回值
        {
            LOGACCOUNT("玩家[%s]口令[%s]登录。帐号名或口令错, IP地址[%s]",
                       cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, pClientIP);
            AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errPassword, "帐号名或口令错4");
            return false;
        }

        // 查ONLINE
        if (nBlock)
        {
//			LOGACCOUNT("玩家[%s]被封号[%d]", cMsg.m_pInfo->szAccount, nBlock);
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBan, "该帐号被封号");
            return false;
        }

        // 查类型
        switch(nAccountType)
        {
        case c_typePoint:
        case c_typeTime:
            break;
        case c_typeNetBarPoint:
        case c_typeNetBarTime:
        case c_typeISP:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "该帐号不能登录");
            return false;
        default:
            LOGERROR("玩家[%s]计费类型错误[%d]。★★★★★★★★★", cMsg.m_pInfo->szAccount, nAccountType);
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknown, "数据库错误");
            return false;
        }

        // 登录
        ASSERT(idAccount);

        // 通知游戏服务器
        int nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer);
        switch(nRet)
        {
        case CPointThread::ERR_NO_SERVER:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "服务器未启动");
            return false;
        case CPointThread::ERR_BUSY:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerBusy, "服务器忙请稍候");
            return false;
        case CPointThread::ERR_FULL:
            RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerFull, "服务器人数已满");
            return false;
        case CPointThread::ERR_NONE:
            // 添加在线表
            g_pOnlineTable->AddNew(idAccount, nAuthenID, pClientIP,
                                   cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                   nLicenceType, cMsg.m_pInfo->szServer, idFeeAccount, cMsg.m_pInfo->szBarAccount);			// return 0: error
            AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
            LOGACCOUNT("同意玩家[%s][%d]用网吧帐号[%s][%d]登录[%s]。认证ID[%08X]，IP[%s]，计费帐号[%s]",
                       cMsg.m_pInfo->szAccount, idAccount,
                       cMsg.m_pInfo->szBarAccount, idFeeAccount, cMsg.m_pInfo->szServer,
                       nAuthenID, pClientIP, cMsg.m_pInfo->szBarAccount);
            return true;
        default:
            ASSERT(!"NewLogin");
            return false;
        }
    }  // if (在线、没在线)
}

void CLoginThread::AllowLogin(int nIndex, OBJID idAccount, DWORD nAuthenID, LPCTSTR szServer)
{
    CMsgConnect cRetMsg;
    char	szServerIP[IPSTRSIZE] = "";
    g_pPointThread->GetServerIP(szServerIP, szServer);
    cRetMsg.Create(idAccount, nAuthenID, szServerIP);		//? 下传游戏服务器的IP地址
    m_aServerSocket[nIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
    if (!STAT_SERIAL)
    {
        m_aServerSocket[nIndex].ShutDown();    //?? 关闭，让对方先关闭(注意：如果没收到CMsgConnect消息则不会关闭)
    }
}

void CLoginThread::RefuseLogin(int nIndex, LPCTSTR szLoginName, int nType, LPCTSTR szText)
{
    CMsgConnect cRetMsg;
    cRetMsg.Create(ID_NONE, nType, (char*)szText);
    m_aServerSocket[nIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
    m_aServerSocket[nIndex].ShutDown();		//?? 关闭，让对方先关闭
    LOGACCOUNT("登录不成功[%s]。原因是[%s][%d]", szLoginName, szText, nType);
}

void CLoginThread::AddBan(DWORD nClientIP, LPCTSTR szClientIP, LPCTSTR szAccount)
{
    // 增加BAN表
    bool	bFoundBan = false;
    int		nFreeSlot = -1;
    int		nBanCount = 0;
    // 找个空位置
    for(int i = 0; i < MAXBANIPS; i++)
    {
        if (m_pBanIPs[i].ClientIP() == 0)
        {
            if (nFreeSlot == -1)
            {
                nFreeSlot = i;
            }
        }
        else
        {
            nBanCount++;
        }

        if (m_pBanIPs[i].ClientIP() == nClientIP)	// 已记录
        {
            bFoundBan = true;
            m_pBanIPs[i].IncError();				//? 不立即踢，超过再踢
            if (m_pBanIPs[i].IsBan())
            {
                LOGERROR("某客户端已非法登录[%d]次，可能有人攻击帐号服务器。IP地址[%s]", BANERRORS, szClientIP);
                LOGACCOUNT("某玩家[%s]登录到帐号服务器连续出错[%d]次。IP地址[%s]被禁止[%d]秒钟",
                           szAccount, BANERRORS, szClientIP, BANSECS);
                PrintText("IP地址“%s”连续出错“%d”次, “%d”秒内将无法登录",
                          szClientIP, BANERRORS, BANSECS);
            }
        }
    }
    if (!bFoundBan)
    {
        // 添加新BAN
        if (nFreeSlot != -1)
        {
            m_pBanIPs[nFreeSlot].Create(nClientIP);
            if (nBanCount * 100 / MAXBANIPS > 78)		// 快满了
            {
                LOGWARNING("添加BAN表[%d/%d]，IP为[%s]", nBanCount + 1, MAXBANIPS, szClientIP);
            }
        }
        else
        {
            LOGERROR("BAN 表太小，有个IP[%s]没填进去。请适当增大 MAXBANIPS 参数", szClientIP);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CLoginThread::ProcessMsgConnect(int nIndex, char* pBuf, int nLen)
{
    CMsgConnect	cMsg;
    cMsg.Create(pBuf, nLen);
    cMsg.m_pInfo->szInfo[_MAX_NAMESIZE-1] = 0;

    g_pOnlineTable->SetSerial(cMsg.m_pInfo->idAccount, cMsg.m_pInfo->dwData);
    m_aServerSocket[nIndex].ShutDown();		//?? 关闭，让对方先关闭

    // dump to database
    char	szStamp[255];
    time_t	tCurr = time(NULL);
    tm*	pTm = localtime(&tCurr);
    sprintf(szStamp, "%04d%02d%02d%02d%02d%02d", pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
    char	szSerial[255];
    ::InsertBackslash(szSerial, cMsg.m_pInfo->szInfo);
    char szSQL[1024];
    sprintf(szSQL, "INSERT DELAYED serial VALUES ('%s',%d,%d,'%s');",
            szStamp, cMsg.m_pInfo->idAccount, cMsg.m_pInfo->dwData, szSerial);
    g_db.ExecuteSQL(szSQL);
    return true;
}















