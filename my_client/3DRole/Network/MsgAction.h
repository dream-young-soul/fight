
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

enum
{
    actionNone				= 0,
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
    actionEnterMap			= 14,
    actionGetItemSet		= 15,
    actionGetGoodFriend		= 16,
    actionForward			= 17,
    actionLeaveMap			= 18,		// 服务器->客户端,idPlayer
    actionJump				= 19,
    actionRun				= 20,
    actionEquip				= 21,
    actionUnequip			= 22,
    actionUplev				= 23,
    actionXpCLear			= 24,
    actionReborn			= 25,
    actionDelRole			= 26,
    actionGetWeaponSkillSet	= 27,
    actionGetMagicSet		= 28,
    actionSetPkMode			= 29,
    actionGetSynAttr		= 30,
    actionGhost				= 31,
    actionSynchro			= 32,
    actionQueryFriendInfo	= 33,
    actionQueryLeaveWord	= 34,
    actionChangeFace		= 35,
    actionMine				= 36,
    actionTeamMemeberPos	= 37,
    actionQueryPlayer		= 38,
    actionAbortMagic		= 39,
    actionMapARGB			= 40,
    actionMapStatus			= 41,		// abandon
    actionQueryTeamMember	= 42,
    actionCreateBooth		= 43,		// 开始摆摊 server/client; idUser: playerID; unPosX,unPosY: playerpos; unDir:dirofbooth; idTarget:idnpc;
    actionSuspendBooth		= 44,		// 收起摆摊
    actionResumeBooth		= 45,		// 继续摆摊 server/client; idUser: playerID; unPosX,unPosY: playerpos; unDir:dirofbooth; idTarget:idnpc;
    actionDestroyBooth		= 46,		// 停止摆摊
    actionQueryCryOut		= 47,		// 查询摆摊吆喝
    actionPostCmd			= 48,		// to client only
    actionQueryEquipment	= 49,		// to server //idUser为需要query的PlayerID
    actionAbortTransform	= 50,		// to server
    actionCombineSubSyn		= 51,		// to client, idUser-> idSubSyn, idTarget-> idTargetSyn
    actionLanding			= 52,		// to server only
    actionGetMoney			= 53,		// to client only // 捡到500以及500以上钱，只传给自己，dwData为捡到的钱
    actionDischarge			= 54,		// to server
    actionQueryEnemyInfo	= 55,		// to server
    actionMoveStop			= 56,		// data=milli secs.
    actionKickBack			= 57,		// to client	idUser is Player ID, unPosX unPosY is Player pos
    actionDropMagic			= 58,		// to client only, data is magic type
    actionDropSkill			= 59,		// to client only, data is weapon skill type
    actionSoundEffect		= 60,		// to client only, client play sound effect, dwData is monster type

    actionQueryMedal		= 61,		// to server	idUser is Player ID, dwData is medal
    actionDelMedal			= 62,		// to server	idUser is Player ID, dwData is medal
    actionAddMedal			= 63,		// to client	idUser is Player ID, dwData is medal
    actionSelectMedal		= 64,		// to server	idUser is Player ID, dwData is medal

    actionQueryHonorTitle	= 65,		// to server	idUser is Player ID, dwData is title
    actionDelHonorTitle		= 66,		// to server	idUser is Player ID, dwData is title
    actionAddHonorTitle		= 67,		// to client	idUser is Player ID, dwData is title
    actionSelectHonorTitle	= 68,		// to server	idUser is Player ID, dwData is title
    actionOpenDialog		= 69,		// to client only, open a dialog, dwData is id of dialog
    actionFlashStatus		= 70,		// broadcast to client only, team member only. dwData is dwStatus

    actionQuerySynInfo		= 71,		// 查询帮派信息		// to server synicate id
    actionStudentApply		= 72,		// 师父申请招收徒弟	// to server/client idUser为师父ID dwData为徒弟ID
    actionTeacherApply		= 73,		// 徒弟申请拜师		// to server/client idUser为徒弟ID dwData为师父ID

    actionAgreeStudentApply	= 74,		// 同意拜师			// to server idUser为师父ID dwData为徒弟ID
    actionAgreeTeacherApply	= 75,		// 同意招收学生		// to server idUser为徒弟ID dwData为师父ID
    actionDismissStudent    = 76,		// 开除学生         // to server idUser为师父ID dwData为徒弟ID
    actionLeaveTeacher		= 77,		// 背叛师门			// to server idUser为徒弟ID dwData为师父ID
    actionQuerySchoolMember	= 78,		// 查询师徒列表		//to server //
    actionTeacherRequest	= 79,		// 在新手工会中发拜师申请	// to server


    actionQueryPlayerTaskAcceptedList		= 80,		// 查询玩家悬赏的已接任务列表 // to server // dwData为上一次查询的最后一个ID
    actionQueryPlayerTaskUnacceptedList		= 81,		// 查询玩家悬赏的未接任务列表 // to server // dwData为上一次查询的最后一个ID
    actionQueryPlayerTaskCompletedList		= 82,		// 查询玩家悬赏的已完成任务列表 // to server // dwData为上一次查询的最后一个ID
    actionQueryPlayerMyTaskList				= 83,		// 查询玩家悬赏的我的任务列表 // to server // dwData为上一次查询的最后一个ID
    actionQueryPlayerTaskDetail				= 84,		// 查询玩家悬赏任务详细内容 // to server // dwData为查询任务ID

    actionAcceptPlayerTask					= 88,		// 接受任务 // to server // dwData为任务ID
    actionCompletePlayerTask				= 89,		 // 完成任务 // to server // dwData为任务ID
    actionCancelPlayerTask					= 90,		// 撤销任务 // to server // dwData为任务ID

    actionLockUser							= 91,		// 锁定客户端并同步坐标方向
    actionUnLockUser						= 92,		// 解锁客户端并同步坐标方向
    actionMagicTrack						= 93,		// 同步坐标方向

    actionQueryStudentsOfStudent			= 94,		// 查询徒孙列表，idTarget = 需要查询徒孙列表的徒弟ID

    actionBurstXp							= 95,		// XP爆发，允许使用XP技能
    actionChangeToRandomPos					= 96,      // 随机移动
    actionDisappear							= 97,
};

class CMsgAction : public CNetMsg
{
public:
    CMsgAction();
    virtual ~CMsgAction();

    BOOL	Create	(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, DWORD dwData = 0);
    BOOL	Create	(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY, DWORD dwTimeStamp = 0);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
		USHORT		unMsgSize;
		USHORT		unMsgType;
		DWORD		dwTimeStamp;

		OBJID		idUser;
		USHORT		unPosX, unPosY;
		USHORT		unDir;
		USHORT		flag;
		//union
		//{
		OBJID		idTarget;
		DWORD		dwData;
		INT			iData;

		// struct
		//{
		USHORT usTargetPosX;
		USHORT usTargetPosY;
		// };
		// }; 

		USHORT		usAction;
		USHORT		flag1;
		BOOL		bSucDone;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGDIR_H__950C0770_DB2D_4D79_970C_E2863ECE9614__INCLUDED_)
