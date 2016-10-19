
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Agent.cpp: implementation of the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "AllMsg.h"
#include "common.h"
#include "define.h"
#include "I_MAPDATA.h"
#include "I_Role.h"
#include "item.h"
#include "GameMap.h"
#include "NpcWorld.h"
#include "Agent.h"
#include "Array.h"

#include "AgentData.h"		// only this cpp include this file

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAgent::CAgent()
    : m_bLocked(false)
{
    m_nStep = STEP_NONE;
    m_lnkThis.Init(Cast<IRole>(this));
    m_nSizeAdd		= 0;
}

//////////////////////////////////////////////////////////////////////
CAgent::~CAgent()
{
}

//////////////////////////////////////////////////////////////////////
bool CAgent::Create(OBJID idAgent, int nTimeOfLife)
{
    m_info.id		= idAgent;
    m_nTimeOfLife	= nTimeOfLife;
    Array<String> setFunction;
    const const char* const* pStrSet = ::GetFactFuncStringSet();
    while(*pStrSet)
    {
        setFunction.Push(*(pStrSet++));
    }
    Array<String> setAction;
    pStrSet = ::GetActionStringSet();
    while(*pStrSet)
    {
        setAction.Push(*(pStrSet++));
    }
    CHECKF(m_pAiCenter->Create(this, NpcWorld()->GetDatabase(), setFunction, setAction, m_nTimeOfLife));
    CHECKF(m_pBody->Create(this));
    CHECKF(m_pSense->Create(this));
    CHECKF(m_pTeam->Create(this));
    //CHECKF(m_pItemPack->Create(this));
    CHECKF(m_pMove->Create(this));
    CHECKF(m_pFight->Create(this));
    ASSERT(m_nStep == STEP_NONE);
    m_nStep	= STEP_OFFLINE;
    LOGDUMP("Agent Create() OK");
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CAgent::AppendInfo(const UserInfoStruct& info)
{
    memcpy(&m_info, &info, sizeof(UserInfoStruct));
    // anwser
    CMsgAction	msg;
    IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionEnterMap))
    SendMsg(&msg);
    LOGDUMP("Agent AppendInfo() OK");
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CAgent::KickBack(int nTargX, int nTargY)
{
    IF_NOT(GetMap()->IsValidPoint(nTargX, nTargY))
    return false;
    //!	if (GetPosX() != nTargX || GetPosY() != nTargY)
    {
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), nTargX, nTargY, GetDir(), actionSynchro, nTargX, nTargY))
        SendMsg(&msg);
    }
    //	ClearPath();
    Cast<CRoleMove>(this)->SetPos(nTargX, nTargY);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CAgent::SynchroPos(int nSourX, int nSourY, int nTargX, int nTargY)
{
    if (GetPosX() != nSourX || GetPosY() != nSourY)
    {
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionSynchro, GetPosX(), GetPosY()))
        SendMsg(&msg);
    }
    else if (GetPosX() != nTargX || GetPosY() != nTargY)
    {
        IF_NOT(GetMap()->IsValidPoint(nTargX, nTargY))
        return false;
        //		ClearPath();
        Cast<CRoleMove>(this)->SetPos(nTargX, nTargY);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CAgent::OnTimer()
{
    if (m_nStep != STEP_LOGIN_OK || IsLocked() || !IsActive())
    {
        return ;
    }
    DEBUG_TRY
    m_pSense->OnTimer();
    DEBUG_CATCH("CSense::OnTimer()");
    DEBUG_TRY
    m_pAiCenter->OnTimer();
    DEBUG_CATCH("CAgent::OnTimer()");
    DEBUG_TRY
    m_pBody->OnTimer();
    m_pFight->OnTimer();
    m_pMove->OnTimer();
    DEBUG_CATCH("CAgent::OnTimer()");
}

//////////////////////////////////////////////////////////////////////
void CAgent::SetDie()
{
    m_pSense->Die();
}

//////////////////////////////////////////////////////////////////////
void CAgent::BeAttack(IRole* pRole)
{
    m_pSense->BeAttack(pRole);
}

//////////////////////////////////////////////////////////////////////
void CAgent::BeKill(IRole* pRole)
{
    m_pSense->BeKill(pRole);
}

//////////////////////////////////////////////////////////////////////
void CAgent::AttackOnce()
{
    m_pFight->AttackOnce();
}

/*/////////////////////////////////////////////////////////////////////
void CAgent::SetPos(int nPosX, int nPosY)
{
	m_info.usPosX	= nPosX;
	m_info.usPosY	= nPosY;
}*/

//////////////////////////////////////////////////////////////////////
void CAgent::SetLife(int nData)
{
    int nOldLife = m_info.usLife;
    m_info.usLife	= nData;
    if (nData > nOldLife && nData < GetMaxLife())
    {
        m_pSense->Recruit(nData - nOldLife);
    }
}

//////////////////////////////////////////////////////////////////////
void CAgent::SetPower(int nData)
{
    m_info.usMana	= nData;
}

//////////////////////////////////////////////////////////////////////
void CAgent::SetEffect(I64 i64Effect)
{
    DWORD	dwOldEffect	= m_info.i64Effect;
    m_info.i64Effect	= i64Effect;
    DWORD	dwEffectInc = i64Effect & (~dwOldEffect);
    DWORD	dwEffectDec = (~i64Effect) & dwOldEffect;
    if (dwEffectInc & KEEPEFFECT_DIE)
    {
        m_pSense->Die();
    }
    else if (dwEffectDec & KEEPEFFECT_DIE)
    {
        m_pSense->Reborn();
    }
}

//////////////////////////////////////////////////////////////////////
OBJID CAgent::GetMapID()									{ return m_pMove->GetMap()->GetID(); }

//////////////////////////////////////////////////////////////////////
bool CAgent::SendMsg(CNetMsg* pMsg)
{
    return NpcWorld()->SendMsg(GetID(), pMsg);
}

//////////////////////////////////////////////////////////////////////
bool CAgent::SendMsg(LPCTSTR szMsg)
{
    CHECKF(szMsg);
    CMsgTalk	msg;
    if (msg.Create(m_info.szName, ALLUSERS_NAME, szMsg))
    {
        return NpcWorld()->SendMsg(GetID(), &msg);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
// IAiCenterOwner
//////////////////////////////////////////////////////////////////////
int	 CAgent::Priority2Durable(int nPriority)
{
    IF_NOT(nPriority >= 0 && nPriority <= 100)
    return 100;
    return ::CutTrail(1, ::Priority2Durable(nPriority) * TICKS_PER_SEC);
}

//////////////////////////////////////////////////////////////////////
int CAgent::GetMaxLife()
{
    return m_info.usMaxLife;
}

void CAgent::PrivateTalkToMe(LPCTSTR szSender, LPCTSTR szWords, LPCTSTR szEmotion)
{
    TalkToMe(szSender, szWords, szEmotion);
}

void CAgent::TalkToMe(LPCTSTR szSender, LPCTSTR szWords, LPCTSTR szEmotion)
{
    int		nPriority	= 50;
    String	strFact = FACT_TALK_TO_ME;			// "TalkToMe"				// <name>(str)		// 50
    strFact	+= "<\"";
    strFact	+= szSender;
    strFact	+= "\">(\"";
    strFact	+= szWords;
    strFact	+= "\")";
    IAiCenter* pAi = Cast<IAiCenter>(this);
    pAi->AddFact(strFact, nPriority);
}

bool CAgent::Login()		// send born msg to game server
{
    DWORD dwVersion = 0;
    // anwser
    CMsgLogin	msg;
    IF_OK_(msg.Create(dwVersion, "", ""))
    SendMsg(&msg);
    ASSERT(m_nStep == STEP_OFFLINE);
    m_nStep	= STEP_LOGIN;
    LOGDUMP("Agent Login() OK");
    return true;
}
