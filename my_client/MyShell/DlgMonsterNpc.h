
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTERNPC_H__97BCF4F2_78B9_4B26_B543_F9F280E9B4D1__INCLUDED_)
#define AFX_DLGMONSTERNPC_H__97BCF4F2_78B9_4B26_B543_F9F280E9B4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonsterNpc.h : header file
//
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterNpc dialog

class CDlgMonsterNpc : public CDialog
{
    // Construction
public:
    BOOL m_bShow;
    void OpenMonsterNpc(OBJID idNpc);
    void Show();
    CDlgMonsterNpc(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgMonsterNpc)
    enum { IDD = IDD_DIALOG_MONSTERNPC };
    CMyButton	m_BtnOpenHatch;
    CMyButton	m_BtnOpenDepot;
    CMyButton	m_BtnImprove;
    CMyButton	m_BtnClose;
    CMyButton	m_BtnAlive;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonsterNpc)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    OBJID m_IdNpc;
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgMonsterNpc)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnAlive();
    afx_msg void OnBtnImprove();
    afx_msg void OnBtnOpendepot();
    afx_msg void OnBtnOpenhatch();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTERNPC_H__97BCF4F2_78B9_4B26_B543_F9F280E9B4D1__INCLUDED_)
