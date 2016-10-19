
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGINSHELP_H__AA7EFB31_A361_4DA3_B32F_98B8A6F64099__INCLUDED_)
#define AFX_DLGINSHELP_H__AA7EFB31_A361_4DA3_B32F_98B8A6F64099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInsHelp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInsHelp dialog
#include "MyButton.h"

class CDlgInsHelp : public CDialog
{
    // Construction
public:
    void Show();
    CDlgInsHelp(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    BOOL				m_bFlash ;
    BOOL				m_bTipShow ;
    int					m_nCurTipIndex ;
    int					m_nActivedHelpAmount;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgInsHelp)
    enum { IDD = IDD_DIALOG_INSHELP };
    CMyButton	m_InsHelpBtnTip;
    CMyButton	m_InsHelpBtnPageUp;
    CMyButton	m_InsHelpBtnPageDown;
    CMyButton	m_InsHelpBtnMore;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgInsHelp)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:
    int GetCurPageHelpAmount();
    int m_nCurTipLine;
    int m_nCurTipsIndex;

    // Generated message map functions
    //{{AFX_MSG(CDlgInsHelp)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnInshelpBtnCloses();
    afx_msg void OnInshelpBtnMore();
    afx_msg void OnInshelpBtnTip();
    afx_msg void OnInshelpBtnPageup();
    afx_msg void OnInshelpBtnPagedown();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
protected:
    void ResetAcitvedHelp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINSHELP_H__AA7EFB31_A361_4DA3_B32F_98B8A6F64099__INCLUDED_)
