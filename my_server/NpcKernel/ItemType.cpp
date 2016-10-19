
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ItemType.cpp: implementation of the CItemType class.
//
//////////////////////////////////////////////////////////////////////

#include "winsock2.h"
#include "ItemType.h"
#include "I_mydb.h"

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
    return false;
    return true;
}

//////////////////////////////////////////////////////////////////////
CItemTypeData* CItemType::QueryItemType(OBJID idType)
{
    return m_setType->GetObj(idType);
}




