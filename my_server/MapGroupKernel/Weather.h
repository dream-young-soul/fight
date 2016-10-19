
//**********************************************************
// 代码编辑器
//**********************************************************

// 天气系统
// 仙剑修，2002.2.22
//////////////////////////////////////////////////////////////////////

#ifndef	WEATHER_H
#define WEATHER_H

#include <windows.h>
#include "Define.h"
#include "TimeOut.h"

//////////////////////////////////////////////////////////////////////

enum { WEATHER_NONE = 0, WEATHER_FINE,
       WEATHER_RAINY, WEATHER_SNOWY, WEATHER_SANDS,
       WEATHER_LEAF, WEATHER_BAMBOO, WEATHER_FLOWER,
       WEATHER_FLYING, WEATHER_DANDELION, WEATHER_WORM, WEATHER_CLOUDY,
       WEATHER_ALL
     };

const int	MAX_WEATHER_INTENSITY	= 1000;
const int	MAX_WEATHER_DIR			= 360;
const int	WEATHER_NORMAL_SPEED		= 10;
const int	WEATHER_FAST_SPEED			= 5;


//////////////////////////////////////////////////////////////////////
#ifdef	PALED_DEBUG
const int	MAX_KEEP_SECS				= 60;
const int	MIN_KEEP_SECS				= 60;
const int	WERTHER_FINE_PERCENT		= 50;
const int	WERTHER_CHANGE_DIR_PERCENT	= 5;

const int	WEATHER_RAINY_INTENSITY		= MAX_WEATHER_INTENSITY / 5;
const int	WEATHER_SNOWY_INTENSITY		= MAX_WEATHER_INTENSITY / 10;
const int	WEATHER_DEFAULT_INTENSITY	= MAX_WEATHER_INTENSITY / 5;
const int	WEATHER_DEFAULT_DIR			= 10;
const int	WEATHER_DEFAULT_COLOR		= 0x00FFFFFF;
const int	WEATHER_RAINY_SPEED			= WEATHER_NORMAL_SPEED;
#else
//////////////////////////////////////////////////////////////////////
//* 春
const int	MAX_KEEP_SECS				= 1200;
const int	MIN_KEEP_SECS				= 500;
const int	WERTHER_FINE_PERCENT		= 60;
const int	WERTHER_CHANGE_DIR_PERCENT	= 5;

const int	WEATHER_RAINY_INTENSITY		= MAX_WEATHER_INTENSITY / 5;
const int	WEATHER_SNOWY_INTENSITY		= MAX_WEATHER_INTENSITY / 10;
const int	WEATHER_DEFAULT_INTENSITY	= MAX_WEATHER_INTENSITY / 5;
const int	WEATHER_DEFAULT_DIR			= 10;
const int	WEATHER_DEFAULT_COLOR		= 0x00FFFFFF;
const int	WEATHER_RAINY_SPEED			= WEATHER_NORMAL_SPEED;
//*/
#endif
//////////////////////////////////////////////////////////////////////
/* 夏
const int	MAX_KEEP_SECS				= 300;
const int	MIN_KEEP_SECS				= 100;
const int	WERTHER_FINE_PERCENT		= 80;
const int	WERTHER_CHANGE_DIR_PERCENT	= 20;

const int	WEATHER_RAINY_INTENSITY		= MAX_WEATHER_INTENSITY - 1;		// 不能 == MAX_WEATHER_INTENSITY
const int	WEATHER_SNOWY_INTENSITY		= MAX_WEATHER_INTENSITY / 10;
const int	WEATHER_DEFAULT_INTENSITY	= MAX_WEATHER_INTENSITY / 5;
const int	WEATHER_DEFAULT_DIR			= 10;
const int	WEATHER_DEFAULT_COLOR		= 0x00FFFFFF;
const int	WEATHER_RAINY_SPEED			= WEATHER_FAST_SPEED;
//*/

//////////////////////////////////////////////////////////////////////
/* 秋
const int	MAX_KEEP_SECS				= 300;
const int	MIN_KEEP_SECS				= 100;
const int	WERTHER_FINE_PERCENT		= 80;
const int	WERTHER_CHANGE_DIR_PERCENT	= 20;

const int	WEATHER_RAINY_INTENSITY		= MAX_WEATHER_INTENSITY / 5;
const int	WEATHER_SNOWY_INTENSITY		= MAX_WEATHER_INTENSITY / 10;
const int	WEATHER_DEFAULT_INTENSITY	= MAX_WEATHER_INTENSITY / 2;
const int	WEATHER_DEFAULT_DIR			= 10;
const int	WEATHER_DEFAULT_COLOR		= 0x00FFFFFF;
const int	WEATHER_RAINY_SPEED			= WEATHER_NORMAL_SPEED;
//*/

//////////////////////////////////////////////////////////////////////
/* 冬
const int	MAX_KEEP_SECS				= 1200;
const int	MIN_KEEP_SECS				= 500;
const int	WERTHER_FINE_PERCENT		= 80;
const int	WERTHER_CHANGE_DIR_PERCENT	= 10;

const int	WEATHER_RAINY_INTENSITY		= MAX_WEATHER_INTENSITY / 5;
const int	WEATHER_SNOWY_INTENSITY		= MAX_WEATHER_INTENSITY - 1;
const int	WEATHER_DEFAULT_INTENSITY	= MAX_WEATHER_INTENSITY / 10;
const int	WEATHER_DEFAULT_DIR			= 10;
const int	WEATHER_DEFAULT_COLOR		= 0x00FFFFFF;
const int	WEATHER_RAINY_SPEED			= WEATHER_NORMAL_SPEED;
//*/

//////////////////////////////////////////////////////////////////////
class	IWeatherOwner
{
public:
    virtual void	BroadcastMsg(CNetMsg* pMsg, CUser* pExclude = NULL)			PURE_VIRTUAL_FUNCTION
};

//////////////////////////////////////////////////////////////////////
class CUser;
class CWeather
{
public:
    CWeather(IWeatherOwner*	pOwner);
    virtual ~CWeather();
public:	// 修改
    bool	Create(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nSpeedSecs);
    bool	SetNewWeather(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nKeepSecs, DWORD nSpeedSecs);
    bool	SendWeather(CUser* pUser);
    static bool	SendNoWeather(CUser* pUser);
    void	OnTimer();
    int		GetParticle(int nIntensity);

public:	// 只读
    int		GetType()		{ return m_nCurrType; }
    int		GetParticle()	{ return m_nCurrParticle; }
    int		GetDir()		{ return m_nCurrDir; }
    DWORD	GetColor()		{ return m_nCurrColor; }

protected: // 成员变量
    IWeatherOwner*	m_pOwner;

    int		m_nDefaultType;
    int		m_nDefaultIntensity;
    int		m_nDefaultDir;
    int		m_nDefaultColor;
    int		m_nDefaultSpeedSecs;

    int		m_nCurrType;
    int		m_nCurrIntensity;
    int		m_nCurrDir;
    int 	m_nCurrColor;
    int		m_nCurrParticle;

    int		m_nTargType;
    int		m_nTargIntensity;
    int		m_nTargDir;
    int		m_nTargColor;

    int		m_nIncrIntensity;
    int		m_nKeepSecs;				// 天气效果(非天晴)保持的秒数。为0则不自动改变
    int 	m_nSpeedSecs;

    CTimeOut	m_tLoop;

public:
    MYHEAP_DECLARATION(s_heap)
};

#endif // WEATHER_H






