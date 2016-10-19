
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTASK_H__033DA5A9_1DB6_4C5C_89BA_A92D0B03D1E8__INCLUDED_)
#define AFX_DLGTASK_H__033DA5A9_1DB6_4C5C_89BA_A92D0B03D1E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTask.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTask dialog
#include "MyEdit.h"
#include "MyButton.h"

const int	HEADPIC_WIDTH = 64 ;
const int	HEADPIC_HEIGHT = 64 ;
const int	TEXT_LIMIT = 48 ;
const int	LINK_LIMIT = 36 ;

class CDlgTask : public CDialog
{
    // Construction
public:
    void Show();
    void SetLineIndex( BYTE btType );
    void ResetTaskDialog();
    void InitTaskDialog();
    CDlgTask(CWnd* pParent = NULL);   // standard constructor

    BOOL				m_bShow ;
private:
    struct TaskText
    {
        char				strText[512] ;					// the ask text buffers
        int					nLineIndex[8] ;					// the ask text line index
        int					nLineNum ;						// the ask text line num
    } m_TaskText[8] ;
    int					m_nTextAmount ;					// the ask text amount

    struct TaskLink
    {
        char	strText[256] ;								// the link text buffers
        CRect   Rect ;										// the link text rect
        int		nLineIndex[8] ;								// the ask text line index
        int		nLineNum ;									// the ask text line num
        int		nIndex ;									// the answer index
    } m_TaskLink[8] ;									// the ask text buffers
    int					m_nLinkAmount ;					// the link amount

    struct TaskEdit
    {
        char	strTitle[32] ;
        DWORD	dwData ;
        int		nIndex ;
    } m_TaskEdit	;									// the task edit buffers
    int					m_nEditAmount ;					// the edit amount
    int					m_nEditIndex ;					// the edit index

    int					m_nPicIndex ;					// the task pic buffers
    CMyPos				m_PicPos;
    CMySize				m_PicSize ;

    CPoint				m_Pnt ;
    int					m_nCloseIndex ;					// the close answer index
    int					m_nDlgHeight ;					// the dialog show height
    BOOL				m_bMouseMove ;					// the mouse move on the link
    BYTE				m_btCurLink ;					// the mouse move current link
    int					m_nAnswerIndex ;
    int					m_nAnswerAmount ;
    CRect				m_DlgRect ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgTask)
    enum { IDD = IDD_DIALOG_TASK };
    CMyEdit		m_TaskEdtAnswer ;
    CMyButton	m_TaskBtnAnswer ;
    CMyButton	m_CloseBtn ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTask)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgTask)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTaskBtnAnswer();
    afx_msg void OnBtnClose();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTASK_H__033DA5A9_1DB6_4C5C_89BA_A92D0B03D1E8__INCLUDED_)
