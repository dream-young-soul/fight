
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSETUP_CHAT_H__268FACCF_8378_4FB6_8E3C_8B5D60EA7581__INCLUDED_)
#define AFX_DLGSETUP_CHAT_H__268FACCF_8378_4FB6_8E3C_8B5D60EA7581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup_Chat.h : header file
//
#include "MyImage.h"
#include "MyCheck.h"
#include "MyButton.h"
#include "MyEdit.h"
#include "MyStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Chat dialog

class CDlgSetup_Chat : public CDialog
{
    // Construction
public:
    void PopupCurNameChk();
    int m_nCurBlackName;
    void SetBlackNameStatic();
    void SetChatMode();
    void ChangeColorImg(int nColorIndex);
    unsigned short m_usCurChannel;
    DWORD m_dwColor[10];
    int m_nColorLog[5];
    BOOL m_bColorImg;
    void Show( int x, int y);
    BOOL m_bShow;
    CDlgSetup_Chat(CWnd* pParent = NULL);   // standard constructor
    int m_nChatSetMode;

    // Dialog Data
    //{{AFX_DATA(CDlgSetup_Chat)
    enum { IDD = IDD_DIALOG_SETUP_CHAT };
    CMyCheck	m_ChkName6;
    CMyCheck	m_ChkName5;
    CMyCheck	m_ChkName4;
    CMyCheck	m_ChkName3;
    CMyCheck	m_ChkName2;
    CMyCheck	m_ChkName1;
    CMyEdit	m_EditBlackName;
    CMyButton	m_BtnDelBlackName;
    CMyButton	m_BtnChatLog;
    CMyCheck	m_BtnBlackName;
    CMyButton	m_BtnAddBlackName;
    CMyCheck	m_ChkTeam;
    CMyCheck	m_ChkSec;
    CMyCheck	m_ChkPub;
    CMyCheck	m_ChkFriend;
    CMyCheck	m_ChkBan;
    CMyCheck	m_BtnChannelSet;
    CMyCheck	m_BtnColorSet;
    CMyImage	m_ImgColor1;
    CMyImage	m_ImgColor2;
    CMyImage	m_ImgColor3;
    CMyImage	m_ImgColor4;
    CMyImage	m_ImgColor5;
    CMyImage	m_ImgColor6;
    CMyImage	m_ImgColor7;
    CMyImage	m_ImgColor8;
    CMyImage	m_ImgColor9;
    CMyImage	m_ImgColor10;
    CMyImage	m_ImgFriend;
    CMyImage	m_ImgPub;
    CMyImage	m_ImgSec;
    CMyImage	m_ImgTeam;
    CMyImage	m_ImgBan;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetup_Chat)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void EnableColorImg(BOOL bFlag);
    CPoint m_Pnt;
    // Generated message map functions
    //{{AFX_MSG(CDlgSetup_Chat)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnBan();
    afx_msg void OnBtnColor1();
    afx_msg void OnBtnColor10();
    afx_msg void OnBtnColor2();
    afx_msg void OnBtnColor3();
    afx_msg void OnBtnColor4();
    afx_msg void OnBtnColor5();
    afx_msg void OnBtnColor6();
    afx_msg void OnBtnColor7();
    afx_msg void OnBtnColor8();
    afx_msg void OnBtnColor9();
    afx_msg void OnBtnFriend();
    afx_msg void OnBtnPub();
    afx_msg void OnBtnSec();
    afx_msg void OnBtnTeam();
    afx_msg void OnBtnChannelset();
    afx_msg void OnBtnColorset();
    afx_msg void OnChkBan();
    afx_msg void OnChkFriend();
    afx_msg void OnChkPub();
    afx_msg void OnChkSec();
    afx_msg void OnChkTeam();
    afx_msg void OnBtnAddblackname();
    afx_msg void OnBtnBlackname();
    afx_msg void OnBtnChatlog();
    afx_msg void OnBtnDelblackname();
    afx_msg void OnChkName1();
    afx_msg void OnChkName2();
    afx_msg void OnChkName3();
    afx_msg void OnChkName4();
    afx_msg void OnChkName5();
    afx_msg void OnChkName6();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_CHAT_H__268FACCF_8378_4FB6_8E3C_8B5D60EA7581__INCLUDED_)
