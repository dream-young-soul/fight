
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _MAPOBJ_H
#define _MAPOBJ_H

#include "ND_Bitmap.h"
#include "BaseFunc.h"

typedef struct
{
    CMyPos posViewPoint;
    CMyPos posViewOffset;
    int    nMoveRateX;
    int    nMoveRateY;
} LayerShowInfo;

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

class CMyBitmap;

class CMapObj
{

public:
    CMapObj()								{m_nType = MAP_NONE;}
    virtual ~CMapObj(void)                  {}

public:
    virtual void Show(void* pInfo)      {}
    virtual void Process(void* pInfo)       {}

    virtual int  GetObjType()               {return m_nType;}
    virtual void GetWorldPos(CMyPos& posWorld) {}

    virtual void GetPos(CMyPos& posCell)      {}
    virtual void SetPos(CMyPos posCell)       {}

    virtual void GetBase(CMySize& infoSize)   {infoSize.iWidth = 1; infoSize.iHeight = 1;}
    virtual void SetBase(CMySize infoSize)   {}

    virtual BOOL IsFocus()                  {return false;}

    virtual CMyBitmap* GetImage(int& nPosX, int& nPosY)	{nPosX = nPosY = 0; return NULL;}
    virtual void SetARGB(DWORD dwARGB) {m_dwARGB = dwARGB;}
    virtual DWORD GetARGB() {return m_dwARGB;}
public:
    int		m_nType;
    DWORD	m_dwARGB;
};
#include <deque>
using namespace std;
typedef deque<CMapObj* >  DEQUE_MAPOBJ;
#endif
