
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTalkAct.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTalkAct.h"

#include "3DRoleData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTalkAct dialog


CDlgTalkAct::CDlgTalkAct(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTalkAct::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTalkAct)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_nSpace = 0 ;
    m_nCurIndex = 0;
    m_nCurAmount = 0;
    m_pParentCWnd = NULL;
    //}}AFX_DATA_INIT
}


void CDlgTalkAct::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTalkAct)
    DDX_Control(pDX, IDC_TALKACT_BTN_PAGEUP, m_TalkActBtnPageUp);
    DDX_Control(pDX, IDC_TALKACT_BTN_PAGEDOWN, m_TalkActBtnPageDown);
    DDX_Control(pDX, IDC_TALKACT_BTN_HELP, m_TalkActBtnHelp);
    DDX_Control(pDX, IDC_TALKACT_BTN_CLOSEB, m_TalkActBtnCloseB);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTalkAct, CDialog)
    //{{AFX_MSG_MAP(CDlgTalkAct)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_TALKACT_BTN_HELP, OnTalkActBtnHelp)
    ON_BN_CLICKED(IDC_TALKACT_BTN_CLOSEB, OnTalkActBtnCloseB)
    ON_BN_CLICKED(IDC_TALKACT_BTN_PAGEUP, OnTalkActBtnPageUp)
    ON_BN_CLICKED(IDC_TALKACT_BTN_PAGEDOWN, OnTalkActBtnPageDown)
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTalkAct message handlers

LRESULT CDlgTalkAct::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2 )
        {
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTalkAct::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgTalkAct::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgTalkAct::GetWindowRect ( rect ) ;
    m_nSpace = ( rect.Width () / 5 - 28 ) / 2 ;
    m_nCurAmount = ( g_obj3DRoleData.GetTextActionAmount () > 20 ) ? 20 : g_obj3DRoleData.GetTextActionAmount () ;
    // Init the button
    m_TalkActBtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    m_TalkActBtnCloseB.Init ( rect.left, rect.top, "Button3" ) ;
    m_TalkActBtnPageUp.Init ( rect.left, rect.top, "Button500" ) ;
    m_TalkActBtnPageDown.Init ( rect.left, rect.top, "Button501" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTalkAct::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgTalkAct::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog50",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            pShowAni->Show ( 0, m_Pnt.x + 3, m_Pnt.y - 5 );
        }
        else
        {
            return ;
        }
        // Show the button
        m_TalkActBtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkActBtnCloseB.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkActBtnPageUp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkActBtnPageDown.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the action text
        for ( int i = 0; i < m_nCurAmount; i++ )
        {
            CMyBitmap::ShowString ( m_Pnt.x + 13 + ( i % 5 ) * ( 2 * m_nSpace + 28 ),
                                    m_Pnt.y + 5 + ( i / 5 ) * 20,
                                    0xffffff,
                                    g_obj3DRoleData.GetTextActionTitle ( m_nCurIndex + i ) ) ;
        }
    }
}

void CDlgTalkAct::OnTalkActBtnHelp()
{
    CDlgTalkAct::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_TALKACT ) ;
}

void CDlgTalkAct::OnTalkActBtnCloseB()
{
    CDlgTalkAct::ShowWindow ( SW_HIDE ) ;
}

void CDlgTalkAct::OnTalkActBtnPageUp()
{
    DXPlaySound("Sound/up_Page.wav");
    if ( m_nCurIndex > 0 )
    {
        m_nCurIndex -= 20 ;
        m_nCurAmount = ( g_obj3DRoleData.GetTextActionAmount () > m_nCurIndex + 20 ) ? 20 : g_obj3DRoleData.GetTextActionAmount () - m_nCurIndex;
    }
}

void CDlgTalkAct::OnTalkActBtnPageDown()
{
    DXPlaySound("Sound/up_Page.wav");
    if ( m_nCurIndex + 20 < g_obj3DRoleData.GetTextActionAmount () )
    {
        m_nCurIndex += 20 ;
        m_nCurAmount = ( g_obj3DRoleData.GetTextActionAmount () > m_nCurIndex + 20 ) ? 20 : g_obj3DRoleData.GetTextActionAmount () - m_nCurIndex;
    }
}

void CDlgTalkAct::OnLButtonDown(UINT nFlags, CPoint point)
{
    point.y -= 2 ;
    int nRow = point.y / 20 ;
    point.x -= 5 ;
    int nLine = point.x / ( 2 * m_nSpace + 28 ) ;
    int nIndex = nRow * 5 + nLine ;
    if ( nIndex > m_nCurAmount )
    {
        return ;
    }
    if ( m_pParentCWnd )
    {
        m_pParentCWnd->PostMessage ( WM_MY_MESSAGE, ACTION_CHOOSE, nIndex + m_nCurIndex ) ;
    }
}

void CDlgTalkAct::SetParentCWnd(CWnd* ParentCWnd)
{
    m_pParentCWnd = ParentCWnd ;
}
