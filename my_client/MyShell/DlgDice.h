
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGDICE_H__6133E727_7386_4DD2_BB51_D5681CAD82F2__INCLUDED_)
#define AFX_DLGDICE_H__6133E727_7386_4DD2_BB51_D5681CAD82F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDice dialog
#include "MyButton.h"
#include "MyImage.h"
#include "MyStatic.h"

class CDlgDice : public CDialog
{
    // Construction
public:
    void ResetDice();
    void SetAntes ();
    void Show();
    CDlgDice(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    BYTE				m_btChipType ;
    int					m_nCurChipIn ;
    CRect				m_ChipRect[3] ;
    int					m_nChipIndex ;
    int					m_nShowChipIndex ;
    // Dialog Data
    //{{AFX_DATA(CDlgDice)
    enum { IDD = IDD_DIALOG_DICE };
    CMyButton	m_DiceBtnResume;
    CMyStatic	m_DiceStaChip;
    CMyButton	m_DiceBtnDiceList;
    CMyStatic	m_DiceStaMpc;
    CMyStatic	m_DiceStaAll;
    CMyButton	m_DiceBtnChip100;
    CMyButton	m_DiceBtnChip1000;
    CMyButton	m_DiceBtnChip10000;
    CMyImage	m_DiceImaSmall;
    CMyImage	m_DiceImaBig;
    CMyButton	m_DiceBtnHide;
    CMyButton	m_DiceBtnChipin;
    CMyImage	m_DiceImaAll1;
    CMyImage	m_DiceImaAll2;
    CMyImage	m_DiceImaAll3;
    CMyImage	m_DiceImaAll4;
    CMyImage	m_DiceImaAll5;
    CMyImage	m_DiceImaAll6;
    CMyImage	m_DiceIma4;
    CMyImage	m_DiceIma5;
    CMyImage	m_DiceIma6;
    CMyImage	m_DiceIma7;
    CMyImage	m_DiceIma8;
    CMyImage	m_DiceIma9;
    CMyImage	m_DiceIma10;
    CMyImage	m_DiceIma11;
    CMyImage	m_DiceIma12;
    CMyImage	m_DiceIma13;
    CMyImage	m_DiceIma14;
    CMyImage	m_DiceIma15;
    CMyImage	m_DiceIma16;
    CMyImage	m_DiceIma17;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgDice)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgDice)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnDiceImaAll1();
    afx_msg void OnDiceImaAll2();
    afx_msg void OnDiceImaAll3();
    afx_msg void OnDiceImaAll4();
    afx_msg void OnDiceImaAll5();
    afx_msg void OnDiceImaAll6();
    afx_msg void OnDiceIma4();
    afx_msg void OnDiceIma5();
    afx_msg void OnDiceIma6();
    afx_msg void OnDiceIma7();
    afx_msg void OnDiceIma8();
    afx_msg void OnDiceIma9();
    afx_msg void OnDiceIma10();
    afx_msg void OnDiceIma11();
    afx_msg void OnDiceIma12();
    afx_msg void OnDiceIma13();
    afx_msg void OnDiceIma14();
    afx_msg void OnDiceIma15();
    afx_msg void OnDiceIma16();
    afx_msg void OnDiceIma17();
    afx_msg void OnDiceImaSmall();
    afx_msg void OnDiceBtnChipin();
    afx_msg void OnDiceBtnHide();
    afx_msg void OnDiceImaBig();
    afx_msg void OnDiceBtnChip100();
    afx_msg void OnDiceBtnChip1000();
    afx_msg void OnDiceBtnChip10000();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnDiceBtnDicelist();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnDiceBtnResume();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDICE_H__6133E727_7386_4DD2_BB51_D5681CAD82F2__INCLUDED_)
