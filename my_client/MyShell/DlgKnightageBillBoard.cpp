
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgKnightageBillBoard.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgKnightageBillBoard.h"

#include "Hero.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageBillBoard dialog


CDlgKnightageBillBoard::CDlgKnightageBillBoard(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgKnightageBillBoard::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgKnightageBillBoard)
    m_Pnt = CPoint(0, 0);
    m_bShow = false;
    //}}AFX_DATA_INIT
}


void CDlgKnightageBillBoard::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgKnightageBillBoard)
    DDX_Control(pDX, IDC_STA_WRITER, m_StaWriter);
    DDX_Control(pDX, IDC_STA_TIME, m_StaTime);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_BTN_BILLBOARD, m_BtnBillBoard);
    DDX_Control(pDX, IDC_EDT_BILLBOARD, m_BillBoardEdt);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKnightageBillBoard, CDialog)
    //{{AFX_MSG_MAP(CDlgKnightageBillBoard)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_BILLBOARD, OnBtnBillboard)
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
    ON_EN_SETFOCUS(IDC_EDT_BILLBOARD, OnSetfocusEdtBillboard)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageBillBoard message handlers

LRESULT CDlgKnightageBillBoard::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow)
        {
            CDlgKnightageBillBoard::SetBillBoard();
        }
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1)
        {
            return true;
        }
        else if (wParam == 2)
        {
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgKnightageBillBoard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgKnightageBillBoard::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgKnightageBillBoard::GetWindowRect(rect);
    // TODO: Add extra initialization here
    m_BillBoardEdt.Init(rect.left, rect.top, true);
    m_BillBoardEdt.SetLimitText(255);
    m_StaTime.Init(rect.left, rect.top);
    m_StaTime.SetStaticPosType(2);
    m_StaWriter.Init(rect.left, rect.top);
    m_StaWriter.SetStaticPosType(2);
    m_BtnBillBoard.Init(rect.left, rect.top, "Button86");
    m_BtnCancel.Init(rect.left, rect.top, "Button87");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKnightageBillBoard::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgKnightageBillBoard::Show(int x, int y)
{
    if (m_bShow)
    {
        m_BillBoardEdt.Show(m_Pnt.x + x, m_Pnt.y + y);
        if (m_BtnBillBoard.IsWindowEnabled())
        {
            m_BtnBillBoard.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        if (m_BtnCancel.IsWindowEnabled())
        {
            m_BtnCancel.Show(m_Pnt.x + x, m_Pnt.y + y);
        }
        m_StaTime.Show(m_Pnt.x + x, m_Pnt.y + y);
        m_StaWriter.Show(m_Pnt.x + x, m_Pnt.y + y);
    }
}

void CDlgKnightageBillBoard::OnBtnBillboard()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    if (g_objHero.IsSynLeader())
    {
        char strAnnouce[256];
        m_BillBoardEdt.GetWindowText(strAnnouce, 256);
        g_objHero.SendSyndicateAnnounce(strAnnouce);
    }
}

void CDlgKnightageBillBoard::OnBtnCancel()
{
    // TODO: Add your control notification handler code here
    DXPlaySound ( "Sound/Dlg_ok.wav" ) ;
    CDlgKnightageBillBoard::SetBillBoard();
}

void CDlgKnightageBillBoard::SetBillBoard()
{
    m_BillBoardEdt.SetWindowText("");
    m_StaTime.SetWindowText("");
    m_StaWriter.SetWindowText("");
    m_BillBoardEdt.EnableWindow(false);
    m_BtnBillBoard.EnableWindow(false);
    m_BtnCancel.EnableWindow(false);
    if (g_objHero.GetSydicateID())
    {
        char szAnnounce[256] = "";
        strcpy(szAnnounce, g_objHero.GetSyndicateAnnounce());
        if (szAnnounce && strlen(szAnnounce) > 0)
        {
            m_BillBoardEdt.SetWindowText(szAnnounce);
            char szWriter[64] = "";
            sprintf(szWriter, g_objGameDataSet.GetStr(10856), g_objHero.GetSyndicateLeaderName());
            m_StaWriter.SetWindowText(szWriter);
            m_StaTime.SetWindowText(g_objHero.GetSyndicateAnnounceTime());
        }
        if (g_objHero.IsSynLeader())
        {
            m_BillBoardEdt.EnableWindow();
        }
    }
}

void CDlgKnightageBillBoard::OnSetfocusEdtBillboard()
{
    // TODO: Add your control notification handler code here
    if (g_objHero.IsSynLeader())
    {
        m_BillBoardEdt.SetFocus();
        m_BtnBillBoard.EnableWindow();
        m_BtnCancel.EnableWindow();
    }
}
