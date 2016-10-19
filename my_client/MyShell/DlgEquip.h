
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGEQUIP_H__681FB56F_D125_4A46_B581_6BF27C04BDAF__INCLUDED_)
#define AFX_DLGEQUIP_H__681FB56F_D125_4A46_B581_6BF27C04BDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEquip.h : header file
//

#include "MyImage.h"
#include "MyButton.h"
#include "MyStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEquip dialog

class CDlgEquip : public CDialog
{
    // Construction
public:
    void SetRoleInfo();
    int GetMouseMovePosition();
    int GetPickUpPosition();
    CPoint GetPickUpMousePnt();
    void SetPickUpItemID( int nID );
    void FlashEquip();
    void HeroEquipItem( int nPos );
    void Show(int x, int y);
    CDlgEquip(CWnd* pParent = NULL);   // standard constructor
    BOOL	m_bShow ;
    int     m_iShowMode;
private:
    CPoint		m_Pnt ;
    int 		m_nPntX ;
    int			m_nPickUpItemID ;
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;

    // for pickup item
    CPoint				m_PickUpMousePnt ;
    int					m_nPickUpItemPosition ;

    // for mouse move item
    int					m_nMouseMovePosition ;
    // Dialog Data
    //{{AFX_DATA(CDlgEquip)
    enum { IDD = IDD_DIALOG_EQUIP };
    CMyImage	m_ImgMount;
    CMyImage	m_ImgWeapon;
    CMyImage	m_ImgMantle;
    CMyStatic	m_StaRoleXp;
    CMyStatic	m_StaRoleMp;
    CMyStatic	m_StaRoleMagicDef;
    CMyStatic	m_StaRoleMagic;
    CMyStatic	m_StaRoleHp;
    CMyStatic	m_StaRoleFightDef;
    CMyStatic	m_StaRoleFight;
    CMyStatic	m_StaRoleAvoid;
    CMyImage	m_ImgShoe;
    CMyImage	m_ImgNeck;
    CMyImage	m_ImgHead;
    CMyImage	m_ImgGear;
    CMyImage	m_ImgBody;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgEquip)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgEquip)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQUIP_H__681FB56F_D125_4A46_B581_6BF27C04BDAF__INCLUDED_)
