
//**********************************************************
// 代码编辑器
//**********************************************************

// MyCheck.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MY_CHECK_TWOMODE	2
#define MY_CHECK_THREEMODE	3
#define MY_CHECK_FOURMODE	4

/////////////////////////////////////////////////////////////////////////////
// CMyCheck

CMyCheck::CMyCheck()
{
    m_ChkPar.m_btCurrent = 0 ;
    m_ChkPar.m_uFrame = 0 ;
    m_ChkPar.m_bStaticMode = false;
    m_ChkPar.m_dwStaticColor = 0xffffff;
    strcpy(m_szTip, "");
    m_dwTipColor = 0XFFFFFF;
}

CMyCheck::~CMyCheck()
{
}


BEGIN_MESSAGE_MAP(CMyCheck, CButton)
    //{{AFX_MSG_MAP(CMyCheck)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCheck message handlers


BOOL CMyCheck::Init(int xPos, int yPos, char* CheckID, BYTE btType)
{
    CRect rect ;
    CMyCheck::GetWindowRect ( rect ) ;
    m_ChkPar.m_IntPoint.x = rect.left - xPos ;
    m_ChkPar.m_IntPoint.y = rect.top - yPos ;
    m_ChkPar.m_btType = btType ;
    m_ChkPar.m_nWidth = rect.right - rect.left ;
    m_ChkPar.m_nHeight = rect.bottom - rect.top ;
    strcpy ( m_ChkPar.m_strCheckID, CheckID ) ;
    m_ChkPar.m_ShowRect.left = 0 ;
    m_ChkPar.m_ShowRect.top = 0 ;
    CMySize Size = { 0, 0 } ;
    CAni* pSizeAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni, CheckID, EXIGENCE_IMMEDIATE ) ;
    if ( pSizeAni != NULL )
    {
        Size = pSizeAni->GetSize ( m_ChkPar.m_uFrame ) ;
        m_ChkPar.m_ShowRect.right = Size.iWidth ;
        m_ChkPar.m_ShowRect.bottom = Size.iHeight ;
    }
    return true ;
}

void CMyCheck::ReInit(int xPos, int yPos)
{
    CRect rect ;
    CMyCheck::GetWindowRect ( rect ) ;
    m_ChkPar.m_IntPoint.x = rect.left - xPos ;
    m_ChkPar.m_IntPoint.y = rect.top - yPos ;
    m_ChkPar.m_nWidth = rect.right - rect.left ;
    m_ChkPar.m_nHeight = rect.bottom - rect.top ;
}

void CMyCheck::Show(int x, int y)
{
    if (strlen(m_ChkPar.m_strCheckID) > 0)
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                        m_ChkPar.m_strCheckID,
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            /*		ShowAni->ShowEx ( m_ChkPar.m_uFrame,
            						  m_ChkPar.m_IntPoint.x + x,
            						  m_ChkPar.m_IntPoint.y + y,
            						  m_ChkPar.m_ShowRect,
            						  m_ChkPar.m_nWidth,
            						  m_ChkPar.m_nHeight ) ;*/
            ShowAni->Show ( m_ChkPar.m_uFrame,
                            m_ChkPar.m_IntPoint.x + x,
                            m_ChkPar.m_IntPoint.y + y ) ;
    }
    //	if (this->IsMouseFocus() && strlen(m_szTip) > 0)
    //		CMyBitmap::ShowString(m_ChkPar.m_IntPoint.x +x ,m_ChkPar.m_IntPoint.y + y - CMyBitmap::GetFontSize (),
    //			m_ChkPar.m_dwStaticColor,m_szTip);
    if (m_ChkPar.m_bStaticMode)
    {
        char strCheck[64] = "";
        CMyCheck::GetWindowText(strCheck, 64);
        int nLen = strlen(strCheck);
        CMySize FontSize;
        CMyBitmap::GetFontSize(FontSize);
        int nPosX = (m_ChkPar.m_nWidth - nLen * FontSize.iWidth) / 2;
        int nPosY = (m_ChkPar.m_nHeight - FontSize.iHeight) / 2;
        if (m_ChkPar.m_uFrame == 1)
        {
            CMyBitmap::ShowBlock(m_ChkPar.m_IntPoint.x + x + nPosX,
                                 m_ChkPar.m_IntPoint.y + y + nPosY, m_ChkPar.m_IntPoint.x + x + nPosX + nLen * FontSize.iWidth,
                                 m_ChkPar.m_IntPoint.y + y + nPosY + FontSize.iHeight, 0xc00000ff);
        }
        CMyBitmap::ShowString(m_ChkPar.m_IntPoint.x + x + nPosX, m_ChkPar.m_IntPoint.y + y + nPosY,
                              m_ChkPar.m_dwStaticColor, strCheck);
    }
    CMyCheck::ShowTip(x, y);
}

void CMyCheck::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bInJustNow = true;
    m_ChkPar.m_btCurrent = ( m_ChkPar.m_btCurrent >= m_ChkPar.m_btType - 1 ) ? 0 : m_ChkPar.m_btCurrent + 1 ;
    m_ChkPar.m_uFrame = m_ChkPar.m_btCurrent ;
    CButton::OnLButtonDown(nFlags, point);
}

void CMyCheck::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your code to draw the specified item
}

BYTE CMyCheck::GetCheckValue()
{
    return m_ChkPar.m_btCurrent ;
}

void CMyCheck::SetCheckValue(BYTE btValue)
{
    if ( btValue < m_ChkPar.m_btType )
    {
        m_ChkPar.m_btCurrent = btValue ;
        m_ChkPar.m_uFrame = btValue ;
    }
}

void CMyCheck::OnMouseMove(UINT nFlags, CPoint point)
{
    LPARAM lParam ;
    lParam = MAKELONG ( point.x + m_ChkPar.m_IntPoint.x, point.y + m_ChkPar.m_IntPoint.y) ;
    CMyCheck::GetParent ()->PostMessage ( WM_MOUSEMOVE, nFlags, lParam ) ;
    //	CButton::OnMouseMove ( nFlags, point ) ;
}

void CMyCheck::SetStaticMode(BOOL bStatic /* = true */, DWORD dwColor/* =0xffffff */)
{
    m_ChkPar.m_bStaticMode = bStatic;
    m_ChkPar.m_dwStaticColor = dwColor;
}

BOOL CMyCheck::IsMouseFocus()
{
    POINT MousePoint ;
    GetCursorPos ( &MousePoint ) ;
    CRect rect ;
    CMyCheck::GetWindowRect ( rect ) ;
    if ( !rect.PtInRect ( MousePoint ) )
    {
        return false;
    }
    return true;
}

void CMyCheck::SetTipShow(char* szTip, DWORD dwColor)
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

void CMyCheck::ShowTip(int x, int y)
{
    if (CMyCheck::IsMouseFocus () && strlen(m_szTip) > 0)
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
            INT nTipLen = strLeft.GetLength();
            nOffsetX = (m_ChkPar.m_nWidth - nTipLen * FontSize.iWidth) / 2;
            if ( m_ChkPar.m_IntPoint.x + x + nOffsetX + nTipLen * FontSize.iWidth > _SCR_WIDTH )
            {
                nOffsetX = _SCR_WIDTH - m_ChkPar.m_IntPoint.x + x + nTipLen * FontSize.iWidth ;
            }
            if (m_ChkPar.m_IntPoint.y + y - FontSize.iHeight < 0)
            {
                nOffsetY = FontSize.iHeight - m_ChkPar.m_IntPoint.y + y ;
            }
            g_objGameDataSet.ShowInfoBack(m_ChkPar.m_IntPoint.x + x + nOffsetX - 5,
                                          m_ChkPar.m_IntPoint.y + y - FontSize.iHeight * 2 + nOffsetY - 10,
                                          nTipLen * FontSize.iWidth + 10,
                                          FontSize.iHeight * 2 + 10) ;
            CMyBitmap::ShowString ( m_ChkPar.m_IntPoint.x + x + nOffsetX,
                                    m_ChkPar.m_IntPoint.y + y  - FontSize.iHeight * 2 + nOffsetY - 5,
                                    m_dwTipColor,
                                    strLeft) ;
            CMyBitmap::ShowString ( m_ChkPar.m_IntPoint.x + x + nOffsetX,
                                    m_ChkPar.m_IntPoint.y + y + nOffsetY - 5 - FontSize.iHeight,
                                    m_dwTipColor,
                                    strRight) ;
        }
        else if (-1 != (nIndex = s.Find('$')))
        {
            CString strLeft = s.Left(nIndex);
            CString strRight = s.Right(s.GetLength() - nIndex - 1);
            INT nTipLen = strLeft.GetLength();
            nOffsetX = (m_ChkPar.m_nWidth - nTipLen * FontSize.iWidth) / 2;
            if ( m_ChkPar.m_IntPoint.x + x + nOffsetX + nTipLen * FontSize.iWidth > _SCR_WIDTH )
            {
                nOffsetX = _SCR_WIDTH - m_ChkPar.m_IntPoint.x + x + nTipLen * FontSize.iWidth ;
            }
            if (m_ChkPar.m_IntPoint.y + y - FontSize.iHeight < 0)
            {
                nOffsetY = FontSize.iHeight - m_ChkPar.m_IntPoint.y + y ;
            }
            g_objGameDataSet.ShowInfoBack(m_ChkPar.m_IntPoint.x + x + nOffsetX - 5,
                                          m_ChkPar.m_IntPoint.y + y - FontSize.iHeight * 2 + nOffsetY - 10,
                                          nTipLen * FontSize.iWidth + 10,
                                          FontSize.iHeight * 2 + 10) ;
            CMyBitmap::ShowString ( m_ChkPar.m_IntPoint.x + x + nOffsetX,
                                    m_ChkPar.m_IntPoint.y + y  - FontSize.iHeight * 2 + nOffsetY - 5,
                                    m_dwTipColor,
                                    strLeft) ;
            nTipLen = strRight.GetLength();
            nOffsetX = (m_ChkPar.m_nWidth - nTipLen * FontSize.iWidth) / 2;
            if ( m_ChkPar.m_IntPoint.x + x + nOffsetX + nTipLen * FontSize.iWidth > _SCR_WIDTH )
            {
                nOffsetX = _SCR_WIDTH - m_ChkPar.m_IntPoint.x + x + nTipLen * FontSize.iWidth ;
            }
            CMyBitmap::ShowString ( m_ChkPar.m_IntPoint.x + x + nOffsetX,
                                    m_ChkPar.m_IntPoint.y + y + nOffsetY - 5 - FontSize.iHeight,
                                    0xFFFF00,
                                    strRight) ;
        }
        else
        {
            INT nTipLen = strlen (m_szTip);
            nOffsetX = (m_ChkPar.m_nWidth - nTipLen * FontSize.iWidth) / 2;
            if ( m_ChkPar.m_IntPoint.x + x + nOffsetX + nTipLen * FontSize.iWidth + 10 > _SCR_WIDTH )
            {
                nOffsetX = _SCR_WIDTH - m_ChkPar.m_IntPoint.x + x + nTipLen * FontSize.iWidth - 10;
            }
            if (m_ChkPar.m_IntPoint.y + y - FontSize.iHeight < 0)
            {
                nOffsetY = FontSize.iHeight - m_ChkPar.m_IntPoint.y + y ;
            }
            g_objGameDataSet.ShowInfoBack( m_ChkPar.m_IntPoint.x + x + nOffsetX - 5,
                                           m_ChkPar.m_IntPoint.y + y - FontSize.iHeight + nOffsetY - 10,
                                           nTipLen * FontSize.iWidth + 10,
                                           FontSize.iHeight + 10) ;
            CMyBitmap::ShowString ( m_ChkPar.m_IntPoint.x + x + nOffsetX,
                                    m_ChkPar.m_IntPoint.y + y  - FontSize.iHeight + nOffsetY - 5,
                                    m_dwTipColor,
                                    m_szTip) ;
        }
    }
    else
    {
        m_bInJustNow = true;
    }
}

