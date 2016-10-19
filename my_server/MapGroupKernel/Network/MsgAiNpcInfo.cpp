
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAiNpcInfo.cpp: implementation of the CMsgMonsterInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "MsgAiNpcInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMonsterInfo::CMsgMonsterInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgMonsterInfo::~CMsgMonsterInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMonsterInfo::Create(const ST_CREATENEWNPC* pInfo)
{
    CHECKF(pInfo->usAction == MSGAINPCINFO_CREATENEW);
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_AINPCINFO;
    m_pInfo->id			= pInfo->id;
    m_pInfo->usAction	= pInfo->usAction;
    m_pInfo->usType		= pInfo->usType;
    m_pInfo->idMap		= pInfo->idMap;
    m_pInfo->usPosX		= pInfo->usPosX;
    m_pInfo->usPosY		= pInfo->usPosY;
    m_pInfo->idGen		= pInfo->idData;
    m_pInfo->idOwner	= pInfo->idOwner;
    m_pInfo->ucOwnerType = pInfo->ucOwnerType;
    m_pInfo->nData		= pInfo->nData;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMonsterInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMonsterInfo::Process(void* pSocket)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgMonsterInfo: ID:0x:%x, Type:%d, LookType:%d, CellX:%d, CellY:%d, Name:%s",
              m_pInfo->id	, m_pInfo->usType,
              m_pInfo->usLook, m_pInfo->usCellX,
              m_pInfo->usCellY, m_pInfo->szName);
#endif
    ASSERT(!"CMsgMonsterInfo::Process()");		// 由WorldKernel处理NPC增加消息
}








