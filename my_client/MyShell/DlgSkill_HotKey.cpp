
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgSkill_HotKey.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgSkill_HotKey.h"
#include "Hero.h"
#include "Magic.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSkill_HotKey dialog


CDlgSkill_HotKey::CDlgSkill_HotKey(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSkill_HotKey::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSkill_HotKey)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;
    m_ParentWnd = NULL;
    //}}AFX_DATA_INIT
}


void CDlgSkill_HotKey::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSkill_HotKey)
    DDX_Control(pDX, IDC_STATIC_MAGICDEC, m_StaMagicDec);
    DDX_Control(pDX, IDC_CHK_HOTKEYF5, m_HotKeyF5Chk);
    DDX_Control(pDX, IDC_STA_MAGICNAME, m_StaMagicName);
    DDX_Control(pDX, IDC_CHK_NONE, m_NoneChk);
    DDX_Control(pDX, IDC_CHK_HOTKEYF8, m_HotKeyF8Chk);
    DDX_Control(pDX, IDC_CHK_HOTKEYF7, m_HotKeyF7Chk);
    DDX_Control(pDX, IDC_CHK_HOTKEYF6, m_HotKeyF6Chk);
    DDX_Control(pDX, IDC_CHK_HOTKEYF4, m_HotKeyF4Chk);
    DDX_Control(pDX, IDC_CHK_HOTKEYF3, m_HotKeyF3Chk);
    DDX_Control(pDX, IDC_CHK_HOTKEYF2, m_HotKeyF2Chk);
    DDX_Control(pDX, IDC_CHK_HOTKEYF1, m_HotKeyF1Chk);
    DDX_Control(pDX, IDC_BTN_OK, m_HotKeyOkBtn);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSkill_HotKey, CDialog)
    //{{AFX_MSG_MAP(CDlgSkill_HotKey)
    ON_WM_MOVE()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF1, OnChkHotkeyf1)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF2, OnChkHotkeyf2)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF3, OnChkHotkeyf3)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF4, OnChkHotkeyf4)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF5, OnChkHotkeyf5)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF6, OnChkHotkeyf6)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF7, OnChkHotkeyf7)
    ON_BN_CLICKED(IDC_CHK_HOTKEYF8, OnChkHotkeyf8)
    ON_BN_CLICKED(IDC_CHK_NONE, OnChkNone)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSkill_HotKey message handlers

BOOL CDlgSkill_HotKey::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgSkill_HotKey::GetWindowRect(rect);
    // TODO: Add extra initialization here
    m_HotKeyF1Chk.Init(rect.left, rect.top, "Check601", 2);
    m_HotKeyF2Chk.Init(rect.left, rect.top, "Check602", 2);
    m_HotKeyF3Chk.Init(rect.left, rect.top, "Check603", 2);
    m_HotKeyF4Chk.Init(rect.left, rect.top, "Check604", 2);
    m_HotKeyF5Chk.Init(rect.left, rect.top, "Check605", 2);
    m_HotKeyF6Chk.Init(rect.left, rect.top, "Check606", 2);
    m_HotKeyF7Chk.Init(rect.left, rect.top, "Check607", 2);
    m_HotKeyF8Chk.Init(rect.left, rect.top, "Check608", 2);
    m_NoneChk.Init(rect.left, rect.top, "Check609", 2);
    m_StaMagicName.Init(rect.left, rect.top);
    m_StaMagicDec.Init(rect.left, rect.top);
    m_HotKeyOkBtn.Init(rect.left, rect.top, "Button820");
    m_CloseBtn.Init(rect.left, rect.top, "Button540");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSkill_HotKey::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x ;
    m_Pnt.y = y ;
}

HBRUSH CDlgSkill_HotKey::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

void CDlgSkill_HotKey::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog60",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show( 0,
                           m_Pnt.x ,
                           m_Pnt.y  );
        }
        else
        {
            return ;
        }
        m_HotKeyF1Chk.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyF2Chk.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyF3Chk.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyF4Chk.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyF5Chk.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyF6Chk.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyF7Chk.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyF8Chk.Show(m_Pnt.x, m_Pnt.y);
        m_NoneChk.Show(m_Pnt.x, m_Pnt.y);
        m_StaMagicName.Show(m_Pnt.x, m_Pnt.y);
        m_StaMagicDec.Show(m_Pnt.x, m_Pnt.y);
        m_HotKeyOkBtn.Show(m_Pnt.x, m_Pnt.y);
        m_CloseBtn.Show(m_Pnt.x, m_Pnt.y);
    }
}



LRESULT CDlgSkill_HotKey::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (wParam == TRUE)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
            PopCurCheck(m_iMagicHotKey, true);
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
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
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgSkill_HotKey::SetParentWnd(CWnd* pParentWnd)
{
    m_ParentWnd = pParentWnd ;
}

void CDlgSkill_HotKey::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgSkill_HotKey::EnableWindow(false);
}

void CDlgSkill_HotKey::OnBtnOk()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    CDlgSkill_HotKey::ReSetHotKeys();
    CDlgSkill_HotKey::GetParent()->PostMessage(WM_MY_MESSAGE, CMD_FLASH_MAGIC);
    CDlgSkill_HotKey::EnableWindow(false);
}

void CDlgSkill_HotKey::SetStaMagic(int iMagic)
{
    CMagic* pMagic = g_objHero.GetMagic(iMagic) ;
    if (pMagic != NULL)
    {
        m_iMagicIdType = pMagic->GetIDType();
        m_StaMagicName.SetWindowText( pMagic->GetName() );
        m_StaMagicDec.SetWindowText( pMagic->GetDescribe());
    }
}

void CDlgSkill_HotKey::PopCurCheck(int nIndex , BOOL bFlag)
{
    switch(nIndex)
    {
    case 0:
        m_HotKeyF1Chk.SetCheckValue(bFlag);
        break;
    case 1:
        m_HotKeyF2Chk.SetCheckValue(bFlag);
        break;
    case 2:
        m_HotKeyF3Chk.SetCheckValue(bFlag);
        break;
    case 3:
        m_HotKeyF4Chk.SetCheckValue(bFlag);
        break;
    case 4:
        m_HotKeyF5Chk.SetCheckValue(bFlag);
        break;
    case 5:
        m_HotKeyF6Chk.SetCheckValue(bFlag);
        break;
    case 6:
        m_HotKeyF7Chk.SetCheckValue(bFlag);
        break;
    case 7:
        m_HotKeyF8Chk.SetCheckValue(bFlag);
        break;
    case 8:
        m_NoneChk.SetCheckValue(bFlag);
        break;
    default:
        break;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf1()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF1Chk.GetCheckValue() == 1)
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 0;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf2()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF2Chk.GetCheckValue() == 1)
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 1;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf3()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF3Chk.GetCheckValue() == 1)
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 2;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf4()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF4Chk.GetCheckValue() == 1)
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 3;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf5()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF5Chk.GetCheckValue() == 1)
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 4;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf6()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF6Chk.GetCheckValue() == 1)
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 5;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf7()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF7Chk.GetCheckValue())
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 6;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkHotkeyf8()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    if (m_HotKeyF8Chk.GetCheckValue())
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 7;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::OnChkNone()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Clear.wav");
    if (m_NoneChk.GetCheckValue() == 1)
    {
        PopCurCheck(m_iMagicHotKey);
        m_iMagicHotKey = 8;
    }
    else
    {
        m_iMagicHotKey = 8;
    }
}

void CDlgSkill_HotKey::ReSetHotKeys()
{
    for(int i = 0 ; i < 8; i++)
    {
        if (m_HotKeyIdType[i] == m_iMagicIdType)
        {
            if (i != m_iMagicHotKey)
            {
                m_HotKeyIdType[i] = 0;
            }
        }
        else
        {
            if (i == m_iMagicHotKey )
            {
                m_HotKeyIdType[i] = m_iMagicIdType;
            }
        }
    }
}

void CDlgSkill_HotKey::SetMouseHotKey(int MagicType, int nHotKey)
{
    m_iMagicIdType = MagicType;
    m_iMagicHotKey = nHotKey;
    CDlgSkill_HotKey::ReSetHotKeys();
    CDlgSkill_HotKey::GetParent()->PostMessage(WM_MY_MESSAGE, CMD_FLASH_MAGIC);
}
