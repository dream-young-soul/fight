
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _ANI_H
#define _ANI_H

#include "basefunc.h"
#include "..\DataModule\DataAniModule.h"
const int _MAX_FRAME	= 64;
class CMyBitmap;
class CAniFileSet;

#include <map>
#include <deque>
#include <vector>
using namespace std;
typedef vector<char*> VECTOR_ANILINE;

typedef struct
{
    VECTOR_ANILINE	setAniLine;
} AniIndexInfo;

typedef map<OBJID, AniIndexInfo>MAP_ANIINDEXINFO;

//typedef vector<AniIndexInfo*> VECTOR_ANIINDEXINFO;


enum {ANISTATUS_INIT = 0, ANISTATUS_LOADED, ANISTATUS_FAILED, ANISTATUS_LOADING};
class CAni
{
public:
    CAni	(void)	{Init();}
    virtual ~CAni	(void)	{Destroy();}

    void	Destroy		(void);

    BOOL	Create		(const char* pszDataFile, const char* pszIndex, BOOL bHalf = false, DWORD dwLoadWay = _LOADWAY_NORMAL);
    BOOL	Create		(AniIndexInfo* pInfo);
    BOOL	Create		(IFrameSet* pFrameSet);
    BOOL	Show		(UINT uFrame, int x, int y, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL) const;
    BOOL	ShowEx		(UINT uFrame, int x, int y, RECT* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha = 0, DWORD dwShowWay = _SHOWWAY_NORMAL);

    CMyBitmap*	GetFrame		(UINT uFrameIndex) const;
    int			GetFrameAmount	(void)	const {return m_iFrameAmount;}

    int			GetStatus		(void) const {return m_nStatus;}
    void		SetStatus		(int nStatus) {m_nStatus = nStatus;}
    DWORD		GetSize			(void) const;
    CMySize		GetSize			(UINT uFrame) const;

    // virtual...
    virtual	BOOL	Load		(void)	{return false;}
    virtual void	Free		(void)	{Destroy();}


private:
    volatile int m_nStatus;

    int			m_iFrameAmount;
    CMyBitmap*	m_pbmpFrame[_MAX_FRAME];

    void	Init (void);

public:
    BOOL	m_bHalf;

    static HANDLE	s_hHeap;
    static UINT		s_uNumAllocsInHeap;

    static BOOL		IsValidPt	(void* p);
    static BOOL		CheckAni	(const char* pszDataFile, const char* pszIndex);

    void*	operator new		(size_t size);
    void	operator delete		(void* p);
public:
    static IDai*		s_pDai;
    static IFrameSet*	s_pFrameSet;
    static CAniFileSet s_setAniFile;
};


typedef deque<CAni* >	DEQUE_ANI;
//-------------------------------------------------------------------

class CDynamicAni : public CAni
{
public:
    CDynamicAni(const char* pszIniFile, const char* pszIndex, BOOL bHalf = false);
    CDynamicAni(void)	{this->ResetLife();}
    virtual ~CDynamicAni() {}

    BOOL	Load(void);
    BOOL	SetAniFilename(const char* pszDataFile, const char* pszIndex);
    void	ResetLife() {m_dwTimeBorn = ::TimeGet();}

    BOOL	CheckAniByFilename(const char* pszDataFile, const char* pszIndex) const;
    DWORD   GetTimeBorn() const {return m_dwTimeBorn;}

    DWORD   GetLife() const {return m_dwLife;}
    void	SetLife(DWORD dwLife) {m_dwLife = dwLife; this->ResetLife();}

public:
    char m_szIniFile[_MAX_PATH];
    char m_szIndex[32];
    OBJID m_idAni;
    DWORD m_dwTimeBorn;
    DWORD m_dwLife;
};

typedef deque<CDynamicAni* >			DEQUE_DYNAMICANI;
typedef map<OBJID, CDynamicAni*>		MAP_DYNAMICANI;
//-------------------------------------------------------------------

class CAniFile
{
public:
    CAniFile();
    virtual ~CAniFile();

private:
    MAP_ANIINDEXINFO m_setAniIndexInfo;
public:
    char m_szFileName[_MAX_PATH];

private:
    void Destroy();
    AniIndexInfo* CreateNewAniIndexInfo();
    void DestroyAniIndexInfo(AniIndexInfo& pInfo);
    void AddAniIndexLine(AniIndexInfo* pInfo, char* pszLine);

public:
    BOOL Create(char* pszAniFile);
    AniIndexInfo* GetAniIndexInfo(char* pszIndex);

public:
    static CAniFile* CreateNew(char* pszAniFile);
};

typedef vector<CAniFile*> VECTOR_ANIFILE;
class CAniFileSet
{
public:
    CAniFileSet();
    virtual ~CAniFileSet();

private:
    VECTOR_ANIFILE m_setAniFile;

private:
    void Destroy();

public:
    void AddAniFile(char* pszAniFile);
    void DelAniFile(char* pszAniFile);
    AniIndexInfo* GetAniIndexInfo(char* pszAniFile, char* pszAniIndex);
};

#endif
