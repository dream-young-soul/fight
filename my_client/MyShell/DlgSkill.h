
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSKILL_H__6F4509F5_AAA5_48C1_B376_16D57C5AC541__INCLUDED_)
#define AFX_DLGSKILL_H__6F4509F5_AAA5_48C1_B376_16D57C5AC541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSkill.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSkill dialog
#include "MyButton.h"
#include "MyImage.h"
#include "MyCheck.h"
#include "MyStatic.h"
#include "DlgSkill_HotKey.h"
// Added by ClassView

class CDlgSkill : public CDialog
{
    // Construction
public:
    int GetSkillDescLine(int nStartX, int nStartY, int nWidth, const char* szString);
    void ShowSkillDesc(int x, int y, int nWidth , const char* szString, DWORD dwColor = 0xffffff);
    void ShowSkillInfo(int x, int y);
    void SetTeamMagicHotKey(int nIndex, int HotKey);
    void SetTeamMagicHotKeys(int nIndex);
    void SetMagicHotKey(int nIndex, int HotKey);
    int GetMouseInMagicIndex();
    void UseTeamMagic(int nIndex);
    void SetTeamMagicSkill();
    CDlgSkill_HotKey m_DlgSkillHotKey;
    void SetMagicHotKeys(int nIndex);
    void ClearMagicSkill();
    void SetMagicSkill();
    void Show (int x, int y);
    CDlgSkill(CWnd* pParent = NULL);   // standard constructor

    BOOL				m_bShow ;
    int					m_nSkillListMode;
    int					m_nAppTime ;		// the dialog show or hide times
private:
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    CRect				m_PrgRect[6];
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;

    int					m_nCurIndex ;
    int					m_nTMagicIndex;
    // Dialog Data
    //{{AFX_DATA(CDlgSkill)
    enum { IDD = IDD_DIALOG_SKILL };
    CMyImage	m_SkillKey1Img;
    CMyImage	m_SkillKey2Img;
    CMyImage	m_SkillKey3Img;
    CMyImage	m_SkillKey4Img;
    CMyImage	m_SkillKey5Img;
    CMyImage	m_SkillKey6Img;
    CMyStatic	m_StaName6;
    CMyStatic	m_StaName5;
    CMyStatic	m_StaName4;
    CMyStatic	m_StaName3;
    CMyStatic	m_StaName2;
    CMyStatic	m_StaName1;
    CMyStatic	m_StaExp6;
    CMyStatic	m_StaExp5;
    CMyStatic	m_StaExp4;
    CMyStatic	m_StaExp3;
    CMyStatic	m_StaExp2;
    CMyStatic	m_StaExp1;
    CMyStatic	m_StaDesc6;
    CMyStatic	m_StaDesc5;
    CMyStatic	m_StaDesc4;
    CMyStatic	m_StaDesc3;
    CMyStatic	m_StaDesc2;
    CMyStatic	m_StaDesc1;
    CMyImage	m_Img6;
    CMyImage	m_Img1;
    CMyImage	m_Img2;
    CMyImage	m_Img3;
    CMyImage	m_Img4;
    CMyImage	m_Img5;
    CMyButton	m_BtnUp;
    CMyButton	m_BtnDown;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSkill)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgSkill)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnSkillBtnDown();
    afx_msg void OnSkillImg1();
    afx_msg void OnSkillImg2();
    afx_msg void OnSkillImg3();
    afx_msg void OnSkillImg4();
    afx_msg void OnSkillImg5();
    afx_msg void OnSkillImg6();
    afx_msg void OnSkillBtnUp();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnButton1();
    afx_msg void OnButton2();
    afx_msg void OnButton3();
    afx_msg void OnButton4();
    afx_msg void OnButton5();
    afx_msg void OnButton6();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSKILL_H__6F4509F5_AAA5_48C1_B376_16D57C5AC541__INCLUDED_)
