
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGIDENTIFY_H__276DF337_1146_4A29_8B97_397D47C2BEF2__INCLUDED_)
#define AFX_DLGIDENTIFY_H__276DF337_1146_4A29_8B97_397D47C2BEF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIdentify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIdentify dialog
#include "MyButton.h"
#include "MyImage.h"
#include "MyStatic.h"

class CDlgIdentify : public CDialog
{
    // Construction
public:
    DWORD GetIdentifyItemId();
    void SetPickUpItemID( int nID );
    void Show();
    CDlgIdentify(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int					m_nPickUpItemID ;
    // Dialog Data
    //{{AFX_DATA(CDlgIdentify)
    enum { IDD = IDD_DIALOG_IDENTIFY };
    CMyStatic	m_StaIdentMoney;
    CMyImage	m_ImgGood;
    CMyButton	m_BtnOk;
    CMyButton	m_BtnCancel;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgIdentify)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgIdentify)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnCancel();
    afx_msg void OnBtnOk();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnImgGood();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIDENTIFY_H__276DF337_1146_4A29_8B97_397D47C2BEF2__INCLUDED_)
