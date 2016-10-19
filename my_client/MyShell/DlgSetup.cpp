
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgSetup.h"

#include "hero.h"
#include "NetMsg.h"
#include "Role.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _SOUND_DISABLE
#define DXPlaySound //
#define DXSetSoundVolume //
#define DXSetMusicVolume //
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup dialog


CDlgSetup::CDlgSetup(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSetup::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetup)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_nScreenMode = 0 ;
    m_nInterface = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgSetup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetup)
    DDX_Control(pDX, IDC_SETUP_BTN_RELOGIN, m_SetupBtnRelogin);
    DDX_Control(pDX, IDC_SETUP_BTN_OTHER, m_SetupBtnChat);
    DDX_Control(pDX, IDC_SETUP_BTN_MUSIC, m_SetupBtnMusic);
    DDX_Control(pDX, IDC_SETUP_BTN_RETURN, m_SetupBtnReturn);
    DDX_Control(pDX, IDC_SETUP_BTN_GRAPH, m_SetupBtnGraph);
    DDX_Control(pDX, IDC_SETUP_BTN_HOTKEY, m_SetupBtnHotkey);
    DDX_Control(pDX, IDC_SETUP_BTN_EXIT, m_SetupBtnExit);
    DDX_Control(pDX, IDC_SETUP_BTN_DELETE, m_SetupBtnDelete);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup, CDialog)
    //{{AFX_MSG_MAP(CDlgSetup)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_SETUP_BTN_EXIT, OnSetupBtnExit)
    ON_BN_CLICKED(IDC_SETUP_BTN_DELETE, OnSetupBtnDelete)
    ON_BN_CLICKED(IDC_SETUP_BTN_HOTKEY, OnSetupBtnHotkey)
    ON_BN_CLICKED(IDC_SETUP_BTN_OTHER, OnSetupBtnOther)
    ON_BN_CLICKED(IDC_SETUP_BTN_MUSIC, OnSetupBtnMusic)
    ON_BN_CLICKED(IDC_SETUP_BTN_RETURN, OnSetupBtnReturn)
    ON_BN_CLICKED(IDC_SETUP_BTN_GRAPH, OnSetupBtnGraph)
    ON_BN_CLICKED(IDC_SETUP_BTN_RELOGIN, OnSetupBtnRelogin)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup message handlers

LRESULT CDlgSetup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        if ( wParam == false )
        {
            m_bShow = false ;
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
        else
        {
            m_bShow = true ;
            DXPlaySound ( "Sound/Open.wav" ) ;
            ProcessCheck(0);
            m_SetupBtnGraph.SetCheckValue(1);
            SetScreenMode(m_nScreenMode);
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            //			CDlgSetup::OnSetupBtnReturn () ;
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgSetup::OnSetupBtnExit () ;
            return true ;
        }
    }
    else if ( message == WM_HSCROLL )
    {
        //	DXSetMusicVolume ( m_SetupSliMusic.GetSliderValue ( ) ) ;
        //	DXSetSoundVolume ( m_SetupSliEffect.GetSliderValue ( ) ) ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgSetup::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Test if the ani already download
    CRect rect ;
    CDlgSetup::GetWindowRect ( rect ) ;
    //Init the setup dialogs
    m_DlgGraph.Create(IDD_DIALOG_SETUP_GRAPH, this);
    m_DlgGraph.ShowWindow(SW_SHOW);
    m_DlgGraph.EnableWindow(false);
    m_DlgMusic.Create(IDD_DIALOG_SETUP_MUSIC, this);
    m_DlgMusic.ShowWindow(SW_SHOW);
    m_DlgMusic.EnableWindow(false);
    m_DlgSChat.Create(IDD_DIALOG_SETUP_CHAT, this);
    m_DlgSChat.ShowWindow(SW_SHOW);
    m_DlgSChat.EnableWindow(false);
    m_DlgHotkey.Create(IDD_DIALOG_SETUP_HOTKEY, this);
    m_DlgHotkey.ShowWindow(SW_SHOW);
    m_DlgHotkey.EnableWindow(false);
    // Init the button
    m_SetupBtnExit.Init ( rect.left, rect.top, "Button103" );
    m_SetupBtnDelete.Init ( rect.left, rect.top, "Button102" );
    m_SetupBtnReturn.Init ( rect.left, rect.top, "Button540" );
    m_SetupBtnRelogin.Init( rect.left, rect.top, "Button104" );
    // Init the check
    m_SetupBtnGraph.Init ( rect.left, rect.top, "Button1014", 2 ) ;
    m_SetupBtnMusic.Init ( rect.left, rect.top, "Button1015", 2 ) ;
    m_SetupBtnHotkey.Init ( rect.left, rect.top, "Button107", 2 );
    m_SetupBtnChat.Init ( rect.left, rect.top, "Button108", 2 );
    m_SetupBtnGraph.SetCheckValue(1);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetup::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
    // TODO: Add your message handler code here
}

void CDlgSetup::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog10",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            if (m_nScreenMode == 3 && m_DlgSChat.m_nChatSetMode == 2)
            {
                ShowAni->Show ( 5,
                                m_Pnt.x,
                                m_Pnt.y ) ;
            }
            else
                ShowAni->Show ( m_nScreenMode,
                                m_Pnt.x,
                                m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        // Show the button
        m_SetupBtnExit.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_SetupBtnDelete.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_SetupBtnReturn.Show ( m_Pnt.x, m_Pnt.y );
        m_SetupBtnRelogin.Show( m_Pnt.x, m_Pnt.y);
        // Show the check
        m_SetupBtnGraph.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_SetupBtnMusic.Show ( m_Pnt.x , m_Pnt.y ) ;
        m_SetupBtnHotkey.Show ( m_Pnt.x , m_Pnt.y ) ;
        m_SetupBtnChat.Show ( m_Pnt.x , m_Pnt.y ) ;
        switch(m_nScreenMode)
        {
        case 0:
            m_DlgGraph.Show(m_Pnt.x, m_Pnt.y);
            break;
        case 1:
            m_DlgMusic.Show(m_Pnt.x, m_Pnt.y);
            break;
        case 2:
            m_DlgHotkey.Show(m_Pnt.x, m_Pnt.y);
            break;
        case 3:
            m_DlgSChat.Show(m_Pnt.x, m_Pnt.y);
            break;
        }
    }
}


void CDlgSetup::OnSetupBtnExit()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    CDlgSetup::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_EXIT ) ;
}


void CDlgSetup::OnSetupBtnDelete()
{
    DXPlaySound ( "Sound/Exit_Ok.wav" ) ;
    if ( IDOK == this->MessageBox ( CMyShellApp::StringChange ( g_objGameDataSet.GetStr ( 10042 ) ), g_objGameDataSet.GetStr ( 10040 ) , MB_OKCANCEL | MB_ICONWARNING ) )
        //	if (IDOK == this->MessageBox("您确定要把当前的角色删除么？\n提示：删除角色后此角色的一切相关数据如物品、装备、朋友等信息都会消失。", "警告", MB_OKCANCEL|MB_ICONWARNING))
    {
        if ( IDOK == this->MessageBox ( g_objGameDataSet.GetStr ( 10043 ), g_objGameDataSet.GetStr ( 10040 ), MB_OKCANCEL | MB_ICONWARNING ) )
            //		if (IDOK == this->MessageBox("您真的确定要删除？", "警告", MB_OKCANCEL|MB_ICONWARNING))
        {
            char szFullFilePath[_MAX_PATH];
            sprintf(szFullFilePath, "LOG\\%s\\%s\\setup.ini", g_szServerName, g_objHero.GetName());
            ::DeleteFile(szFullFilePath);
            g_objHero.DeleteRole();
        }
        CDlgSetup::OnSetupBtnReturn ( ) ;
    }
}

void CDlgSetup::Exit()
{
    this->OnSetupBtnReturn () ;
}

void CDlgSetup::OnSetupBtnMusic()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgSetup::ProcessCheck ( 1 ) ;
    m_SetupBtnMusic.SetCheckValue ( 1 ) ;
    SetScreenMode(m_nScreenMode);
}

void CDlgSetup::OnSetupBtnHotkey()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgSetup::ProcessCheck ( 2 ) ;
    m_SetupBtnHotkey.SetCheckValue ( 1 ) ;
    SetScreenMode(m_nScreenMode);
}

void CDlgSetup::OnSetupBtnOther()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgSetup::ProcessCheck ( 3 ) ;
    m_SetupBtnChat.SetCheckValue ( 1 ) ;
    SetScreenMode(m_nScreenMode);
}

void CDlgSetup::ProcessCheck(int nCurMode)
{
    if ( nCurMode != m_nScreenMode )
    {
        switch ( m_nScreenMode )
        {
        case 0:
            m_SetupBtnGraph.SetCheckValue ( 0 ) ;
            m_DlgGraph.EnableWindow(false);
            break ;
        case 1:
            m_SetupBtnMusic.SetCheckValue ( 0 ) ;
            m_DlgMusic.EnableWindow(false);
            break ;
        case 2:
            m_SetupBtnHotkey.SetCheckValue ( 0 ) ;
            m_DlgMusic.EnableWindow(false);
            break ;
        case 3:
            m_SetupBtnChat.SetCheckValue ( 0 ) ;
            m_DlgSChat.EnableWindow(false);
            break ;
        }
    }
    m_nScreenMode = nCurMode ;
}

void CDlgSetup::OnSetupBtnReturn()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgSetup::EnableWindow(false);
}

void CDlgSetup::SetScreenMode(int mode)
{
    switch(mode)
    {
    case 0:
        m_DlgGraph.EnableWindow(true);
        break;
    case 1:
        m_DlgMusic.EnableWindow(true);
        break;
    case 2:
        m_DlgHotkey.EnableWindow(true);
        break;
    case 3:
        m_DlgSChat.EnableWindow(true);
        break;
    default:
        break;
    }
}

void CDlgSetup::OnResourceLose()
{
}

void CDlgSetup::OnSetupBtnGraph()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    ProcessCheck(0);
    m_SetupBtnGraph.SetCheckValue(1);
    SetScreenMode(m_nScreenMode);
}

void CDlgSetup::OnSetupBtnRelogin()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    CDlgSetup::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_EXIT_LOGIN ) ;
}
