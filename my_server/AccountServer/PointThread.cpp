
//**********************************************************
// ����༭��
//**********************************************************

// PointThread.cpp: �Ƶ��߳���
// �ɽ��ޣ�2001.11.20
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "PointThread.h"
#include "MsgConnect.h"
#include "MsgConnect91U.h"
#include "MsgAccount.h"
#include "MsgAccount91U.h"
#include "MsgLogin.h"
#include "MsgFee.h"
#include "MsgQueryFee.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointThread::CPointThread(u_short nPort, int nSndBuf /*= 0*/)
    : CThreadBase(), m_cListenSocket(nPort, nSndBuf)
{
    for(int i = 0; i < MAXGAMESERVERS; i++)
    {
        Clear(i, false);
    }

    // �����ʺű�
    GetServerAccount();		// Ԥ��
    m_pBanIPs = new CBanIP[MAXBANIPS];
}

CPointThread::~CPointThread()
{
    delete m_pBanIPs;
}

/////////////////////////////////////////////////////////////////////////////
// ����������ע�⻥��

//#define	LOCK	{LOCKTHREAD;
//#define	UNLOCK	}

// ��ʹ�ù�����Ա�����������������������ⲿ�����������Ƚ�����
void	CPointThread::OnInit()
{
    LOCKTHREAD;
    try
    {

        m_cRc5.Rc5InitKey(RC5PASSWORD_KEY);

        m_cListenSocket.Open();
        LOGMSG("�Ƶ��߳���������");
    }
    catch(...) { LOGCATCH("�Ƶ��̳߳�ʼ��ʱ�쳣�˳�");}
}

bool	CPointThread::OnProcess()
{
    LOCKTHREAD;

    try
    {
        time_t	tStart = clock();
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
        for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
        {
            if (m_aServerSocket[i].IsOpen())
            {
                FD_SET(m_aServerSocket[i].Socket(), &readmask);
            }
        }

        struct timeval	timeout = {0, 0};
        int ret = select(FD_SETSIZE, &readmask, (fd_set*) 0, (fd_set*) 0, &timeout);

        // ����Ƿ���յ�������
        if (ret == -1)	// error
        {
            m_cListenSocket.Close();
            LOGERROR("�Ƶ��߳� select ����[%d]", WSAGetLastError());
            PrintText("�Ƶ��̳߳�����SOCKET���رա�%d ��󽫻��Զ��ؽ�", REBUILDLISTENDELAYSEC);
        }
        else if (ret > 0 && FD_ISSET(m_cListenSocket.Socket(), &readmask))
        {
            FD_CLR(m_cListenSocket.Socket(), &readmask);
            ret--;
            u_long	nClientIP;
            SOCKET	sockNew = m_cListenSocket.Accept(nClientIP);
            if (sockNew != INVALID_SOCKET)
            {
                // �ҿ�λ��
                bool bSuccess = false;
                int i = 1;
                for(; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
                {
                    if (!m_aServerSocket[i].IsOpen())
                    {
                        bSuccess = true;
                        break;	// �ɹ�
                    }
                }

                // ��������
                struct in_addr	in;
                in.S_un.S_addr = nClientIP;
                char* pAddr = inet_ntoa(in);
                if (bSuccess)
                {
                    m_aServerSocket[i].Open(sockNew, nClientIP);
                    m_aHeartbeat[i] = clock();
                    LOGMSG("һ������Ϸ���������ӽ�����������ID�ŷ���Ϊ[%d], IP��ַΪ[%s]", i, pAddr);
                    ::SetServerState(i, c_flagSocket);
                }
                else
                {
                    LOGWARNING("�ѽ��յ�һ��������[%s]�������ӱ����������޸�MAXGAMESERVERS�������±������"
                               , pAddr);
                    //	�޸�SOCKET���Գ������ر���
                    struct linger ling;
                    ling.l_onoff = 1;
                    ling.l_linger = 0;
                    setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
                    closesocket(sockNew);
                    InterlockedDecrement(&s_nSocketCount);
                }
            }
        }

        // ���յ���¼��Ϣ������Ҫ�Ż�??
        for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
        {
            if (ret <= 0)
            {
                break;    // û������������
            }

            if (m_aServerSocket[i].IsOpen())
            {
                if (FD_ISSET(m_aServerSocket[i].Socket(), &readmask))
                {
                    FD_CLR(m_aServerSocket[i].Socket(), &readmask);
                    ret--;
                    char	bufMsg[_MAX_MSGSIZE];
                    int		nMsgLen = 0;
                    if (m_aServerSocket[i].Recv(bufMsg, nMsgLen))
                    {
                        if (nMsgLen)
                        {
                            m_aHeartbeat[i] = clock();
                            m_aHeartbeatLast[i] = 0;
                            ProcessMsg(i, bufMsg, nMsgLen);
                        }
					}
                    else	// �����ر�
                    {
                        LOGWARNING("Recv����������[%s]�رա�", m_aServerName[i]);
                        LOGSERVER("���ߣ�������[%s]�رա�", m_aServerName[i]);
                        PrintText("��������%s���رա�", m_aServerName[i]);
                        Clear(i);
                    }
                }
            }
        }
        long	nUsed = clock() - tStart;
        InterlockedExchangeAdd(&s_nAllTimeSum, nUsed);
        InterlockedIncrement(&s_nPointCount);

        long	nRemain = POINTLOOPDELAY - nUsed;
        if (nRemain < 0 || nRemain > POINTLOOPDELAY)
        {
            nRemain = 0;
        }
        if (nRemain)		//? ���л�����¼�̡߳��Ʒ��߳�����
        {
            Sleep(nRemain);
        }

        return true;

    }
    catch(...)
    {
        LOGCATCH("�Ƶ��߳���ѭ���쳣��������������");
        PrintText("�Ƶ��߳���ѭ��������������������...");
        return true;
    }
}

void	CPointThread::OnDestroy()
{
    LOCKTHREAD;

//	LOCK	// VVVVVVVVV
    try
    {
        m_cListenSocket.Close();
        for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
        {
            if (m_aServerSocket[i].IsOpen())
            {
                m_aServerSocket[i].Close();
            }
        }
        LOGMSG("�Ƶ��߳������˳�");

    }
    catch(...) { LOGCATCH("�Ƶ��߳̽���ʱ�쳣�˳�"); }

//	UNLOCK	// AAAAAAAAA
}

// �ﹲ����������Ҫ���⡣��LOGIN�̵߳��ã���ø�Ϊ��Ϣ����ģʽ??
// return ERR_NONE: OK
int	CPointThread::NewLogin(OBJID idAccount, DWORD nAuthenID, const char* szClientIP, const char* pGameServer)
{
    LOCKTHREAD;

    CMsgConnect	cMsg;
    ASSERT(IPSTRSIZE == _MAX_NAMESIZE);
    cMsg.Create(idAccount, nAuthenID, (char*)szClientIP);
    OBJID		nIndex = GetServerIndex_0(pGameServer);		// ���󷵻�NULL
    if (nIndex && m_aServerSocket[nIndex].IsOpen())
    {
        if (m_aState[nIndex] == STATE_NORMAL || idAccount <= MAX_GM_ID)
        {
            m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());	// ����ʧ��ʱ���Դ����´λ�ɾ����Ϸ��������
//			LOGACCOUNT("���͸�[%s]���[%d]�ĵ�¼��Ϣ����֤ID[%08X]���ͻ���IP[%s]",
//						m_aServerName[nIndex], idAccount, nAuthenID, szClientIP);
            return ERR_NONE;
        }
        else if (m_aState[nIndex] == STATE_BUSY)
        {
            return ERR_BUSY;
        }
        else if (m_aState[nIndex] == STATE_FULL)
        {
            return ERR_FULL;
        }
    }

    return ERR_NO_SERVER;	// ����ķ��������������δ������
}

bool	CPointThread::GetServerIP(char* bufIP, const char* pServerName)
{
    LOCKTHREAD;

    int i = 1;
    for(; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
    {
        if (strcmp(m_aServerName[i], pServerName) == 0)
        {
            break;
        }
    }

    if (bufIP && i < MAXGAMESERVERS)
    {
        if (m_aServerSocket[i].IsOpen())
        {
            strcpy(bufIP, m_aServerSocket[i].GetPeerIP());
            return true;
        }
        else
        {
            if (m_aServerName[i][0])
            {
                LOGWARNING("ȡIPʱ����Ϸ������[%s]�Ѿ��ر�", m_aServerName[i]);
                LOGSERVER("������[%s]�Ѿ��ر���", m_aServerName[i]);
                PrintText("��������%s���Ѿ��ر���", m_aServerName[i]);
            }
            Clear(i);
        }
    }

    bufIP[0] = 0;
    return false;
}

int	CPointThread::GetServerState(LPCTSTR szServerName)
{
    LOCKTHREAD;

    int	nIndex = GetServerIndex_0(szServerName);
    if (nIndex)
    {
        return m_aState[nIndex];
    }
    else
    {
        return STATE_OFFLINE;
    }
}

int	CPointThread::GetServerCount()
{
    LOCKTHREAD;

    bool	nCount = 0;
    for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
    {
        if (m_aState[i] != STATE_OFFLINE)
        {
            nCount++;
        }
    }

    return nCount;
}

bool CPointThread::CheckHeartbeatAll()
{
    LOCKTHREAD;

    bool	ret = false;
    for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
    {
        if (m_aServerSocket[i].IsOpen())
        {
            CheckHeartbeat(i);
            ret = true;
        }
    }
    return ret;
}

/*bool	CPointThread::GetServerName(OBJID idServer, char bufName[SERVERNAMESIZE])
{
	LOCKTHREAD;

	ASSERT(bufName);

	bufName[0] = 0;
	if (!idServer)
		return false;

	if (m_aServerSocket[idServer].IsOpen())
	{
		SafeCopy(bufName, m_aServerName[idServer], _MAX_NAMESIZE);
		return true;
	}
	else
	{
		if (m_aServerName[idServer][0])
		{
			LOGWARNING("ȡ��ʱ����Ϸ������[%s]�Ѿ��ر�", m_aServerName[idServer]);
			LOGSERVER("������[%s]�Ѿ��رա�", m_aServerName[idServer]);
			PrintText("��������%s���Ѿ��رա�", m_aServerName[idServer]);
		}
		Clear(idServer);		// SOCKET�رպ�ɾ����Ϸ��������������ͬ��
		return false;
	}
}*/

void	CPointThread::LogCount()
{
    LOCKTHREAD;

    char	bufTime[20];
    DateTime(bufTime, time(NULL));
    char	bufLine[4096] = "";			//?
    int		nAmount = g_pOnlineTable->GetPlayerCount();
    for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
    {
        int	nCount = g_pOnlineTable->GetPlayerCount(g_aServerAccount[i].m_szServerName);
        int	nState = g_pPointThread->GetServerState(g_aServerAccount[i].m_szServerName);
        if (nState == CPointThread::STATE_OFFLINE)
        {
            nAmount	-= nCount;
            nCount	= 0;
        }
        char	buf[256];
        sprintf(buf, " | %12d", nCount);
        strcat(bufLine, buf);
    }
    strcat(bufLine, "\n");

    char	buf[4096];
    sprintf(buf, "%s | %12d", bufTime, nAmount);
    strcat(buf, bufLine);
    log_SaveFile("count", buf, g_szCountHeadLine);		//? LOGFILE�����Լ��Ļ�������˫�ػ���Ч�ʽϵ�
}

void	CPointThread::LogSerialCount()
{
    LOCKTHREAD;

    char	bufTime[20];
    DateTime(bufTime, time(NULL));
    char	bufLine[4096] = "";			//?
    int		nAmount = 0;
    int		nCount = 0;
    for (COnlineTable::SERIAL_INDEX::Iter_t iter = g_pOnlineTable->GetSerialSet()->Begin();
            iter != g_pOnlineTable->GetSerialSet()->End();
            iter++, nCount++)
    {
        char	buf[256];
        sprintf(buf, " | %4d:%4d", iter->first, iter->second);
        strcat(bufLine, buf);
        nAmount	+= iter->second;
        if (strlen(bufLine) > 4000)
        {
            break;
        }
        if (nCount > 50000)
        {
            LOGERROR("��LogSerialCount()������");
            break;
        }
    }
    strcat(bufLine, "\n");

    char	buf[4196];
    sprintf(buf, "%s | %8d", bufTime, nAmount);
    strcat(buf, bufLine);
    log_SaveFile("serial", buf, "ʱ��                |   �ϼ�   | ���к�:����\n");		//? LOGFILE�����Լ��Ļ�������˫�ػ���Ч�ʽϵ�
}

void	CPointThread::LogServer()
{
    LOCKTHREAD;

    char	bufTime[20];
    DateTime(bufTime, time(NULL));
    char	bufLine[4096];			//?
    sprintf(bufLine, "%s | %12d", bufTime, GetServerCount());
    for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
    {
        BOOL	bState = GetServerState(g_aServerAccount[i].m_szServerName);
        char	buf[256];
        if (bState != STATE_OFFLINE)
        {
            sprintf(buf, " | %12s", "��");
        }
        else
        {
            sprintf(buf, " | %12s", "�w");
        }
        strcat(bufLine, buf);
    }
    strcat(bufLine, "\n");

    log_SaveFile("server", bufLine, g_szCountHeadLine);		//? LOGFILE�����Լ��Ļ�������˫�ػ���Ч�ʽϵ�
}

bool	CPointThread::Kickout(OBJID idAccount)
{
    LOCKTHREAD;

    char	bufServer[256];
    if (g_pOnlineTable->GetServerName(idAccount, bufServer))
    {
        return Kickout_0(idAccount, bufServer);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////
// ��ǹ��������������̶߳�ռ���á����ܵ����ⲿ������

bool	CPointThread::Kickout_0(OBJID idAccount, LPCTSTR szServerName)
{
    int nIndex = GetServerIndex_0(szServerName);

    if (nIndex != 0 )
    {
        CMsgFee	cMsg;
        cMsg.Create(idAccount, CMsgFee::FEE_KICKOUT);
        m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
        return true;
    }


    return false;
}

bool	CPointThread::ProcessMsg(int nServerIndex,  char* pBuf, int nLen)
{
    unsigned short unMsgType = CNetMsg::GetType(pBuf, nLen);
    switch(unMsgType)
    {
    case _MSG_ACCOUNT:
        {
            CMsgAccount	cMsg;
            cMsg.Create(pBuf, nLen);
            FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// ��β0��ɾβ��
            FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// ��β0��ɾβ��
            FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// ��β0��ɾβ��
            LOGMSG("���յ�[%s]��Ϸ������ACCOUNT��Ϣ���ʺ�[%s],����[***]��"
                   , cMsg.m_pInfo->szServer
                   , cMsg.m_pInfo->szAccount
                   //, cMsg.m_pInfo->szPassword
                  );

            // ��Ϣ���
            if (strlen(cMsg.m_pInfo->szAccount) == 0
                    || strlen(cMsg.m_pInfo->szPassword) == 0
                    || strlen(cMsg.m_pInfo->szServer) == 0)
            {
                LOGERROR("����ĵ�¼��Ϣ[%s][%s][%s]",
                         cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, cMsg.m_pInfo->szServer);
                return false;
            }

            GetServerAccount();		// ��ʱ����

            // ����ʺ�
            int		nAccount = 0;		// ���ʺű��е���ţ�����Ϸ������ID�š�
            for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
            {
                if (!g_aServerAccount[i].m_b91U
                        && strcmp(g_aServerAccount[i].m_szServerName, cMsg.m_pInfo->szServer) == 0)		// �Է�������ƥ��Ϊ׼
                {
                    nAccount = i;
                    break;
                }
            }

            CMsgLogin	cRetMsg;
            if (nAccount					// 0���޴��ʺ�
                    && strcmp(g_aServerAccount[nAccount].m_szLoginName, cMsg.m_pInfo->szAccount) == 0
                    && strcmp(g_aServerAccount[nAccount].m_szPassword, cMsg.m_pInfo->szPassword) == 0)	// �ҵ�
            {
                for(int i = 1; i < MAXGAMESERVERS; i++)
                {
                    if (strcmp(g_aServerAccount[nAccount].m_szServerName, m_aServerName[i]) == 0)
                    {
                        // �ظ���¼
                        if (m_aServerSocket[i].IsOpen())
                        {
                            LOGMSG("��Ϸ������[%s]�ظ�ACCOUNT��¼��ǰһ������[%d]�ѶϿ���", m_aServerName[i], i);
                            m_aServerSocket[i].Close();		// �ر�ǰһ����¼������
                            Clear(i);
                        }
                        m_aServerName[i][0] = 0;		// ɾ��SERVER����
                        //��С����ȫ��һ��	break;
                    }
                }

                // ������Ϸ����������/IP
                strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
                //?? BUG����ǰ������¼�ˣ�Ӧ�ý��յ�_MSG_LOGIN��ʼ������¼
                PrintText("��%d����Ϸ��������%s���ѵ�¼", nServerIndex, m_aServerName[nServerIndex]);
                LOGSERVER("��%d����Ϸ��������%s���ѵ�¼", nServerIndex, m_aServerName[nServerIndex]);
                //?? BUG����ǰ������¼�ˣ�Ӧ�ý��յ�_MSG_LOGIN��ʼ������¼
                cRetMsg.Create(nServerIndex, 0, "", ACCOUNTVERSION);
                m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                LOGMSG("�µ���Ϸ������[%s]ACCOUNT�ʺ��Ͽɡ����ط�����ID[%d],��Ϸ�汾[%04X]",
                       m_aServerName[nServerIndex], nServerIndex, ACCOUNTVERSION);
                LOGMSG("�µ���Ϸ������[%s]ACCOUNT��¼��......", m_aServerName[nServerIndex]);
                ::SetServerState(nServerIndex, c_flagAccount);

                // ��������
//				m_idxID.Add(nServerIndex, nAccount);
//				g_aServerAccount[nAccount].m_nIndex = nServerIndex;
            }
            else
            {
                cRetMsg.Create(0, 0, "", ACCOUNTVERSION);
                m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                LOGMSG("�µ���Ϸ������[%s]��¼ʧ��", cMsg.m_pInfo->szServer);
            }
        }
        break;
    case _MSG_ACCOUNT91U:
        {
            CMsgAccount91U	cMsg;
            cMsg.Create(pBuf, nLen);
            FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// ��β0��ɾβ��
            FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// ��β0��ɾβ��
            FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// ��β0��ɾβ��
            FormatStr(cMsg.m_pInfo->szIPAddr, _MAX_NAMESIZE);		// ��β0��ɾβ��

            if (cMsg.m_pInfo->id91U == 0)	// 91U��������¼
            {
                LOGMSG("���յ�[%s]91U������ACCOUNT��Ϣ���ʺ�[%s],����[***]��"
                       , cMsg.m_pInfo->szServer
                       , cMsg.m_pInfo->szAccount
                       //, cMsg.m_pInfo->szPassword
                      );

                // ��Ϣ���
                if (strlen(cMsg.m_pInfo->szAccount) == 0
                        || strlen(cMsg.m_pInfo->szPassword) == 0
                        || strlen(cMsg.m_pInfo->szServer) == 0)
                {
                    LOGERROR("�����91U��������¼��Ϣ[%s][%s][%s]",
                             cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, cMsg.m_pInfo->szServer);
                    return false;
                }

                GetServerAccount();		// ��ʱ����

                // ����ʺ�
                int		nAccount = 0;		// ���ʺű��е���ţ�����Ϸ������ID�š���0���ʺ�
                for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
                {
                    if (g_aServerAccount[i].m_b91U
                            && strcmp(g_aServerAccount[i].m_szServerName, cMsg.m_pInfo->szServer) == 0)		// �Է�������ƥ��Ϊ׼
                    {
                        nAccount = i;
                        break;
                    }
                }

                CMsgLogin	cRetMsg;
                if (nAccount					// 0���޴��ʺ�
                        && strcmp(g_aServerAccount[nAccount].m_szLoginName, cMsg.m_pInfo->szAccount) == 0
                        && strcmp(g_aServerAccount[nAccount].m_szPassword, cMsg.m_pInfo->szPassword) == 0)	// �ҵ�
                {
                    for(int i = 1; i < MAXGAMESERVERS; i++)
                    {
                        if (strcmp(g_aServerAccount[nAccount].m_szServerName, m_aServerName[i]) == 0)
                        {
                            // �ظ���¼
                            if (m_aServerSocket[i].IsOpen())
                            {
                                LOGMSG("91U������[%s]�ظ�ACCOUNT��¼��ǰһ������[%d]�ѶϿ���", m_aServerName[i], i);
                                m_aServerSocket[i].Close();		// �ر�ǰһ����¼������
                                Clear(i);
                            }
                            m_aServerName[i][0] = 0;		// ɾ��SERVER����
                            //��С����ȫ��һ��	break;
                        }
                    }

                    // ����91U����������/IP
                    strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
                    //?? BUG����ǰ������¼�ˣ�Ӧ�ý��յ�_MSG_LOGIN��ʼ������¼
                    PrintText("��%d��91U��������%s���ѵ�¼", nServerIndex, m_aServerName[nServerIndex]);
                    LOGSERVER("��%d��91U��������%s���ѵ�¼", nServerIndex, m_aServerName[nServerIndex]);
                    //?? BUG����ǰ������¼�ˣ�Ӧ�ý��յ�_MSG_LOGIN��ʼ������¼
                    cRetMsg.Create(nServerIndex, 0, "", ACCOUNTVERSION);
                    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                    LOGMSG("�µ�91U������[%s]ACCOUNT�ʺ��Ͽɡ����ط�����ID[%d],��Ϸ�汾[%04X]",
                           m_aServerName[nServerIndex], nServerIndex, ACCOUNTVERSION);
                    LOGMSG("�µ�91U������[%s]ACCOUNT��¼�ɹ���", m_aServerName[nServerIndex]);
                    ::SetServerState(nServerIndex, c_flagNormal91U);
                    m_aState[nServerIndex] = STATE_NORMAL91U;		// ��¼���

                    // ��������
                    //				m_idxID.Add(nServerIndex, nAccount);
                    //				g_aServerAccount[nAccount].m_nIndex = nServerIndex;
                }
                else
                {
                    cRetMsg.Create(0, 0, "", ACCOUNTVERSION);
                    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                    LOGMSG("�µ���Ϸ������[%s]��¼ʧ��", cMsg.m_pInfo->szServer);
                }
            }
            else	// ���ͨ��91U��������¼
            {
                if (m_aState[nServerIndex] != STATE_NORMAL91U)
                {
                    LOGERROR("�������յ�_MSG_ACCOUNT91U��Ϣ��");
                    return false;
                }
                ProcessMsgAccount(nServerIndex, pBuf, nLen);	// szPassword �����ǿմ�
            }
        }
        break;
    case _MSG_LOGIN:
        {
            if (g_aServerAccount[nServerIndex].m_b91U)
            {
                LOGERROR("�������յ�_MSG_LOGIN��Ϣ��");
                return false;
            }
            // ��Ϣ���(����Ҫ)

//			CMsgConnect	cMsg;
//			cMsg.Create(nServerIndex, 0, "��¼�ɹ�");

            // ���򵥻ش�
//			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
            m_aState[nServerIndex] = STATE_NORMAL;		// ��¼���

            LOGMSG("......[%s]��Ϸ������LOGIN��¼�ɹ�", m_aServerName[nServerIndex]);
            LOGMSG("���յ���¼��Ϣ���µ���Ϸ������[%s]LOGIN��¼�ɹ�", m_aServerName[nServerIndex]);
            ::SetServerState(nServerIndex, c_flagNormal);
        }
        break;
    case _MSG_FEE:
        {
            CMsgFee		cMsg;
            cMsg.Create(pBuf, nLen);
            OBJID	idAccount = cMsg.m_pInfo->idAccount;

            if (g_aServerAccount[nServerIndex].m_b91U && cMsg.m_pInfo->ucType != CMsgFee::HEARTBEAT)
            {
                LOGERROR("�������յ�_MSG_FEE��Ϣ��");
                return false;
            }

            switch(cMsg.m_pInfo->ucType)
            {
            case CMsgFee::FEE_BEGIN:
                {
                    // ��Ϣ���
                    if (idAccount == ID_NONE)
                    {
                        LOGERROR("�����FEE_BEGIN��Ϣ[%d]��", idAccount);
                        return false;
                    }

                    // �������߱�������������ͬ��
                    if (!g_pOnlineTable->IsOnline(idAccount))		//? ��������APPENDNEW()�ķ�ԭ���Բ���
                    {
                        DWORD	nAuthenID = 0xCCCCCCCC;		// ������֤��
                        ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                        if (g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
                        {
                            char* pName = g_cDatabase.GetName();
                            if (!g_pOnlineTable->AppendNew(idAccount, nAuthenID, "StartFee", pName, "",
//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);
                                                           c_typeNone, m_aServerName[nServerIndex], idAccount, pName))
                            {
                                Kickout_0(idAccount, m_aServerName[nServerIndex]);
                            }
                            else
                                //?if (g_bEnableLogin)
                                LOGERROR("����FEE_BEGIN, ������[%s]�ʺ�[%s][%d]����ΪISPģʽ��Ƶ�ģʽ�����ٿ۵��",
                                         m_aServerName[nServerIndex], pName, idAccount);
                            g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
                        }
                        else
                        {
                            LOGERROR("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]��FEE_BEGIN��Ϣ��δ����",
                                     m_aServerName[nServerIndex], idAccount);
                        }
                        ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    }

//#ifdef	SERVER_X
//					LOGPOINT("[%s]�ϴ�[%d]��START_FEE", m_aServerName[nServerIndex], idAccount);
//#endif	// SERVER_X
                    int nRet = g_pOnlineTable->StartFee(idAccount, m_aServerName[nServerIndex]);		// return -n: ��������ƥ��
                    // ˫�ص�¼������
                    if (nRet < 0)		// <0 : error
                    {
                        LOGERROR("...˫�ص�¼�������START_FEE���ʺ�[%d]�������ߡ�", idAccount);
                        Kickout_0(idAccount, m_aServerName[nServerIndex]);
                    }

                    if (m_aState[nServerIndex] == STATE_NORMAL)
                    {
                        ::SetServerState(nServerIndex, c_flagBegin);
                    }
                    else
                    {
                        ::SetServerState(nServerIndex, c_flagBegin2);
                    }
                }
                break;
            case CMsgFee::FEE_END:
            case CMsgFee::FEE_OFFLINE:
                {
                    // ��Ϣ���
                    if (idAccount == ID_NONE)
                    {
                        LOGERROR("�����FEE_END��Ϣ[%d]��", idAccount);
                        return false;
                    }

                    g_pOnlineTable->EndFee(idAccount, m_aServerName[nServerIndex],
                                           cMsg.m_pInfo->ucType == CMsgFee::FEE_OFFLINE);

                    // ɾ�����߱�
//��ENDFEEɾ��					g_pOnlineTable->Remove(idAccount);				// ����ɾ�����ʺ�
                    if (m_aState[nServerIndex] == STATE_NORMAL)
                    {
                        ::SetServerState(nServerIndex, c_flagEnd);
                    }
                    else
                    {
                        ::SetServerState(nServerIndex, c_flagEnd2);
                    }
                }
                break;
            case CMsgFee::FEE_POINT:
            case CMsgFee::FEE_TICK:
                {
                    // ��Ϣ���
                    if (idAccount == ID_NONE)
                    {
                        LOGERROR("�����FEE_POINT��Ϣ[%d]��", idAccount);
                        return false;
                    }

                    // �������߱�������������ͬ��
                    if (!g_pOnlineTable->IsOnline(idAccount))		//? ��������APPENDNEW()�ķ�ԭ���Բ���
                    {
                        DWORD	nAuthenID = 0xCCCCCCCC;		// ������֤��
                        ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                        if (g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
                        {
                            char* pName = g_cDatabase.GetName();
                            if (!g_pOnlineTable->AppendNew(idAccount, nAuthenID, "PointFee", pName, "",
//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);
                                                           c_typeNone, m_aServerName[nServerIndex], idAccount, pName))
                            {
                                Kickout_0(idAccount, m_aServerName[nServerIndex]);
                            }
                            else
                                //?if (g_bEnableLogin)
                                LOGERROR("����FEE_POINT��������[%s]�ʺ�[%s][%d]����ΪISPģʽ��Ƶ�ģʽ�����ٿ۵��",
                                         m_aServerName[nServerIndex], pName, idAccount);
                            g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
                        }
                        else
                        {
                            LOGERROR("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]��FEE_POINT��Ϣ��δ����",
                                     m_aServerName[nServerIndex], idAccount);
                        }
                        ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    }

                    int		nFeeType		= c_typeNone;
                    char	szServerName[_MAX_NAMESIZE] = "(δ֪)";
                    OBJID	idFeeAccount	= ID_NONE;
                    char	szFeeAccount[_MAX_NAMESIZE] = "(δ֪)";
                    int		nPoint			= 0;
                    if (g_pOnlineTable->GetAttr2(idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))	//? ��������APPENDNEW()�ķ�ԭ���Բ���
                    {
                        if ((nFeeType == c_typePoint || nFeeType == c_typeNetBarPoint
                                || nFeeType == c_typeISP) && cMsg.m_pInfo->ucType != CMsgFee::FEE_TICK)		//? �ǼƵ����Ͳ�����
                        {
                            if (strcmp(szServerName, m_aServerName[nServerIndex]) == 0)
                            {
                                bool	bOk		= false;
                                ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                                if (g_cDatabase.Create(idFeeAccount))	// VVVVVVVVVVVVVVV
                                {
                                    bOk		= true;
                                    nPoint	= g_cDatabase.GetPoint();
                                    g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
                                }
                                ::ReleaseMutex(g_xDatabase);	//------------------------------------------

                                if (bOk)
                                {
                                    if (nPoint > 0)
                                    {
                                        if (g_cDatabase.DecPoint(idFeeAccount))		//?? ����ҪOPEN��ֱ��ʹ��CDATABASE�Ĳ�ѯ��䡣
                                        {
                                            InterlockedIncrement(&s_nPointFee);
                                            InterlockedIncrement(&s_nPointSum);
                                        }
                                        else
                                        {
                                            LOGERROR("���ݿ�����ʺ�ID[%d]�޷��۵㡣��", idFeeAccount);
                                            LOGPOINT("���ݿ�����ʺ�ID[%d]�޷��۵㡣", idFeeAccount);
                                        }
                                    }
                                    else if (KICKOUT_USER_WHEN_NO_POINT)
                                    {
                                        LOGPOINT("������[%s]�����[%d]���ʺ�[%s][%d]�����ľ����ߡ�",
                                                 m_aServerName[nServerIndex], idAccount, szFeeAccount, idFeeAccount);
                                        Kickout_0(idAccount, m_aServerName[nServerIndex]);
                                    }
                                }
                                else
                                {
                                    LOGERROR("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]�ļƵ���Ϣ������δ�Ǽǡ�",
                                             m_aServerName[nServerIndex], idFeeAccount);
                                    LOGPOINT("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]�ļƵ���Ϣ������δ�Ǽ�",
                                             m_aServerName[nServerIndex], idFeeAccount);
                                }
                            }
                            else
                            {
                                LOGERROR("˫�ص�¼�����ID[%d]ʹ���ʺ�[%s][%d]��¼��[%s]�����յ�[%s]�ϴ��˵ĵļƵ���Ϣ����ұ������ߡ�",
                                         idAccount, szFeeAccount, idFeeAccount, szServerName, m_aServerName[nServerIndex]);
                                Kickout_0(idAccount, m_aServerName[nServerIndex]);
                            }
                        }

                        int	nPointCount = g_pOnlineTable->PointFee(idAccount, m_aServerName[nServerIndex]);		// return -n: ��������ƥ��
//							LOGPOINT("[%s]�ϴ�[%d]��POINT_FEE������[%d]��ʣ��[%d]",
//								m_aServerName[nServerIndex], idAccount, nPointCount, nPoint-1);
                        // ˫�ص�¼������
//						if (nPointCount < 0)		// <0 : error
                        {
//							LOGERROR("...˫�ص�¼�������POINT_FEE���ʺ�[%d]�������ߡ�", idAccount);
//							Kickout_0(idAccount, m_aServerName[nServerIndex]);
                        }

                        if (m_aState[nServerIndex] == STATE_NORMAL)
                        {
                            ::SetServerState(nServerIndex, c_flagPoint);
                        }
                        else
                        {
                            ::SetServerState(nServerIndex, c_flagPoint2);
                        }
                    } // GetAttr()
                    else
                    {
                        LOGERROR("������ͻ��FEE_POINT����¼�ʺŲ��ɹ�");
                    }
                }
                break;
            case CMsgFee::HEARTBEAT:
                {
                    /*// ��Ϣ���
                    if (idAccount == ID_NONE)
                    {
                    	LOGERROR("�����HEARTBEAT��Ϣ[%d]��", idAccount);
                    	return false;
                    }*/

                    LOGMSG("���յ�[%s]��Ϸ�������ϴ���������Ϣ.", m_aServerName[nServerIndex]);
                    // �޲�����ONPROCESS�л��Զ�ˢ������ʱ�䡣
                    ::SetServerState(nServerIndex, c_flagHeartbeat);
                }
                break;
            case CMsgFee::SERVER_BUSY:
                {
                    SetServerBusy(nServerIndex);
                }
                break;
            case CMsgFee::SERVER_FULL:
                {
                    SetServerFull(nServerIndex);
                }
                break;
            case CMsgFee::SERVER_CLOSE:
                {
                    PrintText("���յ�������[%s]�Ĺر���Ϣ��", m_aServerName[nServerIndex]);
                    //??? SetServerClose(nServerIndex);
                    //??? ::SetServerState(nIndex, c_flagOffline);
                }
                break;
            default:
                LOGERROR("��Ϸ������[%s]�Ʒ���Ϣ������[%d]����", m_aServerName[nServerIndex], cMsg.m_pInfo->ucType);
            }
        }
        break;
    case _MSG_QUERYFEE:
        {
            if (g_aServerAccount[nServerIndex].m_b91U)
            {
                LOGERROR("�������յ�_MSG_QUERYFEE��Ϣ��");
                return false;
            }

            CMsgQueryFee	cMsg;
            cMsg.Create(pBuf, nLen);

            // ��Ϣ���
            if (cMsg.m_pInfo->idAccount == ID_NONE)
            {
                LOGERROR("�����_MSG_QUERYFEE��Ϣ[%d]��", cMsg.m_pInfo->idAccount);
                return false;
            }

            int		nFeeType	= c_typeNone;
            DWORD	dwData		= 0;
            int		nTime		= 0;
            char	szServerName[_MAX_NAMESIZE];
            OBJID	idFeeAccount;
            char	szFeeAccount[_MAX_NAMESIZE];
            if (g_pOnlineTable->GetAttr2(cMsg.m_pInfo->idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))
            {
                switch(nFeeType)
                {
                case	c_typePoint:
                    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                    if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    {
                        dwData	= g_cDatabase.GetPoint();
                        g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                    }
                    else
                    {
                        LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
                    }
                    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    break;
                case	c_typeTime:
                    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                    if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    {
                        nTime	= g_cDatabase.GetPointTime();
                        g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                    }
                    else
                    {
                        LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
                    }
                    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    break;
                case	c_typeNetBarTime:
                    /* �ݲ�����
                    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                    if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    {
                    	dwData	= g_cDatabase.GetLicence();
                    	g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                    }
                    else
                    {
                    	LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
                    }
                    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    break;
                    //*/
                case	c_typeNetBarPoint:
                case	c_typeISP:
                case	c_typeFree:
                    break;
                }
            }
            /*
            			::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
            			if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
            			{
            				nType	= g_cDatabase.GetType();
            				dwData	= g_cDatabase.GetPoint();
            				nTime	= g_cDatabase.GetPointTime();
            				g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
            			}
            			else
            			{
            				LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪���[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
            			}
            			::ReleaseMutex(g_xDatabase);	//------------------------------------------
            //*/
            // ������Ϣ
            cMsg.m_pInfo->ucType	= nFeeType;
            cMsg.m_pInfo->dwData	= dwData;
            cMsg.m_pInfo->nTime		= nTime;
            m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
//			LOGPOINT("���͸�[%s]��Ϸ������[%d]�ʺŵļƷѲ�ѯ��Ϣ������[%d]������[%d]��ʱ��[%04d-%02d-%02d]",
//						m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount,
//						nFeeType, dwData, nTime/10000, (nTime/100)%100, nTime%100);
        }
        break;
    default:
        LOGERROR("��Ϸ�������ϴ���δ֪����Ϣ����[%d]", unMsgType);
        LOGPOINT("���յ�[%s]��Ϸ�������ϴ���δ֪��Ϣ.", m_aServerName[nServerIndex], unMsgType);
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CPointThread::ProcessMsgAccount(int nIndex, char* pBuf, int nLen)
{
    CMsgAccount91U	cMsg;
    cMsg.Create(pBuf, nLen);
#ifdef	RC5ENCRYPT
    m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
#endif
    FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// ��β0��ɾβ��
    FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// ��β0��ɾβ��
    FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// ��β0��ɾβ��
    FormatStr(cMsg.m_pInfo->szIPAddr, _MAX_NAMESIZE);		// ��β0��ɾβ��

    if (cMsg.m_pInfo->szAccount[0] == 0 || cMsg.m_pInfo->szIPAddr[0] == 0 || cMsg.m_pInfo->szServer[0] == 0)
    {
        return false;
    }

    OBJID	id91U							= cMsg.m_pInfo->id91U;
    char* 	pClientIP						= cMsg.m_pInfo->szIPAddr;
    unsigned long nClientIP					= inet_addr(pClientIP);
    DWORD	nAuthenID						= NewAuthenID(nClientIP);
    char* 	pErrMsg							= "��¼ʧ��";
    char	szNotifyMsg[_MAX_NAMESIZE]		= "";
    OBJID	idAccount						= ID_NONE;
    OBJID	idFeeAccount					= ID_NONE;
    char	szFeeAccount[_MAX_NAMESIZE]		= "";
    int		nFeeType						= c_typeNone;
    int		nPoint							= 0;
    DWORD	nPointTime						= 0;
    int		nBlock							= false;
    bool	bRejoin							= false;
    char	szOldServerName[SERVERNAMESIZE]	= "";
    char	szOldFeeAccount[_MAX_NAMESIZE]	= "";
    char 	szPassword[_MAX_NAMESIZE]		= "";		// ���ñ���
    OBJID	idOldFeeAccount					= ID_NONE;	// ���ñ���
    char	szOldIP[_MAX_NAMESIZE]			= "";		// ���ñ���

    if (g_pOnlineTable->GetAttr(cMsg.m_pInfo->szAccount, idAccount, szPassword, nAuthenID,			// ��nAuthenID��szOldServerName��szOldFeeAccount ������
                                szOldServerName, szOldFeeAccount, idOldFeeAccount, szOldIP))		//??? ���ߡ��ú�����������ADDNEW�ķ�ԭ���Բ���
    {
        bRejoin	= true;
    }

    if (!::GetAccount(cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,			// cMsg.m_pInfo->szPassword: ����Ϊ""
                      idAccount, nFeeType, nPoint, nPointTime, nBlock))			// ����ֵ
    {
        idAccount	= ID_NONE;
        if (/*ENABLE_91U_CREATE_ACCOUNT*/1 && cMsg.m_pInfo->szPassword[0] == 0)
        {
            char szSQL[1024];
            sprintf("INSERT account SET name='%s',type=6,point=0", cMsg.m_pInfo->szAccount);
            if (g_db.ExecuteSQL(szSQL))
            {
                idAccount	= g_db.GetInsertId();
                nFeeType	= c_typePoint;
                nPoint		= 0;
                nPointTime	= 0;
                nBlock		= false;
            }
        }

        if (idAccount == ID_NONE)
        {
            AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
            RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errPassword, "�ʺ���������");
            LOGACCOUNT("���[%s]91U[%u]��¼���ʺ���������, IP��ַ[%s]",
                       cMsg.m_pInfo->szAccount, id91U, pClientIP);
            return false;
        }
    }

    // ����
    if (nBlock)
    {
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errBan, "���ʺű����");
        return false;
    }

    // ���Ƿ���¼
    if (nFeeType == c_typeNetBarPoint || nFeeType == c_typeNetBarTime || nFeeType == c_typeISP)
    {
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errBarPassword, "���ʺŲ��ܵ�¼");
        return false;
    }

    if (IsFreeServer(cMsg.m_pInfo->szServer))
    {
        // ��ѷ�����
        nFeeType		= c_typeFree;
//		SafeCopy(szNotifyMsg, "��ӭ�μӲ���", _MAX_NAMESIZE);
    }
    else if (CheckISP(idAccount, nClientIP, pClientIP,
                      idFeeAccount, szFeeAccount, nPoint, nPointTime, szNotifyMsg))		// ����ֵ
    {
        // ISP����
        nFeeType		= c_typeISP;
    }
    else
    {
        idFeeAccount	= idAccount;
        SafeCopy(szFeeAccount, cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);
    }

    if (bRejoin)
    {
        // ������ͬʱ��¼����һ̨������������������ͼƷ��ʺ����Ƿ���ͬ
        if (strcmp(cMsg.m_pInfo->szServer, szOldServerName) != 0
                || strcmp(szFeeAccount, szOldFeeAccount) != 0)	// �ϴ����������ʺŻ�ISP�ϵ�
        {
            if (!g_pOnlineTable->IsTimeWait(idAccount))
            {
                g_pPointThread->Kickout(idAccount);    // ֪ͨ��Ϸ����������
            }

            RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errOnline, "���Ժ����µ�¼");
            return false;
        }
    }

    // ���������
    switch(nFeeType)
    {
    case c_typePoint:	// û�д����Ƶ㿨��ʱ��(�㿨ҲӦ����ʱ��)
    case c_typeISP:
        if (nPoint <= 0)
        {
            if (ENABLE_LOGIN_NO_POINT)
            {
                SafeCopy(szNotifyMsg, FLAG_NO_POINT, _MAX_NAMESIZE);
            }
            else
            {
                RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errNotPoint, "Сʱ�����þ�");
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
                    RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errNotTime, "�ʺ��ѹ���");
                    return false;
                }
            }
            else
            {
                // ����Ƿ�ʣ7��
                if (szNotifyMsg[0] == 0)		// û��֪ͨ��Ϣʱ
                {
                    Check7DaysNodify(nPointTime, szNotifyMsg);		// return szNotifyMsg
                }
            }
        }
        break;
    case c_typeFree:
        break;
    default:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errUnknown, "���ݿ����");
        LOGERROR("���[%s](91U:%d)�Ʒ����ʹ���[%d]������������", cMsg.m_pInfo->szAccount, id91U, nFeeType);
        return false;
    }

    // ��¼
    ASSERT(idAccount);

    // ֪ͨ��Ϸ������
    int nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer);
    switch(nRet)
    {
    case CPointThread::ERR_NO_SERVER:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errUnknowServer, "������δ����");
        return false;
    case CPointThread::ERR_BUSY:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errServerBusy, "������æ���Ժ�");
        return false;
    case CPointThread::ERR_FULL:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errServerFull, "��������������");
        return false;
    case CPointThread::ERR_NONE:
        {
            if (bRejoin)
            {
                // ���½�����Ϸ���޸�һЩ����
                if (!g_pOnlineTable->Rejoin(idAccount, nAuthenID, pClientIP,
                                            cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                            nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount))
                {
                    if (!g_pOnlineTable->IsTimeWait(idAccount))
                    {
                        g_pPointThread->Kickout(idAccount);    // ֪ͨ��Ϸ����������
                    }

                    RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errOnline, "���Ժ����µ�¼");
                    return false;
                }

                AllowLogin(nIndex, id91U, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
                LOGACCOUNT("ͬ��91U[%d]���[%s][%d]��[%d]�������µ�¼[%s]����֤ID[%08X]��IP[%s]���Ʒ��ʺ�[%s]��֪ͨ[%s]",
                           id91U, cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
                           nAuthenID, pClientIP, szFeeAccount, szNotifyMsg);
            }
            else
            {
                // �������߱�
                if (!g_pOnlineTable->AddNew(idAccount, nAuthenID, pClientIP,
                                            cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                            nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount))
                {
                    if (!g_pOnlineTable->IsTimeWait(idAccount))
                    {
                        g_pPointThread->Kickout(idAccount);    // ֪ͨ��Ϸ����������
                    }

                    RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errOnline, "���Ժ����µ�¼");
                    return false;
                }

                AllowLogin(nIndex, id91U, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
                LOGACCOUNT("ͬ��91U[%d]���[%s][%d]��[%d]���͵�¼[%s]����֤ID[%08X]��IP[%s]���Ʒ��ʺ�[%s]��֪ͨ[%s]",
                           id91U, cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
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
                    nAuthenID, pClientIP, szFeeAccount, szNotifyMsg, bRejoin + 2);		// +2 for 91U
            g_db.ExecuteSQL(szSQL);
            return true;
        }
        break;
    default:
        ASSERT(!"NewLogin");
        return false;
    }
}

void CPointThread::AddBan(DWORD id91U, LPCTSTR szClientIP, LPCTSTR szAccount)
{
    // ����BAN��
    bool	bFoundBan = false;
    int		nFreeSlot = -1;
    int		nBanCount = 0;
    // �Ҹ���λ��
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

        if (m_pBanIPs[i].ClientIP() == id91U)	// �Ѽ�¼
        {
            bFoundBan = true;
            m_pBanIPs[i].IncError();				//? �������ߣ���������
            if (m_pBanIPs[i].IsBan())
            {
                LOGERROR("ĳ�ͻ����ѷǷ���¼[%d]�Σ��������˹����ʺŷ�������91Uid[%u], IP��ַ[%s]", BANERRORS, id91U, szClientIP);
                LOGACCOUNT("ĳ���[%s]��¼���ʺŷ�������������[%d]�Ρ�91U[%u](IP��ַ[%s])����ֹ[%d]����",
                           szAccount, BANERRORS, id91U, szClientIP, BANSECS);
                PrintText("IP��ַ��%s��(91U:%u)����������%d����, ��%d�����ڽ��޷���¼",
                          szClientIP, id91U, BANERRORS, BANSECS);
            }
        }
    }
    if (!bFoundBan)
    {
        // ������BAN
        if (nFreeSlot != -1)
        {
            m_pBanIPs[nFreeSlot].Create(id91U);
            if (nBanCount * 100 / MAXBANIPS > 78)		// ������
            {
                LOGWARNING("����BAN��[%d/%d], 91U[%u]��IPΪ[%s]", nBanCount + 1, MAXBANIPS, id91U, szClientIP);
            }
        }
        else
        {
            LOGERROR("BAN ��̫С���и�91U[%u](IP[%s])û���ȥ�����ʵ����� MAXBANIPS ����", id91U, szClientIP);
        }
    }
}

void CPointThread::AllowLogin(int nServerIndex, DWORD id91U, OBJID idAccount, DWORD nAuthenID, LPCTSTR szServer)
{
    CMsgConnect91U cRetMsg;
    char	szServerIP[IPSTRSIZE] = "";
    g_pPointThread->GetServerIP(szServerIP, szServer);
    cRetMsg.Create(id91U, idAccount, nAuthenID, szServerIP);		//? �´���Ϸ��������IP��ַ
    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
//!	if (!STAT_SERIAL)
//!		m_aServerSocket[nServerIndex].ShutDown();		//?? �رգ��öԷ��ȹر�(ע�⣺���û�յ�CMsgConnect��Ϣ�򲻻�ر�)
}

void CPointThread::RefuseLogin(int nServerIndex, DWORD id91U, LPCTSTR szLoginName, int nType, LPCTSTR szText)
{
    CMsgConnect91U cRetMsg;
    cRetMsg.Create(id91U, ID_NONE, nType, (char*)szText);
    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
//!	m_aServerSocket[nServerIndex].ShutDown();		//?? �رգ��öԷ��ȹر�
    LOGACCOUNT("ͨ��91U[%d]��¼���ɹ�[%s]��ԭ����[%s][%d]", id91U, szLoginName, szText, nType);
}

//////////////////////////////////////////////////////////////////////
// ����0������
OBJID	CPointThread::GetServerIndex_0(const char* pServerName)
{
    int i = 1;
    for(; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
    {
        if (strcmp(m_aServerName[i], pServerName) == 0)
        {
            break;
        }
    }

    if (i < MAXGAMESERVERS)
    {
        if (m_aServerSocket[i].IsOpen())
        {
            return i;
        }
        else
        {
            if (m_aServerName[i][0])
            {
                LOGWARNING("ȡ������IDʱ����Ϸ������[%s]�Ѿ��ر�", m_aServerName[i]);
                LOGSERVER("������[%s]�Ѿ��ر��ˡ�", m_aServerName[i]);
                PrintText("��������%s���Ѿ��ر��ˡ�", m_aServerName[i]);
            }
            Clear(i);		// SOCKET�رպ�ɾ����Ϸ��������������ͬ��
        }
    }

    return 0;
}

// ���������Ƿ����ߣ�������ʱת����ʾ״̬����������ҵ�¼��
bool	CPointThread::CheckHeartbeat(int nIndex)
{
    // �ָ�BUSY��FULL
    if ((m_aState[nIndex] == STATE_BUSY || m_aState[nIndex] == STATE_FULL) && clock() > m_aServerDelay[nIndex])
    {
        m_aState[nIndex] = STATE_NORMAL;
        ::SetServerState(nIndex, c_flagNormal);
    }

    bool	ret = false;
    // �������
    if (m_aHeartbeatLast[nIndex] && (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATKICKSECS)
    {
        LOGWARNING("��Ϸ������[%s]��Ϊ��Ӧ��ʱ[%d]�룬���ӱ�ǿ�жϿ�",
                   m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC);
        LOGSERVER("��Ӧ��ʱ��������[%s]�ѶϿ�����!", m_aServerName[nIndex]);
        PrintText("��Ӧ��ʱ����������%s���ѶϿ�����!", m_aServerName[nIndex]);
        m_aServerSocket[nIndex].Close();
        Clear(nIndex);
        ret = true;
    }
    else if ((clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS
             && (clock() - m_aHeartbeatLast[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS)
    {
        CMsgFee	cMsg;
        cMsg.Create(666666, CMsgFee::HEARTBEAT);		// 666666: ����ʺ�ID��������
        if (m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize()))
        {
            LOGERROR("��Ϸ������[%s]��Ӧ��ʱ[%d]�룬�ѷ���һheartbeat��Ϣ",
                     m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC);
        }
        else
        {
            LOGWARNING("��Ϸ������[%s]�����ѹرգ��޷�SEND������Ϣ", m_aServerName[nIndex]);
            LOGSERVER("��������ʧ�ܣ�������[%s]�ѶϿ�����!", m_aServerName[nIndex]);
            PrintText("��������ʧ�ܣ���������%s���ѶϿ�����!", m_aServerName[nIndex]);
            Clear(nIndex);
        }
        m_aHeartbeatLast[nIndex] = clock();
    }

    return ret;
}

void CPointThread::Clear(int nIndex, bool flag /*= true*/)
{
    m_aState[nIndex] = STATE_OFFLINE;
    m_aServerName[nIndex][0] = m_aHeartbeat[nIndex] = m_aHeartbeatLast[nIndex] = m_aServerDelay[nIndex] = 0;
    if (flag)
    {
        ::SetServerState(nIndex, c_flagOffline);
    }
}

void CPointThread::SetServerBusy(int nServerIndex)
{
    m_aServerDelay[nServerIndex] = clock() + SERVER_BUSY_DELAY_SECS * CLOCKS_PER_SEC;
    m_aState[nServerIndex] = STATE_BUSY;
    ::SetServerState(nServerIndex, c_flagStop);
}

void CPointThread::SetServerFull(int nServerIndex)
{
    m_aServerDelay[nServerIndex] = clock() + SERVER_FULL_DELAY_SECS * CLOCKS_PER_SEC;
    m_aState[nServerIndex] = STATE_FULL;
    ::SetServerState(nServerIndex, c_flagStop);
}
