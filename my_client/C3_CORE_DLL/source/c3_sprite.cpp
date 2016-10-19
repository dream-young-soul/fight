#include "..\include\c3_sprite.h"
#include "..\include\c3_main.h"

C3_CORE_DLL_API
void Sprite_Clear ( C3Sprite *lpSprite )
{
	for ( int v = 0; v < 4; v++ )
	{
		ZeroMemory ( &lpSprite->vertex[v], sizeof ( SpriteVertex ) );

		lpSprite->vertex[v].rhw = 1.0f;
		lpSprite->vertex[v].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
	}
	lpSprite->vertex[0].u = 0.0f;
	lpSprite->vertex[0].v = 0.0f;

	lpSprite->vertex[1].u = 0.0f;
	lpSprite->vertex[1].v = 1.0f;

	lpSprite->vertex[2].u = 1.0f;
	lpSprite->vertex[2].v = 0.0f;

	lpSprite->vertex[3].u = 1.0f;
	lpSprite->vertex[3].v = 1.0f;

	lpSprite->lpTex = 0;
}
C3_CORE_DLL_API
BOOL Sprite_Load ( C3Sprite **lpSprite,
				   char *lpName,
				   D3DPOOL pool,
				   BOOL bDuplicate,
				   D3DCOLOR colorkey )
{
	*lpSprite = new C3Sprite;
	Sprite_Clear ( *lpSprite );

	if ( Texture_Load ( &( *lpSprite )->lpTex, lpName, 1, pool, bDuplicate, colorkey ) == -1 )
		return false;

	Sprite_SetCoor ( *lpSprite, 0, 0, 0 );

	return true;
}
C3_CORE_DLL_API
BOOL Sprite_Create ( C3Sprite **lpSprite,
				     DWORD dwWidth,
					 DWORD dwHeight,
					 DWORD dwMipLevels,
					 D3DFORMAT format,
					 D3DPOOL pool )
{
	*lpSprite = new C3Sprite;
	Sprite_Clear ( *lpSprite );

	if ( !Texture_Create ( &( *lpSprite )->lpTex, dwWidth, dwHeight, dwMipLevels, format, pool ) )
		return false;
	return true;
}
C3_CORE_DLL_API
void Sprite_Unload ( C3Sprite **lpSprite )
{
	if ( !( *lpSprite ) )
		return;
	Texture_Unload ( &( *lpSprite )->lpTex );
	SafeDelete ( *lpSprite );
}

C3_CORE_DLL_API
void Sprite_Mirror ( C3Sprite *lpSprite )
{
	float u, v;

	u = lpSprite->vertex[0].u;
	v = lpSprite->vertex[0].v;

	lpSprite->vertex[0].u = lpSprite->vertex[2].u;
	lpSprite->vertex[0].v = lpSprite->vertex[2].v;

	lpSprite->vertex[2].u = u;
	lpSprite->vertex[2].v = v;

	u = lpSprite->vertex[1].u;
	v = lpSprite->vertex[1].v;

	lpSprite->vertex[1].u = lpSprite->vertex[3].u;
	lpSprite->vertex[1].v = lpSprite->vertex[3].v;

	lpSprite->vertex[3].u = u;
	lpSprite->vertex[3].v = v;
}

C3_CORE_DLL_API
void Sprite_SetCoor ( C3Sprite *lpSprite,
					  RECT *lpSrc,
					  int nX,
					  int nY,
					  DWORD dwWidth,
					  DWORD dwHeight )
{
	float uu = 1.0f / ( float )lpSprite->lpTex->Info.Width;
	float vv = 1.0f / ( float )lpSprite->lpTex->Info.Height;

	uu = 0;
	vv = 0;

	// src
	if ( !lpSrc )
	{
		lpSprite->vertex[0].u = 0.0f + uu;
		lpSprite->vertex[0].v = 0.0f + vv;

		lpSprite->vertex[1].u = 0.0f + uu;
		lpSprite->vertex[1].v = 1.0f - vv;

		lpSprite->vertex[2].u = 1.0f - uu;
		lpSprite->vertex[2].v = 0.0f + vv;

		lpSprite->vertex[3].u = 1.0f - uu;
		lpSprite->vertex[3].v = 1.0f - vv;
	}
	else
	{
		lpSprite->vertex[0].u = ( float )lpSrc->left / ( float )lpSprite->lpTex->Info.Width + uu;
		lpSprite->vertex[0].v = ( float )lpSrc->top / ( float )lpSprite->lpTex->Info.Height + vv;

		lpSprite->vertex[1].u = lpSprite->vertex[0].u;
		lpSprite->vertex[1].v = ( float )lpSrc->bottom / ( float )lpSprite->lpTex->Info.Height - vv;

		lpSprite->vertex[2].u = ( float )lpSrc->right / ( float )lpSprite->lpTex->Info.Width - uu;
		lpSprite->vertex[2].v = lpSprite->vertex[0].v;

		lpSprite->vertex[3].u = lpSprite->vertex[2].u;
		lpSprite->vertex[3].v = lpSprite->vertex[1].v;
	}
	// dest
	lpSprite->vertex[0].x = ( float )nX;
	lpSprite->vertex[0].y = ( float )nY;

	lpSprite->vertex[1].x = ( float )nX;
	lpSprite->vertex[1].y = ( float )( nY + ( int )( dwHeight == 0 ? lpSprite->lpTex->Info.Height : dwHeight ) );

	lpSprite->vertex[2].x = ( float )( nX + ( int )( dwWidth == 0 ? lpSprite->lpTex->Info.Width : dwWidth ) );
	lpSprite->vertex[2].y = ( float )nY;

	lpSprite->vertex[3].x = ( float )( nX + ( int )( dwWidth == 0 ? lpSprite->lpTex->Info.Width : dwWidth ) );
	lpSprite->vertex[3].y = ( float )( nY + ( int )( dwHeight == 0 ? lpSprite->lpTex->Info.Height : dwHeight ) );
}
C3_CORE_DLL_API
void Sprite_SetColor ( C3Sprite *lpSprite, BYTE a, BYTE r, BYTE g, BYTE b )
{
	for ( int v = 0; v < 4; v++ )
		lpSprite->vertex[v].color = D3DCOLOR_ARGB ( a, r, g, b );
}
C3_CORE_DLL_API
void Sprite_SetVertexColor ( C3Sprite *lpSprite, DWORD ltColor, DWORD rtColor, DWORD lbColor, DWORD rbColor )
{
	lpSprite->vertex[0].color = ltColor;
	lpSprite->vertex[1].color = lbColor;	
	lpSprite->vertex[2].color = rtColor;
	lpSprite->vertex[3].color = rbColor;
}
C3_CORE_DLL_API
void Sprite_Prepare ( void )
{
	SetRenderState ( D3DRS_ZENABLE, false );
	SetRenderState ( D3DRS_ZWRITEENABLE, false );
	SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_FLAT );
	SetRenderState ( D3DRS_DITHERENABLE, true );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
	SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
	//dx8----------------------
	//SetRenderState ( D3DRS_SOFTWAREVERTEXPROCESSING, false );
	//--------------------------------
	//使用这个代替
	g_D3DDevice->SetSoftwareVertexProcessing(TRUE);
	
	SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );

	SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	//SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	//SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	//SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);


	SetTextureStageState ( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	SetTextureStageState ( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

	SetTextureStageState ( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	//SetTextureStageState ( 1, D3DTSS_MINFILTER, D3DTEXF_NONE );
	//SetTextureStageState ( 1, D3DTSS_MAGFILTER, D3DTEXF_NONE );
	//SetTextureStageState ( 1, D3DTSS_MIPFILTER, D3DTEXF_NONE );

	g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_NONE);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
}
C3_CORE_DLL_API
BOOL Sprite_Draw ( C3Sprite *lpSprite, DWORD dwShowWay )
{
	switch ( dwShowWay )
	{
	case 0:
		if ( lpSprite->lpTex->Info.Format == D3DFMT_A8R8G8B8 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A1R5G5B5 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A4R4G4B4 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A8 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A8R3G3B2 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_DXT3 ||
			 ( lpSprite->vertex[0].color >> 24 ) < 255 )
		{
			SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
			SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

			SetRenderState ( D3DRS_ALPHATESTENABLE, true );
			SetRenderState ( D3DRS_ALPHAREF, 0x08 );
			SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		}
		else
		{
			SetRenderState ( D3DRS_ALPHABLENDENABLE, false );
			SetRenderState ( D3DRS_ALPHATESTENABLE, false );
		}
		break;
	case 1:
		if ( lpSprite->lpTex->Info.Format == D3DFMT_A8R8G8B8 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A1R5G5B5 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A4R4G4B4 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A8 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_A8R3G3B2 ||
			 lpSprite->lpTex->Info.Format == D3DFMT_DXT3 ||
			 ( lpSprite->vertex[0].color >> 24 ) < 255 )
		{
			SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
			SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_ONE );
			SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ONE );

			SetRenderState ( D3DRS_ALPHATESTENABLE, true );
			SetRenderState ( D3DRS_ALPHAREF, 0x08 );
			SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		}
		else
		{
			SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
			SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ONE);

			SetRenderState ( D3DRS_ALPHATESTENABLE, false );
		}
		break;
	case 2:
		if ( lpSprite->lpTex->Info.Format == D3DFMT_A8R8G8B8 ||
			lpSprite->lpTex->Info.Format == D3DFMT_A1R5G5B5 ||
			lpSprite->lpTex->Info.Format == D3DFMT_A4R4G4B4 ||
			lpSprite->lpTex->Info.Format == D3DFMT_A8 ||
			lpSprite->lpTex->Info.Format == D3DFMT_A8R3G3B2 ||
			lpSprite->lpTex->Info.Format == D3DFMT_DXT3 ||
			( lpSprite->vertex[0].color >> 24 ) < 255 )
		{
			SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
			SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );
			
			SetRenderState ( D3DRS_ALPHATESTENABLE, true );
			SetRenderState ( D3DRS_ALPHAREF, 0x08 );
			SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		}
		else
		{
			SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
			SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_ONE);
			
			SetRenderState ( D3DRS_ALPHATESTENABLE, false );
		}
		break;
	};
		
	if ( FAILED ( g_D3DDevice->SetTexture ( 0, lpSprite->lpTex->lpTex ) ) )
		return false;

	//if ( FAILED ( g_D3DDevice->SetVertexShader ( SPRITE_VERTEX ) ) )
	if ( FAILED (	g_D3DDevice->SetFVF(SPRITE_VERTEX)))
		return false;
	if ( FAILED ( g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP,
											     2,
											     lpSprite->vertex,
											     sizeof ( SpriteVertex ) ) ) )
		return false;
	return true;
}

C3_CORE_DLL_API
BOOL Sprite_Draw (C3Sprite *lpSpriteUp, C3Sprite *lpSpriteDn, UCHAR uAlphaA, UCHAR uAlphaB, UCHAR uAlphaC, UCHAR uAlphaD)
{
	if(!lpSpriteUp || !lpSpriteDn)
		return false;
	DWORD dwData;
	dwData = uAlphaA; dwData = (dwData<<24);
	lpSpriteUp->vertex[0].color = ((lpSpriteUp->vertex[0].color & 0x00ffffff) | dwData);

	dwData = uAlphaB; dwData = (dwData<<24);
	lpSpriteUp->vertex[1].color = ((lpSpriteUp->vertex[0].color & 0x00ffffff) | dwData);

	dwData = uAlphaC; dwData = (dwData<<24);
	lpSpriteUp->vertex[2].color = ((lpSpriteUp->vertex[0].color & 0x00ffffff) | dwData);

	dwData = uAlphaD; dwData = (dwData<<24);
	lpSpriteUp->vertex[3].color = ((lpSpriteUp->vertex[0].color & 0x00ffffff) | dwData);
	typedef struct
	{
		float x, y;
		float z;
		float rhw;
		DWORD color;
		float u1, v1;
		float u2, v2;
	}SpriteVertex2;
	static SpriteVertex2 vSprite[4];
	for(int i = 0; i < 4; i ++)
	{
		vSprite[i].color = lpSpriteUp->vertex[i].color;
		vSprite[i].x = lpSpriteUp->vertex[i].x;
		vSprite[i].y = lpSpriteUp->vertex[i].y;
		vSprite[i].z = lpSpriteUp->vertex[i].z;
		vSprite[i].rhw = lpSpriteUp->vertex[i].rhw;
		vSprite[i].u1 = vSprite[i].u2 = lpSpriteUp->vertex[i].u;
		vSprite[i].v1 = vSprite[i].v2 = lpSpriteUp->vertex[i].v;
	}
	if ( FAILED ( g_D3DDevice->SetTexture ( 0, lpSpriteDn->lpTex->lpTex ) ) )
		return false;
	if ( FAILED ( g_D3DDevice->SetTexture ( 1, lpSpriteUp->lpTex->lpTex ) ) )
		return false;
//	if ( FAILED ( g_D3DDevice->SetVertexShader (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2 )))
	if ( FAILED (	g_D3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)))
		return false;
	g_D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	g_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTOP_DISABLE);
	g_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE ); 
	g_D3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
	g_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_D3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	//g_D3DDevice->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
	//g_D3DDevice->SetTextureStageState(1, D3DTSS_MINFILTER , D3DTEXF_LINEAR );
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );
	//g_D3DDevice->SetTextureStageState(1, D3DTSS_MIPFILTER , D3DTEXF_LINEAR );

	g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

	g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);

	g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	if ( FAILED ( g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP,
		2,
		vSprite,
		sizeof ( SpriteVertex2 ) ) ) )
	{
		g_D3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);		
		return false;
	}
	g_D3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);		
	return true;
}

C3_CORE_DLL_API
void Sprite_Lock ( C3Sprite *lpSprite, RECT *lpRect, D3DLOCKED_RECT *lpReturn )
{
	lpSprite->lpTex->lpTex->LockRect ( 0, lpReturn, lpRect, 0 );
}
C3_CORE_DLL_API
void Sprite_Unlock ( C3Sprite *lpSprite )
{
	lpSprite->lpTex->lpTex->UnlockRect ( 0 );
}
