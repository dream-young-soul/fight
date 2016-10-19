
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgProIntro.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgProIntro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProIntro dialog


CDlgProIntro::CDlgProIntro(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgProIntro::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgProIntro)
    m_bShow = false ;
    m_Pnt = CPoint (0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgProIntro::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgProIntro)
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_STA_1, m_Sta1);
    DDX_Control(pDX, IDC_STA_2, m_Sta2);
    DDX_Control(pDX, IDC_STA_3, m_Sta3);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProIntro, CDialog)
    //{{AFX_MSG_MAP(CDlgProIntro)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProIntro message handlers

BOOL CDlgProIntro::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgProIntro::GetWindowRect ( rect ) ;
    // Init the static
    m_Sta1.Init ( rect.left, rect.top ) ;
    m_Sta2.Init ( rect.left, rect.top ) ;
    m_Sta3.Init ( rect.left, rect.top ) ;
    // Init the button
    m_BtnClose.Init ( rect.left, rect.top, "Button5" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgProIntro::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

LRESULT CDlgProIntro::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_SHOWWINDOW:
        m_bShow = wParam ;
        if ( !m_bShow )
        {
            m_Sta1.SetWindowText ( NULL ) ;
            m_Sta2.SetWindowText ( NULL ) ;
            m_Sta3.SetWindowText ( NULL ) ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgProIntro::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgProIntro::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog48",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the static
        m_Sta1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta3.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_BtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgProIntro::OnBtnClose()
{
    CDlgProIntro::ShowWindow ( SW_HIDE ) ;
}
