
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTASKUPITEM_H__9F2E8688_825D_4809_BE11_5DEA8B825269__INCLUDED_)
#define AFX_DLGTASKUPITEM_H__9F2E8688_825D_4809_BE11_5DEA8B825269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTaskUpItem.h : header file
//
#include "MyButton.h"
#include "MyImage.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTaskUpItem dialog

class CDlgTaskUpItem : public CDialog
{
    // Construction
public:
    void SetTaskUpItem(OBJID idTaskItem);
    void SetUptaskId(UCHAR idTask);
    CPoint m_Pnt;
    void Show();
    BOOL m_bShow;
    CDlgTaskUpItem(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTaskUpItem)
    enum { IDD = IDD_DIALOG_UPTASKITEM };
    CMyImage	m_ImgTaskItem;
    CMyButton	m_BtnUpCancel;
    CMyButton	m_BtnUpOk;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTaskUpItem)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    UCHAR m_ucIdTask;
    OBJID m_TaskItemId;
    // Generated message map functions
    //{{AFX_MSG(CDlgTaskUpItem)
    afx_msg void OnImgTaskitem();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnUpcancel();
    afx_msg void OnBtnUpok();
    afx_msg void OnDoubleclickedImgTaskitem();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTASKUPITEM_H__9F2E8688_825D_4809_BE11_5DEA8B825269__INCLUDED_)
