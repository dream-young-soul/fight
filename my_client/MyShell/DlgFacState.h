
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGFACSTATE_H__C43FC724_0665_4433_8B60_19691EB69718__INCLUDED_)
#define AFX_DLGFACSTATE_H__C43FC724_0665_4433_8B60_19691EB69718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFacState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFacState dialog
#include "MyButton.h"
#include "MyImage.h"
#include "MyStatic.h"

class CDlgFacState : public CDialog
{
    // Construction
public:
    void SetFacState( const char* strMemName );
    void Show();
    CDlgFacState(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow	 ;
private:
    CPoint				m_Pnt ;
    // Dialog Data
    //{{AFX_DATA(CDlgFacState)
    enum { IDD = IDD_DIALOG_FACSTATE };
    CMyImage	m_ImgFace;
    CMyStatic	m_StaSynJob;
    CMyStatic	m_StaProsition;
    CMyStatic	m_StaPk;
    CMyStatic	m_StaNobility;
    CMyStatic	m_StaName;
    CMyStatic	m_StaMate;
    CMyStatic	m_StaLevel;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgFacState)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgFacState)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnFacstateBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFACSTATE_H__C43FC724_0665_4433_8B60_19691EB69718__INCLUDED_)
