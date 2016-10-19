
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgLevWord.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgLevWord.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLevWord dialog


CDlgLevWord::CDlgLevWord(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLevWord::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgLevWord)
    m_Pnt = CPoint (0 ) ;
    m_btShowMode = 0 ;
    m_usCurChannel = _TXTATR_MSG_TRADE ;
    m_nCurIndex = 0 ;
    m_bShow = false ;
    //}}AFX_DATA_INIT
}


void CDlgLevWord::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLevWord)
    DDX_Control(pDX, IDC_BTN_OK, m_BtnOk);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_LEVWORD_EDT_DATA, m_LevWordEdtData);
    DDX_Control(pDX, IDC_LEVWORD_LST_LIST, m_LevWordLstList);
    DDX_Control(pDX, IDC_LEVWORD_CLOSEB, m_LevWordCloseB);
    DDX_Control(pDX, IDC_LEVWORD_BTN_WATCH, m_LevWordBtnWatch);
    DDX_Control(pDX, IDC_LEVWORD_BTN_UP, m_LevWordBtnUp);
    DDX_Control(pDX, IDC_LEVWORD_BTN_TRADE, m_LevWordBtnTrade);
    DDX_Control(pDX, IDC_LEVWORD_BTN_SEND, m_LevWordBtnSend);
    DDX_Control(pDX, IDC_LEVWORD_BTN_OTHER, m_LevWordBtnOther);
    DDX_Control(pDX, IDC_LEVWORD_BTN_GROUP, m_LevWordBtnGroup);
    DDX_Control(pDX, IDC_LEVWORD_BTN_FRIEND, m_LevWordBtnFriend);
    DDX_Control(pDX, IDC_LEVWORD_BTN_FLASH, m_LevWordBtnFlash);
    DDX_Control(pDX, IDC_LEVWORD_BTN_FACTION, m_LevWordBtnFaction);
    DDX_Control(pDX, IDC_LEVWORD_BTN_DOWN, m_LevWordBtnDown);
    DDX_Control(pDX, IDC_LEVWORD_BTN_DEL, m_LevWordBtnDel);
    DDX_Control(pDX, IDC_LEVWORD_BTN_CLOSES, m_LevWordBtnCloseS);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLevWord, CDialog)
    //{{AFX_MSG_MAP(CDlgLevWord)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_LEVWORD_BTN_SEND, OnLevwordBtnSend)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_WATCH, OnLevwordBtnWatch)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_DEL, OnLevwordBtnDel)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_UP, OnLevwordBtnUp)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_DOWN, OnLevwordBtnDown)
    ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_FLASH, OnLevwordBtnFlash)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_TRADE, OnLevwordBtnTrade)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_FRIEND, OnLevwordBtnFriend)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_GROUP, OnLevwordBtnGroup)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_FACTION, OnLevwordBtnFaction)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_OTHER, OnLevwordBtnOther)
    ON_BN_CLICKED(IDC_LEVWORD_CLOSEB, OnLevwordCloseb)
    ON_BN_CLICKED(IDC_LEVWORD_BTN_CLOSES, OnLevwordBtnCloses)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLevWord message handlers

LRESULT CDlgLevWord::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_SHOWWINDOW:
        m_bShow = wParam ;
        if ( m_bShow )
        {
            g_objHero.m_objBulletin.QueryList ( m_nCurIndex, m_usCurChannel ) ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
        break ;
    case CB_GETITEMDATA:
        {
            if ( wParam )
            {
                CDlgLevWord::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_LEVWORD_CHAT, lParam ) ;
            }
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgLevWord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgLevWord::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgLevWord::GetWindowRect ( rect ) ;
    // Init the buttons
    m_LevWordCloseB.Init ( rect.left, rect.top, "Button3" ) ;
    m_LevWordBtnWatch.Init ( rect.left, rect.top, "Button371" ) ;
    m_LevWordBtnUp.Init ( rect.left, rect.top, "Button271" ) ;
    m_LevWordBtnTrade.Init ( rect.left, rect.top, "Button375" ) ;
    m_LevWordBtnTrade.SetCurFrame ( 2 ) ;
    m_LevWordBtnSend.Init ( rect.left, rect.top, "Button370" ) ;
    m_LevWordBtnOther.Init ( rect.left, rect.top, "Button379" ) ;
    m_LevWordBtnGroup.Init ( rect.left, rect.top, "Button377" ) ;
    m_LevWordBtnFriend.Init ( rect.left, rect.top, "Button376" ) ;
    m_LevWordBtnFlash.Init ( rect.left, rect.top, "Button374" ) ;
    m_LevWordBtnFaction.Init ( rect.left, rect.top, "Button378" ) ;
    m_LevWordBtnDown.Init ( rect.left, rect.top, "Button272" ) ;
    m_LevWordBtnDel.Init ( rect.left, rect.top, "Button373" ) ;
    m_LevWordBtnCloseS.Init ( rect.left, rect.top, "Button5" ) ;
    m_BtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    m_BtnOk.Init ( rect.left, rect.top, "Button0" ) ;
    m_BtnCancel.Init ( rect.left, rect.top, "Button7" ) ;
    /*	// Init the list
    	m_LevWordLstList.Init ( rect.left, rect.top, NULL, 1 ) ;
    	m_LevWordLstList.SetFontColor ( 0xffffff ) ;
    	m_LevWordLstList.SetParentCob ( this ) ;*/
    // Init the edit
    m_LevWordEdtData.Init ( rect.left, rect.top , true) ;
    m_LevWordEdtData.SetLimitText ( 256 ) ;
    m_LevWordEdtData.SetOffset ( CPoint ( 2, 3 ) ) ;
    m_LevWordEdtData.EnableWindow ( false ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLevWord::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgLevWord::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog37",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        else
        {
            return ;
        }
        // Show the title
        char strTitle[32] ;
        sprintf ( strTitle, "Image37%d", m_btShowMode + 1 ) ;
        ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                                strTitle,
                                                EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x + 300,
                            m_Pnt.y + 7 ) ;
        else
        {
            return ;
        }
        // Show the button
        m_LevWordCloseB.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnWatch.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnUp.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnTrade.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnSend.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnOther.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnGroup.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnFriend.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnFlash.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnFaction.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnDown.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnDel.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_LevWordBtnCloseS.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the different mode
        switch ( m_btShowMode )
        {
        case 0:
            // Show the list
            m_LevWordLstList.Show ( m_Pnt.x, m_Pnt.y ) ;
            break ;
        default:
            // Show the bk
            ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                                    "Image370",
                                                    EXIGENCE_IMMEDIATE ) ;
            if ( ShowAni != NULL )
                ShowAni->Show ( 0,
                                m_Pnt.x + 54,
                                m_Pnt.y + 60) ;
            else
            {
                return ;
            }
            // Show the edit
            m_LevWordEdtData.Show ( m_Pnt.x, m_Pnt.y ) ;
            // Show the button
            if ( m_BtnOk.IsWindowEnabled () )
            {
                m_BtnOk.Show ( m_Pnt.x, m_Pnt.y ) ;
            }
            m_BtnCancel.Show ( m_Pnt.x, m_Pnt.y ) ;
            break ;
        }
    }
}

void CDlgLevWord::SetLevWordList()
{
    m_LevWordLstList.ResetContent () ;
    for ( int i = 0; i < g_objHero.m_objBulletin.GetListAmount (); i++ )
    {
        BulletinTitleInfo* pBulletInfo = g_objHero.m_objBulletin.GetListLine ( i ) ;
        if ( pBulletInfo )
        {
            char strTitle[128] ;
            sprintf ( strTitle, "%s  %s  %s", pBulletInfo->szTitle, pBulletInfo->szName, pBulletInfo->szTime ) ;
            m_LevWordLstList.AddString ( strTitle ) ;
        }
    }
}

void CDlgLevWord::SetLevWordNote()
{
    if ( 2 == m_btShowMode)
    {
        m_LevWordEdtData.SetWindowText ( g_objHero.m_objBulletin.GetNote () ) ;
    }
}

void CDlgLevWord::OnLevwordBtnSend()
{
    g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10362) ) ;
    m_btShowMode = 1 ;
    m_LevWordLstList.EnableWindow ( false ) ;
    m_LevWordEdtData.EnableWindow ( true ) ;
    m_BtnOk.EnableWindow ( true ) ;
    m_BtnCancel.EnableWindow ( true ) ;
    m_BtnCancel.ChangeImage ( "Button7" ) ;
    m_LevWordEdtData.SetFocus () ;
}

void CDlgLevWord::OnLevwordBtnWatch()
{
    int nSel = m_LevWordLstList.GetCurSel () ;
    if ( nSel != -1 )
    {
        m_btShowMode = 2 ;
        m_LevWordLstList.EnableWindow ( false ) ;
        m_LevWordEdtData.EnableWindow ( false ) ;
        m_BtnOk.EnableWindow ( false ) ;
        m_BtnCancel.EnableWindow ( true ) ;
        m_BtnCancel.ChangeImage ( "Button2" ) ;
        BulletinTitleInfo* pBulletInfo = g_objHero.m_objBulletin.GetListLine ( nSel ) ;
        if ( pBulletInfo != NULL )
        {
            g_objHero.m_objBulletin.QueryNote ( pBulletInfo->szName, m_usCurChannel ) ;
            m_LevWordEdtData.SetWindowText ( g_objHero.m_objBulletin.GetNote () ) ;
        }
    }
    else
    {
        g_objGameMsg.AddMsg ( ( char* ) g_objGameDataSet.GetStr ( 10363) ) ;
        m_btShowMode = 0 ;
    }
}

void CDlgLevWord::OnLevwordBtnDel()
{
    g_objHero.m_objBulletin.DelBulletin ( m_usCurChannel ) ;
}

void CDlgLevWord::OnLevwordBtnUp()
{
    if ( m_nCurIndex > 0 )
    {
        m_nCurIndex -= 8 ;
        g_objHero.m_objBulletin.QueryList ( m_nCurIndex, m_usCurChannel ) ;
    }
}

void CDlgLevWord::OnLevwordBtnDown()
{
    m_nCurIndex += 8 ;
    g_objHero.m_objBulletin.QueryList ( m_nCurIndex, m_usCurChannel ) ;
}

void CDlgLevWord::OnLevwordBtnFlash()
{
    m_nCurIndex = 0 ;
    g_objHero.m_objBulletin.QueryList ( m_nCurIndex, m_usCurChannel ) ;
}

void CDlgLevWord::OnBtnOk()
{
    if ( m_btShowMode == 1 )
    {
        if ( m_LevWordEdtData.GetWindowTextLength () > 0 )
        {
            char strText[256] ;
            m_LevWordEdtData.GetWindowText ( strText, 256 ) ;
            g_objHero.m_objBulletin.SendBulletin ( strText, m_usCurChannel ) ;
            m_LevWordEdtData.SetWindowText ( NULL ) ;
            CDlgLevWord::OnBtnCancel () ;
        }
    }
    else if ( m_btShowMode == 2 )
    {
        CDlgLevWord::OnBtnCancel () ;
    }
}

void CDlgLevWord::OnBtnCancel()
{
    m_btShowMode = 0 ;
    m_LevWordLstList.EnableWindow ( true ) ;
    m_LevWordEdtData.EnableWindow ( false ) ;
    m_BtnOk.EnableWindow ( false ) ;
    m_BtnCancel.EnableWindow ( false ) ;
    m_LevWordEdtData.SetWindowText ( NULL ) ;
    m_LevWordLstList.ResetContent () ;
    g_objHero.m_objBulletin.QueryList ( m_nCurIndex, m_usCurChannel ) ;
}


void CDlgLevWord::OnLevwordBtnTrade()
{
    m_LevWordBtnTrade.SetCurFrame ( 2 ) ;
    if ( m_usCurChannel != _TXTATR_MSG_TRADE )
    {
        CDlgLevWord::ResetFrame( m_usCurChannel ) ;
    }
    m_usCurChannel = _TXTATR_MSG_TRADE ;
    CDlgLevWord::OnBtnCancel () ;
}

void CDlgLevWord::OnLevwordBtnFriend()
{
    m_LevWordBtnFriend.SetCurFrame ( 2 ) ;
    if ( m_usCurChannel != _TXTATR_MSG_FRIEND )
    {
        CDlgLevWord::ResetFrame( m_usCurChannel ) ;
    }
    m_usCurChannel = _TXTATR_MSG_FRIEND ;
    CDlgLevWord::OnBtnCancel () ;
}

void CDlgLevWord::OnLevwordBtnGroup()
{
    m_LevWordBtnGroup.SetCurFrame ( 2 ) ;
    if ( m_usCurChannel != _TXTATR_MSG_TEAM )
    {
        CDlgLevWord::ResetFrame( m_usCurChannel ) ;
    }
    m_usCurChannel = _TXTATR_MSG_TEAM ;
    CDlgLevWord::OnBtnCancel () ;
}

void CDlgLevWord::OnLevwordBtnFaction()
{
    m_LevWordBtnFaction.SetCurFrame ( 2 ) ;
    if ( m_usCurChannel != _TXTATR_MSG_SYN )
    {
        CDlgLevWord::ResetFrame( m_usCurChannel ) ;
    }
    m_usCurChannel = _TXTATR_MSG_SYN ;
    CDlgLevWord::OnBtnCancel () ;
}

void CDlgLevWord::OnLevwordBtnOther()
{
    m_LevWordBtnOther.SetCurFrame ( 2 ) ;
    if ( m_usCurChannel != _TXTATR_MSG_OTHER )
    {
        CDlgLevWord::ResetFrame( m_usCurChannel ) ;
    }
    m_usCurChannel = _TXTATR_MSG_OTHER ;
    CDlgLevWord::OnBtnCancel () ;
}

void CDlgLevWord::OnLevwordCloseb()
{
    CDlgLevWord::ShowWindow ( SW_HIDE ) ;
}

void CDlgLevWord::OnLevwordBtnCloses()
{
    CDlgLevWord::ShowWindow ( SW_HIDE ) ;
}

void CDlgLevWord::OnBtnHelp()
{
    CDlgLevWord::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_LEVWORD ) ;
}

void CDlgLevWord::ResetFrame(unsigned short usCurChannel)
{
    switch ( usCurChannel )
    {
    case _TXTATR_MSG_TRADE:
        m_LevWordBtnTrade.SetCurFrame ( 0 ) ;
        break ;
    case _TXTATR_MSG_FRIEND:
        m_LevWordBtnFriend.SetCurFrame ( 0 ) ;
        break ;
    case _TXTATR_MSG_TEAM:
        m_LevWordBtnGroup.SetCurFrame ( 0 ) ;
        break ;
    case _TXTATR_MSG_SYN:
        m_LevWordBtnFaction.SetCurFrame ( 0 ) ;
        break ;
    case _TXTATR_MSG_OTHER:
        m_LevWordBtnOther.SetCurFrame ( 0 ) ;
        break ;
    }
}
