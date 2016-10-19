
//**********************************************************
// 代码编辑器
//**********************************************************

// NewButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
    m_uFrame = 0 ;
    m_bStaticMode = FALSE;
    m_dwStaticColor = 0xffffff;
    strcpy(m_szTip, "");
    m_dwTipColor = 0XFFFFFF;
    m_nTipPosX = 0;
    m_nTipPosY = 0;
    m_bInJustNow = true;
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
    //{{AFX_MSG_MAP(CMyButton)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers



void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your code to draw the specified item
}


void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG ( point.x + m_IntPnt.x, point.y + m_IntPnt.y) ;
    CMyButton::GetParent ()->PostMessage ( WM_MOUSEMOVE, nFlags, lParam ) ;
    CButton::OnMouseMove(nFlags, point);
}


void CMyButton::Show( int x, int y )
{
    m_BtnPnt.x = m_IntPnt.x  + x ;
    m_BtnPnt.y = m_IntPnt.y  + y ;
    if (m_bStaticMode)
    {
        char strButton[64] = "";
        CMyButton::GetWindowText(strButton, 64);
        if (m_uFrame == 0)
        {
            CMyBitmap::ShowString(m_BtnPnt.x, m_BtnPnt.y, m_dwStaticColor, strButton);
        }
        else
        {
            CMyBitmap::ShowString(m_BtnPnt.x, m_BtnPnt.y, 0xffff00, strButton);
        }
    }
    else
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                        m_strButtonID,
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
        {
            if (CMyButton::IsMouseFocus() && m_uFrame == 0 && CMyButton::IsWindowEnabled() && ShowAni->GetFrameAmount() > 2)
            {
                ShowAni->Show ( 2, m_BtnPnt.x, m_BtnPnt.y ) ;
            }
            else
            {
                ShowAni->Show ( m_uFrame,
                                m_BtnPnt.x,
                                m_BtnPnt.y ) ;
            }
            CMyButton::ShowTip();
        }
    }
}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( 1 == m_uFrame )
    {
        m_uFrame = 0 ;
    }
    if ( m_btOwnerID == DLG_ACT)
    {
        CMyButton::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_DROP, m_btOwnerID ) ;
    }
    CButton::OnLButtonUp(nFlags, point);
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_bInJustNow = true;
    m_uFrame = 1 ;
    if ( m_btOwnerID == DLG_ACT )
    {
        LPARAM lParam ;
        CPoint MousePnt ;
        CMyButton::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_ID, m_nButtonID ) ;
        GetCursorPos ( &MousePnt ) ;
        lParam = MAKELONG ( MousePnt.x, MousePnt.y ) ;
        CMyButton::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PNT, lParam ) ;
        CMyButton::GetParent ()->GetParent ()->PostMessage ( WM_MY_MESSAGE, ON_ICON_PICKUP, m_btOwnerID ) ;
    }
    else if ( m_btOwnerID == DLG_LEVWORD )
    {
        if ( nFlags & MK_CONTROL )
        {
            m_uFrame = 0 ;
            return ;
        }
    }
    CButton::OnLButtonDown(nFlags, point);
}

BOOL CMyButton::Init(int xPos, int yPos, char* ButtonID, BYTE btOwnerID, int nButtonID)
{
    CRect rect;
    CMyButton::GetWindowRect ( rect ) ;
    m_IntPnt.x = rect.left - xPos ;
    m_IntPnt.y = rect.top - yPos ;
    m_nWidth = rect.right - rect.left ;
    m_nHeight = rect.bottom - rect.top ;
    m_btOwnerID = btOwnerID ;
    m_nButtonID = nButtonID ;
    strcpy ( m_strButtonID, ButtonID ) ;
    return true ;
}

void CMyButton::SetCurFrame(UINT uFrame)
{
    m_uFrame = uFrame ;
}

LRESULT CMyButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( message == WM_ENABLE )
        if ( !wParam )
        {
            m_uFrame = 2 ;
        }
        else
        {
            m_uFrame = 0 ;
        }
    return CButton::WindowProc(message, wParam, lParam);
}

void CMyButton::ChangeImage(char* ButtonID)
{
    strcpy ( m_strButtonID, ButtonID ) ;
}

void CMyButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CButton::OnLButtonDblClk(nFlags, point);
}

void CMyButton::ReInit(int xPos, int yPos)
{
    CRect rect;
    CMyButton::GetWindowRect ( rect ) ;
    m_IntPnt.x = rect.left - xPos ;
    m_IntPnt.y = rect.top - yPos ;
    m_nWidth = rect.right - rect.left ;
    m_nHeight = rect.bottom - rect.top ;
}

void CMyButton::SetStaticMode(BOOL bStatic/* =true */, DWORD dwColor/* =0xffffff */)
{
    m_bStaticMode = bStatic;
    m_dwStaticColor = dwColor;
}

BOOL CMyButton::IsMouseFocus()
{
    POINT MousePoint ;
    GetCursorPos ( &MousePoint ) ;
    CRect rect ;
    CMyButton::GetWindowRect ( rect ) ;
    if ( !rect.PtInRect ( MousePoint ) )
    {
        return false;
    }
    return true;
}

void CMyButton::SetTipShow(char* szTip, DWORD dwColor)
{
    if (szTip && strlen(szTip) > 0)
    {
        strcpy(m_szTip, szTip);
    }
    else
    {
        strcpy (m_szTip, "");
    }
    m_dwTipColor = dwColor;
}

void CMyButton::SetTipPos(int x, int y)
{
    m_nTipPosX = x;
    m_nTipPosY = y;
}

void CMyButton::ShowTip()
{
    if (CMyButton::IsWindowEnabled() && CMyButton::IsMouseFocus () && strlen(m_szTip) > 0)
    {
        // 判断鼠标是否刚刚进入
        // 是
        if (m_bInJustNow)
        {
            // 记录进入的时间
            m_dwInTime = TimeGet();
            m_bInJustNow = false;
            return;
        }
        // 判断进入时间是否达到 0.5 秒
        // 否
        else if (TimeGet() - m_dwInTime < 500)
        {
            return;
        }
        CMySize FontSize;
        CMyBitmap::GetFontSize (FontSize);
        int nOffsetX = 0 ;
        int nOffsetY = 0 ;
        CString s(m_szTip);
        int nIndex;
        // 判断是否有特殊格式
        if (-1 != (nIndex = s.Find('#')))
        {
            CString strLeft = s.Left(nIndex);
            CString strRight = s.Right(s.GetLength() - nIndex - 1);
            int nTipLen = strLeft.GetLength();
            nOffsetX = (m_nWidth - nTipLen * FontSize.iWidth) / 2;
            if ( m_BtnPnt.x + nOffsetX + nTipLen * FontSize.iWidth > _SCR_WIDTH )
            {
                nOffsetX = _SCR_WIDTH - m_BtnPnt.x + nTipLen * FontSize.iWidth ;
            }
            nOffsetY = m_BtnPnt.y - FontSize.iHeight * 2 - 12;
            // 判断提示信息是否超出屏幕
            if (nOffsetY < 0)
            {
                nOffsetY = m_BtnPnt.y + m_nHeight / 2 - FontSize.iHeight;
                nOffsetX = - nTipLen * FontSize.iWidth - 12;
                if (m_BtnPnt.x + nOffsetX < 0)
                {
                    nOffsetX = 12;
                }
            }
            g_objGameDataSet.ShowInfoBack(m_BtnPnt.x + nOffsetX - 5 + m_nTipPosX,
                                          nOffsetY - 5 + m_nTipPosY,
                                          nTipLen * FontSize.iWidth + 10,
                                          FontSize.iHeight * 2 + 10) ;
            CMyBitmap::ShowString ( m_BtnPnt.x + nOffsetX + ((nTipLen - strRight.GetLength()) * FontSize.iWidth / 2 ) + m_nTipPosX,
                                    nOffsetY + m_nTipPosY,
                                    m_dwTipColor,
                                    strLeft) ;
            CMyBitmap::ShowString ( m_BtnPnt.x + nOffsetX + m_nTipPosX,
                                    FontSize.iHeight + nOffsetY + m_nTipPosY,
                                    m_dwTipColor,
                                    strRight) ;
        }
        else if (-1 != (nIndex = s.Find('$')))
        {
            CString strLeft = s.Left(nIndex);
            CString strRight = s.Right(s.GetLength() - nIndex - 1);
            int nTipLen = strLeft.GetLength();
            nOffsetX = (m_nWidth - nTipLen * FontSize.iWidth) / 2;
            if ( m_BtnPnt.x + nOffsetX + nTipLen * FontSize.iWidth > _SCR_WIDTH )
            {
                nOffsetX = _SCR_WIDTH - m_BtnPnt.x + nTipLen * FontSize.iWidth ;
            }
            nOffsetY = m_BtnPnt.y - FontSize.iHeight * 2 - 12;
            // 判断提示信息是否超出屏幕
            if (nOffsetY < 0)
            {
                nOffsetY = m_BtnPnt.y + m_nHeight / 2 - FontSize.iHeight;
                nOffsetX = -nTipLen * FontSize.iWidth - 12;
                if (m_BtnPnt.x + nOffsetX < 0)
                {
                    nOffsetX = 12;
                }
            }
            g_objGameDataSet.ShowInfoBack(m_BtnPnt.x + nOffsetX - 5 + m_nTipPosX,
                                          nOffsetY - 5 + m_nTipPosY,
                                          nTipLen * FontSize.iWidth + 10,
                                          FontSize.iHeight * 2 + 10) ;
            CMyBitmap::ShowString ( m_BtnPnt.x + nOffsetX + m_nTipPosX,
                                    nOffsetY + m_nTipPosY,
                                    m_dwTipColor,
                                    strLeft) ;
            CMyBitmap::ShowString ( m_BtnPnt.x + nOffsetX + ((nTipLen - strRight.GetLength()) * FontSize.iWidth / 2 ) + m_nTipPosX,
                                    FontSize.iHeight + nOffsetY + m_nTipPosY,
                                    0xFFFF00,
                                    strRight) ;
        }
        else
        {
            int nTipLen = strlen (m_szTip);
            nOffsetX = (m_nWidth - nTipLen * FontSize.iWidth) / 2;
            if ( m_BtnPnt.x + nOffsetX + nTipLen * FontSize.iWidth + 10 > _SCR_WIDTH )
            {
                nOffsetX = _SCR_WIDTH - m_BtnPnt.x - nTipLen * FontSize.iWidth - 10;
            }
            nOffsetY = m_BtnPnt.y - FontSize.iHeight - 12;
            // 判断提示信息是否超出屏幕
            if (nOffsetY < 0)
            {
                nOffsetY = m_BtnPnt.y + m_nHeight / 2 - FontSize.iHeight / 2;
                nOffsetX = -nTipLen * FontSize.iWidth - 12;
                if (m_BtnPnt.x + nOffsetX < 0)
                {
                    nOffsetX = 12;
                }
            }
            g_objGameDataSet.ShowInfoBack( m_BtnPnt.x + nOffsetX - 5 + m_nTipPosX,
                                           nOffsetY - 5 + m_nTipPosY,
                                           nTipLen * FontSize.iWidth + 10,
                                           FontSize.iHeight + 10) ;
            CMyBitmap::ShowString ( m_BtnPnt.x + nOffsetX + m_nTipPosX,
                                    nOffsetY + m_nTipPosY,
                                    m_dwTipColor,
                                    m_szTip) ;
        }
    }
    else
    {
        m_bInJustNow = true;
    }
}
