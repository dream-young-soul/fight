
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTEREVOLVE_H__CF218F5E_7B1A_4163_B84A_CC9ABECD0D8F__INCLUDED_)
#define AFX_DLGMONSTEREVOLVE_H__CF218F5E_7B1A_4163_B84A_CC9ABECD0D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonsterEvolve.h : header file
//
#include "MyStatic.h"
#include "MyButton.h"
#include "myimage.h"
#include "3dsimplerole.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterEvolve dialog

class CDlgMonsterEvolve : public CDialog
{
    // Construction
public:
    BOOL m_bShow;
    void Show();
    void SetEvoMode(int nMode);
    BOOL CheckEvoMonster(OBJID idMonster);
    void EvolveEudemonOk();
    void InsertMonster(OBJID idMonster);
    CDlgMonsterEvolve(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgMonsterEvolve)
    enum { IDD = IDD_DIALOG_MONSTEREVOLVE };
    CMyStatic	m_StaMonsterName;
    CMyImage	m_ImgBeforeEvolve;
    CMyImage	m_ImgAfterEvolve;
    CMyImage	m_ImgMonster;
    CMyButton	m_BtnEvolve;
    CMyButton	m_BtnCancel;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonsterEvolve)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;
    OBJID m_IdMonster;
    int   m_nEvoMode;
    C3DSimpleRole m_3dPetEvoBefore;
    C3DSimpleRole m_3dPetEvoAfter;
    OBJID m_idTypeBefore;
    OBJID m_idTypeAfter;

    // Generated message map functions
    //{{AFX_MSG(CDlgMonsterEvolve)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnMonster();
    afx_msg void OnBtnEvolve();
    afx_msg void OnBtnCancel();
    afx_msg void OnBtnClose();
    afx_msg void OnDoubleclickedImgMonster();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTEREVOLVE_H__CF218F5E_7B1A_4163_B84A_CC9ABECD0D8F__INCLUDED_)
