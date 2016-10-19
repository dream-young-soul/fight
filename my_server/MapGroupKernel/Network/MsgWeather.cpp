
//**********************************************************
// 代码编辑器
//**********************************************************

// 天气系统
// 仙剑修，2002.2.22
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "MsgWeather.h"
#include "Weather.h"
#include "logfile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWeather::CMsgWeather()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgWeather::~CMsgWeather()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgWeather::Create (int nType, int nParticle, int nDir, DWORD nColor)
{
    // check param
    if (nType <= WEATHER_NONE || nType >= WEATHER_ALL
            || nParticle < 0 || nParticle >= MAX_WEATHER_INTENSITY
            || nDir < 0 || nDir >= MAX_WEATHER_DIR)
    {
        ASSERT(!"CMsgWeather::Create()");
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_WEATHER;
    m_pInfo->nType		= nType;
    m_pInfo->nIntensity	= nParticle;
    m_pInfo->nDir		= nDir;
    m_pInfo->dwColor	= nColor;
    return true;
}

BOOL	CMsgWeather::Create		(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_WEATHER != this->GetType())
    {
        return false;
    }
    return true;
}

void	CMsgWeather::Process		(CGameSocket* pSocket)
{
    // 检查消息格式
    ASSERT(!"CMsgWeather::Process()");
}










