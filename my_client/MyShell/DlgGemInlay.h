
//**********************************************************\\
// ´úÂë±à¼­Æ÷\\
//**********************************************************\\
\\
#if !defined(AFX_DLGGEMINLAY_H__F2F7A57B_DE0B_4448_8AE5_548D2EE87E0E__INCLUDED_)
#define AFX_DLGGEMINLAY_H__F2F7A57B_DE0B_4448_8AE5_548D2EE87E0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGemInlay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGemInlay dialog

class CDlgGemInlay : public CDialog
{
    // Construction
public:
    CDlgGemInlay(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgGemInlay)
    enum { IDD = IDD_DIALOG_GEM };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgGemInlay)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgGemInlay)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGEMINLAY_H__F2F7A57B_DE0B_4448_8AE5_548D2EE87E0E__INCLUDED_)
