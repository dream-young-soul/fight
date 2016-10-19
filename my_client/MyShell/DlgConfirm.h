
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGCONFIRM_H__0189EE5F_CBB9_4DEF_AAAE_E426A36ECF9D__INCLUDED_)
#define AFX_DLGCONFIRM_H__0189EE5F_CBB9_4DEF_AAAE_E426A36ECF9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfirm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog
#include "MyStatic.h"
#include "MyButton.h"
#include "MyEdit.h"

const int TRADE_CONFIRM = 0 ;
const int TEAMJOIN_CONFIRM = 1 ;
const int TEAMINVITE_CONFIRM = 2 ;
const int FACTIONJOIN_CONFIRM = 3 ;
const int FACTIONINVITE_CONFIRM = 4 ;
const int MONEYDROP_CONFIRM = 5 ;
const int FRIENDDEL_CONFIRM = 6 ;
const int TALKDEL_CONFIRM = 7 ;
const int FACTIONOUT_CONFIRM = 8 ;
const int FACTIONSUB_CONFIRM = 9 ;
const int MPCBOOTHADD_CONFIRM = 10 ;
const int MPCBOOTHCHG_CONFIRM = 11 ;
const int FACTIONDEL_CONFIRM = 12 ;
const int BOOTHOPEN_CONFIRM = 13 ;
const int MPCCOURT_CONFIRM = 14 ;
const int IMPTITEM_CONFIRM = 15 ;
const int DROPITEM_CONFIRM = 16 ;
const int TACKOFF_CONFIRM = 17 ;
const int JOINDICE_CONFIRM = 18 ;
const int OUTDICE_CONFIRM = 19 ;
const int ENEMYDEL_CONFIRM = 20 ;
const int RESOURCE_CONFIRM = 21 ;
const int EXIT_CONFIRM = 22 ;
const int EMPTYMSG_CONFIRM = 23 ;
const int WARNING_CONFIRM = 24 ;
const int ROLECREATE_CONFIRM = 25 ;
const int ACTION_CONFIRM = 26 ;
const int FRIENDADD_CONFIRM = 27 ;
const int FRIENDADDSUC_CONFIRM = 28 ;
const int SPLITITEM_CONFIRM = 29 ;
const int TEACHER_CONFIRM = 30;
const int STUDENT_CONFIRM = 31;
const int MONSTERDROP_CONFIRM = 32;
const int TEAMDISMISS_CONFIRM = 33;
const int CHECKOUT_MONSTEREGG = 34;
const int FACTIONADDALLY_CONFIRM = 35;
const int FACTIONADDENEMY_CONFIRM = 36;

const int ACTION_MSG_WIDTH = 50 ;

class CDlgConfirm : public CDialog
{
    // Construction
public:
    BOOL SetConfirmDlg ( UINT uType, const char* strName = NULL, OBJID TargetID = 0 );
    void Show();
    CDlgConfirm(CWnd* pParent = NULL);   // standard constructor

    void	SetButtons(int nMode);
    void	ShowConfirmTextMLine();
    char*	GetConfirmText()	{return m_strText;}
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    char				m_strText[256] ;
    int					m_nLeftLeft ;
    UINT				m_uType ;
    int					m_nEdtNum ;
    int					m_nSkip ;
    OBJID				m_TargetID ;
    // for action confirm
    int					m_nLine ;
    BOOL				m_bLine[10] ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgConfirm)
    enum { IDD = IDD_DIALOG_CONFIRM };
    CMyEdit				m_ConfirmEdtData;
    CMyEdit				m_ConfirmEdtName;
    CMyButton m_ConfirmBtnOk ;
    CMyButton m_ConfirmBtnCancel ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgConfirm)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgConfirm)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnConfirmBtnOk();
    afx_msg void OnConfirmBtnCancel();
    afx_msg void OnChangeConfirmEdtData();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIRM_H__0189EE5F_CBB9_4DEF_AAAE_E426A36ECF9D__INCLUDED_)
