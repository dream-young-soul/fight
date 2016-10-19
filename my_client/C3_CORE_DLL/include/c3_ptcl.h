#ifndef _c3_ptcl_h_
#define _c3_ptcl_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#include "c3_main.h"
#include "c3_sprite.h"
#include "c3_phy.h"

/* 粒子物体顶点定义 */
#define PTCL_VERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
struct PtclVertex
{
	float x, y, z;
	DWORD color;			// 颜色
	float u, v;				// 贴图坐标
};

struct	PtclFrame
{
	DWORD						dwCount;
	D3DXVECTOR3					*lpPos;
	float						*lpAge;
	float						*lpSize;
	D3DXMATRIX					matrix;
};

struct C3Ptcl
{
	char						*lpName;		// 名字
	PtclVertex					*lpVB;
	WORD						*lpIB;
	int							nTex;
	char						*lpTexName;		// [ be used by plugin ]
	DWORD						dwCount;		// [ be used by plugin ]
	DWORD						dwRow;

	PtclFrame					*lpPtcl;
	int							nFrame;
	DWORD						dwFrames;

	D3DXMATRIX					matrix;
};
C3_CORE_DLL_API
void Ptcl_Clear ( C3Ptcl *lpPtcl );
C3_CORE_DLL_API
BOOL Ptcl_Load ( C3Ptcl **lpPtcl,
				 FILE *file,
				 BOOL bTex = false );
C3_CORE_DLL_API
BOOL Ptcl_LoadPack ( C3Ptcl **lpPtcl,
					 HANDLE f,
					 BOOL bTex = false );
C3_CORE_DLL_API
BOOL Ptcl_Save ( char *lpName, C3Ptcl *lpPtcl, BOOL bNew );
C3_CORE_DLL_API
void Ptcl_Unload ( C3Ptcl **lpPtcl );

C3_CORE_DLL_API
void Ptcl_Prepare ( void );
C3_CORE_DLL_API
BOOL Ptcl_Draw ( C3Ptcl *lpPtcl, int nAsb=5, int nAdb=6);

C3_CORE_DLL_API
void Ptcl_NextFrame ( C3Ptcl *lpPtcl, int nStep );
C3_CORE_DLL_API
void Ptcl_SetFrame ( C3Ptcl *lpPtcl, DWORD dwFrame );
C3_CORE_DLL_API
void Ptcl_Muliply ( C3Ptcl *lpPtcl, D3DXMATRIX *matrix );
C3_CORE_DLL_API
void Ptcl_ClearMatrix ( C3Ptcl *lpPtcl );

C3_CORE_DLL_API
void Ptcl_ChangeTexture ( C3Ptcl *lpPtcl, int nTexID );

#endif
