
//**********************************************************
// 代码编辑器
//**********************************************************

// SquareDeal.cpp: implementation of the CSquareDeal class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "windows.h"
#include "SquareDeal.h"
#include "Item.h"
#include "User.h"
#include <algorithm>
using namespace std;

MYHEAP_IMPLEMENTATION(CSquareDeal, s_heap)
//////////////////////////////////////////////////////////////////////
// constant
const int	MAX_SQUAREDEALITEMS		= 20;
const int	MAX_SQUAREDEALMONEY		= 100000000;	// 100 m


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSquareDeal::CSquareDeal()
{
    m_iface1.m_pParent	= this;
    m_iface2.m_pParent	= this;
    m_pUser1			= NULL;
    m_pUser2			= NULL;
    //	m_setItem1;
    //	m_setItem2;
    m_nAllWeight1		= 0;
    m_nAllWeight2		= 0;
    m_nMoney1			= 0;
    m_nMoney2			= 0;
    m_bClickOK1			= false;
    m_bClickOK2			= false;
    m_bTradeSuccess		= false;
}

//////////////////////////////////////////////////////////////////////
CSquareDeal::~CSquareDeal()
{
    if (!m_bTradeSuccess)
    {
        CUser*	pUser1	= m_pUser1->QueryUser();
        CUser*	pUser2	= m_pUser2->QueryUser();
        IF_OK(pUser1 && pUser2)
        {
            CMsgTrade	msg;
            IF_OK(msg.Create(_TRADE_FALSE, 0))
            {
                pUser1->SendMsg(&msg);
                pUser2->SendMsg(&msg);
            }
            pUser1->SendSysMsg(STR_TRADE_FAIL);
            pUser2->SendSysMsg(STR_TRADE_FAIL);
        }
    }
    if (m_pUser1)
    {
        m_pUser1->Release();
    }
    if (m_pUser2)
    {
        m_pUser2->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::TradeOK()
{
    CHECKF(m_pUser1 && m_pUser2);
    CUser*	pUser1	= m_pUser1->QueryUser();
    CUser*	pUser2	= m_pUser2->QueryUser();
    IF_NOT(pUser1 && pUser2)
    return false;
    IF_NOT(pUser1->GetMapID() == pUser2->GetMapID())
    return false;
    IF_NOT(pUser1->GetDistance(pUser2->GetPosX(), pUser2->GetPosY()) <= CELLS_PER_VIEW)
    return false;
    // check money
    bool	bRet = true;
    if (	(m_nMoney1 && (m_nMoney1 > pUser1->GetMoney() || m_nMoney1 + pUser2->GetMoney() > _MAX_MONEYLIMIT))
            || (m_nMoney2 && (m_nMoney2 > pUser2->GetMoney() || m_nMoney2 + pUser1->GetMoney() > _MAX_MONEYLIMIT)) )
    {
        bRet = false;
    }
    // check items
    int	nAmountCount	= 0;
    int	nAmountWeight	= 0;
    int nCount[ITEMPOSITION_PACK_END - ITEMPOSITION_PACK_BEGIN];
    memset(&nCount, 0L, sizeof(nCount));
    IUserPackage* pUserPackage = pUser1->QueryPackage();
    if (!pUserPackage)
    {
        return false;
    }
    for(int i = 0; i < m_setItem1.size(); i++)
    {
        CItemPtr pItem = pUser1->GetItem(m_setItem1[i]);
        if (!pItem)
        {
            return false;
        }
        nAmountCount++;
        nAmountWeight	+= pItem->GetWeight();
        int nPosition = pUserPackage->GetItemPosition(pItem);
        if (nPosition < ITEMPOSITION_PACK_END && nPosition >= ITEMPOSITION_PACK_BEGIN)
        {
            ++nCount[nPosition];
        }
    }
    for (int i = 0; i < ITEMPOSITION_PACK_END - ITEMPOSITION_PACK_BEGIN; i++)
    {
        if (!pUser2->IsBackPackSpare(nCount[i], 0, ID_NONE, ITEMPOSITION_PACK_BEGIN + i))
        {
            return false;
        }
    }
    //	if (!pUser2->IsBackPackSpare(nAmountCount, nAmountWeight))
    //		bRet = false;
    // check items
    nAmountCount	= 0;
    nAmountWeight	= 0;
    memset(&nCount, 0L, sizeof(nCount));
    pUserPackage = pUser2->QueryPackage();
    if (!pUserPackage)
    {
        return false;
    }
    for(int i = 0; i < m_setItem2.size(); i++)
    {
        CItemPtr pItem = pUser2->GetItem(m_setItem2[i]);
        if (!pItem)
        {
            return false;
        }
        nAmountCount++;
        nAmountWeight	+= pItem->GetWeight();
        int nPosition = pUserPackage->GetItemPosition(pItem);
        if (nPosition < ITEMPOSITION_PACK_END && nPosition >= ITEMPOSITION_PACK_BEGIN)
        {
            ++nCount[nPosition];
        }
    }
    for (int i = 0; i < ITEMPOSITION_PACK_END - ITEMPOSITION_PACK_BEGIN; i++)
    {
        if (!pUser1->IsBackPackSpare(nCount[i], 0, ID_NONE, ITEMPOSITION_PACK_BEGIN + i))
        {
            return false;
        }
    }
    //	if (!pUser1->IsBackPackSpare(nAmountCount, nAmountWeight))
    //		bRet = false;
    if (bRet)
    {
        DEBUG_TRY		// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
        // exchange, 客户端自动同步
        if (m_nMoney1)
        {
            ASSERT(pUser1->SpendMoney(m_nMoney1, SYNCHRO_FALSE));
            ASSERT(pUser2->GainMoney(m_nMoney1, SYNCHRO_FALSE));
        }
        if (m_nMoney2)
        {
            ASSERT(pUser2->SpendMoney(m_nMoney2, SYNCHRO_FALSE));
            ASSERT(pUser1->GainMoney(m_nMoney2, SYNCHRO_FALSE));
        }
        for(int i = 0; i < m_setItem1.size(); i++)
        {
            CItemPtr pItem = pUser1->GetItem(m_setItem1[i]);
            ASSERT(pItem);
            if (pItem)
            {
                bool bSynchro = pItem->WarGhostLevelReset();
                // 不是夫妻交易幻兽要扣亲密度
                if (pItem->IsEudemon() && !pUser1->IsMate(pUser2))
                {
                    int nDecFidelity = EUDEMON_DEC_FIDELITY_WHEN_DEAL;
                    if (pUser1->GetFriend(pUser2->GetID()))
                    {
                        nDecFidelity /= 2;    // 好友交易少扣一半的亲密度
                    }
                    int nFidelity = pItem->GetInt(ITEMDATA_FIDELITY);
                    nFidelity = __max(0, (nFidelity - nDecFidelity));
                    pItem->SetInt(ITEMDATA_FIDELITY, nFidelity, true);
                    bSynchro = SYNCHRO_TRUE;
                }
                ASSERT(pUser1->PopItem(pItem->GetID(), SYNCHRO_FALSE, UPDATE_FALSE));
                ASSERT(pUser2->AddItem(pItem, bSynchro, UPDATE_TRUE));
                if (pItem->IsNonsuchItem())
                {
                    ::MyLogSave("gmlog/trade_item", STR_NONSUCH_TRADE_sssu, pUser1->GetName(), pUser2->GetName(), pItem->GetStr(ITEMDATA_NAME), pItem->GetID());
                }
            }
        }
        for(int i = 0; i < m_setItem2.size(); i++)
        {
            CItemPtr pItem = pUser2->GetItem(m_setItem2[i]);
            ASSERT(pItem);
            if (pItem)
            {
                bool bSynchro = pItem->WarGhostLevelReset();
                // 不是夫妻交易幻兽要扣亲密度
                if (pItem->IsEudemon() && !pUser2->IsMate(pUser1))
                {
                    int nDecFidelity = EUDEMON_DEC_FIDELITY_WHEN_DEAL;
                    if (pUser2->GetFriend(pUser1->GetID()))
                    {
                        nDecFidelity /= 2;    // 好友交易少扣一半的亲密度
                    }
                    int nFidelity = pItem->GetInt(ITEMDATA_FIDELITY);
                    nFidelity = __max(0, (nFidelity - nDecFidelity));
                    pItem->SetInt(ITEMDATA_FIDELITY, nFidelity, true);
                    bSynchro = SYNCHRO_TRUE;
                }
                ASSERT(pUser2->PopItem(pItem->GetID(), SYNCHRO_FALSE, UPDATE_FALSE));
                ASSERT(pUser1->AddItem(pItem, bSynchro, UPDATE_TRUE));
                if (pItem->IsNonsuchItem())
                {
                    ::MyLogSave("gmlog/trade_item", STR_NONSUCH_TRADE_sssu, pUser2->GetName(), pUser1->GetName(), pItem->GetStr(ITEMDATA_NAME), pItem->GetID());
                }
            }
        }
        pUser1->SendSysMsg(STR_TRADE_SUCCEED);
        pUser2->SendSysMsg(STR_TRADE_SUCCEED);
        DEBUG_CATCH("★公平交易出错★")		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        CMsgTrade	msg;
        CHECKF(msg.Create(_TRADE_SUCCESS, 0));
        pUser1->SendMsg(&msg);
        pUser2->SendMsg(&msg);
        m_bTradeSuccess	= true;
        pUser1->SaveInfo();
        pUser2->SaveInfo();
        return true;
    }
    else
    {
        /*
        CMsgTrade	msg;
        CHECKF(msg.Create(_TRADE_FALSE, 0));
        pUser1->SendMsg(&msg);
        pUser2->SendMsg(&msg);
        pUser1->SendSysMsg(STR_TRADE_FAIL);
        pUser2->SendSysMsg(STR_TRADE_FAIL);
        return false;
        */
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// ISquareDeal 1
//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal1::Create(IUserSquareDeal* pUser, IUserSquareDeal* pTarget)
{
    This()->m_pUser1		= pUser;
    This()->m_pUser2		= pTarget;
    if (!pUser->Create(&This()->m_iface1))			//? 好象多余
    {
        return false;
    }
    if (!pTarget->Create(&This()->m_iface2))
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal1::AddItem(CItem* pItem)
{
    CHECKF(pItem && pItem->GetID());
    if (This()->m_setItem1.size() >= MAX_SQUAREDEALITEMS)
    {
        return false;
    }
    // 检查重复加入
    if (find(This()->m_setItem1.begin(), This()->m_setItem1.end(), pItem->GetID()) != This()->m_setItem1.end())
    {
        return false;
    }
    if (!This()->m_pUser2->AddItem(pItem, This()->m_setItem1.size() + 1, This()->m_nAllWeight1 + pItem->GetWeight()))
    {
        return false;
    }
    This()->m_nAllWeight1	+= pItem->GetWeight();
    This()->m_setItem1.push_back(pItem->GetID());
    return true;
}

//////////////////////////////////////////////////////////////////////
int CSquareDeal::XSquareDeal1::AddMoney(int nMoney)		// return allmoney
{
    ASSERT(nMoney > 0);
    if (nMoney <= 0)
    {
        return This()->m_nMoney1;
    }
    if (This()->m_nMoney1 + nMoney > MAX_SQUAREDEALMONEY)
    {
        return This()->m_nMoney1;
    }
    if (!This()->m_pUser2->AddMoney(This()->m_nMoney1 + nMoney))
    {
        return This()->m_nMoney1;
    }
    This()->m_nMoney1	+= nMoney;
    return This()->m_nMoney1;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal1::ClickOK()
{
    This()->m_bClickOK1		= true;
    This()->m_pUser2->ClickOK();
    if (This()->m_bClickOK2 == true)
    {
        This()->TradeOK();
        return true;		// release by outside
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CUser* CSquareDeal::XSquareDeal1::GetTarget()
{
    return This()->m_pUser2->QueryUser();
}

//////////////////////////////////////////////////////////////////////
// ISquareDeal 2
//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal2::Create(IUserSquareDeal* pUser, IUserSquareDeal* pTarget)
{
    This()->m_pUser2		= pUser;
    This()->m_pUser1		= pTarget;
    if (!pUser->Create(&This()->m_iface2))			//? 好象多余
    {
        return false;
    }
    if (!pTarget->Create(&This()->m_iface1))
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal2::AddItem(CItem* pItem)
{
    CHECKF(pItem);
    if (This()->m_setItem2.size() >= MAX_SQUAREDEALITEMS)
    {
        return false;
    }
    // 检查重复加入
    if (find(This()->m_setItem2.begin(), This()->m_setItem2.end(), pItem->GetID()) != This()->m_setItem2.end())
    {
        return false;
    }
    if (!This()->m_pUser1->AddItem(pItem, This()->m_setItem2.size() + 1, This()->m_nAllWeight2 + pItem->GetWeight()))
    {
        return false;
    }
    This()->m_nAllWeight2	+= pItem->GetWeight();
    This()->m_setItem2.push_back(pItem->GetID());
    return true;
}

//////////////////////////////////////////////////////////////////////
int CSquareDeal::XSquareDeal2::AddMoney(int nMoney)		// return allmoney
{
    ASSERT(nMoney > 0);
    if (nMoney <= 0)
    {
        return This()->m_nMoney2;
    }
    if (This()->m_nMoney2 + nMoney > MAX_SQUAREDEALMONEY)
    {
        return This()->m_nMoney2;
    }
    if (!This()->m_pUser1->AddMoney(This()->m_nMoney2 + nMoney))
    {
        return This()->m_nMoney2;
    }
    This()->m_nMoney2	+= nMoney;
    return This()->m_nMoney2;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal2::ClickOK()
{
    This()->m_bClickOK2		= true;
    This()->m_pUser1->ClickOK();
    if (This()->m_bClickOK1 == true)
    {
        This()->TradeOK();
        return true;		// release by outside
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CUser* CSquareDeal::XSquareDeal2::GetTarget()
{
    return This()->m_pUser1->QueryUser();
}






