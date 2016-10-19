
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgLay.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgLay.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLay dialog


CDlgLay::CDlgLay(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLay::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgLay)
    m_bShow = false ;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nDir = 7 ;
    //}}AFX_DATA_INIT
}


void CDlgLay::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLay)
    DDX_Control(pDX, IDC_LAY_RIGHT, m_LayRight);
    DDX_Control(pDX, IDC_LAY_LEFT, m_LayLeft);
    DDX_Control(pDX, IDC_LAY_CLOSEB, m_LayCloseB);
    DDX_Control(pDX, IDC_LAY_BTN_CLOSES, m_LayBtnCloseS);
    DDX_Control(pDX, IDC_BTN_OK, m_BtnOk);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLay, CDialog)
    //{{AFX_MSG_MAP(CDlgLay)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_LAY_BTN_CLOSES, OnLayBtnCloses)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_LAY_CLOSEB, OnLayCloseb)
    ON_BN_CLICKED(IDC_LAY_LEFT, OnLayLeft)
    ON_BN_CLICKED(IDC_LAY_RIGHT, OnLayRight)
    ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLay message handlers

LRESULT CDlgLay::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_SHOWWINDOW:
        m_bShow = wParam ;
        if ( wParam )
        {
            CRect rect ;
            CDlgLay::GetWindowRect ( rect ) ;
            if ( m_nFrameMode == 0 )
            {
                DXPlaySound ( "Sound/DlgOpen.wav" ) ;
                m_dwLastShowTime = 0 ;
                m_nFrameMode = 1 ;
                CDlgLay::MoveWindow ( -rect.Width(), m_Pnt.y, rect.Width(), rect.Height(), true ) ;
            }
            else if ( m_nFrameMode == 2 )
            {
                CDlgLay::MoveWindow ( 0, m_Pnt.y, rect.Width(), rect.Height(), true ) ;
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

HBRUSH CDlgLay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgLay::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgLay::GetWindowRect ( rect ) ;
    m_fFrameStep = ( float ) rect.Width () / DLG_FRAME_AMOUNT ;
    // Init the buttons
    m_LayRight.Init ( rect.left, rect.top, "Button383" ) ;
    m_LayLeft.Init ( rect.left, rect.top, "Button382" ) ;
    m_LayCloseB.Init ( rect.left, rect.top, "Button3" ) ;
    m_LayBtnCloseS.Init ( rect.left, rect.top, "Button5" ) ;
    m_BtnOk.Init ( rect.left, rect.top, "Button0" ) ;
    m_BtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    m_BtnCancel.Init ( rect.left, rect.top, "Button7" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLay::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgLay::Show()
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
                    m_Pnt.x += (LONG)(( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep) ;
                }
                else
                {
                    m_Pnt.x = 0;
                    m_nFrameMode = 2 ;
                    CDlgLay::WindowProc ( WM_SHOWWINDOW, true, NULL ) ;
                }
            }
            break ;
        case 2:
            break ;
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog36",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Init the button
        m_LayRight.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LayLeft.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LayCloseB.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LayBtnCloseS.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnOk.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnCancel.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgLay::OnLayBtnCloses()
{
    CDlgLay::OnLayCloseb () ;
}

void CDlgLay::OnBtnHelp()
{
    CDlgLay::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_LAY ) ;
}

void CDlgLay::OnLayCloseb()
{
    g_objHero.DelMousePlayer () ;
    CDlgLay::ShowWindow ( SW_HIDE ) ;
    CDlgLay::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_LAY_CLOSE ) ;
}

void CDlgLay::OnLayLeft()
{
    if ( 7 == m_nDir )
    {
        m_nDir = 0 ;
    }
    else
    {
        m_nDir++ ;
    }
    g_objHero.SetMousePlayerDir ( m_nDir ) ;
}

void CDlgLay::OnLayRight()
{
    if ( 0 == m_nDir )
    {
        m_nDir = 7 ;
    }
    else
    {
        m_nDir-- ;
    }
    g_objHero.SetMousePlayerDir ( m_nDir ) ;
}

void CDlgLay::OnBtnOk()
{
    g_objHero.PutMousePlayer () ;
    CDlgLay::OnLayCloseb () ;
}

void CDlgLay::OnBtnCancel()
{
    g_objHero.DelMousePlayer () ;
    CDlgLay::OnLayCloseb () ;
}
