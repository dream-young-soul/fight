
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// DlgMain.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell.h"
#include "DlgMain.h"
#include "GameMsg.h"

#include "Hero.h"
#include "3DGamemap.h"
#include "GameMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _SOUND_DISABLE
#define DXPlaySound //
#endif
/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMain::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMain)
    m_uCurLDlgID = -1 ;
    m_uCurRDlgID = -1 ;
    m_uCurMDlgID = -1 ;
    m_bItemAct = false ;
    m_bNoMana = true ;
    m_bLevRead = false ;
    m_bFactionRead = false ;
    m_bCreate = false ;
    m_bLeftHide = false ;
    m_bUpHide = false ;
    m_bReacord =  false;
    //}}AFX_DATA_INIT
}


void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMain)
    DDX_Control(pDX, IDC_BTN_UP_HIDE, m_BtnUpHide);
    DDX_Control(pDX, IDC_MAIN_MAGIC, m_MagicBtn);
    DDX_Control(pDX, IDC_MAIN_FACTION, m_FactionBtn);
    DDX_Control(pDX, IDC_MAIN_MONSTER, m_MonsterBtn);
    DDX_Control(pDX, IDC_MAIN_QUERY, m_QueryBtn);
    DDX_Control(pDX, IDC_MAIN_GOOD, m_GoodBtn);
    DDX_Control(pDX, IDC_MAIN_SETUP, m_SetupBtn);
    DDX_Control(pDX, IDC_MAIN_CHAT, m_ChatBtn);
    DDX_Control(pDX, IDC_MAIN_GROUP, m_GroupBtn);
    DDX_Control(pDX, IDC_MAIN_FRIEND, m_FriendBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialog)
    //{{AFX_MSG_MAP(CDlgMain)
    ON_WM_CTLCOLOR()
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_MAIN_QUERY, OnMainQuery)
    ON_BN_CLICKED(IDC_MAIN_GOOD, OnMainGood)
    ON_BN_CLICKED(IDC_MAIN_SETUP, OnMainSetup)
    ON_BN_CLICKED(IDC_MAIN_CHAT, OnMainChat)
    ON_BN_CLICKED(IDC_MAIN_GROUP, OnMainGroup)
    ON_BN_CLICKED(IDC_MAIN_MAGIC, OnMainMagic)
    ON_BN_CLICKED(IDC_MAIN_FRIEND, OnMainFriend)
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_BTN_UP_HIDE, OnBtnUpHide)
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_BN_CLICKED(IDC_MAIN_MONSTER, OnMainMonster)
    ON_BN_CLICKED(IDC_MAIN_FACTION, OnMainFaction)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMain message handlers

BOOL CDlgMain::OnInitDialog()
{
    CDialog::OnInitDialog();
    CRect rect ;
    CDlgMain::GetWindowRect ( rect ) ;
    CPoint pt[6] ;
    pt[0].x = 0;
    pt[0].y = 160;
    pt[1].x = 160;
    pt[1].y = 200;
    pt[2].x = 200;
    pt[2].y = 0;
    pt[3].x = rect.Width();
    pt[3].y = 0;
    pt[4].x = rect.Width();
    pt[4].y = rect.Height();
    pt[5].x = 0;
    pt[5].y = rect.Height();
    CRgn rgn ;
    rgn.CreatePolygonRgn ( pt, 6, ALTERNATE ) ;
    CDlgMain::SetWindowRgn ( rgn, true ) ;
    // Init the buttons
    //left button
    //	m_StoneGoods.Init(rect.left, rect.top, "Button42");
    m_QueryBtn.Init ( rect.left, rect.top, "Button40" );
    m_MagicBtn.Init	( rect.left, rect.top, "Check65" );
    m_GoodBtn.Init ( rect.left, rect.top, "Button47" );
    m_MonsterBtn.Init ( rect.left, rect.top, "Button44" );
    //right button
    m_FriendBtn.Init ( rect.left, rect.top, "Button41" );
    m_SetupBtn.Init ( rect.left, rect.top, "Button43" );
    m_ChatBtn.Init ( rect.left, rect.top, "Button45" );
    m_GroupBtn.Init ( rect.left, rect.top, "Button46" );
    m_BtnUpHide.Init( rect.left, rect.top, "Button415") ;
    m_FactionBtn.Init ( rect.left, rect.top, "Button416" );
    // Init the image
    //	m_MagicBtn.Init ( rect.left, rect.top,  );
    // Init the child dialog ////////////////////////////////////////
    //聊天表情
    m_DlgEmotion.Create (IDD_DIALOG_EMOTION, CDlgMain::GetParent());
    m_DlgEmotion.GetWindowRect(rect);
    m_DlgEmotion.MoveWindow ( 630,
                              565, rect.Width (), rect.Height (), false);
    m_DlgEmotion.ShowWindow (SW_SHOW);
    m_DlgEmotion.EnableWindow (false);
    //幻兽头像
    m_DlgMonsterHeads.Create(IDD_DIALOG_MONSTERHEADS, CDlgMain::GetParent ());
    m_DlgMonsterHeads.GetWindowRect (rect);
    m_DlgMonsterHeads.MoveWindow (_SCR_WIDTH - rect.Width () - 140,
                                  0, rect.Width (), rect.Height (), false);
    m_DlgMonsterHeads.ShowWindow (SW_SHOW);
    m_DlgMonsterHeads.EnableWindow ();
    //拍卖叫价
    m_DlgBidAuction.Create(IDD_DIALOG_BIDAUCTION, CDlgMain::GetParent());
    m_DlgBidAuction.GetWindowRect(rect);
    m_DlgBidAuction.MoveWindow((_SCR_WIDTH - rect.Width()) / 2 - 50,
                               (_SCR_HEIGHT - rect.Height()) / 2 - 70,
                               rect.Width(), rect.Height());
    m_DlgBidAuction.ShowWindow(SW_SHOW);
    m_DlgBidAuction.EnableWindow(false);
    //拍卖仓库
    m_DlgAucPackage.Create(IDD_DIALOG_AUCPACKAGE, CDlgMain::GetParent());
    m_DlgAucPackage.GetWindowRect(rect);
    m_DlgAucPackage.MoveWindow((_SCR_WIDTH - rect.Width()) / 2 - 40,
                               (_SCR_HEIGHT - rect.Height()) / 2,
                               rect.Width(), rect.Height());
    m_DlgAucPackage.ShowWindow(SW_SHOW);
    m_DlgAucPackage.EnableWindow(false);
    //加入拍卖
    m_DlgAddAucItem.Create(IDD_DIALOG_ADDAUCITEM, CDlgMain::GetParent());
    m_DlgAddAucItem.GetWindowRect(rect);
    m_DlgAddAucItem.MoveWindow((_SCR_WIDTH - rect.Width()) / 2 - 50,
                               (_SCR_HEIGHT - rect.Height()) / 2 - 60,
                               rect.Width(), rect.Height());
    m_DlgAddAucItem.ShowWindow(SW_SHOW);
    m_DlgAddAucItem.EnableWindow(false);
    //鉴定
    m_DlgIdentify.Create(IDD_DIALOG_IDENTIFY, CDlgMain::GetParent());
    m_DlgIdentify.GetWindowRect(rect);
    m_DlgIdentify.MoveWindow((_SCR_WIDTH - rect.Width()) / 2 - 30,
                             (_SCR_HEIGHT - rect.Height()) / 2 - 30,
                             rect.Width(), rect.Height());
    m_DlgIdentify.ShowWindow(SW_SHOW);
    m_DlgIdentify.EnableWindow(false);
    //init the npc equip view dialog
    m_DlgNpcEquip.Create(IDD_DIALOG_NPCEQUIP, CDlgMain::GetParent());
    m_DlgNpcEquip.GetWindowRect(rect);
    m_DlgNpcEquip.MoveWindow(_SCR_WIDTH - rect.Width() - 100,
                             450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgNpcEquip.ShowWindow(SW_SHOW);
    m_DlgNpcEquip.EnableWindow(false);
    //init npc monster select dialog
    m_DlgMonsterNpc.Create(IDD_DIALOG_MONSTERNPC, CDlgMain::GetParent());
    m_DlgMonsterNpc.GetWindowRect(rect);
    m_DlgMonsterNpc.MoveWindow((_SCR_WIDTH - rect.Width()) / 2,
                               450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgMonsterNpc.ShowWindow(SW_SHOW);
    m_DlgMonsterNpc.EnableWindow(false);
    //init npc monster depot dialog
    m_DlgMonsterDepot.Create(IDD_DIALOG_MONSTERDEPOT, CDlgMain::GetParent());
    m_DlgMonsterDepot.GetWindowRect(rect);
    m_DlgMonsterDepot.MoveWindow(5,
                                 70, rect.Width(), rect.Height());
    m_DlgMonsterDepot.ShowWindow(SW_SHOW);
    m_DlgMonsterDepot.EnableWindow(false);
    //init npc monster evolve dialog
    m_DlgMonsterEvolve.Create(IDD_DIALOG_MONSTEREVOLVE, CDlgMain::GetParent());
    m_DlgMonsterEvolve.GetWindowRect(rect);
    m_DlgMonsterEvolve.MoveWindow(50,
                                  450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgMonsterEvolve.ShowWindow(SW_SHOW);
    m_DlgMonsterEvolve.EnableWindow(false);
    //init npc monster hatch dialog
    m_DlgMonsterImprove.Create(IDD_DIALOG_MONSTERIMPROVE, CDlgMain::GetParent());
    m_DlgMonsterImprove.GetWindowRect(rect);
    m_DlgMonsterImprove.MoveWindow(50,
                                   450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgMonsterImprove.ShowWindow(SW_SHOW);
    m_DlgMonsterImprove.EnableWindow(false);
    //init npc monster hatch dialog
    m_DlgMonsterHatch.Create(IDD_DIALOG_MONSTERHATCH, CDlgMain::GetParent());
    m_DlgMonsterHatch.GetWindowRect(rect);
    m_DlgMonsterHatch.MoveWindow(50,
                                 450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgMonsterHatch.ShowWindow(SW_SHOW);
    m_DlgMonsterHatch.EnableWindow(false);
    //init the announce dialog
    m_DlgAnnounceList.Create(IDD_DIALOG_TBOARDLIST, CDlgMain::GetParent());
    m_DlgAnnounceList.GetWindowRect(rect);
    m_DlgAnnounceList.MoveWindow(_SCR_WIDTH -  rect.Width() - 100,
                                 450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgAnnounceList.ShowWindow(SW_SHOW);
    m_DlgAnnounceList.EnableWindow(false);
    //init the announce dialog
    m_DlgAnnounceUp.Create(IDD_DIALOG_TEACHERBOARD, CDlgMain::GetParent());
    m_DlgAnnounceUp.GetWindowRect(rect);
    m_DlgAnnounceUp.MoveWindow(_SCR_WIDTH - rect.Width() - 100,
                               450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgAnnounceUp.ShowWindow(SW_SHOW);
    m_DlgAnnounceUp.EnableWindow(false);
    m_DlgAnnounceInfo.Create(IDD_DIALOG_TBOARDDETIAL, CDlgMain::GetParent());
    m_DlgAnnounceInfo.GetWindowRect(rect);
    m_DlgAnnounceInfo.MoveWindow(0, 450 - rect.Height(), rect.Width(), rect.Height());
    m_DlgAnnounceInfo.ShowWindow(SW_SHOW);
    m_DlgAnnounceInfo.EnableWindow(false);
    //init the task up item dialog
    m_DlgTaskUpItem.Create(IDD_DIALOG_UPTASKITEM, CDlgMain::GetParent());
    m_DlgTaskUpItem.GetWindowRect(rect);
    m_DlgTaskUpItem.MoveWindow((_SCR_WIDTH - rect.Width()) / 2,
                               200, rect.Width(), rect.Height());
    m_DlgTaskUpItem.ShowWindow(SW_SHOW);
    m_DlgTaskUpItem.EnableWindow(false);
    //init the task dialog
    m_DlgPlayerTaskView.Create(IDD_DIALOG_PLAYERTASKVIEW, CDlgMain::GetParent());
    m_DlgPlayerTaskView.GetWindowRect(rect);
    m_DlgPlayerTaskView.MoveWindow(_SCR_WIDTH - rect.Width(),
                                   450 - rect.Height(),
                                   rect.Width(), rect.Height(), false);
    m_DlgPlayerTaskView.ShowWindow(SW_SHOW);
    m_DlgPlayerTaskView.EnableWindow(false);
    m_DlgPlayerTaskUp.Create(IDD_DIALOG_PLAYERTASKGET, CDlgMain::GetParent());
    m_DlgPlayerTaskUp.GetWindowRect(rect);
    m_DlgPlayerTaskUp.MoveWindow(_SCR_WIDTH - rect.Width(),
                                 450 - rect.Height(),
                                 rect.Width(), rect.Height(), false);
    m_DlgPlayerTaskUp.ShowWindow(SW_SHOW);
    m_DlgPlayerTaskUp.EnableWindow(false);
    m_DlgPlayerTaskList.Create(IDD_DIALOG_PLAYERTASKLIST, CDlgMain::GetParent());
    m_DlgPlayerTaskList.GetWindowRect(rect);
    m_DlgPlayerTaskList.MoveWindow(_SCR_WIDTH - rect.Width(),
                                   450 - rect.Height(),
                                   rect.Width(), rect.Height(), false);
    m_DlgPlayerTaskList.ShowWindow(SW_SHOW);
    m_DlgPlayerTaskList.EnableWindow(false);
    //Init the InsHelp dialog
    m_DlgInsHelp.Create(IDD_DIALOG_INSHELP, CDlgMain::GetParent());
    m_DlgInsHelp.GetWindowRect(rect);
    m_DlgInsHelp.MoveWindow((_SCR_WIDTH - rect.Width()) / 2,
                            (_SCR_HEIGHT - rect.Height()) / 2 - 100,
                            rect.Width(), rect.Height());
    m_DlgInsHelp.ShowWindow(SW_SHOW);
    m_DlgInsHelp.EnableWindow(false);
    //Init the Bigmap dialog
    m_DlgBigmap.Create( IDD_DIALOG_BIGMAP, CDlgMain::GetParent());
    m_DlgBigmap.GetWindowRect(rect);
    m_DlgBigmap.MoveWindow((_SCR_WIDTH - rect.Width()) / 2, (_SCR_HEIGHT - rect.Height()) / 2,
                           rect.Width(), rect.Height());
    m_DlgBigmap.ShowWindow(SW_SHOW);
    m_DlgBigmap.EnableWindow(false);
    //帮派界面
    // Init the Knightage dialog
    m_DlgKnightage.Create( IDD_DIALOG_KNIGHTAGE, CDlgMain::GetParent());
    m_DlgKnightage.GetWindowRect( rect );
    m_DlgKnightage.MoveWindow(_SCR_WIDTH - rect.Width() - 100,
                              450 - rect.Height(),
                              rect.Width(), rect.Height(), false);
    m_DlgKnightage.ShowWindow( SW_SHOW ) ;
    m_DlgKnightage.EnableWindow(false);
    // Init the Mpcgoods dialog
    m_DlgMpcGoods.Create ( IDD_DIALOG_MPCGOODS, CDlgMain::GetParent () ) ;
    m_DlgMpcGoods.GetWindowRect ( rect ) ;
    m_DlgMpcGoods.MoveWindow ( _SCR_WIDTH -  rect.Width() - 100,
                               450,
                               rect.Width(),
                               rect.Height() ) ;
    m_DlgMpcGoods.ShowWindow( SW_SHOW ) ;
    m_DlgMpcGoods.EnableWindow( false ) ;
    // Init the monster dialog
    m_DlgMonster.Create( IDD_DIALOG_MONSTER, CDlgMain::GetParent());
    m_DlgMonster.GetWindowRect( rect );
    m_DlgMonster.MoveWindow(_SCR_WIDTH - rect.Width() - 100, 450 - rect.Height(),
                            rect.Width(), rect.Height());
    m_DlgMonster.ShowWindow( SW_SHOW ) ;
    m_DlgMonster.EnableWindow(false);
    // Init the Equip dialog
    m_DlgQuery.Create( IDD_DIALOG_QUERY, CDlgMain::GetParent());
    m_DlgQuery.GetWindowRect ( rect );
    m_DlgQuery.MoveWindow ( _SCR_WIDTH - rect.Width() - 100, 450 - rect.Height(),
                            rect.Width(),
                            rect.Height());
    m_DlgQuery.ShowWindow( SW_SHOW ) ;
    m_DlgQuery.EnableWindow(false);
    // Init the talk dialog
    m_DlgTalk.Create ( IDD_DIALOG_TALK, CDlgMain::GetParent () ) ;
    m_DlgTalk.GetWindowRect ( rect ) ;
    m_DlgTalk.MoveWindow ( 96,
                           _SCR_HEIGHT - rect.Height(),
                           rect.Width(),
                           rect.Height() ) ;
    m_DlgTalk.ShowWindow( SW_SHOW ) ;
    m_DlgTalk.EnableWindow( false ) ;
    int nHeight = (CMyBitmap::GetFontSize() + 2) * g_objGameMsg.GetShowLines();
    m_DlgNorMsgDown.Create(IDD_DIALOG_NORMSG, CDlgMain::GetParent ());
    m_DlgNorMsgDown.MoveWindow(0,
                               g_objGameMsg.GetNorAeroTop() - _MSGDLG_SETRECT,
                               36, _MSGDLG_SETRECT);
    m_DlgNorMsgDown.ShowWindow( SW_SHOW );
    m_DlgNorMsgUp.Create(IDD_DIALOG_NORMSG, CDlgMain::GetParent ());
    m_DlgNorMsgUp.GetWindowRect(rect);
    m_DlgNorMsgUp.MoveWindow(0, g_objGameMsg.GetNorAeroTop() - _MSGDLG_SETRECT * 2 - 10 - nHeight, 36, nHeight + _MSGDLG_SETRECT);
    m_DlgNorMsgUp.ShowWindow( SW_SHOW );
    m_DlgNorMsgUp.m_nMsgUpDown = 1;
    m_DlgTalk.m_MenuTalk.GetWindowRect( rect ) ;
    m_DlgTalk.m_MenuTalk.MoveWindow( 107,
                                     _SCR_HEIGHT - rect.Height() - 50,
                                     rect.Width(),
                                     rect.Height() ) ;
    m_DlgTalk.m_MenuTalk.ShowWindow( SW_SHOW ) ;
    m_DlgTalk.m_MenuTalk.EnableWindow( false ) ;
    //Below is Wen Yiku Code 2005-6-30
    //Init the Room login dialog
    /*
    	m_DlgTalk.m_RoomLg.GetWindowRect( rect ) ;
    	m_DlgTalk.m_RoomLg.MoveWindow( (_SCR_WIDTH  -  rect.Width() ) / 2,
    		( _SCR_HEIGHT -  rect.Height() ) / 2 - 100,
    		rect.Width(),
    		rect.Height() ,false) ;
    	m_DlgTalk.m_RoomLg.ShowWindow( SW_SHOW ) ;
    	m_DlgTalk.m_RoomLg.EnableWindow( false ) ;*/
    //Wen Yiku Code 2005-6-30
    // Init the friend dialog
    m_DlgEnemyFriend.Create ( IDD_DIALOG_ENEMY, CDlgMain::GetParent () ) ;
    m_DlgEnemyFriend.GetWindowRect ( rect ) ;
    m_DlgEnemyFriend.MoveWindow ( _SCR_WIDTH - rect.Width() - 100,
                                  450 -  rect.Height(),
                                  rect.Width (),
                                  rect.Height () ) ;
    m_DlgEnemyFriend.ShowWindow( SW_SHOW ) ;
    m_DlgEnemyFriend.EnableWindow( false ) ;
    // Init the trade dialog
    m_DlgTrade.Create ( IDD_DIALOG_TRADE, CDlgMain::GetParent () ) ;
    m_DlgTrade.GetWindowRect ( rect ) ;
    m_DlgTrade.MoveWindow ( 10,
                            550 - rect.Height(),
                            rect.Width (),
                            rect.Height () ) ;
    m_DlgTrade.ShowWindow( SW_SHOW ) ;
    m_DlgTrade.EnableWindow( false ) ;
    // Init the setup dialog
    m_DlgSetup.Create ( IDD_DIALOG_SETUP, CDlgMain::GetParent () ) ;
    m_DlgSetup.GetWindowRect ( rect ) ;
    m_DlgSetup.MoveWindow ( (_SCR_WIDTH  -  rect.Width() ) / 2,
                            ( _SCR_HEIGHT -  rect.Height() ) / 2 - 100,
                            rect.Width() ,
                            rect.Height() ) ;
    m_DlgSetup.ShowWindow( SW_SHOW ) ;
    m_DlgSetup.EnableWindow( false ) ;
    /*/ Init the chat dialog
    m_DlgAct.Create ( IDD_DIALOG_ACT, CDlgMain::GetParent () ) ;
    m_DlgAct.GetWindowRect ( rect ) ;
    m_DlgAct.MoveWindow ( _SCR_WIDTH / 2 - ( rect.Width() ) / 2,
    					  _SCR_HEIGHT - 140,
    					  rect.Width(),
    					  rect.Height() ) ;
    */
    // Init the Npcbuy dialog
    m_DlgNpcBuy.Create ( IDD_DIALOG_NPCBUY, CDlgMain::GetParent () ) ;
    m_DlgNpcBuy.GetWindowRect ( rect ) ;
    m_DlgNpcBuy.MoveWindow ( _SCR_WIDTH - rect.Width() - 100,
                             450 - rect.Height(),
                             rect.Width(),
                             rect.Height () ) ;
    m_DlgNpcBuy.ShowWindow( SW_SHOW ) ;
    m_DlgNpcBuy.EnableWindow( false ) ;
    // Init the depo dialog
    m_DlgDepot.Create ( IDD_DIALOG_DEPOT, CDlgMain::GetParent () ) ;
    m_DlgDepot.GetWindowRect ( rect ) ;
    m_DlgDepot.MoveWindow ( _SCR_WIDTH - rect.Width() - 127,
                            450 - rect.Height(),
                            rect.Width(),
                            rect.Height () ) ;
    m_DlgDepot.ShowWindow( SW_SHOW ) ;
    m_DlgDepot.EnableWindow( false ) ;
    // Init the improve dialog
    m_DlgImprove.Create ( IDD_DIALOG_IMPROVE, CDlgMain::GetParent () ) ;
    m_DlgImprove.GetWindowRect ( rect ) ;
    m_DlgImprove.MoveWindow ( ( _SCR_WIDTH - ( rect.Width() ) ) / 2 - 50,
                              ( _SCR_HEIGHT - ( rect.Height() ) ) / 2,
                              rect.Width (),
                              rect.Height () ) ;
    m_DlgImprove.ShowWindow(SW_SHOW);
    m_DlgImprove.EnableWindow(false);
    /*/ Init the npcface dialog
    m_DlgNpcFace.Create ( IDD_DIALOG_NPCFACE, CDlgMain::GetParent () ) ;
    m_DlgNpcFace.GetWindowRect ( rect ) ;
    m_DlgNpcFace.MoveWindow ( 0,
    						  65,
    						  rect.Width (),
    						  rect.Height () ) ;
    */
    // Create the embed dialog
    m_DlgEmbed.Create ( IDD_DIALOG_EMBED, CDlgMain::GetParent () ) ;
    m_DlgEmbed.GetWindowRect ( rect ) ;
    m_DlgEmbed.MoveWindow ( ( _SCR_WIDTH - ( rect.Width() ) ) / 2 - 50,
                            ( _SCR_HEIGHT - ( rect.Height() ) ) / 2,
                            rect.Width (),
                            rect.Height () ) ;
    m_DlgEmbed.ShowWindow(SW_SHOW);
    m_DlgEmbed.EnableWindow(false);
    // Init the msgsend dialog
    m_DlgMsgSend.Create ( IDD_DIALOG_MSGSEND, CDlgMain::GetParent () ) ;
    m_DlgMsgSend.GetWindowRect ( rect ) ;
    m_DlgMsgSend.MoveWindow ( _SCR_WIDTH - rect.Width() - 295,
                              450 - rect.Height(),
                              rect.Width (),
                              rect.Height () ) ;
    m_DlgMsgSend.ShowWindow( SW_SHOW ) ;
    m_DlgMsgSend.EnableWindow( false ) ;
    // Init the msgreceive dialog
    m_DlgMsgReceive.Create ( IDD_DIALOG_MSGRECEIVE, CDlgMain::GetParent () ) ;
    m_DlgMsgReceive.GetWindowRect ( rect ) ;
    m_DlgMsgReceive.MoveWindow ( _SCR_WIDTH - rect.Width() - 295,
                                 450 - rect.Height(),
                                 rect.Width (),
                                 rect.Height () ) ;
    m_DlgMsgReceive.ShowWindow( SW_SHOW ) ;
    m_DlgMsgReceive.EnableWindow( false ) ;
    // Init the Mpc Booth dialog
    m_DlgMpcBooth.Create ( IDD_DIALOG_MPCBOOTH, CDlgMain::GetParent () ) ;
    m_DlgMpcBooth.GetWindowRect ( rect ) ;
    m_DlgMpcBooth.MoveWindow ( 0,
                               65,
                               rect.Width (),
                               rect.Height () ) ;
    m_DlgMpcBooth.ShowWindow(SW_SHOW) ;
    m_DlgMpcBooth.EnableWindow(false) ;
    // Init the npc booth dialog
    m_DlgNpcBooth.Create ( IDD_DIALOG_NPCBOOTH, CDlgMain::GetParent () ) ;
    m_DlgNpcBooth.GetWindowRect ( rect ) ;
    m_DlgNpcBooth.MoveWindow ( 0,
                               65,
                               rect.Width (),
                               rect.Height () ) ;
    m_DlgNpcBooth.ShowWindow(SW_SHOW);
    m_DlgNpcBooth.EnableWindow(false);
    // Init the Hawk dialog
    m_DlgHawk.Create ( IDD_DIALOG_HAWK, CDlgMain::GetParent () ) ;
    m_DlgHawk.GetWindowRect ( rect ) ;
    m_DlgHawk.MoveWindow ( 0,
                           ( _SCR_HEIGHT - ( rect.Height() ) ) / 2 + 80,
                           rect.Width() ,
                           rect.Height() ) ;
    m_DlgHawk.ShowWindow(SW_SHOW);
    m_DlgHawk.EnableWindow(false);
    /*/ Init the dice dialog
    m_DlgDice.Create ( IDD_DIALOG_DICE, CDlgMain::GetParent () ) ;
    m_DlgDice.GetWindowRect ( rect ) ;
    m_DlgDice.MoveWindow ( _SCR_WIDTH -  rect.Width(),
    					   0,
    					   rect.Width(),
    					   rect.Height() ) ;
    */
    /*/ Init the dice dialog
    m_DlgDiceList.Create ( IDD_DIALOG_DICELIST, CDlgMain::GetParent () ) ;
    m_DlgDiceList.GetWindowRect ( rect ) ;
    m_DlgDiceList.MoveWindow ( _SCR_WIDTH -  rect.Width() - 316,
    						   0,
    						   rect.Width(),
    						   rect.Height() ) ;

    */
    // Init the progress dialog
    m_DlgProgress.Create( IDD_DIALOG_PROGRESS, CDlgMain::GetParent() ) ;
    m_DlgProgress.GetWindowRect(rect);
    m_DlgProgress.MoveWindow( 0, _SCR_HEIGHT - rect.Height(),
                              rect.Width(),
                              rect.Height());
    m_DlgProgress.ShowWindow( SW_SHOW ) ;
    m_DlgProgress.EnableWindow( false ) ;
    // Init the mipmap dialog
    m_DlgMipmap.Create ( IDD_DIALOG_MIPMAP, CDlgMain::GetParent () ) ;
    m_DlgMipmap.GetWindowRect ( rect ) ;
    m_DlgMipmap.MoveWindow ( _SCR_WIDTH - rect.Width (),
                             0,
                             rect.Width (),
                             rect.Height () ) ;
    m_DlgMipmap.ShowWindow( SW_SHOW ) ;
    m_DlgMipmap.EnableWindow( false ) ;
    // Init the group dialog
    m_DlgGroup.Create ( IDD_DIALOG_GROUP, CDlgMain::GetParent () ) ;
    m_DlgGroup.GetWindowRect ( rect ) ;
    m_DlgGroup.MoveWindow ( _SCR_WIDTH - rect.Width(),
                            _SCR_HEIGHT - rect.Height() - 230,
                            rect.Width(),
                            rect.Height() ) ;
    m_DlgGroup.ShowWindow( SW_SHOW ) ;
    m_DlgGroup.EnableWindow( false ) ;
    m_DlgXp.Create ( IDD_DIALOG_XP, CDlgMain::GetParent () ) ;
    m_DlgXp.ShowWindow(SW_HIDE);
    ////////////////////////////////////
    // Init the contorl tip show
    CRect prgRect ;
    CDlgMain::GetWindowRect( rect ) ;
    //role button
    m_QueryBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10741));
    //goodbox button
    m_GoodBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10742));
    //Monster button
    m_MonsterBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10744));
    //skill button
    m_MagicBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10745));
    //faction button
    m_FactionBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10746));
    //sysset button
    m_SetupBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10747));
    //friend button
    m_FriendBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10748));
    //buy button
    m_ChatBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10749));
    //group button
    m_GroupBtn.SetTipShow((char*)g_objGameDataSet.GetStr(10750));
    m_bCreate = true ;
    //monster button
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    pDC->SetBkMode ( TRANSPARENT ) ;
    return g_StockBrush;
}

void CDlgMain::Show()
{
    // Show the progress dialog
    if ( m_DlgProgress.m_bShow )
    {
        m_DlgProgress.Show() ;
    }
    // Show owner
    CAni* ShowAni = g_objGameDataSet.GetDataAni ( ( char* )g_strControlAni,
                    "Dialog4",
                    EXIGENCE_IMMEDIATE ) ;
    if ( ShowAni != NULL )
    {
        // Show the up dialog
        if ( !m_bUpHide )
        {
            ShowAni->Show( 0, 768, 602 ) ;
            ShowAni->Show( 2, 956 , 530) ;
            m_BtnUpHide.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
            m_GroupBtn.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
            m_FriendBtn.Show ( m_DlgPnt.x, m_DlgPnt.y ) ;
            m_ChatBtn.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
        }
        else
        {
            ShowAni->Show( 1, 768, 602 ) ;
            m_BtnUpHide.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
        }
    }
    else
    {
        return ;
    }
    // Show the left dialog
    m_SetupBtn.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
    // 	m_StoneGoods.Show ( m_DlgPnt.x, m_DlgPnt.y);
    m_QueryBtn.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
    m_FactionBtn.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
    m_GoodBtn.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
    m_MonsterBtn.Show ( m_DlgPnt.x, m_DlgPnt.y) ;
    m_MagicBtn.Show ( m_DlgPnt.x, m_DlgPnt.y);
    m_DlgNorMsgDown.Show();
    if(m_DlgTalk.GetIsGameMsgEx())
    {
        m_DlgNorMsgUp.Show();
    }
    if ( ::GetKeyState( VK_MENU ) >> 7  )
    {
        if (m_DlgTalk.GetIsGameMsgEx())
        {
            g_objGameMsg.ShowEx( true, false ) ;
        }
        else { g_objGameMsg.Show(true, false); }
    }
    else
    {
        if (m_DlgTalk.GetIsGameMsgEx())
        {
            g_objGameMsg.ShowEx( false, false ) ;
        }
        else
        {
            g_objGameMsg.Show( false, false ) ;
        }
    }
    if ( m_DlgTalk.m_bShow )
    {
        m_DlgTalk.Show () ;
    }
    // Show child dialog
    ShowChildDlg () ;
}

void CDlgMain::ShowChildDlg()
{
    // Show the message dialog
    // Show the left side dialog
    switch ( m_uCurLDlgID )
    {
    case DLG_TRADE:
        m_DlgTrade.Show() ;
        break ;
    case DLG_TBOARDINFO:
        m_DlgAnnounceInfo.Show();
        break;
    case DLG_MPCBOOTH:
        m_DlgMpcBooth.Show();
        break;
    case DLG_NPCBOOTH:
        m_DlgNpcBooth.Show();
        break;
    default:
        break ;
    }
    // Show the mid side dialog
    switch ( m_uCurMDlgID )
    {
    case DLG_AUCTION:
        m_DlgBidAuction.Show();
        break;
    case DLG_AUCPACKAGE:
        m_DlgAucPackage.Show();
        break;
    case DLG_ADDAUCITEM:
        m_DlgAddAucItem.Show();
        break;
    case DLG_IDENTIFY:
        m_DlgIdentify.Show();
        break;
    case DLG_MONSTERNPC:
        m_DlgMonsterNpc.Show();
        break;
    case DLG_MONSTERIMPROVE:
        m_DlgMonsterImprove.Show();
        break;
    case DLG_MONSTERDEPOT:
        m_DlgMonsterDepot.Show();
        break;
    case DLG_MONSTEREVOLVE:
        m_DlgMonsterEvolve.Show();
        break;
    case DLG_MONSTERHATCH:
        m_DlgMonsterHatch.Show();
        break;
    case DLG_TASKUPITEM:
        m_DlgTaskUpItem.Show();
        break;
    case DLG_INSHELP:
        m_DlgInsHelp.Show();
        break;
    case DLG_IMPROVE:
        m_DlgImprove.Show();
        break;
    case DLG_EMBED:
        m_DlgEmbed.Show();
        break;
    case DLG_SETUP:
        m_DlgSetup.Show() ;
        break ;
    case DLG_MSGSEND:
        m_DlgMsgSend.Show() ;
        break ;
    case DLG_MSGRECEIVE:
        m_DlgMsgReceive.Show() ;
        break ;
    default:
        break ;
    }
    // Show the mipmap dialog
    if ( m_DlgMipmap.m_bShow )
    {
        m_DlgMipmap.Show() ;
    }
    // Show the right side dialog
    switch ( m_uCurRDlgID )
    {
    case DLG_NPCEQUIP:
        m_DlgNpcEquip.Show();
        break;
    case DLG_TBOARDLIST:
        m_DlgAnnounceList.Show();
        break;
    case DLG_TBOARDUP:
        m_DlgAnnounceUp.Show();
        break;
    case DLG_TASKVIEW:
        m_DlgPlayerTaskView.Show();
        break;
    case DLG_TASKGET:
        m_DlgPlayerTaskUp.Show();
        break;
    case DLG_TASKLIST:
        m_DlgPlayerTaskList.Show();
        break;
    case DLG_CONTACT:
        m_DlgKnightage.Show();
        break;
    case DLG_DEPOT:
        m_DlgDepot.Show();
        break ;
    case DLG_NPCBUY:
        m_DlgNpcBuy.Show();
        break ;
    case DLG_MONSTER:
        m_DlgMonster.Show() ;
        break ;
    case DLG_FRIEND:
        m_DlgEnemyFriend.Show() ;
        break ;
    case DLG_REALSKILL:
        m_DlgQuery.Show();
        break;
    case DLG_QUERY:
        m_DlgQuery.Show() ;
        break ;
    default:
        break ;
    }
    // 摆摊叫卖
    if (m_DlgBigmap.m_bShow)
    {
        m_DlgBigmap.Show();
    }
    if (m_DlgHawk.m_bShow)
    {
        m_DlgHawk.Show();
    }
    // the others dialog
    //shwo Relive button
    if (m_DlgXp.m_bShow)
    {
        m_DlgXp.Show();
    }
    if (m_DlgMpcGoods.m_bShow)
    {
        m_DlgMpcGoods.Show();
    }
    if(m_DlgEmotion.m_bShow)
    {
        m_DlgEmotion.Show ();
    }
    //show the monster heads
    m_DlgMonsterHeads.Show ();
    //show the group dialog
    m_DlgGroup.Show();
}

void CDlgMain::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    m_DlgPnt.x = x ;
    m_DlgPnt.y = y ;
}

void CDlgMain::OnMainQuery()
{
    if (!m_DlgQuery.m_bShow)
    {
        if (m_DlgInsHelp.m_bShow)
        {
            m_DlgInsHelp.EnableWindow(false);
        }
        CDlgMain::GetParent()->PostMessage(WM_MY_MESSAGE, ON_DLGTASK_CLOSE);
        CDlgMain::CloseCurDlg(1);
        CDlgMain::CloseCurDlg(2);
        m_uCurRDlgID = DLG_QUERY;
        m_DlgQuery.PopupCurChk();
        m_DlgQuery.m_btCurChildDlgID = 1;
        m_DlgQuery.m_ChkRole.SetCheckValue(1);
        m_DlgQuery.EnableWindow();
    }
    else
    {
        if(m_DlgQuery.m_btCurChildDlgID == 1)
        {
            CDlgMain::CloseCurDlg(2);
            m_uCurRDlgID = -1;
        }
        else
        {
            m_DlgQuery.PopupCurChk();
            m_DlgQuery.HideCurDlg(m_DlgQuery.m_btCurChildDlgID);
            m_DlgQuery.m_btCurChildDlgID = 1;
            m_DlgQuery.m_ChkRole.SetCheckValue(1);
            m_DlgQuery.ShowCurDlg(1);
        }
    }
}

void CDlgMain::OnMainFriend()
{
    if( !m_DlgEnemyFriend.m_bShow)
    {
        if (m_DlgInsHelp.m_bShow)
        {
            m_DlgInsHelp.EnableWindow(false);
        }
        CDlgMain::CloseCurDlg(2);
        m_uCurRDlgID = DLG_FRIEND;
        m_DlgEnemyFriend.EnableWindow();
        return;
    }
    else
    {
        m_uCurRDlgID = -1;
        m_DlgEnemyFriend.EnableWindow(false);
        return;
    }
}

void CDlgMain::OnMainGood()
{
    if ( !m_DlgMpcGoods.m_bShow )
    {
        if (m_DlgGroup.m_MenuGroup.m_bShow)
        {
            m_DlgGroup.m_MenuGroup.EnableWindow(false);
        }
        if(m_DlgEmotion.m_bShow)
        {
            m_DlgEmotion.EnableWindow (false);
        }
        m_DlgMpcGoods.m_ChkStones.SetCheckValue(0);
        m_DlgMpcGoods.m_ChkGoods.SetCheckValue(1);
        m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow();
        m_DlgMpcGoods.m_MpcStonesGri.EnableWindow(false);
        m_DlgMpcGoods.EnableWindow() ;
        return ;
    }
    else if ( m_DlgMpcGoods.m_bShow )
    {
        if (m_DlgMpcGoods.m_ChkGoods.GetCheckValue() == 0)
        {
            m_DlgMpcGoods.m_ChkStones.SetCheckValue(0);
            m_DlgMpcGoods.m_ChkGoods.SetCheckValue(1);
            m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow();
            m_DlgMpcGoods.m_MpcStonesGri.EnableWindow(false);
        }
        else
        {
            m_DlgMpcGoods.EnableWindow(false);
        }
        return ;
    }
}

void CDlgMain::OnMainSetup()
{
    if ( !m_DlgSetup.m_bShow )
    {
        CDlgMain::CloseCurDlg(0) ;
        CDlgMain::CloseCurDlg(1);
        CDlgMain::CloseCurDlg(2);
        CDlgMain::CloseNpcDlg();
        CDlgMain::GetParent()->PostMessage(WM_MY_MESSAGE, ON_DLGTASK_CLOSE);
        m_uCurMDlgID = DLG_SETUP ;
        m_DlgSetup.EnableWindow() ;
        return ;
    }
    else if ( m_DlgSetup.m_bShow )
    {
        m_uCurMDlgID = -1 ;
        m_DlgSetup.Exit ();
        return ;
    }
}



void CDlgMain::OnMainChat()
{
    DXPlaySound ( "Sound/DlgMain_Trade.wav" ) ;
    if (g_objHero.IsDead())
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10712)); //死亡状态下不允许交易!
        return;
    }
    else
    {
        CDlgMain::GetParent()->PostMessage( WM_MY_MESSAGE, ON_MAIN_TRADE ) ;
    }
}

void CDlgMain::OnMainGroup()
{
    if ( !m_DlgGroup.m_bShow )
    {
        m_DlgGroup.EnableWindow() ;
        return ;
    }
    else if ( m_DlgGroup.m_bShow )
    {
        m_DlgGroup.EnableWindow( false ) ;
        return ;
    }
}

void CDlgMain::CloseCurDlg( int nPos )
{
    if ( !m_bCreate )
    {
        return ;
    }
    if ( nPos == 0 )
    {
        switch ( m_uCurLDlgID )
        {
        case DLG_NPCBOOTH:
            m_DlgNpcBooth.EnableWindow(false);
            break;
        case DLG_MPCBOOTH:
            m_DlgMpcBooth.EnableWindow(false);
            break;
        case DLG_TRADE:
            m_DlgTrade.EnableWindow( false ) ;
            break ;
        case DLG_TBOARDINFO:
            m_DlgAnnounceInfo.EnableWindow(false);
            break;
        default:
            break ;
        }
        m_uCurLDlgID = -1 ;
    }
    else if ( nPos == 1 )
    {
        CDlgMain::GetParent()->PostMessage(WM_MY_MESSAGE, ON_TALK_LOGCLOSE);
        switch ( m_uCurMDlgID )
        {
        case DLG_IDENTIFY:
            m_DlgIdentify.EnableWindow(false);
            break;
        case DLG_AUCTION:
            m_DlgBidAuction.EnableWindow(false);
            break;
        case DLG_ADDAUCITEM:
            m_DlgAddAucItem.EnableWindow(false);
            break;
        case DLG_AUCPACKAGE:
            m_DlgAucPackage.EnableWindow(false);
            break;
        case DLG_MONSTERNPC:
            m_DlgMonsterNpc.EnableWindow(false);
            break;
        case DLG_MONSTERDEPOT:
            m_DlgMonsterDepot.EnableWindow(false);
            break;
        case DLG_MONSTEREVOLVE:
            m_DlgMonsterEvolve.EnableWindow(false);
            break;
        case DLG_MONSTERIMPROVE:
            m_DlgMonsterImprove.EnableWindow(false);
            break;
        case DLG_MONSTERHATCH:
            m_DlgMonsterHatch.EnableWindow(false);
            break;
        case DLG_TASKUPITEM:
            m_DlgTaskUpItem.EnableWindow(false);
            break;
        case DLG_INSHELP:
            m_DlgInsHelp.EnableWindow(false);
            break;
        case DLG_EMBED:
            m_DlgEmbed.EnableWindow(false);
            break;
        case DLG_IMPROVE:
            m_DlgImprove.EnableWindow(false);
            break;
        case DLG_SETUP:
            m_DlgSetup.EnableWindow( false ) ;
            break ;
        case DLG_MSGSEND:
            m_DlgMsgSend.EnableWindow( false ) ;
            break ;
        case DLG_MSGRECEIVE:
            m_DlgMsgReceive.EnableWindow( false ) ;
            break ;
        default:
            break ;
        }
        m_uCurMDlgID = -1 ;
    }
    else if ( nPos == 2 )
    {
        switch ( m_uCurRDlgID )
        {
        case DLG_NPCEQUIP:
            m_DlgNpcEquip.EnableWindow(false);
            break;
        case DLG_TBOARDUP:
            m_DlgAnnounceUp.EnableWindow(false);
            break;
        case DLG_TBOARDLIST:
            m_DlgAnnounceList.EnableWindow(false);
            break;
        case DLG_TASKVIEW:
            m_DlgPlayerTaskView.EnableWindow(false);
            break;
        case DLG_TASKGET:
            m_DlgPlayerTaskUp.EnableWindow(false);
            break;
        case DLG_TASKLIST:
            m_DlgPlayerTaskList.EnableWindow(false);
            break;
        case DLG_DEPOT:
            m_DlgDepot.EnableWindow( false ) ;
            break ;
        case DLG_NPCBUY:
            m_DlgNpcBuy.EnableWindow( false ) ;
            m_DlgNpcBuy.CloseShop() ;
            if ( m_DlgMpcGoods.m_bShow )
            {
                m_DlgMpcGoods.EnableWindow( false ) ;
            }
            break ;
        case DLG_MONSTER:
            m_DlgMonster.EnableWindow( false ) ;
            break ;
        case DLG_FRIEND:
            m_DlgEnemyFriend.EnableWindow( false ) ;
            break ;
        case DLG_QUERY:
            m_DlgQuery.EnableWindow( false ) ;
            break ;
        case DLG_CONTACT:
            m_DlgKnightage.EnableWindow(false);
            break;
        default:
            break ;
        }
        m_uCurRDlgID = -1 ;
    }
    else if ( nPos == 3 )
    {
        if ( m_DlgGroup.IsWindowEnabled() )
        {
            m_DlgGroup.EnableWindow( false ) ;
        }
    }
    if (m_DlgBigmap.m_bShow)
    {
        m_DlgBigmap.EnableWindow(false);
    }
}



void CDlgMain::OnMainFaction()
{
    if ( !m_DlgKnightage.m_bShow )
    {
        if (m_DlgInsHelp.m_bShow)
        {
            m_DlgInsHelp.EnableWindow(false);
        }
        if (m_DlgSetup.m_bShow)
        {
            m_DlgSetup.EnableWindow(false);
        }
        CDlgMain::GetParent()->PostMessage(WM_MY_MESSAGE, ON_DLGTASK_CLOSE);
        CDlgMain::CloseCurDlg ( 2 ) ;
        m_uCurRDlgID = DLG_CONTACT ;
        m_DlgKnightage.EnableWindow() ;
        return ;
    }
    else if ( m_DlgKnightage.m_bShow )
    {
        m_uCurRDlgID = -1 ;
        m_DlgKnightage.EnableWindow(false) ;
        return ;
    }
}


LRESULT CDlgMain::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case  WM_COMMAND:
        {
            if ( wParam == 1 || wParam == 2 )
            {
                return true ;
            }
        }
        break;
    case WM_DESTROY:
        {
            CDlgMain::WriteRecord () ;
        }
        break;
    default:
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgMain::OpenNpcShop(int nNpcType, OBJID idNpc)
{
    if ( m_DlgTrade.m_bShow )
    {
        return ;
    }
    CDlgMain::CloseCurDlg ( 2 ) ;
    if ( nNpcType != _ROLE_STORAGE_NPC )
    {
        if(m_DlgNpcBuy.OpenShop ( idNpc ) == 3)
        {
            if (!m_DlgMpcGoods.m_bShow )
            {
                m_DlgMpcGoods.EnableWindow();
                m_DlgMpcGoods.m_ChkStones.SetCheckValue(1);
                m_DlgMpcGoods.m_ChkGoods.SetCheckValue(0);
                m_DlgMpcGoods.m_MpcStonesGri.EnableWindow();
                m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow(false);
            }
        }
        else
        {
            if ( !m_DlgMpcGoods.m_bShow )
            {
                m_DlgMpcGoods.EnableWindow() ;
                m_DlgMpcGoods.m_ChkStones.SetCheckValue(0);
                m_DlgMpcGoods.m_ChkGoods.SetCheckValue(1);
                m_DlgMpcGoods.m_MpcStonesGri.EnableWindow(false);
                m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow();
            }
        }
        m_DlgNpcBuy.EnableWindow() ;
        m_uCurRDlgID = DLG_NPCBUY ;
    }
}

void CDlgMain::OpenDepot(int nNpcType, OBJID idNpc)
{
    if ( m_DlgTrade.m_bShow )
    {
        return ;
    }
    CDlgMain::CloseCurDlg( 2 ) ;
    m_DlgDepot.OpenDepot ( idNpc ) ;
    m_DlgDepot.EnableWindow() ;
    if ( !m_DlgMpcGoods.m_bShow )
    {
        m_DlgMpcGoods.EnableWindow() ;
    }
    m_uCurRDlgID = DLG_DEPOT ;
}

void CDlgMain::OnShellEnter()
{
    m_DlgTalk.m_TalkDataCob.SetFocus () ;
}

void CDlgMain::PlayItemQUse(int nGridIndex)
{
    CAni* pActAni = g_objGameDataSet.GetDataAni ( "ani/effect.ani",
                    "ItemUse",
                    EXIGENCE_IMMEDIATE ) ;
    if ( pActAni != NULL )
    {
        CRect Drect, rect ;
        CDlgMain::GetWindowRect ( Drect ) ;
        m_DlgTalk.m_GoodBox.GetWindowRect ( rect ) ;
        m_ActPnt.x = rect.left - Drect.left + m_DlgPnt.x + ( nGridIndex - 1 ) * 41 - 60;
        m_ActPnt.y = m_DlgPnt.y + 30;
        m_bItemAct = true ;
        m_dwLastActTime = 0 ;
        m_nActAmount = pActAni->GetFrameAmount () ;
    }
}

void CDlgMain::OnMainMagic()
{
    // Open Skill dialog
    if(!m_DlgQuery.m_bShow)
    {
        if (m_DlgInsHelp.m_bShow)
        {
            m_DlgInsHelp.EnableWindow(false);
        }
        CDlgMain::GetParent()->PostMessage(WM_MY_MESSAGE, ON_DLGTASK_CLOSE);
        CDlgMain::CloseCurDlg( 2 );
        m_uCurRDlgID = DLG_QUERY;
        m_DlgQuery.PopupCurChk();
        m_DlgQuery.m_ChkSkill.SetCheckValue(1);
        m_DlgQuery.m_btCurChildDlgID = 2;
        CDlgMain::CloseCurDlg(1);
        m_DlgQuery.EnableWindow(true);
    }
    else
    {
        if(m_DlgQuery.m_btCurChildDlgID == 2)
        {
            if (m_DlgQuery.m_DlgSkill.m_nSkillListMode == 1)
            {
                m_DlgQuery.m_DlgSkill.SetMagicSkill();
            }
            else
            {
                CDlgMain::CloseCurDlg(2);
                m_uCurRDlgID = -1;
                m_DlgQuery.EnableWindow(false);
            }
            return;
        }
        else
        {
            m_DlgQuery.HideCurDlg( m_DlgQuery.m_btCurChildDlgID);
            m_DlgQuery.PopupCurChk();
            m_DlgQuery.m_btCurChildDlgID = 2;
            m_DlgQuery.m_ChkSkill.SetCheckValue(1);
            m_DlgQuery.ShowCurDlg(2);
        }
    }
}

void CDlgMain::WriteRecord()
{
    if ( g_objHero.GetName () == NULL || strlen ( g_objHero.GetName () ) == 0 )
    {
        return ;
    }
    if ( m_bReacord )
    {
        return ;
    }
    char strAppName[32] ;
    sprintf ( strAppName, "%sRecord", g_objHero.GetName () ) ;
    // begin test the account name
    char* pstrTemp = strchr ( strAppName, '[' ) ;
    if ( pstrTemp != NULL )
    {
        strncpy ( pstrTemp, "(", 1 ) ;
        pstrTemp = NULL ;
    }
    pstrTemp = strchr ( strAppName, ']' ) ;
    if ( pstrTemp != NULL )
    {
        strncpy ( pstrTemp, ")", 1 ) ;
        pstrTemp = NULL ;
    }
    // end test the account name
    /*/	itoa ( m_MapChk.GetCheckValue (), strMode, 10 ) ;
    	WritePrivateProfileString ( strAppName, "MapModeRecord", strMode, m_strOwnerIni ) ;
    //	itoa ( m_TalkChk.GetCheckValue (), strMode, 10 ) ;
    	WritePrivateProfileString ( strAppName, "TalkModeRecord", strMode, m_strOwnerIni ) ;*/
    char strTypeID[32] ;
    char strKeyName[16] ;
    int i = 0;
    for ( i = 1; i <= GRID_ICONQUICK_NUM; i++ )
    {
        sprintf ( strKeyName, "QItem%dRecord", i ) ;
        if ( m_DlgTalk.m_GoodBox.GetGridAble ( CPoint ( i, 1 ) ) )
        {
            switch ( m_DlgTalk.m_GoodBox.GetIconType ( i - 1 ) )
            {
            case ICON_TYPE_QUSE:
                itoa ( m_DlgTalk.m_GoodBox.GetIconTypeID ( i - 1 ) * 10 + ICON_TYPE_QUSE, strTypeID, 10 ) ;
                WritePrivateProfileString ( strAppName, strKeyName, strTypeID, m_strOwnerIni ) ;
                break ;
            }
        }
        else
        {
            WritePrivateProfileString ( strAppName, strKeyName, "", m_strOwnerIni ) ;
        }
    }
    for ( i = 1; i <= 8; i++ )
    {
        sprintf ( strKeyName, "SkillHotKeyF%d", i ) ;
        if (m_DlgQuery.m_DlgSkill.m_DlgSkillHotKey.m_HotKeyIdType[i-1] > 0)
        {
            itoa (m_DlgQuery.m_DlgSkill.m_DlgSkillHotKey.m_HotKeyIdType[i-1], strTypeID , 10) ;
            WritePrivateProfileString ( strAppName, strKeyName, strTypeID, m_strOwnerIni ) ;
        }
        else
        {
            WritePrivateProfileString ( strAppName, strKeyName, "", m_strOwnerIni ) ;
        }
    }
    // write the chat color record
    char strInt[8] ;
    int nValue = m_DlgSetup.m_DlgMusic.m_SliderMusic.GetSliderValue () ;
    if (m_DlgSetup.m_DlgMusic.m_MusicChk.GetCheckValue() == 0)
    {
        nValue = m_DlgSetup.m_DlgMusic.m_SliderMusic.GetSliderValue() * 10;
    }
    else
    {
        nValue = m_DlgSetup.m_DlgMusic.m_SliderMusic.GetSliderValue() * 10 + 1;
    }
    // write record
    itoa ( nValue, strInt, 10 ) ;
    WritePrivateProfileString( strAppName, "MusicRecord", strInt, m_strOwnerIni ) ;
    nValue = m_DlgSetup.m_DlgMusic.m_SliderSound.GetSliderValue () ;
    if (m_DlgSetup.m_DlgMusic.m_SoundChk.GetCheckValue() == 0)
    {
        nValue = m_DlgSetup.m_DlgMusic.m_SliderSound.GetSliderValue() * 10;
    }
    else
    {
        nValue = m_DlgSetup.m_DlgMusic.m_SliderSound.GetSliderValue() * 10 + 1;
    }
    // write record
    itoa ( nValue, strInt, 10 ) ;
    WritePrivateProfileString( strAppName, "EffectRecord", strInt, m_strOwnerIni ) ;
    for ( i = 0; i < 5; i++ )
    {
        strInt[i] = m_DlgSetup.m_DlgSChat.m_nColorLog [i] + 48;
    }
    strInt[5] = '\0' ;
    WritePrivateProfileString ( strAppName, "ChatColorRecord", strInt, m_strOwnerIni ) ;
    int nAmount = g_objGameMsg.GetBlackNameAmount();
    if (nAmount > 0)
    {
        char strRecord[7*17];
        char strTemp[16] ;
        itoa ( nAmount, strTemp, 10 ) ;
        strcpy ( strRecord, strTemp ) ;
        strcat ( strRecord, "|" ) ;
        for( i = 0; i < __min(6, nAmount); i++)
        {
            strcat(strRecord, g_objGameMsg.GetBlackNameByIndex(i));
            strcat(strRecord, "|");
        }
        WritePrivateProfileString ( strAppName, "BlackNameRecord", strRecord, m_strOwnerIni ) ;
    }
    else
    {
        WritePrivateProfileString ( strAppName, "BlackNameRecord", "", m_strOwnerIni ) ;
    }
    char strMode[16];
    itoa ( CRole::s_nShadowType, strMode, 10 );
    WritePrivateProfileString ( strAppName, "ShadowRecord", strMode, m_strOwnerIni ) ;
    itoa ( m_DlgSetup.m_DlgGraph.m_ChkRoleName.GetCheckValue (), strInt, 10 ) ;
    WritePrivateProfileString ( strAppName, "ShiftRecord", strInt, m_strOwnerIni ) ;
    // Begin write the talklistrecord to the ini file
    int nCount = m_DlgTalk.m_DlgListBox.m_ListBox.GetCount () ;
    if ( nCount > 0 )
    {
        char strRecord[17*18] ;
        char strTemp[16] ;
        itoa ( nCount, strTemp, 10 ) ;
        strcpy ( strRecord, strTemp ) ;
        strcat ( strRecord, "|" ) ;
        for ( int i = 0; i < __min(16, nCount); i++ )
        {
            m_DlgTalk.m_DlgListBox.m_ListBox.GetText ( i, strTemp ) ;
            strcat ( strRecord, strTemp ) ;
            strcat ( strRecord, "|" ) ;
        }
        WritePrivateProfileString ( strAppName, "TalkListRecord", strRecord, m_strOwnerIni ) ;
    }
    else
    {
        WritePrivateProfileString ( strAppName, "TalkListRecord", "", m_strOwnerIni ) ;
    }
    // End write the talklistrecord to the ini file
    int ChannelOpen[5];
    ChannelOpen[0] = (int)m_DlgSetup.m_DlgSChat.m_ChkBan.GetCheckValue();
    ChannelOpen[1] = (int)m_DlgSetup.m_DlgSChat.m_ChkFriend.GetCheckValue();
    ChannelOpen[2] = (int)m_DlgSetup.m_DlgSChat.m_ChkPub.GetCheckValue();
    ChannelOpen[3] = (int)m_DlgSetup.m_DlgSChat.m_ChkSec.GetCheckValue();
    ChannelOpen[4] = (int)m_DlgSetup.m_DlgSChat.m_ChkTeam.GetCheckValue();
    sprintf(strInt, "%d|%d|%d|%d|%d", ChannelOpen[0], ChannelOpen[1], ChannelOpen[2], ChannelOpen[3], ChannelOpen[4]);
    WritePrivateProfileString(strAppName, "TalkChannelRecord", strInt, m_strOwnerIni);
    /*	char strInt[8] ;
    int nValue = m_DlgSetup.m_SetupSliMusic.GetSliderValue () ;

    // write record
    itoa ( nValue, strInt, 10 ) ;
    WritePrivateProfileString( strAppName, "MusicRecord", strInt, m_strOwnerIni ) ;

    nValue = m_DlgSetup.m_SetupSliEffect.GetSliderValue () ;
    // write record
    itoa ( nValue, strInt, 10 ) ;
    WritePrivateProfileString( strAppName, "EffectRecord", strInt, m_strOwnerIni ) ;

    // write the Faction word record
    nValue = 0;
    char strText [256] ;
    strcpy ( strText, g_objHero.GetSyndicateAnnounce () ) ;
    if ( strcmp ( strText, "NULL" ) && strText[0] != '\0' )
    {
    	for ( i = 0; i < strlen ( strText ); i++ )
    		nValue += abs ( strText[i] ) ;
    	itoa ( nValue, strInt, 10 ) ;
    	WritePrivateProfileString ( strAppName, "FacWordRecord", strInt, m_strOwnerIni ) ;
    }

    // write the shift record
    itoa ( m_DlgSetup.m_SetupChkNameopen.GetCheckValue (), strInt, 10 ) ;
    WritePrivateProfileString ( strAppName, "ShiftRecord", strInt, m_strOwnerIni ) ;


    // write the interface mode record
    itoa ( m_DlgSetup.m_nInterface, strInt, 10 ) ;
    WritePrivateProfileString ( "Interface", "InterfaceRecord", strInt, g_strSetupIni ) ;*/
    m_bReacord = true;
}

void CDlgMain::SetRecord( const char* strIniFile )
{
    if ( strIniFile != NULL )
    {
        strcpy ( m_strOwnerIni, strIniFile ) ;
    }
    else
    {
        return ;
    }
    char strAppName[32] ;
    sprintf ( strAppName, "%sRecord", g_objHero.GetName () ) ;
    // begin test the account name
    char* pstrTemp = strchr ( strAppName, '[' ) ;
    if ( pstrTemp != NULL )
    {
        strncpy ( pstrTemp, "(", 1 ) ;
        pstrTemp = NULL ;
    }
    pstrTemp = strchr ( strAppName, ']' ) ;
    if ( pstrTemp != NULL )
    {
        strncpy ( pstrTemp, ")", 1 ) ;
        pstrTemp = NULL ;
    }
    // end test the account name
    // the user already have record read the record and set
    int nReturn = GetPrivateProfileInt ( strAppName, "RecordAmount", -1, m_strOwnerIni ) ;
    if ( nReturn != -1 )
    {
        // set the music and effect
        int nMusic = GetPrivateProfileInt ( strAppName, "MusicRecord", -1, m_strOwnerIni ) ;
        if ( nMusic != -1 )
        {
            m_DlgSetup.m_DlgMusic.m_SliderMusic.SetSliderValue ( nMusic / 10) ;
            if (nMusic % 10 == 1)
            {
                m_DlgSetup.m_DlgMusic.m_MusicChk.SetCheckValue(1);
                m_DlgSetup.m_DlgMusic.m_SliderMusic.EnableWindow();
                m_DlgSetup.m_DlgMusic.m_MusicLeftBtn.EnableWindow ();
                m_DlgSetup.m_DlgMusic.m_MusicRightBtn.EnableWindow ();
                DXSetMusicVolume ( nMusic / 10 ) ;
                g_objHero.SetMusicVolume(nMusic / 10);
            }
            else
            {
                m_DlgSetup.m_DlgMusic.m_MusicChk.SetCheckValue(0);
                m_DlgSetup.m_DlgMusic.m_SliderMusic.EnableWindow(false);
                m_DlgSetup.m_DlgMusic.m_MusicLeftBtn.EnableWindow (false);
                m_DlgSetup.m_DlgMusic.m_MusicRightBtn.EnableWindow (false);
                DXSetMusicVolume ( 0 ) ;
                g_objHero.SetMusicVolume(0);
            }
        }
        else
        {
            m_DlgSetup.m_DlgMusic.m_SliderMusic.EnableWindow();
            m_DlgSetup.m_DlgMusic.m_MusicLeftBtn.EnableWindow ();
            m_DlgSetup.m_DlgMusic.m_MusicRightBtn.EnableWindow ();
            m_DlgSetup.m_DlgMusic.m_SliderMusic.SetSliderValue ( 75 ) ;
            m_DlgSetup.m_DlgMusic.m_MusicChk.SetCheckValue(1);
            DXSetMusicVolume ( 75) ;
            g_objHero.SetMusicVolume(75);
        }
        nMusic = GetPrivateProfileInt ( strAppName, "EffectRecord", -1, m_strOwnerIni ) ;
        if ( nMusic != -1 )
        {
            m_DlgSetup.m_DlgMusic.m_SliderSound.SetSliderValue ( nMusic / 10 ) ;
            if (nMusic % 10 == 1)
            {
                m_DlgSetup.m_DlgMusic.m_SoundChk.SetCheckValue(1);
                m_DlgSetup.m_DlgMusic.m_SliderSound.EnableWindow();
                m_DlgSetup.m_DlgMusic.m_SoundLeftBtn.EnableWindow ();
                m_DlgSetup.m_DlgMusic.m_SoundRightBtn.EnableWindow ();
                DXSetSoundVolume ( nMusic / 10 ) ;
            }
            else
            {
                m_DlgSetup.m_DlgMusic.m_SoundChk.SetCheckValue(0);
                m_DlgSetup.m_DlgMusic.m_SliderSound.EnableWindow(false);
                m_DlgSetup.m_DlgMusic.m_SoundLeftBtn.EnableWindow (false);
                m_DlgSetup.m_DlgMusic.m_SoundRightBtn.EnableWindow (false);
                DXSetSoundVolume ( 0 ) ;
            }
        }
        else
        {
            m_DlgSetup.m_DlgMusic.m_SliderSound.EnableWindow();
            m_DlgSetup.m_DlgMusic.m_SoundLeftBtn.EnableWindow ();
            m_DlgSetup.m_DlgMusic.m_SoundRightBtn.EnableWindow ();
            m_DlgSetup.m_DlgMusic.m_SliderSound.SetSliderValue ( 75 ) ;
            m_DlgSetup.m_DlgMusic.m_SoundChk.SetCheckValue(1);
            DXSetSoundVolume ( 75 ) ;
        }
        m_DlgSetup.m_DlgMusic.SetSliderStatic();
        // set the shadow record
        nReturn = GetPrivateProfileInt ( strAppName, "ShadowRecord", -1, m_strOwnerIni ) ;
        if ( nReturn != -1 )
        {
            CRole::s_nShadowType = nReturn ;
            if (nReturn == CRole::SHADOW_REAL)
            {
                m_DlgSetup.m_DlgGraph.m_ChkShadow.SetCheckValue(1);
            }
            else if(nReturn == CRole::SHADOW_NONE)
            {
                m_DlgSetup.m_DlgGraph.m_ChkShadow.SetCheckValue(0);
            }
        }
        nReturn = GetPrivateProfileInt ( strAppName, "ShiftRecord", -1, m_strOwnerIni ) ;
        if ( nReturn != -1 )
        {
            CDlgMain::GetParent()->PostMessage( WM_MY_MESSAGE, ON_SETUP_SHIFT, nReturn) ;
            if (nReturn)
            {
                m_DlgSetup.m_DlgGraph.m_ChkRoleName.SetCheckValue(1);
            }
            else
            {
                m_DlgSetup.m_DlgGraph.m_ChkRoleName.SetCheckValue(0);
            }
        }
        else
        {
            m_DlgSetup.m_DlgGraph.m_ChkRoleName.SetCheckValue(0);
        }
        // set the chat color recrod
        char strColor[16] ;
        GetPrivateProfileString ( strAppName, "ChatColorRecord", "99999", strColor, 16, m_strOwnerIni ) ;
        if ( strlen(strColor) > 0 )
        {
            int nTemp = 0;
            char strTemp[16] = "";
            // Temp code
            nTemp = strColor[0] - 48;
            if (nTemp < 0 || nTemp > 9)
            {
                nTemp = 9;
            }
            m_DlgSetup.m_DlgSChat.m_nColorLog [0] = nTemp ;
            g_objGameMsg.SetChannelColor ( _TXTATR_TALK, m_DlgSetup.m_DlgSChat.m_dwColor [nTemp] ) ;
            sprintf ( strTemp, "Image36%d", nTemp ) ;
            m_DlgSetup.m_DlgSChat.m_ImgPub.InsertImage ( strTemp ) ;
            nTemp = strColor[1] - 48;
            if (nTemp < 0 || nTemp > 9)
            {
                nTemp = 9;
            }
            m_DlgSetup.m_DlgSChat.m_nColorLog [1] = nTemp ;
            g_objGameMsg.SetChannelColor ( _TXTATR_FRIEND, m_DlgSetup.m_DlgSChat.m_dwColor [nTemp] ) ;
            sprintf ( strTemp, "Image36%d", nTemp ) ;
            m_DlgSetup.m_DlgSChat.m_ImgFriend.InsertImage ( strTemp ) ;
            nTemp = strColor[2] - 48;
            if (nTemp < 0 || nTemp > 9)
            {
                nTemp = 9;
            }
            m_DlgSetup.m_DlgSChat.m_nColorLog [2] = nTemp ;
            g_objGameMsg.SetChannelColor ( _TXTATR_SYNDICATE, m_DlgSetup.m_DlgSChat.m_dwColor [nTemp] ) ;
            sprintf ( strTemp, "Image36%d", nTemp ) ;
            m_DlgSetup.m_DlgSChat.m_ImgBan.InsertImage ( strTemp ) ;
            nTemp = strColor[3] - 48;
            if (nTemp < 0 || nTemp > 9)
            {
                nTemp = 9;
            }
            m_DlgSetup.m_DlgSChat.m_nColorLog [3] = nTemp ;
            g_objGameMsg.SetChannelColor ( _TXTATR_TEAM, m_DlgSetup.m_DlgSChat.m_dwColor [nTemp] ) ;
            sprintf ( strTemp, "Image36%d", nTemp ) ;
            m_DlgSetup.m_DlgSChat.m_ImgTeam.InsertImage ( strTemp ) ;
            nTemp = strColor[4] - 48;
            if (nTemp < 0 || nTemp > 9)
            {
                nTemp = 9;
            }
            m_DlgSetup.m_DlgSChat.m_nColorLog [4] = nTemp ;
            g_objGameMsg.SetChannelColor ( _TXTATR_PRIVATE, m_DlgSetup.m_DlgSChat.m_dwColor [nTemp] ) ;
            sprintf ( strTemp, "Image36%d", nTemp ) ;
            m_DlgSetup.m_DlgSChat.m_ImgSec.InsertImage ( strTemp ) ;
        }
        else
        {
            g_objGameMsg.SetChannelColor ( _TXTATR_TALK, m_DlgSetup.m_DlgSChat.m_dwColor [3] ) ;
            g_objGameMsg.SetChannelColor ( _TXTATR_FRIEND, m_DlgSetup.m_DlgSChat.m_dwColor [3] ) ;
            g_objGameMsg.SetChannelColor ( _TXTATR_SYNDICATE, m_DlgSetup.m_DlgSChat.m_dwColor [3] ) ;
            g_objGameMsg.SetChannelColor ( _TXTATR_TEAM, m_DlgSetup.m_DlgSChat.m_dwColor [3] ) ;
            g_objGameMsg.SetChannelColor ( _TXTATR_PRIVATE, m_DlgSetup.m_DlgSChat.m_dwColor [3] ) ;
        }
        /*
        // set the interface mode record
        nMode = GetPrivateProfileInt ( "Interface", "InterfaceRecord", -1, g_strSetupIni ) ;
        if ( nMode != -1 )
        {
        	m_DlgSetup.m_nInterface = nMode ;
        	char strTemp[32] ;
        	sprintf ( strTemp, "Button101%d", nMode + 1 ) ;
        	m_DlgSetup.m_SetupBtnMode.ChangeImage ( strTemp ) ;
        	m_DlgSetup.OnSetupBtnMode () ;
        }*/
        // Set the pk mode
        g_objHero.SetPkMode ( PKMODE_MONSTERONLY ) ;
        // set the check record
        int nItemTypeID ;
        char strKeyName[16] ;
        // Set the Skill HotKey
        for (int n = 1; n <= 8; n++)
        {
            sprintf(strKeyName, "SkillHotKeyF%d", n);
            m_DlgQuery.m_DlgSkill.m_DlgSkillHotKey.m_HotKeyIdType[n-1] = GetPrivateProfileInt( strAppName, strKeyName, -1, m_strOwnerIni) ;
        }
        // set the quick item record
        m_DlgTalk.m_GoodBox.RemoveIcon(0, 0, true);
        for ( int i = 1; i <= GRID_ICONQUICK_NUM; i++ )
        {
            sprintf ( strKeyName, "QItem%dRecord", i ) ;
            nItemTypeID = GetPrivateProfileInt ( strAppName, strKeyName, -1, m_strOwnerIni ) ;
            if ( nItemTypeID != -1 )
            {
                switch ( nItemTypeID % 10 )
                {
                case ICON_TYPE_QUSE:
                    {
                        bool bLoad = false;
                        for ( int j = 0; j < g_objHero.GetItemAmount (); j++ )
                        {
                            CItem* pItem = g_objHero.GetItemByIndex ( j ) ;
                            if ( pItem != NULL )
                            {
                                if ( pItem->GetTypeID () == nItemTypeID / 10 )
                                {
                                    m_DlgTalk.m_GoodBox.InsertIcon( 1, i, nItemTypeID / 10, 0, ICON_TYPE_QUSE ) ;
                                    bLoad = true;
                                    break ;
                                }
                            }
                        }
                        if (!bLoad)
                        {
                            for ( int j = 0; j < g_objHero.GetItemAmount (PACKAGE_SOUL); j++ )
                            {
                                CItem* pItem = g_objHero.GetItemByIndex ( j, PACKAGE_SOUL ) ;
                                if ( pItem != NULL )
                                {
                                    if ( pItem->GetTypeID () == nItemTypeID / 10 )
                                    {
                                        m_DlgTalk.m_GoodBox.InsertIcon( 1, i, nItemTypeID / 10, 0, ICON_TYPE_QUSE ) ;
                                        bLoad = true;
                                        break ;
                                    }
                                }
                            }
                        }
                    }
                    break ;
                }
            }
        }
        char strBlackName[7*17];
        GetPrivateProfileString ( strAppName, "BlackNameRecord", "", strBlackName, 7 * 17, m_strOwnerIni ) ;
        if (strBlackName[0] != NULL)
        {
            char* pTemp ;
            char* pLast ;
            char strTemp[16] ;
            int nAmount = 0 ;
            pTemp = strchr ( strBlackName, '|' ) ;
            strncpy ( strTemp, strBlackName, pTemp - strBlackName ) ;
            strTemp[pTemp - strBlackName] = NULL ;
            nAmount = atoi ( strTemp ) ;
            if (nAmount > 0)
            {
                g_objGameMsg.DestroyBlackName();
            }
            for(int i = 0; i < nAmount; i++)
            {
                pLast = pTemp + 1 ;
                pTemp = strchr ( pTemp + 1, '|' ) ;
                strncpy ( strTemp, pLast, pTemp - pLast ) ;
                strTemp[pTemp - pLast] = NULL ;
                g_objGameMsg.AddBlackName(strTemp);
            }
        }
        // set the talk list record
        char strRecord[17*18] ;
        GetPrivateProfileString ( strAppName, "TalkListRecord", "", strRecord, 17 * 18, m_strOwnerIni ) ;
        if ( strRecord[0] != NULL && m_DlgTalk.m_DlgListBox.m_ListBox.GetCount () == 0 )
        {
            char* pTemp ;
            char* pLast ;
            char strTemp[16] ;
            int nAmount = 0 ;
            pTemp = strchr ( strRecord, '|' ) ;
            strncpy ( strTemp, strRecord, pTemp - strRecord ) ;
            strTemp[pTemp - strRecord] = NULL ;
            nAmount = atoi ( strTemp ) ;
            for ( int i = 0; i < __min(16, nAmount); i++ )
            {
                pLast = pTemp + 1 ;
                pTemp = strchr ( pTemp + 1, '|' ) ;
                strncpy ( strTemp, pLast, pTemp - pLast ) ;
                strTemp[pTemp - pLast] = NULL ;
                strcpy ( m_DlgTalk.m_strTargetName[m_DlgTalk.m_nTargetAmount++], strTemp ) ;
                m_DlgTalk.m_DlgListBox.m_ListBox.AddString ( strTemp ) ;
            }
        }
        char strChannel[16];
        GetPrivateProfileString (strAppName, "TalkChannelRecord", "", strChannel, 16, m_strOwnerIni);
        if (strlen(strChannel) > 0)
        {
            int bChannel[5] = {0};
            int bReult = sscanf(strChannel, "%d|%d|%d|%d|%d", &bChannel[0], &bChannel[1], &bChannel[2], &bChannel[3], &bChannel[4]);
            m_DlgSetup.m_DlgSChat.m_ChkBan.SetCheckValue(bChannel[0]);
            g_objGameMsg.SetChannelStatus(_TXTATR_SYNDICATE, bChannel[0]);
            m_DlgSetup.m_DlgSChat.m_ChkFriend.SetCheckValue(bChannel[1]);
            g_objGameMsg.SetChannelStatus(_TXTATR_FRIEND, bChannel[1]);
            m_DlgSetup.m_DlgSChat.m_ChkPub.SetCheckValue(bChannel[2]);
            g_objGameMsg.SetChannelStatus(_TXTATR_TALK, bChannel[2]);
            m_DlgSetup.m_DlgSChat.m_ChkSec.SetCheckValue(bChannel[3]);
            g_objGameMsg.SetPrivateChannelStatusExceptFriend(bChannel[3]);
            m_DlgSetup.m_DlgSChat.m_ChkTeam.SetCheckValue(bChannel[4]);
            g_objGameMsg.SetChannelStatus(_TXTATR_TEAM, bChannel[4]);
        }
    }
    // the user has not record, write a empty record to the .ini file
    else
    {
        char strDir[128] ;
        strcpy( strDir, m_strOwnerIni ) ;
        char* pStrLast = strchr( m_strOwnerIni, '/' ) ;
        pStrLast = strchr( pStrLast + 1, '/' ) ;
        strDir[pStrLast - m_strOwnerIni] = '\0' ;
        ::CreateDirectory( strDir, NULL ) ;
        strcpy( strDir, m_strOwnerIni ) ;
        pStrLast = strrchr( m_strOwnerIni, '/' ) ;
        strDir[pStrLast - m_strOwnerIni] = '\0' ;
        ::CreateDirectory( strDir, NULL ) ;
        ::WritePrivateProfileStruct( strAppName,
                                     NULL,
                                     NULL,
                                     0,
                                     m_strOwnerIni ) ;
        ::WritePrivateProfileSection ( strAppName,
                                       " RecordAmount=1\0 MusicRecord=\0 EffectRecord=\0 PkModeRecord=\0 RunModeRecord=\0 MapModeRecord=\0 TalkModeRecord=\0 QItem1Record=\0 QItem2Record=\0 QItem3Record=\0 QItem4Record=\0 QItem5Record=\0 QItem6Record=\0 QItem7Record=\0 QItem8Record=\0 QItem9Record=\0 QItem10Record=\0 SkillHotKeyF1=\0 SkillHotKeyF2=\0 SkillHotKeyF3=\0 SkillHotKeyF4=\0 SkillHotKeyF5=\0 SkillHotKeyF6=\0 SkillHotKeyF7=\0 SkillHotKeyF8=\0 TalkListRecord=\0 ShadowRecord=\0 LineRecord=\0 LogRecord=\0 FacWordRecord=\0 ShiftRecord=\0 ChatColorRecord=\0 ChatChannelRecord=\0",
                                       m_strOwnerIni ) ;
        //设置默认声音大小
        m_DlgSetup.m_DlgMusic.m_SliderSound.SetSliderValue ( 75 ) ;
        m_DlgSetup.m_DlgMusic.m_SliderSound.EnableWindow();
        m_DlgSetup.m_DlgMusic.m_SoundLeftBtn.EnableWindow ();
        m_DlgSetup.m_DlgMusic.m_SoundRightBtn.EnableWindow ();
        m_DlgSetup.m_DlgMusic.m_SoundChk.SetCheckValue(1);
        DXSetSoundVolume ( 75 ) ;
        m_DlgSetup.m_DlgMusic.m_SliderMusic.SetSliderValue ( 75 ) ;
        m_DlgSetup.m_DlgMusic.m_SliderMusic.EnableWindow();
        m_DlgSetup.m_DlgMusic.m_MusicLeftBtn.EnableWindow ();
        m_DlgSetup.m_DlgMusic.m_MusicRightBtn.EnableWindow ();
        m_DlgSetup.m_DlgMusic.m_MusicChk.SetCheckValue(1);
        DXSetMusicVolume ( 75 ) ;
        g_objHero.SetMusicVolume(75);
        m_DlgSetup.m_DlgMusic.SetSliderStatic();
        g_objHero.SetPkMode ( PKMODE_MONSTERONLY ) ;
    }
}

void CDlgMain::OnXpFull()
{
    if (m_DlgXp.m_btBoothType)
    {
        return;
    }
    int nAmount = g_objHero.GetXpSkillAmount() ;
    if ( nAmount > 0 )
    {
        g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(10342));//您的XP技能已经满了，可以使用XP技能了。
        m_DlgXp.MoveWindow ( _SCR_WIDTH - nAmount * 50 - 55,
                             _SCR_HEIGHT - 115,
                             nAmount * 50,
                             50 ) ;
        m_DlgXp.InitXpDialog ( ) ;
    }
}

void CDlgMain::PlayXp()
{
    m_bXpAct = true ;
    m_dwLastXpTime = 0 ;
}

void CDlgMain::HeroDie()
{
    m_DlgMonster.ResetMonsterDlg();
    CRect rect;
    m_DlgXp.GetWindowRect(rect);
    m_DlgXp.MoveWindow ( _SCR_WIDTH  - 100,
                         _SCR_HEIGHT - 100,
                         rect.Width(),
                         rect.Height() ) ;
    m_DlgXp.SetReborn (true) ;
}

void CDlgMain::HeroReborn()
{
    m_DlgXp.SetReborn (false) ;
}

void CDlgMain::OpenImprove(int nType)
{
    if (!m_DlgImprove.m_bShow)
    {
        CloseCurDlg(1);
        m_uCurMDlgID = DLG_IMPROVE;
        m_DlgImprove.SetImproveType(nType);
        m_DlgImprove.EnableWindow(true);
        if(!m_DlgMpcGoods.m_bShow)
        {
            m_DlgMpcGoods.EnableWindow();
        }
    }
}

void CDlgMain::OpenNpcFace()
{
}

void CDlgMain::OpenEmbed()
{
    if (m_DlgEmbed.m_bShow)
    {
        return;
    }
    CDlgMain::CloseCurDlg(1);
    m_DlgEmbed.EnableWindow(true);
    m_uCurMDlgID = DLG_EMBED;
}

void CDlgMain::OpenBooth(BOOL bOwner)
{
    if ( bOwner )
    {
        if ( m_DlgMpcBooth.m_bShow || m_DlgTrade.m_bShow )
        {
            return ;
        }
        CDlgMain::CloseCurDlg () ;
        m_DlgMpcBooth.EnableWindow(true) ;
        m_uCurLDlgID = DLG_MPCBOOTH;
        if( !m_DlgMpcGoods.m_bShow)
        {
            m_DlgMpcGoods.EnableWindow();
        }
        m_DlgXp.SetBooth(true);
    }
    else
    {
        CDlgMain::CloseCurDlg () ;
        m_DlgNpcBooth.EnableWindow(true);
        m_uCurLDlgID = DLG_NPCBOOTH ;
    }
}

void CDlgMain::OpenEffigy()
{
}

void CDlgMain::OpenLay()
{
}

void CDlgMain::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMain::OpenDice(BOOL bOpen)
{
    if ( bOpen )
    {
        m_DlgDice.ShowWindow ( SW_SHOW ) ;
    }
    else
    {
        m_DlgDice.ResetDice () ;
        m_DlgDice.ShowWindow ( SW_HIDE ) ;
        if ( m_DlgDiceList.m_bShow )
        {
            m_DlgDiceList.ShowWindow ( SW_HIDE ) ;
        }
    }
}

void CDlgMain::OnBtnUpHide()
{
    DXPlaySound("Sound/Dlg_Flex.wav");
    m_bUpHide = !m_bUpHide ;
    if ( m_bUpHide )
        // Disable the up control
    {
        m_FriendBtn.EnableWindow( false ) ;
        m_GroupBtn.EnableWindow( false ) ;
        m_ChatBtn.EnableWindow( false ) ;
        // Move the button
        CRect rect, btnRect ;
        CDlgMain::GetWindowRect( rect ) ;
        m_BtnUpHide.GetWindowRect( btnRect ) ;
        m_BtnUpHide.MoveWindow( btnRect.left - rect.left,
                                btnRect.top - rect.top + 110,
                                btnRect.Width(),
                                btnRect.Height() ) ;
        m_BtnUpHide.Init( rect.left, rect.top, "Button4151" ) ;
        ReSetDlgRgn(m_bUpHide);
    }
    // Enable the up control
    else
    {
        m_FriendBtn.EnableWindow() ;
        m_ChatBtn.EnableWindow() ;
        m_GroupBtn.EnableWindow() ;
        // Move the button
        CRect rect, btnRect ;
        CDlgMain::GetWindowRect( rect ) ;
        m_BtnUpHide.GetWindowRect( btnRect ) ;
        m_BtnUpHide.MoveWindow( btnRect.left - rect.left,
                                btnRect.top - rect.top - 110,
                                btnRect.Width(),
                                btnRect.Height() ) ;
        m_BtnUpHide.Init( rect.left, rect.top, "Button415") ;
        ReSetDlgRgn(m_bUpHide);
    }
}

void CDlgMain::OnLButtonUp(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMain::OnRButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnRButtonDown(nFlags, point);
}

void CDlgMain::OnRButtonUp(UINT nFlags, CPoint point)
{
    CDialog::OnRButtonUp(nFlags, point);
}

void CDlgMain::OnMainMonster()
{
    // TODO: Add your control notification handler code here
    if ( !m_DlgMonster.m_bShow )
    {
        if (m_DlgInsHelp.m_bShow)
        {
            m_DlgInsHelp.EnableWindow(false);
        }
        CDlgMain::GetParent()->PostMessage(WM_MY_MESSAGE, ON_DLGTASK_CLOSE);
        CDlgMain::CloseCurDlg( 2 ) ;
        m_uCurRDlgID = DLG_MONSTER ;
        m_DlgMonster.EnableWindow() ;
        return ;
    }
    else if ( m_DlgMonster.m_bShow )
    {
        m_uCurRDlgID = -1 ;
        m_DlgMonster.EnableWindow( false ) ;
        return ;
    }
}



void CDlgMain::ReSetDlgRgn(BOOL bUpHide)
{
    CRect rect ;
    CDlgMain::GetWindowRect ( rect ) ;
    if(bUpHide)
    {
        CPoint pt[6] ;
        pt[0].x = 0;
        pt[0].y = 160;
        pt[1].x = 200;
        pt[1].y = 160;
        pt[2].x = 200;
        pt[2].y = 120;
        pt[3].x = rect.Width();
        pt[3].y = 120;
        pt[4].x = rect.Width();
        pt[4].y = rect.Height();
        pt[5].x = 0;
        pt[5].y = rect.Height();
        CRgn rgn ;
        rgn.CreatePolygonRgn ( pt, 6, ALTERNATE ) ;
        CDlgMain::SetWindowRgn ( rgn, true ) ;
    }
    else
    {
        CPoint pt[6] ;
        pt[0].x = 0;
        pt[0].y = 160;
        pt[1].x = 200;
        pt[1].y = 160;
        pt[2].x = 200;
        pt[2].y = 0;
        pt[3].x = rect.Width();
        pt[3].y = 0;
        pt[4].x = rect.Width();
        pt[4].y = rect.Height();
        pt[5].x = 0;
        pt[5].y = rect.Height();
        CRgn rgn ;
        rgn.CreatePolygonRgn ( pt, 6, ALTERNATE ) ;
        CDlgMain::SetWindowRgn ( rgn, true ) ;
    }
}

void CDlgMain::ResetMsgLines()
{
    /*
    	CRect rect;
    	m_DlgNorMsgDown.GetWindowRect(&rect);

    	int nLinesDown=(rect.bottom - rect.top -_MSGDLG_SETRECT)/(14+2);

    	m_DlgNorMsgUp.GetWindowRect(&rect);
    	int nLinesUp=(rect.bottom - rect.top-_MSGDLG_SETRECT)/(14+2);
    	if(m_DlgTalk.GetIsGameMsgEx())
    	{
    		g_objGameMsg.SetShowLinesEx(nLinesUp,nLinesDown);
    	}
    	else
    	{
    		g_objGameMsg.SetShowLines(nLinesDown);
    		int dTop = g_objGameMsg.GetNorAeroTop();
    		CRect rect;
    		m_DlgNorMsgDown.GetWindowRect(rect);
    		m_DlgNorMsgDown.MoveWindow(0, dTop - 25 - 16, rect.Width(),rect.Height(),false);
    	}*/
}

void CDlgMain::OpenTaskList(int nKind)
{
    if (!m_DlgPlayerTaskList.m_bShow)
    {
        CloseCurDlg(2);
        m_uCurRDlgID = DLG_TASKLIST;
        m_DlgPlayerTaskList.EnableWindow(true);
        m_DlgPlayerTaskList.SetTaskListKind(nKind);
    }
    else
    {
        m_DlgPlayerTaskList.ResetTaskList();
        m_DlgPlayerTaskList.SetTaskListKind(nKind);
    }
}

void CDlgMain::OpenTaskUpDlg()
{
    if (!m_DlgPlayerTaskUp.m_bShow)
    {
        CloseCurDlg(2);
        m_uCurRDlgID = DLG_TASKGET;
        m_DlgPlayerTaskUp.EnableWindow(true);
    }
}


void CDlgMain::OpenTaskUpItem(UCHAR idTask)
{
    if (!m_DlgTaskUpItem.m_bShow)
    {
        CloseCurDlg(1);
        m_uCurMDlgID = DLG_TASKUPITEM;
        m_DlgTaskUpItem.EnableWindow(true);
        m_DlgTaskUpItem.SetUptaskId(idTask);
    }
}

void CDlgMain::OpenAnnounceList()
{
    if (!m_DlgAnnounceList.m_bShow)
    {
        CloseCurDlg(2);
        m_uCurRDlgID = DLG_TBOARDLIST;
        m_DlgAnnounceList.EnableWindow(true);
    }
}

void CDlgMain::OpenAnnounceUp()
{
    if (!m_DlgAnnounceUp.m_bShow)
    {
        CloseCurDlg(2);
        m_uCurRDlgID = DLG_TBOARDUP;
        m_DlgAnnounceUp.EnableWindow(true);
    }
}

void CDlgMain::OpenAnnounceInfo()
{
    if (!m_DlgAnnounceInfo.m_bShow)
    {
        CloseCurDlg(0);
        m_uCurLDlgID = DLG_TBOARDINFO;
        m_DlgAnnounceInfo.EnableWindow(true);
    }
}

void CDlgMain::CloseNpcDlg()
{
    if ( m_DlgNpcBooth.m_bShow)
    {
        m_DlgNpcBooth.EnableWindow(false);
    }
    if (m_DlgMonsterDepot.m_bShow)
    {
        m_DlgMonsterDepot.EnableWindow(false);
    }
    if (m_DlgMonsterEvolve.m_bShow)
    {
        m_DlgMonsterEvolve.EnableWindow(false);
    }
    if (m_DlgMonsterNpc.m_bShow)
    {
        m_DlgMonsterNpc.EnableWindow(false);
    }
    if (m_DlgMonsterImprove.m_bShow)
    {
        m_DlgMonsterImprove.EnableWindow(false);
    }
    if (m_DlgMonsterHatch.m_bShow)
    {
        m_DlgMonsterHatch.EnableWindow(false);
    }
    if (m_DlgNpcBuy.m_bShow)
    {
        m_DlgNpcBuy.OnCloseB();
    }
    if (m_DlgDepot.m_bShow)
    {
        m_DlgDepot.OnDepotClose();
    }
    if (m_DlgEmbed.m_bShow)
    {
        m_DlgEmbed.OnCloseB();
    }
    if (m_DlgImprove.m_bShow)
    {
        m_DlgImprove.OnCloseB();
    }
    if (m_DlgPlayerTaskList.m_bShow)
    {
        m_DlgPlayerTaskList.EnableWindow(false);
    }
    if (m_DlgPlayerTaskView.m_bShow)
    {
        m_DlgPlayerTaskView.EnableWindow(false);
    }
    if (m_DlgPlayerTaskUp.m_bShow)
    {
        m_DlgPlayerTaskUp.EnableWindow(false);
    }
    if (m_DlgAnnounceUp.m_bShow)
    {
        m_DlgAnnounceUp.EnableWindow(false);
    }
    if (m_DlgAnnounceInfo.m_bShow)
    {
        m_DlgAnnounceInfo.EnableWindow(false);
    }
    if (m_DlgAnnounceList.m_bShow)
    {
        m_DlgAnnounceList.EnableWindow(false);
    }
}

void CDlgMain::OpenHatchDlg(OBJID idNpc)
{
    if (!m_DlgMonsterHatch.m_bShow)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_MONSTERHATCH;
        m_DlgMonsterHatch.OpenHatch(idNpc);
        m_DlgMonsterHatch.EnableWindow(true);
        if (!m_DlgMonster.m_bShow)
        {
            CDlgMain::CloseCurDlg(2);
            m_uCurRDlgID = DLG_MONSTER;
            m_DlgMonster.EnableWindow(true);
        }
    }
}

void CDlgMain::OpenMonsterDepot(OBJID idNpc)
{
    if (!m_DlgMonsterDepot.m_bShow)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_MONSTERDEPOT;
        g_objHero.OpenPackage(idNpc, PACKAGE_EUDEMON_STORAGE);
        m_DlgMonsterDepot.EnableWindow(true);
        if (!m_DlgMonster.m_bShow)
        {
            CDlgMain::CloseCurDlg(2);
            m_uCurRDlgID = DLG_MONSTER;
            m_DlgMonster.EnableWindow(true);
        }
    }
}

void CDlgMain::OpenMonsterEvolve(int nMode)
{
    if (!m_DlgMonsterEvolve.m_bShow)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_MONSTEREVOLVE;
        //		m_DlgMonsterHatch.OpenHatch(idNpc);
        m_DlgMonsterEvolve.EnableWindow(true);
        m_DlgMonsterEvolve.SetEvoMode(nMode);
        if (!m_DlgMonster.m_bShow)
        {
            CDlgMain::CloseCurDlg(2);
            m_uCurRDlgID = DLG_MONSTER;
            m_DlgMonster.EnableWindow(true);
        }
    }
}

void CDlgMain::OpenMonsterImpDlg(int nMode)
{
    if(!m_DlgMonsterImprove.m_bShow)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_MONSTERIMPROVE;
        m_DlgMonsterImprove.EnableWindow(true);
        m_DlgMonsterImprove.SetDlgMode(nMode);
        if (!m_DlgMonster.m_bShow)
        {
            CDlgMain::CloseCurDlg(2);
            m_uCurRDlgID = DLG_MONSTER;
            m_DlgMonster.EnableWindow(true);
        }
        if ( !m_DlgMpcGoods.m_bShow )
        {
            if (m_DlgGroup.m_MenuGroup.m_bShow)
            {
                m_DlgGroup.m_MenuGroup.EnableWindow(false);
            }
            m_DlgMpcGoods.EnableWindow() ;
            m_DlgMpcGoods.m_ChkStones.SetCheckValue(1);
            m_DlgMpcGoods.m_ChkGoods.SetCheckValue(0);
            m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow(false);
            m_DlgMpcGoods.m_MpcStonesGri.EnableWindow();
        }
        else if ( m_DlgMpcGoods.m_bShow )
        {
            if (m_DlgMpcGoods.m_ChkStones.GetCheckValue() == 0)
            {
                m_DlgMpcGoods.m_ChkStones.SetCheckValue(1);
                m_DlgMpcGoods.m_ChkGoods.SetCheckValue(0);
                m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow(false);
                m_DlgMpcGoods.m_MpcStonesGri.EnableWindow();
            }
        }
    }
}

void CDlgMain::OpenAddAucItemDlg(OBJID idNpc)
{
    if (!m_DlgAddAucItem.m_bShow && idNpc)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_ADDAUCITEM;
        m_DlgAddAucItem.EnableWindow(true);
        m_DlgAddAucItem.SetAuctionNpcId(idNpc);
        if (!m_DlgMpcGoods.m_bShow)
        {
            m_DlgMpcGoods.EnableWindow();
        }
    }
}

void CDlgMain::OpenBidAuctionDlg(OBJID idNpc)
{
    if (!m_DlgBidAuction.m_bShow && idNpc)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_AUCTION;
        m_DlgBidAuction.EnableWindow(true);
        m_DlgBidAuction.SetAuctionNpcId(idNpc);
    }
}

void CDlgMain::OpenAucPackage(OBJID idNpc)
{
    if (!m_DlgAucPackage.m_bShow)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_AUCPACKAGE;
        m_DlgAucPackage.EnableWindow(true);
        m_DlgAucPackage.SetAuctionNpcId(idNpc);
        if (!m_DlgMpcGoods.m_bShow)
        {
            m_DlgMpcGoods.EnableWindow();
        }
    }
}

void CDlgMain::OpenIdentifyDlg()
{
    if (!m_DlgIdentify.m_bShow)
    {
        CDlgMain::CloseCurDlg(1);
        m_uCurMDlgID = DLG_IDENTIFY;
        m_DlgIdentify.EnableWindow(true);
        if (!m_DlgMpcGoods.m_bShow)
        {
            m_DlgMpcGoods.EnableWindow();
        }
    }
}

void CDlgMain::OnMainEquip()
{
    if(!m_DlgQuery.m_bShow)
    {
        CDlgMain::CloseCurDlg(1);
        CDlgMain::CloseCurDlg( 2 );
        CDlgMain::GetParent()->PostMessage(WM_MY_MESSAGE, ON_DLGTASK_CLOSE);
        m_uCurRDlgID = DLG_QUERY;
        m_DlgQuery.PopupCurChk();
        m_DlgQuery.m_ChkEquip.SetCheckValue(1);
        m_DlgQuery.m_btCurChildDlgID = 0;
        m_DlgQuery.EnableWindow(true);
    }
    else
    {
        if(m_DlgQuery.m_btCurChildDlgID != 0)
        {
            m_DlgQuery.PopupCurChk();
            m_DlgQuery.HideCurDlg(m_DlgQuery.m_btCurChildDlgID);
            m_DlgQuery.m_btCurChildDlgID = 0;
            m_DlgQuery.m_ChkEquip.SetCheckValue(1);
            m_DlgQuery.ShowCurDlg(0);
        }
        else if (m_DlgQuery.m_btCurChildDlgID == 0)
        {
            m_DlgQuery.EnableWindow(false);
            m_uCurRDlgID = -1;
        }
    }
}


void CDlgMain::OnMainStones()
{
    if ( !m_DlgMpcGoods.m_bShow )
    {
        if (m_DlgGroup.m_MenuGroup.m_bShow)
        {
            m_DlgGroup.m_MenuGroup.EnableWindow(false);
        }
        if(m_DlgEmotion.m_bShow)
        {
            m_DlgEmotion.EnableWindow (false);
        }
        m_DlgMpcGoods.m_ChkStones.SetCheckValue(1);
        m_DlgMpcGoods.m_ChkGoods.SetCheckValue(0);
        m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow(false);
        m_DlgMpcGoods.m_MpcStonesGri.EnableWindow();
        m_DlgMpcGoods.EnableWindow() ;
        return ;
    }
    else if ( m_DlgMpcGoods.m_bShow )
    {
        if (m_DlgMpcGoods.m_ChkStones.GetCheckValue() == 0)
        {
            m_DlgMpcGoods.m_ChkStones.SetCheckValue(1);
            m_DlgMpcGoods.m_ChkGoods.SetCheckValue(0);
            m_DlgMpcGoods.m_MpcGoodsGri.EnableWindow(false);
            m_DlgMpcGoods.m_MpcStonesGri.EnableWindow();
        }
        else
        {
            m_DlgMpcGoods.EnableWindow(false);
        }
        return ;
    }
}
