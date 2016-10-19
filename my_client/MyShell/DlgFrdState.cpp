
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgFrdState.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgFrdState.h"

#include "Hero.h"
#include "Friend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFrdState dialog


CDlgFrdState::CDlgFrdState(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgFrdState::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgFrdState)
    m_bShow = false ;
    //}}AFX_DATA_INIT
}


void CDlgFrdState::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgFrdState)
    DDX_Control(pDX, IDC_FRDSTATE_IMAFACE, m_FrdStateImaface);
    DDX_Control(pDX, IDC_STA6, m_Sta6);
    DDX_Control(pDX, IDC_STA5, m_Sta5);
    DDX_Control(pDX, IDC_STA1, m_Sta1);
    DDX_Control(pDX, IDC_STA2, m_Sta2);
    DDX_Control(pDX, IDC_STA3, m_Sta3);
    DDX_Control(pDX, IDC_STA4, m_Sta4);
    DDX_Control(pDX, IDC_STA7, m_Sta7);
    DDX_Control(pDX, IDC_FRDSTATE_BTN_DEL, m_FrdStateBtnDel);
    DDX_Control(pDX, IDC_FRDSTATE_BTN_CLOSE, m_FrdStateBtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFrdState, CDialog)
    //{{AFX_MSG_MAP(CDlgFrdState)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_FRDSTATE_BTN_CLOSE, OnFrdstateBtnClose)
    ON_BN_CLICKED(IDC_FRDSTATE_BTN_DEL, OnFrdstateBtnDel)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFrdState message handlers

LRESULT CDlgFrdState::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
        if ( wParam )
        {
            CDlgFrdState::GetParent()->PostMessage ( WM_MY_MESSAGE, ON_FRDSTATE_SHOW ) ;
            DXPlaySound ( "Sound/BtnClick.wav" ) ;
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
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgFrdState::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgFrdState::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgFrdState::GetWindowRect ( rect ) ;
    //	m_fFrameStep = ( float ) rect.Width () / DLG_FRAME_AMOUNT ;
    // Init the image
    if ( !m_FrdStateImaface.Init ( rect.left, rect.top, NULL, 2 ) )
    {
        return false ;
    }
    // Init the Static
    m_Sta1.Init ( rect.left, rect.top ) ;
    m_Sta2.Init ( rect.left, rect.top ) ;
    m_Sta3.Init ( rect.left, rect.top ) ;
    m_Sta4.Init ( rect.left, rect.top ) ;
    m_Sta5.Init ( rect.left, rect.top ) ;
    m_Sta6.Init ( rect.left, rect.top ) ;
    m_Sta7.Init ( rect.left, rect.top ) ;
    // Init the button
    if ( ! m_FrdStateBtnDel.Init ( rect.left, rect.top, "Button250" ) )
    {
        return false ;
    }
    if ( ! m_FrdStateBtnClose.Init ( rect.left, rect.top, "Button5" ) )
    {
        return false ;
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFrdState::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgFrdState::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog25",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y + 1 ) ;
        else
        {
            return ;
        }
        // Show the image
        m_FrdStateImaface.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_Sta1.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta2.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta3.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta4.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta5.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta6.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_Sta7.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_FrdStateBtnDel.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_FrdStateBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgFrdState::OnFrdstateBtnClose()
{
    CDlgFrdState::ShowWindow ( SW_HIDE ) ;
}

void CDlgFrdState::OnFrdstateBtnDel()
{
    CDlgFrdState::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FRDSTATE_DEL ) ;
}

void CDlgFrdState::OnBtnHelp()
{
    // TODO: Add your control notification handler code here
}

void CDlgFrdState::SetFriendState(char* strFriendName)
{
    CFriend* pFriend = g_objHero.GetFriendByName ( strFriendName ) ;
    if ( pFriend != NULL )
    {
        m_FrdStateImaface.RemoveImage () ;
        char strText[16] ;
        itoa ( pFriend->GetFace (), strText, 10 ) ;
        m_FrdStateImaface.InsertImage ( strText ) ;
        m_Sta1.SetWindowText ( pFriend->GetName () ) ;
        itoa ( pFriend->GetLevel (), strText, 10 ) ;
        m_Sta2.SetWindowText ( strText ) ;
        m_Sta3.SetWindowText ( pFriend->GetProfessionalName () ) ;
        itoa ( pFriend->GetPK (), strText, 10 ) ;
        m_Sta4.SetWindowText ( strText ) ;
        m_Sta5.SetWindowText ( pFriend->GetSyndicateName () ) ;
        strcpy ( strText, pFriend->GetRankName () ) ;
        m_Sta6.SetWindowText ( strText ) ;
        m_Sta7.SetWindowText ( pFriend->GetMate () ) ;
    }
}

void CDlgFrdState::DeleteFriend()
{
    char strName[16] ;
    m_Sta1.GetWindowText ( strName, 16 ) ;
    if ( strName != NULL )
    {
        CFriend* pFriend = g_objHero.GetFriendByName ( strName ) ;
        if ( pFriend )
        {
            g_objHero.BreakOffFriend ( pFriend->GetID () ) ;
        }
    }
    CDlgFrdState::ShowWindow ( SW_HIDE ) ;
}

void CDlgFrdState::SetFriendNull(char* strName)
{
    m_FrdStateImaface.RemoveImage () ;
    m_Sta1.SetWindowText ( strName ) ;
    m_Sta2.SetWindowText ( NULL ) ;
    m_Sta3.SetWindowText ( NULL ) ;
    m_Sta4.SetWindowText ( NULL ) ;
    m_Sta5.SetWindowText ( NULL ) ;
    m_Sta6.SetWindowText ( NULL ) ;
    m_Sta7.SetWindowText ( NULL ) ;
}
