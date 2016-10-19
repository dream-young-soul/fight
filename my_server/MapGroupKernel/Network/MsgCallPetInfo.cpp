
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgCallPetInfo.cpp: implementation of the CMsgCallPetInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Npc.h"
#include "NpcManager.h"
#include "GameMap.h"
#include "MapGroup.h"
#include "MsgCallPetInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgCallPetInfo::CMsgCallPetInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgCallPetInfo::~CMsgCallPetInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgCallPetInfo::Create(OBJID id, DWORD dwLookFace, int nAIType, int usPosX, int usPosY, OBJID idItem)
{
    CHECKF(id);
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_PETINFO;
    m_pInfo->id				= id;
    m_pInfo->dwLookFace		= dwLookFace;
    m_pInfo->nAIType		= nAIType;
    m_pInfo->usPosX			= usPosX;
    m_pInfo->usPosY			= usPosY;
    m_pInfo->idItem			= idItem;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgCallPetInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgCallPetInfo::Process(CGameSocket* pSocket)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgCallPetInfo: ID:0x:%x, Type:%d, LookType:%d, CellX:%d, CellY:%d, Name:%s",
              m_pInfo->id	, m_pInfo->ucType,
              m_pInfo->usLook, m_pInfo->usCellX,
              m_pInfo->usCellY, m_pInfo->szName);
#endif
    ASSERT(!"CMsgCallPetInfo::Process");
}

