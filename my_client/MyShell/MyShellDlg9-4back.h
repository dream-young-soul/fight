
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// MyShellDlg.h : header file
//

#if !defined(AFX_MYSHELLDLG_H__61E22FCF_BADA_42B3_A353_20C540D23727__INCLUDED_)
#define AFX_MYSHELLDLG_H__61E22FCF_BADA_42B3_A353_20C540D23727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
#include "TestStatic.h"

#include "DlgLogin.h"
#include "DlgRoleCreate.h"
#include "DlgMain.h"
#include "DlgConfirm.h"
#include "DlgTask.h"
#include "DlgTitleBar.h"
#include "MySlider.h"
#include "DlgHelp.h"
#include "DlgLog.h"
#include "DlgLogo.h"

#include "game3deffectex.h"
/////////////////////////////////////////////////////////////////////////////
// CMyShellDlg dialog
class CPlayer ;
class CMyShellDlg : public CDialog
{
    // Construction
public:
    void RealExit();
    void ProcessMouseHold();
    void SetGameCursor();
    void OpenInsHelpDlg();
    BOOL ProcessHotkey ( int nKey );
    void MoveControl();
    void MoveDlg();
    void SetQUseItemNum();
    void SetScreenMode( BYTE btType, BOOL bBegin = true );
    void OnIconQUse ( LPARAM lParam );
    void ShowMessage( CItem* pItem, int nItemNum = 0 , BOOL bShowDur = true);
    void ShowHelpDlg( LPARAM lParam );
    void OnIconBuy(LPARAM lParam );
    void OnIconUse ( LPARAM lParam );
    void PickMapItem ( OBJID ItemID, int nPosx, int nPosy);
    void DropMapItem( int nPosx, int nPosy , int nPackageType = 0);
    void OnExit ( UINT uType = 0 );
    void OnMpcGoodsClose();
    void OnIconTrade( LPARAM lParam );
    void OnIconDrop( LPARAM lParam );
    void ShowPickUpIcon();
    void OnIconPickUp( LPARAM lParam );
    void ShowMsgDlg ();
    void SetMsgDlgPnt ( LPARAM lParam, int nWidth );
    void CreateRole( BYTE btRoleType ) ;
    void StartGame();
    CDlgLogin	m_DlgLogin ;
    CDlgRoleCreate m_DlgRoleCreate ;
    CDlgMain	m_DlgMain ;
    CDlgConfirm	m_DlgConfirm ;
    CDlgTask	m_DlgTask ;
    CDlgTitleBar m_DlgTitleBar ;
    CDlgHelp	m_DlgHelp ;
    CDlgLog		m_DlgLog ;
    BOOL		m_bLogin ;
    BOOL		m_bWait ;
    BOOL		m_bRoleCreate ;
    BOOL		m_bMsgDlg ;
    CPoint		m_MsgDlgPnt ;
    CPoint		m_StaticPoint[32] ;			// the progress's rect
    UINT		m_uMsgDlgType ;
    DWORD		m_dwMsgDlgOwner ;
    BYTE		m_btStaticID ;
    BOOL		m_bIconFix ;
    CRect		m_InitDlgRect ;
    BOOL		m_bDlgMove ;
    // use for screen mode
    int			m_nScreenMode ;
    DEVMODE		m_OldDeviceMode ;
    // use for pickup icon
    CPoint		m_IconMousePnt ;
    CPoint		m_IconShowPnt ;
    OBJID		m_IconTypeID ;
    OBJID		m_IconRealID ;
    DWORD		m_IconIndex ;
    UINT		m_PickUpDlgID ;
    BYTE		m_btIconType ;
    // use for magic
    OBJID		m_MagicID ;
    OBJID		m_DisMagicID ;
    // use for xp
    OBJID		m_XpID ;
    UINT		m_uCurSkill ;
    //Game font
    CFont*		m_pGameFont ;
    // Owner ini file path
    char		m_strOwnerIni[128] ;
    // use for the hotkey
    static int	m_nHotkeyValue[10] ;
    // use for scale the map
    DWORD		m_dwScale ;
    // use for auto magic attack
    BOOL		m_bAutoMagicAtk ;
    OBJID		m_AutoMagicID ;
    OBJID		m_AutoTargetID ;
    // use for the faction leave word
    int			m_nFacWordValue ;
    // use for the importance item
    int			m_nImptIndex ;
    // use for the drop item
    int			m_nDropIndex ;
    CPoint		m_DropPnt ;
    // use for the exp show
    BOOL		m_bExpShow ;
    // the state when exit dialog was show
    BOOL		m_bExit ;
    // for the mouse click state
    BOOL		m_bLbuttonDown ;
    BOOL		m_bLbuttonHold ;
    BOOL		m_bRbuttonDown ;
    BOOL		m_bRbuttonHold ;
    DWORD		m_dwLastBtnDTime ;
    // For test if the dialog exit when init
    BOOL		m_bInitSuc ;
    // For show or hide system message
    BOOL		m_bSysMsg ;
    // For the head and mipmap dialog
    BOOL		m_bHeadClosed ;

    CMyShellDlg(CWnd* pParent = NULL);	// standard constructor

    DWORD m_dwGameLoop;
    CShellDlg(CWnd* pParent = NULL);	// standard constructor

    enum {CMD_NORMAL = 0, CMD_MARRIAGE, CMD_DIVORCE,
          CMD_ATK, CMD_FRDMAKE, CMD_FRDBREAK,
          CMD_TEMJOIN, CMD_TEMINVITE,
          CMD_FACJOIN, CMD_FACINVITE,
          CMD_TALK,
          CMD_FREEZE, CMD_UNFREEZE, CMD_SELFUNFREEZE,
          CMD_TRADE, CMD_LAY, CMD_MPCCOURT, CMD_DELNPC,
          CMD_NPCEQUIP,
          CMD_TEACHERREQ, CMD_STUDENTREQ,
          CMD_USEITEMTO,
          CMD_ADDALLY
         };
    UINT m_uCmd;

    HCURSOR		m_hCursorH;			// Hand cursor
    HCURSOR		m_hCursorFriend;	// friend cursor
    HCURSOR		m_hCursorPnt ;		// the point cursor
private:
    // Dialog Data
    //{{AFX_DATA(CMyShellDlg)
    enum { IDD = IDD_MYSHELL_DIALOG };
    CTestStatic	m_stcBg;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyShellDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL
public:
    int  ShowItemInfo(CItem* pItem, int x, int y, int& nMaxLen, BOOL bCanShow = TRUE);
    BOOL CloseCurDlg();
    void SetMouseMove(int nPosX, int nPosY);
    void AttackMagicXp();
    void SetLoginEffectPos(BOOL bLogin);
    void ResetLogin();
    void ShowAniCursor(char* szCursorAni);
    void OnIconUseGem(OBJID IdGem);
    void AttackMagic(int nHotKey, BOOL bFlag);	//快捷魔法攻击
    void OpenBigmap();
    void OpenMipmap();
    void ReSetShowLines(bool bFlagEx = false);
    void SetChatArea();
    void ResetMusic( BOOL bActive ) ;
    int  m_MusicVolume;
    int  m_SoundVolume;
    BOOL m_bPopWindows;
    afx_msg void OnTimer(UINT nIDEvent);
    // Implementation
protected:
    bool	m_bProgressHold;	// 一直显示进度槽
    INT		m_dwProgressNumber;	// 进度槽编号
    INT		m_HoldX;			// 显示的坐标
    INT		m_HoldY;			// 显示的坐标
    bool	m_bInJustNow;		// 鼠标是否刚刚进入
    DWORD	m_dwInTime;			// 鼠标进入的时间
    BOOL				m_bUseItem;
    CGame3DEffectEx		m_EffectExItem ;
    CMyPos				m_EffectExItemPos;
    void SetUseItemEffect(int x, int y, OBJID id);
    void SetCursorAni(const char* szAni);
    char m_szCursorAni[32];
    void OnMonsterChoose(int nIndex);
    BOOL IsCanCheckOutBagItem(OBJID idType);
    HICON m_hIcon;
    // Generated message map functions
    //{{AFX_MSG(CMyShellDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMyMessage(WPARAM wParam, LPARAM lParam);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMove(int x, int y);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSHELLDLG_H__61E22FCF_BADA_42B3_A353_20C540D23727__INCLUDED_)
