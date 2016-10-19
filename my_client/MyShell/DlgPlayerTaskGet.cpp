
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgPlayerTaskGet.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgPlayerTaskGet.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskGet dialog


CDlgPlayerTaskGet::CDlgPlayerTaskGet(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgPlayerTaskGet::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgPlayerTaskGet)
    m_bShow = false;
    m_uCurSetDlg = -1;
    m_nReqLevel = 0;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgPlayerTaskGet::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPlayerTaskGet)
    DDX_Control(pDX, IDC_LIST_REQLEV, m_ListReqLev);
    DDX_Control(pDX, IDC_STA_REQLEV, m_StaReqLev);
    DDX_Control(pDX, IDC_EDIT_MONEY, m_EditMoney);
    DDX_Control(pDX, IDC_BTN_CLOSED, m_BtnCloseD);
    DDX_Control(pDX, IDC_TASK_ITEMGRID, m_GridTaskItem);
    DDX_Control(pDX, IDC_EDIT_TASKNAME, m_EditTaskName);
    DDX_Control(pDX, IDC_EDIT_TASKINFO, m_EditTaskInfo);
    DDX_Control(pDX, IDC_BTN_SETMONSTER, m_BtnSetMonster);
    DDX_Control(pDX, IDC_BTN_SETKILLER, m_BtnSetKiller);
    DDX_Control(pDX, IDC_BTN_SETITEM, m_BtnSetItem);
    DDX_Control(pDX, IDC_BTN_TASKGETOK, m_BtnTaskGetOk);
    DDX_Control(pDX, IDC_BTN_REQLEV, m_BtnReqLev);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlayerTaskGet, CDialog)
    //{{AFX_MSG_MAP(CDlgPlayerTaskGet)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_REQLEV, OnBtnReqlev)
    ON_BN_CLICKED(IDC_BTN_SETITEM, OnBtnSetitem)
    ON_BN_CLICKED(IDC_BTN_SETKILLER, OnBtnSetkiller)
    ON_BN_CLICKED(IDC_BTN_SETMONSTER, OnBtnSetmonster)
    ON_BN_CLICKED(IDC_BTN_TASKGETOK, OnBtnTaskgetok)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSED, OnBtnClosed)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskGet message handlers

void CDlgPlayerTaskGet::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog71",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_BtnCloseD.Show(m_Pnt.x, m_Pnt.y);
        m_BtnReqLev.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetItem.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetKiller.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetMonster.Show(m_Pnt.x, m_Pnt.y);
        m_BtnTaskGetOk.Show(m_Pnt.x, m_Pnt.y);
        m_GridTaskItem.Show(m_Pnt.x, m_Pnt.y);
        m_StaReqLev.Show(m_Pnt.x, m_Pnt.y);
        m_EditTaskInfo.Show(m_Pnt.x, m_Pnt.y);
        m_EditTaskName.Show(m_Pnt.x, m_Pnt.y);
        m_EditMoney.Show(m_Pnt.x, m_Pnt.y);
        if (m_ListReqLev.IsWindowEnabled())
        {
            m_ListReqLev.Show(m_Pnt.x, m_Pnt.y);
        }
        ShowSetDlg();
    }
}

void CDlgPlayerTaskGet::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgPlayerTaskGet::OnBtnClosed();
}

void CDlgPlayerTaskGet::OnBtnReqlev()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_ListReqLev.IsWindowEnabled())
    {
        m_EditTaskInfo.EnableWindow(true);
        m_ListReqLev.EnableWindow(false);
    }
    else
    {
        m_EditTaskInfo.EnableWindow(false);
        m_ListReqLev.EnableWindow(true);
    }
}

void CDlgPlayerTaskGet::OnBtnSetitem()
{
    // TODO: Add your control notification handler code here
    if (!m_DlgTaskItem.m_bShow)
    {
        CloseCurSetDlg();
        m_uCurSetDlg = 2;
        m_DlgTaskItem.EnableWindow();
    }
}

void CDlgPlayerTaskGet::OnBtnSetkiller()
{
    // TODO: Add your control notification handler code here
    if (!m_DlgTaskKiller.m_bShow)
    {
        CloseCurSetDlg();
        m_uCurSetDlg = 1;
        m_DlgTaskKiller.EnableWindow();
    }
}

void CDlgPlayerTaskGet::OnBtnSetmonster()
{
    // TODO: Add your control notification handler code here
    if (!m_DlgTaskMonster.m_bShow)
    {
        CloseCurSetDlg();
        m_uCurSetDlg = 3;
        m_DlgTaskMonster.EnableWindow();
    }
}

void CDlgPlayerTaskGet::OnBtnTaskgetok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    char szTitle[256] = "";
    m_EditTaskName.GetWindowText(szTitle, 16);
    if (strlen(szTitle) <= 0)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr (10610));	//"没有填写任务名称!"
        return;
    }
    char szDesc[256] = "";
    m_EditTaskInfo.GetWindowText(szDesc, 256);
    if (strlen(szDesc) <= 0)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr (10611));		//"没有填写任务信息!"
        return;
    }
    char szMoney[16] = "";
    m_EditMoney.GetWindowText(szMoney, 16);
    if (strlen(szMoney) <= 0)
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr (10612));		//"没有填写金钱!"
        return;
    }
    DWORD dwMoney = atoi(szMoney);
    if (dwMoney == 0)		//检查是否有奖励物品和金钱
    {
        BOOL bSetItem = false;
        for(int i = 0; i < 5; i++)
        {
            if (m_GridTaskItem.GetIconRealID(i) != 0)
            {
                bSetItem = true;
                break;
            }
        }
        if (bSetItem == false)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr (10613));	//"没有添加奖励物品或金钱!"
            return;
        }
    }
    switch(m_uCurSetDlg)
    {
    case 1:	//want killer
        if (m_DlgTaskKiller.IsSetOk())
        {
            if (0 == strcmp(m_DlgTaskKiller.GetWantKiller(), g_objHero.GetName()))
            {
                g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr (10614));		//"不能下达追杀自己的任务!"
                return;
            }
            g_objHero.WantPk(m_DlgTaskKiller.GetWantKiller(),
                             szTitle, szDesc,
                             dwMoney, m_nReqLevel,
                             m_GridTaskItem.GetIconRealID(0),
                             m_GridTaskItem.GetIconRealID(1),
                             m_GridTaskItem.GetIconRealID(2),
                             m_GridTaskItem.GetIconRealID(3),
                             m_GridTaskItem.GetIconRealID(4));
            m_GridTaskItem.RemoveIcon(0, 0, true);
            CDlgPlayerTaskGet::OnBtnClosed();
        }
        break;
    case 2:	//want item
        if (m_DlgTaskItem.IsSetOk())
        {
            char szName[16] = "";
            m_DlgTaskItem.m_EditItemName.GetWindowText(szName, 16);
            g_objHero.WantItem(szName, szTitle, szDesc, _WANT_ITEM,
                               m_DlgTaskItem.m_nItemAmount,
                               m_DlgTaskItem.m_nItemSoul,
                               m_DlgTaskItem.m_nItemMark,
                              (int) dwMoney, m_nReqLevel,
                               m_GridTaskItem.GetIconRealID(0),
                               m_GridTaskItem.GetIconRealID(1),
                               m_GridTaskItem.GetIconRealID(2),
                               m_GridTaskItem.GetIconRealID(3),
                               m_GridTaskItem.GetIconRealID(4));
            m_GridTaskItem.RemoveIcon(0, 0, true);
            CDlgPlayerTaskGet::OnBtnClosed();
        }
        break;
    case 3: //want monster
        if (m_DlgTaskMonster.IsSetOk())
        {
            char szName[16] = "";
            m_DlgTaskMonster.m_EditMonsterType.GetWindowText(szName, 16);
            g_objHero.WantItem(szName, szTitle, szDesc, _WANT_EUDEMON, 0,
                               m_DlgTaskMonster.m_nMonsterGrow,
                               m_DlgTaskMonster.m_nMonsterLev,
                              (int) dwMoney, m_nReqLevel,
                               m_GridTaskItem.GetIconRealID(0),
                               m_GridTaskItem.GetIconRealID(1),
                               m_GridTaskItem.GetIconRealID(2),
                               m_GridTaskItem.GetIconRealID(3),
                               m_GridTaskItem.GetIconRealID(4));
            m_GridTaskItem.RemoveIcon(0, 0, true);
            CDlgPlayerTaskGet::OnBtnClosed();
        }
        break;
    default:
        break;
    }
}

LRESULT CDlgPlayerTaskGet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            ResetTaskData();
            m_EditTaskName.SetFocus();
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
        break ;
    case WM_COMMAND:
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgPlayerTaskGet::OnBtnClose();
            return true ;
        }
        break ;
    case LB_SETSEL:
        {
            if (wParam == 0 || wParam == -1)
            {
                if (lParam != -1)
                {
                    char strReq[16] = "";
                    m_nReqLevel = lParam;
                    m_ListReqLev.GetText(lParam, strReq);
                    m_StaReqLev.SetWindowText(strReq);
                    CDlgPlayerTaskGet::OnBtnReqlev() ;
                }
            }
        }
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgPlayerTaskGet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgPlayerTaskGet::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgPlayerTaskGet::GetWindowRect(rect);
    m_EditTaskName.Init(rect.left, rect.top);
    m_EditTaskName.SetLimitText(16);
    m_EditTaskName.SetOffset(CPoint(4, 6));
    m_EditTaskInfo.Init(rect.left, rect.top, true);
    m_EditTaskInfo.SetLimitText ( 128 ) ;
    m_EditTaskInfo.SetOffset ( CPoint ( 2, 3 ) ) ;
    m_EditMoney.Init(rect.left, rect.top);
    m_EditMoney.SetLimitText(16);
    m_EditMoney.SetOffset( CPoint(4, 6));
    m_StaReqLev.Init(rect.left, rect.top);
    m_GridTaskItem.MoveWindow(101, 199, 179, 34, false);
    m_GridTaskItem.Init(rect.left, rect.top, 1, 5, DLG_TASKGET);
    m_BtnReqLev.Init(rect.left, rect.top, "Button78");
    m_ListReqLev.Init(rect.left, rect.top, NULL, 2);
    m_ListReqLev.SetParentCob(this);
    m_ListReqLev.EnableWindow(false);
    m_ListReqLev.SetShowBack(true);
    m_ListReqLev.SetFontColor(0xffff00ff);
    m_ListReqLev.AddString("  D  ");
    m_ListReqLev.AddString("  C  ");
    m_ListReqLev.AddString("  B  ");
    m_ListReqLev.AddString("  A  ");
    m_ListReqLev.AddString("  S  ");
    m_BtnSetKiller.Init(rect.left, rect.top, "Button73");
    m_BtnSetItem.Init(rect.left, rect.top, "Button74");
    m_BtnSetMonster.Init(rect.left, rect.top, "Button75");
    m_BtnTaskGetOk.Init(rect.left, rect.top, "Button76");
    m_BtnClose.Init(rect.left, rect.top, "Button77");
    m_BtnCloseD.Init(rect.left, rect.top, "Button540");
    //init the set dialog
    CRect MRect;
    m_DlgTaskMonster.Create(IDD_DIALOG_TASKMONSTER, this->GetParent());
    m_DlgTaskMonster.GetWindowRect(MRect);
    m_DlgTaskMonster.MoveWindow(_SCR_WIDTH - rect.Width() - MRect.Width(),
                                450 - rect.Height(),
                                MRect.Width(), MRect.Height(), false);
    m_DlgTaskMonster.ShowWindow(SW_SHOW);
    m_DlgTaskMonster.EnableWindow(false);
    m_DlgTaskItem.Create(IDD_DIALOG_TASKITEM, this->GetParent());
    m_DlgTaskItem.GetWindowRect(MRect);
    m_DlgTaskItem.MoveWindow(_SCR_WIDTH - rect.Width() - MRect.Width(),
                             450 - rect.Height(),
                             MRect.Width(), MRect.Height(), false);
    m_DlgTaskItem.ShowWindow(SW_SHOW);
    m_DlgTaskItem.EnableWindow(false);
    m_DlgTaskKiller.Create(IDD_DIALOG_TASKKILLER, this->GetParent());
    m_DlgTaskKiller.GetWindowRect(MRect);
    m_DlgTaskKiller.MoveWindow(_SCR_WIDTH - rect.Width() - MRect.Width(),
                               450 - rect.Height(),
                               MRect.Width(), MRect.Height(), false);
    m_DlgTaskKiller.ShowWindow(SW_SHOW);
    m_DlgTaskKiller.EnableWindow(false);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPlayerTaskGet::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgPlayerTaskGet::ShowSetDlg()
{
    switch(m_uCurSetDlg)
    {
    case 1:		//set killer dialog
        m_DlgTaskKiller.Show();
        break;
    case 2:		//set item dialog
        m_DlgTaskItem.Show();
        break;
    case 3:		//set monster dialog
        m_DlgTaskMonster.Show();
        break;
    default:
        break;
    }
}

void CDlgPlayerTaskGet::CloseCurSetDlg()
{
    switch(m_uCurSetDlg)
    {
    case 1:		//set killer dialog
        m_DlgTaskKiller.EnableWindow(false);
        break;
    case 2:		//set item dialog
        m_DlgTaskItem.EnableWindow(false);
        break;
    case 3:		//set monster dialog
        m_DlgTaskMonster.EnableWindow(false);
        break;
    default:
        break;
    }
    m_uCurSetDlg = -1;
}

void CDlgPlayerTaskGet::OnBtnClosed()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgPlayerTaskGet::EnableWindow(false);
    CDlgPlayerTaskGet::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKUP_CLOSE);
}

void CDlgPlayerTaskGet::ResetTaskData()
{
    m_EditTaskName.SetWindowText("");
    m_StaReqLev.SetWindowText(" D ");
    m_EditTaskInfo.SetWindowText("");
    m_EditMoney.SetWindowText("0");
    m_GridTaskItem.RemoveIcon(0, 0, true);
    m_uCurSetDlg = 0;
    m_nReqLevel = 0;
    m_BtnSetKiller.EnableWindow(true);
    m_BtnSetItem.EnableWindow(true);
    m_BtnSetMonster.EnableWindow(true);
    m_ListReqLev.EnableWindow(false);
    m_DlgTaskKiller.ReSetDlg();
    m_DlgTaskItem.ReSetDlg();
    m_DlgTaskMonster.ReSetDlg();
}

BOOL CDlgPlayerTaskGet::CheckItem(OBJID idItem)
{
    CItem* pItem = g_objHero.GetItem (idItem);
    if (pItem)
    {
        if (pItem->TestMonopolyItem (_NOT_MONOPOLY_DISAPPEAR))
        {
            g_objGameMsg.AddMsg ((char*)g_objGameDataSet.GetStr(10670));	//该物品不允许做为佣兵任务奖励物品
            CDlgPlayerTaskGet::GetParent ()->PostMessage(WM_MY_MESSAGE, ON_ICON_DROP, DLG_ALL);
            CDlgPlayerTaskGet::GetParent ()->PostMessage(WM_MY_MESSAGE, CMD_FLASHITEM);
        }
        else
        {
            for(int i = 0; i < 5; i++)
            {
                if (idItem == m_GridTaskItem.GetIconRealID(i)
                        && m_GridTaskItem.GetGridAbleByIndex(i))
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

void CDlgPlayerTaskGet::SetTaskSetOk(int nMode)
{
    switch(nMode)
    {
    case 1:
        m_BtnSetKiller.EnableWindow(true);
        m_BtnSetItem.EnableWindow(false);
        m_BtnSetMonster.EnableWindow(false);
        break;
    case 2:
        m_BtnSetKiller.EnableWindow(false);
        m_BtnSetItem.EnableWindow(true);
        m_BtnSetMonster.EnableWindow(false);
        break;
    case 3:
        m_BtnSetKiller.EnableWindow(false);
        m_BtnSetItem.EnableWindow(false);
        m_BtnSetMonster.EnableWindow(true);
        break;
    default:
        break;
    }
}





















