
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTrade.cpp: implementation of the CMsgTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTrade::CMsgTrade()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTrade::~CMsgTrade()
{
}

//---------------------------------------------------------------------
BOOL CMsgTrade::Create(OBJID idPlayer, unsigned short usAction)
{
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_TRADE;
    m_pInfo->id = idPlayer;
    m_pInfo->usAction = usAction;
    return true;
}
//---------------------------------------------------------------------
BOOL CMsgTrade::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TRADE != this->GetType())
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------
void CMsgTrade::Process(void* pInfo)
{
#ifdef _MSGDEBUG
::LogMsg(Process MsgTrade: Action % u, ID % u, m_pInfo->usAction, m_pInfo->id);
#endif
    switch(m_pInfo->usAction)
    {
    case _TRADE_APPLY:
        g_objHero.AddApply(m_pInfo->id);
        break;
    case _TRADE_OPEN:
        g_objHero.OpenTrade(m_pInfo->id);
        break;
    case _TRADE_SUCCESS:
        g_objHero.CloseTrade(true);
        break;
    case _TRADE_FALSE:
        g_objHero.CloseTrade(false);
        break;
    case _TRADE_PLAYERTOTALMONEY:
        g_objHero.SetPlayerTradeMoney(m_pInfo->dwData);
        break;
    case _TRADE_HEROTOTALMONEY:
        g_objHero.SetHeroTradeMoney(m_pInfo->dwData);
        break;
    case _TRADE_OK:
        g_objHero.TradePlayerAgree();
        break;
    case _TRADE_ADDITEM_FALSE:
        g_objHero.AddItemFalse(m_pInfo->id);
        break;
    default:
        break;
    }
    ::PostCmd(CMD_FLASHITEM, 0);
}
//---------------------------------------------------------------------
