
//**********************************************************
// 代码编辑器
//**********************************************************

// AgentData.h: agent system config file
// paled, 2003.8.18
//////////////////////////////////////////////////////////////////////

#ifndef	AGENT_DATA_H
#define	AGENT_DATA_H

#include "common.h"
#include "BaseFunc.h"

const int		TICKS_PER_SEC			= 2;

//////////////////////////////////////////////////////////////////////
inline const char* const* GetActionStringSet()
{
    static char* setAction[] =
    {
        // attr
        "ChangeMode",//XXXXXXXXXXX				// 修改Mode(_,_)。用action来实现写入永久事实。
        "Reborn",
        "TalkToName",				// (name,str)
        "TalkToID",					// (id,str)

        // move
        "MoveCloseTarget",//XXXXXXXXXXX			// (id)，走到能攻击对方的距离。
        "JumpClose",				// (x,y,dist)，走到距对方一定的距离。
        "JumpToSafePos",			// 跳到安全的地方，防止被怪包围
        "ChangeMapTo",				// (id,x,y)用SUPER命令回到指定地点(类似回城卷)
        "RandomWalk",				// 乱走
        "KeepOutEnemy",				// (dist)，与敌人保持距离，逃跑

        // fight
        "AttackRole",				// (id)

        // item_pack
        "UseItem",					// (type)吃一颗药
        "FullMedicine",				// (type)到SHOP处装满药, SUPER命令

        // team
        "AcceptTeamInvite",			// (id)
        "SendTeamApply",			// (id)
        "LeaveTeam",

        NULL
    };
    return setAction;
}

enum
{
    // // attr
    ACTION_CHANGE_MODE,					// "ChangeMode",				// 修改Mode(_,_)。用action来实现写入永久事实。
    ACTION_REBORN,						// "Reborn",
    ACTION_TALK_TO_NAME,				// "TalkToName",				// (name,str)
    ACTION_TALK_TO_ID,					// "TalkToID",					// (id,str)
    //
    // // move
    ACTION_MOVE_CLOSE,					// "MoveCloseTarget",			// (id)，走到能攻击对方的距离。XXXXXXXXXXX
    ACTION_JUMP_CLOSE,					// "JumpClose":					// (x,y,dist)，走到距对方一定的距离。
    ACTION_JUMP_TO_SAFE,				// "JumpToSafePos",				// 跳到安全的地方，防止被怪包围
    ACTION_CHGMAP,						// "ChangeMapTo",				// (id,x,y)用超级命令回到SHOP附近(类似回城卷)
    ACTION_RANDOM_WALK,					// "RandomWalk",				// 乱走
    ACTION_LEAVE_EVIL,					// "KeepOutEnemy",				// (dist)，与敌人保持距离，逃跑
    //
    // // fight
    ACTION_ATTACK,						// "AttackRole",				// (id)
    //
    // // item_pack
    ACTION_USE_ITEM,					// "UseItem":					// (type)吃一颗药
    ACTION_FULL_MEDICINE,				// "FullMedicine",				// (type)到SHOP处装满药
    //
    // // team
    ACTION_ACCEPT_INVITE,				// "AcceptTeamInvite",			// (id)
    ACTION_SEND_APPLY,					// "SendTeamApply",				// (id)
    ACTION_LEAVE_TEAM,					// "LeaveTeam",
};

//////////////////////////////////////////////////////////////////////
inline const char* const* GetFactFuncStringSet()
{
    static char*	setFactFunc[] =
    {
        // attr
        "IsAlive",					// (id)，检测角色是否活着
        "IsWing",					// (id)，检查角色是否在飞行中
        "IsUser",					// (id)
        "IsMonster",				// (id)
        "EvilValue",				// (id, int)，取角色的邪恶值，怪物和蓝名、黑名为100，红名为50，卫兵和普通玩家为0
        "CoolValue",				// (id, int)，100 ~ 0，有右手极品武器为50
        "Level",					// (id, int)，取角色等级(身上装备的最高等级)
        "MyLife",					// (int)

        // move
        "RolePos",					// (id,x,y)，取角色的坐标
        "MyDistance",				// (x,y,int)，离目标点的距离

        // fight
        "AttackDistance",			// (id,int)，能攻击对方的距离
        "DangerValue",				// (int)，+∞ ~ 0，通常80算危险，单个敌人为50，无敌人为0

        // item pack
        "ItemCount",				// (int)，40 ~ 0，背包中物品数量(药品数量)

        // team
        "MyLeader",					// (id)，队长ID
        "IsLeader",					// (id)，检查是否队长

        NULL
    };
    return setFactFunc;
}

enum
{
    // // attr
    FACTFUNC_IS_ALIVE,					// "IsAlive",					// (id)，检测角色是否活着
    FACTFUNC_IS_WING,					// "IsWing",					// (id)，检查角色是否在飞行中
    FACTFUNC_IS_USER,					// "IsUser",					// (id)
    FACTFUNC_IS_MONSTER,				// "IsMonster",				// (id)
    FACTFUNC_EVIL_VALUE,				// "EvilValue",				// (id, int)，取角色的邪恶值，怪物和蓝名、黑名为100，红名为50，卫兵和普通玩家为0
    FACTFUNC_COOL_VALUE,				// "CoolValue",				// (id, int)，100 ~ 0，有右手极品武器为50
    FACTFUNC_LEVEL,						// "Level",					// (id, int)，取角色等级(身上装备的最高等级)
    FACTFUNC_MY_LIFE,					// "MyLife",					// (int)

    // // move
    FACTFUNC_ROLE_POS,					// "RolePos",					// (id,x,y)，取角色的坐标
    FACTFUNC_MYDISTANCE,				// "MyDistance",				// (x,y,int)，离目标点的距离

    // // fight
    FACTFUNC_ATTACK_DISTNACE,			// "AttackDistance",			// (id,int)，能攻击对方的距离
    FACTFUNC_DANGER_VALUE,				// "DangerValue",				// (int)，+∞ ~ 0，通常80算危险，单个敌人为50，无敌人为0

    // // item pack
    FACTFUNC_ITEM_COUNT,				// "ItemCount"					// (int)，40 ~ 0，背包中物品数量(药品数量)

    // // team
    FACTFUNC_MY_LEADER,					// "MyLeader",					// (id)，队长ID
    FACTFUNC_IS_LEADER,					// "IsLeader",					// (id)，检查是否队长
};

//////////////////////////////////////////////////////////////////////
//* push fact set, add and del
//#define FACT_MODE				"Mode"					// (e,e)			// 0
#define FACT_LOOK_TARGET		"LookTarget"			// <id>				// 80
#define FACT_TALK_TO_ME			"TalkToMe"				// <name>(str)		// 50
#define FACT_TEAM_INVITE		"TeamInvite"			// (id)				// 50
//#define FACT_LOOK_ATTACK		"LookAttack"			// (id,id)			// 80
//#define FACT_LISTEN_TALK		"ListenTalk"			// (name,name,str)		// 50
//*/

//////////////////////////////////////////////////////////////////////
inline int	 Priority2Durable(int nPriority)
{
    static int	setPriority2Durable[] = { INT_MAX / TICKS_PER_SEC,				// 0

                                          // random
                                          1200, 1200, 1200, 1200, 1200,  1200, 1200, 1200, 1200, 1200,		// 1 - 10
                                          300, 300, 300, 300, 300,  300, 300, 300, 300, 300,		// 11 - 20

                                          // idle
                                          8, 8, 8, 8, 8,  8, 8, 8, 8, 8,		// 21 - 30
                                          8, 8, 8, 8, 8,  8, 8, 8, 8, 8,		// 31 - 40
                                          8, 8, 8, 8, 8,  8, 8, 8, 8, 8,		// 41 - 50

                                          // fight
                                          3, 3, 3, 3, 3,  3, 3, 3, 3, 3,		// 51 - 60
                                          3, 3, 3, 3, 3,  3, 3, 3, 3, 3,		// 61 - 70
                                          3, 3, 3, 3, 3,  3, 3, 3, 3, 3,		// 71 - 80

                                          // attack
                                          1, 1, 1, 1, 1,  1, 1, 1, 1, 1,		// 81 - 90
                                          0, 0, 0, 0, 0,  0, 0, 0, 0, 0,		// 91 - 100
                                       };
    return setPriority2Durable[nPriority];
}



#endif// AGENT_DATA_H

