#include	 "..\include\c3_main.h"
#include	 "..\include\c3_texture.h"
#include     "..\include\c3_datafile.h"

C3_CORE_DLL_API LPDIRECT3D9				g_D3D = 0;
C3_CORE_DLL_API LPDIRECT3DDEVICE9		g_D3DDevice = 0;
C3_CORE_DLL_API D3DCAPS9				g_D3DCaps;
C3_CORE_DLL_API D3DDISPLAYMODE			g_DisplayMode;
C3_CORE_DLL_API HWND					g_hWnd;
C3_CORE_DLL_API D3DVIEWPORT9			g_Viewport;
C3_CORE_DLL_API D3DXMATRIX				g_ViewMatrix;
C3_CORE_DLL_API D3DXMATRIX				g_ProjectMatrix;
C3_CORE_DLL_API D3DPRESENT_PARAMETERS	g_Present;

C3_CORE_DLL_API CRITICAL_SECTION		g_CriticalSection;

BOOL APIENTRY DllMain ( HANDLE hModule, 
                        DWORD  ul_reason_for_call, 
                        LPVOID lpReserved )
{
    switch ( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

C3_CORE_DLL_API
int Init3D ( HINSTANCE hInst,
			 const char *lpTitle,
			 DWORD dwWidth,
			 DWORD dwHeight,
			 BOOL bWindowed,
			 WNDPROC proc,
			 DWORD dwBackCount )
{
	// 创建主窗口
	WNDCLASS wc;
	ZeroMemory ( &wc, sizeof ( wc ) );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon ( hInst, IDI_APPLICATION );
	wc.hCursor = LoadCursor ( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH ) GetStockObject ( BLACK_BRUSH );
	wc.lpszClassName = lpTitle;
	RegisterClass ( &wc );

	DEVMODE mode;
	mode.dmSize = sizeof ( mode );
	EnumDisplaySettings ( NULL, ENUM_CURRENT_SETTINGS, &mode );

	HWND hWnd = CreateWindow ( lpTitle,
							   lpTitle,
							   WS_POPUP,
							   ( mode.dmPelsWidth - dwWidth ) / 2,
							   ( mode.dmPelsHeight - dwHeight ) / 2,
							   dwWidth,
							   dwHeight,
							   GetDesktopWindow (),
							   NULL,
							   hInst,
							   NULL );
	if ( !hWnd )
		return 0;

	return Init3DEx ( hWnd,
					  dwWidth,
					  dwHeight,
					  bWindowed,
					  dwBackCount );
}
C3_CORE_DLL_API
int Init3DEx ( HWND hWnd,
		  	   DWORD dwWidth,
			   DWORD dwHeight,
			   BOOL bWindowed,
			   DWORD dwBackCount )
{
	g_DisplayMode.Width = dwWidth;
	g_DisplayMode.Height = dwHeight;
	g_DisplayMode.RefreshRate = 60;

	// 创建 d3d
	
	g_D3D = Direct3DCreate9 ( D3D_SDK_VERSION );
	
	if ( NULL == g_D3D )
		return 0;

	// 得到设备能力
	if ( FAILED ( g_D3D->GetDeviceCaps ( D3DADAPTER_DEFAULT,
										 D3DDEVTYPE_HAL,
										 &g_D3DCaps ) ) )
		return -1;

	// 测试 565 模式
	if ( FAILED ( g_D3D->CheckDeviceType ( D3DADAPTER_DEFAULT,
										   D3DDEVTYPE_HAL,
										   D3DFMT_R5G6B5,
										   D3DFMT_R5G6B5,
										   bWindowed ) ) )
	{
		// 测试 555 模式
		if ( FAILED ( g_D3D->CheckDeviceType ( D3DADAPTER_DEFAULT,
											   D3DDEVTYPE_HAL,
											   D3DFMT_X1R5G5B5,
											   D3DFMT_X1R5G5B5,
											   bWindowed ) ) )
		{
			g_D3D->Release ();
			g_D3D = 0;
			return -2;
		}
		else
			g_DisplayMode.Format = D3DFMT_X1R5G5B5;
	}
	else
		g_DisplayMode.Format = D3DFMT_R5G6B5;
	// 创建 d3d device
    ZeroMemory ( &g_Present, sizeof ( D3DPRESENT_PARAMETERS ) );
    g_Present.Windowed = bWindowed;
	g_Present.BackBufferCount = dwBackCount;
	g_Present.BackBufferWidth = g_DisplayMode.Width;
	g_Present.BackBufferHeight = g_DisplayMode.Height;
	g_Present.hDeviceWindow = g_hWnd;
    g_Present.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_Present.BackBufferFormat = g_DisplayMode.Format;
    g_Present.EnableAutoDepthStencil = true;
    g_Present.AutoDepthStencilFormat = D3DFMT_D16;
	
	if ( FAILED ( g_D3D->CreateDevice ( D3DADAPTER_DEFAULT,
									    D3DDEVTYPE_HAL,
									    hWnd,
									    D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
									    &g_Present,
									    &g_D3DDevice ) ) )
	{
		if ( FAILED ( g_D3D->CreateDevice ( D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
											&g_Present,
											&g_D3DDevice ) ) )
		{
			g_DisplayMode.Format = D3DFMT_A8R8G8B8;
			g_Present.BackBufferFormat = g_DisplayMode.Format;
			if ( FAILED ( g_D3D->CreateDevice ( D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
											&g_Present,
											&g_D3DDevice ) ) )
			{
						return -1;
			}
		}
	}

	g_Viewport.X = 0;
	g_Viewport.Y = 0;
	g_Viewport.Width = dwWidth;
	g_Viewport.Height = dwHeight;
	g_Viewport.MinZ = 0.0f;
	g_Viewport.MaxZ = 1.0f;
	if ( FAILED ( g_D3DDevice->SetViewport ( &g_Viewport ) ) )
		return 0;

	ShowWindow ( hWnd, SW_SHOW );
	UpdateWindow ( hWnd ); 

	// 设置缺省渲染状态
	SetRenderState ( D3DRS_AMBIENT, D3DCOLOR_ARGB ( 255, 255, 255, 255 ) );
	SetRenderState ( D3DRS_LIGHTING, true );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_CW );
	SetRenderState ( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	//SetRenderState (D3DRS_EDGEANTIALIAS, true);
	//边缘抗锯齿使用D3DRS_ANTIALIASEDLINEENABLE代替
	SetRenderState(D3DRS_ANTIALIASEDLINEENABLE,true);
	SetRenderState (D3DRS_MULTISAMPLEANTIALIAS, true);
	//dx8--------------------
	//SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	//SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    //SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
	//--------------------
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	for ( int t = 0; t < TEX_MAX; t++ )
		g_lpTex[t] = 0;

	InitializeCriticalSection ( &g_CriticalSection );

	// test alpha
	if ( !( g_D3DCaps.TextureCaps & D3DPTEXTURECAPS_ALPHA ) )
		return -3;

	// hwnd
	g_hWnd = hWnd;

	return 1;
}

C3_CORE_DLL_API
void Quit3D ( void )
{
	SafeRelease ( g_D3DDevice );
	SafeRelease ( g_D3D );

	DeleteCriticalSection ( &g_CriticalSection );
}

C3_CORE_DLL_API
BOOL Begin3D ( void )
{
	if ( FAILED ( g_D3DDevice->BeginScene () ) )
		return false;
	return true;
}
C3_CORE_DLL_API
BOOL End3D ( void )
{
	if ( FAILED ( g_D3DDevice->EndScene () ) )
		return false;
	return true;
}

C3_CORE_DLL_API
int IfDeviceLost ( void )
{
	HRESULT result;
    if ( FAILED ( result = g_D3DDevice->TestCooperativeLevel () ) )
    {
		if ( result == D3DERR_DEVICELOST )
			return 1;

		if ( result == D3DERR_DEVICENOTRESET )
			return 2;
    }
	return 0;
}
C3_CORE_DLL_API
BOOL ResetDevice ( void )
{
	if ( FAILED ( g_D3DDevice->Reset ( &g_Present ) ) )
		return false;
	SetRenderState ( D3DRS_AMBIENT, D3DCOLOR_ARGB ( 255, 255, 255, 255 ) );
	return true;
}

C3_CORE_DLL_API
BOOL ClearBuffer ( BOOL bZBuffer, BOOL bTarget, D3DCOLOR color )
{
	DWORD dwFlags = 0;

	if ( bZBuffer )
		dwFlags |= D3DCLEAR_ZBUFFER;
	if ( bTarget )
		dwFlags |= D3DCLEAR_TARGET;

	if ( FAILED ( g_D3DDevice->Clear ( 0, 0, dwFlags, color, 1.0f, 0 ) ) )
		return false;
	return true;
}
C3_CORE_DLL_API
BOOL Flip ( void )
{
	if ( FAILED ( g_D3DDevice->Present ( 0, 0, 0, 0 ) ) )
		return false;
	return true;
}

C3_CORE_DLL_API
void SetRenderState ( D3DRENDERSTATETYPE state, DWORD dwValue )
{
	DWORD current;
	g_D3DDevice->GetRenderState ( state, &current );
	if ( current != dwValue )
		g_D3DDevice->SetRenderState ( state, dwValue );
}
C3_CORE_DLL_API
void SetTextureStageState ( DWORD dwStage,
							D3DTEXTURESTAGESTATETYPE type,
							DWORD dwValue )
{
	DWORD current;
	g_D3DDevice->GetTextureStageState ( dwStage, type, &current );
	if ( current != dwValue )
		g_D3DDevice->SetTextureStageState ( dwStage, type, dwValue );
}

/* 计算帧速率 */
C3_CORE_DLL_API
DWORD CalcRate ( void )
{
	static DWORD countcur = 0, countold = 0;
	static DWORD timecur = 0, timeold = 0;
	static DWORD rate = 0;

	countcur++;
	timecur = timeGetTime ();

	if ( timecur - timeold > 1000 )
	{
		rate = ( countcur - countold ) * 1000 / ( timecur - timeold );

		timeold = timecur;
		countold = countcur;
	}
	return rate;
}
/* 限制刷新率 */
C3_CORE_DLL_API
BOOL LimitRate ( DWORD dwRate )
{
	static DWORD dwFrame = 0;
	static DWORD dwFrameOld = 0;
	dwFrame = timeGetTime ();
	if ( dwFrame - dwFrameOld >= dwRate )
	{
		dwFrameOld = dwFrame;
		return TRUE;
	}
	return FALSE;
}
