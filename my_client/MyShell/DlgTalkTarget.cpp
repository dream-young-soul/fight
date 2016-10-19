
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTalkTarget.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTalkTarget.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTalkTarget dialog


CDlgTalkTarget::CDlgTalkTarget(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTalkTarget::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTalkTarget)
    m_Pnt = CPoint(0, 0);
    m_bLoad = false;
    m_bShow = false;// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgTalkTarget::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTalkTarget)
    DDX_Control(pDX, IDC_LIST_TALKTARGET, m_ListBox);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTalkTarget, CDialog)
    //{{AFX_MSG_MAP(CDlgTalkTarget)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTalkTarget message handlers

LRESULT CDlgTalkTarget::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
            m_ListBox.SetFocus () ;
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

HBRUSH CDlgTalkTarget::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgTalkTarget::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect ;
    CDlgTalkTarget::GetWindowRect ( rect ) ;
    // Init the listbox
    if ( ! ( m_ListBox.Init ( rect.left, rect.top, NULL, 1 ) ) )
    {
        return false ;
    }
    m_ListBox.SetFontColor ( 0xffff00 ) ;
    m_bLoad = true ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTalkTarget::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTalkTarget::Show()
{
    if ( m_bLoad && m_bShow )
    {
        // Show the owner
        CRect rect;
        CDlgTalkTarget::GetWindowRect(rect);
        CMyBitmap::ShowBlock(m_Pnt.x - 2, m_Pnt.y - 2,
                             m_Pnt.x + rect.Width(), m_Pnt.y + rect.Height() + 55, 0x4c000000);
        // Show the listbox
        m_ListBox.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}
