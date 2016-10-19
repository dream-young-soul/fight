
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_TESTSTATIC_H__13AB40C2_A84B_4208_9DFF_7E5E6B76FD45__INCLUDED_)
#define AFX_TESTSTATIC_H__13AB40C2_A84B_4208_9DFF_7E5E6B76FD45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestStatic window

class CTestStatic : public CStatic
{
    // Construction
public:
    CTestStatic();

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTestStatic)
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CTestStatic();

    // Generated message map functions
protected:
    //{{AFX_MSG(CTestStatic)
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSTATIC_H__13AB40C2_A84B_4208_9DFF_7E5E6B76FD45__INCLUDED_)
