
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MapItemManager.cpp: implementation of the MapItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MapItemManager.h"
#include "3DGamemap.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapItemManager::CMapItemManager()
{
}

CMapItemManager::~CMapItemManager()
{
    this->Destroy();
}
//--------------------------------------------------------------------
void CMapItemManager::Process()
{
    // ÅÐ¶ÏÉ¾³ý
    CMyPos posHero = g_objHero.GetAlignPos();
    int nAmount = m_setItemInfo.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        MapItemInfo* pInfo = m_setItemInfo[i];
        if (pInfo)
        {
            if (posHero.x > 0 && posHero.y > 0)
            {
                if (g_objGameMap.Outof9Block(posHero, pInfo->posCell))
                {
                    CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
                    if (pLayer)
                    {
                        pLayer->DelObj(pInfo->pMapItem);
                    }
                    m_setItemInfo.erase(m_setItemInfo.begin() + i);
                    SAFE_DELETE(pInfo);
                }
            }
        }
    }
}
//--------------------------------------------------------------------
void CMapItemManager::AddMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell)
{
    int nAmount = m_setItemInfo.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        MapItemInfo* pInfo = m_setItemInfo[i];
        if (idMapItem == pInfo->idMapItem)	// repeat id
        {
            return;
        }
    }
    MapItemInfo* pInfo = new MapItemInfo;
    MYASSERT(pInfo);
    pInfo->pMapItem		= C2DMapItem::CreateNew(posCell, idMapItem, idItemType);
    pInfo->idItemType	= idItemType;
    pInfo->idMapItem	= idMapItem;
    pInfo->posCell		= posCell;
    CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
    if (pLayer)
    {
        pLayer->AddMapObj(pInfo->pMapItem);
    }
    m_setItemInfo.push_back(pInfo);
}
//--------------------------------------------------------------------
void CMapItemManager::DelMapItem(OBJID idMapItem)
{
    int nAmount = m_setItemInfo.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        MapItemInfo* pInfo = m_setItemInfo[i];
        if ((pInfo) && (pInfo->idMapItem == idMapItem))
        {
            CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
            if (pLayer)
            {
                pLayer->DelObj(pInfo->pMapItem);
            }
            m_setItemInfo.erase(m_setItemInfo.begin() + i);
            SAFE_DELETE(pInfo);
            break;
        }
    }
}
//--------------------------------------------------------------------
void CMapItemManager::Destroy()
{
    int nAmount = m_setItemInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        MapItemInfo* pInfo = m_setItemInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setItemInfo.clear();
}
//--------------------------------------------------------------------
OBJID CMapItemManager::GetMapItem(CMyPos posCell)
{
    int nAmount = m_setItemInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        MapItemInfo* pInfo = m_setItemInfo[i];
        if (pInfo)
        {
            if ((pInfo->posCell.x == posCell.x) && (pInfo->posCell.y == posCell.y))
            {
                return pInfo->idMapItem;
            }
        }
    }
    return ID_NONE;
}
//--------------------------------------------------------------------
OBJID CMapItemManager::GetFocusItem(CMyPos& posCell)
{
    CMyPos posMouse;
    ::MouseCheck(posMouse.x, posMouse.y);
    g_objGameMap.MapScaleMousePos(posMouse.x, posMouse.y);
    int nAmount = m_setItemInfo.size();
    OBJID idMapItem = ID_NONE;
    DWORD dwDist = 9999999;
    const DWORD dwFocusRange = 32;
    MapItemInfo* pFocusInfo = NULL;
    for(int i = 0; i < nAmount; i ++)
    {
        MapItemInfo* pInfo = m_setItemInfo[i];
        if (pInfo)
        {
            // ÅÐ¶ÏÊó±êºÍÖÐÐÄµã¾àÀë ...
            CMyPos posCenter;
            g_objGameMap.Cell2Screen(pInfo->posCell.x, pInfo->posCell.y, posCenter.x, posCenter.y);
            DWORD dwMyDist = (posMouse.x - posCenter.x) *
                             (posMouse.x - posCenter.x) +
                             (posMouse.y - posCenter.y) *
                             (posMouse.y - posCenter.y);
            pInfo->pMapItem->BeShowName(false);
            if ((dwMyDist < dwFocusRange * dwFocusRange) && (dwMyDist < dwDist))
            {
                dwDist = dwMyDist;
                idMapItem = pInfo->idMapItem;
                pFocusInfo = pInfo;
            }
        }
    }
    if (pFocusInfo)
    {
        pFocusInfo->pMapItem->BeShowName(true);
        posCell = pFocusInfo->posCell;
    }
    return idMapItem;
}
//--------------------------------------------------------------------
