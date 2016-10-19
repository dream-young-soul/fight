
//**********************************************************
// 代码编辑器
//**********************************************************

// RoleManager.cpp: implementation of the CRoleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "RoleManager.h"
#include "MapGroup.h"
#include "AllMsg.h"

MYHEAP_IMPLEMENTATION(CRoleManager, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CRoleManager::CRoleManager(PROCESS_ID idProcess) : m_idProcess(idProcess)
{
    m_setRole	= CRoleSet::CreateNew(true);
}

//////////////////////////////////////////////////////////////////////
CRoleManager::~CRoleManager()
{
    if (m_setRole)
    {
        m_setRole->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CRoleManager::Create()
{
    CHECKF(m_setRole);
    // need not	LoadAllPet();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CRoleManager::LoadAllPet()		//??? 最好由AI服务器分段创建
{
    CHECKF(m_setRole);
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s", _TBL_PET);
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
        {
            CMonster* pMonster = CMonster::CreateNew();
            IF_OK(pMonster)
            {
                if (pMonster->Create(m_idProcess, pRes))			// false: not this map group
                {
                    m_setRole->AddObj(pMonster->QueryRole());
                    pMonster->EnterMapGroup();
                }
                else
                {
                    pMonster->ReleaseByOwner();
                }
            }
        }
        pRes->Release();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
IRole* CRoleManager::QueryRole(CNetMsg* pMsg, OBJID idRole)
{
    if (!pMsg->IsNpcMsg())
    {
        CUserPtr pUser = UserManager()->GetUserBySocketID(pMsg->GetSocketID());
        if (pUser)
        {
            CMonster* pEudemon = NULL;
            if (idRole && idRole == pUser->GetCallPetID())
            {
                return pUser->QueryCallPet()->QueryRole();
            }
            else if (idRole && (pEudemon = pUser->QueryEudemonByID(idRole)))
            {
                return pEudemon->QueryRole();
            }
            //			else if (idRole && idRole == pUser->GetEudemonID())		// 幻兽
            //				return pUser->QueryEudemon()->QueryRole();
            else
            {
                if (idRole >= CALLPETID_FIRST && idRole <= CALLPETID_LAST
                        || idRole >= EUDEMON_ID_FIRST && idRole <= EUDEMON_ID_LAST)
                {
                    return NULL;
                }
                else
                {
                    return pUser->QueryRole();
                }
            }
        }
    }
    else
    {
        return  QuerySet()->GetObj(pMsg->GetNpcID());
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::OnTimer(time_t tCurr)
{
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); )		// 删除
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        i++;		//??? 提前移动，下面可能会删除当前角色
        if (pRole)
        {
            DEBUG_TRY
            pRole->OnTimer(tCurr);			// 可能删除MONSTER
            if (pRole->IsDelThis())
            {
                m_setRole->DelObj(pRole->GetID());
            }
            DEBUG_CATCH("@CRoleManager::OnTimer@")
        }
    }
    /*	for(int i = m_setRole->GetAmount()-1; i >= 0; i--)		// 删除
    	{
    		IRole* pRole = m_setRole->GetObjByIndex(i);
    		if (pRole)
    			pRole->OnTimer(tCurr);
    	}*/
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::SaveAll()
{
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole)
        {
            pRole->SaveInfo();
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
    CHECK(pMsg);
    BroadcastMapGroupMsg(pMsg, pExcludeSender);
    MapGroup(PID)->QueryIntraMsg()->SendBroadcastMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender)
{
    CHECK(szMsg);
    CMsgTalk	msg;
    if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg))
    {
        BroadcastMsg(&msg, pExcludeSender);
    }
}

//////////////////////////////////////////////////////////////////////
// for mapgroup
//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
    ASSERT(pMsg);
    UserManager()->GetForMapGroup()->BroadcastMapGroupMsg(pMsg, pExcludeSender);
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMapGroupMsg(LPCTSTR szMsg, CUser* pExcludeSender)
{
    CHECK(szMsg);
    CMsgTalk	msg;
    if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg))
    {
        UserManager()->GetForMapGroup()->BroadcastMapGroupMsg(&msg, pExcludeSender);
    }
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountAllNpc(OBJID idMap)
{
    int		nCount = 0;
    CNpc*	pNpc;
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap)
        {
            nCount++;
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountFurnitureNpc(OBJID idMap)
{
    int		nCount = 0;
    CNpc*	pNpc;
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap && (pNpc->GetType() == _ROLE_FURNITURE_NPC || pNpc->GetType() == _ROLE_3DFURNITURE_NPC))
        {
            nCount++;
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountNpcByName(OBJID idMap, LPCTSTR szName)
{
    int		nCount = 0;
    CNpc*	pNpc;
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap && strcmp(pNpc->GetName(), szName) == 0)
        {
            nCount++;
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountNpcByType(OBJID idMap, int nType)
{
    int		nCount = 0;
    CNpc*	pNpc;
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap && pNpc->GetType() ==  nType)
        {
            nCount++;
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountMonsterByName(OBJID idMap, LPCTSTR szName)
{
    int		nCount = 0;
    CMonster*	pMonster;
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->GetMap()->GetID() == idMap && strcmp(pMonster->GetName(), szName) == 0)
        {
            nCount++;
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountMonsterByGenID(OBJID idMap, OBJID idGen)
{
    int		nCount = 0;
    CMonster*	pMonster;
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->GetMap()->GetID() == idMap && pMonster->GetGenID() ==  idGen)
        {
            nCount++;
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
CNpc* CRoleManager::QueryDynNpcByRecordID(OBJID idNpcRecord)
{
    CNpc*	pNpc;
    for(IRoleSet::Iter_t i = m_setRole->Begin(); i != m_setRole->End(); i++)
    {
        IRole* pRole = m_setRole->GetObjByIter(i);
        if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsDynaNpc() && pNpc->GetRecordID() == idNpcRecord)
        {
            return pNpc;
        }
    }
    return NULL;
}
