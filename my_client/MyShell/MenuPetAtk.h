
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MENUPETATK_H__F4D3E476_F34B_4513_8DDD_219C88E526A6__INCLUDED_)
#define AFX_MENUPETATK_H__F4D3E476_F34B_4513_8DDD_219C88E526A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuPetAtk.h : header file
//
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CMenuPetAtk dialog

class CMenuPetAtk : public CDialog
{
    // Construction
public:
    CMenuPetAtk(CWnd* pParent = NULL);   // standard constructor

    void   Show();
    void   ShowPetAtk(int nIndex);
    void   SetPetAtkStatus(OBJID idPetItem);
    CPoint m_Pnt;
    BOOL   m_bShow;
    OBJID  m_idPetItem;
    int	   m_nPetIndex;
    // Dialog Data
    //{{AFX_DATA(CMenuPetAtk)
    enum { IDD = IDD_MENU_PETATK };
    CMyButton	m_BtnAtkStatus2;
    CMyButton	m_BtnAtkStatus1;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMenuPetAtk)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMenuPetAtk)
    afx_msg void OnBtnAtkstatus1();
    afx_msg void OnBtnAtkstatus2();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUPETATK_H__F4D3E476_F34B_4513_8DDD_219C88E526A6__INCLUDED_)
