
//**********************************************************
// 代码编辑器
//**********************************************************

// Booth.cpp: implementation of the CBooth class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "Booth.h"
#include "User.h"
#include "MapGroup.h"
#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
const int	BOOTH_SIZE		= 18;
const int	BOOTH_LOOK		= 407;					// look for booth

MYHEAP_IMPLEMENTATION(CBooth, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBooth::CBooth()
{
    m_id			= ID_NONE;
    m_pMap			= NULL;
    m_bEnterMap		= false;
    m_szCryOut[0]	= 0;
}

//////////////////////////////////////////////////////////////////////
CBooth::~CBooth()
{
    if (m_pMap)
    {
        LeaveMap();
    }
    if (m_id != ID_NONE)
    {
        MapManager()->RecycleDynaNpcID(m_id);
    }
}

CBooth*	CBooth::CreateNew(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir)
{
    CBooth* pBooth = new CBooth;
    if (pBooth)
    {
        if (pBooth->Create(idProcess, pUser, nPosX, nPosY, nDir))
        {
            return pBooth;
        }
        else
        {
            delete pBooth;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CBooth::Create(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir)
{
    m_idProcess	= idProcess;
    m_pUser		= pUser;
    CHECKF(pUser);
    m_pMap		= m_pUser->GetMap();
    CHECKF(m_pMap);
    m_id		= MapManager()->SpawnNewNpcID();
    EnterMap(nPosX, nPosY, nDir);
    return true;
}

//////////////////////////////////////////////////////////////////////
// IMapThing
//////////////////////////////////////////////////////////////////////
void CBooth::SendShow(IRole* pRole)
{
    OBJID idType0 = ID_NONE;
    OBJID idType1 = ID_NONE;
    if (m_setGoods.size() > 0)
    {
        idType0 = m_setGoods[0].nType;
    }
    if (m_setGoods.size() > 1)
    {
        idType1 = m_setGoods[1].nType;
    }
    CMsgNpcInfoEx	msg;
    IF_OK(msg.Create(GetID(), idType0, idType1, _BOOTH_NPC, NPCSORT_NONE,
                     (BOOTH_LOOK / 10) * 10 + m_nDir, GetPosX(), GetPosY(), 0, 0, m_pUser->GetName()))	// 因为修改了npc消息，需要Length和Fat参数，这里暂时用0 -- zlong 2004-02-03
    pRole->SendMsg(&msg);
    if (strlen(m_szCryOut))
    {
        CMsgTalk msg;
        IF_OK_ (msg.Create(m_pUser->GetName(), "ALL", m_szCryOut, NULL, 0xffffff, _TXTATR_CRYOUT))
        pRole->SendMsg(&msg);
    }
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
bool CBooth::AddItem(OBJID idItem, int nMoney)
{
    CHECKF(idItem != ID_NONE && nMoney > 0);
    if (m_setGoods.size() >= BOOTH_SIZE)
    {
        m_pUser->SendSysMsg(STR_BOOTH_FULL);
        return false;
    }
    CItem* pItem = m_pUser->GetItem(idItem);
    if (!pItem)
    {
        return false;
    }
    // add by zlong 2003-11-24 ---- can not sell chest item
    if (pItem->IsChestItem() || !pItem->IsExchangeEnable())
    {
        m_pUser->SendSysMsg(STR_NOT_DISCARDABLE);
        return false;
    }
    if (pItem->IsEudemon())
    {
        m_pUser->CallBackEudemon(pItem->GetID());
        m_pUser->DetachEudemon(pItem);
    }
    GOODS_ST	info;
    info.idItem		= idItem;
    info.nType		= pItem->GetInt(ITEMDATA_TYPE);
    info.nMoney		= nMoney;
    for(int i = 0; i < m_setGoods.size(); i++)
    {
        if (m_setGoods[i].idItem == idItem)
        {
            return false;
        }
    }
    m_setGoods.push_back(info);
    // synchro
    int idx = m_setGoods.size() - 1;
    if (idx < 2)
    {
        CMsgItem	msg;
        IF_OK(msg.Create(m_setGoods[idx].idItem, ITEMACT_BOOTH_ADD, m_setGoods[idx].nMoney))
        m_pUser->BroadcastRoomMsg(&msg, EXCLUDE_SELF);		// need not feedback, feedback outside already.
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
int CBooth::GetCost(OBJID idItem)
{
    for(int i = 0; i < m_setGoods.size(); i++)
    {
        if (m_setGoods[i].idItem == idItem)
        {
            return m_setGoods[i].nMoney;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////
bool CBooth::DelItem(OBJID idItem)
{
    bool ret = false;
    for(int i = 0; i < m_setGoods.size(); i++)
    {
        if (m_setGoods[i].idItem == idItem)
        {
            m_setGoods.erase(m_setGoods.begin() + i);
            ret = true;
        }
    }
    // synchro msg
    CMsgItem	msg;
    IF_OK(msg.Create(idItem, ITEMACT_BOOTH_DEL, GetID()))
    m_pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    return ret;
}

//////////////////////////////////////////////////////////////////////
bool CBooth::BuyItem(CUser* pTarget, OBJID idItem)
{
    CHECKF(pTarget && idItem != ID_NONE);
    if (pTarget->GetID() == GetID())
    {
        return false;
    }
    int nMoney = GetCost(idItem);
    if (nMoney <= 0)		// no this goods
    {
        return false;
    }
    if (pTarget->GetMoney() < nMoney)
    {
        pTarget->SendSysMsg(STR_NOT_SO_MUCH_MONEY);
        return false;
    }
    CItem* pItem = m_pUser->GetItem(idItem);
    if (!pItem)
    {
        pTarget->SendSysMsg(STR_TRADE_FAIL);
        return false;
    }
    if (pTarget->IsItemFull(pItem->GetWeight(), pItem->GetInt(ITEMDATA_TYPE), pItem->GetInt(ITEMDATA_AMOUNTLIMIT)))
    {
        pTarget->SendSysMsg(STR_BAG_FULL);
        return false;
    }
    DEBUG_TRY		// VVVVVVVVVVVVVVVVVVVVV
    ASSERT(DelItem(idItem));
    pItem->WarGhostLevelReset();
    ASSERT(m_pUser->PopItem(idItem, SYNCHRO_TRUE, UPDATE_TRUE));
    ASSERT(pTarget->AddItem(pItem, SYNCHRO_TRUE, UPDATE_TRUE));
    ASSERT(pTarget->SpendMoney(nMoney, SYNCHRO_TRUE));
    ASSERT(m_pUser->GainMoney(nMoney, SYNCHRO_TRUE));
    // save
    m_pUser->SaveInfo();
    pTarget->SaveInfo();
    // log
    if (pItem->IsNonsuchItem())
    {
        ::MyLogSave("gmlog/booth_item", "%s(%u) sell item:[id=%u, type=%u], dur=%d, max_dur=%d to %s(%u) cost money(%d)",
                    m_pUser->GetName(),
                    m_pUser->GetID(),
                    pItem->GetID(),
                    pItem->GetInt(ITEMDATA_TYPE),
                    pItem->GetInt(ITEMDATA_AMOUNT),
                    pItem->GetInt(ITEMDATA_AMOUNTLIMIT),
                    pTarget->GetName(),
                    pTarget->GetID(),
                    nMoney);
    }
    m_pUser->SendSysMsg(STR_BOOTH_BUY_sds, pTarget->GetName(), nMoney, pItem->GetStr(ITEMDATA_NAME));
    DEBUG_CATCH("CBooth::BuyItem()")		// VVVVVVVVVVVVVVVVVVVVV
    return true;
}

//////////////////////////////////////////////////////////////////////
void CBooth::SendGoods(CUser* pTarget)
{
    CHECK(pTarget);
    for(int i = m_setGoods.size() - 1; i >= 0; i--)
    {
        OBJID	idItem	= m_setGoods[i].idItem;
        int		nCost	= m_setGoods[i].nMoney;
        CItem* pItem = m_pUser->GetItem(idItem);
        if (pItem)
        {
            CMsgItemInfoEx	msg;
            IF_OK(msg.Create(pItem, GetID(), nCost, ITEMINFOEX_BOOTH))
            pTarget->SendMsg(&msg);
        }
        else
        {
            DelItem(idItem);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CBooth::EnterMap(int nPosX, int nPosY, int nDir)
{
    if (m_bEnterMap)
    {
        return ;
    }
    // overlap booth
    void* pTemp;
    if (m_pUser->GetMap()->QueryObjInPos(nPosX, nPosY, OBJ_BOOTH, IPP_OF(pTemp)))
    {
        return ;
    }
    // enter map
    m_nPosX		= nPosX;
    m_nPosY		= nPosY;
    m_nDir		= nDir;
    m_pMap->EnterRoom(QueryMapThing());
    // synchro
    CMsgNpcInfo	msg;
    // 因为修改了npc消息，需要Length和Fat参数，这里暂时用0 -- zlong 2004-02-03
    IF_OK(msg.Create(GetID(), _BOOTH_NPC, NPCSORT_NONE, (BOOTH_LOOK / 10) * 10 + m_nDir, GetPosX(), GetPosY(), 0, 0, m_pUser->GetName()))
    m_pMap->BroadcastBlockMsg(QueryMapThing(), &msg);
    if (strlen(m_szCryOut))
    {
        CMsgTalk msg;
        IF_OK_ (msg.Create(m_pUser->GetName(), ALLUSERS_NAME, m_szCryOut, NULL, 0xffffff, _TXTATR_CRYOUT))
        m_pMap->BroadcastBlockMsg(QueryMapThing(), &msg);
    }
    m_bEnterMap	= true;
}

//////////////////////////////////////////////////////////////////////
void CBooth::LeaveMap()
{
    if (!m_bEnterMap)
    {
        return ;
    }
    CMsgNpc	msg;
    IF_OK(msg.Create(EVENT_LEAVEMAP, GetID()))
    m_pMap->BroadcastBlockMsg(QueryMapThing(), &msg);
    m_pMap->LeaveRoom(QueryMapThing());
    m_bEnterMap	= false;
}

//////////////////////////////////////////////////////////////////////
void CBooth::SendCryOut(CUser* pUser)
{
    if (strlen(m_szCryOut))
    {
        CMsgTalk msg;
        IF_OK_ (msg.Create(m_pUser->GetName(), "ALL", m_szCryOut, NULL, 0xffffff, _TXTATR_CRYOUT))
        pUser->SendMsg(&msg);
    }
}




