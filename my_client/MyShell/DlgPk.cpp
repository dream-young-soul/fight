
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgPk.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgPk.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPk dialog


CDlgPk::CDlgPk(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgPk::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgPk)
    m_bShow = false ;
    //}}AFX_DATA_INIT
}


void CDlgPk::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPk)
    DDX_Control(pDX, IDC_PK_ARRESTMENT, m_PkArre);
    DDX_Control ( pDX, IDC_PK_FREE, m_PkFree ) ;
    DDX_Control ( pDX, IDC_PK_GROUP, m_PkGroup ) ;
    DDX_Control ( pDX, IDC_PK_SAFE, m_PkSafe ) ;
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPk, CDialog)
    //{{AFX_MSG_MAP(CDlgPk)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_PK_FREE, OnPkFree)
    ON_BN_CLICKED(IDC_PK_GROUP, OnPkGroup)
    ON_BN_CLICKED(IDC_PK_SAFE, OnPkSafe)
    ON_BN_CLICKED(IDC_PK_ARRESTMENT, OnPkArrestment)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPk message handlers

LRESULT CDlgPk::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
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

HBRUSH CDlgPk::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgPk::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgPk::GetWindowRect ( rect ) ;
    // Init the button
    m_PkFree.Init ( rect.left, rect.top, "Button47" ) ;
    m_PkGroup.Init ( rect.left, rect.top, "Button49" ) ;
    m_PkSafe.Init ( rect.left, rect.top, "Button48" ) ;
    m_PkArre.Init ( rect.left, rect.top, "Button412" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPk::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgPk::Show()
{
    if ( m_bShow )
    {
        // Show the button
        m_PkFree.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_PkGroup.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_PkSafe.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_PkArre.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgPk::OnPkFree()
{
    g_objHero.SetPkMode ( PKMODE_FREE ) ;
    CDlgPk::ShowWindow ( SW_HIDE ) ;
}

void CDlgPk::OnPkGroup()
{
    g_objHero.SetPkMode ( PKMODE_MONSTERONLY ) ;
    CDlgPk::ShowWindow ( SW_HIDE ) ;
}

void CDlgPk::OnPkSafe()
{
    g_objHero.SetPkMode ( PKMODE_TMEXCLUDE ) ;
    CDlgPk::ShowWindow ( SW_HIDE ) ;
}

void CDlgPk::OnPkArrestment()
{
    g_objHero.SetPkMode ( PKMODE_ARRESTMENT ) ;
    CDlgPk::ShowWindow ( SW_HIDE ) ;
}
