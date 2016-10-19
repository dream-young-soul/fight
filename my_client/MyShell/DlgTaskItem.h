
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTASKITEM_H__035B5D44_5E1B_49F3_9410_3F1A1D1BE810__INCLUDED_)
#define AFX_DLGTASKITEM_H__035B5D44_5E1B_49F3_9410_3F1A1D1BE810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTaskItem.h : header file
//
#include "MyList.h"
#include "MyStatic.h"
#include "MyButton.h"
#include "MyEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTaskItem dialog

class CDlgTaskItem : public CDialog
{
    // Construction
public:
    BOOL IsSetOk();
    void ReSetDlg();
    BOOL m_bShow;
    void Show();
    CDlgTaskItem(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTaskItem)
    enum { IDD = IDD_DIALOG_TASKITEM };
    CMyEdit	m_EditItemSoul;
    CMyEdit	m_EditItemName;
    CMyEdit	m_EditItemMark;
    CMyEdit		m_EditItemAmount;
    CMyButton	m_BtnSetOk;
    CMyButton	m_BtnSetCancel;
    CMyButton	m_BtnClose;
    int		m_nItemAmount;
    int		m_nItemMark;
    int		m_nItemSoul;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTaskItem)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_bSetOk;
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgTaskItem)
    afx_msg void OnBtnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnSetok();
    afx_msg void OnBtnSetcancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTASKITEM_H__035B5D44_5E1B_49F3_9410_3F1A1D1BE810__INCLUDED_)
