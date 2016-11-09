#include "..\include\c3_font.h"

//#define BIG5


const int SIZE_FONTBASE = 16;

int GetFontsize(int nSize)
{
	if(nSize <= 16)
		return 16;
	if(nSize <= 32)
		return 32;
	if(nSize <= 64)
		return 64;
	return 64;
}

C3_CORE_DLL_API
void Char_Clear ( C3Char *lpChar )
{
	lpChar->Char[0] = '\0';
	lpChar->Char[1] = '\0';
	lpChar->lpTex = 0;
}

C3_CORE_DLL_API
void Font_Clear ( C3Font *lpFont )
{
	lpFont->lpBuffer = 0;
	lpFont->hDC	= 0;
	
	lpFont->nSize = GetFontsize(lpFont->nSize);
	lpFont->nRealSize = GetFontsize(lpFont->nSize);

	lpFont->dwChars = 0;
	memset ( lpFont->lpChar, 0, sizeof ( C3Char* ) * 256 * 256 );
}

C3_CORE_DLL_API
BOOL Font_Create ( C3Font **lpFont, char *lpFontName, int nSize )
{

	HFONT font;
	font = ::CreateFont ( nSize,
						  0,
						  0,
						  0,
						  0,
						  0,
						  0,
						  0,
#ifdef BIG5
						  CHINESEBIG5_CHARSET,
#else
						  GB2312_CHARSET,
#endif
						  OUT_DEFAULT_PRECIS,
						  CLIP_DEFAULT_PRECIS,
						  ANTIALIASED_QUALITY,
						  FIXED_PITCH,
						  lpFontName );

	if ( !font )
		return false;

	*lpFont = new C3Font;
	if (!(*lpFont))
		return false;

	Font_Clear ( *lpFont );

    BITMAPINFO bi;
    ZeroMemory ( &bi.bmiHeader, sizeof ( BITMAPINFOHEADER ) );
    bi.bmiHeader.biSize		= sizeof ( BITMAPINFOHEADER );
    bi.bmiHeader.biWidth	= GetFontsize(nSize);
    bi.bmiHeader.biHeight	= -GetFontsize(nSize);
    bi.bmiHeader.biPlanes	= 1;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biBitCount = 32;

    ( *lpFont )->hDC = CreateCompatibleDC ( NULL );
    ( *lpFont )->bitmap = CreateDIBSection ( ( *lpFont )->hDC,
											 &bi,
											 DIB_RGB_COLORS,
											 ( void** )&( *lpFont )->lpBuffer,
											 0,
											 0 );
	( *lpFont )->font = font;
    SetMapMode ( ( *lpFont )->hDC, MM_TEXT );

    SelectObject ( ( *lpFont )->hDC, ( *lpFont )->bitmap );
    SelectObject ( ( *lpFont )->hDC, font );

    SetTextColor ( ( *lpFont )->hDC, RGB ( 0, 255, 255 ) );
    SetBkColor ( ( *lpFont )->hDC, 0x00000000 );
    SetTextAlign ( ( *lpFont )->hDC, TA_TOP );

	( *lpFont )->nSize = GetFontsize(nSize);
	( *lpFont )->nRealSize = nSize;
	strcpy(( *lpFont )->szName, lpFontName);

	return true;
}

C3_CORE_DLL_API
void Font_Release ( C3Font **lpFont )
{
	if (!lpFont || !(*lpFont))
		return;

	::DeleteObject((*lpFont)->bitmap);
	::DeleteObject((*lpFont)->font);
	::DeleteDC((*lpFont)->hDC);

	for (int j=0; j<256; j++)
	{
		for (int i=0; i<256; i++)
		{
			if ((*lpFont)->lpChar[i][j])
			{
				SafeRelease(((*lpFont)->lpChar[i][j])->lpTex);
				SafeDelete((*lpFont)->lpChar[i][j]);
			}
		}
	}
		
	SafeDelete(*lpFont);
}

C3_CORE_DLL_API
void Font_Prepare ( void )
{
	SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
	SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	SetRenderState ( D3DRS_ZENABLE, false );
	SetRenderState ( D3DRS_ZWRITEENABLE, false );
	SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_FLAT );
	SetRenderState ( D3DRS_DITHERENABLE, false );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
	SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

	SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );

	SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT );

	SetTextureStageState ( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	SetTextureStageState ( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

	SetTextureStageState ( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	SetTextureStageState ( 1, D3DTSS_MINFILTER, D3DTEXF_NONE );
	SetTextureStageState ( 1, D3DTSS_MAGFILTER, D3DTEXF_NONE );
	SetTextureStageState ( 1, D3DTSS_MIPFILTER, D3DTEXF_NONE );
}

C3_CORE_DLL_API
BOOL Font_Draw ( C3Font *lpFont,
				 float fX,
				 float fY,
				 D3DCOLOR color,
				 char *lpChar )
{
	// 首先查找是否 cache 中已经包含字符
	if ( !lpFont->lpChar[( BYTE )lpChar[0]][( BYTE )lpChar[1]] )
	{
		lpFont->lpChar[( BYTE )lpChar[0]][( BYTE )lpChar[1]] = new C3Char;
		// create new texture
		if ( FAILED ( D3DXCreateTexture ( g_D3DDevice,
										  GetFontsize(lpFont->nSize),
										  GetFontsize(lpFont->nSize),
										  1,
										  0,
										  D3DFMT_A4R4G4B4,
										  D3DPOOL_MANAGED,
										  &lpFont->lpChar[( BYTE )lpChar[0]][( BYTE )lpChar[1]]->lpTex ) ) )
			return false;

		// clear buf
		memset(lpFont->lpBuffer, 0L, 4*lpFont->nSize*lpFont->nSize);

		// draw char
		if ( ( BYTE )lpChar[0] < 0x80 || ( BYTE )lpChar[0] > 0xfe)
			TextOut ( lpFont->hDC, 0, 0, lpChar, 1 );
		else
			TextOut ( lpFont->hDC, 0, 0, lpChar, 2 );

		D3DLOCKED_RECT lock;
		lpFont->lpChar[( BYTE )lpChar[0]][( BYTE )lpChar[1]]->lpTex->LockRect ( 0, &lock, 0, 0 );

		BYTE *pDstRow = ( BYTE* )lock.pBits;
		WORD *pDst16;

		BYTE alpha;
		for ( int y = 0; y < lpFont->nSize; y++ )
		{
			pDst16 = ( WORD* )pDstRow;

			int nWidth = lpFont->nSize;
			if ( ( BYTE )lpChar[0] < 0x80 || ( BYTE )lpChar[0] > 0xfe)
				nWidth = nWidth/2;

			for ( int x = 0; x < nWidth; x++ )
			{
				alpha = ( BYTE )( ( lpFont->lpBuffer[y * lpFont->nSize + x] & 0xff ) >> 4 );
				if ( alpha > 0 )
				{
					*pDst16++ = ( alpha << 12 ) | 0x0fff;
				}
				else
				{
					*pDst16++ = 0x0000;
				}
			}
			pDstRow += lock.Pitch;
		}
		lpFont->lpChar[( BYTE )lpChar[0]][( BYTE )lpChar[1]]->lpTex->UnlockRect ( 0 );
	}

	// draw text
	D3DSURFACE_DESC desc;
	lpFont->lpChar[( BYTE )lpChar[0]][( BYTE )lpChar[1]]->lpTex->GetLevelDesc ( 0, &desc );

	static FontVertex vec[4];
	
	vec[0].x = fX;
	vec[0].y = fY;
	vec[0].z = 0;
	vec[0].rhw = 1.0f;
	vec[0].color = color;
	vec[0].u = 0.0f;
	vec[0].v = 0.0f;

	vec[1].x = fX;
	vec[1].y = fY + ( float )lpFont->nSize;
	vec[1].z = 0;
	vec[1].rhw = 1.0f;
	vec[1].color = color;
	vec[1].u = 0.0f;
	vec[1].v = ( float )lpFont->nSize / ( float )desc.Height;

	vec[2].x = (( BYTE )lpChar[0] < 0x80 || ( BYTE )lpChar[0] > 0xfe)?
			   fX + ( float )lpFont->nSize / 2 :
			   fX + ( float )lpFont->nSize;
	vec[2].y = fY;
	vec[2].z = 0;
	vec[2].rhw = 1.0f;
	vec[2].color = color;
	vec[2].u = (( BYTE )lpChar[0] < 0x80 || ( BYTE )lpChar[0] > 0xfe)?
			   ( float )lpFont->nSize / ( float )desc.Width / 2 :
			   ( float )lpFont->nSize / ( float )desc.Width;
	vec[2].v = 0.0f;

	vec[3].x = (( BYTE )lpChar[0] < 0x80 || ( BYTE )lpChar[0] > 0xfe)?
			   fX + ( float )lpFont->nSize / 2 :
			   fX + ( float )lpFont->nSize;
	vec[3].y = fY + ( float )lpFont->nSize;
	vec[3].z = 0;
	vec[3].rhw = 1.0f;
	vec[3].color = color;
	vec[3].u = (( BYTE )lpChar[0] < 0x80 || ( BYTE )lpChar[0] > 0xfe)?
			   ( float )lpFont->nSize / ( float )desc.Width / 2 :
			   ( float )lpFont->nSize / ( float )desc.Width;
	vec[3].v = ( float )lpFont->nSize / ( float )desc.Height;

	if ( FAILED ( g_D3DDevice->SetTexture ( 0, lpFont->lpChar[( BYTE )lpChar[0]][( BYTE )lpChar[1]]->lpTex ) ) )
		return false;

	if ( FAILED ( g_D3DDevice->SetVertexShader ( FONT_VERTEX ) ) )
		return false;

	if ( FAILED ( g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP,
											     2,
											     vec,
											     sizeof ( FontVertex ) ) ) )
		return false;

	lpFont->dwChars++;
	return true;
}	
