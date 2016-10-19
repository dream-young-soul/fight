
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MenuGroup.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "MenuGroup.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuGroup dialog


CMenuGroup::CMenuGroup(CWnd* pParent /*=NULL*/)
    : CDialog(CMenuGroup::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMenuGroup)
    m_bShow = false ;
    m_ParentWnd = NULL ;
    m_bCreate = false ;
    //}}AFX_DATA_INIT
}


void CMenuGroup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMenuGroup)
    DDX_Control(pDX, IDC_GROUP_CHK_JION, m_GroupChkJoin);
    DDX_Control(pDX, IDC_GROUP_CHK_STOP, m_GroupChkStop);
    DDX_Control(pDX, IDC_GROUP_CHK_MONEY, m_GroupChkMoney);
    DDX_Control(pDX, IDC_GROUP_CHK_ITEM, m_GroupChkItem);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuGroup, CDialog)
    //{{AFX_MSG_MAP(CMenuGroup)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_GROUP_CHK_STOP, OnGroupChkStop)
    ON_BN_CLICKED(IDC_GROUP_CHK_MONEY, OnGroupChkMoney)
    ON_BN_CLICKED(IDC_GROUP_CHK_ITEM, OnGroupChkItem)
    ON_BN_CLICKED(IDC_GROUP_CHK_JION, OnGroupChkJion)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuGroup message handlers

LRESULT CMenuGroup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( m_bShow && m_bCreate )
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
            m_bCreate = false ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
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

HBRUSH CMenuGroup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CMenuGroup::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CMenuGroup::GetWindowRect ( rect ) ;
    // Init the checks
    m_GroupChkStop.Init ( rect.left, rect.top, "Check190", 2 ) ;
    m_GroupChkMoney.Init ( rect.left, rect.top, "Check190", 2 ) ;
    m_GroupChkItem.Init ( rect.left, rect.top, "Check190", 2 ) ;
    m_GroupChkJoin.Init( rect.left, rect.top, "Check190", 2 ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuGroup::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CMenuGroup::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog57",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
            ShowAni->Show( 1,
                           m_Pnt.x + 128,
                           m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        // Show the checks
        m_GroupChkStop.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_GroupChkMoney.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_GroupChkItem.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_GroupChkJoin.Show( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CMenuGroup::OnGroupChkStop()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ParentWnd )
    {
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_GROUP_CLICK, GEAR_CLICK ) ;
    }
}

void CMenuGroup::OnGroupChkMoney()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ParentWnd )
    {
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_GROUP_CLICK, MONEY_CLICK ) ;
    }
}

void CMenuGroup::OnGroupChkItem()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ParentWnd )
    {
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_GROUP_CLICK, ITEM_CLICK ) ;
    }
}

void CMenuGroup::SetParentWnd(CWnd* pParentWnd)
{
    m_ParentWnd = pParentWnd ;
}

void CMenuGroup::OnGroupChkJion()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ParentWnd )
    {
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_GROUP_CLICK, JOIN_CLICK ) ;
    }
}

void CMenuGroup::ResetChecks()
{
    m_GroupChkItem.SetCheckValue(0);
    m_GroupChkJoin.SetCheckValue(0);
    m_GroupChkMoney.SetCheckValue(1);
    m_GroupChkStop.SetCheckValue(0);
}
