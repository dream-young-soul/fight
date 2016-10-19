
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGNPCFACE_H__F71206F5_1221_4BEE_ADB3_13E6B8E126A0__INCLUDED_)
#define AFX_DLGNPCFACE_H__F71206F5_1221_4BEE_ADB3_13E6B8E126A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNpcFace.h : header file
//
#include "MyButton.h"
#include "MyImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcFace dialog

class CDlgNpcFace : public CDialog
{
    // Construction
public:
    void Show();
    CDlgNpcFace(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    int					m_nMaleFaceAmount ;
    int					m_nFemaleFaceAmount ;
    int					m_nCurrentAmount ;
    int					m_uFocusImage ;

    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
public:
    void InsertNpcFace( int nImgIndex, int nFaceIndex );
    void InitNpcFace( BYTE btType );
    // Dialog Data
    //{{AFX_DATA(CDlgNpcFace)
    enum { IDD = IDD_DIALOG_NPCFACE };
    CMyButton	m_NpcFaceBtnUp;
    CMyButton	m_NpcFaceBtnDown;
    CMyImage	m_NpcFaceIma6;
    CMyImage	m_NpcFaceIma5;
    CMyImage	m_NpcFaceIma4;
    CMyImage	m_NpcFaceIma3;
    CMyImage	m_NpcFaceIma2;
    CMyImage	m_NpcFaceIma1;
    CMyButton	m_NpcFaceBtnClose;
    CMyButton	m_NpcFaceBtnAgree;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgNpcFace)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgNpcFace)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnNpcfaceIma1();
    afx_msg void OnNpcfaceIma2();
    afx_msg void OnNpcfaceIma3();
    afx_msg void OnNpcfaceIma4();
    afx_msg void OnNpcfaceIma5();
    afx_msg void OnNpcfaceIma6();
    afx_msg void OnNpcfaceBtnUp();
    afx_msg void OnNpcfaceBtnDown();
    afx_msg void OnNpcfaceBtnAgree();
    afx_msg void OnNpcfaceBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNPCFACE_H__F71206F5_1221_4BEE_ADB3_13E6B8E126A0__INCLUDED_)
