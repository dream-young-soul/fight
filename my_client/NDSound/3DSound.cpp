
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-----------------------------------------------------------------------------
// File: 3DAudio.cpp
//
// Desc: Plays a primary segment using DirectMusic
//
// Copyright (c) 2000-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include <tchar.h>
#include <windows.h>
#include <basetsd.h>
#include <commdlg.h>
#include <commctrl.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <dxerr8.h>
#include <cguid.h>
#include <math.h>
#include <stdio.h>
#include "3dSound.h"
#include "DMUtil.h"


C3DSound::C3DSound()
{
    m_pMusicManager          = NULL;
    m_pMusicSegment          = NULL;
    m_p3DAudioPath           = NULL;
    m_pDS3DBuffer            = NULL;
    m_pDSListener			 = NULL;
    m_bAllowMovementTimer    = TRUE;
    m_nBeginTime			 = 0;
}

BOOL C3DSound::RePlay(int nWorldX, int nWorldY, int nWorldZ, int nRange, int nLoop)
{
    m_dwSleepTime = 0;
    m_nPosX = nWorldX ;
    m_nPosY = nWorldY;
    m_nPosZ = nWorldZ;
    if (nLoop == -1)
    {
        nLoop = DMUS_SEG_REPEAT_INFINITE;
    }
    if ( FAILED(  m_pMusicSegment->SetRepeats(nLoop) ) )
    {
        return 0;
    }
    if ( FAILED( m_pMusicSegment->Play(DMUS_SEGF_DEFAULT , m_p3DAudioPath ) ) )//DMUS_SEGF_BEAT
    {
        return 0;
    }
    return 1;
}

BOOL C3DSound::	PlaySound(HWND hwnd , char* szFileName, int nWorldX , int nWorldY, int nWorldZ, int nRange, int nloop)
{
    m_dwSleepTime = 0;
    this->Close();
    this->InitAudio(hwnd, szFileName, nRange);
    m_nPosX = nWorldX ;
    m_nPosY = nWorldY;
    m_nPosZ = nWorldZ;
    if ( m_pMusicSegment )
    {
        m_pMusicSegment->Stop( 0 );
    }
    LoadSegmentFile( hwnd, szFileName ) ;
    HRESULT hr;
    if ( m_pMusicSegment == NULL )
    {
        return 0;
    }
    // Set the segment to repeat many times
    if (nloop == -1)
    {
        nloop = DMUS_SEG_REPEAT_INFINITE;
    }
    if ( FAILED( hr = m_pMusicSegment->SetRepeats(nloop) ) )
    {
        return 0;
    }
    if ( FAILED( hr = m_pMusicSegment->Play(DMUS_SEGF_DEFAULT , m_p3DAudioPath ) ) )//DMUS_SEGF_BEAT
    {
        return 0;
    }
    return 1;
}

void C3DSound::	Close(void)
{
    // Cleanup everything
    if ( m_pMusicSegment )
    {
        m_pMusicSegment->Unload( m_p3DAudioPath );
        SAFE_DELETE( m_pMusicSegment );
    }
    SAFE_RELEASE( m_pDSListener );
    SAFE_RELEASE( m_pDS3DBuffer );
    SAFE_RELEASE( m_p3DAudioPath );
    SAFE_DELETE( m_pMusicManager );
}

void C3DSound::	Pause(void)
{
    m_pMusicSegment->Stop( DMUS_SEGF_BEAT );
}
void C3DSound::Continue(void)
{
    m_pMusicSegment->Play(DMUS_SEGF_BEAT);
}

void C3DSound::Stop(void)
{
    m_pMusicSegment->Stop( DMUS_SEGF_BEAT );
    m_dwSleepTime = 1;
}

int	C3DSound::GetVolume()
{
    return m_nVolume;
}
void C3DSound::SetVolume(int nVolume)
{
    m_nVolume = nVolume;
    IDirectMusicPerformance* pPerf = NULL;
    if ( m_pMusicManager )
    {
        pPerf = m_pMusicManager->GetPerformance();
    }
    if ( NULL == pPerf )
    {
        return;
    }
    nVolume = -50 * (100 - nVolume); // -5000   --0
    // Adjust the slider position to match GUID_PerfMasterTempo range
    pPerf->SetGlobalParam( GUID_PerfMasterVolume, (void*)&nVolume, sizeof(long) );
}
BOOL C3DSound::IsPlaying(void)
{
    return m_pMusicSegment->IsPlaying();
}

BOOL C3DSound::IsBeginning()
{
    m_nBeginTime++;
    if (m_nBeginTime > 10000)
    {
        m_nBeginTime = 10;
    }
    if (m_nBeginTime < 10)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void C3DSound::SetSoundPos(int nX, int nY, int nZ)
{
    D3DVECTOR vPosition;
    vPosition.x =  (float)( m_nPosX - nX);
    vPosition.y =  (float)( m_nPosY - nY);
    vPosition.z =  (float)( m_nPosZ - nZ);
    m_pDS3DBuffer->SetPosition( vPosition.x / 20  , vPosition.y / 20, vPosition.z / 20 , DS3D_IMMEDIATE ) ;
}

void C3DSound::ResetOrigPos(int nX, int nY, int nZ)
{
    m_nPosX = nX;
    m_nPosY = nY;
    m_nPosZ = nZ;
}


HRESULT C3DSound::InitAudio( HWND hDlg , TCHAR* strFileName , int nRange)
{
    HRESULT hr;
    m_pMusicManager = new CMusicManager();
    if ( FAILED( hr = m_pMusicManager->Initialize( hDlg ) ) )
    {
        return DXTRACE_ERR( TEXT("Initialize"), hr );
    }
    // Create a 3D Audiopath. This creates a synth port that feeds a 3d buffer.
    // We can then play all segments into this buffer and directly control its
    // 3D parameters.
    IDirectMusicPerformance8* pPerformance = m_pMusicManager->GetPerformance();
    if ( FAILED( hr = pPerformance->CreateStandardAudioPath( DMUS_APATH_DYNAMIC_3D, 128,
                      TRUE, &m_p3DAudioPath ) ) )
    {
        return DXTRACE_ERR( TEXT("CreateStandardAudioPath"), hr );
    }
    if ( FAILED( hr = m_p3DAudioPath->GetObjectInPath( 0, DMUS_PATH_BUFFER, 0,
                      GUID_NULL, 0, IID_IDirectSound3DBuffer,
                      (LPVOID*) &m_pDS3DBuffer ) ) )
    {
        return DXTRACE_ERR( TEXT("GetObjectInPath"), hr );
    }
    if ( FAILED( hr = m_p3DAudioPath->GetObjectInPath( 0, DMUS_PATH_PRIMARY_BUFFER, 0,
                      GUID_NULL, 0, IID_IDirectSound3DListener,
                      (LPVOID*) &m_pDSListener ) ) )
    {
        return DXTRACE_ERR( TEXT("GetObjectInPath"), hr );
    }
    // Set a new rolloff factor (1.0f is default)
    if ( FAILED( hr = m_pDSListener->SetRolloffFactor( 0.25f, DS3D_IMMEDIATE ) ) )
    {
        return DXTRACE_ERR( TEXT("SetRolloffFactor"), hr );
    }
    Set3DParameters(0, 0, 0, (float)nRange);
    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: LoadSegmentFile()
// Desc:
//-----------------------------------------------------------------------------
HRESULT C3DSound::LoadSegmentFile( HWND hDlg, TCHAR* strFileName )
{
    // Free any previous segment, and make a new one
    SAFE_DELETE( m_pMusicSegment );
    BOOL bMidiFile = FALSE;
    if ( strstr( strFileName, ".mid" ) != NULL ||
            strstr( strFileName, ".rmi" ) != NULL )
    {
        bMidiFile = TRUE;
    }
    // Load the file into a DirectMusic segment, but don't download
    // it to the default audio path -- use the 3D audio path instead
    if ( FAILED( m_pMusicManager->CreateSegmentFromFile( &m_pMusicSegment, strFileName,
                 TRUE, bMidiFile ) ) )
    {
        // Not a critical failure, so just update the status
        return S_FALSE;
    }
    return S_OK;
}







//-----------------------------------------------------------------------------
// Name: Set3DParameters()
// Desc: Set the 3D buffer parameters
//-----------------------------------------------------------------------------
VOID C3DSound::Set3DParameters( FLOAT fDopplerFactor, FLOAT fRolloffFactor,
                                FLOAT fMinDistance,   FLOAT fMaxDistance )
{
    m_dsBufferParams.dwSize = sizeof(DS3DBUFFER);
    m_pDS3DBuffer->GetAllParameters( &m_dsBufferParams );
    m_dsBufferParams.dwMode = DS3DMODE_HEADRELATIVE;
    m_dsBufferParams.flMinDistance = fMinDistance;
    m_dsBufferParams.flMaxDistance = fMaxDistance;
    if ( m_pDS3DBuffer )
    {
        m_pDS3DBuffer->SetAllParameters( &m_dsBufferParams, DS3D_IMMEDIATE );
    }
}

