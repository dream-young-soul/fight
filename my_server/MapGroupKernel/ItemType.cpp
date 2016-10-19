
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ItemType.cpp: implementation of the CItemType class.
//
//////////////////////////////////////////////////////////////////////

#include "winsock2.h"
#include "ItemType.h"
#include "I_mydb.h"
#include "ItemData.h"

MYHEAP_IMPLEMENTATION(CItemType, s_heap)
char	szItemTypeTable[] = _TBL_ITEMTYPE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItemType::CItemType()
{
    m_setType	= NULL;
}

//////////////////////////////////////////////////////////////////////
CItemType::~CItemType()
{
    if (m_setType)
    {
        m_setType->Release();
    }
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
bool CItemType::Create(IDatabase* pDb)
{
    m_setType	= CItemTypeSet::CreateNew(true);
    CHECKF(m_setType);
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s", _TBL_ITEMTYPE);
    IF_NOT_(m_setType->Create(szSQL, pDb))
	{
		return false;
	}

    return true;
}

//////////////////////////////////////////////////////////////////////
CItemTypeData* CItemType::QueryItemType(OBJID idType)
{
    return m_setType->GetObj(idType);
}

//////////////////////////////////////////////////////////////////////
bool CItemType::GetInfo(OBJID idType, ItemInfoStruct* pInfo)
{
    memset(pInfo, 0, sizeof(ItemInfoStruct));
    CItemTypeData* pType = m_setType->GetObj(idType);
    CHECKF(pType);
    pInfo->id		= ID_NONE;
    pInfo->idOwner	= ID_NONE;
    pInfo->idPlayer	= ID_NONE;
    pInfo->idType	= pType->GetID();
    pInfo->nAmount	= pType->GetInt(ITEMTYPEDATA_AMOUNT);
    pInfo->nAmountLimit	= pType->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
    pInfo->nData	= 0;
    pInfo->nGem1	= pType->GetInt(ITEMTYPEDATA_GEM1);
    pInfo->nGem2	= pType->GetInt(ITEMTYPEDATA_GEM2);
    pInfo->nIdent	= pType->GetInt(ITEMTYPEDATA_IDENT);
    pInfo->nMagic1	= pType->GetInt(ITEMTYPEDATA_MAGIC1);
    pInfo->nMagic2	= pType->GetInt(ITEMTYPEDATA_MAGIC2);
    pInfo->nMagic3	= pType->GetInt(ITEMTYPEDATA_MAGIC3);
    pInfo->nPosition	= ITEMPOSITION_NONE;
    //---jinggy---2004-11-19---begin
    pInfo->dwWarGhostExp = 0;
    pInfo->dwGemAtkType = ID_NONE;
    pInfo->dwAvailabeTime = 0;
    //---jinggy---2004-11-19---end
    return true;
}




