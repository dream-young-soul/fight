
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DlgSchoolList.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgSchoolList.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSchoolList dialog


CDlgSchoolList::CDlgSchoolList(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSchoolList::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSchoolList)
    //}}AFX_DATA_INIT
}


void CDlgSchoolList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSchoolList)
    DDX_Control(pDX, IDC_LIST_SECSCHOOL, m_ListSecSchool);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_BtnClose);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSchoolList, CDialog)
    //{{AFX_MSG_MAP(CDlgSchoolList)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSchoolList message handlers

void CDlgSchoolList::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgSchoolList::EnableWindow(false);
}

HBRUSH CDlgSchoolList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgSchoolList::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CDlgSchoolList::GetWindowRect(rect);
    m_BtnClose.Init(rect.left, rect.top, "Button540");
    m_ListSecSchool.Init(rect.left, rect.top, NULL, 2);
    m_ListSecSchool.SetParentCob(this);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSchoolList::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

LRESULT CDlgSchoolList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch(message)
    {
    case WM_ENABLE:
        m_bShow = wParam;
        if (m_bShow)
        {
            DXPlaySound("Sound/Open.wav");
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
        break;
    case WM_COMMAND:
        if (wParam == 1 || wParam == 2)
        {
            return true;
        }
        break;
    default:
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgSchoolList::Show()
{
    if (m_bShow)
    {
        // Show the owner
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog50",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_Pnt.x,
                            m_Pnt.y  ) ;
        else
        {
            return ;
        }
        m_BtnClose.Show(m_Pnt.x, m_Pnt.y);
        m_ListSecSchool.Show(m_Pnt.x, m_Pnt.y);
    }
}

void CDlgSchoolList::SetSchoolList()
{
    m_ListSecSchool.ResetContent();
    int nAmount = g_objHero.GetSecSchoolMemberAmount();
    for(int i = 0; i < nAmount; i++)
    {
        m_ListSecSchool.InsertString(i, g_objHero.GetSecSchoolMemberByIndex(i));
        m_ListSecSchool.SetListRealIndex(1, i);
    }
}
