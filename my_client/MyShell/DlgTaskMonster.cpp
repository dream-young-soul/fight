
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTaskMonster.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTaskMonster.h"

#include "GameMsg.h"
#include "Item.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskMonster dialog


CDlgTaskMonster::CDlgTaskMonster(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTaskMonster::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTaskMonster)
    m_bShow = false;
    m_bSetOk = false;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    m_nMonsterGrow = 0;
    m_nMonsterLev = 0;
    //}}AFX_DATA_INIT
}


void CDlgTaskMonster::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTaskMonster)
    DDX_Control(pDX, IDC_EDIT_MONSTERTYPE, m_EditMonsterType);
    DDX_Control(pDX, IDC_EDIT_MONSTERLEV, m_EditMonsterLev);
    DDX_Control(pDX, IDC_EDIT_MONSTERGROW, m_EditMonsterGrow);
    DDX_Control(pDX, IDC_BTN_SETOK, m_BtnSetOk);
    DDX_Control(pDX, IDC_BTN_SETCANCEL, m_BtnSetCancel);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Text(pDX, IDC_EDIT_MONSTERGROW, m_nMonsterGrow);
    DDX_Text(pDX, IDC_EDIT_MONSTERLEV, m_nMonsterLev);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTaskMonster, CDialog)
    //{{AFX_MSG_MAP(CDlgTaskMonster)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_SETCANCEL, OnBtnSetcancel)
    ON_BN_CLICKED(IDC_BTN_SETOK, OnBtnSetok)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskMonster message handlers

void CDlgTaskMonster::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog74",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetOk.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetCancel.Show(m_Pnt.x, m_Pnt.y);
        m_EditMonsterLev.Show(m_Pnt.x, m_Pnt.y);
        m_EditMonsterGrow.Show(m_Pnt.x, m_Pnt.y);
        m_EditMonsterType.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgTaskMonster::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgTaskMonster::EnableWindow(false);
}

LRESULT CDlgTaskMonster::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            m_EditMonsterType.SetFocus();
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
            CDlgTaskMonster::OnBtnSetcancel();
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTaskMonster::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTaskMonster::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgTaskMonster::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnSetOk.Init(rect.left, rect.top, "Button76");
    m_BtnSetCancel.Init(rect.left, rect.top, "Button77");
    m_EditMonsterLev.Init(rect.left, rect.top);
    m_EditMonsterLev.SetLimitText(4);
    m_EditMonsterGrow.Init(rect.left, rect.top);
    m_EditMonsterGrow.SetLimitText(4);
    m_EditMonsterType.Init(rect.left, rect.top);
    m_EditMonsterType.SetLimitText(16);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTaskMonster::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTaskMonster::ReSetDlg()
{
    m_EditMonsterLev.SetWindowText("");
    m_EditMonsterGrow.SetWindowText("");
    m_EditMonsterType.SetWindowText("");
}

void CDlgTaskMonster::OnBtnSetcancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgTaskMonster::EnableWindow(false);
}

void CDlgTaskMonster::OnBtnSetok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    char strWant[16] = "";
    m_EditMonsterType.GetWindowText(strWant, 16);
    if (strlen(strWant) <= 0)
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10617));
        return;
    }
    if (!CItem::s_ItemData.CheckItem(strWant))
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10618));
        return;
    }
    m_EditMonsterLev.GetWindowText(strWant, 16);
    if (strlen(strWant) <= 0)
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10619));
        return;
    }
    m_EditMonsterGrow.GetWindowText(strWant, 16);
    if (strlen(strWant) <= 0)
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10620));
        return;
    }
    m_bSetOk = true;
    UpdateData(true);
    CDlgTaskMonster::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKSET_OK, 3);
    CDlgTaskMonster::EnableWindow(false);
}

BOOL CDlgTaskMonster::IsSetOk()
{
    return m_bSetOk;
}
