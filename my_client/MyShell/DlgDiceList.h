
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGDICELIST_H__FEFEE47F_5172_426C_B142_AA5E7A0D793D__INCLUDED_)
#define AFX_DLGDICELIST_H__FEFEE47F_5172_426C_B142_AA5E7A0D793D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDiceList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDiceList dialog
#include "MyList.h"

class CDlgDiceList : public CDialog
{
    // Construction
public:
    void SetDiceList();
    void Show();
    CDlgDiceList(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    // Dialog Data
    //{{AFX_DATA(CDlgDiceList)
    enum { IDD = IDD_DIALOG_DICELIST };
    CMyList	m_DiceListList;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgDiceList)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgDiceList)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDICELIST_H__FEFEE47F_5172_426C_B142_AA5E7A0D793D__INCLUDED_)
