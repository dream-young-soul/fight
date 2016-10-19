
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMSGRECEIVE_H__33C5E3E1_31AD_49ED_AC24_CD19B9DABE80__INCLUDED_)
#define AFX_DLGMSGRECEIVE_H__33C5E3E1_31AD_49ED_AC24_CD19B9DABE80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMsgReceive.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgReceive dialog
#include "MyButton.h"
#include "MyStatic.h"
#include "MyEdit.h"

class CDlgMsgReceive : public CDialog
{
    // Construction
public:
    void SetMsgReceiveName(const char* strName);
    void SetReceiveMsg();
    void Show();
    CDlgMsgReceive(CWnd* pParent = NULL);   // standard constructor
    BOOL	m_bShow ;
private:
    CPoint				m_Pnt ;
    int					m_nCurIndex ;
    // Dialog Data
    //{{AFX_DATA(CDlgMsgReceive)
    enum { IDD = IDD_DIALOG_MSGRECEIVE };
    CMyStatic	m_StaMsgReceive;
    CMyStatic	m_MsgReceiveTime;
    CMyButton	m_BtnUp;
    CMyButton	m_BtnDown;
    CMyStatic	m_MsgReceiveName;
    CMyButton	m_MsgReceiveClose;
    CMyButton	m_MsgReceiveRevert ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMsgReceive)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgMsgReceive)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnMsgreceiveCloseb();
    afx_msg void OnBtnHelp();
    afx_msg void OnMsgreceiveRevert();
    afx_msg void OnBtnUp();
    afx_msg void OnBtnDown();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSGRECEIVE_H__33C5E3E1_31AD_49ED_AC24_CD19B9DABE80__INCLUDED_)
