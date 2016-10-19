
//**********************************************************\\
// ´úÂë±à¼­Æ÷\\
//**********************************************************\\
\\
#if !defined(AFX_DLGROOMLG_H__BBB177FD_E6FB_49BF_8F35_E07AE832FF60__INCLUDED_)
#define AFX_DLGROOMLG_H__BBB177FD_E6FB_49BF_8F35_E07AE832FF60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgroomlg.h : header file
//

//Create By:	Wen Yiku
//Create Date:	2005-6-30

/////////////////////////////////////////////////////////////////////////////
// CDlgRoomLg dialog

#include "MyButton.h"
#include "MyList.h"

//#include "ChatRoom.h"
class CDlgRoomLg : public CDialog
{
    // Construction
public:
    CDlgRoomLg(CWnd* pParent = NULL);   // standard constructor
    void	SetParentWnd( CWnd* pParentWnd );
    void SetListInfo();

    void	Show ();
    LONG	m_bShow ;

    // Dialog Data
    //{{AFX_DATA(CDlgRoomLg)
    enum { IDD = IDD_DIALOG_ROOMLG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
private:
    CPoint		m_Pnt ;
    CWnd*		m_ParentWnd ;
public:
    CMyButton	m_BtnReturn;
    CMyList		m_ListInfo;


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgRoomLg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgRoomLg)
    afx_msg void OnMove(int x, int y);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnRoomlgBtnReturn();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
//extern CDlgRoomLg*	g_pRoomLp;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROOMLG_H__BBB177FD_E6FB_49BF_8F35_E07AE832FF60__INCLUDED_)
