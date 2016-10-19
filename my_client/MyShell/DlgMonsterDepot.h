
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTERDEPOT_H__CF218F5E_7B1A_4163_B84A_CC9ABECD0D8F__INCLUDED_)
#define AFX_DLGMONSTERDEPOT_H__CF218F5E_7B1A_4163_B84A_CC9ABECD0D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonsterDepot.h : header file
//
#include "MyStatic.h"
#include "MyGrid.h"
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterDepot dialog

class CDlgMonsterDepot : public CDialog
{
    // Construction
public:
    void SetMonster();
    BOOL m_bShow;
    void ResetDepot();
    void Show();
    CDlgMonsterDepot(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgMonsterDepot)
    enum { IDD = IDD_DIALOG_MONSTERDEPOT };
    CMyStatic	m_StaMagicAttact1;
    CMyStatic	m_StaName1;
    CMyStatic	m_StaLevel1;
    CMyStatic	m_StaAttact1;
    CMyStatic	m_StaMagicAttact2;
    CMyStatic	m_StaName2;
    CMyStatic	m_StaLevel2;
    CMyStatic	m_StaAttact2;
    CMyStatic	m_StaMagicAttact3;
    CMyStatic	m_StaName3;
    CMyStatic	m_StaLevel3;
    CMyStatic	m_StaAttact3;
    CMyStatic	m_StaMagicAttact4;
    CMyStatic	m_StaName4;
    CMyStatic	m_StaLevel4;
    CMyStatic	m_StaAttact4;
    CMyStatic	m_StaMagicAttact5;
    CMyStatic	m_StaName5;
    CMyStatic	m_StaLevel5;
    CMyStatic	m_StaAttact5;
    CMyGrid		m_GridMonster;
    CMyButton	m_BtnPageUp;
    CMyButton	m_BtnPageDown;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonsterDepot)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgMonsterDepot)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnPagedown();
    afx_msg void OnBtnPageup();
    afx_msg void OnMonster1();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTERDEPOT_H__CF218F5E_7B1A_4163_B84A_CC9ABECD0D8F__INCLUDED_)
