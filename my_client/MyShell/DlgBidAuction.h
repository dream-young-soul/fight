
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGBIDAUCTION_H__A9137852_759A_4B8F_A6CC_944629F02696__INCLUDED_)
#define AFX_DLGBIDAUCTION_H__A9137852_759A_4B8F_A6CC_944629F02696__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBidAuction.h : header file
//
#include "MyButton.h"
#include "MyStatic.h"
#include "MyEdit.h"
#include "MyImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBidAuction dialog

class CDlgBidAuction : public CDialog
{
    // Construction
public:
    void SetAuctionData();
    void SetAuctionNpcId(OBJID idNpc);
    BOOL m_bShow;
    OBJID m_AuctionNpcId;
    void Show();
    CDlgBidAuction(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgBidAuction)
    enum { IDD = IDD_DIALOG_BIDAUCTION };
    CMyStatic	m_StaNowValue;
    CMyStatic	m_StaBidName;
    CMyImage	m_ImgAuctionItem;
    CMyEdit	m_EditBidValue;
    CMyButton	m_BtnClose;
    CMyButton	m_BtnBid;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgBidAuction)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgBidAuction)
    afx_msg void OnBtnBid();
    afx_msg void OnBtnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBIDAUCTION_H__A9137852_759A_4B8F_A6CC_944629F02696__INCLUDED_)
