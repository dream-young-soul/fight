
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMapItem.cpp: implementation of the C3DMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "3DMapItem.h"
#include "3DGameMap.h"
#include "3DRoleData.h"
#include "Item.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DMapItem::C3DMapItem()
{
    m_idType = ID_NONE;
    m_idItem = ID_NONE;
    m_nFrameIndex = 0;
}

C3DMapItem::~C3DMapItem()
{
}
//--------------------------------------------------------------------------------------
void C3DMapItem::Show(void* pInfo)
{
    m_obj3DSimpleObj.Rotate(0.0, 0.0, 0.0);
    m_obj3DSimpleObj.SetFrame(m_nFrameIndex / 3);
    m_obj3DSimpleObj.SetPos(m_posBg.x, m_posBg.y, 0, 0, (float)m_usMapPercent / 100);
    m_obj3DSimpleObj.SetOblique(true);
    m_obj3DSimpleObj.SetLightOffset(2, 300, -300, -1000, 0.8f, 0.1f, 0.1f, 0.1f);
    CMyPos posView;
    g_objGameMap.GetViewPos(posView);
    int nViewportBgX, nViewportBgY;
    g_objGameMap.World2Bg(posView.x, posView.y, nViewportBgX, nViewportBgY);
    m_obj3DSimpleObj.Draw2BG(nViewportBgX, nViewportBgY);
    m_obj3DSimpleObj.ClearMatrix();
    m_objEffect.Show(m_posBg);
    CMyPos posScr;
    g_objGameMap.Cell2Screen(m_posCell.x, m_posCell.y, posScr.x, posScr.y);
    if (g_objGameMap.GetShowMapObjInfoFlag() || this->IsFocus())
    {
        const OBJID idMoney = 300000;
        ItemTypeInfo info;
        if (!CItem::s_ItemData.GetItemTypeInfo(m_idType, info))
        {
            strcpy(info.szName, g_objGameDataSet.GetStr(idMoney));
        }
        int nMsgLen = strlen(info.szName) * (CMyBitmap::GetFontSize() / 2);
        CMyPos posShowName = {posScr.x - nMsgLen / 2, posScr.y};
        g_objGameMap.MapScaleShowPos(posShowName.x, posShowName.y);
        g_objGameMap.m_InfoViewManager.AddStringView(info.szName, posShowName, _COLOR_YELLOW);
    }
}
//--------------------------------------------------------------------------------------
void C3DMapItem::Process(void* pInfo)
{
    m_nFrameIndex ++;
    m_objEffect.Process();
    if (g_objGameMap.IsPosVisible(m_posCell))
    {
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (pLayer)
        {
            pLayer->ApplyShow(this);
        }
    }
}
//--------------------------------------------------------------------------------------
BOOL C3DMapItem::IsFocus()
{
    CMyPos posMouse, posCell;
    ::MouseCheck(posMouse.x, posMouse.y);
    //return m_obj3DSimpleObj.HitTest(posMouse.x, posMouse.y, m_posBg.x, m_posBg.y);
    g_objGameMap.Screen2Cell(posMouse.x, posMouse.y, posCell.x, posCell.y);
    if ((posCell.x == m_posCell.x) && (posCell.y == m_posCell.y))
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL C3DMapItem::Create(OBJID idType, OBJID idItem, CMyPos posWorld)
{
    m_idType = idType;
    m_idItem = idItem;
    OBJID idMesh, idTexture;
    unsigned short usData;
    if (!g_obj3DRoleData.Get3DItem(idType, idMesh, idTexture, usData, m_usMapPercent))
    {
        ErrorOut("Can not found Item in 3DItem.ini IdType = %u", idType);
    }
    m_obj3DSimpleObj.Create(idMesh, idTexture);
    g_objGameMap.World2Bg(posWorld.x, posWorld.y, m_posBg.x, m_posBg.y);
    g_objGameMap.World2Cell(posWorld.x, posWorld.y, m_posCell.x, m_posCell.y);
    m_objEffect.Add("itemflash");
    return true;
}
//--------------------------------------------------------------------------------------
