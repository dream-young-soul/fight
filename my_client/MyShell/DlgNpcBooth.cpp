
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgNpcBooth.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgNpcBooth.h"

#include "Hero.h"
#include "Item.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcBooth dialog


CDlgNpcBooth::CDlgNpcBooth(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgNpcBooth::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgNpcBooth)
    m_bShow = false ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nCurBase = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgNpcBooth::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgNpcBooth)
    DDX_Control(pDX, IDC_STA_PRICE11, m_StaPrice1);
    DDX_Control(pDX, IDC_STA_PRICE22, m_StaPrice2);
    DDX_Control(pDX, IDC_STA_PRICE33, m_StaPrice3);
    DDX_Control(pDX, IDC_STA_PRICE44, m_StaPrice4);
    DDX_Control(pDX, IDC_STA_PRICE55, m_StaPrice5);
    DDX_Control(pDX, IDC_STA_PRICE66, m_StaPrice6);
    DDX_Control(pDX, IDC_NPCBOOTH_IMA_GOOD1, m_NpcBoothImaGood1);
    DDX_Control(pDX, IDC_NPCBOOTH_IMA_GOOD2, m_NpcBoothImaGood2);
    DDX_Control(pDX, IDC_NPCBOOTH_IMA_GOOD3, m_NpcBoothImaGood3);
    DDX_Control(pDX, IDC_NPCBOOTH_IMA_GOOD4, m_NpcBoothImaGood4);
    DDX_Control(pDX, IDC_NPCBOOTH_IMA_GOOD5, m_NpcBoothImaGood5);
    DDX_Control(pDX, IDC_NPCBOOTH_IMA_GOOD6, m_NpcBoothImaGood6);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_CLOSE, m_NpcBoothBtnClose);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_UP, m_NpcBoothBtnUp);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_DOWN, m_NpcBoothBtnDown);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_BUY1, m_BtnBuy1);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_BUY2, m_BtnBuy2);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_BUY3, m_BtnBuy3);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_BUY4, m_BtnBuy4);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_BUY5, m_BtnBuy5);
    DDX_Control(pDX, IDC_NPCBOOTH_BTN_BUY6, m_BtnBuy6);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNpcBooth, CDialog)
    //{{AFX_MSG_MAP(CDlgNpcBooth)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_CLOSE, OnNpcboothBtnClose)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_UP, OnNpcboothBtnUp)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_DOWN, OnNpcboothBtnDown)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_BUY1, OnNpcboothBtnBuy1)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_BUY2, OnNpcboothBtnBuy2)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_BUY3, OnNpcboothBtnBuy3)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_BUY4, OnNpcboothBtnBuy4)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_BUY5, OnNpcboothBtnBuy5)
    ON_BN_CLICKED(IDC_NPCBOOTH_BTN_BUY6, OnNpcboothBtnBuy6)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcBooth message handlers

LRESULT CDlgNpcBooth::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if ( wParam )
        {
            CRect rect ;
            CDlgNpcBooth::GetWindowRect ( rect ) ;
            if ( m_nFrameMode == 0 )
            {
                DXPlaySound ( "Sound/Open.wav" ) ;
                m_dwLastShowTime = 0 ;
                m_nFrameMode = 1 ;
                CDlgNpcBooth::MoveWindow ( -rect.Width(), m_Pnt.y, rect.Width(), rect.Height(), true ) ;
            }
            else if ( m_nFrameMode == 2 )
            {
                CDlgNpcBooth::MoveWindow ( 0, m_Pnt.y, rect.Width(), rect.Height(), true ) ;
                m_nFrameMode = 0 ;
            }
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgNpcBooth::OnNpcboothBtnClose();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgNpcBooth::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgNpcBooth::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgNpcBooth::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float ) rect.Width () / DLG_FRAME_AMOUNT ;
    // Init the button
    m_BtnBuy1.Init ( rect.left, rect.top, "Button342" ) ;
    m_BtnBuy2.Init ( rect.left, rect.top, "Button342" ) ;
    m_BtnBuy3.Init ( rect.left, rect.top, "Button342" ) ;
    m_BtnBuy4.Init ( rect.left, rect.top, "Button342" ) ;
    m_BtnBuy5.Init ( rect.left, rect.top, "Button342" ) ;
    m_BtnBuy6.Init ( rect.left, rect.top, "Button342" ) ;
    m_NpcBoothBtnUp.Init ( rect.left, rect.top, "Button815" ) ;
    m_NpcBoothBtnDown.Init ( rect.left, rect.top, "Button816" ) ;
    m_NpcBoothBtnClose.Init ( rect.left, rect.top, "Button540" ) ;
    // Init the image
    m_NpcBoothImaGood1.Init ( rect.left, rect.top , NULL, 8 ) ;
    m_NpcBoothImaGood1.SetItemPos(DLG_NPCBOOTH) ;
    m_NpcBoothImaGood2.Init ( rect.left, rect.top , NULL, 8 ) ;
    m_NpcBoothImaGood2.SetItemPos(DLG_NPCBOOTH) ;
    m_NpcBoothImaGood3.Init ( rect.left, rect.top , NULL, 8 ) ;
    m_NpcBoothImaGood3.SetItemPos(DLG_NPCBOOTH) ;
    m_NpcBoothImaGood4.Init ( rect.left, rect.top , NULL, 8 ) ;
    m_NpcBoothImaGood4.SetItemPos(DLG_NPCBOOTH) ;
    m_NpcBoothImaGood5.Init ( rect.left, rect.top , NULL, 8 ) ;
    m_NpcBoothImaGood5.SetItemPos(DLG_NPCBOOTH) ;
    m_NpcBoothImaGood6.Init ( rect.left, rect.top , NULL, 8 ) ;
    m_NpcBoothImaGood6.SetItemPos(DLG_NPCBOOTH) ;
    // Init the static
    m_StaPrice1.Init ( rect.left, rect.top ) ;
    m_StaPrice2.Init ( rect.left, rect.top ) ;
    m_StaPrice3.Init ( rect.left, rect.top ) ;
    m_StaPrice4.Init ( rect.left, rect.top ) ;
    m_StaPrice5.Init ( rect.left, rect.top ) ;
    m_StaPrice6.Init ( rect.left, rect.top ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNpcBooth::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgNpcBooth::Show()
{
    if ( m_bShow )
    {
        switch ( m_nFrameMode )
        {
        case 0:
            break ;
        case 1:
            if ( m_dwLastShowTime == 0 )
            {
                m_dwLastShowTime = TimeGet() ;
                m_Pnt.x = m_nPntX ;
            }
            else
            {
                if ( TimeGet() - m_dwLastShowTime <= 300 )
                {
                    m_Pnt.x = m_nPntX ;
                    m_Pnt.x +=(LONG)( ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep) ;
                }
                else
                {
                    m_Pnt.x = 0;
                    m_nFrameMode = 2 ;
                    CDlgNpcBooth::WindowProc(WM_ENABLE, true, NULL);
                }
            }
            break ;
        case 2:
            break ;
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog34",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x ,
                           m_Pnt.y  );
            ShowAni->Show( 1,
                           m_Pnt.x + 128 ,
                           m_Pnt.y  );
            ShowAni->Show( 2,
                           m_Pnt.x + 256,
                           m_Pnt.y );
            ShowAni->Show( 3,
                           m_Pnt.x ,
                           m_Pnt.y + 128 );
            ShowAni->Show( 4,
                           m_Pnt.x + 256,
                           m_Pnt.y + 128 );
            ShowAni->Show( 5,
                           m_Pnt.x + 256,
                           m_Pnt.y + 256 );
        }
        else
        {
            return ;
        }
        // Show the buttons
        m_BtnBuy1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnBuy2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnBuy3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnBuy4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnBuy5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnBuy6.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothBtnUp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothBtnDown.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the image
        m_NpcBoothImaGood1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothImaGood2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothImaGood3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothImaGood4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothImaGood5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_NpcBoothImaGood6.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_StaPrice1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice6.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgNpcBooth::SetNpcBooth()
{
    m_nCurBase = 0 ;
    CDlgNpcBooth::ClearNpcBooth () ;
    int nAmount = g_objHero.m_objBooth.GetItemAmount () ;
    nAmount = ( nAmount > m_nCurBase + 6 ) ? m_nCurBase + 6 : nAmount ;
    for ( int i = m_nCurBase; i < nAmount; i++ )
    {
        CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( i ) ;
        if ( pItem )
        {
            char strName[32] ;
            char strPrice[32] ;
            sprintf ( strName, "Item%u", pItem->GetTypeID () ) ;
            itoa ( pItem->GetBoothSellPrice (), strPrice, 10 ) ;
            strcpy ( strPrice, CMyShellApp::StringChange ( strPrice, 2 ) ) ;
            switch ( i - m_nCurBase )
            {
            case 0:
                m_NpcBoothImaGood1.InsertImage ( strName, pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice1.SetWindowText ( strPrice ) ;
                break ;
            case 1:
                m_NpcBoothImaGood2.InsertImage ( strName, pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice2.SetWindowText ( strPrice ) ;
                break ;
            case 2:
                m_NpcBoothImaGood3.InsertImage ( strName, pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice3.SetWindowText ( strPrice ) ;
                break ;
            case 3:
                m_NpcBoothImaGood4.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice4.SetWindowText ( strPrice ) ;
                break ;
            case 4:
                m_NpcBoothImaGood5.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice5.SetWindowText ( strPrice ) ;
                break ;
            case 5:
                m_NpcBoothImaGood6.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice6.SetWindowText ( strPrice ) ;
                break ;
            }
        }
    }
}

void CDlgNpcBooth::ClearNpcBooth()
{
    m_NpcBoothImaGood1.RemoveImage () ;
    m_NpcBoothImaGood2.RemoveImage () ;
    m_NpcBoothImaGood3.RemoveImage () ;
    m_NpcBoothImaGood4.RemoveImage () ;
    m_NpcBoothImaGood5.RemoveImage () ;
    m_NpcBoothImaGood6.RemoveImage () ;
    m_StaPrice1.SetWindowText ( NULL ) ;
    m_StaPrice2.SetWindowText ( NULL ) ;
    m_StaPrice3.SetWindowText ( NULL ) ;
    m_StaPrice4.SetWindowText ( NULL ) ;
    m_StaPrice5.SetWindowText ( NULL ) ;
    m_StaPrice6.SetWindowText ( NULL ) ;
}

void CDlgNpcBooth::OnNpcboothBtnClose()
{
    DXPlaySound("Sound/window_close.wav");
    CDlgNpcBooth::EnableWindow(false) ;
    CDlgNpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_NPCBOOTH_CLOSE ) ;
}


void CDlgNpcBooth::OnNpcboothBtnUp()
{
    DXPlaySound("Sound/up_Page.wav");
    if ( m_nCurBase > 0 )
    {
        CDlgNpcBooth::ClearNpcBooth () ;
        m_nCurBase -= 6 ;
        for ( int i = m_nCurBase; i < m_nCurBase + 6; i++ )
        {
            CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( i ) ;
            if ( pItem )
            {
                char strName[32] ;
                char strPrice[32] ;
                sprintf ( strName, "Item%u", pItem->GetTypeID () ) ;
                itoa ( pItem->GetBoothSellPrice (), strPrice, 10 ) ;
                strcpy ( strPrice, CMyShellApp::StringChange ( strPrice, 2 ) ) ;
                switch ( i - m_nCurBase )
                {
                case 0:
                    m_NpcBoothImaGood1.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice1.SetWindowText ( strPrice ) ;
                    break ;
                case 1:
                    m_NpcBoothImaGood2.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice2.SetWindowText ( strPrice ) ;
                    break ;
                case 2:
                    m_NpcBoothImaGood3.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice3.SetWindowText ( strPrice ) ;
                    break ;
                case 3:
                    m_NpcBoothImaGood4.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice4.SetWindowText ( strPrice ) ;
                    break ;
                case 4:
                    m_NpcBoothImaGood5.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice5.SetWindowText ( strPrice ) ;
                    break ;
                case 5:
                    m_NpcBoothImaGood6.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice6.SetWindowText ( strPrice ) ;
                    break ;
                }
            }
        }
    }
}

void CDlgNpcBooth::OnNpcboothBtnDown()
{
    DXPlaySound("Sound/up_Page.wav");
    int nAmount = g_objHero.m_objBooth.GetItemAmount () ;
    if ( nAmount > m_nCurBase + 6 )
    {
        CDlgNpcBooth::ClearNpcBooth () ;
        m_nCurBase += 6 ;
        nAmount = ( nAmount > m_nCurBase + 6 ) ? m_nCurBase + 6 : nAmount ;
        for ( int i = m_nCurBase; i < nAmount; i++ )
        {
            CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( i ) ;
            if ( pItem )
            {
                char strName[32] ;
                char strPrice[32] ;
                sprintf ( strName, "Item%u", pItem->GetTypeID () ) ;
                itoa ( pItem->GetBoothSellPrice (), strPrice, 10 ) ;
                strcpy ( strPrice, CMyShellApp::StringChange ( strPrice, 2 ) ) ;
                switch ( i - m_nCurBase )
                {
                case 0:
                    m_NpcBoothImaGood1.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice1.SetWindowText ( strPrice ) ;
                    break ;
                case 1:
                    m_NpcBoothImaGood2.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice2.SetWindowText ( strPrice ) ;
                    break ;
                case 2:
                    m_NpcBoothImaGood3.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice3.SetWindowText ( strPrice ) ;
                    break ;
                case 3:
                    m_NpcBoothImaGood4.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice4.SetWindowText ( strPrice ) ;
                    break ;
                case 4:
                    m_NpcBoothImaGood5.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice5.SetWindowText ( strPrice ) ;
                    break ;
                case 5:
                    m_NpcBoothImaGood6.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice6.SetWindowText ( strPrice ) ;
                    break ;
                }
            }
        }
    }
}

void CDlgNpcBooth::OnNpcboothBtnBuy1()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( m_nCurBase ) ;
    if ( pItem )
    {
        g_objHero.m_objBooth.BuyItem ( pItem->GetID () ) ;
    }
}

void CDlgNpcBooth::OnNpcboothBtnBuy2()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( m_nCurBase + 1 ) ;
    if ( pItem )
    {
        g_objHero.m_objBooth.BuyItem ( pItem->GetID () ) ;
    }
}

void CDlgNpcBooth::OnNpcboothBtnBuy3()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( m_nCurBase + 2 ) ;
    if ( pItem )
    {
        g_objHero.m_objBooth.BuyItem ( pItem->GetID () ) ;
    }
}

void CDlgNpcBooth::OnNpcboothBtnBuy4()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( m_nCurBase + 3 ) ;
    if ( pItem )
    {
        g_objHero.m_objBooth.BuyItem ( pItem->GetID () ) ;
    }
}

void CDlgNpcBooth::OnNpcboothBtnBuy5()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( m_nCurBase + 4 ) ;
    if ( pItem )
    {
        g_objHero.m_objBooth.BuyItem ( pItem->GetID () ) ;
    }
}

void CDlgNpcBooth::OnNpcboothBtnBuy6()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CItem* pItem = g_objHero.m_objBooth.GetItemByIndex ( m_nCurBase + 5 ) ;
    if ( pItem )
    {
        g_objHero.m_objBooth.BuyItem ( pItem->GetID () ) ;
    }
}

int CDlgNpcBooth::GetMouseMoveIndex()
{
    if ( m_NpcBoothImaGood1.m_bMouseMove )
    {
        return m_nCurBase ;
    }
    if ( m_NpcBoothImaGood2.m_bMouseMove )
    {
        return m_nCurBase + 1;
    }
    if ( m_NpcBoothImaGood3.m_bMouseMove )
    {
        return m_nCurBase + 2;
    }
    if ( m_NpcBoothImaGood4.m_bMouseMove )
    {
        return m_nCurBase + 3;
    }
    if ( m_NpcBoothImaGood5.m_bMouseMove )
    {
        return m_nCurBase + 4;
    }
    if ( m_NpcBoothImaGood6.m_bMouseMove )
    {
        return m_nCurBase + 5;
    }
    return -1 ;
}
