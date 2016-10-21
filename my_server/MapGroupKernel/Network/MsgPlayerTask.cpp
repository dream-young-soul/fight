
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgPlayerTask.cpp: implementation of the CMsgPlayerTask class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MercenaryTask.h"
#include "MapGroup.h"
#include "UserManager.h"
#include "User.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgPlayerTask::CMsgPlayerTask()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgPlayerTask::~CMsgPlayerTask()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayerTask::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_PLAYERTASK != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayerTask::Create(CMercenaryTaskData* pData)
{
    CHECKF (pData);
    this->Init();
    m_pInfo->dwMoney		= pData->GetInt(MTASKDATA_PRIZE_MONEY);
    m_pInfo->idTask			= pData->GetID();
    m_pInfo->dwDurability	= pData->GetInt(MTASKDATA_DURA);
    m_pInfo->ucAddition		= pData->GetInt(MTASKDATA_ADDITION);
    m_pInfo->ucDegree		= pData->GetInt(MTASKDATA_DEGREE);
    m_pInfo->ucType			= pData->GetInt(MTASKDATA_TYPE);
    int nIdx = 0;
    for (int i = MTASKDATA_PRIZE_ITEMID0; i <= MTASKDATA_PRIZE_ITEMID4; i++)
    {
        OBJID idItem = pData->GetInt((MTASKDATA)i);
        CItem* pItem = pData->GetItem(idItem);
        if (pItem)
        {
            CHECKF (nIdx < _MAX_PRIZE);
            m_pInfo->infoItem[nIdx].idItemType		= pItem->GetInt(ITEMDATA_TYPE);
            m_pInfo->infoItem[nIdx].usDurability	= pItem->GetInt(ITEMDATA_AMOUNT);
            if (pItem->IsEudemon() || pItem->IsEudemonEgg())
            {
                m_pInfo->infoItem[nIdx].ucAddition		= pItem->GetInt(ITEMDATA_GROWTH);
                m_pInfo->infoItem[nIdx].ucDegree		= pItem->GetEudemonLevel();
            }
            else
            {
                m_pInfo->infoItem[nIdx].ucAddition		= pItem->GetInt(ITEMDATA_ADDITION);
                m_pInfo->infoItem[nIdx].ucDegree		= pItem->GetInt(ITEMDATA_LEVEL);
            }
            ++nIdx;
        }
    }
    m_StrPacker.AddString(pData->GetStr(MTASKDATA_TITLE));
    m_StrPacker.AddString(pData->GetStr(MTASKDATA_DETAIL));
    m_StrPacker.AddString(pData->GetStr(MTASKDATA_TARGET_NAME));
    m_unMsgType	= _MSG_PLAYERTASK;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgPlayerTask::Process(void* pInfo)
{
    CUser* pUser = UserManager()->GetUser(this);
    if (!pUser)
    {
        return;
    }
    if (!pUser->IsAlive())
    {
        pUser->SendSysMsg(STR_DIE);
        return ;
    }
    if (MercenaryTask()->QueryTaskDataByOwner(pUser->GetID()))
    {
        pUser->SendSysMsg(STR_PUBLISH_ONE_TASK_ONLY);
        return;
    }
    if (m_StrPacker.GetStrAmount() < 3)
    {
        return;
    }
    // 奖金检查
    if (pUser->GetMoney() < m_pInfo->dwMoney)
    {
        pUser->SendSysMsg(STR_NOT_SO_MUCH_MONEY);
        return;
    }

    for (int i = 0; i < _MAX_PRIZE; i++)
    {
        if (m_pInfo->infoItem[i].idItem != ID_NONE)
        {
            CItem* pItem = pUser->GetItem(m_pInfo->infoItem[i].idItem);
            if (!pItem)
            {
                pUser->SendSysMsg(STR_ITEM_INEXIST);
                return;
            }
            if (!pItem->IsExchangeEnable())
            {
                pUser->SendSysMsg(STR_NOT_FOR_TASK_PRIZE);
                return;
            }
        }
    }
    // 先取出奖品，防止作弊
    for (int i = 0; i < _MAX_PRIZE; i++)
    {
        if (m_pInfo->infoItem[i].idItem != ID_NONE)
        {
            CItem* pItem = pUser->GetItem(m_pInfo->infoItem[i].idItem);
            if (!pItem)
            {
                pUser->SendSysMsg(STR_ERROR);
                // 出错处理...
                return;
            }
            if (pItem->IsEudemon())
            {
                pUser->CallBackEudemon(pItem->GetID());
                pUser->DetachEudemon(pItem);
            }
            // 简单一点，仅仅改变position，不改变物品的owner_id
            pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_PLAYERTASK, true);
            pUser->DelItem(m_pInfo->infoItem[i].idItem, true);
        }
    }
    if (!pUser->SpendMoney(m_pInfo->dwMoney, true))
    {
        pUser->SendSysMsg(STR_ERROR);
        return;
    }
    // fill task data
    ST_MTASK_DATA	tTaskData;
    memset(&tTaskData, 0L, sizeof(ST_MTASK_DATA));
    tTaskData.idUser	= pUser->GetID();
    ::SafeCopy(tTaskData.szUserName, pUser->GetName(), _MAX_NAMESIZE);
    m_StrPacker.GetString(0, tTaskData.szTitle, 32);
    m_StrPacker.GetString(1, tTaskData.szDetail, 128);
    m_StrPacker.GetString(2, tTaskData.szTargetName, _MAX_NAMESIZE);
    tTaskData.ucType		= m_pInfo->ucType;
    tTaskData.ucAddition	= m_pInfo->ucAddition;
    tTaskData.ucDegree		= m_pInfo->ucDegree;
    tTaskData.dwDura		= m_pInfo->dwDurability;
    tTaskData.ucRankReq		= m_pInfo->ucRankReq;
    tTaskData.dwPrizeMoney	= m_pInfo->dwMoney;
    for (int i = 0; i < _MAX_PRIZE; i++)
    {
        tTaskData.idPrizeItems[i]	= m_pInfo->infoItem[i].idItem;
    }
    // create new mercenary task
    if (!MercenaryTask()->CreateNewTask(&tTaskData))
    {
        pUser->SendSysMsg(STR_ERROR);
        return;
    }
    pUser->SendSysMsg(STR_MERCENARY_TASK_OK);
}
