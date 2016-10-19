
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------//
// TerrainLayer.h
//----------------------------------------------------//
#ifndef _TERRAINLAYER_H
#define _TERRAINLAYER_H
//----------------------------------------------------//
#include "MapLayer.h"
#include "PuzzleBmp.h"
#include "Layer.h"
#include "2DMapRegion.h"
//----------------------------------------------------//
typedef struct
{
    int nPosX;
    int nPosY;
    int nIndex;
} PassageInfo;
//----------------------------------------------------//
#include <deque>
using namespace std;
//typedef deque<CCell* >  DEQUE_CELL;
typedef deque<PassageInfo* >  DEQUE_PASSAGE;
typedef deque<C2DMapRegion* > DEQUE_REGION;
//----------------------------------------------------//


class CTerrainLayer : public CMapLayer
{
public:
    CTerrainLayer();
    ~CTerrainLayer();

private:
    char m_szPuzzleFile[_MAX_PATH];
    CPuzzleBmp	m_bmpPuzzle;
    DEQUE_PASSAGE m_setPassage;
    DEQUE_REGION  m_setRegion;
    // for maplayer
public:
    void Show(CMyPos posShow);
    void Process(void* pInfo);
    int  GetType();
    int  GetExitIndex(CMyPos posCell);

    // static
public:
    static CTerrainLayer* CreateNew();

    //others
public:
    void LoadPuzzle(char* pszFile);
    void ClearPassage();
    void AddPassage(CMyPos posMap, int nIndex);
    void DelPassage(CMyPos posMap);

    void ClearRegion();
    void DelRegion(CMyPos posRegion, int nType);
    void AddRegion(C2DMapRegion* pRegion);
    C2DMapRegion* GetRegion(CMyPos posRegion, int nType);
    void ShowRegion(int nType);
    int  GetRegionAmount();
    C2DMapRegion* GetRegionByINdex(int nIndex);

    void GetPuzzleSize(CMySize& infoSize);
    int GetPuzzleGrideSize();
    char* GetPuzleFileName() {return m_szPuzzleFile;}


    void LoadDataPassage(FILE* fp);
    void LoadTextPassage(FILE* fp);

    void SaveDataPassage(FILE* fp);
    void SaveTextPassage(FILE* fp);

    void LoadDataRegion(FILE* fp);
    void LoadTextRegion(FILE* fp);

    void SaveDataRegion(FILE* fp);
    void SaveTextRegion(FILE* fp);

    void SetPuzzleGrideSize(int nSize) {m_bmpPuzzle.SetGrideSize(nSize);}
};
//----------------------------------------------------//
#endif
//----------------------------------------------------//
