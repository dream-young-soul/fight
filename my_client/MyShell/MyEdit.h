
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYEDIT_H__1CF4E1A5_4240_41A7_A204_8C76772015B8__INCLUDED_)
#define AFX_MYEDIT_H__1CF4E1A5_4240_41A7_A204_8C76772015B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMyEdit window
#include "MyEditRect.h"

class CMyEdit : public CEdit
{
    // Construction
public:
    CMyEdit();

    // Attributes
public:
    CMyEditRect		m_EditRect;			// The edit's rect for receive the mouse&button message
private:
    struct MyEditParameter
    {
        CPoint m_Pnt ;
        CPoint m_IntPnt ;
        int m_nWidth ;
        int m_nHeight ;
        int m_nSelNum ;
        BYTE m_btRightLeft ;				// the owner's right left
        BYTE m_btEndCharPos ;				// the end's char's pos
        DWORD m_dwSelColor ;
        UINT m_uFrame ;						// the current frame
        char m_strEditID[64] ;				// the edit id
        BOOL	m_bMultiLine ;				// the multiline
        BOOL	m_bChinese;
        int				m_nLastSelEnd;
    } m_EdtPar ;
    struct MyEditFont
    {
        BOOL m_bPassword ;
        BYTE m_btFontWidth ;
        int m_nCharNum ;
        CPoint m_StartPoint ;
        CPoint m_EndPoint ;
        CPoint m_OffsetPoint ;
        DWORD m_dwFontColor ;
        DWORD m_dwBackColor ;
    } m_EdtFnt ;
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyEdit)
    //}}AFX_VIRTUAL

    // Implementation
public:
    char* GetEditBkID();
    void SetEditBkID(char* EditID);
    void ReInit(int xPos, int yPos);
    void GoNextLine ();
    void SetPasswordType( );
    void SetOffset ( CPoint offsetPnt );
    void SetFontColor (DWORD dwColor );
    BOOL Init ( int xPos, int yPos, BOOL bMultiLine = false, char* EditID = NULL, DWORD dwBkColor = 0x000000, DWORD dwFontColor = 0xffffff );
    void ShowChar();
    void Show( int x, int y );
    void EnableChinese( BOOL bChinese );
    virtual ~CMyEdit();

    // Generated message map functions
protected:
    void ShowMultiLine();
    //{{AFX_MSG(CMyEdit)
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnEnable(BOOL bEnable);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__1CF4E1A5_4240_41A7_A204_8C76772015B8__INCLUDED_)
