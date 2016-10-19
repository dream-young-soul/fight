
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 2DMapItem.cpp: implementation of the C2DMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "2DMapItem.h"
#include "GameDataSet.h"
#include "3DGameMap.h"
#include "Item.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DMapItem::C2DMapItem(OBJID idItemType)
{
    m_dwFrame		= 0;
    m_idItemType	= idItemType;
    strcpy(m_szName, "");
    ItemEffectInfo* pInfo = g_objGameDataSet.GetItemEffectInfo(m_idItemType);
    if (pInfo)
    {
        if (!m_objEffectEx.TestEffect((char*)pInfo->strMapItemEffect.c_str()))
        {
            m_objEffectEx.Add((char*)pInfo->strMapItemEffect.c_str());
        }
    }
    m_dwBegin	= ::TimeGet();
    m_bShowName = false;
}

C2DMapItem::~C2DMapItem()
{
    m_objEffectEx.Clear();
}

//----------------------------------------------------------------------
C2DMapItem* C2DMapItem::CreateNew(CMyPos posCell, OBJID idItem, OBJID idType, char* szName)
{
    if (idType == ID_NONE)
    {
        return NULL;
    }
    C2DMapItem* pItem = new C2DMapItem(idType);
    MYASSERT(pItem);
    pItem->m_idItem = idItem;
    pItem->SetPos(posCell);
    if (szName && strlen(szName))
    {
        pItem->SetShowName(szName);
    }
    return pItem;
}

//----------------------------------------------------------------------
void C2DMapItem::Show(void* pInfo)
{
    this->GetWorldPos(m_posWorld);
    char szAni[256] = "";
    sprintf(szAni, "%u", m_idItemType);
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/MapItemIcon.Ani", szAni);
    if (!pAni)
    {
        pAni = g_objGameDataSet.GetDataAni("ani/MapItemIcon.Ani", "Default");
        if (!pAni)
        {
            return;
        }
    }
    CMyPos posScr = m_posCell;
    CMyPos posWorld, posBg;
    g_objGameMap.Cell2World(m_posCell.x, m_posCell.y, posWorld.x, posWorld.y);
    g_objGameMap.World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
    m_objEffectEx.Process();
    g_objGameMap.Cell2Screen(m_posCell.x, m_posCell.y, posScr.x, posScr.y);
    CMySize sizeAni = pAni->GetSize(m_dwFrame);
    CMyPos posShow = { posScr.x - sizeAni.iWidth / 4, posScr.y - sizeAni.iHeight / 4};
    CMyBitmap* pBmp = pAni->GetFrame(m_dwFrame++);
    if (pBmp)
    {
        unsigned char ucA, ucR, ucG, ucB;
        ucA = m_dwARGB >> 24;
        ucR = (unsigned char)((m_dwARGB & 0x00ff0000) >> 16);
        ucG = (unsigned char)((m_dwARGB & 0x0000ff00) >> 8);
        ucB = m_dwARGB & 0x000000ff;
        pBmp->SetColor(ucA, ucR, ucG, ucB);
        CMyPos posBmpShow = {posShow.x, posShow.y};
        g_objGameMap.MapScaleShowPos(posBmpShow.x, posBmpShow.y);
        CMySize sizeBmp;
        pBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
        int nScale = g_objGameMap.GetScale ();
        CMySize sizeShow;
        sizeShow.iWidth =  sizeBmp.iWidth * nScale / 256;
        sizeShow.iHeight = sizeBmp.iHeight * nScale / 256;
        pBmp->ShowEx(posBmpShow.x, posBmpShow.y , NULL, sizeShow.iWidth, sizeShow.iHeight );
        if (m_bShowName || g_objGameMap.GetShowMapObjInfoFlag())
        {
            const OBJID idMoney = 300000;
            if (strlen(m_szName) == 0)
            {
                ItemTypeInfo info;
                if (!CItem::s_ItemData.GetItemTypeInfo(m_idItemType, info))
                {
                    strcpy(info.szName, g_objGameDataSet.GetStr(idMoney));
                }
                strcpy(m_szName, info.szName);
            }
            int nMsgLen = strlen(m_szName) * (CMyBitmap::GetFontSize() / 2);
            CMyPos posShowName = {posBmpShow.x + (sizeShow.iWidth - nMsgLen) / 2, posBmpShow.y};
            //		if (m_bShowName)
            //		{
            //			CMyBitmap::ShowString(posShowName.x, posShowName.y-10, _COLOR_YELLOW,m_szName);
            //		}
            //		else
            CMyBitmap::ShowString(posShowName.x, posShowName.y, _COLOR_YELLOW, m_szName);
            //		g_objGameMap.m_InfoViewManager.AddStringView(m_szName, posShowName, _COLOR_YELLOW);
        }
    }
    m_objEffectEx.Show(posBg);
}

//----------------------------------------------------------------------
void C2DMapItem::Process(void* pInfo)
{
    if (::TimeGet() - m_dwBegin < 500)
    {
        return;
    }
    if (g_objGameMap.IsPosVisible(m_posCell))
    {
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (pLayer)
        {
            pLayer->ApplyShow(this);
        }
    }
}
//----------------------------------------------------------------------
BOOL C2DMapItem::IsFocus()
{
    CMyPos posMouse, posCell;
    ::MouseCheck(posMouse.x, posMouse.y);
    g_objGameMap.Mouse2Cell(posMouse.x, posMouse.y, posCell.x, posCell.y);
    if ((posCell.x == m_posCell.x) && (posCell.y == m_posCell.y))
    {
        m_bShowName = true;
        return true;
    }
    else
    {
        m_bShowName = false;
        return false;
    }
}
//----------------------------------------------------------------------




















