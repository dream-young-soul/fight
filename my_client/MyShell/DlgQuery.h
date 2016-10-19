
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGQUERY_H__F2781CE1_7667_4C8A_A2A8_55B1A147C7B5__INCLUDED_)
#define AFX_DLGQUERY_H__F2781CE1_7667_4C8A_A2A8_55B1A147C7B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery dialog
#include "MyCheck.h"
#include "MyButton.h"
#include "DlgEquip.h"
#include "DlgQuery_Role.h"
#include "DlgSkill.h"


class CDlgQuery : public CDialog
{
    // Construction
public:
    void PopupCurChk ( );
    void ShowChildDlg ( );
    void HideCurDlg ( BYTE btCurDlgID );
    void ShowCurDlg ( BYTE btCurDlgID );
    void Show();
    CDlgQuery(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
public:
    void SetAddPoint();
    BOOL				m_bMouseMove ;
    BOOL				m_bLoad ;
    int 				m_nPntX ;
    CPoint				m_Pnt ;
    int					m_Height ;
    int					m_Width ;
    BYTE				m_btCurChildDlgID ;
    CDlgEquip			m_DlgEquip;
    CDlgQuery_Role		m_DlgRole;
    CDlgSkill			m_DlgSkill;
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
    // Dialog Data
    //{{AFX_DATA(CDlgQuery)
    enum { IDD = IDD_DIALOG_QUERY };
    CMyCheck	m_ChkSkill;
    CMyCheck	m_ChkRole;
    CMyCheck	m_ChkEquip;
    CMyCheck	m_ChkTSkill;
    CMyStatic	m_StaName;
    CMyStatic	m_StaFaction;
    CMyButton	m_QueryCloseBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgQuery)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:

    // Generated message map functions
    //{{AFX_MSG(CDlgQuery)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnQueryClose();
    afx_msg void OnChkEquip();
    afx_msg void OnChkRole();
    afx_msg void OnChkSkill();
    afx_msg void OnChkTskill();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQUERY_H__F2781CE1_7667_4C8A_A2A8_55B1A147C7B5__INCLUDED_)
