
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAuction.cpp: implementation of the CMsgAuction class.
// 拍买系统信息相关类    －LW-   2004-11-26
//////////////////////////////////////////////////////////////////////

#include "MsgAuction.h"
#include "Auction.h"
#include "MapGroup.h"
#include "npc.h"
#include "MsgItemInfo.h"
//#include "MsgTalk.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAuction::CMsgAuction()
{
    this->Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szName, _MAX_MSGSIZE - sizeof(MSG_Info) + sizeof(OBJID));
}

CMsgAuction::~CMsgAuction()
{
}
BOOL CMsgAuction::Create(char* szName, OBJID idItem,  DWORD dwValue, int nAuction)
{
    this->Init();
    // fill info now
    m_unMsgType	= _MSG_AUCTION;
    m_pInfo->idItem = idItem;
    m_pInfo->dwValue = dwValue;
    m_pInfo->nAuction = nAuction;
    m_StrPacker.AddString(szName);
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(OBJID) + m_StrPacker.GetSize();
    return true;
}
BOOL CMsgAuction::Create(OBJID dwData, int dwValue, int nAuction)
{
    this->Init();
    // fill info now
    m_unMsgType	= _MSG_AUCTION;
    //	if (nAuction != AUCTION_SYS)
    {
        m_pInfo->dwData = dwData;
        m_pInfo->dwValue = dwValue;
    }
    m_pInfo->nAuction = nAuction;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
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
void CMsgAuction::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgAuction, id:%u", m_pInfo->idItem);
#endif
    CUserPtr pUser = UserManager()->GetUser(this);
    CNpc* pNpc;
    if (pUser)
    {
        switch(m_pInfo->nAuction)
        {
        case AUCTION_ADD://加入拍买物品
            {
                if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->idNpc, IPP_OF(pNpc)))
                {
                    return ;
                }
                if (!pNpc->IsAuctionNpc())
                {
                    return;
                }
                int count = pUser->GetAuctionPackageAmount(m_pInfo->idNpc);
                if (count < PACKAGE_AUCTION_LIMIT)
                {
                    CItem* pTemp = NULL;
                    for(int i = 0; i < pNpc->QueryAuction()->QueryPackage()->GetAmount(); i++)
                    {
                        pTemp = pNpc->QueryAuction()->QueryPackage()->GetItemByIndex(i);
                        if ( pTemp && pTemp->GetInt(ITEMDATA_PLAYERID) == pUser->GetID())
                            if (++count >= PACKAGE_AUCTION_LIMIT)
                            {
                                break;
                            }
                    }
                }
                if (count >= PACKAGE_AUCTION_LIMIT)
                {
                    pUser->SendSysMsg(STR_AUCTION_PACKAGE_FULL);
                    return ;
                }
                pNpc->QueryAuction()->JoinAuction(pUser->GetID(), m_pInfo->idItem , m_pInfo->dwValue, Database());
            }
            break;
        case AUCTION_BID://竞标出价
            {
                if (pUser->GetMoney() < m_pInfo->dwValue)
                {
                    pUser->SendSysMsg(STR_AUCTION_LESS_MONEY);
                    return ;
                }
                if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->idNpc, IPP_OF(pNpc)))
                {
                    return ;
                }
                if (!pNpc->IsAuctionNpc())
                {
                    return;
                }
                int count = pUser->GetAuctionPackageAmount(m_pInfo->idNpc);
                if (count < PACKAGE_AUCTION_LIMIT)
                {
                    CItem* pTemp = NULL;
                    for(int i = 0; i < pNpc->QueryAuction()->QueryPackage()->GetAmount(); i++)
                    {
                        pTemp = pNpc->QueryAuction()->QueryPackage()->GetItemByIndex(i);
                        if ( pTemp && pTemp->GetInt(ITEMDATA_PLAYERID) == pUser->GetID())
                            if (++count >= PACKAGE_AUCTION_LIMIT)
                            {
                                break;
                            }
                    }
                }
                if (count >= PACKAGE_AUCTION_LIMIT)
                {
                    pUser->SendSysMsg(STR_AUCTION_PACKAGE_FULL1);
                    return ;
                }
                pNpc->QueryAuction()->ShoutPrice(m_pInfo->idItem, pUser->GetID(), m_pInfo->dwValue);
            }
            break;
        case AUCTION_ITEM_INFO://查看物品的信息
            if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->idNpc, IPP_OF(pNpc)))
            {
                return ;
            }
            if (!pNpc->IsAuctionNpc())
            {
                return;
            }
            DEBUG_TRY
            pNpc->QueryAuction()->SendAuctionItemInfo(pUser->GetID(), m_pInfo->idItem);
            DEBUG_CATCH("AUCTION Get ItemInfo ERROR!");
            break;
        default:
            ASSERT(!"CMsgAuction");
        }
    }
}
