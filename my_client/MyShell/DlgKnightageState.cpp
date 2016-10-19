
//**********************************************************
// 代码编辑器
//**********************************************************

// DlgKnightageState.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "DlgKnightageState.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageState dialog


CDlgKnightageState::CDlgKnightageState(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgKnightageState::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgKnightageState)
    m_bShow = false;
    m_Pnt	= CPoint(0, 0);
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgKnightageState::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgKnightageState)
    DDX_Control(pDX, IDC_STA_SYNLEV, m_StaSynLev);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
    DDX_Control(pDX, IDC_STA_NUMBER, m_StaNumber);
    DDX_Control(pDX, IDC_STA_NAME, m_StaName);
    DDX_Control(pDX, IDC_STA_LEVEL, m_StaLevel);
    DDX_Control(pDX, IDC_STA_HEADNAME, m_StaHeadName);
    DDX_Control(pDX, IDC_STA_FAMOR, m_StaFamor);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKnightageState, CDialog)
    //{{AFX_MSG_MAP(CDlgKnightageState)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageState message handlers

LRESULT CDlgKnightageState::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (m_bShow == TRUE)
        {
            DXPlaySound("Sound/Open.wav");
            SetStateData();
        }
        else
        {
            DXPlaySound("Sound/window_close.wav");
        }
    }
    else if ( message == WM_COMMAND )
    {
        if ( wParam == 1 || wParam == 2 )
        {
            return true ;
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CDlgKnightageState::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Return a different brush if the default is not desired
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

BOOL CDlgKnightageState::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect;
    CDlgKnightageState::GetWindowRect(rect);
    // TODO: Add extra initialization here
    m_StaName.Init(rect.left, rect.top);
    m_StaName.SetStaticPosType(1);
    m_StaName.SetFontType(2);
    m_StaHeadName.Init(rect.left, rect.top);
    m_StaLevel.Init(rect.left, rect.top);
    m_StaFamor.Init(rect.left, rect.top);
    m_StaNumber.Init(rect.left, rect.top);
    m_StaSynLev.Init(rect.left, rect.top);
    m_StaSynLev.SetStaticPosType(1);
    m_StaSynLev.SetFontType(2);
    m_CloseBtn.Init(rect.left, rect.top, "Button540");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKnightageState::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CDlgKnightageState::Show()
{
    if (m_bShow)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                        "Dialog17",
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            ShowAni->Show ( 0, m_Pnt.x , m_Pnt.y );
        }
        else
        {
            return ;
        }
        m_StaName.Show(m_Pnt.x, m_Pnt.y );
        m_StaSynLev.Show(m_Pnt.x, m_Pnt.y );
        m_StaLevel.Show(m_Pnt.x, m_Pnt.y);
        m_StaHeadName.Show(m_Pnt.x, m_Pnt.y);
        m_StaFamor.Show(m_Pnt.x, m_Pnt.y);
        m_StaNumber.Show(m_Pnt.x, m_Pnt.y);
        m_CloseBtn.Show(m_Pnt.x , m_Pnt.y);
    }
}

void CDlgKnightageState::SetStateData()
{
    SyndicateInfo* pInfo = g_objHero.GetReturnSyndicateInfo();
    if (pInfo)
    {
        char szMsg[64] = "";
        m_StaName.SetWindowText(pInfo->szName);
        m_StaHeadName.SetWindowText(pInfo->szLeader);
        DWORD dwSynLev = 10830 + pInfo->ucRank;		//军团等级
        m_StaSynLev.SetWindowText((char*)g_objGameDataSet.GetStr(dwSynLev));
        sprintf(szMsg, "%d", pInfo->dwSyndicatePopulation);
        m_StaNumber.SetWindowText(szMsg);
        sprintf(szMsg, "%d", pInfo->ucSaint);
        m_StaFamor.SetWindowText(szMsg);
        sprintf(szMsg, "%d", pInfo->dwSyndicateFund);
        m_StaLevel.SetWindowText(szMsg);
        CDlgKnightageState::EnableWindow();
    }
    else
    {
        CDlgKnightageState::EnableWindow(false);
    }
}

void CDlgKnightageState::OnBtnClose()
{
    // TODO: Add your control notification handler code here
    CDlgKnightageState::EnableWindow(false);
}


void CDlgKnightageState::SetParentWnd(CWnd* pParentWnd)
{
    m_ParentWnd = pParentWnd ;
}
