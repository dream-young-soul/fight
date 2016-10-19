
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgDepot.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgDepot.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _SOUND_DISABLE
#define DXPlaySound //
#endif
/////////////////////////////////////////////////////////////////////////////
// CDlgDepot dialog


CDlgDepot::CDlgDepot(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgDepot::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgDepot)
    m_bShow = false ;
    m_nCurPage = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgDepot::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgDepot)
    DDX_Control(pDX, IDC_DEPOT_FACTION, m_StaFaction);
    DDX_Control(pDX, IDC_DEPOT_NAME, m_StaName);
    DDX_Control(pDX, IDC_DEPOT_CLOSEB, m_DepotCloseB);
    DDX_Control(pDX, IDC_DEPOT_BTN_ADD, m_DepotBtnAdd ) ;
    DDX_Control(pDX, IDC_DEPOT_BTN_GET, m_DepotBtnGet ) ;
    DDX_Control(pDX, IDC_DEPOT_EDT_MONEY, m_DepotEdtMoney ) ;
    DDX_Control(pDX, IDC_DEPOT_STA_LAY, m_DepotStaLay ) ;
    DDX_Control(pDX, IDC_DEPOT_GRI_LAY, m_DepotGriLay ) ;
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDepot, CDialog)
    //{{AFX_MSG_MAP(CDlgDepot)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_DEPOT_BTN_GET, OnDepotBtnGet)
    ON_BN_CLICKED(IDC_DEPOT_BTN_ADD, OnDepotBtnAdd)
    ON_BN_CLICKED(IDC_DEPOT_CLOSEB, OnDepotCloseb)
    ON_BN_CLICKED(IDC_DEPOT_CLOSE, OnDepotClose)
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDepot message handlers

LRESULT CDlgDepot::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( wParam == false )
        {
            m_nCurPage = 0 ;
            m_DepotGriLay.RemoveIcon ( 0, 0, true ) ;
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgDepot::OnDepotClose () ;
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgDepot::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgDepot::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgDepot::GetWindowRect ( rect ) ;
    // Init the button
    m_DepotBtnAdd.Init( rect.left, rect.top, "Button180" );
    m_DepotBtnGet.Init( rect.left, rect.top, "Button181" );
    m_DepotCloseB.Init( rect.left, rect.top, "Button540" );
    // Init the edit
    m_DepotEdtMoney.Init ( rect.left, rect.top, false, NULL, 0x000000, 0xffffff );
    m_DepotEdtMoney.SetOffset ( CPoint ( 4, 3 ) ) ;
    m_DepotEdtMoney.SetLimitText ( 8 ) ;
    // Init the static
    m_DepotStaLay.Init ( rect.left, rect.top );
    m_StaFaction.Init(rect.left, rect.top);
    m_StaFaction.SetStaticPosType(1);
    m_StaName.Init(rect.left, rect.top);
    m_StaName.SetStaticPosType(1);
    // Init the grid
    m_DepotGriLay.Init( rect.left, rect.top, 4, 5, DLG_DEPOT );
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDepot::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgDepot::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog18",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x ,
                           m_Pnt.y  );
        }
        else
        {
            return ;
        }
        // Show the button
        m_DepotBtnAdd.Show( m_Pnt.x, m_Pnt.y ) ;
        m_DepotBtnGet.Show( m_Pnt.x, m_Pnt.y ) ;
        m_DepotCloseB.Show( m_Pnt.x, m_Pnt.y ) ;
        // Show the edit
        m_DepotEdtMoney.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_DepotStaLay.Show ( m_Pnt.x, m_Pnt.y ) ;
        char strText[64] = "";
        strcpy(strText, g_objHero.GetName());
        m_StaName.SetWindowText(g_objHero.GetName());
        m_StaName.Show( m_Pnt.x, m_Pnt.y);
        m_StaFaction.SetWindowText("²Ö¿â");
        m_StaFaction.Show(m_Pnt.x, m_Pnt.y);
        // Show the grid
        m_DepotGriLay.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgDepot::OnDepotClose()
{
    // TODO: Add your control notification handler code here
    CloseDepot() ;
    CDlgDepot::EnableWindow( false ) ;
    DXPlaySound ( "Sound/window_close.wav" ) ;
    CDlgDepot::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_NPCBUY_CLOSE ) ;
}

BOOL CDlgDepot::OpenDepot(OBJID DepotID)
{
    g_objHero.OpenBank ( DepotID ) ;
    g_objHero.OpenPackage ( DepotID, PACKAGE_STORAGE ) ;
    m_DepotEdtMoney.SetWindowText ("0") ;
    m_nCurPage = 0 ;
    DXPlaySound("Sound/window_open2.wav");
    return true ;
}

void CDlgDepot::SetMoney()
{
    DWORD dwMoney = g_objHero.GetSavedMoney ( ) ;
    char strMoney[16] ;
    itoa ( dwMoney, strMoney, 10 ) ;
    m_DepotStaLay.SetWindowText ( strMoney ) ;
}

void CDlgDepot::SetItem()
{
    m_DepotGriLay.RemoveIcon ( 0, 0, true ) ;
    int dwItemAmount = g_objHero.GetPackageItemAmount() ;
    dwItemAmount = ( dwItemAmount > ( m_nCurPage + 1 ) * DEPOT_PAGE_AMOUNT ) ? ( m_nCurPage + 1 ) * DEPOT_PAGE_AMOUNT : dwItemAmount ;
    if ( dwItemAmount > 0 )
    {
        for ( int i = m_nCurPage * DEPOT_PAGE_AMOUNT; i < dwItemAmount; i++ )
        {
            CItem* pItem = g_objHero.GetPackageItemByIndex ( i ) ;
            if ( pItem != NULL )
            {
                OBJID ItemTypeID = pItem->GetTypeID () ;
                m_DepotGriLay.InsertIcon( 1, 1, ItemTypeID, i, ICON_TYPE_DEPOT ) ;
            }
        }
    }
}

void CDlgDepot::CloseDepot()
{
    g_objHero.CloseBank () ;
    g_objHero.ClosePackage () ;
}

void CDlgDepot::OnDepotBtnGet()
{
    char strMoney[16] ;
    m_DepotEdtMoney.GetWindowText ( strMoney, 9 ) ;
    DWORD dwMoney = atoi ( strMoney ) ;
    if ( dwMoney > 0 && dwMoney <= g_objHero.GetSavedMoney())
    {
        g_objHero.DrawMoney( dwMoney ) ;
    }
    else
    {
        g_objGameMsg.AddMsg( g_objGameDataSet.GetStr( 10028 ) ) ;
    }
    DXPlaySound("Sound/Depot_GetMoney.wav");
    m_DepotEdtMoney.SetWindowText ( "0" ) ;
    m_DepotEdtMoney.SetFocus () ;
}

void CDlgDepot::OnDepotBtnAdd()
{
    char strMoney[16] ;
    m_DepotEdtMoney.GetWindowText ( strMoney, 9) ;
    DWORD dwMoney = atoi ( strMoney ) ;
    if ( dwMoney > 0 && dwMoney <= g_objHero.GetMoney () )
    {
        g_objHero.SaveMoney ( dwMoney ) ;
    }
    else
    {
        g_objGameMsg.AddMsg( g_objGameDataSet.GetStr( 10028 ) ) ;
    }
    DXPlaySound("Sound/Depot_SaveMoney.wav");
    m_DepotEdtMoney.SetWindowText ( "0" ) ;
    m_DepotEdtMoney.SetFocus () ;
}

void CDlgDepot::OnDepotCloseb()
{
    DXPlaySound ( "Sound/window_close.wav" ) ;
    CloseDepot() ;
    CDlgDepot::EnableWindow( false ) ;
    CDlgDepot::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_NPCBUY_CLOSE ) ;
}





void CDlgDepot::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgDepot::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}
