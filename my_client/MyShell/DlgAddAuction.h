
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGADDAUCTION_H__B1B88F0D_4FA5_47DF_A9CF_8963C945064B__INCLUDED_)
#define AFX_DLGADDAUCTION_H__B1B88F0D_4FA5_47DF_A9CF_8963C945064B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddAuction.h : header file
//
#include "MyButton.h"
#include "MyEdit.h"
#include "MyImage.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAddAuction dialog

class CDlgAddAuction : public CDialog
{
    // Construction
public:
    int m_nAucMoney;
    void SetPickUpIcon(OBJID idItem);
    void SetAuctionNpcId(OBJID idNpc);
    BOOL m_bShow;
    void Show();
    CDlgAddAuction(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgAddAuction)
    enum { IDD = IDD_DIALOG_ADDAUCITEM };
    CMyImage	m_ImgAucItem;
    CMyButton	m_BtnAddCancel;
    CMyButton	m_BtnAddOk;
    CMyEdit	m_EditAucMoney;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgAddAuction)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    OBJID m_AucNpcID;
    OBJID m_AucItemID;
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgAddAuction)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnAddcancel();
    afx_msg void OnBtnAddok();
    afx_msg void OnBtnClose();
    afx_msg void OnImgAucitem();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDAUCTION_H__B1B88F0D_4FA5_47DF_A9CF_8963C945064B__INCLUDED_)
