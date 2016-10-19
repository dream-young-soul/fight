
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcType.cpp: implementation of the CNpcType class.
//
//////////////////////////////////////////////////////////////////////

#include "NpcType.h"

char	szNpcType[] = _TBL_MONSTERTYPE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcType::CNpcType()
{
    SetObjType(OBJ_NPCTYPE);
    m_pData	= CNpcTypeData::CreateNew();
}

//////////////////////////////////////////////////////////////////////
CNpcType::~CNpcType()
{
    if (m_pData)
    {
        m_pData->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpcType::Create(IRecordset* pRes)
{
    CHECKF(m_pData);
    if (!m_pData->Create(pRes))
    {
        return false;
    }
    return true;
}

