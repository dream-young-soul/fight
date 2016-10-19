
//**********************************************************
// 代码编辑器
//**********************************************************

// GameMsg.h: interface for the GameMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEMSG_H__8DCDC868_325E_4751_9C6B_4277BC598AEA__INCLUDED_)
#define AFX_GAMEMSG_H__8DCDC868_325E_4751_9C6B_4277BC598AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <deque>
using namespace std;

#include "basefunc.h"
#include "txtMsgSet.h"
#include "NetworkDef.h"
#include "ChatLog.h"
//--------------------------------------------------------------------
//数组长度定义
const int _MAX_EMOTIONSIZE			= 16;

const int _AREA_NUMBER				= 3 ;		// The show area number
const int _NORAREA_MSG_LINE			= 20 ;		// The normal area message line number
const int _SYSAREA_MSG_LINE			= 3 ;		// the system area message line number
const int _MANAREA_MSG_LINE			= 1 ;		// the manager area message lien number
//-------------------------------------------------------------------
// define color constants
const DWORD _COLOR_SYSTEM_SPEAKER     = 0XFFFE00FB;
const DWORD _COLOR_SELF				  = 0XFF00FF;
const DWORD _COLOR_SYSTEM_MESSAGE     = 0XFF0000;
const DWORD _COLOR_PLAYER_SPEAKER     = 0XAAFFAA;
const DWORD _COLOR_RECEIVER           = 0XAAFFAA;
const DWORD _COLOR_EMOTION            = 0X00FF00;
const DWORD _COLOR_NORMAL             = 0XFFFFFF;
const DWORD _COLOR_TIME               = 0XFFFFFF;
//--------------------------------------------------------------------
class CMyBitmap;
//--------------------------------------------------------------------
/*
typedef struct
{
	DWORD dwSystemColor;
	DWORD dwSenderColor;
	DWORD dwReceiverColor;
	DWORD dwEmotionColor;
}CMyMsgColor;
*/
//--------------------------------------------------------------------
class CGameMsgUnit
{
public:
    CGameMsgUnit(void);
    virtual ~CGameMsgUnit(void);
    char*	Show(int nStartX, int nStartY, BOOL bCapture, int nScreenWidth = _SCR_WIDTH);
    int		GetLines(int nScreenWidth = _SCR_WIDTH);
    void	SetStartX(int nPosX) { m_nStartX = nPosX; }
    BOOL	GetText(char* pszText, BOOL bTimes = true );
    const char* GetMsg(void) const {return m_szMsg;}
    CMyPos ShowColorStr(int nPosX, int nPosY, DWORD dwColor, const char* pszString, int nScreenWidth); // 显示彩色的字符串

private:
    int		Scan(int nScreenWidth);	// 取得需要占用的屏幕行数
    void	GetDateTimeStr(DWORD dwDateTime, char* pszStr);
public:
    char	m_szSender[_MAX_WORDSSIZE];
    char	m_szReceiver[_MAX_WORDSSIZE];
    char	m_szEmotion[_MAX_WORDSSIZE];
    char	m_szMsg[_MAX_WORDSSIZE];
    unsigned short	m_unTxtAttribute;
    unsigned short	m_unTxtStyle;
    DWORD	m_dwWordsColor;
    int		m_nStartX;			// 显示的起始X坐标
    int		m_nDragStep;
    int		m_nQuakeFlag;
    BOOL	m_bSuper;
    DWORD	m_dwTime, m_dwBeginTime;
private:
    CMySize m_sizeFont;
    CMySize m_sizeScreen;
    int		m_nFlashFlag;
    int		m_nStrReturnCounter;
    DWORD	m_dwWordsColorFlash;
    DWORD	m_dwSystemColor;
    DWORD	m_dwSystemMessageColor;
    DWORD	m_dwSenderColor;
    DWORD	m_dwNormalColor;
    DWORD	m_dwReceiverColor;
    DWORD	m_dwEmotionColor;
    int		m_nPrevPosX, m_nPrevPosY;

};
//--------------------------------------------------------------------
typedef deque<CGameMsgUnit*> DEQUE_MSGUNIT;
typedef deque<char*> DEQUE_BLACKNAME;
//--------------------------------------------------------------------

class CGameMsg
{
public:
    CGameMsg(void);
    virtual ~CGameMsg(void);
    BOOL	AddMsg(const char* pszSender, char* pszReceiver, char* pszEmotion, char* pszMsg, DWORD dwWordsColor, unsigned short unTxtStyle, unsigned short unTxtAttribute, DWORD dwTime);
    BOOL	AddMsg(const char* pszMsg, DWORD dwChannel = _TXTATR_SYSTEM, DWORD dwWordsColor = _COLOR_SYSTEM_MESSAGE, DWORD dwStyle = _TXT_NORMAL, BOOL bSuper = false);
    BOOL	AddMsg(DWORD dwChannel, const char* fmt, ...);
    void    ClearMsg(BOOL bSystem = true);
    void	Show(BOOL bCapture = false, BOOL bSystem = true );
    void	ShowEx(BOOL bCapture = false, BOOL bSystem = true );

    void	SetShowLines(UINT uMaxLines) ;
    UINT	GetShowLines(void)	{return m_nMaxLine;}
    void	SetCurShowChannel( unsigned short usCurChannel ) ;
    BOOL	SetLastShowLineIndex( int nIndex, unsigned short usChannel ) ;

    void	ProcessArea ( int& nCurAmount, int nMsgAmount, int nDequeIndex, int nDequeSize, int* pIndex, BOOL bFull ) ;
    void	SetNorAreaShowStates( const CMyPos& pos, int nWidth ) ;
    void	SetSysAreaShowStates( const CMyPos& pos, int nWidth ) ;
    void	SetManAreaShowStates( const CMyPos& pos, int nWidth ) ;

    // use for the leave msg
    BOOL	GetLeaveWordByIndexName( int nIndex, char* szName, char* strTxt, char* strTime);
    BOOL	GetLeaveWordByIndex( int nIndex, char* szName, char* strTxt, char* strTime);
    int		GetLeaveWordAmount() ;
    int		GetLeaveWordAmountByName(const char* pszName);
    void	DelLeaveWordByIndexName(int nIndex, char* strName);
    void	DestroyLeaveWord() ;

    CTxtMsgSet		m_setMsg;

private:
    // Define the deque for every channel
    DEQUE_MSGUNIT	m_dequeMsg[14] ;							// The every channel deque index is the ( Channel id - _TXTATR_NORMAL - 1 )
    int				m_nMaxMsgLine[14] ;							// The every channel's max line num
    int				m_nLastShowLineIndex[14] ;					// The last show line's index
    unsigned short	m_usCurShowChannel ;						// The current's show channel

    // Use for the leave word
    DEQUE_MSGUNIT	m_dequeLeaveMsg ;							// The leave word's deque

    // First element is for the deque's index,second element is for the message index in current deque
    int				m_nNorAreaShowIndex[_NORAREA_MSG_LINE * 2] ;	// The normal area's show index
    int				m_nNorAreaAmount ;								// The normal area's message amount
    CMyPos			m_NorAreaShowPos ;								// The normal area's show pos
    int				m_nNorAreaWidth ;								// The normal area's width
    int				m_nSysAreaShowIndex[_SYSAREA_MSG_LINE * 2] ;	// The system area's show index
    int				m_nSysAreaAmount ;								// The system area's message amount
    CMyPos			m_SysAreaShowPos ;								// The system area's show pos
    int				m_nSysAreaWidth ;								// The system area's width
    int				m_nManAreaShowIndex[_MANAREA_MSG_LINE * 2] ;	// The manager area's show index
    int				m_nManAreaAmount ;								// The manager area's message amount
    CMyPos			m_ManAreaShowPos ;								// The manager area's show pos
    int				m_nManAreaWidth ;								// The manager area's width

    int				m_nMaxLine;		// 最大信息行数

    DWORD			m_dwTime;

    BOOL			m_bChannelTalk;			//	公聊
    BOOL			m_bChannelSyndicate;	//  帮派
    BOOL			m_bChannelFriend;		//  好友
    BOOL			m_bChannelTeam;			//  队伍
    BOOL			m_bChannelAction;		//  动作
    BOOL			m_bChannelServe;
    BOOL			m_bChannelPrivateExceptFriend;	// 除了好友外的私聊
    BOOL			m_bChannelPrivateFriend;		// 好友的私聊

    DWORD			m_colorChannelTalk;
    DWORD			m_colorChannelSyndicate;
    DWORD			m_colorChannelFriend;
    DWORD			m_colorChannelTeam;
    DWORD			m_colorChannelPrivate;
    DWORD			m_colorChannelAction;
    DWORD			m_colorChannelServe ;

    char			m_szCaptureName[_MAX_NAMESIZE];
private:
    int m_nMaxDownLine;
    int m_nMaxUpLine;
    DEQUE_BLACKNAME	m_setBlackName;

public:
    void	DestroyBlackName();
    void	AddBlackName(char* pszName);
    void	DelBlackName(int nIndex);
    int		GetBlackNameAmount();
    char*	GetBlackNameByIndex(int nIndex);
    BOOL	IsBlackName(char* pszName);
public:
    int GetNorAreaWidth()	{return m_nNorAreaWidth;}
    int GetManAreaWidth()	{return m_nManAreaWidth;}
    int GetNorDownAreaTop();
    int GetNorUpAreaTop();
    int GetNorAeroTop();
    void SetShowLinesEx(UINT uMaxUpLines, UINT uMaxDownLines);
    void GetShowLinesEx(int& nMaxUpLines, int& nMaxDownLines);
    void	SetChannelColor(unsigned short usChannel, DWORD dwColor);
    void	GetChannelColor(unsigned short usChannel, DWORD& dwColor);

    void	SetChannelStatus(unsigned short usChannel, BOOL bOn);
    void	SetPrivateChannelStatusExceptFriend(BOOL bOn);
    void	SetPrivateChannelStatusFriendOnly(BOOL bOn);
    BOOL	TestPriveteChannel(char* pszName);
    BOOL	GetChannelStatus(unsigned short usChannel);

    char*	GetCaptureName() {return m_szCaptureName;}

    //public:
    CChatLog		m_objChatLog;

    //Emotion
    int	ShowEmotionWnd(int x, int y, int nPage, int nXAmount = 8, int nYAmount = 4);

    int m_nWarningTime;
    DWORD m_dwShowWarning;
    char m_szWarning[_MAX_WORDSSIZE];
};
//--------------------------------------------------------------------
extern CGameMsg		g_objGameMsg;
#ifdef _ANALYSIS_ON
extern DWORD g_dwTxtMsgTimeCost;
#endif
//--------------------------------------------------------------------
#endif // !defined(AFX_GAMEMSG_H__8DCDC868_325E_4751_9C6B_4277BC598AEA__INCLUDED_)
