
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-----------------------------------------------------------------------------
// HeroTask.cpp
//-----------------------------------------------------------------------------
#include "Hero.h"
#include "AllMsg.h"
#include "3DGamemap.h"
#include "GamePlayerset.h"

//-----------------------------------------------------------------------------
void CHero::DestroyTaskDialog()
{
    m_objTaskDialog.Destroy();
}
//-----------------------------------------------------------------------------
int CHero::GetTaskDialogLineAmount()
{
    return m_objTaskDialog.GetLineAmount();
}
//-----------------------------------------------------------------------------
TaskDialogLine*	CHero::GetTaskDialogLineByIndex(int nIndex)
{
    return m_objTaskDialog.GetLineByIndex(nIndex);
}
//-----------------------------------------------------------------------------
void CHero::AddTaskDialogLine(int nType, DWORD dwData, int nIndex, char* szStr, CMyPos posLine)
{
    m_objTaskDialog.AddLine(nType, dwData, nIndex, szStr, posLine);
}
//-----------------------------------------------------------------------------
void CHero::AnswerTaskDialog(int nIndex, char* pszEdit)
{
    CMsgTaskDialog msg;
    if (msg.Create(nIndex, pszEdit))
    {
        msg.Send();
    }
}
//-----------------------------------------------------------------------------
void CHero::CreateTaskDialog()
{
    m_objTaskDialog.Create();
}
//-----------------------------------------------------------------------------
void CHero::ActiveNpc(OBJID idNpc)
{
    if (idNpc == ID_NONE)
    {
        return;
    }
    CMsgNpc msg;
    if (msg.Create(EVENT_BEACTIVED, idNpc))
    {
        msg.Send();
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idNpc);
    if (pPlayer)
    {
        if (!this->IsDead())
        {
            pPlayer->m_pIRoleView->SetVariable(_VAR_CLICK, 0);
        }
        this->SetLastDialogPara(idNpc);
        CMyPos posPlayer;
        pPlayer->GetPos(posPlayer.x, posPlayer.y);
        CMyPos posHero;
        this->GetPos(posHero.x, posHero.y);
        int nDir = g_objGameMap.GetDirection(posPlayer.x, posPlayer.y, posHero.x, posHero.y);
        int nType = pPlayer->GetRoleType();
        NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(pPlayer->GetLook() / 10);
        if (pInfo && pInfo->bChangeDir)
        {
            pPlayer->m_pIRoleView->SetVariable(_VAR_DIRECT_DIR, nDir);
        }
    }
}
//-----------------------------------------------------------------------------
void CHero::AddTaskDialogLine(TaskDialogLine* pLine)
{
    m_objTaskDialog.AddLine(pLine);
}
//-----------------------------------------------------------------------------
void CHero::ActiveAction(OBJID idAction, char* pszStr)
{
    CMsgTaskDialog msg;
    if (msg.Create(0, pszStr, idAction, MSGDIALOG_TASKID))
    {
        msg.Send();
    }
}
//-----------------------------------------------------------------------------
// Ó¶±øÈÎÎñ
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void	CHero::AddPlayerTaskList(OBJID idTask, const char* szTitle , UCHAR ucReqLev )
{
    ST_TASK_TITLE* pTaskTitle = new ST_TASK_TITLE;
    MYASSERT(pTaskTitle);
    pTaskTitle->idTask = idTask;
    pTaskTitle->ucReqLev = ucReqLev;
    strcpy(pTaskTitle->szTitle , szTitle);
    m_setPlayerTask.push_back(pTaskTitle);
}

void	CHero::ClearPlayerTaskList()
{
    int nAmount = m_setPlayerTask.size();
    for(int i = 0; i < nAmount; i++)
    {
        ST_TASK_TITLE* pTaskTitle = m_setPlayerTask[i];
        SAFE_DELETE(pTaskTitle);
    }
    m_setPlayerTask.clear();
}

DWORD	CHero::GetPlayerTaskAmout()
{
    return m_setPlayerTask.size();
}
//-----------------------------------------------------------------------------
ST_TASK_TITLE*	CHero::GetPlayerTaskIndex(int nIndex)
{
    int nAmount = m_setPlayerTask.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setPlayerTask[nIndex];
}
//-----------------------------------------------------------------------------

void	CHero::QueryPlayerTaskAcceptedList(OBJID idLast)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionQueryPlayerTaskAcceptedList, idLast))
    {
        actionMsg.Send();
    }
}

//-----------------------------------------------------------------------------
void	CHero::QueryPlayerTaskUnacceptedList(OBJID idLast)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionQueryPlayerTaskUnacceptedList, idLast))
    {
        actionMsg.Send();
    }
}

//-----------------------------------------------------------------------------
void	CHero::QueryPlayerTaskCompletedList(OBJID idLast)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionQueryPlayerTaskCompletedList, idLast))
    {
        actionMsg.Send();
    }
}

//-----------------------------------------------------------------------------
void	CHero::QueryPlayerMyTaskList(OBJID idLast)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionQueryPlayerMyTaskList, idLast))
    {
        actionMsg.Send();
    }
}
//-----------------------------------------------------------------------------
void	CHero::QueryPlayerTaskDetial(OBJID idTask)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionQueryPlayerTaskDetail, idTask))
    {
        actionMsg.Send();
    }
}
//-----------------------------------------------------------------------------
void	CHero::AcceptPlayerTask(OBJID idTask)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionAcceptPlayerTask, idTask))
    {
        actionMsg.Send();
    }
}
//-----------------------------------------------------------------------------
void	CHero::CompletePlayerTask(OBJID idTask)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionCompletePlayerTask, idTask))
    {
        actionMsg.Send();
    }
}
//-----------------------------------------------------------------------------
void	CHero::CancelPlayerTask(OBJID idTask)
{
    CMsgAction actionMsg;
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    if (actionMsg.Create(this->GetID(), posHero.x, posHero.y, this->GetDir(), actionCancelPlayerTask, idTask))
    {
        actionMsg.Send();
    }
}
//-----------------------------------------------------------------------------
void	CHero::WantPk(const char* pszName, const char* pszTitle, const char* pszDesc,
                      DWORD dwMoney, UCHAR ucRankReq, OBJID idItem0,
                      OBJID idItem1, OBJID idItem2, OBJID idItem3,
                      OBJID idItem4)
{
    CMsgPlayerTask taskMsg;
    taskMsg.WantPk(pszName, pszTitle, pszDesc, dwMoney, ucRankReq, idItem0, idItem1, idItem2, idItem3, idItem4);
}
//-----------------------------------------------------------------------------
void	CHero::WantItem(const char* pszName,  const char* pszTitle, const char* pszDesc, UCHAR ucType,
                        DWORD dwDurability, UCHAR ucAddition, UCHAR ucDegree,
                        WORD dwMoney, UCHAR ucRankReq, OBJID idItem0, OBJID idItem1, OBJID idItem2, OBJID idItem3,
                        OBJID idItem4)
{
    CMsgPlayerTask taskMsg;
    taskMsg.WantItem(pszName, pszTitle, pszDesc, ucType, dwDurability, ucAddition, ucDegree,
                     dwMoney, ucRankReq, idItem0, idItem1, idItem2, idItem3, idItem4);
}
//-----------------------------------------------------------------------------
