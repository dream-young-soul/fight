
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGACT_H__629F97E1_D69A_4DB7_82E5_28EE20291A05__INCLUDED_)
#define AFX_DLGACT_H__629F97E1_D69A_4DB7_82E5_28EE20291A05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAct.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAct dialog
#include "ND_Bitmap.h"
#include "MyButton.h"

class CDlgAct : public CDialog
{
    // Construction
public:
    void SetCurAct( int nIndex );
    void Show ();
    CDlgAct(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
private:
    BOOL		m_bLoad ;
    //	CMyBitmap	m_Bmp ;
    CPoint		m_Pnt ;
    // Dialog Data
    //{{AFX_DATA(CDlgAct)
    enum { IDD = IDD_DIALOG_ACT };
    CMyButton		m_ActBtn1 ;
    CMyButton		m_ActBtn2 ;
    CMyButton		m_ActBtn3 ;
    CMyButton		m_ActBtn4 ;
    CMyButton		m_ActBtn5 ;
    CMyButton		m_ActBtn6 ;
    CMyButton		m_ActBtn7 ;
    CMyButton		m_ActBtn8 ;
    CMyButton		m_ActBtn9 ;
    CMyButton		m_ActBtn10 ;
    CMyButton		m_CloseBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgAct)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgAct)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnActBtn1();
    afx_msg void OnActBtn2();
    afx_msg void OnActBtn3();
    afx_msg void OnActBtn4();
    afx_msg void OnActBtn5();
    afx_msg void OnActBtn6();
    afx_msg void OnActBtn7();
    afx_msg void OnActBtn8();
    afx_msg void OnActBtn9();
    afx_msg void OnActBtn10();
    afx_msg void OnBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGACT_H__629F97E1_D69A_4DB7_82E5_28EE20291A05__INCLUDED_)
