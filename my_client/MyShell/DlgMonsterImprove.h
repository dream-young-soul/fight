
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTERIMPROVE_H__0A156A3E_2170_4F26_9832_8E8E9F1B5E3C__INCLUDED_)
#define AFX_DLGMONSTERIMPROVE_H__0A156A3E_2170_4F26_9832_8E8E9F1B5E3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonsterImprove.h : header file
//
#include "MyButton.h"
#include "MyImage.h"
#include "MyStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterImprove dialog

class CDlgMonsterImprove : public CDialog
{
    // Construction
public:
    void SetPickUpMonsterId(OBJID idMonster);
    OBJID GetSoulStoneID();
    void SetPickUpSoulStone(OBJID idSoulStone);
    void InsertMonster(OBJID idMonster);
    void SetDlgMode(int nMode);
    BOOL m_bShow;
    void Show();
    CDlgMonsterImprove(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgMonsterImprove)
    enum { IDD = IDD_DIALOG_MONSTERIMPROVE };
    CMyStatic	m_StaNpcInfo;
    CMyImage	m_ImgSoulStone;
    CMyImage	m_ImgNpcFace;
    CMyImage	m_ImgMonster;
    CMyButton	m_BtnClose;
    CMyButton	m_BtnAliveImp;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonsterImprove)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_nDlgMode;
    CPoint m_Pnt;
    OBJID m_IdMonster;
    OBJID m_IdSoulStone;

    // Generated message map functions
    //{{AFX_MSG(CDlgMonsterImprove)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnAliveimp();
    afx_msg void OnBtnClose();
    afx_msg void OnImgMonster();
    afx_msg void OnImgSoulstone();
    afx_msg void OnDoubleclickedImgMonster();
    afx_msg void OnDoubleclickedImgSoulstone();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    int m_nIconRealID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTERIMPROVE_H__0A156A3E_2170_4F26_9832_8E8E9F1B5E3C__INCLUDED_)
