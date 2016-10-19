
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MyEditRect.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "MyEditRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEditRect

CMyEditRect::CMyEditRect()
{
    m_pParentWnd = NULL;
}

CMyEditRect::~CMyEditRect()
{
}


BEGIN_MESSAGE_MAP(CMyEditRect, CButton)
    //{{AFX_MSG_MAP(CMyEditRect)
    ON_WM_DRAWITEM()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEditRect message handlers
void CMyEditRect::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if ( m_pParentWnd != NULL )
    {
        LPARAM lParam;
        lParam = MAKELONG( point.x, point.y );
        m_pParentWnd->PostMessage( WM_LBUTTONDBLCLK, nFlags, lParam );
    }
    CButton::OnLButtonDblClk(nFlags, point);
}

void CMyEditRect::OnLButtonDown(UINT nFlags, CPoint point)
{
    if ( m_pParentWnd != NULL )
    {
        LPARAM lParam;
        lParam = MAKELONG( point.x, point.y );
        m_pParentWnd->PostMessage( WM_LBUTTONDOWN, nFlags, lParam );
    }
    CButton::OnLButtonDown(nFlags, point);
}

void CMyEditRect::OnLButtonUp(UINT nFlags, CPoint point)
{
    if ( m_pParentWnd != NULL )
    {
        LPARAM lParam;
        lParam = MAKELONG( point.x, point.y );
        m_pParentWnd->PostMessage( WM_LBUTTONUP, nFlags, lParam );
    }
    CButton::OnLButtonUp(nFlags, point);
}

void CMyEditRect::OnMouseMove(UINT nFlags, CPoint point)
{
    if ( m_pParentWnd != NULL )
    {
        LPARAM lParam;
        lParam = MAKELONG( point.x, point.y );
        m_pParentWnd->PostMessage( WM_MOUSEMOVE, nFlags, lParam );
    }
    CButton::OnMouseMove(nFlags, point);
}

void CMyEditRect::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    if ( m_pParentWnd != NULL )
    {
        LPARAM lParam;
        lParam = MAKELONG( point.x, point.y );
        m_pParentWnd->PostMessage( WM_RBUTTONDBLCLK, nFlags, lParam );
    }
    CButton::OnRButtonDblClk(nFlags, point);
}

void CMyEditRect::OnRButtonDown(UINT nFlags, CPoint point)
{
    if ( m_pParentWnd != NULL )
    {
        LPARAM lParam;
        lParam = MAKELONG( point.x, point.y );
        m_pParentWnd->PostMessage( WM_RBUTTONDOWN, nFlags, lParam );
    }
    CButton::OnRButtonDown(nFlags, point);
}

void CMyEditRect::OnRButtonUp(UINT nFlags, CPoint point)
{
    if ( m_pParentWnd != NULL )
    {
        LPARAM lParam;
        lParam = MAKELONG( point.x, point.y );
        m_pParentWnd->PostMessage( WM_RBUTTONUP, nFlags, lParam );
    }
    CButton::OnRButtonUp(nFlags, point);
}

void CMyEditRect::Init(CWnd* pParentWnd)
{
    m_pParentWnd = pParentWnd;
}

void CMyEditRect::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
}
