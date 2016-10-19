
//**********************************************************
// 代码编辑器
//**********************************************************

#if !defined(AFX_DLGTALK_H__7B85476C_7BD6_463E_A558_EEE57C74B29A__INCLUDED_)
#define AFX_DLGTALK_H__7B85476C_7BD6_463E_A558_EEE57C74B29A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTalk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTalk dialog
#include "MyCombo.h"
#include "MyStatic.h"
#include "MyEdit.h"
#include "MyCheck.h"
#include "MyImage.h"
#include "MyButton.h"
#include "MyProgress.h"
#include "MenuTalk.h"
#include "MyGrid.h"
#include "DlgTalkTarget.h"
#include "DlgTxtAction.h"
//#include "dlgroomlg.h"


#include "Player.h"

const int _MAX_WORDS_PS = 2 ;
class CDlgTalk : public CDialog
{
    // Construction
public:
    void SetTalkTarget( CPlayer* pTalkTarget );
    void SetTalkMode( UINT uTalkMode);
    void Show();
    CDlgTalk(CWnd* pParent = NULL);   // standard constructor

    BOOL				m_bShow ;
    UINT				m_uTalkMode ;
    char				m_strTargetName[10][16] ;
    UINT				m_uTargetTalkMode[10] ;
    int					m_nTargetAmount ;
    int					m_nCurTargetIndex ;
    CMenuTalk			m_MenuTalk ;
    //	CDlgRoomLg			m_RoomLg;	//Wen Yiku Code 2005-6-30
    CDlgTalkTarget		m_DlgListBox;
    CDlgTxtAction		m_DlgListAction;
    bool				m_bEnter;		// 焦点是否进入
    bool				m_bHotEnter;	// 是按了 Enter 快捷键
private:
    BOOL				m_bShowExp;
    bool				m_bGameMsgEx;
    CBitmap				m_BkBmp ;
    CPoint				m_Pnt ;
    CListBox			m_ListBox ;
    DWORD				m_dwLastTime ;
    int					m_nListAmount ;
    int					m_nListIndex ;
    OBJID				m_TalkPlayerID ;
    UINT				m_uAddBtnMode ;
    UINT				m_uDelIndex ;
    CRect				m_PrgRect[7] ;
    BOOL				m_bMouseMove ;
public:
    void DeleteTalkTarget();
    // Dialog Data
    //{{AFX_DATA(CDlgTalk)
    enum { IDD = IDD_DIALOG_TALK };
    CMyStatic	m_StaTxtAction;
    CMyButton	m_TargetListBtn;
    CMyButton	m_EmotionListBtn;
    CMyButton	m_ViewEquipBtn;
    CMyButton	m_TalkActBtn;
    CMyButton	m_TalkMyChatBtn;
    CMyButton	m_TalkClearBtn;
    CMyGrid	m_GoodBox;
    CMyButton	m_HideGoodBox;
    CMyProgress	m_PrgExp;
    CMyCombo	m_TalkDataCob ;
    CMyEdit		m_TalkNameEdt ;
    CMyButton	m_TalkModeImg ;
    CMyButton	m_TalkSendBtn ;
    CMyCheck	m_TalkMipmapBtn ;
    CMyButton	m_TalkHelpBtn ;
    CMyCheck	m_TalkPkChk ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTalk)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnSetfocusTalkEdtData();
    afx_msg void OnKillfocusTalkEdtData();
    //}}AFX_VIRTUAL

    // Implementation
public:
    void InsertEmotion(int nEmotion);
    void ChangePkMode();
    void SetActionList();
    BOOL m_bFocusNameEdt;
    bool GetIsGameMsgEx();
    void SetTalkDlgRgn();
    BOOL m_bShowGoods;
    void CycleTalkChannel( int nDir );
    void CycleTalkTarget( int nDir );
    void SetTalkTargetList();
    void CheckTalkTargetRect();
    // Generated message map functions
    //{{AFX_MSG(CDlgTalk)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnTalkBtnSend();
    afx_msg void OnTalkImgMode();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnUpdateTalkEdtName();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTalkChkPk();
    afx_msg void OnTalkChkRun();
    afx_msg void OnTalkBtnMipmap();
    afx_msg void OnTalkBtnHelp();
    afx_msg void OnBtnHidegoods();
    afx_msg void OnTalkClearbtn();
    afx_msg void OnTalkMychat();
    afx_msg void OnTalkBtnViewequip();
    afx_msg void OnTalkBtnTargetlist();
    afx_msg void OnTalkBtnEmotionlist();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
protected:
    int m_nTextActionIndex;
    DWORD m_dwFlashTime;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTALK_H__7B85476C_7BD6_463E_A558_EEE57C74B29A__INCLUDED_)
