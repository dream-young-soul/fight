
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGQUERY_ROLE_H__7F2EE870_574F_4B36_890C_111149EC92EF__INCLUDED_)
#define AFX_DLGQUERY_ROLE_H__7F2EE870_574F_4B36_890C_111149EC92EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgQuery_Role.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery_Role dialog
#include "MyStatic.h"
#include "MyImage.h"

class CDlgQuery_Role : public CDialog
{
    // Construction
public:
    void Show ( int xPos, int yPos );
    CDlgQuery_Role(CWnd* pParent = NULL);   // standard constructor
    void SetRoleInfo () ;

private:
    CPoint	m_DlgPnt ;
    int		m_nShowType ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgQuery_Role)
    enum { IDD = IDD_DIALOG_QUERY_ROLE };
    CMyStatic	m_StaPotential;
    CMyStatic	m_StaTitle;
    CMyStatic	m_StaStrong;
    CMyStatic	m_StaSmart;
    CMyStatic	m_StaPk;
    CMyStatic	m_StaLev;
    CMyStatic	m_StaHa;
    CMyStatic	m_StaFere;
    CMyStatic	m_StaFamor;
    CMyStatic	m_StaEnergy;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgQuery_Role)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgQuery_Role)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQUERY_ROLE_H__7F2EE870_574F_4B36_890C_111149EC92EF__INCLUDED_)
