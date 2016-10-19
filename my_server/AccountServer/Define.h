
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _DEFINE_H
#define _DEFINE_H

//#include "StdAfx.h"
#include "windows.h"
typedef unsigned long OBJID;

// obj define
const OBJID	ID_NONE			= 0;

// max size define
const int _MAX_MSGSIZE		= 1024;
const int _MAX_ACCOUNTSIZE	= 16;
const int _MAX_NAMESIZE		= 16;
const int _MAX_PSWSIZE		= 16;
const int _MAX_WORDSSIZE	= 256;
const int _MAX_EMOTIONSIZE	= 16;
const int _MAX_IPSIZE		= 16;
//const int _MAX_STATUSSIZE	=64;
const int _MAX_IDLETIME		= 600;	// 600s idle time

const int _MAX_ITEMSIZE		= 16;
const int _MAX_STRING		= 256;

// time define
const unsigned long _TIME_BONUS		= 180;	// 180s to get bonus

// msg type define
const unsigned short _MSG_NONE		= 0;
const unsigned short _MSG_GENERAL	= 1000;
const unsigned short _MSG_ERROR		= _MSG_GENERAL + 1;
const unsigned short _MSG_LOGIN		= _MSG_GENERAL + 2;
const unsigned short _MSG_LOGOUT	= _MSG_GENERAL + 3;
const unsigned short _MSG_TALK		= _MSG_GENERAL + 4;
const unsigned short _MSG_WALK		= _MSG_GENERAL + 5;
const unsigned short _MSG_EMOTION	= _MSG_GENERAL + 6;
const unsigned short _MSG_POSITION	= _MSG_GENERAL + 7;
const unsigned short _MSG_USERINFO	= _MSG_GENERAL + 8;
const unsigned short _MSG_ATTACK	= _MSG_GENERAL + 9;
const unsigned short _MSG_STEAL		= _MSG_GENERAL + 10;
const unsigned short _MSG_SCHOOL	= _MSG_GENERAL + 11;
const unsigned short _MSG_EXCHANGE	= _MSG_GENERAL + 12;
const unsigned short _MSG_DIE		= _MSG_GENERAL + 13;
const unsigned short _MSG_REBORN	= _MSG_GENERAL + 14;
const unsigned short _MSG_REGISTER	= _MSG_GENERAL + 15;
const unsigned short _MSG_ITEMINFO	= _MSG_GENERAL + 16;
const unsigned short _MSG_ITEM		= _MSG_GENERAL + 17;
const unsigned short _MSG_DIR		= _MSG_GENERAL + 18;
const unsigned short _MSG_ACCIDENT	= _MSG_GENERAL + 19;

const unsigned short _MSG_MARRIAGE	= _MSG_GENERAL + 30;
const unsigned short _MSG_ROOM		= _MSG_GENERAL + 31;
const unsigned short _MSG_PLAYER	= _MSG_GENERAL + 32;
const unsigned short _MSG_PET		= _MSG_GENERAL + 33;
const unsigned short _MSG_PETINFO	= _MSG_GENERAL + 34;
const unsigned short _MSG_SKILLINFO	= _MSG_GENERAL + 35;
const unsigned short _MSG_BEHEAL    = _MSG_GENERAL + 36;
const unsigned short _MSG_NAME		= _MSG_GENERAL + 37;

const unsigned short _MSG_USERATTRIB = _MSG_GENERAL + 40;
const unsigned short _MSG_POINTALLOT = _MSG_GENERAL + 41;


const unsigned short _MSG_ROLE		= _MSG_GENERAL + 50;
const unsigned short _MSG_ACCOUNT	= _MSG_GENERAL + 51;
const unsigned short _MSG_CONNECT	= _MSG_GENERAL + 52;
const unsigned short _MSG_DELETEROLE = _MSG_GENERAL + 53;
const unsigned short _MSG_BARACCOUNT = _MSG_GENERAL + 54;

const unsigned short _MSG_ACCOUNT91U = _MSG_GENERAL + 60;
const unsigned short _MSG_CONNECT91U = _MSG_GENERAL + 61;

const unsigned short _MSG_GM		= _MSG_GENERAL + 100;

const unsigned short _MSG_FEE		= _MSG_GENERAL + 200;		// paled
const unsigned short _MSG_QUERYFEE	= _MSG_GENERAL + 201;		// paled

const unsigned short _MSG_BATTLEACT	= _MSG_GENERAL + 1000;
const unsigned short _MSG_NORMALACT	= _MSG_GENERAL + 1001;
const unsigned short _MSG_SPECIALACT = _MSG_GENERAL + 1002;
const unsigned short _MSG_PETACT	= _MSG_GENERAL + 1003;
const unsigned short _MSG_ITEMACT	= _MSG_GENERAL + 1004;
const unsigned short _MSG_BATTLE	= _MSG_GENERAL + 1005;
const unsigned short _MSG_OPPONENT	= _MSG_GENERAL + 1007;
const unsigned short _MSG_FRIEND	= _MSG_GENERAL + 1008;

const unsigned short _MSG_BATTLERESULT	= _MSG_GENERAL + 1009;
const unsigned short _MSG_PLAYERLEVELUP	= _MSG_GENERAL + 1010;
const unsigned short _MSG_PETLEVELUP	= _MSG_GENERAL + 1011;

const unsigned short _MSG_TEAM		= _MSG_GENERAL + 1020;
const unsigned short _MSG_TEAMINFO	= _MSG_GENERAL + 1021;


// msg attribute define
const unsigned short _TXTATR_NORMAL		= 2000;
const unsigned short _TXTATR_PRIVATE	= _TXTATR_NORMAL + 1;
const unsigned short _TXTATR_ACTION		= _TXTATR_NORMAL + 2;
const unsigned short _TXTATR_REGISTER	= _TXTATR_NORMAL + 10;
const unsigned short _TXTATR_ENTRANCE	= _TXTATR_NORMAL + 11;
const unsigned short _TXTATR_SHOP		= _TXTATR_NORMAL + 12;
const unsigned short _TXTATR_TEAM		= _TXTATR_NORMAL + 13;
const unsigned short _TXTATR_GLOBAL		= _TXTATR_NORMAL + 14;


// msg style define
const unsigned short _TXT_NORMAL	= 0;
const unsigned short _TXT_SCROLL	= 0x0001;
const unsigned short _TXT_FLASH		= 0x0002;
const unsigned short _TXT_BLAST		= 0x0008;


// color define
const unsigned long	_COLOR_WHITE	= 0x00ffffff;
const unsigned long	_COLOR_RED		= 0x00ff0000;
const unsigned long	_COLOR_GREEN	= 0x0000ff00;
const unsigned long	_COLOR_BLUE		= 0x000000ff;
const unsigned long	_COLOR_GRAY		= 0x00999999;
const unsigned long _COLOR_YELLOW	= 0x00ffff00;

// object define
const char szSYSTEM[]		= "SYSTEM";
const char szTEAMMATES[]	= "TEAMMATES";
const char szROOMMATES[]	= "ROOMMATES";
const char szALLUSERES[]	= "ALLUSERES";

// battle info define
const DWORD	_IDMSK_PET		= 0x80000000;
const DWORD _IDMSK_MONSTER	= 0xc0000000;

enum {OBJ_NONE = 1234, OBJ_MONSTER, OBJ_PET, OBJ_USER, OBJ_ITEM};

enum {OBJTYPE_MONSTER = 0, OBJTYPE_VSPET, OBJTYPE_VSPLAYER, OBJTYPE_FRIENDPET, OBJTYPE_FRIENDPLAYER};

// version define
const int _VERSION	= 1000;

// struct define
typedef struct
{
    int	 			nAction;
    int				nTurn;
    OBJID			idTarget;
    OBJID			idObj;		//(idSkill/idItem/idPet)
} BattleActStruct, *pBattleActStruct;


#endif
