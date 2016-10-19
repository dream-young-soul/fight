
//**********************************************************
// 代码编辑器
//**********************************************************

// 天气系统
// 仙剑修，2002.2.22
//////////////////////////////////////////////////////////////////////

#ifndef	MSGWEATHER_H
#define MSGWEATHER_H

#include "NetMsg.h"

//////////////////////////////////////////////////////////////////////

class CMsgWeather : public CNetMsg
{
public:
    CMsgWeather();
    virtual ~CMsgWeather();

    BOOL	Create (int nType, int nIntensity, int nDir, DWORD nColor);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        int 			nType;
        int				nIntensity;					// 0 - 999
        int				nDir;						// 0 - 359
        DWORD			dwColor;					// 0 - (MAX-1)

    } MSG_Info;

    MSG_Info*	m_pInfo;
};



#endif // MSGWEATHER_H
