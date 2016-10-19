
//**********************************************************
// 代码编辑器
//**********************************************************

// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(INTERFACE_ROLE_HEAD_FILE)
#define INTERFACE_ROLE_HEAD_FILE

#include "I_MapData.h"
#include "AutoLink.h"

//////////////////////////////////////////////////////////////////////
// 外部状态，需要通过MsgPlayer通知其它玩家的状态。与客户端同步

const I64		KEEPEFFECT_NORMAL			= 0x00000000; 	// 无特殊状态

const I64		KEEPEFFECT_TEAMLEADER		= 0x00000001;   // 队长

const I64		KEEPEFFECT_DIE				= 0x00000002;	// 死亡
const I64		KEEPEFFECT_GHOST			= 0x00000004;   // 灵魂状态
const I64		KEEPEFFECT_DISAPPEARING		= 0x00000008;   // 尸体消失状态


const I64		KEEPEFFECT_CRIME			= 0x00000010;	// 犯罪 // 闪蓝色
const I64		KEEPEFFECT_RED				= 0x00000020;	// 红名
const I64		KEEPEFFECT_DEEPRED			= 0x00000040;	// 黑名
const I64		KEEPEFFECT_SYNCRIME			= 0x00000080;	// 帮派犯罪

const I64		KEEPEFFECT_POISON			= 0x00000100;   // 中毒
const I64		KEEPEFFECT_HIDDEN			= 0x00000200;   // 隐身
const I64		KEEPEFFECT_FREEZE			= 0x00000400;   // 僵化 -- 也可以认为是冰冻
const I64		KEEPEFFECT_SUPERSOLDIER		= 0x00000800;   // 无双
const I64		KEEPEFFECT_LURKER			= 0x00001000;   // 潜伏

const I64		KEEPEFFECT_DEFENCE1			= 0x00002000;   // 防御提高1
const I64		KEEPEFFECT_DEFENCE2			= 0x00004000;   // 防御提高2
const I64		KEEPEFFECT_DEFENCE3			= 0x00008000;   // 防御提高3

const I64		KEEPEFFECT_ATTACK			= 0x00010000;   // 攻击提升
const I64		KEEPEFFECT_ATKSPEED			= 0x00020000;   // 攻击速度提高

const I64		KEEPEFFECT_MAGICDAMAGE		= 0x00040000;   // 魔法伤害提高
const I64		KEEPEFFECT_MAGICDEFENCE 	= 0x00080000;   // 魔法防御提高

const I64		KEEPEFFECT_REFLECT			= 0x00100000;	// 攻击反射
const I64		KEEPEFFECT_REFLECTMAGIC		= 0x00200000;   // 魔法反射

const I64		KEEPEFFECT_SLOWDONE1		= 0x00400000;   // 减速状态 50%
const I64		KEEPEFFECT_SLOWDONE2		= 0x00800000;	// 减速状态 // 血少于一半时开始减速 // 50%

const I64		KEEPEFFECT_TEAM_HEALTH		= 0x01000000;	// 医疗结界状态
const I64		KEEPEFFECT_TEAM_ATTACK		= 0x02000000;	// 攻击结界状态
const I64		KEEPEFFECT_TEAM_DEFENCE		= 0x04000000;	// 护体结界状态
const I64		KEEPEFFECT_TEAM_SPEED		= 0x08000000;	// 速度结界状态
const I64		KEEPEFFECT_TEAM_EXP			= 0x10000000;	// 修炼结界状态
const I64		KEEPEFFECT_TEAM_SPIRIT		= 0x20000000;	// 心灵结界状态
const I64		KEEPEFFECT_TEAM_CLEAN		= 0x40000000;	// 净化结界状态

const I64		KEEPEFFECT_SMOKE			= 0x80000000;			// 烟雾效果
const I64		KEEPEFFECT_DARKNESS			= 0x0000000100000000;	// 黑暗效果
const I64		KEEPEFFECT_PALSY			= 0x0000000200000000;	// 麻痹效果
const I64		KEEPEFFECT_MAXENERGY		= 0x0000000800000000;	// 最大体力增加/减少
const I64		KEEPEFFECT_ADD_EXP			= 0x0000001000000000;	// 战斗经验增加
const I64		KEEPEFFECT_ATTRACT_MONSTER	= 0x0000002000000000;	// 吸引怪物
const I64		KEEPEFFECT_XPFULL			= 0x0000004000000000;	// XP满

const I64		KEEPEFFECT_ADJUST_DODGE		= 0x0000008000000000;	// 调节总的躲避值
const I64		KEEPEFFECT_ADJUST_XP		= 0x0000010000000000;	// 调节每次增加XP值
const I64		KEEPEFFECT_ADJUST_DROPMONEY	= 0x0000020000000000;	// 调节怪物每次掉钱

const I64		KEEPEFFECT_PK_PROTECT		= 0x0000040000000000;	// pk保护状态
const I64		KEEPEFFECT_PELT				= 0x0000080000000000;	// 疾行状态
const I64		KEEPEFFECT_ADJUST_EXP		= 0x0000100000000000;	// 战斗获得经验调整

const I64		KEEPEFFECT_HEAL				= 0x0000200000000000;	// 治愈状态
const I64		KEEPEFFECT_FAINT			= 0x0000400000000000;	// 晕
const I64		KEEPEFFECT_TRUCULENCE		= 0x0000800000000000;	// 野蛮
const I64		KEEPEFFECT_DAMAGE			= 0x0001000000000000;	// 调整受到的伤害
const I64		KEEPEFFECT_ATKER_DAMAGE		= 0x0002000000000000;	// 调整对目标造成的伤害
const I64		KEEPEFFECT_CONFUSION		= 0x0004000000000000;	// 混乱
const I64		KEEPEFFECT_FRENZY			= 0x0008000000000000;	// 狂暴
const I64		KEEPEFFECT_EXTRA_POWER		= 0x0010000000000000;	// 神力

//////////////////////////////////////////////////////////////////////
// 综合状态
//enum {
const I64		KEEPEFFECT_NOT_ACTIVE		= KEEPEFFECT_DIE | KEEPEFFECT_FREEZE | KEEPEFFECT_FAINT;
const I64		KEEPEFFECT_NOT_BLOCKNPC		= KEEPEFFECT_GHOST;
//		KEEPEFFECT_NOT_VIRTUOUS		=KEEPEFFECT_RED|KEEPEFFECT_DEEPRED|KEEPEFFECT_CRIME,
const I64		KEEPEFFECT_NOT_VIRTUOUS		= KEEPEFFECT_DEEPRED | KEEPEFFECT_CRIME;
//};

class CUser;
class CNpc;
class CAgent;
class IRole : public IRoleAttr
{
protected:
    IRole() {}
    virtual ~IRole() {}
public:
    template<class T>
    T*				QueryInterface(T** ppT)					{ return (T*)QueryInterface(GUID_OF(T), (void**)ppT); }
    virtual void*		QueryInterface(int idObjType, void** ppT)		PURE_VIRTUAL_FUNCTION_0
    virtual IRoleAttr*	QueryRoleAttr()							PURE_VIRTUAL_FUNCTION_0
    virtual CAutoLink<IRole>&	QueryLink(const CAutoLink<IRole>&)				= 0; //PURE_VIRTUAL_FUNCTION_0

public: // get info
    virtual OBJID		GetID()									PURE_VIRTUAL_FUNCTION_0
    //	virtual LPCTSTR		GetName()								PURE_VIRTUAL_FUNCTION_0
    //	virtual DWORD		GetLookFace()							PURE_VIRTUAL_FUNCTION_0
    //	virtual DWORD		GetStatus()								PURE_VIRTUAL_FUNCTION_0

    /*	virtual DWORD		GetHelmetType()							PURE_VIRTUAL_FUNCTION_0
    	virtual DWORD		GetArmorType()							PURE_VIRTUAL_FUNCTION_0
    	virtual DWORD		GetWeaponRType()						PURE_VIRTUAL_FUNCTION_0
    	virtual DWORD		GetWeaponLType()						PURE_VIRTUAL_FUNCTION_0
    	virtual DWORD		GetMountType()							PURE_VIRTUAL_FUNCTION_0

    	virtual DWORD		GetMedalSelect()						PURE_VIRTUAL_FUNCTION_0
    */
    virtual OBJID		GetMapID()								PURE_VIRTUAL_FUNCTION_0
    virtual CGameMap*	GetMap()								PURE_VIRTUAL_FUNCTION_0
    virtual int			GetPosX()								PURE_VIRTUAL_FUNCTION_0
    virtual int			GetPosY()								PURE_VIRTUAL_FUNCTION_0
    virtual int			GetDir()								PURE_VIRTUAL_FUNCTION_0
    //	virtual int			GetPose()								PURE_VIRTUAL_FUNCTION_0
    virtual int			GetAttackRange(int nSizeAdd)			PURE_VIRTUAL_FUNCTION_0
    virtual int			GetSizeAdd()							PURE_VIRTUAL_FUNCTION_0

public: // get
    virtual bool		IsAlive()								PURE_VIRTUAL_FUNCTION_0
    virtual bool		IsFarWeapen()							PURE_VIRTUAL_FUNCTION_0
    virtual bool		IsEvil()								PURE_VIRTUAL_FUNCTION_0
    virtual bool		IsRighteous()							PURE_VIRTUAL_FUNCTION_0
    virtual bool		IsLurker()								PURE_VIRTUAL_FUNCTION_0
    virtual bool		IsWing()								PURE_VIRTUAL_FUNCTION_0

    /*public: // set
    	virtual void		SetStatus(DWORD dwStatus)					PURE_VIRTUAL_FUNCTION
    	virtual void		SetDir(int nDir)							PURE_VIRTUAL_FUNCTION
    	virtual void		SetPose(int nPose)							PURE_VIRTUAL_FUNCTION
    	virtual void		SetPos(int uPosX, int nPosY)				PURE_VIRTUAL_FUNCTION
    	virtual void		FlyMap(OBJID idMap, int nPosX, int nPosY)	PURE_VIRTUAL_FUNCTION
    	virtual bool		MoveForward(int nDir)						PURE_VIRTUAL_FUNCTION_0
    */	virtual void		SetLife(int nData)							PURE_VIRTUAL_FUNCTION
    virtual void		SetPower(int nData)							PURE_VIRTUAL_FUNCTION
    virtual void		SetEffect(I64 i64Effect)					PURE_VIRTUAL_FUNCTION
    virtual void		SetSizeAdd(int nData)						PURE_VIRTUAL_FUNCTION
    //	virtual void		JumpPos(int nPosX, int nPosY, int nDir)		PURE_VIRTUAL_FUNCTION
    virtual I64			GetEffect()									PURE_VIRTUAL_FUNCTION_0

public: // application
    virtual int			Distance(int x, int y)						PURE_VIRTUAL_FUNCTION_0

};

class CNpc;
class CAgent;
class INpc : public IRole
{
public: // main
    virtual void		ReleaseByOwner()							PURE_VIRTUAL_FUNCTION
    virtual void		OnTimer()									PURE_VIRTUAL_FUNCTION
    virtual IRole*		QueryRole()									PURE_VIRTUAL_FUNCTION_0
    template<class T>
    T*				QueryInterface(T** ppT)						{ return (T*)QueryInterface(GUID_OF(T), (void**)ppT); }
    virtual void*		QueryInterface(int id, void** ppT)			PURE_VIRTUAL_FUNCTION_0
    IRole*				QueryInterface(IRole** ppT)					{ if (ppT) { *ppT = static_cast<IRole*>(this); } return static_cast<IRole*>(this); }

public: // fight
    virtual void		SetDie()									PURE_VIRTUAL_FUNCTION
    virtual void		BeAttack(IRole* pRole)						PURE_VIRTUAL_FUNCTION
    virtual void		BeKill(IRole* pRole)						PURE_VIRTUAL_FUNCTION
    virtual void		AttackOnce()								PURE_VIRTUAL_FUNCTION

public: // move
    virtual bool		SynchroPos(int nSourX, int nSourY, int nTargX, int nTargY)			PURE_VIRTUAL_FUNCTION_0
    virtual bool		KickBack(int nTargX, int nTargY)			PURE_VIRTUAL_FUNCTION_0
    virtual OBJID		GetMapID()									PURE_VIRTUAL_FUNCTION_0
    virtual bool		IsBlockNpc()								PURE_VIRTUAL_FUNCTION_0

public:	// others
    virtual bool		IsActive()									PURE_VIRTUAL_FUNCTION_0
    virtual bool		Lock(bool bLock)							PURE_VIRTUAL_FUNCTION_0
    virtual bool		IsLocked()									PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
inline int	Distance(int x0, int y0, int x1, int y1)			{ return __max(abs(x0 - x1), abs(y0 - y1)); }


#endif // !defined(INTERFACE_ROLE_HEAD_FILE)
