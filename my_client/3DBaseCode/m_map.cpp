
//**********************************************************
// 代码编辑器
//**********************************************************

#include "m_map.h"

CElement::CElement ()
{
    m_nTex = -1;
}
CElement::~CElement ()
{
    Release ();
}

BOOL CElement::Create ( LPCSTR lpFilename )
{
    if ( m_nTex > -1 )
    {
        Texture_Unload ( &g_lpTex[m_nTex] );
    }
    C3Texture* tex;
    if ( Texture_Load ( &tex, ( char* )lpFilename, 1 ) == -1 )
    {
        return false;
    }
    m_nTex = tex->nID;
    // 创建一个平行于 xz 轴的平面
    m_Vec[0].x  = 0;
    m_Vec[0].y  = 0;
    m_Vec[0].z  = 0;
    m_Vec[0].nx = 0;
    m_Vec[0].ny = -1;
    m_Vec[0].nz = 0;
    m_Vec[0].u  = 0;
    m_Vec[0].v  = 0;
    m_Vec[1].x  = ( float )tex->Info.Width;
    m_Vec[1].y  = 0;
    m_Vec[1].z  = 0;
    m_Vec[1].nx = 0;
    m_Vec[1].ny = -1;
    m_Vec[1].nz = 0;
    m_Vec[1].u  = 1;
    m_Vec[1].v  = 0;
    m_Vec[2].x  = 0;
    m_Vec[2].y  = 0;
    m_Vec[2].z  = ( float )tex->Info.Height;
    m_Vec[2].nx = 0;
    m_Vec[2].ny = -1;
    m_Vec[2].nz = 0;
    m_Vec[2].u  = 0;
    m_Vec[2].v  = 1;
    m_Vec[3].x  = ( float )tex->Info.Width;
    m_Vec[3].y  = 0;
    m_Vec[3].z  = ( float )tex->Info.Height;
    m_Vec[3].nx = 0;
    m_Vec[3].ny = -1;
    m_Vec[1].nz = 0;
    m_Vec[3].u  = 1;
    m_Vec[3].v  = 1;
    return true;
}
void CElement::Release ( void )
{
    if ( m_nTex > -1 )
    {
        Texture_Unload ( &g_lpTex[m_nTex] );
    }
}
void CElement::Prepare ( void )
{
    SetRenderState ( D3DRS_ZENABLE, false );
    SetRenderState ( D3DRS_ZWRITEENABLE, false );
    SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_FLAT );
    SetRenderState ( D3DRS_DITHERENABLE, false );
    SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
    SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
    SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
	//g_D3DDevice->SetSoftwareVertexProcessing(FALSE);
    SetRenderState ( D3DRS_SOFTWAREVERTEXPROCESSING, false );
    SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
    SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
    SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT );
	//g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
	//g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	//g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
    SetTextureStageState ( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    SetTextureStageState ( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
    SetTextureStageState ( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
    SetTextureStageState ( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    SetTextureStageState ( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    SetTextureStageState ( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
    SetTextureStageState ( 1, D3DTSS_MINFILTER, D3DTEXF_NONE );
    SetTextureStageState ( 1, D3DTSS_MAGFILTER, D3DTEXF_NONE );
    SetTextureStageState ( 1, D3DTSS_MIPFILTER, D3DTEXF_NONE );
	//g_D3DDevice->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_NONE);
	//g_D3DDevice->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
	//g_D3DDevice->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
    // mode
    CMyBitmap::s_nShowMode	= modeElement;
}

BOOL CElement::Draw ( int nOffsetX, int nOffsetY )
{
    if (CMyBitmap::s_nShowMode != modeElement)
    {
        CElement::Prepare();
    }
    if ( g_lpTex[m_nTex]->Info.Format == D3DFMT_A8R8G8B8 ||
            g_lpTex[m_nTex]->Info.Format == D3DFMT_A1R5G5B5 ||
            g_lpTex[m_nTex]->Info.Format == D3DFMT_A4R4G4B4 ||
            g_lpTex[m_nTex]->Info.Format == D3DFMT_A8 ||
            g_lpTex[m_nTex]->Info.Format == D3DFMT_A8R3G3B2 )
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
    // 当前贴图
    if ( FAILED ( g_D3DDevice->SetTexture ( 0, g_lpTex[m_nTex]->lpTex ) ) )
    {
        return false;
    }
    _D3DXMATRIXA16 matrix;
    D3DXMatrixTranslation ( &matrix, ( float )nOffsetX, 0, ( float )nOffsetY );
    if ( FAILED ( g_D3DDevice->SetTransform ( D3DTS_WORLD,
                  &matrix ) ) )
    {
        return false;
    }
    // vertex shader
 /*   if ( FAILED ( g_D3DDevice->SetFVF ( MAP_VERTEX ) ) )
    {
        return false;
    }*/
	if ( FAILED ( g_D3DDevice->SetVertexShader ( MAP_VERTEX ) ) )
		return false;
    // 绘制场景
    if ( FAILED ( g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP,
                  2,
                  m_Vec,
                  sizeof ( MAPVERTEX ) ) ) )
    {
        return false;
    }
    return true;
}

void CElement::GetSize (UINT& uWidth, UINT& uHeight)
{
    if (!g_lpTex[m_nTex])
    {
        return;
    }
    uWidth	= g_lpTex[m_nTex]->Info.Width;
    uHeight	= g_lpTex[m_nTex]->Info.Height;
}

CMapBackground::CMapBackground ()
{
    lpElement = 0;
    m_dwXBlock = 0;
    m_dwYBlock = 0;
    m_Coordinate.x = 0;
    m_Coordinate.y = 0;
    m_camera = 0;
}

CMapBackground::~CMapBackground ()
{
    Release ();
}

void CMapBackground::GetSize (UINT& uWidth, UINT& uHeight)
{
    if (!lpElement)
    {
        return;
    }
    UINT uElementWidth, uElementHeight;
    lpElement[0].GetSize(uElementWidth, uElementHeight);
    uWidth	= uElementWidth * m_dwXBlock;
    uHeight	= uElementHeight * m_dwYBlock;
}

BOOL CMapBackground::Create ( const char* pszDir, DWORD dwXBlock, DWORD dwYBlock )
{
    lpElement = new CElement[dwXBlock * dwYBlock];
    char name[256];
    for ( DWORD n = 0; n < dwXBlock * dwYBlock; n++ )
    {
        sprintf ( name, "%s/%02d.dds", pszDir, n );
        if ( !lpElement[n].Create ( name ) )
        {
            return false;
        }
    }
    m_dwXBlock = dwXBlock;
    m_dwYBlock = dwYBlock;
    m_camera = new C3Camera;
    Camera_Clear ( m_camera );
    m_camera->lpFrom = new D3DXVECTOR3[1];
    m_camera->lpFrom[0].x =  (float)(_SCR_WIDTH / 2);
    m_camera->lpFrom[0].y = -1000;
    m_camera->lpFrom[0].z =  (float)(_SCR_HEIGHT / 2);
    m_camera->lpTo = new D3DXVECTOR3[1];
    m_camera->lpTo[0].x =  (float)(_SCR_WIDTH / 2);
    m_camera->lpTo[0].y = 0;
    m_camera->lpTo[0].z =  (float)(_SCR_HEIGHT / 2);
    m_camera->fNear = 1.0f;
    m_camera->fFar = 10000.0f;
    return true;
}
void CMapBackground::Release ( void )
{
    for ( DWORD n = 0; n < m_dwXBlock * m_dwYBlock; n++ )
    {
        lpElement[n].Release ();
    }
    if ( m_camera )
    {
        SafeDeleteEx ( m_camera->lpFrom );
        SafeDeleteEx ( m_camera->lpTo );
        SafeDelete ( m_camera );
    }
}
void CMapBackground::SetViewport ( int nX, int nY )
{
    m_Coordinate.x = nX;
    m_Coordinate.y = nY;
    m_camera->lpFrom[0].x = _SCR_WIDTH / 2 + ( float )nX;
    m_camera->lpFrom[0].z = _SCR_HEIGHT / 2 + ( float )nY;
    m_camera->lpTo[0].x = _SCR_WIDTH / 2 + ( float )nX;
    m_camera->lpTo[0].z = _SCR_HEIGHT / 2 + ( float )nY;
}

void CMapBackground::Prepare ( void )
{
    CElement::Prepare ();
}

void CMapBackground::Draw ( void )
{
    Camera_BuildView ( m_camera, true );
    Camera_BuildOrtho ( m_camera, (float)_SCR_WIDTH, (float)_SCR_HEIGHT, true );
    //Camera_BuildProject ( m_camera, true );
    //Camera_BuildMix ( m_camera );
    // 画地图
    // 得到左上的所在格子  g_dwXTitles * g_dwYTitles
    int sbx = m_Coordinate.x / 256;
    int sby = m_Coordinate.y / 256;
    int ebx = ( m_Coordinate.x + _SCR_WIDTH ) / 256;
    int eby = ( m_Coordinate.y + _SCR_HEIGHT ) / 256;
    RECT mode = { 0, 0, _SCR_WIDTH, _SCR_HEIGHT };
    RECT bb = { 0, 0, 255, 255 };
    for ( int y = sby; y <= eby; y++ )
    {
        for ( int x = sbx; x <= ebx; x++ )
        {
            if ( y < 0 || x < 0 || y >= (int)m_dwYBlock || x >= (int)m_dwXBlock )
            {
                continue;
            }
            lpElement[y * m_dwXBlock + x].Draw ( x * 256, y * 256 );
        }
    }
}

/*CMapBackground::CMapBackground ()
{
	m_lpElement = 0;
	m_dwXBlock = 0;
	m_dwYBlock = 0;

	m_Coordinate.x = 0;
	m_Coordinate.y = 0;

	m_camera = 0;
}
CMapBackground::~CMapBackground ()
{
	Release ();
}

BOOL CMapBackground::Create ( DWORD dwXBlock, DWORD dwYBlock )
{
	m_lpElement = new CMyBitmap[dwXBlock * dwYBlock];

	char name[256];
	for ( DWORD n = 0; n < dwXBlock * dwYBlock; n++ )
	{
		sprintf ( name, "%d.dds", n + 1 );
		if ( !m_lpElement[n].Load ( name ) )
			return false;
	}

	m_dwXBlock = dwXBlock;
	m_dwYBlock = dwYBlock;

	m_camera = new C3Camera;
	Camera_Clear ( m_camera );

	m_camera->lpFrom = new D3DXVECTOR3[1];
	m_camera->lpFrom[0].x = 400;
	m_camera->lpFrom[0].y = -1000;
	m_camera->lpFrom[0].z = 300;

	m_camera->lpTo = new D3DXVECTOR3[1];
	m_camera->lpTo[0].x = 400;
	m_camera->lpTo[0].y = 0;
	m_camera->lpTo[0].z = 300;

	m_camera->fNear = 1.0f;
	m_camera->fFar = 10000.0f;

	return true;
}
void CMapBackground::Release ( void )
{
	for ( DWORD n = 0; n < m_dwXBlock * m_dwYBlock; n++ )
		m_lpElement[n].Destroy ();

	if ( m_camera )
	{
		SafeDeleteEx ( m_camera->lpFrom );
		SafeDeleteEx ( m_camera->lpTo );
		SafeDelete ( m_camera );
	}
}
void CMapBackground::SetPos ( int nX, int nY )
{
	m_Coordinate.x = nX;
	m_Coordinate.y = nY;

	m_camera->lpFrom[0].x = 400 + ( float )nX;
	m_camera->lpFrom[0].z = 300 + ( float )nY;

	m_camera->lpTo[0].x = 400 + ( float )nX;
	m_camera->lpTo[0].z = 300 + ( float )nY;
}
void CMapBackground::Prepare ( void )
{
	SetRenderState ( D3DRS_ZENABLE, false );
	SetRenderState ( D3DRS_ZWRITEENABLE, true );
	SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_FLAT );
	SetRenderState ( D3DRS_DITHERENABLE, false );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
	SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

	SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

	SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	SetTextureStageState ( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	SetTextureStageState ( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	SetTextureStageState ( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );

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
void CMapBackground::Draw ( void )
{
	Camera_BuildView ( m_camera, true );
	Camera_BuildOrtho ( m_camera, 800, 600, true );
	Camera_BuildMix ( m_camera );

	// 画地图
	CMyBitmap::PrepareSprite ();

	// 得到左上的所在格子  g_dwXTitles * g_dwYTitles
	int sbx = m_Coordinate.x / 256;
	int sby = m_Coordinate.y / 256;

	int ebx = ( m_Coordinate.x + 800 ) / 256;
	int eby = ( m_Coordinate.y + 600 ) / 256;
	RECT mode = { 0, 0, 800, 600 };
	RECT bb = { 0, 0, 255, 255 };

	for ( int y = sby; y <= eby; y++ )
	{
		for ( int x = sbx; x <= ebx; x++ )
		{
			if ( y < 0 || x < 0 || y >= m_dwYBlock || x >= m_dwXBlock )
				continue;
			m_lpElement[y * m_dwXBlock + x].Show ( x * 256 - m_Coordinate.x, y * 256 - m_Coordinate.y );
		}
	}
}
*/
