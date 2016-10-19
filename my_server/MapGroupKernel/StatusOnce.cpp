
//**********************************************************
// 代码编辑器
//**********************************************************

// StatusOnce.cpp: implementation of the CStatusOnce class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "I_Role.h"
#include "TimeOut.h"
#include "myheap.h"
#include "StatusMore.h"
#include "StatusOnce.h"
#include "MapGroup.h"
#include "MsgUserAttrib.h"

MYHEAP_IMPLEMENTATION(CStatusOnce, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStatusOnce::CStatusOnce()
{
    m_pOwner	= NULL;
    m_nStatus	= 0;
}

//////////////////////////////////////////////////////////////////////
CStatusOnce::~CStatusOnce()
{
    if (m_pOwner)
    {
        m_pOwner->ClsStatus(m_nStatus);
    }
}

//////////////////////////////////////////////////////////////////////
int CStatusOnce::GetPower()
{
    if (m_nStatus >= STATUS_TEAM_BEGIN && m_nStatus <= STATUS_TEAM_END)
    {
        CUser* pUser = NULL;
        if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            CTeam* pTeam = pUser->GetTeam();
            if (pTeam)
            {
                // 组队成员为4则power增加10%，组队成员为5则power增加20%
                int nAddPercent = __max(0, (pTeam->GetMemberAmount() - TEAM_STATUS_REQ_ROLES) * 10);
                int nPower = m_nData * (100 + nAddPercent) / 100;
                return nPower;
            }
        }
    }
    return m_nData;
}

//////////////////////////////////////////////////////////////////////
bool CStatusOnce::GetInfo(StatusInfoStruct* pInfo)
{
    pInfo->nPower	= m_nData;
    pInfo->nStatus	= m_nStatus;
    pInfo->nSecs	= m_tKeep.GetRemain() / 1000;
    pInfo->nTimes	= 0;					// Status once
    return IsValid();
}

//////////////////////////////////////////////////////////////////////
bool CStatusOnce::ChangeData(int nPower, int nSecs, int nTimes)
{
    CHECKF(nSecs > 0 && nTimes == 0);
    //	if (nPower)
    //	{
    //		// 时间多则复盖
    //		if (nSecs*1000 >= m_tKeep.GetRemain())
    //		{
    m_nData		= nPower;
    m_tKeep.SetInterval(nSecs * 1000);
    m_tKeep.Update();
    //		}
    return true;
    //	}
    //	else // power == 0: 减时间
    //	{
    //		// 减时间
    //		m_tKeep.DecInterval(nSecs*1000);
    //	}
    //	return false;
}

//////////////////////////////////////////////////////////////////////
bool CStatusOnce::IncTime(int nMilliSecs, int nLimit)
{
    int nInterval = __min(nMilliSecs + m_tKeep.GetRemain(), nLimit);
    m_tKeep.SetInterval(nInterval);
    m_tKeep.Update();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CStatusOnce::ToFlash()
{
    if (!IsValid())
    {
        return false;
    }
    if (!m_bFlash && m_tKeep.GetRemain() <= STATUS_FLASH_REMAIN_MS)
    {
        m_bFlash = true;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CStatusOnce::OnTimer(DWORD tCurr)
{
    if (!IsValid())
    {
        return ;
    }
    if ((m_nStatus >= STATUS_TEAM_BEGIN && m_nStatus <= STATUS_TEAM_END || m_nStatus == STATUS_ADD_EXP)
            && m_pOwner && m_tInterval.ToNextTime())
    {
        CUser* pUser = NULL;
        m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser));
        if (pUser)
        {
            CTeam* pTeam = pUser->GetTeam();
            if (pTeam)
            {
                // if owner is team leader
                if (pTeam->GetLeader() == pUser->GetID())
                {
                    // dispatch magic status exclude self
                    StatusInfoStruct	info;
                    this->GetInfo(&info);
                    pTeam->AttachMemberStatus(info.nStatus, info.nPower, info.nSecs, info.nTimes, pUser->GetID());
                }
                // status effects
                switch (m_nStatus)
                {
                case	STATUS_TEAMHEALTH:
                    {
                        if (pUser->GetLife() < pUser->GetMaxLife())
                        {
                            pUser->AddAttrib(_USERATTRIB_LIFE, this->GetPower(), SYNCHRO_TRUE);
                        }
                        if (pUser->GetMana() < pUser->GetMaxMana())
                        {
                            pUser->AddAttrib(_USERATTRIB_MANA, this->GetPower(), SYNCHRO_TRUE);
                        }
                        // 要不要广播魔法效果？
                    }
                    break;
                }
            }
        } // end of if (pUser)
    }
}
