
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgMsgReceive.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgMsgReceive.h"

#include "GameMsg.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgReceive dialog


CDlgMsgReceive::CDlgMsgReceive(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMsgReceive::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMsgReceive)
    m_bShow = false ;
    m_nCurIndex = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgMsgReceive::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMsgReceive)
    DDX_Control(pDX, IDC_STATIC_MSG, m_StaMsgReceive);
    DDX_Control(pDX, IDC_MSGRECEIVE_TIME, m_MsgReceiveTime);
    DDX_Control(pDX, IDC_BTN_UP, m_BtnUp);
    DDX_Control(pDX, IDC_BTN_DOWN, m_BtnDown);
    DDX_Control(pDX, IDC_MSGRECEIVE_NAME, m_MsgReceiveName);
    DDX_Control(pDX, IDC_MSGRECEIVE_CLOSEB, m_MsgReceiveClose);
    DDX_Control(pDX, IDC_MSGRECEIVE_REVERT, m_MsgReceiveRevert);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMsgReceive, CDialog)
    //{{AFX_MSG_MAP(CDlgMsgReceive)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_MSGRECEIVE_CLOSEB, OnMsgreceiveCloseb)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_MSGRECEIVE_REVERT, OnMsgreceiveRevert)
    ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
    ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgReceive message handlers

LRESULT CDlgMsgReceive::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if ( m_bShow )
        {
            m_nCurIndex = 0;
            CDlgMsgReceive::SetReceiveMsg() ;
            DXPlaySound("Sound/Open.wav");
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
            char strName[32] ;
            m_MsgReceiveName.GetWindowText(strName, 32);
            for(int i = 0; i < m_nCurIndex + 1; i++)
            {
                g_objGameMsg.DelLeaveWordByIndexName(0, strName);
            }
            CDlgMsgReceive::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MSGREC_CLOSE);
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgMsgReceive::OnMsgreceiveCloseb();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgMsgReceive::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgMsgReceive::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMsgReceive::GetWindowRect ( rect ) ;
    // Init the button
    m_MsgReceiveClose.Init ( rect.left, rect.top, "Button540" ) ;
    m_MsgReceiveRevert.Init ( rect.left, rect.top, "Button332" ) ;
    m_BtnUp.Init ( rect.left, rect.top, "Button271" ) ;
    m_BtnDown.Init ( rect.left, rect.top, "Button272" ) ;
    // Init the static
    m_MsgReceiveName.Init ( rect.left, rect.top ) ;
    m_MsgReceiveTime.Init ( rect.left, rect.top ) ;
    m_StaMsgReceive.Init(rect.left, rect.top, 2);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMsgReceive::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgMsgReceive::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog39",
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
        m_MsgReceiveClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MsgReceiveRevert.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnUp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnDown.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the static
        m_MsgReceiveName.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_MsgReceiveTime.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_StaMsgReceive.Show(m_Pnt.x, m_Pnt.y );
    }
}


void CDlgMsgReceive::OnMsgreceiveCloseb()
{
    CDlgMsgReceive::EnableWindow( false ) ;
}

void CDlgMsgReceive::OnBtnHelp()
{
    CDlgMsgReceive::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_MSGRECEIVE ) ;
}

void CDlgMsgReceive::OnMsgreceiveRevert()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    char* strName = new char[32] ;
    m_MsgReceiveName.GetWindowText( strName, 32 ) ;
    CFriend* pFriend = g_objHero.GetFriendByName(strName);
    if (pFriend )
    {
        if (!pFriend->IsOnline())
        {
            CDlgMsgReceive::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MSGREC_REVERT, ( LPARAM )strName ) ;
        }
        else
        {
            CDlgMsgReceive::GetParent()->PostMessage(WM_MY_MESSAGE, ON_FRIEND_CHAT, (LPARAM)strName);
        }
    }
}

void CDlgMsgReceive::SetReceiveMsg()
{
    char strTxt[256] = "" ;
    char strName[32] = "" ;
    char strTime[32] = "" ;
    m_MsgReceiveName.GetWindowText(strName, 32);
    if (g_objGameMsg.GetLeaveWordByIndexName(m_nCurIndex, strName, strTxt, strTime))
    {
        m_StaMsgReceive.SetWindowText(strTxt);
        m_MsgReceiveTime.SetWindowText(strTime);
        CDlgMsgReceive::GetParent()->PostMessage(WM_MY_MESSAGE, CMD_FLASHFRIENDLIST);
    }
}

void CDlgMsgReceive::OnBtnUp()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    if ( m_nCurIndex > 0 )
    {
        m_nCurIndex-- ;
        CDlgMsgReceive::SetReceiveMsg() ;
    }
}

void CDlgMsgReceive::OnBtnDown()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    char strName[32] ;
    m_MsgReceiveName.GetWindowText(strName, 32);
    if ( m_nCurIndex < g_objGameMsg.GetLeaveWordAmountByName(strName) - 1)
    {
        m_nCurIndex++ ;
        CDlgMsgReceive::SetReceiveMsg() ;
    }
}

void CDlgMsgReceive::SetMsgReceiveName(const char* strName)
{
    if (strName && strlen(strName))
    {
        m_MsgReceiveName.SetWindowText(strName);
    }
}
