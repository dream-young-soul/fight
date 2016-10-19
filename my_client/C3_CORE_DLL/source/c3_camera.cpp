#include "..\include\c3_camera.h"
#include "..\include\c3_main.h"

C3_CORE_DLL_API
void Camera_Clear ( C3Camera *lpCamera )
{
	lpCamera->lpName = 0;
	lpCamera->lpFrom = 0;
	lpCamera->lpTo = 0;
	lpCamera->fNear = 10;
	lpCamera->fFar = 10000;
	lpCamera->fFov = D3DXToRadian ( 45 );

	lpCamera->dwFrameCount = 0;
	lpCamera->nFrame = 0;
}

C3_CORE_DLL_API
BOOL Camera_Load ( C3Camera **lpCamera,
				   char *lpName,
				   DWORD dwIndex )
{
	FILE *file = fopen ( lpName, "rb" );
	if ( !file )
		return false;

	char version[64];
	fread ( version, sizeof ( char ), 16, file );
	version[16] = '\0';
	if ( strcmp ( version, C3_VERSION ) != 0 )
	{
		MessageBox ( g_hWnd, "phy version error", "error", MB_OK );
		return false;
	}

	ChunkHeader chunk;
	DWORD add = 0;

	while ( 1 )
	{
		fread ( &chunk, sizeof ( ChunkHeader ), 1, file );
		if ( feof ( file ) )
		{
			fclose ( file );
			return false;
		}
		if ( chunk.byChunkID[0] == 'C' &&
			 chunk.byChunkID[1] == 'A' &&
			 chunk.byChunkID[2] == 'M' &&
			 chunk.byChunkID[3] == 'E' )
		{
			if ( add < dwIndex )
			{
				add++;
				fseek ( file, chunk.dwChunkSize, SEEK_CUR );
				continue;
			}

			*lpCamera = new C3Camera;
			Camera_Clear ( *lpCamera );

			// name
			DWORD temp;
			fread ( &temp, sizeof ( DWORD ), 1, file );
			( *lpCamera )->lpName = new char[temp + 1];
			fread ( ( *lpCamera )->lpName, 1, temp, file );
			( *lpCamera )->lpName[temp] = '\0';
			// fov
			//fread ( &( *lpCamera )->fFov, sizeof ( float ), 1, file );
			fseek ( file, sizeof ( float ), SEEK_CUR );
			fread ( &( *lpCamera )->dwFrameCount, sizeof ( DWORD ), 1, file );
			// from
			( *lpCamera )->lpFrom = new D3DXVECTOR3[( *lpCamera )->dwFrameCount];
			fread ( ( *lpCamera )->lpFrom,
					sizeof ( D3DXVECTOR3 ),
					( *lpCamera )->dwFrameCount,
					file );
			// to
			( *lpCamera )->lpTo = new D3DXVECTOR3[( *lpCamera )->dwFrameCount];
			fread ( ( *lpCamera )->lpTo,
					sizeof ( D3DXVECTOR3 ),
					( *lpCamera )->dwFrameCount,
					file );
			break;
		}
		else
			fseek ( file, chunk.dwChunkSize, SEEK_CUR );
	};
	fclose ( file );
	return true;
}
C3_CORE_DLL_API
BOOL Camera_Save ( char *lpName, C3Camera *lpCamera, BOOL bNew )
{
	FILE *file = fopen ( lpName, bNew ? "w+b" : "r+b" );
	if ( !file )
		return false;
	fseek ( file, 0, SEEK_END );

	// camera
	ChunkHeader chunk;
	chunk.byChunkID[0] = 'C';
	chunk.byChunkID[1] = 'A';
	chunk.byChunkID[2] = 'M';
	chunk.byChunkID[3] = 'E';
	chunk.dwChunkSize = 0;
	fwrite ( &chunk, sizeof ( chunk ), 1, file );

	// name
	DWORD length = strlen ( lpCamera->lpName );
	fwrite ( &length, sizeof ( DWORD), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	fwrite ( lpCamera->lpName, sizeof ( char ), length, file );
	chunk.dwChunkSize += sizeof ( char ) * length;

	// fov
	fwrite ( &lpCamera->fFov, sizeof ( float ), 1, file );
	chunk.dwChunkSize += sizeof ( float );

	fwrite ( &lpCamera->dwFrameCount, sizeof ( DWORD ), 1, file );
	chunk.dwChunkSize += sizeof ( DWORD );

	// from
	fwrite ( lpCamera->lpFrom, sizeof ( D3DXVECTOR3 ), lpCamera->dwFrameCount, file );
	chunk.dwChunkSize += sizeof ( D3DXVECTOR3 ) * lpCamera->dwFrameCount;
	
	// to
	fwrite ( lpCamera->lpTo, sizeof ( D3DXVECTOR3 ), lpCamera->dwFrameCount, file );
	chunk.dwChunkSize += sizeof ( D3DXVECTOR3 ) * lpCamera->dwFrameCount;

	fseek ( file, -( int )( chunk.dwChunkSize + sizeof ( chunk ) ), SEEK_CUR );
	fwrite ( &chunk, sizeof ( chunk ), 1, file );
	fseek ( file, 0, SEEK_END );

	fclose ( file );
	return true;
}
C3_CORE_DLL_API
void Camera_Unload ( C3Camera **lpCamera )
{
	SafeDeleteEx ( ( *lpCamera )->lpName );
	SafeDelete ( *lpCamera );
}
C3_CORE_DLL_API
void Camera_NextFrame ( C3Camera *lpCamera, int nStep )
{
	lpCamera->nFrame = ( lpCamera->nFrame + nStep ) % lpCamera->dwFrameCount;
}

C3_CORE_DLL_API
BOOL Camera_BuildView ( C3Camera *lpCamera, BOOL bSet )
{
	D3DXVECTOR3	up ( 0.0f, 0.0f, -1.0f );
	if ( FAILED ( D3DXMatrixLookAtLH ( &g_ViewMatrix,
									   &lpCamera->lpFrom[lpCamera->nFrame],
									   &lpCamera->lpTo[lpCamera->nFrame],
									   &up ) ) )
		return false;

	if ( bSet )
	{
		if ( FAILED ( g_D3DDevice->SetTransform ( D3DTS_VIEW, 
												  &g_ViewMatrix ) ) )
			return false;
	}
	return true;
}
C3_CORE_DLL_API
BOOL Camera_BuildProject ( C3Camera *lpCamera, BOOL bSet )
{
	if ( FAILED ( D3DXMatrixPerspectiveFovLH ( &g_ProjectMatrix,
											   lpCamera->fFov,
											   ( float )( ( float )g_DisplayMode.Width / ( float )g_DisplayMode.Height ),
											   lpCamera->fNear,
											   lpCamera->fFar ) ) )
		return false;
	
	if ( bSet )
	{
		if ( FAILED ( g_D3DDevice->SetTransform ( D3DTS_PROJECTION,
												  &g_ProjectMatrix ) ) )
			return false;
	}
	return true;
}
C3_CORE_DLL_API
BOOL Camera_BuildOrtho ( C3Camera *lpCamera,
						 float fWidth,
						 float fHeight,
						 BOOL bSet )
{
	if ( FAILED ( D3DXMatrixOrthoLH ( &g_ProjectMatrix,
									  fWidth,
									  fHeight,
									  lpCamera->fNear,
									  lpCamera->fFar ) ) )
		return false;
	
	if ( bSet )
	{
		if ( FAILED ( g_D3DDevice->SetTransform ( D3DTS_PROJECTION,
												  &g_ProjectMatrix ) ) )
			return false;
	}
	return true;
}

C3_CORE_DLL_API
void Camera_Process1stRotate ( C3Camera *lpCamera,
							   float fHRadian,
							   float fVRadian )
{
	D3DXVECTOR3 vec = lpCamera->lpTo[lpCamera->nFrame] - lpCamera->lpFrom[lpCamera->nFrame];

	// 旋转 H
	D3DXMATRIX matrix;
	D3DXMatrixRotationZ ( &matrix, fHRadian );
	D3DXVec3TransformCoord ( &vec, &vec, &matrix );
	
	// 旋转 V
	// 计算旋转轴
	D3DXVECTOR3 vecend;
	D3DXVECTOR3 axis;
	axis.x = 0;
	axis.y = 0;
	axis.z = 1;
	D3DXVec3Cross ( &axis, &axis, &vec );

	D3DXMatrixRotationAxis ( &matrix, &axis, fVRadian );
	D3DXVec3TransformCoord ( &vecend, &vec, &matrix );

	// 角度限制
	axis.x = 0;
	axis.y = 0;
	axis.z = 1;

	D3DXVECTOR3 limit;
	D3DXVec3Normalize ( &limit, &vecend );
	float radian = ( float )acos ( D3DXVec3Dot ( &axis, &limit ) );

	if ( radian > D3DXToRadian ( 10 ) && radian < D3DXToRadian ( 170 ) )
		lpCamera->lpTo[lpCamera->nFrame] = lpCamera->lpFrom[lpCamera->nFrame] + vecend;
	else
		lpCamera->lpTo[lpCamera->nFrame] = lpCamera->lpFrom[lpCamera->nFrame] + vec;
}
C3_CORE_DLL_API
void Camera_Process1stTranslate ( C3Camera *lpCamera,
								  float fDirRandian,
								  float fStep )
{
	D3DXVECTOR3 axis;
	axis.x = 0;
	axis.y = 0;
	axis.z = 1;

	// 移动
	D3DXVECTOR3 vec = lpCamera->lpTo[lpCamera->nFrame] - lpCamera->lpFrom[lpCamera->nFrame];
	
	// 求出移动平面法线
	D3DXVec3Cross ( &axis, &vec, &axis );
	D3DXVec3Cross ( &axis, &vec, &axis );
	D3DXVec3Normalize ( &axis, &axis );

	D3DXMATRIX matrix;
	D3DXMatrixRotationAxis ( &matrix, &axis, fDirRandian );
	D3DXVec3TransformCoord ( &vec, &vec, &matrix );

	axis.x = 0;
	axis.y = 0;
	axis.z = 0;
	D3DXVec3Lerp ( &vec,
				   &axis,
				   &vec,
				   fStep / D3DXVec3Length ( &vec ) );

	lpCamera->lpFrom[lpCamera->nFrame] += vec;
	lpCamera->lpTo[lpCamera->nFrame] += vec;
}
C3_CORE_DLL_API
void Camera_ProcessXYTranslate ( C3Camera *lpCamera,
								 float fDirRandian,
								 float fStep )
{
	D3DXVECTOR3 axis;
	axis.x = 0;
	axis.y = 0;
	axis.z = -1;

	// 移动
	D3DXVECTOR3 vec = lpCamera->lpTo[lpCamera->nFrame] - lpCamera->lpFrom[lpCamera->nFrame];
	vec.z = 0;
	
	D3DXMATRIX matrix;
	D3DXMatrixRotationAxis ( &matrix, &axis, fDirRandian );
	D3DXVec3TransformCoord ( &vec, &vec, &matrix );

	axis.x = 0;
	axis.y = 0;
	axis.z = 0;
	D3DXVec3Lerp ( &vec,
				   &axis,
				   &vec,
				   fStep / D3DXVec3Length ( &vec ) );

	lpCamera->lpFrom[lpCamera->nFrame] += vec;
	lpCamera->lpTo[lpCamera->nFrame] += vec;
}