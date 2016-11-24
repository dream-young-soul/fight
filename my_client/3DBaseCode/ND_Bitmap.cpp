
//**********************************************************
// 代码编辑器
//**********************************************************

#include	<windows.h>
#include	"nd_bitmap.h"
#include	"c3_capscreen.h"
#include	"basefunc.h"
#include	"GameDataSet.h"
#include	 <direct.h>

// globle
int	_SCR_WIDTH	= 1024;
int	_SCR_HEIGHT	= 768;
int _FRAME_SIZE = 21;	//边框宽度,就是创建窗口的任务栏宽度2016.10.14
bool g_FuckWindows = false;
const int _EMOTION_SIZE = 22;

// static
C3Font*	CMyBitmap::s_GameFont	= NULL;
deque	<C3Font*>* CMyBitmap::s_pOtherFontSet = NULL;
C3Camera*   CMyBitmap::s_pCamera = NULL;
DWORD CMyBitmap::s_dwFontSize	= 12;
int CMyBitmap::s_nShowMode		= modeNone;
float CMyBitmap::s_fZoom = 1.0;

#ifdef _ANALYSIS_ON
DWORD g_dwBitmapLoadAmount = 0;
DWORD g_dwBitmapLoadTime = 0;
DWORD g_dwBitmapLoadTimeMax = 0;
DWORD g_dwBitmapLoadTimeFrame = 0;

DWORD g_dw3DObjLoadAmount = 0;
DWORD g_dw3DObjLoadTime = 0;
DWORD g_dw3DObjLoadTimeMax = 0;
DWORD g_dw3DObjLoadTimeFrame = 0;

DWORD g_dw3DMotionLoadAmount = 0;
DWORD g_dw3DMotionLoadTime = 0;
DWORD g_dw3DMotionLoadTimeMax = 0;
DWORD g_dw3DMotionLoadTimeFrame = 0;

DWORD g_dw3DTextureLoadAmount = 0;
DWORD g_dw3DTextureLoadTime = 0;
DWORD g_dw3DTextureLoadTimeMax = 0;
DWORD g_dw3DTextureLoadTimeFrame = 0;

DWORD g_dwFrameInterval = 0;
DWORD g_dwBigTexture = 0;
#endif
//------------------------------------------------------
CMyBitmap::CMyBitmap ( void )
{
    m_sprite = 0;
    m_pMask = NULL;
}

CMyBitmap::~CMyBitmap ( void )
{
    Destroy ();
}

//------------------------------------------------------
void CMyBitmap::Destroy ( void )
{
    Sprite_Unload ( &m_sprite );
    SAFE_DELETE(m_pMask);
}

//------------------------------------------------------
BOOL CMyBitmap::Load(char* pszBmpFile, DWORD dwLoadWay)
{
#ifdef _ANALYSIS_ON
    g_dwBitmapLoadAmount++;
    DWORD dwTimeAnalysis = ::TimeGet();
#endif
    if ( !pszBmpFile )
    {
        return false;
    }
    SAFE_DELETE(m_pMask);
    char szBmpFile[256];
    strcpy(szBmpFile, pszBmpFile);
    _strupr(szBmpFile);
    if (strstr(szBmpFile, ".MSK"))
    {
        m_pMask = new CAlphaMask;
        if (!m_pMask)
        {
            return false;
        }
        char szPicFileName[256];
        strcpy(szPicFileName, pszBmpFile);
        int nLength = strlen(pszBmpFile);
        szPicFileName[nLength-3] = 'D';
        szPicFileName[nLength-2] = 'D';
        szPicFileName[nLength-1] = 'S';
        if (!Sprite_Load ( &m_sprite, szPicFileName ))
        {
            int nLength = strlen(pszBmpFile);
            szPicFileName[nLength-3] = 'T';
            szPicFileName[nLength-2] = 'G';
            szPicFileName[nLength-1] = 'A';
            if (!Sprite_Load ( &m_sprite, szPicFileName ))
            {
                return false;
            }
        }
        CMySize sizeInfo;
        this->GetSize(sizeInfo.iWidth, sizeInfo.iHeight);
        if (!m_pMask->Load(sizeInfo, pszBmpFile))
        {
            this->Destroy();
            return false;
        }
#ifdef _ANALYSIS_ON
        if (m_sprite->lpTex->Info.Width > 256 || m_sprite->lpTex->Info.Height > 256)
        {
            g_dwBigTexture ++;
        }
#endif
        return true;
    }
    BOOL bReturn = Sprite_Load( &m_sprite, pszBmpFile , D3DPOOL_MANAGED , 0, dwLoadWay);
#ifdef _ANALYSIS_ON
    if (m_sprite->lpTex->Info.Width > 256 || m_sprite->lpTex->Info.Height > 256)
    {
        g_dwBigTexture ++;
    }
    DWORD dwTimePass = ::TimeGet() - dwTimeAnalysis;
    g_dwBitmapLoadTime += dwTimePass;
    g_dwBitmapLoadTimeFrame += dwTimePass;
    if (g_dwBitmapLoadTimeMax < dwTimePass)
    {
        g_dwBitmapLoadTimeMax = dwTimePass;
    }
#endif
    return bReturn;
}

//------------------------------------------------------
void CMyBitmap::Show( int x, int y, int alpha, DWORD dwShowWay )
{
    if (s_fZoom == 1.0)
    {
        if (CMyBitmap::s_nShowMode != modeSprite)
        {
            CMyBitmap::PrepareSprite();
        }
        if (alpha != 0)
        {
            Sprite_SetColor ( m_sprite, alpha, 255, 255, 255 );
        }
        Sprite_SetCoor ( m_sprite, 0, x, y );
        Sprite_Draw ( m_sprite, dwShowWay );
    }
    else
    {
        CMySize sizeBitmap = {0, 0};
        this->GetSize(sizeBitmap.iWidth, sizeBitmap.iHeight);
        sizeBitmap.iHeight *= (int)s_fZoom;
        sizeBitmap.iWidth *= (int)s_fZoom;
        this->ShowEx(x, y, NULL, sizeBitmap.iWidth, sizeBitmap.iHeight, dwShowWay);
    }
}

//------------------------------------------------------
void CMyBitmap::ShowEx ( int x, int y, RECT* lpSrc, DWORD dwWidth, DWORD dwHeight, int alpha, DWORD dwShowWay )
{
    int nWidth = 0, nHeight = 0;
    if (!lpSrc && (nWidth == dwWidth) && (nHeight == dwHeight) && (s_fZoom == 1.0))
    {
        if (CMyBitmap::s_nShowMode != modeSprite)
        {
            CMyBitmap::PrepareSprite();
        }
        if (alpha != 0)
        {
            Sprite_SetColor ( m_sprite, alpha, 255, 255, 255 );
        }
        Sprite_SetCoor ( m_sprite, 0, x, y );
        Sprite_Draw ( m_sprite, dwShowWay );
        return;
    }
    if (CMyBitmap::s_nShowMode != modeSprite)
    {
        CMyBitmap::PrepareSprite();
    }
    if (alpha != 0)
    {
        Sprite_SetColor ( m_sprite, alpha, 255, 255, 255 );
    }
    Sprite_SetCoor ( m_sprite, lpSrc, x, y, dwWidth, dwHeight );
    Sprite_Draw ( m_sprite, dwShowWay );
}
//------------------------------------------------------
void CMyBitmap::Show(CMyBitmap* pBmpUp, CMyBitmap* pBmpDn,
                     UCHAR uAlphaA, UCHAR uAlphaB, UCHAR uAlphaC, UCHAR uAlphaD,
                     int x, int y, RECT* lpSrc, DWORD dwWidth, DWORD dwHeight)
{
    CMyBitmap::s_nShowMode = modeNone;
    int nWidth = 0, nHeight = 0;
    if (!pBmpUp || !pBmpDn || !pBmpUp->m_sprite || !pBmpDn->m_sprite)
    {
        return;
    }
    if (!lpSrc && (nWidth == dwWidth) && (nHeight == dwHeight) && (s_fZoom == 1.0))
    {
        Sprite_SetCoor ( pBmpUp->m_sprite, 0, x, y );
        Sprite_Draw ( pBmpUp->m_sprite, pBmpDn->m_sprite, uAlphaA, uAlphaB, uAlphaC, uAlphaD);
        return;
    }
    Sprite_SetCoor( pBmpUp->m_sprite, lpSrc, x, y, dwWidth, dwHeight );
    Sprite_Draw( pBmpUp->m_sprite, pBmpDn->m_sprite, uAlphaA, uAlphaB, uAlphaC, uAlphaD); //wh
}
//------------------------------------------------------
void CMyBitmap::GetSize ( int& nWidth, int& nHeight)
{
    nWidth = m_sprite->lpTex->Info.Width;
    nHeight = m_sprite->lpTex->Info.Height;
}

//------------------------------------------------------
DWORD CMyBitmap::GetSize ( void )
{
    DWORD dwWidth = m_sprite->lpTex->Info.Width;
    DWORD dwHeight = m_sprite->lpTex->Info.Height;
    return dwWidth * dwHeight;
}

//------------------------------------------------------
int	CMyBitmap::GetWidth ( void )
{
    return m_sprite->lpTex->Info.Width;;
}

//------------------------------------------------------
int	CMyBitmap::GetHeight ( void )
{
    return m_sprite->lpTex->Info.Height;
}

//------------------------------------------------------
void CMyBitmap::SetColor ( BYTE a, BYTE r, BYTE g, BYTE b )
{
    Sprite_SetColor ( m_sprite, a, r, g, b );
}
void CMyBitmap::SetVertexColor ( DWORD ltColor, DWORD rtColor, DWORD lbColor, DWORD rbColor )
{
    Sprite_SetVertexColor ( m_sprite, ltColor, rtColor, lbColor, rbColor ); //wh
}
//------------------------------------------------------
BOOL CMyBitmap::IsbTransparentFormat()
{
    if (!m_sprite->lpTex)
    {
        return false;
    }
    if ((m_sprite->lpTex->Info.Format == D3DFMT_A8R8G8B8) ||
            (m_sprite->lpTex->Info.Format == D3DFMT_DXT3))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------
BOOL CMyBitmap::GameCameraCreate()
{
    if (s_pCamera)
    {
        CMyBitmap::GameCameraDestroy();
    }
    s_pCamera = new C3Camera;
    if (!s_pCamera)
    {
        return false;
    }
    Camera_Clear ( s_pCamera );
    s_pCamera->lpFrom = new D3DXVECTOR3[1];
    s_pCamera->lpFrom[0].x = (float)(_SCR_WIDTH / 2);
    s_pCamera->lpFrom[0].y = -1000;
    s_pCamera->lpFrom[0].z = (float)(_SCR_HEIGHT / 2);
    s_pCamera->lpTo = new D3DXVECTOR3[1];
    s_pCamera->lpTo[0].x = (float)(_SCR_WIDTH / 2);
    s_pCamera->lpTo[0].y = 0;
    s_pCamera->lpTo[0].z = (float)(_SCR_HEIGHT / 2);
    s_pCamera->fNear = 1.0f;
    s_pCamera->fFar = 10000.0f;
    return true;
}
//------------------------------------------------------
void CMyBitmap::GameCameraDestroy()
{
    if ( s_pCamera )
    {
        SafeDeleteEx ( s_pCamera->lpFrom );
        SafeDeleteEx ( s_pCamera->lpTo );
        SafeDelete   ( s_pCamera );
        s_pCamera  = NULL;
    }
}
//------------------------------------------------------
void CMyBitmap::GameCameraBuild(CMySize sizeScr)
{
    if (s_pCamera)
    {
        Camera_BuildView ( s_pCamera, true );
        Camera_BuildOrtho ( s_pCamera, (float)sizeScr.iWidth,(float) sizeScr.iHeight, true );
        //		Camera_BuildMix ( s_pCamera );
    }
}
//------------------------------------------------------
void CMyBitmap::GameCameraSet(float fX, float fZ)
{
    if (s_pCamera)
    {
        s_pCamera->lpFrom[0].x = fX;
        s_pCamera->lpFrom[0].z = fZ;
        s_pCamera->lpTo[0].x = fX;
        s_pCamera->lpTo[0].z = fZ;
    }
}

//------------------------------------------------------
BOOL CMyBitmap::GameFontCreate()
{
    s_GameFont		= CMyBitmap::CreateFont(g_objGameDataSet.GetStr(10049), 12);
    s_dwFontSize	= 12;
    return (s_GameFont != NULL);
}

//------------------------------------------------------
void CMyBitmap::GameFontDestroy()
{
    CMyBitmap::DestroyFont(s_GameFont);
    s_GameFont = NULL;
    if (s_pOtherFontSet)
    {
        int nAmount = s_pOtherFontSet->size();
        for(int i = 0; i < nAmount; i ++)
        {
            C3Font* pfont = (*s_pOtherFontSet)[i];
            if (pfont)
            {
                Font_Release(&pfont);
            }
        }
        s_pOtherFontSet->clear();
    }
    SAFE_DELETE(s_pOtherFontSet);
}

//------------------------------------------------------
C3Font* CMyBitmap::CreateFont ( const char* pszFontName, int nSize )
{
    C3Font* font = 0;
    Font_Create ( &font, ( char* )pszFontName, nSize );
    return font;
}

//------------------------------------------------------
void CMyBitmap::DestroyFont ( C3Font* font )
{
    if (font)
    {
        Font_Release(&font);
    }
}

//------------------------------------------------------
void CMyBitmap::GetFontSize(CMySize& infoSize)
{
    infoSize.iHeight = s_dwFontSize;
    infoSize.iWidth  = s_dwFontSize / 2;
}

//------------------------------------------------------
void CMyBitmap::ShowTString ( int iPosX, int iPosY, DWORD color, const char* pszString, char* pszFont, int nFontSize, BOOL bAlpha)
{
    if (!pszString)
    {
        return;
    }
    ::RepairString((char*)pszString);
    if (strlen(pszString) == 0)
    {
        return;
    }
    if (CMyBitmap::s_nShowMode != modeText)
    {
        Font_Prepare();
        CMyBitmap::s_nShowMode	= modeText;
    }
    C3Font* pMyFont = NULL;
    if (!pszFont)
    {
        pMyFont	= CMyBitmap::s_GameFont;
    }
    else
    {
        if (!s_pOtherFontSet)
        {
            s_pOtherFontSet = new (deque	<C3Font*>);
            MYASSERT(s_pOtherFontSet);
        }
        // search ...
        BOOL bFound = false;
        int nAmount = s_pOtherFontSet->size();
        for(int i = 0; i < nAmount; i ++)
        {
            C3Font* pFont = (*s_pOtherFontSet)[i];
            int nSize = nFontSize;
            if (nSize <= 16)
            {
                nSize =  16;
            }
            else if (nSize <= 32)
            {
                nSize = 32;
            }
            else
            {
                nSize = 64;
            }
            if (pFont && strcmp(pFont->szName, pszFont) == 0 && pFont->nSize == nSize)
            {
                // found ...
                bFound = true;
                pMyFont = pFont;
                break;
            }
        }
        // create ...
        if (!bFound)
        {
            pMyFont = CMyBitmap::CreateFont(pszFont, nFontSize);
            if (pMyFont)
            {
                s_pOtherFontSet->push_back(pMyFont);
            }
            else
            {
                return;
            }
        }
        pMyFont->nRealSize = nFontSize;
    }
    if (bAlpha == false)
    {
        color |= 0xff000000;
    }
    int step = iPosY;
    int temp = 0;
    char ss[2];
    for ( int n = 0; n < (int)strlen ( pszString ); n++ )
    {
        ss[0] = pszString[n];
        ss[1] = pszString[n + 1];
        Font_Draw ( pMyFont, (float)iPosX, (float)step, color, ss );
        //		if ( ( BYTE )ss[0] > 0x80  && ( BYTE )ss[0] < 0xfe)
        step += pMyFont->nRealSize;
        if ( ( BYTE )pszString[n] > 0x80 && ( BYTE )ss[0] < 0xfe)
        {
            n++;
        }
        temp++;
    }
}
//------------------------------------------------------
void CMyBitmap::ShowString ( int iPosX, int iPosY, DWORD color, const char* pszString, char* pszFont, int nFontSize, BOOL bAlpha)
{
    if (!pszString)
    {
        return;
    }
    ::RepairString((char*)pszString);
    if (strlen(pszString) == 0)
    {
        return;
    }
    if (CMyBitmap::s_nShowMode != modeText)
    {
        Font_Prepare();
        CMyBitmap::s_nShowMode	= modeText;
    }
    C3Font* pMyFont = NULL;
    if (!pszFont)
    {
        pMyFont	= CMyBitmap::s_GameFont;
    }
    else
    {
        if (!s_pOtherFontSet)
        {
            s_pOtherFontSet = new (deque	<C3Font*>);
            MYASSERT(s_pOtherFontSet);
        }
        // search ...
        BOOL bFound = false;
        int nAmount = s_pOtherFontSet->size();
        for(int i = 0; i < nAmount; i ++)
        {
            C3Font* pFont = (*s_pOtherFontSet)[i];
            int nSize = nFontSize;
            if (nSize <= 16)
            {
                nSize =  16;
            }
            else if (nSize <= 32)
            {
                nSize = 32;
            }
            else
            {
                nSize = 64;
            }
            if (pFont && strcmp(pFont->szName, pszFont) == 0 && pFont->nSize == nSize)
            {
                // found ...
                bFound = true;
                pMyFont = pFont;
                break;
            }
        }
        // create ...
        if (!bFound)
        {
            pMyFont = CMyBitmap::CreateFont(pszFont, nFontSize);
            if (pMyFont)
            {
                s_pOtherFontSet->push_back(pMyFont);
            }
            else
            {
                return;
            }
        }
        pMyFont->nRealSize = nFontSize;
    }
    if (bAlpha == false)
    {
        color |= 0xff000000;
    }
    int step = iPosX;
    int temp = 0;
    char ss[2];
    for ( int n = 0; n < (int)strlen ( pszString ); n++ )
    {
        ss[0] = pszString[n];
        ss[1] = pszString[n + 1];
        Font_Draw ( pMyFont, (float)step, (float)iPosY, color, ss );
        if ( ( BYTE )ss[0] > 0x80  && ( BYTE )ss[0] < 0xfe)
        {
            step += pMyFont->nRealSize;
        }
        else
        {
            step += ( int )( ( float )pMyFont->nRealSize / 2.0f + 0.5f );
        }
        if ( ( BYTE )pszString[n] > 0x80 && ( BYTE )ss[0] < 0xfe)
        {
            n++;
        }
        temp++;
    }
}
//------------------------------------------------------

void CMyBitmap::ShowEmotionString (int iPosX, int iPosY, DWORD color,  const char* pszString, DWORD dwBeginTime, BOOL bEmotion /* = true */, char* pszFont, int nFontSize)
{
    if (!pszString || strlen( pszString ) <= 0)
    {
        return;
    }
    if ( CMyBitmap::s_nShowMode != modeText )
    {
        Font_Prepare();
        CMyBitmap::s_nShowMode	= modeText;
    }
    C3Font* pMyFont = NULL;
    if (!pszFont)
    {
        pMyFont	= CMyBitmap::s_GameFont;
    }
    else
    {
        if (!s_pOtherFontSet)
        {
            s_pOtherFontSet = new (deque	<C3Font*>);
            MYASSERT(s_pOtherFontSet);
        }
        // search ...
        BOOL bFound = false;
        int nAmount = s_pOtherFontSet->size();
        for(int i = 0; i < nAmount; i ++)
        {
            C3Font* pFont = (*s_pOtherFontSet)[i];
            int nSize = nFontSize;
            if (nSize <= 16)
            {
                nSize =  16;
            }
            else if (nSize <= 32)
            {
                nSize = 32;
            }
            else
            {
                nSize = 64;
            }
            if (pFont && strcmp(pFont->szName, pszFont) == 0 && pFont->nSize == nSize)
            {
                // found ...
                bFound = true;
                pMyFont = pFont;
                break;
            }
        }
        // create ...
        if (!bFound)
        {
            pMyFont = CMyBitmap::CreateFont(pszFont, nFontSize);
            if (pMyFont)
            {
                s_pOtherFontSet->push_back(pMyFont);
            }
            else
            {
                return;
            }
        }
        pMyFont->nRealSize = nFontSize;
    }
    color |= 0xff000000;
    int step = iPosX;
    char ss[2];
    int nStrLen = strlen(pszString);
    for ( int n = 0; n < nStrLen; n++ )
    {
        // Jump line
        if ( pszString[n] == 0x0a )
        {
            step = iPosX;
            iPosY += pMyFont->nRealSize + pMyFont->nRealSize / 4;
            continue;
        }
        // Test the emotion char '#'
        if ( pszString[n] == '#' &&
                n + 2 < nStrLen &&
                pszString[n + 1] >= '0' &&
                pszString[n + 1] <= '9' &&
                pszString[n + 2] >= '0' &&
                pszString[n + 2] <= '9')
        {
            char strIndex[3];
            strIndex[0] = pszString[n + 1];
            strIndex[1] = pszString[n + 2];
            strIndex[2] = '\n';
            int nIndex = atoi( strIndex );
            if ( nIndex >= 0 && nIndex < g_objGameDataSet.GetEmotionAmount() )
            {
                CAni* pShowAni = g_objGameDataSet.GetEmotionIcon(nIndex);
                if ( pShowAni != NULL)
                {
                    int nFrame = (::TimeGet() - dwBeginTime) / 200 % pShowAni->GetFrameAmount();
                    if (bEmotion)
                    {
                        pShowAni->ShowEx( nFrame, step, iPosY - 6, NULL, _EMOTION_SIZE, _EMOTION_SIZE);
                    }
                    n += 2;
                    step += _EMOTION_SIZE;
                    //					step += pMyFont->nRealSize + ( int )( ( float )pMyFont->nRealSize / 2.0f + 0.5f );
                    continue;
                }
            }
        }
        ss[0] = pszString[n];
        ss[1] = pszString[n + 1];
        Font_Draw ( pMyFont, (float)step, (float)iPosY, color, ss );
        if ( ( BYTE )ss[0] > 0x80  && ( BYTE )ss[0] < 0xfe )
        {
            step += pMyFont->nRealSize;
            n ++;
        }
        else
        {
            step += ( int )( ( float )pMyFont->nRealSize / 2.0f + 0.5f );
        }
    }
}

//------------------------------------------------------
void CMyBitmap::PrepareLine ( void )
{
	SetRenderState ( D3DRS_ZENABLE, false );
	SetRenderState ( D3DRS_ZWRITEENABLE, false );
	SetRenderState ( D3DRS_SHADEMODE, D3DSHADE_FLAT );
	SetRenderState ( D3DRS_DITHERENABLE, false );
	SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	SetRenderState ( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
	SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
	
	SetRenderState ( D3DRS_ALPHABLENDENABLE, true );
	SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );

	SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
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



	//g_D3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	//g_D3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	//g_D3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
 
	//g_D3DDevice->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_NONE);
	//g_D3DDevice->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
	//g_D3DDevice->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
    // mode
    CMyBitmap::s_nShowMode	= modeLine;
}

//------------------------------------------------------
void CMyBitmap::ShowLine ( LineVertex* line, DWORD num )
{
	if(g_FuckWindows)return;
    if (CMyBitmap::s_nShowMode != modeLine)
    {
        CMyBitmap::PrepareLine();
    }
	
    //g_D3DDevice->SetFVF ( LINE_VERTEX );

	g_D3DDevice->SetVertexShader ( LINE_VERTEX );

    g_D3DDevice->DrawPrimitiveUP ( D3DPT_LINELIST,
                                   num,
                                   line,
                                   sizeof ( LineVertex ) );
}
//------------------------------------------------------
void CMyBitmap::ShowLine ( int nPosX1, int nPosY1, int nPosX2, int nPosY2, DWORD dwColor)
{
    static 	LineVertex s_Line[2];
    s_Line[0].x = (float)nPosX1;
    s_Line[0].y = (float)nPosY1;
    s_Line[0].z = 0;
    s_Line[0].rhw = 0;
    s_Line[0].color = dwColor | 0xff000000;
    s_Line[1].x = (float)nPosX2;
    s_Line[1].y = (float)nPosY2;
    s_Line[1].z = 0;
    s_Line[1].rhw = 0;
    s_Line[1].color = dwColor | 0xff000000;
    ShowLine  ( s_Line, 1 );	// 在此位图上画线，可指定透明度
}
//------------------------------------------------------
void CMyBitmap::ShowRect ( int x1, int y1, int x2, int y2, DWORD color )
{
    static LineVertex s_line[8];
    s_line[0].x =(float) x1;
    s_line[0].y = (float)y1;
    s_line[0].z = 0;
    s_line[0].rhw = 0;
    s_line[0].color = color;
    s_line[1].x = (float)x1;
    s_line[1].y = (float)y2;
    s_line[1].z = 0;
    s_line[1].rhw = 0;
    s_line[1].color = color;
    s_line[2].x =(float) x1;
    s_line[2].y =(float) y2;
    s_line[2].z = 0;
    s_line[2].rhw = 0;
    s_line[2].color = color;
    s_line[3].x =(float) x2;
    s_line[3].y = (float)y2;
    s_line[3].z = 0;
    s_line[3].rhw = 0;
    s_line[3].color = color;
    s_line[4].x = (float)x2;
    s_line[4].y = (float)y2;
    s_line[4].z = 0;
    s_line[4].rhw = 0;
    s_line[4].color = color;
    s_line[5].x =(float) x2;
    s_line[5].y =(float) y1;
    s_line[5].z = 0;
    s_line[5].rhw = 0;
    s_line[5].color = color;
    s_line[6].x =(float) x2;
    s_line[6].y =(float) y1;
    s_line[6].z = 0;
    s_line[6].rhw = 0;
    s_line[6].color = color;
    s_line[7].x = (float)x1;
    s_line[7].y = (float)y1;
    s_line[7].z = 0;
    s_line[7].rhw = 0;
    s_line[7].color = color;
    ShowLine  ( s_line, 4 );	// 在此位图上画线，可指定透明度
}

//------------------------------------------------------
//移植到directx9 画不出来。。
void CMyBitmap::ShowBlock( int x1, int y1, int x2, int y2, DWORD color1, DWORD color2, DWORD color3, DWORD color4)
{
	if(g_FuckWindows)return;
    static LineVertex s_vec[4];
    s_vec[0].x = (float)x1;
    s_vec[0].y = (float)y1;
    s_vec[0].z = 0;
    s_vec[0].rhw = 1;
    s_vec[0].color = color1;
    s_vec[1].x =(float) x1;
    s_vec[1].y = (float)y2;
    s_vec[1].z = 0;
    s_vec[1].rhw = 1;
    s_vec[1].color = color2;
    s_vec[2].x =(float) x2;
    s_vec[2].y =(float) y1;
    s_vec[2].z = 0;
    s_vec[2].rhw = 1;
    s_vec[2].color = color3;
    s_vec[3].x =(float) x2;
    s_vec[3].y = (float)y2;
    s_vec[3].z = 0;
    s_vec[3].rhw = 1;
    s_vec[3].color = color4;
    if (CMyBitmap::s_nShowMode != modeLine)
    {
        CMyBitmap::PrepareLine();
    }
    //g_D3DDevice->SetFVF ( LINE_VERTEX );
	g_D3DDevice->SetVertexShader ( LINE_VERTEX );

    g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP,
                                   2,
                                   s_vec,
                                   sizeof ( LineVertex ) );
}
//------------------------------------------------------

void CMyBitmap::ShowBlock( CMyPos posA, CMyPos posB, CMyPos posC, CMyPos posD, DWORD color )
{
	if(g_FuckWindows)return;
    static LineVertex s_vec[4];
    s_vec[0].x = (float)posA.x;
    s_vec[0].y =(float) posA.y;
    s_vec[0].z = 0;
    s_vec[0].rhw = 0;
    s_vec[0].color = color;
    s_vec[1].x = (float)posB.x;
    s_vec[1].y = (float)posB.y;
    s_vec[1].z = 0;
    s_vec[1].rhw = 0;
    s_vec[1].color = color;
    s_vec[2].x = (float)posC.x;
    s_vec[2].y = (float)posC.y;
    s_vec[2].z = 0;
    s_vec[2].rhw = 0;
    s_vec[2].color = color;
    s_vec[3].x = (float)posD.x;
    s_vec[3].y = (float)posD.y;
    s_vec[3].z = 0;
    s_vec[3].rhw = 0;
    s_vec[3].color = color;
    if (CMyBitmap::s_nShowMode != modeLine)
    {
        CMyBitmap::PrepareLine();
    }

	//LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = NULL;
	// if(FAILED(g_D3DDevice->CreateVertexBuffer(sizeof(LineVertex), 0,
 //       D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_VertexBuffer,
 //       NULL))) return ;
	// void *ptr;

 //   if(FAILED(g_VertexBuffer->Lock(0, sizeof(LineVertex),
 //       (void**)&ptr, 0))) return ;

 //   memcpy(ptr, objData, sizeof(objData));

 //   g_VertexBuffer->Unlock();

 //   g_D3DDevice->SetFVF ( LINE_VERTEX );
	g_D3DDevice->SetVertexShader ( LINE_VERTEX );
    g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP,
                                   2,
                                   s_vec,
                                   sizeof ( LineVertex ) );

	

	
}

//------------------------------------------------------
void CMyBitmap::ShowBlock ( int x1, int y1, int x2, int y2, DWORD color )
{
	if(g_FuckWindows)return;
    static LineVertex s_vec[4];
    s_vec[0].x = (float)x1;
    s_vec[0].y = (float)y1;
    s_vec[0].z = 0;
    s_vec[0].rhw = 0;
    s_vec[0].color = color;
    s_vec[1].x = (float)x1;
    s_vec[1].y = (float)y2;
    s_vec[1].z = 0;
    s_vec[1].rhw = 0;
    s_vec[1].color = color;
    s_vec[2].x = (float)x2;
    s_vec[2].y = (float)y1;
    s_vec[2].z = 0;
    s_vec[2].rhw = 0;
    s_vec[2].color = color;
    s_vec[3].x = (float)x2;
    s_vec[3].y = (float)y2;
    s_vec[3].z = 0;
    s_vec[3].rhw = 0;
    s_vec[3].color = color;
    if (CMyBitmap::s_nShowMode != modeLine)
    {
        CMyBitmap::PrepareLine();
    }
	
    //g_D3DDevice->SetFVF ( LINE_VERTEX );
	g_D3DDevice->SetVertexShader ( LINE_VERTEX );
    g_D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP,
                                   2,
                                   s_vec,
                                   sizeof ( LineVertex ) );
}

//------------------------------------------------------
BOOL CMyBitmap::Init3D ( HINSTANCE hInst,
                         const char* lpTitle,
                         DWORD dwWidth,
                         DWORD dwHeight,
                         BOOL bWindowed,
                         WNDPROC proc,
                         DWORD dwBackCount )
{
    if ( !::Init3D ( hInst, lpTitle, dwWidth, dwHeight, bWindowed, proc, dwBackCount ) )
    {
        return false;
    }
    CMyBitmap::GameCameraCreate();
    CMySize sizeScr = {_SCR_WIDTH, _SCR_HEIGHT};
    CMyBitmap::GameCameraBuild(sizeScr);
    //Phy_Init ( "phy2.vsh" );
    return true;
}

//------------------------------------------------------
BOOL CMyBitmap::Init3DEx ( HWND hWnd,
                           DWORD dwWidth,
                           DWORD dwHeight,
                           BOOL bWindowed,
                           DWORD dwBackCount )
{
    BOOL bSucInit = false;
    int rval = ::Init3DEx ( hWnd, dwWidth, dwHeight, bWindowed, dwBackCount );
    switch(rval)
    {
    case 0:
        {
            const OBJID idDxError0 = 500016;
            ::ErrorMsg(g_objGameDataSet.GetStr(idDxError0));
            break;
        }
    case -1:
        {
            const OBJID idDxError1 = 500017;
            ::ErrorMsg(g_objGameDataSet.GetStr(idDxError1));
            break;
        }
    case -2:
        {
            const OBJID idDxError2 = 500018;
            ::ErrorMsg(g_objGameDataSet.GetStr(idDxError2));
            break;
        }
    case -3:
        {
            const OBJID idDxError3 = 500019;
            ::ErrorMsg(g_objGameDataSet.GetStr(idDxError3));
            bSucInit = true;
            break;
        }
    case 1:
        {
            bSucInit = true;
            break;
        }
    default:
        {
            const OBJID idDxErrorDef = 500020;
            ::ErrorMsg(g_objGameDataSet.GetStr(idDxErrorDef));
            break;
        }
    }
    CMyBitmap::GameCameraCreate();
    CMySize sizeScr = {_SCR_WIDTH, _SCR_HEIGHT};
    CMyBitmap::GameCameraBuild(sizeScr);
    float fX, fZ;
    fX = (float)_SCR_WIDTH / 2;
    fZ = (float)_SCR_HEIGHT / 2;
    CMyBitmap::GameCameraSet(fX, fZ);
    //Phy_Init ( "phy2.vsh" );
    return bSucInit;
}

//------------------------------------------------------
void CMyBitmap::Quit3D (void)
{
    CMyBitmap::GameCameraDestroy();
    CMyBitmap::GameFontDestroy();
    ::Quit3D();
}

//------------------------------------------------------
BOOL CMyBitmap::Begin3D ( void )
{
    return ::Begin3D ();
}

//------------------------------------------------------
BOOL CMyBitmap::End3D ( void )
{
    return ::End3D ();
}

//------------------------------------------------------
BOOL CMyBitmap::ClearBuffer ( BOOL bZBuffer, BOOL bTarget, D3DCOLOR color )
{
    return ::ClearBuffer ( bZBuffer, bTarget, color );
}

//------------------------------------------------------
BOOL CMyBitmap::Flip ( void )
{
    return ::Flip ();
}

//------------------------------------------------------
void CMyBitmap::PrepareSprite ( void )
{
    Sprite_Prepare ();
    // mode
    CMyBitmap::s_nShowMode	= modeSprite;
}

//------------------------------------------------------
DWORD CMyBitmap::CalcRate ( void )
{
    return ::CalcRate ();
}
//------------------------------------------------------
BOOL CMyBitmap::IsTransparent(CMyPos posPoint)
{
    if (!m_pMask)
    {
        return true;
    }
    else
    {
        return m_pMask->IsTransparent(posPoint);
    }
}

//------------------------------------------------------
void CMyBitmap::ScreenShot(const char* pszFileName)
{
    char szFileName[256] = "";
    if (!pszFileName)
    {
        _mkdir("ScreenShot");
        sprintf(szFileName, "ScreenShot/%u.jpg", ::TimeGet());
    }
    else
    {
        strcpy(szFileName, pszFileName);
    }
    ::CapScreen(szFileName);
}

//------------------------------------------------------

//------------------------------------------------------
//       ----------- C3DTexture -----------
//------------------------------------------------------
C3DTexture::C3DTexture ()
{
    m_pTexture	= NULL;
}

C3DTexture::~C3DTexture ()
{
    this->Destroy ();
}

//------------------------------------------------------
BOOL C3DTexture::Create(const char* pszFileName)
{
    if (!pszFileName)
    {
        return false;
    }
#ifdef _ANALYSIS_ON
    g_dw3DTextureLoadAmount++;
    DWORD dwTimeAnalysis = ::TimeGet();
#endif
    int nReturn = ::Texture_Load(&m_pTexture, (char*)pszFileName, 3);
#ifdef _ANALYSIS_ON
    DWORD dwTimePass = ::TimeGet() - dwTimeAnalysis;
    g_dw3DTextureLoadTime += dwTimePass;
    g_dw3DTextureLoadTimeFrame += dwTimePass;
    if (g_dw3DTextureLoadTimeMax < dwTimePass)
    {
        g_dw3DTextureLoadTimeMax = dwTimePass;
    }
#endif
    return (nReturn != -1);
}

//------------------------------------------------------
void C3DTexture::Destroy(void)
{
    if (m_pTexture)
    {
        ::Texture_Unload(&m_pTexture);
    }
}

//------------------------------------------------------
//       ----------- C3DMotion -----------
//------------------------------------------------------
C3DMotion::C3DMotion ()
{
    for ( int n = 0; n < MOTION_MAX; n++ )
    {
        m_motion[n] = 0;
    }
    m_dwMotionNum = 0;
}

//------------------------------------------------------
C3DMotion::~C3DMotion ()
{
    Destroy ();
}

//------------------------------------------------------
bool C3DMotion::IsValid ( void )
{
    if ( m_motion )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------
bool C3DMotion::Create ( char* filename )
{
#ifdef _ANALYSIS_ON
    g_dw3DMotionLoadAmount++;
    DWORD dwTimeAnalysis = ::TimeGet();
#endif
    m_dwMotionNum = 0;
    int nSize ;
    Common_BeforeUseDnp();
    FILE* fp = Common_OpenDnp ( filename, nSize );
    if ( fp )
    {
        ChunkHeader chunk;
        int nOffset = 16 ;
        while ( nOffset < nSize )
        {
            Common_GetChunk ( fp, &chunk );
            nOffset += sizeof(ChunkHeader) ;
            if ( chunk.byChunkID[0] == 'M' &&
                    chunk.byChunkID[1] == 'O' &&
                    chunk.byChunkID[2] == 'T' &&
                    chunk.byChunkID[3] == 'I' )
            {
                if ( !Motion_Load ( &m_motion[m_dwMotionNum], fp ) )
                {
                    break;
                }
                m_dwMotionNum++;
            }
            else
            {
                fp = Common_MoveDnpFPtr(fp, chunk.dwChunkSize);
            }
            nOffset += chunk.dwChunkSize ;
        }
    }
    Common_AfterUseDnp();
#ifdef _ANALYSIS_ON
    DWORD dwTimePass = ::TimeGet() - dwTimeAnalysis;
    g_dw3DMotionLoadTime += dwTimePass;
    g_dw3DMotionLoadTimeFrame += dwTimePass;
    if (g_dw3DMotionLoadTimeMax < dwTimePass)
    {
        g_dw3DMotionLoadTimeMax = dwTimePass;
    }
#endif
    return (m_dwMotionNum > 0);
}

//------------------------------------------------------
bool C3DMotion::Destroy ( void )
{
    for ( DWORD n = 0; n < m_dwMotionNum; n++ )
    {
        Motion_Unload ( &m_motion[n] );
    }
    m_dwMotionNum	= 0;
    return true;
}

//------------------------------------------------------
DWORD C3DMotion::GetFrameAmount()
{
    DWORD dwMax	= 0;
    for ( DWORD n = 0; n < m_dwMotionNum; n++ )
    {
        dwMax	= __max(dwMax, m_motion[n]->dwFrames);
    }
    return dwMax;
}

//------------------------------------------------------
//       ----------- C3DObj -----------
//------------------------------------------------------
C3DObj::C3DObj ()
{
    for ( int n = 0; n < PHY_MAX; n++ )
    {
        m_phy[n] = 0;
    }
    m_dwPhyNum = 0;
}

//------------------------------------------------------
C3DObj::~C3DObj ()
{
    Destroy ();
}

//------------------------------------------------------
bool C3DObj::IsValid ()
{
    if ( m_phy )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------
bool C3DObj::Create ( char* filename )
{
#ifdef _ANALYSIS_ON
    g_dw3DObjLoadAmount++;
    DWORD dwTimeAnalysis = ::TimeGet();
#endif
    if (!filename)
    {
        return false;
    }
    m_dwPhyNum = 0;
    for ( int n = 0; n < PHY_MAX; n++ )
    {
        m_phy[n] = NULL;
    }
    int nSize ;
    Common_BeforeUseDnp();
    FILE* fp = Common_OpenDnp ( filename, nSize );
    if ( fp )
    {
        ChunkHeader chunk;
        int nOffset = 16 ;
        while ( nOffset < nSize )
        {
            Common_GetChunk ( fp, &chunk );
            nOffset += sizeof (ChunkHeader);
            if ( chunk.byChunkID[0] == 'P' &&
                    chunk.byChunkID[1] == 'H' &&
                    chunk.byChunkID[2] == 'Y' &&
                    chunk.byChunkID[3] == ' ' )
            {
                if ( !Phy_Load ( &m_phy[m_dwPhyNum], fp ) )
                {
                    break;
                }
                m_dwPhyNum++;
            }
            else
            {
                fp = Common_MoveDnpFPtr( fp, chunk.dwChunkSize);
            }
            nOffset += chunk.dwChunkSize ;
        }
    }
    Common_AfterUseDnp();
    /*	else
    	{
    		int nSize ;
    		HANDLE f = Common_OpenResPack ( filename, nSize );
    		if ( f )
    		{
    			ChunkHeader chunk;
    			int nOffset = 16 ;
    			DWORD bytes ;

    			while ( nOffset < nSize )
    			{
    				ReadFile ( f, &chunk, sizeof ( ChunkHeader ), &bytes, 0 ) ;
    				nOffset += bytes ;
    				if ( chunk.byChunkID[0] == 'P' &&
    					 chunk.byChunkID[1] == 'H' &&
    					 chunk.byChunkID[2] == 'Y' &&
    					 chunk.byChunkID[3] == ' ' )
    				{
    					if ( !Phy_LoadPack ( &m_phy[m_dwPhyNum], f ) )
    						break;
    					m_dwPhyNum++;
    				}
    				else
    				{
    					SetFilePointer ( f, chunk.dwChunkSize, 0, FILE_CURRENT ) ;
    				}
    				nOffset += chunk.dwChunkSize ;
    			}
    		}
    	}*/
#ifdef _ANALYSIS_ON
    DWORD dwTimePass = ::TimeGet() - dwTimeAnalysis;
    g_dw3DObjLoadTime += dwTimePass;
    g_dw3DObjLoadTimeFrame += dwTimePass;
    if (g_dw3DObjLoadTimeMax < dwTimePass)
    {
        g_dw3DObjLoadTimeMax = dwTimePass;
    }
#endif
    if (m_dwPhyNum > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------
void C3DObj::Destroy ()
{
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        Phy_Unload ( &m_phy[n] );
    }
}

//------------------------------------------------------
void C3DObj::Move ( float x, float y, float z )
{
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Translate ( &matrix, x, y, z );
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        for ( DWORD b = 0; b < m_phy[n]->lpMotion->dwBoneCount; b++ )
        {
            Phy_Muliply ( m_phy[n], b, &matrix );
        }
    }
}

//------------------------------------------------------
void C3DObj::Rotate	( float x, float y, float z )
{
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Rotate ( &matrix, x, y, z );
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        for ( DWORD b = 0; b < m_phy[n]->lpMotion->dwBoneCount; b++ )
        {
            Phy_Muliply ( m_phy[n], b, &matrix );
        }
    }
}

//------------------------------------------------------
void C3DObj::Scale	( float x, float y, float z )
{
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Scale ( &matrix, x, y, z );
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        for ( DWORD b = 0; b < m_phy[n]->lpMotion->dwBoneCount; b++ )
        {
            Phy_Muliply ( m_phy[n], b, &matrix );
        }
    }
}

//------------------------------------------------------
void C3DObj::SetMotion ( C3DMotion* pMotion )
{
    //if (m_dwPhyNum != pMotion->m_dwMotionNum)
    if (m_dwPhyNum > pMotion->m_dwMotionNum)
    {
        return;
    }
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        m_phy[n]->lpMotion	= pMotion->m_motion[n];
    }
}

//------------------------------------------------------
void C3DObj::SetARGB ( float alpha, float red, float green, float blue )
{
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        Phy_SetColor ( m_phy[n], alpha, red, green, blue );
    }
}

//------------------------------------------------------
int C3DObj::GetIndexByName ( char* lpName )
{
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        if ( _stricmp ( m_phy[n]->lpName, lpName ) == 0 )
        {
            return n;
        }
    }
    return -1;
}

//------------------------------------------------------
void C3DObj::NextFrame ( int nStep )
{
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        Phy_NextFrame ( m_phy[n], nStep );
    }
}

//------------------------------------------------------
void C3DObj::SetFrame (DWORD dwFrame)
{
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        Phy_SetFrame ( m_phy[n], dwFrame );
    }
}

//------------------------------------------------------
void C3DObj::Draw ( int type, float lightx, float lighty, float lightz, float sa, float sr, float sg, float sb )
{
    if (CMyBitmap::s_nShowMode != modeMesh)
    {
        C3DObj::Prepare();
    }
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        if ( _stricmp( m_phy[n]->lpName, _V_ARMET	) == 0 ||
                _stricmp ( m_phy[n]->lpName,	_V_MISC		) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_WEAPON ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_WEAPON ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_SHIELD ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_SHIELD ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_SHOE	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_SHOE	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_PET	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_BACK	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_HEAD	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_ARM) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_ARM) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_LEG) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_LEG) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_MANTLE) == 0)
        {
            continue;
        }
        // 画实体
        Phy_Calculate ( m_phy[n] );
        Phy_DrawNormal ( m_phy[n] );
        /*switch ( type )
        {
        case 2:
        {
        // 画阴影
        _D3DXMATRIXA16 matrix;
        D3DXMatrixIdentity ( &matrix );
        Common_Rotate ( &matrix, D3DXToRadian ( -45 ), 0, 0 );

          D3DXVECTOR4	pos;
          pos.x = lightx;
          pos.y = lighty;
          pos.z = lightz;
          pos.w = 1;
          D3DXVec3TransformCoord ( ( D3DXVECTOR3* )&pos, ( D3DXVECTOR3* )&pos, &matrix );

        	pos.x += m_phy[n]->lpMotion->matrix[0]._41;
        	pos.z += m_phy[n]->lpMotion->matrix[0]._43;

        	  D3DXVECTOR3 point;
        	  point.x = m_phy[n]->lpMotion->matrix[0]._41;
        	  point.y = m_phy[n]->lpMotion->matrix[0]._42;
        	  point.z = m_phy[n]->lpMotion->matrix[0]._43;

        		D3DXVECTOR3 normal;
        		normal.x = 0;
        		normal.y = 0;
        		normal.z = -1;
        		D3DXVec3TransformCoord ( &normal, &normal, &matrix );
        		D3DXVec3Normalize ( &normal, &normal );

        		  int tex = m_phy[n]->nTex;
        		  m_phy[n]->nTex = -1;
        		  Phy_SetColor ( m_phy[n], sa, sr, sg, sb );

        			D3DXMatrixIdentity ( &matrix );
        			Common_Shadow ( &matrix, &pos, &point, &normal );
        			for ( int b = 0; b < m_phy[n]->lpMotion->dwBoneCount; b++ )
        			Phy_Muliply ( m_phy[n], b, &matrix );
        			Phy_Draw ( m_phy[n] );

        			  m_phy[n]->nTex = tex;
        			  }
        			  break;
        	};*/
        //Phy_SetColor ( m_phy[n], 1.0f, 1.0f, 1.0f, 1.0f );
        //Phy_ClearMatrix ( m_phy[n] );
    }
}
void C3DObj::DrawAlpha ( int type, float lightx, float lighty, float lightz, float sa, float sr, float sg, float sb, float height )
{
    if (CMyBitmap::s_nShowMode != modeMesh)
    {
        C3DObj::Prepare();
    }
    for ( DWORD n = 0; n < m_dwPhyNum; n++ )
    {
        if ( _stricmp( m_phy[n]->lpName,	_V_ARMET	) == 0 ||
                _stricmp ( m_phy[n]->lpName,	_V_MISC		) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_WEAPON ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_WEAPON ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_SHIELD ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_SHIELD ) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_SHOE	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_SHOE	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_PET	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_BACK	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_HEAD	) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_ARM) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_ARM) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_L_LEG) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_R_LEG) == 0 ||
                _stricmp ( m_phy[n]->lpName, _V_MANTLE) == 0)
        {
            continue;
        }
        // 画实体
        //Phy_Calculate ( m_phy[n] );
        Phy_DrawAlpha ( m_phy[n], true );
        switch ( type )
        {
        case 2:
            {
                // 画阴影
                _D3DXMATRIXA16 matrix;
                D3DXMatrixIdentity ( &matrix );
                Common_Rotate ( &matrix, D3DXToRadian ( -37 ), 0, 0 );
                D3DXVECTOR4	pos;
                pos.x = lightx;
                pos.y = lighty;
                pos.z = lightz;
                pos.w = 1;
                D3DXVec3TransformCoord ( ( D3DXVECTOR3* )&pos, ( D3DXVECTOR3* )&pos, &matrix );
                pos.x += m_x[n];//m_phy[n]->lpMotion->matrix[0]._41;
                pos.z += m_z[n];//m_phy[n]->lpMotion->matrix[0]._43 - height;
                D3DXVECTOR3 point;
                /*point.x = m_phy[n]->lpMotion->matrix[0]._41;
                point.y = m_phy[n]->lpMotion->matrix[0]._42;
                point.z = m_phy[n]->lpMotion->matrix[0]._43 - height;*/
                point.x = m_x[n];
                point.y = m_y[n];
                point.z = m_z[n] - height;
                D3DXVECTOR3 normal;
                normal.x = 0;
                normal.y = 0;
                normal.z = -1;
                D3DXVec3TransformCoord ( &normal, &normal, &matrix );
                D3DXVec3Normalize ( &normal, &normal );
                //int tex = m_phy[n]->nTex;
                //m_phy[n]->nTex = -1;
                Phy_SetColor ( m_phy[n], sa, sr, sg, sb );
                SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
                D3DXMatrixIdentity ( &matrix );
                Common_Shadow ( &matrix, &pos, &point, &normal );
                for ( DWORD b = 0; b < m_phy[n]->lpMotion->dwBoneCount; b++ )
                {
                    Phy_Muliply ( m_phy[n], b, &matrix );
                }
                Phy_Calculate ( m_phy[n] );
                Phy_DrawNormal ( m_phy[n] );
                Phy_DrawAlpha ( m_phy[n], true );
                SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
                //m_phy[n]->nTex = tex;
            }
            break;
        };
        for ( DWORD n = 0; n < m_dwPhyNum; n++ )
        {
            Phy_SetColor ( m_phy[n], 1.0f, 1.0f, 1.0f, 1.0f );
            //			Phy_ClearMatrix ( m_phy[n] );
        }
    }
}
//------------------------------------------------------
void C3DObj::ChangeTexture ( C3DTexture* pTexture, char* objname )
{
    if (!pTexture)
    {
        return;
    }
    if (!objname)
    {
        for (DWORD i = 0; i < m_dwPhyNum; i++)
        {
            Phy_ChangeTexture (m_phy[i], pTexture->GetTexture()->nID );
        }
    }
    else
    {
        int nIndex = GetIndexByName ( objname );
        if (nIndex < 0)
        {
            nIndex = 0;
        }
        Phy_ChangeTexture (m_phy[nIndex], pTexture->GetTexture()->nID );
    }
}

//------------------------------------------------------
void C3DObj::Prepare ( void )
{
    Phy_Prepare ();
    // mode
    CMyBitmap::s_nShowMode	= modeMesh;
}
//------------------------------------------------------
void C3DObj::ClearMatrix ( void )
{
    for (DWORD n = 0; n < m_dwPhyNum; n++)
    {
        Phy_ClearMatrix (m_phy[n]);
    }
}

bool CMyBitmap::IsDBCSLeadByte	(const char* buf, int nOffset)
{
    if (!buf || nOffset < 0)
    {
        return false;
    }
    int nLen = strlen(buf);
    if (nOffset >= nLen)
    {
        return false;
    }
    BYTE btChar = buf[nOffset];
    if (!::IsDBCSLeadByte(btChar))
    {
        return false;
    }
    // 查看offset-1是不是dbcs的lead byte
    int idxLastDbcs = 0;
    for (int i = 0; i < nOffset; i++)
    {
        if (::IsDBCSLeadByte(buf[i]))
        {
            idxLastDbcs = i;
            i++;
        }
    }
    if (idxLastDbcs < nOffset - 1 || nOffset == 0)
    {
        return ::IsDBCSLeadByte(btChar) ? true : false;
    }
    else
    {
        return false;
    }
}
