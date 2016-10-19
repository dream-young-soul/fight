
//**********************************************************
// 代码编辑器
//**********************************************************

//----------------------------------------------------
// MapObj.h
//----------------------------------------------------

#ifndef _MAPOBJ_H
#define _MAPOBJ_H

#include "windows.h"
#include "BaseFunc.h"


// mapobj 的类型定义
enum
{
    MAP_NONE                = 0,
    MAP_TERRAIN             = 1,
    MAP_TERRAIN_PART        = 2,
    MAP_SCENE               = 3,
    MAP_COVER               = 4,
    MAP_ROLE                = 5,
    MAP_HERO				= 6,
    MAP_PLAYER				= 7,
    MAP_PUZZLE				= 8,
    MAP_3DSIMPLE			= 9,
    MAP_3DEFFECT			= 10,
    MAP_2DITEM				= 11,
    MAP_3DNPC				= 12,
    MAP_3DOBJ				= 13,
    MAP_3DTRACE				= 14,
    MAP_SOUND				= 15,
    MAP_2DREGION			= 16,
    MAP_3DMAGICMAPITEM		= 17,
    MAP_3DITEM				= 18,
    MAP_3DEFFECTNEW			= 19,
};

//----------------------------------------------------
class CMapObj
{
public:
    CMapObj();
    virtual ~CMapObj();
protected:
    POINT	m_posCell;
    int		m_nObjType;

    //------------------------------------------------------------
    // for mapobj
public:
    virtual POINT GetPos()				{ return m_posCell; }
    virtual void SetPos(POINT posCell)	{ m_posCell = posCell; }

    enum { TERRAIN_NONE = 0, TERRAIN_TREE, TERRAIN_LEAF, };
    virtual int GetObjType   ()	{ return m_nObjType; }
protected:
    void SetObjType	(int nType) { m_nObjType = nType; }
};
#include <vector>
using namespace std;
typedef vector<CMapObj* >  DEQUE_MAPOBJ;

//----------------------------------------------------
#endif

