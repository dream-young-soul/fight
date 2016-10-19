
//**********************************************************
// 代码编辑器
//**********************************************************

#if !defined(AFX_DLGXP_H__F6B281AF_5C64_48B6_AB7F_DFAC3A3CEC07__INCLUDED_)
#define AFX_DLGXP_H__F6B281AF_5C64_48B6_AB7F_DFAC3A3CEC07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgXp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgXp dialog

#include "MyImage.h"
#include "MyButton.h"

class CDlgXp : public CDialog
{
    // Construction
public:
    void ChooseXp(OBJID XpId);
    void CheckXpSkill();
    void SetTeamMagicImg(OBJID idMagic);
    void SetFly( BOOL bFly );
    void SetTransform ( BOOL bTrans );
    void SetBooth ( BOOL bOpen );
    void SetReborn(BOOL bDie = true);
    void InitXpDialog();
    void Show();
    CDlgXp(CWnd* pParent = NULL);   // standard constructor

    BOOL		m_bShow ;
    BYTE		m_btBoothType ;
    BOOL		m_bDie ;
    BOOL		m_bTransform ;
    BOOL		m_bFly ;
    BOOL		m_bSkillEnable[5];
private:
    bool	m_bInJustNow;		// 鼠标是否刚刚进入
    DWORD	m_dwInTime;			// 鼠标进入的时间
    CRect		m_PrgRect[6];
    CPoint		m_Pnt ;
    DWORD		m_XpIdType[5] ;
    int			m_nCurBase ;
    // Dialog Data
    //{{AFX_DATA(CDlgXp)
    enum { IDD = IDD_DIALOG_XP };
    CMyImage	m_XpImg5;
    CMyImage	m_XpImg4;
    CMyImage	m_XpImg3;
    CMyImage	m_XpImg2;
    CMyImage	m_XpImg1;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgXp)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_bMouseMove;

    // Generated message map functions
    //{{AFX_MSG(CDlgXp)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnXpImg1();
    afx_msg void OnXpImg2();
    afx_msg void OnXpImg3();
    afx_msg void OnXpImg4();
    afx_msg void OnXpImg5();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGXP_H__F6B281AF_5C64_48B6_AB7F_DFAC3A3CEC07__INCLUDED_)
