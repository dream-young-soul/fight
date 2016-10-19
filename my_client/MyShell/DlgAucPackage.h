
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGAUCPACKAGE_H__A81E682A_82CB_41DC_85B0_754CA172D482__INCLUDED_)
#define AFX_DLGAUCPACKAGE_H__A81E682A_82CB_41DC_85B0_754CA172D482__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAucPackage.h : header file
//
#include "MyButton.h"
#include "MyGrid.h"
#include "MyImage.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAucPackage dialog

class CDlgAucPackage : public CDialog
{
    // Construction
public:
    CDlgAucPackage(CWnd* pParent = NULL);   // standard constructor
private:
    CPoint m_Pnt;
public:
    BOOL m_bShow;
public:
    void SetAucPackage();
    void SetAuctionNpcId(OBJID idNpc);
    void Show();
    // Dialog Data
    //{{AFX_DATA(CDlgAucPackage)
    enum { IDD = IDD_DIALOG_AUCPACKAGE };
    CMyImage	m_ImgMoneyItem;
    CMyButton	m_BtnClose;
    CMyGrid	m_GridAucitem;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgAucPackage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    OBJID m_AuctionNpcId;

    // Generated message map functions
    //{{AFX_MSG(CDlgAucPackage)
    afx_msg void OnBtnClose();
    afx_msg void OnImgMoneyitem();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUCPACKAGE_H__A81E682A_82CB_41DC_85B0_754CA172D482__INCLUDED_)
