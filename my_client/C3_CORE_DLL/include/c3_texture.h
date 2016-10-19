#ifndef _c3_texture_h_
#define _c3_texture_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#include "c3_common.h"

typedef struct
{
	int							nID;
	int							nDupCount;		// 复制数
	char						*lpName;		// 文件名
	LPDIRECT3DTEXTURE9			lpTex;			// 贴图地址
	D3DXIMAGE_INFO				Info;			// 贴图信息
}
C3Texture;

C3_CORE_DLL_API
void Texture_Clear ( C3Texture *lpTex );
/*
	读取贴图
	--------
	lpTex		贴图指针
	lpName		文件名
	lMipLevels	mipmap 级别，却省为最高级别
*/
C3_CORE_DLL_API
int Texture_Load ( C3Texture **lpTex,
				   char *lpName,
				   DWORD dwMipLevels = 3,
				   D3DPOOL pool = D3DPOOL_MANAGED,
				   BOOL bDuplicate = true,
				   D3DCOLOR colorkey = 0 );
C3_CORE_DLL_API
void Texture_Unload ( C3Texture **lpTex );
C3_CORE_DLL_API
BOOL Texture_Update ( C3Texture *lpSrcTex, C3Texture *lpDestTex );
C3_CORE_DLL_API
BOOL Texture_Create ( C3Texture **lpTex,
					  DWORD dwWidth,
					  DWORD dwHeight,
					  DWORD dwMipLevels,
					  D3DFORMAT format,
					  D3DPOOL pool );

#define	TEX_MAX			10240
extern C3_CORE_DLL_API DWORD		g_dwTexCount;
extern C3_CORE_DLL_API C3Texture	*g_lpTex[TEX_MAX];

#endif
