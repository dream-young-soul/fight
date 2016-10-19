
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-------------------------------------
// scene.cpp
//-------------------------------------
#pragma warning(disable:4786)
#include "2DMapTerrainObj.h"
#include "BaseFunc.h"
#include "3DGameMap.h"
//-------------------------------------

C2DMapTerrainObj::C2DMapTerrainObj()
{
    m_bSyn = false;
    m_bFixed = true;
}

//-------------------------------------

C2DMapTerrainObj::~C2DMapTerrainObj()
{
    this->ClearScenePart();
}

//-------------------------------------

void C2DMapTerrainObj::Init()
{
    m_setPart.clear();
}

//-------------------------------------

void C2DMapTerrainObj::Destory()
{
    int nAmount = m_setPart.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        C2DMapTerrainObjPart* pPart = m_setPart[i];
        SAFE_DELETE(pPart);
    }
    m_setPart.clear();
}

//-------------------------------------

void C2DMapTerrainObj::AddPart(C2DMapTerrainObjPart* pPart)
{
    if (!pPart)
    {
        return;
    }
    m_setPart.push_back(pPart);
}

//-------------------------------------

void C2DMapTerrainObj::DeletePart(int nIndex)
{
    int nAmount = m_setPart.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return;
    }
    C2DMapTerrainObjPart* pPart = m_setPart[nIndex];
    if (pPart)
    {
        SAFE_DELETE(pPart);
    }
    m_setPart.erase(m_setPart.begin() + nIndex);
}

//-------------------------------------

int C2DMapTerrainObj::GetPartAmount()
{
    return m_setPart.size();
}

//-------------------------------------

C2DMapTerrainObjPart* C2DMapTerrainObj::GetPartByIndex(int nIndex)
{
    int nAmount = m_setPart.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setPart[nIndex];
}

//-------------------------------------

void C2DMapTerrainObj::Show(void* pInfo)
{
    return;
}

//-------------------------------------

void C2DMapTerrainObj::Load(char* pszFile)
{
    if (!pszFile)
    {
        return;
    }
    FILE* fp = fopen(pszFile, "rb");
    if (!fp)
    {
        return;
    }
    this->ClearScenePart();
    // load scenepart amount...
    int nAmount;
    fread(&nAmount, sizeof(int), 1, fp);
    strcpy(m_szFileName, pszFile);
    for(int i = 0; i < nAmount; i ++)
    {
        C2DMapTerrainObjPart* pPart = new C2DMapTerrainObjPart;
        if (pPart)
        {
            pPart->Load(fp);
        }
        this->AddPart(pPart);
    }
    this->SetPos(m_posCell);
    fclose(fp);
}

//-------------------------------------
void C2DMapTerrainObj::ClearScenePart()
{
    int nAmount = m_setPart.size();
    for(int i = 0; i < nAmount; i++)
    {
        C2DMapTerrainObjPart* pPart = m_setPart[i];
        SAFE_DELETE(pPart);
    }
    m_setPart.clear();
}
//-------------------------------------
BOOL C2DMapTerrainObj::IsFocus()
{
    int nAmount = m_setPart.size();
    for(int i = 0; i < nAmount; i++)
    {
        C2DMapTerrainObjPart* pPart = m_setPart[i];
        if (pPart && pPart->IsFocus())
        {
            return true;
        }
    }
    return false;
}
//-------------------------------------
void C2DMapTerrainObj::SetPos(CMyPos posCell)
{
    m_posCell = posCell;
    int nAmount = m_setPart.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMyPos posObj = posCell;
        C2DMapTerrainObjPart* pPart = m_setPart[i];
        posObj.x += pPart->GetSceneOffset().x;
        posObj.y += pPart->GetSceneOffset().y;
        if (pPart)
        {
            pPart->SetPos(posObj);
        }
    }
}
//-------------------------------------
void C2DMapTerrainObj::LoadData(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fread(m_szFileName, sizeof(char), _MAX_PATH, fp);
    fread(&m_posCell.x, sizeof(int), 1, fp);
    fread(&m_posCell.y, sizeof(int), 1, fp);
    this->Load(m_szFileName);
}
//-------------------------------------
void C2DMapTerrainObj::LoadText(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fscanf(fp, "2DMapTerrainObjFile=%s\n", m_szFileName);
    fscanf(fp, "PosX=%d\n", &m_posCell.x);
    fscanf(fp, "PosY=%d\n", &m_posCell.y);
    this->Load(m_szFileName);
    this->SetPos(m_posCell);
}
//-------------------------------------

void C2DMapTerrainObj::Process(void* pInfo)
{
    int nAmount = m_setPart.size();
    for(int i = 0; i < nAmount; i++)
    {
        C2DMapTerrainObjPart* pPart = m_setPart[i];
        if (pPart)
        {
            pPart->Process(pInfo);
        }
    }
}
//-------------------------------------
BOOL C2DMapTerrainObj::GetMouseOffset(CMyPos posMouse, int& nOffset)
{
    int nAmount = this->GetPartAmount();
    for(int i = 0; i < nAmount; i++)
    {
        C2DMapTerrainObjPart* pPart = this->GetPartByIndex(i);
        if (pPart)
        {
            if (pPart->GetMouseOffset(posMouse, nOffset))
            {
                return true;
            }
        }
    }
    return false;
}
//-------------------------------------
void C2DMapTerrainObj::SaveData(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fwrite(m_szFileName, sizeof(char), _MAX_PATH, fp);
    fwrite(&m_posCell.x, sizeof(int), 1, fp);
    fwrite(&m_posCell.y, sizeof(int), 1, fp);
}
//-------------------------------------
void C2DMapTerrainObj::SaveText(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fprintf(fp, "2DMapTerrainObjFile=%s\n", m_szFileName);
    fprintf(fp, "PosX=%d\n", m_posCell.x);
    fprintf(fp, "PosY=%d\n", m_posCell.y);
}
//-------------------------------------
