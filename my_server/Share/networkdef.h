
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef _NETWORKDEF_H
#define _NETWORKDEF_H

#include "define.h"

// max size define
const int _MAX_MSGSIZE		= 1024;
const int _MAX_WORDSSIZE	= 256;
const int _MAX_IDLETIME		= 600;	// 600s idle time

typedef	char	WORDSSTR[_MAX_WORDSSIZE];

// msg type define
const unsigned short _MSG_NONE			= 0;
const unsigned short _MSG_GENERAL		= 1000;
const unsigned short _MSG_REGISTER		= _MSG_GENERAL + 1;
const unsigned short _MSG_LOGIN			= _MSG_GENERAL + 2;
const unsigned short _MSG_LOGOUT		= _MSG_GENERAL + 3;
const unsigned short _MSG_TALK			= _MSG_GENERAL + 4;
const unsigned short _MSG_WALK			= _MSG_GENERAL + 5;
const unsigned short _MSG_USERINFO		= _MSG_GENERAL + 6;
const unsigned short _MSG_ATTACK		= _MSG_GENERAL + 7;
const unsigned short _MSG_ITEMINFO		= _MSG_GENERAL + 8;
const unsigned short _MSG_ITEM			= _MSG_GENERAL + 9;
const unsigned short _MSG_ACTION		= _MSG_GENERAL + 10;
const unsigned short _MSG_ACCIDENT		= _MSG_GENERAL + 11;
const unsigned short _MSG_TICK			= _MSG_GENERAL + 12;
const unsigned short _MSG_ROOM			= _MSG_GENERAL + 13;
const unsigned short _MSG_PLAYER		= _MSG_GENERAL + 14;
const unsigned short _MSG_NAME			= _MSG_GENERAL + 15;
const unsigned short _MSG_WEATHER		= _MSG_GENERAL + 16;
const unsigned short _MSG_USERATTRIB	= _MSG_GENERAL + 17;
const unsigned short _MSG_ROLE			= _MSG_GENERAL + 18;
const unsigned short _MSG_FRIEND		= _MSG_GENERAL + 19;
const unsigned short _MSG_EFFECT		= _MSG_GENERAL + 20;
const unsigned short _MSG_QUIZGAME		= _MSG_GENERAL + 21;
const unsigned short _MSG_INTERACT		= _MSG_GENERAL + 22;
const unsigned short _MSG_TEAM			= _MSG_GENERAL + 23;
const unsigned short _MSG_ALLOT			= _MSG_GENERAL + 24;
const unsigned short _MSG_WEAPONSKILL	= _MSG_GENERAL + 25;
const unsigned short _MSG_TEAMMEMBER	= _MSG_GENERAL + 26;
const unsigned short _MSG_GEMEMBED		= _MSG_GENERAL + 27;

const unsigned short _MSG_CONNECT		= _MSG_GENERAL + 52;
const unsigned short _MSG_TRADE			= _MSG_GENERAL + 56;
const unsigned short _MSG_BATTLESYSTEM	= _MSG_GENERAL + 57;

// new info in cq
const unsigned short _MSG_MAPITEM		= _MSG_GENERAL + 101;
const unsigned short _MSG_PACKAGE		= _MSG_GENERAL + 102;
const unsigned short _MSG_MAGICINFO		= _MSG_GENERAL + 103;
const unsigned short _MSG_FLUSHEXP		= _MSG_GENERAL + 104;
const unsigned short _MSG_MAGICEFFECT	= _MSG_GENERAL + 105;
const unsigned short _MSG_SYNATTRINFO	= _MSG_GENERAL + 106;
const unsigned short _MSG_SYNDICATE		= _MSG_GENERAL + 107;
const unsigned short _MSG_ITEMINFOEX	= _MSG_GENERAL + 108;
const unsigned short _MSG_NPCINFOEX		= _MSG_GENERAL + 109;
const unsigned short _MSG_MAPINFO		= _MSG_GENERAL + 110;
const unsigned short _MSG_MESSAGEBOARD	= _MSG_GENERAL + 111;
const unsigned short _MSG_SYNMEMBERINFO	= _MSG_GENERAL + 112;
const unsigned short _MSG_DICE			= _MSG_GENERAL + 113;
const unsigned short _MSG_SYNINFO		= _MSG_GENERAL + 114;

const unsigned short _MSG_NPCINFO		= _MSG_GENERAL + 1030;
const unsigned short _MSG_NPC			= _MSG_GENERAL + 1031;
const unsigned short _MSG_DIALOG		= _MSG_GENERAL + 1032;
const unsigned short _MSG_FRIENDINFO	= _MSG_GENERAL + 1033;
const unsigned short _MSG_TIME			= _MSG_GENERAL + 1034;
const unsigned short _MSG_PETINFO		= _MSG_GENERAL + 1035;
const unsigned short _MSG_DATAARRAY		= _MSG_GENERAL + 1036;
const unsigned short _MSG_EUDEMONATTRIB	= _MSG_GENERAL + 1037;
const unsigned short _MSG_SCHOOLMEMBER	= _MSG_GENERAL + 1038;
const unsigned short _MSG_PLAYERTASK	= _MSG_GENERAL + 1039;
const unsigned short _MSG_TASK_LIST		= _MSG_GENERAL + 1040;
const unsigned short _MSG_ANNOUNCE_LIST = _MSG_GENERAL + 1041;
const unsigned short _MSG_ANNOUNCE_INFO = _MSG_GENERAL + 1042;
const unsigned short _MSG_AUCTION       = _MSG_GENERAL + 1043;
const unsigned short _MSG_CHATROOM      = _MSG_GENERAL + 1044;	//圣战，聊天室系统消息
const unsigned short _MSG_ITEMATTRIB	= _MSG_GENERAL + 1045;


// new msg in qq
const unsigned short _MSG_WALKEX		= _MSG_GENERAL + 2005;

// msg npc server
const unsigned short _MSG_NPCSERVER		= 20000;
const unsigned short _MSG_AINPCINFO		= _MSG_NPCSERVER + 1;

// msg account server
const unsigned short _MSG_ACCOUNTSERVER	= 1000;
const unsigned short _MSG_LOGIN_A	= _MSG_ACCOUNTSERVER + 2;
const unsigned short _MSG_ACCOUNT_A	= _MSG_ACCOUNTSERVER + 51;
//const unsigned short _MSG_CONNECT	=_MSG_ACCOUNTSERVER+52;
const unsigned short _MSG_FEE_A		= _MSG_ACCOUNTSERVER + 200;		// paled
const unsigned short _MSG_QUERYFEE_A = _MSG_ACCOUNTSERVER + 201;		// paled

// color define
const unsigned long	_COLOR_WHITE	= 0x00ffffff;

// msg style define
const unsigned short _TXT_NORMAL	= 0;
const unsigned short _TXT_SCROLL	= 0x0001;
const unsigned short _TXT_FLASH		= 0x0002;
const unsigned short _TXT_BLAST		= 0x0008;

// msg attribute define
const unsigned short _TXTATR_NORMAL		= 2000;
const unsigned short _TXTATR_PRIVATE	= _TXTATR_NORMAL + 1;	// 私聊
const unsigned short _TXTATR_ACTION		= _TXTATR_NORMAL + 2;	// 动作
const unsigned short _TXTATR_TEAM		= _TXTATR_NORMAL + 3;	// 队伍
const unsigned short _TXTATR_SYNDICATE	= _TXTATR_NORMAL + 4;	// 帮派
const unsigned short _TXTATR_SYSTEM		= _TXTATR_NORMAL + 5;	// 系统
const unsigned short _TXTATR_FAMILY		= _TXTATR_NORMAL + 6;	// 家庭
const unsigned short _TXTATR_TALK		= _TXTATR_NORMAL + 7;	// 交谈
const unsigned short _TXTATR_YELP 		= _TXTATR_NORMAL + 8;	// 叫喊
const unsigned short _TXTATR_FRIEND 	= _TXTATR_NORMAL + 9;	// 朋友
const unsigned short _TXTATR_GLOBAL 	= _TXTATR_NORMAL + 10;	// 千里传音
const unsigned short _TXTATR_GM 		= _TXTATR_NORMAL + 11;	// GM频道
const unsigned short _TXTATR_WHISPER 	= _TXTATR_NORMAL + 12;	// 耳语
const unsigned short _TXTATR_GHOST	 	= _TXTATR_NORMAL + 13;	// 幽灵
const unsigned short _TXTATR_SERVE		= _TXTATR_NORMAL + 14; // 服务

const unsigned short _TXTATR_REGISTER	= _TXTATR_NORMAL + 100;
const unsigned short _TXTATR_ENTRANCE	= _TXTATR_NORMAL + 101;
const unsigned short _TXTATR_SHOP		= _TXTATR_NORMAL + 102;
const unsigned short _TXTATR_PETTALK	= _TXTATR_NORMAL + 103;
const unsigned short _TXTATR_CRYOUT		= _TXTATR_NORMAL + 104;
const unsigned short _TXTATR_WEBPAGE	= _TXTATR_NORMAL + 105;

const unsigned short _TXTATR_NEWMESSAGE	= _TXTATR_NORMAL + 106;
const unsigned short _TXTATR_TASK		= _TXTATR_NORMAL + 107;
const unsigned short _TXTATR_SYNWAR_FIRST	= _TXTATR_NORMAL + 108;	// 帮派战绩
const unsigned short _TXTATR_SYNWAR_NEXT	= _TXTATR_NORMAL + 109;	// 帮派战绩

const unsigned short _TXTATR_LEAVEWORD		= _TXTATR_NORMAL + 110;	// 留言系统
const unsigned short _TXTATR_SYNANNOUNCE	= _TXTATR_NORMAL + 111;	// 帮派公告
const unsigned short _TXTATR_MESSAGEBOX		= _TXTATR_NORMAL + 112;	// 对话框
const unsigned short _TXTATR_REJECT			= _TXTATR_NORMAL + 113;	// 驳回
const unsigned short _TXTATR_SYNTENET		= _TXTATR_NORMAL + 114;	// 帮派宗旨

const unsigned short _TXTATR_MSG_TRADE	= _TXTATR_NORMAL + 201;	// 交易留言板
const unsigned short _TXTATR_MSG_FRIEND	= _TXTATR_NORMAL + 202;	// 交友留言板
const unsigned short _TXTATR_MSG_TEAM	= _TXTATR_NORMAL + 203;	// 组队留言板
const unsigned short _TXTATR_MSG_SYN	= _TXTATR_NORMAL + 204;	// 帮派留言板
const unsigned short _TXTATR_MSG_OTHER	= _TXTATR_NORMAL + 205;	// 其它留言板
const unsigned short _TXTATR_MSG_SYSTEM	= _TXTATR_NORMAL + 206;	// 其它留言板
// ....... 请保留后续ID


const unsigned short _TXTATR_DIALOG_BEGIN	= _TXTATR_NORMAL + 500;	// 对话框第一个
// ... 中间的保留用于通知客户端打开界面
const unsigned short _TXTATR_DIALOG_END		= _TXTATR_NORMAL + 600;	// 对话框最后一个 + 1
#endif
