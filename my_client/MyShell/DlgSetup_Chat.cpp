
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgSetup_Chat.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgSetup_Chat.h"

#include "Gamemsg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Chat dialog


CDlgSetup_Chat::CDlgSetup_Chat(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSetup_Chat::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetup_Chat)
    m_bShow = false;
    m_Pnt = CPoint ( 0, 0 );
    m_usCurChannel = 0 ;
    m_bColorImg = false;
    m_nChatSetMode = 0;
    m_nCurBlackName = 0;
    //}}AFX_DATA_INIT
    m_nColorLog[0] = 3;
    m_nColorLog[1] = 3;
    m_nColorLog[2] = 3;
    m_nColorLog[3] = 3;
    m_nColorLog[4] = 3;
    m_dwColor[0] = 0xff8000 ;
    m_dwColor[1] = 0xFF0000 ;
    m_dwColor[2] = 0x008080 ;
    m_dwColor[3] = 0xffff00 ;
    m_dwColor[4] = 0x808000 ;
    m_dwColor[5] = 0xff0080 ;
    m_dwColor[6] = 0x00ff00 ;
    m_dwColor[7] = 0x8000ff ;
    m_dwColor[8] = 0x0080ff ;
    m_dwColor[9] = 0xffffff ;
}


void CDlgSetup_Chat::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetup_Chat)
    DDX_Control(pDX, IDC_CHK_NAME6, m_ChkName6);
    DDX_Control(pDX, IDC_CHK_NAME5, m_ChkName5);
    DDX_Control(pDX, IDC_CHK_NAME4, m_ChkName4);
    DDX_Control(pDX, IDC_CHK_NAME3, m_ChkName3);
    DDX_Control(pDX, IDC_CHK_NAME2, m_ChkName2);
    DDX_Control(pDX, IDC_CHK_NAME1, m_ChkName1);
    DDX_Control(pDX, IDC_EDIT_BLACKNAME, m_EditBlackName);
    DDX_Control(pDX, IDC_BTN_DELBLACKNAME, m_BtnDelBlackName);
    DDX_Control(pDX, IDC_BTN_CHATLOG, m_BtnChatLog);
    DDX_Control(pDX, IDC_BTN_BLACKNAME, m_BtnBlackName);
    DDX_Control(pDX, IDC_BTN_ADDBLACKNAME, m_BtnAddBlackName);
    DDX_Control(pDX, IDC_CHK_TEAM, m_ChkTeam);
    DDX_Control(pDX, IDC_CHK_SEC, m_ChkSec);
    DDX_Control(pDX, IDC_CHK_PUB, m_ChkPub);
    DDX_Control(pDX, IDC_CHK_FRIEND, m_ChkFriend);
    DDX_Control(pDX, IDC_CHK_BAN, m_ChkBan);
    DDX_Control(pDX, IDC_BTN_CHANNELSET, m_BtnChannelSet);
    DDX_Control(pDX, IDC_BTN_COLORSET, m_BtnColorSet);
    DDX_Control(pDX, IDC_BTN_COLOR1, m_ImgColor1);
    DDX_Control(pDX, IDC_BTN_COLOR2, m_ImgColor2);
    DDX_Control(pDX, IDC_BTN_COLOR3, m_ImgColor3);
    DDX_Control(pDX, IDC_BTN_COLOR4, m_ImgColor4);
    DDX_Control(pDX, IDC_BTN_COLOR5, m_ImgColor5);
    DDX_Control(pDX, IDC_BTN_COLOR6, m_ImgColor6);
    DDX_Control(pDX, IDC_BTN_COLOR7, m_ImgColor7);
    DDX_Control(pDX, IDC_BTN_COLOR8, m_ImgColor8);
    DDX_Control(pDX, IDC_BTN_COLOR9, m_ImgColor9);
    DDX_Control(pDX, IDC_BTN_COLOR10, m_ImgColor10);
    DDX_Control(pDX, IDC_BTN_FRIEND, m_ImgFriend);
    DDX_Control(pDX, IDC_BTN_PUB, m_ImgPub);
    DDX_Control(pDX, IDC_BTN_SEC, m_ImgSec);
    DDX_Control(pDX, IDC_BTN_TEAM, m_ImgTeam);
    DDX_Control(pDX, IDC_BTN_BAN, m_ImgBan);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup_Chat, CDialog)
    //{{AFX_MSG_MAP(CDlgSetup_Chat)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_BAN, OnBtnBan)
    ON_BN_CLICKED(IDC_BTN_COLOR1, OnBtnColor1)
    ON_BN_CLICKED(IDC_BTN_COLOR10, OnBtnColor10)
    ON_BN_CLICKED(IDC_BTN_COLOR2, OnBtnColor2)
    ON_BN_CLICKED(IDC_BTN_COLOR3, OnBtnColor3)
    ON_BN_CLICKED(IDC_BTN_COLOR4, OnBtnColor4)
    ON_BN_CLICKED(IDC_BTN_COLOR5, OnBtnColor5)
    ON_BN_CLICKED(IDC_BTN_COLOR6, OnBtnColor6)
    ON_BN_CLICKED(IDC_BTN_COLOR7, OnBtnColor7)
    ON_BN_CLICKED(IDC_BTN_COLOR8, OnBtnColor8)
    ON_BN_CLICKED(IDC_BTN_COLOR9, OnBtnColor9)
    ON_BN_CLICKED(IDC_BTN_FRIEND, OnBtnFriend)
    ON_BN_CLICKED(IDC_BTN_PUB, OnBtnPub)
    ON_BN_CLICKED(IDC_BTN_SEC, OnBtnSec)
    ON_BN_CLICKED(IDC_BTN_TEAM, OnBtnTeam)
    ON_BN_CLICKED(IDC_BTN_CHANNELSET, OnBtnChannelset)
    ON_BN_CLICKED(IDC_BTN_COLORSET, OnBtnColorset)
    ON_BN_CLICKED(IDC_CHK_BAN, OnChkBan)
    ON_BN_CLICKED(IDC_CHK_FRIEND, OnChkFriend)
    ON_BN_CLICKED(IDC_CHK_PUB, OnChkPub)
    ON_BN_CLICKED(IDC_CHK_SEC, OnChkSec)
    ON_BN_CLICKED(IDC_CHK_TEAM, OnChkTeam)
    ON_BN_CLICKED(IDC_BTN_ADDBLACKNAME, OnBtnAddblackname)
    ON_BN_CLICKED(IDC_BTN_BLACKNAME, OnBtnBlackname)
    ON_BN_CLICKED(IDC_BTN_CHATLOG, OnBtnChatlog)
    ON_BN_CLICKED(IDC_BTN_DELBLACKNAME, OnBtnDelblackname)
    ON_BN_CLICKED(IDC_CHK_NAME1, OnChkName1)
    ON_BN_CLICKED(IDC_CHK_NAME2, OnChkName2)
    ON_BN_CLICKED(IDC_CHK_NAME3, OnChkName3)
    ON_BN_CLICKED(IDC_CHK_NAME4, OnChkName4)
    ON_BN_CLICKED(IDC_CHK_NAME5, OnChkName5)
    ON_BN_CLICKED(IDC_CHK_NAME6, OnChkName6)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Chat message handlers

LRESULT CDlgSetup_Chat::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (m_bShow)
        {
            CDlgSetup_Chat::SetChatMode();
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

HBRUSH CDlgSetup_Chat::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Change any attributes of the DC here
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush ;
}

BOOL CDlgSetup_Chat::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgSetup_Chat::GetWindowRect(rect);
    m_ImgColor1.Init(rect.left, rect.top, "Image360");
    m_ImgColor2.Init(rect.left, rect.top, "Image361");
    m_ImgColor3.Init(rect.left, rect.top, "Image362");
    m_ImgColor4.Init(rect.left, rect.top, "Image363");
    m_ImgColor5.Init(rect.left, rect.top, "Image364");
    m_ImgColor6.Init(rect.left, rect.top, "Image365");
    m_ImgColor7.Init(rect.left, rect.top, "Image366");
    m_ImgColor8.Init(rect.left, rect.top, "Image367");
    m_ImgColor9.Init(rect.left, rect.top, "Image368");
    m_ImgColor10.Init(rect.left, rect.top, "Image369");
    EnableColorImg(false);
    m_ImgPub.Init(rect.left, rect.top);
    m_ImgFriend.Init(rect.left, rect.top);
    m_ImgSec.Init(rect.left, rect.top);
    m_ImgTeam.Init(rect.left, rect.top);
    m_ImgBan.Init(rect.left, rect.top);
    m_ChkPub.Init(rect.left, rect.top, "Check190", 2);
    m_ChkFriend.Init(rect.left, rect.top, "Check190", 2);
    m_ChkSec.Init(rect.left, rect.top, "Check190", 2);
    m_ChkTeam.Init(rect.left, rect.top, "Check190", 2);
    m_ChkBan.Init(rect.left, rect.top, "Check190", 2);
    m_ChkPub.SetCheckValue(1);
    m_ChkFriend.SetCheckValue(1);
    m_ChkSec.SetCheckValue(1);
    m_ChkTeam.SetCheckValue(1);
    m_ChkBan.SetCheckValue(1);
    m_ChkName1.Init(rect.left, rect.top, "", 2);
    m_ChkName1.SetStaticMode();
    m_ChkName2.Init(rect.left, rect.top, "", 2);
    m_ChkName2.SetStaticMode();
    m_ChkName3.Init(rect.left, rect.top, "", 2);
    m_ChkName3.SetStaticMode();
    m_ChkName4.Init(rect.left, rect.top, "", 2);
    m_ChkName4.SetStaticMode();
    m_ChkName5.Init(rect.left, rect.top, "", 2);
    m_ChkName5.SetStaticMode();
    m_ChkName6.Init(rect.left, rect.top, "", 2);
    m_ChkName6.SetStaticMode();
    m_BtnChannelSet.Init(rect.left, rect.top, "Button451", 2);
    m_BtnBlackName.Init(rect.left, rect.top, "Button452", 2);
    m_BtnDelBlackName.Init(rect.left, rect.top, "Button454");
    m_BtnAddBlackName.Init(rect.left, rect.top, "Button453");
    m_BtnChatLog.Init(rect.left, rect.top, "Button455");
    m_BtnColorSet.Init(rect.left, rect.top, "Button450", 2);
    m_BtnColorSet.SetCheckValue(1);
    m_EditBlackName.Init(rect.left, rect.top);
    m_EditBlackName.SetLimitText(15);
    m_EditBlackName.EnableWindow(false);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetup_Chat::Show(int x, int y)
{
    if (m_bShow)
    {
        if (m_nChatSetMode == 0)
        {
            if (m_bColorImg)
            {
                CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                "Dialog10",
                                EXIGENCE_IMMEDIATE ) ;
                if (ShowAni != NULL)
                {
                    ShowAni->Show ( 4,
                                    m_Pnt.x + x + 197,
                                    m_Pnt.y + y + 148) ;
                }
                m_ImgColor1.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor2.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor3.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor4.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor5.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor6.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor7.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor8.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor9.Show(m_Pnt.x + x, m_Pnt.y + y);
                m_ImgColor10.Show(m_Pnt.x + x, m_Pnt.y + y);
            }
            m_ImgPub.Show(m_Pnt.x + x, m_Pnt.y + y);
            m_ImgFriend.Show(m_Pnt.x + x, m_Pnt.y + y);
            m_ImgTeam.Show(m_Pnt.x + x, m_Pnt.y + y);
            m_ImgSec.Show(m_Pnt.x + x, m_Pnt.y + y);
            m_ImgBan.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        else if (m_nChatSetMode == 1)
        {
            m_ChkPub.Show(m_Pnt.x + x, m_Pnt.y + y );
            m_ChkFriend.Show(m_Pnt.x + x, m_Pnt.y + y );
            m_ChkTeam.Show(m_Pnt.x + x, m_Pnt.y + y );
            m_ChkSec.Show(m_Pnt.x + x, m_Pnt.y + y );
            m_ChkBan.Show(m_Pnt.x + x, m_Pnt.y + y );
        }
        else if (m_nChatSetMode == 2)
        {
            if (m_ChkName1.IsWindowEnabled())
            {
                m_ChkName1.Show(m_Pnt.x + x, m_Pnt.y + y);
            }
            if (m_ChkName2.IsWindowEnabled())
            {
                m_ChkName2.Show(m_Pnt.x + x, m_Pnt.y + y);
            }
            if (m_ChkName3.IsWindowEnabled())
            {
                m_ChkName3.Show(m_Pnt.x + x, m_Pnt.y + y);
            }
            if (m_ChkName4.IsWindowEnabled())
            {
                m_ChkName4.Show(m_Pnt.x + x, m_Pnt.y + y);
            }
            if (m_ChkName5.IsWindowEnabled())
            {
                m_ChkName5.Show(m_Pnt.x + x, m_Pnt.y + y);
            }
            if (m_ChkName6.IsWindowEnabled())
            {
                m_ChkName6.Show(m_Pnt.x + x, m_Pnt.y + y);
            }
            m_BtnAddBlackName.Show(m_Pnt.x + x, m_Pnt.y + y);
            m_BtnDelBlackName.Show(m_Pnt.x + x, m_Pnt.y + y);
            m_BtnChatLog.Show(m_Pnt.x + x, m_Pnt.y + y);
            m_EditBlackName.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        m_BtnColorSet.Show(m_Pnt.x + x, m_Pnt.y + y );
        m_BtnBlackName.Show(m_Pnt.x + x, m_Pnt.y + y );
        m_BtnChannelSet.Show(m_Pnt.x + x, m_Pnt.y + y );
    }
}

void CDlgSetup_Chat::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgSetup_Chat::OnBtnBan()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (!m_bColorImg)
    {
        CDlgSetup_Chat::EnableColorImg(true);
        m_bColorImg = TRUE ;
    }
    m_usCurChannel = _TXTATR_SYNDICATE ;
}

void CDlgSetup_Chat::OnBtnColor1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[0] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 360 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor10()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[9] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 369 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[1] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 361 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[2] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 362 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor4()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[3] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 363 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor5()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[4] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 364 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor6()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[5] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 365 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor7()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[6] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 366 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor8()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[7] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 367 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnColor9()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if ( m_bColorImg )
    {
        g_objGameMsg.SetChannelColor ( m_usCurChannel, m_dwColor[8] ) ;
        CDlgSetup_Chat::ChangeColorImg ( 368 ) ;
        m_bColorImg = false ;
        CDlgSetup_Chat::EnableColorImg(false);
    }
}

void CDlgSetup_Chat::OnBtnFriend()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (!m_bColorImg)
    {
        CDlgSetup_Chat::EnableColorImg(true);
        m_bColorImg = TRUE ;
    }
    m_usCurChannel = _TXTATR_FRIEND ;
}

void CDlgSetup_Chat::OnBtnPub()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (!m_bColorImg)
    {
        CDlgSetup_Chat::EnableColorImg(true);
        m_bColorImg = TRUE ;
    }
    m_usCurChannel = _TXTATR_TALK ;
}

void CDlgSetup_Chat::OnBtnSec()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (!m_bColorImg)
    {
        CDlgSetup_Chat::EnableColorImg(true);
        m_bColorImg = TRUE ;
    }
    m_usCurChannel = _TXTATR_PRIVATE ;
}

void CDlgSetup_Chat::OnBtnTeam()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (!m_bColorImg)
    {
        CDlgSetup_Chat::EnableColorImg(true);
        m_bColorImg = TRUE ;
    }
    m_usCurChannel = _TXTATR_TEAM ;
}

void CDlgSetup_Chat::EnableColorImg(BOOL bFlag)
{
    m_ImgColor1.EnableWindow(bFlag);
    m_ImgColor2.EnableWindow(bFlag);
    m_ImgColor3.EnableWindow(bFlag);
    m_ImgColor4.EnableWindow(bFlag);
    m_ImgColor5.EnableWindow(bFlag);
    m_ImgColor6.EnableWindow(bFlag);
    m_ImgColor7.EnableWindow(bFlag);
    m_ImgColor8.EnableWindow(bFlag);
    m_ImgColor9.EnableWindow(bFlag);
    m_ImgColor10.EnableWindow(bFlag);
}

void CDlgSetup_Chat::ChangeColorImg(int nColorIndex)
{
    DXPlaySound("Sound/Set_ChatChgClo.wav");
    char strImg[16] ;
    sprintf ( strImg, "Image%d", nColorIndex ) ;
    switch ( m_usCurChannel )
    {
    case _TXTATR_TALK:
        m_ImgPub.InsertImage ( strImg ) ;
        m_nColorLog[0] = nColorIndex % 10 ;
        break ;
    case _TXTATR_FRIEND:
        m_ImgFriend.InsertImage ( strImg ) ;
        m_nColorLog[1] = nColorIndex % 10 ;
        break ;
    case _TXTATR_SYNDICATE:
        m_ImgBan.InsertImage ( strImg ) ;
        m_nColorLog[2] = nColorIndex % 10 ;
        break ;
    case _TXTATR_TEAM:
        m_ImgTeam.InsertImage ( strImg ) ;
        m_nColorLog[3] = nColorIndex % 10 ;
        break ;
    case _TXTATR_PRIVATE:
        m_ImgSec.InsertImage ( strImg ) ;
        m_nColorLog[4] = nColorIndex % 10 ;
        break ;
    }
}

void CDlgSetup_Chat::OnBtnChannelset()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_nChatSetMode != 1)
    {
        m_nChatSetMode = 1;
        CDlgSetup_Chat::SetChatMode();
    }
    else
    {
        m_BtnChannelSet.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::OnBtnColorset()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_nChatSetMode != 0)
    {
        m_nChatSetMode = 0;
        CDlgSetup_Chat::SetChatMode();
    }
    else
    {
        m_BtnColorSet.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::SetChatMode()
{
    m_ImgColor1.EnableWindow(false);
    m_ImgColor2.EnableWindow(false);
    m_ImgColor3.EnableWindow(false);
    m_ImgColor4.EnableWindow(false);
    m_ImgColor5.EnableWindow(false);
    m_ImgColor6.EnableWindow(false);
    m_ImgColor7.EnableWindow(false);
    m_ImgColor8.EnableWindow(false);
    m_ImgColor9.EnableWindow(false);
    m_ImgColor10.EnableWindow(false);
    m_ImgPub.EnableWindow(false);
    m_ImgSec.EnableWindow(false);
    m_ImgTeam.EnableWindow(false);
    m_ImgBan.EnableWindow(false);
    m_ImgFriend.EnableWindow(false);
    m_ChkPub.EnableWindow(false);
    m_ChkFriend.EnableWindow(false);
    m_ChkSec.EnableWindow(false);
    m_ChkBan.EnableWindow(false);
    m_ChkTeam.EnableWindow(false);
    m_BtnAddBlackName.EnableWindow(false);
    m_BtnDelBlackName.EnableWindow(false);
    m_BtnChatLog.EnableWindow(false);
    m_EditBlackName.EnableWindow(false);
    m_ChkName1.EnableWindow(false);
    m_ChkName2.EnableWindow(false);
    m_ChkName3.EnableWindow(false);
    m_ChkName4.EnableWindow(false);
    m_ChkName5.EnableWindow(false);
    m_ChkName6.EnableWindow(false);
    switch(m_nChatSetMode)
    {
    case 0:
        m_ImgColor1.EnableWindow();
        m_ImgColor2.EnableWindow();
        m_ImgColor3.EnableWindow();
        m_ImgColor4.EnableWindow();
        m_ImgColor5.EnableWindow();
        m_ImgColor6.EnableWindow();
        m_ImgColor7.EnableWindow();
        m_ImgColor8.EnableWindow();
        m_ImgColor9.EnableWindow();
        m_ImgColor10.EnableWindow();
        m_ImgPub.EnableWindow();
        m_ImgSec.EnableWindow();
        m_ImgTeam.EnableWindow();
        m_ImgBan.EnableWindow();
        m_ImgFriend.EnableWindow();
        m_BtnChannelSet.SetCheckValue(0);
        m_BtnBlackName.SetCheckValue(0);
        m_BtnColorSet.SetCheckValue(1);
        break;
    case 1:
        m_ChkPub.EnableWindow();
        m_ChkFriend.EnableWindow();
        m_ChkSec.EnableWindow();
        m_ChkBan.EnableWindow();
        m_ChkTeam.EnableWindow();
        m_BtnChannelSet.SetCheckValue(1);
        m_BtnBlackName.SetCheckValue(0);
        m_BtnColorSet.SetCheckValue(0);
        break;
    case 2:
        m_BtnAddBlackName.EnableWindow();
        m_BtnDelBlackName.EnableWindow();
        m_BtnChatLog.EnableWindow();
        m_EditBlackName.EnableWindow();
        m_ChkName1.EnableWindow();
        m_ChkName2.EnableWindow();
        m_ChkName3.EnableWindow();
        m_ChkName4.EnableWindow();
        m_ChkName5.EnableWindow();
        m_ChkName6.EnableWindow();
        m_BtnChannelSet.SetCheckValue(0);
        m_BtnBlackName.SetCheckValue(1);
        m_BtnColorSet.SetCheckValue(0);
        SetBlackNameStatic();
        break;
    default:
        break;
    }
}

void CDlgSetup_Chat::OnChkBan()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ChkBan.GetCheckValue())
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_SYNDICATE, true);
    }
    else
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_SYNDICATE, false);
    }
}

void CDlgSetup_Chat::OnChkFriend()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ChkFriend.GetCheckValue())
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_FRIEND, true);
    }
    else
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_FRIEND, false);
    }
}

void CDlgSetup_Chat::OnChkPub()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ChkPub.GetCheckValue())
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_TALK, true);
    }
    else
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_TALK, false);
    }
}

void CDlgSetup_Chat::OnChkSec()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ChkSec.GetCheckValue())
    {
        g_objGameMsg.SetPrivateChannelStatusExceptFriend(true);
    }
    else
    {
        g_objGameMsg.SetPrivateChannelStatusExceptFriend(false);
    }
}

void CDlgSetup_Chat::OnChkTeam()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ChkTeam.GetCheckValue())
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_TEAM, true);
    }
    else
    {
        g_objGameMsg.SetChannelStatus(_TXTATR_TEAM, false);
    }
}

void CDlgSetup_Chat::OnBtnAddblackname()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    char strName[_MAX_NAMESIZE];
    m_EditBlackName.GetWindowText(strName, _MAX_NAMESIZE);
    if (!StringCheck(strName))
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100500));
        m_EditBlackName.SetWindowText ( NULL ) ;
        return;
    }
    if (strlen(strName) > 0)
    {
        m_EditBlackName.SetWindowText("");
        g_objGameMsg.AddBlackName(strName);
        SetBlackNameStatic();
    }
}

void CDlgSetup_Chat::OnBtnBlackname()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_nChatSetMode != 2)
    {
        m_nChatSetMode = 2;
        CDlgSetup_Chat::SetChatMode();
    }
    else
    {
        m_BtnChannelSet.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::OnBtnChatlog()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgSetup_Chat::GetParent ()->GetParent()->PostMessage ( WM_MY_MESSAGE, ON_TALK_LOG ) ;
}

void CDlgSetup_Chat::OnBtnDelblackname()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_nCurBlackName > 0)
    {
        g_objGameMsg.DelBlackName(m_nCurBlackName - 1);
        SetBlackNameStatic();
    }
}

void CDlgSetup_Chat::SetBlackNameStatic()
{
    m_ChkName1.EnableWindow(false);
    m_ChkName1.SetCheckValue(0);
    m_ChkName2.EnableWindow(false);
    m_ChkName2.SetCheckValue(0);
    m_ChkName3.EnableWindow(false);
    m_ChkName3.SetCheckValue(0);
    m_ChkName4.EnableWindow(false);
    m_ChkName4.SetCheckValue(0);
    m_ChkName5.EnableWindow(false);
    m_ChkName5.SetCheckValue(0);
    m_ChkName6.EnableWindow(false);
    m_ChkName6.SetCheckValue(0);
    m_nCurBlackName = 0;
    int nAmount = g_objGameMsg.GetBlackNameAmount();
    for(int i = 0; i < nAmount; i++)
    {
        switch(i)
        {
        case 0:
            m_ChkName1.SetWindowText(g_objGameMsg.GetBlackNameByIndex(i));
            m_ChkName1.EnableWindow();
            break;
        case 1:
            m_ChkName2.SetWindowText(g_objGameMsg.GetBlackNameByIndex(i));
            m_ChkName2.EnableWindow();
            break;
        case 2:
            m_ChkName3.SetWindowText(g_objGameMsg.GetBlackNameByIndex(i));
            m_ChkName3.EnableWindow();
            break;
        case 3:
            m_ChkName4.SetWindowText(g_objGameMsg.GetBlackNameByIndex(i));
            m_ChkName4.EnableWindow();
            break;
        case 4:
            m_ChkName5.SetWindowText(g_objGameMsg.GetBlackNameByIndex(i));
            m_ChkName5.EnableWindow();
            break;
        case 5:
            m_ChkName6.SetWindowText(g_objGameMsg.GetBlackNameByIndex(i));
            m_ChkName6.EnableWindow();
            break;
        default:
            break;
        }
    }
}

void CDlgSetup_Chat::PopupCurNameChk()
{
    switch(m_nCurBlackName)
    {
    case 1:
        m_ChkName1.SetCheckValue(0);
        break;
    case 2:
        m_ChkName2.SetCheckValue(0);
        break;
    case 3:
        m_ChkName3.SetCheckValue(0);
        break;
    case 4:
        m_ChkName4.SetCheckValue(0);
        break;
    case 5:
        m_ChkName5.SetCheckValue(0);
        break;
    case 6:
        m_ChkName6.SetCheckValue(0);
        break;
    default:
        break;
    }
}

void CDlgSetup_Chat::OnChkName1()
{
    // TODO: Add your control notification handler code here
    if (m_ChkName1.GetCheckValue() == 1)
    {
        PopupCurNameChk();
        m_nCurBlackName = 1;
    }
    else
    {
        m_ChkName1.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::OnChkName2()
{
    // TODO: Add your control notification handler code here
    if (m_ChkName2.GetCheckValue() == 1)
    {
        PopupCurNameChk();
        m_nCurBlackName = 2;
    }
    else
    {
        m_ChkName2.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::OnChkName3()
{
    // TODO: Add your control notification handler code here
    if (m_ChkName3.GetCheckValue() == 1)
    {
        PopupCurNameChk();
        m_nCurBlackName = 3;
    }
    else
    {
        m_ChkName3.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::OnChkName4()
{
    // TODO: Add your control notification handler code here
    if (m_ChkName4.GetCheckValue() == 1)
    {
        PopupCurNameChk();
        m_nCurBlackName = 4;
    }
    else
    {
        m_ChkName4.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::OnChkName5()
{
    // TODO: Add your control notification handler code here
    if (m_ChkName5.GetCheckValue() == 1)
    {
        PopupCurNameChk();
        m_nCurBlackName = 5;
    }
    else
    {
        m_ChkName5.SetCheckValue(1);
    }
}

void CDlgSetup_Chat::OnChkName6()
{
    // TODO: Add your control notification handler code here
    if (m_ChkName6.GetCheckValue() == 1)
    {
        PopupCurNameChk();
        m_nCurBlackName = 6;
    }
    else
    {
        m_ChkName6.SetCheckValue(1);
    }
}
