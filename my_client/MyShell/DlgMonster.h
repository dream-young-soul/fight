
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMONSTER_H__227D9936_2845_4E23_9D6F_A43CE7A80C0A__INCLUDED_)
#define AFX_DLGMONSTER_H__227D9936_2845_4E23_9D6F_A43CE7A80C0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonster.h : header file
//
#include "MyCheck.h"
#include "mystatic.h"
#include "myimage.h"
#include "Myedit.h"
#include "mygrid.h"
#include "MyButton.h"
#include <vector>
#include "DlgMonsterSkill.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMonster dialog

class CDlgMonster : public CDialog
{
    // Construction
public:
    friend class CMyShellDlg;
    void MoveButtons(BOOL bCall);
    void ResetMonstStatus();
    OBJID GetCurMonsterID();
    void DropMonster();
    BOOL CheckMonster();
    void ResetMonsterDlg();
    void SetMonsterList();
    int  m_iCurMonster;
    void SetEggGrid();
    void SetCurMonster(int mode);
    int m_nPntX;
    void Show();
    void ShowHead(OBJID idMonster);

    CDlgMonster(CWnd* pParent = NULL);   // standard constructor
    BOOL   m_bShow;
    CDlgMonsterSkill m_DlgMonsterSkill;
    // Dialog Data
    //{{AFX_DATA(CDlgMonster)
    enum { IDD = IDD_DIALOG_MONSTER };
    CMyButton	m_BtnRenameOk;
    CMyButton	m_BtnRenameCancel;
    CMyEdit	m_EditName;
    CMyButton	m_ChkCallOut;
    CMyEdit	m_EditMonster4;
    CMyEdit	m_EditMonster3;
    CMyEdit	m_EditMonster2;
    CMyEdit	m_EditMonster1;
    CMyButton	m_BtnRename;
    CMyButton	m_MagicBtn;
    CMyStatic	m_StaPotential;
    CMyStatic	m_StaGrow;
    CMyCheck	m_CallChk;
    CMyStatic	m_StaSmart;
    CMyStatic	m_StaMaster;
    CMyStatic	m_StaMagicDef;
    CMyStatic	m_StaMagic;
    CMyStatic	m_StaLevel;
    CMyStatic	m_StaHp;
    CMyStatic	m_StaFightDef;
    CMyStatic	m_StaFight;
    CMyStatic	m_StaExp;
    CMyImage	m_ImgMonster;
    CMyGrid		m_EggsGri;
    CMyButton	m_CloseBtn;
    CMyButton   m_DropBtn;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonster)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    OBJID m_CurMonsterID;
    vector<OBJID> m_IdCurCallMonster;

    // Generated message map functions
    //{{AFX_MSG(CDlgMonster)
    afx_msg void OnBtnCall();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnDrop();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBtnMagic();
    afx_msg void OnBtnRename();
    afx_msg void OnBtnCallout();
    afx_msg void OnBtnRenamecancel();
    afx_msg void OnBtnRenameok();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
    CRect				m_PrgMonstName[4];

    CPoint m_Pnt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONSTER_H__227D9936_2845_4E23_9D6F_A43CE7A80C0A__INCLUDED_)





















