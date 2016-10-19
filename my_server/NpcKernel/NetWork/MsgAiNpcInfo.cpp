
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAiNpcInfo.cpp: implementation of the CMsgAiNpcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Npc.h"
#include "NpcWorld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAiNpcInfo::CMsgAiNpcInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgAiNpcInfo::~CMsgAiNpcInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAiNpcInfo::Create(int nAction, OBJID idNpc, OBJID idGen, int nType, OBJID idMap, int nPosX, int nPosY)
{
    CHECKF(nAction == MSGAINPCINFO_CREATENEW);
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_AINPCINFO;
    m_pInfo->id			= idNpc;
    m_pInfo->usAction	= nAction;
    m_pInfo->usType		= nType;
    m_pInfo->idMap		= idMap;
    m_pInfo->usPosX		= nPosX;
    m_pInfo->usPosY		= nPosY;
    m_pInfo->idGen		= idGen;
    m_pInfo->idOwner	= ID_NONE;
    m_pInfo->ucOwnerType = OWNER_NONE;
    m_pInfo->nData		= 0;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAiNpcInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgAiNpcInfo::Process(CGameSocket* pSocket)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgAiNpcInfo: ID:0x:%x, Type:%d, LookType:%d, CellX:%d, CellY:%d, Name:%s",
              m_pInfo->id	, m_pInfo->usType,
              m_pInfo->usLook, m_pInfo->usCellX,
              m_pInfo->usCellY, m_pInfo->szName);
#endif
    if (!pSocket)
    {
        return;
    }
    if (NpcManager()->QuerySet()->GetObj(m_pInfo->id))
    {
        return;		//@ 已经有此NPC，如何处理？
    }
    CNpc*	pNpc = CNpc::CreateNew();
    if (pNpc)
    {
        if (pNpc->Create(m_pInfo->id, m_pInfo->usType, m_pInfo->idMap, m_pInfo->usPosX, m_pInfo->usPosY, m_pInfo->idGen,
                        m_pInfo->idOwner, m_pInfo->ucOwnerType, m_pInfo->nData))
        {
            NpcManager()->QuerySet()->AddObj(Cast<INpc>(pNpc));
            return ;
        }
        pNpc->ReleaseByOwner();
    }
}








