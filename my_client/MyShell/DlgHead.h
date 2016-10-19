
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGHEAD_H__C3B20335_4721_4C43_A307_BF077BD05714__INCLUDED_)
#define AFX_DLGHEAD_H__C3B20335_4721_4C43_A307_BF077BD05714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHead.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHead dialog
#include "MyImage.h"
#include "MyButton.h"
#include "MyStatic.h"

class CDlgHead : public CDialog
{
    // Construction
public:
    void Show();
    CDlgHead(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int					m_nFace ;
    int					m_nLook ;
    // Dialog Data
    //{{AFX_DATA(CDlgHead)
    enum { IDD = IDD_DIALOG_HEAD };
    CMyStatic	m_StaTitle;
    CMyStatic	m_StaDuke;
    CMyImage	m_ImgHeadS;
    CMyImage	m_ImgHeadB;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgHead)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgHead)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnImgHeadb();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHEAD_H__C3B20335_4721_4C43_A307_BF077BD05714__INCLUDED_)
