
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef _NETWORKDEF_H
#define _NETWORKDEF_H

#include "define.h"

// max size define
//const int _MAX_MSGSIZE		=1024;
const int _MAX_WORDSSIZE	= 256;
const int _MAX_IDLETIME		= 600;	// 600s idle time

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

const unsigned short _MSG_NPCINFO		= _MSG_GENERAL + 1030;
const unsigned short _MSG_NPC			= _MSG_GENERAL + 1031;
const unsigned short _MSG_DIALOG		= _MSG_GENERAL + 1032;
const unsigned short _MSG_FRIENDINFO	= _MSG_GENERAL + 1033;
const unsigned short _MSG_TIME			= _MSG_GENERAL + 1034;

const unsigned short _MSG_WALKEX		= _MSG_GENERAL + 2005;

// msg npc server
const unsigned short _MSG_NPCSERVER		= 20000;
const unsigned short _MSG_AINPCINFO		= _MSG_NPCSERVER + 1;


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

const unsigned short _TXTATR_REGISTER	= _TXTATR_NORMAL + 100;
const unsigned short _TXTATR_ENTRANCE	= _TXTATR_NORMAL + 101;
const unsigned short _TXTATR_SHOP		= _TXTATR_NORMAL + 102;
const unsigned short _TXTATR_PETTALK	= _TXTATR_NORMAL + 103;
const unsigned short _TXTATR_CRYOUT		= _TXTATR_NORMAL + 104;
const unsigned short _TXTATR_WEBPAGE	= _TXTATR_NORMAL + 105;

const unsigned short _TXTATR_NEWMESSAGE	= _TXTATR_NORMAL + 106;
const unsigned short _TXTATR_TASK		= _TXTATR_NORMAL + 107;

const unsigned short _TXTATR_QUIZQUESTION	= _TXTATR_NORMAL + 108; //智力问答，问题
const unsigned short _TXTATR_QUIZANSWER		= _TXTATR_NORMAL + 109; //智力问答，答案


#endif
