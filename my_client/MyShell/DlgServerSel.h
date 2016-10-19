
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSERVERSEL_H__B21C91C1_0E7F_4867_951B_16D063355696__INCLUDED_)
#define AFX_DLGSERVERSEL_H__B21C91C1_0E7F_4867_951B_16D063355696__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgServerSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgServerSel dialog

class CDlgServerSel : public CDialog
{
    // Construction
public:
    CDlgServerSel(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgServerSel)
    enum { IDD = IDD_DIALOG_SERVER };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgServerSel)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgServerSel)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERVERSEL_H__B21C91C1_0E7F_4867_951B_16D063355696__INCLUDED_)
