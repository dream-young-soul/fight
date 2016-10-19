#ifndef _c3_main_h_
#define _c3_main_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

//#include "d3d8.h"
//#include "d3dx8.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "c3_common.h"

const char		C3_VERSION[64] = "MAXFILE C3 00001";

extern C3_CORE_DLL_API LPDIRECT3D9				g_D3D;
extern C3_CORE_DLL_API LPDIRECT3DDEVICE9		g_D3DDevice;
extern C3_CORE_DLL_API D3DCAPS9				g_D3DCaps;
extern C3_CORE_DLL_API D3DDISPLAYMODE			g_DisplayMode;
extern C3_CORE_DLL_API HWND						g_hWnd;
extern C3_CORE_DLL_API D3DVIEWPORT9			g_Viewport;
extern C3_CORE_DLL_API D3DXMATRIX				g_ViewMatrix;
extern C3_CORE_DLL_API D3DXMATRIX				g_ProjectMatrix;
extern C3_CORE_DLL_API D3DPRESENT_PARAMETERS	g_Present;

extern C3_CORE_DLL_API CRITICAL_SECTION			g_CriticalSection;

// 1   成功
// 0   DX 版本错误
// -1  不支持硬件加速
// -2  不是 16 位模式
// -3  不支持 alpha 模式
C3_CORE_DLL_API
int Init3D ( HINSTANCE hInst,
		     const char *lpTitle,
		     DWORD dwWidth,
		     DWORD dwHeight,
		     BOOL bWindowed,
		     WNDPROC proc,
		     DWORD dwBackCount );
C3_CORE_DLL_API
int Init3DEx ( HWND hWnd,
			   DWORD dwWidth,
			   DWORD dwHeight,
			   BOOL bWindowed,
			   DWORD dwBackCount );
C3_CORE_DLL_API
void Quit3D ( void );

C3_CORE_DLL_API
BOOL Begin3D ( void );
C3_CORE_DLL_API
BOOL End3D ( void );

C3_CORE_DLL_API
int IfDeviceLost ( void );
C3_CORE_DLL_API
BOOL ResetDevice ( void );

C3_CORE_DLL_API
BOOL ClearBuffer ( BOOL bZBuffer, BOOL bTarget, D3DCOLOR color );
C3_CORE_DLL_API
BOOL Flip ( void );

C3_CORE_DLL_API
void SetRenderState ( D3DRENDERSTATETYPE state, DWORD dwValue );
C3_CORE_DLL_API
void SetTextureStageState ( DWORD dwStage,
							D3DTEXTURESTAGESTATETYPE type,
							DWORD dwValue );
C3_CORE_DLL_API
BOOL SetTexture ( DWORD dwStage, LPDIRECT3DTEXTURE9 lpTex );

/* 计算帧速率 */
C3_CORE_DLL_API
DWORD CalcRate ( void );
/* 限制刷新率 */
C3_CORE_DLL_API
BOOL LimitRate ( DWORD dwRate );

#endif