
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGEMBLEMBUY_H__650FD231_DD23_4D2C_9454_BC4F78B526E1__INCLUDED_)
#define AFX_DLGEMBLEMBUY_H__650FD231_DD23_4D2C_9454_BC4F78B526E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEmblemBuy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEmblemBuy dialog
#include "MyImage.h"
#include "MyButton.h"

class CDlgEmblemBuy : public CDialog
{
    // Construction
public:
    void InsertEmblem();
    void SetEmblemBuy();
    void Show();
    CDlgEmblemBuy(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
    int					m_nCurPage ;
private:
    CPoint				m_Pnt ;
    int					m_nCurIndex ;
    // Dialog Data
    //{{AFX_DATA(CDlgEmblemBuy)
    enum { IDD = IDD_DIALOG_EMBLEMBUY };
    CMyButton	m_BtnOk;
    CMyImage	m_Img6;
    CMyImage	m_Img5;
    CMyImage	m_Img4;
    CMyImage	m_Img3;
    CMyImage	m_Img2;
    CMyImage	m_Img1;
    CMyButton	m_BtnUp;
    CMyButton	m_BtnDown;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgEmblemBuy)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgEmblemBuy)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnUp();
    afx_msg void OnBtnDown();
    afx_msg void OnBtnClose();
    afx_msg void OnImgEmblem1();
    afx_msg void OnImgEmblem2();
    afx_msg void OnImgEmblem3();
    afx_msg void OnImgEmblem4();
    afx_msg void OnImgEmblem5();
    afx_msg void OnImgEmblem6();
    afx_msg void OnBtnOk();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEMBLEMBUY_H__650FD231_DD23_4D2C_9454_BC4F78B526E1__INCLUDED_)
