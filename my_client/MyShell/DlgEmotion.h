
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGEMOTION_H__53782E37_68F6_4570_B44B_4C43AB897E03__INCLUDED_)
#define AFX_DLGEMOTION_H__53782E37_68F6_4570_B44B_4C43AB897E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEmotion.h : header file
//
#include "MyButton.h"
#include "MyStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEmotion dialog

class CDlgEmotion : public CDialog
{
    // Construction
public:
    int GetCurSelEmotion();
    BOOL m_bShow;
    void Show();
    CDlgEmotion(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgEmotion)
    enum { IDD = IDD_DIALOG_EMOTION };
    CMyStatic	m_StaPage;
    CMyButton	m_BtnPageRight;
    CMyButton	m_BtnPageLeft;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgEmotion)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgEmotion)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnPageleft();
    afx_msg void OnBtnPageright();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    int m_nCurPage;
    int m_nCurSelEmotion;
    CPoint m_Pnt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEMOTION_H__53782E37_68F6_4570_B44B_4C43AB897E03__INCLUDED_)
