
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGFRIEND_H__22F52277_0D73_4DFE_8755_92780539974B__INCLUDED_)
#define AFX_DLGFRIEND_H__22F52277_0D73_4DFE_8755_92780539974B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFriend.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFriend dialog
#include "MyButton.h"
#include "MyList.h"
#include "DlgFrdState.h"
#include "Hero.h"

const int MAX_FRIEND_NUM = 32 ;

class CDlgFriend : public CDialog
{
    // Construction
public:
    CDlgFriend(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    int					m_nCurIndex ;
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;

    BYTE				m_btSortType ;
    int					m_nOnlineAmount ;
public:
    void InsertFriendInfo( int nIndex, CFriend* pFriend );
    void ClearFriendList();

    void SetFriendState();
    void SetFriendList ();
    void Show ();
    // Dialog Data
    //{{AFX_DATA(CDlgFriend)
    enum { IDD = IDD_DIALOG_FRIEND };
    CMyImage	m_ImgHead1;
    CMyStatic	m_StaStirps1;
    CMyStatic	m_StaSex1;
    CMyStatic	m_StaPro1;
    CMyStatic	m_StaName1;
    CMyStatic	m_StaLev1;
    CMyButton	m_BtnMsg1;
    CMyButton	m_BtnDel1;
    CMyImage	m_ImgHead2;
    CMyStatic	m_StaStirps2;
    CMyStatic	m_StaSex2;
    CMyStatic	m_StaPro2;
    CMyStatic	m_StaName2;
    CMyStatic	m_StaLev2;
    CMyButton	m_BtnMsg2;
    CMyButton	m_BtnDel2;
    CMyImage	m_ImgHead3;
    CMyStatic	m_StaStirps3;
    CMyStatic	m_StaSex3;
    CMyStatic	m_StaPro3;
    CMyStatic	m_StaName3;
    CMyStatic	m_StaLev3;
    CMyButton	m_BtnMsg3;
    CMyButton	m_BtnDel3;
    CMyButton	m_BtnSortOnline;
    CMyButton	m_BtnSortName;
    CMyButton	m_BtnSort;
    CMyButton	m_BtnRight;
    CMyButton	m_BtnLeft;
    CMyButton	m_FriendBtnResume;
    CMyButton	m_FriendBtnAdd ;
    CMyButton	m_FriendBtnClose ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgFriend)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgFriend)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnFriendBtnClose();
    afx_msg void OnBtnHelp();
    afx_msg void OnFriendBtnAdd();
    afx_msg void OnFriendBtnResume();
    afx_msg void OnBtnMsg1();
    afx_msg void OnBtnMsg2();
    afx_msg void OnBtnMsg3();
    afx_msg void OnBtnSort();
    afx_msg void OnBtnRight();
    afx_msg void OnBtnLeft();
    afx_msg void OnBtnDel1();
    afx_msg void OnBtnDel2();
    afx_msg void OnBtnDel3();
    afx_msg void OnBtnSortonline();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFRIEND_H__22F52277_0D73_4DFE_8755_92780539974B__INCLUDED_)
