
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgTaskItem.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTaskItem.h"

#include "Item.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskItem dialog


CDlgTaskItem::CDlgTaskItem(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTaskItem::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTaskItem)
    m_bShow = false;
    m_bSetOk = false;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    m_nItemAmount = 0;
    m_nItemMark = 0;
    m_nItemSoul = 0;
    //}}AFX_DATA_INIT
}


void CDlgTaskItem::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTaskItem)
    DDX_Control(pDX, IDC_EDIT_ITEMSOUL, m_EditItemSoul);
    DDX_Control(pDX, IDC_EDIT_ITEMNAME, m_EditItemName);
    DDX_Control(pDX, IDC_EDIT_ITEMMARK, m_EditItemMark);
    DDX_Control(pDX, IDC_EDIT_ITEMAMOUNT, m_EditItemAmount);
    DDX_Control(pDX, IDC_BTN_SETOK, m_BtnSetOk);
    DDX_Control(pDX, IDC_BTN_SETCANCEL, m_BtnSetCancel);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    DDX_Text(pDX, IDC_EDIT_ITEMAMOUNT, m_nItemAmount);
    DDX_Text(pDX, IDC_EDIT_ITEMMARK, m_nItemMark);
    DDX_Text(pDX, IDC_EDIT_ITEMSOUL, m_nItemSoul);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTaskItem, CDialog)
    //{{AFX_MSG_MAP(CDlgTaskItem)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_SETOK, OnBtnSetok)
    ON_BN_CLICKED(IDC_BTN_SETCANCEL, OnBtnSetcancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTaskItem message handlers

void CDlgTaskItem::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog73",
                         EXIGENCE_IMMEDIATE ) ;
        if ( pShowAni != NULL )
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetOk.Show(m_Pnt.x, m_Pnt.y);
        m_BtnSetCancel.Show(m_Pnt.x, m_Pnt.y);
        m_EditItemAmount.Show(m_Pnt.x, m_Pnt.y);
        m_EditItemName.Show(m_Pnt.x, m_Pnt.y);
        m_EditItemSoul.Show(m_Pnt.x, m_Pnt.y);
        m_EditItemMark.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgTaskItem::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgTaskItem::EnableWindow(false);
}

LRESULT CDlgTaskItem::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch ( message )
    {
    case WM_ENABLE:
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            m_EditItemName.SetFocus();
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
            return true ;
        }
        break ;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTaskItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTaskItem::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgTaskItem::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnSetOk.Init(rect.left, rect.top, "Button76");
    m_BtnSetCancel.Init(rect.left, rect.top, "Button77");
    m_EditItemAmount.Init(rect.left, rect.top);
    m_EditItemAmount.SetLimitText(4);
    m_EditItemName.Init(rect.left, rect.top);
    m_EditItemName.SetLimitText(_MAX_NAMESIZE);
    m_EditItemMark.Init(rect.left, rect.top);
    m_EditItemMark.SetLimitText(4);
    m_EditItemSoul.Init(rect.left, rect.top);
    m_EditItemSoul.SetLimitText(4);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTaskItem::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTaskItem::ReSetDlg()
{
    m_EditItemAmount.SetWindowText("");
    m_EditItemName.SetWindowText("");
    m_EditItemMark.SetWindowText("");
    m_EditItemSoul.SetWindowText("");
}

void CDlgTaskItem::OnBtnSetok()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    m_bSetOk = true;
    UpdateData(true);
    char strItemName[16] = "";
    m_EditItemName.GetWindowText(strItemName, 16);
    if (CItem::s_ItemData.CheckItem(strItemName))
    {
        CDlgTaskItem::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TASKSET_OK, 2);
    }
    else
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10616));    //"没有这个物品!请重新输入!"
    }
    CDlgTaskItem::EnableWindow(false);
}

void CDlgTaskItem::OnBtnSetcancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    m_bSetOk = false;
    CDlgTaskItem::EnableWindow(false);
}

BOOL CDlgTaskItem::IsSetOk()
{
    return m_bSetOk;
}
