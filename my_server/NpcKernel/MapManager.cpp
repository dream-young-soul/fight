
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MapManager.cpp: implementation of the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "MapManager.h"
#include "NpcWorld.h"
#include "GameMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapManager::CMapManager()
{
    m_pMapSet = CGameMapSet::CreateNew(true);		// true : owner
}

//////////////////////////////////////////////////////////////////////
CMapManager::~CMapManager()
{
    if (m_pMapSet)
    {
        m_pMapSet->Release();
    }
    //	CGameMapData::Final();
}

//////////////////////////////////////////////////////////////////////
bool CMapManager::Create()
{
    CHECKF(m_pMapSet);
    //	if (!CGameMapData::Init(NpcWorld()->GetDatabase()))
    //		return false;
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s LIMIT 1234567890", _TBL_MAP);
    IRecordset* pRes = NpcWorld()->GetDatabase()->CreateNewRecordset(szSQL);
    if (!pRes)
    {
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        CMapPtr pMap = CGameMap::CreateNew();
        if (!pMap)
        {
            pRes->Release();
            return false;
        }
#ifdef	PALED_DEBUG
        if (pRes->GetInt(0) == 1041 || pRes->GetInt(0) >= 1060 && pRes->GetInt(0) <= 1062)		//? ²»¼ÓÔØ¡°À¥ÂØÃÔ¹¬¡±£¬Âý£º(
        {
            pMap->ReleaseByOwner();
            continue;	// szSQL[0]=0;//
        }
#endif
        if (!pMap->Create(pRes))
        {
            pMap->ReleaseByOwner();
#ifdef	LOCAL_DEBUG
            NpcWorld()->PrintText("Can't init a map data!");
            continue;
#endif
            pRes->Release();
            return false;
        }
        m_pMapSet->AddObj(pMap);
    }
    pRes->Release();
    return true;
}

//////////////////////////////////////////////////////////////////////
CGameMap* CMapManager::QueryMap(OBJID idMap)
{
    CHECKF(idMap != ID_NONE);
    for(int i = 0; i < m_pMapSet->GetAmount(); i++)
    {
        CMapPtr pMap = m_pMapSet->GetObjByIndex(i);
        if (pMap && pMap->GetID() == idMap)
        {
            return pMap;
        }
    }
    // load dyna map
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE id=%u && del_flag=0 LIMIT 1", _TBL_DYNAMAP, idMap);
    CAutoPtr<IRecordset> pRes = NpcWorld()->GetDatabase()->CreateNewRecordset(szSQL);
    if (pRes)
    {
        CMapPtr pMap = CGameMap::CreateNew();
        if (!pMap)
        {
            return NULL;
        }
        IF_NOT(pMap->Create(pRes))
        {
            pMap->ReleaseByOwner();
#ifdef	LOCAL_DEBUG
            NpcWorld()->PrintText("Can't init a dyna map data!");
#endif
            return NULL;
        }
        m_pMapSet->AddObj(pMap);
        return pMap;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
void CMapManager::OnTimer(DWORD nCurr)
{
}


