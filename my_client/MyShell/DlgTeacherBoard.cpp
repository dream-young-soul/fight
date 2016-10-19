
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTeacherBoard.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTeacherBoard.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherBoard dialog


CDlgTeacherBoard::CDlgTeacherBoard(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTeacherBoard::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTeacherBoard)
    m_bShow = false;
    m_Pnt = CPoint(0, 0);	// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgTeacherBoard::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTeacherBoard)
    DDX_Control(pDX, IDC_EDIT_TITLE, m_EditTitle);
    DDX_Control(pDX, IDC_EDIT_INFO, m_EditInfo);
    DDX_Control(pDX, IDC_BTN_UPBOARD, m_BtnUpBoard);
    DDX_Control(pDX, IDC_BTN_HELP, m_BtnHelp);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTeacherBoard, CDialog)
    //{{AFX_MSG_MAP(CDlgTeacherBoard)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
    ON_BN_CLICKED(IDC_BTN_UPBOARD, OnBtnUpboard)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherBoard message handlers

LRESULT CDlgTeacherBoard::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
            m_EditInfo.SetWindowText("");
            m_EditTitle.SetWindowText("");
            m_EditTitle.SetFocus();
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
            CDlgTeacherBoard::OnBtnClose();
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTeacherBoard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTeacherBoard::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect	rect;
    CDlgTeacherBoard::GetWindowRect(rect);
    m_EditTitle.Init(rect.left, rect.top);
    m_EditTitle.SetLimitText(16);
    m_EditInfo.Init(rect.left, rect.top, true);
    m_EditInfo.SetLimitText(128);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnUpBoard.Init(rect.left, rect.top, "Button1712");
    m_BtnHelp.Init(rect.left, rect.top, "Button15");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTeacherBoard::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTeacherBoard::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog79",
                         EXIGENCE_IMMEDIATE );
        if (pShowAni != NULL)
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_BtnHelp.Show(m_Pnt.x, m_Pnt.y);
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_BtnUpBoard.Show(m_Pnt.x, m_Pnt.y);
        m_EditInfo.Show(m_Pnt.x, m_Pnt.y);
        m_EditTitle.Show(m_Pnt.x, m_Pnt.y);
    }
}



void CDlgTeacherBoard::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgTeacherBoard::EnableWindow(false);
}

void CDlgTeacherBoard::OnBtnHelp()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
}

void CDlgTeacherBoard::OnBtnUpboard()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/Dlg_Ok.wav");
    char szTitle[32] = "";
    char szInfo[128] = "";
    m_EditTitle.GetWindowText(szTitle, 32);
    m_EditInfo.GetWindowText(szInfo, 128);
    if (szTitle && strlen(szTitle) > 0
            && szInfo && strlen(szInfo) > 0 )
    {
        g_objHero.UpTeacherBoard(szTitle, szInfo);
        CDlgTeacherBoard::EnableWindow(false);
    }
    else
    {
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(10642));
    }
}
