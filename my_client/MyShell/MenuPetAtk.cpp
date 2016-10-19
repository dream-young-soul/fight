
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MenuPetAtk.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "MenuPetAtk.h"

#include "Hero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuPetAtk dialog


CMenuPetAtk::CMenuPetAtk(CWnd* pParent /*=NULL*/)
    : CDialog(CMenuPetAtk::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMenuPetAtk)
    m_Pnt = CPoint(0, 0);
    m_nPetIndex = 0;
    m_bShow = FALSE;// NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CMenuPetAtk::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMenuPetAtk)
    DDX_Control(pDX, IDC_BTN_ATKSTATUS2, m_BtnAtkStatus2);
    DDX_Control(pDX, IDC_BTN_ATKSTATUS1, m_BtnAtkStatus1);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuPetAtk, CDialog)
    //{{AFX_MSG_MAP(CMenuPetAtk)
    ON_BN_CLICKED(IDC_BTN_ATKSTATUS1, OnBtnAtkstatus1)
    ON_BN_CLICKED(IDC_BTN_ATKSTATUS2, OnBtnAtkstatus2)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuPetAtk message handlers

void CMenuPetAtk::OnBtnAtkstatus1()
{
    // TODO: Add your control notification handler code here
    CMenuPetAtk::EnableWindow (false);
}

void CMenuPetAtk::OnBtnAtkstatus2()
{
    // TODO: Add your control notification handler code here
    CMenuPetAtk::EnableWindow (false);
}

LRESULT CMenuPetAtk::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message == WM_ENABLE)
    {
        m_bShow = wParam;
        if (!m_bShow)
        {
            m_nPetIndex = 0;
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

HBRUSH CMenuPetAtk::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode (TRANSPARENT);
    return g_StockBrush;
}

BOOL CMenuPetAtk::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here
    CRect rect;
    CMenuPetAtk::GetWindowRect (rect);
    m_BtnAtkStatus1.Init (rect.left, rect.top, "Button575");
    m_BtnAtkStatus2.Init (rect.left, rect.top, "Button576");
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuPetAtk::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    // TODO: Add your message handler code here
    m_Pnt.x = x;
    m_Pnt.y = y;
}

void CMenuPetAtk::SetPetAtkStatus (OBJID idPetItem)
{
    CPet* pPet = g_objHero.GetPetByItemID (idPetItem);
    if (pPet)
    {
        m_idPetItem = idPetItem;
    }
    else
    {
        m_idPetItem = ID_NONE;
    }
}

void CMenuPetAtk::Show()
{
    if (m_bShow)
    {
        m_BtnAtkStatus1.Show (m_Pnt.x, m_Pnt.y);
        m_BtnAtkStatus2.Show (m_Pnt.x, m_Pnt.y);
    }
}

void CMenuPetAtk::ShowPetAtk (int nIndex)
{
    if (m_nPetIndex == nIndex && CMenuPetAtk::IsWindowEnabled ())
    {
        CMenuPetAtk::EnableWindow (false);
    }
    else
    {
        CRect rect;
        CMenuPetAtk::GetWindowRect (rect);
        CMenuPetAtk::MoveWindow (_SCR_WIDTH - 100, nIndex * 25 + 70, rect.Width (), rect.Height (), false);
        CMenuPetAtk::EnableWindow ();
        m_nPetIndex = nIndex;
    }
}
