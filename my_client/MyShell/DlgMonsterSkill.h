
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTERSKILL_H__8F0C2141_C623_4C66_BFEF_77D73B8DA9B4__INCLUDED_)
#define AFX_DLGMONSTERSKILL_H__8F0C2141_C623_4C66_BFEF_77D73B8DA9B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonsterSkill.h : header file
//
#include "MyStatic.h"
#include "MyButton.h"
#include "MyImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterSkill dialog

class CDlgMonsterSkill : public CDialog
{
    // Construction
private:
    OBJID m_IdMonster;
public:
    int GetSkillDescLine(int nStartX, int nStartY, int nWidth, const char* szString);
    void ShowSkillInfo(int x, int y);
    int GetMouseInMagicIndex();
    void ShowSkillDesc(int nStartX, int nStartY, int nWidth, const char* szString , DWORD dwColor);
    void Show();
    CDlgMonsterSkill(CWnd* pParent = NULL);   // standard constructor
    void ResetMonsterSkill();
    void SetMonsterID(int nIndex);
    void SetMonsterSKill();
    // Dialog Data
    BOOL m_bShow;
    CPoint m_Pnt;
    int	 m_nCurSkillIndex;
    //{{AFX_DATA(CDlgMonsterSkill)
    enum { IDD = IDD_DIALOG_MONSTERSKILL };
    CMyButton	m_BtnClose;
    CMyStatic	m_StaName4;
    CMyStatic	m_StaName3;
    CMyStatic	m_StaName2;
    CMyStatic	m_StaName1;
    CMyStatic	m_StaName;
    CMyStatic	m_StaDescM4;
    CMyStatic	m_StaDescM3;
    CMyStatic	m_StaDescM2;
    CMyStatic	m_StaDescM1;
    CMyImage	m_ImgSkill4;
    CMyImage	m_ImgSkill3;
    CMyImage	m_ImgSkill2;
    CMyImage	m_ImgSkill1;
    CMyButton	m_BtnPageUp;
    CMyButton	m_BtnPageDown;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonsterSkill)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgMonsterSkill)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnPagedown();
    afx_msg void OnBtnPageup();
    afx_msg void OnBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTERSKILL_H__8F0C2141_C623_4C66_BFEF_77D73B8DA9B4__INCLUDED_)
