
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Trade.cpp: implementation of the CTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "Trade.h"
#include "Hero.h"
#include "GamePlayerSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrade::CTrade()
{
    m_bHeroAgree		=	false;
    m_bPlayerAgree		=	false;
    m_bActive			=	false;
    m_dwHeroMoney		=	0;
    m_dwPlayerMoney		=	0;
    m_idPlayer			=	ID_NONE;
    m_setHeroItem.clear();
    m_setPlayerItem.clear();
}

CTrade::~CTrade()
{
    this->Destroy();
}

//-------------------------------------------------------------------------

void CTrade::Destroy()
{
    m_bHeroAgree		=	false;
    m_bPlayerAgree		=	false;
    m_bActive			=	false;
    m_idPlayer			=	ID_NONE;
    m_dwHeroMoney		=	0;
    m_dwPlayerMoney		=	0;
    int nAmount, i;
    // clear the items of hero
    nAmount = m_setHeroItem.size();
    for(i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_setHeroItem[i];
        SAFE_DELETE(pItem);
    }
    m_setHeroItem.clear();
    // clear the player's item
    nAmount = m_setPlayerItem.size();
    for(i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_setPlayerItem[i];
        SAFE_DELETE(pItem);
    }
    m_setPlayerItem.clear();
}

//-------------------------------------------------------------------------

void CTrade::Close(BOOL	bSuc)
{
    int i, nAmount;
    if (bSuc)
    {
        // del the hero items
        nAmount = m_setHeroItem.size();
        for(i = 0; i < nAmount; i ++)
        {
            CItem* pItem = m_setHeroItem[i];
            SAFE_DELETE(pItem);
        }
        m_setHeroItem.clear();
        // add the player items to hero
        nAmount = m_setPlayerItem.size();
        for(i = 0; i < nAmount; i ++)
        {
            CItem* pItem = m_setPlayerItem[i];
            g_objHero.AddItem(pItem);
            g_objHero.SetHelpTip(pItem->GetTypeID());
        }
        m_setPlayerItem.clear();
        // add the player money to hero
        g_objHero.SetMoney(g_objHero.GetMoney() + m_dwPlayerMoney);
        // post command
    }
    else
    {
        // add the hero items to the package
        nAmount = m_setHeroItem.size();
        for(i = 0; i < nAmount; i ++)
        {
            CItem* pItem = m_setHeroItem[i];
            g_objHero.AddItem(pItem);
        }
        m_setHeroItem.clear();
        // add the hero money ...
        g_objHero.SetMoney(g_objHero.GetMoney() + m_dwHeroMoney);
        // del the player's item
        nAmount = m_setPlayerItem.size();
        for(i = 0; i < nAmount; i ++)
        {
            CItem* pItem = m_setPlayerItem[i];
            SAFE_DELETE(pItem);
        }
        m_setPlayerItem.clear();
        // post command
    }
    // reset ...
    m_bHeroAgree		=	false;
    m_bPlayerAgree		=	false;
    m_bActive			=	false;
    m_dwHeroMoney		=	0;
    m_dwPlayerMoney		=	0;
}

//-------------------------------------------------------------------------
CItem*	CTrade::GetHeroItemByIndex(int nIndex)
{
    int nAmount = this->GetHeroItemAmount();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setHeroItem[nIndex];
}
//-------------------------------------------------------------------------
CItem*	CTrade::GetPlayerItemByIndex(int nIndex)
{
    int nAmount = this->GetPlayerItemAmount();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setPlayerItem[nIndex];
}
//-------------------------------------------------------------------------
void CTrade::SetPlayerMoney(DWORD dwMoney)
{
    m_dwPlayerMoney = dwMoney;
    ::PostCmd(CMD_TRADEPLAYERITEM);
}
//-------------------------------------------------------------------------
void CTrade::AddHeroMoney(DWORD dwMoney)
{
    m_dwHeroMoney += dwMoney;
}
//-------------------------------------------------------------------------
void CTrade::AddHeroItem(CItem* pItem)
{
    // del item from package
    if (!pItem)
    {
        return;
    }
    // add to trade
    m_setHeroItem.push_back(pItem);
}
//-------------------------------------------------------------------------
void CTrade::DelHeroItem(OBJID idItem, BOOL bDel)
{
    int nAmount = this->GetPlayerItemAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_setHeroItem[i];
        if (pItem && pItem->GetID() == idItem)
        {
            if (bDel)
            {
                SAFE_DELETE(pItem);
            }
            m_setHeroItem.erase(m_setHeroItem.begin() + i);
            return;
        }
    }
}

//-------------------------------------------------------------------------
void CTrade::AddPlayerItem(CItem* pItem)
{
    if (pItem)
    {
        m_setPlayerItem.push_back(pItem);
        ::PostCmd(CMD_TRADEPLAYERITEM);
    }
}
//-------------------------------------------------------------------------
void CTrade::Open(OBJID idPlayer)
{
    m_idPlayer = idPlayer;
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idPlayer);
    if (pPlayer)
    {
        strcpy(m_szPlayerName, pPlayer->GetName());
    }
    else
    {
        strcpy(m_szPlayerName, 0);
    }
    this->Destroy();
    m_bActive = true;
}
//-------------------------------------------------------------------------
void CTrade::SetLastApplyPlayerId(OBJID idplayer)
{
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idplayer);
    MYASSERT(pPlayer);
    m_idLastApplyPlayer = idplayer;
    strcpy(m_szLastApplyPlayerName, pPlayer->GetName());
    ::PostCmd(CMD_TRADEPLAYERAPPLY);
}
//--------------------------------------------------------------
void CTrade::SetHeroMoney(DWORD dwMoney)
{
    m_dwHeroMoney = +dwMoney;
}
//--------------------------------------------------------------
void CTrade::AddItemFalse(OBJID idItem)
{
    int nAmount = m_setHeroItem.size();
    CItem* pItem = NULL;
    for(int i = 0; i < nAmount; i ++)
    {
        pItem = m_setHeroItem[i];
        if (pItem && pItem->GetID() == idItem)
        {
            m_setHeroItem.erase(m_setHeroItem.begin() + i);
            break;
        }
    }
    if (pItem)
    {
        g_objHero.AddItem(pItem);
    }
}
//--------------------------------------------------------------
