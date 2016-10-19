
//**********************************************************
// 代码编辑器
//**********************************************************

// define.h 全局类型和常量定义
// 仙剑修，2002.10.29
//////////////////////////////////////////////////////////////////////

#pragma	warning(disable:4786)

#ifndef	ALL_GLOBAL_DEFINE_H
#define	ALL_GLOBAL_DEFINE_H

#include "common.h"
#include "mycom.h"

// #define ENG_RC
#ifdef ENG_RC
#include "eng_rc.h"
#else
#include "chn_rc.h"
#endif

//////////////////////////////////////////////////////////////////////
// 版本号
const int	GAME_VERSION	= 100;		// 主版本号，客户端和服务器必须同步才能运行。
const int	SERVER_VERSION	= 100;		// 服务器版本号，SERVER每次更新时都可更改，用于显示。
const int	NPCSERVER_VERSION	= 987654321;	// NPC服务器的特殊版本号
const int	NPCSERVER_VERSION_RELOGIN	= 987654322;	// NPC服务器的特殊版本号

//////////////////////////////////////////////////////////////////////
// NPC服务器帐号
#define		NPCSERVER_ACCOUNT 		"NpcServer Tqdig"			// NPC服务器登录名
#define		NPCSERVER_PASSWORD		"skl8934##VJICW "			// NPC服务器密码

//////////////////////////////////////////////////////////////////////
// 通用常量
const int	IPSTR_SIZE			= 16;				// IP串空间
const int	_MAX_NAMESIZE		= 16;
const int	MAX_FIELDSIZE		= 1024;				// 数据库中串域的长度

#define		SYSTEM_NAME 		"SYSTEM"				// 广播消息时代表“系统”的名字串
#define		ALLUSERS_NAME		"ALLUSERS"				// TALK中代表“大家”的名字串
#define		REPLAY_OK_STR		"ANSWER_OK"			// 申请新帐号或LOGIN时返回的成功标志串。
#define		REPLAY_AGAIN_STR	"ANSWER_AGAIN"		// NPC_SERVER LOGIN时返回的重复登录标志串。

//////////////////////////////////////////////////////////////////////
// 配置
#define	CONFIG_FILENAME		"shell.ini"
const int	DBSTR_SIZE				= 16;			// 数据库INI串长度
const int	_MAX_MSGSIZE			= 1024;			// 消息包尺寸
const int	MAX_PACKETSIZE			= 1024;			// 网络消息包尺寸
const int	MAX_MESSAGESIZE			= MAX_PACKETSIZE + 24;			// 内部消息包尺寸
const int	SERVER_LISTEN_PORT		= 5816;			// 服务器监听端口

//////////////////////////////////////////////////////////////////////
// 运行控制
const int	RESEND_MSGLOGIN_SECS		= 5;			// 登录到游戏服务器的速度

//////////////////////////////////////////////////////////////////////
// 类型
typedef	unsigned long	OBJID;
typedef	unsigned long	PROCESS_ID;						// 通常与内部消息接口ID相同
typedef	long			SOCKET_ID;						// 玩家对应的SOCKET_ID，通常为SET的索引号
typedef unsigned __int64	I64;

typedef	char			IPSTR[IPSTR_SIZE];
typedef	char			NAMESTR[_MAX_NAMESIZE];
typedef	char			SQLBUF[1024];
typedef	char			MSGBUF[1024];
typedef	char			NETMSGBUF[1024];				// 网络包的BUF
typedef	char			MESSAGESTR[MAX_MESSAGESIZE];
typedef	char			FIELDBUF[MAX_FIELDSIZE];

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
const int	INVALID_ID		= -1;				// 无效的SOCKET索引
const int	INVALID_ROOMID	= -1;				// 无效的ROOM_ID
const int	BCAST_NPCID		= 0;				// 发给NPC服务器的广播，由NPC服务器转发给相关NPC。

const bool	UPDATE_TRUE				= true;
const bool	UPDATE_FALSE			= false;
const bool	DELRECORD_TRUE			= true;
const bool	DELRECORD_FALSE			= false;
const bool	SYNCHRO_TRUE			= true;
const bool	SYNCHRO_FALSE			= false;

//////////////////////////////////////////////////////////////////////
// ID空间
const OBJID	SCENEID_FIRST		= 000001;			// SCENE的第一个ID
const OBJID	SYSNPCID_FIRST		= 00001;			// SYSNPC的第一个ID		// fixed to database id
const OBJID	SYSNPCID_LAST		= 99999;			// SYSNPC的最后一个ID	// fixed to database id
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
const OBJID SYSTRAPID_FIRST		= 990001;			// 系统陷阱的第一个ID
const OBJID SYSTRAPID_LAST		= 999999;			// 系统陷阱的最后一个ID
const OBJID TRAPID_LAST			= 999999;			// 魔法陷阱的最后一个ID

const OBJID PLAYER_ID_FIRST		= 1000000;			// Player的第一个ID
const OBJID PLAYER_ID_LAST		= 3999999999;		// Player的最后一个ID

const OBJID CLIENT_ID_FIRST		= 4000000000;		// 为客户端保留的ID空间

//////////////////////////////////////////////////////////////////////
// 编译控制
#define	MSGHEAD_DEFINE
//USHORT		unMsgSize;
//USHORT		unMsgType;

//////////////////////////////////////////////////////////////////////
// 彩江常量
const int	DEFAULT_LOGIN_MAPGROUP		= 0;		// 登录点
const int	DEFAULT_LOGIN_MAPID			= 1000;		// 登录点
const int	DEFAULT_LOGIN_POSX			= 44;		// 登录点
const int	DEFAULT_LOGIN_POSY			= 44;		// 登录点
const int	DEFAULT_LOGIN_DIR			= 0;
const int	DEFAULT_LOGIN_STATUS		= 0;		// STATUS_NORMAL
const int	DEFAULT_LOGIN_POSE			= 0;
const int	DEFAULT_LOGIN_EMOTION		= 0;

const int	CELLS_PER_BLOCK				= 18;		// 每ROOM的格子数
const int	CELLS_PER_VIEW				= 18;		// 可视区域的格子数
const int	BLOCKWIDTH_PER_MAP			= 100;		// 地图X方向ROOM数量，不宜过大，空耗内存

const int	DEFAULT_MAP_WIDTH			= 50;		// 每张地图的ROOM数
const int	DEFAULT_MAP_HEIGHT			= 50;

const int	MAPS_PER_MAPGROUP			= 1;

enum
{
    OBJ_NONE	= 1234,
    OBJ_USER,
    OBJ_ITEM,
    OBJ_MAP,
    OBJ_FRIEND,
    OBJ_NPC,
    OBJ_NPCGEN,
    OBJ_NPCTYPE,
    OBJ_AGENT,
};

class CUser;
class CNpc;
class CAgent;
GUID_DECLARE(CUser, OBJ_USER)
GUID_DECLARE(CNpc, OBJ_NPC)
GUID_DECLARE(CAgent, OBJ_AGENT)

#define NOMATE_NAME		"无"						// 无配偶的配偶名

#define	SYNNAME_NONE	"无"						// 无帮派玩家的帮派名
#define	SYNLEADER_TITLE	"帮主"						// 帮主的TITLE
#define	SYNMEMBER_TITLE	"普通帮众"						// 新入帮玩家的TITLE
#define	NOSYN_TITLE		"无"						// 无帮派玩家的TITLE

//////////////////////////////////////////////////////////////////////
// 征服常量
const int	STATESTR_SIZE		= 2048;


//////////////////////////////////////////////////////////////////////
// safe ptr
////////////////////////////////////////////////////////////////////////
class CUser;
typedef	CUser*	CUserPtr;
//typedef	TSafePtr<class CUser, OBJ_USER>		CUserPtr;
class CItem;
typedef	CItem*	CItemPtr;
//typedef	TSafePtr<class CItem, OBJ_ITEM>		CItemPtr;
class CGameMap;
typedef	CGameMap*	CMapPtr;
//typedef	TSafePtr<class CGameMap, OBJ_MAP>	CMapPtr;
class CFriend;
typedef	CFriend*	CFriendPtr;
//typedef	TSafePtr<class CFriend, OBJ_FRIEND>	CFriendPtr;
#define	CNpcPtr			INpc*
//typedef	TSafePtr<class CNpc, OBJ_NPC>		CNpcPtr;
class CNpcGenerator;
typedef	CNpcGenerator*	CGenPtr;
//typedef	TSafePtr<class CNpcGenerator, OBJ_NPCGEN>		CGenPtr;
class CNpcType;
typedef	CNpcType*	CNpcTypePtr;
//typedef	TSafePtr<class CNpcType, OBJ_NPCTYPE>		CNpcTypePtr;

//////////////////////////////////////////////////////////////////////
/*class CNetMsg;
class ISocket
{
public:
	virtual bool SendMsg			(CNetMsg* pNetMsg)				PURE_VIRTUAL_FUNCTION_0
	virtual bool SendMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen)	PURE_VIRTUAL_FUNCTION_0
	virtual bool SendNpcMsg			(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen)			PURE_VIRTUAL_FUNCTION_0
	// 核心通知SOCKET关闭
	virtual bool CloseSocket		(SOCKET_ID idSocket)			PURE_VIRTUAL_FUNCTION_0
};
*/
typedef	void			CGameSocket;		// 兼容于幻灵的消息代码

///////////////////////////////////////////////////////////////////////
// 行走方向
const int _DELTA_X[9] = { 0, -1, -1, -1, 0, 1, 1, 1, 0 };
const int _DELTA_Y[9] = { 1, 1, 0, -1, -1, -1, 0, 1, 0 };
const int MAX_DIRSIZE = 8;
inline int GetDirByPos(int nFromX, int nFromY, int nToX, int nToY)		// return MAX_DIRSIZE: error
{
    if (nFromX < nToX)
    {
        if (nFromY < nToY)
        {
            return 7;
        }
        else if (nFromY > nToY)
        {
            return 5;
        }
        else
        {
            return 6;
        }
    }
    else if (nFromX > nToX)
    {
        if (nFromY < nToY)
        {
            return 1;
        }
        else if (nFromY > nToY)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    else // if (nFromX == nToX)
    {
        if (nFromY < nToY)
        {
            return 0;
        }
        else if (nFromY > nToY)
        {
            return 4;
        }
    }
    return MAX_DIRSIZE;
}


#endif // ALL_GLOBAL_DEFINE_H
