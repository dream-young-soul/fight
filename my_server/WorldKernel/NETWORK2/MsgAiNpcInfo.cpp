
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAiNpcInfo.cpp: implementation of the CMsgAiNpcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#include "allmsg.h"
#include "protocol.h"
#include "MapList.h"
#include "WorldKernel.h"
using namespace world_kernel;

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
BOOL CMsgAiNpcInfo::Create(ST_CREATENEWNPC* pInfo)
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
void CMsgAiNpcInfo::Process(void* pSocket)
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
    OBJID idMap				= m_pInfo->idMap;
    PROCESS_ID idProcess	= MapList()->GetMapProcessID(idMap);
    if (idProcess == PROCESS_NONE)
    {
        return;
    }
    ST_CREATENEWNPC	buf;
    memset(&buf, 0, sizeof(ST_CREATENEWNPC));
    buf.id			= m_pInfo->id;
    buf.usAction	= m_pInfo->usAction;
    buf.usType		= m_pInfo->usType;
    buf.idMap		= m_pInfo->idMap;
    buf.usPosX		= m_pInfo->usPosX;
    buf.usPosY		= m_pInfo->usPosY;
    buf.idData		= m_pInfo->idGen;
    buf.idOwner		= m_pInfo->idOwner;
    buf.ucOwnerType	= m_pInfo->ucOwnerType;
    buf.nData		= m_pInfo->nData;
    IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
    pMsgPort->Send(idProcess, MAPGROUP_CREATENEWNPC, STRUCT_TYPE(ST_CREATENEWNPC), &buf);
}








