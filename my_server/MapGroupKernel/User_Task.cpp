
//**********************************************************
// 代码编辑器
//**********************************************************

// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "User.h"
#include "MapGroup.h"
#include "netmsg.h"
#include "ItemType.h"
#include "MapItem.h"
#include "Package.h"
#include "ActionDefine.h"
#include "Syndicate.h"
#include "npc.h"
#include "WantedList.h"
#include "MercenaryTask.h"

const int VAL_SKIPTEST = 999;
const int PRESENTMONEY_LIMIT = 10000;			// 基金划拨不少于10000

//////////////////////////////////////////////////////////////////////
// task
////////////////////////////////////////////////////////////////////////////////////
bool CUser::TestTask(CTaskData* pTask)
{
    CHECKF(pTask);
    try
    {
        if (!CheckItem(pTask))
        {
            return false;    // 没有任务物品
        }
        if (this->GetMoney() < pTask->GetInt(TASKDATA_MONEY))
        {
            return false;
        }
        if (pTask->GetInt(TASKDATA_PROFESSION) && this->GetProfession() != pTask->GetInt(TASKDATA_PROFESSION))
        {
            return false;
        }
        //		if (pTask->GetInt(TASKDATA_SEX) && ((1 << this->GetSex()) & pTask->GetInt(TASKDATA_SEX)) != 0)
        //			return false;
        if (this->GetPk() < pTask->GetInt(TASKDATA_MINPK) || this->GetPk() > pTask->GetInt(TASKDATA_MAXPK))
        {
            return false;
        }
        if (pTask->GetInt(TASKDATA_TEAM) != VAL_SKIPTEST)
        {
            /*			switch(pTask->GetInt(TASKDATA_TEAM))
            			{
            			case 0:
            			case 1:
            				{
            					int nMembers	=this->GetTeamMembers();
            					if (nMembers != 0 && nMembers != 1)
            						return false;
            				}
            				break;

            			case 2:
            			case 3:
            			case 4:
            			case 5:
            				{
            					if (this->GetTeamMembers() != m_Info.nTeam)
            						return false;
            				}
            				break;

            			case 6:		// no team members check
            				break;

            			case 7:		// couple
            				{
            					CTeam* pTeam	=this->GetTeam();
            					if (!pTeam || !pTeam->IsCoupleTeam())
            						return false;
            				}
            				break;

            			case 8:		// brother
            				{
            					CTeam* pTeam	=this->GetTeam();
            					if (!pTeam || !pTeam->IsBrotherTeam())
            						return false;
            				}
            				break;

            			default:
            				return false;
            */
        }
        if (pTask->GetInt(TASKDATA_SEX) != VAL_SKIPTEST)
        {
            if (this->GetSex() != pTask->GetInt(TASKDATA_SEX))
            {
                return false;
            }
        }
        /*
        		if (pTask->GetInt(TASKDATA_METEMPSYCHOSIS) && this->GetMetempsychosis() < pTask->GetInt(TASKDATA_METEMPSYCHOSIS))
        		{
        			if (g_cSyndicate.GetLevel(this->GetSyndicateID()) != m_Info.nSyndicate)
        				return false;
        		}
        */
        int nMarriageChk = pTask->GetInt(TASKDATA_MARRIAGE);
        if (-1 != nMarriageChk)
        {
            if (0 == nMarriageChk && this->IsMarried())
            {
                return false;
            }
            if (1 == nMarriageChk && !this->IsMarried())
            {
                return false;
            }
        }
        return true;
    }
    catch(...)
    {
        ASSERT(!"catch");
        ::LogSave("exception catched In CGameTask::TestUser.");
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////
bool CUser::CheckItem(CTaskData* pTask)
{
    if (strlen(pTask->GetStr(TASKDATA_ITEMNAME1)) > 0)
    {
        if (!this->GetTaskItem(pTask->GetStr(TASKDATA_ITEMNAME1)))
        {
            return false;
        }
        if (strlen(pTask->GetStr(TASKDATA_ITEMNAME2)) > 0)
        {
            if (!this->GetTaskItem(pTask->GetStr(TASKDATA_ITEMNAME2)))
            {
                return false;
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::ProcessClientTask(OBJID idTask, LPCTSTR pszAccept)
{
    CTaskData* pTask = TaskSet()->GetObj(idTask);
    CHECKF(pTask && pTask->GetInt(TASKDATA_CLIENTACT));
    CNpc* pNpc = NULL;
    GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, m_idTaskNpc, IPP_OF(pNpc));	// pNpc may be null
    if (!this->TestTask(pTask))
    {
        OBJID idNext	= pTask->GetInt(TASKDATA_IDNEXTFAIL);
        if (idNext != ID_NONE)
        {
            GameAction()->ProcessAction(idNext, this, pNpc->QueryRole(), NULL, pszAccept);
            return true;
        }
        this->SendSysMsg(STR_UNQUALIFIED);
        return false;
    }
    OBJID idNext	= pTask->GetInt(TASKDATA_IDNEXT);
    if (idNext != ID_NONE)
    {
        GameAction()->ProcessAction(idNext, this, pNpc->QueryRole(), NULL, pszAccept);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
int CUser::PushTaskID(OBJID idTask, OBJID idIter)
{
    if (idTask != ID_NONE && m_setTaskID.size() < MAX_MENUTASKSIZE)
    {
        m_setTaskID.push_back(idTask);
        m_setIterID.push_back(idIter);
        return m_setTaskID.size() - 1;
    }
    return INDEX_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CUser::ProcessTask(int idx, LPCTSTR pszAccept)
{
    // check npc
    if (m_idTaskNpc == ID_NONE)
    {
        return false;
    }
    CNpc* pNpc = NULL;
    if (!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, m_idTaskNpc, IPP_OF(pNpc)))
    {
        //		LOGERROR("任务系统回馈触发的NPC[%u]找不到!", m_idTaskNpc);
        return false;
    }
    CHECKF(pNpc);
    OBJID	idTask = GetTaskID(idx);
    if (idTask == ID_NONE)
    {
        return false;
    }
    SetIterID(idx);
    ClearTaskID();
    CTaskData* pTask = TaskSet()->GetObj(idTask);
    IF_NOT(pTask)
    {
        LOGERROR("Don't find ACTION ID in menu: %d", idTask);
        return false;
    }
    if (!this->TestTask(pTask))
    {
        OBJID idNext	= pTask->GetInt(TASKDATA_IDNEXTFAIL);
        if (idNext != ID_NONE)
        {
            GameAction()->ProcessAction(idNext, this, pNpc->QueryRole(), NULL, pszAccept);
            return true;
        }
        this->SendSysMsg(STR_UNQUALIFIED);
        return false;
    }
    OBJID idNext	= pTask->GetInt(TASKDATA_IDNEXT);
    if (idNext != ID_NONE)
    {
        GameAction()->ProcessAction(idNext, this, pNpc->QueryRole(), NULL, pszAccept);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::AddTaskMask		(int idx)
{
    IF_NOT (idx >= 0 && idx < 32)
    return;
    DWORD dwMask = m_data.GetTaskMask() | (1 << idx);
    m_data.SetTaskMask(dwMask);
}

//////////////////////////////////////////////////////////////////////
void CUser::ClrTaskMask		(int idx)
{
    IF_NOT (idx >= 0 && idx < 32)
    return;
    DWORD dwMask = m_data.GetTaskMask();
    dwMask	&= ~(1 << idx);
    m_data.SetTaskMask(dwMask);
}

//////////////////////////////////////////////////////////////////////
bool CUser::ChkTaskMask		(int idx)
{
    IF_NOT (idx >= 0 && idx < 32)
    return false;
    return (m_data.GetTaskMask() & (1 << idx)) != 0;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CompleteMercenaryTask(OBJID idTask, OBJID idItem)
{
    // 提交完成的佣兵任务
    // 检查任务合法性
    CMercenaryTaskData* pTaskData = MercenaryTask()->QueryTaskDataByID(idTask);
    if (!pTaskData)
    {
        SendSysMsg(STR_INVALID_MERCENARYTASK);
        return false;
    }
    if (pTaskData->GetInt(MTASKDATA_USER_ID) == GetID())
    {
        // 关闭自己的任务
        if (pTaskData->GetInt(MTASKDATA_STATE) != MTASKSTATE_COMPLETED)
        {
            // 任务还没结束呢
            SendSysMsg(STR_TASK_NOT_FINISHED_YET);
            return false;
        }
        // 取得任务物品
        if (pTaskData->GetInt(MTASKDATA_TYPE) == _WANT_ITEM
                || pTaskData->GetInt(MTASKDATA_TYPE) == _WANT_EUDEMON)
        {
            {
                // 先检查背包空间
                CItem* pItem = pTaskData->GetResultItem();
                if (pItem && QueryPackage()->IsPackFull(1, &pItem))
                {
                    SendSysMsg(STR_BAG_FULL);
                    return false;
                }
            }
            CItem* pItem = pTaskData->PopResultItem();
            ASSERT(pItem);
            AddItem(pItem, true, true);
        }
        else if (pTaskData->GetInt(MTASKDATA_TYPE) == _WANT_PK)
        {
            // 杀人任务，不需要取得任何物品
        }
        else
        {
            ::LogSave("Invalid mercenary task type [ID=%u, type=%u]",
                      pTaskData->GetID(), pTaskData->GetInt(MTASKDATA_TYPE));
            return false;
        }
        // 关闭任务 —— 直接删除任务
        MercenaryTask()->DeleteTask(idTask);
        // return a message
        SendSysMsg(STR_CLOSE_TASK_OK);
    }
    else
    {
        // 完成自己接的任务
        if (pTaskData->GetInt(MTASKDATA_ACCEPT_USERID) != GetID()
                || (pTaskData->GetInt(MTASKDATA_STATE) != MTASKSTATE_ACCEPTED
                    && pTaskData->GetInt(MTASKDATA_STATE) != MTASKSTATE_FINISHED))
        {
            // 不是自己接的任务，或者不是处于已接未完成状态，非法
            SendSysMsg(STR_INVALID_MERCENARYTASK);
            return false;
        }
        // 任务有两种情况：悬赏杀人或者悬赏物品
        if (pTaskData->GetInt(MTASKDATA_TYPE) == _WANT_ITEM
                || pTaskData->GetInt(MTASKDATA_TYPE) == _WANT_EUDEMON)
        {
            // 悬赏物品
            // 检查idItem给顶的物品是否满足条件
            // coding here...
            CItem* pResultItem = GetItem(idItem);	// 先不同步到客户端
            if (pResultItem && pResultItem->IsEudemon())
            {
                CallBackEudemon(pResultItem->GetID());
                DetachEudemon(pResultItem);
            }
            IF_NOT (pResultItem = PopItem(idItem, false, true))
            return false;
            if (pTaskData->AddResultItem(pResultItem))
            {
                CMsgItem msg;
                if (msg.Create(idItem, ITEMACT_DROP))	// 成功后才同步
                {
                    this->SendMsg(&msg);
                }
                // set task finished.
                pTaskData->SetInt(MTASKDATA_STATE, MTASKSTATE_FINISHED, true);		// 为了保证不要出错，这里立刻写库
            }
            else
            {
                // 失败，重新返回给玩家
                AddItem(pResultItem, false);		// 因为前面没有同步通知客户端删除，这里也就没必要通知客户端添加
                SendSysMsg(STR_ILLEGAL_TASK_ITEM);
                return false;
            }
        }
        else if (pTaskData->GetInt(MTASKDATA_TYPE) == _WANT_PK)
        {
            // 悬赏杀人，在杀死目标的时候就自动将任务设置为Finished状态了，这里就不需要做任何事情了
        }
        else
        {
            ::LogSave("Invalid mercenary task type [ID=%u, type=%u]",
                      pTaskData->GetID(), pTaskData->GetInt(MTASKDATA_TYPE));
            return false;
        }
        // 经过上面一步，如果任务成功执行，则已经将任务设置为finished状态。
        if (pTaskData->GetInt(MTASKDATA_STATE) == MTASKSTATE_FINISHED)
        {
            // 任务已经完成，领取奖品
            // 先检查背包空间
            {
                CItem* setItems[_MAX_PRIZE];
                int nAmount = 0;
                for (int i = 0; i < _MAX_PRIZE; i++)
                {
                    OBJID idItem = pTaskData->GetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i));
                    if (idItem != ID_NONE)
                    {
                        CItem* pItem = pTaskData->GetItem(idItem);
                        IF_NOT (pItem)
                        return false;
                        setItems[nAmount]	= pItem;
                        ++nAmount;
                    }
                }
                if (nAmount > 0 && QueryPackage()->IsPackFull(nAmount, setItems))
                {
                    SendSysMsg(STR_BAG_FULL);
                    return false;
                }
            }
            // 先结束任务
            pTaskData->SetInt(MTASKDATA_STATE, MTASKSTATE_COMPLETED);
            // 后领奖
            // 奖品
            for (int i = 0; i < _MAX_PRIZE; i++)
            {
                OBJID idItem = pTaskData->GetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i));
                if (idItem != ID_NONE)
                {
                    CItem* pItem = pTaskData->PopItem(idItem);
                    if (pItem)
                    {
                        AddItem(pItem, true, true);
                    }
                    pTaskData->SetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i), ID_NONE);
                }
            }
            // 奖金
            GainMoney(pTaskData->GetInt(MTASKDATA_PRIZE_MONEY), true);
            pTaskData->SetInt(MTASKDATA_PRIZE_MONEY, 0, true);
            // send a message to client
            SendSysMsg(STR_TAKE_PRIZE_OK);
        }
        else
        {
            // 任务还没完成呢
            // send task unfinished msg to client
            SendSysMsg(STR_TASK_NOT_FINISHED_YET);
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AcceptMercenaryTask(OBJID idTask)
{
    // 检查是否已经接过任务
    {
        CMercenaryTaskData* pTaskData = MercenaryTask()->QueryTaskDataAccepted(GetID());
        if (pTaskData)
        {
            SendSysMsg(STR_DUPLICATE_TASK);
            return false;
        }
    }
    // 检查被接任务的有效性
    CMercenaryTaskData* pTaskData = MercenaryTask()->QueryTaskDataByID(idTask);
    if (!pTaskData)
    {
        SendSysMsg(STR_INVALID_MERCENARYTASK);
        return false;
    }
    if (pTaskData->GetInt(MTASKDATA_USER_ID) == this->GetID())
    {
        SendSysMsg(STR_CANNOT_ACCEPT_MYTASK);
        return false;
    }
    if (pTaskData->GetInt(MTASKDATA_STATE) != MTASKSTATE_NONE)
    {
        SendSysMsg(STR_INVALID_MERCENARYTASK);
        return false;
    }
    if (GetMercenaryRank() < pTaskData->GetInt(MTASKDATA_RANK_REQ))
    {
        SendSysMsg(STR_LOW_MERCENARY_RANK);
        return false;
    }
    // 扣押金
    int nMoney = pTaskData->GetInt(MTASKDATA_PRIZE_MONEY);
    for (int i = 0; i < _MAX_PRIZE; i++)
    {
        CItem* pItem = pTaskData->GetItemByIndex(i);
        if (pItem)
        {
            nMoney += pItem->GetInt(ITEMDATA_PRICE);
        }
    }
    nMoney = __max(500, nMoney / 100);	// 接任务只收任务奖品价格+奖金的1%，但是最低为500
    if (!SpendMoney(nMoney, true))
    {
        SendSysMsg(STR_NOT_ENOUGH_MONEY, nMoney);
        return false;
    }
    // 接任务
    pTaskData->SetInt(MTASKDATA_STATE, MTASKSTATE_ACCEPTED);
    pTaskData->SetInt(MTASKDATA_ACCEPT_TIME, ::TimeGet(TIME_SECOND));	// 记录时间
    pTaskData->SetInt(MTASKDATA_ACCEPT_USERID, GetID(), true);
    // retuen a message
    SendSysMsg(STR_ACCEPT_TASK_OK);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CancelMercenaryTask(OBJID idTask)
{
    // 检查任务合法性
    CMercenaryTaskData* pTaskData = MercenaryTask()->QueryTaskDataByID(idTask);
    if (!pTaskData)
    {
        SendSysMsg(STR_INVALID_MERCENARYTASK);
        return false;
    }
    if (pTaskData->GetInt(MTASKDATA_USER_ID) == GetID())
    {
        // 中止自己的任务
        if (pTaskData->GetInt(MTASKDATA_STATE) != MTASKSTATE_NONE)
        {
            // 任务不是处于未接状态，不能中止
            SendSysMsg(STR_CANNOT_CANCEL_TASK);
            return false;
        }
        // 先检查背包空间
        {
            CItem* setItems[_MAX_PRIZE];
            int nAmount = 0;
            for (int i = 0; i < _MAX_PRIZE; i++)
            {
                OBJID idItem = pTaskData->GetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i));
                if (idItem != ID_NONE)
                {
                    CItem* pItem = pTaskData->GetItem(idItem);
                    IF_NOT (pItem)
                    return false;
                    setItems[nAmount]	= pItem;
                    ++nAmount;
                }
            }
            if (nAmount > 0 && QueryPackage()->IsPackFull(nAmount, setItems))
            {
                SendSysMsg(STR_BAG_FULL);
                return false;
            }
        }
        // 返还奖品和奖金
        // 奖品
        DEBUG_TRY
        for (int i = 0; i < _MAX_PRIZE; i++)
        {
            OBJID idItem = pTaskData->GetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i));
            if (idItem != ID_NONE)
            {
                CItem* pItem = pTaskData->PopItem(idItem);
                if (pItem)
                {
                    AddItem(pItem, true, true);
                }
                pTaskData->SetInt((MTASKDATA)(MTASKDATA_PRIZE_ITEMID0 + i), ID_NONE);
            }
        }
        DEBUG_CATCH("CUser::CancelMercenaryTask crash when return prize items")
        // 奖金
        GainMoney(pTaskData->GetInt(MTASKDATA_PRIZE_MONEY), true);
        //pTaskData->SetInt(MTASKDATA_STATE, MTASKSTATE_CANCELED, true);
        // 中止任务只要直接删除任务就好了
        MercenaryTask()->DeleteTask(idTask);
    }
    else
    {
        // 中止自己接的任务
        if (pTaskData->GetInt(MTASKDATA_ACCEPT_USERID) != GetID()
                || pTaskData->GetInt(MTASKDATA_STATE) != MTASKSTATE_ACCEPTED)
        {
            // 不是自己接的任务，或者不是处于已接未完成状态，非法
            SendSysMsg(STR_INVALID_MERCENARYTASK);
            return false;
        }
        // 撤消正在执行的佣兵任务
        pTaskData->SetInt(MTASKDATA_ACCEPT_USERID, ID_NONE);
        pTaskData->SetInt(MTASKDATA_STATE, MTASKSTATE_NONE, true);
    }
    // return a message
    SendSysMsg(STR_CANCEL_TASK_OK);
    return true;
}
