
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMpcBooth.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMpcBooth.h"

#include "Item.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMpcBooth dialog


CDlgMpcBooth::CDlgMpcBooth(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMpcBooth::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMpcBooth)
    m_bShow = false ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_bPickUp = false ;
    m_nIconRealID = 0 ;
    m_nCurBase = 0 ;
    m_nCurRealID = 0 ;
    m_nCurChangeIndex = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgMpcBooth::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMpcBooth)
    DDX_Control(pDX, IDC_BTN_CLOSEBOOTH, m_BtnCloseBooth);
    DDX_Control(pDX, IDC_STA_PRICE1, m_StaPrice1);
    DDX_Control(pDX, IDC_STA_PRICE2, m_StaPrice2);
    DDX_Control(pDX, IDC_STA_PRICE3, m_StaPrice3);
    DDX_Control(pDX, IDC_STA_PRICE4, m_StaPrice4);
    DDX_Control(pDX, IDC_STA_PRICE5, m_StaPrice5);
    DDX_Control(pDX, IDC_STA_PRICE6, m_StaPrice6);
    DDX_Control(pDX, IDC_MPCBOOTH_IMA_GOOD1, m_MpcBoothImaGood1);
    DDX_Control(pDX, IDC_MPCBOOTH_IMA_GOOD2, m_MpcBoothImaGood2);
    DDX_Control(pDX, IDC_MPCBOOTH_IMA_GOOD3, m_MpcBoothImaGood3);
    DDX_Control(pDX, IDC_MPCBOOTH_IMA_GOOD4, m_MpcBoothImaGood4);
    DDX_Control(pDX, IDC_MPCBOOTH_IMA_GOOD5, m_MpcBoothImaGood5);
    DDX_Control(pDX, IDC_MPCBOOTH_IMA_GOOD6, m_MpcBoothImaGood6);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_GETBACK1, m_BtnGetBack1);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_GETBACK2, m_BtnGetBack2);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_GETBACK3, m_BtnGetBack3);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_GETBACK4, m_BtnGetBack4);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_GETBACK5, m_BtnGetBack5);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_GETBACK6, m_BtnGetBack6);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_UP, m_MpcBoothBtnUp);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_HAWK, m_MpcBoothBtnHawk);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_DOWN, m_MpcBoothBtnDown);
    DDX_Control(pDX, IDC_MPCBOOTH_BTN_CLOSES, m_MpcBoothBtnCloseS);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMpcBooth, CDialog)
    //{{AFX_MSG_MAP(CDlgMpcBooth)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_CLOSES, OnMpcboothBtnCloses)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_UP, OnMpcboothBtnUp)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_DOWN, OnMpcboothBtnDown)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_HAWK, OnMpcboothBtnHawk)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_GETBACK1, OnMpcboothBtnGetback1)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_GETBACK2, OnMpcboothBtnGetback2)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_GETBACK3, OnMpcboothBtnGetback3)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_GETBACK4, OnMpcboothBtnGetback4)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_GETBACK5, OnMpcboothBtnGetback5)
    ON_BN_CLICKED(IDC_MPCBOOTH_BTN_GETBACK6, OnMpcboothBtnGetback6)
    ON_BN_CLICKED(IDC_MPCBOOTH_IMA_GOOD1, OnMpcboothImaGood1)
    ON_BN_CLICKED(IDC_MPCBOOTH_IMA_GOOD2, OnMpcboothImaGood2)
    ON_BN_CLICKED(IDC_MPCBOOTH_IMA_GOOD3, OnMpcboothImaGood3)
    ON_BN_CLICKED(IDC_MPCBOOTH_IMA_GOOD4, OnMpcboothImaGood4)
    ON_BN_CLICKED(IDC_MPCBOOTH_IMA_GOOD5, OnMpcboothImaGood5)
    ON_BN_CLICKED(IDC_MPCBOOTH_IMA_GOOD6, OnMpcboothImaGood6)
    ON_BN_CLICKED(IDC_BTN_CLOSEBOOTH, OnBtnClosebooth)
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMpcBooth message handlers

LRESULT CDlgMpcBooth::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if ( wParam )
        {
            CRect rect ;
            CDlgMpcBooth::GetWindowRect ( rect ) ;
            if ( m_nFrameMode == 0 )
            {
                DXPlaySound ( "Sound/Open.wav" ) ;
                m_dwLastShowTime = 0 ;
                m_nFrameMode = 1 ;
                CDlgMpcBooth::MoveWindow ( -rect.Width(), m_Pnt.y, rect.Width(), rect.Height(), true ) ;
            }
            else if ( m_nFrameMode == 2 )
            {
                CDlgMpcBooth::MoveWindow ( 0, m_Pnt.y, rect.Width(), rect.Height(), true ) ;
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
            CDlgMpcBooth::OnBtnClosebooth();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMpcBooth::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgMpcBooth::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMpcBooth::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float ) rect.Width () / DLG_FRAME_AMOUNT ;
    // Init the button
    m_MpcBoothBtnHawk.Init ( rect.left, rect.top, "Button343" ) ;
    m_MpcBoothBtnUp.Init ( rect.left, rect.top, "Button815" ) ;
    m_MpcBoothBtnDown.Init ( rect.left, rect.top, "Button816" ) ;
    m_MpcBoothBtnCloseS.Init ( rect.left, rect.top, "Button540" ) ;
    m_BtnCloseBooth.Init(rect.left, rect.top, "Button561");
    m_BtnGetBack1.Init ( rect.left, rect.top, "Button341" ) ;
    m_BtnGetBack2.Init ( rect.left, rect.top, "Button341" ) ;
    m_BtnGetBack3.Init ( rect.left, rect.top, "Button341" ) ;
    m_BtnGetBack4.Init ( rect.left, rect.top, "Button341" ) ;
    m_BtnGetBack5.Init ( rect.left, rect.top, "Button341" ) ;
    m_BtnGetBack6.Init ( rect.left, rect.top, "Button341" ) ;
    // Init the image
    m_MpcBoothImaGood1.Init ( rect.left, rect.top,  NULL, 8 ) ;
    m_MpcBoothImaGood1.SetItemPos(DLG_MPCBOOTH);
    m_MpcBoothImaGood2.Init ( rect.left, rect.top,  NULL, 8 ) ;
    m_MpcBoothImaGood2.SetItemPos(DLG_MPCBOOTH);
    m_MpcBoothImaGood3.Init ( rect.left, rect.top,  NULL, 8 ) ;
    m_MpcBoothImaGood3.SetItemPos(DLG_MPCBOOTH);
    m_MpcBoothImaGood4.Init ( rect.left, rect.top,  NULL, 8 ) ;
    m_MpcBoothImaGood4.SetItemPos(DLG_MPCBOOTH);
    m_MpcBoothImaGood5.Init ( rect.left, rect.top,  NULL, 8 ) ;
    m_MpcBoothImaGood5.SetItemPos(DLG_MPCBOOTH);
    m_MpcBoothImaGood6.Init ( rect.left, rect.top,  NULL, 8 ) ;
    m_MpcBoothImaGood6.SetItemPos(DLG_MPCBOOTH);
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

void CDlgMpcBooth::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgMpcBooth::Show()
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
                    CDlgMpcBooth::WindowProc ( WM_ENABLE, true, NULL ) ;
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
        m_MpcBoothBtnUp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothBtnHawk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothBtnDown.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothBtnCloseS.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnCloseBooth.Show(m_Pnt.x, m_Pnt.y);
        if ( m_MpcBoothImaGood1.GetImageLoadState () )
        {
            m_BtnGetBack1.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ( m_MpcBoothImaGood2.GetImageLoadState () )
        {
            m_BtnGetBack2.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ( m_MpcBoothImaGood3.GetImageLoadState () )
        {
            m_BtnGetBack3.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ( m_MpcBoothImaGood4.GetImageLoadState () )
        {
            m_BtnGetBack4.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ( m_MpcBoothImaGood5.GetImageLoadState () )
        {
            m_BtnGetBack5.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if ( m_MpcBoothImaGood6.GetImageLoadState () )
        {
            m_BtnGetBack6.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        // Show the image
        m_MpcBoothImaGood1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothImaGood2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothImaGood3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothImaGood4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothImaGood5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MpcBoothImaGood6.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_StaPrice1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaPrice6.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgMpcBooth::OnMpcboothBtnCloses()
{
    DXPlaySound("Sound/window_close.wav");
    CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_HIDE ) ;
}


void CDlgMpcBooth::OnMpcboothBtnUp()
{
    DXPlaySound("Sound/up_Page.wav");
    if ( m_nCurBase > 0 )
    {
        CDlgMpcBooth::ClearMpcBooteh () ;
        m_nCurBase -= 6 ;
        for ( int i = m_nCurBase; i < m_nCurBase + 6; i++ )
        {
            CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( i ) ;
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
                    m_MpcBoothImaGood1.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice1.SetWindowText ( strPrice ) ;
                    break ;
                case 1:
                    m_MpcBoothImaGood2.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice2.SetWindowText ( strPrice ) ;
                    break ;
                case 2:
                    m_MpcBoothImaGood3.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice3.SetWindowText ( strPrice ) ;
                    break ;
                case 3:
                    m_MpcBoothImaGood4.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice4.SetWindowText ( strPrice ) ;
                    break ;
                case 4:
                    m_MpcBoothImaGood5.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice5.SetWindowText ( strPrice ) ;
                    break ;
                case 5:
                    m_MpcBoothImaGood6.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice6.SetWindowText ( strPrice ) ;
                    break ;
                }
            }
        }
    }
}

void CDlgMpcBooth::OnMpcboothBtnDown()
{
    DXPlaySound("Sound/up_Page.wav");
    int nAmount = g_objHero.m_objBoothManager.GetItemAmount () ;
    if ( nAmount >= m_nCurBase + 6 )
    {
        CDlgMpcBooth::ClearMpcBooteh () ;
        m_nCurBase += 6 ;
        nAmount = ( nAmount > m_nCurBase + 6 ) ? m_nCurBase + 6 : nAmount ;
        for ( int i = m_nCurBase; i < nAmount; i++ )
        {
            CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( i ) ;
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
                    m_MpcBoothImaGood1.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice1.SetWindowText ( strPrice ) ;
                    break ;
                case 1:
                    m_MpcBoothImaGood2.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice2.SetWindowText ( strPrice ) ;
                    break ;
                case 2:
                    m_MpcBoothImaGood3.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice3.SetWindowText ( strPrice ) ;
                    break ;
                case 3:
                    m_MpcBoothImaGood4.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice4.SetWindowText ( strPrice ) ;
                    break ;
                case 4:
                    m_MpcBoothImaGood5.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice5.SetWindowText ( strPrice ) ;
                    break ;
                case 5:
                    m_MpcBoothImaGood6.InsertImage ( strName , 0, 0, true) ;
                    m_StaPrice6.SetWindowText ( strPrice ) ;
                    break ;
                }
            }
        }
    }
}


void CDlgMpcBooth::OnMpcboothBtnHawk()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_HAWK ) ;
}

/*void CDlgMpcBooth::OnMpcboothBtnChange1()
{
	if ( m_MpcBoothImaGood1.GetImageLoadState () )
	{
		m_nCurChangeIndex = m_nCurBase ;
		CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_CHANGE ) ;
	}
}*/

void CDlgMpcBooth::OnMpcboothBtnGetback1()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_MpcBoothImaGood1.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( m_nCurBase ) ;
        if ( pItem )
        {
            g_objHero.m_objBoothManager.DelItem ( pItem->GetID () ) ;
        }
    }
}


void CDlgMpcBooth::OnMpcboothBtnGetback2()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_MpcBoothImaGood1.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( m_nCurBase + 1 ) ;
        if ( pItem )
        {
            g_objHero.m_objBoothManager.DelItem ( pItem->GetID () ) ;
        }
    }
}


void CDlgMpcBooth::OnMpcboothBtnGetback3()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_MpcBoothImaGood1.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( m_nCurBase + 2 ) ;
        if ( pItem )
        {
            g_objHero.m_objBoothManager.DelItem ( pItem->GetID () ) ;
        }
    }
}


void CDlgMpcBooth::OnMpcboothBtnGetback4()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_MpcBoothImaGood1.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( m_nCurBase + 3 ) ;
        if ( pItem )
        {
            g_objHero.m_objBoothManager.DelItem ( pItem->GetID () ) ;
        }
    }
}


void CDlgMpcBooth::OnMpcboothBtnGetback5()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_MpcBoothImaGood1.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( m_nCurBase + 4 ) ;
        if ( pItem )
        {
            g_objHero.m_objBoothManager.DelItem ( pItem->GetID () ) ;
        }
    }
}



void CDlgMpcBooth::OnMpcboothBtnGetback6()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_MpcBoothImaGood1.GetImageLoadState () )
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( m_nCurBase + 5 ) ;
        if ( pItem )
        {
            g_objHero.m_objBoothManager.DelItem ( pItem->GetID () ) ;
        }
    }
}

void CDlgMpcBooth::OnMpcboothImaGood1()
{
    if ( g_bPickUp && !m_MpcBoothImaGood1.GetImageLoadState () )
    {
        m_nCurRealID = m_nIconRealID ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_ADDITEM ) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgMpcBooth::OnMpcboothImaGood2()
{
    if ( g_bPickUp && !m_MpcBoothImaGood2.GetImageLoadState () )
    {
        m_nCurRealID = m_nIconRealID ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_ADDITEM ) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgMpcBooth::OnMpcboothImaGood3()
{
    if ( g_bPickUp && !m_MpcBoothImaGood3.GetImageLoadState () )
    {
        m_nCurRealID = m_nIconRealID ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_ADDITEM ) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgMpcBooth::OnMpcboothImaGood4()
{
    if ( g_bPickUp && !m_MpcBoothImaGood4.GetImageLoadState () )
    {
        m_nCurRealID = m_nIconRealID ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_ADDITEM ) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgMpcBooth::OnMpcboothImaGood5()
{
    if ( g_bPickUp && !m_MpcBoothImaGood5.GetImageLoadState () )
    {
        m_nCurRealID = m_nIconRealID ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_ADDITEM ) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgMpcBooth::OnMpcboothImaGood6()
{
    if ( g_bPickUp && !m_MpcBoothImaGood6.GetImageLoadState () )
    {
        m_nCurRealID = m_nIconRealID ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_ADDITEM ) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
}

void CDlgMpcBooth::SetPickUpState(BOOL bPickUp, OBJID nIconRealID)
{
    m_bPickUp = bPickUp;
    m_nIconRealID = nIconRealID;
}

void CDlgMpcBooth::SetMpcBooth()
{
    CDlgMpcBooth::ClearMpcBooteh () ;
    int nAmount = g_objHero.m_objBoothManager.GetItemAmount () ;
    nAmount = ( nAmount > m_nCurBase + 6 ) ? m_nCurBase + 6 : nAmount ;
    for ( int i = m_nCurBase; i < nAmount; i++ )
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex ( i ) ;
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
                m_MpcBoothImaGood1.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice1.SetWindowText ( strPrice ) ;
                break ;
            case 1:
                m_MpcBoothImaGood2.InsertImage ( strName, pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice2.SetWindowText ( strPrice ) ;
                break ;
            case 2:
                m_MpcBoothImaGood3.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice3.SetWindowText ( strPrice ) ;
                break ;
            case 3:
                m_MpcBoothImaGood4.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice4.SetWindowText ( strPrice ) ;
                break ;
            case 4:
                m_MpcBoothImaGood5.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice5.SetWindowText ( strPrice ) ;
                break ;
            case 5:
                m_MpcBoothImaGood6.InsertImage ( strName , pItem->GetID (), pItem->GetTypeID (), true ) ;
                m_StaPrice6.SetWindowText ( strPrice ) ;
                break ;
            }
        }
    }
}

void CDlgMpcBooth::ClearMpcBooteh()
{
    m_MpcBoothImaGood1.RemoveImage () ;
    m_MpcBoothImaGood2.RemoveImage () ;
    m_MpcBoothImaGood3.RemoveImage () ;
    m_MpcBoothImaGood4.RemoveImage () ;
    m_MpcBoothImaGood5.RemoveImage () ;
    m_MpcBoothImaGood6.RemoveImage () ;
    m_StaPrice1.SetWindowText ( NULL ) ;
    m_StaPrice2.SetWindowText ( NULL ) ;
    m_StaPrice3.SetWindowText ( NULL ) ;
    m_StaPrice4.SetWindowText ( NULL ) ;
    m_StaPrice5.SetWindowText ( NULL ) ;
    m_StaPrice6.SetWindowText ( NULL ) ;
}

int CDlgMpcBooth::GetMouseMoveIndex()
{
    if ( m_MpcBoothImaGood1.m_bMouseMove )
    {
        return m_nCurBase ;
    }
    if ( m_MpcBoothImaGood2.m_bMouseMove )
    {
        return m_nCurBase + 1;
    }
    if ( m_MpcBoothImaGood3.m_bMouseMove )
    {
        return m_nCurBase + 2;
    }
    if ( m_MpcBoothImaGood4.m_bMouseMove )
    {
        return m_nCurBase + 3;
    }
    if ( m_MpcBoothImaGood5.m_bMouseMove )
    {
        return m_nCurBase + 4;
    }
    if ( m_MpcBoothImaGood6.m_bMouseMove )
    {
        return m_nCurBase + 5;
    }
    return -1 ;
}

void CDlgMpcBooth::OnBtnClosebooth()
{
    // TODO: Add your control notification handler code here
    //	g_objHero.m_objBoothManager.Close();
    CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_HIDE ) ;
}

void CDlgMpcBooth::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_bPickUp
            && ( !m_MpcBoothImaGood1.GetImageLoadState ()
                 || !m_MpcBoothImaGood2.GetImageLoadState ()
                 || !m_MpcBoothImaGood3.GetImageLoadState ()
                 || !m_MpcBoothImaGood4.GetImageLoadState ()
                 || !m_MpcBoothImaGood5.GetImageLoadState ()
                 || !m_MpcBoothImaGood6.GetImageLoadState ()))
    {
        m_nCurRealID = m_nIconRealID ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTH_ADDITEM ) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP,  DLG_ALL) ;
        CDlgMpcBooth::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
    }
    CDialog::OnLButtonDown(nFlags, point);
}
