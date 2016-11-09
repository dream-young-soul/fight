#ifndef _c3_common_h_
#define	_c3_common_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#ifndef	STRICT
#define	STRICT
#endif

#ifndef	WIN32_LEAN_AND_MEAN
#define	WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <math.h>
#include <process.h>
#include <signal.h>

#include "d3d8.h"
#include "d3dx8.h"

#ifdef _DEBUG
	#define	Assert(Expression) ( ( Expression ) ? ( void ) NULL : ( void ) AssertDialog ( #Expression, __FILE__, __LINE__ ) )
#else
	#define	Assert(Expression) ( void ) NULL
#endif

/* 使用对话框输出错误信息 */
C3_CORE_DLL_API
void AssertDialog ( char cExpression[], char cFile[], int nLine );
/* 显示错误信息对话框 */
C3_CORE_DLL_API
void ErrorMessage ( char cMessage[] );

#define		SafeRelease(COM)			{ if ( COM ) { COM->Release (); COM = 0; } }
#define		SafeDelete(Pointer)			{ if ( Pointer ) { delete Pointer, Pointer = 0; } }
#define		SafeDeleteEx(Pointer)		{ if ( Pointer ) { delete [] Pointer, Pointer = 0; } } 

#define		C3_TRY(Expression)			FAILED ( Expression ) ? C3_OutputMessage ( #Expression, Expression, __FILE__, __LINE__ ) : void ( NULL )

C3_CORE_DLL_API
void C3_OutputMessage ( char chExpression[], HRESULT hResult, char chFile[], int nLine );

/* 产生随机数 */
C3_CORE_DLL_API
int Random ( int nMin, int nMax );
/*
	比较两个浮点数
	--------------
	返回 -1 小于 0 等于 1 大于
*/
C3_CORE_DLL_API
int FloatCmp ( float f0, float f1, float fDim = 0.0001f );

C3_CORE_DLL_API
void CutString ( char *lpString, DWORD dwLevel );

/*--------------------------------------------------------
	Chunk 头结构 ( ChunkHeader )
	Examples: typedef ChunkHeader WaveChunkHeader;
--------------------------------------------------------*/
struct ChunkHeader
{
	BYTE byChunkID[4];							// Chunk ID
	DWORD dwChunkSize;							// Chunk 大小
};

extern C3_CORE_DLL_API FILE					*g_filetemp;

C3_CORE_DLL_API
void Common_AddDnpDisperseFile(LPCSTR name);
C3_CORE_DLL_API
FILE* Common_OpenDnp( LPCSTR name, int &nSize);
C3_CORE_DLL_API
FILE* Common_MoveDnpFPtr(FILE *file, unsigned long usSize);

C3_CORE_DLL_API
void Common_BeforeUseDnp();
C3_CORE_DLL_API
void Common_AfterUseDnp();

C3_CORE_DLL_API
FILE* Common_OpenRes ( LPCSTR name );
C3_CORE_DLL_API
HANDLE Common_OpenResPack ( LPCSTR name, int &nSize );
C3_CORE_DLL_API
void Common_ClearRes ( FILE *file );
C3_CORE_DLL_API
void Common_GetChunk ( FILE *file, ChunkHeader *chunk );
C3_CORE_DLL_API
void Common_SeekRes ( FILE *file, int seek );
C3_CORE_DLL_API
BOOL Common_IsEofRes ( );
C3_CORE_DLL_API
void Common_Translate ( D3DXMATRIX *matrix, float x, float y, float z );
C3_CORE_DLL_API
void Common_Rotate ( D3DXMATRIX *matrix, float x, float y, float z );
C3_CORE_DLL_API
void Common_Scale ( D3DXMATRIX *matrix, float x, float y, float z );
C3_CORE_DLL_API
void Common_Shadow ( D3DXMATRIX *matrix,
					 D3DXVECTOR4 *lightpos,
					 D3DXVECTOR3 *planepoint,
					 D3DXVECTOR3 *planenor );
C3_CORE_DLL_API
void BuildRay ( int nX,
				int nY,
				D3DXVECTOR3 *lpOrig,
				D3DXVECTOR3 *lpDir );

C3_CORE_DLL_API
void IntersectPlane ( D3DXVECTOR3 *lpOrg,
					  D3DXVECTOR3 *lpDir,
					  D3DXVECTOR3 *lpPlaneNor,
					  D3DXVECTOR3 *lpPlaneVec,
					  D3DXVECTOR3 *lpResult );

#endif