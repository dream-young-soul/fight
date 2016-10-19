
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGENMSTATE_H__CED952E6_768B_4945_88F5_8C93B1831769__INCLUDED_)
#define AFX_DLGENMSTATE_H__CED952E6_768B_4945_88F5_8C93B1831769__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnmState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEnmState dialog
#include "MyImage.h"
#include "MyStatic.h"
#include "MyButton.h"

class CDlgEnmState : public CDialog
{
    // Construction
public:
    void SetStuInfo(char* szName);
    int m_iShowMode;
    void Show();
    void SetEnemyNull( char* strName );
    void SetEnemyState( char* strFriendName, int mode );

    CDlgEnmState(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    // Dialog Data
    //{{AFX_DATA(CDlgEnmState)
    enum { IDD = IDD_DIALOG_ENMSTATE };
    CMyStatic	m_StaNobility;
    CMyImage	m_EnmStateImaface;
    CMyStatic	m_Sta6;
    CMyStatic	m_Sta5;
    CMyStatic	m_Sta1;
    CMyStatic	m_Sta2;
    CMyStatic	m_Sta3;
    CMyStatic	m_Sta4;
    CMyStatic	m_Sta7;
    CMyButton	m_EnmStateBtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgEnmState)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgEnmState)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnEnmstateBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENMSTATE_H__CED952E6_768B_4945_88F5_8C93B1831769__INCLUDED_)
