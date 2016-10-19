
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcScene.cpp: implementation of the CNpcScene class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "GameMap.h"
#include "Npc.h"
#include "Myheap.h"
#include "NpcScene.h"

MYHEAP_IMPLEMENTATION(CNpcScene, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcScene::CNpcScene()
{
    m_pOwner = NULL;
}

//////////////////////////////////////////////////////////////////////
CNpcScene::~CNpcScene()
{
    DelTerrainObj();
}

//////////////////////////////////////////////////////////////////////
void CNpcScene::DelTerrainObj()
{
    if (m_pOwner && m_pMap && m_bTerrainObj)
    {
        m_bTerrainObj = false;
        m_pMap->DelTerrainObj(m_pOwner->GetID());
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpcScene::Create(CNpc* pOwner, CGameMap* pMap)
{
    CHECKF(pOwner);
    CHECKF(pMap);
    m_pOwner = pOwner;
    m_pMap	 = pMap;
    if (!m_pMap->IsValidPoint(m_pOwner->GetPosX(), m_pOwner->GetPosY()))
    {
        LOGERROR("Scenc Npc[%d] position is invalid!", m_pOwner->GetRecordID());
        return false;
    }
    if (m_pMap->AddTerrainObj(m_pOwner->GetID(),
                             m_pOwner->GetPosX(), m_pOwner->GetPosY(),
                             m_pOwner->GetInt(NPCDATA_BASE) * 10 + m_pOwner->GetDir()))
    {
        m_bTerrainObj = true;
        return true;
    }
    return false;
}






