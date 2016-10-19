
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgHawk.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgHawk.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHawk dialog


CDlgHawk::CDlgHawk(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgHawk::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgHawk)
    m_bShow = false ;
    m_Pnt = CPoint (0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgHawk::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgHawk)
    DDX_Control(pDX, IDC_HAWK_EDT_DATA, m_EdtData);
    DDX_Control(pDX, IDC_HAWK_BTN_CLOSES, m_BtnClose);
    DDX_Control(pDX, IDC_HAWK_BTN_AGREE, m_BtnAgree);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHawk, CDialog)
    //{{AFX_MSG_MAP(CDlgHawk)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_HAWK_BTN_CLOSES, OnHawkBtnCloses)
    ON_BN_CLICKED(IDC_HAWK_BTN_AGREE, OnHawkBtnAgree)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHawk message handlers

LRESULT CDlgHawk::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if ( m_bShow )
        {
            DXPlaySound("Sound/Open.wav");
            m_EdtData.SetFocus();
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgHawk::OnHawkBtnCloses();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgHawk::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgHawk::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgHawk::GetWindowRect ( rect ) ;
    // Init the button
    m_BtnClose.Init ( rect.left, rect.top, "Button540" ) ;
    m_BtnAgree.Init ( rect.left, rect.top, "Button76" ) ;
    // Init the edit
    m_EdtData.Init ( rect.left, rect.top, true ) ;
    m_EdtData.SetLimitText ( 110 ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHawk::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgHawk::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog35",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
            ShowAni->Show ( 1,
                            m_Pnt.x + 256,
                            m_Pnt.y ) ;
            ShowAni->Show ( 2,
                            m_Pnt.x + 256,
                            m_Pnt.y + 128 ) ;
        }
        else
        {
            return ;
        }
        // Show the button
        m_BtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnAgree.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the edit
        m_EdtData.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgHawk::OnHawkBtnCloses()
{
    DXPlaySound("Sound/window_close.wav");
    CDlgHawk::EnableWindow(false) ;
}

void CDlgHawk::OnHawkBtnAgree()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_EdtData.GetWindowTextLength () > 0 )
    {
        char strText[256] = "" ;
        m_EdtData.GetWindowText ( strText, m_EdtData.GetWindowTextLength() + 1 ) ;
        RepairString(strText);
        g_objHero.m_objBoothManager.SetCryOut ( strText ) ;
        CDlgHawk::EnableWindow(false);
    }
}
