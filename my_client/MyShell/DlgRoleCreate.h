
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGROLECREATE_H__D27F8954_E6C4_407D_9E3D_A4ADF0879630__INCLUDED_)
#define AFX_DLGROLECREATE_H__D27F8954_E6C4_407D_9E3D_A4ADF0879630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRoleCreate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRoleCreate dialog
#include "MyImage.h"
#include "MyStatic.h"
#include "MyButton.h"
#include "MyEdit.h"
#include "MyCheck.h"
#include "3DSimpleRole.h"

class CDlgRoleCreate : public CDialog
{
    // Construction
public:
    void SetAcnAndPsw( const char* strAccount, const char* strPassword );
    void Show();
    void ShowRoleInfo();
    void SetRoleFace();
    CDlgRoleCreate(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    BYTE				m_btSex ;
    BYTE				m_btPro ;
    int					m_nLookIndex;
    char				m_cLength;
    char				m_cFat;
    char				m_strRoleType[64] ;
    char				m_strRoleName[16] ;
    char				m_strAccount[16] ;
    char				m_strPassword[16] ;
    C3DSimpleRole		m_ShowRoleA ;
    C3DSimpleRole		m_ShowRoleB ;
    C3DSimpleRole		m_ShowRoleC ;
public:
    float m_RoleZoom;
    void EnableCreate(BOOL bFlag);
    void CreateReset();
    void CreateExit();
    void CreateRole();
    void ReInit();
    BOOL IsSelRoleOk() {return m_bSelRoleOk;}
    // Dialog Data
    //{{AFX_DATA(CDlgRoleCreate)
    enum { IDD = IDD_DIALOG_ROLECREATE };
    CMyButton	m_BtnReset;
    CMyButton	m_BtnChgFace;
    CMyButton	m_BtnChgBody;
    CMyStatic	m_StaSex;
    CMyImage	m_ImgFace;
    CMyImage	m_ImgDataBk;
    CMyButton	m_BtnRight3;
    CMyButton	m_BtnRight2;
    CMyButton	m_BtnRight1;
    CMyButton	m_BtnLeft3;
    CMyButton	m_BtnLeft2;
    CMyButton	m_BtnLeft1;
    CMyImage	m_Img3;
    CMyImage	m_ImgFaceSelBk;
    CMyImage	m_ImgRoleSel;
    CMyImage			m_Img2;
    CMyImage			m_Img1;
    CMyButton			m_RoleCreateExitBtn;
    CMyStatic			m_RoleCreateDataSta ;
    CMyEdit				m_RoleCreateNameEdt ;
    CMyButton			m_RoleCreateOkBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgRoleCreate)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_nSexRole;
    BOOL m_bSelected;
    DWORD m_RoleShowTime;
    CMyPos m_RolePos;
    void ProcessRoleShow(BOOL bBack);
    BOOL m_bSelRoleOk;
    int m_nRoleSelIndex;

    // Generated message map functions
    //{{AFX_MSG(CDlgRoleCreate)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnRolecreateBtnOk();
    afx_msg void OnRolecreateBtnExit();
    afx_msg void OnRolechooseMimg1();
    afx_msg void OnRolechooseMimg2();
    afx_msg void OnRolecreateBtnReset();
    afx_msg void OnRolechooseMimg3();
    afx_msg void OnBtnLeft3();
    afx_msg void OnBtnRight3();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBtnChgbody();
    afx_msg void OnBtnChgface();
    afx_msg void OnBtnLeft1();
    afx_msg void OnBtnRight1();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROLECREATE_H__D27F8954_E6C4_407D_9E3D_A4ADF0879630__INCLUDED_)
