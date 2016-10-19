
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgEffigy.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgEffigy.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEffigy dialog


CDlgEffigy::CDlgEffigy(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEffigy::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEffigy)
    m_bShow = false ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_bPlay = true ;
    m_nDir = 7 ;
    //}}AFX_DATA_INIT
}


void CDlgEffigy::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEffigy)
    DDX_Control(pDX, IDC_EFFIGY_RIGHT, m_EffigyRight);
    DDX_Control(pDX, IDC_EFFIGY_PANDP, m_EffigyPandP);
    DDX_Control(pDX, IDC_EFFIGY_OK, m_EffigyOk);
    DDX_Control(pDX, IDC_EFFIGY_LEFT, m_EffigyLeft);
    DDX_Control(pDX, IDC_EFFIGY_CLOSES, m_Effigy_CLoseS);
    DDX_Control(pDX, IDC_EFFIGY_CLOSE, m_Effigy_Close);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    DDX_Control(pDX, IDC_ACT_BTN1, m_ActBtn1);
    DDX_Control(pDX, IDC_ACT_BTN2, m_ActBtn2);
    DDX_Control(pDX, IDC_ACT_BTN3, m_ActBtn3);
    DDX_Control(pDX, IDC_ACT_BTN4, m_ActBtn4);
    DDX_Control(pDX, IDC_ACT_BTN5, m_ActBtn5);
    DDX_Control(pDX, IDC_ACT_BTN6, m_ActBtn6);
    DDX_Control(pDX, IDC_ACT_BTN7, m_ActBtn7);
    DDX_Control(pDX, IDC_ACT_BTN8, m_ActBtn8);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEffigy, CDialog)
    //{{AFX_MSG_MAP(CDlgEffigy)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_EFFIGY_CLOSE, OnEffigyClose)
    ON_BN_CLICKED(IDC_EFFIGY_CLOSES, OnEffigyCloses)
    ON_BN_CLICKED(IDC_ACT_BTN1, OnActBtn1)
    ON_BN_CLICKED(IDC_ACT_BTN2, OnActBtn2)
    ON_BN_CLICKED(IDC_ACT_BTN3, OnActBtn3)
    ON_BN_CLICKED(IDC_ACT_BTN4, OnActBtn4)
    ON_BN_CLICKED(IDC_ACT_BTN5, OnActBtn5)
    ON_BN_CLICKED(IDC_ACT_BTN6, OnActBtn6)
    ON_BN_CLICKED(IDC_ACT_BTN7, OnActBtn7)
    ON_BN_CLICKED(IDC_ACT_BTN8, OnActBtn8)
    ON_BN_CLICKED(IDC_EFFIGY_PANDP, OnEffigyPandp)
    ON_BN_CLICKED(IDC_EFFIGY_LEFT, OnEffigyLeft)
    ON_BN_CLICKED(IDC_EFFIGY_RIGHT, OnEffigyRight)
    ON_BN_CLICKED(IDC_EFFIGY_OK, OnEffigyOk)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEffigy message handlers

LRESULT CDlgEffigy::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_SHOWWINDOW:
        m_bShow = wParam ;
        if ( wParam )
        {
            CRect rect ;
            CDlgEffigy::GetWindowRect ( rect ) ;
            if ( m_nFrameMode == 0 )
            {
                DXPlaySound ( "Sound/DlgOpen.wav" ) ;
                m_dwLastShowTime = 0 ;
                m_nFrameMode = 1 ;
                CDlgEffigy::MoveWindow ( -rect.Width(), m_Pnt.y, rect.Width(), rect.Height(), true ) ;
            }
            else if ( m_nFrameMode == 2 )
            {
                CDlgEffigy::MoveWindow ( 0, m_Pnt.y, rect.Width(), rect.Height(), true ) ;
                m_nFrameMode = 0 ;
            }
        }
        else
        {
            DXPlaySound ( "Sound/DlgClose.wav" ) ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgEffigy::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgEffigy::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgEffigy::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float ) rect.Width () / DLG_FRAME_AMOUNT ;
    // Init the button
    m_EffigyRight.Init ( rect.left, rect.top, "Button383" ) ;
    m_EffigyPandP.Init ( rect.left, rect.top, "Button381" ) ;
    m_EffigyOk.Init ( rect.left, rect.top, "Button0" ) ;
    m_EffigyLeft.Init ( rect.left, rect.top, "Button382" ) ;
    m_Effigy_CLoseS.Init ( rect.left, rect.top, "Button5" ) ;
    m_Effigy_Close.Init ( rect.left, rect.top, "Button3" ) ;
    m_BtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    m_ActBtn1.Init ( rect.left, rect.top, "ButtonA230" ) ;
    m_ActBtn2.Init ( rect.left, rect.top, "ButtonA210" ) ;
    m_ActBtn3.Init ( rect.left, rect.top, "ButtonA170" ) ;
    m_ActBtn4.Init ( rect.left, rect.top, "ButtonA150" ) ;
    m_ActBtn5.Init ( rect.left, rect.top, "ButtonA160" ) ;
    m_ActBtn6.Init ( rect.left, rect.top, "ButtonA1" ) ;
    m_ActBtn7.Init ( rect.left, rect.top, "ButtonA190" ) ;
    m_ActBtn8.Init ( rect.left, rect.top, "ButtonA200" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEffigy::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgEffigy::Show()
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
                    m_Pnt.x +=(LONG)( ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep );
                }
                else
                {
                    m_Pnt.x = 0;
                    m_nFrameMode = 2 ;
                    CDlgEffigy::WindowProc ( WM_SHOWWINDOW, true, NULL ) ;
                }
            }
            break ;
        case 2:
            break ;
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog38",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the button
        m_EffigyRight.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_EffigyPandP.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_EffigyOk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_EffigyLeft.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Effigy_CLoseS.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Effigy_Close.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn6.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn7.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ActBtn8.Show ( m_Pnt.x, m_Pnt.y ) ;
        // show the dummy
        CMyPos dummyPos ;
        dummyPos.x = m_Pnt.x + 100 ;
        dummyPos.y = m_Pnt.y + 275 ;
        g_objHero.m_objDummy.ShowDummy ( dummyPos , 150, -15) ;
    }
}

void CDlgEffigy::OnBtnHelp()
{
    CDlgEffigy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_EFFIGY ) ;
}

void CDlgEffigy::OnEffigyClose()
{
    CDlgEffigy::ShowWindow ( SW_HIDE ) ;
    CDlgEffigy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_EFFIGY_CLOSE ) ;
}

void CDlgEffigy::OnEffigyCloses()
{
    CDlgEffigy::OnEffigyClose () ;
}

void CDlgEffigy::OnActBtn1()
{
    g_objHero.m_objDummy.SetDummyAction ( 230 ) ;
}

void CDlgEffigy::OnActBtn2()
{
    g_objHero.m_objDummy.SetDummyAction ( 210 ) ;
}

void CDlgEffigy::OnActBtn3()
{
    g_objHero.m_objDummy.SetDummyAction ( 170 ) ;
}

void CDlgEffigy::OnActBtn4()
{
    g_objHero.m_objDummy.SetDummyAction ( 150 ) ;
}

void CDlgEffigy::OnActBtn5()
{
    g_objHero.m_objDummy.SetDummyAction ( 160 ) ;
}

void CDlgEffigy::OnActBtn6()
{
    g_objHero.m_objDummy.SetDummyAction ( 1 ) ;
}

void CDlgEffigy::OnActBtn7()
{
    g_objHero.m_objDummy.SetDummyAction ( 190 ) ;
}

void CDlgEffigy::OnActBtn8()
{
    g_objHero.m_objDummy.SetDummyAction ( 200 ) ;
}

void CDlgEffigy::OnEffigyPandp()
{
    if ( m_bPlay )
    {
        g_objHero.m_objDummy.PauseDummy () ;
        m_EffigyPandP.ChangeImage ( "Button380" ) ;
    }
    else
    {
        g_objHero.m_objDummy.PlayDummy () ;
        m_EffigyPandP.ChangeImage ( "Button381" ) ;
    }
    m_bPlay = !m_bPlay ;
}

void CDlgEffigy::OnEffigyLeft()
{
    if ( 7 == m_nDir )
    {
        m_nDir = 0 ;
    }
    else
    {
        m_nDir++ ;
    }
    g_objHero.m_objDummy.SetDummyDir ( m_nDir ) ;
}

void CDlgEffigy::OnEffigyRight()
{
    if ( 0 == m_nDir )
    {
        m_nDir = 7 ;
    }
    else
    {
        m_nDir-- ;
    }
    g_objHero.m_objDummy.SetDummyDir ( m_nDir ) ;
}

void CDlgEffigy::OnEffigyOk()
{
    g_objHero.CreateStatuty () ;
    CDlgEffigy::OnEffigyClose () ;
}
