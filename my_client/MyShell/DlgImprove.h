
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGIMPROVE_H__4370C54E_32A0_46F7_88CC_0E4A60256C90__INCLUDED_)
#define AFX_DLGIMPROVE_H__4370C54E_32A0_46F7_88CC_0E4A60256C90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImprove.h : header file
//
#include "MyButton.h"
#include "MyImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImprove dialog

class CDlgImprove : public CDialog
{
    // Construction
public:
    CDlgImprove(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    int					m_nIconRealID ;
    OBJID				m_GoodID ;
    OBJID				m_GemID ;

public:
    void SetImproveType(int nType) { m_nImproveType = nType;}
    void OnCloseB();
    void ResetImproveDlg();
    DWORD GetMouseMoveId();
    void SetPickUpState(int nIconRealID );
    void Show();
    // Dialog Data
    //{{AFX_DATA(CDlgImprove)
    enum { IDD = IDD_DIALOG_IMPROVE };
    CMyButton	m_ImproveBtnCloseB;
    CMyButton	m_ImproveBtnClose;
    CMyImage	m_ImproveImaGood;
    CMyImage	m_ImproveImaGem;
    CMyButton	m_ImproveBtnImp;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgImprove)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_nImproveType;

    // Generated message map functions
    //{{AFX_MSG(CDlgImprove)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnImproveImaGem();
    afx_msg void OnImproveImaGood();
    afx_msg void OnImproveBtnImp();
    afx_msg void OnImproveBtnClose();
    afx_msg void OnDoubleclickedImproveImaGem();
    afx_msg void OnDoubleclickedImproveImaGood();
    afx_msg void OnImproveBtnCloseb();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPROVE_H__4370C54E_32A0_46F7_88CC_0E4A60256C90__INCLUDED_)
