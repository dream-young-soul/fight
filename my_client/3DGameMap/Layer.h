
//**********************************************************
// 代码编辑器
//**********************************************************

////////////////////////////////////////////
// Layer.h
///////////////////////////////////////////

#ifndef _LAYER_H
#define _LAYER_H

//-------------------------------------------------------------
// header files ...
#include "BaseFunc.h"
//-------------------------------------------------------------

//-------------------------------------------------------------
// 对于地表层而言 layer 的高度为 0
//-------------------------------------------------------------
struct  LayerInfo
{
    unsigned short     usTerrain;      // 地形
    unsigned short     usMask;         // 阻挡掩码
    short              sAltitude;      // 高度－－地基相对于地表的高度
    LayerInfo*         pLayer;         // 下一个Layer
};
typedef struct
{
    unsigned short     usTerrain;      // 地形
    unsigned short     usMask;         // 阻挡掩码
    short              sAltitude;      // 高度－－地基相对于地表的高度
} IndexLayerInfo;


class CLayer
{
public:
    unsigned short     usTerrain;       // 地形
    unsigned short     usMask;         // 阻挡掩码
    short              sAltitude;      // 高度－－地基相对于地表的高度

public:
    CLayer();
    virtual ~CLayer();

public:
    static HANDLE	        s_hHeap;
    static UINT		s_uNumAllocsInHeap;
    static BOOL		IsValidPt(void* p);

public:
    void*  operator new	(size_t size);
    void   operator delete	(void* p);
};

typedef struct
{
    LayerInfo       m_infoLayer;
    bool            m_bSearched;
} CellInfo;

#include <deque>
#include <vector>
using namespace std;
typedef deque<CLayer* >  DEQUE_LAYER;
typedef vector<IndexLayerInfo> VECTOR_INDEXLAYER;
//-------------------------------------------------------------
#endif
