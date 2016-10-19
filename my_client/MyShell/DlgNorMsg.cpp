
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgNorMsg.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgNorMsg.h"

#include "GameMsg.h"
#include "MyShellDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MSGFONT_HEIGHT = 14;
/////////////////////////////////////////////////////////////////////////////
// CDlgNorMsg dialog
//extern CGameMsg g_objGameMsg;

CDlgNorMsg::CDlgNorMsg(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgNorMsg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgNorMsg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgNorMsg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgNorMsg)
    DDX_Control(pDX, IDC_BUTTON_CHANGESIZE, m_btnChange);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNorMsg, CDialog)
    //{{AFX_MSG_MAP(CDlgNorMsg)
    ON_WM_MOVE()
    ON_WM_CTLCOLOR()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNorMsg message handlers

BOOL CDlgNorMsg::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgNorMsg::GetWindowRect(rect);
    m_bShow = true;
    m_nMsgUpDown = 0; //0为默认消息不分屏。
    m_btnChange.Init(rect.left, rect.top, "Button161");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNorMsg::Show()
{
    if ( m_bShow )
    {
        /*		// Show the owner
        		CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char * )g_strControlAni,
        			"Dialog30",
        			EXIGENCE_IMMEDIATE ) ;
        		if ( ShowAni != NULL )
        		{
        			RECT rect = {0,0,64,64};
        			CRect Drect;
        			CDlgNorMsg::GetWindowRect(&Drect);
        			ShowAni->ShowEx ( 0,
        				m_Pnt.x ,
        				m_Pnt.y ,
        				&rect,
        				g_objGameMsg.GetNorAreaWidth(),
        				Drect.Height()) ;
        		}
        		else
        			return;
        */
        m_btnChange.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgNorMsg::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x ;
    m_Pnt.y = y ;
    // TODO: Add your message handler code here
}

HBRUSH CDlgNorMsg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

LRESULT CDlgNorMsg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_MY_MESSAGE)  // packbutton return message
    {
        if (wParam == ON_PACKBTN_MOVE)
        {
            ResetDlgRect();
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}
void CDlgNorMsg::ResetDlgRect()
{
    DXPlaySound("Sound/Dlg_Ok.wav");
    CPoint point = m_btnChange.GetMovePos();
    if (point.x == 0 && point.y == 0)
    {
        return;
    }
    int nBeforLine = g_objGameMsg.GetShowLines();
    int nAddLine  = point.y / (MSGFONT_HEIGHT + 2);
    if (( nBeforLine - nAddLine) > 20 )
    {
        g_objGameMsg.SetShowLines(20);
    }
    else if (nBeforLine - nAddLine < 0)
    {
        g_objGameMsg.SetShowLines(0);
    }
    else
    {
        g_objGameMsg.SetShowLines(nBeforLine - nAddLine);
    }
    CRect rect;
    CDlgNorMsg::GetWindowRect(rect);
    int dTop = g_objGameMsg.GetNorAeroTop();
    CDlgNorMsg::MoveWindow(0, dTop - 25, rect.Width(), rect.Height(), false);
}


void CDlgNorMsg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect rect;
    m_btnChange.GetWindowRect(&rect);
    if (! rect.PtInRect( point ) )
    {
        LPARAM lParam ;
        lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
        GetParent()->PostMessage( WM_LBUTTONDOWN, nFlags, lParam ) ;
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgNorMsg::OnLButtonUp(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    GetParent()->PostMessage( WM_LBUTTONUP, nFlags, lParam ) ;
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgNorMsg::OnRButtonDown(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    GetParent()->PostMessage( WM_RBUTTONDOWN, nFlags, lParam ) ;
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgNorMsg::OnRButtonUp(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    GetParent()->PostMessage( WM_RBUTTONUP, nFlags, lParam ) ;
    CDialog::OnRButtonUp(nFlags, point);
}

void CDlgNorMsg::OnMouseMove(UINT nFlags, CPoint point)
{
    CDialog::OnMouseMove(nFlags, point);
}
