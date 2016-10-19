
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSETUP_MUSIC_H__127BD62A_AEFF_4C3F_BCC6_93B34DB77022__INCLUDED_)
#define AFX_DLGSETUP_MUSIC_H__127BD62A_AEFF_4C3F_BCC6_93B34DB77022__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup_Music.h : header file
//
#include "mybutton.h"
#include "mycheck.h"
#include "mystatic.h"
#include "MySlider.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Music dialog

class CDlgSetup_Music : public CDialog
{
    // Construction
public:
    void SetSliderStatic();
    BOOL m_bShow;
    void Show(int x, int y);
    CDlgSetup_Music(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgSetup_Music)
    enum { IDD = IDD_DIALOG_SETUP_MUSIC };
    CMySlider	m_SliderSound;
    CMySlider	m_SliderMusic;
    CMyStatic	m_StaSound;
    CMyStatic	m_StaMusic;
    CMyCheck	m_SoundChk;
    CMyCheck	m_MusicChk;
    CMyButton	m_SoundRightBtn;
    CMyButton	m_SoundLeftBtn;
    CMyButton	m_MusicRightBtn;
    CMyButton	m_MusicLeftBtn;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetup_Music)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;
    // Generated message map functions
    //{{AFX_MSG(CDlgSetup_Music)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnChkMusic();
    afx_msg void OnChkSound();
    afx_msg void OnBtnMusicleft();
    afx_msg void OnBtnMusicright();
    afx_msg void OnBtnSoundleft();
    afx_msg void OnBtnSoundright();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_MUSIC_H__127BD62A_AEFF_4C3F_BCC6_93B34DB77022__INCLUDED_)
