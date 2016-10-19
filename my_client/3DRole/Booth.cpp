
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Booth.cpp: implementation of the CBooth class.
//
//////////////////////////////////////////////////////////////////////

#include "Booth.h"
#include "AllMsg.h"
#include "Player.h"
#include "GamePlayerSet.h"
#include "gamedataset.h"
#include "Hero.h"
#include "GameMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBooth::CBooth()
{
    m_idNpc = ID_NONE;
    strcpy(m_szName, "");
}

CBooth::~CBooth()
{
    this->Close();
}
//-----------------------------------------------------------------------
DWORD	CBooth::GetItemAmount()
{
    return m_dequeItem.size();
}
//-----------------------------------------------------------------------
CItem*	CBooth::GetItemByIndex(int nIndex)
{
    int nAmount = m_dequeItem.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    return m_dequeItem[nIndex];
}
//-----------------------------------------------------------------------
BOOL	CBooth::Open(OBJID idBooth)
{
    // search npc in the map
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idBooth);
    if (!pPlayer)
    {
        return false;
    }
    CMsgItem msg;
    if (msg.Create(idBooth, idBooth, ITEMACT_BOOTH_QUERY))
    {
        msg.Send();
        m_idNpc = idBooth;
        OBJID idStr = 100124;
        sprintf(m_szName, "%s%s", pPlayer->GetName(), g_objGameDataSet.GetStr(idStr));
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------
void	CBooth::Close()
{
    if (m_idNpc == ID_NONE)
    {
        return;
    }
    int nAmount = m_dequeItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_dequeItem[i]);
    }
    m_dequeItem.clear();
    strcpy(m_szName, "");
    m_idNpc = ID_NONE;
    ::PostCmd(CMD_FLASH_BOOTH_ITEM);
}
//-----------------------------------------------------------------------
void	CBooth::AddItem(CItem* pItem)
{
    if (!pItem)
    {
        return;
    }
    this->DelItem(pItem->GetID());
    m_dequeItem.push_back(pItem);
    ::PostCmd(CMD_FLASH_BOOTH_ITEM);
}
//-----------------------------------------------------------------------
void	CBooth::DelItem(OBJID idItem)
{
    int nAmount = m_dequeItem.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CItem* pItem = m_dequeItem[i];
        if (pItem && pItem->GetID() == idItem)
        {
            SAFE_DELETE(pItem);
            m_dequeItem.erase(m_dequeItem.begin() + i);
            ::PostCmd(CMD_FLASH_BOOTH_ITEM);
            return;
        }
    }
}
//-----------------------------------------------------------------------
void	CBooth::BuyItem(OBJID idItem)
{
    int nAmount = m_dequeItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_dequeItem[i];
        if (pItem && pItem->GetID() == idItem)
        {
            if (g_objHero.GetMoney() < pItem->GetBoothSellPrice())
            {
                OBJID idStr = 100076;
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
                return;
            }
            CMsgItem msg;
            if (msg.Create(idItem, m_idNpc, ITEMACT_BOOTH_BUY))
            {
                msg.Send();
            }
            return;
        }
    }
    // the item is not found ...
    OBJID idStr = 100044;
    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
}
//-----------------------------------------------------------------------
void	CBooth::SetPrice(OBJID idItem, DWORD dwPrice)
{
    int nAmount = m_dequeItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_dequeItem[i];
        if (pItem && pItem->GetID() == idItem)
        {
            pItem->SetBoothSellPrice(dwPrice);
            ::PostCmd(CMD_FLASH_BOOTH_ITEM);
        }
    }
}
//-----------------------------------------------------------------------
