
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgListBox.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgListBox dialog


CDlgListBox::CDlgListBox(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgListBox::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgListBox)
    m_bLoad = false ;
    m_bShow = false ;
    m_nListType = 1;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_dwBackColor = 0x4c000000;
    //}}AFX_DATA_INIT
}


void CDlgListBox::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgListBox)
    DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgListBox, CDialog)
    //{{AFX_MSG_MAP(CDlgListBox)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgListBox message handlers

void CDlgListBox::Show()
{
    if ( m_bLoad && m_bShow )
    {
        // Show the owner
        CRect rect;
        CDlgListBox::GetWindowRect(rect);
        CMyBitmap::ShowBlock(m_Pnt.x - 2, m_Pnt.y - 2,
                             m_Pnt.x + rect.Width(), m_Pnt.y + rect.Height(), m_dwBackColor);
        // Show the listbox
        m_ListBox.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

LRESULT CDlgListBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
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

HBRUSH CDlgListBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgListBox::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgListBox::GetWindowRect ( rect ) ;
    // Init the listbox
    if ( ! ( m_ListBox.Init ( rect.left, rect.top, NULL, m_nListType ) ) )
    {
        return false ;
    }
    m_ListBox.SetFontColor ( 0xffff00 ) ;
    m_bLoad = true ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgListBox::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgListBox::SetListBoxType(int nType)
{
    m_nListType = nType;
}

void CDlgListBox::SetDlgBackColor(DWORD dwColor)
{
    m_dwBackColor = dwColor;
}
