
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgSetup_Graph.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgSetup_Graph.h"

#include "Hero.h"
#include "Role.h"
#include "3DGamemap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Graph dialog


CDlgSetup_Graph::CDlgSetup_Graph(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSetup_Graph::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetup_Graph)
    // NOTE: the ClassWizard will add member initialization here
    m_bShow = false;
    m_Pnt = CPoint( 0, 0 );
    //}}AFX_DATA_INIT
}


void CDlgSetup_Graph::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetup_Graph)
    DDX_Control(pDX, IDC_CHK_WEATHER, m_ChkWeather);
    DDX_Control(pDX, IDC_CHK_SHADOW, m_ChkShadow);
    DDX_Control(pDX, IDC_CHK_ROLENAME, m_ChkRoleName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup_Graph, CDialog)
    //{{AFX_MSG_MAP(CDlgSetup_Graph)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_CHK_ROLENAME, OnChkRolename)
    ON_BN_CLICKED(IDC_CHK_SHADOW, OnChkShadow)
    ON_BN_CLICKED(IDC_CHK_WEATHER, OnChkWeather)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Graph message handlers

LRESULT CDlgSetup_Graph::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

HBRUSH CDlgSetup_Graph::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Return a different brush if the default is not desired
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgSetup_Graph::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect ;
    CDlgSetup_Graph::GetWindowRect ( rect ) ;
    m_ChkRoleName.Init(rect.left, rect.top, "Check190", 2);
    m_ChkShadow.Init(rect.left, rect.top, "Check190", 2);
    m_ChkWeather.Init(rect.left, rect.top, "Check190", 2);
    m_ChkWeather.SetCheckValue(1);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetup_Graph::Show(int x, int y)
{
    if (m_bShow)
    {
        m_ChkRoleName.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_ChkShadow.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_ChkWeather.Show(m_Pnt.x + x, m_Pnt.y + y);
    }
}

void CDlgSetup_Graph::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x;
    m_Pnt.y = y;
    // TODO: Add your message handler code here
}

void CDlgSetup_Graph::OnChkRolename()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    g_objGameMap.SetShowMapObjInfoFlag(m_ChkRoleName.GetCheckValue());
    CDlgSetup_Graph::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_SETUP_SHIFT, m_ChkRoleName.GetCheckValue() ) ;
}

void CDlgSetup_Graph::OnChkShadow()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ChkShadow.GetCheckValue() == 1)
    {
        CRole::s_nShadowType = CRole::SHADOW_REAL ;
    }
    else
    {
        CRole::s_nShadowType = CRole::SHADOW_NONE ;
    }
}

void CDlgSetup_Graph::OnChkWeather()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}
