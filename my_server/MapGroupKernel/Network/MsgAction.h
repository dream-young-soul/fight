
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
        actionChgMap			= 7,			// 无需参数
        actionFlyMap			= 8,
        actionChgWeather		= 9,
        actionFireworks			= 10,
        actionDie				= 11,
        actionQuitSyn			= 12,
        actionWalk				= 13,
        actionEnterMap			= 14,		// 登录第1步，通知客户端登录成功，玩家已经进入地图。仅玩家! (to client: idUser is Map)
        actionGetItemSet		= 15,		// 登录第2步
        actionGetGoodFriend		= 16,		// 登录第3步
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
        actionGetWeaponSkillSet	= 27,		// 登录第4步
        actionGetMagicSet		= 28,		// 登录第5步
        actionSetPkMode			= 29,
        actionGetSynAttr		= 30,
        actionGhost				= 31,		// 变鬼
        actionSynchro			= 32,		// 坐标同步。send to client self, request client broadcast self coord if no synchro; broadcast to other client, set coord of this player
        actionQueryFriendInfo	= 33,		// idTarget = friend id
        actionQueryLeaveWord	= 34,
        actionChangeFace		= 35,
        //		actionMine				=36,		// 挖矿，改用CMsgInteract
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
        //		actionTakeOff			=52,		// to server, wing user take off
        actionGetMoney			= 53,		// to client only // 捡到500以及500以上钱，只传给自己，dwData为捡到的钱
        //		actionCancelKeepBow		=54,		// to server, cancel keep_bow status
        actionQueryEnemyInfo	= 55,		// idTarget = enemy id	// to server
        actionMoveStop			= 56,		// to target client, data=milli secs.
        actionKickBack			= 57,		// to client	idUser is Player ID, unPosX unPosY is Player pos
        actionDropMagic			= 58,		// to client only, data is magic type
        actionDropSkill			= 59,		// to client only, data is weapon skill type
        actionSoundEffect		= 60,		// to client only, client play sound effect, dwData is monster type
        actionQueryMedal		= 61,		// to server idUser is Player ID, dwData is medal
        actionDelMedal			= 62,		// to server idUser is Player ID, dwData is medal
        actionAddMedal			= 63,		// to client idUser is Player ID, dwData is medal
        actionSelectMedal		= 64,		// to server idUser is Player ID, dwData is medal
        actionQueryHonorTitle	= 65,		// to server idUser is Player ID, dwData is title
        actionDelHonorTitle		= 66,		// to server idUser is Player ID, dwData is title
        actionAddHonorTitle		= 67,		// to client idUser is Player ID, dwData is title
        actionSelectHonorTitle	= 68,		// to server idUser is Player ID, dwData is title
        actionOpenDialog		= 69,		// to client only, open a dialog, dwData is id of dialog
        actionFlashStatus		= 70,		// broadcast to client only, team member only. dwData is dwStatus
        actionQuerySynInfo		= 71,		// to server, return CMsgSynInfo. dwData is target syn id.

        ///////////////
        actionStudentApply		= 72,		// 师父申请招收徒弟 // to server/client idUser为师父ID dwData为徒弟ID
        actionTeacherApply		= 73,		// 徒弟申请拜师 // to server/client idUser为徒弟ID dwData为师父ID
        actionAgreeStudentApply	= 74,		// 同意拜师 // to server idUser为师父ID dwData为徒弟ID
        actionAgreeTeacherApply	= 75,		// 同意招收学生 // to server idUser为徒弟ID dwData为师父ID
        actionDismissStudent	= 76,		// 开除学生// to server idUser为师父ID dwData为徒弟ID
        actionLeaveTeacher		= 77,		// 背叛师门 // to server idUser为徒弟ID dwData为师父ID
        actionQuerySchoolMember	= 78,		// 查询师徒列表 //to server //
        actionTeacherRequest    = 79,       // 在新手工会中发拜师申请

        ////////////////
        // 佣兵任务相关
        actionQueryPlayerTaskAcceptedList		= 80, // 查询玩家悬赏的已接任务列表 // to server // dwData为上一次查询的最后一个ID
        actionQueryPlayerTaskUnacceptedList		= 81, // 查询玩家悬赏的未接任务列表 // to server // dwData为上一次查询的最后一个ID
        //		actionQueryPlayerTaskCompletedList		=82, // 查询玩家悬赏的已完成任务列表 // to server // dwData为上一次查询的最后一个ID
        actionQueryPlayerMyTaskList				= 83, // 查询玩家悬赏的我的任务列表 // to server // dwData为上一次查询的最后一个ID
        actionQueryPlayerTaskDetail				= 84, // 查询玩家悬赏任务详细内容 // to server // dwData为查询任务ID

        actionAcceptPlayerTask					= 88, // 接受任务 // to server // dwData为任务ID
        //		actionCompletePlayerTask				=89, // 完成任务 // to server // dwData为任务ID	// <== 改用CMsgItem
        actionCancelPlayerTask					= 90, // 撤销任务 // to server // dwData为任务ID

        actionLockUser							= 91, // 锁定客户端并同步坐标方向
        actionUnlockUser						= 92, // 解锁客户端并同步坐标方向
        actionMagicTrack						= 93, // 同步坐标方向

        actionQueryStudentsOfStudent			= 94, // 查询徒孙列表，idTarget = 需要查询徒孙列表的徒弟ID

        actionBurstXp			= 95,		// XP爆发，允许使用XP技能
        actionTransPos			= 96,		// 通知客户端将玩家移动到随机的位置
        actionDisappear			= 97,		// 通知客户端目标超出视线范围消失, idUser为目标ID，dwData=0表示角色，dwData=1表示其他MapItem

        // for ai server
        actionSuperChgMap		= 200,		// to game server, data=idMap
        actionFullItem			= 201,		// to game server, data=itemtype
        actionChangeMapDoc		= 202,//no use		// to ai server, idPlayer=idMap, data=idDoc
        actionAddTerrainObj		= 203,		// to ai server, idUser=idMap, data=idOwner, npc_id=idTerrainObj
        actionDelTerrainObj		= 204,		// to ai server, idUser=idMap, data=idOwner
     };

enum { PKMODE_FREE = 0, PKMODE_SAFE, PKMODE_TEAM, PKMODE_ARRESTMENT, PKMODE_SYNDICATE };  //change huang 2004.1.15
const int _ACTION_DANCE1		= 001;
const int _ACTION_STANDBY		= 100;
const int _ACTION_LAUGH			= 150;
const int _ACTION_GUFFAW		= 151;
const int _ACTION_FURY          = 160;
const int _ACTION_SAD	        = 170;
const int _ACTION_EXCITEMENT	= 180;
const int _ACTION_SAYHELLO		= 190;
const int _ACTION_SALUTE		= 200;
const int _ACTION_GENUFLECT     = 210;
const int _ACTION_KNEEL			= 220;
const int _ACTION_COOL			= 230;
const int _ACTION_SWIM			= 240;
const int _ACTION_SITDOWN       = 250;
const int _ACTION_ZAZEN			= 260;
const int _ACTION_FAINT	        = 270;
const int _ACTION_LIE	        = 271;
//const int _ACTION_WALK			= 272;


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
