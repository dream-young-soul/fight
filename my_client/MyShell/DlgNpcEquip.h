
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGNPCEQUIP_H__46045E5D_C390_4D48_9FB2_AC710C9C9CFC__INCLUDED_)
#define AFX_DLGNPCEQUIP_H__46045E5D_C390_4D48_9FB2_AC710C9C9CFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNpcEquip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNpcEquip dialog
#include "MyButton.h"
#include "MyImage.h"
#include "MyStatic.h"

class CDlgNpcEquip : public CDialog
{
    // Construction
public:
    void Show();
    CDlgNpcEquip(CWnd* pParent = NULL);   // standard constructor
    BOOL	m_bShow ;
private:
    CPoint		m_Pnt ;
    int 		m_nPntX ;
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;

    int					m_nMouseMovePosition ;

public:
    int GetMouseMovePosition();
    void SetNpcEquip();
    // Dialog Data
    //{{AFX_DATA(CDlgNpcEquip)
    enum { IDD = IDD_DIALOG_NPCEQUIP };
    CMyStatic	m_StaName;
    CMyStatic	m_StaFaction;
    CMyButton	m_BtnClose;
    CMyImage	m_ImgMount;
    CMyImage	m_ImgWeapon;
    CMyImage	m_ImgMantle;
    CMyImage	m_ImgShoe;
    CMyImage	m_ImgNeck;
    CMyImage	m_ImgHead;
    CMyImage	m_ImgGear;
    CMyImage	m_ImgBody;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgNpcEquip)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgNpcEquip)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnNpcequipClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNPCEQUIP_H__46045E5D_C390_4D48_9FB2_AC710C9C9CFC__INCLUDED_)
