
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTeam.cpp: implementation of the CMsgTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "npcWorld.h"
#include "Agent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTeam::CMsgTeam()
{
    this->Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTeam::~CMsgTeam()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeam::Create(OBJID idPlayer, int nAction)
{
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_TEAM;
    m_pInfo->idPlayer	= idPlayer;
    m_pInfo->unAction	= nAction;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeam::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTeam::Process(void* pInfo)
{
    CAgent* pAgent	= NpcManager()->QueryAgent(GetNpcID());
    if (!pAgent)
    {
        return;
    }
    CTeam* pTeam = Cast<CTeam>(pAgent);
    switch(m_pInfo->unAction)
    {
    case _MSGTEAM_APPLYJOIN:
    case _MSGTEAM_INVITE:
        {
            pTeam->Apply(m_pInfo->idPlayer);
        }
        break;
    case _MSGTEAM_LEAVE:
    case _MSGTEAM_KICKOUT:
        {
            if (m_pInfo->idPlayer == pAgent->GetID())
            {
                pTeam->Dismiss(m_pInfo->idPlayer);
            }
            else
            {
                pTeam->DelMember(m_pInfo->idPlayer);
            }
        }
        break;
    case _MSGTEAM_DISMISS:
        {
            pTeam->Dismiss(m_pInfo->idPlayer);
        }
        break;
    }
}
