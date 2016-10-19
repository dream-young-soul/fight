
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NpcServer.h"
#include "NpcServerDlg.h"
#include "typedef.h"
#include "LogFile.h"
#include "inifile.h"
#include "P_ServerManager.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

long				s_nDatabaseTimeSum;
long	g_nRestart = false;
struct STAT_ST g_stat;
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
// CNpcServerDlg dialog

CNpcServerDlg::CNpcServerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CNpcServerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNpcServerDlg)
    m_sKernel = _T("");
    m_sShell = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nState			= STATE_NONE;
    m_pInterPort		= NULL;
    m_szServerName[0]	= 0;
    m_nTextLines		= 0;
}

void CNpcServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNpcServerDlg)
    DDX_Control(pDX, IDC_TEXT, m_ctlText);
    DDX_Text(pDX, IDC_STATIC_KERNEL, m_sKernel);
    DDX_Text(pDX, IDC_STATIC_SHELL, m_sShell);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNpcServerDlg, CDialog)
    //{{AFX_MSG_MAP(CNpcServerDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNpcServerDlg message handlers

BOOL CNpcServerDlg::OnInitDialog()
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
    m_hMutexServer = ::CreateMutex(NULL, false, "ConquerNpcServer");
    if (m_hMutexServer)
    {
        if (ERROR_ALREADY_EXISTS == ::GetLastError())
        {
            ::CloseHandle(m_hMutexServer);
            m_hMutexServer = NULL;
            MessageBox("ERROR: Repeat run server!");
            this->EndDialog(-1);
            return false;
        }
    }
    else
    {
        MessageBox("Create mutext failed!");
        this->EndDialog(-1);
        return false;
    }
    char	szTitle[1024];
    sprintf(szTitle, "%s (%s %s)", NPCSERVER_TITLE, __DATE__, __TIME__);
    SetWindowText(szTitle);
    ::CreateDirectory(LOGFILE_DIRECTION, NULL);
    ::InitLog(szTitle, LOGFILE_FILENAME, time(NULL));
    LOGMSG("\n\n\n=================================================================");
    LOGMSG(szTitle);
    LOGMSG("=================================================================");
    m_nState	= STATE_STARTING;
    m_tUpdateStateWin.Startup(UPDATESTATEWIN_SECS);
    m_tSaveStateWin.Startup(SAVESTATEWIN_SECS);
    CIniFile	ini(CONFIG_INI_FILENAME, "InternetPort");
    int		nPortSize	= ini.GetInt("PORT_SIZE");
    //	CHECKF(nPortSize);
    char	szMasterIP[16];
    ini.GetString(szMasterIP, "MASTER_IP", 16);
    int		nMasterPort	= ini.GetInt("MASTER_PORT");
    int		nCurrPort	= ini.GetInt("CURRENT_PORTID");
    char	szKey[256];
    ini.GetString(szKey, "LOGIN_KEY", 256);
    ini.SetSection("System");
    ini.GetString(m_szServerName, "SERVERNAME", 16);
    if (nCurrPort && nPortSize)
    {
        m_pInterPort = CInternetPort::CreateNew(nCurrPort, nPortSize, szMasterIP, nMasterPort, szKey);
        if (m_pInterPort)
        {
            m_pInterPort->Open();
        }
    }
    SetTimer(1, 500, NULL);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNpcServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNpcServerDlg::OnPaint()
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
HCURSOR CNpcServerDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CNpcServerDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    PrintText("CLOSE button pressed.");
    ShowText();
    if (AfxMessageBox("Are you CLOSE server new?", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
    {
        m_nState	= STATE_CLOSING;
    }
    //	CDialog::OnCancel();
}

void CNpcServerDlg::PrintText(LPCTSTR szMsg)			// ºËÐÄ¿Éµ÷ÓÃ
{
    LOCKTHREAD;		// m_sUpdate»¥³â
    m_sUpdate	+= "¡¾";
    m_sUpdate	+= szMsg;
    m_sUpdate	+= "¡¿\r\n";
    // use for internet message
    const int TEXTWINDOW_SIZE = 8;
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
    m_sText += "¡¾";
    m_sText += szMsg;
    m_sText += "¡¿";
    m_sText += "\r\n";
    m_nTextLines++;
    LOGMSG("SHELL: %s", szMsg);
}

void CNpcServerDlg::CloseAll()
{
    //	LOCKTHREAD;
    m_nState	= STATE_CLOSING;
}

void CNpcServerDlg::SetState(LPCTSTR szMsg)
{
    LOCKTHREAD;
    m_sKernel = szMsg;
}

void CNpcServerDlg::ChangeEncrypt(DWORD nKey)
{
    LOCKTHREAD;
    //@
}

void CNpcServerDlg::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == 1)
    {
        switch(m_nState)
        {
        case	STATE_STARTING:
            {
                m_nState	= STATE_NORMAL;
                if (!m_thdKernel.CreateThread(this))
                {
                    PrintText("Can't create kernel thread!");
                    ShowText();
                    m_nState	= STATE_CLOSING;
                    break;
                }
                PrintText("Create kernel thread ok.");
                ShowText();
            }
            break;
        case	STATE_NORMAL:
            {
                if (!m_sUpdate.IsEmpty())
                {
                    ShowText();
                }
                if (m_tUpdateStateWin.ToNextTime())
                {
                    SetShellState();
                    if (m_tSaveStateWin.ToNextTime())
                    {
                        LOGMSG(m_sShell);
                        LOGMSG(m_sKernel);
                    }
                    LOCKTHREAD;
                    UpdateData(false);
                }
                ProcessInterMsg();
                // restart server
                extern long g_nRestart;
                if (g_nRestart)
                {
                    m_nState = STATE_CLOSING;
                }
            }
            break;
        case	STATE_CLOSING:
            {
                if (!m_thdKernel.CloseThread(3000))
                {
                    PrintText("Closing kernel thread£¬waiting . . .");
                    ShowText();
                    break;
                }
                PrintText("Close kernel thread ok.");
                ShowText();
                if (m_hMutexServer)
                {
                    ::CloseHandle(m_hMutexServer);
                    ::ReleaseMutex(m_hMutexServer);
                    m_hMutexServer = NULL;
                }
                if (m_pInterPort)
                {
                    SAFE_DELETE(m_pInterPort);
                }
                m_nState	= STATE_CLOSED;
            }
            break;
        case	STATE_CLOSED:
            {
                KillTimer(1);
                m_nState	= STATE_NONE;
                Sleep(1000);
                // restart
                extern long g_nRestart;
                if (g_nRestart)
                {
                    ::ShellExecute(m_hWnd, NULL, AfxGetAppName(), NULL, NULL, SW_SHOWNORMAL);
                }
                CDialog::OnCancel();
            }
            break;
        }
    }
    CDialog::OnTimer(nIDEvent);
}


void CNpcServerDlg::ShowText()
{
    if (!m_sUpdate.IsEmpty())
    {
        LOCKTHREAD;		// m_sUpdate»¥³â
        if (m_ctlText.GetLineCount() > MAX_TEXTLINESIZE)
        {
            m_ctlText.SetSel(0, m_ctlText.LineIndex(MAX_TEXTLINESIZE / 2), true);
            m_ctlText.ReplaceSel("");
        }
        m_ctlText.SetSel(END_OF_TEXT, END_OF_TEXT, true);
        m_ctlText.ReplaceSel(m_sUpdate);
        UpdateData(false);
        m_sUpdate.Empty();
    }
}

void CNpcServerDlg::SetShellState()
{
    extern struct STAT_ST	g_stat;
    STAT_ST	stat = g_stat;
    memset(&g_stat, 0, sizeof(STAT_ST));
    memset(&g_stat, 0, sizeof(STAT_ST));		// repeat
    m_sShell.Format("kernel(ms): %d/%d", stat.nKernelCount ? stat.nKernelSum / stat.nKernelCount : 0, stat.nKernelMax);
}

bool CNpcServerDlg::ProcessInterMsg()
{
    if (!m_pInterPort)
    {
        return false;
    }
    char			buf[1024];
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
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_TITLE, STRING_TYPE(m_szServerName), m_szServerName);
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_SHELLMSG, STRING_TYPE(m_sShell), (LPCTSTR)m_sShell);
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_KERNELMSG, STRING_TYPE(m_sKernel), (LPCTSTR)m_sKernel);
                LOCKTHREAD;		// m_sUpdate»¥³â
                m_pInterPort->Send(cStatus.m_nPortFrom, ACK_TEXT, STRING_TYPE(m_sText), m_sText);
            }
            break;
        case	QUERY_COMMOND:
            {
                LPCTSTR pStr = (LPCTSTR)buf;
                CString str;
                str.Format("¡¾remote¡¿%s", pStr);
                PrintText(str);
                // command
                if (strcmp(pStr, "restartserver") == 0)
                {
                    PrintText(pStr);
                    g_nRestart = true;
                }
            }
            break;
        default:
            PrintText("CMsgServerDlg::ProcessInterMsg()");
            return false;
        }
    }
    DEBUG_CATCH("CNpcServerDlg::Process()")	// AAAAAAAAAAA
    return true;
}





