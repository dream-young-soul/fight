
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAction.h: interface for the CMsgAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGDIR_H__950C0770_DB2D_4D79_970C_E2863ECE9614__INCLUDED_)
#define AFX_MSGDIR_H__950C0770_DB2D_4D79_970C_E2863ECE9614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum {	actionNone				= 0,
        actionChgDir			= 1,
        actionPosition			= 2,
        actionEmotion			= 3,
        actionBroadcastPos		= 4,
        actionDivorce			= 5,
        actionSelfUnfreeze		= 6,
        actionChgMap			= 7,
        actionFlyMap			= 8,
        actionChgWeather		= 9,
        actionFireworks			= 10,
        actionDie				= 11,
        actionQuitSyn			= 12,
        actionWalk				= 13,
        actionEnterMap			= 14,		// 登录第1步，通知客户端登录成功，玩家已经进入地图。仅玩家! (to client: idUser is Map)
        actionGetItemSet		= 15,
        actionGetGoodFriend		= 16,
        actionForward			= 17,
        actionLeaveMap			= 18,		// 服务器->客户端,idPlayer。与CMsgPlayer对应，表示玩家离开地图，要求删除对象。
        actionJump				= 19,
        actionRun				= 20,
        actionEquip				= 21,
        actionUnequip			= 22,
        actionUplev				= 23,		// 玩家升级(只显示动画)
        actionXpCLear			= 24,
        actionReborn			= 25,
        actionDelRole			= 26,
        actionGetWeaponSkillSet	= 27,
        actionGetMagicSet		= 28,
        actionSetPkMode			= 29,
        actionGetSynAttr		= 30,
        actionGhost				= 31,		// 变鬼
        actionSynchro			= 32,		// 坐标同步
        actionQueryFriendInfo	= 33,		// idTarget = friend id
        actionQueryLeaveWord	= 34,
        actionChangeFace		= 35,
        actionMine				= 36,
        actionTeamMemeberPos	= 37,
        actionQueryPlayer		= 38,
        actionAbordMagic		= 39,
        actionMapARGB			= 40,		// to client only
        actionMapStatus			= 41,		// to npc server only, idTarget = map id.
        actionQueryTeamMember	= 42,
        actionCreateBooth		= 43,		// 开始摆摊 to server - unPosX,unPosY: playerpos; unDir:dirofbooth; to client - idTarget:idnpc;
        actionSuspendBooth		= 44,		// 收起摆摊
        actionResumeBooth		= 45,		// 继续摆摊 to server - unPosX,unPosY: playerpos; unDir:dirofbooth; to client - idTarget:idnpc;
        actionDestroyBooth		= 46,		// 停止摆摊
        actionQueryCryOut		= 47,		// 查询摆摊吆喝
        actionPostCmd			= 48,		// to client only
        actionQueryEquipment	= 49,		// to server //idTarget为需要query的PlayerID
        actionAbortTransform	= 50,		// to server
        actionCombineSubSyn		= 51,		// to client, idUser-> idSubSyn, idTarget-> idTargetSyn
        actionTakeOff			= 52,		// to server, wing user take off
        actionGetMoney			= 53,		// to client only // 捡到500以及500以上钱，只传给自己，dwData为捡到的钱
        actionCancelKeepBow		= 54,		// to server, cancel keep_bow status
        actionQueryEnemyInfo	= 55,		// idTarget = enemy id	// to server
        actionMoveStop			= 56,		// to target client, data=milli secs.
        actionKickBack			= 57,		// to client	idUser is Player ID, unPosX unPosY is Player pos, client answer actionSynchro


        actionLockUser			= 91, // 锁定客户端并同步坐标方向
        actionUnlockUser		= 92, // 解锁客户端并同步坐标方向
        actionMagicTrack		= 93, // 同步坐标方向


        // for ai server
        actionSuperChgMap		= 200,		// to game server, data=idMap
        actionFullItem			= 201,		// to game server, data=itemtype
        actionChangeMapDoc		= 202,//no use		// to ai server, idUser=idMap, data=idDoc
        actionAddTerrainObj		= 203,		// to ai server, idUser=idMap, data=idOwner, ncp_id=idTerrainObj
        actionDelTerrainObj		= 204,		// to ai server, idUser=idMap, data=idOwner

     };


class CMsgAction : public CNetMsg
{
public:
    CMsgAction();
    virtual ~CMsgAction();

    BOOL	Create	(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, DWORD dwData = 0);
    BOOL	Create	(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        DWORD		dwTimeStamp;

        OBJID		idUser;
        USHORT		unPosX, unPosY;
        USHORT		unDir;
        union
        {
            OBJID		idTarget;
            DWORD		dwData;
            INT			iData;
            BOOL		bSucDone;
            struct
            {
                USHORT usTargetPosX;
                USHORT usTargetPosY;
            };
        };

        USHORT		usAction;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGDIR_H__950C0770_DB2D_4D79_970C_E2863ECE9614__INCLUDED_)
