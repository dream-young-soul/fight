
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------
// scene.cpp
//-------------------------------------
#pragma warning(disable:4786)

#include "TerrainObj.h"
//#include "BaseFunc.h"
#include "common.h"
#include "string.h"
#include "LOGFILE.h"
//-------------------------------------

//-------------------------------------

CTerrainObj::~CTerrainObj()
{
    this->Destory();
}

//-------------------------------------
CTerrainObj* CTerrainObj::CreateNew(LPCTSTR pszFile, OBJID idOwner /*= ID_NONE*/)
{
    if (!pszFile)
    {
        return NULL;
    }
    FILE* fp = fopen(pszFile, "rb");
    if (!fp)
    {
        LOGERROR("无法打开地图物件文件[%s]", pszFile);
        return NULL;
    }
    CTerrainObj* ptr = new CTerrainObj;
    if (!ptr)
    {
        fclose(fp);
        return NULL;
    }
    ptr->m_idOwner = idOwner;
    // load scenepart amount...
    int nAmount;
    fread(&nAmount, sizeof(int), 1, fp);
    strcpy(ptr->m_szFileName, pszFile);
    for(int i = 0; i < nAmount; i ++)
    {
        CTerrainObjPart* pPart = CTerrainObjPart::CreateNew(fp);
        if (!pPart)
        {
            fclose(fp);
            SAFE_DELETE(ptr);
            return NULL;
        }
        ptr->AddPart(pPart);
    }
    fclose(fp);
    return ptr;
}

//-------------------------------------
void CTerrainObj::SetPos(POINT posCell)
{
    m_posCell = posCell;
    int nAmount = m_setPart.size();
    for(int i = 0; i < nAmount; i++)
    {
        POINT posObj = posCell;
        CTerrainObjPart* pPart = m_setPart[i];
        posObj.x += pPart->GetSceneOffset().x;
        posObj.y += pPart->GetSceneOffset().y;
        if (pPart)
        {
            pPart->SetPos(posObj);
        }
    }
}

//-------------------------------------

void CTerrainObj::Destory()
{
    int nAmount = m_setPart.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CTerrainObjPart* pPart = m_setPart[i];
        SAFE_DELETE(pPart);
    }
    m_setPart.clear();
}

//-------------------------------------

void CTerrainObj::AddPart(CTerrainObjPart* pPart)
{
    if (!pPart)
    {
        return;
    }
    m_setPart.push_back(pPart);
}

//-------------------------------------

void CTerrainObj::DelPart(int nIndex)
{
    int nAmount = m_setPart.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return;
    }
    CTerrainObjPart* pPart = m_setPart[nIndex];
    if (pPart)
    {
        SAFE_DELETE(pPart);
    }
    m_setPart.erase(m_setPart.begin() + nIndex);
}

//-------------------------------------

int CTerrainObj::GetPartAmount()
{
    return m_setPart.size();
}

//-------------------------------------

CTerrainObjPart* CTerrainObj::GetPartByIndex(int nIndex)
{
    int nAmount = m_setPart.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setPart[nIndex];
}

