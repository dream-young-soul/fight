
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef MSGWEATHER_HEADER_KATTY
#define MSGWEATHER_HEADER_KATTY


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"


enum { WEATHER_NONE = 0, WEATHER_FINE,
       WEATHER_RAINY, WEATHER_SNOWY, WEATHER_SANDS,
       WEATHER_LEAF, WEATHER_NOUSED, WEATHER_FLOWER,
       WEATHER_FLY, WEATHER_DANDELION, WEATHER_WORM, WEATHER_CLOUD,
       WEATHER_ALL
     };

typedef struct
{
    unsigned short	unMsgSize;
    unsigned short	unMsgType;

    int 			nType;
    int				nIntensity;					// 1 - 1024
    int				nDir;						// 0 +- 60
    DWORD			dwColor;					// 0 - (MAX-1)

} MSG_WeatherInfo;



const int WEATHER_RESET = 1;
const int WEATHER_STOP	= 2;
const int WEATHER_CREATE = 3;
const int WEATHER_DESTROY = 4;



class CMsgWeather : public CNetMsg
{
public:
    CMsgWeather();
    virtual ~CMsgWeather();
    BOOL	ConvertInfoForm  (int nType, int nIntensity, int nDir, DWORD nColor);


public:
    virtual BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    virtual void			Process		(void* pInfo);

protected:
    MSG_WeatherInfo*	m_pInfo;
};

#endif

