
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSKILL_HOTKEY_H__C6FA1088_C5C2_415B_95F6_BADBBE63AF50__INCLUDED_)
#define AFX_DLGSKILL_HOTKEY_H__C6FA1088_C5C2_415B_95F6_BADBBE63AF50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSkill_HotKey.h : header file
//
#include "MyCheck.h"
#include "MyButton.h"
#include "MyStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSkill_HotKey dialog

class CDlgSkill_HotKey : public CDialog
{
    // Construction
public:
    void SetMouseHotKey(int MagicType, int nHotKey);
    void ReSetHotKeys();
    void PopCurCheck(int nIndex, BOOL bFlag = false);
    void SetStaMagic(int iMagic);
    int m_iMagicHotKey;
    void SetParentWnd(CWnd* pParentWnd);
    CWnd* m_ParentWnd;
    void Show();
    CPoint m_Pnt;
    int	 m_HotKeyIdType[8];
    CDlgSkill_HotKey(CWnd* pParent = NULL);   // standard constructor
    BOOL m_bShow;

    // Dialog Data
    //{{AFX_DATA(CDlgSkill_HotKey)
    enum { IDD = IDD_DIALOG_SKILL_HOTKEYS };
    CMyStatic	m_StaMagicDec;
    CMyCheck	m_HotKeyF5Chk;
    CMyStatic	m_StaMagicName;
    CMyCheck	m_NoneChk;
    CMyCheck	m_HotKeyF8Chk;
    CMyCheck	m_HotKeyF7Chk;
    CMyCheck	m_HotKeyF6Chk;
    CMyCheck	m_HotKeyF4Chk;
    CMyCheck	m_HotKeyF3Chk;
    CMyCheck	m_HotKeyF2Chk;
    CMyCheck	m_HotKeyF1Chk;
    CMyButton	m_HotKeyOkBtn;
    CMyButton	m_CloseBtn;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSkill_HotKey)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgSkill_HotKey)
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnOk();
    afx_msg void OnChkHotkeyf1();
    afx_msg void OnChkHotkeyf2();
    afx_msg void OnChkHotkeyf3();
    afx_msg void OnChkHotkeyf4();
    afx_msg void OnChkHotkeyf5();
    afx_msg void OnChkHotkeyf6();
    afx_msg void OnChkHotkeyf7();
    afx_msg void OnChkHotkeyf8();
    afx_msg void OnChkNone();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    int m_iMagicIdType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSKILL_HOTKEY_H__C6FA1088_C5C2_415B_95F6_BADBBE63AF50__INCLUDED_)
