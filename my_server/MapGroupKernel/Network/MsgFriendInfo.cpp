
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgFriendInfo.cpp: implementation of the CMsgFriendInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Npc.h"
#include "NpcManager.h"
#include "GameMap.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgFriendInfo::CMsgFriendInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgFriendInfo::~CMsgFriendInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFriendInfo::Create(int nAction, OBJID idFriend, int nLook, int nLevel, int nProfession, int nPk, OBJID idSyn, int nRank, LPCTSTR szMate, UCHAR ucNobilityRank)
{
    if (!szMate)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_FRIENDINFO;
    m_pInfo->idFriend		= idFriend;
    m_pInfo->dwLook			= nLook;
    m_pInfo->ucLevel		= nLevel;
    m_pInfo->ucProfession	= nProfession;
    m_pInfo->sPk			= nPk;
    m_pInfo->dwSynID_Rank	= (idSyn & MASK_SYNID) | (nRank << MASK_RANK_SHIFT);
    SafeCopy(m_pInfo->szMate, szMate, _MAX_NAMESIZE);
    m_pInfo->ucNobilityRank	= ucNobilityRank;
    m_pInfo->ucAction		= nAction;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFriendInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgFriendInfo::Process(CGameSocket* pSocket)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgFriendInfo: ID:0x:%x, Type:%d, LookType:%d, CellX:%d, CellY:%d, Name:%s",
              m_pInfo->id	, m_pInfo->ucType,
              m_pInfo->usLook, m_pInfo->usCellX,
              m_pInfo->usCellY, m_pInfo->szName);
#endif
    ASSERT(!"CMsgFriendInfo::Process");
}

