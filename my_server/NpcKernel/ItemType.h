
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ItemType.h: interface for the CItemType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ITEMTYPE_H)
#define ITEMTYPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "define.h"
#include "ConstGameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleMap2.h"
#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////
enum ITEMTYPEDATA
{
    ITEMTYPEDATA_ID_ = 0,
    ITEMTYPEDATA_NAME = 1,
    ITEMTYPEDATA_REQ_PROF,
    ITEMTYPEDATA_REQ_SKILL,
    ITEMTYPEDATA_REQ_LEVEL,
    ITEMTYPEDATA_REQ_SEX,
    ITEMTYPEDATA_REQ_FORCE,
    ITEMTYPEDATA_REQ_SPEED,
    ITEMTYPEDATA_REQ_HEALTH,
    ITEMTYPEDATA_REQ_SOUL,
    ITEMTYPEDATA_MONOPOLY,
    ITEMTYPEDATA_WEIGHT,
    ITEMTYPEDATA_PRICE,
    ITEMTYPEDATA_ACTION,
    ITEMTYPEDATA_ATTACK_MAX,
    ITEMTYPEDATA_ATTACK_MIN,
    ITEMTYPEDATA_DEFENSE,
    ITEMTYPEDATA_DEXTERITY,
    ITEMTYPEDATA_DODGE,
    ITEMTYPEDATA_LIFE,
    ITEMTYPEDATA_MANA,
    ITEMTYPEDATA_AMOUNT,
    ITEMTYPEDATA_AMOUNT_LIMIT,
    ITEMTYPEDATA_IDENT,
    ITEMTYPEDATA_GEM1,
    ITEMTYPEDATA_GEM2,
    ITEMTYPEDATA_MAGIC1,
    ITEMTYPEDATA_MAGIC2,
    ITEMTYPEDATA_MAGIC3,
    ITEMTYPEDATA_MAGICATK,
    ITEMTYPEDATA_MAGICDEF,
    ITEMTYPEDATA_ATKRANGE,
    ITEMTYPEDATA_ATKSPEED,
};

char	szItemTypeTable[];
typedef	CConstGameData<ITEMTYPEDATA, szItemTypeTable, ITEMTYPEDATA_ID_>	CItemTypeData;
typedef	ISingleMap2<CItemTypeData>	IItemTypeSet;
typedef	CSingleMap2<CItemTypeData>	CItemTypeSet;

class IDatabase;
class CItemType
{
protected:
    CItemType();
    virtual ~CItemType();
public:
    static CItemType*	CreateNew	()		{ return new CItemType; }
    ULONG	Release			()				{ delete this; return 0; }
    bool	Create			(IDatabase* pDb);
    CItemTypeData*	QueryItemType(OBJID idType);

protected:
    IItemTypeSet*	m_setType;

    MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(ITEMTYPE_H)
