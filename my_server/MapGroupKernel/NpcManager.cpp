
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcManager.cpp: implementation of the CNpcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#include "AllMsg.h"
#include "protocol.h"
#include "MapGroup.h"
#include "MapTrap.h"

MYHEAP_IMPLEMENTATION(CNpcManager, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcManager::CNpcManager(PROCESS_ID idProcess) : m_idProcess(idProcess)
{
}

//////////////////////////////////////////////////////////////////////
CNpcManager::~CNpcManager()
{
}

//////////////////////////////////////////////////////////////////////
bool CNpcManager::Create()
{
    // tasknpc set
    SQLBUF szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE (idxserver=-1 || idxserver=%u)", _TBL_NPC, GetServerID());
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
    if (!pRes)
    {
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++)
    {
        CNpcPtr pNpc = CNpc::CreateNew();
        if (!pNpc)
        {
            pRes->Release();
            return false;
        }
        if (!pNpc->Create(m_idProcess, pRes, false))		// false: not dynamic
        {
            pNpc->ReleaseByOwner();    //?? 非本地图组NPC
        }
        else
        {
            RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
        }
        pRes->MoveNext();
    }
    pRes->Release();
    // dyn npc set
    sprintf(szSQL, "SELECT * FROM %s WHERE (idxserver=-1 || idxserver=%u)", _TBL_DYNANPC, GetServerID());
    pRes = Database()->CreateNewRecordset(szSQL, false);
    if (!pRes)
    {
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++)
    {
        CNpcPtr pNpc = CNpc::CreateNew();
        if (!pNpc)
        {
            pRes->Release();
            return false;
        }
        if (!pNpc->Create(m_idProcess, pRes, true))		// true: dynamic
        {
            pNpc->ReleaseByOwner();    //?? 非本地图组NPC
        }
        else
        {
            RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
        }
        pRes->MoveNext();
    }
    pRes->Release();
    // map trap set (借过：)
    sprintf(szSQL, "SELECT * FROM %s", _TBL_TRAP);
    pRes = Database()->CreateNewRecordset(szSQL, false);
    if (!pRes)
    {
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        CMapTrap* pTrap = CMapTrap::CreateNew(m_idProcess, pRes);
        if (pTrap)		//? may be other map group
        {
            RoleManager()->QuerySet()->AddObj(pTrap->QueryRole());
        }
    }
    pRes->Release();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcManager::DelAllMonster()
{
    DEBUG_TRY		// VVVVVVVVVVVVVVV
    LOGMSG("DelAllMonster() start...");
    IRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
    while(pRole.Next())
    {
        CNpc* pNpc;
        CUser* pUser;
        if (pRole && (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pNpc)) || pRole->QueryObj(OBJ_USER, IPP_OF(pUser)) && pUser->IsAgent()))
        {
            RoleManager()->QuerySet()->DelObj(pRole);
        }
    }
    LOGMSG("DelAllMonster() ok.");
    DEBUG_CATCH("delete all npc")		// AAAAAAAAAAAAAAAAA
    //	RoleManager()->LoadAllPet();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcManager::CreateMonster(const ST_CREATENEWNPC* pInfo)
{
    CMonsterPtr pNpc = CMonster::CreateNew();
    if (pNpc)
    {
        CNpcType*	pType = MonsterType()->GetObj(pInfo->usType);
        if (pType)
        {
            ST_CREATENEWNPC	info;
            memcpy(&info, pInfo, sizeof(ST_CREATENEWNPC));
            info.idOwner	= ID_NONE;
            info.ucOwnerType = OWNER_NONE;
            info.usAction	= MSGAINPCINFO_CREATENEW;
            CMapPtr pMap = MapManager()->GetGameMap(pInfo->idMap);			//? need not ?
            if (pMap && pNpc->Create(m_idProcess, pType, &info))
            {
                RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
                pNpc->EnterMapGroup();
                return true;
            }
        }
        pNpc->ReleaseByOwner();
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpcManager::CreateSynPet(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName/*=NULL*/)
{
    CHECKF(pInfo);
    CMonsterPtr pPet = CMonster::CreateNew();
    if (pPet)
    {
        CNpcType*	pType = MonsterType()->GetObj(pInfo->usType);
        if (pType)
        {
            ST_CREATENEWNPC	info;
            memcpy(&info, pInfo, sizeof(ST_CREATENEWNPC));
            info.id				= MapManager()->SpawnNewPetID();
            info.usAction		= MSGAINPCINFO_CREATENEW;
            CMapPtr pMap = MapManager()->QueryMap(pInfo->idMap);			//??? need not? mast check not current mapgroup case!!!
            if (pMap && pPet->Create(m_idProcess, pType, &info, pszName))
            {
                RoleManager()->QuerySet()->AddObj(pPet->QueryRole());
                pPet->EnterMapGroup();
                return true;
            }
        }
        pPet->ReleaseByOwner();
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CMonster* CNpcManager::CreateCallPet(CUser* pUser, OBJID idType, int x, int y)
{
    CHECKF(pUser);
    CMonsterPtr pPet = CMonster::CreateNew();
    if (pPet)
    {
        CNpcType*	pType = MonsterType()->GetObj(idType);
        if (pType)
        {
            ST_CREATENEWNPC	info;
            memset(&info, 0, sizeof(ST_CREATENEWNPC));
            info.id				= MapManager()->SpawnNewCallPetID();
            info.usAction		= MSGAINPCINFO_CREATENEW;
            info.usType			= idType;
            info.ucOwnerType	= OWNER_USER;
            info.idOwner		= pUser->GetID();
            info.idMap			= pUser->GetMapID();
            info.usPosX			= x;
            info.usPosY			= y;
            info.idData			= pType->GetInt(NPCTYPEDATA_AITYPE);
            info.nData			= 0;
            CMapPtr pMap = pUser->GetMap();			//??? need not? mast check not current mapgroup case!!!
            if (pMap && pPet->Create(m_idProcess, pType, &info, pUser))
            {
                RoleManager()->QuerySet()->AddObj(pPet->QueryRole());
                pPet->EnterMapGroup();
                return pPet;
            }
        }
        pPet->ReleaseByOwner();
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CMonster* CNpcManager::CreateEudemon(CUser* pUser, CItem* pItem, int x, int y)
{
    CHECKF(pUser);
    CMonsterPtr pEudemon = CMonster::CreateNew();
    if (pEudemon)
    {
        CNpcType*	pType = MonsterType()->GetObj(pItem->GetInt(ITEMDATA_MONSTERTYPE));
        if (pType)
        {
            ST_CREATENEWNPC	info;
            memset(&info, 0, sizeof(ST_CREATENEWNPC));
            // 幻兽ID直接从cq_eudemon表中的id字段取得，不再用动态ID
            info.id				= pItem->GetEudemonID();				//MapManager()->SpawnNewCallPetID();
            info.usAction		= MSGAINPCINFO_CREATENEW;
            info.usType			= pItem->GetInt(ITEMDATA_MONSTERTYPE);
            info.ucOwnerType	= OWNER_USER;
            info.idOwner		= pUser->GetID();
            info.idMap			= pUser->GetMapID();
            info.usPosX			= x;
            info.usPosY			= y;
            info.idData			= pType->GetInt(NPCTYPEDATA_AITYPE);
            info.nData			= 0;
            CMapPtr pMap = pUser->GetMap();			//??? need not? mast check not current mapgroup case!!!
            if (pMap && pEudemon->Create(m_idProcess, pType, &info, pUser, pItem->GetID()))
            {
                pEudemon->SetEudemonAddi(pItem);	// 根据幻兽成长率、等级调整属性
                RoleManager()->QuerySet()->AddObj(pEudemon->QueryRole());
                pEudemon->EnterMapGroup();
                if (pEudemon->QueryMagic())
                {
                    pEudemon->QueryMagic()->SendInfoToClient();
                }
                return pEudemon;
            }
        }
        pEudemon->ReleaseByOwner();
    }
    return NULL;
}
