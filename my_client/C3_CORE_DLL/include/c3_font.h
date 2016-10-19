#ifndef _c3_font_h_
#define _c3_font_h_

#include "c3_main.h"
#include "c3_common.h"

#define FONT_VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
struct FontVertex
{
	float x, y;				// ÆÁÄ»×ø±ê
	float z;
	float rhw;
	DWORD color;			// ÑÕÉ«
	float u, v;				// ÌùÍ¼×ø±ê
};

struct	C3Char
{
	char					Char[2];
	LPDIRECT3DTEXTURE9		lpTex;
	DWORD					dwTime;
};

C3_CORE_DLL_API
void Char_Clear ( C3Char *lpChar );

struct	C3Font
{
    DWORD		*lpBuffer;
	HDC			hDC;
	HFONT		font;
	HBITMAP		bitmap;
	char		szName[256];
	int			nSize;
	int			nRealSize;

	DWORD		dwChars;
	C3Char		*lpChar[256][256];
};

C3_CORE_DLL_API
void Font_Clear ( C3Font *lpFont );

C3_CORE_DLL_API
BOOL Font_Create ( C3Font **lpFont, char *lpFontName, int nSize );

C3_CORE_DLL_API
void Font_Release ( C3Font **lpFont );

C3_CORE_DLL_API
void Font_Prepare ( void );

C3_CORE_DLL_API
BOOL Font_Draw ( C3Font *lpFont,
				 float fX,
				 float fY,
				 D3DCOLOR color,
				 char *lpChar );

#endif
