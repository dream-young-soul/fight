
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgKnightageMemList.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgKnightageMemList.h"

#include "Player.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageMemList dialog


CDlgKnightageMemList::CDlgKnightageMemList(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgKnightageMemList::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgKnightageMemList)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;
    m_nMemberIndex = 0;
    m_nCurSelMember = -1;
    //}}AFX_DATA_INIT
}


void CDlgKnightageMemList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgKnightageMemList)
    DDX_Control(pDX, IDC_BTN_RIGHT, m_BtnRight);
    DDX_Control(pDX, IDC_BTN_LEFT, m_BtnLeft);
    DDX_Control(pDX, IDC_BTN_LEADERNEXT, m_BtnLeader);
    DDX_Control(pDX, IDC_BTN_KICKOUT, m_BtnKickOut);
    DDX_Control(pDX, IDC_BTN_ADD, m_BtnAdd);
    DDX_Control(pDX, IDC_STA_TEAMHEAD1, m_StaTeamHead1);
    DDX_Control(pDX, IDC_LIST_MEMBER, m_MemberList);
    DDX_Control(pDX, IDC_STA_HEADNAME, m_StaHeadName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKnightageMemList, CDialog)
    //{{AFX_MSG_MAP(CDlgKnightageMemList)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
    ON_BN_CLICKED(IDC_BTN_KICKOUT, OnBtnKickout)
    ON_BN_CLICKED(IDC_BTN_LEADERNEXT, OnBtnLeadernext)
    ON_BN_CLICKED(IDC_BTN_LEFT, OnBtnLeft)
    ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageMemList message handlers

void CDlgKnightageMemList::Show(int x, int y)
{
    if (m_bShow)
    {
        m_StaHeadName.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaTeamHead1.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_MemberList.Show (m_Pnt.x + x, m_Pnt.y + y);
        if (m_BtnKickOut.IsWindowEnabled())
        {
            m_BtnKickOut.Show (m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnLeader.IsWindowEnabled())
        {
            m_BtnLeader.Show (m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnAdd.IsWindowEnabled())
        {
            m_BtnAdd.Show (m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnLeft.IsWindowEnabled())
        {
            m_BtnLeft.Show (m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnRight.IsWindowEnabled())
        {
            m_BtnRight.Show (m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_DlgFacState.m_bShow)
        {
            m_DlgFacState.Show();
        }
    }
}

LRESULT CDlgKnightageMemList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow)
        {
            g_objHero.QuerySynMemberList(m_nMemberIndex);
            SetMemList();
        }
        else
        {
            if (m_DlgFacState.m_bShow)
            {
                m_DlgFacState.EnableWindow(false);
            }
        }
    }
    if (message == WM_COMMAND)
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
        if ( wParam == 0 )// Begin the listbox sel change
        {
            m_nCurSelMember = lParam;
            SetKgButtonStatus();
            if (m_DlgFacState.m_bShow)
            {
                m_DlgFacState.EnableWindow(false);
            }
        }
        // End the listbox sel change
        else if ( wParam == -1 )
        {
            if (m_DlgFacState.m_bShow)
            {
                m_DlgFacState.EnableWindow(false);
            }
            if ( lParam != -1 )
            {
                m_nCurSelMember = lParam;
                SetKgButtonStatus();
            }
        }
        else if (wParam == 1) //在线
        {
            if (m_DlgFacState.m_bShow)
            {
                m_DlgFacState.EnableWindow(false);
            }
            m_nCurSelMember = lParam;
            char strName[_MAX_NAMESIZE] = "";
            if (GetMemberName(m_nCurSelMember, strName))
            {
                g_objHero.QuerySynMember(strName);
            }
        }
        else if (wParam == 2)
        {
            if (m_DlgFacState.m_bShow)
            {
                m_DlgFacState.EnableWindow(false);
            }
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgKnightageMemList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Return a different brush if the default is not desired
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgKnightageMemList::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgKnightageMemList::GetWindowRect(rect);
    // TODO: Add extra initialization here
    //Init the dialog regoup and rename
    CRect Srect;
    m_StaHeadName.Init(rect.left, rect.top);
    m_StaHeadName.SetStaticPosType(1);
    m_StaHeadName.SetFontType(2);
    m_StaTeamHead1.Init(rect.left, rect.top);
    m_BtnLeft.Init(rect.left, rect.top, "Button88");
    m_BtnLeft.SetTipShow((char*)g_objGameDataSet.GetStr(10852), 0xffff00);
    m_BtnRight.Init(rect.left, rect.top, "Button89");
    m_BtnRight.SetTipShow((char*)g_objGameDataSet.GetStr(10853), 0xffff00);
    m_BtnKickOut.Init(rect.left, rect.top, "Button82");
    m_BtnAdd.Init(rect.left, rect.top, "Button83");
    m_BtnLeader.Init(rect.left, rect.top, "Button832");
    m_MemberList.Init(rect.left, rect.top, NULL, 2);
    m_MemberList.SetListLine(2);
    m_MemberList.SetFontColor(0xffffff);
    m_MemberList.SetParentCob(this);
    m_DlgFacState.Create(IDD_DIALOG_FACSTATE, CDlgKnightageMemList::GetParent()->GetParent());
    CRect Frect;
    m_DlgFacState.GetWindowRect(Frect);
    m_DlgFacState.MoveWindow(_SCR_WIDTH - rect.Width() - Frect.Width() - 100,
                             450 - rect.Height(),
                             Frect.Width(),
                             Frect.Height());
    m_DlgFacState.ShowWindow(SW_SHOW);
    m_DlgFacState.EnableWindow(false);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKnightageMemList::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}



void CDlgKnightageMemList::SetMemData()
{
    if (g_objHero.GetSydicateID() != ID_NONE)
    {
        m_StaHeadName.SetWindowText(g_objHero.GetSyndicate());
    }
    else
    {
        m_StaHeadName.SetWindowText("");
    }
    if (g_objHero.IsSynLeader())
    {
        m_BtnAdd.EnableWindow();
        m_BtnKickOut.EnableWindow();
        m_BtnLeader.EnableWindow();
    }
    else
    {
        m_BtnAdd.EnableWindow(false);
        m_BtnKickOut.EnableWindow(false);
        m_BtnLeader.EnableWindow(false);
    }
}


void CDlgKnightageMemList::SetMemList()
{
    SetMemData();
    m_MemberList.ResetContent();
    CDlgKnightageMemList::SetKgButtonStatus();
    if (g_objHero.GetSydicateID() == ID_NONE)
    {
        return;
    }
    int nAmount = g_objHero.GetSynMemberAmount();
    if (m_nMemberIndex <= 0)
    {
        m_BtnLeft.EnableWindow(false);
    }
    else
    {
        m_BtnLeft.EnableWindow();
    }
    if (m_nMemberIndex + 18 > nAmount)
    {
        m_BtnRight.EnableWindow(false);
    }
    else
    {
        m_BtnRight.EnableWindow();
    }
    if (nAmount > m_nMemberIndex + 18)
    {
        nAmount = m_nMemberIndex + 18;
    }
    int nOnline = 0;
    for(int i = m_nMemberIndex; i < nAmount; i++)
    {
        SynMemberInfo* pInfo = g_objHero.GetSynMemberByIndex(i);
        if (pInfo != NULL && pInfo->bOnLine == 1)
        {
            nOnline++ ;
        }
    }
    int nOffLine = nOnline ;
    for (int i = nAmount - 1 ; i >= m_nMemberIndex ; i--)
    {
        SynMemberInfo* pInfo = g_objHero.GetSynMemberByIndex(i);
        if (pInfo != NULL)
        {
            char strListData[64] ;
            sprintf ( strListData, "%s %s", pInfo->szName, pInfo->szRank ) ;
            if ( pInfo->bOnLine == 1)
            {
                m_MemberList.InsertString ( 0, strListData ) ;
                if (pInfo->ucRank > 10)
                {
                    m_MemberList.SetListRealIndex ( nOnline - 1, 1 ) ;
                }
                else
                {
                    m_MemberList.SetListRealIndex ( nOnline - 1, 2 ) ;
                }
                nOnline-- ;
            }
            else
            {
                m_MemberList.AddString(strListData);
                m_MemberList.SetListRealIndex ( nOffLine, -1 ) ;
                nOffLine++ ;
            }
        }
    }
    char szMsg[32] = "";
    sprintf(szMsg, "%d/%d", m_nMemberIndex / 18 + 1, g_objHero.GetSyndicatePopulation() / 18 + 1);
    m_StaTeamHead1.SetWindowText(szMsg);
}



void CDlgKnightageMemList::OnBtnAdd()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.GetSyndicateLeaderName()
            && strcmp(g_objHero.GetName(), g_objHero.GetSyndicateLeaderName()) == 0)
    {
        CDlgKnightageMemList::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE, ON_FACTION_INVITE);
    }
}

void CDlgKnightageMemList::OnBtnKickout()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.IsSynLeader() && m_nCurSelMember != -1)
    {
        CDlgKnightageMemList::GetParent()->GetParent()->PostMessage(WM_MY_MESSAGE, ON_FACTION_KICK, m_nCurSelMember);
    }
}

void CDlgKnightageMemList::OnBtnLeadernext()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.IsSynLeader() && m_nCurSelMember != -1)
    {
        char strName[_MAX_NAMESIZE] = "";
        GetMemberName(m_nCurSelMember, strName);
        g_objHero.SetSynLeader(1, strName);
    }
}

void CDlgKnightageMemList::OnBtnLeft()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/up_Page.wav" ) ;
    m_nMemberIndex = m_nMemberIndex - 18;
    if (m_nMemberIndex < 0)
    {
        m_nMemberIndex = 0;
    }
    CDlgKnightageMemList::SetMemList();
}

void CDlgKnightageMemList::OnBtnRight()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/up_Page.wav" ) ;
    int nAmount = g_objHero.GetSynMemberAmount();
    if (nAmount > m_nMemberIndex + 18)
    {
        m_nMemberIndex += 18;
    }
    CDlgKnightageMemList::SetMemList();
}

void CDlgKnightageMemList::SetKgButtonStatus()
{
    if (!g_objHero.IsSynLeader())
    {
        m_BtnAdd.EnableWindow(false);
        m_BtnKickOut.EnableWindow(false);
        m_BtnLeader.EnableWindow(false);
        return;
    }
    else
    {
        m_BtnAdd.EnableWindow();
        m_BtnKickOut.EnableWindow(false);
        m_BtnLeader.EnableWindow(false);
    }
    if (m_nCurSelMember != -1)
    {
        char strName[_MAX_NAMESIZE] = "";
        if (GetMemberName(m_nCurSelMember, strName))
        {
            int nAmount = g_objHero.GetSynMemberAmount();
            for(int i = 0; i < nAmount; i++)
            {
                SynMemberInfo* pInfo = g_objHero.GetSynMemberByIndex(i);
                if (pInfo && strcmp(pInfo->szName, strName) == 0)
                {
                    if (pInfo->ucRank == 100) //军团长
                    {
                        m_BtnLeader.EnableWindow(false);
                        m_BtnKickOut.EnableWindow(false);
                    }
                    else
                    {
                        m_BtnLeader.EnableWindow();
                        m_BtnKickOut.EnableWindow();
                    }
                    return;
                }
            }
        }
    }
}

void CDlgKnightageMemList::OpenFacState()
{
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    char strName[_MAX_NAMESIZE] = "";
    if (GetMemberName(m_nCurSelMember, strName))
    {
        m_DlgFacState.SetFacState(strName);
    }
}

BOOL CDlgKnightageMemList::GetMemberName(int nIndex , char* szName)
{
    char szLine[64] = "";
    m_MemberList.GetText(nIndex , szLine );
    if (strlen(szLine))
    {
        char szRank[_MAX_NAMESIZE] = "";
        int nResult = sscanf(szLine, "%s %s", szName, szRank);
        if (nResult == 2)
        {
            return TRUE;
        }
    }
    return FALSE;
}
