
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGPROGRESS_H__7BA0DA9E_A767_4C93_A658_0538136563C4__INCLUDED_)
#define AFX_DLGPROGRESS_H__7BA0DA9E_A767_4C93_A658_0538136563C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
#include "MyImage.h"
#include "MyProgress.h"
#include "MyStatic.h"
#include "MyButton.h"

#include "game3deffectex.h"

class CDlgProgress : public CDialog
{
    // Construction
public:
    void Show();
    CDlgProgress(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
    BOOL				m_bLevUp ;
    int					m_nHeroXp;
    CPoint				m_Pnt ;
private:
    BYTE				m_btCurMovePrg ;
    CGame3DEffectEx		m_EffectExXp ;
public:
    void ShowMsgFlash();
    CRect m_PrgRect[4];
    void SetProgressValue();
    // Dialog Data
    //{{AFX_DATA(CDlgProgress)
    enum { IDD = IDD_DIALOG_PROGRESS };
    CMyButton	m_BadStatusImg;
    CMyProgress	m_PrgHeroXpBase;
    CMyImage	m_ImgMsgFlash;
    CMyProgress	m_PrgHeroXp;
    CMyProgress	m_PrgHeroForce2;
    CMyProgress	m_PrgHeroMp;
    CMyProgress	m_PrgHeroHp;
    CMyProgress	m_PrgHeroForce;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgProgress)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    DWORD m_dwFlashTime;
    int m_iMouseMove;

    // Generated message map functions
    //{{AFX_MSG(CDlgProgress)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnImgMsgflash();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__7BA0DA9E_A767_4C93_A658_0538136563C4__INCLUDED_)
