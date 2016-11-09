#ifndef _c3_sprite_h_
#define _c3_sprite_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#include "c3_common.h"
#include "c3_texture.h"
#include "c3_main.h"

/* 精灵物体顶点定义 */
#define SPRITE_VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
struct SpriteVertex
{
	float x, y;				// 屏幕坐标
	float z;
	float rhw;
	DWORD color;			// 颜色
	float u, v;				// 贴图坐标
};

typedef struct
{
	SpriteVertex		vertex[4];
	C3Texture			*lpTex;
} C3Sprite;

C3_CORE_DLL_API
void Sprite_Clear ( C3Sprite *lpSprite );
C3_CORE_DLL_API
BOOL Sprite_Load ( C3Sprite **lpSprite,
				   char *lpName,
				   D3DPOOL pool = D3DPOOL_MANAGED,
				   BOOL bDuplicate = true,
				   D3DCOLOR colorkey = 0 );
C3_CORE_DLL_API
BOOL Sprite_Create ( C3Sprite **lpSprite,
				     DWORD dwWidth,
					 DWORD dwHeight,
					 DWORD dwMipLevels,
					 D3DFORMAT format,
					 D3DPOOL pool );
C3_CORE_DLL_API
void Sprite_Unload ( C3Sprite **lpSprite );

C3_CORE_DLL_API
void Sprite_SetCoor ( C3Sprite *lpSprite,
					  RECT *lpSrc,
					  int nX,
					  int nY,
					  DWORD dwWidth = 0,
					  DWORD dwHeight = 0 );
C3_CORE_DLL_API
void Sprite_SetColor ( C3Sprite *lpSprite, BYTE a, BYTE r, BYTE g, BYTE b );
C3_CORE_DLL_API
void Sprite_SetVertexColor ( C3Sprite *lpSprite, DWORD ltColor, DWORD rtColor, DWORD lbColor, DWORD rbColor );

C3_CORE_DLL_API
void Sprite_Mirror ( C3Sprite *lpSprite );

C3_CORE_DLL_API
void Sprite_Prepare ( void );
C3_CORE_DLL_API
BOOL Sprite_Draw ( C3Sprite *lpSprite, DWORD dwShowWay = 0 );

C3_CORE_DLL_API
void Sprite_Lock ( C3Sprite *lpSprite, RECT *lpRect, D3DLOCKED_RECT *lpReturn );
C3_CORE_DLL_API
void Sprite_Unlock ( C3Sprite *lpSprite );

C3_CORE_DLL_API
BOOL Sprite_Draw (C3Sprite *lpSpriteUp, C3Sprite *lpSpriteDn, UCHAR uAlphaA, UCHAR uAlphaB, UCHAR uAlphaC, UCHAR uAlphaD);

C3_CORE_DLL_API
BOOL Sprite_Save(C3Sprite* lpSprite,char* szName);
#endif