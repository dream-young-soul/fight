
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSteal.cpp: implementation of the CMsgInteract class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "3dGameMap.h"
#include "GamePlayerSet.h"
#include "Hero.h"
#include "GameDataSet.h"
#include "GameMsg.h"
#include "MagicData.h"
#include "../NdSound/NDSound.h"

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
BOOL CMsgInteract::Create(USHORT usType, OBJID idSender, OBJID idTarget, USHORT usPosX, USHORT usPosY, USHORT usData0, USHORT usData1, OBJID idCopy)
{
    if (idSender == ID_NONE)
    {
        return false;
    }
    m_pInfo->unMsgSize		= sizeof(MSG_Info);
    m_pInfo->unMsgType		= _MSG_INTERACT;
    m_pInfo->dwTimeStamp	=::TimeGet();
    m_pInfo->usData0		= usData0;
    m_pInfo->usData1		= (usData1 + 0x100 * (m_pInfo->dwTimeStamp % 0x100)) ^ 0x3721;
    m_pInfo->idSender		= idSender;
    m_pInfo->idTarget		= idTarget;
    m_pInfo->unPosX			= usPosX;
    m_pInfo->unPosY			= usPosY;
    m_pInfo->unType			= usType;
    switch(usData0)
    {
    case 1000:
    case 1001:
    case 1002:
    case 1030:
    case 1125:
    case 1150:
    case 1165:
    case 1160:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idCopy, true);
            if (pPlayer->IsMouseFocus())
            {
                m_pInfo->usData1 ^= 0x6279;
            }
        }
        break;
    }
    //	if (_MSG_INTERACT == m_pInfo->unMsgType)
    //		g_objGameMsg.AddMsg(_TXTATR_NORMAL, "INTERACT msg create.");
    return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(USHORT usType, OBJID idSender, OBJID idTarget, OBJID idBullet)
{
    if (idSender == ID_NONE || idTarget == ID_NONE)
    {
        return false;
    }
    m_pInfo->unMsgSize		= sizeof(MSG_Info);
    m_pInfo->unMsgType		= _MSG_INTERACT;
    m_pInfo->dwTimeStamp	=::TimeGet();
    m_pInfo->idSender		= idSender;
    m_pInfo->idTarget		= idTarget;
    m_pInfo->unType			= usType;
    m_pInfo->idBullet		= idBullet;
    return true;
}
//////////////////////////////////////////////////////////////////////

BOOL CMsgInteract::Create(USHORT usType, OBJID idSender, OBJID idTarget, USHORT usPosX, USHORT usPosY, DWORD dwData)
{
    if (idSender == ID_NONE || idTarget == ID_NONE)
    {
        return false;
    }
    m_pInfo->unMsgSize		= sizeof(MSG_Info);
    m_pInfo->unMsgType		= _MSG_INTERACT;
    m_pInfo->dwTimeStamp	=::TimeGet();
    m_pInfo->idSender		= idSender;
    m_pInfo->idTarget		= idTarget;
    m_pInfo->unPosX			= usPosX;
    m_pInfo->unPosY			= usPosY;
    m_pInfo->unType			= usType;
    m_pInfo->dwData			= dwData;
    return true;
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
void CMsgInteract::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogSave("Process MsgSteal, idStealer:%u, idTarget:%u, CurPos:(%d,%d)",
              m_pInfo->idSender,
              m_pInfo->idTarget,
              m_pInfo->unPosX,
              m_pInfo->unPosY);
#endif
    // get sender
    BOOL bIncludeHero = true;
    CPlayer* pSender = g_objPlayerSet.GetPlayer(m_pInfo->idSender, bIncludeHero);
    if (!pSender)
    {
        CMyPos posHero = g_objHero.GetAlignPos();
        //if (abs(posHero.x-(int)m_pInfo->unPosX) <= _BLOCK_SIZE
        //	&& abs(posHero.y-(int)m_pInfo->unPosY) <= _BLOCK_SIZE)
        {
            CMsgAction msg;
            if (msg.Create(g_objHero.GetID(), posHero.x, posHero.y, g_objHero.GetDir(), actionQueryPlayer, m_pInfo->idSender))
            {
                msg.Send();
            }
        }
    }
    CPlayer* pTarget	= g_objPlayerSet.GetPlayer(m_pInfo->idTarget, bIncludeHero);
    // ....
    switch(m_pInfo->unType)
    {
    case INTERACT_MAGICATTACK:
        {
            MagicTypeInfo infoMagicType;
            if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_pInfo->usData0, m_pInfo->usData1, infoMagicType))
            {
                return;
            }
            if (m_pInfo->idSender == g_objHero.GetID())
            {
                g_objHero.FlashLastAttackTime();
                g_objHero.StartMagicBusy(infoMagicType.dwIntoneDuration + infoMagicType.dwMagicBreak + 3000);
                return;
            }
            //if (infoMagicType.dwSenderAction == _ACTION_INTONE_DURATION)
            {
                CCommand cmd;
                cmd.iType		= _COMMAND_EMOTION;
                cmd.iStatus		= _CMDSTATUS_BEGIN;
                cmd.nData		= _ACTION_STANDBY;
                if (pSender && pTarget)
                {
                    CMyPos posSender, posTarget;
                    pSender->GetPos(posSender);
                    pSender->FlashLastAttackTime();
                    pTarget->FlashLastAttackTime();
                    pTarget->GetPos(posTarget);
                    cmd.nDir = (g_objGameMap.GetDirection(posSender.x, posSender.y, posTarget.x, posTarget.y) + 1) % 7;
                }
                if (pSender && !pTarget)
                {
                    CMyPos posSender, posTarget;
                    pSender->GetPos(posSender);
                    pSender->FlashLastAttackTime();
                    posTarget.x = m_pInfo->unPosX;
                    posTarget.y = m_pInfo->unPosY;
                    cmd.nDir = (g_objGameMap.GetDirection(posSender.x, posSender.y, posTarget.x, posTarget.y) + 1) % 7;
                }
                pSender->SetCommand(&cmd);
                pSender->m_objEffect.Add(infoMagicType.szIntoneEffect, false, 0, pSender->GetLook());
            }
        }
        break;
    case INTERACT_COURT:
        {
            if (g_objPlayerSet.GetPlayer(m_pInfo->idSender))
            {
                ::PostCmd(CMD_BECOURT, m_pInfo->idSender);
            }
        }
        break;
    case INTERACT_BUMP:
        {
            if (pSender && pTarget )
            {
                if ( pTarget->GetID() == g_objHero.GetID())
                {
                    g_objHero.SetIDWhoAttackMe(pSender->GetID());
                }
                else
                {
                    pTarget->SetIDWhoAttackMe(pSender->GetID());
                }
            }
            if (pSender && pTarget && pSender->GetID() == g_objHero.GetID())
            {
                g_objHero.SetIDMyTarget(pTarget->GetID());
            }
            int nForward	= m_pInfo->dwData / 0x10000000;
            int	nDir		= (m_pInfo->dwData / 0x01000000) % 0x0f;
            CMyPos posTarget = { m_pInfo->unPosX, m_pInfo->unPosY };
            if (pSender)
            {
                // attacker
                if (pSender->GetID() != g_objHero.GetID())
                {
                    pSender->ResetStepDir();
                    CMyPos posSender;
                    pSender->GetPos(posSender);
                    if (nDir >= _MAX_DIRECTION)
                    {
                        pSender->SetPos(posTarget.x, posTarget.y);
                    }
                    else
                    {
                        DWORD dwType = 1;
                        if (nForward > 0)
                        {
                            dwType = 0;
                        }
                        pSender->SetActionCmd(_ACTION_BUMP, nDir, dwType);
                        if (posSender.x != posTarget.x - _DELTA_X[nDir]
                                || posSender.y != posTarget.y - _DELTA_Y[nDir])
                        {
                            pSender->SetPos(posTarget.x - _DELTA_X[nDir], posTarget.y - _DELTA_Y[nDir]);
                        }
                    }
                }
                else
                {
                    g_objHero.SetAlignPos(posTarget.x, posTarget.y);
                }
            }
            if (pTarget)
            {
                // target
                DWORD dwDamage	= m_pInfo->dwData & 0x00ffffff;
                DWORD dwColor	= _COLOR_YELLOW;
                if (pTarget->GetID() == g_objHero.GetID())
                {
                    // target is hero
                    dwColor = _COLOR_RED;
                    if (dwDamage > 0)	// suc atk
                    {
                        if (nDir < _MAX_DIRECTION)
                        {
                            CMyPos posHero;
                            g_objHero.GetPos(posHero);
                            if (posHero.x == posTarget.y
                                    || posHero.y == posTarget.y)
                            {
                                // yes so i must pop off as others
                                if (!g_objHero.IsMoving())
                                {
                                    g_objHero.ResetActionData();
                                }
                                g_objHero.SetActionCmd(_ACTION_POPOFF, nDir, 1);
                                g_objHero.SetAlignPos(posHero.x + _DELTA_X[nDir], posHero.y + _DELTA_Y[nDir]);
                            }
                            else
                            {
                                // no, do nothing but inform others my current postion
                                CMsgAction msg;
                                IF_SUC (msg.Create(g_objHero.GetID(), 0, 0, g_objHero.GetDir(), actionSynchro, posHero.x, posHero.y))
                                msg.Send();
                            }
                        }
                        else	// no bump movment
                        {
                            if (!g_objHero.IsMoving())
                            {
                                g_objHero.ResetActionData();
                                g_objHero.SetActionCmd(_ACTION_BRUISE0, g_objHero.GetDir(pSender));
                            }
                        }
                    }
                    g_objHero.AddZoomNum(dwDamage, dwColor, 300, 30);
                }
                else
                {
                    if (dwDamage > 0)	// suc atk
                    {
                        pTarget->ResetStepDir();
                        if (nDir < _MAX_DIRECTION)
                        {
                            pTarget->SetActionCmd(_ACTION_POPOFF, nDir, 1);
                        }
                        else
                        {
                            pTarget->SetActionCmd(_ACTION_BRUISE0, pTarget->GetDir(pSender));
                        }
                        if (pTarget->IsNpc())
                        {
                            int nLife = (int)pTarget->GetData(CPlayer::_PLAYER_LIFE) - (int)dwDamage;
                            pTarget->SetData(CPlayer::_PLAYER_LIFE, __max(0, nLife));
                            //pTarget->SetLife(__max(0, nLife));
                        }
                    }
                    pTarget->AddZoomNum(dwDamage, dwColor, 300, 30);
                }
            }
        }
        break;
    case INTERACT_MINE:
        {
            if (pSender && pTarget)
            {
                int nDr	= g_objHero.GetDir(pTarget);
                pSender->SetActionCmd(_ACTION_MINE, nDr);
                pSender->FlashLastAttackTime();
            }
            else if (pSender)
            {
                pSender->SetActionCmd(_ACTION_MINE);
                pSender->FlashLastAttackTime();
            }
        }
        break;
    case INTERACT_ATTACK:
        {
            if (pSender && pTarget )
            {
                if ( pTarget->GetID() == g_objHero.GetID())
                {
                    g_objHero.SetIDWhoAttackMe(pSender->GetID());
                }
                else
                {
                    pTarget->SetIDWhoAttackMe(pSender->GetID());
                }
            }
            OBJID idMyAttackTarget = 0;
            if (pSender && pTarget && (pSender->GetID() == g_objHero.GetID() || pTarget->IsMyPet()))
            {
                idMyAttackTarget = g_objHero.GetIDMyTarget();
                if (pSender->GetID() == g_objHero.GetID() && !pTarget->IsMyPet())
                {
                    g_objHero.SetIDMyTarget(pTarget->GetID());
                }
            }
            if (pSender && pTarget && pSender->IsMyPet() && !pTarget->IsMyPet())
            {
                CPet* pPet = g_objHero.GetPetByID(pSender->GetID());
                if (pPet)
                {
                    pPet->SetAtkTarget(pTarget->GetID());
                }
            }
            if (pSender)
            {
                pSender->FlashLastAttackTime();
                // attacker show
                if (g_objHero.GetID() == pSender->GetID())
                {
                    if (g_objHero.m_bFirstAtk)
                    {
                        g_objHero.m_bFirstAtk = false;
                        g_objHero.SetAttackTimes(0);
                        if (idMyAttackTarget && g_objHero.GetCallPetAmount() && g_objHero.GetProfession() == PROFESSIONAL_WARRIOR)//群体攻击
                        {
                            /*
                            							CMyPos posTarget ,posHero,posTemp[3];
                            							int nDirHero2Target;
                            							nDirHero2Target = g_objHero.GetDir(pTarget);
                            							pTarget->GetPos(posTarget);
                            							g_objHero.GetPos(posHero);
                            							int nDistance = g_objGameMap.GetDistance(posTarget.x,posTarget.y,posHero.x,posHero.y);
                            							posTemp[0].x = posTarget.x + _DELTA_X[nDirHero2Target+4]*nDistance;
                            							posTemp[0].y = posTarget.y + _DELTA_Y[nDirHero2Target+4]*nDistance;
                            							posTemp[1].x = posTarget.x + _DELTA_X[(nDirHero2Target+2)%8]*nDistance;
                            							posTemp[1].y = posTarget.y + _DELTA_Y[(nDirHero2Target+2)%8]*nDistance;
                            							posTemp[2].x = posTarget.x + _DELTA_X[(nDirHero2Target+6)%8]*nDistance;
                            							posTemp[2].y = posTarget.y + _DELTA_Y[(nDirHero2Target+6)%8]*nDistance;
                            */
                            for(int i = 0; i < g_objHero.GetCallPetAmount(); i++)
                            {
                                CPet* pPet = g_objHero.GetPetByIndex(i);
                                if (pPet && !pPet->IsGhost())
                                {
                                    pPet->LockAttack(pTarget);
                                    //									OBJID id = pPet->GetID();
                                }
                            }
                        }
                    }
                    else
                    {
                        int nAction = 0;
                        if (idMyAttackTarget && g_objHero.GetCallPetAmount() && g_objHero.GetProfession() == PROFESSIONAL_WARRIOR)//群体攻击
                        {
                            int nAttackTime = pSender->GetAttackTimes();
                            nAction = _ACTION_ATTACK0 + nAttackTime;
                            if (!pSender->IsMoving() && !pSender->IsAttacking())
                            {
                                g_objHero.SetAttackTimes(nAttackTime >= 2 ? 0 : nAttackTime + 1);
#ifdef _DEBUG
                                char sztemp[64];
                                sprintf(sztemp, "Now Action Type %d", nAction);
                                g_objGameMsg.AddMsg(sztemp);
#endif
                            }
                        }
                        else
                        {
                            nAction = _ACTION_ATTACK0 +::RandGet(3);
                        }
                        int nDir	= g_objHero.GetDir(pTarget);
                        if (!g_objHero.IsMoving()
                                && !g_objHero.IsAttacking())
                        {
                            g_objHero.SetActionCmd(nAction, nDir);
#ifdef _DEBUG
                            char sztemp[64];
                            sprintf(sztemp, "Attack Action Type %d", nAction);
                            g_objGameMsg.AddMsg(sztemp);
#endif
                        }
                    }
                }
                else
                {
                    pSender->ResetStepDir();
                    int nAction = _ACTION_ATTACK0 +::RandGet(3);
                    int nDir	= pSender->GetDir(pTarget);
                    if (!pSender->IsMyPet())
                    {
                        pSender->SetActionCmd(nAction, nDir);
                    }
                    else if (!pSender->IsMoving() && !pSender->IsAttacking())
                    {
                        CPet* pPet = (CPet*)pSender;
                        if (pSender->IsMyPet() && g_objHero.GetIDMyTarget() == pPet->GetIDMyTarget() && g_objHero.GetProfession() == PROFESSIONAL_WARRIOR)
                        {
                            int nAttackTime = pSender->GetAttackTimes();
                            nAction = _ACTION_ATTACK0 + nAttackTime;
                            if (!pSender->IsMoving() && !pSender->IsAttacking())
                            {
                                pSender->SetAttackTimes(nAttackTime >= 2 ? 0 : nAttackTime + 1);
                            }
                        }
                        pSender->SetActionCmd(nAction, nDir);
                    }
                }
            }
            if (pTarget)
            {
                // target show
                int nDir = pTarget->GetDir();
                if (pSender)
                {
                    nDir = pTarget->GetDir(pSender);
                }
                DWORD dwColor = _COLOR_YELLOW;
                if (pTarget->GetID() == g_objHero.GetID())
                {
                    g_objHero.FlashLastAttackTime();
                    if (m_pInfo->dwData > 0)
                    {
                        dwColor = _COLOR_RED;
                        g_objHero.AddZoomNum(m_pInfo->dwData, dwColor, 300, 30);
                        if (!g_objHero.IsMoving()
                                && !g_objHero.IsAttacking())
                            //&& !g_objHero.TestStatus(USERSTATUS_SUPERSOLDIER)
                            //&& !g_objHero.TestStatus(USERSTATUS_TORNADO))
                        {
                            g_objHero.SetActionCmd(_ACTION_BRUISE0, nDir);    //(nDir+4)%8);
                        }
                        if (pSender && pSender->TestStatus(USERSTATUS_ADJUST_XP))	//对方处于xp增加状态特殊受击光效
                        {
                            g_objHero.m_objEffect.Add("Xattack05", false, 0, g_objHero.GetLook());
                        }
                    }
                    else
                    {
                        g_objHero.AddZoomNum(m_pInfo->dwData, dwColor, 300, 30);
                        if (!g_objHero.IsMoving()
                                && !g_objHero.IsAttacking())
                            //&& !g_objHero.TestStatus(USERSTATUS_SUPERSOLDIER)
                            //&& !g_objHero.TestStatus(USERSTATUS_TORNADO))
                        {
                            g_objHero.SetActionCmd(_ACTION_DODGE0, nDir);    //(nDir+4)%8);
                        }
                    }
                }
                else
                {
                    pTarget->FlashLastAttackTime();
                    if (m_pInfo->dwData > 0)
                    {
                        if (pTarget->IsGuard())
                        {
                            dwColor = _COLOR_BLUE;
                        }
                        // 添加延时的受伤动作命令.
                        CCommand cmd;
                        cmd.iType	= _COMMAND_WOUND;
                        cmd.iStatus	= _CMDSTATUS_BEGIN;
                        cmd.nDir	= pTarget->GetDir(pSender);
                        cmd.nData	= _ACTION_BRUISE0;
                        CONST CActionDelayInfo* pInfoDelay = NULL;
                        CONST CWeaponInfo* pInfoWeapon = NULL;
                        if (pSender)
                        {
                            DWORD dwKey = pSender->GetActionDelayInfoKey(pTarget);
                            pInfoDelay = g_obj3DRoleData.GetActionDelayInfo(dwKey);
                            dwKey = pSender->GetWeaponInfoKey(pTarget);
                            pInfoWeapon = g_obj3DRoleData.GetWeaponInfo(dwKey);
                        }
                        DWORD tmDelay = 200;
                        if (pInfoDelay)
                        {
                            tmDelay = pInfoDelay->tmWoundDelay;
                        }
                        CONST CHAR* pszEffect	= NULL;
                        CONST CHAR* pszSound	= NULL;
                        if (pInfoWeapon)
                        {
                            pszEffect	= pInfoWeapon->strHitEffect.c_str();
                            pszSound	= pInfoWeapon->strHitSound.c_str();
                        }
                        pTarget->AddZoomNum(m_pInfo->dwData, dwColor, tmDelay, 30);
                        pTarget->SetTimerCommand(cmd, ::TimeGet() + tmDelay, pszEffect, pszSound);
                        if (pTarget->IsNpc())
                        {
                            int nLife = (int)pTarget->GetData(CPlayer::_PLAYER_LIFE) - (int)m_pInfo->dwData;
                            pTarget->SetData(CPlayer::_PLAYER_LIFE, __max(0, nLife));
                            //pTarget->SetLife(__max(0, nLife));
                        }
                        if (pSender && pSender->TestStatus(USERSTATUS_ADJUST_XP))	//对方处于xp增加状态特殊受击光效
                        {
                            pTarget->m_objEffect.Add("Xattack05", false, 0, pTarget->GetLook());
                        }
                    }
                    else
                    {
                        CCommand cmd;
                        cmd.iType	= _COMMAND_ACTION;
                        cmd.iStatus	= _CMDSTATUS_BEGIN;
                        cmd.nData	= _ACTION_DODGE0;
                        cmd.nDir	= nDir;
                        CONST CActionDelayInfo* pInfoDelay = NULL;
                        CONST CWeaponInfo* pInfoWeapon = NULL;
                        if (pSender)
                        {
                            DWORD dwKey = pSender->GetActionDelayInfoKey(pTarget);
                            pInfoDelay = g_obj3DRoleData.GetActionDelayInfo(dwKey);
                            dwKey = pSender->GetWeaponInfoKey(pTarget);
                            pInfoWeapon = g_obj3DRoleData.GetWeaponInfo(dwKey);
                        }
                        DWORD tmDelay = 200;
                        if (pInfoDelay)
                        {
                            tmDelay = pInfoDelay->tmBlockDelay;
                        }
                        CONST CHAR* pszEffect	= NULL;
                        CONST CHAR* pszSound	= NULL;
                        if (pInfoWeapon)
                        {
                            pszEffect	= pInfoWeapon->strBlkEffect.c_str();
                            pszSound	= pInfoWeapon->strBlkSound.c_str();
                        }
                        pTarget->AddZoomNum(m_pInfo->dwData, dwColor, tmDelay, 30);
                        pTarget->SetTimerCommand(cmd, ::TimeGet() + tmDelay, pszEffect, pszSound);
                    }
                }
            }
        }
        break;
    case INTERACT_RUSHATK:
        {
            if (pSender)
            {
            }
        }
        break;
    case INTERACT_KILL:
        {
            if (pTarget)
            {
                pTarget->SetData(CPlayer::_PLAYER_LIFE, 0);
                pTarget->ResetStepDir();
                if (g_objHero.GetID() == m_pInfo->idTarget)
                {
                    pTarget->SetPos(m_pInfo->unPosX, m_pInfo->unPosY);
                    ::PostCmd(CMD_DIE);
                }
                // do die action
                if (pTarget->IsPlayer())
                {
                    // 命令设置
                    CCommand cmd;
                    cmd.iType	= _COMMAND_DIE;
                    cmd.iStatus	= _CMDSTATUS_BEGIN;
                    cmd.nDir		= pTarget->GetDir(pSender);
                    cmd.dwData		= m_pInfo->usData0;
                    pTarget->SetCommand(&cmd);
                }
                else
                {
                    CONST CActionDelayInfo* pInfoDelay = NULL;
                    CONST CWeaponInfo* pInfoWeapon = NULL;
                    if (pSender)
                    {
                        DWORD dwKey = pSender->GetActionDelayInfoKey(pTarget);
                        pInfoDelay = g_obj3DRoleData.GetActionDelayInfo(dwKey);
                        dwKey = pSender->GetWeaponInfoKey(pTarget);
                        pInfoWeapon = g_obj3DRoleData.GetWeaponInfo(dwKey);
                    }
                    DWORD tmDelay = 200;
                    if (pInfoDelay)
                    {
                        tmDelay = pInfoDelay->tmDieDelay;
                    }
                    CONST CHAR* pszEffect	= NULL;
                    CONST CHAR* pszSound	= NULL;
                    if (pInfoWeapon)
                    {
                        pszEffect	= pInfoWeapon->strBlkEffect.c_str();
                        pszSound	= pInfoWeapon->strBlkSound.c_str();
                    }
                    pTarget->ResetActionData();
                    pTarget->ClrTimerCommand();	// 清除没有执行的其他命令
                    // 命令设置
                    CCommand cmd;
                    cmd.iType	= _COMMAND_DIE;
                    cmd.iStatus	= _CMDSTATUS_BEGIN;
                    cmd.nDir	= pTarget->GetDir();
                    cmd.dwData	= m_pInfo->usData0;
                    pTarget->SetTimerCommand(cmd, ::TimeGet() + tmDelay, pszEffect, pszSound);
                }
                if (pSender && !pTarget->IsNpc() && !pTarget->IsMyPet())
                {
                    const OBJID idMsgDie = 100123;
                    g_objGameMsg.AddMsg(_TXTATR_TALK, g_objGameDataSet.GetStr(idMsgDie), pTarget->GetName(), pSender->GetName());
                }
            }
            if (pSender)
            {
                // ko count...
                if (pSender->GetID() == g_objHero.GetID() && pTarget && pTarget->IsMonster()
                        && g_objHero.TestStatus(USERSTATUS_XPFULL))
                {
                    g_objHero.AddKoCount();
                    //					g_objHero.ResetXpFullTime(m_pInfo->usData1);
                }
                if (pTarget->IsPet())
                {
                    pTarget->m_objEffect.Add("baby_d2");
                }
            }
        }
        break;
    case INTERACT_SOUL:
        // soul ...
        if (!pSender->IsNpc() && pSender->IsPet())
        {
            CPet* pPet = (CPet*)pSender;
            if (pPet->IsMyPet())
            {
                pPet->AddPetSoul(m_pInfo->dwData);
            }
            for(DWORD i = 0; i < m_pInfo->dwData; i++)
            {
                CMagicEffect* pEffect =
                    CMagicEffect::CreateNew(m_pInfo->idTarget, m_pInfo->idSender,
                                            0, 0, MAGICEFFECT_SOUL, i);
                if (pEffect)
                {
                    g_objGameMap.AddMagicEffect(pEffect);
                }
            }
        }
        break;
    case INTERACT_ABORTMAGIC:
        {
            if (!pSender->IsMoving() && pSender->GetID() == g_objHero.GetID())
            {
                g_objHero.SetActionCmd(_ACTION_STANDBY);
            }
        }
        break;
    case INTERACT_SHOOT:
        {
            if (pSender && pTarget )
            {
                if ( pTarget->GetID() == g_objHero.GetID())
                {
                    g_objHero.SetIDWhoAttackMe(pSender->GetID());
                }
                else
                {
                    pTarget->SetIDWhoAttackMe(pSender->GetID());
                }
            }
            if (pSender && pTarget && (pTarget->GetID() == g_objHero.GetID() || pTarget->IsMyPet()))
            {
                g_objHero.SetIDMyTarget(pTarget->GetID());
            }
#ifdef _DEBUG
            if (pTarget->IsMyPet())
            {
                g_objGameMsg.AddMsg("4");
            }
#endif
            BOOL bFirst = g_objHero.m_bFirstAtk;
            if (pSender && g_objHero.GetID() == pSender->GetID())
            {
                if (g_objHero.m_bFirstAtk)
                {
                    g_objHero.m_bFirstAtk = false;
                }
                else
                {
                    int nAction = _ACTION_ATTACK0 +::RandGet(3);
                    int nDir	= g_objHero.GetDir(pTarget);
                    g_objHero.SetActionCmd(nAction, nDir);
                    CMagicEffect* pEffect = CMagicEffect::CreateNew(pSender->GetID(), pTarget->GetID(), pSender->GetRWeapon(), 0);
                    g_objGameMap.AddMagicEffect(pEffect);
                    pEffect->AddRoleInfo(m_pInfo->idTarget, m_pInfo->dwData);
                    return;
                }
            }
            else
            {
                pSender->ResetStepDir();
                int nAction = _ACTION_ATTACK0 +::RandGet(3);
                int nDir	= pSender->GetDir(pTarget);
                pSender->SetActionCmd(nAction, nDir);
            }
            if (g_objHero.GetID() == pSender->GetID() && bFirst)
            {
                // show target lose hp
                if (m_pInfo->dwData > 0)
                {
                    if (!pTarget->IsAttacking())
                    {
                        pTarget->SetActionCmd(_ACTION_BRUISE0, pTarget->GetDir(pSender));
                    }
                    DWORD dwColor = _COLOR_YELLOW;
                    if (g_objHero.GetID() == pTarget->GetID())
                    {
                        dwColor = _COLOR_RED;
                    }
                    pTarget->AddZoomNum(m_pInfo->dwData, dwColor, 300, 30);
                    if (pTarget->IsNpc() )
                    {
                        int nLife = (int)pTarget->GetData(CPlayer::_PLAYER_LIFE) - (int)m_pInfo->dwData;
                        pTarget->SetData(CPlayer::_PLAYER_LIFE, __max(0, nLife));
                        pTarget->SetLife(__max(0, nLife));
                    }
                }
                else
                {
                    pTarget->AddZoomNum(m_pInfo->dwData, _COLOR_YELLOW, 300, 30);
                    if (!pTarget->IsAttacking() && pTarget->IsPlayer())
                    {
                        pTarget->SetActionCmd(_ACTION_DODGE0, pTarget->GetDir(pSender));
                    }
                }
            }
            else
            {
                pSender->ResetStepDir();
                if (!pSender->IsMoving())
                {
                    pSender->ResetActionData();
                }
                int nAction = _ACTION_ATTACK0 +::RandGet(3);
                int nDir	= pSender->GetDir(pTarget);
                pSender->SetActionCmd(nAction, nDir);
                // create arrow
                if (pSender && pTarget)
                {
                    CMagicEffect* pEffect = CMagicEffect::CreateNew(pSender->GetID(), pTarget->GetID(), pSender->GetRWeapon(), 0);
                    g_objGameMap.AddMagicEffect(pEffect);
                    pEffect->AddRoleInfo(m_pInfo->idTarget, m_pInfo->dwData);
                    /*
                    CMagicEffect* pEffect = CMagicEffect::CreateNewArrow(pSender->GetID(), pTarget->GetID(), pSender->GetLWeapon());
                    g_objGameMap.AddMagicEffect(pEffect);
                    pEffect->AddRoleInfo(m_pInfo->idTarget, m_pInfo->dwData);
                    */
                }
            }
        }
        break;
    default:
        break;
    }
}
