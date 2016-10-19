
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGFRDSTATE_H__3C1A823C_9F04_4FF2_86F1_CAB3E058B08F__INCLUDED_)
#define AFX_DLGFRDSTATE_H__3C1A823C_9F04_4FF2_86F1_CAB3E058B08F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFrdState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFrdState dialog
#include "MyImage.h"
#include "MyStatic.h"
#include "MyButton.h"

class CDlgFrdState : public CDialog
{
    // Construction
public:
    CDlgFrdState(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
public:
    void SetFriendNull( char* strName );
    void DeleteFriend();
    void SetFriendState( char* strFriendName );
    void Show();
    // Dialog Data
    //{{AFX_DATA(CDlgFrdState)
    enum { IDD = IDD_DIALOG_FRDSTATE };
    CMyImage	m_FrdStateImaface;
    CMyStatic	m_Sta6;
    CMyStatic	m_Sta5;
    CMyStatic	m_Sta1;
    CMyStatic	m_Sta2;
    CMyStatic	m_Sta3;
    CMyStatic	m_Sta4;
    CMyStatic	m_Sta7;
    CMyButton	m_FrdStateBtnDel;
    CMyButton	m_FrdStateBtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgFrdState)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgFrdState)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnFrdstateBtnClose();
    afx_msg void OnFrdstateBtnDel();
    afx_msg void OnBtnHelp();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFRDSTATE_H__3C1A823C_9F04_4FF2_86F1_CAB3E058B08F__INCLUDED_)
