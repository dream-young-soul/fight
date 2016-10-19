
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-------------------------------------------------------------
// InteractiveLayer.h
//-------------------------------------------------------------
#ifndef _INTERACTIVELAYER_H
#define _INTERACTIVELAYER_H
//-------------------------------------------------------------
#include "MapLayer.h"
#include "MapObj.h"
#include "2DMapCoverObj.h"
#include "2DMapTerrainObj.h"
#include "3DMapEffect.h"
#include "MapSound.h"

//-------------------------------------------------------------

class CInteractiveLayer : public CMapLayer
{
public:
    CInteractiveLayer();
    ~CInteractiveLayer();

    // for maplayer
public:
    void Show(CMyPos posShow);
    void Process(void* pInfo);
    int  GetType();

    // static
public:
    static CInteractiveLayer* CreateNew();

    // others
public:
    void ClearMapObj();
    void AddMapObj(CMapObj* pObj);
    void DelObj(CMapObj* pObj);

    void PlaceTerrainObj(C2DMapTerrainObj* pTerrainObj);
    void DisplaceTerrainObj(C2DMapTerrainObj* pTerrainObj);

    C2DMapCoverObj* GetMapCoverObj(CMyPos posCell);
    C2DMapCoverObj* Get2DFocusMapCoverObj();
    C2DMapTerrainObj* Get2DFocusMapTerrainObj();
    C2DMapTerrainObjPart* Get2DFocusMapTerrainObjPart();
    C3DMapEffect* Get3DFocusMapEffect();
    CMapSound* GetFocusSound();
    void LoadDataLayer(FILE* fp);
    void LoadTextLayer(FILE* fp);

    void SaveDataLayer(FILE* fp);
    void SaveTextLayer(FILE* fp);

    void ApplyShow(CMapObj* pObj);
    void ApplyShow(CMapObj* pNewObj, DEQUE_MAPOBJ& m_setApply);
    void Del3DMapEffect(CMyPos posWorld);
    void Del3DMapEffect(C3DMapEffect* pEffect);
    void Del3DMapItem(OBJID idItem);
    OBJID Get3DMapItem(CMyPos posCell);
    OBJID GetFocus3DMapItem(CMyPos& posCell);

    void Del2DMapItem(OBJID idItem);
    OBJID Get2DMapItem(CMyPos posCell);
    OBJID GetFocus2DMapItem(CMyPos& posCell);

    int GetObjAmount();
    CMapObj* GetObjByIndex(int nIndex);
private:
    DEQUE_MAPOBJ m_setMapObj;
    DEQUE_MAPOBJ m_setMapObjShow;
};
//-------------------------------------------------------------
#endif
//-------------------------------------------------------------

