
//**********************************************************
// 代码编辑器
//**********************************************************

// SynManager.cpp: implementation of the CSynManager class.
//
//////////////////////////////////////////////////////////////////////

#include "define.h"
#include "AllMsg.h"
#include "Syndicate.h"
#include "SynManager.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
const int	MINI_SYN_SIZE			= 0;				// 10人以下的帮派，算迷你帮派。不显示列表。

MYHEAP_IMPLEMENTATION(CSynManager, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSynManager::CSynManager(PROCESS_ID idProcess) : m_idProcess(idProcess)
{
    m_setSyn = CSynSet::CreateNew(true);
    m_obj.m_pOwner = this;
    m_idWhiteSyn	= ID_NONE;
    m_idBlackSyn	= ID_NONE;
}

//////////////////////////////////////////////////////////////////////
CSynManager::~CSynManager()
{
    if (m_setSyn)
    {
        m_setSyn->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::Create()
{
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s ORDER BY amount DESC", _TBL_SYNDICATE);
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
    CHECKF(pRes);
    for(int i = 0; i < pRes->RecordCount(); i++)
    {
        CSynPtr pSyn = CSyndicate::CreateNew(m_idProcess);
        IF_OK(pSyn)
        {
            if (pSyn->Create(pRes))
            {
                m_setSyn->AddObj(pSyn);
            }
            else
            {
                pSyn->ReleaseByOwner();
            }
        }
        pRes->MoveNext();
    }
    pRes->Release();
    return true;
}

//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
OBJID CSynManager::CreateSyndicate(const CreateSyndicateInfo* pInfo)
{
    OBJID idSyn = pInfo->idSyn;
    if (this->QuerySyndicate(idSyn))
    {
        return idSyn;
    }
    CSynPtr pSyn = CSyndicate::CreateNew(m_idProcess);
    IF_OK(pSyn)
    {
        IF_OK(pSyn->Create(pInfo, false))		// false: not save
        {
            m_setSyn->AddObj(pSyn);
            //			SQLBUF	szSQL;
            //			sprintf(szSQL, STR_SYNDICATE_CREATE_ss, pInfo->szLeader, pInfo->szName);
            //			RoleManager()->BroadcastMapGroupMsg(szSQL);
            //			if (strlen(pInfo->szTenet))
            //				RoleManager()->BroadcastMapGroupMsg(pInfo->szTenet);
            // set leader info
            CUser* pNewUser = UserManager()->GetUser(pInfo->idLeader);
            if (pNewUser)
            {
                pNewUser->QuerySynAttr()->SetIdRank(pSyn->GetID(), RANK_LEADER);
                pNewUser->QuerySynAttr()->SynchroInfo();
                //		pNewUser->QuerySynAttr()->SaveInfo();
            }
            return pInfo->idSyn;
        }
        else
        {
            pSyn->ReleaseByOwner();
        }
    }
    return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::DestroySyndicate(OBJID idSyn, OBJID idTarget)
{
    CSynPtr pSyn = QuerySyndicate(idSyn);
    if (pSyn)
    {
        SQLBUF	szSQL;
        sprintf(szSQL, STR_SYNDICATE_DESTROY_s, pSyn->GetStr(SYNDATA_LEADERNAME), pSyn->GetStr(SYNDATA_NAME));
        RoleManager()->BroadcastMapGroupMsg(szSQL);
        CMsgSyndicate	msg;
        IF_OK(msg.Create(DESTROY_SYN, idSyn))
        RoleManager()->BroadcastMapGroupMsg(&msg);
        CSyndicate* pTargetSyn = NULL;
        if (idTarget != ID_NONE)
        {
            pTargetSyn = SynManager()->QuerySyndicate(idTarget);
        }
        // 删除帮众
        CUserManager::Iterator pUser = UserManager()->NewEnum();
        while(pUser.Next())
        {
            if (pUser && pUser->QuerySynAttr()->GetSynID() == idSyn)
            {
                if (idTarget == ID_NONE)
                {
                    pUser->QuerySynAttr()->LeaveSyn(false, DELRECORD_FALSE, SYNCHRO_FALSE);    // false: 不删除记录，以防误操作。
                }
                else
                {
                    int nRank = pUser->QuerySynAttr()->GetSynRank();
                    if (nRank > RANK_NEWBIE)
                    {
                        nRank = RANK_NEWBIE;
                    }
                    pUser->QuerySynAttr()->SetIdRank(idTarget, nRank, SYNCHRO_FALSE);
                }
            }
        }
        m_setSyn->DelObj(idSyn);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::CombineSyndicate(OBJID idSyn, OBJID idTarget)
{
    CSynPtr pSyn	= QuerySyndicate(idSyn);
    CSynPtr pTarget	= QuerySyndicate(idTarget);
    if (pSyn && pTarget)
    {
        if (pSyn->GetInt(SYNDATA_FEALTY) == ID_NONE)
        {
            SQLBUF	szSQL;
            sprintf(szSQL, STR_SYNDICATE_COMBINESUB_ss, pSyn->GetStr(SYNDATA_NAME), pTarget->GetStr(SYNDATA_NAME));
            RoleManager()->BroadcastMapGroupMsg(szSQL);
        }
        // synchro
        CMsgAction	msg;
        IF_OK(msg.Create(idSyn, 0, 0, 0, actionCombineSubSyn, idTarget))
        RoleManager()->BroadcastMapGroupMsg(&msg);
        // 合并帮众
        CUserManager::Iterator pUser = UserManager()->NewEnum();
        while(pUser.Next())
        {
            if (pUser && pUser->QuerySynAttr()->GetSynID() == idSyn)
            {
                int nRank = pUser->QuerySynAttr()->GetSynRank();
                if (nRank > RANK_NEWBIE)
                {
                    nRank = RANK_NEWBIE;
                }
                pUser->QuerySynAttr()->SetIdRank(idTarget, nRank, SYNCHRO_FALSE);
            }
        }
        m_setSyn->DelObj(idSyn);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CSyndicate* CSynManager::QuerySynByName(LPCTSTR szSyn)
{
    CHECKF(szSyn);
    for(int i = 0; i < m_setSyn->GetAmount(); i++)
    {
        CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
        if (pSyn && strcmp(pSyn->GetStr(SYNDATA_NAME), szSyn) == 0)
        {
            return pSyn;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
// order by amount, return false: no more syn
bool CSynManager::FindNextSyn(int& nIter)
{
    OBJID	idLastSyn	= nIter;
    CSynPtr pTarget		= NULL;
    bool	bFindNext	= true;
    for(int i = 0; i < m_setSyn->GetAmount(); i++)
    {
        CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
        if (pSyn && pSyn->GetInt(SYNDATA_AMOUNT) > MINI_SYN_SIZE)
        {
            if (bFindNext)
            {
                pTarget = pSyn;
                bFindNext = false;
            }
            if (pSyn->GetID() == idLastSyn)
            {
                pTarget = NULL;
                bFindNext = true;
                continue;
            }
        }
    }
    if (pTarget)
    {
        nIter = pTarget->GetID();
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
OBJID CSynManager::XSynMngModify::CreateSyndicate(const CreateSyndicateInfo* pInfo)
{
    //先检查帮派是否存在---begin
    /*	sprintf(szSQL, "SELECT * FROM %s WHERE name='%s'", _TBL_SYNDICATE, pInfo->szName);
    	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
    	if (pRes)
    	{
    		//初始化已经有的数据---begin
    		pRes->SetString("name", info.szName, _MAX_NAMESIZE);
    		pRes->SetString("tenet", "", _MAX_NAMESIZE);
    		pRes->SetString("announce", "", _MAX_NAMESIZE);
    		pRes->SetString("member_title", "", _MAX_NAMESIZE);
    		pRes->SetInt("leader_id",info.idLeader);
    		pRes->SetString("leader_name", info.szLeader, _MAX_NAMESIZE);
    		pRes->SetInt("money", info.nMoney);
    		pRes->SetInt("fealty_syn", info.idFealty);
    		pRes->SetInt("del_flag", 0);
    		pRes->SetInt("amount", 0);

    		pRes->SetInt("enemy0", 0);	pRes->SetInt("enemy1", 0); pRes->SetInt("enemy2", 0);
    		pRes->SetInt("enemy3", 0);	pRes->SetInt("enemy4", 0);

    		pRes->SetInt("ally0", 0);	pRes->SetInt("ally1", 0);	pRes->SetInt("ally2", 0);
    		pRes->SetInt("ally3", 0);	pRes->SetInt("ally4", 0);

    		pRes->SetInt("rank", 1);
    		pRes->SetInt("saint", 0);
    		pRes->SetInt("mantle", info.nMantle);

    		pRes->SetInt("distime", 0);
    		pRes->SetInt("repute", 0);

    		pRes->UpdateRecord();

    		//初始化已经有的数据---end

    		info.idSyn = pRes->GetInt(0);
    		pRes->Release();

    		SynManager()->QuerySynchro()->CreateSyndicate(&info);

    	}
    	else
    	{
    */
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT %s SET name='%s',leader_id=%u,leader_name='%s',money=%d,fealty_syn=%d,mantle=%d,rank=1,saint=0",
            _TBL_SYNDICATE,
            pInfo->szName, pInfo->idLeader, pInfo->szLeader, pInfo->nMoney, pInfo->idFealty, pInfo->nMantle);
    LOGMSG(szSQL);
    if (!MapGroup(This()->m_idProcess)->GetDatabase()->ExecuteSQL(szSQL))
    {
        return ID_NONE;
    }
    CreateSyndicateInfo	info;
    memcpy(&info, pInfo, sizeof(info));
    info.idSyn = MapGroup(This()->m_idProcess)->GetDatabase()->GetInsertId();
    MapGroup(This()->m_idProcess)->QuerySynMessage()->CreateSyndicate(&info);
    return info.idSyn;
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::XSynMngModify::DestroySyndicate(OBJID idSyn, OBJID idTarget)
{
    CHECKF (idSyn != ID_NONE);
    // 递归调用，先解散子帮派
    for(int i = 0; i < This()->QuerySynSet()->GetAmount(); i++)
    {
        CSyndicate* pSyn = This()->QuerySynSet()->GetObjByIndex(i);
        if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == idSyn)
        {
            if (!DestroySyndicate(pSyn->GetID(), idTarget))
            {
                return false;
            }
        }
    }
    // 然后解散本帮派
    SQLBUF	szSQL;
    //	sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_SYNDICATE, idSyn);
    sprintf(szSQL, "UPDATE %s SET del_flag=1 WHERE id=%u LIMIT 1", _TBL_SYNDICATE, idSyn);
    LOGMSG(szSQL);
    if (!MapGroup(This()->m_idProcess)->GetDatabase()->ExecuteSQL(szSQL))
    {
        return false;
    }
    return MapGroup(This()->m_idProcess)->QuerySynMessage()->DestroySyndicate(idSyn, idTarget);
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::XSynMngModify::CombineSyndicate(OBJID idSyn, OBJID idTarget)
{
    CHECKF(idSyn);
    // 递归调用，先合并子帮派
    for(int i = 0; i < This()->QuerySynSet()->GetAmount(); i++)
    {
        CSyndicate* pSyn = This()->QuerySynSet()->GetObjByIndex(i);
        if (pSyn && pSyn->GetInt(SYNDATA_FEALTY) == idSyn)
        {
            if (!CombineSyndicate(pSyn->GetID(), idTarget))
            {
                return false;
            }
        }
    }
    // 合并本帮派
    SQLBUF	szSQL;
    //		sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_SYNDICATE, idSyn);
    sprintf(szSQL, "UPDATE %s SET del_flag=1 WHERE id=%u LIMIT 1", _TBL_SYNDICATE, idSyn);
    LOGMSG(szSQL);
    if (!MapGroup(This()->m_idProcess)->GetDatabase()->ExecuteSQL(szSQL))
    {
        return false;
    }
    CSyndicate* pSyn	= This()->QuerySyndicate(idSyn);
    CSyndicate* pTarget = This()->QuerySyndicate(idTarget);
    CHECKF(pSyn && pTarget);
    int nCount = pSyn->GetInt(SYNDATA_AMOUNT);
    int nMoney = pSyn->GetInt(SYNDATA_MONEY);
    if (MapGroup(This()->m_idProcess)->QuerySynMessage()->CombineSyndicate(idSyn, idTarget))
    {
        if (nCount)
        {
            pTarget->QueryModify()->AddData(SYNDATA_AMOUNT, nCount);
        }
        if (nMoney)
        {
            pTarget->QueryModify()->AddData(SYNDATA_MONEY, nMoney);
        }
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
void CSynManager::XSynMngModify::SetMapSynID(OBJID idMap, OBJID idSyn)
{
    CHECK(idMap);
    This()->SetMapSynID(idMap, idSyn);
    MapGroup(This()->m_idProcess)->QuerySynMessage()->SetMapSynID(idMap, idSyn);
}


