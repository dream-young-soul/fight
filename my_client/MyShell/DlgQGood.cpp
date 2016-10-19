
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgQGood.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgQGood.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQGood dialog


CDlgQGood::CDlgQGood(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgQGood::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgQGood)
    // NOTE: the ClassWizard will add member initialization here
    m_bShow = false;
    m_Pnt = CPoint( 0, 0 );
    //}}AFX_DATA_INIT
}


void CDlgQGood::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgQGood)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQGood, CDialog)
    //{{AFX_MSG_MAP(CDlgQGood)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQGood message handlers

void CDlgQGood::Show()
{
}

LRESULT CDlgQGood::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgQGood::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgQGood::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
