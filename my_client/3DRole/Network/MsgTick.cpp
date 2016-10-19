
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTick.cpp: implementation of the CMsgTick class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "GameMsg.h"
#include "Hero.h"
#include "GameDataSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgTick::CMsgTick()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTick::~CMsgTick()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTick::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TICK != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTick::Process(void* pInfo)
{
    static DWORD dwLastTick	= 0;
    DWORD dwTimeNow		= ::TimeGet();
    m_pInfo->dwData		= dwTimeNow ^ (m_pInfo->idPlayer * m_pInfo->idPlayer + 9527);
    m_pInfo->dwChkData	= g_objNetwork.m_dwMsgCount;
    this->Send();	// return back
    //g_objNetwork.ChangeCode(m_pInfo->dwData);
    //::LogMsg("Code is %u", m_pInfo->dwData);
    //::LogMsg("Tick feed back");
    const DWORD INTERVAL_TICK = 10 * 1000;
    if (dwLastTick != 0 && dwTimeNow > dwLastTick + INTERVAL_TICK*(100 + 30) / 100)
    {
        const OBJID idNetBlock = 100115;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idNetBlock));
    }
    dwLastTick = dwTimeNow;
}

