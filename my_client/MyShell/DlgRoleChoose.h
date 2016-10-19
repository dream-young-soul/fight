
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGROLECHOOSE_H__F710F48E_D755_4AAC_A795_DA4F0F491E51__INCLUDED_)
#define AFX_DLGROLECHOOSE_H__F710F48E_D755_4AAC_A795_DA4F0F491E51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRoleChoose.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRoleChoose dialog
#include "MyImage.h"
#include "MyStatic.h"
#include "MyButton.h"
#include "3DSimpleRole.h"

class CDlgRoleChoose : public CDialog
{
    // Construction
public:
    void CreateRole ( BYTE btRoleType );
    void Show();
    CDlgRoleChoose(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    BOOL				m_bLoad ;
    BYTE				m_btRoleType ;
    C3DSimpleRole		m_ShowRole ;
    BOOL				m_b1024 ;

public:
    void ReInit();
    // Dialog Data
    //{{AFX_DATA(CDlgRoleChoose)
    enum { IDD = IDD_DIALOG_ROLECHOOSE };
    CMyStatic		m_RoleChooseDataSta ;
    CMyButton		m_RoleChooseReturnBtn ;
    CMyButton		m_RoleChooseOkBtn ;
    CMyButton		m_HelpBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgRoleChoose)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgRoleChoose)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnRolechooseBtnReturn();
    afx_msg void OnBtnHelp();
    afx_msg void OnRolechooseBtnOk();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROLECHOOSE_H__F710F48E_D755_4AAC_A795_DA4F0F491E51__INCLUDED_)
