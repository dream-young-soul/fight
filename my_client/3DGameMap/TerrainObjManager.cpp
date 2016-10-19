
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TerrainObjManager.cpp: implementation of the CTerrainObjManager class.
//
//////////////////////////////////////////////////////////////////////

#include "TerrainObjManager.h"
#include "Hero.h"
#include "3DGameMap.h"
#include "3DRoleData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrainObjManager::CTerrainObjManager()
{
}

CTerrainObjManager::~CTerrainObjManager()
{
    this->Destroy();
}
//------------------------------------------------------------------
void	CTerrainObjManager::Process()
{
    // ÅÐ¶ÏÉ¾³ý
    CMyPos posHero = g_objHero.GetAlignPos();
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if (pInfo && pInfo->idNpc != MOUSE_PLAYER_ID)
        {
            if (posHero.x > 0 && posHero.y > 0)
            {
                if (g_objGameMap.Outof9Block(posHero, pInfo->posCell))
                {
                    CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
                    if (pLayer)
                    {
                        pLayer->DelObj(pInfo->pObj);
                    }
                    m_setTerrainObjInfo.erase(m_setTerrainObjInfo.begin() + i);
                    SAFE_DELETE(pInfo);
                }
            }
        }
    }
}
//------------------------------------------------------------------
void	CTerrainObjManager::Destroy()
{
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setTerrainObjInfo[i]);
    }
    m_setTerrainObjInfo.clear();
}
//------------------------------------------------------------------
void	CTerrainObjManager::AddNpc(int nType, int nDir, OBJID id, CMyPos posCell)
{
    TerrainNpcTypeInfo* pInfo = g_obj3DRoleData.GetTerrainNpcInfo(nType);
    if (!pInfo)
    {
        return;
    }
    TerrainObjInfo* pTerrainInfo = new TerrainObjInfo;
    if (!pTerrainInfo)
    {
        return;
    }
    pTerrainInfo->idNpc = id;
    pTerrainInfo->nType = nType;
    pTerrainInfo->nDir = nDir;
    pTerrainInfo->posCell = posCell;
    strcpy(pTerrainInfo->szName, "");
    pTerrainInfo->pObj = new C2DMapTerrainObj;
    MYASSERT(pTerrainInfo->pObj);
    pTerrainInfo->pObj->Load((char*)pInfo->setStr[nDir%8].c_str());
    pTerrainInfo->pObj->SetPos(posCell);
    if (id == MOUSE_PLAYER_ID)
    {
        pTerrainInfo->pObj->SetFixed(false);
    }
    CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
    if (pLayer)
    {
        pLayer->AddMapObj(pTerrainInfo->pObj);
    }
    m_setTerrainObjInfo.push_back(pTerrainInfo);
}
//------------------------------------------------------------------
void	CTerrainObjManager::Delete(OBJID id)
{
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if ((pInfo) && (pInfo->idNpc == id))
        {
            CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
            if (pLayer)
            {
                pLayer->DelObj(pInfo->pObj);
            }
            m_setTerrainObjInfo.erase(m_setTerrainObjInfo.begin() + i);
            SAFE_DELETE(pInfo);
            break;
        }
    }
}
//------------------------------------------------------------------
OBJID	CTerrainObjManager::GetFocus()
{
    int nAmount = m_setTerrainObjInfo.size();
    TerrainObjInfo* pFocusInfo = NULL;
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if (pInfo && pInfo->pObj && pInfo->pObj->IsFocus())
        {
            if (!pFocusInfo)
            {
                pFocusInfo = pInfo;
            }
            else if (pInfo->posCell.x + pInfo->posCell.y > pFocusInfo->posCell.x + pFocusInfo->posCell.y)
            {
                pFocusInfo = pInfo;
            }
        }
    }
    if (pFocusInfo)
    {
        return pFocusInfo->idNpc;
    }
    else
    {
        return ID_NONE;
    }
}
//------------------------------------------------------------------
void	CTerrainObjManager::SetDir(OBJID id, int nDir)
{
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if (pInfo && pInfo->idNpc == id)
        {
            TerrainNpcTypeInfo* pTypeInfo = g_obj3DRoleData.GetTerrainNpcInfo(pInfo->nType);
            if (!pTypeInfo)
            {
                return;
            }
            CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
            if (pLayer)
            {
                pLayer->DelObj(pInfo->pObj);
            }
            pInfo->nDir = nDir;
            pInfo->pObj = new C2DMapTerrainObj;
            MYASSERT(pInfo->pObj);
            pInfo->pObj->Load((char*)pTypeInfo->setStr[nDir%8].c_str());
            pInfo->pObj->SetPos(pInfo->posCell);
            if (id == MOUSE_PLAYER_ID)
            {
                pInfo->pObj->SetFixed(false);
            }
            if (pLayer)
            {
                pLayer->AddMapObj(pInfo->pObj);
            }
            return;
        }
    }
}
//------------------------------------------------------------------
void	CTerrainObjManager::SetPos(OBJID id, CMyPos posNpc)
{
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if (pInfo && pInfo->idNpc == id)
        {
            pInfo->pObj->SetPos(posNpc);
            pInfo->posCell = posNpc;
            /*			TerrainNpcTypeInfo* pTypeInfo = g_obj3DRoleData.GetTerrainNpcInfo(pInfo->nType);
            			if (!pTypeInfo)
            				return;
            			CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
            			if (pLayer)
            			{
            				pLayer->DelObj(pInfo->pObj);
            			}
            			pInfo->pObj=new C2DMapTerrainObj;
            			pInfo->posCell = posNpc;
            			MYASSERT(pInfo->pObj);
            			pInfo->pObj->Load((char*)pTypeInfo->setStr[pInfo->nDir%8].c_str());
            			pInfo->pObj->SetPos(posNpc);
            			if (id == MOUSE_PLAYER_ID)
            				pInfo->pObj->SetFixed(false);
            			if (pLayer)
            			{
            				pLayer->AddMapObj(pInfo->pObj);
            			}

            			return;*/
        }
    }
}
//------------------------------------------------------------------
int		CTerrainObjManager::GetLook(OBJID id)
{
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if (pInfo && pInfo->idNpc == id)
        {
            int nLook = pInfo->nType * 10 + pInfo->nDir;
            return nLook;
        }
    }
    return 0;
}
//------------------------------------------------------------------
void	CTerrainObjManager::GetPos(OBJID id, CMyPos& posCell)
{
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if (pInfo && pInfo->idNpc == id)
        {
            posCell = pInfo->posCell;
            return;
        }
    }
}
//------------------------------------------------------------------
BOOL	CTerrainObjManager::IsFocus(OBJID id)
{
    int nAmount = m_setTerrainObjInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TerrainObjInfo* pInfo = m_setTerrainObjInfo[i];
        if (pInfo && pInfo->idNpc == id)
        {
            return pInfo->pObj->IsFocus();
        }
    }
    return false;
}
//------------------------------------------------------------------
