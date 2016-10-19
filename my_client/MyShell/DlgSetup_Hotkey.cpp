
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgSetup_Hotkey.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgSetup_Hotkey.h"
#include "MyShellDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Hotkey dialog


CDlgSetup_Hotkey::CDlgSetup_Hotkey(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSetup_Hotkey::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetup_Hotkey)
    // NOTE: the ClassWizard will add member initialization here
    m_bShow = false;
    m_Pnt = CPoint( 0, 0 );
    m_nListIndex = 0;
    //}}AFX_DATA_INIT
}


void CDlgSetup_Hotkey::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetup_Hotkey)
    DDX_Control(pDX, IDC_BTN_UP, m_BtnUp);
    DDX_Control(pDX, IDC_BTN_DOWN, m_BtnDown);
    DDX_Control(pDX, IDC_LIST_HOTKEYN, m_ListHotKeyN);
    DDX_Control(pDX, IDC_LIST_HOTKEYM, m_ListHotKeyM);
    DDX_Control(pDX, IDC_BTN_SETKEY, m_BtnSetKey);
    DDX_Control(pDX, IDC_BTN_RESETKEYS, m_BtnResetKey);
    DDX_Control(pDX, IDC_STA_KEY, m_StaKey);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup_Hotkey, CDialog)
    //{{AFX_MSG_MAP(CDlgSetup_Hotkey)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_RESETKEYS, OnBtnResetkeys)
    ON_BN_CLICKED(IDC_BTN_SETKEY, OnBtnSetkey)
    ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
    ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Hotkey message handlers

void CDlgSetup_Hotkey::Show(int x, int y)
{
    if (m_bShow)
    {
        m_StaKey.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_ListHotKeyM.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_ListHotKeyN.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_BtnSetKey.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_BtnResetKey.Show(m_Pnt.x + x, m_Pnt.y + y);
        if (m_BtnUp.IsWindowEnabled())
        {
            m_BtnUp.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnDown.IsWindowEnabled())
        {
            m_BtnDown.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
    }
}

HBRUSH CDlgSetup_Hotkey::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgSetup_Hotkey::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgSetup_Hotkey::GetWindowRect(rect);
    m_BtnResetKey.Init(rect.left, rect.top, "Button321");
    m_BtnSetKey.Init(rect.left, rect.top, "Button322");
    m_BtnUp.Init(rect.left, rect.top, "Button815");
    m_BtnDown.Init(rect.left, rect.top, "Button816");
    m_ListHotKeyM.Init(rect.left, rect.top);
    m_ListHotKeyM.SetFontColor(0xffffff);
    m_ListHotKeyM.SetParentCob ( this ) ;
    m_ListHotKeyN.Init(rect.left, rect.top);
    m_ListHotKeyN.SetFontColor(0xffffff);
    m_ListHotKeyN.SetParentCob ( this ) ;
    m_StaKey.Init(rect.left, rect.top);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDlgSetup_Hotkey::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (m_bShow)
        {
            CDlgSetup_Hotkey::SetHotKeyList();
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            return true ;
        }
    }
    else if ( message == LB_SETSEL)
    {
        if ( wParam == 0 || wParam == -1)
        {
            char strHotKey[32] = "";
            m_nHotKeyIndex = lParam;
            m_ListHotKeyN.GetText(lParam, strHotKey);
            m_StaKey.SetWindowText(strHotKey);
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgSetup_Hotkey::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_Pnt.x = x;
    m_Pnt.y = y;
    // TODO: Add your message handler code here
}

void CDlgSetup_Hotkey::OnBtnResetkeys()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}

void CDlgSetup_Hotkey::OnBtnSetkey()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}

void CDlgSetup_Hotkey::SetHotKeyList()
{
    m_ListHotKeyM.ResetContent();
    m_ListHotKeyN.ResetContent();
    int nAmount = 11;
    if (m_nListIndex + 7 < nAmount)
    {
        nAmount = m_nListIndex + 7;
    }
    if (m_nListIndex > 0 )
    {
        m_BtnUp.EnableWindow();
    }
    else
    {
        m_BtnUp.EnableWindow(false);
    }
    if (m_nListIndex + 7 < nAmount)
    {
        m_BtnDown.EnableWindow();
    }
    else
    {
        m_BtnDown.EnableWindow(false);
    }
    for(int i = m_nListIndex; i < nAmount; i++)
    {
        m_ListHotKeyM.InsertString(i - m_nListIndex, (char*)g_objGameDataSet.GetStr(10299 + i));
        int nValue = CMyShellDlg::m_nHotkeyValue[i];
        if (nValue != -1)
        {
            char strHotkey[32] = "" ;
            switch ( nValue / 100 )
            {
            case HOTKEYF_ALT:
                strcpy ( strHotkey, "ALT + " ) ;
                break ;
            case HOTKEYF_CONTROL:
                strcpy ( strHotkey, "CTRL + " ) ;
                break ;
            case HOTKEYF_SHIFT:
                strcpy ( strHotkey, "SHIFT + " ) ;
                break ;
            case HOTKEYF_CONTROL + HOTKEYF_ALT:
                strcpy ( strHotkey, "CTRL + ALT + " ) ;
                break ;
            case HOTKEYF_CONTROL + HOTKEYF_SHIFT:
                strcpy ( strHotkey, "CTRL + SHIFT + " ) ;
                break ;
            case HOTKEYF_SHIFT + HOTKEYF_ALT:
                strcpy ( strHotkey, "SHIFT + ALT + " ) ;
                break ;
            }
            int nLen = strlen ( strHotkey ) ;
            strHotkey[ nLen ] = nValue % 100 ;
            strHotkey[ nLen + 1 ] = '\0' ;
            m_ListHotKeyN.InsertString(i - m_nListIndex, strHotkey);
        }
        else
        {
            m_ListHotKeyN.InsertString(i - m_nListIndex, g_objGameDataSet.GetStr(100122));
        }
    }
}



void CDlgSetup_Hotkey::OnBtnDown()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    if (m_nListIndex + 7 < 8)
    {
        m_nListIndex += 7;
        SetHotKeyList();
    }
}

void CDlgSetup_Hotkey::OnBtnUp()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/up_Page.wav");
    if (m_nListIndex - 7 >= 0)
    {
        m_nListIndex -= 7;
        SetHotKeyList();
    }
}
