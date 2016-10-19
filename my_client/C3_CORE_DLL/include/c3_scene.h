#ifndef _c3_scene_h_
#define _c3_scene_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#include "c3_common.h"
#include "c3_texture.h"

/* 场景物体顶点定义 */
#define SCENE_VERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 )
struct SceneVertex
{
	float	x, y, z;		// 坐标
	float	nx, ny, nz;		// 法线
	float	u, v;			// 贴图坐标
	float	lu, lv;
};
struct SceneEdge
{
	DWORD	dwStartVec;
	DWORD	dwEndVec;
};

typedef struct
{
	char						*lpName;		// 名字

	DWORD						dwVecCount;		// 顶点数
	SceneVertex					*lpVB;			// 顶点池
	DWORD						dwTriCount;		// 多边形数
	WORD						*lpIB;			// 索引池

	char						*lpTexName;		// [ be used by plugin ]
	int							nTex;

	char						*lplTexName;	// [ be used by plugin ]
	int							nlTex;

	DWORD						dwFrameCount;
	D3DXMATRIX					*lpFrame;
	int							nFrame;

	D3DXMATRIX					matrix;
} C3Scene;

C3_CORE_DLL_API
void Scene_Clear ( C3Scene *lpScene );
C3_CORE_DLL_API
BOOL Scene_Load ( C3Scene **lpScene,
				  char *lpName,
				  DWORD dwIndex );
C3_CORE_DLL_API
BOOL Scene_Save ( char *lpName, C3Scene *lpScene, BOOL bNew );
C3_CORE_DLL_API
void Scene_Unload ( C3Scene **lpScene );

/* 优化场景 */
C3_CORE_DLL_API
BOOL Scene_Optimize ( C3Scene *lpScene );

C3_CORE_DLL_API
void Scene_Prepare ( void );
C3_CORE_DLL_API
BOOL Scene_Draw ( C3Scene *lpScene );

C3_CORE_DLL_API
void Scene_NextFrame ( C3Scene *lpScene, int nStep );

C3_CORE_DLL_API
void Scene_Muliply ( C3Scene *lpScene, D3DXMATRIX *matrix );

#endif
