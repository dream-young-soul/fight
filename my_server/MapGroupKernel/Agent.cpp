
//**********************************************************
// 代码编辑器
//**********************************************************

// Agent.cpp: implementation of the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "common.h"
#include "Agent.h"
#include "Mapgroup.h"

MYHEAP_IMPLEMENTATION(CAgent, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAgent::CAgent()
{
}

//////////////////////////////////////////////////////////////////////
CAgent::~CAgent()
{
}

//////////////////////////////////////////////////////////////////////
bool CAgent::Create(PROCESS_ID idProcess, OBJID idAgent)
{
    return CUser::Create(idProcess, SOCKET_NONE, idAgent);
}

//////////////////////////////////////////////////////////////////////
bool CAgent::SendMsg(CNetMsg* pMsg)
{
    return MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(GetID(), pMsg);
}

//////////////////////////////////////////////////////////////////////
bool CAgent::FullItem(int nItemType, DWORD dwData)
{
    // 这里需要针对新的背包系统做修改!!!
    for(int i = 0; i < _MAX_USERITEMSIZE; i++)
    {
        if (this->IsItemFull(CItem::GetWeight(nItemType), nItemType, dwData))
        {
            return true;
        }
        else
        {
            this->AwardItem(nItemType, SYNCHRO_TRUE, CUser::IDENT_OK);
        }
    }
    return true;
}
