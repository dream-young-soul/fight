
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTxtAction.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTxtAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTxtAction dialog


CDlgTxtAction::CDlgTxtAction(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTxtAction::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTxtAction)
    m_bLoad = false ;
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgTxtAction::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTxtAction)
    DDX_Control(pDX, IDC_LIST_TXTACTION, m_ListTxtAction);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTxtAction, CDialog)
    //{{AFX_MSG_MAP(CDlgTxtAction)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTxtAction message handlers

LRESULT CDlgTxtAction::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE)
    {
        if ( wParam == false )
        {
            m_bShow = false ;
        }
        else
        {
            m_bShow = true ;
            m_ListTxtAction.SetFocus () ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2 )
        {
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTxtAction::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgTxtAction::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect ;
    CDlgTxtAction::GetWindowRect ( rect ) ;
    // Init the listbox
    int nHeight = 10 * (CMyBitmap::GetFontSize () + 4);
    CRect BRect;
    m_ListTxtAction.GetWindowRect(BRect);
    m_ListTxtAction.MoveWindow(0, 0, BRect.Width(), nHeight);
    if ( ! ( m_ListTxtAction.Init ( rect.left, rect.top, NULL, 1 ) ) )
    {
        return false ;
    }
    m_ListTxtAction.SetFontColor ( 0xffff00 ) ;
    m_bLoad = true ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTxtAction::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}



void CDlgTxtAction::Show()
{
    if ( m_bLoad && m_bShow )
    {
        // Show the owner
        CRect rect;
        CDlgTxtAction::GetWindowRect(rect);
        CMyBitmap::ShowBlock(m_Pnt.x - 2, m_Pnt.y - 2,
                             m_Pnt.x + rect.Width(), m_Pnt.y + rect.Height() + 55, 0x4c000000);
        // Show the listbox
        m_ListTxtAction.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}
