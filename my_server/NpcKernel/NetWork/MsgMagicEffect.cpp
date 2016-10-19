
//**********************************************************
// 代码编辑器
//**********************************************************

////////////////////////////////////////////////////////////////
//
//		MsgMagicEffect.cpp
//

#include "AllMsg.h"
#include "NpcWorld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgMagicEffect::CMsgMagicEffect()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgMagicEffect::~CMsgMagicEffect()
{
}


//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::Create(OBJID idUser, int nType, int nLevel, OBJID idTarget, DWORD dwData)
{
    CHECKF(idUser != ID_NONE && idTarget != ID_NONE);
    // init
    this->Init();
    // fill info now
    m_unMsgSize		= sizeof(MSG_Info);
    m_unMsgType		= _MSG_MAGICEFFECT;
    m_pInfo->idUser			= idUser;
    m_pInfo->usType			= nType;
    m_pInfo->usLevel		= nLevel;
    m_pInfo->idTarget		= idTarget;
    m_pInfo->ucEffectNum	= 0;
    //	if (dwData)
    {
        m_unMsgSize		+= sizeof(EffectRoleStruct);
        m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idTarget;
        m_pInfo->setEffectRole[m_pInfo->ucEffectNum].dwData	= dwData;
        m_pInfo->ucEffectNum++;
    }
    return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::CreateByPos(OBJID idUser, int nType, int nLevel, int x, int y, OBJID idTarget/*=ID_NONE*/, DWORD dwData/*=0*/)
{
    CHECKF(idUser != ID_NONE);
    // init
    this->Init();
    // fill info now
    m_unMsgSize		= sizeof(MSG_Info);
    m_unMsgType		= _MSG_MAGICEFFECT;
    m_pInfo->idUser			= idUser;
    m_pInfo->usType			= nType;
    m_pInfo->usLevel		= nLevel;
    m_pInfo->usPosX			= x;
    m_pInfo->usPosY			= y;
    m_pInfo->ucEffectNum	= 0;
    if (idTarget != ID_NONE)
    {
        m_unMsgSize		+= sizeof(EffectRoleStruct);
        m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idTarget;
        m_pInfo->setEffectRole[m_pInfo->ucEffectNum].dwData	= dwData;
        m_pInfo->ucEffectNum++;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::CreateCollide(OBJID idUser, int nType, int nLevel, OBJID idTarget, DWORD dwData, int nCollideDir)
{
    CHECKF(idUser != ID_NONE && idTarget != ID_NONE);
    // init
    this->Init();
    // fill info now
    m_unMsgSize		= sizeof(MSG_Info);
    m_unMsgType		= _MSG_MAGICEFFECT;
    m_pInfo->idUser			= idUser;
    m_pInfo->usType			= nType;
    m_pInfo->usLevel		= nLevel;
    m_pInfo->ucCollideDir	= nCollideDir;
    m_pInfo->ucEffectNum	= 0;
    //	if (dwData)
    {
        m_unMsgSize		+= sizeof(EffectRoleStruct);
        m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idTarget;
        m_pInfo->setEffectRole[m_pInfo->ucEffectNum].dwData	= dwData;
        m_pInfo->ucEffectNum++;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::AppendRole(OBJID idRole, DWORD dwData)
{
    CHECKF(idRole != ID_NONE);
    // fill info now
    m_unMsgSize		+= sizeof(EffectRoleStruct);
    m_pInfo->setEffectRole[m_pInfo->ucEffectNum].idRole	= idRole;
    m_pInfo->setEffectRole[m_pInfo->ucEffectNum].dwData	= dwData;
    m_pInfo->ucEffectNum++;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicEffect::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_MAGICEFFECT != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMagicEffect::Process(CGameSocket* pSocket)
{
    CNpcPtr pNpc = NpcManager()->QueryNpc(m_pInfo->idUser);
    if (pNpc)
    {
        pNpc->AttackOnce();
    }
    // check npc be attack
    IRole* pRole = UserManager()->QueryRole(m_pInfo->idUser);
    if (pRole && m_pInfo->usType != MAGICSORT_RECRUIT)
    {
        OBJID idTarget = m_pInfo->idTarget;
        if (idTarget && idTarget != pRole->GetID())
        {
            CNpcPtr pNpc = NpcManager()->QueryNpc(idTarget);
            if (pNpc)
            {
                pNpc->BeAttack(pRole);
            }
        }
        for(int i = 0; i < m_pInfo->ucEffectNum; i++)
        {
            OBJID idRole = m_pInfo->setEffectRole[i].idRole;
            if (idRole != idTarget)		// call BeAttack() one times.
            {
                CNpcPtr pNpc = NpcManager()->QueryNpc(idRole);
                if (pNpc)
                {
                    pNpc->BeAttack(pRole);
                }
            }
        }
    }
    CUserPtr pUser = UserManager()->QueryUser(m_pInfo->idUser);
    if (!pUser)
    {
        return;
    }
    switch(m_pInfo->usType)		// 只处理会MOVE的消息
    {
    case	MAGICSORT_JUMPATTACK:
        {
            pUser->JumpPos(m_pInfo->usPosX, m_pInfo->usPosY, 0);
        }
        break;
    case	MAGICSORT_RANDOMTRANS:
        {
            pUser->JumpPos(m_pInfo->usPosX, m_pInfo->usPosY, 0);
        }
        break;
    case	MAGICSORT_COLLIDE:
        {
            /* move code to interact
            int nDir = m_pInfo->ucCollideDir;
            if (nDir >= 0 && nDir < MAX_DIRSIZE)
            {
            	pUser->MoveForward(nDir);
            	OBJID idTarget = m_pInfo->setEffectRole[0].idRole;
            	if (!IsNpcID(idTarget))IsSceneID
            	{
            		CUserPtr pTarget = UserManager()->QueryUser(idTarget);
            		if (pTarget && pTarget->IsAlive())
            			pTarget->MoveForward(m_pInfo->ucCollideDir);
            	}
            	else
            	{
            		CNpcPtr pNpc = NpcManager()->QueryNpc(idTarget);
            		if (pNpc && pNpc->IsAlive())
            		{
            			POINT pos;
            			pos.x = pUser->GetPosX()+_DELTA_X[nDir];
            			pos.y = pUser->GetPosY()+_DELTA_Y[nDir];
            			pNpc->SynchroPos(pos.x, pos.y, pos.x+_DELTA_X[nDir], pos.y+_DELTA_Y[nDir]);
            		}
            	}
            }*/
        }
        break;
    }
}


