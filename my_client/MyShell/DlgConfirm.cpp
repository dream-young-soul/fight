
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgConfirm.h"
#include "ND_Bitmap.h"

#include "Hero.h"
#include "GameMsg.h"
#include "GamePlayerset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const float _DLG_WIDTH = 126.0;

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog


CDlgConfirm::CDlgConfirm(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgConfirm::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgConfirm)
    m_bShow = false ;
    m_nEdtNum = 0 ;
    m_nLine = 0 ;
    //}}AFX_DATA_INIT
}


void CDlgConfirm::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgConfirm)
    DDX_Control(pDX, IDC_CONFIRM_EDT_NAME, m_ConfirmEdtName);
    DDX_Control(pDX, IDC_CONFIRM_BTN_OK, m_ConfirmBtnOk ) ;
    DDX_Control(pDX, IDC_CONFIRM_BTN_CANCEL, m_ConfirmBtnCancel ) ;
    DDX_Control(pDX, IDC_CONFIRM_EDT_DATA, m_ConfirmEdtData);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfirm, CDialog)
    //{{AFX_MSG_MAP(CDlgConfirm)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_CONFIRM_BTN_OK, OnConfirmBtnOk)
    ON_BN_CLICKED(IDC_CONFIRM_BTN_CANCEL, OnConfirmBtnCancel)
    ON_EN_CHANGE(IDC_CONFIRM_EDT_DATA, OnChangeConfirmEdtData)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm message handlers

HBRUSH CDlgConfirm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgConfirm::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgConfirm::GetWindowRect ( rect ) ;
    // Init the Edit
    m_ConfirmEdtData.Init ( rect.left, rect.top ) ;
    m_ConfirmEdtData.SetLimitText ( 12 ) ;
    m_ConfirmEdtName.Init(rect.left, rect.top );
    m_ConfirmEdtName.SetLimitText(16);
    // Init the button
    m_ConfirmBtnOk.Init ( rect.left, rect.top, "Button150" );
    m_ConfirmBtnCancel.Init ( rect.left, rect.top, "Button151" );
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfirm::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

LRESULT CDlgConfirm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        m_nEdtNum = 0;
        m_ConfirmEdtData.SetFocus();
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            //			CDlgConfirm::OnConfirmBtnOk () ;
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgConfirm::OnConfirmBtnCancel () ;
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgConfirm::OnConfirmBtnOk()
{
    if (m_uType != EXIT_CONFIRM)
    {
        DXPlaySound ( "Sound/button_confirm.wav" ) ;
    }
    if (m_ConfirmEdtName.IsWindowVisible())
    {
        char szName[64];
        m_ConfirmEdtName.GetWindowText(szName, sizeof(szName));
        if (!StringCheck(szName))
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100500));
            m_ConfirmEdtName.SetWindowText("");
            return;
        }
    }
    switch ( m_uType )
    {
    case FACTIONADDALLY_CONFIRM:
        {
            CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FACTION_ADDALLYOK , m_TargetID) ;
        }
        break;
    case FACTIONADDENEMY_CONFIRM:
        {
            char szSynName[_MAX_NAMESIZE] = "";
            m_ConfirmEdtName.GetWindowText(szSynName, 16);
            if (strlen(szSynName) > 0)
            {
                g_objHero.AddAntagonizeSyn(szSynName);
            }
        }
        break;
    case TRADE_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TRADE_ACCEPT ) ;
        break ;
    case TEAMJOIN_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TEAMJOIN_ACCEPT ) ;
        break ;
    case TEAMINVITE_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TEAMINVITE_ACCEPT ) ;
        break ;
    case FACTIONJOIN_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FACTIONJOIN_ACCEPT ) ;
        break ;
    case FACTIONINVITE_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FACTIONINVITE_ACCEPT ) ;
        break ;
    case MONEYDROP_CONFIRM:
        if ( m_nEdtNum > 0 )
        {
            CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MONEYDROP_ACCEPT, m_nEdtNum ) ;
        }
        m_ConfirmEdtData.SetWindowText ( NULL ) ;
        break ;
    case FRIENDDEL_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FRIENDDEL_ACCEPT ) ;
        break ;
    case FRIENDADD_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FRIENDADD_ACCEPT, m_TargetID ) ;
        break ;
    case TALKDEL_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TALKDEL_ACCEPT ) ;
        break ;
    case FACTIONOUT_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FACTIONOUT_ACCEPT ) ;
        break ;
    case FACTIONSUB_CONFIRM:
        if ( m_nEdtNum > 0 && m_nEdtNum <= (int)g_objHero.GetMoney () )
        {
            CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FACTIONSUB_ACCEPT, m_nEdtNum ) ;
        }
        m_ConfirmEdtData.SetWindowText ( NULL ) ;
        break ;
    case MPCBOOTHADD_CONFIRM:
        if ( m_nEdtNum > 0 )
        {
            CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTHADD_ACCEPT, m_nEdtNum ) ;
        }
        m_ConfirmEdtData.SetWindowText ( NULL ) ;
        break ;
    case MPCBOOTHCHG_CONFIRM:
        if ( m_nEdtNum > 0 )
        {
            CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCBOOTHCHG_ACCEPT, m_nEdtNum ) ;
        }
        m_ConfirmEdtData.SetWindowText ( NULL ) ;
        break ;
    case FACTIONDEL_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FACTIONDEL_ACCEPT ) ;
        break ;
    case BOOTHOPEN_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_BOOTHOPEN_ACCEPT, m_TargetID ) ;
        break ;
    case MPCCOURT_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_MPCCOURT_ACCEPT, m_TargetID ) ;
        break ;
    case IMPTITEM_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_IMPTITEM_ACCEPT , m_TargetID) ;
        break ;
    case DROPITEM_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_DROPITEM_ACCEPT , m_TargetID) ;
        break ;
    case TACKOFF_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TACKOFF_ACCEPT ) ;
        break ;
    case JOINDICE_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_JOINDICE_ACCEPT, m_TargetID ) ;
        break ;
    case OUTDICE_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_OUTDICE_ACCEPT ) ;
        break ;
    case ENEMYDEL_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ENEMYDEL_ACCEPT ) ;
        break ;
    case RESOURCE_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_RESOURCE_ACCEPT ) ;
        break ;
    case EXIT_CONFIRM:
        DXPlaySound("Sound/Exit_Ok.wav");
        CDlgConfirm::GetParent()->PostMessage( WM_MY_MESSAGE, ON_EXIT_ACCEPT, m_TargetID ) ;
        break ;
    case ROLECREATE_CONFIRM:
        CDlgConfirm::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ROLECREATE_ACCEPT, m_TargetID ) ;
        break ;
    case SPLITITEM_CONFIRM:
        if ( m_nEdtNum > 0 )
        {
            LPARAM lParam = MAKELONG( m_TargetID, m_nEdtNum ) ;
            CDlgConfirm::GetParent()->PostMessage( WM_MY_MESSAGE, ON_SPLITITEM_ACCEPT, lParam ) ;
        }
        break ;
    case TEACHER_CONFIRM:
        CDlgConfirm::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TEACHER_ACCEPT); //同意拜师请求
        break;
    case STUDENT_CONFIRM:
        CDlgConfirm::GetParent()->PostMessage(WM_MY_MESSAGE, ON_STUDENT_ACCEPT); //同意招徒请求
        break;
    case MONSTERDROP_CONFIRM:
        CDlgConfirm::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MONSTERDROP_ACCEPT); //同意丢弃幻兽
        break;
    case TEAMDISMISS_CONFIRM:
        CDlgConfirm::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TEAMDISMISS_ACCEPT); //同意
        break;
    case CHECKOUT_MONSTEREGG:
        CDlgConfirm::GetParent()->PostMessage(WM_MY_MESSAGE, ON_CHECKOUTEGG_ACCEPT, m_TargetID); //拿出幻兽蛋
    default:
        break ;
    }
    if ( m_ConfirmEdtData.IsWindowEnabled() )
    {
        m_ConfirmEdtData.SetWindowText( NULL ) ;
    }
    if (m_ConfirmEdtName.IsWindowEnabled())
    {
        m_ConfirmEdtName.SetWindowText( NULL );
    }
    m_nLine = 0 ;
    CDlgConfirm::EnableWindow( false ) ;
}

void CDlgConfirm::OnConfirmBtnCancel()
{
    DXPlaySound ( "Sound/button_cancel.wav" ) ;
    switch ( m_uType )
    {
    case IMPTITEM_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        break ;
    case DROPITEM_CONFIRM:
        CDlgConfirm::GetParent ()->PostMessage ( WM_MY_MESSAGE, CMD_FLASHITEM ) ;
        break ;
    case EXIT_CONFIRM:
        CDlgConfirm::GetParent()->PostMessage( WM_MY_MESSAGE, ON_EXIT_REFUSE ) ;
        break ;
    case ROLECREATE_CONFIRM:
        CDlgConfirm::GetParent()->PostMessage( WM_MY_MESSAGE, ON_ROLECREATE_REFUSE, m_TargetID ) ;
        break ;
    case TEAMJOIN_CONFIRM:
        g_objHero.Reject( REJECT_TEAM ) ;
        break ;
    case TEAMINVITE_CONFIRM:
        g_objHero.Reject( REJECT_TEAM ) ;
        break ;
    case TRADE_CONFIRM:
        g_objHero.Reject( REJECT_TRADE ) ;
        break ;
    case FRIENDADD_CONFIRM:
        g_objHero.Reject( REJECT_FRIEND ) ;
        break ;
    case TEACHER_CONFIRM:
        g_objHero.Reject( REJECT_TEACHER ) ;
        break;
    case STUDENT_CONFIRM:
        g_objHero.Reject( REJECT_STUDENT ) ;
        break;
    case MONSTERDROP_CONFIRM:
        break;
    default:
        break ;
    }
    m_nLine = 0 ;
    CDlgConfirm::EnableWindow( false ) ;
    CDlgConfirm::GetParent()->SetFocus() ;
}

void CDlgConfirm::Show()
{
    if ( m_bShow )
    {
        // Show the owner
        CAni* ShowAni ;
        if ( m_uType != MPCCOURT_CONFIRM &&
                m_uType != ACTION_CONFIRM &&
                m_uType != FRIENDADD_CONFIRM &&
                m_uType != FACTIONADDALLY_CONFIRM &&
                m_uType != FRIENDADDSUC_CONFIRM &&
                m_uType != CHECKOUT_MONSTEREGG)
        {
            ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                                    "Dialog15",
                                                    EXIGENCE_IMMEDIATE ) ;
            if ( ShowAni != NULL )
                ShowAni->Show ( 0,
                                m_Pnt.x,
                                m_Pnt.y ) ;
            else
            {
                return ;
            }
        }
        else if ( m_uType == MPCCOURT_CONFIRM )
        {
            ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                                    "Dialog45",
                                                    EXIGENCE_IMMEDIATE ) ;
            if ( ShowAni != NULL )
                ShowAni->Show ( 0,
                                m_Pnt.x - 53,
                                m_Pnt.y - 40 ) ;
            else
            {
                return ;
            }
        }
        else if ( m_uType == ACTION_CONFIRM ||
                  m_uType == FRIENDADD_CONFIRM ||
                  m_uType == FRIENDADDSUC_CONFIRM ||
                  m_uType == CHECKOUT_MONSTEREGG ||
                  m_uType  == FACTIONADDALLY_CONFIRM)
        {
            ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                                                    "Dialog15",
                                                    EXIGENCE_IMMEDIATE ) ;
            if ( ShowAni != NULL )
            {
                if (m_uType != ACTION_CONFIRM)
                    ShowAni->Show ( 1,
                                    m_Pnt.x - 125,
                                    m_Pnt.y ) ;
                else
                    ShowAni->Show ( 1,
                                    m_Pnt.x - 140,
                                    m_Pnt.y ) ;
            }
            else
            {
                return ;
            }
        }
        // Show the button
        if (m_ConfirmBtnOk.IsWindowEnabled())
        {
            m_ConfirmBtnOk.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        if (m_ConfirmBtnCancel.IsWindowEnabled())
        {
            m_ConfirmBtnCancel.Show ( m_Pnt.x, m_Pnt.y ) ;
        }
        // Show the char
        switch ( m_uType )
        {
        case TEACHER_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 34,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr(10643)) ;	//10643=要拜你为师
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001 ) ) ;
            }
            break;
        case STUDENT_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 34,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr(10644) ) ;		//10644=要收你为徒
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001 ) ) ;
            }
            break;
        case TRADE_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10000 ) ) ;//"请求交易" ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001 ) ) ;//"是否接受" ) ;
            }
            break ;
        case TEAMJOIN_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 27,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10002 ) ) ;//"请求加入队伍" ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001 ) ) ;//"是否接受" ) ;
            }
            break ;
        case TEAMINVITE_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 27,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10003 ) ) ;//"邀请加入队伍" ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001 ) ) ;//"是否接受" ) ;
            }
            break ;
        case FACTIONJOIN_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 27,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10004 ) ) ;//"申请加入帮派" ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001 ) ) ;//"是否接受" ) ;
            }
            break ;
        case FACTIONINVITE_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 27,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10005 ) ) ;//"邀请加入帮派" ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001) ) ;//"是否接受" ) ;
            }
            break ;
        case MONEYDROP_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 8,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10006 ) ) ;//"输入丢弃金额" ) ;
            m_ConfirmEdtData.Show ( m_Pnt.x, m_Pnt.y ) ;
            break ;
        case FRIENDDEL_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10007 ) ) ;//"确定删除好友" ) ;
            break ;
        case TALKDEL_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10008 ) ) ;//"确定删除对象" ) ;
            break ;
        case FACTIONOUT_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10009 ) ) ;//"确定退出帮派" ) ;
            break ;
        case FACTIONSUB_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 8,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10010 ) ) ;//"输入捐献金额" ) ;
            m_ConfirmEdtData.Show ( m_Pnt.x, m_Pnt.y ) ;
            break ;
        case FACTIONADDALLY_CONFIRM:
            if (m_strText)
            {
                CMyBitmap::ShowString ( m_Pnt.x - m_nLeftLeft,
                                        m_Pnt.y + 13 ,
                                        0xffff00,
                                        m_strText ) ;
            }
            break;
        case FACTIONADDENEMY_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,			//"输入帮派名称:"
                                    m_Pnt.y + 8,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10854 )) ;
            m_ConfirmEdtName.Show ( m_Pnt.x, m_Pnt.y ) ;
            break;
        case MPCBOOTHADD_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 8,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10353 ) ) ;//"输入物品价格" ) ;
            m_ConfirmEdtData.Show ( m_Pnt.x, m_Pnt.y ) ;
            break ;
        case MPCBOOTHCHG_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 8,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10353 ) ) ;//"输入捐献金额" ) ;
            m_ConfirmEdtData.Show ( m_Pnt.x, m_Pnt.y ) ;
            break ;
        case FACTIONDEL_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 20,
                                    m_Pnt.y + 10,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10360 ) ) ;
            CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    m_strText ) ;
            break ;
        case BOOTHOPEN_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10364 ) ) ;
            break ;
        case MPCCOURT_CONFIRM:
            if ( m_strText )
            {
                CMyBitmap::ShowString ( m_Pnt.x + m_nLeftLeft,
                                        m_Pnt.y + 10,
                                        0xffff00,
                                        m_strText ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 27,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10365 ) ) ;
                CMyBitmap::ShowString ( m_Pnt.x + 27,
                                        m_Pnt.y + 46,
                                        0xffff00,
                                        g_objGameDataSet.GetStr ( 10001) ) ;
            }
            break ;
        case IMPTITEM_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10371 ) ) ;//10371="确定卖出物品"
            break ;
        case DROPITEM_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10372 ) ) ;//"确定丢弃物品" ) ;
            break ;
        case TACKOFF_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 34,
                                    m_Pnt.y + 10,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10381 ) ) ;//"宝石将损坏" ) ;
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10380 ) ) ;//"确定取出宝石" ) ;
            break ;
        case JOINDICE_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10394 ) ) ;//"确定加入赌局" ) ;
            break ;
        case OUTDICE_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10395 ) ) ;//"确定退出赌局" ) ;
            break ;
        case ENEMYDEL_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10400 ) ) ;//"确定删除仇人" ) ;
            break ;
        case RESOURCE_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 10,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10405 ) ) ;
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10406 ) ) ;
            break ;
        case EXIT_CONFIRM:
            if ( m_TargetID == 0 )
            {
                CMyBitmap::ShowString ( m_Pnt.x + 41,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr( 10410 ) ) ;
            }
            else if ( m_TargetID == 1 )
            {
                CMyBitmap::ShowString ( m_Pnt.x + 27,
                                        m_Pnt.y + 28,
                                        0xffff00,
                                        g_objGameDataSet.GetStr( 10411 ) ) ;
            }
            break ;
        case EMPTYMSG_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr( 10415 ) ) ;
            break ;
        case WARNING_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr( m_TargetID ) ) ;
            break ;
        case ROLECREATE_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr( 10021 ) ) ;
            break ;
        case FRIENDADD_CONFIRM:
        case FRIENDADDSUC_CONFIRM:
        case ACTION_CONFIRM:
            ShowConfirmTextMLine();
            break ;
        case SPLITITEM_CONFIRM:
            CMyBitmap::ShowString( m_Pnt.x + 27,
                                   m_Pnt.y + 8,
                                   0xffff00,
                                   g_objGameDataSet.GetStr( 10027 ) ) ;
            m_ConfirmEdtData.Show( m_Pnt.x, m_Pnt.y ) ;
            break ;
        case MONSTERDROP_CONFIRM:
            CMyBitmap::ShowString( m_Pnt.x + 21,
                                   m_Pnt.y + 28,
                                   0xffff00,
                                   g_objGameDataSet.GetStr( 10693 ) ) ;
            break;
        case TEAMDISMISS_CONFIRM:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10708) ) ;//"确定解散队伍" ) ;
            break ;
        case CHECKOUT_MONSTEREGG:
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 10,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10685) ) ;//"若取出幻兽蛋" ) ;
            CMyBitmap::ShowString ( m_Pnt.x + 5,
                                    m_Pnt.y + 28,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10686) ) ;//"孵化时间将重新计算!" ) ;
            CMyBitmap::ShowString ( m_Pnt.x + 27,
                                    m_Pnt.y + 46,
                                    0xffff00,
                                    g_objGameDataSet.GetStr ( 10687) ) ;//"确定要取出吗?" ) ;
            break;
        default:
            break ;
        }
    }
}

BOOL CDlgConfirm::SetConfirmDlg(UINT uType, const char* strName, OBJID TargetID)
{
    m_ConfirmEdtData.EnableWindow ( false ) ;
    m_ConfirmEdtName.EnableWindow(false);
    m_uType = uType ;
    m_TargetID = TargetID ;
    CDlgConfirm::SetButtons(0);
    switch ( uType )
    {
    case TRADE_CONFIRM:
        {
            if ( strName )
            {
                strcpy ( m_strText, strName ) ;
                int len = strlen ( m_strText ) ;
                m_nLeftLeft = (int)(( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
            }
        }
        break ;
    case TEAMJOIN_CONFIRM:
        {
            if ( strName )
            {
                strcpy ( m_strText, strName ) ;
                int len = strlen ( m_strText ) ;
                m_nLeftLeft =(int)( ( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
            }
        }
        break ;
    case TEAMINVITE_CONFIRM:
        {
            if ( strName )
            {
                strcpy ( m_strText, strName ) ;
                int len = strlen ( m_strText ) ;
                m_nLeftLeft =(int)( ( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
            }
        }
        break ;
    case FACTIONJOIN_CONFIRM:
        {
            if ( strName )
            {
                strcpy ( m_strText, strName ) ;
                int len = strlen ( m_strText ) ;
                m_nLeftLeft =(int)( ( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
            }
        }
        break ;
    case FACTIONINVITE_CONFIRM:
        {
            if ( strName )
            {
                strcpy ( m_strText, g_objGameDataSet.GetStr ( 10011 ) ) ;//"帮派" ) ;
                strcat ( m_strText, strName ) ;
                int len = strlen ( m_strText ) ;
                m_nLeftLeft =(int)( ( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
            }
        }
        break ;
    case MONEYDROP_CONFIRM:
        {
            m_nSkip = 0 ;
            m_ConfirmEdtData.EnableWindow ( true ) ;
            m_ConfirmEdtData.SetEditBkID("Edit150");
            m_ConfirmEdtData.SetFocus () ;
        }
        break ;
    case FACTIONADDALLY_CONFIRM:
        {
            m_TargetID = TargetID;
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_TargetID);
            if (pPlayer)
            {
                sprintf( m_strText, g_objGameDataSet.GetStr(10855), pPlayer->GetName()) ;
            }
            if ( m_strText != NULL )
            {
                int len = strlen ( m_strText ) ;
                m_nLeftLeft = (int)(( ACTION_MSG_WIDTH * CMyBitmap::GetFontSize() / 2 - ( float )len * CMyBitmap::GetFontSize() / 2  - 86) / 2);
            }
        }
        break;
    case FACTIONADDENEMY_CONFIRM:
        {
            m_nSkip = 0;
            m_ConfirmEdtName.EnableWindow ( true ) ;
            m_ConfirmEdtName.SetEditBkID("Edit150");
            m_ConfirmEdtName.SetFocus () ;
        }
        break;
    case FACTIONSUB_CONFIRM:
        {
            m_nSkip = 0 ;
            m_ConfirmEdtData.EnableWindow ( true ) ;
            m_ConfirmEdtData.SetEditBkID("Edit150");
            m_ConfirmEdtData.SetFocus () ;
        }
        break ;
    case MPCBOOTHADD_CONFIRM:
        {
            m_nSkip = 0 ;
            m_ConfirmEdtData.EnableWindow ( true ) ;
            m_ConfirmEdtData.SetEditBkID("Edit150");
            m_ConfirmEdtData.SetFocus () ;
        }
        break ;
    case MPCBOOTHCHG_CONFIRM:
        {
            m_nSkip = 0 ;
            m_ConfirmEdtData.EnableWindow ( true ) ;
            m_ConfirmEdtData.SetEditBkID("Edit150");
            m_ConfirmEdtData.SetFocus () ;
        }
        break ;
    case FACTIONDEL_CONFIRM:
        {
            if ( strName )
            {
                strcpy ( m_strText, strName ) ;
                int len = strlen ( m_strText ) ;
                m_nLeftLeft =(int)( ( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
            }
        }
        break ;
    case BOOTHOPEN_CONFIRM:
        break ;
    case MPCCOURT_CONFIRM:
        if ( strName )
        {
            strcpy ( m_strText, strName ) ;
            int len = strlen ( m_strText ) ;
            m_nLeftLeft =(int)( ( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
        }
        break ;
    case IMPTITEM_CONFIRM:
        m_TargetID = TargetID;
        break;
    case DROPITEM_CONFIRM:
        m_TargetID = TargetID;
        break;
    case JOINDICE_CONFIRM:
        break ;
    case EXIT_CONFIRM:
        break ;
    case WARNING_CONFIRM:
        break ;
    case FRIENDADD_CONFIRM:
        if ( strName != NULL )
        {
            strcpy( m_strText, strName ) ;
        }
        break ;
    case FRIENDADDSUC_CONFIRM:
        if ( strName != NULL )
        {
            strcpy( m_strText, strName ) ;
        }
        break ;
    case ACTION_CONFIRM:
        if ( strName != NULL )
        {
            strcpy( m_strText, strName ) ;
            CDlgConfirm::SetButtons(1);
        }
        break ;
    case SPLITITEM_CONFIRM:
        {
            m_nSkip = 0 ;
            m_ConfirmEdtData.EnableWindow ( true ) ;
            m_ConfirmEdtData.SetEditBkID("Edit150");
            m_ConfirmEdtData.SetFocus () ;
        }
        break ;
    case TEACHER_CONFIRM:
        if ( strName )
        {
            strcpy ( m_strText, strName ) ;
            int len = strlen ( m_strText ) ;
            m_nLeftLeft = (int)(( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
        }
        break;
    case STUDENT_CONFIRM:
        if ( strName )
        {
            strcpy ( m_strText, strName ) ;
            int len = strlen ( m_strText ) ;
            m_nLeftLeft = (int)(( _DLG_WIDTH - ( float )len * CMyBitmap::GetFontSize() / 2 ) / 2);
        }
        break;
    }
    CDlgConfirm::EnableWindow() ;
    return true ;
}

void CDlgConfirm::OnChangeConfirmEdtData()
{
    char strTemp[12] ;
    m_ConfirmEdtData.GetWindowText ( strTemp, 12 ) ;
    if ( !strcmp ( strTemp, "0" ) )
    {
        m_ConfirmEdtData.SetWindowText ( NULL ) ;
        return ;
    }
    m_nSkip++ ;
    if ( m_nSkip % 2 == 0 )
    {
        return ;
    }
    int nLength = m_ConfirmEdtData.GetWindowTextLength () ;
    if ( nLength >= 3 )
    {
        char* pstrTemp = strchr ( strTemp, ',' ) ;
        if ( pstrTemp != NULL )
        {
            char strText[12] ;
            char strNum[12] ;
            strncpy ( strText, strTemp, pstrTemp - strTemp ) ;
            strText[pstrTemp - strTemp] = 0 ;
            strcat ( strText, pstrTemp + 1 ) ;
            pstrTemp = strchr ( strText, ',' ) ;
            if ( pstrTemp != NULL )
            {
                strncpy ( strTemp, strText, pstrTemp - strText ) ;
                strTemp[pstrTemp - strText] = 0 ;
                strcat ( strTemp, pstrTemp + 1 ) ;
                strcpy ( strNum, strTemp ) ;
            }
            else
            {
                strcpy ( strNum, strText ) ;
            }
            if ( strNum != NULL )
            {
                m_nEdtNum = atoi ( strNum ) ;
                int nlen = strlen ( strNum ) ;
                CString cstrText = strNum;
                if ( nlen > 3 && nlen < 7 )
                {
                    cstrText.Insert ( nlen - 3, ',' ) ;
                }
                else if ( nlen >= 7 )
                {
                    cstrText.Insert ( nlen - 6, ',' ) ;
                    cstrText.Insert ( nlen - 2, ',' ) ;
                }
                m_ConfirmEdtData.SetWindowText ( cstrText ) ;
                m_ConfirmEdtData.SetSel ( 12, 12 ) ;
            }
        }
        else if ( strlen ( strTemp ) == 4 )
        {
            m_nEdtNum = atoi ( strTemp ) ;
            CString cstrText = strTemp;
            cstrText.Insert ( 1, ',' ) ;
            m_ConfirmEdtData.SetWindowText ( cstrText ) ;
            m_ConfirmEdtData.SetSel ( 5, 5 ) ;
        }
        else
        {
            m_nEdtNum = atoi ( strTemp ) ;
            m_nSkip++ ;
        }
    }
    else
    {
        m_nSkip = 0 ;
        m_nEdtNum = atoi ( strTemp ) ;
    }
}

void CDlgConfirm::SetButtons(int nMode)
{
    switch(nMode)
    {
    case 0:
        m_ConfirmBtnCancel.EnableWindow();
        m_ConfirmBtnOk.EnableWindow();
        break;
    case 1:
        m_ConfirmBtnCancel.EnableWindow(false);
        m_ConfirmBtnOk.EnableWindow();
        break;
    default:
        break;
    }
}

void CDlgConfirm::ShowConfirmTextMLine()
{
    int nDescLine = 0 ;
    int nCharIndex[32] = {0};
    nCharIndex[0] = 0;
    int nSingle = 0;
    int nLineNum = 0;
    int i = 0;
    for ( ; i < (int)strlen ( m_strText ); i++ )
    {
        if ( CMyBitmap::IsDBCSLeadByte( m_strText, i ) )
        {
            nLineNum += 2;
            i++;
        }
        else
        {
            nSingle++;
            nLineNum++;
        }
        if ( nLineNum >= ACTION_MSG_WIDTH )
        {
            //----------Begin the english version's process(jump line)----------//
            if ( strcmp( g_objGameDataSet.GetVersionInfo(), "English" ) == 0 )
            {
                // Search the last word
                for ( int j = i; j >= 0; j-- )
                {
                    if ( m_strText[j] == ' ' )
                    {
                        i -= i - j;
                        break;
                    }
                }
            }
            //----------End the english version's process(jump line)----------//
            if ( nSingle % 2 == 0 )
            {
                nSingle = 0;
                nLineNum = 0;
                nCharIndex[nDescLine] = i + 1;
                nDescLine++;
            }
            else
            {
                i -= nLineNum - 2;
                nSingle = 0;
                nLineNum = 0;
                nCharIndex[nDescLine] = i - ( nLineNum - ACTION_MSG_WIDTH );
                nDescLine++;
            }
        }
    }
    if ( nLineNum < ACTION_MSG_WIDTH && nLineNum != 0 )
    {
        nDescLine++ ;
        nCharIndex[nDescLine] = i;
    }
    DWORD STRCOLOR_YELLOW = 0xffff00;
    CMySize psizeFont;
    CMyBitmap::GetFontSize(psizeFont);
    char strTemp[256] ;
    int nOffset = 0;
    for (int i = 0; i < nDescLine; i++ )
    {
        strncpy ( strTemp, m_strText + nCharIndex[i], nCharIndex[i + 1] - nCharIndex[i] );
        strTemp[nCharIndex[i + 1] - nCharIndex[i]] = '\0';
        if (i == 0)
        {
            CMyBitmap::ShowString ( m_Pnt.x - 86,
                                    m_Pnt.y + 13 + nOffset,
                                    0xffff00,
                                    strTemp ) ;
        }
        else
            CMyBitmap::ShowString ( m_Pnt.x - 104,
                                    m_Pnt.y + 13 + nOffset,
                                    0xffff00,
                                    strTemp ) ;
        nOffset += psizeFont.iHeight + 2;
    }
}
