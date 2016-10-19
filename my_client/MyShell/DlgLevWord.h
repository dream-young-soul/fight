
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGLEVWORD_H__A2E7DDF0_2B4A_4A7A_835C_889C3C096CB3__INCLUDED_)
#define AFX_DLGLEVWORD_H__A2E7DDF0_2B4A_4A7A_835C_889C3C096CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLevWord.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLevWord dialog
#include "MyButton.h"
#include "MyList.h"
#include "MyEdit.h"

class CDlgLevWord : public CDialog
{
    // Construction
public:
    void SetLevWordNote();
    void SetLevWordList();
    void Show();
    CDlgLevWord(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    BYTE				m_btShowMode ;
    unsigned short		m_usCurChannel ;
    int					m_nCurIndex ;
public:
    void ResetFrame( unsigned short usCurChannel );
    // Dialog Data
    //{{AFX_DATA(CDlgLevWord)
    enum { IDD = IDD_DIALOG_LEVWORD };
    CMyButton	m_BtnOk;
    CMyButton	m_BtnCancel;
    CMyEdit	m_LevWordEdtData;
    CMyList	m_LevWordLstList;
    CMyButton	m_LevWordCloseB;
    CMyButton	m_LevWordBtnWatch;
    CMyButton	m_LevWordBtnUp;
    CMyButton	m_LevWordBtnTrade;
    CMyButton	m_LevWordBtnSend;
    CMyButton	m_LevWordBtnOther;
    CMyButton	m_LevWordBtnGroup;
    CMyButton	m_LevWordBtnFriend;
    CMyButton	m_LevWordBtnFlash;
    CMyButton	m_LevWordBtnFaction;
    CMyButton	m_LevWordBtnDown;
    CMyButton	m_LevWordBtnDel;
    CMyButton	m_LevWordBtnCloseS;
    CMyButton	m_BtnHelp;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgLevWord)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgLevWord)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLevwordBtnSend();
    afx_msg void OnLevwordBtnWatch();
    afx_msg void OnLevwordBtnDel();
    afx_msg void OnLevwordBtnUp();
    afx_msg void OnLevwordBtnDown();
    afx_msg void OnBtnOk();
    afx_msg void OnBtnCancel();
    afx_msg void OnLevwordBtnFlash();
    afx_msg void OnLevwordBtnTrade();
    afx_msg void OnLevwordBtnFriend();
    afx_msg void OnLevwordBtnGroup();
    afx_msg void OnLevwordBtnFaction();
    afx_msg void OnLevwordBtnOther();
    afx_msg void OnLevwordCloseb();
    afx_msg void OnLevwordBtnCloses();
    afx_msg void OnBtnHelp();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLEVWORD_H__A2E7DDF0_2B4A_4A7A_835C_889C3C096CB3__INCLUDED_)
