
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgKnightageEnemy.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgKnightageEnemy.h"

#include "Hero.h"
#include "GamePlayerset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageEnemy dialog


CDlgKnightageEnemy::CDlgKnightageEnemy(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgKnightageEnemy::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgKnightageEnemy)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;
    m_ListMode = 0;
    //}}AFX_DATA_INIT
}


void CDlgKnightageEnemy::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgKnightageEnemy)
    DDX_Control(pDX, IDC_CHK_FRIENDLIST, m_ChkFriendList);
    DDX_Control(pDX, IDC_CHK_ENEMYLIST, m_ChkEnemyList);
    DDX_Control(pDX, IDC_BTN_DELENEMY, m_BtnDelEnemy);
    DDX_Control(pDX, IDC_BTN_ADDENEMY, m_BtnAddEnemy);
    DDX_Control(pDX, IDC_LIST_ENEMY, m_EnemyList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKnightageEnemy, CDialog)
    //{{AFX_MSG_MAP(CDlgKnightageEnemy)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_WM_LBUTTONDBLCLK()
    ON_BN_CLICKED(IDC_BTN_ADDENEMY, OnBtnAddenemy)
    ON_BN_CLICKED(IDC_BTN_DELENEMY, OnBtnDelenemy)
    ON_BN_CLICKED(IDC_CHK_ENEMYLIST, OnChkEnemylist)
    ON_BN_CLICKED(IDC_CHK_FRIENDLIST, OnChkFriendlist)
    ON_LBN_SETFOCUS(IDC_LIST_ENEMY, OnSetfocusListEnemy)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageEnemy message handlers

LRESULT CDlgKnightageEnemy::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow)
        {
            m_BtnAddEnemy.EnableWindow(false);
            m_BtnDelEnemy.EnableWindow(false);
            if (m_ListMode == 0)
            {
                m_ChkEnemyList.SetCheckValue(0);
                m_ChkFriendList.SetCheckValue(1);
                SetFriendList();
            }
            else
            {
                m_ChkEnemyList.SetCheckValue(1);
                m_ChkFriendList.SetCheckValue(0);
                SetEnemyList();
            }
        }
        else
        {
            if (m_DlgKnightageState.m_bShow)
            {
                m_DlgKnightageState.EnableWindow(false);
            }
        }
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1)
        {
            return true;
        }
        else if (wParam == 2)
        {
            return true;
        }
    }
    else if (message == LB_SETSEL)
    {
        if (wParam == 0)
        {
            if (lParam != -1)
            {
                m_nCurSelIndex = lParam;
                char szSynName[_MAX_NAMESIZE] = "";
                m_EnemyList.GetText(m_nCurSelIndex, szSynName);
                g_objHero.QueryOtherSynicateInfo(szSynName);
            }
        }
        else if (wParam == -1)
        {
            if (lParam != -1)
            {
                if (m_DlgKnightageState.m_bShow)
                {
                    m_DlgKnightageState.EnableWindow(FALSE);
                }
            }
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgKnightageEnemy::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgKnightageEnemy::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgKnightageEnemy::GetWindowRect(rect);
    // TODO: Add extra initialization here
    m_EnemyList.Init(rect.left, rect.top, NULL, 2);
    m_EnemyList.SetParentCob ( this ) ;
    m_ChkEnemyList.Init(rect.left, rect.top, "Button827", 2);
    m_ChkFriendList.Init(rect.left, rect.top, "Button826", 2);
    m_BtnAddEnemy.Init(rect.left, rect.top, "Button822");
    m_BtnDelEnemy.Init(rect.left, rect.top, "Button823");
    m_DlgKnightageState.Create(IDD_DIALOG_KNIGHTAGE_STATE, CDlgKnightageEnemy::GetParent()->GetParent());
    CRect Srect;
    m_DlgKnightageState.GetWindowRect(Srect);
    m_DlgKnightageState.MoveWindow(_SCR_WIDTH - rect.Width() -  Srect.Width() - 100,
                                   450 - rect.Height(),
                                   Srect.Width(), Srect.Height());
    m_DlgKnightageState.ShowWindow(SW_SHOW);
    m_DlgKnightageState.EnableWindow(false);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKnightageEnemy::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgKnightageEnemy::Show(int x, int y)
{
    if (m_bShow)
    {
        m_EnemyList.Show(m_Pnt.x + x, m_Pnt.y + y);
        if (m_BtnAddEnemy.IsWindowEnabled())
        {
            m_BtnAddEnemy.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnDelEnemy.IsWindowEnabled())
        {
            m_BtnDelEnemy.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        m_ChkFriendList.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_ChkEnemyList.Show(m_Pnt.x + x, m_Pnt.y + y);
        if (m_DlgKnightageState.m_bShow)
        {
            m_DlgKnightageState.Show();
        }
    }
}



void CDlgKnightageEnemy::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnLButtonDblClk(nFlags, point);
}

void CDlgKnightageEnemy::SetEnemyList()
{
    if (m_ListMode != 1)
    {
        return;
    }
    m_EnemyList.ResetContent();
    int nAmount = g_objPlayerSet.GetAntagonizeSyndicateAmount();
    for(int i = 0; i < nAmount; i++)
    {
        char strName[_MAX_NAMESIZE];
        strcpy(strName, g_objPlayerSet.GetAntagonizeNameByIndex(i));
        if (strlen(strName))
        {
            m_EnemyList.InsertString ( i, strName ) ;
            m_EnemyList.SetListRealIndex ( i, 1 ) ;
        }
    }
}

void CDlgKnightageEnemy::SetFriendList()
{
    if (m_ListMode != 0)
    {
        return;
    }
    m_EnemyList.ResetContent();
    int nAmount = g_objPlayerSet.GetAllySyndicateAmount();
    for(int i = 0; i < nAmount; i++)
    {
        char strName[_MAX_NAMESIZE];
        strcpy(strName, g_objPlayerSet.GetAllySyndicateNameByIndex(i));
        if (strlen(strName))
        {
            m_EnemyList.InsertString ( i, strName ) ;
            m_EnemyList.SetListRealIndex ( i, 1 ) ;
        }
    }
}

void CDlgKnightageEnemy::OnBtnAddenemy()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.IsSynLeader())
    {
        if (m_ListMode == 1)
        {
            CDlgKnightageEnemy::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE, ON_FACTION_ADDENEMY);
        }
        else
        {
            CDlgKnightageEnemy::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE, ON_FACTION_ADDALLY);
        }
        m_BtnAddEnemy.EnableWindow(false);
        m_BtnDelEnemy.EnableWindow(false);
    }
}

void CDlgKnightageEnemy::OnBtnDelenemy()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.IsSynLeader())
    {
        char szName[_MAX_NAMESIZE] = "";
        m_EnemyList.GetText(m_EnemyList.GetCurSel(), szName);
        if (strlen(szName) > 0 )
        {
            if (m_ListMode == 1)
            {
                g_objHero.ClearAntagonizeSyn(szName);
            }
            else
            {
                g_objHero.ClearAllySyn(szName);
            }
        }
        m_BtnAddEnemy.EnableWindow(false);
        m_BtnDelEnemy.EnableWindow(false);
    }
}

void CDlgKnightageEnemy::OnChkEnemylist()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_ListMode != 1)
    {
        m_BtnAddEnemy.EnableWindow(false);
        m_BtnDelEnemy.EnableWindow(false);
        m_ListMode = 1;
        m_ChkFriendList.SetCheckValue(0);
        m_ChkEnemyList.SetCheckValue(1);
        SetEnemyList();
    }
}


void CDlgKnightageEnemy::OnChkFriendlist()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (m_ListMode != 0)
    {
        m_BtnAddEnemy.EnableWindow(false);
        m_BtnDelEnemy.EnableWindow(false);
        m_ListMode = 0;
        m_ChkFriendList.SetCheckValue(1);
        m_ChkEnemyList.SetCheckValue(0);
        SetFriendList();
    }
}

void CDlgKnightageEnemy::OnSetfocusListEnemy()
{
    // TODO: Add your control notification handler code here
    if (g_objHero.IsSynLeader())
    {
        m_BtnAddEnemy.EnableWindow();
        m_BtnDelEnemy.EnableWindow();
    }
    else
    {
        m_BtnAddEnemy.EnableWindow(false);
        m_BtnDelEnemy.EnableWindow(false);
    }
}
