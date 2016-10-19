
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGEMBED_H__806211B4_80E3_406D_9455_3CD1085724CC__INCLUDED_)
#define AFX_DLGEMBED_H__806211B4_80E3_406D_9455_3CD1085724CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEmbed.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEmbed dialog
#include "MyButton.h"
#include "MyImage.h"

class CDlgEmbed : public CDialog
{
    // Construction
public:
    void OnCloseB();
    void InsertMonster(OBJID idMonster);
    void SetPickUpState (  int nRealIcon );
    void Show();
    CDlgEmbed(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int					m_nRealIcon ;
    OBJID				m_GoodID ;
    OBJID				m_Gem1ID ;
    OBJID				m_MonsterID ;

    // Dialog Data
    //{{AFX_DATA(CDlgEmbed)
    enum { IDD = IDD_DIALOG_EMBED };
    CMyImage	m_EmbedImaMonster;
    CMyButton	m_EmbedBtnCancel;
    CMyButton	m_EmbedBtnClose;
    CMyImage	m_EmbedImaGood;
    CMyImage	m_EmbedImaGem1;
    CMyButton	m_EmbedBtnEmbed;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgEmbed)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgEmbed)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnEmbedImaGood();
    afx_msg void OnEmbedClose();
    afx_msg void OnEmbedImaGem1();
    afx_msg void OnDoubleclickedEmbedImaGood();
    afx_msg void OnDoubleclickedEmbedImaGem1();
    afx_msg void OnEmbedBtnEmbed();
    afx_msg void OnDoubleclickedEmbedImaMonster();
    afx_msg void OnEmbedBtnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEMBED_H__806211B4_80E3_406D_9455_3CD1085724CC__INCLUDED_)
