
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgLogin.h"

#include "allmsg.h"
#include "3dGameMap.h"
//#include "GamePlayerSet.h"
//#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _SOUND_DISABLE
#define DXPlaySound //
#endif

//#define  SHELL_91LOGIN
//#define BIG5

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin dialog


CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLogin::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgLogin)
    m_bLoad = false ;
    m_bShow = false ;
    m_nCurrentFocus = 1 ;
    m_nCurLoginMode = 0;
    m_nCurServerIn = -1;
    m_nCurPage = 0;
    m_nEffectPosX = 0;
    m_nEffectPosY = 0;
    //}}AFX_DATA_INIT
}


void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLogin)
    DDX_Control(pDX, IDC_SEVER_PRS5, m_PrsServer5);
    DDX_Control(pDX, IDC_SEVER_PRS4, m_PrsServer4);
    DDX_Control(pDX, IDC_SEVER_PRS3, m_PrsServer3);
    DDX_Control(pDX, IDC_SEVER_PRS2, m_PrsServer2);
    DDX_Control(pDX, IDC_SEVER_PRS1, m_PrsServer1);
    DDX_Control(pDX, IDC_LOGIN_BTN_CANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_BTN_RIGHTPAGE, m_BtnRightPage);
    DDX_Control(pDX, IDC_BTN_LEFTPAGE, m_BtnLeftPage);
    DDX_Control(pDX, IDC_BTN_BACK, m_BtnBack);
    DDX_Control(pDX, IDC_LOGIN_SERVER5, m_BtnServer5);
    DDX_Control(pDX, IDC_LOGIN_SERVER4, m_BtnServer4);
    DDX_Control(pDX, IDC_LOGIN_SERVER3, m_BtnServer3);
    DDX_Control(pDX, IDC_LOGIN_SERVER2, m_BtnServer2);
    DDX_Control(pDX, IDC_LOGIN_SERVER1, m_BtnServer1);
    DDX_Control(pDX, IDC_LOGIN_GROUP5, m_BtnGroup5);
    DDX_Control(pDX, IDC_LOGIN_GROUP4, m_BtnGroup4);
    DDX_Control(pDX, IDC_LOGIN_GROUP3, m_BtnGroup3);
    DDX_Control(pDX, IDC_LOGIN_GROUP2, m_BtnGroup2);
    DDX_Control(pDX, IDC_LOGIN_GROUP1, m_BtnGroup1);
    DDX_Control(pDX, IDC_LOGIN_GROUPIMG, m_ImgGroupBk);
    DDX_Control(pDX, IDC_LOGIN_EDTIMG, m_LoginEdtImg);
    DDX_Control(pDX, IDC_LOGIN_EDT_ACCOUNT, m_LoginAccountEdt);
    DDX_Control(pDX, IDC_LOGIN_EDT_PASSWORD, m_LoginPasswordEdt);
    DDX_Control(pDX, IDC_LOGIN_BTN_OK, m_LoginOkBtn);
    DDX_Control(pDX, IDC_LOGIN_BTN_EXIT, m_LoginExitBtn);
    DDX_Control(pDX, IDC_LOGIN_BTN_RECORD, m_LoginRecordBtn ) ;
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
    //{{AFX_MSG_MAP(CDlgLogin)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_LOGIN_BTN_OK, OnLoginBtnOk)
    ON_BN_CLICKED(IDC_LOGIN_BTN_EXIT, OnLoginBtnExit)
    ON_BN_CLICKED(IDC_LOGIN_BTN_RECORD, OnLoginBtnRecord)
    ON_EN_SETFOCUS(IDC_LOGIN_EDT_ACCOUNT, OnSetfocusLoginEdtAccount)
    ON_EN_SETFOCUS(IDC_LOGIN_EDT_PASSWORD, OnSetfocusLoginEdtPassword)
    ON_BN_CLICKED(IDC_LOGIN_SERVER1, OnLoginServer1)
    ON_BN_CLICKED(IDC_LOGIN_GROUP1, OnLoginGroup1)
    ON_BN_CLICKED(IDC_BTN_BACK, OnBtnBack)
    ON_BN_CLICKED(IDC_LOGIN_BTN_CANCEL, OnLoginBtnCancel)
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_LOGIN_GROUP2, OnLoginGroup2)
    ON_BN_CLICKED(IDC_LOGIN_GROUP3, OnLoginGroup3)
    ON_BN_CLICKED(IDC_LOGIN_GROUP4, OnLoginGroup4)
    ON_BN_CLICKED(IDC_LOGIN_GROUP5, OnLoginGroup5)
    ON_BN_CLICKED(IDC_LOGIN_SERVER2, OnLoginServer2)
    ON_BN_CLICKED(IDC_LOGIN_SERVER3, OnLoginServer3)
    ON_BN_CLICKED(IDC_LOGIN_SERVER4, OnLoginServer4)
    ON_BN_CLICKED(IDC_LOGIN_SERVER5, OnLoginServer5)
    ON_BN_CLICKED(IDC_BTN_LEFTPAGE, OnBtnLeftpage)
    ON_BN_CLICKED(IDC_BTN_RIGHTPAGE, OnBtnRightpage)
    //}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin message handler

BOOL CDlgLogin::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Init the edit
	this->InitGui();
    // Init the static
    int nGroupAmount = GetPrivateProfileInt ( "Header", "GroupAmount", 0, ".\\Server.dat" ) ;
    if ( nGroupAmount == 0 )
    {
        this->MessageBox( g_objGameDataSet.GetStr ( 10012 ) ) ;//"server.dat损坏或者打开失败, 请重新安装。", "错误", MB_OK|MB_ICONERROR);
        return false;
    }
 
    m_bLoad = true ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

void CDlgLogin::Show()
{
    if ( m_bLoad && m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Image1",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, 0, 0 );
        }
        else
        {
            return ;
        }
        //show effect cloud
        //		CMyPos pPos ={512,382};
        //		m_EffectExLogin.Process();
        //		m_EffectExLogin.Show(pPos);
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 1, 0, 0 );
        }
        else
        {
            return ;
        }
        CMyPos pPos;
        pPos.x = m_nEffectPosX;
        pPos.y = m_nEffectPosY;
        m_EffectExLoginN.Process();
        m_EffectExLoginN.Show(pPos);
        // Show the image
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 2, 0, 0 );
        }
        else
        {
            return ;
        }
        if (m_nCurLoginMode == 0)
        {
            //the group btn
            m_ImgGroupBk.Show(0, 0);
            m_BtnGroup1.Show(0, 0);
            m_BtnGroup2.Show(0, 0);
            m_BtnGroup3.Show(0, 0);
            m_BtnGroup4.Show(0, 0);
            m_BtnGroup5.Show(0, 0);
            m_BtnBack.Show(0, 0);
            m_BtnLeftPage.Show(0, 0);
            m_BtnRightPage.Show(0, 0);
            if (m_nCurServerIn == 5 || m_nCurServerIn == 6)
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                CMyBitmap::ShowBlock(m_PrgServer[m_nCurServerIn].left - FontSize.iWidth * 2,
                                     m_PrgServer[m_nCurServerIn].top - FontSize.iHeight,
                                     m_PrgServer[m_nCurServerIn].left + FontSize.iWidth * 2,
                                     m_PrgServer[m_nCurServerIn].top ,
                                     0xc2000000);
                CMyBitmap::ShowString(m_PrgServer[m_nCurServerIn].left - FontSize.iWidth * 2, m_PrgServer[m_nCurServerIn].top - FontSize.iHeight,
                                      0xffffff, g_objGameDataSet.GetStr(10032));
            }
        }
        else if (m_nCurLoginMode == 1)
        {
            //the server btn
            m_ImgGroupBk.Show(0, 0);
            m_BtnServer1.Show(0, 0);
            m_BtnServer2.Show(0, 0);
            m_BtnServer3.Show(0, 0);
            m_BtnServer4.Show(0, 0);
            m_BtnServer5.Show(0, 0);
            m_BtnBack.Show(0, 0);
            m_BtnLeftPage.Show(0, 0);
            m_BtnRightPage.Show(0, 0);
            if (m_nCurServerIn == 5 || m_nCurServerIn == 6)
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                CMyBitmap::ShowBlock(m_PrgServer[m_nCurServerIn].left - FontSize.iWidth * 2,
                                     m_PrgServer[m_nCurServerIn].top - FontSize.iHeight,
                                     m_PrgServer[m_nCurServerIn].left + FontSize.iWidth * 2,
                                     m_PrgServer[m_nCurServerIn].top,
                                     0xc2000000);
                CMyBitmap::ShowString(m_PrgServer[m_nCurServerIn].left - FontSize.iWidth * 2, m_PrgServer[m_nCurServerIn].top - FontSize.iHeight,
                                      0xffffff, g_objGameDataSet.GetStr(10033));
            }
            m_PrsServer1.ResetValue(0, 100);
            m_PrsServer2.ResetValue(0, 100);
            m_PrsServer3.ResetValue(0, 100);
            m_PrsServer4.ResetValue(0, 100);
            m_PrsServer5.ResetValue(0, 100);
            if (m_bServerActive[0])
            {
                m_PrsServer1.Show(0, 0, 50, 50);
            }
            if (m_bServerActive[1])
            {
                m_PrsServer2.Show(0, 0, 50, 50);
            }
            if (m_bServerActive[2])
            {
                m_PrsServer3.Show(0, 0, 50, 50);
            }
            if (m_bServerActive[3])
            {
                m_PrsServer4.Show(0, 0, 50, 50);
            }
            if (m_bServerActive[4])
            {
                m_PrsServer5.Show(0, 0, 50, 50);
            }
            //			if (m_nCurServerIn>=0 && m_bServerActive[m_nCurServerIn])
            //				ShowServerInfo(m_nCurServerIn);
        }
        else if (m_nCurLoginMode == 2)
        {
            /*			static int nTime = TimeGet() ;
            			if ( TimeGet() - nTime >= 100 && nTime != 0 && ::GetForegroundWindow () == CDlgLogin::GetParent ()->m_hWnd )
            			{
            				if ( m_LoginAccountEdt.GetWindowTextLength () > 0 )
            					m_LoginPasswordEdt.SetFocus () ;
            				else
            					m_LoginAccountEdt.SetFocus () ;
            				nTime = 0 ;
            			}*/
            m_LoginEdtImg.Show ( 0, 0 ) ;
            // Show the edit
            m_LoginAccountEdt.Show ( -1, -3  ) ;
            m_LoginPasswordEdt.Show (-1, -3) ;
            // Show the button
            m_LoginOkBtn.Show ( 0, 0 ) ;
            m_LoginRecordBtn.Show ( 0, 0 ) ;
            m_BtnCancel.Show( 0, 0);
        }
        m_LoginExitBtn.Show ( 0, 0 ) ;
    }
}

LRESULT CDlgLogin::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_SHOWWINDOW:
        m_bShow = wParam ;
        if ( wParam )
        {
            // Init the dxinput when dialog show
            //			m_GameInputStr.Init ( AfxGetInstanceHandle(), this->GetParent()->GetParent()->m_hWnd, 16 ) ;
            //Add 3deffect cloud
            m_EffectExLogin.Add("other13");
            m_EffectExLoginN.Add("other14");
        }
        else
        {
            m_GameInputStr.Destroy () ;
            m_EffectExLogin.Clear();
            m_EffectExLoginN.Clear();
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            if (m_nCurLoginMode == 2)
            {
                CDlgLogin::OnLoginBtnOk () ;
            }
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgLogin::OnLoginBtnExit () ;
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

bool CDlgLogin::LoginGame()
{
    CString strAccount;
    m_LoginAccountEdt.GetWindowText(strAccount);
    if (strAccount.GetLength() <= 0)
    {
        this->MessageBox ( g_objGameDataSet.GetStr ( 10013 ),	g_objGameDataSet.GetStr ( 10017 ), MB_OK | MB_ICONERROR ) ;
        return false;
    }
    CString strPsw;
    m_LoginPasswordEdt.GetWindowText(strPsw);
    if (strPsw.GetLength() <= 0)
    {
        this->MessageBox ( g_objGameDataSet.GetStr ( 10014 ),	g_objGameDataSet.GetStr ( 10017 ), MB_OK | MB_ICONERROR ) ;
        return false;
    }
#ifdef SHELL_91LOGIN
    int nLoginType = 5;
#ifdef BIG5
    strAccount += ".tw";
#endif
    extern int GameLoginServerEx(unsigned long uType, const char * pszAccount, const char * pszPsw, const char * pszServerIP, const char * pszServerName);
    GameLoginServerEx( nLoginType, (LPCTSTR )strAccount, LPCTSTR (strPsw), m_strServer, m_strName );
#else
    extern int GameLoginServer(const char * pszAccount, const char * pszPsw, const char * pszServerIP, const char * pszServerName);
	strcpy(m_strAccount,strAccount);
	strcpy(m_strPsw,strPsw);
	

    GameLoginServer((LPCTSTR )strAccount, LPCTSTR (strPsw), m_strServer, m_strName);
#endif
    extern CHAR g_szServerName[256];
    strcpy( g_szServerName, m_strName );
    return true;
}

void CDlgLogin::OnLoginBtnOk()
{
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (CDlgLogin::LoginGame())
    {
        CDlgLogin::ShowWindow ( SW_HIDE ) ;
    }
}

void CDlgLogin::OnLoginBtnExit()
{
    DXPlaySound ( "Sound/game_exit.wav" ) ;
    WPARAM wParam = ON_EXIT ;
    CDlgLogin::GetParent ()->PostMessage ( WM_MY_MESSAGE, wParam ) ;
}

/*void CDlgLogin::OnBtnHelp()
{
	CDlgLogin::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_LOGIN ) ;
}*/

void CDlgLogin::OnLoginBtnRecord()
{
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if ( CDlgLogin::MessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10015 ) ), g_objGameDataSet.GetStr ( 10016 ), MB_YESNO ) == IDYES )
        //	if ( CDlgLogin::MessageBox ( "\t您确定要纪录吗？\n在网吧这将影响到您帐号的安全性", "纪录确定", MB_YESNO ) == IDYES )
    {
        char strAccount[32] ;
        m_LoginAccountEdt.GetWindowText ( strAccount, 16 ) ;
        if ( strAccount[0] != 0 )
        {
            WritePrivateProfileString ( "Account", "AccountRecord", strAccount, g_strSetupIni ) ;
        }
    }
}

/*void CDlgLogin::OnLoginBtnNet()
{
	// TODO: Add your control notification handler code here
	DXPlaySound ( "Sound/BtnClick.wav" ) ;

//	::ShellExecute(m_hWnd, NULL, g_objGameDataSet.GetStr ( 10367 ), NULL, NULL, SW_SHOW );
}*/

void CDlgLogin::OnSetfocusLoginEdtAccount()
{
    char strText[32] = "" ;
    m_LoginAccountEdt.GetWindowText ( strText, 16 ) ;
    if ( strText != NULL )
    {
        m_GameInputStr.SetStr ( strText ) ;
    }
}

void CDlgLogin::OnSetfocusLoginEdtPassword()
{
    char strText[32] = "" ;
    m_LoginPasswordEdt.GetWindowText ( strText, 16 ) ;
    if ( strText != NULL )
    {
        m_GameInputStr.SetStr ( strText ) ;
    }
}

void CDlgLogin::ReInit()
{
    m_LoginEdtImg.Init ( 0, 0, "Image120", BG_IMAGE ) ;
    m_LoginPasswordEdt.Init ( 0, 0, NULL, 0x000000, 0xffffff ) ;
    m_LoginAccountEdt.Init ( 0, 0, NULL, 0x000000, 0xffffff  ) ;
    m_LoginOkBtn.Init ( 0, 0, "Button13" ) ;
    m_LoginExitBtn.Init ( 0, 0, "Button14" ) ;
    m_LoginRecordBtn.Init ( 0, 0, "Button12" ) ;
    m_b1024 = true ;
}


void CDlgLogin::OnLoginGroup1()
{
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::ResetServerStatus();
    CDlgLogin::LoginGroup(1);
    CDlgLogin::SetCurLoginMode(1);
}

void CDlgLogin::OnLoginServer1()
{
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::LoginServer(1);
}

void CDlgLogin::MoveAllPicAndWord()
{
    // Set the static null
    // remove the pics
}

void CDlgLogin::InsertServer(int nIndex, char* strPicID, char* strText, int nSerSta)
{
    m_bServerActive[nIndex-1] = TRUE;
    switch(nIndex)
    {
    case 1:
        m_BtnServer1.EnableWindow(true);
        m_BtnServer1.ChangeImage("Server1");
        break;
    case 2:
        m_BtnServer2.EnableWindow(true);
        m_BtnServer2.ChangeImage("Server2");
        break;
    case 3:
        m_BtnServer3.EnableWindow(true);
        m_BtnServer3.ChangeImage("Server3");
        break;
    case 4:
        m_BtnServer4.EnableWindow(true);
        m_BtnServer4.ChangeImage("Server4");
        break;
    case 5:
        m_BtnServer5.EnableWindow(true);
        m_BtnServer5.ChangeImage("Server5");
        break;
    default:
        break;
    }
}

void CDlgLogin::SetCurLoginMode(int nMode)
{
    DXSetSoundVolume(75);
    m_nCurLoginMode = nMode;
    BOOL bFlagGroup = false;
    BOOL bFlagServer = false;
    BOOL bFlagLogin = false;
    if (m_nCurLoginMode == 0)
    {
        bFlagGroup = true;
        m_BtnBack.EnableWindow(true);
        m_BtnLeftPage.EnableWindow(true);
        m_BtnRightPage.EnableWindow(true);
    }
    else if (m_nCurLoginMode == 1)
    {
        bFlagServer = true;
        m_BtnBack.EnableWindow(true);
        m_BtnLeftPage.EnableWindow(true);
        m_BtnRightPage.EnableWindow(true);
    }
    else if (m_nCurLoginMode == 2)
    {
        bFlagLogin = true;
        m_BtnBack.EnableWindow(false);
        m_BtnLeftPage.EnableWindow(false);
        m_BtnRightPage.EnableWindow(false);
        m_LoginAccountEdt.SetFocus();
        m_LoginAccountEdt.SetSel(0, 0);
    }
    m_LoginExitBtn.EnableWindow(true);
    CDlgLogin::SetGroupStatus(bFlagGroup);
    m_BtnServer1.EnableWindow(bFlagServer);
    m_BtnServer2.EnableWindow(bFlagServer);
    m_BtnServer3.EnableWindow(bFlagServer);
    m_BtnServer4.EnableWindow(bFlagServer);
    m_BtnServer5.EnableWindow(bFlagServer);
    m_PrsServer1.EnableWindow(bFlagServer);
    m_PrsServer2.EnableWindow(bFlagServer);
    m_PrsServer3.EnableWindow(bFlagServer);
    m_PrsServer4.EnableWindow(bFlagServer);
    m_PrsServer5.EnableWindow(bFlagServer);
    m_LoginAccountEdt.EnableWindow(bFlagLogin);
    m_LoginPasswordEdt.EnableWindow(bFlagLogin);
    m_LoginOkBtn.EnableWindow(bFlagLogin);
    m_LoginRecordBtn.EnableWindow(bFlagLogin);
    m_BtnCancel.EnableWindow(bFlagLogin);
    if (m_LoginAccountEdt.IsWindowEnabled())
    {
        m_LoginAccountEdt.SetFocus();
    }
}

void CDlgLogin::OnBtnBack()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_nCurLoginMode == 1)
    {
        CDlgLogin::SetCurLoginMode(0);
    }
    else if (m_nCurLoginMode == 0)
    {
        CDlgLogin::GetParent()->PostMessage(WM_MY_MESSAGE, ON_BACK_PLAY);
    }
}

void CDlgLogin::OnLoginBtnCancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    if (m_nCurLoginMode == 2)
    {
        CDlgLogin::SetCurLoginMode(1);
    }
}

void CDlgLogin::ShowServerInfo(int nServerIndex)
{
    char strInfo[64] = "";
    //	sprintf(strInfo,"Server%d",nServerIndex+1);
    CMyBitmap::ShowBlock(m_PrgServer[nServerIndex].left + 30,
                         m_PrgServer[nServerIndex].top + 10,
                         m_PrgServer[nServerIndex].left + 120,
                         m_PrgServer[nServerIndex].top + 36,
                         0xc2ffffff);
    CMyBitmap::ShowString(m_PrgServer[nServerIndex].left, m_PrgServer[nServerIndex].top,
                          0x000000, strInfo);
}

void CDlgLogin::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_nCurLoginMode == 1 || m_nCurLoginMode == 0)
    {
        for(int i = 0; i < 7; i++)
        {
            if (m_PrgServer[i].PtInRect(point))
            {
                m_nCurServerIn = i;
                CDialog::OnMouseMove(nFlags, point);
                return;
            }
            else
            {
                m_nCurServerIn = -1;
            }
        }
    }
    CDialog::OnMouseMove(nFlags, point);
}

void CDlgLogin::ResetServerStatus()
{
    for(int i = 0; i < 5; i++)
    {
        m_bServerActive[i] = false;
        m_BtnServer1.EnableWindow(false);
        m_BtnServer1.ChangeImage("ServerNone");
        m_BtnServer2.EnableWindow(false);
        m_BtnServer2.ChangeImage("ServerNone");
        m_BtnServer3.EnableWindow(false);
        m_BtnServer3.ChangeImage("ServerNone");
        m_BtnServer4.EnableWindow(false);
        m_BtnServer4.ChangeImage("ServerNone");
        m_BtnServer5.EnableWindow(false);
        m_BtnServer5.ChangeImage("ServerNone");
    }
}

void CDlgLogin::OnLoginGroup2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::ResetServerStatus();
    CDlgLogin::LoginGroup(2);
    CDlgLogin::SetCurLoginMode(1);
}

void CDlgLogin::OnLoginGroup3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::ResetServerStatus();
    CDlgLogin::LoginGroup(3);
    CDlgLogin::SetCurLoginMode(1);
}

void CDlgLogin::OnLoginGroup4()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::ResetServerStatus();
    CDlgLogin::LoginGroup(4);
    CDlgLogin::SetCurLoginMode(1);
}

void CDlgLogin::OnLoginGroup5()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::ResetServerStatus();
    CDlgLogin::LoginGroup(5);
    CDlgLogin::SetCurLoginMode(1);
}

void CDlgLogin::OnLoginServer2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::LoginServer(2);
}

void CDlgLogin::OnLoginServer3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::LoginServer(3);
}

void CDlgLogin::OnLoginServer4()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::LoginServer(4);
}

void CDlgLogin::OnLoginServer5()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgLogin::LoginServer(5);
}

void CDlgLogin::LoginGroup(int nIndex)
{
    CDlgLogin::ResetServerStatus();
    sprintf ( m_strGroup, "Group%d", nIndex ) ;
    strcpy ( m_strServer, "Ip1" ) ;
    int nServerAmount = GetPrivateProfileInt ( m_strGroup, "ServerAmount", 0, ".\\Server.dat" ) ;
    if ( nServerAmount == 0 )
    {
        this->MessageBox("server.dat损坏或者打开失败, 请重新安装。", "错误", MB_OK | MB_ICONERROR);
        return ;
    }
    char strPicID[16] ;
    char strText[32] ;
    char strKey[16] ;
    int nSerSta ;
    m_SerSta.FlashStatus () ;
    for ( int i = 1; i <= nServerAmount; i++ )
    {
        sprintf ( strKey, "Pic%d", i ) ;
        GetPrivateProfileString ( m_strGroup, strKey, "", strPicID, 16, ".\\Server.dat" ) ;
        sprintf ( strKey, "Server%d", i ) ;
        GetPrivateProfileString ( m_strGroup, strKey, "", strText, 16, ".\\Server.dat" ) ;
        nSerSta = 0;//m_SerSta.GetServerStatus ( strText ) ;
        CDlgLogin::InsertServer ( i, strPicID, strText, nSerSta ) ;
    }
    CDlgLogin::SetCurLoginMode(1);
}

void CDlgLogin::LoginServer(int nIndex)
{
    if (m_bServerActive[nIndex+m_nCurPage-1])
    {
        char strKey[16];
        sprintf(strKey, "Ip%d", nIndex + m_nCurPage);
        GetPrivateProfileString ( m_strGroup, strKey, "", m_strServer, 16, ".\\Server.dat" ) ;
        sprintf(strKey, "Server%d", nIndex + m_nCurPage);
        GetPrivateProfileString ( m_strGroup, strKey, "", m_strName, 16, ".\\Server.dat" ) ;
        CDlgLogin::SetCurLoginMode(2);
    }
}

void CDlgLogin::SetGroupStatus(BOOL bGroup)
{
    m_BtnGroup1.EnableWindow(false);
    m_BtnGroup1.ChangeImage("ServerNone");
    m_BtnGroup2.EnableWindow(false);
    m_BtnGroup2.ChangeImage("ServerNone");
    m_BtnGroup3.EnableWindow(false);
    m_BtnGroup3.ChangeImage("ServerNone");
    m_BtnGroup4.EnableWindow(false);
    m_BtnGroup4.ChangeImage("ServerNone");
    m_BtnGroup5.EnableWindow(false);
    m_BtnGroup5.ChangeImage("ServerNone");
    if (bGroup)
    {
        int nGroupAmount = GetPrivateProfileInt ( "Header", "GroupAmount", 0, ".\\Server.dat" ) ;
        if ( nGroupAmount <= 0 )
        {
            this->MessageBox((char*)g_objGameDataSet.GetStr(10012), g_objGameDataSet.GetStr ( 10017 ), MB_OK | MB_ICONERROR);
            return ;
        }
        else
        {
            for(int i = 0; i < nGroupAmount; i++)
            {
                switch(i)
                {
                case 0:
                    m_BtnGroup1.EnableWindow(true);
                    m_BtnGroup1.ChangeImage("Sgroup1");
                    break;
                case 1:
                    m_BtnGroup2.EnableWindow(true);
                    m_BtnGroup2.ChangeImage("Sgroup2");
                    break;
                case 2:
                    m_BtnGroup3.EnableWindow(true);
                    m_BtnGroup3.ChangeImage("Sgroup3");
                    break;
                case 3:
                    m_BtnGroup4.EnableWindow(true);
                    m_BtnGroup4.ChangeImage("Sgroup4");
                    break;
                case 4:
                    m_BtnGroup5.EnableWindow(true);
                    m_BtnGroup5.ChangeImage("Sgroup5");
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void CDlgLogin::SetEffectPos(int x, int y)
{
    m_nEffectPosX = x;
    m_nEffectPosY = y;
}

void CDlgLogin::OnBtnLeftpage()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/up_Page.wav" ) ;
}

void CDlgLogin::OnBtnRightpage()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/up_Page.wav" ) ;
}


void CDlgLogin::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	
	// TODO: 在此处添加消息处理程序代码
}

void CDlgLogin::InitGui()
{
	m_LoginPasswordEdt.Init ( 0, 0, false, NULL, 0x000000, 0xffffff );
    m_LoginPasswordEdt.SetLimitText ( 15 ) ;
    m_LoginPasswordEdt.SetPasswordType ( ) ;
    m_LoginPasswordEdt.SetOffset ( CPoint ( 4, 6 ) ) ;
#ifdef _DEBUG
    m_LoginPasswordEdt.SetWindowText ( "0000" ) ;
#endif
	
    m_LoginAccountEdt.Init (0, 0, NULL, 0x000000, 0xffffff);
    m_LoginAccountEdt.SetLimitText ( 15 ) ;
    m_LoginAccountEdt.SetOffset ( CPoint ( 4, 6 ) ) ;
    char strAccount[32] ;
    if ( GetPrivateProfileString ( "Account", "AccountRecord", "", strAccount, 16, g_strSetupIni ) != 0 )
    {
        m_LoginAccountEdt.SetWindowText ( strAccount ) ;
    }
    m_LoginAccountEdt.SetSel(0, 0);
    // Init the image
    m_LoginEdtImg.Init ( 0, 0, "Image120", BG_IMAGE ) ;
    m_ImgGroupBk.Init ( 0, 0, "Image100", BG_IMAGE ) ;
    // Init the button
    m_LoginOkBtn.Init ( 0, 0, "Button13" ) ;
    m_LoginExitBtn.Init ( 0, 0, "Button14" ) ;
    m_LoginRecordBtn.Init ( 0, 0, "Button12" ) ;
    m_BtnCancel.Init(0, 0, "Button7");
    // The groups
    m_BtnGroup1.Init(0, 0, "Sgroup1");
    m_BtnGroup2.Init(0, 0, "Sgroup2");
    m_BtnGroup3.Init(0, 0, "Sgroup3");
    m_BtnGroup4.Init(0, 0, "Sgroup4");
    m_BtnGroup5.Init(0, 0, "Sgroup5");
    //the server
    m_BtnServer1.Init(0, 0, "Server1");
    m_BtnServer2.Init(0, 0, "Server2");
    m_BtnServer3.Init(0, 0, "Server3");
    m_BtnServer4.Init(0, 0, "Server4");
    m_BtnServer5.Init(0, 0, "Server5");
    m_PrsServer1.Init(0, 0, "Progress600", 0, 100, MY_PROGRESS_HLEFT);
    m_PrsServer1.GetWindowRect(m_PrgServer[0]);
    m_PrsServer2.Init(0, 0, "Progress600", 0, 100, MY_PROGRESS_HLEFT);
    m_PrsServer2.GetWindowRect(m_PrgServer[1]);
    m_PrsServer3.Init(0, 0, "Progress600", 0, 100, MY_PROGRESS_HLEFT);
    m_PrsServer3.GetWindowRect(m_PrgServer[2]);
    m_PrsServer4.Init(0, 0, "Progress600", 0, 100, MY_PROGRESS_HLEFT);
    m_PrsServer4.GetWindowRect(m_PrgServer[3]);
    m_PrsServer5.Init(0, 0, "Progress600", 0, 100, MY_PROGRESS_HLEFT);
    m_PrsServer5.GetWindowRect(m_PrgServer[4]);
    m_BtnBack.Init(0, 0, "Button17");
    m_BtnLeftPage.Init(0, 0, "Button18");
    m_BtnLeftPage.GetWindowRect(m_PrgServer[5]);
    m_BtnRightPage.Init(0, 0, "Button19");
    m_BtnRightPage.GetWindowRect(m_PrgServer[6]);

}
