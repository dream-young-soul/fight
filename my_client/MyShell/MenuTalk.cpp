
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MenuTalk.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MenuTalk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuTalk dialog


CMenuTalk::CMenuTalk(CWnd* pParent /*=NULL*/)
    : CDialog(CMenuTalk::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMenuTalk)
    m_bShow = false ;
    m_ParentWnd = NULL ;
    //}}AFX_DATA_INIT
}


void CMenuTalk::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMenuTalk)
    DDX_Control(pDX, IDC_TALK_BTN_G, m_TalkBtnG ) ;
    DDX_Control(pDX, IDC_TALK_BTN_S, m_TalkBtnS ) ;
    DDX_Control(pDX, IDC_TALK_BTN_D, m_TalkBtnD ) ;
    DDX_Control(pDX, IDC_TALK_BTN_B, m_TalkBtnB ) ;
    DDX_Control(pDX, IDC_TALK_BTN_H, m_TalkBtnH ) ;
    DDX_Control(pDX, IDC_TALK_BTN_W, m_TalkBtnW ) ;
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuTalk, CDialog)
    //{{AFX_MSG_MAP(CMenuTalk)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_TALK_BTN_G, OnTalkBtnG)
    ON_BN_CLICKED(IDC_TALK_BTN_S, OnTalkBtnS)
    ON_BN_CLICKED(IDC_TALK_BTN_D, OnTalkBtnD)
    ON_BN_CLICKED(IDC_TALK_BTN_B, OnTalkBtnB)
    ON_BN_CLICKED(IDC_TALK_BTN_H, OnTalkBtnH)
    ON_BN_CLICKED(IDC_TALK_BTN_W, OnTalkBtnW)	//Wen Yiku Code 2005-6-29
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuTalk message handlers

LRESULT CMenuTalk::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
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

HBRUSH CMenuTalk::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CMenuTalk::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CMenuTalk::GetWindowRect ( rect ) ;
    // Init the button
    if ( ! ( m_TalkBtnG.Init ( rect.left, rect.top, "Button50" ) ) )
    {
        return false ;
    }
    if ( ! ( m_TalkBtnS.Init ( rect.left, rect.top, "Button51" ) ) )
    {
        return false ;
    }
    if ( ! ( m_TalkBtnD.Init ( rect.left, rect.top, "Button52" ) ) )
    {
        return false ;
    }
    if ( ! ( m_TalkBtnB.Init ( rect.left, rect.top, "Button53" ) ) )
    {
        return false ;
    }
    if ( ! ( m_TalkBtnH.Init ( rect.left, rect.top, "Button54" ) ) )
    {
        return false ;
    }
    if ( ! ( m_TalkBtnW.Init ( rect.left, rect.top, "Button56" ) ) ) //Wen Yiku Code 2005-6-29
    {
        return false ;
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuTalk::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CMenuTalk::Show()
{
    if ( m_bShow )
    {
        // show the button
        m_TalkBtnG.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkBtnS.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkBtnD.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkBtnB.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkBtnH.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkBtnW.Show ( m_Pnt.x, m_Pnt.y ) ; //Wen Yiku Code 2005-6-29
    }
}

void CMenuTalk::SetParentWnd(CWnd* pParentWnd)
{
    m_ParentWnd = pParentWnd ;
}

void CMenuTalk::OnTalkBtnG()
{
    if ( m_ParentWnd )
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_TALK_CLICK, G_CLICK ) ;
    }
    CMenuTalk::EnableWindow( false ) ;
}

void CMenuTalk::OnTalkBtnS()
{
    if ( m_ParentWnd )
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_TALK_CLICK, S_CLICK ) ;
    }
    CMenuTalk::EnableWindow( false ) ;
}

void CMenuTalk::OnTalkBtnD()
{
    if ( m_ParentWnd )
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_TALK_CLICK, D_CLICK ) ;
    }
    CMenuTalk::EnableWindow( false ) ;
}

void CMenuTalk::OnTalkBtnB()
{
    if ( m_ParentWnd )
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_TALK_CLICK, B_CLICK ) ;
    }
    CMenuTalk::EnableWindow( false ) ;
}

void CMenuTalk::OnTalkBtnH()
{
    if ( m_ParentWnd )
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_TALK_CLICK, H_CLICK ) ;
    }
    CMenuTalk::EnableWindow( false ) ;
}

// Wen Yiku Code 2005-6-29
void CMenuTalk::OnTalkBtnW()
{
    if ( m_ParentWnd )
    {
        //not sound file	// must modify
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        m_ParentWnd->PostMessage ( WM_MY_MESSAGE, MENU_TALK_CLICK, W_CLICK ) ;
    }
    CMenuTalk::EnableWindow( false ) ;
}
