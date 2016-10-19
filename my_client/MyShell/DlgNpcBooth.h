
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGNPCBOOTH_H__363C2C8D_01F3_4E85_AE19_AA22476C951D__INCLUDED_)
#define AFX_DLGNPCBOOTH_H__363C2C8D_01F3_4E85_AE19_AA22476C951D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNpcBooth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcBooth dialog
#include "MyButton.h"
#include "MyImage.h"
#include "MyStatic.h"

class CDlgNpcBooth : public CDialog
{
    // Construction
public:
    int GetMouseMoveIndex();
    void ClearNpcBooth();
    void SetNpcBooth ();
    void Show();
    CDlgNpcBooth(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    int					m_nCurBase ;

    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
    // Dialog Data
    //{{AFX_DATA(CDlgNpcBooth)
    enum { IDD = IDD_DIALOG_NPCBOOTH };
    CMyStatic	m_StaPrice1;
    CMyStatic	m_StaPrice2;
    CMyStatic	m_StaPrice3;
    CMyStatic	m_StaPrice4;
    CMyStatic	m_StaPrice5;
    CMyStatic	m_StaPrice6;
    CMyImage	m_NpcBoothImaGood1;
    CMyImage	m_NpcBoothImaGood2;
    CMyImage	m_NpcBoothImaGood3;
    CMyImage	m_NpcBoothImaGood4;
    CMyImage	m_NpcBoothImaGood5;
    CMyImage	m_NpcBoothImaGood6;
    CMyButton	m_NpcBoothBtnClose;
    CMyButton	m_NpcBoothBtnUp;
    CMyButton	m_NpcBoothBtnDown;
    CMyButton	m_BtnBuy1;
    CMyButton	m_BtnBuy2;
    CMyButton	m_BtnBuy3;
    CMyButton	m_BtnBuy4;
    CMyButton	m_BtnBuy5;
    CMyButton	m_BtnBuy6;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgNpcBooth)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgNpcBooth)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnNpcboothBtnClose();
    afx_msg void OnNpcboothBtnUp();
    afx_msg void OnNpcboothBtnDown();
    afx_msg void OnNpcboothBtnBuy1();
    afx_msg void OnNpcboothBtnBuy2();
    afx_msg void OnNpcboothBtnBuy3();
    afx_msg void OnNpcboothBtnBuy4();
    afx_msg void OnNpcboothBtnBuy5();
    afx_msg void OnNpcboothBtnBuy6();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNPCBOOTH_H__363C2C8D_01F3_4E85_AE19_AA22476C951D__INCLUDED_)
