
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MYEDITRECT_H__DBB46217_5B5F_49AA_9DAF_8560DAD443BA__INCLUDED_)
#define AFX_MYEDITRECT_H__DBB46217_5B5F_49AA_9DAF_8560DAD443BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEditRect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEditRect window

class CMyEditRect : public CButton
{
    // Construction
public:
    CMyEditRect();

    // Attributes
private:
    CWnd* m_pParentWnd;
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyEditRect)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void Init( CWnd* pParentWnd);
    virtual ~CMyEditRect();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyEditRect)
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDITRECT_H__DBB46217_5B5F_49AA_9DAF_8560DAD443BA__INCLUDED_)
