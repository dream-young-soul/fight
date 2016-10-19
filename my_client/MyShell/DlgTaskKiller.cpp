
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgTaskKiller.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTaskKiller.h"

#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskKiller dialog


CDlgTaskKiller::CDlgTaskKiller(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTaskKiller::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTaskKiller)
    m_bShow = false;
    m_Pnt = CPoint(0, 0);
    //}}AFX_DATA_INIT
}


void CDlgTaskKiller::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTaskKiller)
    DDX_Control(pDX, IDC_BTN_SETOK, m_BtnSetOk);
    DDX_Control(pDX, IDC_BTN_SETCANCEL, m_BtnSetCancel);
    DDX_Control(pDX, IDC_EDIT_KILLNAME, m_EditKillName);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTaskKiller, CDialog)
    //{{AFX_MSG_MAP(CDlgTaskKiller)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_SETCANCEL, OnBtnSetcancel)
    ON_BN_CLICKED(IDC_BTN_SETOK, OnBtnSetok)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskKiller message handlers

void CDlgTaskKiller::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog72",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetOk.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetCancel.Show(m_Pnt.x, m_Pnt.y);
        m_EditKillName.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgTaskKiller::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgTaskKiller::EnableWindow(false);
}

LRESULT CDlgTaskKiller::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            m_EditKillName.SetFocus();
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
            CDlgTaskKiller::OnBtnSetcancel();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTaskKiller::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTaskKiller::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgTaskKiller::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnSetOk.Init(rect.left, rect.top, "Button76");
    m_BtnSetCancel.Init(rect.left, rect.top, "Button77");
    m_EditKillName.Init(rect.left, rect.top);
    m_EditKillName.SetLimitText(_MAX_NAMESIZE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTaskKiller::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTaskKiller::ReSetDlg()
{
    m_bSetOk = FALSE;
    m_EditKillName.SetWindowText("");
    strcpy(m_szWantKiller, "");
}

void CDlgTaskKiller::OnBtnSetcancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgTaskKiller::EnableWindow(false);
}

void CDlgTaskKiller::OnBtnSetok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    m_EditKillName.GetWindowText(m_szWantKiller, 16);
    if (m_szWantKiller && strlen(m_szWantKiller) > 0)
    {
        m_bSetOk = true;
        CDlgTaskKiller::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKSET_OK, 1);
        CDlgTaskKiller::EnableWindow(false);
    }
    else
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10624));	//"请输入要追杀的玩家的名字!"
    }
}

char* CDlgTaskKiller::GetWantKiller()
{
    return m_szWantKiller;
}

BOOL CDlgTaskKiller::IsSetOk()
{
    return m_bSetOk;
}
