
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcType.h: interface for the CNpcType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCTYPE_H__34FC8339_FCB6_4D96_8574_01FEC2969274__INCLUDED_)
#define AFX_NPCTYPE_H__34FC8339_FCB6_4D96_8574_01FEC2969274__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "GameData.h"

enum NPCTYPEDATA
{
    NPCTYPEDATA_NAME = 1,
    NPCTYPEDATA_TYPE,
    NPCTYPEDATA_LOOKFACE,
    NPCTYPEDATA_LENGTH,
    NPCTYPEDATA_FAT,

    NPCTYPEDATA_LIFE,
    NPCTYPEDATA_MANA,
    NPCTYPEDATA_ATTACKMAX,
    NPCTYPEDATA_ATTACKMIN,
    NPCTYPEDATA_DEFENCE,
    NPCTYPEDATA_DEXTERITY,
    NPCTYPEDATA_DODGE,

    NPCTYPEDATA_HELMET_TYPE,
    NPCTYPEDATA_ARMOR_TYPE,
    NPCTYPEDATA_WEAPONR_TYPE,
    NPCTYPEDATA_WEAPONL_TYPE,

    NPCTYPEDATA_ATKRANGE,
    NPCTYPEDATA_VIEWRANGE,
    NPCTYPEDATA_ESCAPELIFE,
    NPCTYPEDATA_ATKSPEED,				// 毫秒
    NPCTYPEDATA_MOVESPEED,				// 毫秒

    NPCTYPEDATA_LEVEL,
    NPCTYPEDATA_ATKUSER,

    NPCTYPEDATA_DROPMONEY,
    NPCTYPEDATA_DROPITEMTYPE,
    NPCTYPEDATA_SIZEADD,				// 1: 增加一个CELL的攻击距离
    NPCTYPEDATA_ACTION,					// action for kill award

    NPCTYPEDATA_RUNSPEED,				// 毫秒

    NPCTYPEDATA_DROP_ARMET,				//
    NPCTYPEDATA_DROP_NECKLACE,			//
    NPCTYPEDATA_DROP_ARMOR,				//
    NPCTYPEDATA_DROP_RING,				//
    NPCTYPEDATA_DROP_WEAPON,			//
    NPCTYPEDATA_DROP_SHIELD,			//
    NPCTYPEDATA_DROP_SHOES,				//

    NPCTYPEDATA_DROP_HP,				//
    NPCTYPEDATA_DROP_MP,				//

    NPCTYPEDATA_MAGIC_TYPE,
    NPCTYPEDATA_MAGIC_DEF,
    NPCTYPEDATA_MAGIC_HITRATE,
    NPCTYPEDATA_ATK_HITRATE,

    NPCTYPEDATA_AITYPE,					// for client
    NPCTYPEDATA_DEFENCE2,				// defalut 1

    //以下是增加的
    NPCTYPEDATA_EXPLODE_ITEM_CHANCE1,	//小爆的几率
    NPCTYPEDATA_EXPLODE_ITEM_CHANCE2,	//中爆的几率
    NPCTYPEDATA_EXPLODE_ITEM_CHANCE3,	//大爆的几率
    NPCTYPEDATA_DROP_ITEM_RULE,			//掉物品的规则组ID

    NPCTYPEDATA_PROFESSION,				// 职业编号
};

char	szNpcType[];
typedef	CGameData<NPCTYPEDATA, szNpcType, szID>	CNpcTypeData;

class CNpcType;
class CNpcType : public CGameObj
{
public:
    CNpcType();
    virtual ~CNpcType();
    bool	Create(IRecordset* pRes);
    static CNpcType*	CreateNew()	{ return new CNpcType; }
    ULONG	ReleaseByOwner() { delete this; return 0; }

public: // get
    OBJID	GetID()			{ return m_pData->GetKey(); }
    int		GetInt(NPCTYPEDATA idx)	{ return m_pData->GetInt(idx); }
    LPCTSTR	GetStr(NPCTYPEDATA idx)	{ return m_pData->GetStr(idx); }

protected: // data
    CNpcTypeData*		m_pData;
};

#endif // !defined(AFX_NPCTYPE_H__34FC8339_FCB6_4D96_8574_01FEC2969274__INCLUDED_)
