
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgEmotion.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgEmotion.h"

#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int _XAMOUNT = 7;
const int _YAMOUNT = 3;
const int _PAGEAMOUNT = 21;
/////////////////////////////////////////////////////////////////////////////
// CDlgEmotion dialog


CDlgEmotion::CDlgEmotion(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEmotion::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEmotion)
    m_bShow = FALSE;
    m_nCurPage = 0;
    m_nCurSelEmotion = -1;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgEmotion::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEmotion)
    DDX_Control(pDX, IDC_STA_PAGE, m_StaPage);
    DDX_Control(pDX, IDC_BTN_PAGERIGHT, m_BtnPageRight);
    DDX_Control(pDX, IDC_BTN_PAGELEFT, m_BtnPageLeft);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEmotion, CDialog)
    //{{AFX_MSG_MAP(CDlgEmotion)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_PAGELEFT, OnBtnPageleft)
    ON_BN_CLICKED(IDC_BTN_PAGERIGHT, OnBtnPageright)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEmotion message handlers

HBRUSH CDlgEmotion::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode (TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgEmotion::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgEmotion::GetWindowRect (rect);
    m_BtnPageLeft.Init (rect.left, rect.top, "Button580");
    m_BtnPageRight.Init (rect.left, rect.top, "Button581");
    m_StaPage.Init(rect.left, rect.top);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEmotion::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

LRESULT CDlgEmotion::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        char szPage[16] = "";
        sprintf(szPage, "%d/%d", m_nCurPage + 1, (g_objGameDataSet.GetEmotionAmount () - 1) / _PAGEAMOUNT + 1);
        m_StaPage.SetWindowText (szPage);
        if (wParam)
        {
            DXPlaySound ( "Sound/Open.wav" ) ;
        }
        else
        {
            DXPlaySound ( "Sound/window_close.wav" ) ;
        }
    }
    else if (message == WM_COMMAND)
    {
        if (wParam == 1 || wParam == 2)
        {
            return true;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgEmotion::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog85",
                         EXIGENCE_IMMEDIATE ) ;
        if (pShowAni != NULL)
        {
            pShowAni->Show (0, m_Pnt.x, m_Pnt.y);
        }
        else
        {
            return;
        }
        m_BtnPageLeft.Show (m_Pnt.x, m_Pnt.y);
        m_BtnPageRight.Show (m_Pnt.x, m_Pnt.y);
        m_StaPage.Show(m_Pnt.x, m_Pnt.y);
        m_nCurSelEmotion = g_objGameMsg.ShowEmotionWnd (m_Pnt.x + 2, m_Pnt.y + 3, m_nCurPage, _XAMOUNT, _YAMOUNT);
    }
}



void CDlgEmotion::OnBtnPageleft()
{
    // TODO: Add your control notification handler code here
    if (m_nCurPage > 0)
    {
        m_nCurPage --;
    }
    char szPage[16] = "";
    sprintf(szPage, "%d/%d", m_nCurPage + 1, (g_objGameDataSet.GetEmotionAmount () - 1) / _PAGEAMOUNT + 1);
    m_StaPage.SetWindowText (szPage);
}

void CDlgEmotion::OnBtnPageright()
{
    // TODO: Add your control notification handler code here
    if (m_nCurPage < (g_objGameDataSet.GetEmotionAmount () - 1) / _PAGEAMOUNT)
    {
        m_nCurPage++;
    }
    char szPage[16] = "";
    sprintf(szPage, "%d/%d", m_nCurPage + 1, (g_objGameDataSet.GetEmotionAmount () - 1) / _PAGEAMOUNT + 1);
    m_StaPage.SetWindowText (szPage);
}

int CDlgEmotion::GetCurSelEmotion()
{
    return m_nCurSelEmotion;
}

void CDlgEmotion::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_nCurSelEmotion != -1)
    {
        CDlgEmotion::GetParent()->PostMessage (WM_MY_MESSAGE, ON_EMOTION_INSERT, m_nCurSelEmotion);
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgEmotion::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LPARAM lParam ;
    lParam = MAKELONG( point.x + m_Pnt.x, point.y + m_Pnt.y ) ;
    CDlgEmotion::GetParent()->PostMessage( WM_MOUSEMOVE, nFlags, lParam ) ;
    CDialog::OnMouseMove(nFlags, point);
}
