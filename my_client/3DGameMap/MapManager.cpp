
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-------------------------------------------------------------------
// MapManager.cpp
//-------------------------------------------------------------------

#include "3DGameMap.h"
#include "3DMapEffect.h"

//-------------------------------------------------------------------
void CGameMap::AddMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell)
{
    m_objItemManager.AddMapItem(idMapItem, idItemType, posCell);
}
//-------------------------------------------------------------------
void CGameMap::DelMapItem(OBJID idMapItem)
{
    m_objItemManager.DelMapItem(idMapItem);
}
//-------------------------------------------------------------------
OBJID CGameMap::GetMapItem(CMyPos posCell)
{
    return m_objItemManager.GetMapItem(posCell);
}
//-------------------------------------------------------------------
C3DMapEffect* CGameMap::Add3DMapEffect(CMyPos posWorld, char* pszIndex, BOOL bDelSelf)
{
    if (!pszIndex)
    {
        return NULL;
    }
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if (!pLayer)
    {
        return NULL;
    }
    C3DMapEffect* pEffect = C3DMapEffect::CreateNew(posWorld, pszIndex, bDelSelf);
    if (pEffect)
    {
        pLayer->AddMapObj(pEffect);
    }
    return pEffect;
}
//-------------------------------------------------------------------
void CGameMap::Del3DMapEffect(C3DMapEffect* pEffect)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if (pLayer)
    {
        pLayer->Del3DMapEffect(pEffect);
    }
}
//-------------------------------------------------------------------
void CGameMap::Del3DMapEffect(CMyPos posWorld)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if (pLayer)
    {
        pLayer->Del3DMapEffect(posWorld);
    }
}
//-------------------------------------------------------------------
OBJID CGameMap::GetFocusMapItem(CMyPos& posCell)
{
    return m_objItemManager.GetFocusItem(posCell);
}
//--------------------------------------------------------------------
void CGameMap::AddTerrainNpc(int nType, int nDir, OBJID id, CMyPos posCell)
{
    m_objTerrainObjManager.AddNpc(nType, nDir, id, posCell);
}
//--------------------------------------------------------------------
void CGameMap::DeleteTerrainNpc(OBJID id)
{
    m_objTerrainObjManager.Delete(id);
}
//--------------------------------------------------------------------
void CGameMap::SetTerrainNpcDir(OBJID id, int nDir)
{
    m_objTerrainObjManager.SetDir(id, nDir);
}
//--------------------------------------------------------------
void CGameMap::SetTerrainNpcPos(OBJID id, CMyPos posCell)
{
    m_objTerrainObjManager.SetPos(id, posCell);
}
//--------------------------------------------------------------
void CGameMap::GetTerrainNpcPos(OBJID id, CMyPos& posCell)
{
    m_objTerrainObjManager.GetPos(id, posCell);
}
//--------------------------------------------------------------
int CGameMap::GetTerrainNpcLook(OBJID id)
{
    return m_objTerrainObjManager.GetLook(id);
}
//--------------------------------------------------------------
OBJID CGameMap::GetFocusTerrainNpc()
{
    return m_objTerrainObjManager.GetFocus();
}
//--------------------------------------------------------------
BOOL CGameMap::IsTerrainNpcFocus(OBJID id)
{
    return m_objTerrainObjManager.IsFocus(id);
}
//--------------------------------------------------------------
