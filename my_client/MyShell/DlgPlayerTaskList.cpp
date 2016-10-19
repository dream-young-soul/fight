
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgPlayerTaskList.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgPlayerTaskList.h"
#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskList dialog


CDlgPlayerTaskList::CDlgPlayerTaskList(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgPlayerTaskList::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgPlayerTaskList)
    m_bShow = false;
    m_nListKind = 0;
    m_nListUnPage = 0;
    m_nListAcPage = 0;
    m_nListMyPage = 0;
    m_bListDown = true;
    m_bListUp = false;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgPlayerTaskList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPlayerTaskList)
    DDX_Control(pDX, IDC_BTN_LISTUP, m_BtnListUp);
    DDX_Control(pDX, IDC_BTN_LISDOWN, m_BtnListDown);
    DDX_Control(pDX, IDC_LIST_PLAYERTASK, m_ListPlayerTask);
    DDX_Control(pDX, IDC_BTN_LISTCLOSE, m_BtnListClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlayerTaskList, CDialog)
    //{{AFX_MSG_MAP(CDlgPlayerTaskList)
    ON_BN_CLICKED(IDC_BTN_LISTCLOSE, OnBtnListclose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_LISTUP, OnBtnListup)
    ON_BN_CLICKED(IDC_BTN_LISDOWN, OnBtnLisdown)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskList message handlers

void CDlgPlayerTaskList::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog70",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, m_Pnt.x, m_Pnt.y );
        }
        else
        {
            return ;
        }
        m_BtnListClose.Show(m_Pnt.x, m_Pnt.y);
        if (m_BtnListDown.IsWindowEnabled())
        {
            m_BtnListDown.Show(m_Pnt.x, m_Pnt.y);
        }
        if (m_BtnListUp.IsWindowEnabled())
        {
            m_BtnListUp.Show(m_Pnt.x, m_Pnt.y);
        }
        m_ListPlayerTask.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgPlayerTaskList::OnBtnListclose()
{
    // TODO: Add your control notification handler code here
    CDlgPlayerTaskList::EnableWindow(false);
}

LRESULT CDlgPlayerTaskList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            CDlgPlayerTaskList::ResetTaskList();
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
            CDlgPlayerTaskList::OnBtnListclose();
            return true ;
        }
        break ;
    case LB_SETSEL:
        {
            if ( wParam == 0 )
            {
                if ( lParam != -1 )
                {
                    g_objHero.QueryPlayerTaskDetial(g_objHero.GetPlayerTaskIndex(lParam)->idTask);
                    CDlgPlayerTaskList::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKVIEW_OPEN);
                }
            }
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgPlayerTaskList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgPlayerTaskList::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgPlayerTaskList::GetWindowRect(rect);
    m_ListPlayerTask.Init(rect.left, rect.top, NULL, 2);
    m_ListPlayerTask.SetParentCob(this);
    m_BtnListClose.Init(rect.left, rect.top, "Button540");
    m_BtnListUp.Init(rect.left, rect.top, "Button161");
    m_BtnListDown.Init(rect.left, rect.top, "Button162");
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPlayerTaskList::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgPlayerTaskList::SetTaskListKind(int nKind)
{
    m_nListKind = nKind;
    switch(m_nListKind)
    {
    case 0:
        g_objHero.QueryPlayerTaskUnacceptedList(m_LastPageidUnTask[m_nListUnPage]);
        break;
    case 1:
        g_objHero.QueryPlayerTaskAcceptedList(m_LastPageidAcTask[m_nListAcPage]);
        break;
    case 2:
        g_objHero.QueryPlayerMyTaskList(m_LastPageidMyTask[m_nListMyPage]);
        break;
    default:
        break;
    }
}

void CDlgPlayerTaskList::GetTaskList()
{
    int nAmount = g_objHero.GetPlayerTaskAmout();
    m_ListPlayerTask.ResetContent();
    if (nAmount > 0)
    {
        ST_TASK_TITLE* pTaskTitle = NULL;
        for(int i = 0; i < nAmount; i++)
        {
            pTaskTitle = g_objHero.GetPlayerTaskIndex(i);
            if (pTaskTitle)
            {
                m_ListPlayerTask.InsertString(i, pTaskTitle->szTitle);
                m_ListPlayerTask.SetListRealIndex(i, 1);
                if (nAmount == 15)
                {
                    CDlgPlayerTaskList::SetLastPageTaskID(m_nListKind, pTaskTitle->idTask);
                    break;
                }
            }
        }
    }
    bool bCanUp = false;
    bool bCanDown = false;
    switch(m_nListKind)
    {
    case 0:
        if (m_nListUnPage > 0)
        {
            bCanUp = true;
        }
        if (nAmount > 15)
        {
            bCanDown = true;
        }
        break;
    case 1:
        if (m_nListAcPage > 0)
        {
            bCanUp = true;
        }
        if (nAmount > 15)
        {
            bCanDown = true;
        }
        break;
    case 2:
        break;
    default:
        break;
    }
    if (bCanUp)
    {
        m_BtnListUp.EnableWindow();
    }
    else
    {
        m_BtnListUp.EnableWindow(false);
    }
    if (bCanDown)
    {
        m_BtnListDown.EnableWindow();
    }
    else
    {
        m_BtnListDown.EnableWindow(false);
    }
}



void CDlgPlayerTaskList::OnBtnListup()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    m_bListDown = true;
    m_bListUp = false;
    switch(m_nListKind)
    {
    case 0:
        g_objHero.QueryPlayerTaskUnacceptedList(m_LastPageidUnTask[m_nListUnPage+1]);
        break;
    case 1:
        g_objHero.QueryPlayerTaskAcceptedList(m_LastPageidAcTask[m_nListAcPage+1]);
        break;
    case 2:
        g_objHero.QueryPlayerMyTaskList(m_LastPageidMyTask[m_nListMyPage+1]);
        break;
    default:
        break;
    }
}

void CDlgPlayerTaskList::OnBtnLisdown()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    m_bListUp = true;
    m_bListDown = false;
    switch(m_nListKind)
    {
    case 0:
        g_objHero.QueryPlayerTaskUnacceptedList(m_LastPageidUnTask[m_nListUnPage]);
        break;
    case 1:
        g_objHero.QueryPlayerTaskAcceptedList(m_LastPageidAcTask[m_nListAcPage]);
        break;
    case 2:
        g_objHero.QueryPlayerMyTaskList(m_LastPageidMyTask[m_nListMyPage]);
        break;
    default:
        break;
    }
}

void CDlgPlayerTaskList::SetLastPageTaskID(int nKind, OBJID idTask)
{
    switch(nKind)
    {
    case 0:
        if (m_bListDown)
        {
            m_nListUnPage++;
        }
        else if (m_bListUp && m_nListUnPage > 0)
        {
            m_nListUnPage--;
        }
        m_LastPageidUnTask[m_nListUnPage] = idTask;
        break;
    case 1:
        if (m_bListDown)
        {
            m_nListAcPage++;
        }
        else if (m_bListUp && m_nListAcPage > 0)
        {
            m_nListAcPage--;
        }
        m_LastPageidAcTask[m_nListAcPage] = idTask;
        break;
    case 2:
        if (m_bListDown)
        {
            m_nListMyPage++;
        }
        else if (m_bListUp && m_nListMyPage > 0)
        {
            m_nListMyPage--;
        }
        m_LastPageidMyTask[m_nListMyPage] = idTask;
        break;
    default:
        break;
    }
}

void CDlgPlayerTaskList::ResetTaskList()
{
    for(int i = 0; i < 64; i++)
    {
        m_LastPageidUnTask[i] = 0;
        m_LastPageidAcTask[i] = 0;
        m_LastPageidMyTask[i] = 0;
    }
    m_nListUnPage = 0;
    m_nListAcPage = 0;
    m_nListMyPage = 0;
    m_bListDown = true;
    m_bListUp = false;
    m_ListPlayerTask.ResetContent();
}
