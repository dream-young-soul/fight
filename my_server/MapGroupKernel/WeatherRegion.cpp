
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// WeatherRegion.cpp: implementation of the CWeatherRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "GameMap.h"
#include "myheap.h"
#include "Weather.h"
#include "WeatherRegion.h"
#include "MapGroup.h"

MYHEAP_IMPLEMENTATION(CWeatherRegion, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWeatherRegion::CWeatherRegion()
{
    m_pWeather	= NULL;
    m_pData		= NULL;
}

//////////////////////////////////////////////////////////////////////
CWeatherRegion::~CWeatherRegion()
{
    if (m_pWeather)
    {
        delete m_pWeather;
    }
}

//////////////////////////////////////////////////////////////////////
bool CWeatherRegion::Create(CRegionData* pData, PROCESS_ID idProcess)
{
    m_idProcess	= idProcess;
    CHECKF(pData);
    m_pData		= pData;
    m_pWeather	= new CWeather((IWeatherOwner*)this);
    CHECKF(m_pWeather);
    if (!m_pWeather->Create(pData->GetInt(REGIONDATA_DATA0),
                           abs(pData->GetInt(REGIONDATA_DATA1)),
                           pData->GetInt(REGIONDATA_DATA2),
                           pData->GetInt(REGIONDATA_DATA3),
                           pData->GetInt(REGIONDATA_DATA1) > 0 ? WEATHER_NORMAL_SPEED : WEATHER_FAST_SPEED ))
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CWeatherRegion::BroadcastMsg(CNetMsg* pMsg, CUser* pExclude/*=NULL*/)
{
    bool	bSendNpcServer = true;
    CUserManager::Iterator pUser = UserManager()->NewEnum();
    while(pUser.Next())
    {
        if (pUser && (!pExclude || pUser->GetID() != pExclude->GetID()) && (!pUser->IsAgent() || bSendNpcServer))
        {
            if (CGameMap::IsInRegion(m_pData, pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY()))
            {
                pUser->SendMsg(pMsg);
            }
            if (pUser->IsAgent())
            {
                bSendNpcServer = false;
            }
        }
    }
}










