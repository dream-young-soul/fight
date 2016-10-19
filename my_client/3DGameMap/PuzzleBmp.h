
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef _PUZZLEBMP_H
#define _PUZZLEBMP_H
//------------------------------------------------
// PuzzleBmp.h
//------------------------------------------------
#include "BaseFunc.h"

//constant...
const int PUZZLE_GRIDE_SIZE = 128;	// 格子大小

	
const  int		_MAX_DIRTY_RECT = 256;

class CPuzzleBmp
{
public:
    CPuzzleBmp();
    virtual ~CPuzzleBmp();

protected:
    int m_nPuzzleAmount;
    unsigned short* m_usPuzzleIndex;
    BOOL m_bTerrain;

public:
    void SetGrid(int iPosX, int iPosY, unsigned short usIndex);
    unsigned short GetGrid(CMyPos posShow);
    unsigned short GetGrid(int iPosX, int iPosY);
    void Bmp2Grid	(CMyPos posBmp, CMyPos& posGrid);
    void Grid2Bmp	(CMyPos posGrid, CMyPos& posBmp);
    void   SetSize(CMySize infoSize);

public:
    void SetAniFile(char* pszFile) {if (pszFile) { strcpy(m_szAniFile , pszFile); }}
    char* GetAniFile() {return m_szAniFile;}

public:
    virtual void Show(int iPosX, int iPosY, BOOL bGrid = false, BOOL bPos = false);

    void GetSizeByGride(CMySize& infoSize) {memcpy(&infoSize , &m_infoSize, sizeof(infoSize));}
    void GetSize(CMySize& infoSize);

    virtual void Save(char* pszFile);
    virtual BOOL Load(char* pszFile);
    virtual void Destroy();

    virtual void PreLoad(int iPosX, int iPosY);
    void SetZoomInNum(int nZoom);
    int	 GetZoomInNum() {return m_nZoomInNum;}

    void Unit(CPuzzleBmp* pPuzzle, CMyPos posGrid);
    void SetTerrainFlag(BOOL bTerrain) {m_bTerrain = bTerrain;}
    void SetRollSpeedX(int nSpeedX) {m_nRollSpeedX = nSpeedX;}
    void SetRollSpeedY(int nSpeedY) {m_nRollSpeedY = nSpeedY;}
    int	 GetRoolSpeedX() {return m_nRollSpeedX;}
    int	 GetRoolSpeedY() {return m_nRollSpeedY;}
    void SetMoveRate(int nMoveRateX, int nMoveRateY) {m_nMoveRateX = nMoveRateX; m_nMoveRateY = nMoveRateY;}
    void SetRoll(BOOL bRoll) {m_bRoll = bRoll;}

    void SetGrideSize(int nSize) {m_nGrideSize = nSize;}
    int  GetGrideSize() {return m_nGrideSize;}
    static int  GetNormalGrideSize();
    void ReapetRateCount(DWORD& dwPicAmount, DWORD& dwGrideAmount);
    void GenerateAni(const char* pszAniFile);
    void SetColor(DWORD dwColor) {m_dwColor = dwColor;}
protected:
    CMySize m_infoSize;
    char	m_szAniFile[256];
    int		m_nZoomInNum;
    int		m_nGrideSize;
    int		m_nRollSpeedX;
    int		m_nRollSpeedY;
    DWORD	m_dwVersion;
    DWORD	m_dwTimeBegin;
    int		m_nMoveRateX;
    int		m_nMoveRateY;
    BOOL	m_bRoll;
    DWORD	m_dwTimer;
    DWORD	m_dwColor;
public:
    // 采用脏矩形原理优化
    // 为防止频繁分配内存，使用栈空间
    static CMyRect	s_setRect[_MAX_DIRTY_RECT];
    static int		s_nRectAmount;

public:
    static void ClearRectSet() {s_nRectAmount = 0;}
    static BOOL TestRect(CMyRect rectInfo, BOOL bTerrain, BOOL bTransparent);
    static BOOL IsDirty(CMyRect rectInfo);

};
//------------------------------------------------
#endif

