
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGDEPOT_H__DFE57E0E_6953_49F6_9820_AE06E69745A3__INCLUDED_)
#define AFX_DLGDEPOT_H__DFE57E0E_6953_49F6_9820_AE06E69745A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDepot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDepot dialog
#include "MyButton.h"
#include "MyEdit.h"
#include "MyStatic.h"
#include "MyGrid.h"

const int	DEPOT_PAGE_AMOUNT = 20 ;

class CDlgDepot : public CDialog
{
    // Construction
public:
    void Show ();
    CDlgDepot(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
    int					m_nCurPage ;
private:
    CPoint				m_Pnt ;

public:
    void OnDepotClose();
    void CloseDepot ();
    void SetItem ();
    void SetMoney ();
    BOOL OpenDepot ( OBJID DepotID );
    // Dialog Data
    //{{AFX_DATA(CDlgDepot)
    enum { IDD = IDD_DIALOG_DEPOT };
    CMyStatic	m_StaFaction;
    CMyStatic	m_StaName;
    CMyButton	m_DepotCloseB;
    CMyButton	m_DepotBtnAdd ;
    CMyButton	m_DepotBtnGet ;
    CMyEdit		m_DepotEdtMoney ;
    CMyStatic	m_DepotStaLay ;
    CMyGrid		m_DepotGriLay ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgDepot)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgDepot)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnDepotBtnGet();
    afx_msg void OnDepotBtnAdd();
    afx_msg void OnDepotCloseb();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEPOT_H__DFE57E0E_6953_49F6_9820_AE06E69745A3__INCLUDED_)
