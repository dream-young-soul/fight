
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgNpcBuy.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgNpcBuy.h"

#include "Hero.h"
#include "GamePlayerset.h"
#include "Shop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcBuy dialog


CDlgNpcBuy::CDlgNpcBuy(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgNpcBuy::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgNpcBuy)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_nCurrentAmount = SHOP_AMOUNT ;
    m_IdShopNpc = 0;
    //}}AFX_DATA_INIT
}


void CDlgNpcBuy::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgNpcBuy)
    DDX_Control(pDX, IDC_NPCBUY_BTN_TALK, m_NpcTalkBtn);
    DDX_Control(pDX, IDC_NPCBUY_BTN_ASK, m_NpcAskBtn);
    DDX_Control(pDX, IDC_NPCBUY_GRI_SALE, m_NpcBuySaleGri);
    DDX_Control(pDX, IDC_NPCBUY_BTN_CLOSE, m_NpcBuyCloseBtn);
    DDX_Control(pDX, IDC_NPCBUY_BTN_FIX, m_NpcBuyFixBtn);
    DDX_Control(pDX, IDC_NPCBUY_BTN_UP, m_NpcButUpBtn );
    DDX_Control(pDX, IDC_NPCBUY_BTN_DOWN, m_NpcButDownBtn ) ;
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNpcBuy, CDialog)
    //{{AFX_MSG_MAP(CDlgNpcBuy)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_NPCBUY_BTN_CLOSE, OnNpcbuyBtnClose)
    ON_BN_DOUBLECLICKED(IDC_NPCBUY_GRI_SALE, OnDoubleclickedNpcbuyGriSale)
    ON_BN_CLICKED(IDC_NPCBUY_BTN_FIX, OnNpcbuyBtnFix)
    ON_BN_CLICKED(IDC_NPCBUY_BTN_UP, OnNpcbuyBtnUp)
    ON_BN_CLICKED(IDC_NPCBUY_BTN_DOWN, OnNpcbuyBtnDown)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_NPCBUY_BTN_TALK, OnNpcbuyBtnTalk)
    ON_BN_CLICKED(IDC_NPCBUY_BTN_ASK, OnNpcbuyBtnAsk)
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcBuy message handlers

BOOL CDlgNpcBuy::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgNpcBuy::GetWindowRect ( rect ) ;
    // Init the grid
    m_NpcBuySaleGri.Init ( rect.left, rect.top, 4, 6, DLG_NPCBUY );
    // Init the button
    m_NpcBuyCloseBtn.Init ( rect.left, rect.top, "Button540" );
    m_NpcBuyFixBtn.Init ( rect.left, rect.top, "Button110" );
    m_NpcButUpBtn.Init ( rect.left, rect.top, "Button161" );
    m_NpcButDownBtn.Init ( rect.left, rect.top, "Button162" ) ;
    // 	m_NpcTalkBtn.Init(rect.left,rect.top,"Button113");
    m_NpcAskBtn.Init(rect.left, rect.top, "Button114");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgNpcBuy::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

void CDlgNpcBuy::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

LRESULT CDlgNpcBuy::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
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
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgNpcBuy::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog11",
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
        // Show the grid
        m_NpcBuySaleGri.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_NpcBuyCloseBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBuyFixBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        //		m_HelpBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        if (m_nCurrentAmount > SHOP_AMOUNT)
        {
            m_NpcButUpBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ((int)g_objShop.GetItemAmount () > m_nCurrentAmount)
        {
            m_NpcButDownBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        // 		m_NpcTalkBtn.Show( m_Pnt.x, m_Pnt.y );
        m_NpcAskBtn.Show( m_Pnt.x, m_Pnt.y );
    }
}

void CDlgNpcBuy::OnNpcbuyBtnClose()
{
    DXPlaySound("Sound/window_close.wav");
    CDlgNpcBuy::EnableWindow( false ) ;
    CloseShop() ;
    CDlgNpcBuy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_NPCBUY_CLOSE ) ;
}

int CDlgNpcBuy::OpenShop(OBJID idNpc)
{
    if ( !g_objShop.Open( idNpc ) )
    {
        return 0 ;
    }
    else
    {
        m_IdShopNpc = idNpc;
    }
    int nType = 0;
    // fill icon
    DWORD dwItemAmount = g_objShop.GetItemAmount () ;
    int nRealAmount = ( dwItemAmount > SHOP_AMOUNT ) ? SHOP_AMOUNT : dwItemAmount ;
    for ( int i = 0; i < nRealAmount; i++ )
    {
        CItem* pItem = g_objShop.GetItem( i ) ;
        if ( pItem != NULL )
        {
            OBJID ItemTypeID = pItem->GetTypeID() ;
            if (nType == 0)
            {
                nType = pItem->GetSubType() / 10000;
            }
            int nIndex = m_NpcBuySaleGri.InsertIcon ( 1, 1, ItemTypeID, i, ICON_TYPE_SHOP ) ;
            m_NpcBuySaleGri.SetIconRealID( nIndex, pItem->GetID() ) ;
        }
    }
    return nType ;
}

void CDlgNpcBuy::CloseShop()
{
    g_objShop.Close() ;
    m_NpcBuySaleGri.RemoveIcon ( 0, 0, true ) ;
    m_IdShopNpc = 0;
    m_nCurrentAmount = SHOP_AMOUNT ;
}

void CDlgNpcBuy::OnDoubleclickedNpcbuyGriSale()
{
    // TODO: Add your control notification handler code here
    //	AfxMessageBox ( "a" ) ;
}

void CDlgNpcBuy::OnNpcbuyBtnFix()
{
    if (g_objPlayerSet.GetPlayer(m_IdShopNpc) != NULL)
    {
        CDlgNpcBuy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_NPCBUY_FIX , DLG_NPCBUY) ;
    }
}

void CDlgNpcBuy::OnNpcbuyBtnUp()
{
    if ( m_nCurrentAmount > SHOP_AMOUNT )
    {
        DXPlaySound ( "Sound/up_Page.wav" ) ;
        m_NpcBuySaleGri.RemoveIcon ( 0, 0, true ) ;
        m_nCurrentAmount -= SHOP_AMOUNT ;
        for ( int i = m_nCurrentAmount - SHOP_AMOUNT; i < m_nCurrentAmount; i++ )
        {
            CItem* pItem = g_objShop.GetItem( i ) ;
            if ( pItem != NULL )
            {
                OBJID ItemTypeID = pItem->GetTypeID() ;
                int nIndex = m_NpcBuySaleGri.InsertIcon ( 1, 1, ItemTypeID, i, ICON_TYPE_SHOP ) ;
                m_NpcBuySaleGri.SetIconRealID( nIndex, pItem->GetID() ) ;
            }
        }
    }
}

void CDlgNpcBuy::OnNpcbuyBtnDown()
{
    int dwAmount =(int) g_objShop.GetItemAmount () ;
    if (dwAmount > m_nCurrentAmount )
    {
        DXPlaySound ( "Sound/up_Page.wav" ) ;
        m_NpcBuySaleGri.RemoveIcon ( 0, 0, true ) ;
        m_nCurrentAmount += SHOP_AMOUNT ;
        int nNextAmount = ( dwAmount > m_nCurrentAmount ) ? m_nCurrentAmount : dwAmount ;
        for ( int i = m_nCurrentAmount - SHOP_AMOUNT; i < nNextAmount; i++ )
        {
            CItem* pItem = g_objShop.GetItem( i ) ;
            if ( pItem != NULL )
            {
                OBJID ItemTypeID = pItem->GetTypeID() ;
                int nIndex = m_NpcBuySaleGri.InsertIcon ( 1, 1, ItemTypeID, i, ICON_TYPE_SHOP ) ;
                m_NpcBuySaleGri.SetIconRealID( nIndex, pItem->GetID() ) ;
            }
        }
    }
}

void CDlgNpcBuy::OnBtnHelp()
{
    CDlgNpcBuy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_NPCBUY ) ;
}

void CDlgNpcBuy::OnNpcbuyBtnTalk()
{
    // TODO: Add your control notification handler code here
}

void CDlgNpcBuy::OnNpcbuyBtnAsk()
{
    // TODO: Add your control notification handler code here
}

void CDlgNpcBuy::OnCloseB()
{
    CDlgNpcBuy::OnNpcbuyBtnClose();
}

void CDlgNpcBuy::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgNpcBuy::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}

void CDlgNpcBuy::ReSetShop()
{
    m_nCurrentAmount = SHOP_AMOUNT ;
    m_NpcBuySaleGri.RemoveIcon ( 0, 0, true ) ;
    DWORD dwItemAmount = g_objShop.GetItemAmount () ;
    int nRealAmount = ( dwItemAmount > SHOP_AMOUNT ) ? SHOP_AMOUNT : dwItemAmount ;
    for ( int i = 0; i < nRealAmount; i++ )
    {
        CItem* pItem = g_objShop.GetItem( i ) ;
        if ( pItem != NULL )
        {
            OBJID ItemTypeID = pItem->GetTypeID() ;
            int nIndex = m_NpcBuySaleGri.InsertIcon ( 1, 1, ItemTypeID, i, ICON_TYPE_SHOP ) ;
            m_NpcBuySaleGri.SetIconRealID( nIndex, pItem->GetID() ) ;
        }
    }
}





















