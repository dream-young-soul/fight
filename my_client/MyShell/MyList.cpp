
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MyList.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "MyList.h"

#define		MY_LISTBOX	0
#define		MY_COMBOBOX 1

const int _MAX_LINE = 4;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyList

CMyList::CMyList()
{
    m_LstFnt.m_dwFontColor = 0x000000 ;
    m_LstPar.m_OffsetPoint = CPoint ( 0, 0 ) ;
    m_LstPar.m_bVShow = false ;
    m_LstPar.m_btListCount = 0 ;
    m_LstPar.m_nLastClkIndex = -1 ;
    m_LstPar.m_ParentCob = NULL ;
    m_bShowBack = false;
    m_LstPar.m_nLine = 1;
    for ( int i = 0; i < 64; i++ )
    {
        m_LstPar.m_strListID[i] = 0 ;
        m_LstPar.m_nListRealIndex[i] = -1 ;
        m_LstPar.m_bIsFlash[i] = FALSE;
        m_LstPar.m_dwShowTime[i] = 0;
    }
}

CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListBox)
    //{{AFX_MSG_MAP(CMyList)
    ON_WM_VSCROLL()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_CTLCOLOR()
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyList message handlers

void CMyList::Show(int x, int y)
{
    m_LstPar.m_ListPoint.x = m_LstPar.m_IntPoint.x + x ;
    m_LstPar.m_ListPoint.y = m_LstPar.m_IntPoint.y + y ;
    if ( m_LstPar.m_strListID [0] != 0 )
    {
        CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* ) g_strControlAni,
                        m_LstPar.m_strListID,
                        EXIGENCE_IMMEDIATE ) ;
        if ( ShowAni != NULL )
            ShowAni->Show ( 0,
                            m_LstPar.m_ListPoint.x + m_LstPar.m_OffsetPoint.x,
                            m_LstPar.m_ListPoint.y + m_LstPar.m_OffsetPoint.y) ;
    }
    else if (m_bShowBack)
    {
        CMyBitmap::ShowBlock(m_LstPar.m_ListPoint.x + m_LstPar.m_OffsetPoint.x,
                             m_LstPar.m_ListPoint.y + m_LstPar.m_OffsetPoint.y,
                             m_LstPar.m_ListPoint.x + m_LstPar.m_nWidth,
                             m_LstPar.m_ListPoint.y + m_LstPar.m_nHeight,
                             0xff000000);
    }
    if ( m_LstPar.m_btListCount  > 0 )
    {
        ShowSel() ;
        ShowChar () ;
    }
    if ( m_LstPar.m_bVShow )
    {
        m_LstScr.m_ScrUpBtn.Show ( m_LstPar.m_ListPoint.x,
                                   m_LstPar.m_ListPoint.y ) ;
        m_LstScr.m_ScrDoBtn.Show ( m_LstPar.m_ListPoint.x,
                                   m_LstPar.m_ListPoint.y ) ;
    }
}

void CMyList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your code to draw the specified item
}

void CMyList::ShowSel()
{
    m_LstPar.m_nTopIndex = CMyList::GetTopIndex () ;
    m_LstPar.m_nCurIndex = CMyList::GetCurSel () ;
    if ( m_LstPar.m_nCurIndex >= m_LstPar.m_nTopIndex
            && m_LstPar.m_nCurIndex < m_LstPar.m_nTopIndex + m_LstPar.m_btListLineNum )
    {
        CMyList::GetItemRect ( m_LstPar.m_nCurIndex, m_LstPar.m_SelRect ) ;
        CMyBitmap::ShowBlock ( m_LstPar.m_ListPoint.x,
                               m_LstPar.m_ListPoint.y + m_LstPar.m_SelRect.top,
                               m_LstPar.m_ListPoint.x + m_LstPar.m_SelRect.right,
                               m_LstPar.m_ListPoint.y + m_LstPar.m_SelRect.bottom,
                               0xff000080 ) ;
    }
}

void CMyList::ShowChar()
{
    char strChar [1024] ;
    int i = m_LstPar.m_nTopIndex;
    for ( ; i < m_LstPar.m_nTopIndex + m_LstPar.m_btShowLineNum; i++ )
    {
        CMyList::GetText ( i, strChar ) ;
        CMyList::GetItemRect ( i, m_LstPar.m_ItemRect ) ;
        strChar [m_LstFnt.m_btCharNum] = 0 ;
        if ( m_LstPar.m_btType == COMPLEX_LIST )
        {
            if (m_LstPar.m_bIsFlash[i] == TRUE)
            {
                DWORD colorString;
                DWORD dwTime = ::TimeGet();
                if (m_LstPar.m_dwShowTime[i] == 0)
                {
                    m_LstPar.m_dwShowTime[i] = dwTime;
                }
                else if (dwTime - m_LstPar.m_dwShowTime[i] <= 500
                         && dwTime - m_LstPar.m_dwShowTime[i] > 0)
                {
                    colorString = 0xff0000;
                }
                else if (dwTime - m_LstPar.m_dwShowTime[i] > 500
                         && dwTime - m_LstPar.m_dwShowTime[i] <= 1000)
                {
                    colorString = 0xffffff;
                    m_LstPar.m_dwShowTime[i] = 0;
                }
                else if (dwTime - m_LstPar.m_dwShowTime[i] > 1000)
                {
                    m_LstPar.m_dwShowTime[i] = 0;
                }
                CMyList::ShowMLine( m_LstPar.m_ListPoint.x,
                                    m_LstPar.m_ListPoint.y + m_LstPar.m_ItemRect.top,
                                    colorString,
                                    strChar ) ;
            }
            else
            {
                if ( m_LstPar.m_nListRealIndex [i] == 1 )
                    CMyList::ShowMLine ( m_LstPar.m_ListPoint.x,
                                         m_LstPar.m_ListPoint.y + m_LstPar.m_ItemRect.top,
                                         0xffffff,
                                         strChar ) ;
                else if ( m_LstPar.m_nListRealIndex [i] == -1 )
                    CMyList::ShowMLine ( m_LstPar.m_ListPoint.x,
                                         m_LstPar.m_ListPoint.y + m_LstPar.m_ItemRect.top,
                                         0xff0000,
                                         strChar ) ;
                else if ( m_LstPar.m_nListRealIndex [i] == 2 )
                    CMyList::ShowMLine ( m_LstPar.m_ListPoint.x,
                                         m_LstPar.m_ListPoint.y + m_LstPar.m_ItemRect.top,
                                         0xffff00,
                                         strChar ) ;
            }
        }
        else
            CMyList::ShowMLine ( m_LstPar.m_ListPoint.x,
                                 m_LstPar.m_ListPoint.y + m_LstPar.m_ItemRect.top,
                                 m_LstFnt.m_dwFontColor,
                                 strChar ) ;
    }
    if ( m_LstPar.m_nCurIndex >= m_LstPar.m_nTopIndex && m_LstPar.m_nCurIndex < i )
    {
        CMyList::GetText ( m_LstPar.m_nCurIndex, strChar ) ;
        strChar [m_LstFnt.m_btCharNum] = 0 ;
        CMyList::ShowMLine ( m_LstPar.m_ListPoint.x,
                             m_LstPar.m_ListPoint.y + m_LstPar.m_SelRect.top,
                             0xffffff,
                             strChar ) ;
    }
}


void CMyList::ShowVScroll()
{
}


void CMyList::SetFontColor(DWORD dwColor)
{
    m_LstFnt.m_dwFontColor = dwColor ;
}



void CMyList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}




BOOL CMyList::Init(int xPos, int yPos, char* ListID, BYTE btType)
{
    CRect rect;
    CRgn rgn ;
    // init the list's parameters
    CMyList::GetWindowRect ( rect ) ;
    m_LstPar.m_IntPoint.x = rect.left - xPos ;
    m_LstPar.m_IntPoint.y = rect.top - yPos ;
    m_LstPar.m_nHeight = rect.bottom - rect.top ;
    m_LstPar.m_nWidth = rect.right - rect.left ;
    m_LstFnt.m_btFontHeight =(BYTE)( CMyBitmap::GetFontSize () + 4 );
    m_LstFnt.m_btCharNum =(BYTE)( m_LstPar.m_nWidth / ( CMyBitmap::GetFontSize () / 2 )) ;
    m_LstPar.m_btListLineNum = ( m_LstPar.m_nHeight ) / m_LstFnt.m_btFontHeight ;
    m_LstPar.m_btType = btType ;
    if ( ListID )
    {
        strcpy ( m_LstPar.m_strListID, ListID ) ;
    }
    // create the window cut off the vscrollbar
    rgn.CreateRectRgn ( 0, 0, m_LstPar.m_nWidth - 16, m_LstPar.m_nHeight  ) ;
    CMyList::SetWindowRgn ( rgn, true ) ;
    // create the show vscrollbar
    m_LstScr.m_ScrUpRect = CRect ( m_LstPar.m_IntPoint.x + m_LstPar.m_nWidth - 16,
                                   m_LstPar.m_IntPoint.y,
                                   m_LstPar.m_IntPoint.x + m_LstPar.m_nWidth,
                                   m_LstPar.m_IntPoint.y + 16 ) ;
    m_LstScr.m_ScrDoRect = CRect ( m_LstPar.m_IntPoint.x + m_LstPar.m_nWidth - 16,
                                   m_LstPar.m_IntPoint.y + m_LstPar.m_nHeight - 19,
                                   m_LstPar.m_IntPoint.x + m_LstPar.m_nWidth,
                                   m_LstPar.m_IntPoint.y + m_LstPar.m_nHeight - 3) ;
    m_LstScr.m_ScrUpBtn.Create ( "",
                                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                                 m_LstScr.m_ScrUpRect,
                                 CMyList::GetParent (),
                                 1 );
    m_LstScr.m_ScrUpBtn.Init( this, MYSCROLL_UP, rect.left, rect.top ) ;
    m_LstScr.m_ScrDoBtn.Create ( "",
                                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                                 m_LstScr.m_ScrDoRect,
                                 CMyList::GetParent (),
                                 2 );
    m_LstScr.m_ScrDoBtn.Init( this, MYSCROLL_DOWN, rect.left, rect.top ) ;
    m_LstScr.m_ScrUpBtn.Load ( "Button815" ) ;
    m_LstScr.m_ScrDoBtn.Load ( "Button816" ) ;
    return true ;
}

void CMyList::SetOffset(CPoint XandY)
{
    m_LstPar.m_OffsetPoint.x = XandY.x ;
    m_LstPar.m_OffsetPoint.y = XandY.y ;
}

LRESULT CMyList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( message == LB_ADDSTRING || message == LB_INSERTSTRING || message == LB_DELETESTRING || message == LB_RESETCONTENT )
    {
        LRESULT result = CListBox::WindowProc ( message, wParam, lParam ) ;
        SetParameter() ;
        return result ;
    }
    return CListBox::WindowProc(message, wParam, lParam);
}

void CMyList::SetParameter()
{
    m_LstPar.m_nLastClkIndex = -1 ;
    m_LstPar.m_btListCount = CMyList::GetCount () ;
    if ( m_LstPar.m_btListLineNum >= m_LstPar.m_btListCount )
    {
        m_LstPar.m_btShowLineNum = m_LstPar.m_btListCount ;
        m_LstPar.m_bVShow = false;
    }
    else
    {
        m_LstPar.m_btShowLineNum = m_LstPar.m_btListLineNum ;
        m_LstPar.m_bVShow = true ;
    }
}

void CMyList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( m_LstPar.m_btType == SIMPLE_LIST )
    {
        int nCurIndex = CMyList::GetCurSel () ;
        if ( nCurIndex != -1 && m_LstPar.m_ParentCob != NULL )
            m_LstPar.m_ParentCob->PostMessage ( CB_GETITEMDATA,
                                                true,
                                                nCurIndex ) ;
    }
    else if ( m_LstPar.m_btType == COMPLEX_LIST )
    {
        int nCurIndex = CMyList::GetCurSel () ;
        if ( nCurIndex != -1 && m_LstPar.m_ParentCob != NULL )
            if ( m_LstPar.m_nListRealIndex[nCurIndex] != -1 )
                m_LstPar.m_ParentCob->PostMessage ( LB_SETSEL,
                                                    1,
                                                    nCurIndex ) ;
            else
                m_LstPar.m_ParentCob->PostMessage ( LB_SETSEL,
                                                    2,
                                                    nCurIndex ) ;
    }
    CListBox::OnLButtonDblClk(nFlags, point);
}

void CMyList::OnLButtonDown(UINT nFlags, CPoint point)
{
    CListBox::OnLButtonDown ( nFlags, point) ;
    if ( m_LstPar.m_btType == SIMPLE_LIST )
    {
        int nCurIndex = CMyList::GetCurSel () ;
        if ( nCurIndex != -1 && m_LstPar.m_ParentCob != NULL )
            m_LstPar.m_ParentCob->PostMessage ( CB_GETITEMDATA,
                                                NULL,
                                                nCurIndex ) ;
    }
    else if ( m_LstPar.m_btType == COMPLEX_LIST )
    {
        int nCurIndex = CMyList::GetCurSel () ;
        if ( nCurIndex != -1 && m_LstPar.m_ParentCob != NULL )
        {
            if ( m_LstPar.m_nLastClkIndex != nCurIndex )
            {
                if ( m_LstPar.m_nListRealIndex[nCurIndex] != -1 )
                {
                    m_LstPar.m_nLastClkIndex = nCurIndex ;
                    m_LstPar.m_ParentCob->PostMessage ( LB_SETSEL,
                                                        0,
                                                        nCurIndex ) ;
                }
                else
                {
                    m_LstPar.m_nLastClkIndex = nCurIndex ;
                    m_LstPar.m_ParentCob->PostMessage ( LB_SETSEL,
                                                        -1,
                                                        nCurIndex ) ;
                }
            }
            else
            {
                static bool bDouble = false ;
                if ( bDouble )
                {
                    if ( m_LstPar.m_nListRealIndex[nCurIndex] != -1 )
                    {
                        m_LstPar.m_nLastClkIndex = nCurIndex ;
                        m_LstPar.m_ParentCob->PostMessage ( LB_SETSEL,
                                                            0,
                                                            nCurIndex ) ;
                    }
                    else
                    {
                        m_LstPar.m_nLastClkIndex = nCurIndex ;
                        m_LstPar.m_ParentCob->PostMessage ( LB_SETSEL,
                                                            -1,
                                                            nCurIndex ) ;
                    }
                }
                else
                {
                    m_LstPar.m_nLastClkIndex = nCurIndex ;
                    m_LstPar.m_ParentCob->PostMessage ( LB_SETSEL,
                                                        -1,
                                                        -1 ) ;
                }
                bDouble = !bDouble ;
            }
        }
    }
}

void CMyList::SetParentCob(CWnd* ParentCob)
{
    m_LstPar.m_ParentCob = ParentCob ;
}

void CMyList::SetListRealIndex(int nIndex, int nRealIndex)
{
    if ( nIndex < 64 )
    {
        m_LstPar.m_nListRealIndex [nIndex] = nRealIndex ;
    }
}

int CMyList::GetListRealIndex(int nIndex)
{
    if ( nIndex < 64 )
    {
        return m_LstPar.m_nListRealIndex [nIndex] ;
    }
    return -1 ;
}



HBRUSH CMyList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // TODO: Change any attributes of the DC here
    // TODO: Return a different brush if the default is not desired
    pDC->SetBkMode(TRANSPARENT);
    return g_StockBrush;
}

void CMyList::SetShowBack(bool bFlag)
{
    m_bShowBack = bFlag;
}

void CMyList::SetItemFlash(int nIndex, BOOL bFlag)
{
    m_LstPar.m_bIsFlash[nIndex] = bFlag;
}

void CMyList::SetListLine(int nLine)
{
    if (nLine >= 1 && nLine <= _MAX_LINE)
    {
        m_LstPar.m_nLine = nLine;
    }
    else
    {
        m_LstPar.m_nLine = 1;
    }
}

void CMyList::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if ( m_LstPar.m_btType == SIMPLE_LIST )
    {
        int nCurIndex = CMyList::GetCurSel () ;
        if ( nCurIndex != -1 && m_LstPar.m_ParentCob != NULL )
            m_LstPar.m_ParentCob->PostMessage ( WM_MY_MESSAGE,
                                                DEL_LISTITEM,
                                                nCurIndex ) ;
    }
    else if ( m_LstPar.m_btType == COMPLEX_LIST )
    {
        int nCurIndex = CMyList::GetCurSel () ;
        if ( nCurIndex != -1 && m_LstPar.m_ParentCob != NULL )
            if ( m_LstPar.m_nListRealIndex[nCurIndex] != -1 )
                m_LstPar.m_ParentCob->PostMessage ( WM_MY_MESSAGE,
                                                    DEL_LISTITEM,
                                                    nCurIndex ) ;
            else
                m_LstPar.m_ParentCob->PostMessage ( WM_MY_MESSAGE,
                                                    DEL_LISTITEM,
                                                    nCurIndex ) ;
    }
    CListBox::OnRButtonDown(nFlags, point);
}

void CMyList::ShowMLine(int x , int y, DWORD dwColor, char* szText)
{
    if (szText && strlen(szText) > 0)
    {
        char szLine[_MAX_LINE][_MAX_NAMESIZE];
        switch(m_LstPar.m_nLine)
        {
        case 1:
            CMyBitmap::ShowString(x, y, dwColor, szText);
            break;
        case 2:
            {
                int nResult = sscanf(szText, "%s %s", szLine[0], szLine[1]);
                if (nResult != 2)
                {
                    return;
                }
                CMyBitmap::ShowString(x, y, dwColor, szLine[0]);
                CMyBitmap::ShowString(x + CMyBitmap::GetFontSize() * 10, y, dwColor, szLine[1]);
            }
            break;
        case 3:
            {
                int nResult = sscanf(szText, "%s %s %s", szLine[0], szLine[1] , szLine[2]);
                if (nResult != 3)
                {
                    return;
                }
                CMyBitmap::ShowString(x, y, dwColor, szLine[0]);
                CMyBitmap::ShowString(x + CMyBitmap::GetFontSize() * 10, y, dwColor, szLine[1]);
                CMyBitmap::ShowString(x + CMyBitmap::GetFontSize() * 20, y, dwColor, szLine[2]);
            }
            break;
        case 4:
            {
                int nResult = sscanf(szText, "%s %s %s %s", szLine[0], szLine[1] , szLine[2] , szLine[3]);
                if (nResult != 4)
                {
                    return;
                }
                CMyBitmap::ShowString(x, y, dwColor, szLine[0]);
                CMyBitmap::ShowString(x + CMyBitmap::GetFontSize() * 10, y, dwColor, szLine[1]);
                CMyBitmap::ShowString(x + CMyBitmap::GetFontSize() * 20, y, dwColor, szLine[2]);
                CMyBitmap::ShowString(x + CMyBitmap::GetFontSize() * 30, y, dwColor, szLine[3]);
            }
            break;
        default:
            break;
        }
    }
}
