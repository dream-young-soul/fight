
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// WeatherRegion.h: interface for the CWeatherRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEATHERREGION_H__41C09DAF_9327_4256_9638_2D7FB2CAF6F0__INCLUDED_)
#define AFX_WEATHERREGION_H__41C09DAF_9327_4256_9638_2D7FB2CAF6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weather.h"

class CWeatherRegion : IWeatherOwner
{
protected:
    CWeatherRegion();
    virtual ~CWeatherRegion();
public:
    static CWeatherRegion* CreateNew()		{ return new CWeatherRegion; }
    void	ReleaseByOwner()				{ delete this; }
    OBJID	GetID()							{ return m_pData->GetID(); }
    bool	Create(CRegionData* pData, PROCESS_ID idProcess);
    CWeather* QueryWeather()				{ CHECKF(m_pWeather); return m_pWeather; }
    CRegionData* QueryRegion()				{ CHECKF(m_pData); return m_pData; }

public: // interface
    virtual void	BroadcastMsg(CNetMsg* pMsg, CUser* pExclude = NULL);

protected:
    CWeather*		m_pWeather;
    CRegionData*	m_pData;

protected: // ctrl
    PROCESS_ID		m_idProcess;

    MYHEAP_DECLARATION(s_heap)
};

typedef	IGameObjSet<CWeatherRegion>		IWeatherSet;
typedef	CGameObjSet<CWeatherRegion>		CWeatherSet;

#endif // !defined(AFX_WEATHERREGION_H__41C09DAF_9327_4256_9638_2D7FB2CAF6F0__INCLUDED_)
