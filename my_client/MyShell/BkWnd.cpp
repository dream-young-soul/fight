
//**********************************************************
// 代码编辑器
//**********************************************************

// BkWnd.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "BkWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBkWnd

CBkWnd::CBkWnd()
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBkWnd::~CBkWnd()
{
}


BEGIN_MESSAGE_MAP(CBkWnd, CWnd)
    //{{AFX_MSG_MAP(CBkWnd)
    ON_WM_SHOWWINDOW()
    ON_WM_CLOSE()
    ON_WM_ACTIVATEAPP()
    ON_WM_ACTIVATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBkWnd message handlers

void CBkWnd::Create()
{
	//_FRAME_SIZE
	//windows7 与 windows10 窗口标题栏高度是不一样的，改为动态获取 2016.10.18
	_FRAME_SIZE = ::GetSystemMetrics(SM_CYCAPTION);
	_FRAME_SIZE += _FRAME_SIZE /2;
	_SCR_WIDTH = GetPrivateProfileInt ( "ScreenMode", "ScreenWidth", 1024, g_strSetupIni ) ;
	_SCR_HEIGHT = GetPrivateProfileInt ( "ScreenMode", "ScreenHeight", 768, g_strSetupIni ) ;	
	PUZZLE_SCRX = _SCR_WIDTH;
	PUZZLE_SCRY = _SCR_HEIGHT;
    CRect rect = CRect( 0, 0, _SCR_WIDTH, _SCR_HEIGHT + _FRAME_SIZE  ) ;
	////2016.5.2 设置有有边框样式	
    CreateEx( WS_MAXIMIZEBOX,
              AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
              "MyWnd",
              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			  //WS_POPUP, 无边框
              rect,
              NULL,
              NULL,
              NULL);
	//CRect rect = CRect( 0, 0, 1024, 768 ) ;
	//CreateEx( NULL,  
	//		  AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
	//		  "MyWnd",
	//		  WS_POPUP,
	//		  rect,
	//		  NULL,
	//		  NULL,
	//		  NULL);
}

void CBkWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CWnd::OnShowWindow(bShow, nStatus);
    m_MyShellDlg.Create( IDD_MYSHELL_DIALOG, this ) ;
    if ( m_MyShellDlg.m_hWnd && m_MyShellDlg.m_bInitSuc )
    {
        m_MyShellDlg.ShowWindow( SW_SHOW ) ;
        m_MyShellDlg.SetWindowPos(&m_MyShellDlg.wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        m_MyShellDlg.SetFocus() ;
		m_MyShellDlg.MoveWindow(0,0,_SCR_WIDTH,_SCR_HEIGHT);
	 }
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
}
void CBkWnd::OnClose()
{
    m_MyShellDlg.RealExit();
    CWnd::OnClose();
}

void CBkWnd::OnActivateApp(BOOL bActive, /*HTASK*/DWORD hTask)
{
    CWnd::OnActivateApp(bActive, hTask);
    //	if (!bActive )
    //	{
    //		this->m_MyShellDlg.PostMessage (WM_MY_MESSAGE, ON_MIN);
    //	}
    // TODO: Add your message handler code here
}

void CBkWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CWnd::OnActivate(nState, pWndOther, bMinimized);
    // TODO: Add your message handler code here
    if (nState == WA_INACTIVE && !bMinimized )
    {
        m_MyShellDlg.SetWindowPos(&m_MyShellDlg.wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else if ( (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
              && !bMinimized)
    {
        m_MyShellDlg.SetWindowPos(&m_MyShellDlg.wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}
//这段代码仅仅是标题栏的最大化按钮变为灰色
BOOL CBkWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( !CWnd::PreCreateWindow(cs) )     return FALSE;

	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	//cs.style &= ~WS_MAXIMIZEBOX; //禁止窗口最大化
	////cs.style &= ~WS_MINIMIZEBOX; //禁止窗口最小化
	//cs.style &= ~WS_THICKFRAME;//使窗口不能用鼠标改变大小
	//cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
	

}