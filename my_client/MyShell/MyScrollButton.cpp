
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MyScrollButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyScrollButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyScrollButton

CMyScrollButton::CMyScrollButton()
{
    m_bDrpValue = false ;
    m_uFrame = 0 ;
}

CMyScrollButton::~CMyScrollButton()
{
}


BEGIN_MESSAGE_MAP(CMyScrollButton, CButton)
    //{{AFX_MSG_MAP(CMyScrollButton)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyScrollButton message handlers

void CMyScrollButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_uFrame = 1 ;
    SetTimer( 1, 100, NULL ) ;
    DWORD wparam ;
    switch ( m_btScrBtnType )
    {
    case MYSCROLL_UP:
        wparam = MAKELONG ( SB_LINEUP, 0 ) ;
        m_ParentWnd->PostMessage ( WM_VSCROLL, wparam, NULL ) ;
        break ;
    case MYSCROLL_DOWN:
        wparam = MAKELONG ( SB_LINEDOWN, 0 ) ;
        m_ParentWnd->PostMessage ( WM_VSCROLL, wparam, NULL ) ;
        break ;
    case MYSCROLL_ROLL:
        break ;
    case MYSCROLL_DROP:
        m_bDrpValue = !m_bDrpValue ;
        wparam = m_bDrpValue ;
        m_ParentWnd->PostMessage ( CB_SHOWDROPDOWN, wparam, 1 ) ;
        break ;
    }
    //	CButton::OnLButtonDown(nFlags, point);
}

void CMyScrollButton::Init(CWnd* ListWnd, BYTE btScrBtnType, int nPosX, int nPosY)
{
    m_ParentWnd = ListWnd ;
    m_btScrBtnType = btScrBtnType ;
    CRect rect ;
    CMyScrollButton::GetWindowRect( rect ) ;
    m_ScrBtnPnt.x = rect.left - nPosX;
    m_ScrBtnPnt.y = rect.top - nPosY;
}

void CMyScrollButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    DWORD wparam ;
    switch ( m_btScrBtnType )
    {
    case MYSCROLL_UP:
        wparam = MAKELONG ( SB_LINEUP, 0 ) ;
        m_ParentWnd->PostMessage ( WM_VSCROLL, wparam, NULL ) ;
        break ;
    case MYSCROLL_DOWN:
        wparam = MAKELONG ( SB_LINEDOWN, 0 ) ;
        m_ParentWnd->PostMessage ( WM_VSCROLL, wparam, NULL ) ;
        break ;
    case MYSCROLL_ROLL:
        break ;
    case MYSCROLL_DROP:
        break ;
    }
    //	CButton::OnLButtonDblClk(nFlags, point);
}

void CMyScrollButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    KillTimer ( 1 ) ;
    m_uFrame = 0 ;
    DWORD wparam ;
    if ( m_btScrBtnType != MYSCROLL_DROP )
    {
        wparam = MAKELONG ( SB_ENDSCROLL, 0 ) ;
        m_ParentWnd->PostMessage ( WM_VSCROLL, wparam, NULL ) ;
    }
    //	return ;
    //	CButton::OnLButtonUp(nFlags, point);
}

void CMyScrollButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your code to draw the specified item
}

void CMyScrollButton::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    DWORD wparam ;
    switch ( m_btScrBtnType )
    {
    case MYSCROLL_UP:
        wparam = MAKELONG ( SB_LINEUP, 0 ) ;
        m_ParentWnd->PostMessage ( WM_VSCROLL, wparam, NULL ) ;
        break ;
    case MYSCROLL_DOWN:
        wparam = MAKELONG ( SB_LINEDOWN, 0 ) ;
        m_ParentWnd->PostMessage ( WM_VSCROLL, wparam, NULL ) ;
        break ;
    }
    CButton::OnTimer(nIDEvent);
}

BOOL CMyScrollButton::Load(char* ScrButtonID)
{
    strcpy ( m_strScrButtonID, ScrButtonID ) ;
    return true ;
}

void CMyScrollButton::Show(int xPos, int yPos)
{
    CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                    m_strScrButtonID,
                    EXIGENCE_IMMEDIATE ) ;
    if ( ShowAni != NULL )
    {
        ShowAni->Show ( m_uFrame,
                        xPos + m_ScrBtnPnt.x,
                        yPos + m_ScrBtnPnt.y ) ;
    }
}

HBRUSH CMyScrollButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}
