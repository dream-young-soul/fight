
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSTONEGOODS_H__3920D30F_67A3_4A7E_8038_C74063251464__INCLUDED_)
#define AFX_DLGSTONEGOODS_H__3920D30F_67A3_4A7E_8038_C74063251464__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStoneGoods.h : header file
//
#include "MyButton.h"
#include "MyGrid.h"
#include "MyCheck.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgStoneGoods dialog

class CDlgStoneGoods : public CDialog
{
    // Construction
public:
    void SetCurGridMode();
    void PopUpCurChk();
    int m_uCurStoneKind;
    void FlashGhostGem();
    void Show();
    CDlgStoneGoods(CWnd* pParent = NULL);   // standard constructor
    BOOL m_bShow;

    // Dialog Data
    //{{AFX_DATA(CDlgStoneGoods)
    enum { IDD = IDD_DIALOG_STONEGOODS };
    CMyCheck	m_ChkStone3;
    CMyCheck	m_ChkStone2;
    CMyCheck	m_ChkStone1;
    CMyGrid	m_StoneGri0;
    CMyGrid	m_StoneGri1;
    CMyButton	m_CloseBtn;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgStoneGoods)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;
    int    m_nPntX;
    DWORD	m_dwLastShowTime ;
    float	m_fFrameStep ;
    int		m_nFrameMode ;

    // Generated message map functions
    //{{AFX_MSG(CDlgStoneGoods)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnStone1();
    afx_msg void OnBtnStone2();
    afx_msg void OnBtnStone3();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTONEGOODS_H__3920D30F_67A3_4A7E_8038_C74063251464__INCLUDED_)
