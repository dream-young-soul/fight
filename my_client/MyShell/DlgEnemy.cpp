
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgEnemy.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgEnemy.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnemy dialog


CDlgEnemy::CDlgEnemy(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEnemy::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEnemy)
    m_Pnt = CPoint( 0, 0);
    m_bShow = false ;
    m_dwLastShowTime = 0;
    m_fFrameStep = 0.0f ;
    m_nFrameMode = 0 ;
    m_nPntX = 0 ;
    m_nCurrentQueryIndex = -1 ;
    m_iShowMode = 0;
    m_nTsStatus = 0;
    //}}AFX_DATA_INIT
}


void CDlgEnemy::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEnemy)
    DDX_Control(pDX, IDC_BTN_TEACHER, m_TeacherBtn);
    DDX_Control(pDX, IDC_BTN_DELSTUDENT, m_DelStuBtn);
    DDX_Control(pDX, IDC_ENEMY_BTN_TEACHER, m_TeacherListChk);
    DDX_Control(pDX, IDC_ENEMY_BTN_ENEMY, m_EnemyListChk);
    DDX_Control(pDX, IDC_ENEMY_BTN_FRIEND, m_FriendListChk);
    DDX_Control(pDX, IDC_ENEMY_BTN_ADD, m_AddBtn);
    DDX_Control(pDX, IDC_ENEMY_LST_DATA, m_EnemyLstData);
    DDX_Control(pDX, IDC_ENEMY_BTN_CLOSE, m_EnemyBtnClose);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnemy, CDialog)
    //{{AFX_MSG_MAP(CDlgEnemy)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_ENEMY_BTN_CLOSE, OnEnemyBtnClose)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_ENEMY_BTN_FRIEND, OnEnemyBtnFriend)
    ON_BN_CLICKED(IDC_ENEMY_BTN_ADD, OnEnemyBtnAdd)
    ON_BN_CLICKED(IDC_ENEMY_BTN_ENEMY, OnEnemyBtnEnemy)
    ON_BN_CLICKED(IDC_ENEMY_BTN_TEACHER, OnEnemyBtnTeacher)
    ON_BN_CLICKED(IDC_BTN_TEACHER, OnBtnTeacher)
    ON_BN_CLICKED(IDC_BTN_DELSTUDENT, OnBtnDelstudent)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnemy message handlers

LRESULT CDlgEnemy::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if ( wParam )
        {
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            m_nPntX = _SCR_WIDTH;
            DXPlaySound ( "Sound/Open.wav" ) ;
            m_EnemyListChk.SetCheckValue(0);
            m_TeacherListChk.SetCheckValue(0);
            m_FriendListChk.SetCheckValue(1);
            m_iShowMode = 0;
            m_AddBtn.EnableWindow();
            m_DlgEnmState.m_iShowMode = m_iShowMode;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
            if ( m_DlgEnmState.m_bShow )
            {
                m_DlgEnmState.EnableWindow(false) ;
            }
            if (m_DlgSchoolList.m_bShow)
            {
                m_DlgSchoolList.EnableWindow(false) ;
            }
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgEnemy::OnEnemyBtnClose () ;
            return true ;
        }
        break ;
    case LB_SETSEL:
        {
            if ( wParam == 0 )
                // Begin the listbox sel change
            {
                if (m_iShowMode == 0)
                {
                    char strEnemyName[16] ;
                    m_nCurrentQueryIndex = lParam ;
                    m_EnemyLstData.GetText ( lParam, strEnemyName ) ;
                    CFriend* pFriend = g_objHero.GetFriendByName ( strEnemyName ) ;
                    if ( pFriend )
                    {
                        if (m_DlgSchoolList.m_bShow)
                        {
                            m_DlgSchoolList.EnableWindow(false);
                        }
                        g_objHero.QueryFriend ( pFriend->GetID() ) ;
                    }
                }
                else if (m_iShowMode == 1)
                {
                    char strEnemyName[16] ;
                    m_nCurrentQueryIndex = lParam ;
                    m_EnemyLstData.GetText ( lParam, strEnemyName ) ;
                    CEnemy* pEnemy = g_objHero.GetEnemyByName ( strEnemyName ) ;
                    if ( pEnemy )
                    {
                        if (m_DlgSchoolList.m_bShow)
                        {
                            m_DlgSchoolList.EnableWindow(false);
                        }
                        g_objHero.QueryEnemy ( pEnemy->GetID() ) ;
                    }
                }
                else if (m_iShowMode == 2)
                {
                    char strStuName[16] ;
                    m_nCurrentQueryIndex = lParam ;
                    m_EnemyLstData.GetText(lParam, strStuName);
                    CSchoolMember* pSchoolMember = g_objHero.GetSchoolMemberByName(strStuName);
                    if (pSchoolMember)
                    {
                        if (m_DlgSchoolList.m_bShow)
                        {
                            m_DlgSchoolList.EnableWindow(false);
                        }
                        m_DlgEnmState.m_iShowMode = 2;
                        m_DlgEnmState.EnableWindow(TRUE);
                        m_DlgEnmState.SetStuInfo(strStuName);
                        if (pSchoolMember->GetRelation() == RELATION_TEACHER)
                        {
                            m_nTsStatus = 2;    //sel teacher
                        }
                        else
                        {
                            m_nTsStatus = 3;    //sel student
                        }
                        CDlgEnemy::SetTeacherBtn();
                    }
                }
            }
            // End the listbox sel change
            else if ( wParam == -1 )
            {
                if ( lParam != -1 )
                {
                    if ( m_DlgEnmState.m_bShow )
                    {
                        m_DlgEnmState.EnableWindow(false) ;
                    }
                    if (m_DlgSchoolList.m_bShow)
                    {
                        m_DlgSchoolList.EnableWindow(false);
                        m_nTsStatus = 4; //sel student
                        CDlgEnemy::SetTeacherBtn();
                    }
                    if (m_iShowMode == 2)
                    {
                        char strStuName[16] ;
                        m_nCurrentQueryIndex = lParam ;
                        m_EnemyLstData.GetText(lParam, strStuName);
                        CSchoolMember* pSchoolMember = g_objHero.GetSchoolMemberByName(strStuName);
                        if (pSchoolMember)
                        {
                            if (pSchoolMember->GetRelation() == RELATION_TEACHER)
                            {
                                m_nTsStatus = 2;    //sel teacher
                            }
                            CDlgEnemy::SetTeacherBtn();
                        }
                    }
                }
            }
            else if (wParam == 1)
            {
                if (lParam != -1)
                {
                    if (m_iShowMode == 0)
                    {
                        m_nCurrentQueryIndex = lParam ;
                        CDlgEnemy::GetParent()->PostMessage(WM_MY_MESSAGE, ON_FRIEND_CHAT, lParam);
                    }
                }
            }
            else if (wParam == 2)
            {
                if (lParam != -1)
                {
                    if (m_iShowMode == 0)
                    {
                        m_nCurrentQueryIndex = lParam;
                        char strName[16];
                        m_EnemyLstData.GetText(m_nCurrentQueryIndex, strName);
                        if (m_DlgSchoolList.m_bShow)
                        {
                            m_DlgSchoolList.EnableWindow(false);
                        }
                        if (m_DlgEnmState.m_bShow)
                        {
                            m_DlgEnmState.EnableWindow(false);
                        }
                        if (g_objGameMsg.GetLeaveWordAmountByName(strName) > 0)
                        {
                            m_EnemyLstData.SetItemFlash(m_nCurrentQueryIndex, FALSE);
                            CDlgEnemy::GetParent()->PostMessage(WM_MY_MESSAGE, ON_MSGSEND_VIEW, lParam);
                        }
                        else
                        {
                            CDlgEnemy::GetParent()->PostMessage(WM_MY_MESSAGE, ON_FRIEND_LEAVE, lParam);
                        }
                    }
                }
            }
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgEnemy::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgEnemy::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgEnemy::GetWindowRect(rect);
    m_fFrameStep = ( float ) (rect.Width() + 100) / DLG_FRAME_AMOUNT ;
    // Init the button
    m_EnemyBtnClose.Init ( rect.left, rect.top, "Button540" ) ;
    m_BtnHelp.Init ( rect.left, rect.top, "Button4" ) ;
    m_DelStuBtn.Init(rect.left, rect.top, "Button179");
    m_DelStuBtn.EnableWindow(false);
    m_TeacherBtn.Init(rect.left, rect.top, "Button176");
    m_TeacherBtn.EnableWindow(false);
    m_AddBtn.Init ( rect.left, rect.top, "Button170");
    m_FriendListChk.Init(rect.left, rect.top, "Button174", 2);
    m_EnemyListChk.Init(rect.left, rect.top, "Button173", 2);
    m_TeacherListChk.Init(rect.left, rect.top, "Button175", 2);
    m_FriendListChk.SetCheckValue(1);
    // Init the list
    if ( ! m_EnemyLstData.Init ( rect.left, rect.top, NULL, 2 ) )
    {
        return false ;
    }
    m_EnemyLstData.SetFontColor ( 0xffff00 ) ;
    m_EnemyLstData.SetParentCob ( this ) ;
    // Init the Enemy state dialog
    m_DlgEnmState.Create ( IDD_DIALOG_ENMSTATE, CDlgEnemy::GetParent () ) ;
    CRect Temprect ;
    m_DlgEnmState.GetWindowRect ( Temprect ) ;
    m_DlgEnmState.MoveWindow ( _SCR_WIDTH - rect.Width () -  Temprect.Width() - 105,
                               450 - rect.Height(),
                               Temprect.Width (),
                               Temprect.Height () ) ;
    m_DlgEnmState.ShowWindow ( SW_SHOW);
    m_DlgEnmState.EnableWindow(false);
    m_DlgSchoolList.Create(IDD_DIALOG_SCHOOLLIST, CDlgEnemy::GetParent());
    m_DlgSchoolList.GetWindowRect(Temprect);
    m_DlgSchoolList.MoveWindow( _SCR_WIDTH - rect.Width() - Temprect.Width() - 105,
                                450 - rect.Height(),
                                Temprect.Width(),
                                Temprect.Height());
    m_DlgSchoolList.ShowWindow(SW_SHOW);
    m_DlgSchoolList.EnableWindow(false);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnemy::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_nPntX = x ;
    m_Pnt.y = y ;
}

void CDlgEnemy::Show()
{
    if ( m_bShow )
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x = (LONG)(m_nPntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep);
            }
            else
            {
                CRect rect;
                CDlgEnemy::GetWindowRect(rect);
                m_Pnt.x = _SCR_WIDTH - rect.Width() - 100;
                m_nFrameMode = 0 ;
            }
        }
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog46",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( m_iShowMode,
                            m_Pnt.x,
                            m_Pnt.y ) ;
        }
        else
        {
            return ;
        }
        // Show the list
        m_EnemyLstData.Show ( m_Pnt.x, m_Pnt.y ) ;
        // Show the button
        m_EnemyBtnClose.Show ( m_Pnt.x, m_Pnt.y ) ;
        m_BtnHelp.Show ( m_Pnt.x, m_Pnt.y ) ;
        if (m_iShowMode == 0)
        {
            m_AddBtn.Show ( m_Pnt.x, m_Pnt.y );
        }
        else if (m_iShowMode == 2)
        {
            if (m_TeacherBtn.IsWindowEnabled())
            {
                m_TeacherBtn.Show(m_Pnt.x, m_Pnt.y);
            }
        }
        if (m_DelStuBtn.IsWindowEnabled())
        {
            m_DelStuBtn.Show(m_Pnt.x, m_Pnt.y);
        }
        m_FriendListChk.Show (m_Pnt.x, m_Pnt.y);
        m_EnemyListChk.Show (m_Pnt.x, m_Pnt.y);
        m_TeacherListChk.Show (m_Pnt.x, m_Pnt.y);
        // Show the Enemy state dialog
        if ( m_DlgEnmState.m_bShow )
        {
            m_DlgEnmState.Show() ;
        }
        if ( m_DlgSchoolList.m_bShow)
        {
            m_DlgSchoolList.Show();
        }
    }
}

void CDlgEnemy::OnEnemyBtnClose()
{
    DXPlaySound("Sound/window_close.wav");
    CDlgEnemy::EnableWindow(false) ;
}

void CDlgEnemy::OnBtnHelp()
{
    CDlgEnemy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_HELPDLG_SHOW, DLG_FRIEND ) ;
}

void CDlgEnemy::SetEnemyList()
{
    m_DelStuBtn.ChangeImage("Button171");
    m_DelStuBtn.EnableWindow(true);
    CRect rect;
    CDlgEnemy::GetWindowRect(rect);
    CRect rectB;
    m_DelStuBtn.GetWindowRect(rectB);
    m_DelStuBtn.MoveWindow(70, 295, rectB.Width(), rectB.Height(), false);
    m_DelStuBtn.ReInit(rect.left, rect.top );
    m_EnemyLstData.ResetContent ( ) ;
    int nAmount = g_objHero.GetEnemyAmount () ;
    // Test the online Enemy num
    int nOnline = 0 ;
    for (int i = 0; i < nAmount; i++)
    {
        CEnemy* pEnemy = g_objHero.GetEnemyByIndex ( i ) ;
        if ( pEnemy && pEnemy->IsOnline () )
        {
            nOnline++ ;
        }
    }
    int nOffLine = nOnline ;
    for (int i = 0; i < nAmount; i++)
    {
        CEnemy* pEnemy	= g_objHero.GetEnemyByIndex(i);
        if (pEnemy)
        {
            char strListData[64] ;
            strcpy ( strListData, pEnemy->GetName () ) ;
            if ( pEnemy->IsOnline () )
            {
                m_EnemyLstData.InsertString ( 0, strListData ) ;
                m_EnemyLstData.SetListRealIndex ( nOnline - 1, 1 ) ;
                nOnline-- ;
            }
            else
            {
                m_EnemyLstData.AddString(strListData);
                m_EnemyLstData.SetListRealIndex ( nOffLine, -1 ) ;
                nOffLine++ ;
            }
        }
    }
}


void CDlgEnemy::SetEnemyState()
{
    char strEnemyName[16] ;
    m_EnemyLstData.GetText ( m_nCurrentQueryIndex, strEnemyName ) ;
    if ( strEnemyName != NULL )
    {
        if (m_DlgSchoolList.m_bShow)
        {
            m_DlgSchoolList.EnableWindow(false);
        }
        if ( !m_DlgEnmState.m_bShow )
        {
            m_DlgEnmState.EnableWindow( ) ;
        }
        m_DlgEnmState.SetEnemyState ( strEnemyName , m_iShowMode) ;
    }
}



void CDlgEnemy::OnEnemyBtnFriend()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_FriendListChk.GetCheckValue() == 1)
    {
        PopCurListChk();
        m_iShowMode = 0;
        m_AddBtn.EnableWindow();
        m_DlgEnmState.m_iShowMode = m_iShowMode;
        SetFriendList();
        m_DlgEnmState.EnableWindow(false);
        m_nCurrentQueryIndex = -1;
    }
    else
    {
        m_FriendListChk.SetCheckValue(1);
    }
}

void CDlgEnemy::SetFriendList()
{
    m_DelStuBtn.ChangeImage("Button171");
    m_DelStuBtn.EnableWindow(true);
    CRect rect;
    CDlgEnemy::GetWindowRect(rect);
    CRect rectB;
    m_DelStuBtn.GetWindowRect(rectB);
    m_DelStuBtn.MoveWindow(108, 294, rectB.Width(), rectB.Height(), false);
    m_DelStuBtn.ReInit(rect.left, rect.top );
    m_EnemyLstData.ResetContent ( ) ;
    int nAmount = g_objHero.GetFriendAmount () ;
    // Test the online Friend num
    int nOnline = 0 ;
    for (int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = g_objHero.GetFriendByIndex ( i ) ;
        if ( pFriend && pFriend->IsOnline () )
        {
            nOnline++ ;
        }
    }
    int nOffLine = nOnline ;
    for (int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend	= g_objHero.GetFriendByIndex(i);
        if (pFriend)
        {
            char strListData[64] ;
            strcpy ( strListData, pFriend->GetName () ) ;
            if ( pFriend->IsOnline () )
            {
                m_EnemyLstData.InsertString ( 0, strListData ) ;
                m_EnemyLstData.SetListRealIndex ( nOnline - 1, 1 ) ;
                nOnline-- ;
            }
            else
            {
                m_EnemyLstData.AddString(strListData);
                if (g_objGameMsg.GetLeaveWordAmountByName(pFriend->GetName()) > 0)
                {
                    m_EnemyLstData.SetItemFlash(nOffLine, TRUE);
                }
                else
                {
                    m_EnemyLstData.SetItemFlash(nOffLine, FALSE);
                }
                m_EnemyLstData.SetListRealIndex ( nOffLine, -1 ) ;
                nOffLine++ ;
            }
        }
    }
}

void CDlgEnemy::SetFriendState()
{
    char strEnemyName[16] ;
    m_EnemyLstData.GetText ( m_nCurrentQueryIndex, strEnemyName ) ;
    if ( strEnemyName != NULL )
    {
        if (m_DlgSchoolList.m_bShow)
        {
            m_DlgSchoolList.EnableWindow(false);
        }
        if ( !m_DlgEnmState.m_bShow )
        {
            m_DlgEnmState.EnableWindow() ;
        }
        m_DlgEnmState.SetEnemyState ( strEnemyName , m_iShowMode) ;
    }
}

void CDlgEnemy::OnEnemyBtnAdd()
{
    // TODO: Add your control notification handler code here
    if (m_iShowMode == 0)
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        CDlgEnemy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_FRIEND_ADD ) ;
    }
}

void CDlgEnemy::SetTsList()
{
    CRect rect;
    CDlgEnemy::GetWindowRect(rect);
    CRect rectB;
    m_DelStuBtn.GetWindowRect(rectB);
    m_DelStuBtn.MoveWindow(108, 294, rectB.Width(), rectB.Height(), false);
    m_DelStuBtn.ReInit(rect.left, rect.top );
    m_EnemyLstData.ResetContent ( ) ;
    int nAmount = g_objHero.GetSchoolMemberAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CSchoolMember* pSchoolMember = g_objHero.GetSchoolMemberByIndex(i);
        if (pSchoolMember)
        {
            char strListData[64] ;
            strcpy ( strListData, pSchoolMember->GetName () ) ;
            if (pSchoolMember->GetRelation() == RELATION_TEACHER)
            {
                m_EnemyLstData.InsertString(0, strListData);
                if (pSchoolMember->GetStatus())
                {
                    m_EnemyLstData.SetListRealIndex(0, 1);
                }
                else
                {
                    m_EnemyLstData.SetListRealIndex(0, -1);
                }
            }
            else if (pSchoolMember->GetRelation() == RELATION_STUDENT)
            {
                m_EnemyLstData.AddString(strListData);
                if (pSchoolMember->GetStatus())
                {
                    m_EnemyLstData.SetListRealIndex ( i, 1 ) ;
                }
                else
                {
                    m_EnemyLstData.SetListRealIndex( i, -1);
                }
            }
        }
    }
    SetTeacherStatus();
    SetTeacherBtn();
}

void CDlgEnemy::OnEnemyBtnEnemy()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_EnemyListChk.GetCheckValue() == 1)
    {
        PopCurListChk();
        m_iShowMode = 1;
        m_DlgEnmState.m_iShowMode = m_iShowMode;
        SetEnemyList();
        m_DlgEnmState.EnableWindow(false);
        m_nCurrentQueryIndex = -1;
    }
    else
    {
        m_EnemyListChk.SetCheckValue(1);
    }
}

void CDlgEnemy::PopCurListChk()
{
    DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
    switch(m_iShowMode)
    {
    case 0:
        m_FriendListChk.SetCheckValue(0);
        m_AddBtn.EnableWindow(false);
        break;
    case 1:
        m_EnemyListChk.SetCheckValue(0);
        break;
    case 2:
        m_TeacherListChk.SetCheckValue(0);
        m_DelStuBtn.EnableWindow(false);
        m_TeacherBtn.EnableWindow(false);
        break;
    default:
        break;
    }
}

void CDlgEnemy::OnEnemyBtnTeacher()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_TeacherListChk.GetCheckValue() == 1)
    {
        PopCurListChk();
        m_AddBtn.EnableWindow(false);
        m_TeacherBtn.EnableWindow(true);
        m_DelStuBtn.EnableWindow(true);
        m_iShowMode = 2;
        m_DlgEnmState.EnableWindow(false);
        SetTsList();
        m_nCurrentQueryIndex = -1;
    }
    else
    {
        m_TeacherListChk.SetCheckValue(1);
    }
}

void CDlgEnemy::SetTeacherStatus()
{
    if (!g_objHero.IsHaveTeacher())
    {
        m_nTsStatus = 0;
    }
    else
    {
        m_nTsStatus = 1;
    }
}

void CDlgEnemy::OnBtnTeacher()
{
    // TODO: Add your control notification handler code here
    switch(m_nTsStatus)
    {
    case 0://no teacher
        {
            DXPlaySound ( "Sound/Get_Teacher.wav" ) ;
            if (g_objHero.GetLev () >= 40 )
            {
                g_objGameMsg.AddMsg (g_objGameDataSet.GetStr (10774));
                return;
            }
            CDlgEnemy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_TEACHER_REQUEST );
        }
        break;
    case 1://have teacher
        {
            g_objGameMsg.AddMsg (g_objGameDataSet.GetStr(10770));
        }
        break;
    case 2://sel teacher
        {
            DXPlaySound("Sound/Dlg_Ok.wav");
            g_objHero.LeaveTeacher();
            if (m_DlgEnmState.m_bShow)
            {
                m_DlgEnmState.EnableWindow (false);
            }
        }
        break;
    case 3://sel student
        {
            //打开二级学员列表
            DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
            char strName[16] = "";
            m_EnemyLstData.GetText(m_nCurrentQueryIndex, strName);
            if ( strName && strlen(strName) > 0)
            {
                CSchoolMember* pSchoolMember = g_objHero.GetSchoolMemberByName(strName);
                if (pSchoolMember && pSchoolMember->GetRelation() == RELATION_STUDENT)
                {
                    g_objHero.QuerySecSchoolMember(pSchoolMember->GetID());
                    if (!m_DlgSchoolList.m_bShow)
                    {
                        if (m_DlgEnmState.m_bShow)
                        {
                            m_DlgEnmState.EnableWindow(false);
                        }
                        m_DlgSchoolList.EnableWindow(true);
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}

void CDlgEnemy::SetTeacherBtn()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    switch(m_nTsStatus)
    {
    case 0:    //no teacher
        m_TeacherBtn.EnableWindow(true);
        m_TeacherBtn.ChangeImage("Button176");//拜师
        m_DelStuBtn.EnableWindow(true);
        m_DelStuBtn.ChangeImage("Button179");  //招收
        break;
    case 1:    // have teacher can get student
        m_TeacherBtn.EnableWindow(false);
        m_DelStuBtn.EnableWindow(true);
        m_DelStuBtn.ChangeImage("Button179");  //招收
        break;
    case 2:    // sel the teacher
        m_TeacherBtn.EnableWindow(true);
        m_TeacherBtn.ChangeImage("Button177"); //判师
        m_DelStuBtn.EnableWindow(false);
        break;
    case 3:	   // sel a student
        m_TeacherBtn.EnableWindow(true);
        m_TeacherBtn.ChangeImage("Button1791");	//二级学员
        m_DelStuBtn.EnableWindow(true);
        m_DelStuBtn.ChangeImage("Button178"); //开除
        break;
    case 4:
        m_TeacherBtn.EnableWindow(false);
        m_DelStuBtn.EnableWindow(true);
        m_DelStuBtn.ChangeImage("Button178"); //开除
    default:
        break;
    }
}

void CDlgEnemy::OnBtnDelstudent()
{
    // TODO: Add your control notification handler code here
    if (m_iShowMode == 0)
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        if (m_DlgEnmState.m_bShow)
        {
            m_DlgEnmState.EnableWindow (false);
        }
        if (m_nCurrentQueryIndex == -1)
        {
            return;
        }
        char strName[16] = "";
        m_EnemyLstData.GetText(m_nCurrentQueryIndex, strName);
        if ( strName && strlen(strName) > 0)
        {
            CDlgEnemy::GetParent()->PostMessage ( WM_MY_MESSAGE, ON_FRDSTATE_DEL);
        }
    }
    else if (m_iShowMode == 1)
    {
        DXPlaySound ( "Sound/Dlg_Ok.wav" ) ;
        if (m_DlgEnmState.m_bShow)
        {
            m_DlgEnmState.EnableWindow (false);
        }
        if (m_nCurrentQueryIndex == -1)
        {
            return;
        }
        char strName[16] = "";
        m_EnemyLstData.GetText(m_nCurrentQueryIndex, strName);
        if ( strName && strlen(strName) > 0)
        {
            CDlgEnemy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ENMSTATE_DEL ) ;
        }
    }
    else if (m_iShowMode == 2)
    {
        switch(m_nTsStatus)
        {
        case 0: //no teacher
        case 1: //have teacher can get student
            {
                DXPlaySound ( "Sound/Get_Student.wav" ) ;
                if (g_objHero.GetLev () < 50)
                {
                    g_objGameMsg.AddMsg (g_objGameDataSet.GetStr(10771));
                    return;
                }
                if (g_objHero.GetTutorExp() < 10)
                {
                    g_objGameMsg.AddMsg (g_objGameDataSet.GetStr (10772));
                    return;
                }
                if (g_objHero.GetTutorLevel () == 1 && g_objHero.GetSchoolMemberAmount () >= 5)
                {
                    g_objGameMsg.AddMsg (g_objGameDataSet.GetStr (10776));
                    return;
                }
                if (g_objHero.GetTutorLevel () == 2 && g_objHero.GetSchoolMemberAmount () >= 10 )
                {
                    g_objGameMsg.AddMsg (g_objGameDataSet.GetStr (10777));
                    return;
                }
                if (g_objHero.GetTutorLevel () == 3 && g_objHero.GetSchoolMemberAmount () >= 15 )
                {
                    g_objGameMsg.AddMsg (g_objGameDataSet.GetStr (10778));
                    return;
                }
                g_objGameMsg.AddMsg (g_objGameDataSet.GetStr(10773));
                CDlgEnemy::GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_STUDENT_REQUEST );
            }
            break;
        case 2: //sel teacher
            break;
        case 3: //sel student
            {
                if (m_DlgEnmState.m_bShow)
                {
                    m_DlgEnmState.EnableWindow (false);
                }
                if (m_nCurrentQueryIndex == -1)
                {
                    return;
                }
                char strName[16] = "";
                m_EnemyLstData.GetText(m_nCurrentQueryIndex, strName);
                DXPlaySound ( "Sound/Del_Student.wav" ) ;
                if ( strName && strlen(strName) > 0)
                {
                    CSchoolMember* pSchoolMember = g_objHero.GetSchoolMemberByName(strName);
                    if (pSchoolMember && pSchoolMember->GetRelation() == RELATION_STUDENT)
                    {
                        g_objHero.DismissStudent(pSchoolMember->GetID());
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

void CDlgEnemy::DeleteEnemy()
{
    char strName[16] = "";
    m_EnemyLstData.GetText(m_nCurrentQueryIndex, strName);
    if ( strName && strlen(strName) > 0)
    {
        CEnemy* pEnemy = g_objHero.GetEnemyByName ( strName ) ;
        if ( pEnemy )
        {
            g_objHero.DelEnemy ( pEnemy->GetID () ) ;
        }
    }
}

void CDlgEnemy::DeleteFriend()
{
    char strName[16] = "";
    m_EnemyLstData.GetText(m_nCurrentQueryIndex, strName);
    if ( strName && strlen(strName) > 0)
    {
        CFriend* pFriend = g_objHero.GetFriendByName ( strName ) ;
        if ( pFriend )
        {
            g_objHero.BreakOffFriend ( pFriend->GetID () ) ;
        }
    }
}
