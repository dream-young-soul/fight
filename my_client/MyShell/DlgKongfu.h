
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGKONGFU_H__2B36748F_2B85_4C0D_B1A6_CFC0CCA7554D__INCLUDED_)
#define AFX_DLGKONGFU_H__2B36748F_2B85_4C0D_B1A6_CFC0CCA7554D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKongfu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgKongfu dialog
#include "MyImage.h"
#include "MyButton.h"

class CDlgKongfu : public CDialog
{
    // Construction
public:
    void InitKongfuDialog();
    void Show();
    CDlgKongfu(CWnd* pParent = NULL);   // standard constructor

    BOOL		m_bShow ;
private:
    CPoint		m_Pnt ;
    DWORD		m_KongfuIdType[50] ;
    int			m_nCurBase ;
    // Dialog Data
    //{{AFX_DATA(CDlgKongfu)
    enum { IDD = IDD_DIALOG_KONGFU };
    CMyButton	m_KongfuBtnU;
    CMyButton	m_KongfuBtnD;
    CMyImage	m_KongfuImg1;
    CMyImage	m_KongfuImg2;
    CMyImage	m_KongfuImg3;
    CMyImage	m_KongfuImg4;
    CMyImage	m_KongfuImg5;
    CMyImage	m_KongfuImg6;
    CMyImage	m_KongfuImg7;
    CMyImage	m_KongfuImg8;
    CMyImage	m_KongfuImg9;
    CMyImage	m_KongfuImg10;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgKongfu)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgKongfu)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnKongfuImg1();
    afx_msg void OnKongfuImg2();
    afx_msg void OnKongfuImg3();
    afx_msg void OnKongfuImg4();
    afx_msg void OnKongfuImg5();
    afx_msg void OnKongfuImg6();
    afx_msg void OnKongfuImg7();
    afx_msg void OnKongfuImg8();
    afx_msg void OnKongfuImg9();
    afx_msg void OnKongfuImg10();
    afx_msg void OnKongfuBtnUp();
    afx_msg void OnKongfuBtnDown();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKONGFU_H__2B36748F_2B85_4C0D_B1A6_CFC0CCA7554D__INCLUDED_)
