
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYCOMBO_H__055278BE_217C_49C2_947E_54B6D33E54A7__INCLUDED_)
#define AFX_MYCOMBO_H__055278BE_217C_49C2_947E_54B6D33E54A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCombo window
#include "DlgListBox.h"
#include "MyEditRect.h"

class CMyCombo : public CEdit
{
    // Construction
public:
    CMyCombo();

    // Attributes
public:
    struct MyComboParameter
    {
        CPoint m_Pnt ;						// the show point
        CPoint m_IntPnt ;					// the left point
        CMyBitmap m_Bmp ;					// the owner bitmap
        int m_nWidth ;						// the owner's width
        int m_nHeight ;						// the owner's height
        BYTE m_btRightLeft ;				// the owner's right left
        BYTE m_btEndCharPos ;				// the end's char's pos
        int m_nSelNum ;						// the sel number
        BOOL m_bBorder ;					// the edit's border
        BOOL m_bChinese;
        int	m_nLastSelEnd;
    } m_CobPar ;
    struct MyComboFont
    {
        BYTE m_btFontWidth ;				// the font width
        CPoint m_StartPoint ;				// the sel start point
        CPoint m_EndPoint ;					// the sel end point
        DWORD  m_dwFontColor ;				// the font color
        DWORD  m_dwBackColor ;				// the back color
        int		m_nCharNum;
    } m_CobFnt ;
    CDlgListBox*	m_CobLstBox;
    CMyEditRect		m_EditRect;				// The edit's rect for receive the mouse&button message
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyCombo)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void InitListBox ( CDlgListBox* ListBox );
    BOOL Init ( int xPos, int yPos, char* FileName = NULL, DWORD dwFontColor = 0xffffff, DWORD dwBackColor = 0x000000 );
    void ShowChar();
    void Show ( int x, int y );
    virtual ~CMyCombo();
    void EnableChinese( BOOL bChinese );
    void ReInit( int xPos, int yPos );

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyCombo)
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCOMBO_H__055278BE_217C_49C2_947E_54B6D33E54A7__INCLUDED_)
