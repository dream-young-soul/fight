
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "protocol.h"
#include "common.h"
#include "IniFile.h"
#include "LogFile.h"
#include "I_Shell.h"
#include "SocketThread.h"
#include "WorldThread.h"
#include "MapGroupThread.h"
#include "MessagePort.h"
#include "P_ServerManager.h"
#include "basetype.h"
#include "array.h"
#include "mystack.h"
#include "I_MessagePort.h"

#include "MsgServer.h"
#include "MsgServerDlg.h"

typedef	CMyStack<const char*>	DEADLOOP;
typedef	Array<DEADLOOP>		DEADLOOP_SET;
DEADLOOP_SET	g_setDeadLoop;
inline DEADLOOP_SET&	DeadLoopSet() { return g_setDeadLoop; }
/////////////////////////////////////////////////////////////////////////////
ST_CONFIG		CONFIG;

/////////////////////////////////////////////////////////////////////////////
struct STAT_STRUCT	g_stat = {0};
long				g_nRestart		= false;
long				g_nServerClosed = false;

long				s_nDatabaseTimeSum;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgServerDlg dialog

CMsgServerDlg::CMsgServerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMsgServerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMsgServerDlg)
    m_sKernelState = _T("");
    m_sShellState = _T("");
    m_sTalk = _T("");
    m_sText = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nState		= SHELLSTATE_NONE;
    m_nTextLines	= 0;
    m_pMsgPort		= NULL;
    m_pInterPort		= NULL;
    m_pSocketThread	= NULL;
    m_pWorldThread	= NULL;
    m_hMutexThread	= NULL;
    m_nAllPlayers	= 0;
    m_nMaxPlayers	= 0;
}

void CMsgServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgServerDlg)
    DDX_Text(pDX, IDC_KERNEL, m_sKernelState);
    DDX_Text(pDX, IDC_SHELL, m_sShellState);
    DDX_Text(pDX, IDC_TALK, m_sTalk);
    DDX_Text(pDX, IDC_TEXT, m_sText);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgServerDlg, CDialog)
    //{{AFX_MSG_MAP(CMsgServerDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_SEND, OnSend)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgServerDlg message handlers

BOOL CMsgServerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    // TODO: Add extra initialization here
#if	!defined(PALED_DEBUG)
#ifdef _DEBUG
    //this->MessageBox("这个版本的服务器端,只可以在内部运行,绝对禁止在外部运行!", "Warning", MB_OK | MB_ICONEXCLAMATION);
#endif
#endif
    memset(&g_stat, 0, sizeof(g_stat));
    DateTime(m_szStartServer, time(NULL));
    m_hMutexServer = ::CreateMutex(NULL, false, "ConquerServer");
    if (m_hMutexServer)
    {
        if (ERROR_ALREADY_EXISTS == ::GetLastError())
        {
            ::ReleaseMutex(m_hMutexServer);
            ::CloseHandle(m_hMutexServer);
            m_hMutexServer = NULL;
            MessageBox("Repeat run game server!");
            this->EndDialog(-1);
            return false;
        }
    }
    else
    {
        MessageBox("Create mutex failed!");
        this->EndDialog(-1);
        return false;
    }
#ifdef	DEBUG_MULTITHREAD
    m_hMutexThread    =::CreateMutex(NULL, false, "FW_DEBUG_MULTITHREAD");
    if (!m_hMutexThread)
    {
        PrintText("Create mutex handle failed!");
        return false;
    }
#endif
    if (!LoadConfigIni())
    {
        MessageBox("Load config.ini failed!");
    }
    else if (!CMessagePort::InitPortSet(MSGPORT_MAPGROUP_FIRST + CONFIG.MAPGROUP_SIZE))
    {
        MessageBox("Initial intra message port failed!");
    }
    else
    {
        m_pMsgPort = CMessagePort::GetInterface(MSGPORT_SHELL);
        m_pMsgPort->Open();
        m_nState	= SHELLSTATE_INIT;
        if (CONFIG.CURRENT_PORTID)
        {
            m_pInterPort = CInternetPort::CreateNew(CONFIG.CURRENT_PORTID, CONFIG.PORT_SIZE, CONFIG.MASTER_IP, CONFIG.MASTER_PORT, CONFIG.LOGIN_KEY);
            if (m_pInterPort)
            {
                m_pInterPort->Open();
            }
        }
    }
    // dead loop init
    for(int i = 0; i < CONFIG.MAPGROUP_SIZE + MSGPORT_MAPGROUP_FIRST; i++)
    {
        DeadLoopSet().Push(DEADLOOP());
    }
    // get game title
    CIniFile	ini("shell.ini", "AccountServer");
    ini.GetString(m_szServer, "SERVERNAME", _MAX_NAMESIZE);
    // windows title
    //CString strTitle;
    //strTitle.Format("%s - %s (%s %s)", GAME_TITLE, m_szServer, __DATE__, __TIME__);
    SetWindowText(GAME_TITLE/*strTitle*/);
    // init log file
    CreateDirectory(LOGFILE_DIR, NULL);
    InitLog(GAME_TITLE/*strTitle*/, LOGFILE_FILENAME, time(NULL));
    LOGMSG("\n\n\n=================================================================");
    LOGMSG(GAME_TITLE/*strTitle*/);
    LOGMSG("=================================================================");
    SetTimer(1, 200, NULL);				// 句柄为1，刷新间隔为200MS
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMsgServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else if ((nID & 0xFFF0) == SC_CLOSE)
    {
        PostMessage(WM_COMMAND, IDCANCEL);
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMsgServerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMsgServerDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CMsgServerDlg::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
#ifdef	DEBUG_MULTITHREAD
    if (::WaitForSingleObject(m_hMutexThread, INFINITE) == WAIT_ABANDONED)
    {
        return ;
    }
#endif
    static	int nLock = 0;
    if (nIDEvent == 1 && nLock <= 0)
    {
        nLock++;	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //!!! Can't use 'return' in this block
        if (m_pMsgPort)
        {
            Process();
        }
        DEBUG_TRY		// VVVVVVVVVVVVVVVVVVVVVVVVVVVV
        if (m_pInterPort)
        {
            ProcessInterMsg();
        }
        DEBUG_CATCH("ProcessInterMsg()")		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        extern long				g_nRestart;
        switch(m_nState)
        {
        case	SHELLSTATE_NONE:
            {
                m_nState	= SHELLSTATE_END;
            }
            break;
        case	SHELLSTATE_INIT:
            {
                m_nState	= SHELLSTATE_CLOSING;		// 失败时退出
                PrintText("1、Init world thread...");
                m_pWorldThread = new CWorldThread(CMessagePort::GetInterface(MSGPORT_WORLD));
                if (!m_pWorldThread)
                {
                    break;
                }
                if (!m_pWorldThread->CreateThread(false))
                {
                    break;
                }
                PrintText("2、Init map group thread...");
                int i = 0;
                for(; i < CONFIG.MAPGROUP_SIZE; i++)
                {
                    CMapGroupThread* pThread = new CMapGroupThread(CMessagePort::GetInterface(i + MSGPORT_MAPGROUP_FIRST));
                    if (!pThread)
                    {
                        break;    // for
                    }
                    m_setMapGroupThread.push_back(pThread);
                    if (!pThread->CreateThread(false))
                    {
                        break;    // for
                    }
                }
                if (i < CONFIG.MAPGROUP_SIZE)
                {
                    break;
                }
                PrintText("3、Init socket thread...");
                m_pSocketThread = new CSocketThread(CMessagePort::GetInterface(MSGPORT_SOCKET));
                if (!m_pSocketThread)
                {
                    break;
                }
                if (!m_pSocketThread->CreateThread(false))
                {
                    break;
                }
                PrintText("Init OK。");
                m_pWorldThread->ResumeThread();
                for(int i = 0; i < CONFIG.MAPGROUP_SIZE; i++)
                {
                    m_setMapGroupThread[i]->ResumeThread();
                }
                m_pSocketThread->ResumeThread();
                PrintText("All thread start OK。");
                m_nState	= SHELLSTATE_RUNNING;
            }
            break;
        case	SHELLSTATE_RUNNING:
            {
                // 状态刷新
                const int nSecsPerLogin = 5 * 60;
                if (m_tStat5Min.ToNextTime(nSecsPerLogin))
                {
                    m_nLoginPlayers		= InterlockedExchange(&g_stat.nLoginPlayers, 0);
                    m_nLogoutPlayers	= InterlockedExchange(&g_stat.nLogoutPlayers, 0);
                    LOGMSG("\n-----------------------------------------------\n"
                           "%s"
                           "-----------------------------\n"
                           "%s"
                           "-----------------------------------------------\n",
                           m_sShellState, m_sKernelState);
                }
                const int nSecsPerUpdate = 5;
                if (m_tStat.ToNextTime(nSecsPerUpdate))
                {
                    long lDatabaseTime	= ::InterlockedExchange(&s_nDatabaseTimeSum, 0);
                    memcpy(&m_stat, &g_stat, sizeof(g_stat));
                    STAT_STRUCT stat;
                    memset(&stat, 0L, sizeof(STAT_STRUCT));
                    stat.nAllPlayers	= m_stat.nAllPlayers;
                    memcpy(&g_stat, &stat, sizeof(STAT_STRUCT));
                    memcpy(&g_stat, &stat, sizeof(STAT_STRUCT));
                    //					memset(&g_stat, 0, sizeof(g_stat));
                    //					memset(&g_stat, 0, sizeof(g_stat));		// 保证清空:)
                    //					g_stat.nAllPlayers	= m_stat.nAllPlayers;
                    SafeCopy(m_stat.szStartServer, m_szStartServer, 20);
                    DateTime(m_stat.szCurr, time(NULL));
                    //					if (m_stat.nAllPlayers)
                    m_nAllPlayers = m_stat.nAllPlayers;
                    if (m_stat.nMaxPlayers)
                    {
                        m_nMaxPlayers = m_stat.nMaxPlayers;
                    }
                    // update
                    m_sShellState.Format("start server: %s\r\n"
                                         "current: %s\r\n\r\n"
                                         "socket thread: %3d(max:%d)\r\n"
                                         "world thread:  %3d(max:%d)\r\n"
                                         "map group 0 thread: %3d(max:%d)[%3d]\r\n"
                                         "map group 1 thread: %3d(max:%d)\r\n"
                                         "map group 2 thread: %3d(max:%d)\r\n"
                                         "map group 3 thread: %3d(max:%d)\r\n"
                                         "map group 4 thread: %3d(max:%d)\r\n"
                                         "debug [%d][%d][%d][%d][%d]\r\n"
                                         "OnTimer [%3d] Database [%3d]\r\n"
                                         , m_stat.szStartServer
                                         , m_stat.szCurr
                                         , m_stat.nSocketTimes ? m_stat.nAllSocketMS / m_stat.nSocketTimes : 0
                                         , m_stat.nMaxSocketMS
                                         , m_stat.nWorldTimes ? m_stat.nAllWorldMS / m_stat.nWorldTimes : 0
                                         , m_stat.nMaxWorldMS
                                         , m_stat.setMapGroupTimes[0] ? m_stat.setAllMapGroupMS[0] / m_stat.setMapGroupTimes[0] : 0
                                         , m_stat.setMaxMapGroupMS[0]
                                         , m_stat.nPacketID
                                         , m_stat.setMapGroupTimes[1] ? m_stat.setAllMapGroupMS[1] / m_stat.setMapGroupTimes[1] : 0
                                         , m_stat.setMaxMapGroupMS[1]
                                         , m_stat.setMapGroupTimes[2] ? m_stat.setAllMapGroupMS[2] / m_stat.setMapGroupTimes[2] : 0
                                         , m_stat.setMaxMapGroupMS[2]
                                         , m_stat.setMapGroupTimes[3] ? m_stat.setAllMapGroupMS[3] / m_stat.setMapGroupTimes[3] : 0
                                         , m_stat.setMaxMapGroupMS[3]
                                         , m_stat.setMapGroupTimes[4] ? m_stat.setAllMapGroupMS[4] / m_stat.setMapGroupTimes[4] : 0
                                         , m_stat.setMaxMapGroupMS[4]
                                         , m_stat.setDebug[0], m_stat.setDebug[1], m_stat.setDebug[2], m_stat.setDebug[3], m_stat.setDebug[4]
                                         , m_stat.setMapGroupTimerMS[0], lDatabaseTime
                                        );
                    m_sKernelState.Format("players: %3d(max:%d)\r\n"
                                          "login: %d/5min, logout: %d/5min\r\n\r\n"
                                          "socket bytes: %3d/s (pps: %d)\r\n"
                                          "npc socket bytes: %3d/s (pps: %d)\r\n"
                                          , m_nAllPlayers, m_nMaxPlayers
                                          , m_stat.nLoginPlayers, m_stat.nLogoutPlayers
                                          , m_stat.nSocketBytes / nSecsPerUpdate, m_stat.nSocketPackets / nSecsPerUpdate
                                          , m_stat.nNpcSocketBytes / nSecsPerUpdate, m_stat.nNpcSocketPackets / nSecsPerUpdate
                                         );
                    UpdateData(false);
                    FILE* pFile = fopen(ONLINE_FILENAME, "wt");
                    if (pFile)
                    {
                        fprintf(pFile, m_sShellState);
                        fprintf(pFile, "\n\n");
                        fprintf(pFile, m_sKernelState);
                        fclose(pFile);
                    }
                }
                // dead loop
                extern long g_nMessagePortErrorCount;
                if (g_nMessagePortErrorCount > 0 && g_nMessagePortErrorCount % 50000 == 0)
                {
                    InterlockedExchange(&g_nMessagePortErrorCount, 0);
                    PrintText("Message pipe block, server may be slowly!!!");
                    LOGERROR("dump all call stack:");
                    while(DeadLoopSet()[MSGPORT_MAPGROUP_FIRST].size())
                    {
                        LOGERROR("* %s", (LPCTSTR)DeadLoopSet()[MSGPORT_MAPGROUP_FIRST].pop());
                    }
                }
                // restart server
                extern long g_nRestart;
                if (g_nRestart)
                {
                    m_nState = SHELLSTATE_CLOSING;
                }
            }
            break;
        case	SHELLSTATE_CLOSING:
            {
                // 关闭核心
                PrintText("Kernel closing...");
                int i;
                //*
                // 通知线程关闭
                if (m_pSocketThread)
                {
                    m_pSocketThread->CloseThread();
                }
                for(int i = 0; i < m_setMapGroupThread.size(); i++)
                {
                    m_setMapGroupThread[i]->CloseThread();
                }
                if (m_pWorldThread)
                {
                    m_pWorldThread->CloseThread();
                }
                //*/
                // 等待线程关闭
                if (m_pSocketThread)
                {
                    if (m_pSocketThread->CloseThread(CLOSETHREAD_MILLISECS))
                    {
                        PrintText("Close socket OK。");
                    }
                    else
                    {
                        PrintText("Close socket failed!");
                    }
                    SAFE_DELETE(m_pSocketThread);
                    Process();
                }
                for(int i = 0; i < m_setMapGroupThread.size(); i++)
                {
                    char szText[1024];
                    if (m_setMapGroupThread[i]->CloseThread(CLOSETHREAD_MILLISECS))
                    {
                        sprintf(szText, "Close map group thread[%d] OK。", i);
                    }
                    else
                    {
                        sprintf(szText, "Close map group thread[%d] failed!", i);
                    }
                    PrintText(szText);
                    SAFE_DELETE(m_setMapGroupThread[i]);
                    Process();
                }
                if (m_pWorldThread)
                {
                    if (m_pWorldThread->CloseThread(CLOSETHREAD_MILLISECS))
                    {
                        PrintText("Close world thread OK。");
                    }
                    else
                    {
                        PrintText("Close world thread failed!");
                    }
                    SAFE_DELETE(m_pWorldThread);
                    Process();
                }
                if (m_hMutexServer)
                {
                    ::WaitForSingleObject(m_hMutexServer, 10 * 1000);
                    ::ReleaseMutex(m_hMutexServer);
                    ::CloseHandle(m_hMutexServer);
                    m_hMutexServer = NULL;
                }
                PrintText("Server is over, close all  after 3 second!");
                Process();
                m_pMsgPort->Close();
                CMessagePort::ClearPortSet();
                m_pMsgPort = NULL;
                if (m_pInterPort)
                {
                    SAFE_DELETE(m_pInterPort);
                }
                m_nState	= SHELLSTATE_END;
            }
            break;
        case	SHELLSTATE_END:
            {
                KillTimer(1);
#ifdef	DEBUG_MULTITHREAD
                if (m_hMutexThread)
                {
                    ::CloseHandle(m_hMutexThread);
                    m_hMutexThread = NULL;
                }
#endif
                Sleep(3000);
                if (g_nRestart)
                {
                    ::ShellExecute(m_hWnd, NULL, AfxGetAppName(), NULL, NULL, SW_SHOWNORMAL);
                }
                CDialog::OnOK();
            }
            break;
        default:
            ASSERT(!"switch(m_nState)");
        }
        nLock--;	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
#ifdef	DEBUG_MULTITHREAD
    ::ReleaseMutex(m_hMutexThread);
#endif
    CDialog::OnTimer(nIDEvent);
}

void CMsgServerDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    PrintText("CLOSE key pressed");
    if (AfxMessageBox("Are you sure?", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
    {
        m_nState	= SHELLSTATE_CLOSING;
        PrintText("Server closing...");
    }
    //	CDialog::OnCancel();
}

void CMsgServerDlg::OnSend()
{
    // TODO: Add your control notification handler code here
    this->UpdateData(TRUE);
    if (m_pMsgPort)
    {
        m_pMsgPort->Send(MSGPORT_WORLD, WORLD_SHELLTALK, STRING_TYPE((LPCTSTR)m_sTalk), (LPCTSTR)m_sTalk);
    }
}

bool CMsgServerDlg::Process()
{
    char			buf[MAX_MESSAGESIZE];
    CMessageStatus	cStatus;
    DEBUG_TRY	// VVVVVVVVVVVVVV
    while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
    {
        if (ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
        {
            return true;
        }
        PrintText("ERROR: CMsgServerDlg::Process() ProcessMsg()");
        return false;
    }
    DEBUG_CATCH("CMsgServerDlg::Process()")	// AAAAAAAAAAA
    return true;
}

bool CMsgServerDlg::ProcessInterMsg()
{
    if (!m_pInterPort)
    {
        return false;
    }
    char			buf[MAX_MESSAGESIZE];
    CMessageStatus	cStatus;
    DEBUG_TRY	// VVVVVVVVVVVVVV
    if (!m_pInterPort->IsOpen())
    {
        if (!m_pInterPort->Open())
        {
            return false;
        }
    }
    while(m_pInterPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
    {
        switch(cStatus.m_nPacket)
        {
        case	QUERY_STATUS:
            {
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_TITLE, STRING_TYPE(m_szServer), m_szServer);
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_SHELLMSG, STRING_TYPE(m_sShellState), m_sShellState);
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_KERNELMSG, STRING_TYPE(m_sKernelState), m_sKernelState);
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_TEXT, STRING_TYPE(m_sText), m_sText);
            }
            break;
        case	QUERY_COMMOND:
            {
                LPCTSTR pStr = (LPCTSTR)buf;
                CString str;
                str.Format("【remote】%s", pStr);
                PrintText(str);
                m_pMsgPort->Send(MSGPORT_WORLD, WORLD_SHELLTALK, STRING_TYPE(pStr), pStr);
            }
            break;
        default:
            PrintText("CMsgServerDlg::ProcessInterMsg()");
            return false;
        }
    }
    DEBUG_CATCH("CMsgServerDlg::Process()")	// AAAAAAAAAAA
    return true;
}

void CMsgServerDlg::PrintText(LPCTSTR szMsg)
{
    if (m_nTextLines >= TEXTWINDOW_SIZE)
    {
        int nPos = m_sText.Find("\n", 0);
        if (nPos != -1)
        {
            m_sText = m_sText.Mid(nPos + 1);
        }
    }
    char	buf[20];
    DateTime(buf);
    m_sText += buf + 11;
    m_sText += "【";
    m_sText += szMsg;
    m_sText += "】";
    m_sText += "\r\n";
    m_nTextLines++;
    UpdateData(false);
    LOGMSG("SHELL: %s", szMsg);
}

bool CMsgServerDlg::ProcessMsg(OBJID idPacket, void* pMsg, int nType, int nSource)
{
    switch(idPacket)
    {
    case	SHELL_PRINTTEXT:
        {
            PrintText((char*)pMsg);
        }
        break;
    case	SHELL_KERNELSTATE:
        {
            m_sKernelState = (char*)pMsg;
            UpdateData(false);
        }
    default:
        LOGERROR("Shell process invalid msg [%d].", idPacket);
        return false;
    }
    return true;
}

bool CMsgServerDlg::LoadConfigIni()
{
    CIniFile	ini(CONFIG_FILENAME, "System");
    // 初值
    CONFIG.MAPGROUP_SIZE	= 0;
    CONFIG.CURRENT_PORTID	= 0;
    CONFIG.PORT_SIZE		= 0;
    CONFIG.MASTER_IP[0]		= 0;
    CONFIG.MASTER_PORT		= 0;
    CONFIG.LOGIN_KEY[0]		= 0;
    // 读入
    CONFIG.MAPGROUP_SIZE	= ini.GetInt("MAPGROUP_SIZE");
    if (CONFIG.MAPGROUP_SIZE == 0)
    {
        return false;
    }
    ini.SetSection("InternetPort");
    CONFIG.CURRENT_PORTID	= ini.GetInt("CURRENT_PORTID");
    CONFIG.PORT_SIZE		= ini.GetInt("PORT_SIZE");
    ini.GetString(CONFIG.MASTER_IP, "MASTER_IP", IPSTR_SIZE);
    CONFIG.MASTER_PORT		= ini.GetInt("MASTER_PORT");
    ini.GetString(CONFIG.LOGIN_KEY, "LOGIN_KEY", 256);
    if (CONFIG.MAPGROUP_SIZE == 0)
    {
        return false;
    }
    return true;
}

