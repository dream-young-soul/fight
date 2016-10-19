
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMIPMAP_H__89EA4D28_B9C6_41BA_911C_F2B753893F6B__INCLUDED_)
#define AFX_DLGMIPMAP_H__89EA4D28_B9C6_41BA_911C_F2B753893F6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMipmap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMipmap dialog
#include "MyButton.h"
#include "MyStatic.h"

class CDlgMipmap : public CDialog
{
    // Construction
public:
    void Show();
    CDlgMipmap(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    OBJID				m_MapID ;
public:
    void OpenBigmap();
    // Dialog Data
    //{{AFX_DATA(CDlgMipmap)
    enum { IDD = IDD_DIALOG_MIPMAP };
    CMyButton	m_BtnViewchg;
    CMyButton	m_BtnBigout;
    CMyStatic	m_StaPosy;
    CMyStatic	m_StaPosx;
    CMyStatic	m_StaName;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMipmap)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_iMipmapmode;

    // Generated message map functions
    //{{AFX_MSG(CDlgMipmap)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnViewchg();
    afx_msg void OnBtnBigout();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMIPMAP_H__89EA4D28_B9C6_41BA_911C_F2B753893F6B__INCLUDED_)
