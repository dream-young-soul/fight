
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MENUGROUP_H__07ED8FDF_E009_4EA8_9A33_BDAA228D7F2A__INCLUDED_)
#define AFX_MENUGROUP_H__07ED8FDF_E009_4EA8_9A33_BDAA228D7F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuGroup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuGroup dialog
#include "MyCheck.h"
#include "MyButton.h"
const int MENU_GROUP_CLICK = 5000 ;
const int GEAR_CLICK	=	MENU_GROUP_CLICK + 1 ;
const int MONEY_CLICK	=	MENU_GROUP_CLICK + 2 ;
const int ITEM_CLICK	=	MENU_GROUP_CLICK + 3 ;
const int JOIN_CLICK	=	MENU_GROUP_CLICK + 4 ;

class CMenuGroup : public CDialog
{
    // Construction
public:
    void SetParentWnd ( CWnd* pParentWnd);
    void Show();
    CMenuGroup(CWnd* pParent = NULL);   // standard constructor

    BOOL				m_bShow ;
    BOOL				m_bCreate ;
private:
    CPoint				m_Pnt ;
    CWnd*				m_ParentWnd ;
public:
    void ResetChecks();
    // Dialog Data
    //{{AFX_DATA(CMenuGroup)
    enum { IDD = IDD_MENU_GROUP };
    CMyCheck	m_GroupChkJoin;
    CMyCheck	m_GroupChkStop;
    CMyCheck	m_GroupChkMoney;
    CMyCheck	m_GroupChkItem;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMenuGroup)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMenuGroup)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnGroupChkStop();
    afx_msg void OnGroupChkMoney();
    afx_msg void OnGroupChkItem();
    afx_msg void OnGroupChkJion();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUGROUP_H__07ED8FDF_E009_4EA8_9A33_BDAA228D7F2A__INCLUDED_)
