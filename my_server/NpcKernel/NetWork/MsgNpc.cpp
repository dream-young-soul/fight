
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgNpc.cpp: implementation of the CMsgNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Npc.h"
#include "NpcWorld.h"
#include "basefunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNpc::CMsgNpc()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgNpc::~CMsgNpc()
{
}

//////////////////////////////////////////////////////////////////////

BOOL CMsgNpc::Create(OBJID id, int nAction, int nType /*= 0*/, DWORD dwData /*= 0*/)
{
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_NPC;
    m_pInfo->id			= id;
    m_pInfo->usAction	= (unsigned short)nAction;
    m_pInfo->usType		= nType;
    m_pInfo->dwData		= dwData;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNpc::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgNpc::Process(CGameSocket* pSocket)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgNpc, id:%u, usAction:%u",
              m_pInfo->id,
              m_pInfo->usAction);
#endif
    if (!pSocket)
    {
        return;
    }
    INpc* pNpc	= NpcManager()->QuerySet()->GetObj(m_pInfo->id);
    if (!pNpc)
    {
        return ;
    }
    switch(m_pInfo->usAction)
    {
    case MSGNPC_DELNPC:
        {
        }
        break;
    default:
        ::LogSave("ERROR: CMsgNpc::Process() 内部异常");
    }
}






