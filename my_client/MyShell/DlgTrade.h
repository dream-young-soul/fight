
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTRADE_H__FDF39A2D_36C3_48F1_85CD_854601D04581__INCLUDED_)
#define AFX_DLGTRADE_H__FDF39A2D_36C3_48F1_85CD_854601D04581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTrade.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTrade dialog
#include "MyGrid.h"
#include "MyStatic.h"
#include "MyButton.h"
#include "MyEdit.h"
#include "MyImage.h"
#include "MyCheck.h"

class CDlgTrade : public CDialog
{
    // Construction
public:
    CDlgTrade(CWnd* pParent = NULL);   // standard constructor

    BOOL		m_bShow ;
private:
    CPoint		m_Pnt ;
    BOOL		m_bMoneyAdd ;
public:
    void ResetTrade();
    void SetNpcMoney ();
    void SetNpcGridContain();
    void SetMpcGridContain();
    void Show();
    // Dialog Data
    //{{AFX_DATA(CDlgTrade)
    enum { IDD = IDD_DIALOG_TRADE };
    CMyCheck	m_NpcAgreeChk;
    CMyStatic	m_TradeStaMpcName;
    CMyImage	m_MpcAgreeChk;
    CMyCheck	m_MoneyNpcOkChk;
    CMyImage	m_MoneyMpcOkChk;
    CMyButton	m_TradeCloseB;
    CMyGrid	m_TradeGriNpc ;
    CMyGrid m_TradeGriMpc ;
    CMyStatic m_TradeStaNpcName ;
    CMyStatic m_TradeStaNpcMoney ;
    CMyEdit	  m_TradeEdtMpcMoney ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTrade)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgTrade)
    virtual BOOL OnInitDialog();
    afx_msg void OnTradeBtnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnTradeBtnMpcagree();
    afx_msg void OnBtnHelp();
    afx_msg void OnTradeCloseb();
    afx_msg void OnTradeBtnMoneynpcok();
    afx_msg void OnTradeBtnMoneympcok();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRADE_H__FDF39A2D_36C3_48F1_85CD_854601D04581__INCLUDED_)
