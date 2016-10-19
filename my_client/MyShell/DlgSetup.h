
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSETUP_H__EF87810C_42E8_4B00_981A_0646A6D9AFDB__INCLUDED_)
#define AFX_DLGSETUP_H__EF87810C_42E8_4B00_981A_0646A6D9AFDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup dialog
#include "MyButton.h"
#include "MyStatic.h"
#include "MySlider.h"
#include "MyCheck.h"
#include "DlgSetup_Chat.h"
#include "DlgSetup_Graph.h"
#include "DlgSetup_Hotkey.h"
#include "DlgSetup_Music.h"

const int	MAX_INTERFACE_NUM = 2 ;

class CDlgSetup : public CDialog
{
    // Construction
public:
    void Exit();
    void Show ();
    CDlgSetup(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
    int					m_nInterface ;
private:
    CPoint				m_Pnt ;
    int					m_nScreenMode ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgSetup)
    enum { IDD = IDD_DIALOG_SETUP };
    CMyButton	m_SetupBtnRelogin;
    CMyCheck	m_SetupBtnChat;
    CMyCheck	m_SetupBtnMusic;
    CMyButton	m_SetupBtnReturn;
    CMyCheck	m_SetupBtnGraph;
    CMyCheck	m_SetupBtnHotkey;
    CMyButton		m_SetupBtnExit ;
    CMyButton		m_SetupBtnDelete ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetup)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:
    CDlgSetup_Graph m_DlgGraph;
    CDlgSetup_Music m_DlgMusic;
    CDlgSetup_Chat  m_DlgSChat;
    CDlgSetup_Hotkey m_DlgHotkey;
    void OnResourceLose();
    void SetScreenMode(int mode);
    void ProcessCheck ( int nCurMode );

    // Generated message map functions
    //{{AFX_MSG(CDlgSetup)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnSetupBtnExit();
    afx_msg void OnSetupBtnDelete();
    afx_msg void OnSetupBtnHotkey();
    afx_msg void OnSetupBtnOther();
    afx_msg void OnSetupBtnMusic();
    afx_msg void OnSetupBtnReturn();
    afx_msg void OnSetupBtnGraph();
    afx_msg void OnSetupBtnRelogin();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_H__EF87810C_42E8_4B00_981A_0646A6D9AFDB__INCLUDED_)
