#include "..\include\c3_shape.h"
#include "..\include\c3_texture.h"
#include "..\include\c3_datafile.h"

const int TEARAIR_TEX_SIZE		= 1024;
const int TEARAIR_SEGMENT_NUM	= 4;

C3_CORE_DLL_API
void SMotion_Clear ( C3SMotion *lpSMotion )
{
	lpSMotion->dwFrames = 0;
	lpSMotion->lpFrames = 0;
	lpSMotion->nFrame = 0;
}

C3_CORE_DLL_API
BOOL SMotion_LoadPack ( C3SMotion **lpSMotion, HANDLE f )
{
	*lpSMotion = new C3SMotion;
	SMotion_Clear ( *lpSMotion );
	DWORD bytes ;

	// matrix
	ReadFile ( f, &( *lpSMotion )->dwFrames, sizeof ( DWORD ), &bytes, 0 ) ;

	( *lpSMotion )->lpFrames = new D3DXMATRIX[( *lpSMotion )->dwFrames];
	ReadFile ( f,
			   ( *lpSMotion )->lpFrames,
			   sizeof ( D3DXMATRIX ) * ( *lpSMotion )->dwFrames,
			   &bytes,
			   0 ) ;

	D3DXMatrixIdentity ( &( *lpSMotion )->matrix );
	return true; 
}

C3_CORE_DLL_API
BOOL SMotion_Load ( C3SMotion **lpSMotion, FILE *file )
{
	*lpSMotion = new C3SMotion;
	SMotion_Clear ( *lpSMotion );

	// matrix
	fread ( &( *lpSMotion )->dwFrames, sizeof ( DWORD ), 1, file );

	( *lpSMotion )->lpFrames = new D3DXMATRIX[( *lpSMotion )->dwFrames];
	fread ( ( *lpSMotion )->lpFrames,
			sizeof ( D3DXMATRIX ),
			( *lpSMotion )->dwFrames,
			file );

	D3DXMatrixIdentity ( &( *lpSMotion )->matrix );
	return true;
}	
C3_CORE_DLL_API
BOOL SMotion_Save ( char *lpName, C3SMotion *lpSMotion, BOOL bNew )
{
	FILE *file = fopen ( lpName, bNew ? "w+b" : "r+b" );
	if ( !file )
		return false;
	fseek ( file, 0, SEEK_END );

	// phy
	ChunkHeader chunk;
	chunk.byChunkID[0] = 'S';
	chunk.byChunkID[1] = 'M';
	chunk.byChunkID[2] = 'O';
	chunk.byChunkID[3] = 'T';
	chunk.dwChunkSize = 0;
	fwrite ( &chunk, sizeof ( chunk ), 1, file );

	// matrix
	fwrite ( &lpSMotion->dwFrames, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpSMotion->lpFrames,
			 sizeof ( D3DXMATRIX ),
			 lpSMotion->dwFrames,
			 file );
	chunk.dwChunkSize += sizeof ( D3DXMATRIX ) * lpSMotion->dwFrames;

	fseek ( file, -( int )( chunk.dwChunkSize + sizeof ( chunk ) ), SEEK_CUR );
	fwrite ( &chunk, sizeof ( chunk ), 1, file );
	fseek ( file, 0, SEEK_END );

	fclose ( file );
	return true;
}
C3_CORE_DLL_API
void SMotion_Unload ( C3SMotion **lpSMotion )
{
	SafeDeleteEx ( ( *lpSMotion )->lpFrames );
	SafeDelete ( *lpSMotion );
}

C3_CORE_DLL_API
void Shape_Clear ( C3Shape *lpShape )
{
	lpShape->lpName = 0;
	
	lpShape->dwLineCount = 0;
	lpShape->lpLine = 0;

	lpShape->vb = 0;
	lpShape->dwSegment = 0;
	lpShape->dwSegmentCur = 0;

	lpShape->bFrist = true;

	lpShape->dwSmooth = 0;

	lpShape->lpTexName = 0;
	lpShape->nTex = -1;

	lpShape->lpMotion = 0;

	lpShape->pTearAirTex	= NULL;
	lpShape->pScreenPnt		= NULL;
}

C3_CORE_DLL_API
BOOL Shape_LoadPack ( C3Shape **lpShape, HANDLE f, BOOL bTex )
{
	*lpShape = new C3Shape;
	Shape_Clear ( *lpShape );
	DWORD bytes ;

	// 读入名称
	DWORD temp;
	ReadFile ( f, &temp, sizeof ( unsigned long ), &bytes, 0 ) ;
	( *lpShape )->lpName = new char[temp + 1];
	ReadFile ( f, ( *lpShape )->lpName, temp, &bytes, 0 ) ;
	( *lpShape )->lpName[temp] = '\0';

	ReadFile ( f, &( *lpShape )->dwLineCount, sizeof ( DWORD ), &bytes, 0 ) ;

	( *lpShape )->lpLine = new C3Line[( *lpShape )->dwLineCount];

	for ( DWORD n = 0; n < ( *lpShape )->dwLineCount; n++ )
	{
		ReadFile ( f,
				   &( *lpShape )->lpLine[n].dwVecCount,
				   sizeof ( DWORD ),
				   &bytes,
				   0 ) ;

		( *lpShape )->lpLine[n].lpVB = new D3DXVECTOR3[( *lpShape )->lpLine[n].dwVecCount];
		ReadFile ( f,
				   ( *lpShape )->lpLine[n].lpVB,
				   sizeof ( D3DXVECTOR3 ) * ( *lpShape )->lpLine[n].dwVecCount,
				   &bytes,
				   0 ) ;
				  
	}

	// 读入贴图
	ReadFile ( f, &temp, sizeof ( DWORD ), &bytes, 0 ) ;
	( *lpShape )->lpTexName = new char[temp + 1];
	ReadFile ( f, ( *lpShape )->lpTexName, temp, &bytes, 0 ) ;
	( *lpShape )->lpTexName[temp] = '\0';
	// 创建贴图
	if ( bTex )
	{
		C3Texture *tex;
		( *lpShape )->nTex = Texture_Load ( &tex, ( *lpShape )->lpTexName );
		if ( ( *lpShape )->nTex == -1 )
			return false;
	}

	DWORD segment;
	ReadFile ( f, &segment, sizeof ( DWORD ), &bytes, 0 ) ;
	Shape_SetSegment ( *lpShape, segment );

	return true ;
}

C3_CORE_DLL_API
BOOL Shape_Load ( C3Shape **lpShape, FILE *file, BOOL bTex )
{
	*lpShape = new C3Shape;
	Shape_Clear ( *lpShape );

	// 读入名称
	DWORD temp;
	fread ( &temp, sizeof ( unsigned long ), 1, file );
	( *lpShape )->lpName = new char[temp + 1];
	fread ( ( *lpShape )->lpName, 1, temp, file );
	( *lpShape )->lpName[temp] = '\0';

	fread ( &( *lpShape )->dwLineCount, sizeof ( DWORD ), 1, file );

	( *lpShape )->lpLine = new C3Line[( *lpShape )->dwLineCount];

	for ( DWORD n = 0; n < ( *lpShape )->dwLineCount; n++ )
	{
		fread ( &( *lpShape )->lpLine[n].dwVecCount,
				sizeof ( DWORD ),
				1,
				file );

		( *lpShape )->lpLine[n].lpVB = new D3DXVECTOR3[( *lpShape )->lpLine[n].dwVecCount];
		fread ( ( *lpShape )->lpLine[n].lpVB,
				sizeof ( D3DXVECTOR3 ),
				( *lpShape )->lpLine[n].dwVecCount,
				file );
	}

	// 读入贴图
	fread ( &temp, sizeof ( DWORD ), 1, file );
	( *lpShape )->lpTexName = new char[temp + 1];
	fread ( ( *lpShape )->lpTexName, 1, temp, file );
	( *lpShape )->lpTexName[temp] = '\0';
	// 创建贴图
	if ( bTex )
	{
		C3Texture *tex;
		( *lpShape )->nTex = Texture_Load ( &tex, ( *lpShape )->lpTexName );
		if ( ( *lpShape )->nTex == -1 )
			return false;
	}

	DWORD segment;
	fread ( &segment, sizeof ( DWORD ), 1, file );
	Shape_SetSegment ( *lpShape, segment );
	return true;
}

C3_CORE_DLL_API
BOOL Shape_Save ( char *lpName, C3Shape *lpShape, BOOL bNew )
{
	FILE *file = fopen ( lpName, bNew ? "w+b" : "r+b" );
	if ( !file )
		return false;
	fseek ( file, 0, SEEK_END );

	// shape
	ChunkHeader chunk;
	chunk.byChunkID[0] = 'S';
	chunk.byChunkID[1] = 'H';
	chunk.byChunkID[2] = 'A';
	chunk.byChunkID[3] = 'P';
	chunk.dwChunkSize = 0;
	fwrite ( &chunk, sizeof ( chunk ), 1, file );

	// name
	DWORD length = strlen ( lpShape->lpName );
	fwrite ( &length, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpShape->lpName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	//
	fwrite ( &lpShape->dwLineCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	for ( DWORD n = 0; n < lpShape->dwLineCount; n++ )
	{
		fwrite ( &lpShape->lpLine[n].dwVecCount,
				 sizeof ( DWORD ),
				 1,
				 file );
		chunk.dwChunkSize += sizeof ( DWORD );

		fwrite ( lpShape->lpLine[n].lpVB,
				 sizeof ( D3DXVECTOR3 ),
				 lpShape->lpLine[n].dwVecCount,
				 file );
		chunk.dwChunkSize += sizeof ( D3DXVECTOR3 ) * lpShape->lpLine[n].dwVecCount;
	}

	// texture
	length = strlen ( lpShape->lpTexName );
	fwrite ( &length, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpShape->lpTexName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	fwrite ( &lpShape->dwSegment, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );


	fseek ( file, -( int )( chunk.dwChunkSize + sizeof ( chunk ) ), SEEK_CUR );
	fwrite ( &chunk, sizeof ( chunk ), 1, file );
	fseek ( file, 0, SEEK_END );

	fclose ( file );
	return true;
}
C3_CORE_DLL_API
void Shape_Unload ( C3Shape **lpShape )
{
	SafeDeleteEx ( ( *lpShape )->lpName );

	for ( DWORD n = 0; n < ( *lpShape )->dwLineCount; n++ )
	{
		SafeDeleteEx ( ( *lpShape )->lpLine[n].lpVB );
	}

	if(( *lpShape )->lpLine)
	{
		SafeDeleteEx (( *lpShape )->lpLine);
		SafeDeleteEx (( *lpShape )->lpTexName);
		SafeDeleteEx (( *lpShape )->lpName);
	}
	SafeDeleteEx ( (*lpShape)->vb );

	if ( (*lpShape)->pTearAirTex != NULL )
	{
		SafeRelease( (*lpShape)->pTearAirTex );
	}
	
	if ( (*lpShape)->pScreenPnt != NULL )
	{
		SafeDeleteEx( (*lpShape)->pScreenPnt );
	}
	
	SafeDelete ( *lpShape );
}
/*
C3_CORE_DLL_API
void Shape_Link ( C3Shape *lpShape, D3DXMATRIX *lpMatrix )
{
	D3DXMATRIX matrix;
	D3DXMatrixInverse ( &matrix, 0, lpMatrix );

	for ( DWORD n = 0; n < lpShape->dwLineCount; n++ )
	{
		for ( DWORD m = 0; m < lpShape->lpLine[n].dwVecCount; m++ )
		{
			D3DXVec3TransformCoord ( ( D3DXVECTOR3* )&lpShape->lpLine[n].lpVB[m],
									 ( D3DXVECTOR3* )&lpShape->lpLine[n].lpVB[m],
									 &matrix );
		}
	}
}*/
C3_CORE_DLL_API
void Shape_SetSegment ( C3Shape *lpShape, DWORD dwSegment, DWORD dwSmooth )
{
	dwSmooth = 10;

	dwSegment = dwSegment * ( dwSmooth + 1 );


	lpShape->dwSegment = dwSegment;
	lpShape->dwSmooth = dwSmooth;
	lpShape->dwSegmentCur = 0;

	SafeDeleteEx ( lpShape->vb );
	lpShape->vb = new ShapeOutVertex[dwSegment * 6];

	ZeroMemory ( lpShape->vb, sizeof ( ShapeOutVertex ) * dwSegment * 6 );

}

C3_CORE_DLL_API
BOOL Shape_Draw ( C3Shape *lpShape, BOOL bLocal, int nAsb, int nAdb)
{
	SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
	SetRenderState ( D3DRS_SRCBLEND, nAsb );
	SetRenderState ( D3DRS_DESTBLEND, nAdb );

	// 首先转换线顶点
	D3DXMATRIX mm;

	if ( bLocal )
		mm = lpShape->lpMotion->lpFrames[lpShape->lpMotion->nFrame];
	else
		D3DXMatrixMultiply ( &mm,
							 &lpShape->lpMotion->lpFrames[lpShape->lpMotion->nFrame],
							 &lpShape->lpMotion->matrix );

	D3DXVECTOR3 vec[2];
	D3DXVec3TransformCoord ( &vec[0],
							 ( D3DXVECTOR3* )&lpShape->lpLine[0].lpVB[0],
							 &mm );
	D3DXVec3TransformCoord ( &vec[1],
							 ( D3DXVECTOR3* )&lpShape->lpLine[0].lpVB[1],
							 &mm );

	
	if ( !lpShape->bFrist )
	{
		// 计算长度
		D3DXVECTOR3 sub;
		D3DXVec3Subtract ( &sub, &vec[0], &vec[1] );
		float len = D3DXVec3Length ( &sub );

		D3DXVECTOR3 last[2];

		last[0] = lpShape->last[0];
		last[1] = lpShape->last[1];

		// 计算插值的点
		for ( DWORD nn = 0; nn < lpShape->dwSmooth; nn++ )
		{
			D3DXVECTOR3 smooths[2];

			D3DXVec3Lerp ( &smooths[0],
						   &lpShape->last[0],
						   &vec[0],
						   1.0f / ( lpShape->dwSmooth + 1 ) * ( nn + 1 ) );
			D3DXVec3Lerp ( &smooths[1],
						   &lpShape->last[1],
						   &vec[1],
						   1.0f / ( lpShape->dwSmooth + 1 ) * ( nn + 1 ) );

			// 计算长度
			D3DXVec3Subtract ( &sub,
							   &smooths[0],
							   &smooths[1] );

			float lennow = D3DXVec3Length ( &sub );

			D3DXVec3Lerp ( &smooths[0],
						   &smooths[1],
						   &smooths[0],
						   len / lennow );

			D3DXVec3Subtract ( &sub,
							   &smooths[0],
							   &smooths[1] );

			lennow = D3DXVec3Length ( &sub );

			// 算刀光
			int cur = lpShape->dwSegmentCur * 6;

			lpShape->vb[cur + 0].x = smooths[0].x;
			lpShape->vb[cur + 0].y = smooths[0].y;
			lpShape->vb[cur + 0].z = smooths[0].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 0].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 0].u = 0;
			lpShape->vb[cur + 0].v = 0;

			lpShape->vb[cur + 1].x = smooths[1].x;
			lpShape->vb[cur + 1].y = smooths[1].y;
			lpShape->vb[cur + 1].z = smooths[1].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 1].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 1].u = 0;
			lpShape->vb[cur + 1].v = 0;

			lpShape->vb[cur + 2].x = last[1].x;
			lpShape->vb[cur + 2].y = last[1].y;
			lpShape->vb[cur + 2].z = last[1].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 2].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 2].u = 0;
			lpShape->vb[cur + 2].v = 0;


			lpShape->vb[cur + 3].x = last[0].x;
			lpShape->vb[cur + 3].y = last[0].y;
			lpShape->vb[cur + 3].z = last[0].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 3].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 3].u = 0;
			lpShape->vb[cur + 3].v = 0;

			lpShape->vb[cur + 4].x = last[1].x;
			lpShape->vb[cur + 4].y = last[1].y;
			lpShape->vb[cur + 4].z = last[1].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 4].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 4].u = 0;
			lpShape->vb[cur + 4].v = 0;

			lpShape->vb[cur + 5].x = smooths[0].x;
			lpShape->vb[cur + 5].y = smooths[0].y;
			lpShape->vb[cur + 5].z = smooths[0].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 5].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 5].u = 0;
			lpShape->vb[cur + 5].v = 0;

		
			// alpha
	/*		DWORD add = lpShape->dwSegment;
			float uvstep = 0.9f / add;
		
			for ( int n = lpShape->dwSegmentCur; n >= 0; n-- )
			{
				float u = add * uvstep + 0.05f;

				lpShape->vb[n * 6 + 0].u = u;
				lpShape->vb[n * 6 + 0].v = 0;

				lpShape->vb[n * 6 + 1].u = u;
				lpShape->vb[n * 6 + 1].v = 1;

				lpShape->vb[n * 6 + 5].u = u;
				lpShape->vb[n * 6 + 5].v = 0;

				u -= uvstep;

				lpShape->vb[n * 6 + 2].u = u;
				lpShape->vb[n * 6 + 2].v = 1;

				lpShape->vb[n * 6 + 3].u = u;
				lpShape->vb[n * 6 + 3].v = 0;

				lpShape->vb[n * 6 + 4].u = u;
				lpShape->vb[n * 6 + 4].v = 1;

				add--;
			}
			for ( n = lpShape->dwSegment - 1; n > ( int )lpShape->dwSegmentCur; n-- )
			{
				float u = add * uvstep + 0.05f;

				lpShape->vb[n * 6 + 0].u = u;
				lpShape->vb[n * 6 + 0].v = 0;

				lpShape->vb[n * 6 + 1].u = u;
				lpShape->vb[n * 6 + 1].v = 1;

				lpShape->vb[n * 6 + 5].u = u;
				lpShape->vb[n * 6 + 5].v = 0;

				u -= uvstep;

				lpShape->vb[n * 6 + 2].u = u;
				lpShape->vb[n * 6 + 2].v = 1;

				lpShape->vb[n * 6 + 3].u = u;
				lpShape->vb[n * 6 + 3].v = 0;

				lpShape->vb[n * 6 + 4].u = u;
				lpShape->vb[n * 6 + 4].v = 1;

				add--;
			}*/
			/*if ( lpShape->nTex == -1 )
			{
				if ( FAILED ( g_D3DDevice->SetTexture ( 0, 0 ) ) )
					return false;
			}
			else
			{
				if ( FAILED ( g_D3DDevice->SetTexture ( 0, g_lpTex[lpShape->nTex]->lpTex ) ) )
					return false;
			}


			D3DXMatrixIdentity ( &mm );
			g_D3DDevice->SetTransform ( D3DTS_WORLD, &mm );

			if ( FAILED ( g_D3DDevice->SetVertexShader ( SHAPE_OUT_VERTEX ) ) )
				return false;

			if ( FAILED ( g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLELIST,
														 lpShape->dwSegment * 2,
														 lpShape->vb,
														 sizeof ( ShapeOutVertex ) ) ) )
				return false;
*/
			lpShape->dwSegmentCur++;
			if ( lpShape->dwSegmentCur == lpShape->dwSegment )
				lpShape->dwSegmentCur = 0;

			last[0] = smooths[0];
			last[1] = smooths[1];
			
			if ( nn == lpShape->dwSmooth - 1 )
			{
				lpShape->last[0] = smooths[0];
				lpShape->last[1] = smooths[1];
			}
		}
		int cur = lpShape->dwSegmentCur * 6;

		lpShape->vb[cur + 0].x = vec[0].x;
		lpShape->vb[cur + 0].y = vec[0].y;
		lpShape->vb[cur + 0].z = vec[0].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 0].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 0].u = 0;
		lpShape->vb[cur + 0].v = 0;

		lpShape->vb[cur + 1].x = vec[1].x;
		lpShape->vb[cur + 1].y = vec[1].y;
		lpShape->vb[cur + 1].z = vec[1].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 1].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 1].u = 0;
		lpShape->vb[cur + 1].v = 0;

		lpShape->vb[cur + 2].x = lpShape->last[1].x;
		lpShape->vb[cur + 2].y = lpShape->last[1].y;
		lpShape->vb[cur + 2].z = lpShape->last[1].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 2].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 2].u = 0;
		lpShape->vb[cur + 2].v = 0;


		lpShape->vb[cur + 3].x = lpShape->last[0].x;
		lpShape->vb[cur + 3].y = lpShape->last[0].y;
		lpShape->vb[cur + 3].z = lpShape->last[0].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 3].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 3].u = 0;
		lpShape->vb[cur + 3].v = 0;

		lpShape->vb[cur + 4].x = lpShape->last[1].x;
		lpShape->vb[cur + 4].y = lpShape->last[1].y;
		lpShape->vb[cur + 4].z = lpShape->last[1].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 4].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 4].u = 0;
		lpShape->vb[cur + 4].v = 0;

		lpShape->vb[cur + 5].x = vec[0].x;
		lpShape->vb[cur + 5].y = vec[0].y;
		lpShape->vb[cur + 5].z = vec[0].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 5].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 5].u = 0;
		lpShape->vb[cur + 5].v = 0;

	
		// alpha
		DWORD add = lpShape->dwSegment;
		float uvstep = 0.9f / add;
		int n = 0;
		for (  n = lpShape->dwSegmentCur; n >= 0; n-- )
		{
			float u = add * uvstep + 0.05f;

			lpShape->vb[n * 6 + 0].u = u;
			lpShape->vb[n * 6 + 0].v = 0;

			lpShape->vb[n * 6 + 1].u = u;
			lpShape->vb[n * 6 + 1].v = 1;

			lpShape->vb[n * 6 + 5].u = u;
			lpShape->vb[n * 6 + 5].v = 0;

			u -= uvstep;

			lpShape->vb[n * 6 + 2].u = u;
			lpShape->vb[n * 6 + 2].v = 1;

			lpShape->vb[n * 6 + 3].u = u;
			lpShape->vb[n * 6 + 3].v = 0;

			lpShape->vb[n * 6 + 4].u = u;
			lpShape->vb[n * 6 + 4].v = 1;

			add--;
		}
		for ( n = lpShape->dwSegment - 1; n > ( int )lpShape->dwSegmentCur; n-- )
		{
			float u = add * uvstep + 0.05f;

			lpShape->vb[n * 6 + 0].u = u;
			lpShape->vb[n * 6 + 0].v = 0;

			lpShape->vb[n * 6 + 1].u = u;
			lpShape->vb[n * 6 + 1].v = 1;

			lpShape->vb[n * 6 + 5].u = u;
			lpShape->vb[n * 6 + 5].v = 0;

			u -= uvstep;

			lpShape->vb[n * 6 + 2].u = u;
			lpShape->vb[n * 6 + 2].v = 1;

			lpShape->vb[n * 6 + 3].u = u;
			lpShape->vb[n * 6 + 3].v = 0;

			lpShape->vb[n * 6 + 4].u = u;
			lpShape->vb[n * 6 + 4].v = 1;

			add--;
		}

		lpShape->dwSegmentCur++;
		if ( lpShape->dwSegmentCur == lpShape->dwSegment )
			lpShape->dwSegmentCur = 0;

		if ( lpShape->nTex == -1 )
		{
			if ( FAILED ( g_D3DDevice->SetTexture ( 0, 0 ) ) )
				return false;
		}
		else
		{
			if ( FAILED ( g_D3DDevice->SetTexture ( 0, g_lpTex[lpShape->nTex]->lpTex ) ) )
				return false;
		}


		if ( bLocal )
			mm = lpShape->lpMotion->matrix;
		else
			D3DXMatrixIdentity ( &mm );

		g_D3DDevice->SetTransform ( D3DTS_WORLD, &mm );

		//if ( FAILED ( g_D3DDevice->SetVertexShader ( SHAPE_OUT_VERTEX ) ) )
		if ( FAILED (	g_D3DDevice->SetFVF(SHAPE_OUT_VERTEX)))
			return false;

		if ( FAILED ( g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLELIST,
													 lpShape->dwSegment * 2,
													 lpShape->vb,
													 sizeof ( ShapeOutVertex ) ) ) )
			return false;
	}
	else
	{
		ZeroMemory ( lpShape->vb, sizeof ( ShapeOutVertex ) * lpShape->dwSegment * 6 );
		lpShape->bFrist = false;
	}

	lpShape->last[0] = vec[0];
	lpShape->last[1] = vec[1];

	return true;
}
C3_CORE_DLL_API
void Shape_Prepare ( void )
{
	SetRenderState ( D3DRS_ZENABLE, true );
	SetRenderState ( D3DRS_ZWRITEENABLE, false );
	SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_FLAT );
	SetRenderState ( D3DRS_DITHERENABLE, true );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
	SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

	SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
	SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );

	SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	//SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	//SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	//SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);


	SetTextureStageState ( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	SetTextureStageState ( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

	SetTextureStageState ( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	//SetTextureStageState ( 1, D3DTSS_MINFILTER, D3DTEXF_NONE );
	//SetTextureStageState ( 1, D3DTSS_MAGFILTER, D3DTEXF_NONE );
	//SetTextureStageState ( 1, D3DTSS_MIPFILTER, D3DTEXF_NONE );

	g_D3DDevice->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_NONE);
	g_D3DDevice->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
	g_D3DDevice->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
}
C3_CORE_DLL_API
void Shape_ChangeTexture ( C3Shape *lpShape, int nTexID )
{
	lpShape->nTex = nTexID;
}
C3_CORE_DLL_API
void Shape_Muliply ( C3Shape *lpShape, D3DXMATRIX *matrix )
{
	D3DXMatrixMultiply ( &lpShape->lpMotion->matrix,
						 &lpShape->lpMotion->matrix,
						 matrix );
}
C3_CORE_DLL_API
void Shape_NextFrame ( C3Shape *lpShape, int nStep )
{
	lpShape->lpMotion->nFrame = ( lpShape->lpMotion->nFrame + nStep ) % lpShape->lpMotion->dwFrames;
}
C3_CORE_DLL_API
void Shape_SetFrame ( C3Shape *lpShape, DWORD dwFrame )
{
	lpShape->lpMotion->nFrame = dwFrame;
}
C3_CORE_DLL_API
void Shape_ClearMatrix ( C3Shape *lpShape )
{
	// 清除运动矩阵
	D3DXMatrixIdentity ( &lpShape->lpMotion->matrix );
}

C3_CORE_DLL_API
BOOL Shape_DrawAlpha(C3Shape *lpShape, BOOL bLocal)
{
	// 首先转换线顶点
	D3DXMATRIX mm;

	if ( bLocal )
		mm = lpShape->lpMotion->lpFrames[lpShape->lpMotion->nFrame];
	else
		D3DXMatrixMultiply( &mm,
							&lpShape->lpMotion->lpFrames[lpShape->lpMotion->nFrame],
							&lpShape->lpMotion->matrix );

	D3DXVECTOR3 vec[2];
	D3DXVec3TransformCoord( &vec[0],
							(D3DXVECTOR3*)&lpShape->lpLine[0].lpVB[0],
							&mm );
	D3DXVec3TransformCoord( &vec[1],
							(D3DXVECTOR3*)&lpShape->lpLine[0].lpVB[1],
							&mm );
	if ( !lpShape->bFrist )
	{
		// 计算长度
		D3DXVECTOR3 sub;
		D3DXVec3Subtract ( &sub, &vec[0], &vec[1] );
		float len = D3DXVec3Length ( &sub );

		D3DXVECTOR3 last[2];

		last[0] = lpShape->last[0];
		last[1] = lpShape->last[1];
		
		D3DXVECTOR3 vPos;
		if ( bLocal )
			mm = lpShape->lpMotion->matrix;
		else
			D3DXMatrixIdentity ( &mm );
		// 计算插值的点
		for ( DWORD nn = 0; nn < lpShape->dwSmooth; nn++ )
		{
			D3DXVECTOR3 smooths[2];

			D3DXVec3Lerp ( &smooths[0],
						   &lpShape->last[0],
						   &vec[0],
						   1.0f / ( lpShape->dwSmooth + 1 ) * ( nn + 1 ) );
			D3DXVec3Lerp ( &smooths[1],
						   &lpShape->last[1],
						   &vec[1],
						   1.0f / ( lpShape->dwSmooth + 1 ) * ( nn + 1 ) );

			// 计算长度
			D3DXVec3Subtract ( &sub,
							   &smooths[0],
							   &smooths[1] );

			float lennow = D3DXVec3Length ( &sub );

			D3DXVec3Lerp ( &smooths[0],
						   &smooths[1],
						   &smooths[0],
						   len / lennow );

			D3DXVec3Subtract ( &sub,
							   &smooths[0],
							   &smooths[1] );

			lennow = D3DXVec3Length ( &sub );

			// 算刀光
			int cur = lpShape->dwSegmentCur * 6;

			lpShape->vb[cur + 0].x = smooths[0].x;
			lpShape->vb[cur + 0].y = smooths[0].y;
			lpShape->vb[cur + 0].z = smooths[0].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 0].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 0].u = 0;
			lpShape->vb[cur + 0].v = 0;

			lpShape->vb[cur + 1].x = smooths[1].x;
			lpShape->vb[cur + 1].y = smooths[1].y;
			lpShape->vb[cur + 1].z = smooths[1].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 1].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 1].u = 0;
			lpShape->vb[cur + 1].v = 0;

			lpShape->vb[cur + 2].x = last[1].x;
			lpShape->vb[cur + 2].y = last[1].y;
			lpShape->vb[cur + 2].z = last[1].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 2].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 2].u = 0;
			lpShape->vb[cur + 2].v = 0;


			lpShape->vb[cur + 3].x = last[0].x;
			lpShape->vb[cur + 3].y = last[0].y;
			lpShape->vb[cur + 3].z = last[0].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 3].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 3].u = 0;
			lpShape->vb[cur + 3].v = 0;

			lpShape->vb[cur + 4].x = last[1].x;
			lpShape->vb[cur + 4].y = last[1].y;
			lpShape->vb[cur + 4].z = last[1].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 4].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 4].u = 0;
			lpShape->vb[cur + 4].v = 0;

			lpShape->vb[cur + 5].x = smooths[0].x;
			lpShape->vb[cur + 5].y = smooths[0].y;
			lpShape->vb[cur + 5].z = smooths[0].z;
			lpShape->vb[lpShape->dwSegmentCur * 6 + 5].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
			lpShape->vb[cur + 5].u = 0;
			lpShape->vb[cur + 5].v = 0;

			lpShape->dwSegmentCur++;
			if ( lpShape->dwSegmentCur == lpShape->dwSegment )
				lpShape->dwSegmentCur = 0;

			last[0] = smooths[0];
			last[1] = smooths[1];
			
			if ( nn == lpShape->dwSmooth - 1 )
			{
				lpShape->last[0] = smooths[0];
				lpShape->last[1] = smooths[1];
			}

			for ( int i = 0; i < 6; i++ )
			{
				vPos.x = lpShape->vb[cur + i].x;
				vPos.y = lpShape->vb[cur + i].y;
				vPos.z = lpShape->vb[cur + i].z;
				
				D3DXVec3Project( &lpShape->pScreenPnt[cur + i],
								 &vPos,
								 &g_Viewport,
								 &g_ProjectMatrix,
								 &g_ViewMatrix,
								 &mm );
				
				if ( lpShape->pScreenPnt[cur + i].x < lpShape->TearAirTexRect.left )
					lpShape->TearAirTexRect.left = (int)lpShape->pScreenPnt[cur + i].x;
				else if ( lpShape->pScreenPnt[cur + i].x > lpShape->TearAirTexRect.right )
					lpShape->TearAirTexRect.right = (int)lpShape->pScreenPnt[cur + i].x;
				
				if ( lpShape->pScreenPnt[cur + i].y < lpShape->TearAirTexRect.top )
					lpShape->TearAirTexRect.top = (int)lpShape->pScreenPnt[cur + i].y;
				else if ( lpShape->pScreenPnt[cur + i].y > lpShape->TearAirTexRect.bottom )
					lpShape->TearAirTexRect.bottom = (int)lpShape->pScreenPnt[cur + i].y;
			}
		}
		int cur = lpShape->dwSegmentCur * 6;

		lpShape->vb[cur + 0].x = vec[0].x;
		lpShape->vb[cur + 0].y = vec[0].y;
		lpShape->vb[cur + 0].z = vec[0].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 0].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 0].u = 0;
		lpShape->vb[cur + 0].v = 0;

		lpShape->vb[cur + 1].x = vec[1].x;
		lpShape->vb[cur + 1].y = vec[1].y;
		lpShape->vb[cur + 1].z = vec[1].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 1].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 1].u = 0;
		lpShape->vb[cur + 1].v = 0;

		lpShape->vb[cur + 2].x = lpShape->last[1].x;
		lpShape->vb[cur + 2].y = lpShape->last[1].y;
		lpShape->vb[cur + 2].z = lpShape->last[1].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 2].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 2].u = 0;
		lpShape->vb[cur + 2].v = 0;


		lpShape->vb[cur + 3].x = lpShape->last[0].x;
		lpShape->vb[cur + 3].y = lpShape->last[0].y;
		lpShape->vb[cur + 3].z = lpShape->last[0].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 3].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 3].u = 0;
		lpShape->vb[cur + 3].v = 0;

		lpShape->vb[cur + 4].x = lpShape->last[1].x;
		lpShape->vb[cur + 4].y = lpShape->last[1].y;
		lpShape->vb[cur + 4].z = lpShape->last[1].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 4].color = D3DCOLOR_ARGB ( 255, 255, 255, 255 );
		lpShape->vb[cur + 4].u = 0;
		lpShape->vb[cur + 4].v = 0;

		lpShape->vb[cur + 5].x = vec[0].x;
		lpShape->vb[cur + 5].y = vec[0].y;
		lpShape->vb[cur + 5].z = vec[0].z;
		lpShape->vb[lpShape->dwSegmentCur * 6 + 5].color = D3DCOLOR_ARGB ( 255, 255, 255, 255);
		lpShape->vb[cur + 5].u = 0;
		lpShape->vb[cur + 5].v = 0;
		DWORD i = 0;
		for (  i = 0; i < 6; i++ )
		{
			vPos.x = lpShape->vb[cur + i].x;
			vPos.y = lpShape->vb[cur + i].y;
			vPos.z = lpShape->vb[cur + i].z;
			
			D3DXVec3Project( &lpShape->pScreenPnt[cur + i],
							 &vPos,
							 &g_Viewport,
							 &g_ProjectMatrix,
							 &g_ViewMatrix,
							 &mm );
			
			if ( lpShape->pScreenPnt[cur + i].x < lpShape->TearAirTexRect.left )	
				lpShape->TearAirTexRect.left = (int)lpShape->pScreenPnt[cur + i].x;
			else if ( lpShape->pScreenPnt[cur + i].x > lpShape->TearAirTexRect.right )
				lpShape->TearAirTexRect.right = (int)lpShape->pScreenPnt[cur + i].x;
			
			if ( lpShape->pScreenPnt[cur + i].y < lpShape->TearAirTexRect.top )
				lpShape->TearAirTexRect.top = (int)lpShape->pScreenPnt[cur + i].y;
			else if ( lpShape->pScreenPnt[cur + i].y > lpShape->TearAirTexRect.bottom )
				lpShape->TearAirTexRect.bottom = (int)lpShape->pScreenPnt[cur + i].y;
		}

		// Set the texcoord
		for ( i = 0; i < lpShape->dwSegment; i++ )
		{
			for ( int j = 0; j < 6; j++ )
			{
				if ( lpShape->vb[i * 6 + j].x == 0 )
				{
					lpShape->vb[i * 6 + j].u = 0;
					lpShape->vb[i * 6 + j].v = 0;
				}
				else
				{
					float u = ( lpShape->pScreenPnt[i * 6 + j].x - lpShape->TearAirTexRect.left ) / TEARAIR_TEX_SIZE;
					float v = ( lpShape->pScreenPnt[i * 6 + j].y - lpShape->TearAirTexRect.top ) / TEARAIR_TEX_SIZE;
					lpShape->vb[i * 6 + j].u = u - u / 10;
					lpShape->vb[i * 6 + j].v = v - v / 10;
				}
			}
		}

		lpShape->dwSegmentCur++;
		if ( lpShape->dwSegmentCur == lpShape->dwSegment )
			lpShape->dwSegmentCur = 0;

		LPDIRECT3DSURFACE9 pBackBufferSurface;
		LPDIRECT3DSURFACE9 pTextureSurface;
		POINT point = { 0, 0 };
		
		lpShape->pTearAirTex->GetSurfaceLevel( 0, &pTextureSurface );
	//	g_D3DDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBufferSurface );
		g_D3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBufferSurface );
		//g_D3DDevice->CopyRects( pBackBufferSurface, &lpShape->TearAirTexRect, 1, pTextureSurface, &point );
		g_D3DDevice->UpdateSurface(pBackBufferSurface, &lpShape->TearAirTexRect, pTextureSurface, &point );
		lpShape->LastTearAirTexRect.left	= lpShape->TearAirTexRect.left;
		lpShape->LastTearAirTexRect.right	= lpShape->TearAirTexRect.right;
		lpShape->LastTearAirTexRect.top		= lpShape->TearAirTexRect.top;	
		lpShape->LastTearAirTexRect.bottom	= lpShape->TearAirTexRect.bottom;
	
		char strText[64];
		sprintf( strText, "Capture the backbuffer, Width = %d, Height = %d!\n", lpShape->LastTearAirTexRect.right - lpShape->LastTearAirTexRect.left, lpShape->LastTearAirTexRect.bottom - lpShape->LastTearAirTexRect.top );
		::OutputDebugString( strText );

		g_D3DDevice->SetTexture( 0, lpShape->pTearAirTex );

		g_D3DDevice->SetTransform ( D3DTS_WORLD, &mm );

	//	if ( FAILED ( g_D3DDevice->SetVertexShader ( SHAPE_OUT_VERTEX ) ) )
		if ( FAILED (	g_D3DDevice->SetFVF(SHAPE_OUT_VERTEX)))
			return false;

		if ( FAILED ( g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLELIST,
													 lpShape->dwSegment * 2,
													 lpShape->vb,
													 sizeof ( ShapeOutVertex ) ) ) )
			return false;
	}
	else
	{
		// Tear air require less segment
		Shape_SetSegment( lpShape, 4 );
		ZeroMemory ( lpShape->vb, sizeof ( ShapeOutVertex ) * lpShape->dwSegment * 6 );
		lpShape->bFrist = false;

		// Create the tearair texture only the user invoke the Shape_DrawAlpha()
		if ( lpShape->pTearAirTex == NULL )
		{
			LPDIRECT3DSURFACE9	pBackBufferSurface;
			D3DSURFACE_DESC		BackBufferDesc;
			
			if ( g_D3DDevice->GetBackBuffer(0,0, D3DBACKBUFFER_TYPE_MONO, &pBackBufferSurface ) == D3D_OK )
			{
				pBackBufferSurface->GetDesc( &BackBufferDesc );
				//g_D3DDevice->CreateTexture( TEARAIR_TEX_SIZE, TEARAIR_TEX_SIZE, 1, 0, BackBufferDesc.Format, D3DPOOL_MANAGED, &lpShape->pTearAirTex );
				g_D3DDevice->CreateTexture( TEARAIR_TEX_SIZE, TEARAIR_TEX_SIZE, 1, 0, BackBufferDesc.Format, D3DPOOL_MANAGED, &lpShape->pTearAirTex ,0);
				::OutputDebugString( "Create the tear air texture!\n" );
			}
		}

		// Create the screen point buffer only the user invoke the Shape_DrawAlpha()
		if ( lpShape->pScreenPnt == NULL )
		{
			lpShape->pScreenPnt = new D3DXVECTOR3[lpShape->dwSegment * 6];
			ZeroMemory( lpShape->pScreenPnt, sizeof(D3DXVECTOR3) * lpShape->dwSegment * 6 );
		}

		lpShape->TearAirTexRect.left	= 1280;
		lpShape->TearAirTexRect.top		= 1280;
		lpShape->TearAirTexRect.right	= 0;
		lpShape->TearAirTexRect.bottom	= 0;

		lpShape->LastTearAirTexRect.left	= 0;
		lpShape->LastTearAirTexRect.top		= 0;
		lpShape->LastTearAirTexRect.right	= 0;
		lpShape->LastTearAirTexRect.bottom	= 0;
	}

	lpShape->last[0] = vec[0];
	lpShape->last[1] = vec[1];

	return true;
}