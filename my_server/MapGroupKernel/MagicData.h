
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicData.h: interface for the CTempMagicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_)
#define AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "MagicType.h"
#include "MyHeap.h"
#include "TimeOut.h"

//////////////////////////////////////////////////////////////////////
enum MAGICDATA
{
    MAGICDATA_ID_ = 0,				//? 仅用于武器上带魔法的非数据库对象和再次学习时的UPDATE

    MAGICDATA_OWNERID = 1,
    MAGICDATA_TYPE,
    MAGICDATA_LEVEL,
    MAGICDATA_EXP,
    MAGICDATA_UNLEARN,
    MAGICDATA_OLD_LEVEL,

    MAGICTYPEDATA_OFFSET = 100,		// 控制量，请勿使用
    MAGICDATA_TYPE_X = 101,		// 不使用
    MAGICDATA_SORT,
    MAGICDATA_NAME,
    MAGICDATA_CRIME,
    MAGICDATA_GROUND,
    MAGICDATA_MULTI,
    MAGICDATA_TARGET,
    MAGICDATA_LEVEL_X,			// 不使用
    MAGICDATA_USE_MP,
    MAGICDATA_USE_POTENTIAL,	// 消耗潜能
    MAGICDATA_POWER,
    MAGICDATA_INTONE,			// 魔法吟唱时间(ms)
    MAGICDATA_PERCENT,
    MAGICDATA_STEP,
    MAGICDATA_RANGE,
    MAGICDATA_DISTANCE,
    MAGICDATA_STATUS_CHANCE,		// ATKSTATUS类型魔法附加状态的概率百分比
    MAGICDATA_STATUS,
    MAGICDATA_NEED_PROFMASK,
    MAGICDATA_NEED_EXP,
    MAGICDATA_NEED_LEVEL,
    MAGICDATA_NEED_GEMTYPE,		//---jinggy---需要的宝石类型
    MAGICDATA_USE_XP,				// == TYPE_XPSKILL: use xp
    MAGICDATA_WEAPON_SUBTYPE,
    MAGICDATA_TIMES,				// =0 : once status
    MAGICDATA_AUTO_ACTIVE,			// 0x01: active when kill target
    MAGICDATA_FLOOR_ATTR,			// 0: need not match.
    MAGICDATA_AUTO_LEARN,			// bool
    MAGICDATA_LEARN_LEVEL,			// auto learn need level, nn: equ level, nnmm: between mm - nn level.
    MAGICDATA_DROPWEAPON,			// 1: throw weapon_r to target
    MAGICDATA_USE_EP,				// use energy
    MAGICDATA_WEAPON_HIT,			// 1: hit by weapon
    MAGICDATA_USE_ITEM,				// use equip item
    MAGICDATA_NEXT_MAGIC,			// launch this magic when filished current magic, use same target and pos
    MAGICDATA_DELAY,				// milli-secs
    MAGICDATA_ITEM_NUM,				// use equip item number
    MAGICDATA_WIDTH,				// width of magic. 0-180 for degree of fan (sort 4), 0 for default.
    MAGICDATA_DURABILITY,

    MAGICDATA_APPLY_MS,				// 魔法每次作用施放时间
    MAGICDATA_TRACK_ID,
    MAGICDATA_TRACK_ID2,

    MAGICDATA_AUTO_LEARN_PROB,		// 自动学习的成功率(10000=100%)
};


struct ST_MAGIC_DATA
{
    OBJID	idMagic;
    OBJID	idOwner;
    USHORT	usType;
    USHORT	usLevel;
    DWORD	dwExp;
    UCHAR	ucUnlearn;
    USHORT	usOldLevel;
};

//////////////////////////////////////////////////////////////////////
class IMagicData
{
protected:
    IMagicData()			{ m_pType = NULL; }
    virtual ~IMagicData()	{}
public:
    // 由于CSingleObjSet2需要CreateNew接口，不得不增加
    static IMagicData*	CreateNew();
    virtual void	ReleaseByOwner()											PURE_VIRTUAL_FUNCTION
    virtual bool	Create(IRecordset* pRecordset)								PURE_VIRTUAL_FUNCTION_0
    virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey)			PURE_VIRTUAL_FUNCTION_0
    virtual bool	Create(ST_MAGIC_DATA* pMagicData)							PURE_VIRTUAL_FUNCTION_0
    virtual int		GetInt(MAGICDATA nDataName)									PURE_VIRTUAL_FUNCTION_0
    virtual void	SetInt(MAGICDATA idx, int nData)							PURE_VIRTUAL_FUNCTION
    virtual bool	FindType()													PURE_VIRTUAL_FUNCTION_0

    virtual OBJID	GetID()														PURE_VIRTUAL_FUNCTION_0
    virtual bool	Update()													PURE_VIRTUAL_FUNCTION_0
    virtual void	ClearUpdateFlags()											PURE_VIRTUAL_FUNCTION
    virtual OBJID	InsertRecord()												PURE_VIRTUAL_FUNCTION_0
    virtual bool	DeleteRecord()												PURE_VIRTUAL_FUNCTION_0

public:
    virtual const char*		GetName()		{ CHECKF(m_pType); return m_pType->GetStr(MAGICTYPEDATA_NAME); }

    // 移动轨迹
    virtual int				GetSTrackAmount()			{ CHECKF(m_pType); return m_pType->GetSTrackAmount(); }
    virtual const ST_TRACK&	GetSTrackByIndex(int idx)	{ ASSERT(m_pType); return m_pType->GetSTrackByIndex(idx); }

    virtual int				GetTTrackAmount()			{ CHECKF(m_pType); return m_pType->GetTTrackAmount(); }
    virtual const ST_TRACK&	GetTTrackByIndex(int idx)	{ ASSERT(m_pType); return m_pType->GetTTrackByIndex(idx); }

    virtual int				GetApplyMS();
    virtual int				GetLockSecs();

    virtual bool	IsReady()							{ return m_tDelay.IsTimeOut(); }
    virtual void	StartDelay()						{ m_tDelay.Update(); }
    virtual void	SetDelay();

protected:
    CMagicTypeData*	m_pType;
    CTimeOutMS		m_tDelay;			// 施展结束延时
};


//////////////////////////////////////////////////////////////////////
class CMagicTempData  : public IMagicData
{
protected:
    CMagicTempData();
    virtual ~CMagicTempData()	{}

public:
    static IMagicData*	CreateNew()			{ return new CMagicTempData; }
    virtual void	ReleaseByOwner()		{ delete this; }

    virtual bool	Create(IRecordset* pRecordset);
    virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
    virtual bool	Create(ST_MAGIC_DATA* pMagicData);
    virtual int		GetInt(MAGICDATA nDataName);
    virtual void	SetInt(MAGICDATA idx, int nData);
    virtual bool	FindType();

    virtual OBJID	GetID()					{ return m_data.idMagic; }
    virtual bool	Update()				{ return false; }
    virtual void	ClearUpdateFlags()		{}
    virtual OBJID	InsertRecord()			{ return m_data.idMagic; }
    virtual bool	DeleteRecord()			{ return false; }

protected:
    ST_MAGIC_DATA	m_data;
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_)
