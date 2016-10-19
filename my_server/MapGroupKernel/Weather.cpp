
//**********************************************************
// 代码编辑器
//**********************************************************

// 天气系统
// 仙剑修，2002.2.22
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "AllMsg.h"
#include "GameMap.h"
#include "MsgWeather.h"
#include "Weather.h"
#include "math.h"

MYHEAP_IMPLEMENTATION(CWeather, s_heap)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWeather
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWeather::CWeather(IWeatherOwner* pOwner) : m_pOwner(pOwner)
{
    //	DWORD	m_nTypeMask;
    //	int		m_nMaxIntensity;
    //	int		m_nMinIntensity;
    m_nDefaultType		= WEATHER_NONE;
    m_nDefaultIntensity	= 0;
    m_nDefaultDir		= 0;
    m_nDefaultColor		= 0x888888;
    m_nDefaultSpeedSecs	= 0;
    m_nCurrType			= 0;
    m_nCurrIntensity	= 0;
    m_nCurrDir			= 0;
    m_nCurrColor		= 0;
    m_nCurrParticle		= 0;
    m_nTargType			= 0;
    m_nTargIntensity	= 0;
    m_nTargDir			= 0;
    m_nTargColor		= 0;
    m_nIncrIntensity	= 0;
    m_nKeepSecs			= 0;
    m_nSpeedSecs		= 0;
}

//////////////////////////////////////////////////////////////////////
CWeather::~CWeather()
{
}

//////////////////////////////////////////////////////////////////////
// 修改
//////////////////////////////////////////////////////////////////////
bool CWeather::Create(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nSpeedSecs)
{
    ASSERT(nType > WEATHER_NONE && nType < WEATHER_ALL);
    ASSERT(nIntensity >= 0 && nIntensity < MAX_WEATHER_INTENSITY);
    ASSERT(nDir >= 0 && nDir < MAX_WEATHER_DIR);
    if (nType == WEATHER_FINE || nIntensity == 0)
    {
        nType		= WEATHER_FINE;
        nIntensity	= 0;
    }
    m_nDefaultType		= nType;
    m_nDefaultIntensity	= nIntensity;
    m_nDefaultDir		= nDir;
    m_nDefaultColor		= nColor;
    m_nDefaultSpeedSecs	= nSpeedSecs;
    m_nCurrType			= nType;
    m_nCurrIntensity	= nIntensity;
    m_nCurrDir			= nDir;
    m_nCurrColor		= nColor;
    m_nCurrParticle		= GetParticle(m_nCurrIntensity);
    m_nTargType			= nType;
    m_nTargIntensity	= nIntensity;
    m_nTargDir			= nDir;
    m_nTargColor		= nColor;
    m_nIncrIntensity	= 0;
    m_nKeepSecs			= ::RandGet(MAX_KEEP_SECS - MIN_KEEP_SECS + 1) + MIN_KEEP_SECS;
    if (nType == WEATHER_CLOUDY)
    {
        m_nKeepSecs	*= 5;
    }
    m_nSpeedSecs		= WEATHER_NORMAL_SPEED;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWeather::SetNewWeather(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nKeepSecs, DWORD nSpeedSecs)
{
    // 参数检查
    ASSERT(nType > WEATHER_NONE && nType < WEATHER_ALL);
    ASSERT(nIntensity >= 0 && nIntensity < MAX_WEATHER_INTENSITY);
    ASSERT(nDir >= 0 && nDir < MAX_WEATHER_DIR);
    if (nType == WEATHER_FINE || nIntensity == 0)
    {
        nType		= WEATHER_FINE;
        nIntensity	= 0;
    }
    if (nKeepSecs == 0)
    {
        m_nDefaultType		= nType;
        m_nDefaultIntensity	= nIntensity;
        m_nDefaultDir		= nDir;
        m_nDefaultColor		= nColor;
        m_nDefaultSpeedSecs	= nSpeedSecs;
    }
    m_nTargType			= nType;
    m_nTargIntensity	= nIntensity;
    m_nTargDir			= nDir;
    m_nTargColor		= nColor;
    if (m_nCurrType == WEATHER_FINE || m_nTargType == WEATHER_FINE
            || m_nCurrType == m_nTargType && m_nCurrDir == m_nTargDir && m_nCurrColor == m_nTargColor)
    {
        m_nIncrIntensity	= m_nTargIntensity - m_nCurrIntensity;
    }
    else
    {
        m_nIncrIntensity	= (0 - m_nCurrIntensity);
    }
    if (nKeepSecs == 0)
    {
        m_nKeepSecs		= ::RandGet(MAX_KEEP_SECS - MIN_KEEP_SECS + 1) + MIN_KEEP_SECS;
        if (nType == WEATHER_CLOUDY)
        {
            m_nKeepSecs	*= 5;
        }
    }
    else
    {
        m_nKeepSecs		= nKeepSecs;
    }
    m_nSpeedSecs	= nSpeedSecs;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CWeather::OnTimer()
{
    if (!m_tLoop.ToNextTime(1))
    {
        return;
    }
    int nOldType		= m_nCurrType;
    int	nOldIntensity	= m_nCurrIntensity;
    int	nOldParticle	= m_nCurrParticle;
    if (m_nCurrType == m_nTargType && m_nTargType == m_nDefaultType && m_nDefaultType == WEATHER_FINE)
    {
        return;
    }
    if (m_nCurrType == WEATHER_FINE || m_nTargType == WEATHER_FINE
            || m_nCurrType == m_nTargType && m_nCurrDir == m_nTargDir && m_nCurrColor == m_nTargColor)
    {
        // 时间到，转为缺省天气
        if (m_nCurrType == m_nTargType && (m_nKeepSecs == 0 || --m_nKeepSecs <= 0))
        {
            int	nType		= (::RandGet(100) < WERTHER_FINE_PERCENT) ? WEATHER_FINE : m_nDefaultType;		//? 缺省为天晴时，会重复设置。可优化
            int	nIntensity	= m_nDefaultIntensity - MAX_WEATHER_INTENSITY / 4 + ::RandGet(MAX_WEATHER_INTENSITY / 2);
            if (nIntensity < 1)
            {
                nIntensity	= 1;    // 0为停止
            }
            else if (nIntensity >= MAX_WEATHER_INTENSITY)
            {
                nIntensity	= MAX_WEATHER_INTENSITY - 1;
            }
            //幻灵			int	nDir		= (::RandGet(100) < WERTHER_CHANGE_DIR_PERCENT) ? ::RandGet(MAX_WEATHER_DIR) : m_nTargDir;
            int	nDir		= (nType != nOldType && ::RandGet(100) < WERTHER_CHANGE_DIR_PERCENT) ? ::RandGet(MAX_WEATHER_DIR) : m_nTargDir;
            int	nKeepSecs	= ::RandGet(MAX_KEEP_SECS - MIN_KEEP_SECS + 1) + MIN_KEEP_SECS;
            if (nType == WEATHER_CLOUDY)
            {
                nKeepSecs	*= 5;
            }
            SetNewWeather(nType, nIntensity, nDir, m_nDefaultColor, nKeepSecs, m_nDefaultSpeedSecs);
            return;		// type不同，下次OnTimer()再发消息
        }
        // 天晴，转TARG天气
        if (m_nCurrType == WEATHER_FINE && m_nTargType != WEATHER_FINE)
        {
            m_nCurrType			= m_nTargType;
            m_nCurrIntensity	= 0;
            m_nCurrDir			= m_nTargDir;
            m_nCurrColor		= m_nTargColor;
            m_nIncrIntensity	= m_nTargIntensity - 0;
        }
        // 需要渐变
        if (m_nCurrType != WEATHER_FINE && m_nIncrIntensity && m_nCurrIntensity != m_nTargIntensity )
        {
            //幻灵			m_nCurrIntensity = (m_nCurrIntensity*m_nSpeedSecs + m_nIncrIntensity) / m_nSpeedSecs;
            m_nCurrIntensity = m_nTargIntensity;
            // 检查越界
            if (m_nIncrIntensity > 0 && m_nCurrIntensity > m_nTargIntensity
                    || m_nIncrIntensity < 0 && m_nCurrIntensity < m_nTargIntensity)
            {
                m_nCurrIntensity = m_nTargIntensity;
            }
        }
    }
    else	// return to fine
    {
        ASSERT(m_nIncrIntensity <= 0);
        if (m_nIncrIntensity && m_nCurrIntensity != 0 )			// 渐变
        {
            const int times = 1;	// 2: 类型不同，加快停
            //幻灵			m_nCurrIntensity = (m_nCurrIntensity*(m_nSpeedSecs/times) + m_nIncrIntensity) / (m_nSpeedSecs/times);
            m_nCurrIntensity = 0;
            // 检查越界
            if (m_nCurrIntensity < 0)
            {
                m_nCurrIntensity = 0;
            }
        }
    }
    // 转换为粒子数
    if (m_nCurrIntensity != nOldIntensity)
    {
        m_nCurrParticle = GetParticle(m_nCurrIntensity);
    }
    // 发送消息
    /*
    	if (m_nCurrType != WEATHER_FINE && m_nCurrType == m_nTargType
    						&& (m_nCurrDir != m_nTargDir || m_nCurrColor != m_nTargColor))
    	{
    		m_nCurrDir		= m_nTargDir;
    		m_nCurrColor	= m_nTargColor;

    		CMsgWeather	cMsg;
    		if (cMsg.Create(m_nCurrType, m_nCurrParticle, m_nCurrDir, m_nCurrColor))
    			m_pOwner->BroadcastMsg(&cMsg);
    	}
    	else
    //*/
    if (m_nCurrType != nOldType || m_nCurrParticle != nOldParticle)
    {
        CMsgWeather	cMsg;
        if (cMsg.Create(m_nCurrType, m_nCurrParticle, m_nCurrDir, m_nCurrColor))
        {
            m_pOwner->BroadcastMsg(&cMsg);
        }
#ifdef	PALED_DEBUG
        MSGBUF	szMsg;
        sprintf(szMsg, " 类型：%d，粒子数：%d，方向：%d，颜色：0x%06X",
                m_nCurrType, m_nCurrParticle, m_nCurrDir, m_nCurrColor);
        CMsgTalk	msg;
        msg.Create("[天气]", "大家", szMsg);
        m_pOwner->BroadcastMsg(&msg);
#endif
    }
    // 强度为0，转天晴
    if (m_nCurrType != WEATHER_FINE && m_nCurrIntensity == 0 && m_nIncrIntensity <= 0)
    {
        // 天晴了
        m_nCurrType			= WEATHER_FINE;
        m_nCurrIntensity	= 0;
        m_nCurrDir			= 0;
        m_nCurrColor		= 0;
        m_nCurrParticle		= 0;
        m_nIncrIntensity	= 0;
    }
}

int CWeather::GetParticle(int nIntensity)
{
    int nParticle;
    nParticle	= nIntensity + (int)sqrt((float)MAX_WEATHER_INTENSITY);
    if (nParticle >= MAX_WEATHER_INTENSITY)
    {
        nParticle = MAX_WEATHER_INTENSITY - 1;
    }
    nParticle = nParticle * nParticle / MAX_WEATHER_INTENSITY;
    // 针对每种效果调整粒子数
    switch(m_nCurrType)
    {
    case	WEATHER_FINE:
        break;
    case	WEATHER_RAINY:
        nParticle	= (nParticle + 1) / 2;
        break;
    case	WEATHER_SNOWY:
        nParticle	= (nParticle + 3) / 4;
        break;
    case	WEATHER_SANDS:
        nParticle	= (nParticle + 1) / 2;
        break;
    case	WEATHER_LEAF:
    case	WEATHER_BAMBOO:
    case	WEATHER_FLOWER:
    case	WEATHER_FLYING:
    case	WEATHER_DANDELION:
        nParticle	= (nParticle + 24) / 25;
        break;
    case	WEATHER_WORM:
        nParticle	= (nParticle + 29) / 30;
        break;
    case	WEATHER_CLOUDY:
        nParticle	= (nParticle + 33) / 34;			// max_particle is 30
        break;
    default:
        ASSERT(!"switch(nType)");
        nParticle = 0;
    }
    return nParticle;
}

bool CWeather::SendWeather(CUser* pUser)
{
    CHECKF(pUser);
    CMsgWeather	msg;
    IF_OK(msg.Create(GetType(), GetParticle(), GetDir(), GetColor()))
    pUser->SendMsg(&msg);
    return true;
}

bool CWeather::SendNoWeather(CUser* pUser)
{
    CHECKF(pUser);
    CMsgWeather	msg;
    IF_OK(msg.Create(WEATHER_FINE, 0, 0, 0))
    pUser->SendMsg(&msg);
    return true;
}






