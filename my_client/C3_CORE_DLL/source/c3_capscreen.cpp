#include	"..\include\c3_capscreen.h"

C3_CORE_DLL_API
void CapScreen ( char *lpName )
{
    BITMAPINFO bi;
    ZeroMemory ( &bi.bmiHeader, sizeof ( BITMAPINFOHEADER ) );
    bi.bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
    bi.bmiHeader.biWidth = g_DisplayMode.Width;
    bi.bmiHeader.biHeight = -1*int(g_DisplayMode.Height);
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biBitCount = 24;

	void *buffer;
	HDC bitmapDC = CreateCompatibleDC ( NULL );
    HBITMAP bitmap = CreateDIBSection ( bitmapDC,
										&bi,
										DIB_RGB_COLORS,
										&buffer,
										0,
										0 );
    SelectObject ( bitmapDC, bitmap );

	HDC wndDC;
	wndDC = GetDC ( g_hWnd );

	BitBlt ( bitmapDC,
			 0,
			 0,
			 g_DisplayMode.Width,
			 g_DisplayMode.Height, 
			 wndDC,
			 0,
			 0,
			 SRCCOPY );

	JPGEncode ( lpName,
				g_DisplayMode.Width,
				g_DisplayMode.Height,
				( unsigned char* )buffer,
				IJL_BGR,
				95);

	ReleaseDC ( g_hWnd, wndDC );
	ReleaseDC ( NULL, bitmapDC );

	DeleteObject ( bitmap );
}

BOOL JPGEncode ( char *lpJpegName,
				 DWORD dwWidth,
				 DWORD dwHeight, 
				 unsigned char *lpBuffer,
				 _IJL_COLOR mode,
				 DWORD dwQuality )
{
	JPEG_CORE_PROPERTIES pro;
	if ( IJL_OK != ijlInit ( &pro ) )
		return false;

	pro.DIBWidth = dwWidth;
	pro.DIBHeight = dwHeight;
	pro.DIBBytes = lpBuffer;
	pro.DIBPadBytes = 0;
	pro.DIBChannels = 3;
	pro.DIBColor = mode;

	pro.JPGFile = lpJpegName;
	pro.JPGWidth = dwWidth;
	pro.JPGHeight = dwHeight;
	pro.JPGChannels  = 3;
	pro.JPGColor = IJL_YCBCR;
	pro.JPGSubsampling = IJL_411;
	pro.jquality = dwQuality;

	if ( IJL_OK != ijlWrite ( &pro, IJL_JFILE_WRITEWHOLEIMAGE ) )
		return false;

	ijlFree ( &pro );
	return true;
}
