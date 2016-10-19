
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYSCROLLBUTTON_H__D4F2A33E_BAAF_4B8B_B16B_400A6231FA49__INCLUDED_)
#define AFX_MYSCROLLBUTTON_H__D4F2A33E_BAAF_4B8B_B16B_400A6231FA49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyScrollButton.h : header file
//
#define MYSCROLL_UP		0
#define MYSCROLL_DOWN	1
#define MYSCROLL_ROLL	2
#define MYSCROLL_DROP	3
/////////////////////////////////////////////////////////////////////////////
// CMyScrollButton window
class CMyScrollButton : public CButton
{
    // Construction
public:
    CMyScrollButton();

    // Attributes
private:
    CWnd* m_ParentWnd ;
    BYTE m_btScrBtnType ;
    char m_strScrButtonID[64] ;
    CPoint m_ScrBtnPnt ;
    BOOL	m_bDrpValue ;
    UINT	m_uFrame ;
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyScrollButton)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void Show ( int xPos, int yPos );
    BOOL Load ( char* ScrButtonID);
    void Init ( CWnd* ListWnd, BYTE btScrBtnType, int nPosX = 0, int nPosY = 0 );
    virtual ~CMyScrollButton();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyScrollButton)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSCROLLBUTTON_H__D4F2A33E_BAAF_4B8B_B16B_400A6231FA49__INCLUDED_)
