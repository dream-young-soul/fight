
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgTboardList.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgTboardList.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTboardList dialog


CDlgTboardList::CDlgTboardList(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTboardList::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTboardList)
    m_bShow = false;
    m_Pnt = CPoint(0, 0); // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgTboardList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTboardList)
    DDX_Control(pDX, IDC_LIST_TBOARD, m_ListTboard);
    DDX_Control(pDX, IDC_BTN_UPPAGE, m_BtnUppage);
    DDX_Control(pDX, IDC_BTN_DOWNPAGE, m_BtnDownPage);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTboardList, CDialog)
    //{{AFX_MSG_MAP(CDlgTboardList)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_BTN_DOWNPAGE, OnBtnDownpage)
    ON_BN_CLICKED(IDC_BTN_UPPAGE, OnBtnUppage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTboardList message handlers

LRESULT CDlgTboardList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
    {
        m_bShow = wParam ;
        if (m_bShow)
        {
            m_ListTboard.ResetContent();
            DXPlaySound("Sound/Open.wav");
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
            CDlgTboardList::OnBtnClose();
            return true ;
        }
    }
    else if (message == LB_SETSEL)
    {
        if ( wParam == 0 )
        {
            if ( lParam != -1 )
            {
                ST_ANNOUNCE_TITLE* pAnnounce = g_objHero.GetAnnounceByIndex(lParam);
                if (pAnnounce != NULL)
                {
                    g_objHero.QueryTeacherBoardInfo(pAnnounce->idAnnounce);
                    CDlgTboardList::GetParent()->PostMessage(WM_MY_MESSAGE, ON_ANNOUNCEINFO_OPEN);
                }
            }
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgTboardList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

BOOL CDlgTboardList::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgTboardList::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_BtnUppage.Init(rect.left, rect.top, "Button161");
    m_BtnDownPage.Init(rect.left, rect.top, "Button162");
    m_ListTboard.Init(rect.left, rect.top, NULL, 2);
    m_ListTboard.SetParentCob(this);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTboardList::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgTboardList::Show()
{
    if (m_bShow)
    {
        CAni* pShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                         "Dialog77",
                         EXIGENCE_IMMEDIATE );
        if (pShowAni != NULL)
        {
            pShowAni->Show(0, m_Pnt.x, m_Pnt.y);
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_ListTboard.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgTboardList::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    DXPlaySound("Sound/window_close.wav");
    CDlgTboardList::EnableWindow(false);
}

void CDlgTboardList::OnBtnDownpage()
{
    // TODO: Add your control notification handler code here
}

void CDlgTboardList::OnBtnUppage()
{
    // TODO: Add your control notification handler code here
}

void CDlgTboardList::SetTboardList()
{
    m_ListTboard.ResetContent();
    int nAmount = g_objHero.GetAnnounceAmount();
    for(int i = 0; i < nAmount; i++)
    {
        ST_ANNOUNCE_TITLE* pAnnounceTitle = g_objHero.GetAnnounceByIndex(i);
        m_ListTboard.InsertString(i, pAnnounceTitle->szTitle);
        m_ListTboard.SetListRealIndex(i, 1);
    }
}
