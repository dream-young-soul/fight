
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTERHATCH_H__85ADCAA4_355D_437A_A6C9_D3B812474A43__INCLUDED_)
#define AFX_DLGMONSTERHATCH_H__85ADCAA4_355D_437A_A6C9_D3B812474A43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonsterHatch.h : header file
//
#include "MyStatic.h"
#include "MyButton.h"
#include "MyGrid.h"
#include "MyImage.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterHatch dialog

class CDlgMonsterHatch : public CDialog
{
    // Construction
public:
    OBJID m_IdAddEggs[5];
    void SetGrid();
    void OpenHatch(OBJID idNpc);
    void GetMonsterHatch();
    void Show();
    void AddEgg(OBJID idEgg);
    BOOL m_bShow;
    CDlgMonsterHatch(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgMonsterHatch)
    enum { IDD = IDD_DIALOG_MONSTERHATCH };
    CMyButton	m_BtnCheckOk;
    CMyButton	m_BtnCancel;
    CMyStatic	m_StaNpcInfo;
    CMyImage	m_ImgNpcFace;
    CMyGrid	m_GridEggs;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonsterHatch)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void ResetAddEggs();
    int m_nAddEggs;
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgMonsterHatch)
    afx_msg void OnBtnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnCancel();
    afx_msg void OnBtnCheckok();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTERHATCH_H__85ADCAA4_355D_437A_A6C9_D3B812474A43__INCLUDED_)
