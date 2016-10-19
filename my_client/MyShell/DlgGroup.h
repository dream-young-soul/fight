
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGGROUP_H__11D93147_8947_47E5_B993_3A2FE54D533E__INCLUDED_)
#define AFX_DLGGROUP_H__11D93147_8947_47E5_B993_3A2FE54D533E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGroup.h : header file
//
const	int	TEAM_MODE_NONE		=	0 ;
const	int	TEAM_MODE_LEADER	=	1 ;
const	int	TEAM_MODE_MEMBER	=	2 ;
/////////////////////////////////////////////////////////////////////////////
// CDlgGroup dialog
#include "MyImage.h"
#include "MyButton.h"
#include "MenuGroup.h"

class CDlgGroup : public CDialog
{
    // Construction
public:
    void SetGroupContain();
    OBJID GetGroupImgID(int nIndex) {return m_PlayerID[nIndex];}
    void ShowTeamMemberPosition(int nIndex, CPoint posPosition);
    void Show ();
    CDlgGroup(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
    UINT		m_uTeamMode ;
    BYTE		m_btOwnerPos ;
    CMenuGroup  m_MenuGroup ;
    CRect		m_ImgRect[4] ;
private:
    CPoint		m_Pnt ;
    OBJID		m_PlayerID[5] ;
    UINT		m_uFocusPlayer ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgGroup)
    enum { IDD = IDD_DIALOG_GROUP };
    CMyButton	m_GroupBtn5;
    CMyButton	m_GroupBtn3;
    CMyImage		m_GroupImg1 ;
    CMyImage		m_GroupImg2 ;
    CMyImage		m_GroupImg3 ;
    CMyImage		m_GroupImg4 ;
    CMyButton		m_GroupBtn1 ;
    CMyButton		m_GroupBtn2 ;
    CMyButton		m_GroupBtn4 ;
    CMyButton		m_GroupUnlayBtn ;
    CMyButton		m_GroupCloseBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgGroup)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void SetFocusPlayer(int nIndex);
    OBJID GetMouseInMember();
    void TeamDismiss();
    void CreateTeamOk();
    void ShowMemberName();

    // Generated message map functions
    //{{AFX_MSG(CDlgGroup)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnGroupBtnClose();
    afx_msg void OnGroupBtn1();
    afx_msg void OnGroupBtn2();
    afx_msg void OnGroupChk3();
    afx_msg void OnGroupBtn4();
    afx_msg void OnGroupImg1();
    afx_msg void OnGroupImg2();
    afx_msg void OnGroupImg3();
    afx_msg void OnGroupImg4();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnGroupBtn5();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
protected:
    DWORD m_dwFocusTime;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGROUP_H__11D93147_8947_47E5_B993_3A2FE54D533E__INCLUDED_)
