
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGLISTBOX_H__A923459B_E192_4E5E_AEB7_A4589E1E8679__INCLUDED_)
#define AFX_DLGLISTBOX_H__A923459B_E192_4E5E_AEB7_A4589E1E8679__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgListBox dialog
#include "MyList.h"

class CDlgListBox : public CDialog
{
    // Construction
public:
    void Show ();
    CDlgListBox(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
private:
    int			m_nListType;
    BOOL		m_bLoad ;
    //	CMyBitmap	m_Bmp ;
    CPoint		m_Pnt ;
public:
    void SetDlgBackColor(DWORD dwColor);
    void SetListBoxType(int nType);
    // Dialog Data
    //{{AFX_DATA(CDlgListBox)
    enum { IDD = IDD_DIALOG_LISTBOX };
    CMyList			m_ListBox ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgListBox)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    DWORD m_dwBackColor;

    // Generated message map functions
    //{{AFX_MSG(CDlgListBox)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLISTBOX_H__A923459B_E192_4E5E_AEB7_A4589E1E8679__INCLUDED_)
