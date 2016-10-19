
//**********************************************************
// 代码编辑器
//**********************************************************

// 宏定义
// 仙剑修，2001.11.19

#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#include "Define.h"

//////////////////////////////////////////////////////////////////////////////////
#define	ENCRYPT
#define	SERVERNAME			"帐号服务器"
#define	INDEXOPTIMIZE						// 索引优化
#define	RC5ENCRYPT							// 打开玩家登录口令加密

#define	FLAG_ISP_TO_NORMAL	"!ISP计费失败!"		// 注意：该字符串必须与游戏服务器相匹配。每个IP限一个帐号。否则自动转普通帐号并提示
#define	FLAG_NO_POINT		"!NO_POINT!"		// 注意：该字符串必须与游戏服务器相匹配。没有点数也可登录。由游戏服务器判断玩家等级是否可免费玩
//#define	DATABASE_PASSWORD					// 玩家数据库口令加密
//#define		TEST_INDEX		// 用于调试INDEX的同步。临时宏，要删除

// 为保持和幻灵的兼容，千万不要修改主版本号。
#define	ACCOUNTVERSION		(0xC200+19)		// 游戏服务器版本号0x0102表示1.02版, 0x8102为ALPHA版本1.2，0xC102为BETA版本1.2
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// PASSWORD加密密钥
const unsigned char RC5PASSWORD_KEY[16] = {	0x3C, 0xDC, 0xFE, 0xE8, 0xC4, 0x54, 0xD6, 0x7E,
        0x16, 0xA6, 0xF8, 0x1A, 0xE8, 0xD0, 0x38, 0xBE
                                          };
const unsigned char RC5BARPASSWORD_KEY[16] = {	0x44, 0xD0, 0xE2, 0xBA, 0x4A, 0x38, 0x14, 0x44,
        0x64, 0xE0, 0x12, 0xAE, 0xDA, 0x56, 0x1C, 0xF8
                                             };

#define POINT_KEY1				0x92502490	// A = 0xEE, B = 0x17, C = 0x33, first = 0x50, key = 92502490
#define POINT_KEY2				0xc03310fc	// A = 0x82, B = 0x23, C = 0x61, first = 0x33, key = c03310fc
//#define ACCOUNT_KEY1				0xf2cfa8cc	// A = 0xB2, B = 0x9B, C = 0x53, first = 0xCF, key = f2cfa8cc
//#define ACCOUNT_KEY2				0x78c6fa82	// A = 0xFC, B = 0xC9, C = 0xD9, first = 0xC6, key = 78c6fa82
#define ACCOUNT_KEY1			0xa61fce5e	// A = 0x20, B = 0xFD, C = 0x07, first = 0x1F, key = a61fce5e
#define ACCOUNT_KEY2			0x443ffc04	// A = 0x7A, B = 0xCF, C = 0xE5, first = 0x3F, key = 443ffc04
/////////////////////////////////////////////////////////////////////////////

//#define	DUMP									// 输出所有调试细节LOG
#define	ONLINEFILE			"Online.txt"
#define LOGFILE				"AccountServer"
#ifdef	ENCRYPT
#define	SERVERTITLE			SERVERNAME /*"(x)"*/
#else
#define	SERVERTITLE			SERVERNAME
#endif
#define IPSTRSIZE		16
//#define _MAX_MSGSIZE		1024			// 不能重定义
//#define	FD_SETSIZE			600			//??? 与 StdAfx.h 中同步
#define RECVREPEATTIMES		3
#define MAXGAMESERVERS		100			//? ★注意与DLG中同步。游戏服务器最大数量(最大连接数)
#define	MAXCONNECTS			(FD_SETSIZE - MAXGAMESERVERS - 10)	//? 登录线程最大连接数
//ONLINETABLESIZE	#define MAXPLAYERS			1500		//? 玩家在线表尺寸
#define SERVERNAMESIZE		1024		// 游戏服务器域名(IP)串空间尺寸
////////////////////////////////////////////////////////////////////////////////
extern	char	SERVERKEY[];
extern	int		LOGINLOOPDELAY;
extern	int		POINTLOOPDELAY;
extern	int		REBUILDLISTENDELAYSEC;
extern	int		LOGINLISTENPORT;
extern	int		POINTLISTENPORT;
extern	int		POINTSOCKETSNDBUF;
extern	int		MAXBANIPS;
extern	int		BANERRORS;
extern	int		BANERRORSECS;
extern	int		BANSECS;
extern	int		ONLINETABLESIZE;
extern	int		HEARTBEATINTERVALSECS;
extern	int		HEARTBEATKICKSECS;
extern	int		ONLINEHEARTBEATSECS;
extern	int		POINTFEEINTERVALSECS;
extern	int		ENABLELOGINDELAY;
extern	int		OUTPUTONLINESECS;
extern	int		REJOINTIMEWAITSECS;
extern	int		COUNTFILESECS;
extern	int		ACCOUNT_AHEAD_DAYS;
extern	int		SERVER_BUSY_DELAY_SECS;
extern	int		SERVER_FULL_DELAY_SECS;
extern	int		KICKOUT_USER_WHEN_NO_POINT;
extern	int		ENABLE_LOGIN_NO_POINT;
extern	int		MONTH_CARD_TO_POINT_CARD;
extern	int		MAX_GM_ID;
extern	int		STAT_SERIAL;
extern	char	SERVER_TITLE[];
extern	int		ENABLE_91U_CREATE_ACCOUNT;

extern	char	DBHOSTNAME[];
//extern	char	DBUSER[];
//extern	char	DBPASSWORD[];
extern	char	DATABASENAME[];
extern  char    POINTTABLE[];
////////////////////////////////////////////////////////////////////////////////

bool	GetConfig();
bool	GetServerAccount();


////////////////////////////////////////////////////////////////////////////////

#undef	ASSERT
#define	ASSERT(x)	{if (!(x)) LOGERROR("ASSERT(" #x ") in %d line of %s", __LINE__, __FILE__); }
//#define LOCKTHREAD		CSingleLock(&m_xCtrl, true)			★有大BUG
#define LOCKTHREAD		CSingleLock xLock(&m_xCtrl, true); ASSERT(xLock.IsLocked());


#endif // _TYPEDEF_H
