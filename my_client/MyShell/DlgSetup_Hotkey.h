
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSETUP_HOTKEY_H__7C9858AF_855B_4F11_B485_8638C4BE511F__INCLUDED_)
#define AFX_DLGSETUP_HOTKEY_H__7C9858AF_855B_4F11_B485_8638C4BE511F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup_Hotkey.h : header file
//
#include "MyList.h"
#include "MyButton.h"
#include "MyStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Hotkey dialog

class CDlgSetup_Hotkey : public CDialog
{
    // Construction
public:
    int m_nHotKeyIndex;
    int m_nListIndex;
    void SetHotKeyList();
    BOOL m_bShow;
    void Show(int x, int y);
    CDlgSetup_Hotkey(CWnd* pParent = NULL);   // standard constructor
    // Dialog Data
    //{{AFX_DATA(CDlgSetup_Hotkey)
    enum { IDD = IDD_DIALOG_SETUP_HOTKEY };
    CMyButton	m_BtnUp;
    CMyButton	m_BtnDown;
    CMyList	m_ListHotKeyN;
    CMyList	m_ListHotKeyM;
    CMyButton	m_BtnSetKey;
    CMyButton	m_BtnResetKey;
    CMyStatic	m_StaKey;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetup_Hotkey)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;
    // Generated message map functions
    //{{AFX_MSG(CDlgSetup_Hotkey)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnResetkeys();
    afx_msg void OnBtnSetkey();
    afx_msg void OnBtnDown();
    afx_msg void OnBtnUp();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_HOTKEY_H__7C9858AF_855B_4F11_B485_8638C4BE511F__INCLUDED_)
