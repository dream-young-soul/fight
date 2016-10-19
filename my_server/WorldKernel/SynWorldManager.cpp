
//**********************************************************
// 代码编辑器
//**********************************************************

// SynManager.cpp: implementation of the CSynWorldManager class.
//
//////////////////////////////////////////////////////////////////////

#define	_WINSOCKAPI_		// 阻止加载winsock.h
#include "MessagePort.h"
#include "windows.h"
#include "define.h"
#include "MessageBoard.h"
#include "SynWorldShell.h"
#include "SynWorldManager.h"
#include "WorldKernel.h"

MYHEAP_IMPLEMENTATION(CSynWorldManager, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSynWorldManager::CSynWorldManager()
{
    m_setSyn = CSynWorldSet::CreateNew(true);
    m_obj.m_pOwner	= this;
    m_pDefault		= NULL;
}

//////////////////////////////////////////////////////////////////////
CSynWorldManager::~CSynWorldManager()
{
    SAFE_RELEASE (m_setSyn);
    SAFE_RELEASE (m_pDefault);
}

//////////////////////////////////////////////////////////////////////
bool CSynWorldManager::Create(IDatabase* pDb)
{
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s", _TBL_SYNDICATE);
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
    CHECKF(pRes);
    for(int i = 0; i < pRes->RecordCount(); i++)
    {
        CSyndicateWorld* pSyn = CSyndicateWorld::CreateNew();
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
    sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_SYNDICATE);
    m_pDefault	= pDb->CreateNewRecordset(szSQL, false);
    CHECKF(m_pDefault);
    return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CSynWorldManager::CreateSyndicate(const CreateSyndicateInfo* pInfo)
{
    CSyndicateWorld* pSyn = CSyndicateWorld::CreateNew();
    IF_OK(pSyn)
    {
        IF_OK(pSyn->Create(m_pDefault, pInfo, false))		// false: not save
        {
            m_setSyn->AddObj(pSyn);
            SQLBUF	szSQL;
            sprintf(szSQL, "恭喜 %s 创建%s成功。 宗旨是：", pInfo->szLeader, pInfo->szName);
            //			UserManager()->BroadcastMapGroupMsg(szSQL);
            //			UserManager()->BroadcastMapGroupMsg(pInfo->szTenet);
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
bool CSynWorldManager::DestroySyndicate(OBJID idSyn, OBJID idTarget)
{
    CSyndicateWorld* pSyn = QuerySyndicate(idSyn);
    if (pSyn)
    {
        SQLBUF	szSQL;
        sprintf(szSQL, "帮派 %s 解散了。", pSyn->GetStr(SYNDATA_NAME));
        //		UserManager()->BroadcastMapGroupMsg(szSQL);
        m_setSyn->DelObj(idSyn);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynWorldManager::CombineSyndicate(OBJID idSyn, OBJID idTarget)
{
    CSyndicateWorld* pSyn = QuerySyndicate(idSyn);
    if (pSyn)
    {
        //		SQLBUF	szSQL;
        //		sprintf(szSQL, "帮派 %s 合并了。", pSyn->GetStr(SYNDATA_NAME));
        //		UserManager()->BroadcastMapGroupMsg(szSQL);
        // combine member
        CSyndicateWorld* pTarget = QuerySyndicate(idTarget);
        if (pTarget)
        {
            pTarget->CombineMemberList(pSyn->QueryMemberList());
        }
        m_setSyn->DelObj(idSyn);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CSynWorldManager::XSynMngModify::CreateSyndicate(const CreateSyndicateInfo* pInfo)
{
    OBJID	ret = ID_NONE;
    CSyndicateWorld* pSyn = CSyndicateWorld::CreateNew();
    IF_OK(pSyn)
    {
        IF_OK(pSyn->Create(This()->m_pDefault, pInfo, false))		// true: save know
        {
            This()->m_setSyn->AddObj(pSyn);
            SQLBUF	szSQL;
            sprintf(szSQL, "[%s]创建帮派[%s]. 宗旨：...", pInfo->szLeader, pInfo->szName);
            LOGMSG(szSQL);
            LOGMSG(pInfo->szTenet);
            ret = pInfo->idSyn;
        }
        else
        {
            pSyn->ReleaseByOwner();
        }
    }
    if (ret)
    {
        // broadcast
        GameWorld()->QuerySynMessage()->CreateSyndicate(pInfo);
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////
bool CSynWorldManager::XSynMngModify::DestroySyndicate(OBJID idSyn, OBJID idTarget)
{
    bool	ret = false;
    CSyndicateWorld* pSyn = This()->QuerySyndicate(idSyn);
    if (pSyn)
    {
        This()->m_setSyn->DelObj(idSyn);
        ret = true;
    }
    if (ret)
    {
        // broadcast
        GameWorld()->QuerySynMessage()->DestroySyndicate(idSyn, idTarget);
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////
bool CSynWorldManager::XSynMngModify::CombineSyndicate(OBJID idSyn, OBJID idTarget)
{
    bool	ret = false;
    CSyndicateWorld* pSyn = This()->QuerySyndicate(idSyn);
    if (pSyn)
    {
        // combine member
        CSyndicateWorld* pTarget = This()->QuerySyndicate(idTarget);
        if (pTarget)
        {
            pTarget->CombineMemberList(pSyn->QueryMemberList());
        }
        This()->m_setSyn->DelObj(idSyn);
        ret = true;
    }
    if (ret)
    {
        // broadcast
        GameWorld()->QuerySynMessage()->CombineSyndicate(idSyn, idTarget);
    }
    return ret;
}









int CSynWorldManager::GetSynAmount()
{
    return this->m_setSyn->GetAmount();
}

CSyndicateWorld* CSynWorldManager::GetSynByIndex(int nIndex)
{
    return m_setSyn->GetObjByIndex(nIndex);
}
