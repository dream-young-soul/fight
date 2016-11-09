#ifndef _c3_shape_h_
#define _c3_shape_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#include "c3_common.h"
#include "c3_phy.h"
#include "c3_main.h"

#define SHAPE_OUT_VERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
struct ShapeOutVertex
{
	float							x, y, z;
	//float							nx, ny, nz;
	DWORD							color;
	float							u, v;
};

struct ShapeBackupInfo 
{
	ShapeOutVertex* vb;
	DWORD			dwSegmentCur;
	
	D3DXVECTOR3		last[2];
	BOOL			bFrist;
};

struct C3Line
{
	DWORD			dwVecCount;
	D3DXVECTOR3*	lpVB;
};

struct C3SMotion
{
	DWORD							dwFrames;			// 帧数
	D3DXMATRIX						*lpFrames;			// 运动矩阵
	D3DXMATRIX						matrix;
	int								nFrame;				// 当前帧
};
C3_CORE_DLL_API
void SMotion_Clear ( C3SMotion *lpSMotion );
C3_CORE_DLL_API
BOOL SMotion_Load ( C3SMotion **lpSMotion, FILE *file );
C3_CORE_DLL_API
BOOL SMotion_LoadPack ( C3SMotion **lpSMotion, HANDLE f );
C3_CORE_DLL_API
BOOL SMotion_Save ( char *lpName, C3SMotion *lpSMotion, BOOL bNew );
C3_CORE_DLL_API
void SMotion_Unload ( C3SMotion **lpSMotion );

struct C3Shape
{
	char*			lpName;			// 名字
	
	DWORD			dwLineCount;
	C3Line*			lpLine;

	char			*lpTexName;			// [ be used by plugin ]
	int				nTex;

	C3SMotion		*lpMotion;

	// flash
	ShapeOutVertex*	vb;
	DWORD			dwSegment;
	DWORD			dwSegmentCur;
	
	D3DXVECTOR3		last[2];
	BOOL			bFrist;

	DWORD			dwSmooth;
	D3DXVECTOR3		*lpSmooths0;
	D3DXVECTOR3		*lpSmooths1;
	// flash

	// Use for tearAir
	LPDIRECT3DTEXTURE8	pTearAirTex;
	D3DXVECTOR3			*pScreenPnt;
	RECT				TearAirTexRect;
	RECT				LastTearAirTexRect;
};

C3_CORE_DLL_API
void Shape_Clear ( C3Shape *lpShape );
C3_CORE_DLL_API
BOOL Shape_Load ( C3Shape **lpShape, FILE *file, BOOL bTex = false );
C3_CORE_DLL_API
BOOL Shape_LoadPack ( C3Shape **lpShape, HANDLE f, BOOL bTex = false );
C3_CORE_DLL_API
BOOL Shape_Save ( char *lpName, C3Shape *lpShape, BOOL bNew );
C3_CORE_DLL_API
void Shape_Unload ( C3Shape **lpShape );

/*C3_CORE_DLL_API
void Shape_Link ( C3Shape *lpShape, D3DXMATRIX *lpMatrix );*/
C3_CORE_DLL_API
void Shape_SetSegment ( C3Shape *lpShape, DWORD dwSegment, DWORD dwSmooth = 1 );
C3_CORE_DLL_API
BOOL Shape_Draw ( C3Shape *lpShape, BOOL bLocal = false, int nAsb=5, int nAdb=6);
C3_CORE_DLL_API
void Shape_Prepare ( void );
C3_CORE_DLL_API
void Shape_ChangeTexture ( C3Shape *lpShape, int nTexID );
C3_CORE_DLL_API
void Shape_Muliply ( C3Shape *lpShape, D3DXMATRIX *matrix );
C3_CORE_DLL_API
void Shape_NextFrame ( C3Shape *lpShape, int nStep );
C3_CORE_DLL_API
void Shape_SetFrame ( C3Shape *lpShape, DWORD dwFrame );
C3_CORE_DLL_API
void Shape_ClearMatrix ( C3Shape *lpShape );
C3_CORE_DLL_API
BOOL Shape_DrawAlpha(C3Shape *lpShape, BOOL bLocal);
#endif