
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgUserInfo.cpp: interface for the CMsgUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "GamePlayerset.h"
#include "Hero.h"
#include "MsgUserinfo.h"
#include "GameMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgUserInfo::CMsgUserInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgUserInfo::~CMsgUserInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_USERINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgUserInfo::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgUserInfo, id:%u", m_pInfo->id);
#endif
    char szName[_MAX_NAMESIZE]		= "";
    char szMate[_MAX_NAMESIZE]		= "";
    m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
    m_StrPacker.GetString(1, szMate, _MAX_NAMESIZE);
    g_objHero.SetName(szName);
    g_objHero.SetMate(szMate);
    g_objHero.SetID(m_pInfo->id);
    //g_objHero.SetLife(m_pInfo->usLife);
    g_objHero.SetData(CPlayer::_PLAYER_LIFE, m_pInfo->usLife);
    g_objHero.SetMaxLife(m_pInfo->usMaxLife);
    g_objHero.SetData(CPlayer::_PLAYER_MANA, m_pInfo->usMana);
    g_objHero.SetForce(m_pInfo->usForce);
    g_objHero.SetConstitution(m_pInfo->usConstitution);
    g_objHero.SetSpeedRate(m_pInfo->usSpeed);
    g_objHero.SetSpeed(m_pInfo->usDexterity);
    g_objHero.SetHealth(m_pInfo->usHealth);
    g_objHero.SetSoul(m_pInfo->usSoul);
    g_objHero.SetExp(m_pInfo->nExp);
    //g_objHero.SetDeed(m_pInfo->lDeed);
    //	g_objHero.CPlayer::SetMedalSelect(m_pInfo->ucMedalSelected);
    g_objHero.SetMoney(m_pInfo->dwMoney);
    //	g_objHero.m_dwMoneySaved=m_pInfo->dwMoneySaved;
    g_objHero.Create(m_pInfo->dwLookFace % 1000);
    g_objHero.SetFace((m_pInfo->dwLookFace / 10000) % 1000);
    g_objHero.SetSex(m_pInfo->dwLookFace % 1000);
    g_objHero.Transform(m_pInfo->dwLookFace / 10000000);
    g_objHero.SetHair(m_pInfo->usHair);
    g_objHero.SetAutoAllot((DWORD)m_pInfo->ucAutoAllot);
    g_objHero.SetLev(m_pInfo->ucLevel);
    g_objHero.SetProfession(m_pInfo->ucProfession);
    g_objHero.SetAddPoint(m_pInfo->usAdditionalPoint);
    g_objHero.m_dwNobility = (DWORD)m_pInfo->ucNobility;
    g_objHero.m_dwMetempsychosis = (DWORD)m_pInfo->ucMetempsychosis;
    g_objHero.SetPk(m_pInfo->sPk);
    // now the Hero name is ok...
    g_objGameMsg.m_objChatLog.SearchFile();
    //	g_objGameMsg.m_objChatLog.LoadLeaveWordLog();
    //	g_objHero.SetVirtue((DWORD)m_pInfo->dwVirtue);
    //	g_objHero.CPlayer::SetSelectedHonorTitle(m_pInfo->ucHonorTitleSelected);
    //	g_objHero.SetNobilityRank((m_pInfo->dwLookFace/1000000)%10);
    g_objHero.SetNobilityRank(m_pInfo->ucNobilityRank);
    g_objHero.SetExploit(m_pInfo->dwExploit);
    g_objHero.SetMercenaryExp(m_pInfo->dwMercenaryExp);
    g_objHero.SetMercenaryLevel(m_pInfo->ucMercenaryLevel);
    g_objHero.SetTutorExp(m_pInfo->dwTutorExp);
    g_objHero.SetTutorLevel(m_pInfo->ucTutorLevel);
    g_objHero.SetDexterity(m_pInfo->usDexterity);
    g_objHero.SetPotential(m_pInfo->nPotential);
    g_objHero.SetMaxPetCall(m_pInfo->usMaxEudemon);
    // enter map now
    CMsgAction msg;
    MYASSERT (msg.Create(g_objHero.GetID(), 0, 0, 0, actionEnterMap));
    msg.Send();
    ::PostCmd(CMD_FLASH_MAGIC);
    ::PostCmd(CMD_FLASHEQUIPMENT);
}
