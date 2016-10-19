
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcLink.cpp: implementation of the CNpcLink class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "MapGroup.h"
#include "Npc.h"
#include "Myheap.h"
#include "NpcLink.h"

MYHEAP_IMPLEMENTATION(CNpcLink, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcLink::CNpcLink()
{
}

//////////////////////////////////////////////////////////////////////
CNpcLink::~CNpcLink()
{
}

//////////////////////////////////////////////////////////////////////
CNpcLink* CNpcLink::CreateNew(PROCESS_ID idProcess, CNpc* pOwner)
{
    CNpcLink* pObj = new CNpcLink;
    CHECKF(pObj);
    pObj->m_pOwner		= pOwner;
    pObj->m_idProcess	= idProcess;
    pObj->m_pLinkMap	= NULL;
    return pObj;
}

//////////////////////////////////////////////////////////////////////
CGameMap* CNpcLink::GetLinkMap()
{
    if (m_pLinkMap)
    {
        return m_pLinkMap;
    }
    m_pLinkMap	= MapManager()->QueryMap(m_pOwner->GetInt(NPCDATA_LINKID));
    CHECKF(m_pLinkMap);
    return m_pLinkMap;
}

//////////////////////////////////////////////////////////////////////
bool CNpcLink::EraseMap()
{
    CGameMap* pMap = GetLinkMap();
    if (!pMap)
    {
        return true;
    }
    if (pMap->EraseMap())
    {
        m_pLinkMap	= NULL;
        return true;
    }
    return false;
}






