
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_MENUTALK_H__3F17A627_804B_44B0_AEDB_E3CDADB762A1__INCLUDED_)
#define AFX_MENUTALK_H__3F17A627_804B_44B0_AEDB_E3CDADB762A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuTalk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuTalk dialog
#include "MyButton.h"
const int MENU_TALK_CLICK = 5000 ;
const int G_CLICK	=	MENU_TALK_CLICK + 1 ;
const int S_CLICK	=	MENU_TALK_CLICK + 2 ;
const int D_CLICK	=	MENU_TALK_CLICK + 3 ;
const int B_CLICK	=	MENU_TALK_CLICK + 4 ;
const int H_CLICK	=	MENU_TALK_CLICK + 5 ;
const int W_CLICK	=	MENU_TALK_CLICK + 6 ;	//Wen YiKu Code 2005-6-29

class CMenuTalk : public CDialog
{
    // Construction
public:
    void SetParentWnd( CWnd* pParentWnd );
    void Show();
    CMenuTalk(CWnd* pParent = NULL);   // standard constructor

    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    CWnd*				m_ParentWnd ;
    // Dialog Data
    //{{AFX_DATA(CMenuTalk)
    enum { IDD = IDD_MENU_TALK };
    CMyButton	m_TalkBtnG ;
    CMyButton	m_TalkBtnS ;
    CMyButton	m_TalkBtnD ;
    CMyButton	m_TalkBtnB ;
    CMyButton	m_TalkBtnH ;
    CMyButton	m_TalkBtnW ;	//Wen YiKu Code 2005-6-29
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMenuTalk)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMenuTalk)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnTalkBtnG();
    afx_msg void OnTalkBtnS();
    afx_msg void OnTalkBtnD();
    afx_msg void OnTalkBtnB();
    afx_msg void OnTalkBtnH();
    afx_msg void OnTalkBtnW();	//Wen Yiku Code 2005-6-29
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUTALK_H__3F17A627_804B_44B0_AEDB_E3CDADB762A1__INCLUDED_)
