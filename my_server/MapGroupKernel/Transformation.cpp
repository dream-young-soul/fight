
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Transformation.cpp: implementation of the CTransformation class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "TimeOut.h"
#include "myheap.h"
#include "StatusOnce.h"
#include "MapGroup.h"
#include "Transformation.h"

MYHEAP_IMPLEMENTATION(CTransformation, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTransformation::CTransformation()
{
    m_pUser		= NULL;
    m_pMonster	= NULL;
    m_nLife		= 0;
}

//////////////////////////////////////////////////////////////////////
CTransformation::~CTransformation()
{
}

//////////////////////////////////////////////////////////////////////
bool CTransformation::Create(CUser* pUser, CNpcType* pMonster)
{
    CHECKF(pMonster && pMonster->GetInt(NPCTYPEDATA_LIFE) > 0);
    m_pMonster	= pMonster;
    m_pUser		= pUser;
    m_nLife		= ::CutTrail(1, MulDiv(m_pUser->GetLife(), GetMaxLife(), m_pUser->GetMaxLife()));
    return true;
}

//////////////////////////////////////////////////////////////////////
DWORD CTransformation::GetInterAtkRate		()
{
    int nRate = GetIntervalAtkRate();
    IStatus* pStatus = m_pUser->QueryStatus(STATUS_ATKSPEED);
    if (pStatus)
    {
        nRate	= ::CutTrail(0, CRole::AdjustDataEx(nRate, pStatus->GetPower()));
    }
    return nRate;
}




