
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGNPCBUY_H__1D440B00_667A_4B83_96A1_EAE50D717B19__INCLUDED_)
#define AFX_DLGNPCBUY_H__1D440B00_667A_4B83_96A1_EAE50D717B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNpcBuy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcBuy dialog
#include "MyGrid.h"
#include "MyStatic.h"
#include "MyButton.h"

const int SHOP_AMOUNT = 24 ;

class CDlgNpcBuy : public CDialog
{
    // Construction
public:
    void Show ();
    CDlgNpcBuy(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
private:
    CPoint		m_Pnt ;
    int			m_nCurrentAmount ;
public:
    void ReSetShop();
    void OnCloseB();
    void CloseShop();
    DWORD GetShopID() {return m_IdShopNpc;}
    int  OpenShop (OBJID idNpc );
    // Dialog Data
    //{{AFX_DATA(CDlgNpcBuy)
    enum { IDD = IDD_DIALOG_NPCBUY };
    CMyButton	m_NpcTalkBtn;
    CMyButton	m_NpcAskBtn;
    CMyGrid		m_NpcBuySaleGri;
    CMyButton	m_NpcBuyCloseBtn ;
    CMyButton	m_NpcBuyFixBtn ;
    CMyButton	m_NpcButUpBtn ;
    CMyButton	m_NpcButDownBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgNpcBuy)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    OBJID m_IdShopNpc;
    // Generated message map functions
    //{{AFX_MSG(CDlgNpcBuy)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnNpcbuyBtnClose();
    afx_msg void OnDoubleclickedNpcbuyGriSale();
    afx_msg void OnNpcbuyBtnFix();
    afx_msg void OnNpcbuyBtnUp();
    afx_msg void OnNpcbuyBtnDown();
    afx_msg void OnBtnHelp();
    afx_msg void OnNpcbuyBtnTalk();
    afx_msg void OnNpcbuyBtnAsk();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNPCBUY_H__1D440B00_667A_4B83_96A1_EAE50D717B19__INCLUDED_)
