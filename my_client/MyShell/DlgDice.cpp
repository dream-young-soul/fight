
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgDice.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgDice.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDice dialog


CDlgDice::CDlgDice(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgDice::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgDice)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_btChipType = 0 ;
    m_nCurChipIn = -1 ;
    m_nChipIndex = 0 ;
    m_nShowChipIndex = DICE_VALUE_ALL ;
    //}}AFX_DATA_INIT
}


void CDlgDice::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgDice)
    DDX_Control(pDX, IDC_DICE_BTN_RESUME, m_DiceBtnResume);
    DDX_Control(pDX, IDC_DICE_STA_CHIP, m_DiceStaChip);
    DDX_Control(pDX, IDC_DICE_BTN_DICELIST, m_DiceBtnDiceList);
    DDX_Control(pDX, IDC_DICE_STA_MPC, m_DiceStaMpc);
    DDX_Control(pDX, IDC_DICE_STA_ALL, m_DiceStaAll);
    DDX_Control(pDX, IDC_DICE_BTN_CHIP100, m_DiceBtnChip100);
    DDX_Control(pDX, IDC_DICE_BTN_CHIP1000, m_DiceBtnChip1000);
    DDX_Control(pDX, IDC_DICE_BTN_CHIP10000, m_DiceBtnChip10000);
    DDX_Control(pDX, IDC_DICE_IMA_SMALL, m_DiceImaSmall);
    DDX_Control(pDX, IDC_DICE_IMA_BIG, m_DiceImaBig);
    DDX_Control(pDX, IDC_DICE_BTN_HIDE, m_DiceBtnHide);
    DDX_Control(pDX, IDC_DICE_BTN_CHIPIN, m_DiceBtnChipin);
    DDX_Control(pDX, IDC_DICE_IMA_ALL1, m_DiceImaAll1);
    DDX_Control(pDX, IDC_DICE_IMA_ALL2, m_DiceImaAll2);
    DDX_Control(pDX, IDC_DICE_IMA_ALL3, m_DiceImaAll3);
    DDX_Control(pDX, IDC_DICE_IMA_ALL4, m_DiceImaAll4);
    DDX_Control(pDX, IDC_DICE_IMA_ALL5, m_DiceImaAll5);
    DDX_Control(pDX, IDC_DICE_IMA_ALL6, m_DiceImaAll6);
    DDX_Control(pDX, IDC_DICE_IMA_4, m_DiceIma4);
    DDX_Control(pDX, IDC_DICE_IMA_5, m_DiceIma5);
    DDX_Control(pDX, IDC_DICE_IMA_6, m_DiceIma6);
    DDX_Control(pDX, IDC_DICE_IMA_7, m_DiceIma7);
    DDX_Control(pDX, IDC_DICE_IMA_8, m_DiceIma8);
    DDX_Control(pDX, IDC_DICE_IMA_9, m_DiceIma9);
    DDX_Control(pDX, IDC_DICE_IMA_10, m_DiceIma10);
    DDX_Control(pDX, IDC_DICE_IMA_11, m_DiceIma11);
    DDX_Control(pDX, IDC_DICE_IMA_12, m_DiceIma12);
    DDX_Control(pDX, IDC_DICE_IMA_13, m_DiceIma13);
    DDX_Control(pDX, IDC_DICE_IMA_14, m_DiceIma14);
    DDX_Control(pDX, IDC_DICE_IMA_15, m_DiceIma15);
    DDX_Control(pDX, IDC_DICE_IMA_16, m_DiceIma16);
    DDX_Control(pDX, IDC_DICE_IMA_17, m_DiceIma17);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDice, CDialog)
    //{{AFX_MSG_MAP(CDlgDice)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_DICE_IMA_ALL1, OnDiceImaAll1)
    ON_BN_CLICKED(IDC_DICE_IMA_ALL2, OnDiceImaAll2)
    ON_BN_CLICKED(IDC_DICE_IMA_ALL3, OnDiceImaAll3)
    ON_BN_CLICKED(IDC_DICE_IMA_ALL4, OnDiceImaAll4)
    ON_BN_CLICKED(IDC_DICE_IMA_ALL5, OnDiceImaAll5)
    ON_BN_CLICKED(IDC_DICE_IMA_ALL6, OnDiceImaAll6)
    ON_BN_CLICKED(IDC_DICE_IMA_4, OnDiceIma4)
    ON_BN_CLICKED(IDC_DICE_IMA_5, OnDiceIma5)
    ON_BN_CLICKED(IDC_DICE_IMA_6, OnDiceIma6)
    ON_BN_CLICKED(IDC_DICE_IMA_7, OnDiceIma7)
    ON_BN_CLICKED(IDC_DICE_IMA_8, OnDiceIma8)
    ON_BN_CLICKED(IDC_DICE_IMA_9, OnDiceIma9)
    ON_BN_CLICKED(IDC_DICE_IMA_10, OnDiceIma10)
    ON_BN_CLICKED(IDC_DICE_IMA_11, OnDiceIma11)
    ON_BN_CLICKED(IDC_DICE_IMA_12, OnDiceIma12)
    ON_BN_CLICKED(IDC_DICE_IMA_13, OnDiceIma13)
    ON_BN_CLICKED(IDC_DICE_IMA_14, OnDiceIma14)
    ON_BN_CLICKED(IDC_DICE_IMA_15, OnDiceIma15)
    ON_BN_CLICKED(IDC_DICE_IMA_16, OnDiceIma16)
    ON_BN_CLICKED(IDC_DICE_IMA_17, OnDiceIma17)
    ON_BN_CLICKED(IDC_DICE_IMA_SMALL, OnDiceImaSmall)
    ON_BN_CLICKED(IDC_DICE_BTN_CHIPIN, OnDiceBtnChipin)
    ON_BN_CLICKED(IDC_DICE_BTN_HIDE, OnDiceBtnHide)
    ON_BN_CLICKED(IDC_DICE_IMA_BIG, OnDiceImaBig)
    ON_BN_CLICKED(IDC_DICE_BTN_CHIP100, OnDiceBtnChip100)
    ON_BN_CLICKED(IDC_DICE_BTN_CHIP1000, OnDiceBtnChip1000)
    ON_BN_CLICKED(IDC_DICE_BTN_CHIP10000, OnDiceBtnChip10000)
    ON_WM_SETCURSOR()
    ON_BN_CLICKED(IDC_DICE_BTN_DICELIST, OnDiceBtnDicelist)
    ON_WM_RBUTTONDOWN()
    ON_BN_CLICKED(IDC_DICE_BTN_RESUME, OnDiceBtnResume)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDice message handlers

HBRUSH CDlgDice::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgDice::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgDice::GetWindowRect ( rect ) ;
    // Init the image
    m_DiceImaAll1.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_111  ) ;
    m_DiceImaAll2.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_222  ) ;
    m_DiceImaAll3.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_333  ) ;
    m_DiceImaAll4.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_444  ) ;
    m_DiceImaAll5.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_555  ) ;
    m_DiceImaAll6.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_666  ) ;
    m_DiceIma4.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_4  ) ;
    m_DiceIma5.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_5  ) ;
    m_DiceIma6.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_6  ) ;
    m_DiceIma7.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_7  ) ;
    m_DiceIma8.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_8  ) ;
    m_DiceIma9.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_9  ) ;
    m_DiceIma10.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_10  ) ;
    m_DiceIma11.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_11  ) ;
    m_DiceIma12.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_12  ) ;
    m_DiceIma13.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_13  ) ;
    m_DiceIma14.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_14  ) ;
    m_DiceIma15.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_15  ) ;
    m_DiceIma16.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_16  ) ;
    m_DiceIma17.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_17  ) ;
    m_DiceImaBig.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_BIG  ) ;
    m_DiceImaSmall.Init ( rect.left, rect.top, NULL, 0, DICE_VALUE_SMALL  ) ;
    // Init the button
    m_DiceBtnChipin.Init ( rect.left, rect.top, "Button430" ) ;
    m_DiceBtnHide.Init ( rect.left, rect.top, "Button431" ) ;
    m_DiceBtnChip100.Init ( rect.left, rect.top, "Button433" ) ;
    m_DiceBtnChip1000.Init ( rect.left, rect.top, "Button434" ) ;
    m_DiceBtnChip10000.Init ( rect.left, rect.top, "Button435" ) ;
    m_DiceBtnDiceList.Init ( rect.left, rect.top, "Button382" ) ;
    m_DiceBtnResume.Init ( rect.left, rect.top, "Button436" ) ;
    m_DiceBtnResume.EnableWindow ( false ) ;
    // Init the static
    m_DiceStaAll.Init ( rect.left, rect.top ) ;
    m_DiceStaMpc.Init ( rect.left, rect.top ) ;
    m_DiceStaChip.Init ( rect.left, rect.top ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDice::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgDice::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog43",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y + 2 ) ;
        else
        {
            return ;
        }
        // Show the image
        m_DiceImaAll1.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceImaAll2.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceImaAll3.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceImaAll4.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceImaAll5.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceImaAll6.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma4.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma5.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma6.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma7.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma8.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma9.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma10.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma11.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma12.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma13.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma14.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma15.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma16.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceIma17.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceImaBig.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceImaSmall.ShowMoney ( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_DiceBtnChipin.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceBtnHide.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceBtnChip100.Show ( m_Pnt.x + 4, m_Pnt.y + 4 ) ;
        m_DiceBtnChip1000.Show ( m_Pnt.x + 4, m_Pnt.y + 4 ) ;
        m_DiceBtnChip10000.Show ( m_Pnt.x + 4, m_Pnt.y + 4 ) ;
        m_DiceBtnDiceList.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Set the chip money
        CPoint MousePnt ;
        GetCursorPos ( &MousePnt ) ;
        if ( m_ChipRect[0].PtInRect ( MousePnt )  )
        {
            if ( m_nChipIndex != 1 )
            {
                m_DiceStaChip.SetWindowText ( "100" ) ;
                m_nChipIndex = 1 ;
            }
        }
        else if ( m_ChipRect[1].PtInRect ( MousePnt ) )
        {
            if ( m_nChipIndex != 2 )
            {
                m_DiceStaChip.SetWindowText ( "1000" ) ;
                m_nChipIndex = 2 ;
            }
        }
        else if ( m_ChipRect[2].PtInRect ( MousePnt ) )
        {
            if ( m_nChipIndex != 3 )
            {
                m_DiceStaChip.SetWindowText ( "10000" ) ;
                m_nChipIndex = 3 ;
            }
        }
        else if ( m_nChipIndex != 0 )
        {
            m_DiceStaChip.SetWindowText ( NULL ) ;
            m_nChipIndex = 0 ;
        }
        // Show the static
        m_DiceStaAll.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceStaMpc.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_DiceStaChip.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the resume button
        if ( m_DiceBtnResume.IsWindowEnabled () )
        {
            m_DiceBtnResume.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        // Test if the mouse is out off the chip area
        if ( m_nShowChipIndex != DICE_VALUE_ALL )
        {
            CPoint MousePnt ;
            CRect rect ;
            CDlgDice::GetWindowRect ( rect ) ;
            rect.bottom -= 58 ;
            GetCursorPos ( &MousePnt ) ;
            if ( !rect.PtInRect ( MousePnt ) )
            {
                CDlgDice::PostMessage ( WM_MY_MESSAGE, DICE_VALUE_ALL ) ;
            }
        }
    }
}

LRESULT CDlgDice::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
        if ( m_bShow )
        {
            m_DiceBtnChip100.GetWindowRect ( m_ChipRect[0] ) ;
            m_DiceBtnChip1000.GetWindowRect ( m_ChipRect[1] ) ;
            m_DiceBtnChip10000.GetWindowRect ( m_ChipRect[2] ) ;
            if ( m_DiceBtnResume.IsWindowEnabled ()	)
            {
                CRect rect ;
                CDlgDice::GetWindowRect ( rect ) ;
                CDlgDice::MoveWindow ( _SCR_WIDTH - rect.Width (),
                                       0,
                                       rect.Width (),
                                       rect.Height () ) ;
                m_DiceBtnResume.EnableWindow ( false ) ;
                m_DiceBtnDiceList.EnableWindow ( true ) ;
                m_DiceBtnChip100.EnableWindow ( true ) ;
                m_DiceBtnChip1000.EnableWindow ( true ) ;
                m_DiceBtnChip10000.EnableWindow ( true ) ;
            }
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
    else if ( message == WM_MY_MESSAGE )
    {
        m_nShowChipIndex = wParam ;
        char strMoney [16] ;
        itoa ( g_objHero.m_objDiceManager.GetSelfAnte ( wParam ), strMoney, 10 ) ;
        m_DiceStaMpc.SetWindowText ( strMoney ) ;
        itoa ( g_objHero.m_objDiceManager.GetTotalAnte ( wParam ), strMoney, 10 ) ;
        m_DiceStaAll.SetWindowText ( strMoney ) ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgDice::OnDiceImaAll1()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_111, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_111 ;
    }
}

void CDlgDice::OnDiceImaAll2()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_222, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_222 ;
    }
}

void CDlgDice::OnDiceImaAll3()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_333, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_333 ;
    }
}

void CDlgDice::OnDiceImaAll4()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_444, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_444 ;
    }
}

void CDlgDice::OnDiceImaAll5()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_555, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_555 ;
    }
}

void CDlgDice::OnDiceImaAll6()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_666, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_666 ;
    }
}

void CDlgDice::OnDiceIma4()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_4, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_4 ;
    }
}

void CDlgDice::OnDiceIma5()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_5, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_5 ;
    }
}

void CDlgDice::OnDiceIma6()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_6, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_6 ;
    }
}

void CDlgDice::OnDiceIma7()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_7, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_7 ;
    }
}

void CDlgDice::OnDiceIma8()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_8, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_8 ;
    }
}

void CDlgDice::OnDiceIma9()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_9, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_9 ;
    }
}

void CDlgDice::OnDiceIma10()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_10, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_10 ;
    }
}

void CDlgDice::OnDiceIma11()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_11, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_11 ;
    }
}

void CDlgDice::OnDiceIma12()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_12, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_12 ;
    }
}

void CDlgDice::OnDiceIma13()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_13, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_13 ;
    }
}

void CDlgDice::OnDiceIma14()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_14, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_14 ;
    }
}

void CDlgDice::OnDiceIma15()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_15, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_15 ;
    }
}

void CDlgDice::OnDiceIma16()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_16, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_16 ;
    }
}

void CDlgDice::OnDiceIma17()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney = (DWORD)(pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_17, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_17 ;
    }
}

void CDlgDice::OnDiceImaSmall()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10 );
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_SMALL, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_SMALL ;
    }
}

void CDlgDice::OnDiceImaBig()
{
    if ( m_btChipType != 0 )
    {
        DWORD dwMoney =(DWORD)( pow ( 10.f, m_btChipType ) * 10) ;
        g_objHero.m_objDiceManager.ChipIn ( DICE_VALUE_BIG, dwMoney ) ;
        m_nCurChipIn = DICE_VALUE_BIG ;
    }
}

void CDlgDice::OnDiceBtnChip100()
{
    m_btChipType = 1 ;
}

void CDlgDice::OnDiceBtnChip1000()
{
    m_btChipType = 2 ;
}

void CDlgDice::OnDiceBtnChip10000()
{
    m_btChipType = 3 ;
}

void CDlgDice::OnDiceBtnChipin()
{
    g_objHero.m_objDiceManager.Close () ;
}

void CDlgDice::OnDiceBtnHide()
{
    CRect rect ;
    CDlgDice::GetWindowRect ( rect ) ;
    CDlgDice::MoveWindow ( _SCR_WIDTH - 74,
                           - rect.Height () + 30,
                           rect.Width (),
                           rect.Height () ) ;
    m_DiceBtnResume.EnableWindow ( true ) ;
    m_DiceBtnDiceList.EnableWindow ( false ) ;
    m_DiceBtnChip100.EnableWindow ( false ) ;
    m_DiceBtnChip1000.EnableWindow ( false ) ;
    m_DiceBtnChip10000.EnableWindow ( false ) ;
    CDlgDice::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_DICE_HIDE ) ;
}

void CDlgDice::OnDiceBtnResume()
{
    CRect rect ;
    CDlgDice::GetWindowRect ( rect ) ;
    CDlgDice::MoveWindow ( _SCR_WIDTH - rect.Width (),
                           0,
                           rect.Width (),
                           rect.Height () ) ;
    m_DiceBtnResume.EnableWindow ( false ) ;
    m_DiceBtnDiceList.EnableWindow ( true ) ;
    m_DiceBtnChip100.EnableWindow ( true ) ;
    m_DiceBtnChip1000.EnableWindow ( true ) ;
    m_DiceBtnChip10000.EnableWindow ( true ) ;
}

void CDlgDice::SetAntes()
{
    // Set the static antes
    CDlgDice::PostMessage ( WM_MY_MESSAGE, m_nShowChipIndex ) ;
    // Set the every antes
    for ( int i = 0; i < 22; i++ )
    {
        DWORD dwMoney = g_objHero.m_objDiceManager.GetTotalAnte ( i ) ;
        switch ( i )
        {
        case DICE_VALUE_SMALL:
            m_DiceImaSmall.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_BIG:
            m_DiceImaBig.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_111:
            m_DiceImaAll1.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_222:
            m_DiceImaAll2.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_333:
            m_DiceImaAll3.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_444:
            m_DiceImaAll4.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_555:
            m_DiceImaAll5.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_666:
            m_DiceImaAll6.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_4:
            m_DiceIma4.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_5:
            m_DiceIma5.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_6:
            m_DiceIma6.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_7:
            m_DiceIma7.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_8:
            m_DiceIma8.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_9:
            m_DiceIma9.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_10:
            m_DiceIma10.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_11:
            m_DiceIma11.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_12:
            m_DiceIma12.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_13:
            m_DiceIma13.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_14:
            m_DiceIma14.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_15:
            m_DiceIma15.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_16:
            m_DiceIma16.SetMoney ( dwMoney ) ;
            break ;
        case DICE_VALUE_17:
            m_DiceIma17.SetMoney ( dwMoney ) ;
            break ;
        }
    }
}

void CDlgDice::ResetDice()
{
}

BOOL CDlgDice::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    switch ( m_btChipType )
    {
    case 0:
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_H) ) ;
        return true ;
        break ;
    case 1:
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_CHIP100) ) ;
        return true ;
        break ;
    case 2:
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_CHIP1000) ) ;
        return true ;
        break ;
    case 3:
        ::SetCursor ( AfxGetApp()->LoadCursor(IDC_CURSOR_CHIP10000) ) ;
        return true ;
        break ;
    }
    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgDice::OnDiceBtnDicelist()
{
    CDlgDice::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_DICE_LIST ) ;
}

void CDlgDice::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_btChipType = 0 ;
    g_objHero.m_objDiceManager.CancelChipIn ( DICE_VALUE_ALL ) ;
    CDialog::OnRButtonDown(nFlags, point);
}
