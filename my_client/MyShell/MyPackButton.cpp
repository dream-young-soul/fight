
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// PackButton.cpp : implementation file
//

#include "stdafx.h"
#include "myshell.h"
#include "MyPackButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPackButton

CMyPackButton::CMyPackButton()
{
}

CMyPackButton::~CMyPackButton()
{
}


BEGIN_MESSAGE_MAP(CMyPackButton, CButton)
    //{{AFX_MSG_MAP(CMyPackButton)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPackButton message handlers

void CMyPackButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_uFrame = 1 ;
    CPoint posMouse;
    GetCursorPos(&posMouse);
    m_MoveInitPos.x = posMouse.x;
    m_MoveInitPos.y = posMouse.y;
    CButton::OnLButtonDown(nFlags, point);
}

void CMyPackButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( 1 == m_uFrame )
    {
        m_uFrame = 0 ;
        CPoint posMouse;
        GetCursorPos(&posMouse);
        m_MovePos.x = posMouse.x - m_MoveInitPos.x;
        m_MovePos.y = posMouse.y - m_MoveInitPos.y;
        CMyPackButton::GetParent()->PostMessage(WM_MY_MESSAGE, ON_PACKBTN_MOVE, NULL);
    }
    CButton::OnLButtonUp(nFlags, point);
}

CPoint CMyPackButton::GetMovePos()
{
    return m_MovePos;
}




void CMyPackButton::Show(int x, int y)
{
    // 	if (0== m_uFrame)
    {
        m_BtnPnt.x = m_IntPnt.x  + x ;
        m_BtnPnt.y = m_IntPnt.y  + y ;
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                        m_strButtonID,
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( m_uFrame,
                            m_BtnPnt.x,
                            m_BtnPnt.y ) ;
    }
}
