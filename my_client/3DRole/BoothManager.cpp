
//**********************************************************
// 代码编辑器
//**********************************************************

// BoothManager.cpp: implementation of the CBoothManager class.
//
//////////////////////////////////////////////////////////////////////

#include "BoothManager.h"
#include "AllMsg.h"
#include "Hero.h"
#include "3DGameMap.h"
#include "GameMsg.h"
#include "GameDataSet.h"
#include "BoothNpc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoothManager::CBoothManager()
{
    m_idNpc = ID_NONE;
    m_bActive = false;
    strcpy(m_szCryOut, "");
}

CBoothManager::~CBoothManager()
{
    int nAmount = m_dequeItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_dequeItem[i];
        SAFE_DELETE(pItem);
    }
    m_dequeItem.clear();
}

//------------------------------------------------------------------------
DWORD	CBoothManager::GetItemAmount()
{
    return m_dequeItem.size();
}
//------------------------------------------------------------------------
CItem*	CBoothManager::GetItemByIndex(int nIndex)
{
    int nAmount = m_dequeItem.size();
    if (nIndex >= nAmount || nIndex < 0)
    {
        return NULL;
    }
    return m_dequeItem[nIndex];
}
//------------------------------------------------------------------------
BOOL	CBoothManager::Open(OBJID idBooth)
{
    if (g_objHero.IsMoving())
    {
        return false;
    }
    if (idBooth == ID_NONE)
    {
        // 开始摆摊, 挑选位置
        int nAmount = m_dequeItem.size();
        int i = 0;
        for(i = 0; i < nAmount; i ++)
        {
            CItem* pItem = m_dequeItem[i];
            g_objHero.AddItem(pItem);
        }
        m_idNpc = ID_NONE;
        this->SetActive(false);
        CMyPos posHero, posSooth;
        g_objHero.GetPos(posHero);
        g_objHero.SetDir(6);
        int nDir = g_objHero.GetDir();
        posSooth.x = posHero.x + _DELTA_X[nDir];
        posSooth.y = posHero.y + _DELTA_Y[nDir];
        /*	for(i = 0; i < 5; i ++)
        	{
        			CMyPos posMask;
        			posMask.x = posSooth.x + BoothMaskOffset[nDir][i][0];
        			posMask.y = posSooth.y + BoothMaskOffset[nDir][i][1];
        			CellInfo* pInfo = g_objGameMap.GetCell(posMask.x, posMask.y);
        			if (pInfo)
        			{
        				LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pInfo);
        				const DWORD dwMarket = 16;
        				if (pLayerInfo->usMask != 0)// || pLayerInfo->usTerrain != dwMarket)
        				{
        					OBJID idStr = 100125;//请找个开阔的地方摆摊
        					g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
        					return false;
        				}
        			}
        	}*/
        CMsgAction msg;
        if (msg.Create(g_objHero.GetID(), posSooth.x, posSooth.y, nDir, actionCreateBooth, posSooth.x, posSooth.y))
        {
            msg.Send();
        }
    }
    else
    {
        int nAmount = m_dequeItem.size();
        for(int i = 0; i < nAmount; i ++)
        {
            CItem* pItem = m_dequeItem[i];
            g_objHero.AddItem(pItem);
        }
        m_idNpc = idBooth;
        this->SetActive(true);
        g_objHero.Emotion(_ACTION_SITDOWN);
        ::PostCmd(CMD_ACTION_OPENBOOTH);
    }
    return true;
}
//------------------------------------------------------------------------
void	CBoothManager::Close()
{
    // copy all item to hero item set ...
    int nAmount = m_dequeItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_dequeItem[i];
        g_objHero.AddItem(pItem);
        ::PostCmd(CMD_FLASH_BOOTHMANAGER_ITEM);
        ::PostCmd(CMD_FLASHITEM);
    }
    m_dequeItem.clear();
    m_idNpc = ID_NONE;
    this->SetActive(false);
    CMsgAction msg;
    if (msg.Create(g_objHero.GetID(), 0, 0, 0, actionDestroyBooth))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------------
void	CBoothManager::AddItem(OBJID idItem, DWORD dwBoothPrice, BOOL bMsg)
{
    if (!bMsg)
    {
        CItem* pItem = g_objHero.GetItem(idItem);
        if (pItem)
        {
            m_dequeItem.push_back(pItem);
            pItem->SetBoothSellPrice(dwBoothPrice);
        }
        g_objHero.DelItem(idItem, false);
        ::PostCmd(CMD_FLASH_BOOTHMANAGER_ITEM);
        ::PostCmd(CMD_FLASHITEM);
    }
    else
    {
        CItem* pItem = g_objHero.GetItem(idItem);
        if (!pItem || pItem->IsMonopolyItem())
        {
            return;
        }
        CMsgItem msg;
        if (msg.Create(idItem, dwBoothPrice, ITEMACT_BOOTH_ADD))
        {
            msg.Send();
        }
    }
}
//------------------------------------------------------------------------
void	CBoothManager::DelItem(OBJID idItem, BOOL bMsg)
{
    if (!bMsg)
    {
        int nAmount = m_dequeItem.size();
        for(int i = 0; i < nAmount; i ++)
        {
            CItem* pItem = m_dequeItem[i];
            if (pItem && pItem->GetID() == idItem)
            {
                g_objHero.AddItem(pItem);
                ::PostCmd(CMD_FLASH_BOOTHMANAGER_ITEM);
                ::PostCmd(CMD_FLASHITEM);
                m_dequeItem.erase(m_dequeItem.begin() + i);
                return;
            }
        }
    }
    else
    {
        CMsgItem msg;
        if (msg.Create(idItem, m_idNpc, ITEMACT_BOOTH_DEL))
        {
            msg.Send();
        }
    }
}
//------------------------------------------------------------------------
void	CBoothManager::PauseBooth()
{
    m_idNpc = ID_NONE;
    CMsgAction msg;
    if (msg.Create(g_objHero.GetID(), 0, 0, 0, actionSuspendBooth, 0, 0))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------------
BOOL	CBoothManager::ContinueBooth(OBJID idBooth)
{
    if (idBooth == ID_NONE)
    {
        // 开始摆摊, 挑选位置
        CMyPos posHero, posSooth;
        g_objHero.GetPos(posHero);
        int nDir = g_objHero.GetDir();
        posSooth.x = posHero.x + _DELTA_X[nDir];
        posSooth.y = posHero.y + _DELTA_Y[nDir];
        for(int i = 0; i < 5; i ++)
        {
            CMyPos posMask;
            posMask.x = posSooth.x + BoothMaskOffset[nDir][i][0];
            posMask.y = posSooth.y + BoothMaskOffset[nDir][i][1];
            CellInfo* pInfo = g_objGameMap.GetCell(posMask.x, posMask.y);
            if (pInfo)
            {
                LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pInfo);
                if (pLayerInfo->usMask != 0)
                {
                    return false;
                }
            }
        }
        CMsgAction msg;
        if (msg.Create(g_objHero.GetID(), posSooth.x, posSooth.y, nDir, actionResumeBooth, posSooth.x, posSooth.y))
        {
            msg.Send();
        }
    }
    else
    {
        this->Close();
        m_idNpc = idBooth;
    }
    return true;
}
//------------------------------------------------------------------------
void	CBoothManager::SetCryOut(char* pszStr)
{
    if (pszStr && strlen(pszStr) <= 128)
    {
        strcpy(m_szCryOut, pszStr);
        CMsgTalk msg;
        if (msg.Create((char*)g_objHero.GetName(), "大家", pszStr, NULL, 0xffffff, _TXTATR_CRYOUT, _TXT_NORMAL))
        {
            msg.Send();
        }
        g_objHero.SetCryOut(pszStr);
    }
}
//------------------------------------------------------------------------
BOOL	CBoothManager::BeShowOpenDialog()
{
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        if (!m_bActive)
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------
BOOL	CBoothManager::BeShowCloseDialog()
{
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        if (m_bActive)
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------
