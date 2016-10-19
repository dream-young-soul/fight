
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgTrade.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgTrade.h"
#include "gameplayerset.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTrade dialog


CDlgTrade::CDlgTrade(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTrade::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTrade)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_bMoneyAdd = false ;
    //}}AFX_DATA_INIT
}


void CDlgTrade::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTrade)
    DDX_Control(pDX, IDC_TRADE_BTN_NPCOK, m_NpcAgreeChk);
    DDX_Control(pDX, IDC_TRADE_STA_MPCNAME, m_TradeStaMpcName);
    DDX_Control(pDX, IDC_TRADE_BTN_MPCAGREE, m_MpcAgreeChk);
    DDX_Control(pDX, IDC_TRADE_BTN_MONEYNPCOK, m_MoneyNpcOkChk);
    DDX_Control(pDX, IDC_TRADE_BTN_MONEYMPCOK, m_MoneyMpcOkChk);
    DDX_Control(pDX, IDC_TRADE_CLOSEB, m_TradeCloseB);
    DDX_Control(pDX, IDC_TRADE_GRI_NPC, m_TradeGriNpc);
    DDX_Control(pDX, IDC_TRADE_GRI_MPC, m_TradeGriMpc);
    DDX_Control(pDX, IDC_TRADE_STA_NPCNAME, m_TradeStaNpcName);
    DDX_Control(pDX, IDC_TRADE_STA_NPCMONEY, m_TradeStaNpcMoney);
    DDX_Control(pDX, IDC_TRADE_EDT_MPCMONEY, m_TradeEdtMpcMoney);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTrade, CDialog)
    //{{AFX_MSG_MAP(CDlgTrade)
    ON_BN_CLICKED(IDC_TRADE_BTN_CLOSE, OnTradeBtnClose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_TRADE_BTN_MPCAGREE, OnTradeBtnMpcagree)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_TRADE_CLOSEB, OnTradeCloseb)
    ON_BN_CLICKED(IDC_TRADE_BTN_MONEYNPCOK, OnTradeBtnMoneynpcok)
    ON_BN_CLICKED(IDC_TRADE_BTN_MONEYMPCOK, OnTradeBtnMoneympcok)
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTrade message handlers
LRESULT CDlgTrade::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        this->ResetTrade () ;
        if ( wParam == false )
        {
            m_bShow = false ;
            DXPlaySound ( "Sound/window_close.wav" ) ;
            CDlgTrade::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TRADE_CLOSE ) ;
        }
        else
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
            m_bShow = true ;
            char strName[16] ;
            strcpy ( strName, g_objHero.GetPlayerTradeName () ) ;
            m_TradeStaNpcName.SetWindowText( strName ) ;
            m_TradeStaMpcName.SetWindowText(g_objHero.GetName());
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

BOOL CDlgTrade::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgTrade::GetWindowRect ( rect ) ;
    // Init the Grid
    m_TradeGriNpc.Init ( rect.left, rect.top, 4, 5, DLG_TRADENPC );
    m_TradeGriNpc.SetEnable ( false ) ;
    m_TradeGriMpc.Init ( rect.left, rect.top, 4, 5, DLG_TRADEMPC );
    m_TradeGriMpc.SetPickEnable ( false ) ;
    // Init the edit
    m_TradeEdtMpcMoney.Init ( rect.left, rect.top, false, NULL, 0x000000, 0xffffff );
    m_TradeEdtMpcMoney.SetOffset ( CPoint ( 1, 0 ) ) ;
    m_TradeEdtMpcMoney.SetLimitText ( 8 ) ;
    // Init the static
    m_TradeStaNpcName.Init ( rect.left, rect.top );
    m_TradeStaNpcName.SetFontColor ( 0xffffff ) ;
    m_TradeStaNpcMoney.Init ( rect.left, rect.top );
    m_TradeStaNpcMoney.SetFontColor ( 0xffffff ) ;
    m_TradeStaMpcName.Init(rect.left, rect.top);
    // Init the button
    m_MoneyMpcOkChk.Init(rect.left, rect.top, "Button141", 2);
    m_MoneyNpcOkChk.Init(rect.left, rect.top, "Button141", 2);
    m_MpcAgreeChk.Init ( rect.left, rect.top, "Button140", 2 );
    m_NpcAgreeChk.Init ( rect.left, rect.top, "Button140", 2 );
    m_NpcAgreeChk.EnableWindow(false);
    m_TradeCloseB.Init ( rect.left, rect.top, "Button540" );
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTrade::Show()
{
    if ( m_bShow )
    {
        // Show the Owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog14",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x,
                           m_Pnt.y );
            ShowAni->Show( 1,
                           m_Pnt.x,
                           m_Pnt.y + 256 );
        }
        else
        {
            return ;
        }
        // Show the grid
        m_TradeGriNpc.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TradeGriMpc.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the edit
        char szMoney[8];
        m_TradeEdtMpcMoney.GetWindowText(szMoney, 8);
        if (strlen(szMoney) > 1 && szMoney[0] == '0')
        {
            int nMoney = atoi(szMoney);
            char szNowMoney[8] = "";
            sprintf(szNowMoney, "%d", nMoney);
            m_TradeEdtMpcMoney.SetWindowText(szNowMoney);
            int nLen = strlen(szNowMoney);
            m_TradeEdtMpcMoney.SetSel(nLen, nLen);
        }
        m_TradeEdtMpcMoney.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_TradeStaNpcName.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TradeStaNpcMoney.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TradeStaMpcName.Show(m_Pnt.x, m_Pnt.y);
        // Show the button
        m_MpcAgreeChk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcAgreeChk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MoneyMpcOkChk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MoneyNpcOkChk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TradeCloseB.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgTrade::OnTradeBtnClose()
{
    DXPlaySound ( "Sound/Dlg_Clear.wav" ) ;
    g_objHero.QuitTrade () ;
}

void CDlgTrade::SetMpcGridContain()
{
    m_TradeGriMpc.RemoveIcon ( 0, 0 , true ) ;
    int nAmount = g_objHero.GetHeroTradeItemAmount () ;
    if ( nAmount == 20 )
    {
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10390 ) ) ;
        m_TradeGriMpc.SetEnable ( false ) ;
    }
    for ( int i = 0; i < nAmount; i++ )
    {
        OBJID ItemTypeId = g_objHero.GetHeroTradeItemByIndex ( i )->GetTypeID () ;
        m_TradeGriMpc.InsertIcon ( 1, 1, ItemTypeId, i, ICON_TYPE_TRADE ) ;
    }
}

void CDlgTrade::SetNpcGridContain()
{
    m_TradeGriNpc.RemoveIcon ( 0, 0 , true ) ;
    int nAmount = g_objHero.GetPlayerTradeItemAmount () ;
    for ( int i = 0; i < nAmount; i++ )
    {
        OBJID ItemTypeId = g_objHero.GetPlayerTradeItemByIndex ( i )->GetTypeID () ;
        m_TradeGriNpc.InsertIcon ( 1, 1, ItemTypeId, i, ICON_TYPE_TRADE ) ;
    }
}

HBRUSH CDlgTrade::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

void CDlgTrade::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}


void CDlgTrade::SetNpcMoney()
{
    if (m_MoneyNpcOkChk.GetCheckValue() == 0)
    {
        m_MoneyNpcOkChk.SetCheckValue(1);
    }
    int nNpcMoney = g_objHero.GetPlayerTradeMoney ( ) ;
    char strMoney[16] ;
    itoa ( nNpcMoney, strMoney, 10 ) ;
    m_TradeStaNpcMoney.SetWindowText ( strMoney ) ;
}

void CDlgTrade::OnTradeBtnMpcagree()
{
    char strMoney[16];
    m_TradeEdtMpcMoney.GetWindowText(strMoney, 16);
    int nMpcMoney = atoi(strMoney);
    if ( !m_bMoneyAdd && nMpcMoney > 0)
    {
        g_objGameMsg.AddMsg ( ( char* )g_objGameDataSet.GetStr ( 10046 ) ) ;    //"您未确定交易金额" ) ;
    }
    else
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        g_objHero.AgreeTrade () ;
        m_MpcAgreeChk.SetCurFrame(1);
        m_MpcAgreeChk.EnableWindow(false);
        m_TradeGriMpc.SetEnable( false ) ;
        m_TradeEdtMpcMoney.EnableWindow ( false ) ;
    }
}

void CDlgTrade::ResetTrade()
{
    m_TradeGriNpc.RemoveIcon ( 0, 0, true ) ;
    m_TradeGriMpc.RemoveIcon ( 0, 0, true ) ;
    m_TradeGriMpc.SetEnable( true ) ;
    m_MpcAgreeChk.EnableWindow(true);
    m_MpcAgreeChk.SetCurFrame(0);
    m_NpcAgreeChk.EnableWindow(false);
    m_NpcAgreeChk.SetCheckValue(0) ;
    m_MoneyMpcOkChk.EnableWindow(true);
    m_MoneyMpcOkChk.SetCurFrame(0);
    m_MoneyNpcOkChk.SetCheckValue(0);
    m_MoneyNpcOkChk.EnableWindow(false);
    m_TradeEdtMpcMoney.EnableWindow(true) ;
    m_TradeEdtMpcMoney.SetWindowText("0");
    m_TradeStaNpcMoney.SetWindowText ("") ;
    m_bMoneyAdd = false ;
}

void CDlgTrade::OnTradeCloseb()
{
    DXPlaySound( "Sound/window_close.wav" );
    CDlgTrade::OnTradeBtnClose () ;
}

void CDlgTrade::OnBtnHelp ()
{
    CDlgTrade::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_TRADE ) ;
}

void CDlgTrade::OnTradeBtnMoneynpcok()
{
    // TODO: Add your control notification handler code here
}

void CDlgTrade::OnTradeBtnMoneympcok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    char strMpcMoney[16] ;
    m_TradeEdtMpcMoney.GetWindowText ( strMpcMoney, 16 ) ;
    int nMpcMoney = atoi ( strMpcMoney ) ;
    if ( nMpcMoney > 0 && nMpcMoney <= (int)g_objHero.GetMoney () )
    {
        m_TradeEdtMpcMoney.EnableWindow ( false ) ;
        m_MoneyMpcOkChk.SetCurFrame(1);
        m_MoneyMpcOkChk.EnableWindow(false);
        g_objHero.AddHeroTradeMoney ( nMpcMoney ) ;
        m_bMoneyAdd = TRUE ;
    }
    else if (nMpcMoney >  (int)g_objHero.GetMoney())
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10045));
    }
    else if (nMpcMoney < 0)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10028));
    }
}

void CDlgTrade::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp)
    {
        CDlgTrade::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
    }
    CDialog::OnRButtonDown(nFlags, point);
}

