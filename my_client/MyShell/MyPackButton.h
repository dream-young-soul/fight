
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_PACKBUTTON_H__09937443_ADBD_4C9E_8331_EFF50D12A5F5__INCLUDED_)
#define AFX_PACKBUTTON_H__09937443_ADBD_4C9E_8331_EFF50D12A5F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PackButton.h : header file
//
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CMyPackButton window

class CMyPackButton : public CMyButton
{
    // Construction
public:
    CMyPackButton();

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyPackButton)
    //}}AFX_VIRTUAL

    // Implementation
public:
    void Show(int x, int y);
    CPoint m_MoveInitPos;
    CPoint GetMovePos();
    CPoint m_MovePos;
    virtual ~CMyPackButton();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyPackButton)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKBUTTON_H__09937443_ADBD_4C9E_8331_EFF50D12A5F5__INCLUDED_)
