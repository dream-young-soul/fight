
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMsgSend.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMsgSend.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgSend dialog


CDlgMsgSend::CDlgMsgSend(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMsgSend::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMsgSend)
    m_bShow = false ;
    m_FriendID = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgMsgSend::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMsgSend)
    DDX_Control(pDX, IDC_MSGSEND_DATA, m_MsgSendData);
    DDX_Control(pDX, IDC_MSGSEND_SEND, m_MsgSendSend);
    DDX_Control(pDX, IDC_MSGSEND_NAME, m_MsgSendName);
    DDX_Control(pDX, IDC_MSGSEND_CLOSEB, m_MsgSendClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMsgSend, CDialog)
    //{{AFX_MSG_MAP(CDlgMsgSend)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_MSGSEND_CLOSEB, OnMsgsendCloseb)
    ON_BN_CLICKED(IDC_MSGSEND_SEND, OnMsgsendSend)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgSend message handlers

LRESULT CDlgMsgSend::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            //			CDlgMsgSend::OnMsgsendSend();
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgMsgSend::OnMsgsendCloseb();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMsgSend::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgMsgSend::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMsgSend::GetWindowRect ( rect ) ;
    // Init the button
    m_MsgSendSend.Init ( rect.left, rect.top, "Button331" ) ;
    m_MsgSendClose.Init ( rect.left, rect.top, "Button540" ) ;
    // Init the static
    m_MsgSendName.Init ( rect.left, rect.top ) ;
    // Init the edit
    m_MsgSendData.Init ( rect.left, rect.top , true) ;
    m_MsgSendData.SetLimitText ( 256 ) ;
    m_MsgSendData.SetOffset ( CPoint ( 2, 3 ) ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMsgSend::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgMsgSend::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog33",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
            ShowAni->Show ( 1,
                            m_Pnt.x + 256,
                            m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        // Show the buttons
        m_MsgSendSend.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MsgSendClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_MsgSendName.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the edit
        m_MsgSendData.Show ( m_Pnt.x, m_Pnt.y ) ;
    }
}

void CDlgMsgSend::OnBtnHelp()
{
    CDlgMsgSend::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_MSGSEND ) ;
}

void CDlgMsgSend::OnMsgsendCloseb()
{
    CDlgMsgSend::EnableWindow( false ) ;
}

void CDlgMsgSend::OnMsgsendSend()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    char strText[256] ;
    m_MsgSendData.GetWindowText ( strText, 256 ) ;
    if ( strlen( strText ) > 0 )
    {
        g_objHero.LeaveWords2Friend ( m_FriendID, strText ) ;
        m_MsgSendData.SetWindowText ( NULL ) ;
        CDlgMsgSend::EnableWindow( false ) ;
    }
    else
    {
        CDlgMsgSend::GetParent()->PostMessage( WM_MY_MESSAGE, ON_EMPTY_MSG ) ;
    }
}

void CDlgMsgSend::SetTargetFriend(const char* strName)
{
    if ( strName == NULL )
    {
        return ;
    }
    CFriend* pFriend = g_objHero.GetFriendByName( strName ) ;
    if ( pFriend )
    {
        m_MsgSendName.SetWindowText ( pFriend->GetName () ) ;
        m_FriendID = pFriend->GetID () ;
    }
    /*	if ( g_objGameMsg.m_objChatLog.GetLeaveWordLineAmount() > 0 )
    		m_MsgSendAllMsg.EnableWindow ( true ) ;
    	else
    		m_MsgSendAllMsg.EnableWindow ( false ) ;*/
}
