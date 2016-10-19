
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
#pragma	warning(disable:4786)
#include "BaseFunc.h"
#include "MyHeap.h"
#include <vector>
using namespace std;
//-------------------------------------------------------------

//-------------------------------------------------------------
// 对于地表层而言 layer 的高度为 0
//-------------------------------------------------------------
class CLayer
{
public:
    unsigned short		nTerrian;       // 地形(属性)
    short				nAltitude;      // 高度－－地基相对于地表的高度
    //	int					nHeight;        // layer的高度（厚度）
    unsigned short		dwMask;         // 阻挡掩码

public:
    CLayer();
    virtual ~CLayer();
    static CLayer*	CreateNew() { return new CLayer; }

protected:
    MYHEAP_DECLARATION(s_heap)
};

#include <vector>
using namespace std;
typedef vector<CLayer* >  DEQUE_LAYER;
typedef vector<DEQUE_LAYER>  DEQUE2_LAYER;

//-------------------------------------------------------------
#endif
