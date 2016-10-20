
//**********************************************************
// 代码编辑器
//**********************************************************

// MyShellDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyShellDlg.h"

#include "m_map.h"
#include "basefunc.h"
//#include "chatroom.h"
#include "3dgamemap.h"
#include "role.h"
#include "gameplayerset.h"
#include "hero.h"
#include "allmsg.h"
#include "gamemsg.h"
#include "allchanel.h"
#include "GameDataSet.h"
#include "ItemObserver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	REPLAY_OK_STR		"ANSWER_OK"			// 申请新帐号或LOGIN时返回的成功标志串。
#define NEW_ROLE_STR		"NEW_ROLE"			// 产生新人物

//#define BIG5   //繁体

// external...
extern HWND	g_hMyWnd;
extern UINT	g_uStatus;

// proptype
int		GameLoginServer	(const char* pszAccount, const char* pszPsw, const char* pszServer, const char* pszServerName);
void	GameScreenMsg	(const char* pszMsg);
BOOL	GameInit		(HWND hWnd);
BOOL	m_bAlt = false ;
BOOL	m_bShiftDown = false ;
DWORD	g_dwLastTime = 0 ;

const int CHAR_WIDTH = 6 ;
const int	_SHELL_ITEMTYPE_SPRITE = 103 ;

////////////////////////////////////////////////////////////////////////////////////
bool TestDebuger(const char* pszFile)
{
    if (!pszFile)
    {
        return false;
    }
    HFILE hFile = _lopen(pszFile, OF_READ);
    if (HFILE_ERROR == hFile)
    {
        return false;
    }
    _lclose(hFile);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
void GameShow ()
{
	extern DWORD g_dwCurTime ;
	g_dwCurTime = ::TimeGet() ;
    // process player set
    CMyPos posHeroAlign	= g_objHero.GetAlignPos();
    g_objPlayerSet.Process(posHeroAlign.x, posHeroAlign.y);
    // process game map
    char szTime[64] = "";
    DWORD dwProcTime = ::TimeGet();
    g_objGameMap.Process(NULL);
    dwProcTime = ::TimeGet() - dwProcTime;
    // view port
    CMyPos posWorld;
    g_objHero.GetWorldPos(posWorld);
    posWorld.x	-= _SCR_WIDTH / 2;
    posWorld.y	-= _SCR_HEIGHT / 2;
    // Get the keyboard state
    BYTE btState[256] ;
    GetKeyboardState( btState ) ;
    if ( btState [ VK_MENU ] >> 7  == 1 && m_bAlt )
    {
        g_objGameMap.SetViewPos(posWorld, true);
    }
    else
    {
        g_objGameMap.SetViewPos(posWorld);
    }
    // reset mouse event now
    ::MouseProcess();
    // show game objs
    DWORD dwShowTime = ::TimeGet();
    if ( m_bShiftDown )
    {
        g_objGameMap.Show ( true ) ;
    }
    else
    {
        if ( btState [ VK_SHIFT ] >> 7 == 1 )
        {
            g_objGameMap.Show( true );
        }
        else
        {
            g_objGameMap.Show () ;
        }
    }
    // show x?
    g_objPlayerSet.ShowX();
    g_objHero.ShowXp () ;
    if ((::TimeGet() - g_objHero.GetTimeWhoAttackme() < 10 * 1000)
            || CPlayer::s_bShowName || g_objGameMap.GetShowMapObjInfoFlag()
            || g_objHero.IsMouseFocus())
    {
        g_objHero.ShowLife();
    }
    //g_objObserver.Show();
    dwShowTime = ::TimeGet() - dwShowTime;
    extern DWORD g_dwMeshTime, g_dwMeshLoad;
    // process data set
    g_objGameDataSet.Process();
    // process sound
    int nWorldX, nWorldY;
    g_objHero.GetWorldPos(nWorldX, nWorldY);
    ::DXProcessSound(nWorldX, nWorldY);
    // process net msg
    g_objNetwork.ProcessNetMsg();
}

BOOL BeginShow ()
{
    int re = IfDeviceLost ();
    //	if ( re == 1 )
    //		return false;
    if ( re == 2 )
    {
        ResetDevice ();
    }
    // show time
    CMyBitmap::Begin3D ();
    CMyBitmap::ClearBuffer ( true, false, D3DCOLOR_XRGB ( 0, 0, 0 ) );
    return true;
}

void EndShow ()
{
    CMyBitmap::End3D ();
    CMyBitmap::Flip ();
}

////////////////////////////////////////////////////////////////////////////////////
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
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyShellDlg dialog
int CMyShellDlg::m_nHotkeyValue [32] ;

CMyShellDlg::CMyShellDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMyShellDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMyShellDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_dwGameLoop	= 0;
    m_uCmd			= CMD_NORMAL;
    m_hCursorH	= ::LoadCursorFromFile( "Data/Cursor/Hand.ani" ) ;
    m_hCursorFriend = ::LoadCursorFromFile( "Data/Cursor/Friend.ani" ) ;
    m_hCursorPnt = ::LoadCursorFromFile( "Data/Cursor/Point.ani" ) ;
    strcpy(m_szCursorAni, "");
    m_bLogin = false ;
    //	m_bMsgDlg = false ;
    m_uMsgDlgType = 0 ;
    m_bIconFix = false ;
    m_nScreenMode = 0 ;
    m_MagicID = 0 ;
    m_DisMagicID = 0 ;
    m_XpID = 0 ;
    m_uCurSkill = 0 ;
    m_bDlgMove = false ;
    m_bWait = false ;
    m_dwScale = 256 ;
    m_bExpShow = false ;
    m_bExit = false ;
    m_bLbuttonDown = false ;
    m_bLbuttonHold = false ;
    m_bRbuttonDown = false ;
    m_bRbuttonHold = false ;
    m_dwLastBtnDTime = 0 ;
    m_bInitSuc = false ;
    m_bSysMsg = true ;
    m_bHeadClosed = false ;
    m_bProgressHold = false;
}

void CMyShellDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMyShellDlg)
    DDX_Control(pDX, IDC_STCBG, m_stcBg);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyShellDlg, CDialog)
    //{{AFX_MSG_MAP(CMyShellDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_CTLCOLOR()
    ON_WM_SETCURSOR()
    ON_MESSAGE(WM_MY_MESSAGE, OnMyMessage)	//wh
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_TIMER()
    ON_WM_ACTIVATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyShellDlg message handlers

BOOL CMyShellDlg::OnInitDialog()
{
    EnumDisplaySettings( NULL,
                         ENUM_CURRENT_SETTINGS,
                         &m_OldDeviceMode ) ;
    // Test the current screen
    if (m_OldDeviceMode.dmBitsPerPel != 16 && m_OldDeviceMode.dmBitsPerPel != 32)
    {
        AfxMessageBox ( g_objGameDataSet.GetStr(10022) ) ;
        CMyShellDlg::EndDialog( 0 ) ;
        ::PostQuitMessage( WM_QUIT ) ;
        return false ;
    }
    if (m_OldDeviceMode.dmBitsPerPel == 32)
    {
        /*		if ( CMyShellDlg::MessageBox( "系统颜色为32位色会影响游戏速度\n选择“是”自动修为16位色\n选择“否”使用32位色继续游戏", "警告", MB_YESNO ) == IDYES )
        		{
        			m_OldDeviceMode.dmBitsPerPel = 16;
        			ChangeDisplaySettings( &m_OldDeviceMode,
        								   0 ) ;
        		}
        */
    }
    if ( m_OldDeviceMode.dmPelsWidth < 1024 ||
            m_OldDeviceMode.dmPelsHeight < 768 )
    {
        if ( CMyShellDlg::MessageBox( g_objGameDataSet.GetStr(10023),  g_objGameDataSet.GetStr(10040), MB_YESNO ) == IDYES )
        {
            m_OldDeviceMode.dmPelsWidth = 1024 ;
            m_OldDeviceMode.dmPelsHeight = 768 ;
            ChangeDisplaySettings( &m_OldDeviceMode,
                                   0 ) ;
        }
        else
        {
            CMyShellDlg::EndDialog( 0 ) ;
            ::PostQuitMessage( WM_QUIT ) ;
            return false ;
        }
    }
   // CDlgLogo dlglogo ;
    //	dlglogo.DoModal();
    ///dlglogo.Create ( IDD_DIALOG_LOGO, this ) ;
   // dlglogo.ShowWindow ( SW_SHOW ) ;
    CDialog::OnInitDialog();
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    // set the screen mode
    m_nScreenMode = GetPrivateProfileInt ( "ScreenMode", "ScreenModeRecord", -1, g_strSetupIni ) ;
    if ( m_nScreenMode == 1 )
    {
        CMyShellDlg::SetScreenMode ( 1 ) ;
    }
    else if ( m_nScreenMode == 2 )
    {
        _SCR_WIDTH = 1024 ;
        _SCR_HEIGHT = 768 ;
        m_nScreenMode = 2 ;
        CMyShellDlg::SetScreenMode ( 2 ) ;
    }
    else if ( m_nScreenMode == 3 )
    {
        _SCR_WIDTH = 1024 ;
        _SCR_HEIGHT = 768 ;
        m_nScreenMode = 3 ;
        CMyShellDlg::SetScreenMode ( 3 ) ;
    }
    else
    {
        m_nScreenMode = 0 ;
        CMyShellDlg::SetScreenMode ( 0 ) ;
    }
	CItem::s_ItemData.LoadAllItemInfo();
    // init global obj
    g_obj3DRoleData.Init();
    // TODO: Add extra initialization here
    this->MoveWindow(0, 0, _SCR_WIDTH, _SCR_HEIGHT);
    m_stcBg.MoveWindow(0, 0, _SCR_WIDTH, _SCR_HEIGHT);
    // init game
    if ( ! GameInit ( m_stcBg.m_hWnd) )
    {
        EndDialog(0) ;
        return false;
    }
    g_hGameWnd = this->GetSafeHwnd () ;
    m_DlgHelp.Create ( IDD_DIALOG_HELP, this ) ;
    m_DlgLog.Create ( IDD_DIALOG_LOG, this ) ;
  
    // set the interface mode record
    int nMode = GetPrivateProfileInt ( "Interface", "InterfaceRecord", -1, g_strSetupIni ) ;
    if ( nMode != -1 )
    {
        if ( nMode != 0 )
        {
            sprintf ( g_strControlAni, "ani/Control%d.Ani", nMode + 1 ) ;
        }
        else
        {
            strcpy ( g_strControlAni, "ani/Control.Ani" ) ;
        }
     
        m_DlgHelp.ResetBk ( nMode ) ;
        m_DlgLog.ResetBk ( nMode ) ;
    }
    else
    {
      
        m_DlgHelp.ResetBk ( 0 ) ;
        m_DlgLog.ResetBk ( 0 ) ;
    }
    // Init the confirm dialog
    CRect rect ;
    m_DlgConfirm.Create ( IDD_DIALOG_CONFIRM, this );
    m_DlgConfirm.GetWindowRect ( rect ) ;
    m_DlgConfirm.MoveWindow ( ( _SCR_WIDTH - 110 ) / 2,
                              ( _SCR_HEIGHT - 94 ) / 2,
                              110,
                              94 ) ;
    m_DlgConfirm.ShowWindow( SW_SHOW ) ;
    m_DlgConfirm.EnableWindow( false ) ;
    m_DlgRoleCreate.Create ( IDD_DIALOG_ROLECREATE, this ) ;
    m_DlgRoleCreate.ShowWindow(SW_HIDE);
    m_DlgRoleCreate.EnableWindow(false) ;
	m_DlgRoleCreate.MoveWindow(0,_FRAME_SIZE,_SCR_WIDTH,_SCR_HEIGHT);
    m_DlgLogin.Create ( IDD_DIALOG_LOGIN, this ) ;
    SetLoginEffectPos(m_bLogin);
    m_DlgLogin.ShowWindow ( SW_SHOW ) ;
    m_DlgLogin.SetCurLoginMode(0);
    m_DlgLogin.SetFocus() ;
	m_DlgLogin.GetWindowRect(rect);
	
	m_DlgLogin.MoveWindow(0,_FRAME_SIZE,_SCR_WIDTH,_SCR_HEIGHT);
    if ( m_nScreenMode == 2 || m_nScreenMode == 3 )
    {
        CMyShellDlg::MoveControl () ;
    }
    // Set the edit font
    HFONT font;
    font = ::CreateFont ( 12,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
                          0,
#ifdef BIG5
                          CHINESEBIG5_CHARSET,
#else
                          GB2312_CHARSET,
#endif
                          OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS,
                          ANTIALIASED_QUALITY,
                          VARIABLE_PITCH,
                          g_objGameDataSet.GetStr ( 10049 ) ) ;
    m_pGameFont = CFont::FromHandle ( font ) ;
    m_DlgLogin.m_LoginAccountEdt.SetFont ( m_pGameFont ) ;
    m_DlgLogin.m_LoginPasswordEdt.SetFont ( m_pGameFont ) ;
    m_DlgRoleCreate.m_RoleCreateNameEdt.SetFont ( m_pGameFont ) ;
    // Test the system font
    LOGFONT SystemLogFont ;
    CMyShellDlg::GetFont ()->GetLogFont ( &SystemLogFont ) ;

    if ( SystemLogFont.lfHeight != -12 )
    {
        AfxMessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10407 ) ) ) ;
		//OnExit会抛出异常- 直接终止当前进程比较好- 因为程序确定要结束了
		::ExitProcess(0);
		
       //CMyShellDlg::OnExit ( 1 ) ;
    }
    // create form
    const int nFrmPosX	= _SCR_WIDTH, nFrmPosY = 0;
    const int nFrmWidth	= 160, nFrmHeight	= 450;
#ifndef _DEBUG
    if (TestDebuger("\\\\.\\SICE")
            || TestDebuger("\\\\.\\SIWDEBUG")
            || TestDebuger("\\\\.\\NTICE")
            || TestDebuger("\\\\.\\SIWVID"))
    {
        EndDialog(0);
        return false;
    }
    if (IsDebuggerPresent())
    {
        EndDialog(0) ;
        return false;
    }
#endif
//    dlglogo.ShowWindow ( SW_HIDE ) ;
    CMyShellDlg::GetParent()->CenterWindow() ;
    CMyShellDlg::GetParent()->GetWindowRect ( rect ) ;
   

    m_bInitSuc = true ;
#ifndef _SOUND_DISABLE
    if ( 0 != DXSoundinit ( this->GetSafeHwnd ( ) ) )
    {
        this->MessageBox ( g_objGameDataSet.GetStr ( 10048 ), "Error", MB_OK | MB_ICONERROR ) ;
        //		this->MessageBox("初始化声效系统失败。请检查声卡驱动程序", "Error", MB_OK|MB_ICONERROR);
    }
#endif
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyShellDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyShellDlg::OnPaint()
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
HCURSOR CMyShellDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CMyShellDlg::OnTimer(UINT nIDEvent)
{
    static DWORD dwTicks = 0;
    dwTicks ++;
    if (IsIconic())
    {
        if (dwTicks % 5 != 0)
        {
            return;
        }
    }
    else
    {
        if (this != CWnd::GetActiveWindow()
                && dwTicks % 5 != 0)
        {
            return;
        }
    }
    static volatile bool bInLoop = false;
    if (!bInLoop)
    {
        bInLoop = true;
        if (nIDEvent == 1)
        {
            if ( m_bDlgMove )
            {
                CMyShellDlg::MoveDlg( ) ;
            }
            switch ( g_uStatus )
            {
            case ( _STATUS_NONE ):
                {
                    if (!BeginShow())
                    {
                        break;
                    }
                    if ( m_DlgLogin.m_bShow )
                    {
                        m_DlgLogin.Show () ;
                    }
                    if ( m_DlgRoleCreate.m_bShow )
                    {
                        m_DlgRoleCreate.Show() ;
                    }
                    if ( m_DlgConfirm.m_bShow )
                    {
                        m_DlgConfirm.Show() ;
                    }
                    ::DXProcessSound(0, 0);
                    EndShow ( ) ;
                }
                break ;
            case ( _STATUS_EXIT ):
                {
                    m_DlgMain.WriteRecord();
                    const char* pszMsg	= g_objEntrance.GetLastMsg();
                    if (pszMsg)
                    {
                        ::MessageBox(NULL, pszMsg, g_objGameDataSet.GetStr ( 10050 ), MB_OK | MB_ICONERROR);
                        g_uStatus	= _STATUS_NONE;
                        g_objGameMsg.ClearMsg() ;
                        g_objGameMsg.DestroyLeaveWord() ;
                        if (m_bLogin)
                        {
                            CMyShellDlg::CloseCurDlg();
                            ::DXCloseAllMedia();
                            ::DXCloseAllSound();
                        }
                        SetLoginEffectPos(m_bLogin);
                        g_bPickUp = false;
                        m_uCmd = CMD_NORMAL;
                        m_DlgLogin.ShowWindow ( SW_SHOW ) ;
                        m_DlgLogin.SetCurLoginMode(2);
                        m_DlgLogin.SetFocus() ;
                        m_DlgLogin.m_LoginPasswordEdt.SetFocus();
                    }
                }
                break ;
            case ( _STATUS_ACCOUNT ):
                {
                    GameScreenMsg(g_objGameDataSet.GetStr ( 10051 ) ) ;//"连接账号服务器中。。。");//
                    g_objNetwork.ProcessNetMsg();
                }
                break;
            case ( _STATUS_LOGIN ):
                {
                    char szMsg[128];
                    sprintf(szMsg, g_objGameDataSet.GetStr ( 10052 ), g_szServerName,g_szServerName);
                    GameScreenMsg(szMsg); //"登录服务器中。。。");//
                    g_objNetwork.ProcessNetMsg();
                    if ( m_DlgConfirm.m_bShow )
                    {
                        BeginShow() ;
                        m_DlgConfirm.Show() ;
                        EndShow() ;
                    }
                    const char* pszMsg	= g_objEntrance.GetLastMsg();
                    if (pszMsg)
                    {
                        if (0 == stricmp(REPLAY_OK_STR, pszMsg ) )
                        {
                            CMyShellDlg::StartGame () ;
                        }
                        else if (0 == stricmp(NEW_ROLE_STR, pszMsg ) )
                        {
                            g_uStatus = _STATUS_NONE ;
                            WPARAM wParam = ON_LOGIN_NEW ;
                            this->PostMessage ( WM_MY_MESSAGE, wParam ) ;
                        }
                        else
                        {
                            ::MessageBox(NULL, pszMsg, "Error", MB_OK | MB_ICONERROR);
                            g_uStatus = _STATUS_NONE ;
                            SetLoginEffectPos(m_bLogin);
                            m_DlgLogin.ShowWindow ( SW_SHOW ) ;
                            m_DlgLogin.SetCurLoginMode(2);
                            m_DlgLogin.SetFocus() ;
                            m_DlgLogin.m_LoginPasswordEdt.SetFocus();
                        }
                    }
                }
                break ;
            case ( _STATUS_REGISTER ):
                {
					if(!m_bWait)
					{
						GameScreenMsg(g_objGameDataSet.GetStr ( 10054 ) ) ;//"注册服务器中。。。") ;//
					}else
					{
						//创建角色成功之后- 重新连接服务器开始游戏
						 extern int GameLoginServer(const char * pszAccount, const char * pszPsw, const char * pszServerIP, const char * pszServerName);
						
						 GameLoginServer( m_DlgLogin.GetAccount(),m_DlgLogin.GetPswd(), m_DlgLogin.GetServer(), m_DlgLogin.GetServerName());
					}
                    
                    g_objNetwork.ProcessNetMsg () ;
                    const char* pszMsg	= g_objRegister.GetLastMsg();
                    if ( pszMsg )
                    {
                        if ( 0 != stricmp(REPLAY_OK_STR, pszMsg ) )
                        {
                            g_uStatus = _STATUS_NONE ;
                            ::MessageBox(NULL, pszMsg, "Error", MB_OK | MB_ICONERROR);
                            CMyShellDlg::CreateRole( 0 ) ;
                        }
                        else
                        {
                            CMyShellDlg::StartGame () ;
                            m_bWait = true ;
                            g_dwLastTime = TimeGet() ;
                        }
                    }
                    if ( m_bWait )
                    {
                        GameScreenMsg(g_objGameDataSet.GetStr ( 10055 ) ) ;
                    }
                }
                break ;
            case ( _STATUS_WAITING ):
                {
                    GameScreenMsg(g_objGameDataSet.GetStr ( 10057 ) ) ;//"与服务器交互中，请稍候。。。");//
                    g_objNetwork.ProcessNetMsg();
                }
                break;
            case ( _STATUS_NORMAL ):
                {
                    if (!BeginShow())
                    {
                        break;
                    }
                    GameShow() ;
                    g_objHero.ShowHelpTips(50, 72, _SCR_WIDTH - 230, 0xF7E38C, 0x3e000080);
                    g_objGameMsg.Show() ;
                    m_DlgMain.Show();
                    if ( m_DlgTask.m_bShow )
                    {
                        m_DlgTask.Show () ;
                    }
                    if ( m_DlgConfirm.m_bShow )
                    {
                        m_DlgConfirm.Show () ;
                    }
                    ShowAniCursor(m_szCursorAni);
                    /*
                    					// 一直显示的提示文本
                    					CMySize FontSize;
                    					CMyBitmap::GetFontSize(FontSize);

                    					// 判断是否有一直显示的信息
                    					if (m_bProgressHold)
                    					{
                    						char strHoldMessage[256];

                    						switch ( m_dwProgressNumber )
                    						{
                    						case 0: //LIFE STATIC
                    							sprintf( strHoldMessage, "HP:%d/%d", g_objHero.GetData(CPlayer::_PLAYER_LIFE), g_objHero.GetMaxLife() );
                    							break ;
                    						case 1:  //MANA STATIC
                    							if (g_objHero.GetMaxMana()<=0)
                    								sprintf( strHoldMessage, "HP:%d/%d", g_objHero.GetData(CPlayer::_PLAYER_LIFE), g_objHero.GetMaxLife() );
                    							else
                    								sprintf( strHoldMessage, "MP:%d/%d", g_objHero.GetData(CPlayer::_PLAYER_MANA), g_objHero.GetMaxMana() );
                    							break ;
                    						case 2: //FORCE STATIC
                    							sprintf( strHoldMessage, "SP:%d/%d", g_objHero.GetPhysicalForce(), g_objHero.GetMaxPhysicalForce() );
                    							break ;
                    						case 3: //xp
                    							sprintf(strHoldMessage,"XP:%d/%d",g_objHero.GetXP(),g_objHero.GetMaxXP());
                    							break;
                    						}


                    						if (0 != m_HoldX && 0 != m_HoldY)
                    						{
                    							if (m_dwProgressNumber == 0
                    								|| (m_dwProgressNumber == 1&& g_objHero.GetMana() <= 0) )		//hp 特殊处理
                    							{
                    								char szCurHp[16] = "";
                    								char szMaxHp[16] = "";
                    								char szRealMaxHp[16] = "";
                    								sprintf( szCurHp, "HP:%d/", g_objHero.GetData(CPlayer::_PLAYER_LIFE));
                    								sprintf( szMaxHp, "%d", g_objHero.GetMaxLife());
                    								if (g_objHero.GetMaxLifePercent() > 0 && g_objHero.GetMaxLifePercent() < 1000)
                    									sprintf( szRealMaxHp, "/%d",g_objHero.GetMaxLife()*1000/g_objHero.GetMaxLifePercent());
                    								int nMsgLength = strlen(szCurHp) + strlen(szMaxHp) + strlen(szRealMaxHp);

                    								int nOffset = nMsgLength * CHAR_WIDTH/2-10;
                    								if ((m_StaticPoint[m_dwProgressNumber].x - nOffset - 5) < 0) nOffset = m_StaticPoint[m_dwProgressNumber].x - 5;
                    								g_objGameDataSet.ShowInfoBack(m_StaticPoint[m_dwProgressNumber].x - nOffset - 5,
                    									m_StaticPoint[m_dwProgressNumber].y -1 - 7,
                    									nMsgLength*CHAR_WIDTH + 10,
                    									FontSize.iHeight+1+ 10) ;

                    								CMyBitmap::ShowString ( m_StaticPoint[m_dwProgressNumber].x - nOffset,
                    									m_StaticPoint[m_dwProgressNumber].y - 2,
                    									0xffffff,
                    									szCurHp) ;

                    								if (g_objHero.GetMaxLifePercent() > 0 &&  g_objHero.GetMaxLifePercent() < 1000)
                    									CMyBitmap::ShowString ( m_StaticPoint[m_dwProgressNumber].x - nOffset + strlen(szCurHp) * CHAR_WIDTH,
                    									m_StaticPoint[m_dwProgressNumber].y - 2,
                    									0xff0000,
                    									szMaxHp) ;
                    								else
                    									CMyBitmap::ShowString ( m_StaticPoint[m_dwProgressNumber].x - nOffset + strlen(szCurHp) * CHAR_WIDTH,
                    									m_StaticPoint[m_dwProgressNumber].y - 2,
                    									0xffffff,
                    									szMaxHp) ;
                    							}
                    							else
                    							{
                    								g_objGameDataSet.ShowInfoBack( m_HoldX - 5,
                    									m_HoldY - 5,
                    									strlen(strHoldMessage)*CHAR_WIDTH + 10,
                    									FontSize.iHeight+1+ 10) ;

                    								CMyBitmap::ShowString ( m_HoldX,
                    									m_HoldY +1,
                    									0xffffff,
                    									strHoldMessage) ;
                    							}
                    						}
                    					}*/
                    if ( g_bPickUp )
                    {
                        CMyShellDlg::ShowPickUpIcon () ;
                    }
                    else if ( !g_bPickUp && g_bMouseMoveIn )
                    {
                        CMyShellDlg::ShowMsgDlg () ;
                    }
                    if ( m_bLbuttonDown || m_bRbuttonDown)
                    {
                        CMyShellDlg::ProcessMouseHold() ;
                    }
                    /*			else
                    			{
                    				if ( m_bLbuttonHold || m_bRbuttonHold)
                    				{
                    					CMyShellDlg::ProcessMouseHold() ;
                    				}
                    			}

                    		if ( m_bAutoMagicAtk )
                    		{
                    			CPlayer* pPlayer = g_objPlayerSet.GetFocusPlayer() ;
                    			if ( pPlayer != NULL && m_AutoTargetID == pPlayer->GetID() )
                    			{
                    				if ( pPlayer->IsDead () )
                    				{
                    					m_bAutoMagicAtk = false ;
                    				}
                    				else if ( !g_objHero.IsMagicAttackBusy() )
                    				{
                    					g_objHero.MagicAttack( m_AutoMagicID, m_AutoTargetID ) ;
                    				}
                    			}
                    			else
                    			{
                    				m_bAutoMagicAtk = false ;
                    			}
                    		}*/
                    CMyPos pPosWorld, pPosBg;
                    g_objGameMap.Screen2World(m_EffectExItemPos.x, m_EffectExItemPos.y , pPosWorld.x, pPosWorld.y);
                    g_objGameMap.World2Bg(pPosWorld.x, pPosWorld.y, pPosBg.x, pPosBg.y );
                    m_EffectExItem.Process();
                    m_EffectExItem.Show(pPosBg);
                    EndShow ( ) ;
                }
                break ;
            default:
                nIDEvent = 1 ;
                break ;
            }
        }
        bInLoop = false;
    }
    CDialog::OnTimer(nIDEvent);
}

void CMyShellDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bLbuttonDown = false ;
    m_bRbuttonDown = false ;
    m_dwLastBtnDTime = 0 ;
    CDialog::OnLButtonUp(nFlags, point);
}

void CMyShellDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    if ( m_bIconFix )
    {
        m_bIconFix = false ;
        return ;
    }
    CMyShellDlg::SetFocus();
    if (m_DlgMain.m_DlgEmotion.m_bShow)
    {
        m_DlgMain.m_DlgEmotion.EnableWindow (false);
    }
    // capture the screen name
    if ( strlen( g_objGameMsg.GetCaptureName () ) > 0 )
    {
        m_DlgMain.m_DlgTalk.m_TalkNameEdt.SetWindowText ( g_objGameMsg.GetCaptureName () ) ;
        return ;
    }
    if (g_objHero.IsDead() && !g_objHero.TestStatus(USERSTATUS_GHOST))
    {
        return;
    }
    if (_STATUS_WAITING == g_uStatus)
    {
        return;
    }
    CMyShellDlg::SetFocus () ;
    extern void	MouseSet (int x, int y, int event);
    MouseSet(point.x, point.y, 1);
    int nAmount = g_objHero.GetTeamMemberAmount();
    if (nAmount > 1)
    {
        for(int i = 0; i < nAmount; i++)
        {
            if (m_DlgMain.m_DlgGroup.m_ImgRect[i].PtInRect(point))
            {
                return;
            }
        }
    }
    if (g_objHero.TestStatus(USERSTATUS_GHOST))
    {
        if (m_uCmd == CMD_TEMJOIN)
        {
            DWORD dwNFlag = PLAYER_NORMAL;
            dwNFlag |= PLAYER_INCLUDEDEAD;
            CPlayer* pMember = g_objPlayerSet.GetFocusPlayer(dwNFlag);
            if (pMember && pMember->GetRoleType() == _ROLE_PLAYER)
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10705));
                m_uCmd = CMD_NORMAL;
                if (m_DlgMain.m_DlgGroup.m_bShow)
                {
                    m_DlgMain.m_DlgGroup.EnableWindow(false);
                }
                return;
            }
        }
        int nPosX, nPosY;
        g_objGameMap.Mouse2Cell(point.x, point.y, nPosX, nPosY);
        g_objHero.Walk(nPosX, nPosY);
        CMyShellDlg::SetMouseMove(nPosX, nPosY);
        m_bLbuttonDown = true ;
        return;
    }
    DWORD dwFlag = PLAYER_NORMAL;
    if (CMD_ATK == dwFlag || CMD_NORMAL == dwFlag)
    {
        dwFlag |= PLAYER_MONSTERPRI;
    }
    else
    {
        dwFlag |= PLAYER_PLAYERPRI;
    }
    CPlayer* pTarget = g_objPlayerSet.GetFocusPlayer(dwFlag);
    if (pTarget == NULL && m_uCmd == CMD_USEITEMTO && m_DlgMain.m_DlgGroup.GetMouseInMember())
    {
        pTarget = g_objPlayerSet.GetPlayer(m_DlgMain.m_DlgGroup.GetMouseInMember());
        if (pTarget )
        {
            CItem* pItem = g_objHero.GetItem(m_IconRealID);
            if (pItem)
            {
                g_objHero.UseItem(pItem->GetID(), pTarget->GetID());
                return;
            }
        }
    }
    if (pTarget && (pTarget->GetRoleType() == _ROLE_CALL_PET || pTarget->GetRoleType() == _ROLE_MONSTER || pTarget->GetRoleType() == _ROLE_PLAYER || pTarget->GetRoleType() == _ROLE_HERO))	// interact command
    {
        int nHeroX, nHeroY;
        g_objHero.GetPos(nHeroX, nHeroY);
        int nTargetX, nTargetY;
        pTarget->GetPos(nTargetX, nTargetY);
        switch(m_uCmd)
        {
        case CMD_NORMAL:
        case CMD_ATK:
            if (nFlags & MK_CONTROL)
            {
                if (g_objHero.TestStatus(USERSTATUS_XPFULL))
                {
                    if (pTarget)
                    {
                        CMyShellDlg::AttackMagicXp();
                    }
                }
                else
                {
                    g_objHero.LockAttack (pTarget, true);
                }
            }
            else if ( nFlags & MK_SHIFT && !pTarget->IsNpc () )
            {
                m_DlgMain.m_DlgTalk.SetTalkTarget ( pTarget ) ;
                g_objHero.Direction ( nTargetX, nTargetY ) ;
            }
            else
            {
                OBJID idItem = ID_NONE;
                CMyPos ItemPos;
                if (( g_objHero.GetPkMode () != PKMODE_MONSTERONLY || pTarget->IsNpc () ) && !pTarget->TestStatus(USERSTATUS_DIE))
                {
                    g_objHero.LockAttack (pTarget) ;
                }
                else if ( !g_bPickUp && (idItem = g_objGameMap.Get2DFocusMapItem (ItemPos)) != ID_NONE )
                {
                    //CMyPos ItemPos ;
                    //g_objGameMap.Mouse2Cell(point.x, point.y, ItemPos.x, ItemPos.y);
                    CMyShellDlg::PickMapItem ( idItem, ItemPos.x, ItemPos.y ) ;
                    return ;
                }
            }
            break;
        case CMD_TRADE:
            {
                if ( pTarget->IsPlayer() )
                {
                    if (pTarget->IsDead())
                    {
                        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10713)); //不允许与亡灵进行交易!
                    }
                    else
                    {
                        g_objHero.ApplyTrade ( pTarget ) ;
                    }
                }
            }
            break ;
        case CMD_TEMJOIN:
            if ( pTarget->IsPlayer() )
            {
                if (pTarget->IsDead())
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100066)); //错误的申请对象！
                }
                else
                {
                    g_objHero.TeamApplyJion ( pTarget->GetID () ) ;
                }
            }
            break ;
        case CMD_TEMINVITE:
            if ( pTarget->IsPlayer() )
            {
                if (pTarget->IsDead())
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100066)); //错误的申请对象！
                }
                else
                {
                    g_objHero.TeamInvite ( pTarget->GetID () ) ;
                }
            }
            break ;
        case CMD_FACJOIN:
            if ( pTarget->IsPlayer())
            {
                if (pTarget->IsDead())
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100066)); //错误的申请对象！
                }
                else if (pTarget->GetSyndicateRank() != 100)
                {
                    char msg[128] = "";
                    sprintf(msg, (char*)g_objGameDataSet.GetStr(100068), pTarget->GetName());
                    g_objGameMsg.AddMsg(msg);
                }
                else
                {
                    g_objHero.SyndicateApplyJoin ( pTarget->GetID () ) ;
                }
                m_DlgMain.OnMainFaction();
            }
            break ;
        case CMD_FACINVITE:
            if (pTarget->IsDead())
            {
                g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100066)); //错误的申请对象！
            }
            else if (pTarget->GetSydicateID() != ID_NONE)
            {
                g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100073)); //对方已有军团！
            }
            else
            {
                g_objHero.SyndicateApplyInvite ( pTarget->GetID () ) ;
            }
            m_DlgMain.OnMainFaction();
            break ;
        case CMD_MPCCOURT:
            g_objHero.Court(pTarget);
            break;
        case CMD_DIVORCE:
            g_objHero.Divorce(pTarget);
            break;
        case CMD_FRDMAKE:
            g_objHero.MakeFriend(pTarget->GetID());
            break;
        case CMD_FRDBREAK:
            g_objHero.BreakOffFriend(pTarget->GetID());
            break;
        case CMD_NPCEQUIP:
            g_objHero.m_objPlayerDummy.LockDummy ( pTarget->GetID () ) ;
            break ;
        case CMD_TEACHERREQ:
            if (pTarget->IsPlayer())
            {
                if (pTarget->IsDead ())
                {
                    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10775));    //10775=错误对象！
                }
                else
                {
                    g_objHero.TeacherApply(pTarget->GetID());
                }
            }
            break;
        case CMD_STUDENTREQ:
            if (pTarget->IsPlayer())
            {
                if (pTarget->IsDead ())
                {
                    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10775));    //10775=错误对象！
                }
                else
                {
                    g_objHero.StudentApply(pTarget->GetID());
                }
            }
            break;
        case CMD_USEITEMTO:
            if (pTarget->IsPlayer() || pTarget->IsMonster() || pTarget->IsPet())
            {
                CItem* pItem = g_objHero.GetItem(m_IconRealID);
                if (pItem)
                {
                    g_objHero.UseItem(pItem->GetID(), pTarget->GetID());
                }
            }
            break;
        case CMD_ADDALLY:
            if (pTarget->IsPlayer() && !pTarget->IsDead())
            {
                if (pTarget->IsDead())
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100066)); //错误的申请对象！
                }
                else  if (pTarget->GetSyndicateRank() == RANK_LEADER)
                {
                    g_objHero.AddAllySyn(pTarget->GetID());
                }
                else
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100076));    //对方不是军团长！
                }
            }
            break;
        }
        // reset cmd now...
        m_uCmd	= CMD_NORMAL;
        return;
    }
    else
    {
        if ( m_uCmd == CMD_LAY )
        {
            g_objHero.FixMousePlayer () ;
            m_uCmd = CMD_NORMAL ;
            return ;
        }
        if (m_uCmd == CMD_TEMINVITE)
        {
            DWORD dwNFlag = PLAYER_NORMAL;
            dwNFlag |= PLAYER_INCLUDEDEAD;
            CPlayer* pMember = g_objPlayerSet.GetFocusPlayer(dwNFlag);
            if (pMember && pMember->GetRoleType() == _ROLE_PLAYER && pMember->IsDead())
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10703));
                if (m_DlgMain.m_DlgGroup.m_bShow)
                {
                    m_DlgMain.m_DlgGroup.EnableWindow(false);
                }
                return;
            }
        }
        if (m_uCmd == CMD_TEMJOIN )
        {
            DWORD dwNFlag = PLAYER_NORMAL;
            dwNFlag |= PLAYER_INCLUDEDEAD;
            CPlayer* pMember = g_objPlayerSet.GetFocusPlayer(dwNFlag);
            if (pMember && pMember->GetRoleType() == _ROLE_PLAYER && pMember->IsDead())
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10704));
                if (m_DlgMain.m_DlgGroup.m_bShow)
                {
                    m_DlgMain.m_DlgGroup.EnableWindow(false);
                }
                return;
            }
        }
        int nPosX, nPosY;
        int nNpcType = 0;
        OBJID idNpcFocus = ID_NONE;
        g_objGameMap.Mouse2Cell(point.x, point.y, nPosX, nPosY);
        CMyPos Pos = { nPosX, nPosY };
        if ( nFlags & MK_SHIFT )
        {
            OBJID idItem = ID_NONE;
            CMyPos posItem;
            if ( !g_bPickUp && (idItem = g_objGameMap.Get2DFocusMapItem (posItem)) != ID_NONE )
            {
                CMyShellDlg::PickMapItem ( idItem, posItem.x, posItem.y ) ;
            }
            else
            {
                g_objHero.Direction ( nPosX, nPosY ) ;
            }
            return ;
        }
        else if ((nFlags & MK_CONTROL) && g_objHero.TestStatus(USERSTATUS_XPFULL))
        {
            for(int i = 0; i < g_objHero.GetXpSkillAmount(); i++)
            {
                CMagic* pXp = g_objHero.GetXpSkillByIndex(i);
                if (pXp && pXp->GetIDType() == m_XpID)
                {
                    //					if (pXp->TestTarget ( MAGIC_TARGET_NONE ) || pXp->m_infoMagicType.dwActionSort == MAGICSORT_LINE)
                    CMyShellDlg::AttackMagicXp();
                }
            }
        }
        else
        {
            OBJID idItem = ID_NONE;
            CMyPos posItem;
            CPlayer* pPlayer = g_objPlayerSet.GetFocusPlayer();
            if ( g_bPickUp )
            {
                switch ( m_PickUpDlgID )
                {
                case DLG_MONSTEREGG:
                    CMyShellDlg::DropMapItem ( point.x, point.y , PACKAGE_EGG);
                    break;
                case DLG_STONEGOODS0:
                    CMyShellDlg::DropMapItem ( point.x, point.y , PACKAGE_SOUL);
                    break;
                case DLG_MPCGOODS:
                    CMyShellDlg::DropMapItem ( point.x, point.y );
                    break ;
                case DLG_EQUIP:
                    g_bPickUp = false ;
                    m_DlgMain.m_DlgQuery.m_DlgEquip.FlashEquip();
                    break ;
                case DLG_MAIN:
                    g_bPickUp = false ;
                    break ;
                default:
                    break ;
                }
                return ;
            }
            else if ( !g_bPickUp && (idItem = g_objGameMap.Get2DFocusMapItem (posItem)) != ID_NONE )
            {
                CMyPos posHero ;
                g_objHero.GetPos ( posHero.x, posHero.y ) ;
                if ( nFlags& MK_CONTROL && !( posHero.x == posItem.x && posHero.y == posItem.y ) )
                {
                    g_objHero.Run(nPosX, nPosY);
                }
                else
                {
                    CMyShellDlg::PickMapItem ( idItem, posItem.x, posItem.y ) ;
                }
                return ;
            }
            else if ( !g_bPickUp && pPlayer)//(idNpcFocus=g_objGameMap.GetFocusNpc ( nNpcType )) != ID_NONE)
            {
                if (pPlayer)
                {
                    if ( CMD_DELNPC == m_uCmd )
                    {
                        m_uCmd = CMD_NORMAL ;
                        g_objHero.DelNpc ( pPlayer->GetID () ) ;
                        return ;
                    }
                    switch(pPlayer->GetRoleType())
                    {
                    case _AUCTION_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break;
                    case _ROLE_STONE_MINE:
                        //					case _ROLE_WOOD_NPC:
                        //					case _ROLE_METAL_MINE:
                        g_objHero.CollectRes(pPlayer->GetID());
                        break;
                    case _ROLE_SHOPKEEPER_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break;
                    case _ROLE_TASK_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break;
                    case _ROLE_NEIGHBOR_DOOR:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break;
                    case _SYNTRANS_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break;
                    case _ROLE_BOOTH_FLAG_NPC:
                        if ( g_objHero.CanBooth ( pPlayer ) )
                        {
                            if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                            {
                                m_DlgMain.CloseCurDlg();
                                m_DlgMain.CloseCurDlg(1);
                                m_DlgMain.CloseCurDlg(2);
                                m_DlgMain.CloseNpcDlg();
                                if (m_DlgTask.m_bShow)
                                {
                                    m_DlgTask.EnableWindow(false);
                                }
                            }
                            if (!m_DlgTask.m_bShow)
                            {
                                g_objHero.ActiveNpc(pPlayer->GetID());
                            }
                        }
                        break;
                    case _TRAINPLACE_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break;
                    case _ROLE_STORAGE_NPC:
                    case _ROLE_TRUNCK_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break ;
                    case _ROLE_FACE_NPC:
                        g_objHero.ActiveNpc(pPlayer->GetID());
                        break ;
                    case _ROLE_FORGE_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break  ;
                    case _ROLE_EMBED_NPC:
                        if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                        {
                            m_DlgMain.CloseCurDlg();
                            m_DlgMain.CloseCurDlg(1);
                            m_DlgMain.CloseCurDlg(2);
                            m_DlgMain.CloseNpcDlg();
                            if (m_DlgTask.m_bShow)
                            {
                                m_DlgTask.EnableWindow(false);
                            }
                        }
                        if (!m_DlgTask.m_bShow)
                        {
                            g_objHero.ActiveNpc(pPlayer->GetID());
                        }
                        break ;
                    case _ROLE_STATUARY_NPC:
                        if ( nFlags & MK_CONTROL && g_objHero.GetPkMode() == PKMODE_FREE )
                        {
                            g_objHero.LockAttack (pPlayer, true);
                        }
                        else
                        {
                            g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10352) ) ;
                        }
                        break ;
                    case _ROLE_SYNFLAG_NPC:
                        if ( nFlags & MK_CONTROL )
                        {
                            g_objHero.LockAttack (pPlayer, true);
                        }
                        else if ( !(nFlags & MK_CONTROL) )
                        {
                            if (g_objHero.GetLastDialogPara() != pPlayer->GetID())
                            {
                                m_DlgMain.CloseCurDlg();
                                m_DlgMain.CloseCurDlg(1);
                                m_DlgMain.CloseCurDlg(2);
                                m_DlgMain.CloseNpcDlg();
                                if (m_DlgTask.m_bShow)
                                {
                                    m_DlgTask.EnableWindow(false);
                                }
                            }
                            if (!m_DlgTask.m_bShow)
                            {
                                g_objHero.ActiveNpc(pPlayer->GetID());
                            }
                            g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10349) ) ;
                        }
                        else
                        {
                            g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10349) ) ;
                        }
                        break ;
                    case _ROLE_BOOTH_NPC:
                        //						g_objHero.ActiveNpc(pPlayer->GetID());
                        {
                            // MpcBooth
                            if ( pPlayer->GetID() == g_objHero.m_objBoothManager.GetID () )
                            {
                                m_DlgMain.OpenBooth ( true ) ;
                            }
                            else
                            {
                                if ( m_DlgMain.m_DlgTrade.m_bShow || ( m_DlgMain.m_DlgNpcBooth.m_bShow && g_objHero.GetLastDialogPara() == g_objHero.m_objBooth.GetID () ) )
                                {
                                    g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10398) ) ;
                                }
                                else
                                {
                                    if (g_objHero.m_objBooth.Open ( pPlayer->GetID() ))
                                    {
                                        m_DlgMain.OpenBooth ( false ) ;
                                    }
                                }
                            }
                        }
                        break ;
                    case _ROLE_MAGICGOAL_NPC:
                        g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10354 ) ) ;
                        break ;
                    case _ROLE_WEAPONGOAL_NPC:
                        g_objHero.LockAttack (pPlayer, true);
                        break ;
                    case _ROLE_DICE_NPC:
                        if ( !m_DlgMain.m_DlgDice.m_bShow)
                        {
                            if (pPlayer)
                            {
                                g_objHero.ActiveNpc(pPlayer->GetID());
                            }
                            //								m_DlgConfirm.SetConfirmDlg ( 18, NULL, pTarget->GetID () ) ;
                        }
                        else
                        {
                            m_DlgConfirm.SetConfirmDlg ( 19 ) ;
                        }
                        break ;
                    case _ROLE_TARGET_NPC:
                        g_objHero.LockAttack ( pPlayer, true ) ;
                        break ;
                    }
                }
                return ;
            }
            if ( !pPlayer && CMD_DELNPC == m_uCmd )
            {
                OBJID tempID = g_objGameMap.GetFocusTerrainNpc () ;
                if ( tempID != ID_NONE )
                {
                    m_uCmd = CMD_NORMAL ;
                    g_objHero.DelNpc ( tempID ) ;
                    return ;
                }
            }

            if ( g_objHero.GetCommandType() == _COMMAND_WALK)
            {
                CMyPos posTarget = {nPosX, nPosY};
                g_objHero.AddStep(posTarget);
            }
            else
            {
				g_objHero.Run(nPosX,nPosY);
               // g_objHero.Walk(nPosX, nPosY);
            }
            CMyShellDlg::SetMouseMove(nPosX, nPosY);
            if (m_DlgTask.m_bShow)
            {
                m_DlgTask.EnableWindow(false);
            }
            //			m_DlgMain.CloseNpcDlg();
			
			m_bLbuttonDown = true ;
			
            
        }
    }
    CDialog::OnLButtonDown(nFlags, point);
}


HBRUSH CMyShellDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CMyShellDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    BOOL bSetAni = false;
    switch ( m_uCmd )
    {
    case CMD_MPCCOURT:
        SetCursorAni("");
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_MARRY) ) ;
        bSetAni = true;
        break ;
    case CMD_TEMJOIN:
        SetCursorAni("");
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_TEAMJOIN) ) ;
        bSetAni = true;
        break ;
    case CMD_TEMINVITE:
        SetCursorAni("");
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_TEAMINVITE) ) ;
        bSetAni = true;
        break ;
    case CMD_FRDMAKE:
        ::SetCursor(NULL);
        SetCursorAni("FrdCursor");
        bSetAni = true;
        break ;
    case CMD_FACINVITE:
        ::SetCursor(NULL);
        SetCursorAni("FacCursor");
        bSetAni = true;
        break;
    case CMD_ADDALLY:
        ::SetCursor(NULL);
        SetCursorAni("FacCursor");
        bSetAni = true;
        break ;
    case CMD_FACJOIN:
        ::SetCursor(NULL);
        SetCursorAni("FacCursor");
        bSetAni = true;
        break ;
    case CMD_TRADE:
        ::SetCursor(NULL);
        SetCursorAni("TradeCursor");
        bSetAni = true;
        break ;
    case CMD_DELNPC:
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_DELNPC) ) ;
        bSetAni = true;
        break ;
    case CMD_NPCEQUIP:
        SetCursorAni("");
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_NPCEQUIP ) ) ;
        bSetAni = true;
        break ;
    case CMD_TEACHERREQ:
        SetCursorAni("");
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_TEAMINVITE) ) ;
        bSetAni = true;
        break;
    case CMD_STUDENTREQ:
        SetCursorAni("");
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_TEAMJOIN) ) ;
        bSetAni = true;
        break;
    case CMD_USEITEMTO:
        ::SetCursor(NULL);
        SetCursorAni("GemCursor");
        bSetAni = true;
        break;
    default:
        SetCursorAni("");
        break ;
    }
    CPoint 	point;
    GetCursorPos(&point);
    CMyPos posMouse;
    ::MouseCheck(posMouse.x, posMouse.y);
    CRect rect;
    CMyShellDlg::GetWindowRect(rect);
    posMouse.x += rect.left;
    posMouse.y += rect.top;
    if (point.x == posMouse.x && point.y == posMouse.y && !g_bPickUp && !m_bIconFix)
    {
        CMyShellDlg::SetGameCursor();
        bSetAni = true;
    }
    if ( g_bPickUp )
    {
        ::SetCursor ( NULL ) ;
        SetCursorAni("");
    }
    else if ( m_bIconFix )
    {
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_FIX) ) ;
    }
    else if (m_bLbuttonDown || m_bRbuttonDown)
    {
        ::SetCursor( AfxGetApp()->LoadCursor(IDC_CURSOR_POINTDOWN));
    }
    else if ( m_bLbuttonHold || m_bRbuttonHold)
    {
        ::SetCursor( AfxGetApp()->LoadCursor(IDC_CURSOR_POINTDOWN));
    }
    else if ( bSetAni == false)
    {
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_POINT) ) ;
    }
    return true ;
}

LRESULT CMyShellDlg::OnMyMessage ( WPARAM wParam, LPARAM lParam )
{
    switch ( wParam )
    {
    case ON_EXIT:
        CMyShellDlg::OnExit ( )	;
        break ;
    case ON_EXIT_LOGIN:
        if ( m_DlgLogin.m_bShow && !m_bExit )
        {
            CMyShellDlg::OnExit () ;
        }
        else
        {
            m_bExit = true ;
            m_DlgConfirm.SetConfirmDlg( EXIT_CONFIRM, NULL, 1 ) ;
        }
        break ;
    case ON_EXIT_ACCEPT:
        if ( lParam == 1 )
        {
            if ( m_bLogin )
            {
                DXPlaySound("Sound/Dlg_Ok.wav");
                m_bExit = false ;
                m_bIconFix = false;
                g_bPickUp = false;
                m_uCmd = CMD_NORMAL;
                m_DlgMain.CloseCurDlg();
                m_DlgMain.CloseCurDlg(1);
                m_DlgMain.CloseCurDlg(2);
                m_DlgMain.CloseNpcDlg();
                m_DlgMain.m_DlgMpcGoods.EnableWindow(false);
                m_DlgMain.m_DlgTalk.m_TalkDataCob.SetWindowText("");
                m_DlgMain.WriteRecord();
                m_DlgMain.m_DlgTalk.m_GoodBox.RemoveIcon ( 0, 0, true ) ;
                g_objNetwork.Destroy() ;
                g_objGameMsg.ClearMsg() ;
                g_objGameMsg.DestroyLeaveWord() ;
                g_uStatus	= _STATUS_NONE ;
                m_MagicID = 0 ;
                m_DlgRoleCreate.ShowWindow(SW_HIDE);
                m_DlgRoleCreate.EnableWindow(false) ;
                SetLoginEffectPos(m_bLogin);
                m_DlgLogin.ShowWindow ( SW_SHOW ) ;
                m_DlgLogin.SetCurLoginMode(2);
                m_DlgLogin.SetFocus() ;
                m_DlgLogin.m_LoginPasswordEdt.SetFocus();
            }
            else
            {
                m_bExit = false ;
                m_bIconFix = false;
                g_bPickUp = false;
                m_uCmd = CMD_NORMAL;
                m_DlgRoleCreate.EnableWindow(false) ;
                m_DlgRoleCreate.ShowWindow(SW_HIDE);
                SetLoginEffectPos(m_bLogin);
                m_DlgLogin.ShowWindow( SW_SHOW ) ;
                m_DlgLogin.EnableWindow(true);
                m_DlgLogin.SetCurLoginMode(2);
                m_DlgLogin.SetFocus() ;
                m_DlgLogin.m_LoginPasswordEdt.SetFocus();
            }
            g_objGameMap.Reset();
            g_objHero.Reset();
            ::DXCloseAllMedia();
            ::DXCloseAllSound();
        }
        else
        {
            m_DlgMain.WriteRecord();
            CMyShellDlg::RealExit();
        }
        break ;
    case ON_BACK_PLAY:
        {
            ::ShellExecute(m_hWnd, NULL, "Play.exe", NULL, NULL, SW_SHOW | SW_RESTORE );
            CMyShellDlg::RealExit();
        }
        break;
    case ON_EXIT_REFUSE:
        m_bExit = false ;
        if ( !m_bLogin )
        {
            if ( m_DlgLogin.m_bShow && !m_DlgLogin.IsWindowEnabled() )
            {
                m_DlgLogin.EnableWindow() ;
                m_DlgLogin.SetFocus() ;
                DXSetSoundVolume(75);
            }
            else if ( m_DlgRoleCreate.m_bShow && !m_DlgRoleCreate.IsWindowEnabled() )
            {
                m_DlgRoleCreate.ShowWindow(SW_SHOW) ;
                m_DlgRoleCreate.EnableWindow();
            }
        }
        break ;
    case ON_ROLECREATE_CREATE:
        m_DlgRoleCreate.ShowWindow(SW_HIDE);
        m_DlgRoleCreate.EnableWindow( false ) ;
        m_DlgConfirm.SetConfirmDlg( ROLECREATE_CONFIRM ) ;
        break ;
    case ON_MIN:
        if ( !m_bExit )
        {
            CMyShellDlg::GetParent()->ShowWindow ( SW_SHOWMINIMIZED ) ;
          
        }
        break ;
    case ON_HELPDLG_SHOW:
        CMyShellDlg::ShowHelpDlg ( lParam ) ;
        break ;
    case ON_LOGIN_NEW:
        {
            DXSetSoundVolume(75);
            m_DlgRoleCreate.ShowWindow(SW_SHOW);
            m_DlgRoleCreate.EnableWindow(true) ;
            char Acn[16];
            char Pws[16];
            m_DlgLogin.m_LoginAccountEdt.GetWindowText ( Acn, 16) ;
            m_DlgLogin.m_LoginPasswordEdt.GetWindowText ( Pws, 16) ;
            m_DlgRoleCreate.SetAcnAndPsw ( Acn, Pws ) ;
            m_DlgRoleCreate.SetFocus () ;
        }
        break ;
    case ON_ROLECHOOSE_RETURN:
        SetLoginEffectPos(m_bLogin);
        m_DlgLogin.ShowWindow ( SW_SHOW ) ;
        m_DlgLogin.SetCurLoginMode(2);
        m_DlgLogin.SetFocus() ;
        m_DlgLogin.m_LoginPasswordEdt.SetFocus();
        break ;
    case ON_ROLECHOOSE_CHOOSE:
        CMyShellDlg::CreateRole ( (BYTE)lParam ) ;
        break ;
    case ON_CLOSE_MDLG:
        if (m_bLogin)
        {
            m_DlgMain.CloseCurDlg(1);
        }
        break;
    case ON_DLGTASK_CLOSE:
        if (m_DlgTask.m_bShow)
        {
            m_DlgTask.EnableWindow(false);
        }
        break;
    case ON_EMOTION_INSERT:
        {
            m_DlgMain.m_DlgTalk.InsertEmotion(lParam);
            if (m_DlgMain.m_DlgEmotion.m_bShow)
            {
                m_DlgMain.m_DlgEmotion.EnableWindow (false);
            }
        }
        break;
    case ON_DLGEMOTION_OPEN:
        {
            if (m_DlgMain.m_DlgEmotion.m_bShow)
            {
                m_DlgMain.m_DlgEmotion.EnableWindow (false);
            }
            else
            {
                if (m_DlgLog.IsWindowVisible ())
                {
                    m_DlgLog.ShowWindow (SW_HIDE);
                }
                if (m_DlgMain.m_DlgMpcGoods.m_bShow)
                {
                    m_DlgMain.m_DlgMpcGoods.EnableWindow (false);
                }
                m_DlgMain.m_DlgEmotion.EnableWindow ();
            }
        }
        break;
    case ON_TALK_CLOSE:
        m_DlgMain.OnShellEnter () ;
        break ;
    case ON_MSGDLG_POINT:
        m_MsgDlgPnt.x = LOWORD ( lParam ) ;
        m_MsgDlgPnt.y = HIWORD ( lParam ) ;
        break ;
    case ON_MSGDLG_GOODS:
        g_bMouseMoveIn = true ;
        m_uMsgDlgType = ON_MSGDLG_GOODS ;
        m_dwMsgDlgOwner = lParam ;
        break ;
    case ON_MSGDLG_IMAGE:
        g_bMouseMoveIn = true ;
        m_uMsgDlgType = ON_MSGDLG_IMAGE;
        m_dwMsgDlgOwner = lParam ;
        break ;
    case ON_MSGDLG_FACE:
        g_bMouseMoveIn = true ;
        m_uMsgDlgType = ON_MSGDLG_FACE;
        m_dwMsgDlgOwner = lParam ;
        break ;
    case ON_MSGDLG_HIDE:
        g_bMouseMoveIn = false ;
        if ( !m_bExpShow )
        {
            g_objGameMap.SetShowExpFlag ( false ) ;
        }
        break ;
    case ON_MSGDLG_STATIC:
        g_bMouseMoveIn = true ;
        m_uMsgDlgType = ON_MSGDLG_STATIC ;
        // 判断是否点击按钮
        if (0xFF000000 == (0xFF000000 & lParam))
        {
            // 判断是一直显示进度槽
            // 是
            if (m_dwProgressNumber != (0xFFFFFF & lParam))
            {
                m_dwProgressNumber = 0xFFFFFF & lParam;
                m_bProgressHold = true;
                m_HoldX = 0;
                m_HoldY = 0;
            }
            // 否
            else
            {
                m_bProgressHold = false;
            }
            lParam &= 0xFFFFFF;
        }
        m_btStaticID = (BYTE)lParam ;
        break ;
    case ON_MSGDLG_TEST:
        break ;
    case ON_ICON_PICKUP:
        CMyShellDlg::OnIconPickUp ( lParam ) ;
        break ;
    case ON_ICON_DROP:
        CMyShellDlg::OnIconDrop ( lParam ) ;
        break ;
    case ON_ICON_ID:
        m_IconTypeID = lParam ;
        break ;
    case ON_ICON_PNT:
        m_IconMousePnt.x = HIWORD ( lParam ) ;
        m_IconMousePnt.y = LOWORD ( lParam ) ;
        break ;
    case ON_ICON_TRADE:
        CMyShellDlg::OnIconTrade ( lParam ) ;
        break ;
    case ON_ICON_USE:
        if ( !g_bPickUp )
        {
            CMyShellDlg::OnIconUse ( lParam ) ;
        }
        break ;
    case ON_ICON_QUSE:
        CMyShellDlg::OnIconQUse ( lParam ) ;
        break ;
    case ON_ICON_BUY:
        CMyShellDlg::OnIconBuy ( lParam ) ;
        break ;
    case ON_MONSTERHEAD_SET:
        m_DlgMain.m_DlgMonsterHeads.SetMonsterHeads (lParam);
        break;
    case ON_CHECKOUTEGG_ACCEPT:
        {
            CItem* pItem = g_objHero.GetPackageItemByIndex(lParam);
            if (pItem && pItem->GetPackageType() == PACKAGE_EGG
                    && g_objHero.GetPackageType() == PACKAGE_EUDEMON_BROODER)
            {
                if ( g_objHero.GetItemAmount(PACKAGE_EGG) >= g_objHero.GetPackageLimit (PACKAGE_EGG))
                {
                    g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10699) ) ;
                    m_DlgMain.m_DlgMonsterHatch.m_GridEggs.ReSumePickUp();
                    return 0;
                }
                g_objHero.CheckOutItem(pItem->GetID());
            }
        }
        break;
    case ON_FACTION_RENAME:
        break;
    case ON_FACTION_ADDENEMY:
        if (g_objHero.IsSynLeader())
        {
            m_DlgConfirm.SetConfirmDlg(36, NULL);
        }
        break;
    case ON_FACTION_ADDALLY:
        if (g_objHero.IsSynLeader())
        {
            if (m_DlgMain.m_DlgKnightage.m_bShow)
            {
                m_DlgMain.m_DlgKnightage.EnableWindow(false);
                m_DlgMain.m_uCurRDlgID = -1;
            }
            g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10345 ) ) ;
            m_uCmd = CMD_ADDALLY ;
            //			m_DlgConfirm.SetConfirmDlg(35,NULL);
        }
        break;
    case ON_FACTION_ADDALLYOK:
        if (g_objHero.IsSynLeader())
        {
            g_objHero.AddAllySyn(lParam);
        }
        break;
    case ON_MONSTER_OPTION:
        {
            for(int i = 0; i < g_objHero.GetItemAmount (PACKAGE_PET); i++)
            {
                CItem* pItem  = g_objHero.GetItemByIndex (i, PACKAGE_PET);
                if (pItem && pItem->GetID () == lParam)
                {
                    m_DlgMain.m_DlgMonster.SetCurMonster (i);
                    break;
                }
            }
        }
        break;
    case ON_MONSTER_CALL:
        {
            m_DlgMain.m_DlgMonsterHeads.AddCallMonster(lParam);
        }
        break;
    case ON_MONSTER_KILL:
        {
            m_DlgMain.m_DlgMonsterHeads.DelCallMonster(lParam);
        }
        break;
    case ON_MONSTER_CHOOSE:
        CMyShellDlg::OnMonsterChoose(lParam);
        break;
    case ON_ANNOUNCEINFO_OPEN:
        m_DlgMain.m_DlgAnnounceInfo.SetInfoMode(0);
        break;
    case ON_MPCGOODS_CLOSE:
        CMyShellDlg::OnMpcGoodsClose ( ) ;
        break ;
    case ON_MPCGOODS_MONEYDROP:
        m_DlgConfirm.SetConfirmDlg( 5, NULL ) ;
        break ;
    case ON_MONSTER_DROP:
        m_DlgConfirm.SetConfirmDlg( 32, NULL);
        break;
    case ON_MONSTERDROP_ACCEPT:
        m_DlgMain.m_DlgMonster.DropMonster();
        break;
    case ON_NPCBUY_CLOSE:
        m_DlgMain.m_uCurRDlgID = -1 ;
        m_bIconFix = false ;
        if ( m_DlgMain.m_DlgMpcGoods.m_bShow )
        {
            m_DlgMain.m_DlgMpcGoods.EnableWindow( false ) ;
        }
        if (m_DlgMain.m_DlgQuery.m_bShow)
        {
            m_DlgMain.m_DlgQuery.EnableWindow(false);
        }
        if ( g_bPickUp && m_PickUpDlgID == DLG_NPCBUY )
        {
            CMyShellDlg::OnIconDrop ( DLG_ALL ) ;
            g_bPickUp = false ;
        }
        m_DlgMain.m_DlgMpcGoods.SetGridContain ();
        m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
        break ;
    case ON_MAIN_TRADE:
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10065 ) ) ;//"请用鼠标左键选取交易对象" ) ;//
        m_uCmd = CMD_TRADE ;
        break ;
    case ON_TRADE_ACCEPT:
        if ( !m_DlgMain.m_DlgTrade.m_bShow )
        {
            g_objHero.AcceptTrade ( ) ;
        }
        break ;
    case ON_TRADE_CLOSE:
        m_DlgMain.m_uCurLDlgID = -1 ;
        if ( m_DlgMain.m_DlgMpcGoods.m_bShow )
        {
            m_DlgMain.m_DlgMpcGoods.EnableWindow( false ) ;
        }
        break ;
    case ON_TEAMMAGIC_SEL:
        {
            /*			CMagic* pMagic = g_objHero.GetMagic(lParam);
            			if (pMagic)
            			{
            				m_DlgMain.m_DlgXp.SetTeamMagicImg(pMagic->GetIDType());
            				m_DlgMain.m_DlgXp.ShowWindow(SW_SHOW);
            			}*/
        }
        break;
    case ON_TEAMMAGIC_OPEN:
        {
            if (m_DlgMain.m_DlgQuery.m_bShow && m_DlgMain.m_DlgQuery.m_btCurChildDlgID == 3)
            {
                m_DlgMain.m_DlgQuery.EnableWindow(false);
            }
            else
            {
                if (m_DlgMain.m_DlgQuery.m_bShow)
                {
                    m_DlgMain.m_DlgQuery.PopupCurChk();
                    m_DlgMain.m_DlgQuery.m_btCurChildDlgID = 3;
                    m_DlgMain.m_DlgQuery.m_ChkTSkill.SetCheckValue(1);
                    m_DlgMain.m_DlgQuery.m_DlgSkill.SetTeamMagicSkill();
                }
                else
                {
                    m_DlgMain.CloseCurDlg(1);
                    m_DlgMain.CloseCurDlg(2);
                    m_DlgMain.m_uCurRDlgID = DLG_QUERY;
                    m_DlgMain.m_DlgQuery.EnableWindow();
                    m_DlgMain.m_DlgQuery.PopupCurChk();
                    m_DlgMain.m_DlgQuery.m_btCurChildDlgID = 3;
                    m_DlgMain.m_DlgQuery.m_ChkTSkill.SetCheckValue(1);
                    m_DlgMain.m_DlgQuery.m_DlgSkill.SetTeamMagicSkill();
                }
            }
        }
        break;
    case ON_IMPROVE_CLOSE:
        m_DlgMain.m_uCurMDlgID = -1 ;
        if ( m_DlgMain.m_DlgMpcGoods.m_bShow )
        {
            m_DlgMain.m_DlgMpcGoods.EnableWindow(false) ;
        }
        break ;
    case ON_EMBED_CLOSE:
        m_DlgMain.m_uCurMDlgID = -1 ;
        if ( m_DlgMain.m_DlgMpcGoods.m_bShow )
        {
            m_DlgMain.m_DlgMpcGoods.EnableWindow( false ) ;
        }
        m_DlgMain.m_DlgMpcGoods.SetGridContain() ;
        m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
        break ;
    case ON_TASKITEM_UP:
        m_DlgMain.OpenTaskUpItem((UCHAR)lParam);
        break;
    case ON_TASKSET_OK:
        m_DlgMain.m_DlgPlayerTaskUp.SetTaskSetOk(lParam);
        break;
    case ON_TASKUP_OK:
        break;
    case ON_TASKVIEW_OPEN:
        if (!m_DlgMain.m_DlgPlayerTaskView.m_bShow)
        {
            m_DlgMain.CloseCurDlg(2);
            m_DlgMain.m_DlgPlayerTaskView.EnableWindow(true);
            m_DlgMain.m_uCurRDlgID = DLG_TASKVIEW;
        }
        break;
    case ON_TASKVIEW_CLOSE:
        if (m_DlgMain.m_DlgPlayerTaskView.m_bShow)
        {
            m_DlgMain.m_DlgPlayerTaskView.EnableWindow(false);
            m_DlgMain.m_uCurRDlgID = -1;
        }
        break;
    case ON_TASKVIEW_BACK:
        if (!m_DlgMain.m_DlgPlayerTaskList.m_bShow)
        {
            m_DlgMain.CloseCurDlg(2);
            m_DlgMain.m_uCurRDlgID = DLG_TASKLIST;
            m_DlgMain.m_DlgPlayerTaskList.EnableWindow(true);
            m_DlgMain.OpenTaskList(m_DlgMain.m_DlgPlayerTaskList.m_nListKind);
        }
        break;
    case ON_TASKUP_CLOSE:
        m_DlgMain.m_uCurRDlgID = -1;
        m_DlgMain.m_DlgMpcGoods.SetGridContain();
        m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
        break;
    case ON_EFFIGY_CLOSE:
        //		m_DlgMain.m_uCurDlgID = -1 ;
        break ;
    case ON_LAY_CLOSE:
        //		m_DlgMain.m_uCurDlgID = -1 ;
        break ;
    case ON_NPCFACE_CLOSE:
        //		m_DlgMain.m_uCurDlgID = -1 ;
        break ;
    case ON_MPCBOOTH_HIDE:
        if ( m_DlgMain.m_DlgMpcBooth.m_bShow )
        {
            m_DlgMain.m_DlgMpcBooth.EnableWindow(false) ;
            m_DlgMain.m_uCurLDlgID = -1;
        }
        break;
    case ON_MPCBOOTH_CLOSE:
        if ( m_DlgMain.m_DlgMpcBooth.m_bShow )
        {
            m_DlgMain.m_DlgMpcBooth.EnableWindow(false) ;
            m_DlgMain.m_uCurLDlgID = -1;
        }
        if (m_DlgMain.m_DlgHawk.m_bShow)
        {
            m_DlgMain.m_DlgHawk.EnableWindow(false);
        }
        g_objHero.m_objBooth.Close () ;
        g_objHero.Emotion (_ACTION_STANDBY);
        m_DlgMain.m_DlgXp.SetBooth(false);
        if (m_DlgMain.m_DlgMpcGoods.m_bShow)
        {
            m_DlgMain.m_DlgMpcGoods.EnableWindow(false);
        }
        break ;
    case ON_NPCBOOTH_CLOSE:
        g_objHero.m_objBooth.Close () ;
        break ;
    case ON_FRIEND_ADD:
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10345 ) ) ;
        m_uCmd = CMD_FRDMAKE ;
        break ;
    case ON_FRIEND_CHAT:
        {
            char strName[_MAX_NAMESIZE] = "";
            m_DlgMain.m_DlgEnemyFriend.m_EnemyLstData.GetText(lParam, strName);
            m_DlgMain.m_DlgTalk.m_TalkNameEdt.SetWindowText ( strName ) ;
            m_DlgMain.m_DlgTalk.SetTalkMode ( 5005 ) ;
            m_DlgMain.m_DlgTalk.m_TalkDataCob.SetFocus () ;
            m_DlgMain.m_DlgMsgReceive.EnableWindow(false);
        }
        break ;
    case ON_FRIEND_LEAVE:
        // Open the msg post dialog
        if ( !m_DlgMain.m_DlgMsgSend.m_bShow )
        {
            m_DlgMain.CloseCurDlg ( 1 ) ;
            m_DlgMain.m_uCurMDlgID = DLG_MSGSEND ;
            m_DlgMain.m_DlgMsgSend.EnableWindow() ;
            char strFriendName[16] ;
            m_DlgMain.m_DlgEnemyFriend.m_EnemyLstData.GetText ( lParam, strFriendName ) ;
            m_DlgMain.m_DlgMsgSend.SetTargetFriend ( strFriendName ) ;
        }
        break ;
    case ON_FRDSTATE_DEL:
        m_DlgConfirm.SetConfirmDlg( 6, NULL ) ;
        break ;
    case ON_FRIENDDEL_ACCEPT:
        m_DlgMain.m_DlgEnemyFriend.DeleteFriend () ;
        break ;
    case ON_FRIENDADD_ACCEPT:
        g_objHero.MakeFriend( lParam );
        break ;
    case ON_TALKDEL_ACCEPT:
        m_DlgMain.m_DlgTalk.DeleteTalkTarget () ;
        break ;
    case ON_GROUP_SETUP:
        if (!m_DlgMain.m_DlgGroup.m_MenuGroup.m_bShow)
        {
            if (m_DlgMain.m_DlgMpcGoods.m_bShow)
            {
                m_DlgMain.m_DlgMpcGoods.EnableWindow(false);
            }
            m_DlgMain.m_DlgGroup.m_MenuGroup.EnableWindow(true);
        }
        break ;
    case ON_GROUP_JOIN:
        m_uCmd = CMD_TEMJOIN ;
        break ;
    case ON_GROUP_INVITE:
        m_uCmd = CMD_TEMINVITE ;
        break ;
    case ON_TEAM_DISMISS:
        m_DlgConfirm.SetConfirmDlg(33, NULL); //确认解散队伍?
        break;
    case ON_TEAMDISMISS_ACCEPT:
        m_DlgMain.m_DlgGroup.TeamDismiss();
        break;
    case ON_TEAMJOIN_ACCEPT:
        g_objHero.TeamAgreeApply () ;
        break ;
    case ON_TEAMINVITE_ACCEPT:
        g_objHero.TeamAcceptInvite () ;
        break ;
    case ON_TALK_DEL:
        m_DlgConfirm.SetConfirmDlg ( 7 ) ;
        break ;
    case ON_TALK_SETUP:
        break ;
    case ON_TALK_LOGCLOSE:
        if ( m_DlgLog.IsWindowVisible () )
        {
            m_DlgLog.ShowWindow ( SW_HIDE ) ;
        }
        break;
    case ON_TALK_LOG:
        {
            if ( !m_DlgLog.IsWindowVisible () )
            {
                if (m_DlgMain.m_DlgEmotion.m_bShow)
                {
                    m_DlgMain.m_DlgEmotion.EnableWindow (false);
                }
                m_DlgMain.m_DlgSetup.EnableWindow(false);
                CRect rect, Hrect ;
                CMyShellDlg::GetWindowRect ( rect ) ;
                m_DlgLog.GetWindowRect ( Hrect ) ;
                m_DlgLog.MoveWindow ( rect.left + ( rect.Width () - Hrect.Width () ) / 2,
                                      rect.top + ( rect.Height () - Hrect.Height () ) / 2,
                                      Hrect.Width (),
                                      Hrect.Height () , false) ;
                m_DlgLog.ShowWindow ( SW_SHOW ) ;
            }
            else
            {
                m_DlgLog.ShowWindow ( SW_HIDE ) ;
            }
        }
        break ;
    case ON_MAIN_SCREENMOVE:
        m_bAlt = lParam ;
        if ( m_bAlt == 0 )
        {
            g_objGameMap.ResetViewPos () ;
        }
        break ;
    case ON_NPCBUY_FIX:
        if (lParam == DLG_QUERY)
        {
            if (m_bIconFix)
            {
                m_bIconFix = false ;
            }
        }
        else
        {
            if ( !g_bPickUp )
            {
                m_bIconFix = true ;
            }
        }
        break ;
    case ON_SETUP_MODECHANGE:
        if ( m_nScreenMode != lParam )
        {
            CMyShellDlg::SetScreenMode ( (BYTE)lParam, false ) ;
        }
        break ;
    case ON_SETUP_HOTKEY:
        break ;
    case ON_SETUP_OTHER:
        break ;
    case ON_MONSTER_HATCH:
        if (!m_DlgMain.m_DlgMonsterHatch.m_bShow)
        {
            m_DlgMain.OpenHatchDlg(lParam);
        }
        break;
    case ON_MONSTER_DEPOT:
        if (!m_DlgMain.m_DlgMonsterDepot.m_bShow)
        {
            m_DlgMain.OpenMonsterDepot(lParam);
        }
        break;
    case ON_MONSTER_IMP:
        if (!m_DlgMain.m_DlgMonsterImprove.m_bShow)
        {
            m_DlgMain.OpenMonsterImpDlg(1);
        }
        break;
    case ON_MONSTER_ALIVE:
        if (!m_DlgMain.m_DlgMonsterImprove.m_bShow)
        {
            m_DlgMain.OpenMonsterImpDlg(0);
        }
        break;
    case ON_MAGIC_CHOOSE:
        break ;
    case ON_XP_CHOOSE:
        {
            m_XpID = lParam ;
            for ( int i = 0; i < g_objHero.GetXpSkillAmount (); i++ )
            {
                CMagic* pXp = g_objHero.GetXpSkillByIndex ( i ) ;
                if ( pXp && pXp->GetIDType() == m_XpID )
                {
                    if (!g_objHero.TestStatus(USERSTATUS_XPFULL))
                    {
                        g_objHero.BurstXp();
                        m_DlgMain.m_DlgXp.ChooseXp(m_XpID);
                        break;
                    }
                    else
                    {
                        if ( pXp->TestTarget ( MAGIC_TARGET_NONE ) && pXp->m_infoMagicType.dwActionSort != MAGICSORT_LINE)
                        {
                            g_objHero.MagicAttack( m_XpID, g_objHero.GetID() ) ;
                            break;
                        }
                    }
                }
            }
        }
        break ;
    case ON_FACTIONDOC_OPEN:
        break;
    case ON_FACTION_BACK:
        m_DlgMain.OnMainFaction();
        break;
    case ON_FACTION_MANAGER:
        break;
    case ON_FACTION_REGROUP:
        break;
    case ON_FACTION_KICK:
        {
            char szName[_MAX_NAMESIZE] = "";
            if (m_DlgMain.m_DlgKnightage.m_DlgKnightageMemList.GetMemberName(lParam, szName))
            {
                m_DlgConfirm.SetConfirmDlg(12 , szName);
            }
        }
        break ;
    case CMD_ACTION_ADDALLY:
        {
            m_DlgConfirm.SetConfirmDlg(35 , NULL, g_objHero.GetLastSynAlly());
        }
        break;
    case ON_FACTION_SUB:
        m_DlgConfirm.SetConfirmDlg ( 9 ) ;
        break ;
    case ON_FACTION_INVITE:
        if (g_objHero.IsSynLeader())
        {
            if (m_DlgMain.m_DlgKnightage.m_bShow)
            {
                m_DlgMain.m_DlgKnightage.EnableWindow(false);
                m_DlgMain.m_uCurRDlgID = -1;
            }
            g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10701 )  ) ;
            m_uCmd = CMD_FACINVITE ;
        }
        else
        {
            g_objGameMsg.AddMsg(( char* )g_objGameDataSet.GetStr (100071 ));
        }
        break ;
    case ON_FACTION_JOIN:
        if (m_DlgMain.m_DlgKnightage.m_bShow)
        {
            m_DlgMain.m_DlgKnightage.EnableWindow(false);
            m_DlgMain.m_uCurRDlgID = -1;
        }
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10066 ) ) ;//"请用鼠标左键点击想要加入帮会的帮主。" ) ;//
        m_uCmd = CMD_FACJOIN ;
        break ;
    case ON_FACTION_OUT:
        m_DlgConfirm.SetConfirmDlg ( 8 ) ;
        break ;
    case ON_FACTIONINVITE_ACCEPT:
        g_objHero.SyndicateAcceptInvite () ;
        break ;
    case ON_FACTIONSUB_ACCEPT:
        break ;
    case ON_FACTIONJOIN_ACCEPT:
        g_objHero.SynDicateAcceptJoin () ;
        break ;
    case ON_FACTIONOUT_ACCEPT:
        g_objHero.LeaveSyndicate () ;
        break ;
    case ON_FACTIONDEL_ACCEPT:
        g_objHero.KickoutSynMember(m_DlgConfirm.GetConfirmText() ) ;
        break ;
    case ON_TEACHER_REQUEST:
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10345 ) ) ;
        m_uCmd = CMD_TEACHERREQ;
        break;
    case ON_STUDENT_REQUEST:
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10345 ) ) ;
        m_uCmd = CMD_STUDENTREQ;
        break;
    case ON_TEACHER_ACCEPT:
        g_objHero.AgreeTeacherApply();
        break;
    case ON_STUDENT_ACCEPT:
        g_objHero.AgreeStudentApply();
        break;
    case ON_OWNER_MOVE:
        m_bDlgMove = true ;
        CMyShellDlg::GetParent()->GetWindowRect ( m_InitDlgRect ) ;
        break ;
    case ON_OWNER_STOP:
        m_bDlgMove = false ;
        break ;
    case ON_MONEYDROP_ACCEPT:
        if ( lParam )
        {
            int xPos, yPos ;
            g_objHero.GetScrPos ( xPos, yPos ) ;
            DXPlaySound( "Sound/DropMoney.wav" );
            g_objHero.DropMoney ( lParam, xPos, yPos ) ;
        }
        break ;
    case ON_MSGSEND_VIEW:
        char strFriendName[16] ;
        if (lParam != -1)
        {
            m_DlgMain.m_DlgEnemyFriend.m_EnemyLstData.GetText ( lParam, strFriendName ) ;
        }
        else
        {
            char strTime[32];
            char strTxt[_MAX_WORDSSIZE];
            if (!g_objGameMsg.GetLeaveWordByIndex(0, strFriendName, strTxt, strTime))
            {
                g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10691)); //没有查询到朋友留言!!
                return 0;
            }
        }
        m_DlgMain.m_DlgMsgReceive.SetMsgReceiveName(strFriendName);
        if (!m_DlgMain.m_DlgEnemyFriend.m_bShow)
        {
            m_DlgMain.CloseCurDlg(2);
            m_DlgMain.m_uCurRDlgID = DLG_FRIEND;
            m_DlgMain.m_DlgEnemyFriend.EnableWindow(true);
        }
        if (m_DlgMain.m_DlgEnemyFriend.m_DlgSchoolList.m_bShow)
        {
            m_DlgMain.m_DlgEnemyFriend.m_DlgSchoolList.EnableWindow(false);
        }
        if (m_DlgMain.m_DlgEnemyFriend.m_DlgEnmState.m_bShow)
        {
            m_DlgMain.m_DlgEnemyFriend.m_DlgEnmState.EnableWindow(false);
        }
        m_DlgMain.CloseCurDlg ( 1 ) ;
        m_DlgMain.m_uCurMDlgID = DLG_MSGRECEIVE;
        m_DlgMain.m_DlgMsgReceive.EnableWindow() ;
        break ;
    case ON_MSGREC_REVERT:
        if (m_DlgMain.m_DlgEnemyFriend.m_DlgSchoolList.m_bShow)
        {
            m_DlgMain.m_DlgEnemyFriend.m_DlgSchoolList.EnableWindow(false);
        }
        if (m_DlgMain.m_DlgEnemyFriend.m_DlgEnmState.m_bShow)
        {
            m_DlgMain.m_DlgEnemyFriend.m_DlgEnmState.EnableWindow(false);
        }
        m_DlgMain.CloseCurDlg ( 1 ) ;
        m_DlgMain.m_uCurMDlgID = DLG_MSGSEND ;
        if ( lParam != NULL )
        {
            m_DlgMain.m_DlgMsgSend.SetTargetFriend( (char*)lParam ) ;
            delete[] (char*)lParam ;
        }
        m_DlgMain.m_DlgMsgSend.EnableWindow() ;
        break ;
    case ON_MSGREC_CLOSE:
        m_DlgMain.m_DlgProgress.m_ImgMsgFlash.EnableWindow(true);
        if (m_DlgMain.m_DlgEnemyFriend.m_iShowMode == 0)
        {
            m_DlgMain.m_DlgEnemyFriend.SetFriendList();
        }
        break ;
    case ON_BOOTHOPEN_ACCEPT:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer ( lParam ) ;
            if ( pPlayer != NULL )
            {
                if (!g_objHero.m_objBoothManager.IsActive())
                {
                    g_objHero.LockBooth(pPlayer);
                }
            }
        }
        break ;
    case ON_MPCBOOTH_ADDITEM:
        if (g_objHero.m_objBooth.GetItemAmount() >= 18)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10693)); //"您的摊位已满,不能在放置更多物品!"
        }
        else
        {
            m_DlgConfirm.SetConfirmDlg ( 10 ) ;
        }
        break ;
    case ON_MPCBOOTHADD_ACCEPT:
        {
            CItem* pItem = g_objHero.GetItem ( m_DlgMain.m_DlgMpcBooth.m_nCurRealID ) ;
            if ( pItem )
                g_objHero.m_objBoothManager.AddItem ( pItem->GetID (),
                                                      lParam ) ;
        }
        break ;
        /*	case ON_MPCBOOTH_CHANGE:
        		m_DlgConfirm.SetConfirmDlg ( 11 ) ;
        		break ;
        	case ON_MPCBOOTHCHG_ACCEPT:
        		CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( m_DlgMain.m_DlgMpcBooth.m_nCurChangeIndex ) ;
        		if ( pItem )
        		{
        			g_objHero.m_objBoothManager.SetPri ( pItem->GetID (),
        												  lParam ) ;
        		}
        		break ;*/
    case ON_MPCBOOTH_HAWK:
        if ( !m_DlgMain.m_DlgHawk.m_bShow )
        {
            m_DlgMain.m_DlgHawk.EnableWindow() ;
        }
        break ;
    case ON_CHAT_SIFT:
        break ;
    case ON_MPCCOURT_ACCEPT:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer ( lParam ) ;
            if ( pPlayer != NULL )
            {
                g_objHero.Marriage ( pPlayer ) ;
            }
        }
        break ;
    case ON_OTHER_DELNPC:
        m_DlgMain.m_DlgOther.ShowWindow ( SW_HIDE ) ;
        m_uCmd = CMD_DELNPC ;
        break ;
    case ON_MAIN_NPCEQUIP:
        if ( CMD_NPCEQUIP != m_uCmd)
        {
            m_uCmd = CMD_NPCEQUIP ;
        }
        else if ( CMD_NPCEQUIP == m_uCmd )
        {
            m_uCmd = CMD_NORMAL ;
        }
        break ;
    case ON_MAIN_EXPDOWN:
        m_bExpShow = !m_bExpShow ;
        if ( m_bExpShow )
        {
            g_objGameMap.SetShowExpFlag ( true ) ;
        }
        else
        {
            g_objGameMap.SetShowExpFlag ( false ) ;
        }
        break ;
    case ON_IMPTITEM_ACCEPT:
        {
            CItem* pItem = g_objHero.GetItem( lParam ) ;
            if ( pItem )
            {
                g_objHero.SellItem ( pItem->GetID () ) ;
            }
        }
        break ;
    case ON_DROPITEM_ACCEPT:
        {
            CItem* pItem = g_objHero.GetItem ( lParam ) ;
            if ( pItem )
            {
                if ( !g_objHero.DropItem ( pItem->GetID (), m_DropPnt.x, m_DropPnt.y ) )
                {
                    CMyShellDlg::WindowProc ( WM_MY_MESSAGE, CMD_FLASHITEM, NULL ) ;
                }
            }
        }
        break ;
    case ON_EMBED_TACKOFF:
        //		m_DlgConfirm.SetConfirmDlg ( 17 ) ;
        break ;
    case ON_TACKOFF_ACCEPT:
        //		m_DlgMain.m_DlgEmbed.TackOffEmbed () ;
        break ;
    case ON_SETUP_SHIFT:
        m_bShiftDown = lParam ;
        break ;
    case ON_JOINDICE_ACCEPT:
        g_objHero.m_objDiceManager.Open ( lParam ) ;
        break ;
    case ON_OUTDICE_ACCEPT:
        g_objHero.m_objDiceManager.Close () ;
        break ;
    case ON_DICE_LIST:
        m_DlgMain.m_DlgDiceList.ShowWindow ( !m_DlgMain.m_DlgDiceList.m_bShow ) ;
        break ;
    case ON_ENEMYDEL_ACCEPT:
        m_DlgMain.m_DlgEnemyFriend.DeleteEnemy () ;
        break ;
    case ON_FRDSTATE_SHOW:
        if ( m_DlgMain.m_DlgEnemyFriend.m_bShow )
        {
            m_DlgMain.m_DlgEnemyFriend.EnableWindow ( false ) ;
        }
        break ;
    case ON_ENMSTATE_DEL:
        m_DlgConfirm.SetConfirmDlg( 20, NULL ) ;
        break ;
    case ON_DICE_HIDE:
        if ( m_DlgMain.m_DlgDiceList.m_bShow )
        {
            m_DlgMain.m_DlgDiceList.ShowWindow ( SW_HIDE ) ;
        }
        break ;
    case ON_FRIEND_RESUME:
        m_DlgMain.m_DlgEnemyFriend.EnableWindow() ;
        break ;
    case ON_BLACK_CLOSE:
        break ;
    case ON_INTERFACE_CHANGE:
     
        m_DlgHelp.ResetBk ( lParam ) ;
        m_DlgLog.ResetBk ( lParam ) ;
        break ;
    case ON_INSHELP_SHOW:
        OpenInsHelpDlg();
        break ;
    case ON_RESOURCE_ACCEPT:
        ::ShellExecute(m_hWnd, NULL, g_objGameDataSet.GetStr ( 10367 ), NULL, NULL, SW_SHOW );
        break ;
    case ON_TALKACT_SHOW:
        if ( m_DlgMain.m_DlgPk.m_bShow )
        {
            m_DlgMain.m_DlgPk.ShowWindow ( SW_HIDE ) ;
        }
        break ;
    case ON_LEVWORD_CHAT:
        {
            BulletinTitleInfo* pBulletInfo = g_objHero.m_objBulletin.GetListLine ( lParam ) ;
            if ( pBulletInfo )
            {
                m_DlgMain.m_DlgTalk.m_TalkNameEdt.SetWindowText ( pBulletInfo->szName ) ;
                m_DlgMain.m_DlgTalk.SetTalkMode ( 5002 ) ;
                m_DlgMain.m_DlgTalk.m_TalkDataCob.SetFocus () ;
            }
        }
        break ;
    case ON_EMPTY_MSG:
        m_DlgConfirm.SetConfirmDlg( EMPTYMSG_CONFIRM ) ;
        break ;
    case ON_MSG_WARNING:
        m_DlgConfirm.SetConfirmDlg( WARNING_CONFIRM, NULL, lParam ) ;
        break ;
    case ON_ROLECREATE_ACCEPT:
        m_DlgRoleCreate.CreateRole() ;
        break ;
    case ON_ROLECREATE_REFUSE:
        m_DlgRoleCreate.ShowWindow(SW_SHOW);
        m_DlgRoleCreate.EnableWindow() ;
        break ;
    case ON_SPLITITEM_ACCEPT:
        {
            OBJID id = LOWORD( lParam ) ;
            int nNum = HIWORD( lParam ) ;
            g_objHero.SplitItem( id, nNum ) ;
        }
        break ;
    case ON_HEAD_LEVUP:
        {
            /*			if ( !m_DlgMain.m_DlgQuery.m_bShow )
            			{
            				m_DlgMain.OnMainQuery() ;
            			}

            			m_DlgMain.m_DlgQuery.OnQueryMission() ;
            */
        }
        break ;
    case ON_RSIDEDLG_OPEN:
        break ;
    case ON_RSIDEDLG_CLOSE:
        break ;
    case ON_CLICK_CHATSET:
        m_DlgMain.ResetMsgLines();
        break;
    case ON_MIPMAP_OPEN:
        OpenMipmap();
        break;
    case ON_BIGMAP_OPEN:
        OpenBigmap();
        break;
    default:
        break ;
    }
	return 0;
}

HHOOK glhHook ;
CDlgMain* gpDlgMain = NULL ;
CMyShellDlg* gpDlgShell = NULL ;
DWORD	gdwLastTime = 0 ;
int		gnLastCode = 0 ;
LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    if ( wparam == 13 )
    {
        if ( !gpDlgShell->m_bLogin)
        {
            return true ;
        }
        else if (gpDlgShell->m_DlgLogin.m_bShow && gpDlgShell->m_DlgLogin.m_nCurLoginMode == 2)
        {
            return CallNextHookEx(glhHook, nCode, wparam, lparam);
        }
        else if (gpDlgMain->m_DlgTalk.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgTalk.m_TalkDataCob.GetSafeHwnd()
                 && gpDlgMain->m_DlgTalk.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgTalk.m_TalkNameEdt.GetSafeHwnd()
                 && gpDlgShell->m_DlgTask.GetFocus()->GetSafeHwnd() != gpDlgShell->m_DlgTask.m_TaskEdtAnswer.GetSafeHwnd()
                 && gpDlgShell->m_DlgConfirm.GetFocus()->GetSafeHwnd() != gpDlgShell->m_DlgConfirm.m_ConfirmEdtData.GetSafeHwnd()
                 && gpDlgShell->m_DlgConfirm.GetFocus()->GetSafeHwnd() != gpDlgShell->m_DlgConfirm.m_ConfirmEdtName.GetSafeHwnd()
                 && gpDlgMain->m_DlgPlayerTaskUp.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgPlayerTaskUp.m_EditTaskName.GetSafeHwnd()
                 && gpDlgMain->m_DlgPlayerTaskUp.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgPlayerTaskUp.m_EditTaskInfo.GetSafeHwnd()
                 && gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskKiller.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskKiller.m_EditKillName.GetSafeHwnd()
                 && !gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskItem.m_bShow
                 && !gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskMonster.m_bShow
                 && gpDlgMain->m_DlgAnnounceUp.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgAnnounceUp.m_EditInfo.GetSafeHwnd()
                 && gpDlgMain->m_DlgAnnounceUp.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgAnnounceUp.m_EditTitle.GetSafeHwnd()
                 && gpDlgMain->m_DlgMsgSend.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgMsgSend.m_MsgSendData.GetSafeHwnd()
                 && gpDlgMain->m_DlgHawk.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgHawk.m_EdtData.GetSafeHwnd()
                 && gpDlgMain->m_DlgAddAucItem.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgAddAucItem.m_EditAucMoney.GetSafeHwnd()
                 && gpDlgMain->m_DlgBidAuction.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgBidAuction.m_EditBidValue.GetSafeHwnd()
                 && !gpDlgShell->m_DlgLogin.m_bShow
                 && !gpDlgShell->m_DlgRoleCreate.m_bShow
                 && gpDlgMain->m_DlgKnightage.m_DlgKnightageBillBoard.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgKnightage.m_DlgKnightageBillBoard.m_BillBoardEdt.GetSafeHwnd()
                 && gpDlgMain->m_DlgSetup.m_DlgSChat.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgSetup.m_DlgSChat.m_EditBlackName.GetSafeHwnd()
                 && gpDlgMain->m_DlgMonster.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgMonster.m_EditName.GetSafeHwnd()
                 && gpDlgMain->m_DlgKnightage.m_DlgKnightageBase.GetFocus()->GetSafeHwnd() != gpDlgMain->m_DlgKnightage.m_DlgKnightageBase.m_EditMoney.GetSafeHwnd())
        {
            gpDlgMain->m_DlgTalk.m_TalkDataCob.SetFocus () ;
            gpDlgMain->m_DlgTalk.m_bHotEnter = false;
            return true ;
        }
        else
        {
            return CallNextHookEx(glhHook, nCode, wparam, lparam);
        }
    }
    // 输入框有焦点
    if ( gpDlgShell->m_DlgLogin.m_bShow
            || gpDlgShell->m_DlgRoleCreate.m_bShow
            || gpDlgShell->m_DlgConfirm.GetFocus()->GetSafeHwnd() == gpDlgShell->m_DlgConfirm.m_ConfirmEdtData.GetSafeHwnd()
            || gpDlgShell->m_DlgConfirm.GetFocus()->GetSafeHwnd() == gpDlgShell->m_DlgConfirm.m_ConfirmEdtName.GetSafeHwnd()
            || gpDlgMain->GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgTalk.m_TalkDataCob.GetSafeHwnd()
            || gpDlgMain->m_DlgTrade.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgTrade.m_TradeEdtMpcMoney.GetSafeHwnd()
            || gpDlgMain->m_DlgTalk.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgTalk.m_TalkNameEdt.GetSafeHwnd()
            || gpDlgMain->m_DlgDepot.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgDepot.m_DepotEdtMoney.GetSafeHwnd()
            || gpDlgShell->m_DlgTask.GetFocus()->GetSafeHwnd() == gpDlgShell->m_DlgTask.m_TaskEdtAnswer.GetSafeHwnd()
            || gpDlgMain->m_DlgPlayerTaskUp.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgPlayerTaskUp.m_EditMoney.GetSafeHwnd()
            || gpDlgMain->m_DlgPlayerTaskUp.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgPlayerTaskUp.m_EditTaskName.GetSafeHwnd()
            || gpDlgMain->m_DlgPlayerTaskUp.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgPlayerTaskUp.m_EditTaskInfo.GetSafeHwnd()
            || gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskKiller.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskKiller.m_EditKillName.GetSafeHwnd()
            || gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskItem.m_bShow
            || gpDlgMain->m_DlgPlayerTaskUp.m_DlgTaskMonster.m_bShow
            || gpDlgMain->m_DlgAnnounceUp.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgAnnounceUp.m_EditInfo.GetSafeHwnd()
            || gpDlgMain->m_DlgAnnounceUp.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgAnnounceUp.m_EditTitle.GetSafeHwnd()
            || gpDlgMain->m_DlgMsgSend.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgMsgSend.m_MsgSendData.GetSafeHwnd()
            || gpDlgMain->m_DlgHawk.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgHawk.m_EdtData.GetSafeHwnd()
            || gpDlgMain->m_DlgAddAucItem.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgAddAucItem.m_EditAucMoney.GetSafeHwnd()
            || gpDlgMain->m_DlgBidAuction.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgBidAuction.m_EditBidValue.GetSafeHwnd()
            || gpDlgMain->m_DlgKnightage.m_DlgKnightageBillBoard.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgKnightage.m_DlgKnightageBillBoard.m_BillBoardEdt.GetSafeHwnd()
            || gpDlgMain->m_DlgSetup.m_DlgSChat.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgSetup.m_DlgSChat.m_EditBlackName.GetSafeHwnd()
            || gpDlgMain->m_DlgMonster.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgMonster.m_EditName.GetSafeHwnd()
            || gpDlgMain->m_DlgKnightage.m_DlgKnightageBase.GetFocus()->GetSafeHwnd() == gpDlgMain->m_DlgKnightage.m_DlgKnightageBase.m_EditMoney.GetSafeHwnd())
    {
        if (wparam < 112 || wparam > 123)
        {
            return CallNextHookEx(glhHook, nCode, wparam, lparam);
        }
    }
    // Test if is the useful key
    if ( (wparam < 16 ||
            ( wparam > 18 && wparam < 37 ) ||
            ( wparam > 40 && wparam < 48 ) ||
            ( wparam > 90 && wparam < 112) ||
            wparam > 123 ) && wparam != 27) //|| gpDlgMain->m_DlgHotkey.m_bShow )
    {
        return CallNextHookEx(glhHook, nCode, wparam, lparam);
    }
    // Is hotkey but system key not down
    int nValue = 0 ;
    BYTE btState[256] ;
    GetKeyboardState ( btState ) ;
    nValue += btState[VK_SHIFT] >> 7 ;
    // Test if the key input is "%"
    if ( nValue && wparam == 53 )
    {
        return true;
    }
    nValue += btState[VK_MENU] >> 7 ;
    nValue += btState[VK_CONTROL] >> 7 ;
    //	if ( wparam >=58 && wparam <= 90 && !nValue )
    //		return CallNextHookEx(glhHook,nCode,wparam,lparam);
    // Process the key hold
    static BOOL bKeyUp = true ;
    if ( !bKeyUp )
    {
        if ( lparam & 0x80000000 )
        {
            bKeyUp = true ;
        }
    }
    if ( gnLastCode == wparam )
    {
        if ( ( TimeGet() - gdwLastTime ) < 200 )
        {
            return CallNextHookEx(glhHook, nCode, wparam, lparam);
        }
        else
        {
            gdwLastTime = TimeGet() ;
        }
    }
    else
    {
        gnLastCode = wparam ;
        gdwLastTime = TimeGet() ;
    }
    if (wparam == 27) //esc
    {
        if ( g_objHero.IsTradeActive() )
        {
            g_objGameMsg.AddMsg( g_objGameDataSet.GetStr( 10026 ) ) ;
            return true;
        }
        if ( g_bPickUp )
        {
            g_bPickUp = false ;
            gpDlgShell->OnIconDrop(DLG_ALL);
            return true;
        }
        if (gpDlgShell->CloseCurDlg() && !gpDlgShell->m_bExit)
        {
            gpDlgShell->m_DlgConfirm.SetConfirmDlg( EXIT_CONFIRM, NULL, 0 ) ;
            gpDlgShell->m_bExit = true ;
            return true;
        }
    }
    if ( wparam >= 59 && wparam <= 90 )
        if ( gpDlgShell->ProcessHotkey ( wparam ) )
        {
            return true ;
        }
    int nIndex = -1;
    int nHotKey = -1;
    switch ( wparam )
    {
    case 37:
        if ( btState[VK_CONTROL] >> 7 )
        {
            gpDlgMain->m_DlgTalk.CycleTalkTarget ( SB_LEFT ) ;
            return true ;
        }
        break ;
    case 38:
        if ( btState[VK_CONTROL] >> 7 )
        {
            gpDlgMain->m_DlgTalk.CycleTalkChannel ( SB_TOP ) ;
            return true ;
        }
        break ;
    case 39:
        if ( btState[VK_CONTROL] >> 7 )
        {
            gpDlgMain->m_DlgTalk.CycleTalkTarget ( SB_RIGHT ) ;
            return true ;
        }
        break ;
    case 40:
        if ( btState[VK_CONTROL] >> 7 )
        {
            gpDlgMain->m_DlgTalk.CycleTalkChannel ( SB_BOTTOM ) ;
            return true ;
        }
        break ;
    case 48:		//number 0
        nIndex = 9 ;
        break ;
    case 49:		//number 1
        nIndex = 0 ;
        break ;
    case 50:
        nIndex = 1 ;
        break ;
    case 51:
        nIndex = 2 ;
        break ;
    case 52:
        nIndex = 3 ;
        break ;
    case 53:
        nIndex = 4 ;
        break ;
    case 54:
        nIndex = 5 ;
        break ;
    case 55:
        nIndex = 6 ;
        break ;
    case 56:
        nIndex = 7 ;
        break ;
    case 57:
        nIndex = 8 ;
        break ;
    case 112:		//F1
        nHotKey = 1 ;
        break ;
    case 113:
        nHotKey = 2 ;
        break ;
    case 114:
        nHotKey = 3 ;
        break ;
    case 115:
        nHotKey = 4;
        break ;
    case 116:
        nHotKey = 5;
        break ;
    case 117:
        nHotKey = 6;
        break ;
    case 118:	//F7
        nHotKey = 7;
        break ;
    case 119:  //F8
        nHotKey = 8;
        break ;
    case 123:
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10067 ) ) ;//"屏幕截图，截取的图片存放在游戏目录下的ScreenShot目录中" ) ;	//
        CMyBitmap::ScreenShot ( NULL ) ;
        gdwLastTime = TimeGet() ;
        return true ;
    }
    if ( nIndex != -1 )
    {
        if ( gpDlgMain->m_DlgTalk.m_GoodBox.GetGridAble ( CPoint ( nIndex + 1, 1 ) ) )
        {
            LPARAM lParam = nIndex | gpDlgMain->m_DlgTalk.m_GoodBox.GetIconTypeID ( nIndex ) << 8 ;
            if ( bKeyUp )
            {
                gpDlgShell->OnIconQUse ( lParam ) ;
                bKeyUp = false ;
            }
        }
        return true;
    }
    if (nHotKey != -1)
    {
        if (gpDlgMain->m_DlgQuery.m_DlgSkill.m_bShow
                && (gpDlgMain->m_DlgQuery.m_btCurChildDlgID == 2 || gpDlgMain->m_DlgQuery.m_btCurChildDlgID == 3))
        {
            int nMagicIndex = gpDlgMain->m_DlgQuery.m_DlgSkill.GetMouseInMagicIndex();
            if (nMagicIndex)
            {
                if (gpDlgMain->m_DlgQuery.m_btCurChildDlgID == 2)
                {
                    gpDlgMain->m_DlgQuery.m_DlgSkill.SetMagicHotKey(nMagicIndex - 1, nHotKey - 1);
                }
                else if (gpDlgMain->m_DlgQuery.m_btCurChildDlgID == 3 && g_objHero.IsTeamLeader())
                {
                    gpDlgMain->m_DlgQuery.m_DlgSkill.SetTeamMagicHotKey(nMagicIndex - 1, nHotKey - 1);
                }
                return CallNextHookEx(glhHook, nCode, wparam, lparam);
            }
        }
        if ( btState[VK_CONTROL] >> 7 )
        {
            gpDlgShell->AttackMagic(nHotKey - 1, true);
        }
        else
        {
            gpDlgShell->AttackMagic(nHotKey - 1, false);
        }
    }
    return CallNextHookEx(glhHook, nCode, wparam, lparam);
}

void CMyShellDlg::StartGame()
{
    if ( !m_bLogin )
    {
        m_bLogin = true ;
        m_DlgLogin.m_LoginPasswordEdt.SetWindowText( NULL ) ;
        m_DlgLogin.EndDialog ( 0 ) ;
        m_DlgRoleCreate.EndDialog ( 0 ) ;
        CRect rect ;
        // Set the system show state
        CMyPos pos;
        pos.x = 0;
        pos.y = 620;
        g_objGameMsg.SetNorAreaShowStates( pos, 715 ) ;
        g_objGameMsg.SetShowLines(5);
        g_objGameMsg.SetShowLinesEx(5, 5) ;
        // Init the task dialog
        m_DlgTask.Create ( IDD_DIALOG_TASK, this ) ;
        m_DlgTask.GetWindowRect( rect ) ;
        m_DlgTask.MoveWindow( (_SCR_WIDTH - 512) / 2,
                              135,
                              512,
                              rect.Height() ) ;
        m_DlgTask.ShowWindow( SW_SHOW ) ;
        m_DlgTask.EnableWindow( false ) ;
        // Init the main dialog
        m_DlgMain.Create ( IDD_MAIN_DIALOG, this ) ;
        m_DlgMain.GetWindowRect ( rect ) ;
        m_DlgMain.MoveWindow ( _SCR_WIDTH - rect.Width(),
                               _SCR_HEIGHT - rect.Height(),
                               rect.Width(),
                               rect.Height() ) ;
        m_DlgMain.ReSetDlgRgn(false);
        m_DlgMain.ShowWindow ( SW_SHOW ) ;
        m_DlgMain.m_DlgProgress.EnableWindow() ;
        m_DlgMain.m_DlgMipmap.EnableWindow() ;
        m_DlgMain.m_DlgTalk.EnableWindow() ;
        // set the dlgsetup's mode
        m_DlgMain.m_DlgGroup.SetGroupContain () ;
        m_DlgMain.m_DlgMonsterHeads.ResetCallMonster();
        m_DlgMain.m_DlgMonster.ResetMonsterDlg();
        // Set the font
        m_DlgTask.m_TaskEdtAnswer.SetFont ( m_pGameFont ) ;
        m_DlgMain.m_DlgDepot.m_DepotEdtMoney.SetFont ( m_pGameFont ) ;
        m_DlgMain.m_DlgTalk.m_TalkDataCob.SetFont ( m_pGameFont ) ;
        m_DlgMain.m_DlgTalk.m_TalkNameEdt.SetFont ( m_pGameFont ) ;
        m_DlgMain.m_DlgTrade.m_TradeEdtMpcMoney.SetFont ( m_pGameFont ) ;
        m_DlgMain.m_DlgMsgSend.m_MsgSendData.SetFont ( m_pGameFont ) ;
        m_DlgMain.m_DlgHawk.m_EdtData.SetFont ( m_pGameFont ) ;
        m_DlgConfirm.m_ConfirmEdtData.SetFont( m_pGameFont );
        m_DlgConfirm.m_ConfirmEdtName.SetFont( m_pGameFont );
        m_DlgMain.m_DlgPlayerTaskUp.m_EditMoney.SetFont(m_pGameFont);
        m_DlgMain.m_DlgPlayerTaskUp.m_EditTaskInfo.SetFont(m_pGameFont);
        m_DlgMain.m_DlgPlayerTaskUp.m_EditTaskName.SetFont(m_pGameFont);
        m_DlgMain.m_DlgPlayerTaskUp.m_DlgTaskKiller.m_EditKillName.SetFont(m_pGameFont);
        m_DlgMain.m_DlgAnnounceUp.m_EditInfo.SetFont(m_pGameFont);
        m_DlgMain.m_DlgAnnounceUp.m_EditTitle.SetFont(m_pGameFont);
        m_DlgMain.m_DlgKnightage.m_DlgKnightageBillBoard.m_BillBoardEdt.SetFont(m_pGameFont);
        m_DlgMain.m_DlgAddAucItem.m_EditAucMoney.SetFont(m_pGameFont);
        m_DlgMain.m_DlgBidAuction.m_EditBidValue.SetFont(m_pGameFont);
        m_DlgMain.m_DlgSetup.m_DlgSChat.m_EditBlackName.SetFont(m_pGameFont);
        m_DlgMain.m_DlgMonster.m_EditMonster1.SetFont(m_pGameFont);
        m_DlgMain.m_DlgMonster.m_EditMonster2.SetFont(m_pGameFont);
        m_DlgMain.m_DlgMonster.m_EditMonster3.SetFont(m_pGameFont);
        m_DlgMain.m_DlgMonster.m_EditMonster4.SetFont(m_pGameFont);
        m_DlgMain.m_DlgMonster.m_EditName.SetFont(m_pGameFont);
        m_DlgMain.m_DlgKnightage.m_DlgKnightageBase.m_EditMoney.SetFont(m_pGameFont);
        // Init the static rect
        CMyShellDlg::GetWindowRect ( rect ) ;
        CRect PrgRect ;
        //Hp
        m_DlgMain.m_DlgProgress.m_PrgHeroHp.GetWindowRect ( PrgRect ) ;
        m_StaticPoint[0] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //Mana
        m_DlgMain.m_DlgProgress.m_PrgHeroMp.GetWindowRect ( PrgRect ) ;
        m_StaticPoint[1] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //Force
        m_DlgMain.m_DlgProgress.m_PrgHeroForce2.GetWindowRect ( PrgRect ) ;
        m_StaticPoint[2] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 4 ) ;
        //Xp
        m_DlgMain.m_DlgProgress.m_PrgHeroXp.GetWindowRect( PrgRect ) ;
        m_StaticPoint[3] = CPoint (PrgRect.left - rect.left + 20, PrgRect.top - rect.top - 10 ) ;
        //Exp
        m_DlgMain.m_DlgTalk.m_PrgExp.GetWindowRect( PrgRect ) ;
        m_StaticPoint[4] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //pk mode
        m_DlgMain.m_DlgTalk.m_TalkPkChk.GetWindowRect(PrgRect);
        m_StaticPoint[5] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //mipmap
        m_DlgMain.m_DlgTalk.m_TalkMipmapBtn.GetWindowRect(PrgRect);
        m_StaticPoint[6] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //Emotion str
        m_DlgMain.m_DlgTalk.m_EmotionListBtn.GetWindowRect(PrgRect);
        m_StaticPoint[7] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //Help button
        m_DlgMain.m_DlgTalk.m_TalkHelpBtn.GetWindowRect(PrgRect);
        m_StaticPoint[8] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //equip view
        m_DlgMain.m_DlgTalk.m_ViewEquipBtn.GetWindowRect(PrgRect);
        m_StaticPoint[9] = CPoint ( PrgRect.left - rect.left, PrgRect.top - rect.top - 20 ) ;
        //Query
        ////----////
        m_DlgMain.m_DlgGroup.m_GroupImg1.GetWindowRect(PrgRect);
        m_DlgMain.m_DlgGroup.m_ImgRect[0] = CRect (PrgRect.left - rect.left, PrgRect.top - rect.top,
                                            PrgRect.right - rect.left, PrgRect.bottom - rect.top);
        m_DlgMain.m_DlgGroup.m_GroupImg2.GetWindowRect(PrgRect);
        m_DlgMain.m_DlgGroup.m_ImgRect[1] = CRect (PrgRect.left - rect.left, PrgRect.top - rect.top,
                                            PrgRect.right - rect.left, PrgRect.bottom - rect.top);
        m_DlgMain.m_DlgGroup.m_GroupImg3.GetWindowRect(PrgRect);
        m_DlgMain.m_DlgGroup.m_ImgRect[2] = CRect (PrgRect.left - rect.left, PrgRect.top - rect.top,
                                            PrgRect.right - rect.left, PrgRect.bottom - rect.top);
        m_DlgMain.m_DlgGroup.m_GroupImg4.GetWindowRect(PrgRect);
        m_DlgMain.m_DlgGroup.m_ImgRect[3] = CRect (PrgRect.left - rect.left, PrgRect.top - rect.top,
                                            PrgRect.right - rect.left, PrgRect.bottom - rect.top);
        // statr keyboard hook
        glhHook = SetWindowsHookEx ( WH_KEYBOARD,
                                     KeyBoardProc,
                                     NULL,
                                     GetCurrentThreadId() );
        if ( glhHook != NULL )
        {
            gpDlgMain = &m_DlgMain ;
            gpDlgShell = this ;
        }
    }
    else
    {
        m_DlgMain.m_DlgGroup.SetGroupContain () ;
        m_DlgMain.m_DlgMonsterHeads.ResetCallMonster();
        m_DlgMain.m_DlgMonster.ResetMonsterDlg();
        m_DlgMain.m_DlgMonsterHeads.EnableWindow (false);
        m_DlgMain.m_DlgXp.ShowWindow(SW_HIDE);
        m_DlgMain.m_DlgProgress.m_nHeroXp = 0;
        m_MagicID = 0 ;
    }
    m_DlgMain.m_bReacord = false;
    CMyShellDlg::SetFocus () ;
}

void CMyShellDlg::CreateRole ( BYTE btRoleType )
{
    m_DlgRoleCreate.ShowWindow(SW_SHOW);
    m_DlgRoleCreate.EnableWindow(true) ;
    m_DlgRoleCreate.SetFocus () ;
}

void CMyShellDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( m_bDlgMove )
    {
        if ( !nFlags & MK_LBUTTON )
        {
            m_bDlgMove = false ;
        }
    }
    MouseSet(point.x, point.y, 0);
    if (!g_bPickUp && !m_bIconFix)
    {
        CMyShellDlg::SetGameCursor();
    }
    CDialog::OnMouseMove(nFlags, point);
}

void CMyShellDlg::SetGameCursor()
{
    CPlayer* pTarget	= g_objPlayerSet.GetFocusPlayer();
    if ( pTarget )
    {
        if ( pTarget->IsNpc () &&  !pTarget->IsMonster()
                &&	!g_bPickUp )
        {
            NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(pTarget->GetLook());
            if (pInfo && pInfo->bMouseSign)
            {
                switch ( pTarget->GetRoleType() )
                {
                case _ROLE_STONE_MINE:
                    ::SetCursor(NULL);
                    SetCursorAni("StoneCursor");
                    break;
                case _ROLE_SHOPKEEPER_NPC:
                    ::SetCursor(NULL);
                    SetCursorAni("NpcCursor");
                    break ;
                case _ROLE_TASK_NPC:
                    ::SetCursor(NULL);
                    SetCursorAni("NpcCursor");
                    break ;
                case _ROLE_STORAGE_NPC:
                    ::SetCursor(NULL);
                    SetCursorAni("NpcCursor");
                    break;
                case _ROLE_TRUNCK_NPC:
                    ::SetCursor(NULL);
                    SetCursorAni("NpcCursor");
                    break;
                default:
                    ::SetCursor(NULL);
                    SetCursorAni("NpcCursor");
                    break ;
                }
            }
        }
        else if (pTarget->IsMonster())
        {
            ::SetCursor(NULL);
            SetCursorAni("PkCursor");
        }
        else
        {
            if ( m_uCmd == CMD_NORMAL && !pTarget->IsMyPet())
            {
                switch ( g_objHero.GetPkMode () )
                {
                case 0:
                    if ( !g_bPickUp )
                    {
                        ::SetCursor( NULL ) ;
                        SetCursorAni("PkCursor");
                    }
                    break ;
                case 1:
                    break ;
                case 2:
                    if ( !g_objHero.IsTeamMember ( pTarget->GetID () )  && !g_bPickUp )
                    {
                        ::SetCursor( NULL ) ;
                        SetCursorAni("PkCursor");
                    }
                    break ;
                }
            }
            else if ( ( m_uCmd == CMD_TEMJOIN || m_uCmd == CMD_TEMINVITE ) && m_hCursorFriend != NULL )
            {
                SetCursorAni("");
                ::SetCursor( m_hCursorFriend ) ;
            }
        }
    }
    else
    {
        CMyPos Pos = { 0, 0 };
        if ( !g_bPickUp && g_objGameMap.GetFocusMapItem (Pos) != ID_NONE && m_hCursorH != NULL )
        {
            SetCursorAni("");
            ::SetCursor ( m_hCursorH ) ;
        }
    }
}

void CMyShellDlg::SetMsgDlgPnt(LPARAM lParam, int nWidth)
{
    /*	int nMsgDlgHeight = lParam * 18 ;
    	if ( m_MsgDlgPnt.x + nWidth > _SCR_WIDTH )
    		m_MsgDlgPnt.x -= (m_MsgDlgPnt.x + nWidth) - _SCR_WIDTH ;
    	if ( m_MsgDlgPnt.y + nMsgDlgHeight > _SCR_HEIGHT )
    	{
    		if ( m_MsgDlgPnt.y - 33 - nMsgDlgHeight < 0 )
    			m_MsgDlgPnt.y = 0 ;
    		else
    			m_MsgDlgPnt.y -= 33 + nMsgDlgHeight ;
    	}		*/
}

void CMyShellDlg::ShowMsgDlg()
{
    static UINT uiDT;
    // 判断两次对话框是否一样
    // 否
    if (uiDT != m_uMsgDlgType)
    {
        m_bInJustNow = true;
        uiDT = m_uMsgDlgType;
    }
    switch ( m_uMsgDlgType )
    {
    case ON_MSGDLG_GOODS:
        {
            switch ( m_dwMsgDlgOwner )
            {
            case DLG_TASKGET:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgPlayerTaskUp.m_GridTaskItem.GetPickUpRealID ();
                    CItem* pItem = g_objHero.GetItem( dwIndex ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgPlayerTaskUp.m_bShow)
                    {
                        ShowMessage ( pItem, -1 ) ;
                    }
                }
                break;
            case DLG_TASKVIEW:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgPlayerTaskView.m_GridItem.GetMouseMoveIndex();
                    if (g_objHero.m_infoTask.infoItem[dwIndex].idItemType
                            && m_DlgMain.m_DlgPlayerTaskView.m_bShow)
                    {
                        OBJID idTask = g_objHero.m_infoTask.idTask;
                        OBJID idItemType = g_objHero.m_infoTask.infoItem[dwIndex].idItemType;
                        CItem pTaskItem;
                        if (pTaskItem.Create(idTask, idItemType))
                        {
                            ShowMessage(&pTaskItem);
                        }
                    }
                }
                break;
            case DLG_MONSTERHATCH:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgMonsterHatch.m_GridEggs.GetMouseMoveIndex();
                    CItem* pItem = g_objHero.GetPackageItemByIndex(dwIndex);
                    if (pItem != NULL && m_DlgMain.m_DlgMonsterHatch.m_bShow)
                    {
                        ShowMessage(pItem);
                    }
                }
                break;
            case DLG_MONSTERIMPROVE:
                {
                    DWORD dwID = m_DlgMain.m_DlgMonsterImprove.GetSoulStoneID() ;
                    CItem* pItem = g_objHero.GetItem(dwID) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgMonsterImprove.m_bShow)
                    {
                        ShowMessage ( pItem) ;
                    }
                }
                break;
            case DLG_NPCBUY:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.GetMouseMoveIndex () ;
                    CItem* pItem = g_objShop.GetItem ( dwIndex ) ;
                    if (pItem != NULL && m_DlgMain.m_DlgNpcBuy.m_bShow)
                    {
                        ShowMessage ( pItem ) ;
                    }
                }
                break ;
            case DLG_MONSTEREGG:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgMonster.m_EggsGri.GetPickUpRealID() ;
                    CItem* pItem = g_objHero.GetItem( dwIndex ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgMonster.m_bShow)
                    {
                        ShowMessage ( pItem, -1 ) ;
                    }
                }
                break;
            case DLG_STONEGOODS0:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.GetPickUpRealID() ;
                    CItem* pItem = g_objHero.GetItem( dwIndex ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgMpcGoods.m_bShow && m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.IsWindowEnabled())
                    {
                        ShowMessage ( pItem, -1 ) ;
                    }
                }
                break;
            case DLG_MPCGOODS:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetMouseMoveIndex () ;
                    CItem* pItem = g_objHero.GetItemByIndex ( dwIndex ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgMpcGoods.m_bShow &&  m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.IsWindowEnabled())
                    {
                        ShowMessage ( pItem,  -1 ) ;
                    }
                }
                break ;
            case DLG_NPCEQUIP:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgNpcEquip.GetMouseMovePosition() ;
                    CItem* pItem = g_objHero.m_objPlayerDummy.GetEquipment(dwIndex);
                    if (pItem != NULL && m_DlgMain.m_DlgNpcEquip.m_bShow)
                    {
                        ShowMessage(pItem);
                    }
                }
                break;
            case DLG_EQUIP:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgQuery.m_DlgEquip.GetMouseMovePosition() ;
                    CItem* pItem = g_objHero.GetEquipment( dwIndex ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgQuery.m_DlgEquip.m_bShow)
                    {
                        ShowMessage ( pItem, -1 ) ;
                    }
                }
                break ;
            case DLG_ADDAUCITEM:
                {
                    DWORD dwID = m_DlgMain.m_DlgAddAucItem.m_ImgAucItem.GetImageRealID();
                    CItem* pItem = g_objHero.GetItem(dwID) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgAddAucItem.m_bShow)
                    {
                        ShowMessage ( pItem) ;
                    }
                }
                break;
            case DLG_IMPROVE:
                {
                    DWORD dwID = m_DlgMain.m_DlgImprove.GetMouseMoveId() ;
                    CItem* pItem = g_objHero.GetItem(dwID) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgImprove.m_bShow)
                    {
                        ShowMessage ( pItem) ;
                    }
                }
            case DLG_TRADEMPC:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgTrade.m_TradeGriMpc.GetMouseMoveIndex () ;
                    CItem* pItem = g_objHero.GetHeroTradeItemByIndex ( dwIndex  ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgTrade.m_bShow)
                    {
                        ShowMessage ( pItem) ;
                    }
                }
                break ;
            case DLG_TRADENPC:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgTrade.m_TradeGriNpc.GetMouseMoveIndex () ;
                    CItem* pItem = g_objHero.GetPlayerTradeItemByIndex ( dwIndex ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgTrade.m_bShow)
                    {
                        ShowMessage ( pItem) ;
                    }
                }
                break ;
            case DLG_IDENTIFY:
                {
                    DWORD dwID = m_DlgMain.m_DlgIdentify.GetIdentifyItemId();
                    CItem* pItem = g_objHero.GetItem(dwID);
                    if (pItem != NULL && m_DlgMain.m_DlgIdentify.m_bShow)
                    {
                        ShowMessage(pItem);
                    }
                }
                break;
            case DLG_AUCTION:
                {
                    CItem* pItem = g_objHero.GetAuctionItem();
                    if (pItem != NULL && m_DlgMain.m_DlgBidAuction.m_bShow)
                    {
                        ShowMessage(pItem);
                    }
                }
                break;
            case DLG_AUCPACKAGE:
                {
                    OBJID idItem = 0;
                    if (m_DlgMain.m_DlgAucPackage.m_ImgMoneyItem.m_bMouseMove)
                    {
                        idItem = m_DlgMain.m_DlgAucPackage.m_ImgMoneyItem.GetImageRealID();
                    }
                    else
                    {
                        DWORD dwIndex = m_DlgMain.m_DlgAucPackage.m_GridAucitem.GetMouseMoveIndex();
                        idItem = m_DlgMain.m_DlgAucPackage.m_GridAucitem.GetIconRealID(dwIndex);
                    }
                    if (idItem == 0)
                    {
                        return;
                    }
                    for(int i = 0; i < g_objHero.GetPackageItemAmount(); i++)
                    {
                        CItem* pItem = g_objHero.GetPackageItemByIndex(i);
                        if (pItem != NULL && pItem->GetID() == idItem && m_DlgMain.m_DlgAucPackage.m_bShow)
                        {
                            ShowMessage(pItem);
                            break;
                        }
                    }
                }
                break;
            case DLG_DEPOT:
                {
                    DWORD dwIndex = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetMouseMoveIndex () ;
                    //					dwIndex = m_DlgMain.m_DlgDepot.m_nCurPage * DEPOT_PAGE_AMOUNT + dwIndex ;
                    CItem* pItem = g_objHero.GetPackageItemByIndex ( dwIndex ) ;
                    if ( pItem != NULL && m_DlgMain.m_DlgDepot.m_bShow)
                    {
                        ShowMessage ( pItem,  0) ;
                    }
                }
                break ;
            case DLG_MAIN:
                {
                    DWORD dwTypeID = m_DlgMain.m_DlgTalk.m_GoodBox.GetMouseMoveTypeID () ;
                    int nItemNum = 0 ;
                    int nItemIndex = 0 ;
                    int nType = (CItem::ItemGetSubType(dwTypeID) / 10000) * 10000;
                    if (nType != ITEMTYPE_GHOSTGEM)
                    {
                        for (int i = 0; i < g_objHero.GetItemAmount (); i++ )
                        {
                            if ( g_objHero.GetItemByIndex ( i )->GetTypeID () == dwTypeID )
                            {
                                if ( nItemNum  == 0 )
                                {
                                    nItemNum++ ;
                                    nItemIndex = i ;
                                }
                                else
                                {
                                    nItemNum++ ;
                                }
                            }
                        }
                        if (nItemNum > 0)
                        {
                            CItem* pItem = g_objHero.GetItemByIndex ( nItemIndex ) ;
                            if ( pItem != NULL )
                            {
                                ShowMessage ( pItem, nItemNum ) ;
                            }
                        }
                    }
                    else
                    {
                        for(int i = 0; i < g_objHero.GetItemAmount(PACKAGE_SOUL); i++)
                        {
                            if ( g_objHero.GetItemByIndex(i, PACKAGE_SOUL)->GetTypeID() == dwTypeID)
                            {
                                if (nItemNum == 0)
                                {
                                    nItemNum++;
                                    nItemIndex = i;
                                }
                                else
                                {
                                    nItemNum++;
                                }
                            }
                        }
                        if (nItemNum > 0)
                        {
                            CItem* pItem = g_objHero.GetItemByIndex(nItemIndex, PACKAGE_SOUL);
                            if (pItem != NULL)
                            {
                                ShowMessage(pItem, nItemNum);
                            }
                        }
                    }
                }
                break ;
            case DLG_NPCFACE:
                {
                    CMyBitmap::ShowString ( m_MsgDlgPnt.x,
                                            m_MsgDlgPnt.y + 20,
                                            0xffff00,
                                            g_objGameDataSet.GetStr ( 10068 ) ) ;//"更换头像" ) ;//
                    CMyBitmap::ShowString ( m_MsgDlgPnt.x,
                                            m_MsgDlgPnt.y + 38,
                                            0xffff00,
                                            g_objGameDataSet.GetStr ( 10069 ) ) ;//"需银子500两" ) ;//
                }
                break ;
            case DLG_EMBED:
                break ;
            case DLG_MPCBOOTH:
                {
                    if (!m_DlgMain.m_DlgMpcBooth.m_bShow)
                    {
                        break;
                    }
                    int nIndex = m_DlgMain.m_DlgMpcBooth.GetMouseMoveIndex () ;
                    if ( nIndex != -1 )
                    {
                        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( nIndex ) ;
                        if ( pItem )
                        {
                            ShowMessage ( pItem, -1 ) ;
                        }
                    }
                }
                break ;
            case DLG_NPCBOOTH:
                {
                    if (!m_DlgMain.m_DlgNpcBooth.m_bShow)
                    {
                        break;
                    }
                    int nIndex = m_DlgMain.m_DlgNpcBooth.GetMouseMoveIndex () ;
                    if ( nIndex != -1 )
                    {
                        CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( nIndex ) ;
                        if ( pItem )
                        {
                            if ( pItem != NULL )
                            {
                                ShowMessage ( pItem, -1 ) ;
                            }
                        }
                    }
                }
                break ;
            default:
                break ;
            }
        }
        break ;
    case ON_MSGDLG_STATIC:
        {
            int nMsgLength = 0;
            char message[256] = "";
            BOOL bUp = true ;
            switch ( m_btStaticID )
            {
            case 0: //LIFE STATIC
                sprintf( message, "HP:%d/%d", g_objHero.GetData(CPlayer::_PLAYER_LIFE), g_objHero.GetMaxLife() );
                break ;
            case 1:  //MANA STATIC
                if (g_objHero.GetMaxMana() <= 0)
                {
                    sprintf( message, "HP:%d/%d", g_objHero.GetData(CPlayer::_PLAYER_LIFE), g_objHero.GetMaxLife() );
                }
                else
                {
                    sprintf( message, "MP:%d/%d", g_objHero.GetData(CPlayer::_PLAYER_MANA), g_objHero.GetMaxMana() );
                }
                break ;
            case 2: //FORCE STATIC
                sprintf( message, "SP:%d/%d", g_objHero.GetPhysicalForce(), g_objHero.GetMaxPhysicalForce() );
                break ;
            case 3: //xp
                sprintf(message, "XP:%d/%d", g_objHero.GetXP(), g_objHero.GetMaxXP());
                break;
            case 4: //EXP STATIC
                if (!m_bExpShow)
                {
                    g_objGameMap.SetShowExpFlag(true);
                }
                return;
                break ;
            case 5: //pkmode
                sprintf(message, g_objGameDataSet.GetStr(10755 + g_objHero.GetPkMode()));
                break ;
            case 6: //mipmap
                sprintf(message, g_objGameDataSet.GetStr(10731));
                break ;
            case 7: //actionstr
                sprintf(message, g_objGameDataSet.GetStr(10732));
                break ;
            case 8: //help btn
                sprintf(message, g_objGameDataSet.GetStr(10733));
                break ;
            case 9: //equipview btn
                sprintf(message, g_objGameDataSet.GetStr(10734));
                break ;
            default:
                break ;
            }
            CMySize FontSize;
            CMyBitmap::GetFontSize(FontSize);
            nMsgLength = strlen ( message ) ;
            static BYTE bSID;
            // 判断两次 ID 是否一样
            // 否
            if (bSID != m_btStaticID)
            {
                m_bInJustNow = true;
                bSID = m_btStaticID;
            }
            if (nMsgLength <= 0)
            {
                return;
            }
            if ( bUp )
            {
                // 判断鼠标是否刚刚进入
                // 是
                if (m_bInJustNow)
                {
                    // 记录进入的时间
                    m_dwInTime = TimeGet();
                    m_bInJustNow = false;
                    return;
                }
                // 判断进入时间是否达到 0.5 秒
                // 否
                else if (TimeGet() - m_dwInTime < 500)
                {
                    return;
                }
                if (m_btStaticID == 0
                        || (m_btStaticID == 1 && g_objHero.GetMana() <= 0) )		//hp 特殊处理
                {
                    char szCurHp[16] = "";
                    char szMaxHp[16] = "";
                    char szRealMaxHp[16] = "";
                    sprintf( szCurHp, "HP:%d/", g_objHero.GetData(CPlayer::_PLAYER_LIFE));
                    sprintf( szMaxHp, "%d", g_objHero.GetMaxLife());
                    if (g_objHero.GetMaxLifePercent() > 0 && g_objHero.GetMaxLifePercent() < 1000 && !g_objHero.IsDead())
                    {
                        sprintf( szRealMaxHp, "/%d", g_objHero.GetMaxLife() * 1000 / g_objHero.GetMaxLifePercent());
                    }
                    int nMsgLength = strlen(szCurHp) + strlen(szMaxHp) + strlen(szRealMaxHp);
                    int nOffset = nMsgLength * CHAR_WIDTH / 2 - 10;
                    if ((m_StaticPoint[m_btStaticID].x - nOffset - 5) < 0) { nOffset = m_StaticPoint[m_btStaticID].x - 5; }
                    g_objGameDataSet.ShowInfoBack(m_StaticPoint[m_btStaticID].x - nOffset - 5,
                                                  m_StaticPoint[m_btStaticID].y - 1 - 7,
                                                  nMsgLength * CHAR_WIDTH + 10,
                                                  FontSize.iHeight + 1 + 10) ;
                    CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset,
                                            m_StaticPoint[m_btStaticID].y - 2,
                                            0xffffff,
                                            szCurHp) ;
                    if (g_objHero.GetMaxLifePercent() > 0 &&  g_objHero.GetMaxLifePercent() < 1000 && !g_objHero.IsDead())
                        CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset + strlen(szCurHp) * CHAR_WIDTH,
                                                m_StaticPoint[m_btStaticID].y - 2,
                                                0xff0000,
                                                szMaxHp) ;
                    else
                        CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset + strlen(szCurHp) * CHAR_WIDTH,
                                                m_StaticPoint[m_btStaticID].y - 2,
                                                0xffffff,
                                                szMaxHp) ;
                    // 判断是否计算坐标
                    if (0 == m_HoldX || 0 == m_HoldY)
                    {
                        m_HoldX = m_StaticPoint[m_btStaticID].x - nOffset;
                        m_HoldY = m_StaticPoint[m_btStaticID].y - 3;
                    }
                    return;
                }
                CString s(message);
                int nIndex;
                // 判断是否有特殊格式
                if (-1 != (nIndex = s.Find('#')))
                {
                    CString strLeft = s.Left(nIndex);
                    CString strRight = s.Right(s.GetLength() - nIndex - 1);
                    //				int nOffset = 0 ;
                    //					if ( m_StaticPoint[m_btStaticID].x + strLeft.GetLength() * CHAR_WIDTH > _SCR_WIDTH )
                    //					{
                    //						nOffset = m_StaticPoint[m_btStaticID].x + strLeft.GetLength() * CHAR_WIDTH - _SCR_WIDTH;
                    //					}
                    int nOffset = strLeft.GetLength() * CHAR_WIDTH / 2 - 10;
                    if ((m_StaticPoint[m_btStaticID].x - nOffset - 5) < 0) { nOffset = m_StaticPoint[m_btStaticID].x - 5; }
                    g_objGameDataSet.ShowInfoBack(m_StaticPoint[m_btStaticID].x - nOffset - 5,
                                                  m_StaticPoint[m_btStaticID].y - 1 - 17,
                                                  strLeft.GetLength()*CHAR_WIDTH + 10,
                                                  FontSize.iHeight * 2 + 1 + 10) ;
                    CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset,
                                            m_StaticPoint[m_btStaticID].y - 12,
                                            0xffffff,
                                            strLeft) ;
                    CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset,
                                            m_StaticPoint[m_btStaticID].y + FontSize.iHeight - 12,
                                            0xffffff,
                                            strRight) ;
                }
                else if (-1 != (nIndex = s.Find('$')))
                {
                    CString strLeft = s.Left(nIndex);
                    CString strRight = s.Right(s.GetLength() - nIndex - 1);
                    //					int nOffset = 0 ;
                    //					if ( m_StaticPoint[m_btStaticID].x + strLeft.GetLength() * CHAR_WIDTH > _SCR_WIDTH )
                    //					{
                    //						nOffset = m_StaticPoint[m_btStaticID].x + strLeft.GetLength() * CHAR_WIDTH - _SCR_WIDTH;
                    //					}
                    int nOffset = strLeft.GetLength() * CHAR_WIDTH / 2 - 10;
                    if ((m_StaticPoint[m_btStaticID].x - nOffset - 5) < 0) { nOffset = m_StaticPoint[m_btStaticID].x - 5; }
                    g_objGameDataSet.ShowInfoBack(m_StaticPoint[m_btStaticID].x - nOffset - 5,
                                                  m_StaticPoint[m_btStaticID].y - 1 - 17,
                                                  strLeft.GetLength()*CHAR_WIDTH + 10,
                                                  FontSize.iHeight * 2 + 1 + 10) ;
                    CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset,
                                            m_StaticPoint[m_btStaticID].y - 12,
                                            0xffffff,
                                            strLeft) ;
                    CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset + CHAR_WIDTH *(strLeft.GetLength() - strRight.GetLength()) / 2,
                                            m_StaticPoint[m_btStaticID].y + FontSize.iHeight - 12,
                                            0xffff00,
                                            strRight) ;
                }
                else
                {
                    //					int nOffset = 0 ;
                    //					if ( m_StaticPoint[m_btStaticID].x + nMsgLength * CHAR_WIDTH > _SCR_WIDTH )
                    //					{
                    //						nOffset = m_StaticPoint[m_btStaticID].x + nMsgLength * CHAR_WIDTH - _SCR_WIDTH;
                    //					}
                    int nOffset = nMsgLength * CHAR_WIDTH / 2 - 10;
                    if ((m_StaticPoint[m_btStaticID].x - nOffset - 5) < 0) { nOffset = m_StaticPoint[m_btStaticID].x - 5; }
                    g_objGameDataSet.ShowInfoBack(m_StaticPoint[m_btStaticID].x - nOffset - 5,
                                                  m_StaticPoint[m_btStaticID].y - 1 - 7,
                                                  nMsgLength * CHAR_WIDTH + 10,
                                                  FontSize.iHeight + 1 + 10) ;
                    CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x - nOffset,
                                            m_StaticPoint[m_btStaticID].y - 2,
                                            0xffffff,
                                            message) ;
                    // 判断是否计算坐标
                    if (0 == m_HoldX || 0 == m_HoldY)
                    {
                        m_HoldX = m_StaticPoint[m_btStaticID].x - nOffset;
                        m_HoldY = m_StaticPoint[m_btStaticID].y - 3;
                    }
                }
            }
            else
            {
                CMyBitmap::ShowString ( m_StaticPoint[m_btStaticID].x,
                                        m_StaticPoint[m_btStaticID].y + 40,
                                        0x00ffff,
                                        message) ;
            }
        }
        POINT point;
        GetCursorPos(&point);
        // 判断鼠标是否在面板内
        if (point.x > 760 || point.y < 668)
        {
            m_btStaticID = -1;
        }
        break ;
    case ON_MSGDLG_FACE:
        {
            int nIndex = m_dwMsgDlgOwner ;
            if ( m_DlgMain.m_DlgGroup.m_btOwnerPos < m_dwMsgDlgOwner )
            {
                nIndex++ ;
            }
            TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex - 1 ) ;
            if ( pInfo != NULL )
            {
                /*				CMyBitmap::ShowString( m_MsgDlgPnt.x - 34,
                									   m_MsgDlgPnt.y + 20,
                									   0xffffffff,
                									   pInfo->szName ) ;*/
                CPoint posPosition;
                posPosition.x = m_MsgDlgPnt.x - 28;
                posPosition.y = m_MsgDlgPnt.y + 13;
                m_DlgMain.m_DlgGroup.ShowTeamMemberPosition(nIndex - 1, posPosition);
            }
        }
        break ;
    case ON_MSGDLG_IMAGE:
        {
            CMagic* pMagic = g_objHero.GetMagic ( m_dwMsgDlgOwner ) ;
            if ( pMagic )
            {
                char strText[128] = "" ;
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                sprintf( strText, "%s", pMagic->GetName()) ;
                int nPosX = (36 - (int)strlen(strText) * CHAR_WIDTH - 10 ) / 2 ;
                g_objGameDataSet.ShowInfoBack(m_MsgDlgPnt.x + nPosX,
                                              m_MsgDlgPnt.y - FontSize.iHeight - 1 - 17,
                                              strlen(strText)*CHAR_WIDTH + 10,
                                              FontSize.iHeight + 1 + 10);
                CMyBitmap::ShowString ( m_MsgDlgPnt.x + nPosX + 5,
                                        m_MsgDlgPnt.y - FontSize.iHeight - 12,
                                        0xffffff,
                                        strText ) ;
            }
            break;
        }
    default:
        break;
    }
}

void CMyShellDlg::ShowMessage(CItem* pItem, int nItemNum, BOOL bShowDur)
{
    if (pItem == NULL)
    {
        return;
    }
    DWORD STRCOLOR_WHITE = 0xffffff;
    DWORD STRCOLOR_GREEN = 0x00ff00;
    DWORD STRCOLOR_YELLOW = 0xffff00;
    DWORD STRCOLOR_LYELLOW = 0xe8a900;
    DWORD STRCOLOR_GYELLOW = 0xebe57f;
    DWORD STRCOLOR_BLUE  = 0x0080ff;
    DWORD STRCOLOR_RED = 0xff0000;
    DWORD STRCOLOR_ORANGE = 0xff8040;
    DWORD STRCOLOR_BRED	  = 0xff00ff;
    CMySize FontSize;
    CMyBitmap::GetFontSize(FontSize);
    int nMsgNum = 0;
    char strMsg[32][128];
    char strAddtionLev[32] = "";
    CPoint pMsgPos;
    DWORD strMsgColor[32];
    for(int i = 0; i < 32; i++)
    {
        strMsgColor[i] = STRCOLOR_WHITE;
    }
    if (pItem->GetPackageType() == PACKAGE_EGG || pItem->GetPackageType() == PACKAGE_PET)
    {
        sprintf(strMsg[nMsgNum++], "%s", pItem->GetName());
        if (m_DlgMain.m_DlgMonsterHatch.m_bShow && pItem->GetPackageType() == PACKAGE_EGG)
        {
            if (pItem->GetAvailabeTime() > 0 )
            {
                DWORD dwTime = pItem->GetAvailabeTime();
                DWORD dwHour = dwTime / 3600;
                DWORD dwMinute = (dwTime % 3600) / 60;
                if (dwHour > 23)
                {
                    dwHour = 23;
                    dwMinute = 59;
                }
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10134));
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10135), dwHour, dwMinute);
            }
        }
        int nMaxStrLen = 0;
        for(int n = 0; n < nMsgNum; n++)
        {
            if ((int)strlen(strMsg[n]) > nMaxStrLen)
            {
                nMaxStrLen = strlen(strMsg[n]);
            }
        }
        int nInfoLine = ShowItemInfo(pItem, 0 , 0, nMaxStrLen, FALSE);
        int nX;
        int nY;
        //		int nX = m_MsgDlgPnt.x + 35 + nMaxStrLen*FontSize.iWidth+4;
        //		int nY = m_MsgDlgPnt.y + 35 + (nMsgNum+nInfoLine)*(FontSize.iHeight+2)+2;
        //		if (nY < _SCR_HEIGHT - 40 && nX < _SCR_WIDTH - 40)
        //		{
        //			pMsgPos.x = m_MsgDlgPnt.x + 35;
        //			pMsgPos.y = m_MsgDlgPnt.y + 35;
        //		}
        //		else if (nY < _SCR_HEIGHT - 40 && nX >= _SCR_WIDTH - 40)
        //		{
        //			pMsgPos.x = m_MsgDlgPnt.x - 35 - (nX - m_MsgDlgPnt.x - 70);
        //			pMsgPos.y = m_MsgDlgPnt.y + 35;
        //		}
        //		else if (nY >= _SCR_HEIGHT - 40 && nX >= _SCR_WIDTH - 40)
        //		{
        //			pMsgPos.x = m_MsgDlgPnt.x - 35 - (nX - m_MsgDlgPnt.x - 70);
        //			pMsgPos.y = m_MsgDlgPnt.y - 35 - (nY-m_MsgDlgPnt.y - 70);
        //		}
        //		else
        //		{
        //			pMsgPos.x = m_MsgDlgPnt.x + 35;
        //			pMsgPos.y = m_MsgDlgPnt.y - 35 - (nY-m_MsgDlgPnt.y - 70);
        //		}
        // 判断蛋的类型
        // 孵化
        if (pItem->GetAvailabeTime() > 0)
        {
            nX = m_MsgDlgPnt.x - (nMaxStrLen * FontSize.iWidth + 4) - 10;
            nY = m_MsgDlgPnt.y + (nMsgNum + nInfoLine) * (FontSize.iHeight + 2) + 2 + 10;
            pMsgPos.x = nX + 5 - 30;
            pMsgPos.y = m_MsgDlgPnt.y + 5 - 32;
        }
        else if (DLG_MPCBOOTH == m_dwMsgDlgOwner)
        {
            nX = m_MsgDlgPnt.x - (nMaxStrLen * FontSize.iWidth + 4) - 10;
            nY = m_MsgDlgPnt.y + (nMsgNum + nInfoLine) * (FontSize.iHeight + 2) + 2 + 10;
            pMsgPos.x = nX + 5 - 3;
            pMsgPos.y = m_MsgDlgPnt.y + 5 - 6;
            if (pMsgPos.x - 5 < 0) { pMsgPos.x += nMaxStrLen * FontSize.iWidth + 52; }
        }
        else
        {
            nX = m_MsgDlgPnt.x - (nMaxStrLen * FontSize.iWidth + 4) - 10;
            nY = m_MsgDlgPnt.y + (nMsgNum + nInfoLine) * (FontSize.iHeight + 2) + 2 + 10;
            pMsgPos.x = nX + 6;
            pMsgPos.y = m_MsgDlgPnt.y + 5 - 1;
        }
        if (nInfoLine > 0)
            g_objGameDataSet.ShowInfoBack(pMsgPos.x - 5,	pMsgPos.y - 5,
                                          nMaxStrLen * FontSize.iWidth + 4 + 5, (nMsgNum + nInfoLine)*(FontSize.iHeight + 2) + 2 + 10);
        else
            g_objGameDataSet.ShowInfoBack(pMsgPos.x - 5,	pMsgPos.y - 5,
                                          nMaxStrLen * FontSize.iWidth + 4 + 5, (nMsgNum + nInfoLine)*(FontSize.iHeight + 2) + 10);
        for(int j = 0; j < nMsgNum; j++)
        {
            CMyBitmap::ShowString(pMsgPos.x, pMsgPos.y + (FontSize.iHeight + 2)*j, strMsgColor[j], strMsg[j]);
        }
        ShowItemInfo(pItem, pMsgPos.x,
                     pMsgPos.y + nMsgNum*(FontSize.iHeight + 2) + 2, nMaxStrLen);
        return;
    }
    //物品名称
    if (pItem->GetSort() == ITEMSORT_HELMET ||
            pItem->GetSort() == ITEMSORT_NECKLACE ||
            pItem->GetSort() == ITEMSORT_ARMOR ||
            pItem->GetSort() == ITEMSORT_TREASURE ||
            pItem->GetSort() == ITEMSORT_WEAPON_SINGLE_HAND ||
            pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND ||
            pItem->GetSort() == ITEMSORT_SHIELD ||
            pItem->GetSort() == ITEMSORT_RING ||
            pItem->GetSort() == ITEMSORT_SHOES )
    {
        if (!pItem->TestStatus( _ITEM_STATUS_NOT_IDENT ))
        {
            switch(pItem->GetTypeID() % 10)
            {
            case 1:
                strMsgColor[nMsgNum] = STRCOLOR_GREEN;	//良品
                sprintf(strMsg[nMsgNum++], "%s%s", g_objGameDataSet.GetStr(10130), pItem->GetName());
                break;
            case 2:
                strMsgColor[nMsgNum] = STRCOLOR_BLUE;  //上品
                sprintf(strMsg[nMsgNum++], "%s%s", g_objGameDataSet.GetStr(10131), pItem->GetName());
                break;
            case 3:
                strMsgColor[nMsgNum] = STRCOLOR_RED;	//精品
                sprintf(strMsg[nMsgNum++], "%s%s", g_objGameDataSet.GetStr(10132), pItem->GetName());
                break;
            case 4:
                strMsgColor[nMsgNum] = STRCOLOR_ORANGE;	//极品
                sprintf(strMsg[nMsgNum++], "%s%s", g_objGameDataSet.GetStr(10133), pItem->GetName());
                break;
            default:
                sprintf(strMsg[nMsgNum++], "%s", pItem->GetName());
                break;
            }
        }
        else
        {
            sprintf(strMsg[nMsgNum++], "%s", pItem->GetName());
        }
    }
    else
    {
        if (pItem->GetPackageType() == PACKAGE_PET)
        {
            sprintf(strMsg[nMsgNum++], "%s", pItem->GetMonsterName());
        }
        else
        {
            sprintf(strMsg[nMsgNum++], "%s", pItem->GetName());
        }
    }
    // 战魂等级
    if (pItem->GetWarGhostLevel() > 0 && pItem->GetWarGhostLevel() <= 9 && pItem->GetWarGhostExp() > 0)
    {
        strMsgColor[nMsgNum] = STRCOLOR_BRED;
        sprintf(strMsg[nMsgNum++], g_objGameDataSet.GetStr(10120), pItem->GetWarGhostLevel());
    }
    // 镶嵌宝石
    if (pItem->GetGemAtkType())
    {
        ItemTypeInfo pItemInfo;
        if (CItem::s_ItemData.GetItemTypeInfo(pItem->GetGemAtkType(), pItemInfo))
        {
            strMsgColor[nMsgNum] = STRCOLOR_GREEN;
            sprintf(strMsg[nMsgNum++], g_objGameDataSet.GetStr(10118), pItemInfo.szName);
        }
    }
    //职业要求
    if (pItem->GetProfessionRequired())
    {
        if (pItem->GetSexRequried())
        {
            if (g_objHero.GetSex() != pItem->GetSexRequried()
                    || g_objHero.GetProfession() != pItem->GetProfessionRequired())
            {
                strMsgColor[nMsgNum] = STRCOLOR_RED;
            }
            if (pItem->GetSexRequried() == 1)
                sprintf(strMsg[nMsgNum++], "%s%s%s", g_objGameDataSet.GetStr ( 10081 ),
                        g_objGameDataSet.GetStr(10031),
                        g_objHero.GetProfessionalName(pItem->GetProfessionRequired()));
            else
                sprintf(strMsg[nMsgNum++], "%s%s%s", g_objGameDataSet.GetStr ( 10081 ),
                        g_objGameDataSet.GetStr(10030),
                        g_objHero.GetProfessionalName(pItem->GetProfessionRequired()));
        }
        else
        {
            if (g_objHero.GetProfession() != pItem->GetProfessionRequired())
            {
                strMsgColor[nMsgNum] = STRCOLOR_RED;
            }
            sprintf(strMsg[nMsgNum++], "%s%s", g_objGameDataSet.GetStr ( 10081 ),
                    g_objHero.GetProfessionalName(pItem->GetProfessionRequired()));
        }
    }
    else
    {
        if (pItem->GetSexRequried())	//性别需求:
        {
            if (g_objHero.GetSex() != pItem->GetSexRequried())
            {
                strMsgColor[nMsgNum] = STRCOLOR_RED;
            }
            sprintf ( strMsg[nMsgNum++], g_objGameDataSet.GetStr ( 10085 ), pItem->GetSexRequried()) ;
        }
    }
    if ( !pItem->TestStatus( _ITEM_STATUS_NONE ) )
    {
        if ( pItem->TestStatus( _ITEM_STATUS_NOT_IDENT ) )	//未鉴定
        {
            strMsgColor[nMsgNum] = STRCOLOR_RED;
            sprintf(strMsg[nMsgNum++], g_objGameDataSet.GetStr ( 10412 )  ) ;
            int nMaxStrLen = 0;
            for(int n = 0; n < nMsgNum; n++)
            {
                if ((int)strlen(strMsg[n]) > nMaxStrLen)
                {
                    nMaxStrLen = strlen(strMsg[n]);
                }
            }
            int nInfoLine = ShowItemInfo(pItem, 0 , 0, nMaxStrLen, FALSE);
            int nX = m_MsgDlgPnt.x + 35 + nMaxStrLen * FontSize.iWidth + 4;
            int nY = m_MsgDlgPnt.y + 35 + (nMsgNum + nInfoLine) * (FontSize.iHeight + 2) + 2;
            if (nY < _SCR_HEIGHT - 40 && nX < _SCR_WIDTH - 40)
            {
                pMsgPos.x = m_MsgDlgPnt.x + 35;
                pMsgPos.y = m_MsgDlgPnt.y + 35;
            }
            else if (nY < _SCR_HEIGHT - 40 && nX >= _SCR_WIDTH - 40)
            {
                pMsgPos.x = m_MsgDlgPnt.x - 35 - (nX - m_MsgDlgPnt.x - 70);
                pMsgPos.y = m_MsgDlgPnt.y + 35;
            }
            else if (nY >= _SCR_HEIGHT - 40 && nX >= _SCR_WIDTH - 40)
            {
                pMsgPos.x = m_MsgDlgPnt.x - 35 - (nX - m_MsgDlgPnt.x - 70);
                pMsgPos.y = m_MsgDlgPnt.y - 35 - (nY - m_MsgDlgPnt.y - 70);
            }
            else
            {
                pMsgPos.x = m_MsgDlgPnt.x + 35;
                pMsgPos.y = m_MsgDlgPnt.y - 35 - (nY - m_MsgDlgPnt.y - 70);
            }
            if (nInfoLine > 0)
                g_objGameDataSet.ShowInfoBack(pMsgPos.x - 5,	pMsgPos.y - 5,
                                              nMaxStrLen * FontSize.iWidth + 4 + 10,
                                              (nMsgNum + nInfoLine)*(FontSize.iHeight + 2) + 2 + 10);
            else
                g_objGameDataSet.ShowInfoBack(pMsgPos.x - 5,	pMsgPos.y - 5,
                                              nMaxStrLen * FontSize.iWidth + 4 + 10,
                                              (nMsgNum + nInfoLine)*(FontSize.iHeight + 2) + 10) ;
            /*
            CMyBitmap::ShowBlock(pMsgPos.x,pMsgPos.y,pMsgPos.x + nMaxStrLen*FontSize.iWidth+4,
            				pMsgPos.y+nMsgNum*(FontSize.iHeight+2)+2,
            				0xff000000);*/
            for(int j = 0; j < nMsgNum; j++)
            {
                CMyBitmap::ShowString(pMsgPos.x + 2, pMsgPos.y + 2 + (FontSize.iHeight + 2)*j, STRCOLOR_RED, strMsg[j]);
            }
            ShowItemInfo(pItem, pMsgPos.x,
                         pMsgPos.y + nMsgNum*(FontSize.iHeight + 2) + 2, nMaxStrLen);
            return;
        }
    }
    //筹码兑换价格
    if (pItem->IsMoneyItem ())
    {
        strMsgColor[nMsgNum] = STRCOLOR_YELLOW;
        sprintf( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr( 10519 ), pItem->GetMoneyItemPrice () ) ;
    }
    //等级需求
    if ( pItem->GetPackageType() != PACKAGE_EGG && pItem->GetLevelRequired())
    {
        if (g_objHero.GetLev() < pItem->GetLevelRequired())
        {
            strMsgColor[nMsgNum] = STRCOLOR_RED;
        }
        sprintf( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr( 10084 ), pItem->GetLevelRequired () ) ;
    }
    //力量需求
    if (pItem->GetForceRequired())
    {
        if (g_objHero.GetForce() < pItem->GetForceRequired())
        {
            strMsgColor[nMsgNum] = STRCOLOR_RED;
        }
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10086 ), pItem->GetForceRequired ()) ;
    }
    //灵巧需求
    if (pItem->GetSpeedRequired())
    {
        if (g_objHero.GetDex() < pItem->GetForceRequired())
        {
            strMsgColor[nMsgNum] = STRCOLOR_RED;
        }
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10087 ), pItem->GetSpeedRequired () ) ;
    }
    //体质需求
    if (pItem->GetHealthRequired())
    {
        if (g_objHero.GetHealth() < pItem->GetHealthRequired())
        {
            strMsgColor[nMsgNum] = STRCOLOR_RED;
        }
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10088 ), pItem->GetHealthRequired () ) ;
    }
    //精神需求
    if (pItem->GetSoulRequired())
    {
        if (g_objHero.GetSoul() < pItem->GetSoulRequired())
        {
            strMsgColor[nMsgNum] = STRCOLOR_RED;
        }
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10089 ), pItem->GetSoulRequired () ) ;
    }
    //最大--最小攻击
    if (pItem->GetMaxAttack())
    {
        sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10090), pItem->GetMinAttack(), pItem->GetMaxAttack());
    }
    //防御
    if (pItem->GetDefense())
    {
        sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10091), pItem->GetDefense());
    }
    //+魔攻
    if (pItem->GetMaxMagicAttack())
    {
        strMsgColor[nMsgNum] = STRCOLOR_BLUE;
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10094 ), pItem->GetMinMagicAttack(), pItem->GetMaxMagicAttack() );
    }
    //+魔防
    if ( pItem->GetMagicDefence () )
    {
        strMsgColor[nMsgNum] = STRCOLOR_BLUE;
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10093 ), pItem->GetMagicDefence () );
    }
    //+生命
    if (pItem->GetLife())
    {
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10097 ), pItem->GetLife () );
    }
    //+魔法
    if (pItem->GetMana())
    {
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10098 ), pItem->GetMana () );
    }
    //+躲避
    if (pItem->GetDodge())
    {
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10096 ), pItem->GetDodge () );
    }
    //+命中
    if (pItem->GetHitarte())
    {
        sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10119), pItem->GetHitarte());
    }
    //+攻击速度
    if (pItem->GetAttackSpeed()
            && (pItem->GetSort() == ITEMSORT_WEAPON_SINGLE_HAND || pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND)
            && pItem->GetAttackSpeed() != 800)
    {
        int nAtkSpeed = (800 - pItem->GetAttackSpeed()) / 10 ;
        if (nAtkSpeed > 0)
        {
            sprintf(strMsg[nMsgNum++], "%s+%d", (char*)g_objGameDataSet.GetStr(10100), nAtkSpeed);
        }
        else
        {
            sprintf(strMsg[nMsgNum++], "%s-%d", (char*)g_objGameDataSet.GetStr(10100), abs(nAtkSpeed));
        }
    }
    //+攻击范围
    if (pItem->GetAtkRange())
    {
        sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10099), pItem->GetAtkRange());
    }
    if (pItem->GetSort() == ITEMSORT_EXPEND)//数量
    {
        if (pItem->GetAmount() > 1 && pItem->GetAmount() != pItem->GetAmountLimit())
        {
            sprintf( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr( 10544 ), pItem->GetAmount() ) ;
        }
    }
    else	//损耗度
    {
        if (pItem->GetAmountLimit() / 100 > 0 )
        {
            sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10101 ), pItem->GetAmount() / 100, pItem->GetAmountLimit () / 100 );
        }
    }
    //物品价格
    if ( m_DlgMain.m_DlgNpcBuy.m_bShow || m_DlgMain.m_DlgTrade.m_bShow && !pItem->TestMonopolyItem (_MONOPOLY_NOT_SELL))
    {
        strMsgColor[nMsgNum] = STRCOLOR_YELLOW;
        if ( nItemNum == 0 )
        {
            sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10102 ), pItem->GetPrice () );
        }
        else if (nItemNum == -1 || nItemNum >= 1)
        {
            sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10103 ), pItem->GetSellPrice () );
        }
    }
    //修理价格
    if (nItemNum == -1 && m_bIconFix && pItem->GetSort() != ITEMSORT_EXPEND)
    {
        strMsgColor[nMsgNum] = STRCOLOR_LYELLOW;
        sprintf ( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10104 ), pItem->GetRepairCost () );
    }
    //特殊物品
    if (pItem->IsMonopolyItem() || pItem->TestMonopolyItem(_MONOPOLY_NOTDROP))
    {
        strMsgColor[nMsgNum] = STRCOLOR_YELLOW;
        sprintf( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10095));
    }
    if (pItem->TestMonopolyItem (_MONOPOLY_NOT_SELL))
    {
        strMsgColor[nMsgNum] = STRCOLOR_YELLOW;
        sprintf( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10675));
    }
    //鉴定
    // 物品鉴定
    if ( !pItem->TestStatus( _ITEM_STATUS_NONE ) )
    {
        if ( pItem->TestStatus( _ITEM_STATUS_CANNOT_REPAIR ) ) //不可修复
        {
            strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
            sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10413 )  ) ;
        }
        else if ( pItem->TestStatus( _ITEM_STATUS_NEVER_DAMAGE ) ) //永不磨损
        {
            strMsgColor[nMsgNum] = STRCOLOR_GREEN;
            sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr ( 10414 )  ) ;
        }
    }
    //追加属性addition
    if (pItem->GetAddition())
    {
        ItemAdditionInfo pAddtionInfo;
        sprintf(strAddtionLev, (char*)g_objGameDataSet.GetStr(10122), pItem->GetAddition());
        if (CItem::s_ItemData.GetItemAdditionInfo(pItem->GetTypeID(), pItem->GetAddition(), pAddtionInfo))
        {
            if (pAddtionInfo.nLife)//追加生命:%d
            {
                strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
                sprintf( strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10123), pAddtionInfo.nLife);
            }
            if (pAddtionInfo.nAttackMax) //"追加物攻:%d-%d"
            {
                strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10124), pAddtionInfo.nAttackMin, pAddtionInfo.nAttackMax);
            }
            if (pAddtionInfo.nDefense)//"追加防御:%d"
            {
                strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10125), pAddtionInfo.nDefense);
            }
            if (pAddtionInfo.nMAttackMax)//"追加魔攻:%d-%d"
            {
                strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10126), pAddtionInfo.nMAttackMin, pAddtionInfo.nMAttackMax);
            }
            if (pAddtionInfo.nMDef)//"追加魔防:%d"
            {
                strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10127), pAddtionInfo.nMDef);
            }
            if (pAddtionInfo.nDexterity)//"追加灵巧:%d"
            {
                strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10128), pAddtionInfo.nDexterity);
            }
            if (pAddtionInfo.nDodge)//"追加躲避:%d"
            {
                strMsgColor[nMsgNum] = STRCOLOR_GYELLOW;
                sprintf(strMsg[nMsgNum++], (char*)g_objGameDataSet.GetStr(10129), pAddtionInfo.nDodge);
            }
        }
    }
    int nMaxStrLen = 0;
    for(int n = 0; n < nMsgNum; n++)
    {
        if ((int)strlen(strMsg[n]) > nMaxStrLen)
        {
            nMaxStrLen = strlen(strMsg[n]);
        }
    }
    int nInfoLine = ShowItemInfo(pItem, 0 , 0, nMaxStrLen, FALSE);
    int nX = m_MsgDlgPnt.x - (nMaxStrLen * FontSize.iWidth + 4) - 10;
    int nY = m_MsgDlgPnt.y + (nMsgNum + nInfoLine) * (FontSize.iHeight + 2) + 2 + 10;
    if (nY < _SCR_HEIGHT - 40 && nX >= 0)
    {
        //		pMsgPos.x = m_MsgDlgPnt.x + 35;
        //		pMsgPos.y = m_MsgDlgPnt.y + 35;
        switch(m_dwMsgDlgOwner)
        {
        case DLG_AUCTION:
        case 29:
            pMsgPos.x = nX - 30;
            pMsgPos.y = m_MsgDlgPnt.y - 30;
            break;
        case DLG_MPCBOOTH:
            pMsgPos.x = nX - 3;
            pMsgPos.y = m_MsgDlgPnt.y - 2;
            break;
        default:
            pMsgPos.x = nX + 5;
            pMsgPos.y = m_MsgDlgPnt.y + 5;
            break;
        }
    }
    else if (nY < _SCR_HEIGHT - 40 && nX < 0)
    {
        //		pMsgPos.x = m_MsgDlgPnt.x - 35 - (nX - m_MsgDlgPnt.x - 70);
        //		pMsgPos.y = m_MsgDlgPnt.y + 35;
        //		pMsgPos.x = m_MsgDlgPnt.x + 35;
        //		pMsgPos.y = m_MsgDlgPnt.y + 35;
        switch(m_dwMsgDlgOwner)
        {
        case DLG_MPCBOOTH:
            pMsgPos.x = m_MsgDlgPnt.x + 35 + 5;
            pMsgPos.y = m_MsgDlgPnt.y - 2;
            break;
        default:
            pMsgPos.x = m_MsgDlgPnt.x + 35 + 5;
            pMsgPos.y = m_MsgDlgPnt.y + 5;
            break;
        }
    }
    else if (nY >= _SCR_HEIGHT - 40 && nX >= 0)
    {
        //		pMsgPos.x = m_MsgDlgPnt.x - 35 - (nX - m_MsgDlgPnt.x - 70);
        //		pMsgPos.y = m_MsgDlgPnt.y - 35 - (nY-m_MsgDlgPnt.y - 70);
        pMsgPos.x = nX + 5;
        pMsgPos.y = m_MsgDlgPnt.y + 35 - ((nMsgNum + nInfoLine) * (FontSize.iHeight + 2) + 2) - 5;
    }
    else
    {
        //		pMsgPos.x = m_MsgDlgPnt.x + 35;
        //		pMsgPos.y = m_MsgDlgPnt.y - 35 - (nY-m_MsgDlgPnt.y - 70);
        pMsgPos.x = m_MsgDlgPnt.x + 35 + 5;
        pMsgPos.y = m_MsgDlgPnt.y + 35 - ((nMsgNum + nInfoLine) * (FontSize.iHeight + 2) + 2) - 5;
    }
    if (nInfoLine > 0)
        g_objGameDataSet.ShowInfoBack(pMsgPos.x - 5,	pMsgPos.y - 5,
                                      nMaxStrLen * FontSize.iWidth + 4 + 10,
                                      (nMsgNum + nInfoLine)*(FontSize.iHeight + 2) + 2 + 10);
    else
        g_objGameDataSet.ShowInfoBack(pMsgPos.x - 5,	pMsgPos.y - 5,
                                      nMaxStrLen * FontSize.iWidth + 4 + 10,
                                      (nMsgNum + nInfoLine)*(FontSize.iHeight + 2) + 10)  ;
    for(int j = 0; j < nMsgNum; j++)
    {
        CMyBitmap::ShowString(pMsgPos.x + 2, pMsgPos.y + 2 + (FontSize.iHeight + 2)*j, strMsgColor[j], strMsg[j]);
        if (j == 0)
        {
            CMyBitmap::ShowString(pMsgPos.x + 2 + strlen(strMsg[0])*FontSize.iWidth,
                                  pMsgPos.y + 2 + (FontSize.iHeight + 2)*j, STRCOLOR_GYELLOW, strAddtionLev);
        }
    }
    ShowItemInfo(pItem, pMsgPos.x,
                 pMsgPos.y + nMsgNum*(FontSize.iHeight + 2) + 2, nMaxStrLen);
}

void CMyShellDlg::OnIconPickUp(LPARAM lParam)
{
    m_PickUpDlgID = lParam ;
    switch ( lParam )
    {
    case DLG_MAIN:
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgTalk.m_GoodBox.GetPickUpMousePnt ( ) ;
        m_IconTypeID = m_DlgMain.m_DlgTalk.m_GoodBox.GetPickUpTypeID () ;
        m_IconIndex = m_DlgMain.m_DlgTalk.m_GoodBox.GetPickUpIndex () ;
        m_btIconType = ICON_TYPE_ITEM ;
        m_IconRealID = m_DlgMain.m_DlgTalk.m_GoodBox.GetPickUpRealID() ;
        break ;
    case DLG_MONSTER:
        g_bPickUp = true;
        m_btIconType = ICON_TYPE_ITEM ;
        m_IconRealID = m_DlgMain.m_DlgMonster.GetCurMonsterID() ;
        m_DlgMain.m_DlgMonsterImprove.SetPickUpMonsterId(m_IconRealID);
        m_DlgMain.m_DlgMonsterEvolve.InsertMonster(m_IconRealID);
        m_DlgMain.m_DlgMpcBooth.SetPickUpState(g_bPickUp, m_IconRealID);
        break;
    case DLG_MONSTEREGG:
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgMonster.m_EggsGri.GetPickUpMousePnt () ;
        m_IconTypeID = m_DlgMain.m_DlgMonster.m_EggsGri.GetPickUpTypeID () ;
        m_IconIndex = m_DlgMain.m_DlgMonster.m_EggsGri.GetPickUpIndex () ;
        m_IconRealID = m_DlgMain.m_DlgMonster.m_EggsGri.GetPickUpRealID() ;
        m_btIconType = m_DlgMain.m_DlgMonster.m_EggsGri.GetPickUpType () ;
        m_DlgMain.m_DlgMpcBooth.SetPickUpState(g_bPickUp, m_IconRealID);
        break;
    case DLG_STONEGOODS0:
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.GetPickUpMousePnt () ;
        m_IconTypeID = m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.GetPickUpTypeID () ;
        m_IconIndex = m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.GetPickUpIndex () ;
        m_IconRealID = m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.GetPickUpRealID() ;
        m_btIconType = m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.GetPickUpType () ;
        m_DlgMain.m_DlgEmbed.SetPickUpState(m_IconRealID) ;
        m_DlgMain.m_DlgImprove.SetPickUpState(m_IconRealID);
        m_DlgMain.m_DlgTaskUpItem.SetTaskUpItem(m_IconRealID);
        m_DlgMain.m_DlgMonsterImprove.SetPickUpSoulStone(m_IconRealID);
        m_DlgMain.m_DlgMpcBooth.SetPickUpState(g_bPickUp, m_IconRealID);
        m_DlgMain.m_DlgAddAucItem.SetPickUpIcon(m_IconRealID);
        m_DlgMain.m_DlgIdentify.SetPickUpItemID(m_IconRealID);
        break;
    case DLG_MPCGOODS:
        if ( m_bIconFix )
        {
            if (g_objPlayerSet.GetPlayer(m_DlgMain.m_DlgNpcBuy.GetShopID()))
            {
                int nRealID = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpRealID();
                CItem* RepairItem = g_objHero.GetItem(nRealID) ;
                if ( RepairItem != NULL )
                {
                    DXPlaySound ( "Sound/Dlg_Repair.wav" ) ;
                    g_objHero.RepairItem ( RepairItem->GetID () ) ;
                }
            }
            m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.ReSumePickUp();
            //			m_bIconFix = false ;
            break ;
        }
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpMousePnt () ;
        m_IconTypeID = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpTypeID () ;
        m_IconIndex = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpIndex () ;
        m_IconRealID = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpRealID() ;
        m_btIconType = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpType () ;
        m_DlgMain.m_DlgEmbed.SetPickUpState(m_IconRealID) ;
        m_DlgMain.m_DlgImprove.SetPickUpState(m_IconRealID);
        m_DlgMain.m_DlgMpcGoods.SetPickUpItemID( m_IconRealID ) ;
        m_DlgMain.m_DlgTaskUpItem.SetTaskUpItem( m_IconRealID ) ;
        m_DlgMain.m_DlgQuery.m_DlgEquip.SetPickUpItemID(m_IconRealID);
        m_DlgMain.m_DlgMpcBooth.SetPickUpState(g_bPickUp, m_IconRealID);
        m_DlgMain.m_DlgAddAucItem.SetPickUpIcon(m_IconRealID);
        m_DlgMain.m_DlgIdentify.SetPickUpItemID(m_IconRealID);
        break ;
    case DLG_NPCBUY:
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.GetPickUpMousePnt () ;
        m_IconTypeID = m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.GetPickUpTypeID () ;
        m_IconIndex =  m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.GetPickUpIndex () ;
        m_IconRealID = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpRealID() ;
        m_btIconType = m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.GetPickUpType () ;
        break ;
    case DLG_EQUIP:
        if ( m_bIconFix )
        {
            if (g_objPlayerSet.GetPlayer(m_DlgMain.m_DlgNpcBuy.GetShopID()))
            {
                int nPos = m_DlgMain.m_DlgQuery.m_DlgEquip.GetPickUpPosition() ;
                CItem* RepairItem = g_objHero.GetEquipment(nPos) ;
                if ( RepairItem != NULL )
                {
                    DXPlaySound ( "Sound/Dlg_Repair.wav" ) ;
                    g_objHero.RepairItem ( RepairItem->GetID () ) ;
                }
            }
            //			m_bIconFix = false ;
            break ;
        }
        m_IconMousePnt = m_DlgMain.m_DlgQuery.m_DlgEquip.GetPickUpMousePnt() ;
        m_IconIndex = m_DlgMain.m_DlgQuery.m_DlgEquip.GetPickUpPosition() ;
        m_btIconType = ICON_TYPE_EQUIP ;
        g_bPickUp = true ;
        break ;
    case DLG_MONSTERHATCH:
        {
            int nIndex = m_DlgMain.m_DlgMonsterHatch.m_GridEggs.GetPickUpIndex ();
            CItem* pOutItem = g_objHero.GetPackageItemByIndex ( nIndex ) ;
            if ( pOutItem != NULL )
            {
                if (pOutItem->GetPackageType() == PACKAGE_PET && g_objHero.GetItemAmount (PACKAGE_PET) >= 3 )
                {
                    g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr(10698) ) ;
                    m_DlgMain.m_DlgMonsterHatch.m_GridEggs.ReSumePickUp();
                    return ;
                }
                else if (pOutItem->GetPackageType() == PACKAGE_EGG)
                {
                    m_DlgMain.m_DlgMonsterHatch.m_GridEggs.ReSumePickUp();
                    m_DlgConfirm.SetConfirmDlg(34, NULL, nIndex);
                    return;
                }
                OBJID OutItemID = pOutItem->GetID () ;
                g_objHero.CheckOutItem ( OutItemID ) ;
            }
        }
        break;
    case DLG_MONSTERDEPOT:
        if ( m_bIconFix )
        {
            m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp();
            m_bIconFix = false ;
            break ;
        }
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgMonsterDepot.m_GridMonster.GetPickUpMousePnt () ;
        m_IconTypeID = m_DlgMain.m_DlgMonsterDepot.m_GridMonster.GetPickUpTypeID () ;
        m_IconIndex = m_DlgMain.m_DlgMonsterDepot.m_GridMonster.GetPickUpIndex () ;
        m_IconRealID = m_DlgMain.m_DlgMonsterDepot.m_GridMonster.GetPickUpRealID() ;
        m_btIconType = m_DlgMain.m_DlgMonsterDepot.m_GridMonster.GetPickUpType () ;
        break;
    case DLG_DEPOT:
        if ( m_bIconFix )
        {
            m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp();
            m_bIconFix = false ;
            break ;
        }
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpMousePnt () ;
        m_IconTypeID = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpTypeID () ;
        m_IconIndex = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpIndex () ;
        m_IconRealID = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpRealID() ;
        m_btIconType = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpType () ;
        break ;
    case DLG_AUCPACKAGE:
        if ( m_bIconFix )
        {
            m_DlgMain.m_DlgAucPackage.m_GridAucitem.ReSumePickUp();
            m_bIconFix = false ;
            break ;
        }
        g_bPickUp = true ;
        m_IconMousePnt = m_DlgMain.m_DlgAucPackage.m_GridAucitem.GetPickUpMousePnt () ;
        m_IconTypeID = m_DlgMain.m_DlgAucPackage.m_GridAucitem.GetPickUpTypeID () ;
        m_IconIndex = m_DlgMain.m_DlgAucPackage.m_GridAucitem.GetPickUpIndex () ;
        m_IconRealID = m_DlgMain.m_DlgAucPackage.m_GridAucitem.GetPickUpRealID() ;
        m_btIconType = m_DlgMain.m_DlgAucPackage.m_GridAucitem.GetPickUpType () ;
        break;
    case DLG_ACT:
        m_btIconType = ICON_TYPE_ACTION ;
        g_bPickUp = true ;
        break ;
    case DLG_MAGIC:
        break ;
    }
    //	m_bIconFix = false ;
    CRect rect ;
    CMyShellDlg::GetWindowRect ( rect ) ;
    switch ( lParam )
    {
    case DLG_ACT:
        m_IconShowPnt.x = m_IconMousePnt.x - rect.left - 16;
        m_IconShowPnt.y = m_IconMousePnt.y - rect.top - 16;
        break ;
    case DLG_MAGIC:
        break ;
    default:
        m_IconShowPnt.x = m_IconMousePnt.x - rect.left ;
        m_IconShowPnt.y = m_IconMousePnt.y - rect.top ;
        break ;
    }
}

void CMyShellDlg::ShowPickUpIcon()
{
    if (!g_bPickUp)
    {
        return;
    }
    POINT mousePnt ;
    GetCursorPos ( &mousePnt ) ;
    mousePnt.x -= 16;
    mousePnt.y -= 16;
    switch ( m_btIconType )
    {
    case ICON_TYPE_DEPOT:
        {
            CItem* pItem = g_objHero.GetPackageItemByIndex( m_IconIndex ) ;
            if ( pItem != NULL )
            {
                CMyPos posMouse;
                ::MouseCheck(posMouse.x, posMouse.y);
                pItem->ShowMiniItem( m_IconShowPnt.x + ( mousePnt.x - m_IconMousePnt.x ),
                                     m_IconShowPnt.y + ( mousePnt.y - m_IconMousePnt.y ), false);
            }
        }
        break;
    case ICON_TYPE_EQUIP:
        {
            CItem* pItem = g_objHero.GetEquipment( m_IconIndex ) ;
            if ( pItem != NULL )
            {
                CMyPos posMouse;
                ::MouseCheck(posMouse.x, posMouse.y);
                pItem->ShowMiniItem( m_IconShowPnt.x + ( mousePnt.x - m_IconMousePnt.x ),
                                     m_IconShowPnt.y + ( mousePnt.y - m_IconMousePnt.y ), false);
            }
        }
        break ;
    case ICON_TYPE_ITEM:
        {
            CItem* pItem = g_objHero.GetItem( m_IconRealID ) ;
            if ( pItem != NULL )
            {
                CMyPos posMouse;
                ::MouseCheck(posMouse.x, posMouse.y);
                pItem->ShowMiniItem( m_IconShowPnt.x + ( mousePnt.x - m_IconMousePnt.x ),
                                     m_IconShowPnt.y + ( mousePnt.y - m_IconMousePnt.y ), false);
            }
        }
        break ;
    case ICON_TYPE_SHOP:
        {
            CItem* pItem = g_objShop.GetItem( m_IconIndex ) ;
            if ( pItem != NULL )
            {
                CMyPos posMouse;
                ::MouseCheck(posMouse.x, posMouse.y);
                pItem->ShowMiniItem( m_IconShowPnt.x + ( mousePnt.x - m_IconMousePnt.x ),
                                     m_IconShowPnt.y + ( mousePnt.y - m_IconMousePnt.y ), false);
            }
        }
        break ;
    case ICON_TYPE_ACTION:
        {
            char strName[32] ;
            sprintf ( strName, "ButtonA%d", m_IconTypeID ) ;
            CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                            strName,
                            EXIGENCE_IMMEDIATE ) ;
            if ( ShowAni != NULL )
                ShowAni->Show ( 0,
                                m_IconShowPnt.x + ( mousePnt.x - m_IconMousePnt.x ),
                                m_IconShowPnt.y + ( mousePnt.y - m_IconMousePnt.y ) ) ;
        }
        break ;
    case ICON_TYPE_SKILL:
        /*		{
        			char strName[32] ;
        			sprintf ( strName, "MagicSkillType%d", m_IconTypeID ) ;
        			if (m_IconTypeID != ID_NONE)
        			{
        				CAni* ShowAni = g_objGameDataSet.GetDataAni ( g_strMagicAni,
        													      strName,
        															  EXIGENCE_IMMEDIATE ) ;
        				if ( ShowAni != NULL )
        					ShowAni->Show ( 0,
        									m_IconShowPnt.x + ( mousePnt.x - m_IconMousePnt.x ),
        									m_IconShowPnt.y + ( mousePnt.y - m_IconMousePnt.y ) ) ;
        			}
        		}*/
        break ;
    case ICON_TYPE_XP:
        /*		{
        			char strName[32] ;
        			sprintf ( strName, "MagicSkillType%d", m_IconTypeID ) ;
        			CAni* ShowAni = g_objGameDataSet.GetDataAni ( g_strMagicAni,
        													      strName,
        														  EXIGENCE_IMMEDIATE ) ;
        			if ( ShowAni != NULL )
        				ShowAni->Show ( 0,
        								m_IconShowPnt.x + ( mousePnt.x - m_IconMousePnt.x ),
        								m_IconShowPnt.y + ( mousePnt.y - m_IconMousePnt.y ) ) ;
        		}*/
        break;
    }
}

void CMyShellDlg::OnIconDrop( LPARAM lParam )
{
    CPoint InsertLandR ;
    switch ( lParam )
    {
    case DLG_AUCPACKAGE:
        break;
    case DLG_MONSTERDEPOT:
        if (m_PickUpDlgID == DLG_MONSTER)
        {
            if (g_objHero.GetPackageItemAmount() >= 6)
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10107 ) ) ;//"您的仓库已满，不能放入物品" ) ;//
                return;
            }
            else
            {
                g_objHero.CheckInItem(m_IconRealID);
                g_bPickUp = false;
            }
        }
        else if (m_PickUpDlgID == DLG_MONSTEREGG)
        {
            if (g_objHero.GetPackageItemAmount() >= 6)
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10107 ) ) ;//"您的仓库已满，不能放入物品" ) ;//
                return;
            }
            else
            {
                g_objHero.CheckInItem(m_IconRealID);
                g_bPickUp = false;
            }
        }
        else if (m_PickUpDlgID == DLG_MONSTERDEPOT)
        {
            m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp();
            g_bPickUp = false;
        }
        break;
    case DLG_MONSTERHATCH:
        if (m_PickUpDlgID == DLG_MONSTEREGG)
        {
            if ( g_objHero.GetPackageItemAmount () >= 5 )
            {
                g_objGameMsg.AddMsg ( g_objGameDataSet.GetStr(10860) ) ;	 //"您的幻兽孵化器已满，不能放入!"
                return ;
            }
            else
            {
                m_DlgMain.m_DlgMonsterHatch.AddEgg(m_IconRealID);
                g_bPickUp = false ;
            }
        }
        else if (m_PickUpDlgID == DLG_MONSTERHATCH)
        {
            m_DlgMain.m_DlgMonsterHatch.m_GridEggs.ReSumePickUp();
            g_bPickUp = false;
        }
        break;
    case DLG_MAIN:
        // Only the mpcgoods can insert the item
        if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            g_bPickUp = false ;
            m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.ReSumePickUp () ;
            //检验是否能放到快捷栏
            CItem* pItem = g_objHero.GetItem(m_IconRealID);
            if (pItem == NULL || pItem->GetSort() != ITEMSORT_EXPEND)
            {
                return;
            }
            InsertLandR = m_DlgMain.m_DlgTalk.m_GoodBox.GetInsertLandR () ;
            m_DlgMain.m_DlgTalk.m_GoodBox.InsertIcon ( InsertLandR.y,
                    InsertLandR.x,
                    m_IconTypeID,
                    m_IconIndex,
                    ICON_TYPE_QUSE ) ;
            CMyShellDlg::SetQUseItemNum ( ) ;
        }
        else if (m_PickUpDlgID == DLG_STONEGOODS0)
        {
            g_bPickUp = false ;
            m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
            //检验是否能放到快捷栏
            CItem* pItem = g_objHero.GetItem(m_IconRealID);
            OBJID ItemType = pItem->GetSubType() / 1000 * 1000;
            if (pItem == NULL )
            {
                return;
            }
            InsertLandR = m_DlgMain.m_DlgTalk.m_GoodBox.GetInsertLandR () ;
            m_DlgMain.m_DlgTalk.m_GoodBox.InsertIcon ( InsertLandR.y,
                    InsertLandR.x,
                    pItem->GetTypeID(),
                    m_IconIndex,
                    ICON_TYPE_QUSE ) ;
            CMyShellDlg::SetQUseItemNum ( ) ;
        }
        else if ( m_PickUpDlgID == DLG_MAIN )
        {
            g_bPickUp = false ;
            InsertLandR = m_DlgMain.m_DlgTalk.m_GoodBox.GetInsertLandR () ;
            m_DlgMain.m_DlgTalk.m_GoodBox.InsertIcon ( InsertLandR.y,
                    InsertLandR.x,
                    m_IconTypeID,
                    m_IconIndex,
                    ICON_TYPE_QUSE) ;
            CMyShellDlg::SetQUseItemNum ( ) ;
        }
        break ;
    case DLG_MONSTER:
        {
            if (m_PickUpDlgID == DLG_MONSTERDEPOT)
            {
                g_bPickUp = false;
                if ( g_objHero.GetItemAmount (PACKAGE_PET) >= g_objHero.GetPackageLimit (PACKAGE_PET) )
                {
                    g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10698) ) ;
                    m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp () ;
                    return ;
                }
                else
                {
                    CItem* pOutItem = g_objHero.GetPackageItemByIndex ( m_IconIndex ) ;
                    if ( pOutItem != NULL )
                    {
                        OBJID OutItemID = pOutItem->GetID () ;
                        g_objHero.CheckOutItem ( OutItemID ) ;
                    }
                }
            }
            else if (m_PickUpDlgID == DLG_MONSTER)
            {
                g_bPickUp = false;
                CMyShellDlg::PostMessage(WM_MY_MESSAGE, CMD_FLASHITEM);
            }
        }
    case DLG_MONSTEREGG:
        if (m_PickUpDlgID == DLG_MONSTERDEPOT)
        {
            if ( g_objHero.GetItemAmount (PACKAGE_EGG) >= g_objHero.GetPackageLimit (PACKAGE_EGG) )
            {
                g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10699) ) ;
                m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp () ;
                return ;
            }
            else
            {
                CItem* pOutItem = g_objHero.GetPackageItemByIndex ( m_IconIndex ) ;
                if ( pOutItem != NULL )
                {
                    OBJID OutItemID = pOutItem->GetID () ;
                    g_objHero.CheckOutItem ( OutItemID ) ;
                    g_bPickUp = false;
                }
            }
        }
        else if (m_PickUpDlgID == DLG_MONSTEREGG)
        {
            g_bPickUp = false;
            m_DlgMain.m_DlgMonster.m_EggsGri.ReSumePickUp();
        }
        break;
    case DLG_STONEGOODS0:
        if (m_PickUpDlgID == DLG_NPCBUY)
        {
            g_bPickUp = false;
            g_objHero.BuyItem ( m_IconTypeID ) ;
            m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.ReSumePickUp ( ) ;
        }
        else if (m_PickUpDlgID == DLG_DEPOT)
        {
            m_IconIndex = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpIndex () ;
            m_IconIndex = m_DlgMain.m_DlgDepot.m_nCurPage * 20 + m_IconIndex ;
            CItem* pOutItem = g_objHero.GetPackageItemByIndex ( m_IconIndex ) ;
            if ( pOutItem != NULL && pOutItem->GetPackageType() == PACKAGE_SOUL)
            {
                if ( g_objHero.GetItemAmount (PACKAGE_SOUL) >= g_objHero.GetPackageLimit (PACKAGE_SOUL) )
                {
                    g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10677)) ;
                    m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp () ;
                    return ;
                }
                g_objHero.CheckOutItem (pOutItem->GetID ()) ;
                g_bPickUp = false;
            }
        }
        else if (m_PickUpDlgID == lParam)
        {
            g_bPickUp = false;
            m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
        }
        else if ( m_PickUpDlgID == DLG_MAIN )
        {
            g_bPickUp = false ;
        }
        break;
    case DLG_MPCGOODS:
        // Buy a good from npc
        if ( m_PickUpDlgID == DLG_MAIN )
        {
            g_bPickUp = false ;
        }
        else if ( m_PickUpDlgID == DLG_NPCBUY )
        {
            g_bPickUp = false ;
            g_objHero.BuyItem ( m_IconTypeID ) ;
            m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.ReSumePickUp ( ) ;
        }
        // Hide a main icon
        else if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            g_bPickUp = false ;
            InsertLandR = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetInsertLandR () ;
            int nIndex = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.InsertIcon( InsertLandR.y,
                         InsertLandR.x,
                         m_IconTypeID,
                         m_IconIndex,
                         ICON_TYPE_ITEM ) ;
            m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.SetIconRealID( nIndex, m_IconRealID ) ;
        }
        else if ( m_PickUpDlgID == DLG_EQUIP )
        {
            g_bPickUp = false ;
            g_objHero.UnequipItem( m_IconIndex ) ;
        }
        else if (m_PickUpDlgID == DLG_DEPOT)
        {
            if ( g_objHero.GetItemAmount () >= g_objHero.GetPackageLimit () )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10106 ) ) ;//"您的物品栏已满，不能取出物品" ) ;
                m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp () ;
                g_bPickUp = false;
                return ;
            }
            m_IconIndex = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpIndex () ;
            m_IconIndex = m_DlgMain.m_DlgDepot.m_nCurPage * 20 + m_IconIndex ;
            CItem* pOutItem = g_objHero.GetPackageItemByIndex ( m_IconIndex ) ;
            if ( pOutItem != NULL && pOutItem->GetPackageType() == PACKAGE_BAG)
            {
                OBJID OutItemID = pOutItem->GetID () ;
                g_objHero.CheckOutItem ( OutItemID ) ;
                g_bPickUp = false;
            }
        }
        else if (m_PickUpDlgID == DLG_AUCPACKAGE)
        {
            if ( g_objHero.GetItemAmount () >= g_objHero.GetPackageLimit () )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10106 ) ) ;//"您的物品栏已满，不能取出物品" ) ;
                m_DlgMain.m_DlgAucPackage.m_GridAucitem.ReSumePickUp () ;
                return ;
            }
            CItem* pOutItem = g_objHero.GetPackageItemByIndex ( m_IconIndex ) ;
            if ( pOutItem != NULL )
            {
                OBJID OutItemID = pOutItem->GetID () ;
                g_objHero.CheckOutItem ( OutItemID ) ;
                g_bPickUp = false;
            }
        }
        break ;
    case DLG_TASKGET:
        if (m_PickUpDlgID == DLG_MPCGOODS
                || m_PickUpDlgID == DLG_STONEGOODS0 )
        {
            g_bPickUp = false;
            if (m_DlgMain.m_DlgPlayerTaskUp.CheckItem(m_IconRealID))
            {
                InsertLandR = m_DlgMain.m_DlgPlayerTaskUp.m_GridTaskItem.GetInsertLandR();
                int nIndex = m_DlgMain.m_DlgPlayerTaskUp.m_GridTaskItem.InsertIcon(InsertLandR.y,
                             InsertLandR.x,
                             m_IconTypeID,
                             m_IconIndex,
                             ICON_TYPE_ITEM ) ;
                m_DlgMain.m_DlgPlayerTaskUp.m_GridTaskItem.SetIconRealID(nIndex, m_IconRealID);
            }
            else
            {
                g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr (10676));	//"你已经添加过这个物品!"
            }
            m_DlgMain.m_DlgMpcGoods.SetGridContain();
            m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
        }
        break;
    case DLG_EMBED:
        break;
    case DLG_NPCBUY:
        if ( m_PickUpDlgID == DLG_MPCGOODS || m_PickUpDlgID == DLG_STONEGOODS0 )
        {
            g_bPickUp = false ;
            DXPlaySound ( "Sound/Sell.wav" ) ;
            CItem* pItem = g_objHero.GetItem( m_IconRealID ) ;
            if ( pItem )
            {
                if (pItem->TestMonopolyItem(_MONOPOLY_SELLCONFIRM)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetAddition() >= 4)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetWarGhostLevel() >= 9)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetGemAtkType() != 0 ))
                {
                    m_DlgConfirm.SetConfirmDlg(15, NULL, pItem->GetID());
                }
                else if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR)
                         || pItem->TestMonopolyItem(_MONOPOLY_NOT_SELL))
                {
                    g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10671)); //该物品不允许进行交易!
                    CMyShellDlg::PostMessage (WM_MY_MESSAGE, CMD_FLASHITEM);
                }
                else
                {
                    g_objHero.SellItem ( pItem->GetID () ) ;
                }
            }
        }
        else if ( m_PickUpDlgID == DLG_NPCBUY )
        {
            g_bPickUp = false ;
            if (m_DlgMain.m_DlgNpcBuy.m_bShow)
            {
                m_DlgMain.m_DlgNpcBuy.ReSetShop();
            }
        }
        break ;
    case DLG_TRADEMPC:
        if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            if (g_objHero.GetHeroTradeItemAmount() < 20)
            {
                CItem* pItem = g_objHero.GetItemByIndex ( m_IconIndex );
                if (pItem)
                {
                    g_bPickUp = false;
                    if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR)
                            || pItem->TestMonopolyItem(_MONOPOLY_NOT_SELL))
                    {
                        g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10673)); //该物品不允许被摆摊出售!
                        CMyShellDlg::PostMessage (WM_MY_MESSAGE, CMD_FLASHITEM);
                    }
                    else
                    {
                        g_objHero.AddHeroTradeItem ( pItem ) ;
                    }
                }
            }
        }
        else if (m_PickUpDlgID == DLG_STONEGOODS0 )
        {
            if (g_objHero.GetHeroTradeItemAmount() < 20)
            {
                CItem* pItem = g_objHero.GetItem ( m_IconRealID );
                if (pItem)
                {
                    g_bPickUp = false;
                    if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR)
                            || pItem->TestMonopolyItem(_MONOPOLY_NOT_SELL))
                    {
                        g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10673)); //该物品不允许被摆摊出售!
                        CMyShellDlg::PostMessage (WM_MY_MESSAGE, CMD_FLASHITEM);
                    }
                    else
                    {
                        g_objHero.AddHeroTradeItem ( pItem ) ;
                    }
                }
            }
        }
        else if (m_PickUpDlgID == DLG_MONSTER)
        {
            if (g_objHero.GetHeroTradeItemAmount() < 20)
            {
                CItem* pItem = g_objHero.GetItem ( m_IconRealID );
                if (pItem)
                {
                    g_bPickUp = false;
                    if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR)
                            || pItem->TestMonopolyItem(_MONOPOLY_NOT_SELL))
                    {
                        g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10673)); //该物品不允许被摆摊出售!
                        CMyShellDlg::PostMessage (WM_MY_MESSAGE, CMD_FLASHITEM);
                    }
                    else
                    {
                        g_objHero.AddHeroTradeItem ( pItem ) ;
                    }
                }
            }
        }
        else if (m_PickUpDlgID == DLG_MONSTEREGG)
        {
            if (g_objHero.GetHeroTradeItemAmount() < 20)
            {
                CItem* pItem = g_objHero.GetItem ( m_IconRealID );
                if (pItem)
                {
                    g_bPickUp = false;
                    if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR)
                            || pItem->TestMonopolyItem(_MONOPOLY_NOT_SELL))
                    {
                        g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10673)); //该物品不允许被摆摊出售!
                        CMyShellDlg::PostMessage (WM_MY_MESSAGE, CMD_FLASHITEM);
                    }
                    else
                    {
                        g_objHero.AddHeroTradeItem ( pItem ) ;
                    }
                }
            }
        }
        break ;
    case DLG_EQUIP:
        if (m_PickUpDlgID == DLG_MPCGOODS)
        {
            g_bPickUp = false;
        }
        break;
    case DLG_ALL:
        g_bPickUp = false ;
        CMyShellDlg::WindowProc ( WM_MY_MESSAGE, CMD_FLASHPACKAGEITEM, NULL ) ;
        CMyShellDlg::WindowProc ( WM_MY_MESSAGE, CMD_FLASHITEM, NULL ) ;
        if (m_DlgMain.m_DlgNpcBuy.m_bShow)
        {
            m_DlgMain.m_DlgNpcBuy.ReSetShop();
        }
        break ;
    case DLG_DEPOT:
        if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            if ( g_objHero.GetPackageItemAmount () >= g_objHero.GetPackageMaxItemAmount () )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10107 ) ) ;//"您的仓库已满，不能放入物品" ) ;//
                return ;
            }
            CItem* pItem = g_objHero.GetItemByIndex ( m_IconIndex , PACKAGE_BAG) ;
            if ( pItem && pItem->GetID () == m_IconRealID )
            {
                if (pItem->TestMonopolyItem(_MONOPOLY_NOT_DISAPPEAR) )
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10674)); //不能存入仓库
                    m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.ReSumePickUp ();
                    g_bPickUp = FALSE;
                }
                else
                {
                    g_objHero.CheckInItem ( pItem->GetID () ) ;
                    g_bPickUp = false ;
                }
            }
        }
        else if (m_PickUpDlgID == DLG_STONEGOODS0 )
        {
            if ( g_objHero.GetPackageItemAmount () >= g_objHero.GetPackageMaxItemAmount () )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10107 ) ) ;//"您的仓库已满，不能放入物品" ) ;//
                return ;
            }
            CItem* pItem = g_objHero.GetItem(m_IconRealID);
            if (pItem && pItem->GetPackageType() == PACKAGE_SOUL)
            {
                g_objHero.CheckInItem ( pItem->GetID () ) ;
                g_bPickUp = false ;
            }
        }
        else if (m_PickUpDlgID == DLG_DEPOT)
        {
            m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp();
            g_bPickUp = false;
        }
        break ;
    case DLG_ACT:
        {
            if ( g_bPickUp )
            {
                CPoint MousePnt ;
                CRect rect ;
                GetCursorPos ( &MousePnt ) ;
                m_DlgMain.m_DlgTalk.m_GoodBox.GetWindowRect ( rect ) ;
                if ( rect.PtInRect ( MousePnt ) )
                {
                    MousePnt.x = MousePnt.x - rect.left ;
                    MousePnt.y = MousePnt.y - rect.top ;
                    MousePnt = m_DlgMain.m_DlgTalk.m_GoodBox.GetLineAndRow ( MousePnt ) ;
                    m_DlgMain.m_DlgTalk.m_GoodBox.InsertIcon ( MousePnt.y,
                            MousePnt.x,
                            m_IconTypeID,
                            0,
                            ICON_TYPE_ACTION,
                            false ) ;
                }
                g_bPickUp = false ;
            }
        }
        break ;
    case DLG_MAGIC:
        /*		{
        			if ( g_bPickUp )
        			{
        				CPoint MousePnt ;
        				CRect rect ;
        				GetCursorPos ( &MousePnt ) ;
        				m_DlgMain.m_GoodBox.GetWindowRect ( rect ) ;
        				if ( rect.PtInRect ( MousePnt ) )
        				{
        					MousePnt.x = MousePnt.x - rect.left ;
        					MousePnt.y = MousePnt.y - rect.top ;
        					MousePnt = m_DlgMain.m_GoodBox.GetLineAndRow ( MousePnt ) ;
        					m_DlgMain.m_GoodBox.InsertIcon ( MousePnt.y,
        													 MousePnt.x,
        													 m_IconTypeID,
        													 0,
        													 ICON_TYPE_SKILL,
        													 false ) ;
        					g_bPickUp = false ;
        					break ;
        				}

        				// Drop the maigc image
        				m_DlgMain.m_MagicImg.GetWindowRect ( rect ) ;
        				if ( rect.PtInRect ( CPoint ( m_IconMousePnt.y, m_IconMousePnt.x ) ) )
        				{
        					if ( !rect.PtInRect ( MousePnt ) )
        					{
        						m_DlgMain.m_MagicImg.RemoveImage () ;
        						m_uCurSkill = 0 ;
        						m_MagicID = 0 ;
        						m_XpID = 0 ;
        					}
        				}
        				g_bPickUp = false ;
        			}
        		}*/
        break ;
    case DLG_XP:
        {
            if ( g_bPickUp )
            {
                CPoint MousePnt ;
                CRect rect ;
                GetCursorPos ( &MousePnt ) ;
                m_DlgMain.m_DlgTalk.m_GoodBox.GetWindowRect ( rect ) ;
                if ( rect.PtInRect ( MousePnt ) )
                {
                    MousePnt.x = MousePnt.x - rect.left ;
                    MousePnt.y = MousePnt.y - rect.top ;
                    MousePnt = m_DlgMain.m_DlgTalk.m_GoodBox.GetLineAndRow ( MousePnt ) ;
                    m_DlgMain.m_DlgTalk.m_GoodBox.InsertIcon ( MousePnt.y,
                            MousePnt.x,
                            m_IconTypeID,
                            0,
                            ICON_TYPE_XP,
                            false ) ;
                }
                g_bPickUp = false ;
            }
        }
        break ;
    default:	// in map?
        {
            int nMouseX, nMouseY;
            ::MouseCheck(nMouseX, nMouseY);
            OBJID idItem = ID_NONE;
            if ( m_PickUpDlgID == DLG_MPCGOODS )
            {
                CItem* pItem = g_objHero.GetItemByIndex(m_IconIndex);
                if (pItem)
                {
                    idItem = pItem->GetID();
                    g_bPickUp = false ;
                    g_objHero.DropItem(idItem, nMouseX, nMouseY);
                }
            }
            else if (m_PickUpDlgID == DLG_DEPOT)
            {
                m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp();
                g_bPickUp = false;
            }
            else if (m_PickUpDlgID == DLG_MONSTERDEPOT)
            {
                m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp();
                g_bPickUp = false;
            }
        }
        break;
    }
}

void CMyShellDlg::OnIconTrade( LPARAM lParam)
{
    CPoint TradeLandR ;
    switch ( lParam )
    {
    case DLG_MAIN:
        if ( m_PickUpDlgID == DLG_MAIN )
        {
            g_bPickUp = false ;
            TradeLandR = m_DlgMain.m_DlgTalk.m_GoodBox.GetInsertLandR () ;
            m_DlgMain.m_DlgTalk.m_GoodBox.ChangeIcon( TradeLandR,
                    m_IconTypeID,
                    m_IconRealID,
                    m_IconIndex,
                    ICON_TYPE_QUSE ) ;
            CMyShellDlg::SetQUseItemNum ( ) ;
        }
        else if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            g_bPickUp = false ;
            TradeLandR = m_DlgMain.m_DlgTalk.m_GoodBox.GetInsertLandR () ;
            m_DlgMain.m_DlgTalk.m_GoodBox.ChangeIcon( TradeLandR,
                    m_IconTypeID,
                    m_IconRealID,
                    m_IconIndex,
                    ICON_TYPE_QUSE ) ;
            m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.ReSumePickUp () ;
            CMyShellDlg::SetQUseItemNum ( ) ;
        }
        break ;
    case DLG_MONSTEREGG:
        if (m_PickUpDlgID == lParam)
        {
            g_bPickUp = false ;
            m_DlgMain.m_DlgMonster.m_EggsGri.ReSumePickUp();
        }
        break;
    case DLG_STONEGOODS0:
        if (m_PickUpDlgID == lParam)
        {
            g_bPickUp = false ;
            m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
        }
        else if ( m_PickUpDlgID == DLG_MAIN )
        {
            g_bPickUp = false ;
        }
        break;
    case DLG_MPCGOODS:
        if ( m_PickUpDlgID == DLG_NPCBUY )
        {
            g_bPickUp = false ;
            g_objHero.BuyItem ( m_IconTypeID ) ;
            m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.ReSumePickUp () ;
        }
        else if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            TradeLandR = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetInsertLandR () ;
            m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.ChangeIcon( TradeLandR,
                    m_IconTypeID,
                    m_IconRealID,
                    m_IconIndex,
                    ICON_TYPE_ITEM ) ;
            m_IconRealID = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpRealID () ;
            m_IconTypeID = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpTypeID () ;
            m_IconIndex = m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.GetPickUpIndex () ;
            m_DlgMain.m_DlgEmbed.SetPickUpState(m_IconRealID);
            m_DlgMain.m_DlgImprove.SetPickUpState(m_IconRealID);
            m_DlgMain.m_DlgTaskUpItem.SetTaskUpItem(m_IconRealID);
            m_DlgMain.m_DlgMpcGoods.SetPickUpItemID( m_IconRealID ) ;
            m_DlgMain.m_DlgQuery.m_DlgEquip.SetPickUpItemID(m_IconRealID);
            m_DlgMain.m_DlgIdentify.SetPickUpItemID(m_IconRealID);
        }
        else if ( m_PickUpDlgID == DLG_EQUIP )
        {
            g_objHero.UnequipItem( m_IconIndex ) ;
            g_bPickUp = false;
        }
        else if ( m_PickUpDlgID == DLG_MAIN )
        {
            g_bPickUp = false ;
        }
        else if (m_PickUpDlgID == DLG_DEPOT)
        {
            if ( g_objHero.GetItemAmount () >= g_objHero.GetPackageLimit () )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10106 ) ) ;//"您的物品栏已满，不能取出物品" ) ;
                m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp () ;
                g_bPickUp = false;
                return ;
            }
            m_IconIndex = m_DlgMain.m_DlgDepot.m_DepotGriLay.GetPickUpIndex () ;
            m_IconIndex = m_DlgMain.m_DlgDepot.m_nCurPage * 20 + m_IconIndex ;
            CItem* pOutItem = g_objHero.GetPackageItemByIndex ( m_IconIndex ) ;
            if ( pOutItem != NULL && pOutItem->GetPackageType() == PACKAGE_BAG)
            {
                OBJID OutItemID = pOutItem->GetID () ;
                g_objHero.CheckOutItem ( OutItemID ) ;
                g_bPickUp = false;
            }
        }
        break ;
    case DLG_NPCBUY:
        if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            g_bPickUp = false ;
            DXPlaySound ( "Sound/Sell.wav" ) ;
            CItem* pItem = g_objHero.GetItem( m_IconRealID ) ;
            if ( pItem )
            {
                if (pItem->TestMonopolyItem(_MONOPOLY_SELLCONFIRM)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetAddition() >= 4)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetWarGhostLevel() >= 9)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetGemAtkType() != 0 ))
                {
                    m_DlgConfirm.SetConfirmDlg(15, NULL, pItem->GetID());
                }
                else if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR)
                         || pItem->TestMonopolyItem(_MONOPOLY_NOT_SELL))
                {
                    g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr (10671)); //该物品不允许进行交易!
                    CMyShellDlg::PostMessage (WM_MY_MESSAGE, CMD_FLASHITEM);
                }
                else
                {
                    g_objHero.SellItem ( pItem->GetID () ) ;
                }
            }
        }
        else if ( m_PickUpDlgID == DLG_NPCBUY )
        {
            if (m_DlgMain.m_DlgNpcBuy.m_bShow)
            {
                m_DlgMain.m_DlgNpcBuy.ReSetShop();
            }
            g_bPickUp = false;
            break;
        }
        else if (m_PickUpDlgID == DLG_STONEGOODS0 )
        {
            g_bPickUp = false ;
            DXPlaySound ( "Sound/Sell.wav" ) ;
            CItem* pItem = g_objHero.GetItem( m_IconRealID ) ;
            if ( pItem )
            {
                if (pItem->TestMonopolyItem(_MONOPOLY_SELLCONFIRM)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetAddition() >= 4)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetWarGhostLevel() >= 9)
                        || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetGemAtkType() != 0 ))
                {
                    m_DlgConfirm.SetConfirmDlg(15, NULL, pItem->GetID());
                }
                else
                {
                    g_objHero.SellItem ( pItem->GetID () ) ;
                }
            }
        }
        break ;
    case DLG_DEPOT:
        if ( m_PickUpDlgID == DLG_MPCGOODS )
        {
            if ( g_objHero.GetPackageItemAmount () >= g_objHero.GetPackageMaxItemAmount () )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10107 ) ) ;//"您的仓库已满，不能放入物品" ) ;//
                return ;
            }
            CItem* pItem = g_objHero.GetItemByIndex ( m_IconIndex ) ;
            if ( pItem && pItem->GetID () == m_IconRealID )
            {
                if (pItem->TestMonopolyItem(_MONOPOLY_NOT_DISAPPEAR) )
                {
                    g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10674)); //不能存入仓库
                    m_DlgMain.m_DlgMpcGoods.m_MpcGoodsGri.ReSumePickUp ();
                    g_bPickUp = FALSE;
                }
                else
                {
                    g_objHero.CheckInItem ( pItem->GetID () ) ;
                    g_bPickUp = false ;
                }
            }
        }
        else if (m_PickUpDlgID == DLG_DEPOT)
        {
            CMyShellDlg::WindowProc ( WM_MY_MESSAGE, CMD_FLASHPACKAGEITEM, NULL ) ;
            g_bPickUp = false;
        }
        break;
    case DLG_AUCPACKAGE:
        break;
    }
}

void CMyShellDlg::OnIconUse(LPARAM lParam)
{
    BYTE nGridOwner = (BYTE)HIWORD(lParam) ;
    if (nGridOwner == DLG_MONSTERHATCH && m_DlgMain.m_DlgMonsterHatch.m_bShow)
    {
        int nGridIndex = LOWORD(lParam);
        CItem* pOutItem = g_objHero.GetPackageItemByIndex ( nGridIndex ) ;
        if ( pOutItem != NULL )
        {
            if (pOutItem->GetPackageType() == PACKAGE_PET && g_objHero.GetItemAmount (PACKAGE_PET) >= g_objHero.GetPackageLimit (PACKAGE_PET) )
            {
                g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10698) ) ;
                return ;
            }
            else if (pOutItem->GetPackageType() == PACKAGE_EGG)
            {
                m_DlgConfirm.SetConfirmDlg(34, NULL, nGridIndex);
                return;
            }
            OBJID OutItemID = pOutItem->GetID () ;
            g_objHero.CheckOutItem ( OutItemID ) ;
        }
    }
    else if (nGridOwner == DLG_MONSTERDEPOT && m_DlgMain.m_DlgMonster.m_bShow)
    {
        int nGridIndex = LOWORD(lParam);
        CItem* pOutItem = g_objHero.GetPackageItemByIndex ( nGridIndex ) ;
        if ( pOutItem != NULL )
        {
            if (pOutItem->GetPackageType() == PACKAGE_PET &&  g_objHero.GetItemAmount (PACKAGE_PET) >= g_objHero.GetPackageLimit (PACKAGE_PET) )
            {
                g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10698) ) ;
                m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp () ;
                return ;
            }
            if (pOutItem->GetPackageType() == PACKAGE_EGG &&  g_objHero.GetItemAmount (PACKAGE_EGG) >= g_objHero.GetPackageLimit (PACKAGE_EGG) )
            {
                g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10698) ) ;
                m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp () ;
                return ;
            }
            OBJID OutItemID = pOutItem->GetID () ;
            g_objHero.CheckOutItem ( OutItemID ) ;
        }
    }
    else if (nGridOwner == DLG_AUCPACKAGE && m_DlgMain.m_DlgMpcGoods.m_bShow)
    {
        int nGridIndex = LOWORD(lParam);
        CItem* pOutItem = g_objHero.GetPackageItemByIndex ( nGridIndex ) ;
        if ( pOutItem != NULL )
        {
            if (pOutItem->GetPackageType() == PACKAGE_BAG && g_objHero.GetItemAmount () >= g_objHero.GetPackageLimit (PACKAGE_BAG) )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10106 ) ) ;//"您的物品栏已满，不能取出物品" ) ;
                m_DlgMain.m_DlgAucPackage.m_GridAucitem.ReSumePickUp () ;
                return ;
            }
            else if (pOutItem->GetPackageType() == PACKAGE_SOUL && g_objHero.GetItemAmount(PACKAGE_SOUL) >= g_objHero.GetPackageLimit (PACKAGE_SOUL))
            {
                g_objGameMsg.AddMsg (( char* )g_objGameDataSet.GetStr ( 10677 )) ;
                m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp () ;
                return ;
            }
            OBJID OutItemID = pOutItem->GetID () ;
            g_objHero.CheckOutItem ( OutItemID ) ;
        }
    }
    else if (nGridOwner == DLG_DEPOT )
    {
        int nGridIndex = LOWORD(lParam);
        nGridIndex = m_DlgMain.m_DlgDepot.m_nCurPage * 20 + nGridIndex ;
        CItem* pOutItem = g_objHero.GetPackageItemByIndex ( nGridIndex ) ;
        if ( pOutItem != NULL )
        {
            if (pOutItem->GetPackageType() == PACKAGE_BAG && g_objHero.GetItemAmount () >= g_objHero.GetPackageLimit (PACKAGE_BAG) )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10106 ) ) ;//"您的物品栏已满，不能取出物品" ) ;
                m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp () ;
                return ;
            }
            else if (pOutItem->GetPackageType() == PACKAGE_SOUL && g_objHero.GetItemAmount(PACKAGE_SOUL) >= g_objHero.GetPackageLimit (PACKAGE_SOUL))
            {
                g_objGameMsg.AddMsg (( char* )g_objGameDataSet.GetStr ( 10677 ) ) ;
                m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp () ;
                return ;
            }
            OBJID OutItemID = pOutItem->GetID () ;
            g_objHero.CheckOutItem ( OutItemID ) ;
        }
    }
    else if (nGridOwner == DLG_MPCGOODS)
    {
        int nGridIndex = LOWORD(lParam);
        CItem* pItem;
        pItem = g_objHero.GetItemByIndex ( nGridIndex) ;
        if (pItem != NULL)
        {
            if (!g_objHero.TryItem( pItem ) )
            {
                return ;
            }
            switch(pItem->GetSort())
            {
            case ITEMSORT_EXPEND:
                if (!pItem->IsArrow())
                {
                    DXPlaySound ( "Sound/Eat.wav" ) ;
                }
                break;
            case ITEMSORT_WEAPON_SINGLE_HAND:
            case ITEMSORT_WEAPON_DOUBLE_HAND:
                DXPlaySound("Sound/Role_Weapon.wav");
                break;
            case ITEMSORT_HELMET:
                DXPlaySound("Sound/Role_Armet.wav");
                break;
            case ITEMSORT_SHOES:
                DXPlaySound("Sound/Role_Shoes.wav");
                break;
            case ITEMSORT_RING:
                DXPlaySound("Sound/Role_Bangle.wav");
                break;
            case ITEMSORT_MOUNT:
                DXPlaySound("Sound/Role_Armet.wav");
                break;
            case ITEMSORT_ARMOR:
                DXPlaySound("Sound/Role_Armor.wav");
                break;
            case ITEMSORT_NECKLACE:
                DXPlaySound("Sound/Role_Necklace.wav");
                break;
            default:
                DXPlaySound("Sound/Role_Armet.wav");
                break;
            }
            CMyShellDlg::SetUseItemEffect(m_MsgDlgPnt.x, m_MsgDlgPnt.y, pItem->GetTypeID());
            g_objHero.UseItem(pItem->GetID());
        }
    }
    else if (nGridOwner == DLG_STONEGOODS0 )
    {
        int nIconIndex = LOWORD(lParam);
        int nIconRealID = 0;
        nIconRealID = m_DlgMain.m_DlgMpcGoods.m_MpcStonesGri.GetIconRealID(nIconIndex);
        if (nIconRealID != 0)
        {
            CItem* pGem = g_objHero.GetItem(nIconRealID);
            if (pGem )
            {
                CMyShellDlg::SetUseItemEffect(m_MsgDlgPnt.x, m_MsgDlgPnt.y, pGem->GetTypeID());
                OnIconUseGem(nIconRealID);
            }
        }
    }
}

void CMyShellDlg::OnIconBuy(LPARAM lParam)
{
    g_objHero.BuyItem( lParam ) ;
}

void CMyShellDlg::OnIconQUse (LPARAM lParam)
{
    int nGridIndex = (BYTE)lParam ;
    int nIconTypeID = lParam >> 8 & 0xffffff ;
    switch ( m_DlgMain.m_DlgTalk.m_GoodBox.GetIconType ( nGridIndex ) )
    {
    case ICON_TYPE_QUSE:
        {
            CItem* pItem = NULL ;
            bool bLoad = false;
            for ( int i = 0; i < g_objHero.GetItemAmount(); i++ )
            {
                pItem = g_objHero.GetItemByIndex( i ) ;
                if ( pItem != NULL && pItem->GetTypeID() == nIconTypeID )
                {
                    g_objHero.UseItem( pItem->GetID() ) ;
                    CPoint posItem = m_DlgMain.m_DlgTalk.m_GoodBox.GetGridPnt(1, nGridIndex + 1);
                    CMyShellDlg::SetUseItemEffect(posItem.x, posItem.y, pItem->GetTypeID());
                    CMyShellDlg::SetQUseItemNum() ;
                    bLoad = true;
                    break ;
                }
            }
            if ( !bLoad )
            {
                for( int j = 0; j < g_objHero.GetItemAmount(PACKAGE_SOUL); j++)
                {
                    pItem = g_objHero.GetItemByIndex(j, PACKAGE_SOUL);
                    if (pItem != NULL && pItem->GetTypeID() == nIconTypeID)
                    {
                        CPoint posItem = m_DlgMain.m_DlgTalk.m_GoodBox.GetGridPnt(1, nGridIndex + 1);
                        CMyShellDlg::SetUseItemEffect(posItem.x, posItem.y, pItem->GetTypeID());
                        CMyShellDlg::OnIconUseGem( pItem->GetID() ) ;
                        CMyShellDlg::SetQUseItemNum() ;
                        break;
                    }
                }
            }
        }
        break ;
    case ICON_TYPE_SKILL:
        /*		{
        			m_MagicID = nIconTypeID ;
        			m_uCurSkill = 1 ;
        			char SkillType[32] ;
        			sprintf ( SkillType, "MagicSkillType%d", m_MagicID ) ;
        			m_DlgMain.m_MagicImg.InsertImage ( SkillType, 0, m_MagicID, true ) ;
        		}		*/
        break ;
    default:
        break ;
    }
}

LRESULT CMyShellDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            if ( m_bLogin )
            {
                m_DlgMain.OnShellEnter () ;
            }
            return true ;
        }
        else if ( wParam == 2 )
        {
            /*
            			if ( g_objHero.IsTradeActive() )
            			{
            				g_objGameMsg.AddMsg( g_objGameDataSet.GetStr( 10026 ) ) ;
            				return true;
            			}

            			if ( g_bPickUp )
            			{
            				g_bPickUp = false ;
            				return true;
            			}
            			if (!CMyShellDlg::CloseCurDlg())
            				return true;

            			CMyShellDlg::OnExit ( 2 )	;	*/
            return true ;
        }
        else if ( wParam == 57670 )
        {
            return true ;
        }
    }
    else if ( message == WM_ACTIVATE)
    {
        int a = 0 ;
    }
    /*
    	else if (message == WM_91U_MESSAGE)
    	{
    		CMyShellDlg::On91UMessage(wParam,lParam);
    	}*/
    else if ( message == WM_MY_MESSAGE )
    {
        switch ( wParam )
        {
        case CMD_PET_ALIVE:
            m_DlgMain.m_DlgMonsterHeads.AddEffect("baby_d8", lParam);
            break;
        case CMD_PET_DIE:
            m_DlgMain.m_DlgMonsterHeads.AddEffect("baby_d4", lParam);
            break;
        case CMD_ACTION_EVOMONSTERG:
        case CMD_ACTION_EVOMONSTERS:
        case CMD_ACTION_EVOMONSTER1:
        case CMD_ACTION_EVOMONSTER2:
        case CMD_ACTION_EVOMONSTER3:
        case CMD_ACTION_EVOMONSTER4:
        case CMD_ACTION_EVOMONSTER5:
        case CMD_ACTION_EVOMONSTER6:
        case CMD_ACTION_EVOMONSTER7:
        case CMD_ACTION_EVOMONSTER8:
            m_DlgMain.OpenMonsterEvolve(wParam - CMD_ACTION_EVOMONSTERG + 1);
            break;
        case CMD_EVOMONSTER_SUC:
            m_DlgMain.m_DlgMonsterEvolve.EvolveEudemonOk();
            break;
        case CMD_ACTION_OVERAUCTION:
            if (m_DlgMain.m_DlgBidAuction.m_bShow)
            {
                m_DlgMain.m_DlgBidAuction.EnableWindow (false);
            }
            break;
        case CMD_CHANGE_MAP:
            m_DlgMain.m_DlgMonsterHeads.ReCallMonsters();
            if ((g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE )
                    && m_DlgMain.m_DlgXp.m_bShow
                    && !m_DlgMain.m_DlgXp.m_btBoothType
                    && !m_DlgMain.m_DlgXp.m_bDie)
            {
                m_DlgMain.m_DlgXp.ShowWindow(SW_HIDE);
            }
            break;
        case CMD_FLASH_SYNALLY:
            m_DlgMain.m_DlgKnightage.m_DlgKnightageEnemy.SetEnemyList();
            m_DlgMain.m_DlgKnightage.m_DlgKnightageEnemy.SetFriendList();
            break;
        case CMD_FLASH_SYNDOC:
            break;
        case CMD_FLASH_PETSTATUS:
            {
                m_DlgMain.m_DlgMonsterHeads.UpdataCallMonster();
                m_DlgMain.m_DlgMonster.SetCurMonster (m_DlgMain.m_DlgMonster.m_iCurMonster);
                m_DlgMain.m_DlgMonster.m_DlgMonsterSkill.SetMonsterSKill ();
                m_DlgMain.m_DlgMonster.SetMonsterList ();
            }
            break;
        case CMD_FLASH_MYPETINFO:
            {
                m_DlgMain.m_DlgMonsterHeads.UpdataCallMonster();
                m_DlgMain.m_DlgMonster.SetCurMonster(m_DlgMain.m_DlgMonster.m_iCurMonster);
            }
            break;
        case CMD_ACTION_NEWAUCTION:
            if (m_DlgMain.m_DlgBidAuction.m_bShow && m_DlgMain.m_DlgBidAuction.m_AuctionNpcId)
            {
                m_DlgMain.m_DlgBidAuction.SetAuctionNpcId(m_DlgMain.m_DlgBidAuction.m_AuctionNpcId);
            }
            break;
        case CMD_ACTION_FLASHAUCTION:
            m_DlgMain.m_DlgBidAuction.SetAuctionData();
            break;
        case CMD_ACTION_OPENAUCTIONBID:
            m_DlgMain.OpenBidAuctionDlg(g_objHero.GetLastDialogPara());
            break;
        case CMD_ACTION_OPENAUCPACKAGE:
            m_DlgMain.OpenAucPackage(g_objHero.GetLastDialogPara());
            break;
        case CMD_ACTION_OPENADDAUCTION:
            m_DlgMain.OpenAddAucItemDlg(g_objHero.GetLastDialogPara());
            break;
        case CMD_ACTION_CLOSEBOOTH:
            if ( m_DlgMain.m_DlgNpcBooth.m_bShow )
            {
                m_DlgMain.m_DlgNpcBooth.EnableWindow(false) ;
                m_DlgMain.m_uCurLDlgID = -1;
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10707)); //"摆摊玩家取消了摆摊!"
                g_objHero.m_objBooth.Close () ;
            }
            break;
        case CMD_ACTION_OPENDICECFM:
            m_DlgConfirm.SetConfirmDlg ( 18, NULL, g_objHero.GetLastDialogPara() ) ;
            break;
        case CMD_ACTION_OPENBOOTHFLAG:
            m_DlgConfirm.SetConfirmDlg( 13 , NULL, g_objHero.GetLastDialogPara() ) ;
            break;
        case CMD_ACTION_OPENBOOTH:
            {
                //				if ( g_objHero.GetLastDialogPara() == g_objHero.m_objBoothManager.GetID () )
                m_DlgMain.OpenBooth ( true ) ;
                /*				else
                				{
                					if ( m_DlgMain.m_DlgTrade.m_bShow || ( m_DlgMain.m_DlgNpcBooth.m_bShow && g_objHero.GetLastDialogPara() == g_objHero.m_objBooth.GetID () ) )
                					{
                						g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10398) ) ;
                					}
                					else
                					{
                						if (g_objHero.m_objBooth.Open ( g_objHero.GetLastDialogPara() ) )
                							m_DlgMain.OpenBooth ( false ) ;
                					}
                				}	*/
            }
            break;
        case CMD_ACTION_OPENNPCFACE:
            if (!m_DlgMain.m_DlgNpcFace.m_bShow)
            {
                m_DlgMain.m_DlgNpcFace.EnableWindow();
            }
            break;
        case CMD_FLASH_SECSCHOOLLIST:
            m_DlgMain.m_DlgEnemyFriend.m_DlgSchoolList.SetSchoolList();
            break;
        case CMD_ACTION_OPENMONSTERDEPOT:
            m_DlgMain.OpenMonsterDepot(g_objHero.GetLastDialogPara());
            break;
        case CMD_ACTION_OPENMONSTERALIVE:
            m_DlgMain.OpenMonsterImpDlg(0);
            break;
        case CMD_ACTION_OPENMONSTERIMPROVE:
            m_DlgMain.OpenMonsterImpDlg(1);
            break;
        case CMD_ACTION_OPENMONSTERHATCH:
            m_DlgMain.OpenHatchDlg(g_objHero.GetLastDialogPara());
            break;
        case CMD_FLASH_ANNOUNCELIST:
            m_DlgMain.OpenAnnounceList();
            m_DlgMain.m_DlgAnnounceList.SetTboardList();
            break;
        case CMD_FLASH_ANNOUNCEINFO:
            m_DlgMain.OpenAnnounceInfo();
            m_DlgMain.m_DlgAnnounceInfo.SetTboardInfo();
            break;
        case CMD_ACTION_OPENUPANNOUNCE:
            m_DlgMain.OpenAnnounceUp();
            break;
        case CMD_ACTION_OPENANNOUNCELIST:
            g_objHero.QueryTeacherBoardList(0);
            break;
        case CMD_ACTION_OPENMYANNOUNCE:
            g_objHero.QueryMyTeacherBoard();
            m_DlgMain.m_DlgAnnounceInfo.SetInfoMode(1);
            break;
        case CMD_ACTION_OPENSHOP:
            m_DlgMain.OpenNpcShop(_ROLE_SHOPKEEPER_NPC, g_objHero.GetLastDialogPara());
            break;
        case CMD_ACTION_OPENIMPROVE:	//升级品质
            m_DlgMain.OpenImprove(0);
            break;
        case CMD_ACTION_OPENIMPROVESOUL: //升级魔魂
            m_DlgMain.OpenImprove(1);
            break;
        case CMD_ACTION_OPENIMPROVELEVEL: //升级等级
            m_DlgMain.OpenImprove(2);
            break;
        case CMD_ACTION_OPENIMPROVEGEM: //注入宝石
            m_DlgMain.OpenImprove(3);
            break;
        case CMD_ACTION_OPENDEPOT:
            m_DlgMain.OpenDepot(_ROLE_STORAGE_NPC, g_objHero.GetLastDialogPara());
            break;
        case CMD_ACTION_OPENEMBED:
            m_DlgMain.OpenEmbed();
            break;
        case CMD_ACTION_UNACCEPTEDTASKLIST:
            m_DlgMain.OpenTaskList(0);
            m_DlgMain.m_DlgPlayerTaskView.SetPlayerTaskViewMode(0);
            break;
        case CMD_ACTION_ACCEPTEDTASKLIST:
            m_DlgMain.OpenTaskList(1);
            m_DlgMain.m_DlgPlayerTaskView.SetPlayerTaskViewMode(1);
            break;
        case CMD_ACTION_MYTASKLIST:
            m_DlgMain.OpenTaskList(2);
            m_DlgMain.m_DlgPlayerTaskView.SetPlayerTaskViewMode(2);
            break;
        case CMD_ACTION_PLAYERTASKUP:
            m_DlgMain.OpenTaskUpDlg();
            break;
        case CMD_FLASH_PLAYERTASKDETIAL:
            m_DlgMain.m_DlgPlayerTaskView.SetPlayerTask();
            break;
        case CMD_FLASH_PLAYERTASKLIST:
            if (m_DlgMain.m_DlgPlayerTaskList.m_bShow)
            {
                m_DlgMain.m_DlgPlayerTaskList.GetTaskList();
            }
            break;
        case CMD_FLASH_SCHOOLINFO:
            if (m_DlgMain.m_DlgEnemyFriend.m_iShowMode == 2)
            {
                m_DlgMain.m_DlgEnemyFriend.SetTsList();
            }
            break;
        case CMD_ACTION_TEACHAPPLY:		// 有人向你拜师了
            m_DlgConfirm.SetConfirmDlg(TEACHER_CONFIRM, g_objHero.GetLastTeacherApply());
            break;
        case CMD_ACTION_STUDENTAPPLY:	// 有人要收你做徒弟
            m_DlgConfirm.SetConfirmDlg(STUDENT_CONFIRM, g_objHero.GetLastStudentApply());
            break;
        case CMD_FLASHITEM:
            m_DlgMain.m_DlgMpcGoods.SetGridContain() ;
            m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
            if (m_DlgMain.m_DlgMonster.m_bShow)
            {
                m_DlgMain.m_DlgMonster.SetMonsterList();
                m_DlgMain.m_DlgMonster.SetEggGrid();
                m_DlgMain.m_DlgMonster.ResetMonstStatus();
            }
            m_DlgMain.m_DlgMonsterHeads.FlashMonsterHeads ();
            CMyShellDlg::SetQUseItemNum () ;
            break ;
        case CMD_FLASHEQUIPMENT:
            m_DlgMain.m_DlgQuery.m_DlgEquip.FlashEquip();
            m_DlgMain.m_DlgQuery.m_DlgEquip.SetRoleInfo() ;
            break;
        case CMD_TRADEPLAYERAPPLY:
            {
                if ( !m_DlgMain.m_DlgTrade.m_bShow )
                {
                    char TradePlayName[16] ;
                    strcpy ( TradePlayName, g_objHero.GetApplyTradeName() ) ;
                    m_DlgConfirm.SetConfirmDlg( 0, TradePlayName ) ;
                }
            }
            break ;
        case CMD_OPENTRADE:
            m_DlgMain.CloseCurDlg(0);
            m_DlgMain.CloseCurDlg(1);
            m_DlgMain.CloseCurDlg(2);
            m_DlgMain.m_uCurLDlgID = DLG_TRADE ;
            m_DlgMain.m_DlgTrade.EnableWindow() ;
            if ( !m_DlgMain.m_DlgMpcGoods.m_bShow )
            {
                m_DlgMain.m_DlgMpcGoods.EnableWindow() ;
            }
            break ;
        case CMD_TRADEHEROITEM:
            m_DlgMain.m_DlgTrade.SetMpcGridContain () ;
            break ;
        case CMD_TRADEPLAYERITEM:
            m_DlgMain.m_DlgTrade.SetNpcGridContain () ;
            break ;
        case CMD_TRADHEROEMONEY:
            //			m_DlgMain.m_DlgTrade.SetNpcMoney () ;
            break ;
        case CMD_FLASHMONEY:
            //			m_DlgMain.m_Dlg.SetNpcMoney () ;
            break ;
        case CMD_TRADPLAYERMONEY:
            m_DlgMain.m_DlgTrade.SetNpcMoney () ;
            break ;
        case CMD_TRADEPLAYERAGREE:
            g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10108 ) ) ;//"对方同意交易" ) ;//
            m_DlgMain.m_DlgTrade.m_NpcAgreeChk.SetCheckValue(1) ;
            break ;
        case CMD_TRADEFALSE:
            if (g_objHero.IsTradeActive () )
            {
                g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10109 ) ) ;    //"对方取消交易" ) ;//
            }
            m_DlgMain.m_DlgTrade.EnableWindow( false ) ;
            m_DlgMain.m_DlgMpcGoods.SetGridContain () ;
            m_DlgMain.m_DlgMpcGoods.SetStoneGrid();
            break ;
        case CMD_TRADEOK:
            m_DlgMain.m_DlgTrade.EnableWindow( false ) ;
            break ;
        case CMD_FLASHBANKMONEY:
            m_DlgMain.m_DlgDepot.SetMoney ( ) ;
            break ;
        case CMD_FLASHPACKAGEITEM:
            if (g_objHero.GetPackageType() == PACKAGE_STORAGE)
            {
                m_DlgMain.m_DlgDepot.SetItem() ;
            }
            else if (g_objHero.GetPackageType() == PACKAGE_EUDEMON_STORAGE)
            {
                m_DlgMain.m_DlgMonsterDepot.SetMonster();
            }
            else if (g_objHero.GetPackageType() == PACKAGE_EUDEMON_BROODER)
            {
                m_DlgMain.m_DlgMonsterHatch.SetGrid();
            }
            else if (g_objHero.GetPackageType() == PACKAGE_AUCTION_STORAGE)
            {
                m_DlgMain.m_DlgAucPackage.SetAucPackage();
            }
            break ;
        case CMD_FLASHFRIENDLIST:
            if (m_DlgMain.m_DlgEnemyFriend.m_iShowMode == 0)
            {
                m_DlgMain.m_DlgEnemyFriend.SetFriendList() ;
            }
            break ;
        case CMD_QUERYFRIEND_OK:
            if (m_DlgMain.m_DlgMsgReceive.m_bShow)
            {
                m_DlgMain.m_DlgMsgReceive.EnableWindow(false);
            }
            if (m_DlgMain.m_DlgMsgSend.m_bShow)
            {
                m_DlgMain.m_DlgMsgSend.EnableWindow(false);
            }
            m_DlgMain.m_DlgEnemyFriend.SetFriendState() ;
            break ;
        case CMD_QUERYFRIEND_TIMEOUT:
            this->MessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10110 ) ), g_objGameDataSet.GetStr ( 10117 ), MB_OK | MB_ICONERROR);
            //			this->MessageBox("网络延时\n取好友信息失败", "延时", MB_OK|MB_ICONERROR);
            break ;
        case CMD_FLASHTEAM:
            if (m_DlgMain.m_DlgQuery.m_btCurChildDlgID == 3)
            {
                m_DlgMain.m_DlgQuery.m_DlgSkill.SetTeamMagicSkill();
            }
            m_DlgMain.m_DlgGroup.SetGroupContain () ;
            if (lParam == 1)
            {
                m_DlgMain.m_DlgGroup.CreateTeamOk();
            }
            break ;
        case CMD_FLASH_TEAM_FACE:
            m_DlgMain.m_DlgGroup.SetGroupContain () ;
            break ;
        case CMD_TEAMINVITE:
            {
                char TeamPlayName[16] ;
                strcpy ( TeamPlayName, g_objHero.GetTeamApplyerName () ) ;
                m_DlgConfirm.SetConfirmDlg( 2, TeamPlayName ) ;
            }
            break ;
        case CMD_TEAMAPPLY:
            {
                char TeamPlayName[16] ;
                strcpy ( TeamPlayName, g_objHero.GetTeamApplyerName () ) ;
                m_DlgConfirm.SetConfirmDlg( 1, TeamPlayName ) ;
            }
            break ;
        case CMD_FLASHWEAPONSKILL:
            /*			m_DlgMain.m_DlgQuery.m_DlgWeapon.SetWeaponSkill ( ) ;*/
            break ;
        case CMD_CHANGEPKMODE:
            {
                m_DlgMain.m_DlgTalk.m_TalkPkChk.SetCheckValue( g_objHero.GetPkMode()) ;
                // Begin Get the record and set the slider
                char strAppName[32] ;
                sprintf ( strAppName, "%sRecord", g_objHero.GetName () ) ;
                // begin test the account name
                char* pstrTemp = strchr ( strAppName, '[' ) ;
                if ( pstrTemp != NULL )
                {
                    strncpy ( pstrTemp, "(", 1 ) ;
                    pstrTemp = NULL ;
                }
                pstrTemp = strchr ( strAppName, ']' ) ;
                if ( pstrTemp != NULL )
                {
                    strncpy ( pstrTemp, ")", 1 ) ;
                    pstrTemp = NULL ;
                }
                // end test the account name
                char strMode[16] ;
                itoa ( g_objHero.GetPkMode (), strMode, 10 ) ;
                WritePrivateProfileString ( strAppName, "PkModeRecord", strMode, m_strOwnerIni ) ;
            }
            break ;
        case CMD_SETPKMODE:
            {
                // Being set the owner ini file path
                sprintf ( m_strOwnerIni, "Log/%s/%s/Setup.ini", g_szServerName, g_objHero.GetName () ) ;
                // end set the owner ini file path
                // set the record
                // Begin Get the record and set the slider
                char strAppName[32] ;
                sprintf ( strAppName, "%sRecord", g_objHero.GetName () ) ;
                // begin test the account name
                char* pstrTemp = strchr ( strAppName, '[' ) ;
                if ( pstrTemp != NULL )
                {
                    strncpy ( pstrTemp, "(", 1 ) ;
                    pstrTemp = NULL ;
                }
                pstrTemp = strchr ( strAppName, ']' ) ;
                if ( pstrTemp != NULL )
                {
                    strncpy ( pstrTemp, ")", 1 ) ;
                    pstrTemp = NULL ;
                }
                // end test the account name
                int nLog = GetPrivateProfileInt ( strAppName, "LogRecord", -1, m_strOwnerIni ) ;
                if ( nLog == 0 )
                {
                    g_objGameMsg.m_objChatLog.LogOff () ;
                }
                else if ( nLog == 1 )
                {
                    g_objGameMsg.m_objChatLog.LogOn () ;
                    m_DlgLog.m_LogRecord.SetCheck ( 1 ) ;
                }
                // Get the hotkey record
                for ( int i = 0; i < 10; i++ )
                {
                    char strKeyName[32] ;
                    sprintf ( strKeyName, "HotKey%d", i) ;
                    m_nHotkeyValue[i] = GetPrivateProfileInt ( strAppName, strKeyName, -1, m_strOwnerIni ) ;
                }
                if (m_nHotkeyValue[0] == -1)
                {
                    m_nHotkeyValue[0] = 'Q';
                }
                if (m_nHotkeyValue[1] == -1)
                {
                    m_nHotkeyValue[1] = 'W';
                }
                if (m_nHotkeyValue[2] == -1)
                {
                    m_nHotkeyValue[2] = 'I';
                }
                if (m_nHotkeyValue[3] == -1)
                {
                    m_nHotkeyValue[3] = 'S';
                }
                if (m_nHotkeyValue[4] == -1)
                {
                    m_nHotkeyValue[4] = 'M';
                }
                if (m_nHotkeyValue[5] == -1)
                {
                    m_nHotkeyValue[5] = 'G';
                }
                if (m_nHotkeyValue[6] == -1)
                {
                    m_nHotkeyValue[6] = 'T';
                }
                if (m_nHotkeyValue[7] == -1)
                {
                    m_nHotkeyValue[7] = 'H' + 100 * HOTKEYF_CONTROL;
                }
                if (m_nHotkeyValue[8] == -1)
                {
                    m_nHotkeyValue[8] = 'A' ;
                }
                if (m_nHotkeyValue[9] == -1)
                {
                    m_nHotkeyValue[9] = 'B' ;
                }
                if (m_nHotkeyValue[10] == -1)
                {
                    m_nHotkeyValue[10] = 'P' ;
                }
                // Get the faction left word value
                m_nFacWordValue = GetPrivateProfileInt ( strAppName, "FacWordRecord", -1, m_strOwnerIni ) ;
                // End Get the record and set the slider
                m_DlgMain.SetRecord ( m_strOwnerIni ) ;
                CMyShellDlg::SetQUseItemNum () ;
            }
            break ;
        case CMD_LEVELUP:
            m_DlgMain.m_DlgQuery.m_DlgRole.SetRoleInfo () ;
            g_objHero.SetHelpTip(0, 0, 0, true);
            break ;
        case CMD_FLASHPK:
            m_DlgMain.m_DlgQuery.m_DlgRole.SetRoleInfo () ;
            break ;
        case CMD_TASKDIALOGOPEN:
            if (!m_DlgTask.m_bShow)
            {
                m_DlgMain.CloseCurDlg(1);
                m_DlgMain.CloseCurDlg(2);
                m_DlgTask.EnableWindow() ;
            }
            break ;
        case CMD_FLASH_MAGIC:
            if (!m_DlgMain.m_DlgQuery.m_DlgSkill)
            {
                break;
            }
            if (m_DlgMain.m_DlgQuery.m_btCurChildDlgID == 2)
            {
                m_DlgMain.m_DlgQuery.m_DlgSkill.SetMagicSkill() ;
            }
            else
            {
                m_DlgMain.m_DlgQuery.m_DlgSkill.SetTeamMagicSkill();
            }
            break ;
        case CMD_FLASH_MAGIC_EXP:
            if (m_DlgMain.m_DlgQuery.m_btCurChildDlgID == 2)
            {
                m_DlgMain.m_DlgQuery.m_DlgSkill.SetMagicSkill() ;
            }
            else
            {
                m_DlgMain.m_DlgQuery.m_DlgSkill.SetTeamMagicSkill();
            }
            break ;
        case CMD_FLASH_SYNDICATE:
            if (g_objHero.GetSydicateID() == ID_NONE)
            {
                m_DlgMain.m_DlgKnightage.EnableWindow(false);
            }
            else
            {
                m_DlgMain.m_DlgKnightage.FlashKnightage();
            }
            break ;
        case CMD_FLASH_SYNDICATE_WORD:
            m_DlgMain.m_DlgKnightage.m_DlgKnightageBillBoard.SetBillBoard();
            break ;
        case CMD_SYNDICATE_INVITE:
            {
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer(g_objHero.GetLastSyndicateApplyID ());
                if ( pPlayer)
                {
                    m_DlgConfirm.SetConfirmDlg( 4, pPlayer->GetName() ) ;
                }
            }
            break ;
        case CMD_SYNDICATE_APPLY:
            {
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer(g_objHero.GetLastSyndicateApplyID ());
                if ( pPlayer)
                {
                    m_DlgConfirm.SetConfirmDlg ( 3, pPlayer->GetName(),  g_objHero.GetLastSyndicateApplyID ()) ;
                }
            }
            break ;
        case CMD_START_MAGIC_BUSY:
            m_DlgMain.m_DlgProgress.m_PrgHeroMp.SetShowMode ( 1 ) ;
            break ;
        case CMD_END_MAGIC_BUSY:
            m_DlgMain.m_DlgProgress.m_PrgHeroMp.SetShowMode ( 0 ) ;
            break ;
        case CMD_XPFULL:
            if (g_objHero.GetXP() >= g_objHero.GetMaxXP()
                    && !g_objHero.IsDead()
                    && !(g_objGameMap.GetType () & MAPTYPE_BOOTH_ENABLE))
            {
                m_DlgMain.OnXpFull();
            }
            break ;
        case CMD_XPEMPTY:
            if (g_objHero.GetXP() == 0
                    && !g_objHero.TestStatus(USERSTATUS_GHOST)
                    && !m_DlgMain.m_DlgXp.m_btBoothType
                    && !m_DlgMain.m_DlgXp.m_bDie)
            {
                m_DlgMain.m_DlgXp.ShowWindow(SW_HIDE);
            }
            break ;
        case CMD_XPCHANGE:
            break ;
        case CMD_GHOST_BEGIN:
            m_DlgMain.HeroDie () ;
            break ;
        case CMD_GHOST_END:
            m_DlgMain.HeroReborn () ;
            break ;
        case CMD_REBORN:
            m_DlgMain.HeroReborn () ;
            break ;
        case CMD_FLASHCHATLOGFILELIST:
            m_DlgLog.SetLog ( 0 ) ;
            break ;
        case CMD_FLASHCURRENTCHATLOGLINELIST:
            if ( m_DlgLog.IsWindowVisible () )
            {
                m_DlgLog.SetLog ( 1 ) ;
            }
            else if ( m_DlgLog.m_bFirst )
            {
                m_DlgLog.SetLog ( 1 ) ;
                m_DlgLog.m_bFirst = false ;
            }
            break ;
        case CMD_FLASH_LEAVE_WORD:
            //			m_DlgMain.m_DlgMsgReceive.SetReceiveMsg () ;
            break ;
        case CMD_NEW_LEAVE_WORD:
            if ( !m_DlgMain.m_bLevRead )
            {
                m_DlgMain.m_bLevRead = true ;
            }
            break ;
        case CMD_FLASH_BOOTH_ITEM:
            m_DlgMain.m_DlgNpcBooth.SetNpcBooth () ;
            break ;
        case CMD_FLASH_BOOTHMANAGER_ITEM:
            m_DlgMain.m_DlgMpcBooth.SetMpcBooth () ;
            break ;
        case CMD_FLASH_BULLETIN_LIST:
            break ;
        case CMD_FLASH_BULLETIN_NOTE:
            break ;
        case CMD_OPEN_PLACE_STATUARY_DIALOG:
            break ;
        case CMD_OPEN_PLACE_NPC_INFO_DIALOG:
            m_uCmd = CMD_LAY ;
            break ;
        case CMD_OPEN_PLACE_NPC_MODIFY_DIALOG:
            break ;
        case CMD_COURT:
            m_uCmd = CMD_MPCCOURT ;
            break ;
        case CMD_BECOURT:
            {
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer ( lParam ) ;
                if ( pPlayer != NULL )
                {
                    m_DlgConfirm.SetConfirmDlg ( 14, pPlayer->GetName (), lParam ) ;
                }
            }
            break ;
        case CMD_OPEN_FLASH_SYN_MEMBER:
            if (m_DlgMain.m_DlgKnightage.m_DlgKnightageMemList.m_bShow)
            {
                m_DlgMain.m_DlgKnightage.m_DlgKnightageMemList.SetMemList() ;
            }
            break ;
        case CMD_OPEN_FLASH_SYN_MEMBER_INFO:
            m_DlgMain.m_DlgKnightage.m_DlgKnightageMemList.OpenFacState () ;
            break ;
        case CMD_OPEN_FLASH_SYNINFO:
            m_DlgMain.m_DlgKnightage.m_DlgKnightageEnemy.m_DlgKnightageState.SetStateData();
            break ;
        case CMD_CALL_PET_OUT:
            {
                const int MAX_PET = (const int)g_objHero.GetItemAmount(PACKAGE_PET);
                for(int i = 0; i < MAX_PET; i++)
                {
                    CItem* pPet = g_objHero.GetItemByIndex(i, PACKAGE_PET);
                    if (pPet)
                    {
                        if (g_objHero.GetPetByItemID(pPet->GetID()) != NULL)
                        {
                            continue;
                        }
                        m_DlgMain.m_DlgMonster.SetCurMonster(i);
                        m_DlgMain.m_DlgMonster.OnBtnCall();
                        m_DlgMain.m_DlgMonster.OnBtnCallout();
                    }
                }
            }
            break;
        case CMD_FLASH_MAGIC_STATUS:
            //			if (m_DlgMain.m_DlgXp.m_bShow)
            //				m_DlgMain.m_DlgXp.InitXpDialog();
            break ;
        case CMD_FLASH_OTHERPLAYER_EQUIPMENT:
            if (!m_DlgMain.m_DlgNpcEquip.m_bShow)
            {
                m_DlgMain.CloseCurDlg(2);
                m_DlgMain.m_uCurRDlgID = DLG_NPCEQUIP;
                m_DlgMain.m_DlgNpcEquip.EnableWindow(true);
            }
            m_DlgMain.m_DlgNpcEquip.SetNpcEquip();
            break ;
        case CMD_OPEN_ABORT_TRANSFORM_DLG:
            break ;
        case CMD_CLOSE_ABORT_TRANSFORM_DLG:
            break ;
        case CMD_DICE_JOIN:
            m_DlgMain.OpenDice ( true ) ;
            break ;
        case CMD_DICE_QUIT:
            m_DlgMain.OpenDice ( false ) ;
            break ;
        case CMD_DICE_FLASH_PLAYERLIST:
            m_DlgMain.m_DlgDiceList.SetDiceList () ;
            break ;
        case CMD_DICE_FLASH_ANTES:
            m_DlgMain.m_DlgDice.SetAntes () ;
            break ;
        case CMD_FLASHENEMYLIST:
            if (m_DlgMain.m_DlgEnemyFriend.m_iShowMode == 1)
            {
                m_DlgMain.m_DlgEnemyFriend.SetEnemyList () ;
            }
            break ;
        case CMD_QUERYENEMY_OK:
            if (m_DlgMain.m_DlgMsgReceive.m_bShow)
            {
                m_DlgMain.m_DlgMsgReceive.EnableWindow(false);
            }
            if (m_DlgMain.m_DlgMsgSend.m_bShow)
            {
                m_DlgMain.m_DlgMsgSend.EnableWindow(false);
            }
            m_DlgMain.m_DlgEnemyFriend.SetEnemyState () ;
            break ;
        case CMD_QUERYENEMY_TIMEOUT:
            this->MessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10399 ) ), g_objGameDataSet.GetStr ( 10117 ), MB_OK | MB_ICONERROR);
            break ;
        case CMD_RESET:
            m_DlgMain.CloseCurDlg ( 0 ) ;
            m_DlgMain.CloseCurDlg ( 1 ) ;
            m_DlgMain.CloseCurDlg ( 2 ) ;
            m_DlgMain.CloseCurDlg ( 3 ) ;
            if ( m_DlgConfirm.m_bShow )
            {
                m_DlgConfirm.EnableWindow( false ) ;
            }
            break ;
        case CMD_RESOURCE_LOSE:
            m_DlgMain.m_DlgSetup.OnResourceLose () ;
            m_DlgConfirm.SetConfirmDlg ( 21 ) ;
            break ;
        case CMD_ACTION_MESSAGEBOX:
            m_DlgConfirm.SetConfirmDlg( ACTION_CONFIRM, g_objHero.GetActionMessage() ) ;
            break ;
        case CMD_ACTION_IDENTBOX:
            m_DlgMain.OpenIdentifyDlg();
            break ;
        case CMD_FRIEND_APPLY:
            m_DlgConfirm.SetConfirmDlg( FRIENDADD_CONFIRM, g_objHero.GetFriendLastApplyInfo(), g_objHero.GetIdFriendLastApply() ) ;
            break ;
        case CMD_FRIEND_ACCEPT:
            m_DlgConfirm.SetConfirmDlg( FRIENDADDSUC_CONFIRM, g_objHero.GetFriendLastAcceptInfo() ) ;
            break ;
        case CMD_ACTION_SELECTEMBLEM:
            break ;
        default:
            break ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CMyShellDlg::OnMpcGoodsClose()
{
    m_DlgMain.m_DlgMpcGoods.EnableWindow( false ) ;
    if (m_bIconFix)
    {
        m_bIconFix = false;
    }
}

void CMyShellDlg::OnExit( UINT uType )
{
    if ( uType == 1 )
    {
        CMyShellDlg::RealExit() ;
    }
    if ( !m_bLogin )
    {
        if ( m_DlgLogin.m_bShow )
        {
            m_DlgLogin.EnableWindow( false ) ;
        }
        else if ( m_DlgRoleCreate.m_bShow )
        {
            m_DlgRoleCreate.ShowWindow(SW_HIDE);
            m_DlgRoleCreate.EnableWindow( false ) ;
        }
        if (m_DlgLog.IsWindowVisible())
        {
            m_DlgLog.ShowWindow(SW_HIDE);
        }
        m_DlgConfirm.SetConfirmDlg( EXIT_CONFIRM, NULL, 0 ) ;
    }
    else
    {
        if (m_DlgLog.IsWindowVisible())
        {
            m_DlgLog.ShowWindow(SW_HIDE);
        }
        if (m_DlgTask.IsWindowEnabled())
        {
            m_DlgTask.EnableWindow(false);
        }
        m_DlgMain.m_DlgMonster.ResetMonsterDlg ();
        m_DlgMain.m_DlgMonsterHeads.EnableWindow (false);
        m_DlgMain.WriteRecord();
        if ( m_DlgConfirm.m_bShow )
        {
            m_DlgConfirm.EnableWindow( false ) ;
            m_bExit = false ;
        }
        else
        {
            m_DlgConfirm.SetConfirmDlg( EXIT_CONFIRM, NULL, 0 ) ;
            m_bExit = true ;
        }
    }
}

void CMyShellDlg::RealExit()
{
    CMyShellDlg::KillTimer ( 1 ) ;
    if ( m_nScreenMode == 1 )
        ChangeDisplaySettings( &m_OldDeviceMode,
                               0 ) ;
    // Delete game font
    if ( m_pGameFont )
    {
        m_pGameFont->DeleteObject () ;
    }
    if ( m_hCursorH != NULL )
    {
        DeleteObject( m_hCursorH ) ;
    }
    if ( m_hCursorFriend )
    {
        DeleteObject( m_hCursorFriend ) ;
    }
    if ( m_hCursorPnt )
    {
        DeleteObject( m_hCursorPnt ) ;
    }
#ifndef _SOUND_DISABLE
    DXDestroy() ;
#endif
    gpDlgMain = NULL ;
    gpDlgShell = NULL ;
    if ( m_bLogin )
    {
        // Begin write the slider value
        char strAppName[32] ;
        sprintf ( strAppName, "%sRecord", g_objHero.GetName () ) ;
        // begin test the account name
        char* pstrTemp = strchr ( strAppName, '[' ) ;
        if ( pstrTemp != NULL )
        {
            strncpy ( pstrTemp, "(", 1 ) ;
            pstrTemp = NULL ;
        }
        pstrTemp = strchr ( strAppName, ']' ) ;
        if ( pstrTemp != NULL )
        {
            strncpy ( pstrTemp, ")", 1 ) ;
            pstrTemp = NULL ;
        }
        // end test the account name
        char strRecord[8] ;
        // write the log record
        itoa ( m_DlgLog.m_LogRecord.GetCheck (), strRecord, 10 ) ;
        WritePrivateProfileString ( strAppName, "LogRecord", strRecord, m_strOwnerIni ) ;
        // End write the slider value
        // Write the hotkey record
        for ( int i = 0; i < 11; i++ )
        {
            if ( m_nHotkeyValue[i] == -1 )
            {
                continue ;
            }
            char strKeyName[32], strValue[16]  ;
            sprintf ( strKeyName, "HotKey%d", i) ;
            itoa ( m_nHotkeyValue[i], strValue, 10 ) ;
            WritePrivateProfileString ( strAppName, strKeyName, strValue, m_strOwnerIni ) ;
        }
    }
    CMyShellDlg::EndDialog ( 0 ) ;
    ::PostQuitMessage( WM_QUIT ) ;
    return ;
}

void CMyShellDlg::DropMapItem(int nPosx, int nPosy, int nPackageType)
{
    if (m_DlgMain.m_DlgXp.m_btBoothType)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10727)); //摆摊状态下不能丢弃物品!
        CMyShellDlg::OnIconDrop ( DLG_ALL ) ;
        return;
    }
    CItem* pItem = g_objHero.GetItem ( m_IconRealID ) ;
    if ( pItem )
    {
        // Test if the item is importance
        if ( pItem->IsTaskItem ()
                || pItem->IsMonopolyItem ()
                || pItem->TestMonopolyItem(_MONOPOLY_DROPCONFIRM)
                || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetWarGhostLevel() >= 9)
                || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetAddition() >= 4)
                || (pItem->GetPackageType() == PACKAGE_BAG && pItem->GetGemAtkType() != 0))
        {
            m_nDropIndex = m_IconIndex ;
            m_DropPnt = CPoint ( nPosx, nPosy ) ;
            m_DlgConfirm.SetConfirmDlg ( 16 , NULL, pItem->GetID()) ;
        }
        else
        {
            if ( !g_objHero.DropItem ( g_objHero.GetItem( m_IconRealID )->GetID (), nPosx, nPosy	) )
            {
                if ( pItem->GetSort() == ITEMSORT_WEAPON_SINGLE_HAND ||
                        pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND )
                {
                    DXPlaySound ( "Sound/Drop_weapon.wav" ) ;
                }
                else if ( pItem->GetSort() == ITEMSORT_HELMET ||
                          pItem->GetSort() == ITEMSORT_SHOES ||
                          pItem->GetSort() == ITEMSORT_ARMOR )
                {
                    DXPlaySound ( "Sound/Drop_armor.wav" ) ;
                }
                else if	( pItem->GetSort() == ITEMSORT_RING ||
                          pItem->GetSort() == ITEMSORT_NECKLACE)
                {
                    DXPlaySound ( "Sound/Drop_adorn.wav" ) ;
                }
                else if ( pItem->GetSubType() == EXPEND_MEDICINE)
                {
                    DXPlaySound ( "Sound/Drop_drug.wav" );
                }
                else if ( pItem->GetTypeID() == ITEMTYPE_GHOSTGEM)
                {
                    DXPlaySound ( "Sound/Drop_stone.wav");
                }
                else if ( pItem->GetTypeID() == ITEMTYPE_SPRITE )
                {
                    DXPlaySound ( "Sound/Drop_egg.wav" );
                }
                CMyShellDlg::WindowProc ( WM_MY_MESSAGE, CMD_FLASHITEM, NULL ) ;
            }
        }
    }
    CMyShellDlg::OnIconDrop ( DLG_ALL ) ;
}

void CMyShellDlg::PickMapItem(OBJID ItemID, int nPosx, int nPosy)
{
    g_objHero.PickUpItem ( ItemID, nPosx, nPosy ) ;
}


void CMyShellDlg::ShowHelpDlg(LPARAM lParam)
{
    char strFileName[16] ;
    sprintf ( strFileName, "Help%d.htm", lParam ) ;
    CRect rect, Hrect ;
    CMyShellDlg::GetWindowRect ( rect ) ;
    m_DlgHelp.GetWindowRect ( Hrect ) ;
    m_DlgHelp.MoveWindow ( rect.left + ( rect.Width () - Hrect.Width () ) / 2,
                           rect.top + ( rect.Height () - Hrect.Height () ) / 2,
                           Hrect.Width (),
                           Hrect.Height () ) ;
    m_DlgHelp.OpenHelpDlg ( strFileName ) ;
}

void CMyShellDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMyShellDlg::SetQUseItemNum()
{
    for ( int i = 1; i <= GRID_ICONQUICK_NUM; i++ )
    {
        if ( m_DlgMain.m_DlgTalk.m_GoodBox.GetGridAble ( CPoint ( i, 1 ) ) )
        {
            if ( m_DlgMain.m_DlgTalk.m_GoodBox.GetIconType ( i - 1 ) != ICON_TYPE_QUSE )
            {
                continue ;
            }
            DWORD dwTypeID = m_DlgMain.m_DlgTalk.m_GoodBox.GetIconTypeID ( i - 1 ) ;
            int nItemNum = 0 ;
            int j = 0;
            for ( j = 0; j < g_objHero.GetItemAmount (); j++ )
            {
                CItem* pItem = g_objHero.GetItemByIndex( j ) ;
                if ( pItem != NULL && pItem->GetTypeID () == dwTypeID )
                {
                    if ( pItem->GetSort() == ITEMSORT_EXPEND )
                    {
                        nItemNum += pItem->GetAmount() ;
                    }
                    else
                    {
                        nItemNum++ ;
                    }
                }
            }
            for ( j = 0; j < g_objHero.GetItemAmount (PACKAGE_SOUL); j++ )
            {
                CItem* pItem = g_objHero.GetItemByIndex( j, PACKAGE_SOUL ) ;
                if ( pItem != NULL && pItem->GetTypeID () == dwTypeID )
                {
                    nItemNum += pItem->GetAmount() ;
                }
            }
            if ( nItemNum == 0 )
            {
                m_DlgMain.m_DlgTalk.m_GoodBox.RemoveIcon ( 1, i ) ;
            }
            else
            {
                m_DlgMain.m_DlgTalk.m_GoodBox.SetIconNum ( i - 1, nItemNum ) ;
            }
        }
    }
}

void CMyShellDlg::SetScreenMode(BYTE btType, BOOL bBegin)
{
    switch ( btType )
    {
    case 0:
        if ( m_nScreenMode == 2 || m_nScreenMode == 3 )
        {
            if ( CMyShellDlg::MessageBox ( g_objGameDataSet.GetStr ( 10112 ), g_objGameDataSet.GetStr ( 10116 ), MB_YESNO ) == IDYES )
                //			if ( CMyShellDlg::MessageBox ( "修改游戏分辨率必须重启游戏", "修改确定", MB_YESNO ) == IDYES )
            {
                WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "0", g_strSetupIni ) ;
                CMyShellDlg::OnExit( 1 ) ;
                return ;
            }
            else
            {
                return ;
            }
        }
        m_nScreenMode = btType ;
        if ( ( m_OldDeviceMode.dmBitsPerPel != 16 ||
                m_OldDeviceMode.dmPelsWidth != 800 ||
                m_OldDeviceMode.dmPelsHeight != 600 ) &&
                !bBegin )
            ChangeDisplaySettings( &m_OldDeviceMode,
                                   0) ;
        WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "0", g_strSetupIni ) ;
        break ;
    case 1:
        {
            if ( m_nScreenMode == 2 || m_nScreenMode == 3 )
            {
                if ( CMyShellDlg::MessageBox ( g_objGameDataSet.GetStr ( 10112 ), g_objGameDataSet.GetStr ( 10116 ), MB_YESNO ) == IDYES )
                    //			if ( CMyShellDlg::MessageBox ( "修改游戏分辨率必须重启游戏", "修改确定", MB_YESNO ) == IDYES )
                {
                    WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "1", g_strSetupIni ) ;
                    CMyShellDlg::OnExit( 1 ) ;
                    return ;
                }
                else
                {
                    return ;
                }
            }
            m_nScreenMode = btType ;
            EnumDisplaySettings( NULL,
                                 ENUM_CURRENT_SETTINGS,
                                 &m_OldDeviceMode ) ;
            if ( m_OldDeviceMode.dmBitsPerPel != 16 ||
                    m_OldDeviceMode.dmPelsWidth != 800 ||
                    m_OldDeviceMode.dmPelsHeight != 600 )
            {
                DEVMODE ChangeMode ;
                ChangeMode = m_OldDeviceMode ;
                ChangeMode.dmBitsPerPel = 16 ;
                ChangeMode.dmPelsWidth  = 800 ;
                ChangeMode.dmPelsHeight = 600 ;
                ChangeDisplaySettings( &ChangeMode,
                                       0 ) ;
            }
            WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "1", g_strSetupIni ) ;
        }
        break ;
    case 2:
        if ( m_nScreenMode == 0 || m_nScreenMode == 1 )
        {
            if ( m_nScreenMode == 1 )
            {
                if ( m_OldDeviceMode.dmPelsWidth < 1024 || m_OldDeviceMode.dmPelsHeight < 768 )
                {
                    AfxMessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10113 ) ) ) ; //"您桌面的当前分辨率小于你所修改的分辨率\n请先修改您的桌面分辨率" ) ;//
                    return ;
                }
            }
            else
            {
                DEVMODE CurrentMode ;
                EnumDisplaySettings( NULL,
                                     ENUM_CURRENT_SETTINGS,
                                     &CurrentMode ) ;
                if ( CurrentMode.dmPelsWidth < 1024 || CurrentMode.dmPelsHeight < 768 )
                {
                    AfxMessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10113 ) ) ) ; //"您桌面的当前分辨率小于你所修改的分辨率\n请先修改您的桌面分辨率" ) ;//
                    return ;
                }
            }
            if ( CMyShellDlg::MessageBox ( g_objGameDataSet.GetStr ( 10112 ), g_objGameDataSet.GetStr ( 10116 ), MB_YESNO ) == IDYES )
                //			if ( CMyShellDlg::MessageBox ( "修改游戏分辨率必须重启游戏", "修改确定", MB_YESNO ) == IDYES )
            {
                WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "2", g_strSetupIni ) ;
                CMyShellDlg::OnExit( 1 ) ;
                return ;
            }
            else
            {
                return ;
            }
        }
        DEVMODE CurrentMode ;
        EnumDisplaySettings( NULL,
                             ENUM_CURRENT_SETTINGS,
                             &CurrentMode ) ;
        if ( CurrentMode.dmPelsWidth < 1024 || CurrentMode.dmPelsHeight < 768 )
        {
            AfxMessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10113 ) ) ) ; //"您桌面的当前分辨率小于你所修改的分辨率\n请先修改您的桌面分辨率" ) ;//x
            _SCR_WIDTH = 800 ;
            _SCR_WIDTH = 600 ;
            return ;
        }
        m_nScreenMode = btType ;
        if ( ( m_OldDeviceMode.dmBitsPerPel != 16 ||
                m_OldDeviceMode.dmPelsWidth != 1024 ||
                m_OldDeviceMode.dmPelsHeight != 768 ) &&
                !bBegin )
            ChangeDisplaySettings( &m_OldDeviceMode,
                                   0) ;
        WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "2", g_strSetupIni ) ;
        break ;
    case 3:
        {
            if ( m_nScreenMode == 0 || m_nScreenMode == 1 )
            {
                if ( m_nScreenMode == 1 )
                {
                    if ( m_OldDeviceMode.dmPelsWidth < 1024 || m_OldDeviceMode.dmPelsHeight < 768 )
                    {
                        AfxMessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10113 ) ) ) ; //"您桌面的当前分辨率小于你所修改的分辨率\n请先修改您的桌面分辨率" ) ;//x
                        return ;
                    }
                }
                else
                {
                    DEVMODE CurrentMode ;
                    EnumDisplaySettings( NULL,
                                         ENUM_CURRENT_SETTINGS,
                                         &CurrentMode ) ;
                    if ( CurrentMode.dmPelsWidth < 1024 || CurrentMode.dmPelsHeight < 768 )
                    {
                        AfxMessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10113 ) ) ) ; //"您桌面的当前分辨率小于你所修改的分辨率\n请先修改您的桌面分辨率" ) ;//
                        return ;
                    }
                }
                if ( CMyShellDlg::MessageBox ( g_objGameDataSet.GetStr ( 10112 ), g_objGameDataSet.GetStr ( 10116 ), MB_YESNO ) == IDYES )
                    //			if ( CMyShellDlg::MessageBox ( "修改游戏分辨率必须重启游戏", "修改确定", MB_YESNO ) == IDYES )
                {
                    WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "3", g_strSetupIni ) ;
                    CMyShellDlg::OnExit( 1 ) ;
                    return ;
                }
                else
                {
                    return ;
                }
            }
            DEVMODE CurrentMode ;
            EnumDisplaySettings( NULL,
                                 ENUM_CURRENT_SETTINGS,
                                 &CurrentMode ) ;
            if ( CurrentMode.dmPelsWidth < 1024 || CurrentMode.dmPelsHeight < 768 )
            {
                AfxMessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10113 ) ) ) ; //"您桌面的当前分辨率小于你所修改的分辨率\n请先修改您的桌面分辨率" ) ;//
                _SCR_WIDTH = 800 ;
                _SCR_WIDTH = 600 ;
                return ;
            }
            m_nScreenMode = btType ;
            EnumDisplaySettings( NULL,
                                 ENUM_CURRENT_SETTINGS,
                                 &m_OldDeviceMode ) ;
            if ( m_OldDeviceMode.dmBitsPerPel != 16 ||
                    m_OldDeviceMode.dmPelsWidth != 1024 ||
                    m_OldDeviceMode.dmPelsHeight != 768 )
            {
                DEVMODE ChangeMode ;
                ChangeMode = m_OldDeviceMode ;
                ChangeMode.dmBitsPerPel = 16 ;
                ChangeMode.dmPelsWidth  = 1024 ;
                ChangeMode.dmPelsHeight = 768 ;
                ChangeDisplaySettings( &ChangeMode,
                                       0 ) ;
            }
            WritePrivateProfileString ( "ScreenMode", "ScreenModeRecord", "3", g_strSetupIni ) ;
        }
        break ;
    }
}

void CMyShellDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_DlgMain.m_DlgEmotion.m_bShow)
    {
        m_DlgMain.m_DlgEmotion.EnableWindow (false);
    }
    CMyShellDlg::SetFocus();
    if ( g_bPickUp )
    {
        g_bPickUp = false;
        if (m_PickUpDlgID == DLG_NPCBUY)
        {
            m_DlgMain.m_DlgNpcBuy.m_NpcBuySaleGri.ReSumePickUp();
        }
        else if (m_PickUpDlgID == DLG_DEPOT)
        {
            m_DlgMain.m_DlgDepot.m_DepotGriLay.ReSumePickUp();
        }
        else if (m_PickUpDlgID == DLG_MONSTERDEPOT)
        {
            m_DlgMain.m_DlgMonsterDepot.m_GridMonster.ReSumePickUp();
        }
        else if (m_PickUpDlgID == DLG_AUCPACKAGE)
        {
            m_DlgMain.m_DlgAucPackage.m_GridAucitem.ReSumePickUp();
        }
        else
        {
            CMyShellDlg::WindowProc ( WM_MY_MESSAGE, CMD_FLASHITEM, NULL ) ;
        }
        return;
    }
    extern void	MouseSet (int x, int y, int event);
    MouseSet(point.x, point.y, 1);
    if (_STATUS_WAITING == g_uStatus)
    {
        return;
    }
    // cancel the mouse state
    if ( m_uCmd == CMD_LAY )
    {
        g_objHero.DelMousePlayer () ;
    }
    if ( m_bIconFix )
    {
        m_bIconFix = false ;
        return ;
    }
    if ( m_uCmd != CMD_NORMAL )
    {
        m_uCmd = CMD_NORMAL ;
        return ;
    }
    // if hero died return
    if ( g_objHero.IsDead () )
    {
        return ;
    }
    if ( m_bLbuttonHold )
    {
        m_bLbuttonHold = false ;
    }
    if (m_bRbuttonHold)
    {
        m_bRbuttonHold = false ;
    }
	if (g_objHero.IsMagicAttackBusy())
	{
		return;
	}
	const int RIGHT_MAGIC = 7;//F8
	gpDlgShell->AttackMagic(RIGHT_MAGIC, false);
	//屏蔽 右键现在用来释放技能 F8
	/* if ( m_bRbuttonDown == false)
	{
	m_bRbuttonDown = true;
	}
	if (m_bRbuttonDown)
	{
	if (g_objHero.IsMagicAttackBusy())
	{
	return;
	}
	const int RIGHT_MAGIC = 7;
	gpDlgShell->AttackMagic(RIGHT_MAGIC, false);
	int nPosX, nPosY;
	g_objGameMap.Mouse2Cell(point.x, point.y, nPosX, nPosY);
	if (g_objHero.GetCommandType() == _COMMAND_RUN)
	{
	CMyPos posTarget = {nPosX, nPosY};
	g_objHero.AddStep(posTarget);
	}
	else
	{
	g_objHero.Run(nPosX, nPosY);
	}
	CMyShellDlg::SetMouseMove(nPosX, nPosY);
	if (m_DlgTask.m_bShow)
	{
	m_DlgTask.EnableWindow(false);
	}

	}*/
 
    CDialog::OnRButtonDown(nFlags, point);
}

void CMyShellDlg::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
}

void CMyShellDlg::MoveDlg()
{
    CPoint MousePnt ;
    CRect rect ;
   
    ::GetCursorPos( &MousePnt ) ;
    if ( !rect.PtInRect( MousePnt ) )
    {
        m_bDlgMove = false ;
        return ;
    }
  
}

void CMyShellDlg::MoveControl()
{
}

BOOL CMyShellDlg::ProcessHotkey(int nKey)
{
    int nMod = 0 ;
    BYTE btState[256] ;
    GetKeyboardState ( btState ) ;
    if ( btState[VK_MENU] >> 7 )
    {
        nMod += HOTKEYF_ALT ;
    }
    if ( btState[VK_SHIFT] >> 7 )
    {
        nMod += HOTKEYF_SHIFT ;
    }
    if ( btState[VK_CONTROL] >> 7 )
    {
        nMod += HOTKEYF_CONTROL ;
    }
    //	if ( nMod == 0 )
    //		return false;
    int nIndex = 0 ;
    for ( int i = 0; i < 11; i++ )
    {
        if ( m_nHotkeyValue[i] == nMod * 100 + nKey )
        {
            nIndex = i + 1;
            break ;
        }
    }
    if ( nIndex != 0 )
    {
        switch ( nIndex )
        {
        case 1:	//人物属性
            m_DlgMain.OnMainQuery () ;
            break ;
        case 2: //人物装备
            m_DlgMain.OnMainEquip() ;
            break ;
        case 3: //物品栏
            m_DlgMain.OnMainGood () ;
            break ;
        case 4: //系统设置
            m_DlgMain.OnMainSetup () ;
            break ;
        case 5: //大地图
            m_DlgMain.m_DlgMipmap.OpenBigmap();
            break ;
        case 6: //魔魂宝石
            m_DlgMain.OnMainStones();
            break;
        case 7: //技能
            m_DlgMain.OnMainMagic();
            break;
        case 8: //PK模式
            m_DlgMain.m_DlgTalk.ChangePkMode();
            break ;
        case 9:  //帮派
            m_DlgMain.OnMainFaction();
            break;
        case 10: //交易
            m_DlgMain.OnMainChat();
            break;
        case 11: //幻兽
            m_DlgMain.OnMainMonster();
            break;
        default:
            return false ;
        }
        return true ;
    }
    else
    {
        return false ;
    }
}

BOOL CMyShellDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if ( m_bLogin )
    {
        if ( zDelta > 0 )
            // big
        {
            if ( m_dwScale > 128 )
            {
                m_dwScale -= 32 ;
                g_objGameMap.SetScale(m_dwScale);
            }
        }
        // small
        else
        {
            if ( m_dwScale < 256 )
            {
                m_dwScale += 32 ;
                g_objGameMap.SetScale(m_dwScale);
            }
        }
    }
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyShellDlg::OpenInsHelpDlg()
{
    if (!m_DlgMain.m_DlgInsHelp.m_bShow)
    {
        m_DlgMain.CloseCurDlg(2);
        m_DlgMain.CloseCurDlg(1);
        m_DlgMain.m_uCurMDlgID = DLG_INSHELP;
        m_DlgMain.m_DlgInsHelp.EnableWindow(true) ;
    }
    else
    {
        m_DlgMain.m_DlgInsHelp.EnableWindow(false);
        m_DlgMain.m_uCurMDlgID = -1;
    }
}

void CMyShellDlg::ProcessMouseHold()
{
    //	if ( !m_bLbuttonHold  || !m_bRbuttonHold )
    //	{
    //		if ( m_dwLastBtnDTime == 0 )
    //		{
    //			m_dwLastBtnDTime = TimeGet() ;
    //		}
    //
    //		if ( TimeGet() - m_dwLastBtnDTime > 3000 )
    //		{
    //			m_dwLastBtnDTime = 0 ;
    //			if (m_bLbuttonDown)
    //			{
    //				m_bLbuttonHold = true ;
    //			}
    //			if (m_bRbuttonDown)
    //			{
    //				m_bRbuttonHold = true ;
    //			}
    //		}
    //	}
    //	if ( !g_objHero.IsMoving() )
    {
        CPoint	MousePnt;
        CRect	rect;
        CMyShellDlg::GetWindowRect( rect ) ;
        ::GetCursorPos( &MousePnt ) ;
        MousePnt.x -= rect.left;
        MousePnt.y -= rect.top;
        int nPosX, nPosY;
        extern void	MouseSet (int x, int y, int event);
        MouseSet(MousePnt.x, MousePnt.y, 1);
        g_objGameMap.Mouse2Cell(MousePnt.x, MousePnt.y, nPosX, nPosY);
        BOOL bRun = true;
		//左键用来操作角色移动- 2016.10.19
		/*       if (!m_bRbuttonDown )
		{
		bRun = false;
		}*/
        if (g_objHero.TestStatus(USERSTATUS_GHOST))
        {
            bRun = false;
        }
        CMyPos posTarget = {nPosX, nPosY};
        if (bRun)
        {
            if (g_objHero.GetCommandType() == _COMMAND_RUN)
            {
                g_objHero.AddStep(posTarget);
            }
            else
            {
                g_objHero.Run(nPosX, nPosY);
            }
        }
        else
        {
            if (g_objHero.GetCommandType() == _COMMAND_WALK)
            {
                g_objHero.AddStep(posTarget);
            }
            else
            {
                g_objHero.Walk(nPosX, nPosY);
            }
        }
    }
}


void CMyShellDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    //set rbutton status
    m_bRbuttonDown = false;
    m_bLbuttonDown = false ;
    m_dwLastBtnDTime = 0 ;
    // Set auto state false
    m_bAutoMagicAtk = false ;
    CDialog::OnRButtonUp(nFlags, point);
}


void CMyShellDlg::SetChatArea()
{
    /*	if (m_DlgMain.m_DlgTalk.GetIsGameMsgEx())
    	{
    	int nUp=g_objGameMsg.GetNorUpAreaTop();
    	int nDown=g_objGameMsg.GetNorDownAreaTop();
    	m_ChatSet1Rect = CRect(0,nUp-20,20,nUp);
    	m_ChatSet2Rect = CRect(0,nDown-20,20,nDown);
    	}
    	else
    	{
    		m_ChatSet1Rect = CRect(0,g_objGameMsg.GetNorAeroTop()-20,20,g_objGameMsg.GetNorAeroTop());
    	}*/
}

void CMyShellDlg::ReSetShowLines(bool bFlagEx)
{
    /*	int nFontSize = CMyBitmap::GetFontSize() + 2;
    	if (!bFlagEx)
    	{
    		int tLine = (650 - m_ChatSet1Rect.bottom)/nFontSize;
    		g_objGameMsg.SetShowLines(tLine);
    	}
    	else
    	{
    		int tLine1,tLine2;
    		g_objGameMsg.GetShowLinesEx(tLine1, tLine2);
    		if (m_iChatSet1 == 1)
    		{
    			tLine1 = (g_objGameMsg.GetNorDownAreaTop() - m_ChatSet1Rect.bottom)/nFontSize - 1;
    		}
    		if (m_iChatSet2 == 1)
    		{
    			tLine2 = (650 - m_ChatSet2Rect.bottom)/nFontSize -1;
    		}
    		g_objGameMsg.SetShowLinesEx(tLine1, tLine2);
    	}
    	CMyShellDlg::SetChatArea();*/
}

void CMyShellDlg::OpenMipmap()
{
    BOOL bShow = m_DlgMain.m_DlgMipmap.m_bShow;
    m_DlgMain.m_DlgMipmap.EnableWindow(!bShow);
}

void CMyShellDlg::OpenBigmap()
{
    BOOL bShow = m_DlgMain.m_DlgBigmap.m_bShow;
    if (!bShow)
    {
        m_DlgMain.CloseCurDlg(0);
        m_DlgMain.CloseCurDlg(1);
        m_DlgMain.CloseCurDlg(2);
        m_DlgMain.CloseCurDlg(3);
    }
    m_DlgMain.m_DlgBigmap.EnableWindow(!bShow);
}

void CMyShellDlg::AttackMagic(int nHotKey, BOOL bFlag)
{
    //	static int nStatic = 0;
    if ( g_objHero.IsDead () )
    {
        return ;
    }
    if (m_MagicID != m_DlgMain.m_DlgQuery.m_DlgSkill.m_DlgSkillHotKey.m_HotKeyIdType[nHotKey])
    {
        m_MagicID = m_DlgMain.m_DlgQuery.m_DlgSkill.m_DlgSkillHotKey.m_HotKeyIdType[nHotKey];
        //		return;
    }
    BOOL bOperation = false;
    if ( m_MagicID )
    {
        // Begin use the magic
        CMagic* pMagic = g_objHero.GetMagic(m_MagicID);
        if (pMagic)
        {
            CPlayer* pTarget = NULL;
            if ( pMagic->TestTarget ( MAGIC_TARGET_TERRAIN ) )
            {
                CMyPos 	point;
                ::MouseCheck(point.x, point.y);
                CMyPos MagicPos ;
                g_objGameMap.Screen2Cell ( point.x, point.y, MagicPos.x, MagicPos.y ) ;
                if (!g_objHero.IsMiner())
                {
                    if ( !g_objHero.IsMagicAttackBusy() )
                    {
                        g_objHero.MagicAttack ( m_MagicID, MagicPos) ;
                    }
                    return ;
                }
            }
            else if ( pMagic->TestTarget ( MAGIC_TARGET_SELF ) )
            {
                pTarget	= g_objPlayerSet.GetFocusPlayer(PLAYER_INCLUDESELF);
            }
            else if ( pMagic->TestTarget ( MAGIC_TARGET_NONE ) )
            {
                pTarget = &g_objHero;
            }
            else
            {
                DWORD dwFlag = PLAYER_NORMAL;
                CONST DWORD XP_REBORN	= 2009;	//复活技能
                CONST DWORD XP_GETSOUL  = 3012; //收魂技能
                if (XP_REBORN == pMagic->GetIDType() || XP_GETSOUL == pMagic->GetIDType())
                {
                    dwFlag |= PLAYER_DEADONLY;
                }
                if (pMagic->m_infoMagicType.dwTarget == MAGIC_TARGET_BODY)
                {
                    dwFlag |= PLAYER_INCLUDEDEADMONSTER;
                }
                if (pMagic->m_infoMagicType.dwCrime == 1)
                {
                    dwFlag |= PLAYER_MONSTERPRI;
                }
                else
                {
                    dwFlag |= PLAYER_PLAYERPRI;
                }
                if (pTarget == NULL )
                {
                    pTarget	= g_objPlayerSet.GetFocusPlayer(dwFlag);
                }
            }
            OBJID idMember = m_DlgMain.m_DlgGroup.GetMouseInMember();
            if (idMember && pTarget == NULL)
            {
                pTarget = g_objPlayerSet.GetPlayer(idMember);
#ifdef _DEBUG
                static int nMagicAmount = 0;
                nMagicAmount++;
                char strMsg[64];
                sprintf(strMsg, "队友使用魔法%d!", nMagicAmount);
                g_objGameMsg.AddMsg(strMsg);
#endif
            }
            if ( pTarget )
            {
                // can attack ?
                if (pTarget->GetRoleType() !=  _ROLE_SHOPKEEPER_NPC &&
                        pTarget->GetRoleType() !=  _ROLE_TASK_NPC &&
                        pTarget->GetRoleType() !=  _ROLE_STORAGE_NPC &&
                        pTarget->GetRoleType() !=  _ROLE_TRUNCK_NPC )
                {
                    // Begin auto magic attack
                    if ( g_objHero.GetID() != pTarget->GetID () &&
                            pTarget->IsNpc () &&
                            !pMagic->TestTarget ( MAGIC_TARGET_SELF ) &&
                            !pMagic->TestTarget ( MAGIC_TARGET_NONE ) )
                    {
                        //m_bAutoMagicAtk = true ;
                        m_AutoMagicID = m_MagicID ;
                        m_AutoTargetID = pTarget->GetID () ;
                    }
                    bOperation = true;
                    if (bFlag && g_objHero.GetPkMode () != PKMODE_MONSTERONLY && !g_objHero.IsMagicAttackBusy() )
                    {
                        g_objHero.MagicAttack( m_MagicID, pTarget->GetID (), true ) ;
                    }
                    else if (!g_objHero.IsMagicAttackBusy())
                    {
                        g_objHero.MagicAttack( m_MagicID, pTarget->GetID () ) ;
                    }
                }
            }
        }
    }		// End use the magic
}

void CMyShellDlg::OnMonsterChoose(int nIndex)
{
    /*	CItem* pPet = g_objHero.GetItemByIndex(nIndex,PACKAGE_PET);
    	if (pPet)
    	{
    		if (m_DlgMain.m_DlgTrade.m_bShow)
    		{
    			g_objHero.AddHeroTradeItem(pPet);
    		}
    		else if (m_DlgMain.m_DlgEmbed.m_bShow )
    		{
    			m_DlgMain.m_DlgEmbed.InsertMonster(pPet->GetID());
    		}
    		else if (m_DlgMain.m_DlgMonsterDepot.m_bShow)
    		{
    			CItem* pMonster = g_objHero.GetItem(pPet->GetID());
    			g_objHero.CheckInItem(pPet->GetID());
    		}
    		else if (m_DlgMain.m_DlgMonsterImprove.m_bShow)
    		{
    			m_DlgMain.m_DlgMonsterImprove.InsertMonster(pPet->GetID());
    		}
    	}*/
}

void CMyShellDlg::OnIconUseGem(OBJID IdGem)
{
    CItem* pItem = g_objHero.GetItem(IdGem);
    if (pItem)
    {
        BOOL bUseSelf = pItem->TestTarget(TARGET_SELF);
        BOOL bUseUser = pItem->TestTarget(TARGET_USER);
        BOOL bUseNone = pItem->TestTarget(TARGET_NONE);
        if (( bUseSelf && bUseUser) || bUseNone)
        {
            g_objHero.UseItem(pItem->GetID());
        }
        else // if (pItem->TestTarget(TARGET_OTHERS) || pItem->TestTarget(TARGET_BODY))
        {
            m_uCmd = CMD_USEITEMTO;
            m_IconRealID = pItem->GetID();
        }
    }
}

void CMyShellDlg::ShowAniCursor(char* szCursorAni)
{
    if (!szCursorAni && strlen(szCursorAni) <= 0)
    {
        return;
    }
    CAni* pCursorAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni, szCursorAni, EXIGENCE_IMMEDIATE ) ;
    if ( pCursorAni )
    {
        CPoint mousePnt;
        CRect rect;
        ::GetCursorPos( &mousePnt );
        this->GetWindowRect( rect );
        mousePnt.x -= rect.left;
        mousePnt.y -= rect.top;
        int nFrame = (::TimeGet() / 200) % pCursorAni->GetFrameAmount();
        pCursorAni->Show ( nFrame,
                           mousePnt.x,
                           mousePnt.y);
    }
}

void CMyShellDlg::SetCursorAni(const char* szAni)
{
    strcpy(m_szCursorAni, szAni);
}


void CMyShellDlg::SetLoginEffectPos(BOOL bLogin)
{
    if (!bLogin)
    {
        m_DlgLogin.SetEffectPos(0, 348);
        return;
    }
    CMyPos pPos = {0, 348};
    CMyPos pPosWord, posBG;
    g_objGameMap.Screen2World(pPos.x, pPos.y, pPosWord.x, pPosWord.y);
    g_objGameMap.World2Bg(pPosWord.x, pPosWord.y, posBG.x, posBG.y);
    m_DlgLogin.SetEffectPos(posBG.x , posBG.y);
}

void CMyShellDlg::AttackMagicXp()
{
    //	static int nStatic = 0;
    if ( g_objHero.IsDead () )
    {
        return ;
    }
    for(int i = 0; i < g_objHero.GetXpSkillAmount(); i++)
    {
        CMagic* pXp = g_objHero.GetXpSkillByIndex(i);
        if (pXp && pXp->GetIDType() == m_XpID)
        {
            CPlayer* pTarget = NULL;
            if ( pXp->TestTarget ( MAGIC_TARGET_TERRAIN ) )
            {
                CMyPos 	point;
                ::MouseCheck(point.x, point.y);
                CMyPos MagicPos ;
                g_objGameMap.Screen2Cell ( point.x, point.y, MagicPos.x, MagicPos.y ) ;
                if (!g_objHero.IsMiner())
                {
                    if ( !g_objHero.IsMagicAttackBusy() )
                    {
                        g_objHero.MagicAttack ( m_XpID, MagicPos) ;
                    }
                    return ;
                }
            }
            else if ( pXp->TestTarget ( MAGIC_TARGET_SELF ) )
            {
                pTarget	= g_objPlayerSet.GetFocusPlayer(PLAYER_INCLUDESELF);
            }
            else if ( pXp->TestTarget ( MAGIC_TARGET_NONE ) )
            {
                pTarget = &g_objHero;
            }
            else
            {
                DWORD dwFlag = PLAYER_NORMAL;
                CONST DWORD XP_REBORN	= 2009;	//复活技能
                CONST DWORD XP_GETSOUL  = 3012; //收魂技能
                if (XP_REBORN == pXp->GetIDType() || XP_GETSOUL == pXp->GetIDType())
                {
                    dwFlag |= PLAYER_DEADONLY;
                }
                if (pXp->m_infoMagicType.dwTarget == MAGIC_TARGET_BODY)
                {
                    dwFlag |= PLAYER_INCLUDEDEADMONSTER;
                }
                if (pXp->m_infoMagicType.dwCrime == 1)
                {
                    dwFlag |= PLAYER_MONSTERPRI;
                }
                else
                {
                    dwFlag |= PLAYER_PLAYERPRI;
                }
                if (pTarget == NULL )
                {
                    pTarget	= g_objPlayerSet.GetFocusPlayer(dwFlag);
                }
            }
            OBJID idMember = m_DlgMain.m_DlgGroup.GetMouseInMember();
            if (idMember &&  pTarget == NULL)
            {
                pTarget = g_objPlayerSet.GetPlayer(idMember);
            }
            if ( pTarget )
            {
                // can attack ?
                if (pTarget->GetRoleType() !=  _ROLE_SHOPKEEPER_NPC &&
                        pTarget->GetRoleType() !=  _ROLE_TASK_NPC &&
                        pTarget->GetRoleType() !=  _ROLE_STORAGE_NPC &&
                        pTarget->GetRoleType() !=  _ROLE_TRUNCK_NPC )
                {
                    if (!g_objHero.IsMagicAttackBusy())
                    {
                        g_objHero.MagicAttack( m_XpID, pTarget->GetID () ) ;
                    }
                }
            }
            return;
        }
    }
}

void CMyShellDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    int nAmount = g_objHero.GetTeamMemberAmount();
    if (nAmount > 1)
    {
        for(int i = 0; i < nAmount; i++)
        {
            if (m_DlgMain.m_DlgGroup.m_ImgRect[i].PtInRect(point))
            {
                OBJID idPlayer = m_DlgMain.m_DlgGroup.GetGroupImgID(i);
                TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfoByID(idPlayer);
                if (pInfo != NULL)
                {
                    m_DlgMain.m_DlgTalk.m_TalkNameEdt.SetWindowText ( pInfo->szName ) ;
                    m_DlgMain.m_DlgTalk.SetTalkMode ( 5003 ) ;	//team talk
                    m_DlgMain.m_DlgTalk.m_TalkDataCob.SetFocus () ;
                    CDialog::OnLButtonDblClk(nFlags, point);
                    return;
                }
            }
        }
    }
    m_bLbuttonDown = true;
    CDialog::OnLButtonDblClk(nFlags, point);
}

void CMyShellDlg::SetUseItemEffect(int x, int y, OBJID id)
{
    m_EffectExItemPos.x = x;
    m_EffectExItemPos.y = y;
    m_EffectExItem.Clear();
    ItemEffectInfo* pInfo = g_objGameDataSet.GetItemEffectInfo(id);
    if (pInfo)
    {
        m_EffectExItem.Add((char*)pInfo->strItemUseEffect.c_str());
    }
}


void CMyShellDlg::SetMouseMove(int nPosX, int nPosY)
{
    CMyPos posWorld;
    g_objGameMap.Cell2World(nPosX, nPosY, posWorld.x, posWorld.y);
    int nCellX, nCellY;
    g_objGameMap.World2Cell(posWorld.x, posWorld.y, nCellX, nCellY);
    CellInfo* pCellInfo = g_objGameMap.GetCell(nCellX, nCellY);
    if (pCellInfo != NULL
            && g_objGameMap.GetGroundMask(pCellInfo) == 0)
    {
        C3DMapEffect* pEffect = C3DMapEffect::CreateNew(posWorld, "mouse");
        g_objGameMap.GetInteractiveLayer()->AddMapObj(pEffect);
    }
}

BOOL CMyShellDlg::CloseCurDlg()
{
    BOOL bClosed = true;
    if (m_DlgLog.IsWindowVisible())
    {
        m_DlgLog.ShowWindow(SW_HIDE);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgNpcBooth.m_bShow)
    {
        m_DlgMain.m_DlgNpcBooth.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgTrade.m_bShow)
    {
        m_DlgMain.m_DlgTrade.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgAnnounceInfo.m_bShow)
    {
        m_DlgMain.m_DlgAnnounceInfo.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgIdentify.m_bShow)
    {
        m_DlgMain.m_DlgIdentify.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgBidAuction.m_bShow)
    {
        m_DlgMain.m_DlgBidAuction.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgAddAucItem.m_bShow)
    {
        m_DlgMain.m_DlgAddAucItem.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgAucPackage.m_bShow)
    {
        m_DlgMain.m_DlgAucPackage.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgMonsterNpc.m_bShow)
    {
        m_DlgMain.m_DlgMonsterNpc.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgMonsterDepot.m_bShow)
    {
        m_DlgMain.m_DlgMonsterDepot.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgMonsterImprove.m_bShow)
    {
        m_DlgMain.m_DlgMonsterImprove.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgMonsterHatch.m_bShow)
    {
        m_DlgMain.m_DlgMonsterHatch.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgTaskUpItem.m_bShow)
    {
        m_DlgMain.m_DlgTaskUpItem.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgInsHelp.m_bShow)
    {
        m_DlgMain.m_DlgInsHelp.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgEmbed.m_bShow)
    {
        m_DlgMain.m_DlgEmbed.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgImprove.m_bShow)
    {
        m_DlgMain.m_DlgImprove.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgSetup.m_bShow)
    {
        m_DlgMain.m_DlgSetup.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgMsgSend.m_bShow)
    {
        m_DlgMain.m_DlgMsgSend.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgMsgReceive.m_bShow)
    {
        m_DlgMain.m_DlgMsgReceive.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgNpcEquip.m_bShow)
    {
        m_DlgMain.m_DlgNpcEquip.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgAnnounceUp.m_bShow)
    {
        m_DlgMain.m_DlgAnnounceUp.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgAnnounceList.m_bShow)
    {
        bClosed = false;
        m_DlgMain.m_DlgAnnounceList.EnableWindow(false);
    }
    if (m_DlgMain.m_DlgPlayerTaskView.m_bShow)
    {
        m_DlgMain.m_DlgPlayerTaskView.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgPlayerTaskUp.m_bShow)
    {
        m_DlgMain.m_DlgPlayerTaskUp.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgPlayerTaskList.m_bShow)
    {
        m_DlgMain.m_DlgPlayerTaskList.EnableWindow(false);
        bClosed = false;
    }
    if (m_DlgMain.m_DlgDepot.m_bShow)
    {
        m_DlgMain.m_DlgDepot.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgNpcBuy.m_bShow)
    {
        m_DlgMain.m_DlgNpcBuy.EnableWindow( false ) ;
        bClosed = false;
    }
    if ( m_DlgMain.m_DlgMpcGoods.m_bShow )
    {
        m_DlgMain.m_DlgMpcGoods.EnableWindow( false ) ;
        bClosed = false;
    }
    if ( m_DlgMain.m_DlgMonster.m_bShow)
    {
        m_DlgMain.m_DlgMonster.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgEnemyFriend.m_bShow)
    {
        m_DlgMain.m_DlgEnemyFriend.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgQuery.m_bShow)
    {
        m_DlgMain.m_DlgQuery.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgMain.m_DlgKnightage.m_bShow)
    {
        m_DlgMain.m_DlgKnightage.EnableWindow(false);
        bClosed = false;
    }
    if ( m_DlgMain.m_DlgGroup.IsWindowEnabled() )
    {
        m_DlgMain.m_DlgGroup.EnableWindow( false ) ;
        bClosed = false;
    }
    if (m_DlgTask.m_bShow)
    {
        m_DlgTask.EnableWindow(false);
        bClosed = false;
    }
    return bClosed;
}

int CMyShellDlg::ShowItemInfo(CItem* pItem, int x, int y, int& nMaxLen, BOOL bCanShow)
{
    if (pItem != NULL)
    {
        char Static[64] ;
        strcpy(Static, pItem->GetDesc());
        if (!Static || strlen(Static) == 0 || strcmp(Static, g_objGameDataSet.GetStr(100122)) == 0 )
        {
            return 0;
        }
        //------------------------------------------------------------
        if (nMaxLen <= 16)
        {
            if ((int)strlen(Static) > nMaxLen)
            {
                if (strlen(Static) >= 16)
                {
                    nMaxLen = 16;
                }
                else
                {
                    nMaxLen = strlen(Static);
                }
            }
        }
        int nDescLine = 1 ;
        int nCharIndex[10];
        nCharIndex[0] = 0;
        int nSingle = 0;
        int nLineNum = 0;
        int i = 0;
        for ( ; i <(int) strlen ( pItem->GetDesc () ); i++ )
        {
            if ( CMyBitmap::IsDBCSLeadByte( pItem->GetDesc(), i ) )
            {
                nLineNum += 2;
                i++;
            }
            else
            {
                nSingle++;
                nLineNum++;
            }
            if ( nLineNum >= nMaxLen )
            {
                //----------Begin the english version's process(jump line)----------//
                if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
                {
                    // Search the last word
                    for ( int j = i; j >= 0; j-- )
                    {
                        if ( pItem->GetDesc()[j] == ' ' )
                        {
                            i -= i - j;
                            break;
                        }
                    }
                }
                //----------End the english version's process(jump line)----------//
                if ( nSingle % 2 == 0 )
                {
                    nSingle = 0;
                    nLineNum = 0;
                    nCharIndex[nDescLine] = i + 1;
                    nDescLine++;
                }
                else
                {
                    i -= nLineNum - nMaxLen;
                    nCharIndex[nDescLine] = i + 1 - ( nLineNum - nMaxLen );
                    nSingle = 0;
                    nLineNum = 0;
                    nDescLine++;
                }
            }
        }
        if ( nSingle % 2 == 0 )
        {
            nCharIndex[nDescLine] = i + 1 ;
        }
        else
        {
            nCharIndex[nDescLine] = i ;
        }
        DWORD STRCOLOR_YELLOW = 0xffff00;
        int nPosX  = x;
        int nPosY  = y + 2;
        int nLineDes = 0;
        CMySize psizeFont;
        CMyBitmap::GetFontSize(psizeFont);
        char strTemp[32] = "" ;
        int nOffset = 0;
        for ( int i = 0; i < nDescLine; i++ )
        {
            strncpy ( strTemp, pItem->GetDesc () + nCharIndex[i], nCharIndex[i + 1] - nCharIndex[i] );
            strTemp[nCharIndex[i + 1] - nCharIndex[i]] = '\0';
            if (strlen(strTemp) > 0)
            {
                if (bCanShow)
                {
                    CMyBitmap::ShowString (nPosX,
                                           nPosY + nOffset,
                                           STRCOLOR_YELLOW,
                                           CMyShellApp::StringChange ( strTemp, 1 ) ) ;
                    nOffset += psizeFont.iHeight + 2;
                }
                nLineDes++;
            }
        }
        return nLineDes;
    }
    return 0;
}


void CMyShellDlg::ResetMusic(BOOL bActive)
{
    if (!bActive)
    {
        m_MusicVolume = DXGetMusicVolume ();
        m_SoundVolume = DXGetSoundVolume ();
        DXSetMusicVolume ( 0 ) ;
        DXSetSoundVolume ( 0 ) ;
    }
    else
    {
        DXSetMusicVolume ( m_MusicVolume ) ;
        DXSetSoundVolume ( m_SoundVolume ) ;
    }
}

void CMyShellDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    int nAmount = g_objHero.GetTeamMemberAmount();
    if (nAmount > 1)
    {
        for(int i = 0; i < nAmount; i++)
        {
            if (m_DlgMain.m_DlgGroup.m_ImgRect[i].PtInRect(point))
            {
                OBJID idPlayer = m_DlgMain.m_DlgGroup.GetGroupImgID(i);
                TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfoByID(idPlayer);
                if (pInfo != NULL)
                {
                    CDialog::OnRButtonDblClk(nFlags, point);
                    return;
                }
            }
        }
    }
   // m_bRbuttonDown = true;
    CDialog::OnRButtonDblClk(nFlags, point);
}


void CMyShellDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CDialog::OnActivate(nState, pWndOther, bMinimized);
    // TODO: Add your message handler code here
    if (nState == WA_INACTIVE && !bMinimized )
    {
        ResetMusic(false);
    }
    else if ( (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
              && !bMinimized)
    {
        ResetMusic(true);
    }
}

void CMyShellDlg::On91UMessage(WPARAM wParam, LPARAM lParam)
{
    /*
    	if (wParam == CMD_91U_LOGIN)
    		return;
    	else if (wParam == CMD_91U_SEARCHROOMBACK)
    	{
    		if (m_DlgMain.m_DlgTalk.m_RoomLg.m_bShow)
    			m_DlgMain.m_DlgTalk.m_RoomLg.SetListInfo();
    	}*/
}
