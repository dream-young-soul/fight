
//**********************************************************
// 代码编辑器
//**********************************************************

// protocol.h
// 仙剑修，2002.10.23
//////////////////////////////////////////////////////////////////////

#ifndef	PROTOCOL_H
#define	PROTOCOL_H

#include <string.h>
#include "define.h"
#ifdef MAPGROUP_KERNEL
#include "UserData.h"
#endif

enum
{
    //////////////////////////////////////////////////////////////////////
    // 地图组或世界处理的消息
    KERNEL_BASE					= 1000,
    KERNEL_CLIENTMSG			= KERNEL_BASE + 1,				// CLIENTMSG_PACKET0(size:1024+12), 客户端上传的消息。
    KERNEL_NPCMSG				= KERNEL_BASE + 2,				// NPCMSG_PACKET0(size:1024+12), NPC服务器上传的消息。
    KERNEL_CLOSEKERNEL			= KERNEL_BASE + 3,				// INT, SOCKET_ID。通知核心关闭(核心必须再次通知SOCKET层!!!)
    KERNEL_CREATESYNDICATE		= KERNEL_BASE + 10,				// CreateSyndicateInfo
    KERNEL_DESTROYSYNDICATE		= KERNEL_BASE + 11,				// OBJID
    KERNEL_CHANGESYNDICATE		= KERNEL_BASE + 12,				// SynFuncInfo0
    KERNEL_COMBINESYNDICATE		= KERNEL_BASE + 13,				// OBJID+OBJID
    KERNEL_SUPERMANLIST			= KERNEL_BASE + 14,				// OBJID+int, to world: 添加或查询某人的排名。to mapgroup: 更新某人的排行
    KERNEL_QUERYSUPERMANLIST	= KERNEL_BASE + 15,				// OBJID+int, 下传排名名单。


    //////////////////////////////////////////////////////////////////////
    // 由地图组核心处理的消息
    MAPGROUP_BASE				= 10000,
    MAPGROUP_LOGIN				= MAPGROUP_BASE + 1,			// ST_LOGIN, 玩家登录。
    MAPGROUP_BROADCASTMSG		= MAPGROUP_BASE + 2,			// CLIENTMSG_PACKET0(size:1024+12), 转发广播消息。
    MAPGROUP_SOCKETUSERINFO		= MAPGROUP_BASE + 3,			// ST_USERCHANGEMAPGORUP。STRUCT(SOCKET_ID,UserInfoStruct)，玩家换地图组。
    MAPGROUP_SENDOBJINFO		= MAPGROUP_BASE + 4,			// CHANGEMAPGORUP_INFO0{id,type,info}
    MAPGROUP_TRANSMITMSG		= MAPGROUP_BASE + 5,			// TRANSMSG_PACKET0(size:1024+16), 转发广播消息。
    MAPGROUP_CHANGEMAPGROUP		= MAPGROUP_BASE + 10,			// ST_CHANGEMAPGROUP。玩家换地图组。
    MAPGROUP_CREATENEWNPC		= MAPGROUP_BASE + 11,			// ST_CREATENEWNPC。创建一个新NPC。
    MAPGROUP_CHANGETEAM			= MAPGROUP_BASE + 12,			// ST_CHANGETEAM, 转发TEAM修改OPERATION。
    MAPGROUP_REMOTECALL			= MAPGROUP_BASE + 13,			// REMOTE_CALL0, 转发给USER。
    MAPGROUP_CHANGENPC			= MAPGROUP_BASE + 14,			// int+str+str, 转发NPC修改OPERATION。
    MAPGROUP_DELTRANSNPC		= MAPGROUP_BASE + 15,			// int, 转发删除TRANS_NPC
    MAPGROUP_DELALLMONSTER		= MAPGROUP_BASE + 16,			// int, 删除所有怪物，以便接收新怪物。同时发到SOCKET_KERNRL
    MAPGROUP_SETMAPSYNID		= MAPGROUP_BASE + 17,			// idMap+idSyn, 修改某地图的占领帮派
    MAPGROUP_LOADALLPET			= MAPGROUP_BASE + 18,			// int, 重新装载所有帮派守护兽
    MAPGROUP_RPC				= MAPGROUP_BASE + 19,			// CEventPack, 转发给指定对象。
    MAPGROUP_TRANSMITMSG_FORCHATROOM = MAPGROUP_BASE + 20,		// 聊天室消息


    //////////////////////////////////////////////////////////////////////
    // 由世界核心处理的消息
    WORLD_BASE					= 20000,
    WORLD_SHELLTALK				= WORLD_BASE + 1,				// STRING, 外壳发送TALK消息。
    WORLD_SETPROCESSID			= WORLD_BASE + 2,				// CHANGE_USERDATA, 设置新的PROCESS_ID
    WORLD_CLOSESOCKET			= WORLD_BASE + 3,				// INT, SOCKET_ID，核心通知SOCKET关闭(SOCKET不再通知核心)
    WORLD_USERNAMEMSG			= WORLD_BASE + 4,				// USERNAMEMSG_PACKET0(size:1024+16+12), 按名字转发给玩家。
    WORLD_USERIDMSG				= WORLD_BASE + 5,				// USERIDMSG_PACKET0(size:1024+4+12), 按ID转发给玩家。
    WORLD_LEVELUP				= WORLD_BASE + 6,				// OBJID, 玩家升到10级。
    WORLD_QUERYFEE				= WORLD_BASE + 7,				// OBJID, ACCOUNT ID。
    WORLD_SENDTIME				= WORLD_BASE + 8,				// INT, SOCKET_ID
    WORLD_TRANSMITMSG			= WORLD_BASE + 9,				// TRANSMSG_PACKET0(size:1024+16), 转发消息。


    //////////////////////////////////////////////////////////////////////
    // 由SOCKET核心处理的消息
    SOCKET_BASE					= 30000,
    SOCKET_CLOSESOCKET			= SOCKET_BASE + 1,				// INT, SOCKET_ID，核心通知SOCKET关闭(SOCKET不再通知核心)
    SOCKET_SETPROCESSID			= SOCKET_BASE + 2,				// CHANGE_USERDATA, 设置新的PROCESS_ID
    //!!! 核心发送该消息时，要同时更新CUSER中的PROCESS_ID标志。核心在处理KERNEL_CLIENTMSG时，一定要检查PROCESS_ID是否匹配!!!
    SOCKET_SENDCLIENTMSG		= SOCKET_BASE + 3,				// SENDCLIENTMSG_PACKET0(size:1024+12), 发向客户端和NPC服务器的消息。
    SOCKET_SENDNPCMSG			= SOCKET_BASE + 4,				// SENDNPCMSG_PACKET0(size:1024+12), 发向客户端和NPC服务器的消息。
    SOCKET_SETNPCSOCKETID		= SOCKET_BASE + 5,				// INT, SOCKET_ID, 世界核心接收到NPC服务器的登录消息时，通知SOCKET核心。(可能为SOCKET_NONE)
    SOCKET_SETNPCPROCESSID		= SOCKET_BASE + 6,				// CHANGE_NPCPROCESSID, 设置新的PROCESS_ID
    SOCKET_CHANGECODE			= SOCKET_BASE + 7,				// CHANGE_USERDATA。修改动态密码。
    SOCKET_BREAKCONNECT			= SOCKET_BASE + 8,				// INT, SOCKET_ID, 踢玩家时，先断开SOCKET连接。
    SOCKET_BREAKALLCONNECT		= SOCKET_BASE + 11,				// INT, GM_SOCKET_ID, 踢所有玩家，禁止所有连接。


    //////////////////////////////////////////////////////////////////////
    // 由外壳处理的消息
    SHELL_BASE					= 40000,
    SHELL_PRINTTEXT				= SHELL_BASE + 1,				// STRING, 核心发送的TEXT消息
    SHELL_KERNELSTATE			= SHELL_BASE + 2,				// STRING, 核心发送的STATE串，多行


    //////////////////////////////////////////////////////////////////////
    // 由数据库核心处理的消息
    DB_BASE						= 50000,
    DB_QUERY_USERINFO			= DB_BASE + 1,

    DB_MSG_BASEINFO				= DB_BASE + 101,
    DB_MSG_FRIENDS				= DB_BASE + 102,
    DB_MSG_ENEMYS				= DB_BASE + 103,
    DB_MSG_ITEMS				= DB_BASE + 104,
    DB_MSG_WEAPON_SKILL			= DB_BASE + 105,
    DB_MSG_MAGICS				= DB_BASE + 106,
    DB_MSG_SYNATTR				= DB_BASE + 107,
};

struct CLIENTMSG_PACKET0			// 接收的CLIENT消息
{
    SOCKET_ID		idSocket;			// SOCKET_ID
    OBJID	idPacket;
    int		nSize;
    char	buf[1];				// 用于结构指针，不需要空间
};

struct NPCMSG_PACKET0			// 接收的NPC消息
{
    OBJID	idNpc;
    OBJID	idPacket;
    int		nSize;
    char	buf[1];				// 用于结构指针，不需要空间
};

struct SENDCLIENTMSG_PACKET0
{
    SOCKET_ID		idSocket;			// SOCKET_ID
    OBJID	idPacket;
    int		nSize;
    char	buf[1];				// 用于结构指针，不需要空间
};

struct SENDNPCMSG_PACKET0
{
    OBJID	idNpc;				// 广播消息用BCAST_NPCID
    OBJID	idPacket;
    int		nSize;
    char	buf[1];				// 用于结构指针，不需要空间
};

struct TRANSMSG_PACKET0			// 转发给其它进程处理的消息
{
    SOCKET_ID		idSocket;			// SOCKET_ID
    OBJID			idNpc;				// 同时支持NPC的消息
    OBJID	idPacket;
    int		nSize;
    int		nTrans;				// 附加数据
    char	buf[1];				// 用于结构指针，不需要空间
};

struct USERNAMEMSG_PACKET0			// 让WORLD转发的消息，按名字转发
{
    SOCKET_ID		idSocket;			// SOCKET_ID
    OBJID	idPacket;
    int		nSize;
    char	szName[_MAX_NAMESIZE];		// 转发给该玩家
    char	buf[1];				// 用于结构指针，不需要空间
};

struct USERIDMSG_PACKET0			// 让WORLD转发的消息，按ID转发
{
    SOCKET_ID		idSocket;			// SOCKET_ID
    OBJID	idPacket;
    int		nSize;
    OBJID	idUser;				// 转发给该玩家
    char	buf[1];				// 用于结构指针，不需要空间
};


struct CHANGE_USERDATA
{
    SOCKET_ID	idSocket;
    int			nData;
};

struct CHANGE_NPCPROCESSID
{
    SOCKET_ID	idNpc;
    PROCESS_ID	idProcess;		// 为SOCKET_NONE时为删除
};

struct ST_LOGIN
{
    SOCKET_ID	idSocket;
    OBJID		idUser;
};

#ifdef MAPGROUP_KERNEL
struct ST_USERCHANGEMAPGORUP
{
    SOCKET_ID	idSocket;
    UserInfoStruct		info;
};
#endif

struct CHANGEMAPGORUP_INFO0
{
    OBJID		idUser;
    int			nInfoType;
    char		info[1];
};

struct CHANGE_MAPGROUP
{
    OBJID		idUser;
    PROCESS_ID	idProcess;
};

struct ST_CHANGEMAPGROUP
{
    OBJID		idUser;
    OBJID		idMap;
    int			nPosX;
    int			nPosY;
};

struct ST_CREATENEWNPC
{
    OBJID			id;
    OBJID			idMap;
    OBJID			idData;
    OBJID			idOwner;
    int				nData;
    unsigned short	usPosX;
    unsigned short	usPosY;
    unsigned short	usAction;
    unsigned short	usType;
    unsigned char	ucOwnerType;
};

struct CreateSyndicateInfo
{
    OBJID		idSyn;
    NAMESTR		szName;
    TENETSTR	szTenet;
    NAMESTR		szLeaderTitle;
    NAMESTR		szMemberTitle;
    OBJID		idLeader;
    NAMESTR		szLeader;
    int			nMoney;
    OBJID		idFealty;
    int			nMantle;
    int 		nSynFlag;//0:总团，1：分团，2：分队
};

struct	SynFuncInfo0
{
    OBJID			idSyn;
    union
    {
        struct
        {
            int		nRank;				//帮派成员的军衔
            int		nSynMemberLevel;	//帮派成员级别
        };
        struct
        {
            DWORD	dwData[2];
        };
    };
    unsigned short	nSize;
    unsigned char	ucFuncType;
    unsigned char	ucSaveFlag;
    union
    {
        long		IntParam[1];
        char		StrParam[1];
    };
};

struct ST_CHANGETEAM
{
    int				nAction;
    OBJID			idTeam;
    OBJID			idUser;
    int				nData;		// idSocket or (x,y)
};

struct	REMOTE_CALL0
{
    OBJID			idUser;
    unsigned short	nSize;
    unsigned char	ucFuncType;
    union
    {
        long		IntParam[1];
        char		StrParam[1];
    };
};

struct	VARIANT_SET0
{
    long		m_setLong[1];
    long&		IntParam(int idx)			{ return m_setLong[idx]; }
    char*		StrParam(int idx)			{ return (char*)&m_setLong[idx]; }
};


/////////////////////////////////////////
// DBService用	-- zlong 2004.09.06

struct	ST_QUERY_USERINFO
{
    OBJID	idUser;
};

const int		_MEDAL_BUFSIZE		= 64 + 1;
const int		_TITLE_BUFSIZE		= 64 + 1;

struct	ST_DB_USERINFO
{
    OBJID	idUser;

    // 以下为了4字节对齐，没有按照原来的顺序
    DWORD	dwCoin_money;
    DWORD	dwLast_login;
    DWORD	dwLookface;
    DWORD	dwHair;
    char	setMedal[_MEDAL_BUFSIZE];
    UCHAR	ucLevel;
    UCHAR	ucMetempsychosis;
    UCHAR	ucNobility;
    DWORD	dwMedal_select;

    char	setTitle[_TITLE_BUFSIZE];
    UCHAR	ucProfession;
    UCHAR	ucAdditional_point;
    UCHAR	ucAuto_allot;
    DWORD	dwTitle_select;

    DWORD	dwMoney;
    DWORD	dwMoney_saved;
    USHORT	usRecordX;
    USHORT	usRecordY;
    //OBJID	id;
    OBJID	idAccount;
    OBJID	idRecordmap;
    OBJID	idSyndicate;
    int		nDeed;
    int		nExp;

    DWORD	dwTaskMask;
    DWORD	dwVirtue;
    OBJID	idHome;
    DWORD	dwLockKey;
    //DWORD	dwCurrLockKey;
    char	szMate[_MAX_NAMESIZE];
    char	szName[_MAX_NAMESIZE];
    USHORT	usForce;
    USHORT	usConstitution;
    USHORT	usHealth;
    USHORT	usLife;
    USHORT	usMana;
    USHORT	usSoul;
    USHORT	usSpeed;
    SHORT	sPk;
};

struct	ST_DB_FRIEND_INFO
{
    OBJID	id;
    OBJID	idUser;
    OBJID	idFriend;
    char	szFriendName[_MAX_NAMESIZE];
};

struct	ST_DB_FRIEND
{
    OBJID	idUser;

    int		nAmount;
    ST_DB_FRIEND_INFO	setFriends[1];
};

struct	ST_DB_ENEMY
{
    OBJID	idUser;
};

struct	ST_DB_ITEMINFO
{
    OBJID	id;

};
struct	ST_DB_ITEM
{
    OBJID	idUser;

    int		nAmount;
    ST_DB_ITEMINFO	setItems[1];
};

struct	ST_DB_WEAPON_SKILL
{
    OBJID	idUser;
};

struct	ST_DB_MAGIC
{
    OBJID	idUser;
};

struct	ST_DB_SYNATTR
{
    OBJID	idUser;
};



//////////////////////////////////////////////////////////////////////
// C/S 协议的客户端部分

// 该部分常量、定义仅由以下两个函数使用
const int	PROTOCOL_MSG_HEADSIZE		= 4;
struct CProtocolMsgStruct
{
    unsigned short	nMsgSize;
    unsigned short	idPacket;
    char			pMsg[1];
};

// 分解消息包
// return: netpacket size
inline int	SplitPacket(const char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize)
{
    if (nBufLen > PROTOCOL_MSG_HEADSIZE)
    {
        CProtocolMsgStruct*	pMsgPtr = (CProtocolMsgStruct*)pBuf;
        if (pMsgPtr->nMsgSize <= nBufLen)
        {
            if (pMsgPtr->nMsgSize < 4 || pMsgPtr->nMsgSize > MAX_PACKETSIZE)
            {
                return 0;
            }
            *pidPacket	= pMsgPtr->idPacket;
            *pMsgSize	= pMsgPtr->nMsgSize - PROTOCOL_MSG_HEADSIZE;
            *ppMsg		= pMsgPtr->pMsg;
            return pMsgPtr->nMsgSize;
        }
    }
    return 0;
}

// 合成消息包
// return: netpacket size
inline int	UnitePacket(char* pBuf, int nBufLen, OBJID idPacket, const char* pMsg, int nMsgSize)
{
    if (nBufLen >= nMsgSize + PROTOCOL_MSG_HEADSIZE)
    {
        CProtocolMsgStruct*	pMsgPtr = (CProtocolMsgStruct*)pBuf;
        pMsgPtr->idPacket		= (unsigned short)idPacket;
        pMsgPtr->nMsgSize		= nMsgSize + PROTOCOL_MSG_HEADSIZE;
        memcpy(pMsgPtr->pMsg,	pMsg, (size_t)nMsgSize);
        return pMsgPtr->nMsgSize;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////
// C/S 协议的NPC部分

// 该部分常量、定义仅由以下两个函数使用
const int	PROTOCOL_NPCMSG_HEADSIZE		= 8;
struct CProtocolNpcMsgStruct
{
    unsigned short	nMsgSize;
    unsigned short	idPacket;
    unsigned long	idNpc;
    char			pMsg[1];
};

// 分解消息包
// return: netpacket size
inline int	SplitNpcPacket(const char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize, OBJID* pidNpc)
{
    if (nBufLen > PROTOCOL_NPCMSG_HEADSIZE)
    {
        CProtocolNpcMsgStruct*	pMsgPtr = (CProtocolNpcMsgStruct*)pBuf;
        if (pMsgPtr->nMsgSize <= nBufLen)
        {
            if (pMsgPtr->nMsgSize < 4 || pMsgPtr->nMsgSize > MAX_PACKETSIZE)
            {
                return 0;
            }
            *pidPacket	= pMsgPtr->idPacket;
            *pMsgSize	= pMsgPtr->nMsgSize - PROTOCOL_NPCMSG_HEADSIZE;
            *pidNpc		= pMsgPtr->idNpc;
            *ppMsg		= pMsgPtr->pMsg;
            return pMsgPtr->nMsgSize;
        }
    }
    return 0;
}

// 合成消息包
// return: netpacket size
inline int	UniteNpcPacket(char* pBuf, int nBufLen, OBJID idPacket, const char* pMsg, int nMsgSize, OBJID idNpc)
{
    if (nBufLen >= nMsgSize + PROTOCOL_NPCMSG_HEADSIZE)
    {
        CProtocolNpcMsgStruct*	pMsgPtr = (CProtocolNpcMsgStruct*)pBuf;
        pMsgPtr->idPacket		= (unsigned short)idPacket;
        pMsgPtr->nMsgSize		= nMsgSize + PROTOCOL_NPCMSG_HEADSIZE;
        pMsgPtr->idNpc			= idNpc;
        memcpy(pMsgPtr->pMsg,	pMsg, (size_t)nMsgSize);
        return pMsgPtr->nMsgSize;
    }
    return 0;
}


#endif // PROTOCOL_H
