
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTitleBar.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTitleBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTitleBar dialog


CDlgTitleBar::CDlgTitleBar(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTitleBar::IDD, pParent)
{
    m_bLBtnDown = false ;
    m_pParentWnd = NULL ;
}


void CDlgTitleBar::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTitleBar)
    DDX_Control(pDX, IDC_BTN_MIN, m_BtnMin);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTitleBar, CDialog)
    //{{AFX_MSG_MAP(CDlgTitleBar)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_MIN, OnBtnMin)
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTitleBar message handlers

BOOL CDlgTitleBar::OnInitDialog()
{
    CDialog::OnInitDialog();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bLBtnDown = true ;
    GetCursorPos ( &m_InitDlgPnt ) ;
    CDlgTitleBar::GetWindowRect ( m_InitDlgRect ) ;
    if ( m_pParentWnd )
    {
        m_pParentWnd->PostMessage( WM_MY_MESSAGE, ON_OWNER_MOVE ) ;
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgTitleBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bLBtnDown = false ;
    if ( m_pParentWnd )
    {
        m_pParentWnd->PostMessage( WM_MY_MESSAGE, ON_OWNER_STOP ) ;
    }
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgTitleBar::OnMouseMove(UINT nFlags, CPoint point)
{
    if ( m_bLBtnDown )
    {
    }
    CDialog::OnMouseMove(nFlags, point);
}

BOOL CDlgTitleBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_POINT) ) ;
    return true ;
    //	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CDlgTitleBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2 )
        {
            return true ;
        }
    }
    else if ( message == 43 )
    {
        return true ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

CPoint CDlgTitleBar::GetOffset()
{
    GetCursorPos ( &m_MouseMovePnt ) ;
    CDlgTitleBar::MoveWindow ( m_InitDlgRect.left + ( m_MouseMovePnt.x - m_InitDlgPnt.x ),
                               m_InitDlgRect.top + ( m_MouseMovePnt.y - m_InitDlgPnt.y ),
                               m_InitDlgRect.Width (),
                               m_InitDlgRect.Height () ) ;
    return CPoint ( m_MouseMovePnt.x - m_InitDlgPnt.x, m_MouseMovePnt.y - m_InitDlgPnt.y ) ;
}


HBRUSH CDlgTitleBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return m_Hbr ;
}

void CDlgTitleBar::OnBtnClose()
{
    if ( m_pParentWnd )
    {
        m_pParentWnd->PostMessage( WM_MY_MESSAGE, ON_DLGTASK_CLOSE);
        m_pParentWnd->PostMessage( WM_MY_MESSAGE, ON_EXIT_LOGIN ) ;
    }
}

void CDlgTitleBar::OnBtnMin()
{
    if ( m_pParentWnd )
    {
        m_pParentWnd->PostMessage( WM_MY_MESSAGE, ON_MIN ) ;
    }
}

void CDlgTitleBar::OnDestroy()
{
    CDialog::OnDestroy();
    DeleteObject( m_Hbr ) ;
}

void CDlgTitleBar::ResetBk(int nMode)
{
    char strTemp[32] ;
    if ( nMode == 0 )
    {
        strcpy ( strTemp, "Data/Main/TitleBarBk.bmp" ) ;
    }
    else
    {
        sprintf ( strTemp, "Data/Main%d/TitleBarBk.bmp", nMode ) ;
    }
    HBITMAP hBmp ;
    hBmp =  (HBITMAP )::LoadImage ( AfxGetInstanceHandle(),
                                    strTemp,
                                    IMAGE_BITMAP,
                                    0,
                                    0,
                                    LR_LOADFROMFILE ) ;
    m_Hbr = CreatePatternBrush ( hBmp ) ;
    CDlgTitleBar::RedrawWindow () ;
}
