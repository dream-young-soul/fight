
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgOther.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgOther.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOther dialog


CDlgOther::CDlgOther(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgOther::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgOther)
    m_bShow = false ;
    m_Pnt = CPoint (0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgOther::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgOther)
    DDX_Control(pDX, IDC_OTHER_BTN_LAY, m_OtherBtnLay);
    DDX_Control(pDX, IDC_OTHER_BTN_DEL, m_OtherBtnDel);
    DDX_Control(pDX, IDC_BTN_CLOSEB, m_BtnCloseB);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOther, CDialog)
    //{{AFX_MSG_MAP(CDlgOther)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSEB, OnBtnCloseb)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_OTHER_BTN_LAY, OnOtherBtnLay)
    ON_BN_CLICKED(IDC_OTHER_BTN_DEL, OnOtherBtnDel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOther message handlers

LRESULT CDlgOther::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_SHOWWINDOW:
        m_bShow = wParam ;
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

HBRUSH CDlgOther::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgOther::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgOther::GetWindowRect ( rect ) ;
    // Init the button
    m_OtherBtnLay.Init ( rect.left, rect.top, "Button401" ) ;
    m_OtherBtnDel.Init ( rect.left, rect.top, "Button402" ) ;
    m_BtnCloseB.Init ( rect.left, rect.top, "Button3" ) ;
    m_BtnClose.Init ( rect.left, rect.top, "Button5" ) ;
    m_BtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOther::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgOther::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog40",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the button
        m_OtherBtnLay.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_OtherBtnDel.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnCloseB.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgOther::OnBtnCloseb()
{
    CDlgOther::ShowWindow ( SW_HIDE ) ;
}

void CDlgOther::OnBtnClose()
{
    CDlgOther::ShowWindow ( SW_HIDE ) ;
}

void CDlgOther::OnBtnHelp()
{
    CDlgOther::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_OTHER ) ;
}

void CDlgOther::OnOtherBtnLay()
{
    // TODO: Add your control notification handler code here
}

void CDlgOther::OnOtherBtnDel()
{
    CDlgOther::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_OTHER_DELNPC ) ;
}
