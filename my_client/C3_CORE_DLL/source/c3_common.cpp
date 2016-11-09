#pragma warning(disable:4786)
#include	"..\include\C3_Main.h"
#include	"..\include\c3_datafile.h"
#include	"..\include\DnFile.h"

#include    <io.h>

/* 使用对话框输出错误信息 */
C3_CORE_DLL_API
void AssertDialog ( char cExpression[], char cFile[], int nLine )
{
    char OutputMessage[256];
	wsprintf ( OutputMessage, "Assert ( %s );\nFile: %s\nLine: %d", cExpression, cFile, nLine );
	ErrorMessage ( OutputMessage );
}
/* 显示错误信息对话框 */
C3_CORE_DLL_API
void ErrorMessage ( char cMessage[] )
{
	switch ( MessageBox ( g_hWnd, cMessage, "ERROR MESSAGE", MB_ABORTRETRYIGNORE ) )
	{
	case IDABORT:
		// 发出中断信号
		raise ( SIGABRT );
		// 有时中断信号会被忽略，因此使用另一种方法退出
		exit ( 3 );
		break;
	case IDRETRY:
		// 返回代码的中断处
		_asm int 3;
		break;
	case IDIGNORE:
		ShowWindow ( GetActiveWindow (), SW_SHOW );
		break;
	};
}

/* 产生随机数 */
C3_CORE_DLL_API
int Random ( int nMin, int nMax )
{
	int ra = rand ();
	return ra % ( nMax + 1 - nMin ) + nMin;
}
C3_CORE_DLL_API
int FloatCmp ( float f0, float f1, float fDim )
{
	if ( fabs ( f0 - f1 ) < fDim )
		return 0;
	if ( f0 > f1 )
		return 1;
	return -1;
}

C3_CORE_DLL_API
void CutString ( char *lpString, DWORD dwLevel )
{
	if ( dwLevel == 0 )
		return;

	DWORD l;
	DWORD nowS = 0;
	for (  l = 0; l < strlen ( lpString ); l++ )
	{
		if ( lpString[l] == '\\' )
		{
			nowS++;
			if ( nowS == dwLevel )
			{
				l++;
				break;
			}
		}
	}
	if ( nowS < dwLevel )
		return;
	DWORD n;
	for (  n = 0; n < strlen ( lpString ) - l; n++ )
		lpString[n] = lpString[n + l];

	lpString[n] = '\0';
}

C3_CORE_DLL_API FILE					*g_filetemp;
C3_CORE_DLL_API
void Common_BeforeUseDnp()
{
	g_objDnFile.BeforeUseDnFile();
}
C3_CORE_DLL_API
void Common_AfterUseDnp()
{
	g_objDnFile.AfterUseDnFile();
}

C3_CORE_DLL_API
void Common_AddDnpDisperseFile(LPCSTR name)
{
	g_objDnFile.AddDisperseFile(name);
}

C3_CORE_DLL_API
FILE* Common_OpenDnp( LPCSTR name, int &nSize )
{
	unsigned long uSize;
	FILE* fp = g_objDnFile.GetFPtr(name, uSize);
	nSize = uSize;
	if(fp)
		fseek(fp, 16, SEEK_CUR);
	return fp;
}
C3_CORE_DLL_API
FILE* Common_MoveDnpFPtr(FILE *file, unsigned long usSize)
{
	if(!file)
		return NULL;
	FILE* fp = file;
	fseek(fp, usSize, SEEK_CUR);
	return fp;
}

C3_CORE_DLL_API
FILE* Common_OpenRes ( LPCSTR name )
{
	if ( g_filetemp )
		fclose ( g_filetemp );
	g_filetemp = fopen ( name, "rb" );
	if ( g_filetemp )
		fseek ( g_filetemp, 16, SEEK_SET );
	return g_filetemp;
}

C3_CORE_DLL_API
HANDLE Common_OpenResPack ( LPCSTR name, int &nSize )
{
	HANDLE f;
	DWORD id = pack_name ( name ) ;
	DWORD fid= real_name ( name ) ;//string_id(filename);
	DWORD m_Offset ;
	C3DataFileIndex * pf;
	int i;
	// begin search the WFile
	for ( i=0; i < MAXDATAFILE; i++ )
	{
		if ( DataFile_IsOpen ( &_WDF[i], id ) )
		{
			pf = DataFile_SearchFile ( &_WDF[i], fid ) ;
			if (pf==0)
				return false;
			f = DataFile_GetFileHandle ( &_WDF[i] ) ;
			m_Offset = pf->offset;
			nSize = pf->size;
			SetFilePointer(f,m_Offset + 16,0,FILE_BEGIN);
			break;
		}
	}
	if ( i == MAXDATAFILE )
		return NULL;
	else
		return f ;
}

C3_CORE_DLL_API
void Common_ClearRes ( FILE *file )
{
	fclose ( g_filetemp );
}
C3_CORE_DLL_API
void Common_GetChunk ( FILE *file, ChunkHeader *chunk )
{
	fread ( chunk, sizeof ( ChunkHeader ), 1, file );
}
C3_CORE_DLL_API
void Common_SeekRes ( FILE *file, int seek )
{
	fseek ( file, seek, SEEK_CUR );
}

C3_CORE_DLL_API
BOOL Common_IsEofRes ( )
{
	if ( ftell ( g_filetemp ) >= filelength ( fileno ( g_filetemp ) ) )
		return true ;
	return false ;
}


C3_CORE_DLL_API
void Common_Translate ( D3DXMATRIX *matrix, float x, float y, float z )
{
	D3DXMATRIX m;
	D3DXMatrixTranslation ( &m, x, y, z );
	D3DXMatrixMultiply ( matrix,
					     matrix,
						 &m );
}
C3_CORE_DLL_API
void Common_Rotate ( D3DXMATRIX *matrix, float x, float y, float z )
{
	D3DXMATRIX m;

	D3DXMatrixRotationX ( &m, x );
	D3DXMatrixMultiply ( matrix,
					     matrix,
						 &m );

	D3DXMatrixRotationY ( &m, y );
	D3DXMatrixMultiply ( matrix,
					     matrix,
						 &m );

	D3DXMatrixRotationZ ( &m, z );
	D3DXMatrixMultiply ( matrix,
					     matrix,
						 &m );
}
C3_CORE_DLL_API
void Common_Scale ( D3DXMATRIX *matrix, float x, float y, float z )
{
	D3DXMATRIX m;
	D3DXMatrixScaling ( &m, x, y, z );
	D3DXMatrixMultiply ( matrix,
					     matrix,
						 &m );
}
C3_CORE_DLL_API
void Common_Shadow ( D3DXMATRIX *matrix,
					 D3DXVECTOR4 *lightpos,
					 D3DXVECTOR3 *planepoint,
					 D3DXVECTOR3 *planenor )
{
	D3DXPLANE plane;
	D3DXPlaneFromPointNormal ( &plane, planepoint, planenor );

	D3DXMATRIX m;
	D3DXMatrixShadow ( &m, lightpos, &plane );
	D3DXMatrixMultiply ( matrix,
					     matrix,
						 &m );
}

C3_CORE_DLL_API
void BuildRay ( int nX,
				int nY,
				D3DXVECTOR3 *lpOrig,
				D3DXVECTOR3 *lpDir )
{
	D3DXMATRIX matrix;
	g_D3DDevice->GetTransform ( D3DTS_PROJECTION, &matrix );

	D3DXVECTOR3 vec;
	vec.x =  ( ( ( 2.0f * nX ) / g_DisplayMode.Width  ) - 1 ) / matrix._11;
	vec.y = -( ( ( 2.0f * nY ) / g_DisplayMode.Height ) - 1 ) / matrix._22;
	vec.z =  1.0f;

	g_D3DDevice->GetTransform ( D3DTS_VIEW, &matrix );
	D3DXMatrixInverse ( &matrix, 0, &matrix );

	lpDir->x = vec.x * matrix._11 + vec.y * matrix._21 + vec.z * matrix._31;
	lpDir->y = vec.x * matrix._12 + vec.y * matrix._22 + vec.z * matrix._32;
	lpDir->z = vec.x * matrix._13 + vec.y * matrix._23 + vec.z * matrix._33;

	lpOrig->x = matrix._41;
	lpOrig->y = matrix._42;
	lpOrig->z = matrix._43;

	D3DXVec3Normalize ( lpDir, lpDir );
}
C3_CORE_DLL_API
void IntersectPlane ( D3DXVECTOR3 *lpOrg,
					  D3DXVECTOR3 *lpDir,
					  D3DXVECTOR3 *lpPlaneNor,
					  D3DXVECTOR3 *lpPlaneVec,
					  D3DXVECTOR3 *lpResult )
{
	// 计算点到平面的距离
	float l = D3DXVec3Length ( lpDir );
	D3DXVECTOR3 vec = *lpOrg - *lpPlaneVec;
	float dist = D3DXVec3Dot ( &vec, lpPlaneNor );	

	D3DXVec3Scale ( &vec, lpDir, dist );
	vec = *lpOrg + vec;

	D3DXPLANE plane;
	D3DXPlaneFromPointNormal ( &plane, lpPlaneVec, lpPlaneNor );
	D3DXPlaneIntersectLine ( &vec, &plane, lpOrg, &vec );


/*

	// 线段是否与平面有交点
	D3DXVECTOR3 e0, e1, normal, temp;

	e0 = *lpV1 - *lpV0;
	e1 = *lpV0 - *lpV2;
	D3DXVec3Cross ( &normal, &e0, &e1 );
	D3DXVec3Normalize ( &normal, &normal );

	temp = *lpStart - *lpV0;
	float ftemp1 = D3DXVec3Dot ( &temp, &normal );

	temp = *lpStart - *lpEnd;
	float ftemp2 = D3DXVec3Dot ( &temp, &normal );

	float fScale = ftemp1 / ftemp2;
	if ( FloatCmp ( fScale, 0 ) < 0 || FloatCmp ( fScale, 1 ) > 0 )
		return FALSE;
	D3DXVec3Lerp ( lpResult, lpStart, lpEnd, fScale );
	
	// 交点是否在三角形内
	D3DXVECTOR3 E2, E3, E4;*/
}