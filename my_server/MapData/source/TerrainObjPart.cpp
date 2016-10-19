
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-------------------------------------
// TerrainObjPart.cpp
//-------------------------------------

#pragma warning(disable:4786)
#include "typedef.h"
#include "TerrainObjPart.h"
#include "common.h"
#include "MapData.h"

//-------------------------------------

CTerrainObjPart::CTerrainObjPart()
{
    SetObjType(TERRAIN_LEAF);
    m_sizeBase.cx			= 2;
    m_sizeBase.cy	 		= 2;
    m_nThickness 			= 40;
    m_dwFrameInterval 		= 200;
    m_posSceneOffset.x 		= 0;
    m_posSceneOffset.y 		= 0;
}

//-------------------------------------

CTerrainObjPart::~CTerrainObjPart()
{
    this->Destroy();
}

//-------------------------------------

void CTerrainObjPart::Destroy()
{
    int nAmount = m_setLayer.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CLayer* pLayer = m_setLayer[i];
        if (pLayer)
        {
            SAFE_DELETE(pLayer);
        }
    }
    m_setLayer.clear();
}

//-------------------------------------

CTerrainObjPart* CTerrainObjPart::CreateNew(FILE* fp)
{
    if (!fp)
    {
        return NULL;
    }
    CTerrainObjPart* ptr = new CTerrainObjPart;
    if (!ptr)
    {
        return NULL;
    }
    // Get Ani ...
    // Get anifilename ..
    fread(ptr->m_szAniFile, 1, 256, fp);
    // get anititle ...
    fread(ptr->m_szAniTitle, 1, 64, fp);
    // get ani offset
    fread(&ptr->m_posOffset.x, sizeof(int), 1, fp);
    fread(&ptr->m_posOffset.y, sizeof(int), 1, fp);
    // get ani interval ...
    fread(&ptr->m_dwFrameInterval, sizeof(DWORD), 1, fp);
    // get wide long high...
    fread(&ptr->m_sizeBase.cx, sizeof(UINT), 1, fp);
    fread(&ptr->m_sizeBase.cy, sizeof(UINT), 1, fp);
    fread(&ptr->m_nThickness, sizeof(int), 1, fp);
    // load pos offset ...
    fread(&ptr->m_posSceneOffset.x, sizeof(int), 1, fp);
    fread(&ptr->m_posSceneOffset.y, sizeof(int), 1, fp);
    // load height ...
    fread(&ptr->m_nHeight, sizeof(int), 1, fp);
    // get layer ...
    SIZE infoSize = ptr->GetBase();
    for(int i = 0; i < infoSize.cy; i++)
    {
        for(int j = 0; j < infoSize.cx; j++)
        {
            // Get layer ...
            CLayer* pLayer = CLayer::CreateNew();
            if (!pLayer)
            {
                SAFE_DELETE(ptr);
                return NULL;
            }
            // write mask ...
            int	nData;
            fread(&nData, sizeof(DWORD), 1, fp);
            pLayer->dwMask = nData;
            fread(&nData, sizeof(int), 1, fp);
            pLayer->nTerrian = nData;
            fread(&nData, sizeof(int), 1, fp);
            pLayer->nAltitude = nData;
            ptr->m_setLayer.push_back(pLayer);
        }
    }
    return ptr;
}

//-------------------------------------

CLayer* CTerrainObjPart::GetLayer(int nX, int nY)
{
    int nAmount = m_setLayer.size();
    int nIndex = POS2INDEX(nX, nY, m_sizeBase.cx, m_sizeBase.cy);
    if ((nIndex >= nAmount) || (nIndex < 0))
    {
        return NULL;
    }
    return m_setLayer[nIndex];
}

//-------------------------------------
BOOL CTerrainObjPart::InMyBase(POINT posCell)
{
    POINT posA, posC;
    posA = m_posCell;
    posC.x = m_posCell.x - m_sizeBase.cx;
    posC.y = m_posCell.y - m_sizeBase.cy;
    if ((posCell.x <= posA.x) &&
            (posCell.y <= posA.y) &&
            (posCell.x > posC.x) &&
            (posCell.y > posC.y))
    {
        return true;
    }
    return false;
}
//-------------------------------------------------------------


