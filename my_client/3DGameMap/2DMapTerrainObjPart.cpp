
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------
// 2DMapTerrainObjPart.cpp
//-------------------------------------

#pragma warning(disable:4786)
#include "2DMapTerrainObjPart.h"
#include "gamedataset.h"
#include "3DGameMap.h"
#include "Hero.h"


//----------------------------------------------------
BOOL C2DMapTerrainObjPart::s_bStroke = false;
//----------------------------------------------------

C2DMapTerrainObjPart::C2DMapTerrainObjPart()
{
    m_sizeBase.iWidth = 2;
    m_sizeBase.iHeight = 2;
    m_nThickness = 40;
    this->Init();
    m_nIndex = 0;
    m_dwPreFrame = ::TimeGet();
    m_dwFrameInterval = 200;
    m_posSceneOffset.x = 0;
    m_posSceneOffset.y = 0;
    m_nType = MAP_TERRAIN_PART;
    m_setObj.clear();
}

//-------------------------------------

C2DMapTerrainObjPart::~C2DMapTerrainObjPart()
{
    this->Destroy();
}

//-------------------------------------
void C2DMapTerrainObjPart::Show(void* pInfo)
{
    CAni* pAni = g_objGameDataSet.GetDataAni(m_szAniFile, m_szAniTitle);
    if (pAni)
    {
        int nAmount = pAni->GetFrameAmount();
        if (m_dwFrameInterval == 0)
        {
            m_dwFrameInterval = 1;
        }
        m_nIndex = ((::TimeGet() - m_dwPreFrame) / m_dwFrameInterval) % nAmount;
        CMyBitmap* pBmp = pAni->GetFrame(m_nIndex);
        if (pBmp)
        {
            CMyPos posScr, posWorld, posShow;
            g_objGameMap.Cell2World(m_posCell.x, m_posCell.y, posWorld.x, posWorld.y);
            g_objGameMap.World2Screen(posWorld.x, posWorld.y, posScr.x, posScr.y);
            posShow.x = posScr.x + m_posOffset.x;
            posShow.y = posScr.y + m_posOffset.y;
            g_objGameMap.MapScaleShowPos(posShow.x, posShow.y);
            unsigned char ucA, ucR, ucG, ucB;
            ucA = m_dwARGB >> 24;
            ucR = (unsigned char)((m_dwARGB & 0x00ff0000) >> 16);
            ucG =  (unsigned char)((m_dwARGB & 0x0000ff00) >> 8);
            ucB = m_dwARGB & 0x000000ff;
            pBmp->SetColor(ucA, ucR, ucG, ucB);
            CMySize sizeBmp;
            pBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
            sizeBmp.iWidth = sizeBmp.iWidth * g_objGameMap.GetScale() / _NORMAL_SCALE;
            sizeBmp.iHeight = sizeBmp.iHeight * g_objGameMap.GetScale() / _NORMAL_SCALE;
            pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeBmp.iWidth, sizeBmp.iHeight);
            if (s_bStroke)
            {
                CMyBitmap::ShowRect(posShow.x, posShow.y,
                                    posShow.x + sizeBmp.iWidth, posShow.y + sizeBmp.iHeight,
                                    0xffffff00);
            }
        }
    }
    int nAmount = m_setObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setObj[i];
        pObj->SetARGB(g_objGameMap.GetARGB());
        switch(pObj->GetObjType())
        {
        case MAP_COVER:
            {
                C2DMapCoverObj* pCoverObj =  (C2DMapCoverObj*)pObj;
                pCoverObj->Show(NULL);
            }
            break;
        case  MAP_TERRAIN:
            {
                C2DMapTerrainObj* pTerrainObj =  (C2DMapTerrainObj*)pObj;
                pTerrainObj->Show(NULL);
            }
            break;
        case  MAP_TERRAIN_PART:
            {
                C2DMapTerrainObjPart* pTerrainObjPart =  (C2DMapTerrainObjPart*)pObj;
                pTerrainObjPart->Show(NULL);
            }
            break;
        case MAP_HERO:
            {
                CHero* pHero = (CHero*)pObj;
                pHero->Show(NULL);
            }
            break;
        case MAP_PLAYER:
            {
                CPlayer* pPlayer = (CPlayer*)pObj;
                pPlayer->Show(NULL);
            }
            break;
        default:
            pObj->Show(NULL);
            break;
        }
    }
    m_setObj.clear();
}

//-------------------------------------

void C2DMapTerrainObjPart::SetOffset(CMyPos posOffset)
{
    m_posOffset.x = posOffset.x;
    m_posOffset.y = posOffset.y;
}

//-------------------------------------

CMyPos C2DMapTerrainObjPart::GetOffset()
{
    return m_posOffset;
}

//-------------------------------------

void C2DMapTerrainObjPart::Destroy()
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

void C2DMapTerrainObjPart::Init()
{
    m_setLayer.clear();
    for(int i = 0; i < m_sizeBase.iHeight; i++)
    {
        for(int j = 0; j < m_sizeBase.iWidth; j ++)
        {
            CLayer* pLayer = new(CLayer);
            if (pLayer)
            {
                m_setLayer.push_back(pLayer);
            }
        }
    }
}

//-------------------------------------

CLayer* C2DMapTerrainObjPart::GetLayer(int nX, int nY)
{
    int nAmount = m_setLayer.size();
    int nIndex = nX + nY * m_sizeBase.iWidth;
    if ((nIndex >= nAmount) || (nIndex < 0))
    {
        return NULL;
    }
    return m_setLayer[nIndex];
}

//-------------------------------------

void C2DMapTerrainObjPart::SetSceneOffset(CMyPos posOffset)
{
    m_posSceneOffset.x = posOffset.x;
    m_posSceneOffset.y = posOffset.y;
}

//-------------------------------------

void C2DMapTerrainObjPart::Load(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    // Get Ani ...
    // Get anifilename ..
    fread(m_szAniFile, 1, 256, fp);
    // get anititle ...
    fread(m_szAniTitle, 1, 64, fp);
    // get ani offset
    CMyPos posOffset;
    fread(&posOffset.x, sizeof(int), 1, fp);
    fread(&posOffset.y, sizeof(int), 1, fp);
    this->SetOffset(posOffset);
    // get ani interval ...
    DWORD dwData ;
    fread(&dwData, sizeof(DWORD), 1, fp);
    this->SetAniInterval(dwData);
    // get wide long high...
    CMySize sizeBase;
    fread(&sizeBase.iWidth, sizeof(UINT), 1, fp);
    fread(&sizeBase.iHeight, sizeof(UINT), 1, fp);
    this->SetBase(sizeBase);
    int nData;
    fread(&nData, sizeof(int), 1, fp);
    this->SetThick(nData);
    // load pos offset ...
    CMyPos posSceneOffset;
    fread(&posSceneOffset.x, sizeof(int), 1, fp);
    fread(&posSceneOffset.y, sizeof(int), 1, fp);
    this->SetSceneOffset(posSceneOffset);
    // load height ...
    fread(&nData, sizeof(int), 1, fp);
    this->SetHeight(nData);
    // get layer ...
    this->Destroy();
    this->Init();
    CMySize infoSize;
    this->GetBase(infoSize);
    for(int i = 0; i < infoSize.iHeight; i++)
    {
        for(int j = 0; j < infoSize.iWidth; j++)
        {
            // Get layer ...
            CLayer* pLayer = this->GetLayer(j, i);
            if (pLayer)
            {
                // read mask ...
                DWORD dwMask;
                int nTerrain;
                int nAltitude;
                fread(&dwMask, sizeof(DWORD), 1, fp);
                fread(&nTerrain, sizeof(int), 1, fp);
                fread(&nAltitude, sizeof(int), 1, fp);
                pLayer->usTerrain = nTerrain;
                pLayer->usMask	  = (unsigned short)dwMask;
                pLayer->sAltitude = (short)nAltitude;
            }
        }
    }
}

//-------------------------------------
BOOL C2DMapTerrainObjPart::IsFocus()
{
    CAni* pAni = g_objGameDataSet.GetDataAni(m_szAniFile, m_szAniTitle);
    if (!pAni)
    {
        return false;
    }
    CMyBitmap* pBmp = pAni->GetFrame(m_nIndex);
    if (!pBmp)
    {
        return false;
    }
    CMyPos posMouse;
    ::MouseCheck(posMouse.x, posMouse.y);
    CMyPos posCell;
    this->GetPos(posCell);
    posCell.x -= this->GetSceneOffset().x;
    posCell.y -= this->GetSceneOffset().y;
    CMyPos posWorld;
    g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
    CMyPos posScreen;
    g_objGameMap.World2Screen(posWorld.x, posWorld.y, posScreen.x, posScreen.y);
    posScreen.x += this->GetOffset().x;
    posScreen.y += this->GetOffset().y;
    CMyPos posTransparent;
    posTransparent.x = posMouse.x - posScreen.x;
    posTransparent.y = posMouse.y - posScreen.y;
    return !pBmp->IsTransparent(posTransparent);
    return false;
}
//-------------------------------------
void C2DMapTerrainObjPart::Process(void* pInfo)
{
    if (g_objGameMap.IsPosVisible(m_posCell))
    {
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (pLayer)
        {
            pLayer->ApplyShow(this);
        }
    }
}
//-------------------------------------
int  C2DMapTerrainObjPart::GetObjAmount()
{
    return m_setObj.size();
}
//-------------------------------------
void C2DMapTerrainObjPart::ClearObj()
{
    m_setObj.clear();
}
//-------------------------------------
void C2DMapTerrainObjPart::AddObj(CMapObj* pNewObj)
{
    if (!pNewObj)
    {
        return;
    }
    CMyPos posNewObj;
    pNewObj->GetPos(posNewObj);
    int nNewObjX = posNewObj.x;
    int nNewObjY = posNewObj.y;
    UINT i;
    DWORD dwSize	= m_setObj.size();
    for(i = 0; i < dwSize; i++)
    {
        CMapObj* pOldObj	= m_setObj[i];
        CMyPos posOldObj;
        pOldObj->GetPos(posOldObj);
        int nOldObjX = posOldObj.x;
        int nOldObjY = posOldObj.y;
        CMySize sizeOldObj, sizeNewObj;
        g_objGameMap.GetMapObjSize(sizeOldObj, pOldObj);
        g_objGameMap.GetMapObjSize(sizeNewObj, pNewObj);
        // sort now!
        if ((nNewObjX - sizeNewObj.iWidth + 1) - nOldObjX > 0 ||
                (nOldObjX - sizeOldObj.iWidth + 1) - nNewObjX > 0)
        {
            // x no overlay
            int nNewData =  nNewObjY - (int)sizeNewObj.iHeight + 1;
            int nOldData =  nOldObjY - (int)sizeOldObj.iHeight + 1;
            if (nNewData > nOldObjY || nOldData > nNewObjY)
            {
                // y no overlay
                if (nNewObjX + nNewObjY < nOldObjX + nOldObjY)
                {
                    m_setObj.insert(m_setObj.begin() + i, pNewObj);
                    return;
                }
                // do nothing in this condition
            }
            else // got y overlay
            {
                if (nNewObjX < nOldObjX)
                {
                    m_setObj.insert(m_setObj.begin() + i, pNewObj);
                    return;
                }
            }
        }
        else // got x overlay
        {
            if (nNewObjY < nOldObjY)
            {
                m_setObj.insert(m_setObj.begin() + i, pNewObj);
                return;
            }
        }
    }
    if (i >= dwSize)
    {
        m_setObj.push_back(pNewObj);
    }
}
//-------------------------------------
CMapObj* C2DMapTerrainObjPart::GetMapObjByIndex(int nIndex)
{
    int nAmount = m_setObj.size();
    if (nIndex >= nAmount)
    {
        return NULL;
    }
    if (nIndex < 0)
    {
        return NULL;
    }
    return m_setObj[nIndex];
}
//-------------------------------------
BOOL C2DMapTerrainObjPart::InMyBase(CMyPos posCell)
{
    CMyPos posA, posC;
    posA = m_posCell;
    posC.x = m_posCell.x - m_sizeBase.iWidth;
    posC.y = m_posCell.y - m_sizeBase.iHeight;
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
BOOL C2DMapTerrainObjPart::GetMouseOffset(CMyPos posMouse, int& nOffset)
{
    CMySize sizeBase;
    this->GetBase(sizeBase);
    for(int i = 0; i < sizeBase.iHeight; i++)
    {
        for(int j = 0; j < sizeBase.iWidth; j++)
        {
            CMyPos posMyMouse = posMouse;
            CLayer* pLayer = this->GetLayer(j, i);
            if (!pLayer)
            {
                continue;
            }
            // 改变鼠标高度
            posMyMouse.y += pLayer->sAltitude;
            // 判断是否在格子内
            CMyPos posCell, posWorld, posCell1;
            g_objGameMap.Screen2World(posMyMouse.x, posMyMouse.y, posWorld.x, posWorld.y);
            g_objGameMap.World2Cell(posWorld.x, posWorld.y, posCell1.x, posCell1.y);
            g_objGameMap.Screen2Cell(posMyMouse.x, posMyMouse.y, posCell.x, posCell.y);
            if ((posCell.x == m_posCell.x - j) && (posCell.y == m_posCell.y - i))
            {
                nOffset = pLayer->sAltitude;
                return true;
            }
        }
    }
    return false;
}
//-------------------------------------------------------------


