
//**********************************************************
// 代码编辑器
//**********************************************************

// define.h 全局类型和常量定义
// 仙剑修，2002.10.29
//////////////////////////////////////////////////////////////////////

#ifndef	ALL_GLOBAL_DEFINE_H
#define	ALL_GLOBAL_DEFINE_H

#pragma warning(disable:4786)

#include "common.h"
#include "myDDstring.h"
#include "Myheap.h"

//////////////////////////////////////////////////////////////////////
// 版本号
const int	GAME_VERSION	= 108;		// 主版本号，客户端和服务器必须同步才能运行。
const int	SERVER_VERSION	= 107;		// 服务器版本号，SERVER每次更新时都可更改，用于显示。
const int	NPCSERVER_VERSION	= 987654321;	// NPC服务器的特殊版本号
const int	NPCSERVER_VERSION_RELOGIN	= 987654322;	// NPC服务器的特殊版本号

#define		NEW_LOGOUT		// for new logout plan

//////////////////////////////////////////////////////////////////////
// NPC服务器帐号
#define		NPCSERVER_ACCOUNT 		"NpcServer Tqdig"			// NPC服务器登录名
#define		NPCSERVER_PASSWORD		"skl8934##VJICW "			// NPC服务器密码

//////////////////////////////////////////////////////////////////////
// 帐号服务器相关常量
const int	LOGIN_FREE_LEVEL	= 16;			// 点卡收费的等级，小于该值不收费。
#define	FLAG_ISP_TO_NORMAL	"!ISP计费失败!"		// 注意：该字符串必须与游戏服务器相匹配。每个IP限一个帐号。否则自动转普通帐号并提示
#define	FLAG_NO_POINT		"!NO_POINT!"		// 注意：该字符串必须与游戏服务器相匹配。没有点数也可登录。由游戏服务器判断玩家等级是否可免费玩

//////////////////////////////////////////////////////////////////////
// 通用常量
const int	IPSTR_SIZE			= 16;				// IP串空间
const int	_MAX_NAMESIZE		= 16;
const int	MAX_FIELDSIZE		= 256;				// 存串型字段的空间大小。mysql 的串类型字段最大长度+1
const int	MAX_PARAMSIZE		= 256;				// action param space
const int	MAX_TENETSIZE		= 256;				// announce

#define		SYSTEM_NAME 		"SYSTEM"			// 广播消息时代表“系统”的名字串
#define		ALLUSERS_NAME		"ALLUSERS"			// TALK中代表“大家”的名字串
#define		REPLAY_OK_STR		"ANSWER_OK"			// 申请新帐号或LOGIN时返回的成功标志串。
#define		REPLAY_AGAIN_STR	"ANSWER_AGAIN"		// NPC_SERVER LOGIN时返回的重复登录标志串。

//////////////////////////////////////////////////////////////////////
// 配置
#define	CONFIG_FILENAME		"shell.ini"
const int	DBSTR_SIZE				= 16;			// 数据库INI串长度
const int	MAX_PACKETSIZE			= 1024;			// 网络消息包尺寸
const int	MAX_MESSAGESIZE			= MAX_PACKETSIZE + 24;			// 内部消息包尺寸
const int	SERVER_LISTEN_PORT		= 5816;			// 服务器监听端口
const int	POINTFEE_SECS			= 3 * 60;			// 计点频率
const int	POINTFEE_DELAY_SECS		= 10;			// 首次计点延时
const int	SOCKET_SENDBUFSIZE		= 64 * 1024;			// SOCKET_BUF
const int	SOCKET_NPCBUFSIZE		= 128 * 1024;			// SOCKET_BUF
const int	SOCKET_ACCOUNTBUFSIZE	= 32 * 1024;			// SOCKET_BUF
const int	MAX_MAPGROUPSIZE		= 10;				// 最多10个地图组

//////////////////////////////////////////////////////////////////////
// 类型
typedef	unsigned long	OBJID;
typedef	unsigned long	PROCESS_ID;						// 通常与内部消息接口ID相同
typedef	long			SOCKET_ID;						// 玩家对应的SOCKET_ID，通常为SET的索引号
#define	IPP_OF(x)		((void**)&(x))
typedef unsigned __int64	I64;

typedef	char			NAMESTR[_MAX_NAMESIZE];
typedef	char			SQLBUF[1024];
typedef	char			MSGBUF[1024];
typedef	char			MESSAGESTR[MAX_MESSAGESIZE];
typedef	char			PARAMBUF[MAX_PARAMSIZE];
typedef	char			TENETSTR[MAX_TENETSIZE];

//////////////////////////////////////////////////////////////////////
// 接口常量
const int	GAMESERVER_INTERPORT	= 0;			// 游戏服务器网络接口(主接口)

enum
{
    MSGPORT_SHELL			= 0,			// 对话框接口
    MSGPORT_SOCKET			= 1,			// 底层通讯接口
    MSGPORT_WORLD			= 2,			// 内部主接口(全局进程)
    MSGPORT_MAPGROUP_FIRST	= 3,			// 第一个地图组接口
};

//////////////////////////////////////////////////////////////////////
// 核心常量
const int	ID_NONE			= 0;
const int	SOCKET_NONE		= -1;				// 无效的SOCKET索引
const int	PROCESS_NONE	= -1;				// 无效的PROCESS索引
const int	INVALID_ROOMID	= -1;				// 无效的ROOM_ID
const int	BCAST_NPCID		= 0;				// 发给NPC服务器的广播，由NPC服务器转发给相关NPC。
const int	INDEX_NONE		= -1;

const bool	UPDATE_TRUE				= true;
const bool	UPDATE_FALSE			= false;
const bool	DELRECORD_TRUE			= true;
const bool	DELRECORD_FALSE			= false;
const bool  INCLUDE_SELF			= true;
const bool  EXCLUDE_SELF			= false;
enum {SYNCHRO_FALSE = false, SYNCHRO_TRUE = true, SYNCHRO_BROADCAST};

//////////////////////////////////////////////////////////////////////
// 编译控制
#define	MSGHEAD_DEFINE
//USHORT		unMsgSize;
//USHORT		unMsgType;

//////////////////////////////////////////////////////////////////////
// ID空间
const OBJID	SCENEID_FIRST		= 000001;			// SCENE的第一个ID
const OBJID	SYSNPCID_FIRST		= 00001;			// SYSNPC的第一个ID			// fixed to database id
const OBJID	SYSNPCID_LAST		= 99999;			// SYSNPC的最后一个ID		// fixed to database id
const OBJID	DYNANPCID_FIRST		= 100001;			// DYNANPC的第一个ID
const OBJID	DYNANPCID_LAST		= 199999;			// DYNANPC的最后一个ID
const OBJID	SCENEID_LAST		= 299999;			// SCENE的最后一个ID

const OBJID	NPCSERVERID_FIRST	= 400001;			// NPC的第一个ID
const OBJID	MONSTERID_FIRST		= 400001;			// MONSTER的第一个ID
const OBJID	MONSTERID_LAST		= 499999;			// MONSTER的最后一个ID
const OBJID	PETID_FIRST			= 500001;			// PET的第一个ID
const OBJID	PETID_LAST			= 599999;			// PET的最后一个ID
const OBJID	NPCSERVERID_LAST	= 699999;			// NPC的最后一个ID

const OBJID	CALLPETID_FIRST		= 700001;			// CALLPET的第一个ID
const OBJID	CALLPETID_LAST		= 799999;			// CALLPET的最后一个ID

const OBJID TRAPID_FIRST		= 900001;			// 魔法陷阱的第一个ID
const OBJID MAGICTRAPID_FIRST	= 900001;			// 玩家魔法的第一个ID
const OBJID MAGICTRAPID_LAST	= 989999;			// 玩家魔法的最后一个ID
const OBJID SYSTRAPID_FIRST		= 990001;			// 系统陷阱的第一个ID		// fixed to database id
const OBJID SYSTRAPID_LAST		= 999999;			// 系统陷阱的最后一个ID		// fixed to database id
const OBJID TRAPID_LAST			= 999999;			// 魔法陷阱的最后一个ID

const OBJID PLAYER_ID_FIRST		= 1000000;			// Player的第一个ID
const OBJID PLAYER_ID_LAST		= 1999999999;		// Player的最后一个ID

const OBJID EUDEMON_ID_FIRST	= 2000000000;		// 幻兽的最后一个ID
const OBJID EUDEMON_ID_LAST		= 3999999999;		// 幻兽的最后一个ID

const OBJID CLIENT_ID_FIRST		= 4000000000;		// 为客户端保留的ID空间

//////////////////////////////////////////////////////////////////////
// 征服常量
const int	DEFAULT_LOGIN_MAPGROUP		= 0;		// 登录点
const int	DEFAULT_LOGIN_MAPID			= 1000;		// 登录点
const int	DEFAULT_LOGIN_POSX			= 362;		// 登录点
const int	DEFAULT_LOGIN_POSY			= 594;		// 登录点

const int	CELLS_PER_BLOCK				= 18;		// 每ROOM的格子数
const int	CELLS_PER_VIEW				= 18;		// 可视区域的格子数
const int	ITEMTYPEMONEY_MIN			= 1090000;	// 最小堆钱
const int	ITEMTYPEMONEY_MAX			= 1091020;	// 最大堆钱
const int	SMALLHEAPMONEY_LIMIT		= 10;		// 小堆钱
const int	SMALLHEAPMONEY_TYPE			= 1090000;		// 小堆钱ITEMTYPE
const int	MIDDLEHEAPMONEY_LIMIT		= 100;		// 中堆钱
const int	MIDDLEHEAPMONEY_TYPE		= 1090010;		// 中堆钱ITEMTYPE
const int	BIGHEAPMONEY_LIMIT			= 1000;		// 大堆钱
const int	BIGHEAPMONEY_TYPE			= 1090020;		// 大堆钱ITEMTYPE

const int	SMALLHEAPGOLD_LIMIT			= 2000;		// 小堆金子
const int	SMALLHEAPGOLD_TYPE			= 1091000;		// 小堆金子ITEMTYPE
const int	MIDDLEHEAPGOLD_LIMIT		= 5000;		// 中堆金子
const int	MIDDLEHEAPGOLD_TYPE			= 1091010;		// 中堆金子ITEMTYPE
const int	BIGHEAPGOLD_LIMIT			= 10000;		// 大堆金子
const int	BIGHEAPGOLD_TYPE			= 1091020;		// 大堆金子ITEMTYPE

enum
{
    OBJ_NONE	= 0x1000,
    OBJ_USER	= 0x1001,
    OBJ_MONSTER	= 0x1002,
    OBJ_ITEM	= 0x1004,
    OBJ_MAP		= 0x1008,
    OBJ_FRIEND	= 0x1010,
    //	OBJ_NPCTYPE	= 0x1020,
    OBJ_NPC		= 0x1040,
    OBJ_MAPITEM	= 0x1080,
    OBJ_SYN		= 0x1100,
    OBJ_BOOTH	= 0x1200,
    OBJ_TRAP	= 0x1400,
    OBJ_TUTOR	= 0x1800,
};
inline bool IsObjType(OBJID idObjType, OBJID idUnion) { return (idObjType & idUnion & 0x0FFF) != 0; }

//////////////////////////////////////////////////////////////////////
// safe ptr
////////////////////////////////////////////////////////////////////////
class CUser;
typedef	CUser*	CUserPtr;
//typedef	TSafePtr<class CUser, OBJ_USER>			CUserPtr;
class CMonster;
typedef	CMonster*	CMonsterPtr;
//typedef	TSafePtr<class CMonster, OBJ_MONSTER>	CMonsterPtr;
class CItem;
typedef	CItem*	CItemPtr;
//typedef	TSafePtr<class CItem, OBJ_ITEM>			CItemPtr;
class CGameMap;
typedef	CGameMap*	CMapPtr;
//typedef	TSafePtr<class CGameMap, OBJ_MAP>		CMapPtr;
class CFriend;
typedef	CFriend*	CFriendPtr;
//typedef	TSafePtr<class CFriend, OBJ_FRIEND>		CFriendPtr;
class CNpc;
typedef	CNpc*	CNpcPtr;
//typedef	TSafePtr<class CNpc, OBJ_NPC>			CNpcPtr;
class CMapItem;
typedef	CMapItem*	CMapItemPtr;
//typedef	TSafePtr<class CMapItem, OBJ_MAPITEM>	CMapItemPtr;
class CSyndicate;
typedef	CSyndicate*	CSynPtr;
//typedef	TSafePtr<class CSyndicate, OBJ_SYN>		CSynPtr;

//////////////////////////////////////////////////////////////////////
// 通用接口
//////////////////////////////////////////////////////////////////////
#ifdef	WORLD_KERNEL
#else
class CNetMsg;
class ISocket
{
public:
    virtual bool SendMsg			(CNetMsg* pNetMsg)				PURE_VIRTUAL_FUNCTION_0
    virtual bool SendMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen)	PURE_VIRTUAL_FUNCTION_0
    virtual bool SendNpcMsg			(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen)			PURE_VIRTUAL_FUNCTION_0
    // 核心通知SOCKET关闭
    virtual bool CloseSocket		(SOCKET_ID idSocket)			PURE_VIRTUAL_FUNCTION_0
};

typedef	void			CGameSocket;		//? 兼容于幻灵的消息代码
#endif

#ifdef	PALED_DEBUG_X
#define	DEBUG_CREATEMSG(x,y,a,s,n1,n2)	//{MSGBUF D_CM;sprintf(D_CM,"◎ msg:%s,id:%d,action:%d,%s,%d,%d",x,y,a,s,n1,n2);CMsgTalk msg;if (msg.Create("DEBUG","DEBUG",D_CM))SendMsg(&msg);}
#define	DEBUG_SENDMSG(x,y)				{MSGBUF D_CM;sprintf(D_CM,"↓ msgtype:%d,socketid:%d",x,y);CMsgTalk msg;if (msg.Create("DEBUG","DEBUG",D_CM))SendMsg(&msg);}
#define	DEBUG_PROCESSMSG(x,y,a,s,n1,n2)	{MSGBUF D_CM;sprintf(D_CM,"↑ msgtype:%d,socketid:%d,msg:%s,id:%d,action:%d,%s,%d,%d",GetType(),GetSocketID(),x,y,a,s,n1,n2);CMsgTalk msg;if (msg.Create("DEBUG","DEBUG",D_CM))SendMsg(&msg);}
#else
#define	DEBUG_CREATEMSG(x,y,a,s,n1,n2)	//{MSGBUF D_CM;sprintf(D_CM,"◎ msg:%s,id:%d,action:%d,%s,%d,%d",x,y,a,s,n1,n2);CMsgTalk msg;if (msg.Create("DEBUG","DEBUG",D_CM))SendMsg(&msg);}
#define	DEBUG_SENDMSG(x,y)				{}//{MSGBUF D_CM;sprintf(D_CM,"↓ msgid:%d,socketid:%d",x,y);CMsgTalk msg;if (msg.Create("DEBUG","DEBUG",D_CM))SendMsg(&msg);}
#define	DEBUG_PROCESSMSG(x,y,a,s,n1,n2)	{}//{MSGBUF D_CM;sprintf(D_CM,"↑ msg:%s,id:%d,action:%d,%s,%d,%d",x,y,a,s,n1,n2);CMsgTalk msg;if (msg.Create("DEBUG","DEBUG",D_CM))SendMsg(&msg);}
#endif

inline void EmptyFunction(...) {}
#ifdef MULTI_KERNEL_LOG
#define LOGDEBUG	LOGMSG
#else
#define LOGDEBUG	EmptyFunction
#endif


#endif // ALL_GLOBAL_DEFINE_H
