
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-------------------------------------
// 2DMapTerrainObj.h
//-------------------------------------
#ifndef _2DMAPTERRAINOBJ_H
#define _2DMAPTERRAINOBJ_H

//-------------------------------------
#include "2DMapTerrainObjPart.h"
#include "2DMapObj.h"
//-------------------------------------
class C2DMapTerrainObj : public C2DMapObj
{
public:
    C2DMapTerrainObj();
    virtual ~C2DMapTerrainObj();

private:
    DEQUE_SCENEPART m_setPart;
    BOOL            m_bSyn;
    char            m_szFileName[_MAX_PATH];
    BOOL			m_bFixed;
    BOOL			m_bDie;
    //--------------------------------------------
    // for mapobj
public:
    void Show       (void* pInfo);
    void Process	(void* pInfo);

    int  GetObjType   () {return MAP_TERRAIN;}
    void GetWorldPos(CMyPos& posWorld) {}

    void GetPos(CMyPos& posCell) {posCell = m_posCell;}
    void SetPos(CMyPos posCell);

    BOOL IsFocus();

    //--------------------------------------------
    // additional
public:
    BOOL GetFixed() {return m_bFixed;}
    void SetFixed(BOOL bFix) {m_bFixed = bFix;}

    void Init();
    void Destory();

    void LoadData(FILE* fp);
    void LoadText(FILE* fp);

    void SaveData(FILE* fp);
    void SaveText(FILE* fp);
    void Load(char* pszFile);

    void ClearScenePart();

    void AddPart(C2DMapTerrainObjPart* pPart);
    void DeletePart(int nIndex);

    int  GetPartAmount();
    C2DMapTerrainObjPart* GetPartByIndex(int nIndex);

    BOOL GetMouseOffset(CMyPos posMouse, int& nOffset);
    void Die() {m_bDie = true;}
    BOOL IsDie() {return m_bDie;}
};


//-------------------------------------
#endif
