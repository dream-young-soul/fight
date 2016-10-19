
//**********************************************************
// 代码编辑器
//**********************************************************

// ItemAddition.h: interface for the CItemAddition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMADDITION_H__4C270148_7B1D_46C5_A799_B85CF277CF6E__INCLUDED_)
#define AFX_ITEMADDITION_H__4C270148_7B1D_46C5_A799_B85CF277CF6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "define.h"
#include "ConstGameData.h"
#include "GameData.h"
#include "GameObj.h"
#include "Myheap.h"

#include "SingleMap64.h"

#include <vector>
using namespace std;



//////////////////////////////////////////////////////////////////////
enum ITEMADDITIONDATA
{
    //	ITEMADDITIONDATA_ID = 0,		// 不会用到的
    ITEMADDITIONDATA_TYPEID = 1,	// 追加类型(与itemtype相关)
    ITEMADDITIONDATA_LEVEL,			// 追加等级
    ITEMADDITIONDATA_LIFE,			// 生命值
    ITEMADDITIONDATA_ATTACK_MAX,	// 最大物理攻击
    ITEMADDITIONDATA_ATTACK_MIN,	// 最小物理攻击
    ITEMADDITIONDATA_DEFENSE,		// 防御
    ITEMADDITIONDATA_MGCATK_MAX,	// 最大魔攻
    ITEMADDITIONDATA_MGCATK_MIN,	// 最小魔攻
    ITEMADDITIONDATA_MAGICDEF,		// 魔防
    ITEMADDITIONDATA_DEXTERITY,		// 敏捷
    ITEMADDITIONDATA_DODGE,			// 躲避
};

char szItemAdditionTable[];
typedef	CGameData<ITEMADDITIONDATA, szItemAdditionTable, szID>	CItemAdditionData;


// ITEMADDITIONDATA 应该增加一个ITEMADDITIONDATA_LEVEL,以此为第2个键
typedef ISingleMap64<CItemAdditionData, ITEMADDITIONDATA, ITEMADDITIONDATA_TYPEID, ITEMADDITIONDATA_LEVEL>	IItemAdditionSet64;
typedef CSingleMap64<CItemAdditionData, ITEMADDITIONDATA, ITEMADDITIONDATA_TYPEID, ITEMADDITIONDATA_LEVEL>	CItemAdditionSet64;

//////////////////////////////////////////////////////////////////////
class CItemAddition
{
public:
    CItemAddition();
    virtual ~CItemAddition();
public:
    static CItemAddition*	CreateNew	()		{ return new CItemAddition; }
    ULONG	Release			()					{ delete this; return 0; }
    bool	Create			(IDatabase* pDb);
    CItemAdditionData*	QueryItemAddition(OBJID idType, int nLevel);

protected:
    IItemAdditionSet64*	m_setAddition;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_ITEMADDITION_H__4C270148_7B1D_46C5_A799_B85CF277CF6E__INCLUDED_)
