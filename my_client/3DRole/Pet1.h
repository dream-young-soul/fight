
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
//--------------------------------------------------------------------------------------
// File Name: pet.h
// Create by: Huang Yuhang
// Create on: 2003/8/25 15:00
//--------------------------------------------------------------------------------------
#ifndef _PET_H
#define _PET_H

#include "Player.h"
#include "Magic.h"
#include "3DRoleData.h"

class CPet	: public CPlayer
{
public:
    // 状态
    enum {_PET_IDLE, _PET_BEHAVIOR_READY, _PET_BEHAVIOR_FIND_HOST, _PET_ATTACK};
    // 寻的方式
    enum
    {
        _PET_SE_STATUS_ASSISTANT	=	1,	//	目标为Hero，用于状态辅助型召唤兽
        _PET_SE_ATTACK_ASSISTANT	=	2,	//	以Hero的目标为目标，用于攻击辅助召唤兽
        _PET_SE_REVENGE				=	3,	//	以攻击者为目标，复仇类型
        _PET_SE_GUARD				=	4,	//	以靠近Hero的怪物为目标，好战类型
        _PET_SE_WARLIKE				=	5,	//	寻找与自己最近的敌人战斗, 好战类型
        _PET_SE_JUSTICAL_A			=	6,	//	寻找黑名的玩家战斗，正义类型A
        _PET_SE_JUSTICAL_B			=	7,	//	寻找红名的玩家战斗，正义类型B
        _PET_SE_JUSTICAL_C			=	8,	//	寻找蓝名的玩家战斗，正义类型C
    };
    // 环境策略
    enum
    {
        _PET_ENV_ELUDE				=	1,	// 规避敌人		 （适合远程攻击）
        _PET_ENV_FEARLESS			=	2,	// 无畏的		 （适合近程攻击）
    };
public:
    CPet();
    virtual ~CPet();

    void	Init	(void);
    void	Reset	(void);
    int		GetAtkRange	(void);
    //void	SetAlignPos(int nX, int nY);
public:
    void	LockAttack		(CPlayer* pTarget);
    void	Walk			(int nPosX, int nPosY);
    void	Run				(int nPosX, int nPosY);
    void	Transfer		(int nPosX, int nPosY);
    void	Jump			(int nPosX, int nPosY);

    //	void	Direction		(int nPosX, int nPosY);
    //	void	Stop			(void);
    DWORD	SetCommand		(CCommand* pCommand);
    BOOL	ProcessCommand	();

    void	Process		(void* pInfo);
    void	ProcessAI	();

    BOOL	CanJump(CMyPos posTarGet);
    void	SetAIType(int nType);
public:
    static CPet*	CreateNewPet	(OBJID idPet, int nType, OBJID idItem);
    void	SetItemID (OBJID idItem) {m_idItem = idItem;}
    OBJID   GetItemID ( ) { return m_idItem;}
public:
    void   AddMagic(OBJID idMagic, DWORD dwLevel, DWORD dwExp);
    int	   GetMagicAmount();
    CMagic*   GetMagicByIndex(int nIndex);
    CMagic*   GetMagic(OBJID idMagicType);
private:
    OBJID   m_idItem;		//幻兽的物品ID

    VECTOR_MAGIC		m_setMagic;
    void   DestroyMagicSet();
    // for AI
private:
    int		m_nBehaviorType;
    BOOL	m_bFirstAtk;
    DWORD	m_dwIdleEnd;
    OBJID	m_idTarget;
    PetInfo m_AIInfo;

    OBJID	m_idHeroTarget;
    BOOL	m_bRevengeID;
public:
    void	SetHeroTarget(OBJID idTarger) {m_idHeroTarget = idTarger;}
private:
    // 寻的部分
    BOOL	SearchEnemy();
    void	SearchEnemy(int nMethod);

    void	SearchStatusAsistant();
    void	SearchAttackAsistant();
    void	SearchEnemyRevenge();
    void	SearchEnemyGuard();
    void	SearchEnemyWarlike();
    void	SearchEnemyJusticalA();
    void	SearchEnemyJusticalB();
    void	SearchEnemyJusticalC();

    void	AttackAI_1();
    void	AttackAI_2();
    void	AttackAI_3();
    void	AttackAI_4();
    void	AttackAI_5();

    // 脱离战斗以及后继处理
    BOOL	CheckEnemy(CPlayer* pPlayer, int nRange = 12 , BOOL bHero = true);

    void	CheckTarget();
    BOOL	CheckTargetDie();
    BOOL	CheckTargetNotFound();
    BOOL	CheckTargetTooFar();
    BOOL	CheckTargetSpecialCommand();
    BOOL	CheckTargetElude();



    void	Idle(DWORD dwDelay);
private:
    void	ProcessAI_Idle();
    void	ProcessAI_Ready();
    void	ProcessAI_GenerateNewBehavior();
    void	ProcessAI_FindHost();
    void	ProcessAI_Attack();

};

#endif
