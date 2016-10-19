
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicType.h: interface for the CMagicType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_)
#define AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "define.h"
#include "ConstGameData.h"
#include "GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////
enum MAGICTYPEDATA
{
    MAGICTYPEDATA_ID_ = 0,
    MAGICTYPEDATA_TYPE,
    MAGICTYPEDATA_SORT,
    MAGICTYPEDATA_NAME,
    MAGICTYPEDATA_CRIME,
    MAGICTYPEDATA_GROUND,
    MAGICTYPEDATA_MULTI,
    MAGICTYPEDATA_TARGET,
    MAGICTYPEDATA_LEVEL,
    MAGICTYPEDATA_USE_MP,
    MAGICTYPEDATA_USE_POTENTIAL,
    MAGICTYPEDATA_POWER,
    MAGICTYPEDATA_INTONE,
    MAGICTYPEDATA_PERCENT,
    MAGICTYPEDATA_STEP,
    MAGICTYPEDATA_RANGE,
    MAGICTYPEDATA_DISTANCE,
    MAGICTYPEDATA_STATUS_CHANCE,		// ATKSTATUS类型魔法攻击附加状态的概率百分比
    MAGICTYPEDATA_STATUS,
    MAGICTYPEDATA_NEED_PROFMASK,
    MAGICTYPEDATA_NEED_EXP,
    MAGICTYPEDATA_NEED_LEVEL,
    MAGICTYPEDATA_NEED_GEMTYPE,		//---jinggy---需要的宝石类型
    MAGICTYPEDATA_USE_XP,				// == TYPE_XPSKILL: use xp
    MAGICTYPEDATA_WEAPON_SUBTYPE,
    MAGICTYPEDATA_TIMES,				// =0 : once status
    MAGICTYPEDATA_AUTO_ACTIVE,			// 0x01: active when kill target
    MAGICTYPEDATA_FLOOR_ATTR,			// 0: need not match.
    MAGICTYPEDATA_AUTO_LEARN,			// bool
    MAGICTYPEDATA_LEARN_LEVEL,			// auto learn need level, nn: equ level, nnmm: between mm - nn level.
    MAGICTYPEDATA_DROPWEAPON,			// 1: throw weapon_r to target
    MAGICTYPEDATA_USE_EP,				// use energy
    MAGICTYPEDATA_WEAPON_HIT,			// 1: hit by weapon
    MAGICTYPEDATA_USE_ITEM,				// use equip item
    MAGICTYPEDATA_NEXT_MAGIC,			// launch this magic when filished current magic, use same target and pos
    MAGICTYPEDATA_DELAY,				// milli-secs
    MAGICTYPEDATA_ITEM_NUM,				// use equip item number
    MAGICTYPEDATA_WIDTH,
    MAGICTYPEDATA_DURABILITY,
    MAGICTYPEDATA_APPLY_MS,				// 魔法每次作用施展时间间隔
    MAGICTYPEDATA_TRACK_ID,
    MAGICTYPEDATA_TRACK_ID2,
    MAGICTYPEDATA_AUTO_LEARN_PROB,		// 自动学习的成功率(10000=100%)
};
char	szMagicTypeTable[];
typedef	CGameData<MAGICTYPEDATA, szMagicTypeTable, szID>	CGameMagicTypeData;


typedef struct
{
    UCHAR	ucDir;
    UCHAR	ucStep;
    UCHAR	ucAlt;
    OBJID	idAction;
    int		nPower;
    int		nApplyMS;
} ST_TRACK;

class CMagicTypeData
{
public:
    CMagicTypeData()			{ m_pData = NULL; }
    virtual ~CMagicTypeData()	{ if (m_pData) { SAFE_RELEASE (m_pData); } }

public:
    static CMagicTypeData*	CreateNew	()	{ return new CMagicTypeData; }
    ULONG	Release			()				{ delete this; return 0; }
    ULONG	ReleaseByOwner	()				{ delete this; return 0; }
    bool	Create			(IRecordset* pRes);

public: // get set
    OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }

    int		GetInt(MAGICTYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
    LPCTSTR	GetStr(MAGICTYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); }
    //	void	SetInt(ITEMDATA idx, int nData)						{ CHECK(m_pData); m_pData->SetInt(idx, nData) }
    //	void	SetStr(ITEMDATA idx, LPCTSTR szData, int nSize)		{ CHECK(m_pData); m_pData->SetStr(idx, szData, nSize); }

public:	// track
    int				GetSTrackAmount	()				{ return m_setTrackS.size(); }
    const ST_TRACK&	GetSTrackByIndex(int idx)		{ ASSERT(idx < m_setTrackS.size()); return m_setTrackS[idx]; }

    int				GetTTrackAmount	()				{ return m_setTrackT.size(); }
    const ST_TRACK&	GetTTrackByIndex(int idx)		{ ASSERT(idx < m_setTrackT.size()); return m_setTrackT[idx]; }

protected:
    CGameMagicTypeData*		m_pData;
    typedef vector<ST_TRACK>	TRACK_SET;
    TRACK_SET				m_setTrackS;
    TRACK_SET				m_setTrackT;
public:
    MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////

typedef	ISingleObjSet2<CMagicTypeData>		IMagicTypeSet;
typedef	CSingleObjSet2<CMagicTypeData>		CMagicTypeSet;


//class CMagicType
//{
//public:
//	CMagicType();
//	virtual ~CMagicType();
//
//public:
//	static CMagicType*	CreateNew	()		{ return new CMagicType; }
//	ULONG	Release			()				{ delete this; return 0; }
//	bool	Create			(IDatabase* pDb);
//	CMagicTypeData*	QueryMagicType(OBJID idType);
//
//protected:
//	IMagicTypeSet*	m_setType;
//public:
//	MYHEAP_DECLARATION(s_heap)
//};

#endif // !defined(AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_)
