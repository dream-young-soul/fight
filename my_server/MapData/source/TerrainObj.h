
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-------------------------------------
// TerrainObj.h
//-------------------------------------
#ifndef _TERRAINOBJ_H
#define _TERRAINOBJ_H

//-------------------------------------
#include "TerrainObjPart.h"
#include "MapObj.h"
#include "typedef.h"
//-------------------------------------
class CTerrainObj : public CMapObj
{
public: // construct
    CTerrainObj() { SetObjType(TERRAIN_TREE); }
    virtual ~CTerrainObj();
    static CTerrainObj*		CreateNew(LPCTSTR szFileName, OBJID idOwner = ID_NONE);
    void SetPos(POINT posCell);

private:
    OBJID			m_idOwner;
    DEQUE_SCENEPART m_setPart;
    char            m_szFileName[_MAX_PATH];

    //--------------------------------------------
    // additional
public:
    OBJID	GetOwnerID() { return m_idOwner; }

    int  GetPartAmount();
    CTerrainObjPart* GetPartByIndex(int nIndex);

protected:
    void Destory();

    void AddPart(CTerrainObjPart* pPart);
    void DelPart(int nIndex);
};

#include <vector>
using namespace std;
typedef vector<CTerrainObj* >  DEQUE_TERRAINOBJ;

//-------------------------------------
#endif
