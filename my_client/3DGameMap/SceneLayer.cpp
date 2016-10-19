
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------------------------
// SceneLayer.cpp
//-------------------------------------------------------
#include "SceneLayer.h"
#include "2DMapSceneObj.h"
#include "2DMapPuzzleObj.h"
#include "3DGameMap.h"
//-------------------------------------------------------
CSceneLayer::CSceneLayer()
{
    m_setMapObj.clear();
}
//-------------------------------------------------------
CSceneLayer::~CSceneLayer()
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        SAFE_DELETE(pObj);
    }
    m_setMapObj.clear();
}
//-------------------------------------------------------------
void CSceneLayer::Show(CMyPos posShow)
{
    LayerShowInfo infoShow;
    //算出自己的视口
    //1.取标准视口中心点
    CMyPos posView;
    g_objGameMap.GetViewPos(posView);
    posView.x += _SCR_WIDTH / 2;
    posView.y += _SCR_HEIGHT / 2;
    //2.算出中心点
    CMySize sizeMap;
    g_objGameMap.GetMapSize(sizeMap);
    CMyPos posCenter;
    posCenter.x         = sizeMap.iWidth * _CELL_WIDTH / 2;
    posCenter.y         = sizeMap.iHeight * _CELL_HEIGHT / 2;
    //3.算出标准视口点与中心点之间的偏移
    CMyPos posOffset;
    posOffset.x = posView.x - posCenter.x;
    posOffset.y = posView.y - posCenter.y;
    //4.算出自己视口中心点与中心点之间的偏移
    CMyPos posMyView;
    posMyView.x = posCenter.x + posOffset.x * this->GetMoveRateX() / 100;
    posMyView.y = posCenter.y + posOffset.y * this->GetMoveRateY() / 100;
    posMyView.x -= _SCR_WIDTH / 2;
    posMyView.y -= _SCR_HEIGHT / 2;
    infoShow.posViewPoint = posMyView;
    infoShow.nMoveRateX = this->GetMoveRateX();
    infoShow.nMoveRateY = this->GetMoveRateY();
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            pObj->Show(&infoShow);
        }
    }
}
//-------------------------------------------------------------
void CSceneLayer::Process(void* pInfo)
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            pObj->Process(NULL);
        }
    }
}
//-------------------------------------------------------------
int  CSceneLayer::GetType()
{
    return LAYER_SCENE;
}
//-------------------------------------------------------------
CSceneLayer* CSceneLayer::CreateNew()
{
    CSceneLayer* pLayer = new CSceneLayer;
    return pLayer;
}
//-------------------------------------------------------------
void CSceneLayer::AddObj(CMapObj* pMapObj)
{
    if (!pMapObj)
    {
        return;
    }
    m_setMapObj.push_back(pMapObj);
}
//-------------------------------------------------------------
void CSceneLayer::SaveDataLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount = m_setMapObj.size();
    int nRateX, nRateY;
    nRateX = this->GetMoveRateX();
    nRateY = this->GetMoveRateY();
    fwrite(&nRateX, sizeof(int), 1, fp);
    fwrite(&nRateY, sizeof(int), 1, fp);
    fwrite(&nAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            switch(pObj->GetObjType())
            {
            case MAP_SCENE:
                {
                    int nObjType = MAP_SCENE;
                    fwrite(&nObjType, sizeof(int), 1, fp);
                    C2DMapSceneObj* pSceneObj = (C2DMapSceneObj*)pObj;
                    pSceneObj->SaveDataObj(fp);
                }
                break;
            case MAP_PUZZLE:
                {
                    int nObjType = MAP_PUZZLE;
                    fwrite(&nObjType, sizeof(int), 1, fp);
                    C2DMapPuzzleObj* pPuzzleObj = (C2DMapPuzzleObj*)pObj;
                    pPuzzleObj->SaveData(fp);
                }
                break;
            default:
                break;
            }
        }
    }
}
//-------------------------------------------------------------

void CSceneLayer::LoadDataLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nRateX, nRateY;
    fread(&nRateX, sizeof(int), 1, fp);
    fread(&nRateY, sizeof(int), 1, fp);
    this->SetMoveRateX(nRateX);
    this->SetMoveRateY(nRateY);
    int nAmount = 0;
    fread(&nAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nAmount; i++)
    {
        // get obj type
        int nMapObjType;
        fread(&nMapObjType, sizeof(int), 1, fp);
        switch(nMapObjType)
        {
        case MAP_SCENE:
            {
                C2DMapSceneObj* pSceneObj = new C2DMapSceneObj;
                if (pSceneObj)
                {
                    pSceneObj->LoadDataObj(fp);
                    this->AddObj(pSceneObj);
                }
            }
            break;
        case MAP_PUZZLE:
            {
                C2DMapPuzzleObj* pPuzzleObj = new C2DMapPuzzleObj;
                if (pPuzzleObj)
                {
                    pPuzzleObj->LoadData(fp);
                    this->AddObj(pPuzzleObj);
                }
            }
            break;
        default:
            break;
        }
    }
}
//-------------------------------------------------------------
void CSceneLayer::DelObj(CMapObj* pObj)
{
    if (!pObj)
    {
        return;
    }
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        if (pObj == m_setMapObj[i])
        {
            SAFE_DELETE(pObj);
        }
        m_setMapObj.erase(m_setMapObj.begin() + i);
    }
}
//-------------------------------------------------------------
CMapObj* CSceneLayer::GetFocusObj()
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (!pObj)
        {
            continue;
        }
        switch(pObj->GetObjType())
        {
        case CMapLayer::LAYER_SCENE:
            {
                C2DMapSceneObj* pSceneObj = (C2DMapSceneObj*)pObj;
                if (pSceneObj->IsFocus())
                {
                    return  pSceneObj;
                }
            }
            break;
        default:
            break;
        }
    }
    return NULL;
}
//-------------------------------------------------------------
void CSceneLayer::SaveTextLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount = m_setMapObj.size();
    fprintf(fp, "========SceneLayer ObjAmount=%d========\n", nAmount);
    fprintf(fp, "MoveRate[%d,%d]\n", this->GetMoveRateX(), this->GetMoveRateY());
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            switch(pObj->GetObjType())
            {
            case MAP_SCENE:
                {
                    fprintf(fp, "========ObjType=%d========\n", MAP_SCENE);
                    C2DMapSceneObj* pSceneObj = (C2DMapSceneObj*)pObj;
                    pSceneObj->SaveTextObj(fp);
                }
                break;
            case MAP_PUZZLE:
                {
                    fprintf(fp, "========ObjType=%d========\n", MAP_PUZZLE);
                    C2DMapPuzzleObj* pPuzzleObj = (C2DMapPuzzleObj*)pObj;
                    pPuzzleObj->SaveText(fp);
                }
                break;
            default:
                break;
            }
        }
    }
}
//-------------------------------------------------------------
void CSceneLayer::LoadTextLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount;
    fscanf(fp, "========SceneLayer ObjAmount=%d========\n", &nAmount);
    int nRateX, nRateY;
    fscanf(fp, "MoveRate[%d,%d]\n", &nRateX, &nRateY);
    this->SetMoveRateX(nRateX);
    this->SetMoveRateY(nRateY);
    for(int i = 0; i < nAmount; i++)
    {
        int nObjType;
        fscanf(fp, "========ObjType=%d========\n", &nObjType);
        switch(nObjType)
        {
        case MAP_SCENE:
            {
                C2DMapSceneObj* pSceneObj = new C2DMapSceneObj;
                if (pSceneObj)
                {
                    pSceneObj->LoadTextObj(fp);
                    this->AddObj(pSceneObj);
                }
            }
            break;
        case MAP_PUZZLE:
            {
                C2DMapPuzzleObj* pPuzzleObj = new C2DMapPuzzleObj;
                if (pPuzzleObj)
                {
                    pPuzzleObj->LoadText(fp);
                    this->AddObj(pPuzzleObj);
                }
            }
            break;
        default:
            break;
        }
    }
}
//-------------------------------------------------------------
