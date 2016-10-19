
//**********************************************************
// 代码编辑器
//**********************************************************

// DropRule.cpp: implementation of the CDropRule class.
//
//////////////////////////////////////////////////////////////////////

#include "DropRule.h"
#include "basefunc.h"


MYHEAP_IMPLEMENTATION(CDropRule, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropRule::CDropRule()
{
    m_idRule	= ID_NONE;
}

CDropRule::~CDropRule()
{
    m_setItemType.clear();
}

//////////////////////////////////////////////////////////////////////
// 可以重复Create——重复Create则认为是同一个概率的物品
bool	CDropRule::Create(IRecordset*	pRes)
{
    if (!pRes)
    {
        return false;
    }
    IRecord* pRecord = pRes->CreateNewRecord();
    bool ret = LoadInfo(pRecord);
    SAFE_RELEASE (pRecord);
    return ret;
}
//////////////////////////////////////////////////////////////////////

bool	CDropRule::LoadInfo(IRecord*	pRes)
{
    if (!pRes)
    {
        return false;
    }
    if (m_idRule == ID_NONE)
    {
        m_idRule	= pRes->LoadDWord("ruleid");
        m_nChance	= pRes->LoadDWord("Chance");
    }
    DEBUG_TRY
    for (int i = DROPRULEDATA_ITEM0; i <= DROPRULEDATA_ITEM14; i++)
    {
        OBJID	idType	= (OBJID)pRes->GetInt(i);
        if (idType != ID_NONE)
        {
            m_setItemType.push_back(idType);
        }
    }
    DEBUG_CATCH("CDropRule::LoadInfo()")
    return true;
}
//////////////////////////////////////////////////////////////////////

OBJID	CDropRule::GetDropItem(int nTotalChance, int nRand)
{
    OBJID	idType = ID_NONE;
    // TODO: 根据掉宝几率计算掉的物品ID；如果计算出来不掉物品则返回ID_NONE
    DEBUG_TRY
    if (m_nChance > 0)
    {
        if (nRand < nTotalChance + m_nChance)
        {
            //掉宝概率以万分比为单位
            int nIndex = ::RandGet(m_setItemType.size());
            idType = m_setItemType[nIndex];
        }
    }
    DEBUG_CATCH("CDropRule::GetDropItem()")
    return idType;
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
