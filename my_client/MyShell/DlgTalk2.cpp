
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// DlgTalk.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgTalk.h"

#include "3DRoleData.h"

#include "GameMsg.h"
#include "Hero.h"
#include "GamePlayerSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int	_FLASH_SPEED = 300;
/////////////////////////////////////////////////////////////////////////////
// CDlgTalk dialog


CDlgTalk::CDlgTalk(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTalk::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTalk)
    m_bShow = false ;
    m_bFocusNameEdt = false;
    m_Pnt = CPoint ( 0, 0 ) ;
    m_dwLastTime = 0 ;
    m_nListAmount = 0;
    m_nListIndex = 0 ;
    m_uTalkMode = G_CLICK ;
    m_uAddBtnMode = 0 ;
    m_uDelIndex = 0 ;
    m_nTargetAmount = 0 ;
    m_nCurTargetIndex = 0 ;
    m_bMouseMove = false ;
    m_bShowGoods = true;
    m_bGameMsgEx = false;
    m_bShowExp = false;
    m_dwFlashTime = 0;
    m_nTextActionIndex = 0;
    m_bEnter = false;
    m_bHotEnter = true;
    //}}AFX_DATA_INIT
    for(int i = 0; i < 10; i++)
    {
        m_uTargetTalkMode[i] = G_CLICK;
        strcpy(m_strTargetName[i], "");
    }
}


void CDlgTalk::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTalk)
    DDX_Control(pDX, IDC_STATIC_TXTACTION, m_StaTxtAction);
    DDX_Control(pDX, IDC_TALK_BTN_TARGETLIST, m_TargetListBtn);
    DDX_Control(pDX, IDC_TALK_BTN_EMOTIONLIST, m_EmotionListBtn);
    DDX_Control(pDX, IDC_TALK_BTN_VIEWEQUIP, m_ViewEquipBtn);
    DDX_Control(pDX, IDC_TALK_CHK_RUN, m_TalkActBtn);
    DDX_Control(pDX, IDC_TALK_MYCHAT, m_TalkMyChatBtn);
    DDX_Control(pDX, IDC_TALK_CLEARBTN, m_TalkClearBtn);
    DDX_Control(pDX, IDC_GRI_GOODBOX, m_GoodBox);
    DDX_Control(pDX, IDC_BTN_HIDEGOODS, m_HideGoodBox);
    DDX_Control(pDX, IDC_PRG_EXP, m_PrgExp);
    DDX_Control(pDX, IDC_TALK_COB_DATA, m_TalkDataCob);
    DDX_Control(pDX, IDC_TALK_EDT_NAME, m_TalkNameEdt);
    DDX_Control(pDX, IDC_TALK_IMG_MODE, m_TalkModeImg);
    DDX_Control(pDX, IDC_TALK_BTN_SEND, m_TalkSendBtn);
    DDX_Control(pDX, IDC_TALK_BTN_MIPMAP, m_TalkMipmapBtn);
    DDX_Control(pDX, IDC_TALK_BTN_HELP, m_TalkHelpBtn);
    DDX_Control(pDX, IDC_TALK_CHK_PK, m_TalkPkChk);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTalk, CDialog)
    //{{AFX_MSG_MAP(CDlgTalk)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_TALK_BTN_SEND, OnTalkBtnSend)
    ON_BN_CLICKED(IDC_TALK_IMG_MODE, OnTalkImgMode)
    ON_WM_LBUTTONDOWN()
    ON_EN_UPDATE(IDC_TALK_EDT_NAME, OnUpdateTalkEdtName)
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_TALK_CHK_PK, OnTalkChkPk)
    ON_BN_CLICKED(IDC_TALK_CHK_RUN, OnTalkChkRun)
    ON_BN_CLICKED(IDC_TALK_BTN_MIPMAP, OnTalkBtnMipmap)
    ON_BN_CLICKED(IDC_TALK_BTN_HELP, OnTalkBtnHelp)
    ON_BN_CLICKED(IDC_BTN_HIDEGOODS, OnBtnHidegoods)
    ON_BN_CLICKED(IDC_TALK_CLEARBTN, OnTalkClearbtn)
    ON_BN_CLICKED(IDC_TALK_MYCHAT, OnTalkMychat)
    ON_BN_CLICKED(IDC_TALK_BTN_VIEWEQUIP, OnTalkBtnViewequip)
    ON_BN_CLICKED(IDC_TALK_BTN_TARGETLIST, OnTalkBtnTargetlist)
    ON_BN_CLICKED(IDC_TALK_BTN_EMOTIONLIST, OnTalkBtnEmotionlist)
    ON_EN_SETFOCUS(IDC_TALK_EDT_DATA, OnSetfocusTalkEdtData)
    ON_EN_KILLFOCUS(IDC_TALK_EDT_DATA, OnKillfocusTalkEdtData)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTalk message handlers


LRESULT CDlgTalk::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        if ( wParam == false )
        {
            m_bShow = false ;
            if ( m_MenuTalk.m_bShow )
            {
                m_MenuTalk.EnableWindow( false ) ;
            }
        }
        else
        {
            m_bShow = true ;
            SetTalkDlgRgn();
            m_TalkDataCob.SetFocus () ;
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 2 )
        {
            return true ;
        }
        else if ( wParam == 1 )
        {
            return true ;
        }
        break ;
    case WM_VSCROLL:
        if ( wParam == SB_LINEUP )
        {
            if ( m_nListIndex > 0 )
            {
                m_nListIndex-- ;
                char strChar[1024] ;
                m_ListBox.GetText ( m_nListIndex, strChar ) ;
                m_TalkDataCob.SetWindowText ( strChar ) ;
                return true ;
            }
        }
        else if ( wParam == SB_LINEDOWN )
        {
            if ( m_nListIndex <= m_nListAmount - 1 )
            {
                char strChar[1024] ;
                m_ListBox.GetText ( m_nListIndex, strChar ) ;
                if ( 0 == m_nListIndex )
                {
                    char strTemp[1024] ;
                    m_TalkDataCob.GetWindowText ( strTemp, 1024 ) ;
                    if ( !strcmp ( strTemp, strChar ) )
                    {
                        m_nListIndex++ ;
                        m_ListBox.GetText ( m_nListIndex, strChar ) ;
                    }
                }
                m_TalkDataCob.SetWindowText ( strChar ) ;
                if ( m_nListIndex < m_nListAmount - 1  )
                {
                    m_nListIndex++ ;
                }
                return true ;
            }
        }
        break ;
    case WM_MY_MESSAGE:
        if ( wParam == MENU_TALK_CLICK )
        {
            CDlgTalk::SetTalkMode ( lParam ) ;
        }
        else if (wParam == DEL_LISTITEM)
        {
            m_uDelIndex = lParam;
            CDlgTalk::DeleteTalkTarget();
        }
        break ;
    case CB_GETITEMDATA:
        {
            if ( wParam )
            {
                char strTemp[16] ;
                if (m_DlgListBox.m_bShow)
                {
                    m_DlgListBox.m_ListBox.GetText ( lParam, strTemp ) ;
                    m_TalkNameEdt.SetWindowText ( strTemp ) ;
                    m_DlgListBox.EnableWindow(false) ;
                }
                else if (m_DlgListAction.m_bShow)
                {
                    m_DlgListAction.m_ListTxtAction.GetText ( lParam, strTemp ) ;
                    m_StaTxtAction.SetWindowText ( strTemp ) ;
                    m_nTextActionIndex = lParam;
                    m_DlgListAction.EnableWindow(false) ;
                }
                return true ;
            }
            else
            {
                char strTemp[16] ;
                if (m_DlgListBox.m_bShow)
                {
                    m_DlgListBox.m_ListBox.GetText ( lParam, strTemp ) ;
                    m_TalkNameEdt.SetWindowText ( strTemp ) ;
                    m_DlgListBox.EnableWindow(false) ;
                }
                else if (m_DlgListAction.m_bShow)
                {
                    m_DlgListAction.m_ListTxtAction.GetText(lParam, strTemp);
                    m_StaTxtAction.SetWindowText(strTemp);
                    m_nTextActionIndex = lParam;
                    m_DlgListAction.EnableWindow(false) ;
                }
                return true ;
            }
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTalk::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTalk::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgTalk::GetWindowRect ( rect ) ;
    m_GoodBox.Init ( rect.left, rect.top, 1, 10, DLG_MAIN );
    m_GoodBox.ReSetGridSpace(11, 1);
    // Create the talk data list box
    m_ListBox.Create ( WS_CHILD | WS_HSCROLL, CRect ( 0, 0, 0, 0 ), this, 0 ) ;
    // Init the combo
    m_TalkDataCob.Init ( rect.left, rect.top, NULL ) ;
    //	m_TalkDataCob.SetLimitText ( 256 ) ;
    // Init the edit
    m_TalkNameEdt.Init ( rect.left, rect.top, false, NULL, 0x00ffff );
    m_TalkNameEdt.SetLimitText ( 16 ) ;
    m_TalkNameEdt.SetOffset ( CPoint ( 4, 2 ) ) ;
    m_StaTxtAction.Init(rect.left, rect.top);
    // Init the image
    m_TalkModeImg.Init ( rect.left, rect.top, "Button50", 10 );
    // Init the button
    m_TalkMipmapBtn.Init( rect.left, rect.top, "Button417", 2 );
    m_TalkMipmapBtn.SetCheckValue(1);
    m_TalkHelpBtn.Init( rect.left, rect.top, "Button418" );
    m_TalkPkChk.Init( rect.left, rect.top, "Check42", 4 );
    m_TalkActBtn.Init( rect.left, rect.top, "Check40");
    m_TalkSendBtn.Init ( rect.left, rect.top, "Button511" );
    m_TalkClearBtn.Init ( rect.left, rect.top , "Button55" );
    m_HideGoodBox.Init(rect.left, rect.top, "Button414");
    m_TalkMyChatBtn.Init ( rect.left, rect.top, "Button413");
    m_ViewEquipBtn.Init( rect.left , rect.top, "Button419");
    m_TargetListBtn.Init(rect.left, rect.top, "Button420");
    m_EmotionListBtn.Init(rect.left, rect.top, "Button582");
    // Init the check
    // Init the progress
    m_PrgExp.Init( rect.left, rect.top, "Progress50", 0, 100, MY_PROGRESS_HRIGHT ) ;
    // Init the menu
    m_MenuTalk.Create ( IDD_MENU_TALK, this->GetParent () );
    m_MenuTalk.SetParentWnd ( this ) ;
    // Create target name list box dialog
    m_DlgListBox.Create(IDD_DIALOG_TALKTARGET, this->GetParent());
    m_DlgListBox.ShowWindow(SW_SHOW);
    m_DlgListBox.EnableWindow(false);
    m_DlgListBox.m_ListBox.SetParentCob(this);
    m_DlgListAction.Create(IDD_DIALOG_TXTACTION, this->GetParent());
    m_DlgListAction.ShowWindow(SW_SHOW);
    m_DlgListAction.EnableWindow(false);
    m_DlgListAction.m_ListTxtAction.SetParentCob(this);
    SetActionList();
    CRect prgRect ;
    m_PrgExp.GetWindowRect( prgRect ) ;
    m_PrgRect[0] = CRect ( prgRect.left - rect.left,
                           prgRect.top - rect.top,
                           prgRect.right - rect.left,
                           prgRect.bottom - rect.top + 10) ;
    m_TalkPkChk.GetWindowRect( prgRect ) ;
    m_PrgRect[1] = CRect ( prgRect.left - rect.left,
                           prgRect.top - rect.top,
                           prgRect.right - rect.left,
                           prgRect.bottom - rect.top ) ;
    m_TalkMipmapBtn.GetWindowRect( prgRect ) ;
    m_PrgRect[2] = CRect ( prgRect.left - rect.left,
                           prgRect.top - rect.top,
                           prgRect.right - rect.left,
                           prgRect.bottom - rect.top ) ;
    m_EmotionListBtn.GetWindowRect( prgRect ) ;
    m_PrgRect[3] = CRect ( prgRect.left - rect.left,
                           prgRect.top - rect.top,
                           prgRect.right - rect.left,
                           prgRect.bottom - rect.top ) ;
    m_TalkHelpBtn.GetWindowRect( prgRect ) ;
    m_PrgRect[4] = CRect ( prgRect.left - rect.left,
                           prgRect.top - rect.top,
                           prgRect.right - rect.left,
                           prgRect.bottom - rect.top ) ;
    m_ViewEquipBtn.GetWindowRect( prgRect ) ;
    m_PrgRect[5] = CRect ( prgRect.left - rect.left,
                           prgRect.top - rect.top,
                           prgRect.right - rect.left,
                           prgRect.bottom - rect.top ) ;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTalk::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

void CDlgTalk::Show()
{
    if ( m_bShow )
    {
        // Show the button
        m_TalkModeImg.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkPkChk.Show( m_Pnt.x, m_Pnt.y);
        m_TalkActBtn.Show( m_Pnt.x, m_Pnt.y);
        m_TalkHelpBtn.Show( m_Pnt.x, m_Pnt.y);
        if (g_objHero.IsFlyHelpShow())
        {
            CAni* HelpFlash = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                              "Button418",
                              EXIGENCE_IMMEDIATE ) ;
            if (0 == m_dwFlashTime)
            {
                m_dwFlashTime = ::TimeGet();
            }
            if ( HelpFlash != NULL )
            {
                int nFrame = (::TimeGet() - m_dwFlashTime) / _FLASH_SPEED;
                nFrame = nFrame % 2;
                if (1 == nFrame)
                    HelpFlash->Show(2, m_Pnt.x + m_TalkHelpBtn.m_IntPnt.x - 1,
                                    m_Pnt.y + m_TalkHelpBtn.m_IntPnt.y - 1);
            }
        }
        else
        {
            if (m_dwFlashTime != 0)
            {
                m_dwFlashTime = 0;
            }
        }
        m_HideGoodBox.Show(m_Pnt.x, m_Pnt.y);
        // Show the owner
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog5",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            if (m_bShowGoods)
            {
                pShowAni->Show( 0, 256, 602 );
                pShowAni->Show( 1, 512, 602 );
            }
            else
            {
                pShowAni->Show( 2, 256, 602 );
                pShowAni->Show( 3, 512, 602 );
            }
        }
        else
        {
            return ;
        }
        // Shwo the progress
        m_PrgExp.ResetValue( 0, g_objHero.GetNextLevelExp() ) ;
        m_PrgExp.Show( m_Pnt.x, m_Pnt.y, g_objHero.GetExp(), g_objHero.GetExp() ) ;
        m_TalkMipmapBtn.Show( m_Pnt.x, m_Pnt.y);
        // Show the GoodBox
        if (m_bShowGoods)
        {
            m_GoodBox.Show(m_Pnt.x, m_Pnt.y);
        }
        // Show the combo
        m_TalkDataCob.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the edit
        m_TalkNameEdt.Show ( m_Pnt.x, m_Pnt.y ) ;
        //		m_StaTxtAction.Show	(m_Pnt.x, m_Pnt.y );
        m_TalkSendBtn.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_TalkClearBtn.Show( m_Pnt.x, m_Pnt.y);
        //		m_TalkMyChatBtn.Show(m_Pnt.x,m_Pnt.y );
        m_ViewEquipBtn.Show(m_Pnt.x, m_Pnt.y);
        m_TargetListBtn.Show(m_Pnt.x, m_Pnt.y);
        m_EmotionListBtn.Show(m_Pnt.x, m_Pnt.y);
        // Show the menu
        m_MenuTalk.Show () ;
        CheckTalkTargetRect ();
        m_DlgListBox.Show();
        m_DlgListAction.Show();
    }
}

void CDlgTalk::CheckTalkTargetRect ()
{
    if (m_DlgListBox.m_bShow)
    {
        CRect rect;
        m_DlgListBox.GetWindowRect (rect);
        rect.bottom = rect.bottom + 100;
        CPoint pointMouse;
        GetCursorPos(&pointMouse);
        if(!rect.PtInRect (pointMouse) )
        {
            m_DlgListBox.EnableWindow (false);
        }
    }
}

void CDlgTalk::OnTalkBtnSend()
{
    // 判断是否进入焦点
    if (!m_bEnter)
    {
        m_TalkDataCob.SetFocus();
        return;
    }
    DXPlaySound("Sound/Dlg_Ok.wav");
    char Char[256] = "";
    memset(Char, 0L, sizeof(Char));
    char StrEmotion[16] = "";
    memset(StrEmotion, 0L, sizeof(StrEmotion));
    if (m_nTextActionIndex > 0)
    {
        sprintf(StrEmotion, "%d", m_nTextActionIndex);
    }
    m_TalkDataCob.GetWindowText (Char, 256) ;
    if ( Char[0] != NULL  && m_bShow )
    {
        m_TalkDataCob.SetWindowText ( NULL ) ;
        m_TalkDataCob.SetFocus () ;
        if (Char[0] == '/')
        {
            char szCmd[256] = "";
            if (!sscanf(Char + 1, "%s", szCmd))
            {
                return;
            }
            if (0 == stricmp(szCmd, "3deffect"))
            {
                char szIndex[256] = "";
                if (2 == sscanf(Char + 1, "%s %s", szCmd, szIndex))
                {
                    g_objHero.m_objEffect.Add(szIndex);
                }
                return;
            }
        }
        char strName[32] = "" ;
        m_TalkNameEdt.GetWindowText ( strName, 32 ) ;
        if (strcmp(strName, g_objHero.GetName()) == 0)
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10690));//"您不能对自己说话!"
            return;
        }
        // Show the screen message
        if (::TimeGet() - m_dwLastTime <= 1000 / _MAX_WORDS_PS)
        {
            g_objGameMsg.AddMsg( ( char* )g_objGameDataSet.GetStr ( 10044 ) ) ;//"阁下说话太快，还是歇歇先吧。");
            return;
        }
        m_dwLastTime	=::TimeGet();
        DWORD dwColor;
        switch ( m_uTalkMode )
        {
        case G_CLICK:
            g_objGameMsg.GetChannelColor(_TXTATR_TALK, dwColor);
            if ( m_TalkNameEdt.GetWindowTextLength () > 0 )
            {
                g_objHero.Talk(	strName,
                                Char,
                                StrEmotion,
                                dwColor,
                                _TXTATR_TALK);
            }
            else
                g_objHero.Talk(	( CPlayer* )NULL,
                                Char,
                                NULL,
                                dwColor,
                                _TXTATR_TALK);
            break ;
        case S_CLICK:
            {
                if ( strName )
                    g_objHero.Talk(	strName,
                                    Char,
                                    StrEmotion,
                                    0xffffff,
                                    _TXTATR_PRIVATE);
            }
            break ;
        case D_CLICK:
            {
                if (strName && strlen(strName) > 0)
                    g_objHero.Talk(	strName,
                                    Char,
                                    StrEmotion,
                                    0xffffff,
                                    _TXTATR_TEAM);
                else
                    g_objHero.Talk(	( CPlayer* )NULL,
                                    Char,
                                    NULL,
                                    0xffffff,
                                    _TXTATR_TEAM);
            }
            break ;
        case B_CLICK:
            {
                if (strName && strlen(strName) > 0 )
                    g_objHero.Talk(	strName,
                                    Char,
                                    StrEmotion,
                                    0xffffff,
                                    _TXTATR_SYNDICATE);
                else
                    g_objHero.Talk(	( CPlayer* )NULL,
                                    Char,
                                    NULL,
                                    0xffffff,
                                    _TXTATR_SYNDICATE);
            }
            break ;
        case H_CLICK:
            if ( m_TalkNameEdt.GetWindowTextLength () > 0 )
            {
                g_objHero.Talk(	strName,
                                Char,
                                StrEmotion,
                                0xffffff,
                                _TXTATR_FRIEND);
            }
            else
            {
                g_objHero.Talk(	( CPlayer* )NULL,
                                Char,
                                NULL,
                                0xffffff,
                                _TXTATR_FRIEND);
            }
            break ;
        }
        // Set the Talk target list
        CDlgTalk::SetTalkTargetList () ;
        for ( int i = 0; i < m_nListAmount; i++ )
        {
            char tempChar[1024];
            m_ListBox.GetText ( i, tempChar ) ;
            if ( strcmp ( tempChar, Char ) == 0 )
            {
                m_ListBox.DeleteString ( i ) ;
                m_ListBox.InsertString ( 0, Char ) ;
                m_nListIndex = 0 ;
                return ;
            }
        }
        m_ListBox.InsertString ( 0, Char ) ;
        m_nListAmount++ ;
        m_nListIndex = 0 ;
        if ( m_nListAmount > 20 )
        {
            m_nListAmount-- ;
            m_ListBox.DeleteString ( 20 ) ;
        }
    }
    else
    {
        CDlgTalk::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TALK_CLOSE ) ;
    }
    // 关闭聊天信息输入状态
    PostMessage(WM_KEYUP, VK_ESCAPE);
}


void CDlgTalk::OnTalkImgMode()
{
    if ( !m_MenuTalk.m_bShow )
    {
        m_MenuTalk.EnableWindow() ;
    }
    else
    {
        m_MenuTalk.EnableWindow( false ) ;
    }
}

void CDlgTalk::SetTalkTarget(CPlayer* pTalkTarget)
{
    m_TalkPlayerID = pTalkTarget->GetID() ;
    if ( pTalkTarget->GetName() )
    {
        m_TalkNameEdt.SetWindowText ( pTalkTarget->GetName () ) ;
    }
}

void CDlgTalk::OnLButtonDown(UINT nFlags, CPoint point)
{
    if ( m_PrgRect[0].PtInRect( point ) == TRUE)
    {
        CDlgTalk::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MAIN_EXPDOWN ) ;
        m_bShowExp = !m_bShowExp;
    }
    else
    {
        CDlgTalk::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TALK_CLOSE ) ;
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgTalk::OnUpdateTalkEdtName()
{
    /*	char strName[16] ;
    	m_TalkNameEdt.GetWindowText ( strName, 16 ) ;
    	if ( strName[0] != NULL )
    	{
    		char strTemp[16] ;
    		BOOL bOk = false ;
    		int nCount = m_DlgListBox.m_ListBox.GetCount () ;
    		if ( nCount > 0 )
    		{
    			for ( int i = 0; i < nCount; i++ )
    			{
    				m_DlgListBox.m_ListBox.GetText ( i, strTemp ) ;
    				if ( strcmp ( strName, strTemp ) == 0 )
    				{
    					bOk = true ;
    					m_TalkNameEdt.SetFontColor ( 0xff0000 ) ;
    					m_uDelIndex = i ;
    					m_uAddBtnMode = 2 ;
    					break ;

    				}
    			}
    			if ( !bOk )
    			{
    				if ( nCount < 18 )
    				{
    					m_TalkNameEdt.SetFontColor ( 0xffffff ) ;
    					m_uAddBtnMode = 1 ;
    				}
    				else
    				{
    					m_TalkNameEdt.SetFontColor ( 0xffffff ) ;
    					m_uAddBtnMode = 0 ;
    				}
    			}
    		}
    		else
    		{
    			m_TalkNameEdt.SetFontColor ( 0xffffff ) ;
    			m_uAddBtnMode = 1 ;
    		}
    	}
    	else
    	{
    		m_TalkNameEdt.SetFontColor ( 0xffffff ) ;
    		m_uAddBtnMode = 0 ;
    	}*/
}

void CDlgTalk::SetTalkMode( UINT uTalkMode )
{
    m_uTalkMode = uTalkMode ;
    switch ( m_uTalkMode )
    {
    case G_CLICK:
        m_TalkModeImg.ChangeImage( "Button50" ) ;
        break ;
    case S_CLICK:
        m_TalkModeImg.ChangeImage( "Button51" ) ;
        break ;
    case D_CLICK:
        m_TalkModeImg.ChangeImage( "Button52" ) ;
        break ;
    case B_CLICK:
        m_TalkModeImg.ChangeImage( "Button53" ) ;
        break ;
    case H_CLICK:
        m_TalkModeImg.ChangeImage( "Button54" ) ;
        break ;
    }
}

void CDlgTalk::DeleteTalkTarget()
{
    m_DlgListBox.m_ListBox.DeleteString ( m_uDelIndex ) ;
    m_nTargetAmount = m_DlgListBox.m_ListBox.GetCount();
    m_TalkNameEdt.SetWindowText ( NULL ) ;
    m_TalkNameEdt.SetFocus () ;
    if (m_DlgListBox.m_bShow)
    {
        CRect rect ;
        m_DlgListBox.GetWindowRect ( rect ) ;
        int nHeight = m_DlgListBox.m_ListBox.GetCount() * (CMyBitmap::GetFontSize () + 4);
        m_DlgListBox.MoveWindow(170, _SCR_HEIGHT - 80 - nHeight, rect.Width(), nHeight, false);
        m_DlgListBox.EnableWindow(true);
    }
}

void CDlgTalk::SetTalkTargetList()
{
    // Must record the target name and talk mode
    char strName[16] ;
    m_TalkNameEdt.GetWindowText ( strName, 16 ) ;
    if ( strName && strlen(strName) > 0 )
    {
        // Test if already record
        for ( int i = 0; i < 10; i++ )
        {
            if ( m_uTargetTalkMode[i] == 0 || strlen(m_strTargetName[i]) <= 0)
            {
                break ;
            }
            else if (strcmp ( strName, m_strTargetName[i] ) == 0 )
            {
                return ;
            }
        }
        // The recordset is already full
        if ( i == 10 )
        {
            for ( int i = 0; i < 10; i++ )
            {
                m_uTargetTalkMode[i] = m_uTargetTalkMode[i + 1] ;
                strcpy ( m_strTargetName[i], m_strTargetName[i + 1] ) ;
            }
            m_uTargetTalkMode[i] = m_uTalkMode ;
            strcpy ( m_strTargetName[i], strName ) ;
            m_DlgListBox.m_ListBox.DeleteString(0);
            m_DlgListBox.m_ListBox.AddString(strName);
            m_nTargetAmount = 10 ;
        }
        // not full
        else
        {
            m_uTargetTalkMode[i] = m_uTalkMode ;
            strcpy ( m_strTargetName[i], strName ) ;
            m_nTargetAmount = i + 1 ;
            m_DlgListBox.m_ListBox.AddString(strName);
        }
    }
    if(m_DlgListBox.m_bShow)
    {
        CRect rect ;
        m_DlgListBox.GetWindowRect ( rect ) ;
        int nHeight = m_DlgListBox.m_ListBox.GetCount() * (CMyBitmap::GetFontSize () + 4);
        m_DlgListBox.MoveWindow(170, _SCR_HEIGHT - 80 - nHeight, rect.Width(), nHeight, false);
        m_DlgListBox.EnableWindow(true);
    }
}

void CDlgTalk::CycleTalkTarget( int nDir )
{
    if ( nDir == SB_LEFT )
    {
        if ( m_nTargetAmount > 0 )
        {
            if ( m_nCurTargetIndex == 0 )
            {
                m_nCurTargetIndex = m_nTargetAmount - 1;
            }
            else
            {
                m_nCurTargetIndex-- ;
            }
            CDlgTalk::SetTalkMode ( m_uTargetTalkMode[m_nCurTargetIndex] ) ;
            if ( m_strTargetName[m_nCurTargetIndex][0] != NULL )
            {
                m_TalkNameEdt.SetWindowText ( m_strTargetName[m_nCurTargetIndex] ) ;
            }
            else
            {
                m_TalkNameEdt.SetWindowText ( NULL ) ;
            }
        }
    }
    else if ( nDir == SB_RIGHT )
    {
        if ( m_nTargetAmount > 0 )
        {
            if ( m_nCurTargetIndex == m_nTargetAmount - 1)
            {
                m_nCurTargetIndex = 0 ;
            }
            else
            {
                m_nCurTargetIndex++ ;
            }
            CDlgTalk::SetTalkMode ( m_uTargetTalkMode[m_nCurTargetIndex] ) ;
            if ( m_strTargetName[m_nCurTargetIndex][0] != NULL )
            {
                m_TalkNameEdt.SetWindowText ( m_strTargetName[m_nCurTargetIndex] ) ;
            }
            else
            {
                m_TalkNameEdt.SetWindowText ( NULL ) ;
            }
        }
    }
}

void CDlgTalk::CycleTalkChannel(int nDir)
{
    if ( nDir == SB_TOP )
    {
        UINT uMode = m_uTalkMode;
        if ( m_uTalkMode == G_CLICK )
        {
            uMode = H_CLICK ;
        }
        else
        {
            uMode-- ;
        }
        CDlgTalk::SetTalkMode ( uMode ) ;
    }
    else if ( nDir == SB_BOTTOM )
    {
        UINT uMode = m_uTalkMode;
        if ( m_uTalkMode == H_CLICK )
        {
            uMode = G_CLICK ;
        }
        else
        {
            uMode++ ;
        }
        CDlgTalk::SetTalkMode ( uMode ) ;
    }
}

void CDlgTalk::OnMouseMove(UINT nFlags, CPoint point)
{
    CDialog::OnMouseMove(nFlags, point);
    if ( !m_bMouseMove )
    {
        for(int i = 0; i < 6; i++)
        {
            if ( m_PrgRect[i].PtInRect( point ))
            {
                CDlgTalk::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_STATIC, 4 + i ) ;
                m_bMouseMove = true ;
                break;
            }
        }
    }
    else
    {
        for(int i = 0; i < 6; i++)
        {
            if ( m_PrgRect[i].PtInRect( point ) )
            {
                m_bMouseMove = false;
                break;
            }
        }
        if (i == 6)
        {
            CDlgTalk::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MSGDLG_HIDE ) ;
            m_bMouseMove = false ;
        }
    }
}


void CDlgTalk::OnTalkChkPk()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/DlgTalk_Pk.wav" ) ;
    g_objHero.SetPkMode( m_TalkPkChk.GetCheckValue()) ;
}

void CDlgTalk::OnTalkChkRun()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}

void CDlgTalk::OnTalkBtnMipmap()
{
    // TODO: Add your control notification handler code here
    CDlgTalk::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MIPMAP_OPEN);
}

void CDlgTalk::OnTalkBtnHelp()
{
    // TODO: Add your control notification handler code here
    CDlgTalk::GetParent()->PostMessage(WM_MY_MESSAGE, ON_INSHELP_SHOW);
    DXPlaySound("Sound/Dlg_Ok.wav");
}



void CDlgTalk::OnBtnHidegoods()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_Flex.wav" ) ;
    m_bShowGoods = !m_bShowGoods;
    m_GoodBox.EnableWindow(m_bShowGoods);
    SetTalkDlgRgn();
}

void CDlgTalk::SetTalkDlgRgn()
{
    /////////////////////////////////////////////////////
    if (!m_bShowGoods)
    {
        CRect rect;
        CDlgTalk::GetWindowRect(rect);
        CPoint pnt[4] ;
        pnt[0].x = 0;
        pnt[0].y = 15;
        pnt[1].x = rect.right;
        pnt[1].y = 15;
        pnt[2].x = rect.right;
        pnt[2].y = rect.bottom;
        pnt[3].x = 0;
        pnt[3].y = rect.bottom;
        CRgn rgn ;
        rgn.CreatePolygonRgn ( pnt, 4, ALTERNATE ) ;
        CDlgTalk::SetWindowRgn ( rgn, true ) ;
    }
    else
    {
        CRect rect;
        CDlgTalk::GetWindowRect(rect);
        CPoint pnt[8] ;
        pnt[0].x = 0;
        pnt[0].y = 15;
        pnt[1].x = 190;
        pnt[1].y = 15;
        pnt[2].x = 190;
        pnt[2].y = 0;
        pnt[3].x = 620;
        pnt[3].y = 0;
        pnt[4].x = 620;
        pnt[4].y = 15;
        pnt[5].x = rect.right;
        pnt[5].y = 15;
        pnt[6].x = rect.right;
        pnt[6].y = rect.bottom;
        pnt[7].x = 0;
        pnt[7].y = rect.bottom;
        CRgn rgn ;
        rgn.CreatePolygonRgn ( pnt, 8, ALTERNATE ) ;
        CDlgTalk::SetWindowRgn ( rgn, true ) ;
    }
}

void CDlgTalk::OnTalkClearbtn()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    g_objGameMsg.ClearMsg();
}

bool CDlgTalk::GetIsGameMsgEx()
{
    return m_bGameMsgEx;
}

void CDlgTalk::OnTalkMychat()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    m_bGameMsgEx = !m_bGameMsgEx;
    CDlgTalk::GetParent()->PostMessage(WM_MY_MESSAGE, ON_CLICK_CHATSET);
}

void CDlgTalk::OnTalkBtnViewequip()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgTalk::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MAIN_NPCEQUIP , 1);
}

void CDlgTalk::OnTalkBtnTargetlist()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_DlgListAction.m_bShow)
    {
        m_DlgListAction.EnableWindow(false);
    }
    if (!m_DlgListBox.m_bShow)
    {
        CRect rect ;
        m_DlgListBox.GetWindowRect ( rect ) ;
        int nHeight = m_DlgListBox.m_ListBox.GetCount() * (CMyBitmap::GetFontSize () + 4);
        m_DlgListBox.MoveWindow(170, _SCR_HEIGHT - 80 - nHeight, rect.Width(), nHeight, false);
        m_DlgListBox.EnableWindow(true);
    }
    else
    {
        m_DlgListBox.EnableWindow(false);
    }
}

void CDlgTalk::OnTalkBtnEmotionlist()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    //	if (m_DlgListBox.m_bShow)
    //	{
    //		m_DlgListBox.EnableWindow(false);
    //	}
    //	if ( !m_DlgListAction.m_bShow)
    //	{
    //		CRect rect;
    //		m_DlgListAction.GetWindowRect(rect);
    //
    //		int nHeight = 10* (CMyBitmap::GetFontSize () + 4);
    //		m_DlgListAction.MoveWindow(660,_SCR_HEIGHT - 80 - nHeight,rect.Width(),nHeight,false);
    //		m_DlgListAction.EnableWindow(true);
    //	}
    //	else
    //		m_DlgListAction.EnableWindow(false);
    CDlgTalk::GetParent()->PostMessage (WM_MY_MESSAGE, ON_DLGEMOTION_OPEN);
}

void CDlgTalk::SetActionList()
{
    int nAmount = g_obj3DRoleData.GetTextActionAmount();
    for(int i = 0; i < nAmount; i++)
    {
        m_DlgListAction.m_ListTxtAction.AddString(g_obj3DRoleData.GetTextActionTitle(i));
    }
    m_StaTxtAction.SetWindowText(g_obj3DRoleData.GetTextActionTitle(m_nTextActionIndex));
}

void CDlgTalk::ChangePkMode()
{
    if (m_TalkPkChk.GetCheckValue() >= 0 && m_TalkPkChk.GetCheckValue() < 3)
    {
        m_TalkPkChk.SetCheckValue(m_TalkPkChk.GetCheckValue() + 1);
    }
    else
    {
        m_TalkPkChk.SetCheckValue(0);
    }
    g_objHero.SetPkMode( m_TalkPkChk.GetCheckValue()) ;
}

void CDlgTalk::InsertEmotion(int nEmotion)
{
    if (nEmotion >= 0 && nEmotion < g_objGameDataSet.GetEmotionAmount ())
    {
        char Char[256] = "";
        char szEmotion[16] = "";
        m_TalkDataCob.GetWindowText (Char, 256) ;
        if (nEmotion < 10)
        {
            sprintf(szEmotion, "#0%d", nEmotion);
        }
        else
        {
            sprintf(szEmotion, "#%d", nEmotion);
        }
        strcat(Char, szEmotion);
        m_TalkDataCob.SetWindowText (Char);
        m_TalkDataCob.SetFocus();
        m_TalkDataCob.GetWindowText (Char, 256) ;
        m_TalkDataCob.SetSel((int)strlen(Char), (int)strlen(Char));
    }
}


BOOL CDlgTalk::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (WM_KEYUP == pMsg->message)
    {
        // 判断是否按下 ESC 键
        if (VK_ESCAPE == pMsg->wParam)
        {
            CDlgTalk::GetParentOwner()->SetFocus();
        }
        // 判断是否按下 ENTER 键
        else if (VK_RETURN == pMsg->wParam)
        {
            if (this->GetFocus()->GetSafeHwnd() == m_TalkDataCob.GetSafeHwnd())
            {
                if (m_bHotEnter)
                {
                    CDlgTalk::OnTalkBtnSend ( ) ;
                }
                else
                {
                    m_bHotEnter = true;
                }
            }
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CDlgTalk::OnSetfocusTalkEdtData()
{
    // TODO: Add your control notification handler code here
    m_bEnter = true;
}

void CDlgTalk::OnKillfocusTalkEdtData()
{
    // TODO: Add your control notification handler code here
    if (GetFocus()->GetSafeHwnd() != m_TalkSendBtn.GetSafeHwnd())
    {
        m_bEnter = false;
    }
}
