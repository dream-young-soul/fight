
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef _WEATHER_HEAD_KATTY
#define _WEATHER_HEAD_KATTY

#include <windows.h>
#include <Windef.h>
#include "Ani.h"
#include <deque>
#include "firebug.h"
#include "nd_bitmap.h"
#include "cloud.h"
using namespace std;
class CAni;
const int _MAX_AMOUNT			= 512;//1024;
const int FLYING_MAX_AMOUNT		= 100;
const int LEAF_MAX_AMOUNT		= 200;
const int _LEAF_FRAME			= 24;
const int WORMTYPE				= 10;
const int CLOUDTYPE				= 11;
const int FLYINGTYPE			= 2;
const int DANDELIONTYPE			= 1;
const int FLYFLOWER_FRAME		= 24;
const int FLYFLOWER_PER_LAYER	=  6;


enum { __RAIN = 1, __SNOW = 2, __LEAF_G = 3, __LEAF_R = 4, __LEAF_Y = 5, __LEAF_Y_R = 6, __LEAF_5 = 7, __LEAF_6 = 8, __LEAF_7 = 9, __FLOWER_1 = 10, __FLOWER_2 = 11, __FLOWER_3 = 12, __SAND = 13, __FLYING_1, __FLYING_2, __FLYING_3, __FLYING_4, __DANDELION, __WORM, __CLOUD};
enum {_RAINSTATUS_NONE	= 0, _RAINSTATUS_CREATE, _RAINSTATUS_SPLASH};
enum {_LEAFSTATUS_NONE	= 0, _LEAFSTATUS_CREATE, _LEAFSTATUS_SPLASH};
enum {_SNOWSTATUS_NONE	= 0, _SNOWSTATUS_CREATE, _SNOWSTATUS_SPLASH};
enum {_SANDSTATUS_NONE	= 0, _SANDSTATUS_CREATE, _SANDSTATUS_SPLASH};
enum {_FLYFLOWERSTATUS_NONE	= 0, _FLYFLOWERSTATUS_CREATE, _FLYFLOWERSTATUS_STOP, _FLYFLOWERSTATUS_BEGIN, FLYFLOWERSTATUS_PAUSE, FLYFLOWERSTATUS_RESET};
enum {_LEAF_BEGIN = 11, _LEAF_STOP = 10 , _LEAF_RESET = 13, _LEAF_NONE = 14, _LEAF_PAUSE};

class CLeaf
{
public:
    CLeaf();
    ~CLeaf()	{Destroy();}

    BOOL	Create	(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity);
    void	Show	( );
    void	Process	( );
    void	Destroy	(void);

    BOOL	IsCreated	(void) {if (m_iStatus) { return TRUE; } else { return FALSE; }}
    int		GetInt		(double fValue);
    void	GetSplashPos(int& iPosX, int& iPosY)	{iPosX = m_iSplashX; iPosY = m_iSplashY;}
    void	SetPicture	(char* pszFileName)	{strcpy(m_szFileName, pszFileName);}
private:

    int		m_iLayerFrame;
    int		m_iStatus;
    double	m_dwElapseTime;
    DWORD	m_dwLifeTime;
    DWORD	m_dStayGroundTime;

    int		m_iSize;
    int		m_iSceneDeepth;

    int		m_iStartX, m_iStartY;
    int		m_iSplashX, m_iSplashY;
    int		m_iLastPosX, m_iLastPosY;
    int		m_iLastFrame;

    int		m_fGravity;
    int		m_fSpeed;
    int		m_fWind;

    int		m_ibmpFrameIndex;
    char 	m_szFileName[100];
    int		m_bLieOnGround;
public:
    int		m_nLevel;
    int		m_fSinA, m_fCosA;
    int		m_iScreenX, m_iScreenY;
    int		m_nLoopCount;
    DWORD	m_dwLifeBeginTime;

};


typedef deque<CLeaf*> CLEAF;


class CLeafSys
{
public:
    CLeafSys(void)	{m_iStatu = _LEAF_NONE; }
    ~CLeafSys(void) {Destroy();}

    BOOL Create	(int nWindDir, int nLevel, int nType);
    void Process(int& nStatus);

private:
    char 	m_szFileName[100];
    int		m_iLeafAmount;
    int		m_iTargetAmount;
    CLEAF	m_Leaf;

    int m_nWindDirection;
    int	m_iStatu;

public:

    void LeafStop();
    void Destroy();
    void SetLeaf(int nWindDir, int nLevel);
    DWORD	m_dwPauseTime;
    void Pause();
    void Continue();

};



class CRain
{
public:
    CRain();
    ~CRain()	{Destroy();}

    BOOL	Create	(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity, int nLevel, DWORD dwColor);
    void	Show	();
    void	Process	();
    void	Reset	(void);
    void	Destroy	(void);

    BOOL	IsCreated	(void) {if (m_iStatus) { return TRUE; } else { return FALSE; }}
    int		GetInt		(double fValue);
    void	GetSplashPos(int& iPosX, int& iPosY)	{iPosX = m_iSplashX; iPosY = m_iSplashY;}

    void	SetRipple	(char  szFileName[2][100])	{strcpy(m_szFileName[0], szFileName[0]); strcpy(m_szFileName[1], szFileName[1]);}

private:
    int		m_iStatus;
    DWORD	m_dwElapseTime;
    DWORD	m_dwLifeTime;
    int		m_iSize;
    int		m_iSceneDeepth;
    int		m_iStartX, m_iStartY;
    int		m_iSplashX, m_iSplashY;
    int		m_iLastPosX, m_iLastPosY;

    int		m_fGravity;
    int		m_fSpeed;
    int		m_fWind;
    int		m_iRippleFrameIndex;
    char	m_szFileName[2][100];
    int		m_bOnWater;
public:
    DWORD	m_dwColor;
    int		m_nLevel;
    int		m_fSinA, m_fCosA;
    DWORD	m_dwLifeBeginTime;

};




typedef deque<CRain*> CRAIN;
enum {_RAIN_BEGIN = 11, _RAIN_STOP = 10 , _ADD_AMOUNT = 12 , _RAIN_RESET = 13, _RAIN_NONE = 14, _RAIN_PAUSE};
class CRainSys
{
public:
    CRainSys(void);
    ~CRainSys(void) {Destroy();}

    BOOL Create	(int nWindDir, int nLevel, DWORD dwColor);
    void Process(int& nStatus);
    void Reset	(void);
    void PlayRainSound(int iLevel);
    void	RainStop();
    void	Destroy();
    void	SetRain(int nWindDir, int nLevel);
    void	SetColor (DWORD dwColor);
    DWORD	m_dwPauseTime;
    void	Pause();
    void	Continue();
    int		RendBMP(DWORD dwColor);

private:

    DWORD	m_dwColor;
    char	m_szFileName[2][100];
    int		m_iRainAmount;
    int		m_iTargetAmount;
    CRAIN	m_Rain;
    int		m_nWindDirection;
    int		m_iStatu;

};



////////////////////////////////////////////////////////////////////////////////////////////
//snow
////////////////////////////////////////////////////////////////////////////////////////////



class CSnow
{
public:
    CSnow();
    ~CSnow()	{Destroy();}

    BOOL	Create	(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity);
    void	Process	();
    void	Destroy	(void);
    void	Reset	(void);

    void	ShowImage	( int iPosX, int iPosY );
    void	Show		(void );

    BOOL	IsCreated	(void) {if (m_iStatus) { return TRUE; } else { return FALSE; }}
    void	GetSplashPos(int& iPosX, int& iPosY)	{iPosX = m_iSplashX; iPosY = m_iSplashY;}
    void	SetImageBmp		(char* pszFileName)	{strcpy(m_szFileName, pszFileName);}
private:
    int		m_iStatus;
    DWORD	m_dwElapseTime;
    DWORD	m_dwLifeTime;
    int		m_iSceneDeepth;

    int		m_iStartX, m_iStartY;
    int		m_iSplashX, m_iSplashY;
    int		m_iLastPosX, m_iLastPosY;
    int		m_fGravity;
    int		m_fSpeed;
    int		m_fWind;
    int		m_nStayTick;
    char 	m_szFileName[100];
public:
    int	   m_fSinA, m_fCosA;
    DWORD	m_dwLifeBeginTime;
};


typedef deque<CSnow*> CSNOW;
enum { _SNOW_STOP = 0, _SNOW_BEGIN = 1, _SNOW_NONE = 2, _SNOW_RESET = 3, _SNOW_PAUSE};
class CSnowSys
{
public:
    CSnowSys(void)	{Init();}
    ~CSnowSys(void) {Destroy();}
    void Pause();
    void Continue();

    void Init	(void);
    void Destroy(void);
    BOOL Create	(int iWindDir, int iLevel, DWORD dwColor);
    void Process(int& nStatus);
    void Reset	(void);
    void SnowStop();
    void SetSnow(int nWindDir, int nLevel);

private:
    char		m_szFileName[100];
    int			m_iSnowAmount;
    CSNOW		m_Snow;
    int			m_iStatu;
    int			m_iWindDir;
    int			m_iTargetAmount;
    DWORD		m_dwPauseTime;
};



//------------------------------------------------------------------
//沙
//------------------------------------------------------------------

class CSand
{
public:
    CSand();
    ~CSand()	{Destroy();}

    BOOL	Create	(int iPosX, int iPosY, int iSceneDeepth, int fWind, int fGravity, int nLevel);
    void	Show	();
    void	Process	();
    void	Reset	(void);
    void	Destroy	(void);

    BOOL	IsCreated	(void) {if (m_iStatus) { return TRUE; } else { return FALSE; }}
    int		GetInt		(double fValue);
    void	GetSplashPos(int& iPosX, int& iPosY)	{iPosX = m_iSplashX; iPosY = m_iSplashY;}

private:
    int		m_iStatus;
    DWORD	m_dwElapseTime;
    DWORD	m_dwLifeTime;
    int		m_iSize;
    int		m_iSceneDeepth;
    int		m_iStartX, m_iStartY;
    int		m_iSplashX, m_iSplashY;
    int		m_fGravity;
    int		m_fSpeed;
    int		m_fWind;
    int		m_ibmpFrameIndex;
public:
    int		m_nLevel;
    int		m_fSinA, m_fCosA;
    int		m_nDir;
    DWORD	m_dwLifeBeginTime;

};


typedef deque<CSand*> CSAND;
enum {_SAND_BEGIN = 11, _SAND_STOP = 10 , _SAND_ADD_AMOUNT = 12 , _SAND_RESET = 13, _SAND_NONE = 14, _SAND_PAUSE};

class CSandSys
{
public:
    CSandSys(void)	{m_iStatu = _SAND_NONE;}
    ~CSandSys(void) {Destroy();}

    BOOL Create	(int nWindDir, int nLevel);
    void Process(int& nStatus);
    void Reset	(void);


private:

    int		m_iSandAmount;
    int		m_iTargetAmount;
    CSAND	m_Sand;

    int		m_nWindDirection;
    int		m_nLevel;
    int		m_iStatu;

public:
    void	SandStop();
    void	Destroy();
    void	SetSand(int nWindDir, int nLevel);
    void	Pause();
    void	Continue();
    DWORD 	m_dwPauseTime;

};




//--------------------------------------------------------------
//
//烟花.
//------------------------------------------------------------------------

enum {_YINFASTATUS_NONE	= 0, _YINFASTATUS_CREATE, _YINFASTATUS_FALL , _YINFASTATUS_START, _YINFASTATUS_END};
enum {_YINFA_BEGIN = 10, _YINFA_STOP = 11 , _YINFA_RESET = 13, _YINFA_NONE = 14};


const int _YINFA_FRAME	= 8;

class CYinFa
{
public:
    CYinFa() { m_iStatus	= _YINFASTATUS_NONE;}
    ~CYinFa()	{Destroy();}

    BOOL	Create	(int iOrigPosX, int iOrigPosY, int nMarginX, int nMarginY, int nRadiuint);
    void	Show	();
    void	Process	();
    void	Reset	(void);
    void	Destroy	(void);

    int 	IsCreated	(void) {return m_iStatus;}
    int		GetInt		(double fValue);
    void	SetPicture	(char* pszFileName)	{strcpy(m_szFileTitle, pszFileName);}

private:
    int		m_iShowNum;
    int		m_iStatus;
    int		m_nPathCount;    // 粒子向四周散开的已用的贞数
    int 	m_nPathLength;   //向四周散开要用的贞数

    POINT	m_nCenterWorldPoint;
    POINT	m_nMarginWorldPoint;
    POINT	m_nFallWorldPoint;

    POINT	m_nStartWoldPoint;
    float	m_fRadius;
    int		m_nFallStep;  // 下落已用的贞数
    int		m_nFallLenght;
    int		m_nRadius;
    char	m_szFileTitle[100];
    DWORD	m_dwLifeBeginTime;
public:
    float   m_fSinA, m_fCosA;

};


typedef deque<CYinFa*> DequeYinFa;
class CYinFaSys
{
public:
    CYinFaSys(void)	{m_nCreated = 0; }
    ~CYinFaSys(void) {Destroy();}
    BOOL Create	(int nScreenX, int nScreenY, int nNum, int nShowNum, int nRadius, int nType, DWORD dwDelayTime);
    void Process();
    void Destroy();

private:
    int 	m_nOrigWorldPointX, m_nOrigWorldPointY;
    double	m_fRadius;
    DWORD	m_dwCreateTime;

    DWORD	m_dwDelayTime;
    DWORD	m_dwBeginTime;
    char	m_szFileTitle[100];

    DequeYinFa	m_YinFa;
    int		m_iStatu;
    int		m_nYinFaTotal;
    int		m_nYinFaUsed;
    int		m_nYinFaShowNum;
    int		m_nYinFaType;
    int		m_nInterval;
private:
    void YinFaStop();
public:
    POINT m_nShowCenter[20];
    int m_nCreated;

};


typedef deque<POINT*> DequePoint;

//--------------------------------------------------------------
//
//烟花2
//------------------------------------------------------------------------



class CYinFa2
{
public:
    CYinFa2() { m_iStatus	= _YINFASTATUS_NONE;}
    ~CYinFa2()	{Destroy();}

    BOOL	Create	(int iOrigPosX, int iOrigPosY, int nMarginX, int nMarginY, int nRadiuint);
    void	Show	();
    void	Process	();
    void	Reset	(void);
    void	Destroy	(void);

    int		IsCreated	(void) {return m_iStatus;}
    int		GetInt		(double fValue);
    void	SetPicture	(char* pszFileName)	{strcpy(m_szFileTitle, pszFileName);}

private:
    int		m_iShowNum;
    int		m_iStatus;
    int		m_nPathCount;    // 粒子向四周散开的已用的贞数
    int 	m_nPathLength;    //向四周散开要用的贞数

    POINT	m_nCenterWorldPoint;
    POINT	m_nMarginWorldPoint;
    POINT	m_nFallWorldPoint;

    POINT	m_nStartWoldPoint;
    float	m_fRadius;
    int		m_nFallStep;  // 下落已用的贞数
    int		m_nFallLenght;
    int		m_nRadius;
    char	m_szFileTitle[100];
    DWORD	m_dwLifeBeginTime;
    DequePoint m_dequePoint;

public:
    float   m_fSinA, m_fCosA;

};


typedef deque<CYinFa2*> DequeYinFa2;
class CYinFaSys2
{
public:
    CYinFaSys2(void)	{m_nCreated = 0; }
    ~CYinFaSys2(void) {Destroy();}

    BOOL Create	(int nScreenX, int nScreenY, int nNum, int nShowNum, int nRadius, int nType, DWORD dwDelayTime);
    void Process();
    void Destroy();


private:
    int 	m_nOrigWorldPointX, m_nOrigWorldPointY;
    double	m_fRadius;
    DWORD	m_dwCreateTime;
    DWORD	m_dwDelayTime;
    DWORD	m_dwBeginTime;
    char	m_szFileTitle[100];
    DequeYinFa2		m_YinFa;
    int		m_iStatu;
    int		m_nYinFaTotal;
    int		m_nYinFaUsed;
    int		m_nYinFaShowNum;
    int		m_nYinFaType;
    int		m_nInterval;
private:
    void YinFaStop();
public:
    int m_nCreated;

};




typedef deque<CYinFaSys*> DequeFireWork;
class CFireWork
{
public:
    CFireWork() {}
    ~CFireWork() {Destroy();}
    void Destroy();
    void Create(int nScreenX, int nScreenY, int nNum,  int nRadius, int nType, DWORD dwDelayTime);
    void Process();
private:
    DequeFireWork dequeFireWork;


};


typedef deque<CYinFaSys2*> DequeFireWork2;
class CFireWork2
{
public:
    CFireWork2() {}
    ~CFireWork2() {Destroy();}
    void Destroy();
    void Create(int nScreenX, int nScreenY, int nNum,  int nRadius, int nType, DWORD dwDelayTime);
    void Process();
private:
    DequeFireWork2 dequeFireWork2;

};




class Firework
{
public:
    Firework() {}
    ~Firework() {Destroy();}
    void Destroy();
    void Create(int nScreenX, int nScreenY, int nNum,  int nRadius, int nType , int color, DWORD dwDelayTime);
    void Process();
private:
    CFireWork2 firework2;
    CFireWork  firework1;
    int m_nType;
};



//-----------------------------------------------------------------------
// fly flower
//-----------------------------------------------------------------------






class CFlyFlower
{
public:
    CFlyFlower() { m_iStatus	= _FLYFLOWERSTATUS_NONE;}
    ~CFlyFlower()	{Destroy();}
    void  InitialLayer();

    BOOL	Create	( int nCurFrame, int type);
    void	Show	();
    void	Destroy	(void);
    BOOL	IsCreated	(void) {if (m_iStatus) { return TRUE; } else { return FALSE; }}
    void	SetPicture	(char* pszFilename)	{strcpy(m_szFileTitle, pszFilename);}
    int		GetY(int nx);

private:

    int		m_iStatus;
    POINT	m_nCurWorldPoint;
    POINT	m_nShowWorldPoint;
    int		m_nLayer;
    int		m_nCurState;
    int		m_nCurFrame;
    int		m_nCurX;
    int		m_nCurY;
    int		m_nSpeed;
    int		m_nLoopCount;
    int		m_nType;
    char 	m_szFileTitle[100];
    int		m_nA, m_nB;

};



typedef deque<CFlyFlower*> DequeCFlyFlower;
class CSysFlyFlower
{
public:

    CSysFlyFlower(void)	{m_nCreated = 0; m_iStatu = _FLYFLOWERSTATUS_NONE; }
    ~CSysFlyFlower(void) {Destroy();}

    BOOL Create	( int nNum, int nType, DWORD dwDelayTime);
    void Process(int& nStatus);
    void Destroy();
    void Stop();
    void ReSet(int inum);
    void Pause();
    void Continue();

private:
    int			m_iCurNum;
    int			m_iTargetNum;
    int			m_iStatu;
    int			m_nType;
    DWORD		m_dwCreateTime;
    DWORD		m_dwDelayTime;
    DWORD		m_dwBeginTime;
    char		m_szFileTitle[100];
    int			m_nSpeed;
    DequeCFlyFlower		m_dequeFlyFlower;

public:
    int m_nCreated;

};


//---------------------------------------------------------------------------
//天气
//---------------------------------------------------------------------------------
class CWeather
{
public:
    CWeather() { m_nType = 0;}
    ~CWeather() {Destroy();}
    int m_nType;
    BOOL CreateEnv(int nType, int nWindDir, int nLevel, DWORD dwColor = 0x00ffffff);
    void SetColor(DWORD dwColor);
    void SetEnv(int nWindDir, int nLevel);
    BOOL StopEnv();
    void EnvShow( );
    void Destroy();
    void Pause();
    void Continue();
private:
    CSnowSys m_objSnow;
    CRainSys m_objRain;
    CLeafSys m_objLeaf;
    CSandSys m_objSand;
    CSysFlyFlower m_objFlying;
    CSysFireBug m_objFlyBug;
    CCloudSys	m_objCloud;
};




enum {_FIREWORKLETTER_NONE	= 0, _FIREWORKLETTER_CREATE, _FIREWORKLETTER_FALL, _FIREWORKLETTER_BEGIN  , _FIREWORKLETTER_STOP , _FIREWORKLETTER_LETTER, _FIREWORKLETTER_END };

const int	_FIREWORKLETTER_FRAME = 8 ;
class CFireworkLetter
{
public:
    CFireworkLetter() { m_iStatus	= _FIREWORKLETTER_NONE;}
    ~CFireworkLetter()	{Destroy();}

    BOOL	Create	(int iOrigPosX, int iOrigPosY, int nMarginX, int nMarginY);
    void	Show	();
    void	Process	();
    void	Reset	(void);
    void	Destroy	(void);

    int	IsCreated	(void) {return m_iStatus;}
    void	SetPicture	(char* pszFileName)	{strcpy(m_szFileTitle, pszFileName);}

private:

    int		m_iStatus;
    int		m_nPathCount;		// 粒子向四周散开的已用的贞数
    int 	m_nPathLength;		//向四周散开要用的贞数

    POINT	m_nCenterWorldPoint;
    POINT	m_nMarginWorldPoint;
    POINT	m_nFallWorldPoint;
    float	m_fRadius;
    int		m_nFallStep;		// 下落已用的贞数
    int		m_nFallLenght;
    char	m_szFileTitle[100];

};


typedef deque<CFireworkLetter*> DequeFireworkLetter;
typedef deque<POINT* > DequePoint;
class CFireworkLetterSys
{
public:
    CFireworkLetterSys(void)	{m_nCreated = 0; }
    ~CFireworkLetterSys(void) {Destroy();}

    BOOL Create	(int nScreenX, int nScreenY, char* szLetter , int nType, DWORD dwDelayTime);
    void Process();
    void Destroy();


private:
    char szLetter[20];
    int 	m_nOrigWorldPointX, m_nOrigWorldPointY;
    double	m_fRadius;
    char	m_szFileTitle[100];

    DequeFireworkLetter	m_firework;
    int		m_iStatu;
    int		m_nType;
    DequePoint pPoint ;
    DWORD	m_dwDelayTime;
    DWORD	m_dwBeginTime;

private:
    void  Stop();
    int		GetBmpPoint(LPSTR lpfilename, int X, int Y);
    int		test(DWORD a, int k);
    BOOL	GetPoint(char* zw, int X, int Y);
public:
    int m_nCreated;

};


typedef deque<CFireworkLetterSys*> DequeFireworkChar;
class CFireWorkChar
{
public:
    CFireWorkChar() {}
    ~CFireWorkChar() {Destroy();}
    void Destroy();
    void Create(int nScreenX, int nScreenY, char* szLetter , int nType, DWORD dwDelayTime);
    void Process();
private:
    DequeFireWork2 m_dequeFireWork;
    DequeFireworkChar m_dequeFireworkChar;

};


#endif
