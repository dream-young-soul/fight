
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************


#include "AllMsg.h"
#include "MsgWeather.h"
#include "SkyLayer.h"
#include "3dgamemap.h"


const int MAX_WEATHER_INTENSITY	= 1023;
const int MAX_WEATHER_DIR		= 360;
CMsgWeather::CMsgWeather()
{
    Init();
    m_pInfo	= (MSG_WeatherInfo*)m_bufMsg;
}

CMsgWeather::~CMsgWeather()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgWeather::ConvertInfoForm (int nType, int nIntensity, int nDir, DWORD nColor)
{
    // check param
    if (nType <= WEATHER_NONE || nType >= WEATHER_ALL
            || nIntensity < 0 || nIntensity >= MAX_WEATHER_INTENSITY
            || nDir < 0 || nDir >= MAX_WEATHER_DIR)
    {
        return false;
    }
    this->Init();
    int iWeatherDir;
    if (nDir > 180)
    {
        iWeatherDir = (360 - nDir) * -1;
    }
    else
    {
        iWeatherDir = nDir;
    }
    int iType;
    switch(nType)
    {
    case	WEATHER_RAINY:
        iType	= 1;
        break;
    case WEATHER_SNOWY:
        iType	= 2 ;
        break;
    case	WEATHER_LEAF:
        iType	= 3 + nColor;
        break;
    case WEATHER_FLOWER:
        iType	= 10 + nColor;
        break;
    case WEATHER_SANDS:
        iType = 13;
        break;
    case WEATHER_FLY:
        iType = 14 + nColor;
        break;
    case WEATHER_DANDELION:
        iType = 18;
        break;
    case WEATHER_WORM:
        iType = 19;
        break;
    case WEATHER_CLOUD:
        iType = 20;
        break;
    case  WEATHER_FINE:
        iType	= 0;
        break;
    default:
        return false;
    }
    m_pInfo->unMsgSize	= sizeof(MSG_WeatherInfo);
    m_pInfo->unMsgType	= _MSG_WEATHER;
    m_pInfo->nType		= iType;// target type
    m_pInfo->nIntensity	= nIntensity;
    m_pInfo->nDir		= iWeatherDir; // target type
    m_pInfo->dwColor	= nColor;
    return true;
}

BOOL	CMsgWeather::Create		(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return FALSE;
    }
    if (_MSG_WEATHER != this->GetType())
    {
        return FALSE;
    }
    return TRUE;
}

extern CGameMap		g_objGameMap;

void	CMsgWeather::Process(void* pInfo)
{
    if (!ConvertInfoForm (m_pInfo->nType, m_pInfo->nIntensity, m_pInfo->nDir, m_pInfo->dwColor))
    {
        return ;
    }
    CSkyLayer* pSky = g_objGameMap.GetSkyLayer();
    int iCommand;
    int iNewType = m_pInfo->nType;
    if (iNewType == pSky->GetWeathType())
    {
        iCommand = WEATHER_RESET;
    }
    else if (iNewType == 0)
    {
        iCommand = WEATHER_DESTROY;
    }
    else
    {
        iCommand = WEATHER_CREATE;
    }
    switch(iCommand)
    {
    case WEATHER_RESET:
        if (m_pInfo->nIntensity == 0)
        {
            pSky->StopWeather();
        }
        else
        {
            pSky->SetWeather(m_pInfo->nDir, m_pInfo->nIntensity);
        }
        break;
    case WEATHER_DESTROY:
        pSky->DestroyWeather();
        break;
    case WEATHER_CREATE:
        pSky->CreateWeather(m_pInfo->nType, m_pInfo->nDir, m_pInfo->nIntensity, m_pInfo->dwColor);
        break;
    }
    return;
}















