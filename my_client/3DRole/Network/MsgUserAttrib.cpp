
//**********************************************************
// 代码编辑器
//**********************************************************

#include "AllMsg.h"
#include "hero.h"
#include "GamePlayerSet.h"
#include "3DGameMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgUserAttrib::CMsgUserAttrib()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgUserAttrib::~CMsgUserAttrib()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserAttrib::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_USERATTRIB != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgUserAttrib::Process(void* pInfo)
{
    CPlayer* pPlayer	= g_objPlayerSet.GetPlayer(m_pInfo->idUser);
    if (!pPlayer)
    {
        if (m_pInfo->idUser == g_objHero.GetID())
        {
            pPlayer	= &g_objHero;
        }
        else if (g_objHero.GetTeamMemberInfoByID (m_pInfo->idUser))	//在playerset中找不到队友的hp maxhp变化处理
        {
            int nAmount = (int)m_pInfo->dwAttributeNum;
            for(int i = 0; i < nAmount; i ++)
            {
                switch(m_pInfo->aUserAttrib[i].ucAttributeType)
                {
                case _USERATTRIB_MAXLIFE:
                    g_objHero.SetTeamMemberMaxHp(m_pInfo->idUser, m_pInfo->aUserAttrib[i].dwAttributeData);
                    break;
                case _USERATTRIB_LIFE:
                    g_objHero.SetTeamMemberHp(m_pInfo->idUser, m_pInfo->aUserAttrib[i].dwAttributeData);
                    break;
                default:
                    break;
                }
            }
            return;
        }
        else
        {
            return;
        }
    }
    int nAmount = (int)m_pInfo->dwAttributeNum;
    int i = 0;
    for(; i < nAmount; i ++)
    {
        switch(m_pInfo->aUserAttrib[i].ucAttributeType)
        {
        case _USERATTRIB_MAXEUDEMON:
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.SetMaxPetCall(m_pInfo->aUserAttrib[i].dwAttributeData);
            }
            break;
        case _USERATTRIB_MAXENERGY:
            pPlayer->SetMaxPhysicalForce(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_LEV:
            pPlayer->SetLev(m_pInfo->aUserAttrib[i].dwAttributeData);
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.SetData(CPlayer::_PLAYER_CURRENTLIFE, 0);
                g_objHero.SetCurMana(0);
                ::PostCmd(CMD_LEVELUP);
            }
            break;
        case _USERATTRIB_MAXLIFE:
            pPlayer->SetMaxLife(m_pInfo->aUserAttrib[i].dwAttributeData);
            g_objHero.SetTeamMemberMaxHp(pPlayer->GetID(), m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_LIFE:
            pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->aUserAttrib[i].dwAttributeData);
            //pPlayer->SetLife(m_pInfo->aUserAttrib[i].dwAttributeData);
            g_objHero.SetTeamMemberHp(pPlayer->GetID(), m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_MONEY:
            if (g_objHero.GetID() == m_pInfo->idUser)
            {
                g_objHero.SetMoney(m_pInfo->aUserAttrib[i].dwAttributeData);
            }
            break;
        case _USERATTRIB_MANA:
            pPlayer->SetData(CPlayer::_PLAYER_MANA, m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_HAIR:
            pPlayer->SetHair((USHORT)(m_pInfo->aUserAttrib[i].dwAttributeData));
            break;
        case _USERATTRIB_EXP:
            {
                DWORD dwExp = pPlayer->GetExp();
                if (dwExp < m_pInfo->aUserAttrib[i].dwAttributeData)
                {
                    int nShowExp = m_pInfo->aUserAttrib[i].dwAttributeData - dwExp;
                    if (pPlayer->GetLev() > 70)
                    {
                        nShowExp = nShowExp * 10 + ::RandGet(10);
                    }
                    g_objGameMap.SetShowExp(nShowExp);
                }
                pPlayer->SetExp(m_pInfo->aUserAttrib[i].dwAttributeData);
            }
            break;
        case _USERATTRIB_LOOK:
            pPlayer->SetFace((m_pInfo->aUserAttrib[i].dwAttributeData / 10000) % 1000);
            pPlayer->SetSex((m_pInfo->aUserAttrib[i].dwAttributeData / 1000) % 10);
            pPlayer->Transform(m_pInfo->aUserAttrib[i].dwAttributeData / 10000000);
            break;
        case _USERATTRIB_USERSTATUS:
            {
                __int64 dwStatus = pPlayer->GetStatus();
                pPlayer->ReplaceStatus((dwStatus & 0xffffffff00000000) + m_pInfo->aUserAttrib[i].dwAttributeData);
            }
            break;
        case _USERATTRIB_KEEPEFFECT2:
            {
                __int64 dwStatus = pPlayer->GetStatus();
                __int64 dwStatusCopy = m_pInfo->aUserAttrib[i].dwAttributeData;
                pPlayer->ReplaceStatus((dwStatus & 0x00000000ffffffff) + (dwStatusCopy << 32));
            }
            break;
        case _USERATTRIB_ADDPOINT:
            pPlayer->SetAddPoint(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_PK:
            pPlayer->SetPk(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_PORFESSION:
            {
                pPlayer->SetProfession(m_pInfo->aUserAttrib[i].dwAttributeData);
                if (m_pInfo->idUser == g_objHero.GetID() && g_objHero.m_bCanSetHelpTips)
                {
                    g_objHero.SetHelpTip();
                }
            }
            break;
        case _USERATTRIB_SOUL:
            pPlayer->SetSoul(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_HEALTH:
            pPlayer->SetHealth(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_FORCE:
            pPlayer->SetForce(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_SPEED:
            {
                if (pPlayer->TestStatus(USERSTATUS_TEAMLEADER) &&
                        (pPlayer->TestStatus(USERSTATUS_TEAM_HEALTH) ||
                         pPlayer->TestStatus(USERSTATUS_TEAM_ATTACK) ||
                         pPlayer->TestStatus(USERSTATUS_TEAM_DEFENCE) ||
                         pPlayer->TestStatus(USERSTATUS_TEAM_SPEED) ||
                         pPlayer->TestStatus(USERSTATUS_TEAM_EXP) ||
                         pPlayer->TestStatus(USERSTATUS_TEAM_SPIRIT) ||
                         pPlayer->TestStatus(USERSTATUS_TEAM_CLEAN)) )
                {
                    break;
                }
                else
                {
                    pPlayer->SetSpeedRate(m_pInfo->aUserAttrib[i].dwAttributeData);
                }
            }
            break;
        case _USERATTRIB_XP:
            if (pPlayer->GetID() == g_objHero.GetID() && g_objHero.IsXpSkillEnable() && !(g_objGameMap.GetType () & MAPTYPE_BOOTH_ENABLE))
            {
                pPlayer->SetXP(m_pInfo->aUserAttrib[i].dwAttributeData);
                ::PostCmd(CMD_XPCHANGE);
                if (m_pInfo->aUserAttrib[i].dwAttributeData == g_objHero.GetMaxXP())
                {
                    ::PostCmd(CMD_XPFULL);
                }
                if (m_pInfo->aUserAttrib[i].dwAttributeData == 0)
                {
                    ::PostCmd(CMD_XPEMPTY);
                }
            }
            else if (pPlayer->GetID() != g_objHero.GetID())
            {
                pPlayer->SetXP(m_pInfo->aUserAttrib[i].dwAttributeData);
            }
            break;
        case _USERATTRIB_PP:
            pPlayer->SetPhysicalForce(m_pInfo->aUserAttrib[i].dwAttributeData);
            ::PostCmd(CMD_PPCHANGE);
            break;
        case _USERATTRIB_PROFFER:
            pPlayer->SetSyndicateProffer((int)m_pInfo->aUserAttrib[i].dwAttributeData);
            ::PostCmd(CMD_FLASH_SYNDICATE);
            break;
        case _USERATTRIB_KO_ORDER:
            if (pPlayer->GetID() == g_objHero.GetID())
            {
                g_objHero.SetKoXpAdd(m_pInfo->aUserAttrib[i].dwAttributeData);
            }
            break;
        case _USERATTRIB_EXPLOIT:
            pPlayer->SetExploit(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_NOBILITYRANK:
            pPlayer->SetNobilityRank(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_ACTIONSPEED:
            pPlayer->SetSpeedRate(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_MERCENARYLEVEL:
            {
                pPlayer->SetMercenaryLevel(m_pInfo->aUserAttrib[i].dwAttributeData);
                if (m_pInfo->idUser == g_objHero.GetID() && g_objHero.m_bCanSetHelpTips)
                {
                    g_objHero.SetHelpTip();
                }
            }
            break;
        case _USERATTRIB_MERCENARYEXP:
            pPlayer->SetMercenaryExp(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_FAT:
            break;
        case _USERATTRIB_TUTOR_EXP:
            pPlayer->SetTutorExp(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_TUTOR_LEVEL:
            pPlayer->SetTutorLevel(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_DEX:
            pPlayer->SetSpeed(m_pInfo->aUserAttrib[i].dwAttributeData);
            pPlayer->SetDexterity(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_POTENTIAL:
            pPlayer->SetPotential(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        case _USERATTRIB_MAXLIFEPERCENT:
            pPlayer->SetMaxLifePercent(m_pInfo->aUserAttrib[i].dwAttributeData);
            break;
        default:
            break;
        }
    }
    if ((m_pInfo->aUserAttrib[i].ucAttributeType != _USERATTRIB_EXP) &&
            (m_pInfo->aUserAttrib[i].ucAttributeType != _USERATTRIB_MANA) &&
            (m_pInfo->aUserAttrib[i].ucAttributeType != _USERATTRIB_LIFE) &&
            (m_pInfo->aUserAttrib[i].ucAttributeType != _USERATTRIB_MONEY))
    {
        if (m_pInfo->idUser == g_objHero.GetID())
        {
            ::PostCmd(CMD_LEVELUP);
        }
    }
}
