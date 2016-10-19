
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-------------------------------------------------------
// SceneLayer.h
//-------------------------------------------------------
#ifndef _SCENE_LAYER
#define _SCENE_LAYER
//-------------------------------------------------------
#include "MapLayer.h"
#include "MapObj.h"
//-------------------------------------------------------
class CSceneLayer : public CMapLayer
{
public:
    CSceneLayer();
    ~CSceneLayer();

public:
    void Show(CMyPos posShow);
    void Process(void* pInfo);
    void AddObj(CMapObj* pMapObj);
    int  GetType();

public:
    void LoadDataLayer(FILE* fp);
    void SaveDataLayer(FILE* fp);
    void LoadTextLayer(FILE* fp);
    void SaveTextLayer(FILE* fp);
    void DelObj(CMapObj* pObj);
    CMapObj* GetFocusObj();

public:
    static CSceneLayer* CreateNew();
public:
    DEQUE_MAPOBJ m_setMapObj;
};
//-------------------------------------------------------
#endif
