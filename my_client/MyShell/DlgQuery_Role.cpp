
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgQuery_Role.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgQuery_Role.h"

#include "Hero.h"
#include "3dRoleData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery_Role dialog


CDlgQuery_Role::CDlgQuery_Role(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgQuery_Role::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgQuery_Role)
    m_DlgPnt = CPoint ( 0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgQuery_Role::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgQuery_Role)
    DDX_Control(pDX, IDC_STA_POTENTIAL, m_StaPotential);
    DDX_Control(pDX, IDC_STA_TITLE, m_StaTitle);
    DDX_Control(pDX, IDC_STA_STRONG, m_StaStrong);
    DDX_Control(pDX, IDC_STA_SMART, m_StaSmart);
    DDX_Control(pDX, IDC_STA_PK, m_StaPk);
    DDX_Control(pDX, IDC_STA_LEV, m_StaLev);
    DDX_Control(pDX, IDC_STA_HA, m_StaHa);
    DDX_Control(pDX, IDC_STA_FERE, m_StaFere);
    DDX_Control(pDX, IDC_STA_FAMOR, m_StaFamor);
    DDX_Control(pDX, IDC_STA_ENERGY, m_StaEnergy);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQuery_Role, CDialog)
    //{{AFX_MSG_MAP(CDlgQuery_Role)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery_Role message handlers

HBRUSH CDlgQuery_Role::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgQuery_Role::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgQuery_Role::GetWindowRect ( rect ) ;
    // Init the static
    m_StaEnergy.Init(rect.left, rect.top);
    m_StaFamor.Init(rect.left, rect.top);
    m_StaFere.Init(rect.left, rect.top);
    m_StaTitle.Init(rect.left, rect.top);
    m_StaHa.Init(rect.left, rect.top);
    m_StaLev.Init(rect.left, rect.top);
    m_StaPk.Init(rect.left, rect.top);
    m_StaSmart.Init(rect.left, rect.top);
    m_StaStrong.Init(rect.left, rect.top);
    m_StaPotential.Init(rect.left, rect.top);
    // Init the image
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgQuery_Role::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_DlgPnt.x = x ;
    m_DlgPnt.y = y ;
    // TODO: Add your message handler code here
}

void CDlgQuery_Role::Show( int xPos, int yPos )
{
    // Show the static
    CDlgQuery_Role::SetRoleInfo();
    m_StaEnergy.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaHa.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaSmart.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaStrong.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaLev.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaTitle.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaPk.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaFere.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaFamor.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
    m_StaPotential.Show(m_DlgPnt.x + xPos, m_DlgPnt.y + yPos);
}

LRESULT CDlgQuery_Role::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2)
        {
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgQuery_Role::SetRoleInfo ()
{
    char strText[64] ;
    // Level
    itoa( g_objHero.GetLev (), strText, 10 ) ;
    m_StaLev.SetWindowText ( strText ) ;
    //职业
    m_StaFamor.SetWindowText( g_objHero.GetProfessionalName(g_objHero.GetProfession())) ;
    // Pk
    itoa( g_objHero.GetPk(), strText, 10);
    m_StaPk.SetWindowText ( strText) ;
    // Fere
    m_StaFere.SetWindowText ( g_objHero.GetMate () ) ;
    //Title
    m_StaTitle.SetWindowText((char*)g_obj3DRoleData.GetNobilityTitle(g_objHero.GetNobilityRank()));
    //health
    itoa( g_objHero.GetHealth(), strText, 10);
    m_StaHa.SetWindowText( strText);
    // strong
    itoa( g_objHero.GetForce(), strText, 10);
    m_StaStrong.SetWindowText(strText);
    //energy
    itoa( g_objHero.GetSoul(), strText, 10);
    m_StaEnergy.SetWindowText(strText);
    //smart
    itoa( g_objHero.GetSpeed(), strText, 10);
    m_StaSmart.SetWindowText(strText);
    //potential 潜能
    itoa(g_objHero.GetPotential(), strText, 10);
    m_StaPotential.SetWindowText(strText);
}

void CDlgQuery_Role::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_DlgPnt.x, point.y + m_DlgPnt.y ) ;
    CDlgQuery_Role::GetParent()->PostMessage(WM_MOUSEMOVE, nFlags, lParam);
    CDialog::OnMouseMove(nFlags, point);
}
