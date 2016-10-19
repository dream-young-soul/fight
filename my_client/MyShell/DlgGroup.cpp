
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgGroup.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgGroup.h"

#include "Hero.h"
#include "GamePlayerset.h"
#include "GameMsg.h"
#include "3dGameMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGroup dialog

CDlgGroup::CDlgGroup(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgGroup::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgGroup)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_uTeamMode = TEAM_MODE_NONE ;
    m_btOwnerPos = 0 ;
    m_dwFocusTime = 0;
    //}}AFX_DATA_INIT
}


void CDlgGroup::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgGroup)
    DDX_Control(pDX, IDC_GROUP_BTN_5, m_GroupBtn5);
    DDX_Control(pDX, IDC_GROUP_CHK_3, m_GroupBtn3);
    DDX_Control(pDX, IDC_GROUP_IMG1, m_GroupImg1);
    DDX_Control(pDX, IDC_GROUP_IMG2, m_GroupImg2);
    DDX_Control(pDX, IDC_GROUP_IMG3, m_GroupImg3);
    DDX_Control(pDX, IDC_GROUP_IMG4, m_GroupImg4);
    DDX_Control(pDX, IDC_GROUP_BTN_1, m_GroupBtn1);
    DDX_Control(pDX, IDC_GROUP_BTN_2, m_GroupBtn2);
    DDX_Control(pDX, IDC_GROUP_BTN_4, m_GroupBtn4);
    DDX_Control(pDX, IDC_GROUP_BTN_CLOSE, m_GroupCloseBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGroup, CDialog)
    //{{AFX_MSG_MAP(CDlgGroup)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_GROUP_BTN_CLOSE, OnGroupBtnClose)
    ON_BN_CLICKED(IDC_GROUP_BTN_1, OnGroupBtn1)
    ON_BN_CLICKED(IDC_GROUP_BTN_2, OnGroupBtn2)
    ON_BN_CLICKED(IDC_GROUP_CHK_3, OnGroupChk3)
    ON_BN_CLICKED(IDC_GROUP_BTN_4, OnGroupBtn4)
    ON_BN_CLICKED(IDC_GROUP_IMG1, OnGroupImg1)
    ON_BN_CLICKED(IDC_GROUP_IMG2, OnGroupImg2)
    ON_BN_CLICKED(IDC_GROUP_IMG3, OnGroupImg3)
    ON_BN_CLICKED(IDC_GROUP_IMG4, OnGroupImg4)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_BN_CLICKED(IDC_GROUP_BTN_5, OnGroupBtn5)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGroup message handlers

LRESULT CDlgGroup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if ( !m_bShow )
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
            if ( m_MenuGroup.m_bShow )
            {
                m_MenuGroup.EnableWindow( false ) ;
            }
        }
        else
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
            //			CDlgGroup::SetGroupContain();
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
            CDlgGroup::OnGroupBtnClose () ;
            return true ;
        }
    }
    else if ( message == WM_MY_MESSAGE )
    {
        if ( wParam == MENU_GROUP_CLICK )
        {
            switch ( lParam )
            {
            case MONEY_CLICK:
                g_objHero.SetTeamMoneyAccess ( m_MenuGroup.m_GroupChkMoney.GetCheckValue () ) ;
                break ;
            case ITEM_CLICK:
                g_objHero.SetTeamItemAccess ( m_MenuGroup.m_GroupChkItem.GetCheckValue () ) ;
                break ;
            case GEAR_CLICK:
                g_objHero.SetTeamGemAccess( m_MenuGroup.m_GroupChkStop.GetCheckValue() ) ;
                break ;
            case JOIN_CLICK:
                {
                    if (m_MenuGroup.m_GroupChkJoin.GetCheckValue())
                    {
                        g_objHero.TeamClose();
                    }
                    else
                    {
                        g_objHero.TeamOpen();
                    }
                }
            }
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgGroup::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgGroup::GetWindowRect ( rect ) ;
    // Init the Image
    m_GroupImg1.Init ( rect.left, rect.top, NULL, FACE_IMAGE) ;
    m_GroupImg2.Init ( rect.left, rect.top, NULL, FACE_IMAGE) ;
    m_GroupImg3.Init ( rect.left, rect.top, NULL, FACE_IMAGE) ;
    m_GroupImg4.Init ( rect.left, rect.top, NULL, FACE_IMAGE) ;
    m_GroupImg1.SetItemPos( 1 ) ;
    m_GroupImg2.SetItemPos( 2 ) ;
    m_GroupImg3.SetItemPos( 3 ) ;
    m_GroupImg4.SetItemPos( 4 ) ;
    m_GroupImg1.EnableWindow( false ) ;
    m_GroupImg2.EnableWindow( false ) ;
    m_GroupImg3.EnableWindow( false ) ;
    m_GroupImg4.EnableWindow( false ) ;
    // Init the buttons
    m_GroupBtn1.Init ( rect.left, rect.top, "Button190" );
    m_GroupBtn2.Init ( rect.left, rect.top, "Button191" );
    m_GroupBtn3.Init ( rect.left, rect.top, "Button197" );
    m_GroupBtn4.Init ( rect.left, rect.top, "Button195" );
    m_GroupBtn5.Init ( rect.left, rect.top, "Button198" );
    m_GroupCloseBtn.Init ( rect.left, rect.top, "Button3" );
    // Init the menu
    m_MenuGroup.Create ( IDD_MENU_GROUP, this->GetParent () );
    m_MenuGroup.SetParentWnd ( this ) ;
    m_MenuGroup.ShowWindow( SW_SHOW ) ;
    m_MenuGroup.EnableWindow( false ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgGroup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

void CDlgGroup::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgGroup::Show()
{
    // Show the owner
    if ( m_bShow )
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog19",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            switch(m_uTeamMode)
            {
            case TEAM_MODE_NONE:
                ShowAni->Show( 0,
                               m_Pnt.x,
                               _SCR_HEIGHT - 304) ;
                break;
            case TEAM_MODE_LEADER:
                ShowAni->Show( 1,
                               m_Pnt.x - 2,
                               _SCR_HEIGHT - 320) ;
                break;
            case TEAM_MODE_MEMBER:
                ShowAni->Show( 0,
                               m_Pnt.x,
                               _SCR_HEIGHT - 304) ;
                break;
            default:
                break;
            }
        }
        else
        {
            return ;
        }
        // Show the button
        m_GroupBtn1.Show ( m_Pnt.x, m_Pnt.y ) ;
        //		if ( m_uTeamMode != TEAM_MODE_MEMBER )
        m_GroupBtn2.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_uTeamMode == TEAM_MODE_LEADER )
        {
            m_GroupBtn3.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_GroupBtn4.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_GroupBtn5.Show ( m_Pnt.x, m_Pnt.y) ;
        }
        m_GroupCloseBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the menu
        if ( m_MenuGroup.m_bShow )
        {
            m_MenuGroup.Show () ;
        }
    }
    CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                     "Dialog191",
                     EXIGENCE_IMMEDIATE ) ;
    if (pShowAni == NULL)
    {
        return;
    }
    int nYPos = 0;
    if ( m_GroupImg1.GetImageLoadState() )
    {
        nYPos = 0 ;
        pShowAni->Show(0, m_Pnt.x + 50 , m_Pnt.y + nYPos);
        m_GroupImg1.Show ( m_Pnt.x, m_Pnt.y) ;
        int nIndex = 0 ;
        if ( m_btOwnerPos <= 0 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            m_GroupImg1.ShowHp( m_Pnt.x, m_Pnt.y, pInfo->dwHp, pInfo->dwMaxHp ) ;
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg1.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CPoint posPosition;
                posPosition.x = m_Pnt.x + 26;
                posPosition.y = m_Pnt.y + nYPos + 17;
                CDlgGroup::ShowTeamMemberPosition(nIndex, posPosition);
            }
        }
    }
    if ( m_GroupImg2.GetImageLoadState() )
    {
        nYPos += 64 ;
        pShowAni->Show(0, m_Pnt.x + 50 , m_Pnt.y + nYPos );
        m_GroupImg2.Show ( m_Pnt.x , m_Pnt.y ) ;
        int nIndex = 1 ;
        if ( m_btOwnerPos <= 1 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            m_GroupImg2.ShowHp( m_Pnt.x, m_Pnt.y, pInfo->dwHp, pInfo->dwMaxHp ) ;
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg2.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CPoint posPosition;
                posPosition.x = m_Pnt.x + 26;
                posPosition.y = m_Pnt.y + nYPos + 17;
                CDlgGroup::ShowTeamMemberPosition(nIndex, posPosition);
            }
        }
    }
    if ( m_GroupImg3.GetImageLoadState() )
    {
        nYPos += 64 ;
        pShowAni->Show(0, m_Pnt.x + 50, m_Pnt.y + nYPos);
        m_GroupImg3.Show ( m_Pnt.x, m_Pnt.y ) ;
        int nIndex = 2 ;
        if ( m_btOwnerPos <= 2 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            m_GroupImg3.ShowHp( m_Pnt.x, m_Pnt.y, pInfo->dwHp, pInfo->dwMaxHp ) ;
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg3.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CPoint posPosition;
                posPosition.x = m_Pnt.x + 26;
                posPosition.y = m_Pnt.y + nYPos + 17;
                CDlgGroup::ShowTeamMemberPosition(nIndex, posPosition);
            }
        }
    }
    if ( m_GroupImg4.GetImageLoadState() )
    {
        nYPos += 64 ;
        pShowAni->Show(0, m_Pnt.x + 50, m_Pnt.y + nYPos);
        m_GroupImg4.Show ( m_Pnt.x , m_Pnt.y ) ;
        int nIndex = 3 ;
        if ( m_btOwnerPos <= 3 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            m_GroupImg4.ShowHp( m_Pnt.x, m_Pnt.y, pInfo->dwHp, pInfo->dwMaxHp ) ;
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg4.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CPoint posPosition;
                posPosition.x = m_Pnt.x + 26;
                posPosition.y = m_Pnt.y + nYPos + 17;
                CDlgGroup::ShowTeamMemberPosition(nIndex, posPosition);
            }
        }
    }
    switch(m_uFocusPlayer)
    {
    case 1:
        m_GroupImg1.ShowFocus();
        break;
    case 2:
        m_GroupImg2.ShowFocus();
        break;
    case 3:
        m_GroupImg3.ShowFocus();
        break;
    case 4:
        m_GroupImg4.ShowFocus();
        break;
    default:
        break;
    }
    if (m_dwFocusTime != 0 && ::TimeGet() - m_dwFocusTime >= 5000)
    {
        m_dwFocusTime = 0;
        m_uFocusPlayer = 0;
    }
    CDlgGroup::ShowMemberName();
}

void CDlgGroup::OnGroupBtnClose()
{
    DXPlaySound( "Sound/Dlg_Ok.wav" ) ;
    CDlgGroup::EnableWindow( false ) ;
}

void CDlgGroup::SetGroupContain()
{
    m_GroupImg1.RemoveImage ( ) ;
    m_GroupImg2.RemoveImage ( ) ;
    m_GroupImg3.RemoveImage ( ) ;
    m_GroupImg4.RemoveImage ( ) ;
    m_GroupImg1.EnableWindow( false ) ;
    m_GroupImg2.EnableWindow( false ) ;
    m_GroupImg3.EnableWindow( false ) ;
    m_GroupImg4.EnableWindow( false ) ;
    m_btOwnerPos = 255 ;
    m_uFocusPlayer = 0 ;
    m_dwFocusTime = 0;
    int nNum = 0 ;
    int i = 0;
    for ( ; i < g_objHero.GetTeamMemberAmount (); i++ )
    {
        TeamMemberInfo* pTeamInfo = g_objHero.GetTeamMemberInfo ( i ) ;
        // Test code
        if ( pTeamInfo != NULL )
        {
            if ( pTeamInfo->id != g_objHero.GetID() )
            {
                nNum++ ;
                int nIndex = i ;
                if ( m_btOwnerPos != 255 && i > m_btOwnerPos )
                {
                    nIndex-- ;
                }
                switch ( nIndex )
                {
                case 0:
                    m_GroupImg1.EnableWindow() ;
                    m_GroupImg1.InsertImage ( NULL, pTeamInfo->dwFace, pTeamInfo->dwLook, false ) ;
                    break ;
                case 1:
                    m_GroupImg2.EnableWindow() ;
                    m_GroupImg2.InsertImage ( NULL, pTeamInfo->dwFace, pTeamInfo->dwLook, false ) ;
                    break ;
                case 2:
                    m_GroupImg3.EnableWindow() ;
                    m_GroupImg3.InsertImage ( NULL, pTeamInfo->dwFace, pTeamInfo->dwLook, false ) ;
                    break ;
                case 3:
                    m_GroupImg4.EnableWindow() ;
                    m_GroupImg4.InsertImage ( NULL, pTeamInfo->dwFace, pTeamInfo->dwLook, false ) ;
                    break ;
                }
                m_PlayerID[nIndex] = pTeamInfo->id ;
            }
            else
            {
                m_btOwnerPos = i ;
            }
        }
    }
    // Move the control buttons
    CRect rect ;
    CDlgGroup::GetWindowRect( rect ) ;
    m_GroupBtn1.MoveWindow( 5,
                            rect.Height() - 66,
                            44,
                            22, false ) ;
    m_GroupBtn1.ReInit( rect.left, rect.top ) ;
    m_GroupBtn2.MoveWindow( 54,
                            rect.Height() - 66,
                            44,
                            22, false ) ;
    m_GroupBtn2.ReInit( rect.left, rect.top ) ;
    m_GroupBtn3.MoveWindow( 5,
                            rect.Height() - 53,
                            44,
                            22 , false) ;
    m_GroupBtn3.ReInit( rect.left, rect.top ) ;
    m_GroupBtn4.MoveWindow( 54,
                            rect.Height() - 53,
                            44,
                            22 , false) ;
    m_GroupBtn4.ReInit( rect.left, rect.top ) ;
    m_GroupCloseBtn.MoveWindow( rect.Width() - 22,
                                rect.Height() - 34,
                                14,
                                14 , false) ;
    m_GroupCloseBtn.ReInit(rect.left, rect.top);
    if ( g_objHero.IsTeamLeader () )
    {
        m_uTeamMode = TEAM_MODE_LEADER ;
        m_GroupBtn1.ChangeImage ( "Button192" ) ;
        m_GroupBtn2.ChangeImage ( "Button194" ) ;
        m_GroupBtn1.MoveWindow( 5,
                                rect.Height() - 83,
                                44,
                                22 , false) ;
        m_GroupBtn1.ReInit( rect.left, rect.top ) ;
        m_GroupBtn2.MoveWindow( 54,
                                rect.Height() - 83,
                                44,
                                22 , false) ;
        m_GroupBtn2.ReInit( rect.left, rect.top ) ;
        m_GroupCloseBtn.MoveWindow( rect.Width() - 23,
                                    rect.Height() - 13,
                                    14,
                                    14 , false) ;
        m_GroupCloseBtn.ReInit(rect.left, rect.top);
        CRect Mrect;
        m_MenuGroup.GetWindowRect( Mrect ) ;
        m_MenuGroup.MoveWindow( m_Pnt.x - Mrect.Width(),
                                _SCR_HEIGHT - Mrect.Height() - 228,
                                Mrect.Width(),
                                Mrect.Height(), false ) ;
    }
    else if ( i > 0 )
    {
        m_uTeamMode = TEAM_MODE_MEMBER ;
        m_GroupBtn1.ChangeImage ( "Button193" ) ;
        m_GroupBtn2.ChangeImage ( "Button196" ) ;
    }
    else
    {
        m_uTeamMode = TEAM_MODE_NONE ;
        m_GroupBtn1.ChangeImage ( "Button190" ) ;
        m_GroupBtn2.ChangeImage ( "Button191" ) ;
    }
}

void CDlgGroup::CreateTeamOk()
{
    if ( g_objHero.IsTeamLeader () )
    {
        m_MenuGroup.m_GroupChkMoney.SetCheckValue( 1 ) ;
        m_MenuGroup.m_GroupChkItem.SetCheckValue( 0 ) ;
        m_MenuGroup.m_GroupChkStop.SetCheckValue( 0 ) ;
        m_MenuGroup.m_GroupChkJoin.SetCheckValue( 0 ) ;
        g_objHero.SetTeamMoneyAccess ( m_MenuGroup.m_GroupChkMoney.GetCheckValue () ) ;
        g_objHero.SetTeamItemAccess ( m_MenuGroup.m_GroupChkItem.GetCheckValue () ) ;
        g_objHero.SetTeamGemAccess( m_MenuGroup.m_GroupChkStop.GetCheckValue() ) ;
        g_objHero.TeamOpen();
    }
}
void CDlgGroup::OnGroupBtn1()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    switch ( m_uTeamMode )
    {
    case TEAM_MODE_NONE:
        if (g_objHero.TestStatus(USERSTATUS_GHOST))
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10700));
            CDlgGroup::EnableWindow(false);
            return;
        }
        g_objHero.TeamCreate () ;
        m_MenuGroup.m_bCreate = true ;
        m_MenuGroup.ResetChecks();
        m_GroupBtn5.EnableWindow(true);
        break ;
    case TEAM_MODE_LEADER:
        if (g_objHero.TestStatus(USERSTATUS_GHOST))
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10706));
            CDlgGroup::EnableWindow(false);
            return;
        }
        if (g_objHero.IsTeamClosed())
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100112));
            return;
        }
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10701));//请选择您想邀请的对象
        CDlgGroup::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_GROUP_INVITE ) ;
        break ;
    case TEAM_MODE_MEMBER:
        g_objHero.TeamLeave () ;
        CDlgGroup::EnableWindow(false);
        break ;
    }
}

void CDlgGroup::OnGroupBtn2()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    switch ( m_uTeamMode )
    {
    case TEAM_MODE_NONE:
        if (g_objHero.IsDead())
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10705));//亡灵状态下不能申请入队
            CDlgGroup::EnableWindow(false);
        }
        else
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10702));//请选择您想加入的队伍
            CDlgGroup::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_GROUP_JOIN ) ;
        }
        break ;
    case TEAM_MODE_LEADER:
        CDlgGroup::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TEAM_DISMISS);
        break ;
    case TEAM_MODE_MEMBER:
        if (!g_objHero.IsDead ())
        {
            g_objHero.TeamFollow () ;
        }
        break ;
    }
}

void CDlgGroup::OnGroupChk3()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    switch ( m_uTeamMode )
    {
    case TEAM_MODE_NONE:
        break ;
    case TEAM_MODE_LEADER:
        if ( !m_MenuGroup.m_bShow )
        {
            CDlgGroup::GetParent()->PostMessage(WM_MY_MESSAGE, ON_GROUP_SETUP);
        }
        else
        {
            m_MenuGroup.EnableWindow( false ) ;
        }
        break ;
    case TEAM_MODE_MEMBER:
        break ;
    }
}

void CDlgGroup::OnGroupBtn4()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    switch ( m_uTeamMode )
    {
    case TEAM_MODE_NONE:
        break ;
    case TEAM_MODE_LEADER:
        {
            if ( m_uFocusPlayer != 0 )
            {
                g_objHero.TeamKickOut ( m_PlayerID[m_uFocusPlayer - 1] ) ;
                m_uFocusPlayer = 0;
                m_dwFocusTime = 0;
            }
            else
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10709));//请选择你要踢出队伍的对象
            }
        }
        break ;
    case TEAM_MODE_MEMBER:
        break ;
    }
}

void CDlgGroup::OnGroupImg1()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_GroupImg1.GetImageLoadState () && m_uTeamMode == TEAM_MODE_LEADER )
    {
        SetFocusPlayer(1);
    }
}

void CDlgGroup::OnGroupImg2()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_GroupImg2.GetImageLoadState () && m_uTeamMode == TEAM_MODE_LEADER )
    {
        SetFocusPlayer(2);
    }
}

void CDlgGroup::OnGroupImg3()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_GroupImg3.GetImageLoadState () && m_uTeamMode == TEAM_MODE_LEADER )
    {
        SetFocusPlayer(3);
    }
}

void CDlgGroup::OnGroupImg4()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_GroupImg4.GetImageLoadState () && m_uTeamMode == TEAM_MODE_LEADER )
    {
        SetFocusPlayer(4);
    }
}


void CDlgGroup::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgGroup::GetParent()->PostMessage( WM_MOUSEMOVE, nFlags, lParam ) ;
    CDlgGroup::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MSGDLG_HIDE);
    CDialog::OnMouseMove(nFlags, point);
}

void CDlgGroup::OnLButtonDown(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgGroup::GetParent()->PostMessage( WM_LBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgGroup::OnLButtonUp(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgGroup::GetParent()->PostMessage( WM_LBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgGroup::OnRButtonDown(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgGroup::GetParent()->PostMessage( WM_RBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgGroup::OnRButtonUp(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgGroup::GetParent()->PostMessage( WM_RBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgGroup::OnGroupBtn5()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgGroup::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TEAMMAGIC_OPEN );
}

void CDlgGroup::ShowTeamMemberPosition(int nIndex, CPoint posPosition)
{
    TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo(nIndex);
    if (pInfo != NULL)
    {
        //		if (pInfo->id != g_objHero.GetTeamMemberFlashID())
        if (nIndex != 0)
        {
            g_objHero.QueryTeamMemberPos(pInfo->id);
        }
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Position",
                        EXIGENCE_IMMEDIATE ) ;
        if (ShowAni != NULL)
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pInfo->id);
            if (pPlayer != NULL)
            {
                int nFrame = -1;
                CMyPos posTeamCell, posTeamBg,  posTeamWorld;
                pPlayer->GetPos(posTeamCell);
                g_objGameMap.Cell2World(posTeamCell.x, posTeamCell.y, posTeamWorld.x, posTeamWorld.y);
                g_objGameMap.World2Bg(posTeamWorld.x, posTeamWorld.y, posTeamBg.x, posTeamBg.y);
                CMyPos posHeroCell, posHeroBg,  posHeroWorld;
                g_objHero.GetPos(posHeroCell);
                g_objGameMap.Cell2World(posHeroCell.x, posHeroCell.y, posHeroWorld.x, posHeroWorld.y);
                g_objGameMap.World2Bg(posHeroWorld.x, posHeroWorld.y, posHeroBg.x, posHeroBg.y);
                if (posTeamBg.x < posHeroBg.x
                        && posTeamBg.y == posHeroBg.y)
                {
                    nFrame = 0;	//Left
                }
                else if (posTeamBg.x > posHeroBg.x
                         && posTeamBg.y == posHeroBg.y)
                {
                    nFrame = 1; //right
                }
                else if (posTeamBg.x == posHeroBg.x
                         && posTeamBg.y < posHeroBg.y)
                {
                    nFrame = 2; //Down
                }
                else if (posTeamBg.x == posHeroBg.x
                         && posTeamBg.y > posHeroBg.y)
                {
                    nFrame = 3; //Up
                }
                else if (posTeamBg.x > posHeroBg.x
                         && posTeamBg.y < posHeroBg.y)
                {
                    nFrame = 4;
                }
                else if (posTeamBg.x < posHeroBg.x
                         && posTeamBg.y < posHeroBg.y)
                {
                    nFrame = 5;
                }
                else if (posTeamBg.x > posHeroBg.x
                         && posTeamBg.y > posHeroBg.y)
                {
                    nFrame = 6;
                }
                else if (posTeamBg.x < posHeroBg.x
                         && posTeamBg.y > posHeroBg.y)
                {
                    nFrame = 7;
                }
                if (nFrame != -1)
                    ShowAni->Show(nFrame, posPosition.x , //m_MsgDlgPnt.x - 34,
                                  posPosition.y );//m_MsgDlgPnt.y + 20);
            }
        }
    }
}

void CDlgGroup::ShowMemberName()
{
    int nYPos = 0;
    if ( m_GroupImg1.GetImageLoadState() )
    {
        nYPos = 0 ;
        int nIndex = 0 ;
        if ( m_btOwnerPos <= 0 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg1.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                int nXPos = _SCR_WIDTH - strlen(pInfo->szName) * FontSize.iWidth;
                CMyBitmap::ShowString( nXPos,
                                       m_Pnt.y + nYPos + 66,
                                       0xffffffff,
                                       pInfo->szName ) ;
            }
        }
    }
    if ( m_GroupImg2.GetImageLoadState() )
    {
        nYPos += 64 ;
        int nIndex = 1 ;
        if ( m_btOwnerPos <= 1 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg2.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                int nXPos = _SCR_WIDTH - strlen(pInfo->szName) * FontSize.iWidth;
                CMyBitmap::ShowString( nXPos,
                                       m_Pnt.y + nYPos + 66,
                                       0xffffffff,
                                       pInfo->szName ) ;
            }
        }
    }
    if ( m_GroupImg3.GetImageLoadState() )
    {
        nYPos += 64 ;
        int nIndex = 2 ;
        if ( m_btOwnerPos <= 2 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg3.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                int nXPos = _SCR_WIDTH - strlen(pInfo->szName) * FontSize.iWidth;
                CMyBitmap::ShowString( nXPos,
                                       m_Pnt.y + nYPos + 66,
                                       0xffffffff,
                                       pInfo->szName ) ;
            }
        }
    }
    if ( m_GroupImg4.GetImageLoadState() )
    {
        nYPos += 64 ;
        int nIndex = 3 ;
        if ( m_btOwnerPos <= 3 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg4.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                CMySize FontSize;
                CMyBitmap::GetFontSize(FontSize);
                int nXPos = _SCR_WIDTH - strlen(pInfo->szName) * FontSize.iWidth;
                CMyBitmap::ShowString( nXPos,
                                       m_Pnt.y + nYPos + 66,
                                       0xffffffff,
                                       pInfo->szName ) ;
            }
        }
    }
}

void CDlgGroup::TeamDismiss()
{
    if ( m_uTeamMode == TEAM_MODE_LEADER )
    {
        g_objHero.TeamDismiss () ;
        m_GroupBtn5.EnableWindow(false);
        if ( m_MenuGroup.m_bShow )
        {
            m_MenuGroup.EnableWindow( false ) ;
        }
        CDlgGroup::EnableWindow(false);
    }
}

OBJID CDlgGroup::GetMouseInMember()
{
    if ( m_GroupImg1.GetImageLoadState() )
    {
        int nIndex = 0 ;
        if ( m_btOwnerPos <= 0 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg1.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                return pInfo->id;
            }
        }
    }
    if ( m_GroupImg2.GetImageLoadState() )
    {
        int nIndex = 1 ;
        if ( m_btOwnerPos <= 1 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg2.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                return pInfo->id;
            }
        }
    }
    if ( m_GroupImg3.GetImageLoadState() )
    {
        int nIndex = 2 ;
        if ( m_btOwnerPos <= 2 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg3.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                return pInfo->id;
            }
        }
    }
    if ( m_GroupImg4.GetImageLoadState() )
    {
        int nIndex = 3 ;
        if ( m_btOwnerPos <= 3 )
        {
            nIndex++ ;
        }
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo( nIndex ) ;
        if ( pInfo )
        {
            CPoint 	point;
            ::GetCursorPos(&point);
            CRect rect;
            m_GroupImg4.GetWindowRect(rect);
            if (rect.PtInRect(point))
            {
                return pInfo->id;
            }
        }
    }
    return 0;
}

void CDlgGroup::SetFocusPlayer(int nIndex)
{
    m_uFocusPlayer = nIndex;
    m_dwFocusTime = ::TimeGet();
}
