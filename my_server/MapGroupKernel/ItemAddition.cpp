
//**********************************************************
// 代码编辑器
//**********************************************************

// ItemAddition.cpp: implementation of the CItemAddition class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemAddition.h"
#include "i_mydb.h"
#include "Item.h"

MYHEAP_IMPLEMENTATION(CItemAddition, s_heap)
char szItemAdditionTable[] = _TBL_ITEMADDITION;		//"cq_itemaddition";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemAddition::CItemAddition()
{
    m_setAddition = NULL;
}

CItemAddition::~CItemAddition()
{
    if (m_setAddition)
    {
        m_setAddition->Release();
    }
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
bool CItemAddition::Create(IDatabase* pDb)
{
    IF_NOT (pDb)
    return false;
    m_setAddition	= CItemAdditionSet64::CreateNew(true);
    IF_NOT (m_setAddition)
    return false;
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s", _TBL_ITEMADDITION);
    IF_NOT_(m_setAddition->Create(szSQL, pDb))
    return false;
    return true;
}

//////////////////////////////////////////////////////////////////////
CItemAdditionData* CItemAddition::QueryItemAddition(OBJID idType, int nLevel)
{
    OBJID idKey = idType;
    /*
    	if (CItem::IsMSword(idType) || CItem::IsBow(idType))
    	{
    		// 法剑和弓不做修改，直接使用type
    	}
    	else if (CItem::IsWeapon1(idType))
    	{
    		idKey = (idType/100000)*100000 + idType % 1000 + 44000;	// 短兵器统一用444***类型
    	}
    	else if (CItem::IsWeapon2(idType))
    	{
    		idKey = (idType/100000)*100000 + idType % 1000 + 55000;	// 长兵器统一用555***类型
    	}
    	else if (CItem::IsHelmet(idType) || CItem::IsArmor(idType) || CItem::IsShield(idType))
    	{
    		idKey = (idType/1000)*1000 + idType % 100;				// 头盔、衣服、盾牌统一把表示颜色的百位置0
    	}
    */
    _int64	key = (idKey / 10) * 10;	// 不考虑物品品质，最后一位全部清0 -- zlong 2004-02-08
    key = (key << 32) + nLevel;
    return m_setAddition->GetObj(key);
}
