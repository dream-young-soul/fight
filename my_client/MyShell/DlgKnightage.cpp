
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgKnightage.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgKnightage.h"

#include "Hero.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightage dialog


CDlgKnightage::CDlgKnightage(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgKnightage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgKnightage)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;
    m_dwLastShowTime = 0;
    m_fFrameStep = 0.0f;
    m_nFrameMode = 0;
    m_PntX = 0;
    m_iCurDlg = 0;
    //}}AFX_DATA_INIT
}


void CDlgKnightage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgKnightage)
    DDX_Control(pDX, IDC_CHK_MEMBER, m_ChkMember);
    DDX_Control(pDX, IDC_CHK_ENEMY, m_ChkEnemy);
    DDX_Control(pDX, IDC_CHK_BILLBOARD, m_ChkBillBoard);
    DDX_Control(pDX, IDC_CHK_BASE, m_ChkBase);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKnightage, CDialog)
    //{{AFX_MSG_MAP(CDlgKnightage)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_CHK_BASE, OnChkBase)
    ON_BN_CLICKED(IDC_CHK_BILLBOARD, OnChkBillboard)
    ON_BN_CLICKED(IDC_CHK_ENEMY, OnChkEnemy)
    ON_BN_CLICKED(IDC_CHK_MEMBER, OnChkMember)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightage message handlers

LRESULT CDlgKnightage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (wParam == false)
        {
            DXPlaySound("Sound/window_close.wav");
            HideCurDlg(false);
        }
        else
        {
            DXPlaySound("Sound/Open.wav");
            m_dwLastShowTime = TimeGet() ;
            m_nFrameMode = 1 ;
            m_PntX = _SCR_WIDTH ;
            if (m_iCurDlg != 0)
            {
                HideCurDlg(false);
                m_ChkBase.SetCheckValue(1);
                m_iCurDlg = 0;
            }
            HideCurDlg(true);
            //			SetDlgMode();
        }
    }
    if (message == WM_COMMAND)
    {
        if ( wParam == 1 )
        {
            return true ;
        }
        else if ( wParam == 2 )
        {
            CDlgKnightage::OnBtnClose () ;
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgKnightage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgKnightage::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgKnightage::GetWindowRect(rect);
    m_ChkBase.Init( rect.left, rect.top, "Check80", 2);
    m_ChkMember.Init(rect.left, rect.top, "Check81", 2);
    m_ChkBillBoard.Init(rect.left, rect.top, "Check83", 2);
    m_ChkEnemy.Init(rect.left, rect.top, "Check85", 2);
    m_CloseBtn.Init(rect.left, rect.top, "Button540");
    m_fFrameStep = ( float ) (rect.Width() + 100) / DLG_FRAME_AMOUNT ;
    m_DlgKnightageBase.Create( IDD_DIALOG_KNIGHTAGE_BASE, this);
    m_DlgKnightageBase.ShowWindow(SW_SHOW);
    m_DlgKnightageBase.EnableWindow(false);
    m_DlgKnightageEnemy.Create( IDD_DIALOG_KNIGHTAGE_ENEMY, this);
    m_DlgKnightageEnemy.ShowWindow(SW_SHOW);
    m_DlgKnightageEnemy.EnableWindow(false);
    m_DlgKnightageMemList.Create( IDD_DIALOG_KNIGHTAGE_MEMBER, this);
    m_DlgKnightageMemList.ShowWindow(SW_SHOW);
    m_DlgKnightageMemList.EnableWindow(false);
    m_DlgKnightageBillBoard.Create(IDD_DIALOG_KNIGHTAGE_BILLBOARD, this);
    m_DlgKnightageBillBoard.ShowWindow(SW_SHOW);
    m_DlgKnightageBillBoard.EnableWindow(false);
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKnightage::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_PntX = x;
    m_Pnt.y = y;
}

void CDlgKnightage::Show()
{
    if (m_bShow)
    {
        if ( m_nFrameMode != 0 )
        {
            if ( TimeGet() - m_dwLastShowTime <= 300 )
            {
                m_Pnt.x =(LONG)( m_PntX - ( TimeGet() - m_dwLastShowTime ) / 15 * m_fFrameStep);
            }
            else
            {
                CRect rect;
                CDlgKnightage::GetWindowRect(rect);
                m_Pnt.x = _SCR_WIDTH - rect.Width() - 100;
                m_nFrameMode = 0 ;
            }
        }
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog8",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( m_iCurDlg, m_Pnt.x, m_Pnt.y);
            if (g_objHero.GetSydicateID() != ID_NONE)
            {
                ShowAni->Show ( 4, m_Pnt.x + 282 , m_Pnt.y + 80);
                ShowAni->Show ( 5, m_Pnt.x + 282, m_Pnt.y + 144 + 80);
            }
        }
        else
        {
            return ;
        }
        if (g_objHero.GetSydicateID() != ID_NONE)
        {
            m_ChkBase.Show(m_Pnt.x, m_Pnt.y);
            m_ChkBillBoard.Show(m_Pnt.x, m_Pnt.y);
            m_ChkEnemy.Show(m_Pnt.x, m_Pnt.y);
            m_ChkMember.Show(m_Pnt.x, m_Pnt.y);
        }
        m_CloseBtn.Show(m_Pnt.x, m_Pnt.y);
        ShowChildDlg();
    }
}

void CDlgKnightage::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgKnightage::EnableWindow(false);
}

void CDlgKnightage::ShowChildDlg()
{
    switch(m_iCurDlg)
    {
    case 0:
        m_DlgKnightageBase.Show(m_Pnt.x, m_Pnt.y);
        break;
    case 1:
        m_DlgKnightageMemList.Show(m_Pnt.x, m_Pnt.y);
        break;
    case 2:
        m_DlgKnightageBillBoard.Show(m_Pnt.x, m_Pnt.y);
        break;
    case 3:
        m_DlgKnightageEnemy.Show(m_Pnt.x, m_Pnt.y);
        break;
    default:
        break;
    }
}

void CDlgKnightage::HideCurDlg(BOOL flag)
{
    if (g_objHero.GetSydicateID() == ID_NONE)
    {
        m_iCurDlg = 0;
        m_ChkBase.SetCheckValue(flag);
        m_DlgKnightageBase.EnableWindow(flag);
        return;
    }
    switch(m_iCurDlg)
    {
    case 0:
        m_ChkBase.SetCheckValue(flag);
        m_DlgKnightageBase.EnableWindow(flag);
        break;
    case 1:
        m_ChkMember.SetCheckValue(flag);
        m_DlgKnightageMemList.EnableWindow(flag);
        break;
    case 2:
        m_ChkBillBoard.SetCheckValue(flag);
        m_DlgKnightageBillBoard.EnableWindow(flag);
        break;
    case 3:
        m_ChkEnemy.SetCheckValue(flag);
        m_DlgKnightageEnemy.EnableWindow(flag);
        break;
    default:
        break;
    }
}



void CDlgKnightage::OnChkBase()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    if (g_objHero.GetSydicateID() != ID_NONE)
    {
        HideCurDlg(false);
        m_iCurDlg = 0;
        HideCurDlg(true);
    }
    else
    {
        m_ChkBase.SetCheckValue(0);
    }
}

void CDlgKnightage::OnChkBillboard()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    if (g_objHero.GetSydicateID() != ID_NONE)
    {
        HideCurDlg(false);
        m_iCurDlg = 2;
        HideCurDlg(true);
    }
    else
    {
        m_ChkBillBoard.SetCheckValue(0);
    }
}

void CDlgKnightage::OnChkEnemy()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    if (g_objHero.GetSydicateID() != ID_NONE)
    {
        HideCurDlg(false);
        m_iCurDlg = 3;
        HideCurDlg(true);
    }
    else
    {
        m_ChkEnemy.SetCheckValue(0);
    }
}


void CDlgKnightage::OnChkMember()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_ok.wav");
    if (g_objHero.GetSydicateID() != ID_NONE)
    {
        HideCurDlg(false);
        m_iCurDlg = 1;
        HideCurDlg(true);
    }
    else
    {
        m_ChkMember.SetCheckValue(0);
    }
}


void CDlgKnightage::FlashKnightage()
{
    if (g_objHero.GetSydicateID() == ID_NONE)
    {
        m_ChkBase.EnableWindow(false);
        m_ChkMember.EnableWindow(false);
        m_ChkBillBoard.EnableWindow(false);
        m_ChkEnemy.EnableWindow(false);
    }
    else
    {
        m_ChkBase.EnableWindow();
        m_ChkMember.EnableWindow();
        m_ChkBillBoard.EnableWindow();
        m_ChkEnemy.EnableWindow();
    }
    switch(m_iCurDlg)
    {
    case 0:
        m_DlgKnightageBase.SetKnightBaseData();
        break;
    case 1:
        g_objHero.QuerySynMemberList(0);
        break;
    case 2:
        m_DlgKnightageBillBoard.SetBillBoard();
        break;
    case 3:
        if (m_DlgKnightageEnemy.m_ListMode == 1)
        {
            m_DlgKnightageEnemy.SetEnemyList();
        }
        else
        {
            m_DlgKnightageEnemy.SetFriendList();
        }
        break;
    default:
        break;
    }
}
