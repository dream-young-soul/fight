
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgBlack.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgBlack.h"

#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlack dialog


CDlgBlack::CDlgBlack(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgBlack::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgBlack)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    //}}AFX_DATA_INIT
}


void CDlgBlack::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgBlack)
    DDX_Control(pDX, IDC_BLACK_EDT_NAME, m_BlackEdtName);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    DDX_Control(pDX, IDC_BLACK_LST_DATA, m_BlackLstData);
    DDX_Control(pDX, IDC_BLACK_CLOSEB, m_BlackCloseb);
    DDX_Control(pDX, IDC_BLACK_BTN_DEL, m_BlackBtnDel);
    DDX_Control(pDX, IDC_BLACK_BTN_ADD, m_BlackBtnAdd);
    DDX_Control(pDX, IDC_BLACK_BTN_CLOSES, m_BlackBtnCloses);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBlack, CDialog)
    //{{AFX_MSG_MAP(CDlgBlack)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BLACK_BTN_CLOSES, OnBlackBtnCloses)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_BLACK_CLOSEB, OnBlackCloseb)
    ON_BN_CLICKED(IDC_BLACK_BTN_DEL, OnBlackBtnDel)
    ON_BN_CLICKED(IDC_BLACK_BTN_ADD, OnBlackBtnAdd)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlack message handlers

LRESULT CDlgBlack::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
        if ( m_bShow )
        {
            CDlgBlack::SetBlack () ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2 )
        {
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgBlack::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgBlack::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgBlack::GetWindowRect ( rect ) ;
    // Init the buttons
    m_BtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    m_BlackCloseb.Init ( rect.left, rect.top, "Button3" ) ;
    m_BlackBtnDel.Init ( rect.left, rect.top, "Button363" ) ;
    m_BlackBtnAdd.Init ( rect.left, rect.top, "Button362" ) ;
    m_BlackBtnCloses.Init ( rect.left, rect.top, "Button5" ) ;
    // Init the list
    m_BlackLstData.Init ( rect.left, rect.top, NULL, SIMPLE_LIST ) ;
    m_BlackLstData.SetFontColor ( 0xffffff ) ;
    m_BlackLstData.SetParentCob ( this ) ;
    // Init the edit
    m_BlackEdtName.Init ( rect.left, rect.top, false, NULL, 0x00ffff ) ;
    m_BlackEdtName.SetLimitText ( 15 ) ;
    m_BlackEdtName.SetOffset ( CPoint ( 4, 2 ) ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBlack::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgBlack::Show()
{
    if ( m_bShow )
    {
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
        ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                                "Image37",
                                                EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x + 90,
                            m_Pnt.y + 6 ) ;
        else
        {
            return ;
        }
        // Show the button
        m_BtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BlackCloseb.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BlackBtnDel.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BlackBtnAdd.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BlackBtnCloses.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the list
        m_BlackLstData.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the edit
        // Temp code
        CMyBitmap::ShowString ( m_Pnt.x + 20,
                                m_Pnt.y + 124,
                                0xffffff,
                                "ÇëÊäÈëÃû×Ö:" ) ;
        m_BlackEdtName.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgBlack::OnBlackBtnCloses()
{
    CDlgBlack::ShowWindow ( SW_HIDE ) ;
    CDlgBlack::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_BLACK_CLOSE ) ;
}

void CDlgBlack::OnBtnHelp()
{
    CDlgBlack::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_BLACK ) ;
}

void CDlgBlack::OnBlackCloseb()
{
    CDlgBlack::ShowWindow ( SW_HIDE ) ;
    CDlgBlack::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_BLACK_CLOSE ) ;
}

void CDlgBlack::OnBlackBtnDel()
{
    int nIndex = m_BlackLstData.GetCurSel () ;
    if ( nIndex != -1 )
    {
        g_objGameMsg.DelBlackName ( nIndex ) ;
        CDlgBlack::SetBlack () ;
    }
}

void CDlgBlack::OnBlackBtnAdd()
{
    if ( m_BlackEdtName.GetWindowTextLength () > 0 )
    {
        char strName[32] ;
        m_BlackEdtName.GetWindowText ( strName, 32 ) ;
        g_objGameMsg.AddBlackName ( strName ) ;
        CDlgBlack::SetBlack () ;
        m_BlackEdtName.SetWindowText ( NULL ) ;
    }
}

void CDlgBlack::SetBlack()
{
    m_BlackLstData.ResetContent () ;
    int nAmount = g_objGameMsg.GetBlackNameAmount () ;
    for ( int i = 0; i < nAmount; i++ )
    {
        m_BlackLstData.AddString ( g_objGameMsg.GetBlackNameByIndex ( i ) ) ;
    }
}
