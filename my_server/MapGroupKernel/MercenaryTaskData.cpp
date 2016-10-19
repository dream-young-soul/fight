
//**********************************************************
// 代码编辑器
//**********************************************************

// MercenaryTaskData.cpp: implementation of the CMercenaryTaskData class.
//
//////////////////////////////////////////////////////////////////////

#include "MercenaryTaskData.h"
#include "MapGroup.h"


char	szMercenaryTaskTable[]	= _TBL_MERCENARYTASK;

MYHEAP_IMPLEMENTATION(CMercenaryTaskData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMercenaryTaskData::CMercenaryTaskData()
    : m_pData(NULL), m_pResultItem(NULL)
{
}

CMercenaryTaskData::~CMercenaryTaskData()
{
    SAFE_RELEASE (m_pData);
    if (m_pResultItem)
    {
        m_pResultItem->ReleaseByOwner();
    }
    ITEM_SET::iterator it = m_setPrizeItem.begin();
    for (; it != m_setPrizeItem.end(); it++)
    {
        CItem* pItem = *it;
        if (pItem)
        {
            pItem->ReleaseByOwner();
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CMercenaryTaskData::Create(OBJID idTask, IDatabase* pDb)
{
    CHECKF (idTask != ID_NONE);
    CHECKF (!m_pData);
    CHECKF (pDb);
    // load task data
    m_pData	= CGameMTaskData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(idTask, pDb))
    return false;
    // load prize items
    return LoadItems(pDb);
}

//////////////////////////////////////////////////////////////////////
bool CMercenaryTaskData::Create(IRecordset* pRes, IDatabase* pDb)
{
    CHECKF (!m_pData);
    CHECKF (pRes);
    // load task data
    m_pData	= CGameMTaskData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(pRes))
    return false;
    return LoadItems(pDb);
}

//////////////////////////////////////////////////////////////////////
bool CMercenaryTaskData::CreateRecord(const ST_MTASK_DATA* pInfo, IDatabase* pDb)
{
    CHECKF (!m_pData);
    CHECKF (pInfo);
    CHECKF (pDb);
    m_pData = CGameMTaskData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(pDb, ID_NONE))		// create default
    return false;
    // fill data
    m_pData->SetInt(MTASKDATA_TYPE, pInfo->ucType);
    m_pData->SetInt(MTASKDATA_USER_ID, pInfo->idUser);
    m_pData->SetStr(MTASKDATA_USER_NAME, pInfo->szUserName, _MAX_NAMESIZE);
    m_pData->SetStr(MTASKDATA_TITLE, pInfo->szTitle, 32);
    m_pData->SetStr(MTASKDATA_DETAIL, pInfo->szDetail, 128);
    m_pData->SetStr(MTASKDATA_TARGET_NAME, pInfo->szTargetName, _MAX_NAMESIZE);
    m_pData->SetInt(MTASKDATA_DURA, pInfo->dwDura);
    m_pData->SetInt(MTASKDATA_ADDITION, pInfo->ucAddition);
    m_pData->SetInt(MTASKDATA_DEGREE, pInfo->ucDegree);
    m_pData->SetInt(MTASKDATA_PRIZE_MONEY, pInfo->dwPrizeMoney);
    for (int i = 0; i < _MAX_PRIZE; i++)
    {
        m_pData->SetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i), pInfo->idPrizeItems[i]);
    }
    m_pData->SetInt(MTASKDATA_RANK_REQ, pInfo->ucRankReq);
    m_pData->SetInt(MTASKDATA_ACCEPT_USERID, pInfo->idAcceptUser);
    m_pData->SetInt(MTASKDATA_ACCEPT_TIME, 0);
    m_pData->SetInt(MTASKDATA_RESULT_ITEMID, pInfo->idResultItem);
    m_pData->SetInt(MTASKDATA_STATE, MTASKSTATE_NONE);
    // load items
    if (!LoadItems(pDb))
    {
        return false;
    }
    if (ID_NONE == m_pData->InsertRecord())	// insert record
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMercenaryTaskData::DeleteRecord()
{
    return m_pData->DeleteRecord();
}

//////////////////////////////////////////////////////////////////////
bool CMercenaryTaskData::LoadItems(IDatabase* pDb)
{
    CHECKF (m_pData);
    CHECKF (pDb);
    OBJID idItem = m_pData->GetInt(MTASKDATA_RESULT_ITEMID);
    if (idItem != ID_NONE)
    {
        m_pResultItem = CItem::CreateNew();
        if (!m_pResultItem || !m_pResultItem->Create(idItem, pDb))
        {
            ::LogSave("Mercenary task data load item failed! [idItem=%u]", idItem);
            return false;
        }
    }
    m_setPrizeItem.resize(_MAX_PRIZE);
    for (int i = 0; i < _MAX_PRIZE; i++)
    {
        OBJID idItem = m_pData->GetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i));
        if (idItem != ID_NONE)
        {
            CItem* pItem = CItem::CreateNew();
            if (pItem && pItem->Create(idItem, pDb))
            {
                m_setPrizeItem[i] = pItem;
            }
            else
            {
                ::LogSave("Mercenary task data load item failed! [idItem=%u]", idItem);
                return false;
            }
        }
        else
        {
            m_setPrizeItem[i] = NULL;
        }
    }
    return true;
}

CItem* CMercenaryTaskData::GetItem(OBJID idItem)
{
    if (idItem == ID_NONE)
    {
        return NULL;
    }
    ITEM_SET::iterator it = m_setPrizeItem.begin();
    for (; it != m_setPrizeItem.end(); it++)
    {
        CItem* pItem = *it;
        if (pItem && pItem->GetID() == idItem)
        {
            return pItem;
        }
    }
    return NULL;
}

CItem* CMercenaryTaskData::PopItem(OBJID idItem)
{
    if (idItem == ID_NONE)
    {
        return NULL;
    }
    ITEM_SET::iterator it = m_setPrizeItem.begin();
    for (; it != m_setPrizeItem.end(); it++)
    {
        CItem* pItem = *it;
        if (pItem && pItem->GetID() == idItem)
        {
            *it = NULL;
            return pItem;
        }
    }
    return NULL;
}


CItem* CMercenaryTaskData::GetItemByIndex(int nIndex)
{
    CHECKF (nIndex >= 0 && nIndex < _MAX_PRIZE);
    return m_setPrizeItem[nIndex];
}

bool CMercenaryTaskData::AddResultItem(CItem* pItem, bool bUpdate)
{
    CHECKF (pItem);
    CHECKF (!m_pResultItem);
    // 任务物品有效性检查
    if (strcmp(GetStr(MTASKDATA_TARGET_NAME), pItem->GetStr(ITEMDATA_NAME)) != 0)
    {
        // 物品不满足条件
        return false;
    }
    if (this->GetInt(MTASKDATA_TYPE) == _WANT_EUDEMON)
    {
        if (pItem->GetEudemonLevel() < GetInt(MTASKDATA_DEGREE)
                || pItem->GetInt(ITEMDATA_GROWTH) < GetInt(MTASKDATA_ADDITION))
        {
            return false;
        }
    }
    else if (this->GetInt(MTASKDATA_TYPE) == _WANT_ITEM)
    {
        if (pItem->GetInt(ITEMDATA_LEVEL) < GetInt(MTASKDATA_DEGREE)
                || pItem->GetInt(ITEMDATA_ADDITION) < GetInt(MTASKDATA_ADDITION)
                || pItem->GetInt(ITEMDATA_AMOUNT) < GetInt(MTASKDATA_DURA))
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    m_pResultItem	= pItem;
    pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_PLAYERTASK, true);
    this->SetInt(MTASKDATA_RESULT_ITEMID, pItem->GetID(), bUpdate);
    return true;
}
