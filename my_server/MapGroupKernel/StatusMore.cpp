
//**********************************************************
// 代码编辑器
//**********************************************************

// StatusOnce.cpp: implementation of the CStatusMore class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "windows.h"
#include "define.h"
#include "I_Role.h"
#include "TimeOut.h"
#include "StatusMore.h"
#include "magic.h"

const int	MAX_POISON_LOSE_LIFE			= 200;

MYHEAP_IMPLEMENTATION(CStatusMore, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStatusMore::CStatusMore()
{
    m_pOwner	= NULL;
    m_nStatus	= 0;
}

//////////////////////////////////////////////////////////////////////
CStatusMore::~CStatusMore()
{
    if (m_pOwner)
    {
        m_pOwner->ClsStatus(m_nStatus);
    }
}

//////////////////////////////////////////////////////////////////////
int CStatusMore::GetPower()
{
    if (m_nStatus >= STATUS_TEAM_BEGIN && m_nStatus <= STATUS_TEAM_END)
    {
        CUser* pUser = NULL;
        if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            CTeam* pTeam = pUser->GetTeam();
            if (pTeam)
            {
                int nAddPercent = __min(0, (pTeam->GetMemberAmount() - 3) * 10);
                int nPower = m_nData * (100 + nAddPercent) / 100;
                return nPower;
            }
        }
    }
    return m_nData;
}

//////////////////////////////////////////////////////////////////////
bool CStatusMore::GetInfo(StatusInfoStruct* pInfo)
{
    pInfo->nPower	= m_nData;
    pInfo->nStatus	= m_nStatus;
    pInfo->nSecs	= m_tKeep.GetInterval();
    pInfo->nTimes	= m_nTimes;			//? mast >0
    return IsValid();		// m_nTimes > 0
}

//////////////////////////////////////////////////////////////////////
void CStatusMore::OnTimer(DWORD tCurr)
{
    if (!IsValid())
    {
        return ;
    }
    if (!m_tKeep.ToNextTime())
    {
        return ;
    }
    m_nTimes--;
    if (m_nStatus == STATUS_POISON)
    {
        if (m_pOwner->IsAlive())
        {
            int nLoseLife = ::CutOverflow(m_nData, (int)m_pOwner->GetLife() - 1);		// 剩余1点血
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1 * nLoseLife, SYNCHRO_TRUE);
            // effect
            CMsgMagicEffect	msg;
            IF_OK(msg.Create(m_pOwner->GetID(), POISON_MAGIC_TYPE, 0, m_pOwner->GetID(), nLoseLife, m_pOwner->GetDir()))
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
            if (!m_pOwner->IsAlive())
            {
                m_pOwner->BeKill(NULL);    // 不算PK
            }
        }
    }
    else if (m_nStatus == STATUS_HEAL)
    {
        if (m_pOwner->IsAlive())
        {
            int nAddLife = ::CutOverflow(m_nData, (int)m_pOwner->GetMaxLife() - (int)m_pOwner->GetLife());
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
            // effect
            //			CMsgMagicEffect	msg;
            //			IF_OK(msg.Create(m_pOwner->GetID(), POISON_MAGIC_TYPE, 0, m_pOwner->GetID(), nLoseLife, m_pOwner->GetDir()))
            //				m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CStatusMore::ChangeData(int nPower, int nSecs, int nTimes)
{
    CHECKF(nSecs > 0 && nTimes > 0);
    //	if (nPower)
    //	{
    //		// 次数多则复盖
    //		if (nTimes > m_nTimes)
    //		{
    m_nData		= nPower;
    m_tKeep.SetInterval(nSecs);
    m_nTimes	= nTimes;
    //			m_idPoisonMe	= idRole;
    //		}
    return true;
    //	}
    //	else // power == 0: 减次数
    //	{
    //		// 减时间
    //		m_nTimes = ::CutTrail(m_nTimes - nTimes, 0);
    //	}
    //	return false;
}

//////////////////////////////////////////////////////////////////////
bool CStatusMore::ToFlash()
{
    if (!IsValid())
    {
        return false;
    }
    if (!m_bFlash && m_nTimes <= (STATUS_FLASH_REMAIN_MS / 1000) / m_tKeep.GetInterval())
    {
        m_bFlash = true;
        return true;
    }
    return false;
}







