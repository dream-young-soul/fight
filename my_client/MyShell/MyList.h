
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYLIST_H__A7857E9D_70A8_4A65_86E6_93923883A587__INCLUDED_)
#define AFX_MYLIST_H__A7857E9D_70A8_4A65_86E6_93923883A587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyList.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMyList window
#include "MyScrollButton.h"

const int SIMPLE_LIST = 1 ;
const int COMPLEX_LIST = 2 ;

class CMyList : public CListBox
{
    // Construction
public:
    CMyList();

    // Attributes
private:
    struct MyListParameter
    {
        BYTE		m_btType ;				// the list type
        BYTE		m_btListCount ;			// the list count
        BYTE		m_btListLineNum ;		// the list include line number
        BYTE		m_btShowLineNum ;		// the list show line number
        int			m_nWidth ;				// the list width
        int			m_nHeight ;				// the list height
        CPoint		m_ListPoint ;			// the list current show point
        CPoint		m_IntPoint ;			// the list comparative to the parent dialog point
        CRect		m_SelRect ;				// the list sel's rect
        CRect		m_ItemRect ;			// the list current show string's rect
        int			m_nTopIndex ;			// the list top visible line's index
        int			m_nCurIndex ;			// the list current select line's index
        CPoint		m_OffsetPoint ;			// the list pic offset point
        BOOL		m_bVShow ;				// Show the vscroll or not
        CWnd*		m_ParentCob ;			// the parent combo's cwnd
        char		m_strListID[64] ;		// the list id
        int			m_nLastClkIndex ;		// the lbutton last click index
        int			m_nListRealIndex[64] ;	// the list able
        BOOL		m_bIsFlash[64] ;		// flash flag
        int			m_nLine;				// list line number
        DWORD       m_dwShowTime[64] ;
    } m_LstPar;
    struct MyListFont
    {
        BYTE		m_btFontHeight ;		// the list's font height
        DWORD		m_dwFontColor ;			// hte list's font color
        BYTE		m_btCharNum ;			// the list line char number
    } m_LstFnt;
    struct MyScrParameter
    {
        CRect		m_ScrUpRect ;			// the scrollbar up button rect
        CRect		m_ScrDoRect ;			// the scrollbar down button bmp
        CMyScrollButton m_ScrUpBtn ;		// the scrollbar up button
        CMyScrollButton m_ScrDoBtn ;		// the scrollbar Down button
    } m_LstScr;
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyList)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL
    // Implementation
public:
    void ShowMLine(int x , int y, DWORD dwColor, char* szText);
    void SetItemFlash(int nIndex, BOOL bFlag);
    void SetListLine(int nLine);
    void SetShowBack(bool bFlag);
    int GetListRealIndex ( int nIndex );
    void SetListRealIndex ( int nIndex, int nRealIndex );
    void SetParentCob( CWnd* ParentCob ) ;
    void SetParameter ();
    void SetOffset ( CPoint XandY );
    BOOL Init ( int xPos, int yPos, char* ListID = NULL, BYTE btType = 0 );
    void SetFontColor ( DWORD dwColor );
    void ShowVScroll();
    void ShowChar ();
    void ShowSel ();
    void Show ( int x, int y);
    virtual ~CMyList();

    // Generated message map functions
protected:
    bool m_bShowBack;
    //{{AFX_MSG(CMyList)
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLIST_H__A7857E9D_70A8_4A65_86E6_93923883A587__INCLUDED_)
