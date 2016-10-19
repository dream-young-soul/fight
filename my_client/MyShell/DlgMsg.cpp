
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMsg.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMsg.h"

#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog
const int ALL_CHANNEL		= 0 ;
const int CUR_CHANNEL		= ALL_CHANNEL + 7 ;
const int FRIEND_CHANNEL	= ALL_CHANNEL + 9 ;
const int TEAM_CHANNEL		= ALL_CHANNEL + 3 ;
const int PRIVATE_CHANNEL	= ALL_CHANNEL + 1 ;
const int KNIGHT_CHANNEL	= ALL_CHANNEL + 4 ;
const int LINE_HEIGHT		= 14 ;
const int MAX_SHOWLINE		= 17 ;

CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMsg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMsg)
    m_bShow = true ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_btCurChannel = 0 ;
    m_nHeight = 0 ;
    m_nWidth = 0 ;
    m_btCurShowLine = 0 ;
    m_bLBtnDown = false ;
    //}}AFX_DATA_INIT
}


void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMsg)
    DDX_Control(pDX, IDC_BTN_RESUME, m_BtnResume);
    DDX_Control(pDX, IDC_BTN_UP, m_BtnUp);
    DDX_Control(pDX, IDC_BTN_DOWN, m_BtnDown);
    DDX_Control(pDX, IDC_BTN_CLEAR, m_BtnClear);
    DDX_Control(pDX, IDC_BTN_UPTEAM, m_BtnUpTeam);
    DDX_Control(pDX, IDC_BTN_UPPRIVATE, m_BtnUpPrivate);
    DDX_Control(pDX, IDC_BTN_UPKNIGHT, m_BtnUpKnight);
    DDX_Control(pDX, IDC_BTN_UPFRIEND, m_BtnUpFriend);
    DDX_Control(pDX, IDC_BTN_UPCUR, m_BtnUpCur);
    DDX_Control(pDX, IDC_BTN_UPALL, m_BtnUpAll);
    DDX_Control(pDX, IDC_BTN_TEAM, m_BtnTeam);
    DDX_Control(pDX, IDC_BTN_PRIVATE, m_BtnPrivate);
    DDX_Control(pDX, IDC_BTN_KNIGHT, m_BtnKnight);
    DDX_Control(pDX, IDC_BTN_FRIEND, m_BtnFriend);
    DDX_Control(pDX, IDC_BTN_CUR, m_BtnCur);
    DDX_Control(pDX, IDC_BTN_ALL, m_BtnAll);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMsg, CDialog)
    //{{AFX_MSG_MAP(CDlgMsg)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_ALL, OnBtnAll)
    ON_BN_CLICKED(IDC_BTN_CUR, OnBtnCur)
    ON_BN_CLICKED(IDC_BTN_FRIEND, OnBtnFriend)
    ON_BN_CLICKED(IDC_BTN_TEAM, OnBtnTeam)
    ON_BN_CLICKED(IDC_BTN_PRIVATE, OnBtnPrivate)
    ON_BN_CLICKED(IDC_BTN_KNIGHT, OnBtnKnight)
    ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_BN_CLICKED(IDC_BTN_UPALL, OnBtnUpall)
    ON_BN_CLICKED(IDC_BTN_UPCUR, OnBtnUpcur)
    ON_BN_CLICKED(IDC_BTN_UPFRIEND, OnBtnUpfriend)
    ON_BN_CLICKED(IDC_BTN_UPTEAM, OnBtnUpteam)
    ON_BN_CLICKED(IDC_BTN_UPPRIVATE, OnBtnUpprivate)
    ON_BN_CLICKED(IDC_BTN_UPKNIGHT, OnBtnUpknight)
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg message handlers

LRESULT CDlgMsg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
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
    else if ( message == WM_VSCROLL )
    {
        if ( wParam == SB_LINEUP )
        {
            g_objGameMsg.SetLastShowLineIndex( -1, m_btCurChannel - 1 ) ;
        }
        else if ( wParam == SB_LINEDOWN )
        {
            g_objGameMsg.SetLastShowLineIndex( 1, m_btCurChannel - 1 ) ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMsg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgMsg::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMsg::GetWindowRect( rect ) ;
    m_nHeight = rect.Height() ;
    m_nWidth = rect.Width() ;
    // Init the buttons
    m_BtnUpTeam.Init( rect.left, rect.top, "Button550", 2 ) ;
    m_BtnUpPrivate.Init( rect.left, rect.top, "Button550", 2 ) ;
    m_BtnUpKnight.Init( rect.left, rect.top, "Button550", 2 ) ;
    m_BtnUpFriend.Init( rect.left, rect.top, "Button550", 2 ) ;
    m_BtnUpCur.Init( rect.left, rect.top, "Button550", 2 ) ;
    m_BtnUpAll.Init( rect.left, rect.top, "Button550", 2 ) ;
    m_BtnTeam.Init( rect.left, rect.top, "Button551", 2 ) ;
    m_BtnPrivate.Init( rect.left, rect.top, "Button552", 2 ) ;
    m_BtnKnight.Init( rect.left, rect.top, "Button553", 2 ) ;
    m_BtnFriend.Init( rect.left, rect.top, "Button554", 2 ) ;
    m_BtnCur.Init( rect.left, rect.top, "Button555", 2 ) ;
    m_BtnAll.Init( rect.left, rect.top, "Button556", 2 ) ;
    m_BtnClear.Init( rect.left, rect.top, "Button558" ) ;
    // Init the scroll button
    m_BtnUp.Init( this, 0, rect.left, rect.top );
    m_BtnUp.Load( "Button815" ) ;
    m_BtnDown.Init( this, 1, rect.left, rect.top );
    m_BtnDown.Load( "Button816" ) ;
    m_BtnAll.SetCheckValue ( 1 ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMsg::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
    CMyPos pos ;
    pos.x = x + 4;
    pos.y = y + 250;
    g_objGameMsg.SetNorAreaShowStates( pos, 500 ) ;
    g_objGameMsg.SetShowLines( 17 ) ;
    m_btCurShowLine = 17 ;
}

void CDlgMsg::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni( ( char* )g_strControlAni,
                        "Dialog55",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            int nUpYPos = ( MAX_SHOWLINE - m_btCurShowLine ) * LINE_HEIGHT ;
            // Show the dlg top
            ShowAni->Show( 0, m_Pnt.x, m_Pnt.y + nUpYPos ) ;
            ShowAni->Show( 1, m_Pnt.x + 256, m_Pnt.y + nUpYPos ) ;
            nUpYPos += 20 ;
            // Show the dlg mid
            if ( m_btCurShowLine > 1 )
            {
                RECT rect = { 0, 0, 256, 115} ;
                ShowAni->ShowEx( 4,
                                 m_Pnt.x,
                                 m_Pnt.y + nUpYPos,
                                 &rect,
                                 256,
                                 ( m_btCurShowLine - 2 ) * LINE_HEIGHT + 6 ) ;
                ShowAni->ShowEx( 5,
                                 m_Pnt.x + 256,
                                 m_Pnt.y + nUpYPos,
                                 &rect,
                                 256,
                                 ( m_btCurShowLine - 2 ) * LINE_HEIGHT + 6 ) ;
            }
            nUpYPos += ( m_btCurShowLine - 2 ) * LINE_HEIGHT ;
            // Show the dlg bottom
            ShowAni->Show( 2, m_Pnt.x, m_Pnt.y + nUpYPos + 6 ) ;
            ShowAni->Show( 3, m_Pnt.x + 256, m_Pnt.y + nUpYPos + 6 ) ;
        }
        else
        {
            return ;
        }
        // Show the buttons
        m_BtnUpTeam.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnUpPrivate.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnUpKnight.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnUpFriend.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnUpCur.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnUpAll.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnTeam.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnPrivate.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnKnight.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnFriend.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnCur.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnAll.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnClear.Show( m_Pnt.x, m_Pnt.y ) ;
        // Show the scroll buttons
        m_BtnUp.Show( m_Pnt.x, m_Pnt.y ) ;
        m_BtnDown.Show( m_Pnt.x, m_Pnt.y ) ;
        // Show the game msg
        if ( ::GetKeyState( VK_MENU ) >> 7  )
        {
            g_objGameMsg.Show( true, false ) ;
        }
        else
        {
            g_objGameMsg.Show( false, false ) ;
        }
    }
}

void CDlgMsg::OnBtnAll()
{
    m_BtnAll.SetCheckValue( 1 ) ;
    if ( m_btCurChannel != ALL_CHANNEL )
    {
        CDlgMsg::PopupCurCheck( ALL_CHANNEL ) ;
    }
    g_objGameMsg.SetCurShowChannel( _TXTATR_NORMAL ) ;
}

void CDlgMsg::OnBtnCur()
{
    m_BtnCur.SetCheckValue( 1 ) ;
    if ( m_btCurChannel != CUR_CHANNEL )
    {
        CDlgMsg::PopupCurCheck( CUR_CHANNEL ) ;
    }
    g_objGameMsg.SetCurShowChannel( _TXTATR_TALK - _TXTATR_NORMAL - 1 ) ;
}

void CDlgMsg::OnBtnFriend()
{
    m_BtnFriend.SetCheckValue( 1 ) ;
    if ( m_btCurChannel != FRIEND_CHANNEL )
    {
        CDlgMsg::PopupCurCheck( FRIEND_CHANNEL ) ;
    }
    g_objGameMsg.SetCurShowChannel( _TXTATR_FRIEND - _TXTATR_NORMAL - 1 ) ;
}

void CDlgMsg::OnBtnTeam()
{
    m_BtnTeam.SetCheckValue( 1 ) ;
    if ( m_btCurChannel != TEAM_CHANNEL )
    {
        CDlgMsg::PopupCurCheck( TEAM_CHANNEL ) ;
    }
    g_objGameMsg.SetCurShowChannel( _TXTATR_TEAM - _TXTATR_NORMAL - 1 ) ;
}

void CDlgMsg::OnBtnPrivate()
{
    m_BtnPrivate.SetCheckValue( 1 ) ;
    if ( m_btCurChannel != PRIVATE_CHANNEL )
    {
        CDlgMsg::PopupCurCheck( PRIVATE_CHANNEL ) ;
    }
    g_objGameMsg.SetCurShowChannel( _TXTATR_PRIVATE - _TXTATR_NORMAL - 1 ) ;
}

void CDlgMsg::OnBtnKnight()
{
    m_BtnKnight.SetCheckValue( 1 ) ;
    if ( m_btCurChannel != KNIGHT_CHANNEL )
    {
        CDlgMsg::PopupCurCheck( KNIGHT_CHANNEL ) ;
    }
    g_objGameMsg.SetCurShowChannel( _TXTATR_SYNDICATE - _TXTATR_NORMAL - 1 ) ;
}

void CDlgMsg::PopupCurCheck(BYTE btCurChannel)
{
    switch ( m_btCurChannel )
    {
    case ALL_CHANNEL:
        m_BtnAll.SetCheckValue( 0 ) ;
        break ;
    case CUR_CHANNEL:
        m_BtnCur.SetCheckValue( 0 ) ;
        break ;
    case FRIEND_CHANNEL:
        m_BtnFriend.SetCheckValue( 0 ) ;
        break ;
    case TEAM_CHANNEL:
        m_BtnTeam.SetCheckValue( 0 ) ;
        break ;
    case PRIVATE_CHANNEL:
        m_BtnPrivate.SetCheckValue( 0 ) ;
        break ;
    case KNIGHT_CHANNEL:
        m_BtnKnight.SetCheckValue( 0 ) ;
        break ;
    default:
        break ;
    }
    m_btCurChannel = btCurChannel ;
}

void CDlgMsg::OnBtnClear()
{
    g_objGameMsg.ClearMsg( false ) ;
}

void CDlgMsg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Test if the cursor int the move rect
    CRect rect = CRect( 0,
                        ( MAX_SHOWLINE - m_btCurShowLine ) * LINE_HEIGHT,
                        512,
                        ( MAX_SHOWLINE - m_btCurShowLine ) * LINE_HEIGHT + 20 ) ;
    if ( rect.PtInRect( point ) )
    {
        m_bLBtnDown = true ;
        ::GetCursorPos( &m_MousePnt ) ;
        CDialog::OnLButtonDown(nFlags, point);
        return ;
    }
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMsg::GetParent()->PostMessage( WM_LBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMsg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if ( m_bLBtnDown )
    {
        m_bLBtnDown = false ;
        CPoint CurMousePnt ;
        ::GetCursorPos( &CurMousePnt ) ;
        m_btCurShowLine -=(BYTE)( ( CurMousePnt.y - m_MousePnt.y ) / LINE_HEIGHT );
        m_btCurShowLine = ( m_btCurShowLine < 2 ) ? 2 : m_btCurShowLine ;
        g_objGameMsg.SetShowLines( m_btCurShowLine ) ;
        CRect rect ;
        m_BtnUp.GetWindowRect( rect ) ;
        m_BtnUp.MoveWindow( rect.left - m_Pnt.x,
                            ( MAX_SHOWLINE - m_btCurShowLine ) * LINE_HEIGHT,
                            rect.Width(),
                            rect.Height() ) ;
        CDlgMsg::GetWindowRect( rect ) ;
        m_BtnUp.Init( this, 0, rect.left, rect.top );
        CDialog::OnLButtonUp(nFlags, point);
        return ;
    }
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMsg::GetParent()->PostMessage( WM_LBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMsg::OnRButtonDown(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMsg::GetParent()->PostMessage( WM_RBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMsg::OnRButtonUp(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgMsg::GetParent()->PostMessage( WM_RBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMsg::OnBtnUpall()
{
}

void CDlgMsg::OnBtnUpcur()
{
    g_objGameMsg.SetChannelStatus( _TXTATR_TALK, m_BtnUpCur.GetCheckValue() ) ;
}

void CDlgMsg::OnBtnUpfriend()
{
    g_objGameMsg.SetChannelStatus( _TXTATR_FRIEND, m_BtnUpFriend.GetCheckValue() ) ;
}

void CDlgMsg::OnBtnUpteam()
{
    g_objGameMsg.SetChannelStatus( _TXTATR_TEAM, m_BtnUpTeam.GetCheckValue() ) ;
}

void CDlgMsg::OnBtnUpprivate()
{
    g_objGameMsg.SetChannelStatus( _TXTATR_PRIVATE, m_BtnUpPrivate.GetCheckValue() ) ;
}

void CDlgMsg::OnBtnUpknight()
{
    g_objGameMsg.SetChannelStatus( _TXTATR_SYNDICATE, m_BtnUpKnight.GetCheckValue() ) ;
}

void CDlgMsg::OnMouseMove(UINT nFlags, CPoint point)
{
    MouseSet( point.x + m_Pnt.x, point.y + m_Pnt.y, 0);
    CDialog::OnMouseMove(nFlags, point);
}
