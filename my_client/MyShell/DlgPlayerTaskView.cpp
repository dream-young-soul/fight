
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgPlayerTaskView.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgPlayerTaskView.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskView dialog


CDlgPlayerTaskView::CDlgPlayerTaskView(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgPlayerTaskView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgPlayerTaskView)
    m_bShow = false;
    m_nViewMode = 0;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgPlayerTaskView::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPlayerTaskView)
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Control(pDX, IDC_TASK_ITEMGRID, m_GridItem);
    DDX_Control(pDX, IDC_STATIC_TASKTITLE, m_StaTaskTitle);
    DDX_Control(pDX, IDC_STATIC_TASKREQINFO, m_StaTaskReqInfo);
    DDX_Control(pDX, IDC_STATIC_TASKMONEY, m_StaTaskMoney);
    DDX_Control(pDX, IDC_STATIC_TASKINFO, m_StaTaskInfo);
    DDX_Control(pDX, IDC_STATIC_REQLEV, m_StaReqLev);
    DDX_Control(pDX, IDC_BTN_TASKOK, m_BtnTaskOk);
    DDX_Control(pDX, IDC_BTN_TASKEXIT, m_BtnTaskExit);
    DDX_Control(pDX, IDC_BTN_TASKCANCEL, m_BtnTaskCancel);
    DDX_Control(pDX, IDC_BTN_TASKBACK, m_BtnTaskBack);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlayerTaskView, CDialog)
    //{{AFX_MSG_MAP(CDlgPlayerTaskView)
    ON_BN_CLICKED(IDC_BTN_TASKBACK, OnBtnTaskback)
    ON_BN_CLICKED(IDC_BTN_TASKCANCEL, OnBtnTaskcancel)
    ON_BN_CLICKED(IDC_BTN_TASKEXIT, OnBtnTaskexit)
    ON_BN_CLICKED(IDC_BTN_TASKOK, OnBtnTaskok)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskView message handlers

void CDlgPlayerTaskView::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog75",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnTaskBack.IsWindowEnabled())
        {
            m_BtnTaskBack.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnTaskCancel.IsWindowEnabled())
        {
            m_BtnTaskCancel.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnTaskExit.IsWindowEnabled())
        {
            m_BtnTaskExit.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnTaskOk.IsWindowEnabled())
        {
            m_BtnTaskOk.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnClose.IsWindowEnabled())
        {
            m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        }
        m_GridItem.Show(m_Pnt.x, m_Pnt.y);
        m_StaTaskTitle.Show(m_Pnt.x, m_Pnt.y);
        m_StaReqLev.Show(m_Pnt.x, m_Pnt.y);
        m_StaTaskInfo.Show(m_Pnt.x, m_Pnt.y);
        m_StaTaskMoney.Show(m_Pnt.x, m_Pnt.y);
        m_StaTaskReqInfo.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgPlayerTaskView::OnBtnTaskback()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgPlayerTaskView::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKVIEW_BACK);
}

void CDlgPlayerTaskView::OnBtnTaskcancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    g_objHero.CancelPlayerTask(g_objHero.m_infoTask.idTask);
    CDlgPlayerTaskView::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKVIEW_BACK);
}

void CDlgPlayerTaskView::OnBtnTaskexit()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgPlayerTaskView::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKVIEW_CLOSE);
}

void CDlgPlayerTaskView::OnBtnTaskok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    switch(m_nViewMode)
    {
    case 0:
        if (g_objHero.m_infoTask.ucType == _WANT_PK)
        {
            if (0 == strcmp(g_objHero.m_infoTask.strName.c_str(), g_objHero.GetName()))
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10615));
                return;
            }
        }
        g_objHero.AcceptPlayerTask(g_objHero.m_infoTask.idTask);
        break;
    case 1:
        if (g_objHero.m_infoTask.ucType == _WANT_PK)
        {
            g_objHero.CompletePlayerTaskForItem(g_objHero.m_infoTask.idTask, ID_NONE);
        }
        else
        {
            CDlgPlayerTaskView::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKITEM_UP, g_objHero.m_infoTask.idTask);
        }
        break;
    case 2:
        g_objHero.CompletePlayerTaskForItem(g_objHero.m_infoTask.idTask, ID_NONE);
        break;
    default:
        break;
    }
    CDlgPlayerTaskView::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKVIEW_CLOSE);
}

LRESULT CDlgPlayerTaskView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
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
            CDlgPlayerTaskView::OnBtnClose();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgPlayerTaskView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgPlayerTaskView::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgPlayerTaskView::GetWindowRect(rect);
    m_BtnTaskBack.Init(rect.left, rect.top, "Button79");
    m_BtnTaskCancel.Init(rect.left, rect.top, "Button793");
    m_BtnTaskExit.Init(rect.left, rect.top, "Button794");
    m_BtnTaskOk.Init(rect.left, rect.top, "Button791"); //button792 overbutton
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_GridItem.Init(rect.left, rect.top, 1, 5, DLG_TASKVIEW);
    m_GridItem.SetEnable(false);
    m_StaTaskTitle.Init(rect.left, rect.top);
    m_StaReqLev.Init(rect.left, rect.top);
    m_StaTaskInfo.Init(rect.left, rect.top, 2);
    m_StaTaskMoney.Init(rect.left, rect.top);
    m_StaTaskReqInfo.Init(rect.left, rect.top, 2);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPlayerTaskView::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgPlayerTaskView::SetPlayerTaskViewMode(int nMode)
{
    m_nViewMode = nMode;
    switch(m_nViewMode)
    {
    case 0:	//un accept task
        m_BtnTaskCancel.EnableWindow(false);
        m_BtnTaskOk.EnableWindow(true);
        m_BtnTaskOk.ChangeImage("Button791"); //button792 overbutton
        break;
    case 1: //accept task
        m_BtnTaskOk.EnableWindow(true);
        m_BtnTaskCancel.EnableWindow(true);
        m_BtnTaskOk.ChangeImage("Button792");
        break;
    case 2:	//my task
        m_BtnTaskCancel.EnableWindow(true);
        m_BtnTaskCancel.ChangeImage("Button793");
        m_BtnTaskOk.EnableWindow(true);
        m_BtnTaskOk.ChangeImage("Button792");
        break;
    default:
        break;
    }
}

void CDlgPlayerTaskView::SetPlayerTask()
{
    m_StaTaskTitle.SetWindowText(g_objHero.m_infoTask.strName.c_str());
    m_StaReqLev.SetWindowText(g_objGameDataSet.GetStr(10601 + g_objHero.m_infoTask.ucRankReq));
    m_StaTaskInfo.SetWindowText(g_objHero.m_infoTask.strDesc.c_str());
    char strTask[16] = "";
    sprintf(strTask, "%d", g_objHero.m_infoTask.dwMoney);
    m_StaTaskMoney.SetWindowText(strTask);
    char strInfo[256] = "";
    switch(g_objHero.m_infoTask.ucType)
    {
    case _WANT_PK:
        sprintf(strInfo, g_objGameDataSet.GetStr(10861), g_objHero.m_infoTask.strWant.c_str());	//"追杀: %s"
        break;
    case _WANT_ITEM:
        sprintf(strInfo, g_objGameDataSet.GetStr(10862),			//"寻找:%s,耐久度:%d,等级:%d,魔魂等级:%d"
                g_objHero.m_infoTask.strWant.c_str(),
                g_objHero.m_infoTask.dwDurability,
                g_objHero.m_infoTask.ucDegree,
                g_objHero.m_infoTask.ucAddition);
        break;
    case _WANT_EUDEMON:
        sprintf(strInfo, g_objGameDataSet.GetStr(10863),				//"寻找:%s,等级:%d,成长:%d"
                g_objHero.m_infoTask.strWant.c_str(),
                g_objHero.m_infoTask.ucDegree,
                g_objHero.m_infoTask.ucAddition);
        break;
    default:
        break;
    }
    m_StaTaskReqInfo.SetWindowText(strInfo);
    m_GridItem.RemoveIcon(0, 0, true);
    for(int i = 0; i < 5; i++)
    {
        if (g_objHero.m_infoTask.infoItem[i].idItemType)
        {
            int nIndex =  m_GridItem.InsertIcon(1,
                                                1,
                                                g_objHero.m_infoTask.infoItem[i].idItemType,
                                                i,
                                                ICON_TYPE_TASKITEM ) ;
        }
    }
}

void CDlgPlayerTaskView::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgPlayerTaskView::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKVIEW_CLOSE);
}
