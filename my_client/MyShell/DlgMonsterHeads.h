
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTERHEADS_H__39D297D5_A47E_43DE_A97D_50B271D2FFCC__INCLUDED_)
#define AFX_DLGMONSTERHEADS_H__39D297D5_A47E_43DE_A97D_50B271D2FFCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonsterHeads.h : header file
//
#include "MyStatic.h"
#include "MyButton.h"
#include "MyImage.h"
#include "MyProgress.h"
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// CDlgMonsterHeads dialog

class CDlgMonsterHeads : public CDialog
{
    // Construction
private:
    BOOL m_bHeadClick1;
    BOOL m_bHeadClick2;
    BOOL m_bHeadClick3;
    BOOL m_bHeadClick4;

    DWORD  m_Hp1;
    DWORD  m_Hp2;
    DWORD  m_Hp3;
    DWORD  m_Hp4;

    DWORD  m_Exp1;
    DWORD  m_Exp2;
    DWORD  m_Exp3;
    DWORD  m_Exp4;

    DWORD  m_Xp1;
    DWORD  m_Xp2;
    DWORD  m_Xp3;
    DWORD  m_Xp4;

    vector<OBJID>	m_setMonsterHead;
    OBJID			m_setMonsterCall[4];


    CMyPos GetEffectHeadPos(int x, int y);
public:
    BOOL KillMonster(OBJID idMonster);
    BOOL CallMonster(OBJID idMonster);
    void AddCallMonster(OBJID idMonster);
    void DelCallMonster(OBJID idMonster);
    void UpdataCallMonster();
    void FlashMonsterHeads();
    void FlashMonsterStatus();
    int  ShowProgressInfo();
    void ResetCallMonster();
    void AddEffect(char* szEffect, OBJID idPet);
    CPoint m_Pnt;
    BOOL   m_bShow;

    CGame3DEffectEx m_EffectHead1;
    CGame3DEffectEx m_EffectHead2;
    CGame3DEffectEx m_EffectHead3;
    CGame3DEffectEx m_EffectHead4;

    void ReCallMonsters();
    void ResetMonsterHeads();
    void HideCurMonsterBtns(int nIndex);
    void SetMonsterHeads(OBJID idMonster);
    void Show();
    CDlgMonsterHeads(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgMonsterHeads)
    enum { IDD = IDD_DIALOG_MONSTERHEADS };
    CMyStatic	m_StaXp1;
    CMyStatic	m_StaXp2;
    CMyStatic	m_StaXp3;
    CMyStatic	m_StaXp4;
    CMyStatic	m_StaHp1;
    CMyStatic	m_StaHp2;
    CMyStatic	m_StaHp3;
    CMyStatic	m_StaHp4;
    CMyStatic	m_StaExp1;
    CMyStatic	m_StaExp2;
    CMyStatic	m_StaExp3;
    CMyStatic	m_StaExp4;
    CMyStatic	m_StaName4;
    CMyStatic	m_StaName3;
    CMyStatic	m_StaName2;
    CMyStatic	m_StaName1;
    CMyProgress	m_ProgressXp4;
    CMyProgress	m_ProgressXp3;
    CMyProgress	m_ProgressXp2;
    CMyProgress	m_ProgressXp1;
    CMyProgress	m_ProgressHp4;
    CMyProgress	m_ProgressHp3;
    CMyProgress	m_ProgressHp2;
    CMyProgress	m_ProgressHp1;
    CMyProgress	m_ProgressExp4;
    CMyProgress	m_ProgressExp3;
    CMyProgress	m_ProgressExp2;
    CMyProgress	m_ProgressExp1;
    CMyStatic	m_ImgHeadBack4;
    CMyStatic	m_ImgHeadBack3;
    CMyStatic	m_ImgHeadBack2;
    CMyStatic	m_ImgHeadBack1;
    CMyImage	m_ImgHead4;
    CMyImage	m_ImgHead3;
    CMyImage	m_ImgHead2;
    CMyImage	m_ImgHead1;
    CMyButton	m_BtnCall4;
    CMyButton	m_BtnCall3;
    CMyButton	m_BtnCall2;
    CMyButton	m_BtnCall1;
    CMyButton	m_BtnAttach4;
    CMyButton	m_BtnAttach3;
    CMyButton	m_BtnAttach2;
    CMyButton	m_BtnAttach1;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonsterHeads)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgMonsterHeads)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnBtnAttach1();
    afx_msg void OnBtnAttach2();
    afx_msg void OnBtnAttach3();
    afx_msg void OnBtnAttach4();
    afx_msg void OnBtnCall1();
    afx_msg void OnBtnCall2();
    afx_msg void OnBtnCall3();
    afx_msg void OnBtnCall4();
    afx_msg void OnImgHead1();
    afx_msg void OnImgHead2();
    afx_msg void OnImgHead3();
    afx_msg void OnImgHead4();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTERHEADS_H__39D297D5_A47E_43DE_A97D_50B271D2FFCC__INCLUDED_)
