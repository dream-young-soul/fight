
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgInteract.cpp: interface for the CMsgInteract class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MsgInteract.h"
#include "npcworld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgInteract::CMsgInteract()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgInteract::~CMsgInteract()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_INTERACT != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY, DWORD dwData /*= 0*/)
{
    CHECKF(unType && idSender && idTarget);
    // init
    this->Init();
    // fill info now
    m_pInfo->unType			= unType;
    m_pInfo->idSender		= idSender;
    m_pInfo->idTarget		= idTarget;
    m_pInfo->unPosX			= unPosX;
    m_pInfo->unPosY			= unPosY;
    m_pInfo->dwData			= dwData;
    m_unMsgType	= _MSG_INTERACT;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgInteract::Process(void* pInfo)
{
    IRole* pRole = UserManager()->QueryRole(m_pInfo->idSender);
    if (!pRole)
    {
        return;
    }
    switch(m_pInfo->unType)
    {
    case	INTERACT_STEAL:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            pNpc->BeAttack(pRole);
        }
        break;
    case	INTERACT_ATTACK:
    case	INTERACT_SHOOT:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idSender);
            if (pNpc)
            {
                pNpc->AttackOnce();
            }
            {
                CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
                if (pNpc)
                {
                    pNpc->BeAttack(pRole);
                }
            }
        }
        break;
    case	INTERACT_RUSHATK:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (pNpc)
            {
                pNpc->BeAttack(pRole);
            }
            CUser* pUser = UserManager()->QueryUser(m_pInfo->idSender);
            if (pUser)
            {
                pUser->JumpPos(m_pInfo->unPosX, m_pInfo->unPosY, m_pInfo->dwData);
            }
        }
        break;
    case	INTERACT_MAGICATTACK:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
        }
        break;
    case	INTERACT_ABORTMAGIC:
        {
            ASSERT(!"INTERACT_ABORTMAGIC");
        }
        break;
    case	INTERACT_HEAL:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("thanks!");
        }
        break;
    case	INTERACT_POISON:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            pNpc->BeAttack(pRole);
        }
        break;
    case	INTERACT_ASSASSINATE:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            pNpc->BeAttack(pRole);
        }
        break;
    case	INTERACT_FREEZE:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            pNpc->BeAttack(pRole);
        }
        break;
    case	INTERACT_UNFREEZE:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("thanks!");
        }
        break;
    case	INTERACT_COURT:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("sorry!");
        }
        break;
    case	INTERACT_MARRY:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("sorry!");
        }
        break;
    case	INTERACT_DIVORCE:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("sorry!");
        }
        break;
    case	INTERACT_PRESENTMONEY:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("thanks!");
        }
        break;
    case	INTERACT_PRESENTITEM:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("thanks!");
        }
        break;
    case	INTERACT_SENDFLOWERS:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("thanks!");
        }
        break;
    case	INTERACT_JOINSYN:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("sorry!");
        }
        break;
    case	INTERACT_ACCEPTSYNMEMBER:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("sorry!");
        }
        break;
    case	INTERACT_KICKOUTSYNMEMBER:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("sorry!");
        }
        break;
    case	INTERACT_POWER:
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
            if (!pNpc)
            {
                return;
            }
            //pNpc->SendMsg("thanks!");
        }
        break;
    case	INTERACT_QUERYINFO:
        {
            ASSERT(!"INTERACT_QUERYINFO");
        }
        break;
    case	INTERACT_KILL:
        {
            if (IsSceneID(m_pInfo->idTarget))
            {
                ;
            }
            else
            {
                CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idTarget);
                if (pNpc)
                {
                    //pNpc->SetEffect(KEEPEFFECT_DIE|pNpc->GetEffect());
                    pNpc->BeKill(pRole);
                }
            }
            //else
            {
                /*CUserPtr pUser = UserManager()->QueryUser(m_pInfo->idTarget);
                if (pUser)
                	pUser->SetEffect(KEEPEFFECT_DIE|pUser->GetEffect());*/
            }
        }
        break;
    case	INTERACT_BUMP:
        {
            bool	nBack	= (m_pInfo->dwData & 0x10000000) == 0;
            CUser* pUser = UserManager()->QueryUser(m_pInfo->idSender);
            if (pUser && nBack)
            {
                pUser->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
            }
            int		nDir	= (m_pInfo->dwData & 0x0FFFFFFF) / 0x01000000;
            if (nDir >= 0 && nDir < MAX_DIRSIZE)
            {
                OBJID idTarget = m_pInfo->idTarget;
                if (IsSceneID(idTarget))
                {
                    ASSERT(!"INTERACT_BUMP");
                }
                else
                {
                    if (!IsNpcID(idTarget))
                    {
                        CUserPtr pTarget = UserManager()->QueryUser(idTarget);
                        if (pTarget)
                        {
                            if (pTarget->IsAlive())
                            {
                                pTarget->MoveForward(nDir);
                            }
                            break;
                        }
                    }
                    CNpcPtr pNpc = NpcManager()->QueryNpc(idTarget);
                    if (pNpc && pNpc->IsAlive())
                    {
                        POINT pos;
                        pos.x = m_pInfo->unPosX;
                        pos.y = m_pInfo->unPosY;
                        pNpc->SynchroPos(pos.x, pos.y, pos.x + _DELTA_X[nDir], pos.y + _DELTA_Y[nDir]);
                        pNpc->BeAttack(pRole);
                    }
                }
            }
        }
        break;
    } // switch
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgInteract, id:%u", m_pInfo->id);
#endif
}







