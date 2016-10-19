
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMAGIC_H__98A26557_6094_4F87_84BE_5A8A71982AF3__INCLUDED_)
#define AFX_DLGMAGIC_H__98A26557_6094_4F87_84BE_5A8A71982AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMagic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMagic dialog
#include "MyImage.h"
#include "MyButton.h"

const int MAGIC_ICON_SIZE = 32 ;
const int MAIN_DLG_SIZE	= 40 ;

class CDlgMagic : public CDialog
{
    // Construction
public:
    void InitMagicDialog();
    void Show();
    CDlgMagic(CWnd* pParent = NULL);   // standard constructor

    BOOL		m_bShow ;
private:
    CPoint		m_Pnt ;
    DWORD		m_MagicIdType[50] ;
    int			m_nCurBase ;
    // Dialog Data
    //{{AFX_DATA(CDlgMagic)
    enum { IDD = IDD_DIALOG_MAGIC };
    CMyButton	m_MagicBtnU;
    CMyButton	m_MagicBtnD;
    CMyImage	m_MagicImg10;
    CMyImage	m_MagicImg9;
    CMyImage	m_MagicImg8;
    CMyImage	m_MagicImg7;
    CMyImage	m_MagicImg6;
    CMyImage	m_MagicImg5;
    CMyImage	m_MagicImg4;
    CMyImage	m_MagicImg3;
    CMyImage	m_MagicImg2;
    CMyImage	m_MagicImg1;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMagic)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgMagic)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnMagicImg1();
    afx_msg void OnMagicImg2();
    afx_msg void OnMagicImg3();
    afx_msg void OnMagicImg4();
    afx_msg void OnMagicImg5();
    afx_msg void OnMagicImg6();
    afx_msg void OnMagicImg7();
    afx_msg void OnMagicImg8();
    afx_msg void OnMagicImg9();
    afx_msg void OnMagicImg10();
    afx_msg void OnMagicBtnDown();
    afx_msg void OnMagicBtnUp();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAGIC_H__98A26557_6094_4F87_84BE_5A8A71982AF3__INCLUDED_)
