
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------
// 2DMapTerrainObjPart.h
//-------------------------------------
#ifndef _2DMAPTERRAINOBJPART_H
#define _2DMAPTERRAINOBJPART_H

// header ...
#include "2DMapObj.h"
#include "Layer.h"
#include "BaseFunc.h"
//-------------------------------------
class C2DMapTerrainObjPart : public C2DMapObj
{
public:
    static BOOL s_bStroke;
public:
    C2DMapTerrainObjPart();
    virtual ~C2DMapTerrainObjPart();


private:
    CMyPos            m_posOffset;
    CMySize           m_sizeBase;
    int               m_nThickness;
    char              m_szAniTitle[64];
    char              m_szAniFile[255];
    DWORD             m_dwFrameInterval;
    int               m_nHeight;
    CMyPos            m_posSceneOffset;  // 基于CELL的偏移
    CMyPos            m_posShow;
    DEQUE_LAYER       m_setLayer;
    int               m_nIndex;
    DWORD		      m_dwPreFrame;
    DEQUE_MAPOBJ      m_setObj;


    //-------------------------------------------------
    // for mapobj
public:
    void Show(void* pInfo);
    void Process(void* pInfo);

    int  GetObjType() {return MAP_TERRAIN_PART;}

    void GetWorldPos(CMyPos& posWorld) {}

    void GetPos(CMyPos& posCell) {posCell = m_posCell;}
    void SetPos(CMyPos posCell) {m_posCell = posCell;}

    void SetBase(CMySize sizeBase) {m_sizeBase.iWidth = sizeBase.iWidth; m_sizeBase.iHeight = sizeBase.iHeight;}
    void GetBase(CMySize& sizeBase) {sizeBase = m_sizeBase;}

    BOOL IsFocus();

    //-----------------------------------------------------
    // Additional

public:
    void    Init();
    void    Destroy();

    void    Load(FILE* fp);

    void    SetOffset(CMyPos posOffset);
    CMyPos  GetOffset();

    void    SetAniInterval(int nInterval) {m_dwFrameInterval = nInterval;}
    DWORD   GetAniInterval() {return m_dwFrameInterval;}

    void    SetThick(int nThick) {m_nThickness = nThick;}
    int     GetThick() {return m_nThickness;}

    CLayer* GetLayer(int nX, int nY);
    char*   GetAniTitle() {return m_szAniTitle;}

    void    SetSceneOffset(CMyPos posOffset);
    CMyPos  GetSceneOffset() {return m_posSceneOffset;}

    int     GetHeight() {return m_nHeight;}
    void    SetHeight(int nHeight) {m_nHeight = nHeight;}

    BOOL    InMyBase(CMyPos posCell);

    void	ClearObj();
    void	AddObj(CMapObj* pObj);

    int		GetObjAmount();
    CMapObj* GetMapObjByIndex(int nIndex);

    BOOL GetMouseOffset(CMyPos posMouse, int& nOffset);
};

#include <deque>
using namespace std;
typedef deque<C2DMapTerrainObjPart* >  DEQUE_SCENEPART;

//-------------------------------------
#endif

