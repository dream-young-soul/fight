
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAuction.cpp: implementation of the CMsgAuction class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgAuction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAuction::CMsgAuction()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->name, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgAuction::~CMsgAuction()
{
}

BOOL CMsgAuction::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_AUCTION != this->GetType())
    {
        return false;
    }
    return true;
}

BOOL CMsgAuction::Create(OBJID idUser, OBJID idItem, int nAction)
{
    this->Init();
    m_pInfo->unMsgSize = sizeof(MSG_Info);
    m_pInfo->unMsgType = _MSG_AUCTION;
    m_pInfo->id = idUser;
    m_pInfo->idItem = idItem;
    m_pInfo->nAuction = nAction;
    return true;
}

BOOL CMsgAuction::Create(OBJID idUser, OBJID idItem, int dwValue, int nAction)
{
    this->Init();
    m_pInfo->unMsgSize = sizeof(MSG_Info);
    m_pInfo->unMsgType = _MSG_AUCTION;
    m_pInfo->id		= idUser;
    m_pInfo->idItem = idItem;
    m_pInfo->dwValue = dwValue;
    m_pInfo->nAuction = nAction;
    return true;
}

void CMsgAuction::Process(void* pInfo)
{
    switch(m_pInfo->nAuction)
    {
    case AUCTION_NEW:
        {
            char szName[_MAX_NAMESIZE];
            if (m_StrPacker.GetStrAmount() > 0)
            {
                m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
            }
            g_objHero.SetAuctionOwnerName(szName);
            g_objHero.SetAuctionItemId(m_pInfo->idItem);
            g_objHero.SetAuctionItemMoney(m_pInfo->dwValue);
            ::PostCmd(CMD_ACTION_NEWAUCTION);
        }
        break;
    case AUCTION_HINT_USER:
        {
            char szName[_MAX_NAMESIZE];
            if (m_StrPacker.GetStrAmount() > 0)
            {
                m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
            }
            g_objHero.SetAuctionName(szName);
            g_objHero.SetAuctionItemId(m_pInfo->idItem);
            g_objHero.SetAuctionItemMoney(m_pInfo->dwValue);
            ::PostCmd(CMD_ACTION_FLASHAUCTION);
        }
        break;
    case AUCTION_HAMMER:
        {
            g_objHero.DelAuctionItem ();
            ::PostCmd(CMD_ACTION_OVERAUCTION);
        }
        break;
    default:
        break;
    }
}
