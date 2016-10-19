
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DEffect.cpp: implementation of the C3DEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "3DEffect.h"

#ifdef _ANALYSIS_ON
DWORD g_dw3DEffectLoadAmount = 0;
DWORD g_dw3DEffectLoadTime = 0;
DWORD g_dw3DEffectLoadTimeMax = 0;
DWORD g_dw3DEffectLoadTimeFrame = 0;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DEffect::C3DEffect()
{
    m_dwShapes = 0;
    m_dwPtcls = 0;
    for ( int n = 0; n < _EFFECT_PART_MAX_; n++ )
    {
        m_Shape[n] = 0;
        m_SMotion[n] = 0;
        m_Ptcl[n] = 0;
        m_Phy[n] = 0;
        m_Motion[n] = 0;
    }
    m_nShapeAir = FALSE;
    D3DXMatrixIdentity ( &matrix );
}

C3DEffect::~C3DEffect()
{
    this->Unload();
}

//-------------------------------------------------------------------------
BOOL C3DEffect::Load ( char* lpFilename )
{
#ifdef _ANALYSIS_ON
    g_dw3DEffectLoadAmount++;
    DWORD dwTimeAnalysis = ::TimeGet();
#endif
    m_dwShapes = 0;
    m_dwPtcls = 0;
    m_dwPhys = 0;
    DWORD ms = 0;
    DWORD sms = 0;
    int nSize ;
    Common_BeforeUseDnp();
    FILE* fp = Common_OpenDnp ( lpFilename, nSize);
    if ( fp )
    {
        ChunkHeader chunk;
        int nOffset = 16 ;
        while (  nOffset < nSize  )
        {
            Common_GetChunk ( fp, &chunk );
            nOffset += sizeof(ChunkHeader);
            if ( chunk.byChunkID[0] == 'P' &&
                    chunk.byChunkID[1] == 'H' &&
                    chunk.byChunkID[2] == 'Y' &&
                    chunk.byChunkID[3] == ' ' )
            {
                if ( m_Phy[m_dwPhys] )
                {
                    Phy_Unload ( &m_Phy[m_dwPhys] );
                }
                if ( !Phy_Load ( &m_Phy[m_dwPhys], fp ) )
                {
                    break;
                }
                m_dwPhys++;
            }
            else if ( chunk.byChunkID[0] == 'S' &&
                      chunk.byChunkID[1] == 'H' &&
                      chunk.byChunkID[2] == 'A' &&
                      chunk.byChunkID[3] == 'P' )
            {
                if ( m_Shape[m_dwShapes] )
                {
                    Shape_Unload ( &m_Shape[m_dwShapes] );
                }
                if ( !Shape_Load ( &m_Shape[m_dwShapes], fp ) )
                {
                    break;
                }
                m_dwShapes++;
            }
            else if ( chunk.byChunkID[0] == 'P' &&
                      chunk.byChunkID[1] == 'T' &&
                      chunk.byChunkID[2] == 'C' &&
                      chunk.byChunkID[3] == 'L' )
            {
                if ( m_Ptcl[m_dwPtcls] )
                {
                    Ptcl_Unload ( &m_Ptcl[m_dwPtcls] );
                }
                if ( !Ptcl_Load ( &m_Ptcl[m_dwPtcls], fp ) )
                {
                    break;
                }
                m_dwPtcls++;
            }
            else if ( chunk.byChunkID[0] == 'M' &&
                      chunk.byChunkID[1] == 'O' &&
                      chunk.byChunkID[2] == 'T' &&
                      chunk.byChunkID[3] == 'I' )
            {
                if ( m_Motion[ms] )
                {
                    Motion_Unload ( &m_Motion[ms] );
                }
                if ( !Motion_Load ( &m_Motion[ms], fp ) )
                {
                    break;
                }
                ms++;
            }
            else if ( chunk.byChunkID[0] == 'S' &&
                      chunk.byChunkID[1] == 'M' &&
                      chunk.byChunkID[2] == 'O' &&
                      chunk.byChunkID[3] == 'T' )
            {
                if ( m_SMotion[sms] )
                {
                    SMotion_Unload ( &m_SMotion[sms] );
                }
                if ( !SMotion_Load ( &m_SMotion[sms], fp ) )
                {
                    break;
                }
                sms++;
            }
            else
            {
                fp = Common_MoveDnpFPtr(fp, chunk.dwChunkSize);
            }
            nOffset += chunk.dwChunkSize ;
        }
    }
    Common_AfterUseDnp();
    for ( DWORD n = 0; n < m_dwPhys; n++ )
    {
        m_Phy[n]->lpMotion = m_Motion[n];
    }
    for ( DWORD n = 0; n < m_dwShapes; n++ )
    {
        m_Shape[n]->lpMotion = m_SMotion[n];
    }
#ifdef _ANALYSIS_ON
    DWORD dwTimePass = ::TimeGet() - dwTimeAnalysis;
    g_dw3DEffectLoadTime += dwTimePass;
    g_dw3DEffectLoadTimeFrame += dwTimePass;
    if (g_dw3DEffectLoadTimeMax < dwTimePass)
    {
        g_dw3DEffectLoadTimeMax = dwTimePass;
    }
#endif
    return true;
}
//-------------------------------------------------------------------------
void C3DEffect::Unload ( void )
{
    m_dwShapes = 0;
    m_dwPtcls = 0;
    m_dwPhys = 0;
    for ( int n = 0; n < _EFFECT_PART_MAX_; n++ )
    {
        if ( m_Shape[n] )
        {
            Shape_Unload ( &m_Shape[n] );
        }
        if ( m_SMotion[n] )
        {
            SMotion_Unload ( &m_SMotion[n] );
        }
        if ( m_Ptcl[n] )
        {
            Ptcl_Unload ( &m_Ptcl[n] );
        }
        if ( m_Phy[n] )
        {
            Phy_Unload ( &m_Phy[n] );
        }
        if ( m_Motion[n] )
        {
            Motion_Unload ( &m_Motion[n] );
        }
    }
}
//-------------------------------------------------------------------------
void C3DEffect::Bind ( _D3DXMATRIXA16* matrix)
{
    if (!matrix)
    {
        return;
    }
    for ( DWORD n = 0; n < m_dwShapes; n++ )
    {
        CopyMemory ( &m_Shape[n]->lpMotion->matrix,
                     &matrix,
                     sizeof ( _D3DXMATRIXA16 ) );
        m_Shape[n]->lpMotion->nFrame = 0;
    }
    for ( DWORD n = 0; n < m_dwPtcls; n++ )
    {
        CopyMemory ( &m_Ptcl[n]->matrix,
                     &matrix,
                     sizeof ( _D3DXMATRIXA16 ) );
    }
    for ( DWORD n = 0; n < m_dwPhys; n++ )
    {
        CopyMemory ( &m_Phy[n]->lpMotion->matrix[0],
                     &matrix,
                     sizeof ( _D3DXMATRIXA16 ) );
    }
}
//-------------------------------------------------------------------------
void C3DEffect::Bind(C3DObj* lpObj, char* pszPartName)
{
    if (!lpObj)
    {
        return;
    }
    if (!pszPartName || strlen(pszPartName) == 0)
    {
        this->Bind(lpObj);
    }
    for(DWORD j = 0; j < lpObj->m_dwPhyNum; j ++)
    {
        if (strcmp(pszPartName, lpObj->m_phy[j]->lpName) != 0)
        {
            continue;
        }
        for ( DWORD n = 0; n < m_dwShapes; n++ )
        {
            _D3DXMATRIXA16 mm;
            Motion_GetMatrix ( lpObj->m_phy[j]->lpMotion,
                               0,
                               &mm );
            D3DXMatrixMultiply ( &mm,
                                 &mm,
                                 &lpObj->m_phy[j]->lpMotion->matrix[0] );
            CopyMemory ( &m_Shape[n]->lpMotion->matrix,
                         &mm,
                         sizeof ( _D3DXMATRIXA16 ) );
            m_Shape[n]->lpMotion->nFrame = 0;
        }
        for ( DWORD n = 0; n < m_dwPtcls; n++ )
        {
            _D3DXMATRIXA16 mm;
            Motion_GetMatrix ( lpObj->m_phy[j]->lpMotion,
                               0,
                               &mm );
            D3DXMatrixMultiply ( &mm,
                                 &mm,
                                 &lpObj->m_phy[j]->lpMotion->matrix[0] );
            CopyMemory ( &m_Ptcl[n]->matrix,
                         &mm,
                         sizeof ( _D3DXMATRIXA16 ) );
        }
        for ( DWORD n = 0; n < m_dwPhys; n++ )
        {
            _D3DXMATRIXA16 mm;
            Motion_GetMatrix ( lpObj->m_phy[j]->lpMotion,
                               0,
                               &mm );
            D3DXMatrixMultiply ( &mm,
                                 &mm,
                                 &lpObj->m_phy[j]->lpMotion->matrix[0] );
            CopyMemory ( &m_Phy[n]->lpMotion->matrix[0],
                         &mm,
                         sizeof ( _D3DXMATRIXA16 ) );
        }
    }
}
//-------------------------------------------------------------------------
void C3DEffect::Bind ( C3DObj* lpObj )
{
    for ( DWORD n = 0; n < m_dwShapes; n++ )
    {
        CopyMemory ( &m_Shape[n]->lpMotion->matrix,
                     &lpObj->m_phy[0]->lpMotion->matrix[0],
                     sizeof ( _D3DXMATRIXA16 ) );
        /*
        D3DXQUATERNION q;
        D3DXQuaternionRotationMatrix(&q, &m_Shape[n]->lpMotion->matrix);
        D3DXQuaternionInverse(&q, &q);
        D3DXMATRIX m;
        D3DXMatrixRotationQuaternion(&m, &q);
        D3DXMatrixMultiply(&m_Shape[n]->lpMotion->matrix, &m_Shape[n]->lpMotion->matrix, &m);*/
        m_Shape[n]->lpMotion->nFrame = 0;
    }
    for ( DWORD n = 0; n < m_dwPtcls; n++ )
    {
        CopyMemory ( &m_Ptcl[n]->matrix,
                     &lpObj->m_phy[0]->lpMotion->matrix[0],
                     sizeof ( _D3DXMATRIXA16 ) );
    }
    for ( DWORD n = 0; n < m_dwPhys; n++ )
    {
        CopyMemory ( &m_Phy[n]->lpMotion->matrix[0],
                     &lpObj->m_phy[0]->lpMotion->matrix[0],
                     sizeof ( _D3DXMATRIXA16 ) );
        /*		D3DXQUATERNION q;
        		D3DXQuaternionRotationMatrix(&q, &m_Phy[n]->lpMotion->matrix[0]);
        		D3DXQuaternionInverse(&q, &q);
        		FLOAT Yaw, FLOAT Pitch, FLOAT Roll;

        		D3DXMATRIX m;
        		D3DXMatrixRotationQuaternion(&m, &q);

        		float fPos[3];
        		fPos[0] = m_Phy[n]->lpMotion->matrix[0]._41;
        		fPos[1] = m_Phy[n]->lpMotion->matrix[0]._43;
        		fPos[2] = m_Phy[n]->lpMotion->matrix[0]._43;

        		D3DXMatrixIdentity(&m_Phy[n]->lpMotion->matrix[0]);

        		m_Phy[n]->lpMotion->matrix[0]._41 = fPos[0];
        		m_Phy[n]->lpMotion->matrix[0]._43 = fPos[1];
        		m_Phy[n]->lpMotion->matrix[0]._43 = fPos[2];*/
    }
}
//-------------------------------------------------------------------------
void C3DEffect::Draw ( BOOL bNextFrame,  int nAsb, int nAdb )
{
    if (m_dwShapes > 0)
    {
        if (CMyBitmap::s_nShowMode != modeShape)
        {
            Shape_Prepare ();
            CMyBitmap::s_nShowMode = modeShape;
        }
        for ( DWORD n = 0; n < m_dwShapes; n++ )
        {
            if ( m_Shape[n] )
            {
                Shape_Muliply(m_Shape[n], &matrix);
                if (m_Shape[n]->nTex > 0 &&  m_nShapeAir != 1)
                {
                    Shape_Draw ( m_Shape[n], false, nAsb, nAdb );
                }
                else
                {
                    Shape_DrawAlpha ( m_Shape[n], false);
                }
                Shape_ClearMatrix ( m_Shape[n] );
            }
        }
    }
    if (m_dwPtcls > 0)
    {
        if (CMyBitmap::s_nShowMode != modePtcl)
        {
            Shape_Prepare ();
            CMyBitmap::s_nShowMode = modePtcl;
        }
        Ptcl_Prepare ();
        for (DWORD n = 0; n < m_dwPtcls; n++ )
        {
            if ( m_Ptcl[n] )
            {
                Ptcl_Muliply(m_Ptcl[n], &matrix);
                Ptcl_Draw ( m_Ptcl[n], nAsb, nAdb );
                Ptcl_ClearMatrix ( m_Ptcl[n] );
                if (bNextFrame)
                {
                    Ptcl_NextFrame ( m_Ptcl[n], 1 );
                }
            }
        }
    }
    if (m_dwPhys > 0)
    {
        static int s_nAsb = 0;
        s_nAsb = ::TimeGet() % 2550 / 10;
        if (CMyBitmap::s_nShowMode != modeMesh)
        {
            Phy_Prepare ();
            CMyBitmap::s_nShowMode = modeMesh;
        }
        for (DWORD n = 0; n < m_dwPhys; n++)
        {
            if (m_Phy[n])
            {
                Phy_Muliply(m_Phy[n], -1, &matrix);
                Phy_Calculate(m_Phy[n]);
                Phy_DrawNormal(m_Phy[n]);
                Phy_DrawAlpha(m_Phy[n], false, nAsb, nAdb);
                Phy_ClearMatrix(m_Phy[n]);
                if (bNextFrame)
                {
                    Phy_NextFrame(m_Phy[n], 1);
                }
            }
        }
    }
    D3DXMatrixIdentity ( &matrix );
}
//-------------------------------------------------------------------------
void C3DEffect::SetTexture(C3DTexture* pTexture)
{
    if (!pTexture)
    {
        return;
    }
   
    C3Texture* pMyTexture = pTexture->GetTexture();
    int nTextureID = pMyTexture->nID;
    MYASSERT(pMyTexture);
    for(DWORD n = 0; n < m_dwShapes; n++)
    {
        if (m_Shape[n])
        {
            m_Shape[n]->nTex = nTextureID;
        }
    }
    for(DWORD n = 0; n < m_dwPtcls; n++)
    {
        if (m_Ptcl[n])
        {
            m_Ptcl[n]->nTex = nTextureID;
        }
    }
    for(DWORD n = 0; n < m_dwPhys; n++)
    {
        if (m_Phy[n])
        {
            m_Phy[n]->nTex = nTextureID;
        }
    }
}
//-------------------------------------------------------------------------
void C3DEffect::Move(float x, float y, float z)
{
    Common_Translate(&matrix, x, y, z);
}
//-------------------------------------------------------------------------
void C3DEffect::Rotate(float x, float y, float z)
{
    Common_Rotate(&matrix, x, y, z);
}
//-------------------------------------------------------------------------
void C3DEffect::Scale(float x, float y, float z)
{
    Common_Scale (&matrix, x, y, z);
}

//-------------------------------------------------------------------------
void C3DEffect::ResetShape ( void )
{
    for (DWORD  n = 0; n < m_dwShapes; n++ )
    {
        if ( m_Shape[n] )
        {
            m_Shape[n]->bFrist = true;
            m_Shape[n]->dwSegmentCur = 0;
        }
    }
}

//-------------------------------------------------------------------------
ShapeBackupInfo* C3DEffect::CreateShapeInfo(void)
{
    if (m_dwShapes <= 0)
    {
        return NULL;
    }
    ShapeBackupInfo* pBuf = new ShapeBackupInfo[m_dwShapes];
    if (!pBuf)
    {
        return NULL;
    }
    //memset (pBuf, 0L, sizeof(ShapeBackupInfo)*m_dwShapes);
    for (int i = 0; i < (int)m_dwShapes; i++)
    {
        MYASSERT (m_Shape[i]);
        ShapeBackupInfo* pInfo = pBuf + i;
        pInfo->vb = new ShapeOutVertex[m_Shape[i]->dwSegment * 6];
    }
    return pBuf;
}

//-------------------------------------------------------------------------
void C3DEffect::DestroyShapeInfo (ShapeBackupInfo*& pBuf, int nShapes)
{
    if (!pBuf || nShapes <= 0)
    {
        return;
    }
    for (int i = 0; i < nShapes; i++)
    {
        ShapeBackupInfo* pInfo = pBuf + i;
        delete[] pInfo->vb;
        pInfo->vb = NULL;
    }
    delete[] pBuf;
    pBuf = NULL;
}

//-------------------------------------------------------------------------
void C3DEffect::BackupShapeInfo	(ShapeBackupInfo* pBuf)
{
    if (!pBuf)
    {
        return;
    }
    for (int i = 0; i < (int)m_dwShapes; i++)
    {
        MYASSERT (m_Shape[i]);
        memcpy(pBuf->vb, m_Shape[i]->vb, sizeof(ShapeOutVertex)*m_Shape[i]->dwSegment * 6);
        pBuf->dwSegmentCur	= m_Shape[i]->dwSegmentCur;
        pBuf->last[0]		= m_Shape[i]->last[0];
        pBuf->last[1]		= m_Shape[i]->last[1];
        pBuf->bFrist		= m_Shape[i]->bFrist;
        // next
        pBuf ++;
    }
}

//-------------------------------------------------------------------------
void C3DEffect::RestoreShapeInfo (ShapeBackupInfo* pBuf)
{
    if (!pBuf)
    {
        return;
    }
    for (int i = 0; i < (int)m_dwShapes; i++)
    {
        MYASSERT (m_Shape[i]);
        memcpy(m_Shape[i]->vb, pBuf->vb, sizeof(ShapeOutVertex)*m_Shape[i]->dwSegment * 6);
        m_Shape[i]->dwSegmentCur	= pBuf->dwSegmentCur;
        m_Shape[i]->last[0]			= pBuf->last[0];
        m_Shape[i]->last[1]			= pBuf->last[1];
        m_Shape[i]->bFrist			= pBuf->bFrist;
        // next
        pBuf ++;
    }
}

//-------------------------------------------------------------------------
void C3DEffect::SetPtclCurFrame ( DWORD dwIndex, DWORD dwFrame )
{
    Ptcl_SetFrame ( m_Ptcl[dwIndex], dwFrame );
}

//-------------------------------------------------------------------------
int C3DEffect::GetPtclFrames ( DWORD dwIndex )
{
    return m_Ptcl[dwIndex]->dwFrames;
}

//-------------------------------------------------------------------------
void C3DEffect::SetPhyCurFrame ( DWORD dwIndex, DWORD dwFrame )
{
    Phy_SetFrame ( m_Phy[dwIndex], dwFrame );
}
//-------------------------------------------------------------------------
int C3DEffect::GetPhyFrames ( DWORD dwIndex )
{
    return m_Phy[dwIndex]->lpMotion->dwFrames;
}
//-------------------------------------------------------------------------
DWORD C3DEffect::GetFrames()
{
    if (m_dwPtcls > 0)
    {
        return m_Ptcl[0]->dwFrames;
    }
    if (m_dwPhys > 0)
    {
        return m_Phy[0]->lpMotion->dwFrames;
    }
    if (m_dwShapes > 0)
    {
        return m_Shape[0]->lpMotion->dwFrames;
    }
    return 0;
}
//-------------------------------------------------------------------------
void C3DEffect::SetFrames(DWORD dwFrame)
{
    if (m_dwPtcls > 0)
    {
        for(DWORD i = 0; i < m_dwPtcls; i++)
        {
            Ptcl_SetFrame ( m_Ptcl[i], dwFrame );
        }
    }
    if (m_dwPhys > 0)
    {
        for(DWORD i = 0; i < m_dwPhys; i++)
        {
            Phy_SetFrame ( m_Phy[i], dwFrame );
        }
    }
    if (m_dwShapes > 0)
    {
        for(DWORD i = 0; i < m_dwShapes; i++)
        {
            Shape_SetFrame (m_Shape[i], dwFrame);
        }
    }
}
//-------------------------------------------------------------------------
