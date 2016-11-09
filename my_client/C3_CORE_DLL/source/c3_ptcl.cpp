#include "..\include\c3_ptcl.h"
#include "..\include\c3_datafile.h"

C3_CORE_DLL_API
void Ptcl_Clear ( C3Ptcl *lpPtcl )
{
	lpPtcl->lpName = 0;
	lpPtcl->lpVB = 0;
	lpPtcl->lpIB = 0;
	lpPtcl->nTex = -1;
	lpPtcl->lpTexName = 0;
	lpPtcl->dwCount = 0;
	lpPtcl->dwRow = 1;

	lpPtcl->lpPtcl = 0;
	lpPtcl->nFrame = 0;
	lpPtcl->dwFrames = 0;

	D3DXMatrixIdentity ( &lpPtcl->matrix );
}

C3_CORE_DLL_API
BOOL Ptcl_LoadPack ( C3Ptcl **lpPtcl,
					 HANDLE f,
					 BOOL bTex )
{
	*lpPtcl = new C3Ptcl;
	Ptcl_Clear ( *lpPtcl );
	DWORD bytes ;

	// 读入粒子名称
	DWORD temp;
	ReadFile ( f, &temp, sizeof ( unsigned long ), &bytes, 0 ) ;
	( *lpPtcl )->lpName = new char[temp + 1];
	ReadFile ( f, ( *lpPtcl )->lpName, temp, &bytes, 0 ) ;
	( *lpPtcl )->lpName[temp] = '\0';

	// 读入贴图
	ReadFile ( f, &temp, sizeof ( DWORD ), &bytes, 0 ) ;
	( *lpPtcl )->lpTexName = new char[temp + 1];
	ReadFile ( f, ( *lpPtcl )->lpTexName, temp, &bytes, 0 ) ;
	( *lpPtcl )->lpTexName[temp] = '\0';
	
	// 创建贴图
	if ( bTex )
	{
		C3Texture *tex;
		( *lpPtcl )->nTex = Texture_Load ( &tex, ( *lpPtcl )->lpTexName, 1 );
		if ( ( *lpPtcl )->nTex == -1 )
			return false;
	}

	ReadFile ( f, &( *lpPtcl )->dwRow, sizeof ( DWORD ), &bytes, 0 ) ;

	// count
	ReadFile ( f, &( *lpPtcl )->dwCount, sizeof ( DWORD ), &bytes, 0 ) ;
	( *lpPtcl )->lpVB = new PtclVertex[( *lpPtcl )->dwCount * 4];
	( *lpPtcl )->lpIB = new WORD[( *lpPtcl )->dwCount * 6];

	ReadFile ( f, &( *lpPtcl )->dwFrames, sizeof ( DWORD ), &bytes, 0 ) ;
	( *lpPtcl )->lpPtcl = new PtclFrame[( *lpPtcl )->dwFrames];

	for ( DWORD n = 0; n < ( *lpPtcl )->dwFrames; n++ )
	{
		ReadFile ( f, & ( *lpPtcl )->lpPtcl[n].dwCount, sizeof ( DWORD ), &bytes, 0 ) ;

		if ( ( *lpPtcl )->lpPtcl[n].dwCount > 0 )
		{
			( *lpPtcl )->lpPtcl[n].lpPos = new D3DXVECTOR3[( *lpPtcl )->lpPtcl[n].dwCount];
			ReadFile ( f,
					   ( *lpPtcl )->lpPtcl[n].lpPos,
					   sizeof ( D3DXVECTOR3 ) * ( *lpPtcl )->lpPtcl[n].dwCount,
					   &bytes,
					   0 ) ;

			( *lpPtcl )->lpPtcl[n].lpAge = new float[( *lpPtcl )->lpPtcl[n].dwCount];
			ReadFile ( f, 
					   ( *lpPtcl )->lpPtcl[n].lpAge,
					   sizeof ( float ) * ( *lpPtcl )->lpPtcl[n].dwCount,
					   &bytes,
					   0 ) ;

			( *lpPtcl )->lpPtcl[n].lpSize = new float[( *lpPtcl )->lpPtcl[n].dwCount];
			ReadFile ( f,
					   ( *lpPtcl )->lpPtcl[n].lpSize,
					   sizeof ( float ) * ( *lpPtcl )->lpPtcl[n].dwCount,
					   &bytes,
					   0 ) ;

			ReadFile ( f,
					   &( *lpPtcl )->lpPtcl[n].matrix,
					   sizeof ( D3DXMATRIX ),
					   &bytes,
					   0 ) ;
		}
		else
		{
			( *lpPtcl )->lpPtcl[n].lpPos = NULL;
			( *lpPtcl )->lpPtcl[n].lpAge = NULL;
			( *lpPtcl )->lpPtcl[n].lpSize = NULL;
		}
	}
	return true ;
}

C3_CORE_DLL_API
BOOL Ptcl_Load ( C3Ptcl **lpPtcl,
				 FILE *file,
				 BOOL bTex )
{
	*lpPtcl = new C3Ptcl;
	Ptcl_Clear ( *lpPtcl );

	// 读入粒子名称
	DWORD temp;
	fread ( &temp, sizeof ( unsigned long ), 1, file );
	( *lpPtcl )->lpName = new char[temp + 1];
	fread ( ( *lpPtcl )->lpName, 1, temp, file );
	( *lpPtcl )->lpName[temp] = '\0';

	// 读入贴图
	fread ( &temp, sizeof ( DWORD ), 1, file );
	( *lpPtcl )->lpTexName = new char[temp + 1];
	fread ( ( *lpPtcl )->lpTexName, 1, temp, file );
	( *lpPtcl )->lpTexName[temp] = '\0';
	// 创建贴图
	if ( bTex )
	{
		C3Texture *tex;
		( *lpPtcl )->nTex = Texture_Load ( &tex, ( *lpPtcl )->lpTexName, 1 );
		if ( ( *lpPtcl )->nTex == -1 )
			return false;
	}

	fread ( &( *lpPtcl )->dwRow, sizeof ( DWORD ), 1, file );
	//( *lpPtcl )->dwRow = max ( 1, atoi ( &( *lpPtcl )->lpTexName[strlen ( ( *lpPtcl )->lpTexName ) - 5] ) );

	// count
	fread ( &( *lpPtcl )->dwCount, sizeof ( DWORD ) , 1, file );
	( *lpPtcl )->lpVB = new PtclVertex[( *lpPtcl )->dwCount * 4];
	( *lpPtcl )->lpIB = new WORD[( *lpPtcl )->dwCount * 6];

	fread ( &( *lpPtcl )->dwFrames, sizeof ( DWORD ), 1, file );
	( *lpPtcl )->lpPtcl = new PtclFrame[( *lpPtcl )->dwFrames];

	for ( DWORD n = 0; n < ( *lpPtcl )->dwFrames; n++ )
	{
		fread ( &( *lpPtcl )->lpPtcl[n].dwCount, sizeof ( DWORD ), 1, file );

		if ( ( *lpPtcl )->lpPtcl[n].dwCount > 0 )
		{
			( *lpPtcl )->lpPtcl[n].lpPos = new D3DXVECTOR3[( *lpPtcl )->lpPtcl[n].dwCount];
			fread ( ( *lpPtcl )->lpPtcl[n].lpPos,
					sizeof ( D3DXVECTOR3 ),
					( *lpPtcl )->lpPtcl[n].dwCount,
					file );

			( *lpPtcl )->lpPtcl[n].lpAge = new float[( *lpPtcl )->lpPtcl[n].dwCount];
			fread ( ( *lpPtcl )->lpPtcl[n].lpAge,
					sizeof ( float ),
					( *lpPtcl )->lpPtcl[n].dwCount,
					file );

			( *lpPtcl )->lpPtcl[n].lpSize = new float[( *lpPtcl )->lpPtcl[n].dwCount];
			fread ( ( *lpPtcl )->lpPtcl[n].lpSize,
					sizeof ( float ),
					( *lpPtcl )->lpPtcl[n].dwCount,
					file );

			fread ( &( *lpPtcl )->lpPtcl[n].matrix,
					sizeof ( D3DXMATRIX ),
					1,
					file );
		}
		else
		{
			( *lpPtcl )->lpPtcl[n].lpPos = NULL;
			( *lpPtcl )->lpPtcl[n].lpAge = NULL;
			( *lpPtcl )->lpPtcl[n].lpSize = NULL;
		}
	}
	return true;
}
C3_CORE_DLL_API
BOOL Ptcl_Save ( char *lpName, C3Ptcl *lpPtcl, BOOL bNew )
{
	FILE *file = fopen ( lpName, bNew ? "w+b" : "r+b" );
	if ( !file )
		return false;
	fseek ( file, 0, SEEK_END );

	// ptcl
	ChunkHeader chunk;
	chunk.byChunkID[0] = 'P';
	chunk.byChunkID[1] = 'T';
	chunk.byChunkID[2] = 'C';
	chunk.byChunkID[3] = 'L';
	chunk.dwChunkSize = 0;
	fwrite ( &chunk, sizeof ( chunk ), 1, file );

	// name
	DWORD length = strlen ( lpPtcl->lpName );
	fwrite ( &length, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpPtcl->lpName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	// texture
	length = strlen ( lpPtcl->lpTexName );
	fwrite ( &length, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpPtcl->lpTexName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	fwrite ( &lpPtcl->dwRow, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	// count
	fwrite ( &lpPtcl->dwCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( &lpPtcl->dwFrames, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );
	
	for ( DWORD n = 0; n < lpPtcl->dwFrames; n++ )
	{
		fwrite ( &lpPtcl->lpPtcl[n].dwCount, sizeof ( DWORD ), 1, file );
		chunk.dwChunkSize += sizeof ( DWORD );

		if ( lpPtcl->lpPtcl[n].dwCount > 0 )
		{
			fwrite ( lpPtcl->lpPtcl[n].lpPos,
					 sizeof ( D3DXVECTOR3 ),
					 lpPtcl->lpPtcl[n].dwCount,
					 file );
			chunk.dwChunkSize += sizeof ( D3DXVECTOR3 ) * lpPtcl->lpPtcl[n].dwCount;

			fwrite ( lpPtcl->lpPtcl[n].lpAge,
					 sizeof ( float ),
					 lpPtcl->lpPtcl[n].dwCount,
					 file );
			chunk.dwChunkSize += sizeof ( float ) * lpPtcl->lpPtcl[n].dwCount;

			fwrite ( lpPtcl->lpPtcl[n].lpSize,
					 sizeof ( float ),
					 lpPtcl->lpPtcl[n].dwCount,
					 file );
			chunk.dwChunkSize += sizeof ( float ) * lpPtcl->lpPtcl[n].dwCount;

			fwrite ( &lpPtcl->lpPtcl[n].matrix,
					 sizeof ( D3DXMATRIX ),
					 1,
					 file );
			chunk.dwChunkSize += sizeof ( D3DXMATRIX );
		}
	}

	fseek ( file, -( int )( chunk.dwChunkSize + sizeof ( chunk ) ), SEEK_CUR );
	fwrite ( &chunk, sizeof ( chunk ), 1, file );
	fseek ( file, 0, SEEK_END );
	
	fclose ( file );
	return true;
}
C3_CORE_DLL_API
void Ptcl_Unload ( C3Ptcl **lpPtcl )
{
	int nDel = 0;
	if(( *lpPtcl ))
	{
		int nAmount = ( *lpPtcl )->dwFrames;
		for(int i = 0; i < nAmount; i ++)
		{
			PtclFrame* pFrame = ( *lpPtcl )->lpPtcl + i;
			SafeDeleteEx(pFrame->lpAge);
			SafeDeleteEx(pFrame->lpPos);
			SafeDeleteEx(pFrame->lpSize);
		}
	}
	SafeDeleteEx ( ( *lpPtcl )->lpName );
	SafeDeleteEx ( ( *lpPtcl )->lpVB );
	SafeDeleteEx ( ( *lpPtcl )->lpIB );
	SafeDeleteEx ( ( *lpPtcl )->lpTexName );

	( *lpPtcl )->nTex	=-1;
	SafeDeleteEx(( *lpPtcl )->lpPtcl->lpPos);
	SafeDeleteEx(( *lpPtcl )->lpPtcl->lpAge);
	SafeDeleteEx(( *lpPtcl )->lpPtcl->lpSize);
	
	SafeDeleteEx(( *lpPtcl )->lpPtcl);

	SafeDelete ( *lpPtcl );
}

C3_CORE_DLL_API
void Ptcl_Prepare ( void )
{
	SetRenderState ( D3DRS_SOFTWAREVERTEXPROCESSING, true );

	SetRenderState ( D3DRS_ZENABLE, true );
	SetRenderState ( D3DRS_ZWRITEENABLE, false );
	SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	SetRenderState ( D3DRS_DITHERENABLE, true );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
	SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

	SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );

	SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );

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
BOOL Ptcl_Draw ( C3Ptcl *lpPtcl, int nAsb, int nAdb)
{
	PtclVertex *vertex;
	PtclFrame *frame = &lpPtcl->lpPtcl[lpPtcl->nFrame];

	if ( frame->dwCount == 0 )
		return true;

	DWORD segcount = lpPtcl->dwRow * lpPtcl->dwRow;
	float segsize = 1.0f / lpPtcl->dwRow;

	D3DXMATRIX inv = frame->matrix;
	D3DXMatrixMultiply ( &inv, &inv, &lpPtcl->matrix );
	D3DXMatrixMultiply ( &inv, &inv, &g_ViewMatrix );

	for ( DWORD n = 0; n < frame->dwCount; n++ )
	{
		DWORD index = ( DWORD )( frame->lpAge[n] * segcount );

		float u = index % lpPtcl->dwRow * segsize;
		float v = index / lpPtcl->dwRow * segsize;

		D3DXVECTOR3 vec = frame->lpPos[n];
		D3DXVec3TransformCoord ( &vec, &vec, &inv );

		// vec0
		vertex = &lpPtcl->lpVB[n * 4];

		vertex->x = vec.x - frame->lpSize[n];
		vertex->y = vec.y - frame->lpSize[n];
		vertex->z = vec.z;

		vertex->u = u;
		vertex->v = v + segsize;
		vertex->color = D3DCOLOR_RGBA ( 255, 255, 255, 255 );

		// vec1
		vertex = &lpPtcl->lpVB[n * 4 + 1];

		vertex->x = vec.x + frame->lpSize[n];
		vertex->y = vec.y - frame->lpSize[n];
		vertex->z = vec.z;

		vertex->u = u + segsize;
		vertex->v = v + segsize;
		vertex->color = D3DCOLOR_RGBA ( 255, 255, 255, 255 );

		// vec2
		vertex = &lpPtcl->lpVB[n * 4 + 2];

		vertex->x = vec.x - frame->lpSize[n];
		vertex->y = vec.y + frame->lpSize[n];
		vertex->z = vec.z;

		vertex->u = u;
		vertex->v = v;
		vertex->color = D3DCOLOR_RGBA ( 255, 255, 255, 255 );

		// vec3
		vertex = &lpPtcl->lpVB[n * 4 + 3];

		vertex->x = vec.x + frame->lpSize[n];
		vertex->y = vec.y + frame->lpSize[n];
		vertex->z = vec.z;

		vertex->u = u + segsize;
		vertex->v = v;
		vertex->color = D3DCOLOR_RGBA ( 255, 255, 255, 255 );

		// ib
		lpPtcl->lpIB[n * 6] = ( WORD )( n * 4 );
		lpPtcl->lpIB[n * 6 + 1] = ( WORD )( n * 4 + 1 );
		lpPtcl->lpIB[n * 6 + 2] = ( WORD )( n * 4 + 2 );
		lpPtcl->lpIB[n * 6 + 3] = ( WORD )( n * 4 + 2 );
		lpPtcl->lpIB[n * 6 + 4] = ( WORD )( n * 4 + 1 );
		lpPtcl->lpIB[n * 6 + 5] = ( WORD )( n * 4 + 3 );
	}

/*	if ( g_lpTex[lpPtcl->nTex]->Info.Format == D3DFMT_A8R8G8B8 ||
		 g_lpTex[lpPtcl->nTex]->Info.Format == D3DFMT_A1R5G5B5 ||
		 g_lpTex[lpPtcl->nTex]->Info.Format == D3DFMT_A4R4G4B4 ||
		 g_lpTex[lpPtcl->nTex]->Info.Format == D3DFMT_A8 ||
		 g_lpTex[lpPtcl->nTex]->Info.Format == D3DFMT_A8R3G3B2 ||
		 g_lpTex[lpPtcl->nTex]->Info.Format == D3DFMT_DXT3 )
	{*/
		SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
		SetRenderState ( D3DRS_SRCBLEND, nAsb);
		SetRenderState ( D3DRS_DESTBLEND, nAdb);

        SetRenderState ( D3DRS_ALPHATESTENABLE, true );
        SetRenderState ( D3DRS_ALPHAREF, 0x08 );
        SetRenderState ( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
/*	}
	else
	{
		SetRenderState ( D3DRS_ALPHABLENDENABLE, false );
        SetRenderState ( D3DRS_ALPHATESTENABLE, false );
	}*/
	// 当前贴图
	if ( FAILED ( g_D3DDevice->SetTexture ( 0, g_lpTex[lpPtcl->nTex]->lpTex ) ) )
		return false;

	D3DXMatrixInverse ( &inv, 0, &g_ViewMatrix );
	/*D3DXMatrixMultiply ( &inv,
						 &frame->matrix,
						 &inv );*/

	if ( FAILED ( g_D3DDevice->SetTransform ( D3DTS_WORLD, &inv ) ) )
		return false;

	// vertex shader
	if ( FAILED ( g_D3DDevice->SetVertexShader ( PTCL_VERTEX ) ) )
		return false;

	// 绘制场景
	if ( FAILED ( g_D3DDevice->DrawIndexedPrimitiveUP ( D3DPT_TRIANGLELIST,
														0,
														frame->dwCount * 4,
														frame->dwCount * 2,
														lpPtcl->lpIB,
														D3DFMT_INDEX16,
														lpPtcl->lpVB,
														sizeof ( PtclVertex ) ) ) )
		return false;
	return true;
}
C3_CORE_DLL_API
void Ptcl_NextFrame ( C3Ptcl *lpPtcl, int nStep )
{
	lpPtcl->nFrame = ( lpPtcl->nFrame + nStep ) % lpPtcl->dwFrames;
}
C3_CORE_DLL_API
void Ptcl_SetFrame ( C3Ptcl *lpPtcl, DWORD dwFrame )
{
	lpPtcl->nFrame = dwFrame % lpPtcl->dwFrames;
}
C3_CORE_DLL_API
void Ptcl_Muliply ( C3Ptcl *lpPtcl, D3DXMATRIX *matrix )
{
	D3DXMatrixMultiply ( &lpPtcl->matrix,
						 &lpPtcl->matrix,
						 matrix );
}
C3_CORE_DLL_API
void Ptcl_ClearMatrix ( C3Ptcl *lpPtcl )
{
	// 清除运动矩阵
	D3DXMatrixIdentity ( &lpPtcl->matrix );
}
C3_CORE_DLL_API
void Ptcl_ChangeTexture ( C3Ptcl *lpPtcl, int nTexID )
{
	lpPtcl->nTex = nTexID;
}
