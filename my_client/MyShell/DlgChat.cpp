
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgAct.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgChat.h"

#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChat dialog


CDlgChat::CDlgChat(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgChat::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgChat)
    m_bShow = false ;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_usCurChannel = 0 ;
    m_dwCurColor = 0 ;
    m_bColorList = false ;
    m_nColorLog[0] = 7 ;
    m_nColorLog[1] = 6 ;
    m_nColorLog[2] = 6 ;
    m_nColorLog[3] = 6 ;
    m_nColorLog[4] = 6 ;
    m_nColorLog[5] = 6 ;
    m_dwColor[0] = 0xDE6E2C ;
    m_dwColor[1] = 0xFFADAD ;
    m_dwColor[2] = 0xF62D1E ;
    m_dwColor[3] = 0x34EEFF ;
    m_dwColor[4] = 0x126AFC ;
    m_dwColor[5] = 0xADFFAD ;
    m_dwColor[6] = 0xF1FF00 ;
    m_dwColor[7] = 0xEAE9E9 ;
    //}}AFX_DATA_INIT
}


void CDlgChat::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgChat)
    DDX_Control(pDX, IDC_CHAT_IMG_CC1, m_ChatImgCc1);
    DDX_Control(pDX, IDC_CHAT_IMG_CC2, m_ChatImgCc2);
    DDX_Control(pDX, IDC_CHAT_IMG_CC3, m_ChatImgCc3);
    DDX_Control(pDX, IDC_CHAT_IMG_CC4, m_ChatImgCc4);
    DDX_Control(pDX, IDC_CHAT_IMG_CC5, m_ChatImgCc5);
    DDX_Control(pDX, IDC_CHAT_IMG_CC6, m_ChatImgCc6);
    DDX_Control(pDX, IDC_CHAT_IMG_CC7, m_ChatImgCc7);
    DDX_Control(pDX, IDC_CHAT_IMG_CC8, m_ChatImgCc8);
    DDX_Control(pDX, IDC_CHAT_IMG_BC, m_ChatImgBc);
    DDX_Control(pDX, IDC_CHAT_IMG_DC, m_ChatImgDc);
    DDX_Control(pDX, IDC_CHAT_IMG_GC, m_ChatImgGc);
    DDX_Control(pDX, IDC_CHAT_IMG_SC, m_ChatImgSc);
    DDX_Control(pDX, IDC_CHAT_IMG_HC, m_ChatImgHc);
    DDX_Control(pDX, IDC_CHAT_IMG_ZC, m_ChatImgZc);
    DDX_Control(pDX, IDC_CHAT_CHK_SF, m_ChatChkSF);
    DDX_Control(pDX, IDC_CHAT_CHK_S, m_ChatChkS);
    DDX_Control(pDX, IDC_CHAT_BTN_CLOSES, m_ChatBtnCloseS);
    DDX_Control(pDX, IDC_CHAT_CLOSEB, m_ChatBtnCloseB);
    DDX_Control(pDX, IDC_CHAT_CHK_H, m_ChatCHkH);
    DDX_Control(pDX, IDC_CHAT_CHK_G, m_ChatCHkG);
    DDX_Control(pDX, IDC_CHAT_CHK_D, m_ChatCHkD);
    DDX_Control(pDX, IDC_CHAT_CHK_B, m_ChatChkB);
    DDX_Control(pDX, IDC_CHAT_CHK_Z, m_ChatChkZ);
    DDX_Control(pDX, IDC_CHAT_BTN_SIFT, m_ChatBtnSift);
    DDX_Control(pDX, IDC_CHAT_BTN_RECORD, m_ChatBtnRecord);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChat, CDialog)
    //{{AFX_MSG_MAP(CDlgChat)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_CHAT_BTN_CLOSES, OnChatBtnCloses)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_CHAT_CLOSEB, OnChatCloseb)
    ON_BN_CLICKED(IDC_CHAT_BTN_RECORD, OnChatBtnRecord)
    ON_BN_CLICKED(IDC_CHAT_BTN_SIFT, OnChatBtnSift)
    ON_BN_CLICKED(IDC_CHAT_CHK_G, OnChatChkG)
    ON_BN_CLICKED(IDC_CHAT_CHK_B, OnChatChkB)
    ON_BN_CLICKED(IDC_CHAT_CHK_D, OnChatChkD)
    ON_BN_CLICKED(IDC_CHAT_CHK_H, OnChatChkH)
    ON_BN_CLICKED(IDC_CHAT_CHK_S, OnChatChkS)
    ON_BN_CLICKED(IDC_CHAT_CHK_Z, OnChatChkZ)
    ON_BN_CLICKED(IDC_CHAT_CHK_SF, OnChatChkSf)
    ON_BN_CLICKED(IDC_CHAT_IMG_GC, OnChatImgGc)
    ON_BN_CLICKED(IDC_CHAT_IMG_DC, OnChatImgDc)
    ON_BN_CLICKED(IDC_CHAT_IMG_SC, OnChatImgSc)
    ON_BN_CLICKED(IDC_CHAT_IMG_BC, OnChatImgBc)
    ON_BN_CLICKED(IDC_CHAT_IMG_HC, OnChatImgHc)
    ON_BN_CLICKED(IDC_CHAT_IMG_ZC, OnChatImgZc)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC1, OnChatImgCc1)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC2, OnChatImgCc2)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC3, OnChatImgCc3)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC4, OnChatImgCc4)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC5, OnChatImgCc5)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC6, OnChatImgCc6)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC7, OnChatImgCc7)
    ON_BN_CLICKED(IDC_CHAT_IMG_CC8, OnChatImgCc8)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChat message handlers

LRESULT CDlgChat::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_SHOWWINDOW )
    {
        m_bShow = wParam ;
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

HBRUSH CDlgChat::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgChat::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgChat::GetWindowRect ( rect ) ;
    // Init the button
    m_ChatBtnCloseS.Init ( rect.left, rect.top, "Button5" ) ;
    m_ChatBtnCloseB.Init ( rect.left, rect.top, "Button3" ) ;
    m_ChatBtnSift.Init ( rect.left, rect.top, "Button361" ) ;
    m_ChatBtnRecord.Init ( rect.left, rect.top, "Button360" ) ;
    m_BtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    // Init the check
    m_ChatCHkG.Init ( rect.left, rect.top, "Check360", 2 ) ;
    m_ChatChkB.Init ( rect.left, rect.top, "Check361", 2 ) ;
    m_ChatCHkD.Init ( rect.left, rect.top, "Check362", 2 ) ;
    m_ChatCHkH.Init ( rect.left, rect.top, "Check363", 2 ) ;
    m_ChatChkS.Init ( rect.left, rect.top, "Check364", 2 ) ;
    m_ChatChkSF.Init ( rect.left, rect.top, "Check365", 2 ) ;
    m_ChatChkZ.Init ( rect.left, rect.top, "Check366", 2 ) ;
    m_ChatCHkG.SetCheckValue ( 1 ) ;
    m_ChatChkB.SetCheckValue ( 1 ) ;
    m_ChatCHkD.SetCheckValue ( 1 ) ;
    m_ChatCHkH.SetCheckValue ( 1 ) ;
    m_ChatChkS.SetCheckValue ( 1 ) ;
    m_ChatChkZ.SetCheckValue ( 1 ) ;
    // Init the image
    m_ChatImgBc.Init ( rect.left, rect.top, "Image366" ) ;
    m_ChatImgDc.Init ( rect.left, rect.top, "Image366" ) ;
    m_ChatImgGc.Init ( rect.left, rect.top, "Image367" ) ;
    m_ChatImgSc.Init ( rect.left, rect.top, "Image366" ) ;
    m_ChatImgHc.Init ( rect.left, rect.top, "Image366" ) ;
    m_ChatImgZc.Init ( rect.left, rect.top, "Image366" ) ;
    m_ChatImgCc1.Init ( rect.left, rect.top, "Image360" ) ;
    m_ChatImgCc2.Init ( rect.left, rect.top, "Image361" ) ;
    m_ChatImgCc3.Init ( rect.left, rect.top, "Image362" ) ;
    m_ChatImgCc4.Init ( rect.left, rect.top, "Image363" ) ;
    m_ChatImgCc5.Init ( rect.left, rect.top, "Image364" ) ;
    m_ChatImgCc6.Init ( rect.left, rect.top, "Image365" ) ;
    m_ChatImgCc7.Init ( rect.left, rect.top, "Image366" ) ;
    m_ChatImgCc8.Init ( rect.left, rect.top, "Image367" ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgChat::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgChat::Show()
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
                                                "Image36",
                                                EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x + 90,
                            m_Pnt.y + 6 ) ;
        else
        {
            return ;
        }
        // Show the buttons
        m_ChatBtnCloseS.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatBtnCloseB.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatBtnSift.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatBtnRecord.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the check
        m_ChatCHkH.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatCHkG.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatCHkD.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatChkB.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatChkS.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatChkZ.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatChkSF.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the image
        m_ChatImgBc.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatImgDc.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatImgGc.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatImgSc.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatImgHc.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_ChatImgZc.Show ( m_Pnt.x, m_Pnt.y ) ;
        if ( m_bColorList )
        {
            m_ChatImgCc1.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ChatImgCc2.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ChatImgCc3.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ChatImgCc4.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ChatImgCc5.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ChatImgCc6.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ChatImgCc7.Show ( m_Pnt.x, m_Pnt.y ) ;
            m_ChatImgCc8.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
    }
}

void CDlgChat::OnChatBtnCloses()
{
    CDlgChat::ShowWindow ( SW_HIDE ) ;
}

void CDlgChat::OnBtnHelp()
{
    CDlgChat::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_CHAT ) ;
}

void CDlgChat::OnChatCloseb()
{
    CDlgChat::ShowWindow ( SW_HIDE ) ;
}

void CDlgChat::OnChatBtnRecord()
{
    CDlgChat::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TALK_LOG ) ;
}

void CDlgChat::OnChatBtnSift()
{
    CDlgChat::ShowWindow ( SW_HIDE ) ;
    CDlgChat::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_CHAT_SIFT ) ;
}

void CDlgChat::OnChatChkG()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ChatCHkG.GetCheckValue () )
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_TALK, true ) ;
    }
    else
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_TALK, false ) ;
    }
}

void CDlgChat::OnChatChkB()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ChatChkB.GetCheckValue () )
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_SYNDICATE, true ) ;
    }
    else
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_SYNDICATE, false ) ;
    }
}

void CDlgChat::OnChatChkD()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ChatCHkD.GetCheckValue () )
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_TEAM, true ) ;
    }
    else
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_TEAM, false ) ;
    }
}

void CDlgChat::OnChatChkH()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ChatCHkH.GetCheckValue () )
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_FRIEND, true ) ;
    }
    else
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_FRIEND, false ) ;
    }
}

void CDlgChat::OnChatChkS()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ChatChkS.GetCheckValue () )
    {
        g_objGameMsg.SetPrivateChannelStatusExceptFriend(true);
        g_objGameMsg.SetPrivateChannelStatusFriendOnly(true);
    }
    else
    {
        g_objGameMsg.SetPrivateChannelStatusExceptFriend(false);
        g_objGameMsg.SetPrivateChannelStatusFriendOnly(false);
    }
}

void CDlgChat::OnChatChkZ()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ChatChkZ.GetCheckValue () )
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_ACTION, true ) ;
    }
    else
    {
        g_objGameMsg.SetChannelStatus ( _TXTATR_ACTION, false ) ;
    }
}

void CDlgChat::OnChatChkSf()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_ChatChkSF.GetCheckValue () )
    {
        g_objGameMsg.SetPrivateChannelStatusExceptFriend(false);
        g_objGameMsg.SetPrivateChannelStatusFriendOnly(true);
    }
    else
    {
        g_objGameMsg.SetPrivateChannelStatusExceptFriend(true);
        g_objGameMsg.SetPrivateChannelStatusFriendOnly(true);
    }
}

void CDlgChat::OnChatImgGc()
{
    m_bColorList = !m_bColorList ;
    if ( m_bColorList )
    {
        m_usCurChannel = _TXTATR_TALK ;
    }
}

void CDlgChat::OnChatImgDc()
{
    m_bColorList = !m_bColorList ;
    if ( m_bColorList )
    {
        m_usCurChannel = _TXTATR_TEAM ;
    }
}

void CDlgChat::OnChatImgSc()
{
    m_bColorList = !m_bColorList ;
    if ( m_bColorList )
    {
        m_usCurChannel = _TXTATR_PRIVATE ;
    }
}

void CDlgChat::OnChatImgBc()
{
    m_bColorList = !m_bColorList ;
    if ( m_bColorList )
    {
        m_usCurChannel = _TXTATR_SYNDICATE ;
    }
}

void CDlgChat::OnChatImgHc()
{
    m_bColorList = !m_bColorList ;
    if ( m_bColorList )
    {
        m_usCurChannel = _TXTATR_FRIEND ;
    }
}

void CDlgChat::OnChatImgZc()
{
    m_bColorList = !m_bColorList ;
    if ( m_bColorList )
    {
        m_usCurChannel = _TXTATR_ACTION ;
    }
}

void CDlgChat::OnChatImgCc1()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[0] ) ;
        CDlgChat::ChangeColor ( 360 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::OnChatImgCc2()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[1] ) ;
        CDlgChat::ChangeColor ( 361 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::OnChatImgCc3()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[2] ) ;
        CDlgChat::ChangeColor ( 362 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::OnChatImgCc4()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[3] ) ;
        CDlgChat::ChangeColor ( 363 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::OnChatImgCc5()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[4] ) ;
        CDlgChat::ChangeColor ( 364 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::OnChatImgCc6()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[5] ) ;
        CDlgChat::ChangeColor ( 365 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::OnChatImgCc7()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[6] ) ;
        CDlgChat::ChangeColor ( 366 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::OnChatImgCc8()
{
    if ( m_bColorList )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[7] ) ;
        CDlgChat::ChangeColor ( 367 ) ;
        m_bColorList = false ;
    }
}

void CDlgChat::ChangeColor(int nColorIndex)
{
    char strImg[16] ;
    sprintf ( strImg, "Image%d", nColorIndex ) ;
    switch ( m_usCurChannel )
    {
    case _TXTATR_TALK:
        m_ChatImgGc.InsertImage ( strImg ) ;
        m_nColorLog[0] = nColorIndex % 10 ;
        break ;
    case _TXTATR_FRIEND:
        m_ChatImgHc.InsertImage ( strImg ) ;
        m_nColorLog[1] = nColorIndex % 10 ;
        break ;
    case _TXTATR_SYNDICATE:
        m_ChatImgBc.InsertImage ( strImg ) ;
        m_nColorLog[2] = nColorIndex % 10 ;
        break ;
    case _TXTATR_TEAM:
        m_ChatImgDc.InsertImage ( strImg ) ;
        m_nColorLog[3] = nColorIndex % 10 ;
        break ;
    case _TXTATR_PRIVATE:
        m_ChatImgSc.InsertImage ( strImg ) ;
        m_nColorLog[4] = nColorIndex % 10 ;
        break ;
    case _TXTATR_ACTION:
        m_ChatImgZc.InsertImage ( strImg ) ;
        m_nColorLog[5] = nColorIndex % 10 ;
        break ;
    }
}
