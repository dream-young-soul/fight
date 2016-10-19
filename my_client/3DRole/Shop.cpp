
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Shop.cpp: implementation of the CShop class.
//
//////////////////////////////////////////////////////////////////////
#include "Item.h"
#include "TxtMsgSet.h"
#include "Shop.h"
#include "GameMsg.h"
#include "ShopData.h"
#include "3DGameMap.h"
#include "GameDataSet.h"
#include "Hero.h"
#include "GamePlayerSet.h"

// constant...
const char szShopInfoFile[]		= "ini/Shop.ini";

// global...
CShop	g_objShop;
CShopData CShop::s_ShopData;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CShop::CShop()
{
    m_dequeItem.clear();
    m_bActive = false;
}

CShop::~CShop()
{
    this->Close();
}

//-------------------------------------------------------------------
DWORD	CShop::GetItemAmount()
{
    if (!m_bActive)
    {
        OBJID idOpenShopFirst = 100083;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idOpenShopFirst));
        return 0;
    }
    return m_dequeItem.size();
}
//-------------------------------------------------------------------
CItem*	CShop::GetItem(DWORD dwIndex)
{
    if (!m_bActive)
    {
        OBJID idOpenShopFirst = 100083;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idOpenShopFirst));
        return NULL;
    }
    DWORD dwAmount = m_dequeItem.size();
    if (dwIndex >= dwAmount)
    {
        return NULL;
    }
    return m_dequeItem[dwIndex];
}
//-------------------------------------------------------------------
BOOL	CShop::Open(OBJID idNpc)
{
    if (m_bActive)
    {
        this->Close();
    }
    if (idNpc == ID_NONE)
    {
        OBJID idShopIdError = 100084;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idShopIdError));
        return false;
    }
    if (s_ShopData.GetItemList(idNpc, m_dequeItem, m_szName, m_nShopType))
    {
        m_bActive	= true;
        m_idNpc		= idNpc;
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idNpc);
        if (pPlayer)
        {
            CMyPos posPlayer;
            pPlayer->GetPos(posPlayer.x, posPlayer.y);
            CMyPos posHero;
            g_objHero.GetPos(posHero.x, posHero.y);
            int nDir = g_objGameMap.GetDirection(posPlayer.x, posPlayer.y, posHero.x, posHero.y);
            pPlayer->m_pIRoleView->SetVariable(_VAR_DIRECT_DIR, nDir);
        }
        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------
void	CShop::Close()
{
    if (!m_bActive)
    {
        OBJID idOpenShopFirst = 100083;
        return;
    }
    m_bActive = false;
    this->ClearItem();
}
//-------------------------------------------------------------------
void	CShop::ClearItem()
{
    int nAmount = m_dequeItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_dequeItem[i];
        SAFE_DELETE(pItem);
    }
    m_dequeItem.clear();
}
//-------------------------------------------------------------------
char*	CShop::GetShopName()
{
    if (!m_bActive)
    {
        OBJID idOpenShopFirst = 100083;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idOpenShopFirst));
        return NULL;
    }
    return m_szName;
}
//-------------------------------------------------------------------
OBJID	CShop::GetShopID()
{
    if (!m_bActive)
    {
        OBJID idOpenShopFirst = 100083;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idOpenShopFirst));
        return ID_NONE;
    }
    return m_idNpc;
}
//-------------------------------------------------------------------
int		CShop::GetShopType()
{
    if (!m_bActive)
    {
        OBJID idOpenShopFirst = 100083;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idOpenShopFirst));
        return _SHOP_NONE;
    }
    return m_nShopType;
}
//-------------------------------------------------------------------
