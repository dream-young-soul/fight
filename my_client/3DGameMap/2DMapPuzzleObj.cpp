
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 2DMapPuzzleObj.cpp: implementation of the C2DMapPuzzleObj class.
//
//////////////////////////////////////////////////////////////////////

#include "2DMapPuzzleObj.h"
#include "3DGameMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DMapPuzzleObj::C2DMapPuzzleObj()
{
    m_dwARGB = 0xffffffff;
}

C2DMapPuzzleObj::~C2DMapPuzzleObj()
{
}
//------------------------------------------------------------------------
void C2DMapPuzzleObj::Show(void* pInfo)
{
    if (!pInfo)
    {
        return;
    }
    LayerShowInfo* pShowInfo = (LayerShowInfo*)pInfo;
    CMyPos posView;
    posView.x = pShowInfo->posViewPoint.x;
    posView.y = pShowInfo->posViewPoint.y;
    CMySize sizeMap;
    g_objGameMap.GetMapSize(sizeMap);
    CMyPos posCenter;
    posCenter.x         = sizeMap.iWidth * _CELL_WIDTH / 2;
    posCenter.y         = sizeMap.iHeight * _CELL_HEIGHT / 2;
    CMySize sizeBmp;
    m_bmpPuzzle.GetSize(sizeBmp);
    CMyPos posLU;
    posLU.x             = posCenter.x - sizeBmp.iWidth / 2;
    posLU.y             = posCenter.y - sizeBmp.iHeight / 2;
    CMyPos posShow;
    posShow.x = posView.x - posLU.x;
    posShow.y = posView.y - posLU.y;
    m_bmpPuzzle.SetColor(m_dwARGB);
    m_bmpPuzzle.SetTerrainFlag(false);
    m_bmpPuzzle.SetMoveRate(pShowInfo->nMoveRateX, pShowInfo->nMoveRateY);
    m_bmpPuzzle.Show(-posShow.x, -posShow.y);
}
//------------------------------------------------------------------------
void C2DMapPuzzleObj::LoadData(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    char szFileName[_MAX_PATH];
    fread(szFileName, sizeof(char), _MAX_PATH, fp);
    MYASSERT(m_bmpPuzzle.Load(szFileName));
    strcpy(m_szFileName, szFileName);
}
//------------------------------------------------------------------------
void C2DMapPuzzleObj::SaveData(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fwrite(m_szFileName, sizeof(char), _MAX_PATH, fp);
}
//------------------------------------------------------------------------
void C2DMapPuzzleObj::LoadFile(char* pszFile)
{
    if (!pszFile)
    {
        return;
    }
    MYASSERT(m_bmpPuzzle.Load(pszFile));
    strcpy(m_szFileName, pszFile);
}
//------------------------------------------------------------------------
void C2DMapPuzzleObj::SaveText(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fprintf(fp, "PuzzleFile=%s\n", m_szFileName);
}
//------------------------------------------------------------------------
void C2DMapPuzzleObj::LoadText(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fscanf(fp, "PuzzleFile=%s\n", m_szFileName);
}
//------------------------------------------------------------------------
