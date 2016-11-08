
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMAIN_H__7BCFAE77_739A_4B63_8203_299C48F01BF0__INCLUDED_)
#define AFX_DLGMAIN_H__7BCFAE77_739A_4B63_8203_299C48F01BF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog
#include "MyButton.h"
#include "MyList.h"
#include "MyProgress.h"
#include "MySlider.h"
#include "MyStatic.h"
#include "MyGrid.h"
#include "MyCheck.h"
#include "MyImage.h"

#include "DlgQuery.h"
#include "DlgEnemy.h"
#include "DlgMpcGoods.h"
#include "DlgTrade.h"
#include "DlgSetup.h"
#include "DlgGroup.h"
#include "DlgTalk.h"
#include "DlgNpcBuy.h"
#include "DlgNpcFace.h"
#include "DlgDepot.h"
#include "DlgListBox.h"
#include "DlgPk.h"
#include "DlgImprove.h"
#include "DlgEmbed.h"
#include "DlgMsgSend.h"
#include "DlgMsgReceive.h"
#include "DlgMpcBooth.h"
#include "DlgNpcBooth.h"
#include "DlgHawk.h"
#include "DlgLog.h"
#include "DlgEffigy.h"
#include "DlgOther.h"
#include "DlgDice.h"
#include "DlgDiceList.h"
#include "DlgInsHelp.h"
#include "DlgMipmap.h"
#include "DlgMsg.h"
#include "DlgProgress.h"
#include "DlgXp.h"
//new
#include "DlgEmotion.h"
#include "DlgMonsterHeads.h"
#include "DlgAucPackage.h"
#include "DlgBidAuction.h"
#include "DlgAddAuction.h"
#include "DlgIdentify.h"
#include "DlgNpcEquip.h"
#include "DlgTboardList.h"
#include "DlgTboardDetial.h"
#include "DlgTeacherboard.h"
#include "DlgTaskUpItem.h"
#include "DlgPlayerTaskView.h"
#include "DlgPlayerTaskGet.h"
#include "DlgPlayerTaskList.h"
#include "DlgBigmap.h"
#include "DlgKnightage.h"
#include "DlgMonster.h"
#include "DlgMonsterHatch.h"
#include "DlgMonsterDepot.h"
#include "DlgMonsterImprove.h"
#include "DlgMonsterNpc.h"
#include "DlgMonsterEvolve.h"
#include "DlgNorMsg.h"

class CDlgMain : public CDialog
{
    // Construction
public:
    void CloseCurDlg ( int nPos = 0 );
    void ShowChildDlg ();
    void Show ();
    CDlgMain(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bNoMana ;				// test mana

    // use for the leave msg
    BOOL		m_bLevRead ;

    // use for the faction msg
    BOOL		m_bFactionRead ;

    BOOL		m_bReacord;
private:
    POINT		m_DlgPnt ;				// the show point
    // use for play the item action
    CPoint		m_ActPnt ;				// the action show point
    BOOL		m_bItemAct ;			// the action play
    int			m_nActAmount ;			// the action amount
    DWORD		m_dwLastActTime ;		// the last act time

    // use for play the XP action
    BOOL		m_bXpAct ;				// the action play
    DWORD		m_dwLastXpTime ;		// the last xp act time

    // the owner ini file path
    char		m_strOwnerIni[128] ;

    // use for hide the dialog
    BOOL		m_bUpHide ;
    BOOL		m_bLeftHide ;
    // child dialog
public:
    UINT			m_uCurLDlgID;			// the current left side dialog
    UINT			m_uCurRDlgID;			// the current right side dialog
    UINT			m_uCurMDlgID;			// the current mid side dialog
    BOOL			m_bCreate ;
	
    CDlgEmotion		   m_DlgEmotion;
    CDlgMonsterHeads   m_DlgMonsterHeads;
    CDlgAddAuction     m_DlgAddAucItem;
    CDlgAucPackage	   m_DlgAucPackage;
    CDlgBidAuction	   m_DlgBidAuction;
    CDlgIdentify	   m_DlgIdentify;
    CDlgNpcEquip	   m_DlgNpcEquip;
    CDlgMonsterNpc	   m_DlgMonsterNpc;
    CDlgMonsterImprove m_DlgMonsterImprove;
    CDlgMonsterDepot m_DlgMonsterDepot;
    CDlgMonsterEvolve  m_DlgMonsterEvolve;
    CDlgMonsterHatch m_DlgMonsterHatch;
    CDlgTboardDetial m_DlgAnnounceInfo;
    CDlgTboardList	m_DlgAnnounceList;
    CDlgTeacherBoard m_DlgAnnounceUp;
    CDlgTaskUpItem  m_DlgTaskUpItem;
    CDlgPlayerTaskView m_DlgPlayerTaskView;
    CDlgPlayerTaskGet  m_DlgPlayerTaskUp;
    CDlgPlayerTaskList m_DlgPlayerTaskList; //task list dialog 3-kind
    CDlgBigmap		m_DlgBigmap;
    CDlgKnightage	m_DlgKnightage;
    CDlgMonster		m_DlgMonster;
    // 	CDlgLog			m_DlgChatLog;
    CDlgXp			m_DlgXp;
    CDlgQuery		m_DlgQuery ;			// the query dialog
    CDlgEnemy		m_DlgEnemyFriend ;			// the friend dialog
    CDlgMpcGoods	m_DlgMpcGoods ;			// the mpc goods dialog
    CDlgTrade		m_DlgTrade ;			// the trade dialog
    CDlgSetup		m_DlgSetup ;			// the setup dialog
    CDlgGroup		m_DlgGroup ;			// the group dialog
    CDlgTalk		m_DlgTalk ;				// the talk dialog
    CDlgNpcBuy		m_DlgNpcBuy ;			// the npc buy dialog
    CDlgDepot		m_DlgDepot ;			// the depot dialog
    CDlgPk			m_DlgPk ;				// the pk dialog
    CDlgImprove		m_DlgImprove ;			// the improve dialog
    CDlgEmbed		m_DlgEmbed ;			// the embed dialog
    CDlgMsgSend		m_DlgMsgSend ;			// the msg send dialog
    CDlgMsgReceive	m_DlgMsgReceive ;		// the msg receive dialog
    CDlgMpcBooth	m_DlgMpcBooth ;			// the mpc booth dialog
    CDlgNpcBooth	m_DlgNpcBooth ;			// the npc booth dialog
    CDlgNpcFace		m_DlgNpcFace ;			// the npc face dialog
    CDlgHawk		m_DlgHawk ;				// the hawk dialog
    CDlgOther		m_DlgOther ;			// the other dialog
    CDlgDice		m_DlgDice ;				// the dice dialog
    CDlgDiceList	m_DlgDiceList ;			// the dice List dialog
    CDlgInsHelp		m_DlgInsHelp ;			// the instand help dialog
    CDlgMipmap		m_DlgMipmap ;			// the mipmap dialog
    CDlgMsg			m_DlgMsg ;				// the message dialog
    CDlgProgress	m_DlgProgress ;			// the progress dialog
    CDlgNorMsg		m_DlgNorMsgDown;
    CDlgNorMsg      m_DlgNorMsgUp;

public:
    void OpenEmbed();
    void OpenNpcFace();
    void OpenImprove(int nType);
    void HeroDie();
    void HeroReborn();
    void PlayXp();
    void OnXpFull();
    void SetRecord( const char* strIniFile );
    void OpenDepot( int nNpcType, OBJID idNpc );
    void PlayItemQUse( int nGridIndex );
    void OnShellEnter();
    void OpenNpcShop ( int nNpcType, OBJID idNpc );
    // Dialog Data
    //{{AFX_DATA(CDlgMain)
    enum { IDD = IDD_MAIN_DIALOG };
    CMyButton	m_BtnUpHide;
    CMyButton	m_MagicBtn;
    CMyButton	m_FactionBtn;
    CMyButton	m_MonsterBtn;
    CMyButton	m_QueryBtn ;
    CMyButton	m_GoodBtn ;
    CMyButton	m_SetupBtn ;
    CMyButton	m_ChatBtn ;
    CMyButton	m_GroupBtn ;
    CMyButton	m_FriendBtn;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMain)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:
    void OnMainStones();
    void OnMainEquip();
    void OpenIdentifyDlg();
    void OpenAucPackage(OBJID idNpc);
    void OpenBidAuctionDlg(OBJID idNpc);
    void OpenAddAucItemDlg(OBJID idNpc);
    void OpenMonsterImpDlg(int nMode);
    void OpenMonsterDepot(OBJID idNpc);
    void OpenMonsterEvolve(int nMode);
    void OpenHatchDlg(OBJID idNpc);
    void CloseNpcDlg();
    void OpenAnnounceInfo();
    void OpenAnnounceUp();
    void OpenAnnounceList();
    void OpenTaskUpItem(UCHAR idTask);
    void OpenTaskUpDlg();
    void OpenTaskList(int nKind);
    void ResetMsgLines();
    void ReSetDlgRgn(BOOL bUpHide);
    void WriteRecord();
    void OpenDice( BOOL bOpen );
    void OpenLay();
    void OpenEffigy();
    void OpenBooth( BOOL bOwner );

    // Generated message map functions
    //{{AFX_MSG(CDlgMain)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnMainQuery();
    afx_msg void OnMainGood();
    afx_msg void OnMainSetup();
    afx_msg void OnMainChat();
    afx_msg void OnMainGroup();
    afx_msg void OnMainMagic();
    afx_msg void OnMainFriend();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBtnUpHide();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMainMonster();
    afx_msg void OnMainFaction();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAIN_H__7BCFAE77_739A_4B63_8203_299C48F01BF0__INCLUDED_)
