
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------
// TerrainObjPart.h
//-------------------------------------
#ifndef _TERRAINOBJPART_H
#define _TERRAINOBJPART_H

// header ...
#include "MapObj.h"
#include "Layer.h"
#include "BaseFunc.h"
//-------------------------------------
class CTerrainObjPart : public CMapObj
{
public:
    CTerrainObjPart();
    virtual ~CTerrainObjPart();
    static CTerrainObjPart*		CreateNew(FILE* fp);

private:
    // about ani
    char              m_szAniTitle[64];
    char              m_szAniFile[255];
    POINT             m_posOffset;
    DWORD             m_dwFrameInterval;

    // about obj
    POINT             m_posSceneOffset;			// 基于CELL的偏移
    SIZE              m_sizeBase;
    DEQUE_LAYER       m_setLayer;
    int               m_nThickness;				// 无意义，保留
    int               m_nHeight;				// 无意义，保留


    //-------------------------------------------------
    // for mapobj
public:

    //-----------------------------------------------------
    // Additional

protected:
    void    Destroy();

public:
    char*   GetAniTitle()					{return m_szAniTitle;}
    POINT	GetOffset()						{return m_posOffset;}
    DWORD   GetAniInterval()				{return m_dwFrameInterval;}

    POINT	GetSceneOffset()				{return m_posSceneOffset;}
    SIZE 	GetBase()						{ return m_sizeBase; }
    CLayer* GetLayer(int nX, int nY);
    int     GetThick()						{return m_nThickness;}
    int     GetHeight()						{return m_nHeight;}

    BOOL    InMyBase(POINT posCell);
};

#include <vector>
using namespace std;
typedef vector<CTerrainObjPart* >  DEQUE_SCENEPART;

//-------------------------------------
#endif

