
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//-----------------------------------------------------------------------------
// File: DSUtil.cpp
//
// Desc: DirectSound framework classes for reading and writing wav files and
//       playing them in DirectSound buffers. Feel free to use this class
//       as a starting point for adding extra functionality.
//
// Copyright (c) 1999-2000 Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include <windows.h>
#include <mmsystem.h>
#include <dxerr8.h>
#include <dsound.h>
#include "DSUtil.h"
#include "ndsound.h"

//-----------------------------------------------------------------------------
// Name: CSoundManager::CSoundManager()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
CSoundManager::CSoundManager()
{
    m_pDS = NULL;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::~CSoundManager()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
CSoundManager::~CSoundManager()
{
    SAFE_RELEASE( m_pDS );
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::Initialize()
// Desc: Initializes the IDirectSound object and also sets the primary buffer
//       format.  This function must be called before any others.
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Initialize( HWND  hWnd,
                                   DWORD dwCoopLevel,
                                   DWORD dwPrimaryChannels,
                                   DWORD dwPrimaryFreq,
                                   DWORD dwPrimaryBitRate )
{
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;
    SAFE_RELEASE( m_pDS );
    // Create IDirectSound using the primary sound device
    if ( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS, NULL ) ) )
    {
        return -1;    //DXTRACE_ERR( TEXT("DirectSoundCreate8"), hr );
    }
    // Set DirectSound coop level
    if ( FAILED( hr = m_pDS->SetCooperativeLevel( hWnd, dwCoopLevel ) ) )
    {
        m_pDS->Release();
        m_pDS = NULL;
        return -2;//DXTRACE_ERR( TEXT("SetCooperativeLevel"), hr );
    }
    // Set primary buffer format
    SetPrimaryBufferFormat( dwPrimaryChannels, dwPrimaryFreq, dwPrimaryBitRate );
    return 0;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::SetPrimaryBufferFormat()
// Desc: Set primary buffer to a specified format
//       For example, to set the primary buffer format to 22kHz stereo, 16-bit
//       then:   dwPrimaryChannels = 2
//               dwPrimaryFreq     = 22050,
//               dwPrimaryBitRate  = 16
//-----------------------------------------------------------------------------
int CSoundManager::SetPrimaryBufferFormat( DWORD dwPrimaryChannels,
        DWORD dwPrimaryFreq,
        DWORD dwPrimaryBitRate )
{
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;
    if ( m_pDS == NULL )
    {
        return -3;
    }
    // Get the primary buffer
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
    if ( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) )
    {
        return -3;    //DXTRACE_ERR( TEXT("CreateSoundBuffer"), hr );
    }
    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
    wfx.wFormatTag      = WAVE_FORMAT_PCM;
    wfx.nChannels       = (WORD) dwPrimaryChannels;
    wfx.nSamplesPerSec  = dwPrimaryFreq;
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate;
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    if ( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
    {
        SAFE_RELEASE( pDSBPrimary );
        return -3;//DXTRACE_ERR( TEXT("SetFormat"), hr );
    }
    SAFE_RELEASE( pDSBPrimary );
    return 0;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::Get3DListenerInterface()
// Desc: Returns the 3D listener interface associated with primary buffer.
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener )
{
    HRESULT             hr;
    DSBUFFERDESC        dsbdesc;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;
    if ( ppDSListener == NULL )
    {
        return E_INVALIDARG;
    }
    if ( m_pDS == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    *ppDSListener = NULL;
    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    if ( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
    {
        return -1;    //DXTRACE_ERR( TEXT("CreateSoundBuffer"), hr );
    }
    if ( FAILED( hr = pDSBPrimary->QueryInterface( IID_IDirectSound3DListener,
                      (VOID**)ppDSListener ) ) )
    {
        SAFE_RELEASE( pDSBPrimary );
        return -1;//DXTRACE_ERR( TEXT("QueryInterface"), hr );
    }
    // Release the primary buffer, since it is not need anymore
    SAFE_RELEASE( pDSBPrimary );
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Create( CSound** ppSound,
                               LPTSTR strWaveFileName,
                               DWORD dwCreationFlags,
                               GUID guid3DAlgorithm,
                               DWORD dwNumBuffers )
{
    HRESULT hr;
    HRESULT hrRet = S_OK;
    LPDIRECTSOUNDBUFFER* apDSBuffer     = NULL;
    DWORD                dwDSBufferSize = NULL;
    CWaveFile*           pWaveFile      = NULL;
    if ( m_pDS == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    if ( strWaveFileName == NULL || ppSound == NULL || dwNumBuffers < 1 )
    {
        return E_INVALIDARG;
    }
    pWaveFile = new CWaveFile();
    if ( pWaveFile == NULL )
    {
        hr = E_OUTOFMEMORY;
        goto LFail;
    }
    apDSBuffer = new LPDIRECTSOUNDBUFFER;
    pWaveFile->Open( strWaveFileName, NULL, WAVEFILE_READ );
    if ( pWaveFile->GetSize() == 0 )
    {
        // Wave is blank, so don't create it.
        hr = E_FAIL;
        goto LFail;
    }
    // Make the DirectSound buffer the same size as the wav file
    dwDSBufferSize = pWaveFile->GetSize();
    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = guid3DAlgorithm;
    dsbd.lpwfxFormat     = pWaveFile->m_pwfx;
    // DirectSound is only guarenteed to play PCM data.  Other
    // formats may or may not work depending the sound card driver.
    hr = m_pDS->CreateSoundBuffer( &dsbd, apDSBuffer, NULL );
    // Be sure to return this error code if it occurs so the
    // callers knows this happened.
    if ( hr == DS_NO_VIRTUALIZATION )
    {
        hrRet = DS_NO_VIRTUALIZATION;
    }
    if ( FAILED(hr) )
    {
        goto LFail;
    }
    // Create the sound
    *ppSound = new CSound( *apDSBuffer, dwDSBufferSize, 1, pWaveFile );
    SAFE_DELETE( apDSBuffer );
    return hrRet;
LFail:
    // Cleanup
    SAFE_DELETE( pWaveFile );
    SAFE_DELETE( apDSBuffer );
    return hr;
}









//-----------------------------------------------------------------------------
// Name: CSoundManager::CreateFromMemory()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CSoundManager::CreateFromMemory( CSound** ppSound,
        BYTE* pbData,
        ULONG  ulDataSize,
        LPWAVEFORMATEX pwfx,
        DWORD dwCreationFlags,
        GUID guid3DAlgorithm,
        DWORD dwNumBuffers )
{
    HRESULT hr;
    LPDIRECTSOUNDBUFFER apDSBuffer     = NULL;
    DWORD                dwDSBufferSize = NULL;
    CWaveFile*           pWaveFile      = NULL;
    if ( m_pDS == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    if ( pbData == NULL || ppSound == NULL || dwNumBuffers < 1 )
    {
        return E_INVALIDARG;
    }
    pWaveFile = new CWaveFile();
    if ( pWaveFile == NULL )
    {
        hr = E_OUTOFMEMORY;
        goto LFail;
    }
    pWaveFile->OpenFromMemory( pbData, ulDataSize, pwfx, WAVEFILE_READ );
    // Make the DirectSound buffer the same size as the wav file
    dwDSBufferSize = ulDataSize;
    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = guid3DAlgorithm;
    dsbd.lpwfxFormat     = pwfx;
    if ( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer, NULL ) ) )
    {
        goto LFail;
    }
    // Create the sound
    *ppSound = new CSound( apDSBuffer, dwDSBufferSize, 1, pWaveFile );
    SAFE_RELEASE( apDSBuffer );
    return S_OK;
LFail:
    // Cleanup
    SAFE_RELEASE( apDSBuffer );
    return hr;
}





//-----------------------------------------------------------------------------
// Name: CSound::CSound()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
CSound::CSound( LPDIRECTSOUNDBUFFER apDSBuffer, DWORD dwDSBufferSize,
                DWORD dwNumBuffers, CWaveFile* pWaveFile ): m_n3DSoundActived(0)
{
    m_apDSBuffer = apDSBuffer;
    m_dwDSBufferSize = dwDSBufferSize;
    m_dwNumBuffers   = 1;
    m_pWaveFile      = pWaveFile;
    FillBufferWithSound( m_apDSBuffer, FALSE );
    m_apDSBuffer->SetCurrentPosition(0);
}

void CSound::SetVolume(int nVolume)
{
    if ( nVolume < 0 || nVolume > 100 )
    {
        return;
    }
    int VolSet = (100 - nVolume *  m_nSelfVolume / 100) * VOL_MIN / 100;
    if (VolSet > 0)
    {
        VolSet = 0;
    }
    if (VolSet < VOL_MIN)
    {
        VolSet = VOL_MIN;
    }
    m_apDSBuffer->SetVolume( VolSet );
    m_nVolume = nVolume;
}
void CSound::SetPan(int nPan)
{
    if ( nPan < -100 || nPan > 100 )
    {
        return;
    }
    int PanSet = nPan * PAN_MAX / 100;
    m_apDSBuffer->SetPan( PanSet );
    m_nPan = nPan;
}



//-----------------------------------------------------------------------------
// Name: CSound::~CSound()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
CSound::~CSound()
{
    SAFE_RELEASE( m_apDSBuffer );
    SAFE_DELETE( m_pWaveFile );
}




//-----------------------------------------------------------------------------
// Name: CSound::FillBufferWithSound()
// Desc: Fills a DirectSound buffer with a sound file
//-----------------------------------------------------------------------------
HRESULT CSound::FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger )
{
    HRESULT hr;
    VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file
    if ( pDSB == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    // Make sure we have focus, and we didn't just switch in from
    // an app which had a DirectSound device
    if ( FAILED( hr = RestoreBuffer( pDSB, NULL ) ) )
    {
        return -1;
    }
    // Lock the buffer down
    if ( FAILED( hr = pDSB->Lock( 0, m_dwDSBufferSize,
                                  &pDSLockedBuffer, &dwDSLockedBufferSize,
                                  NULL, NULL, 0L ) ) )
    {
        return -1;
    }
    // Reset the wave file to the beginning
    m_pWaveFile->ResetFile();
    if ( FAILED( hr = m_pWaveFile->Read( (BYTE*) pDSLockedBuffer,
                                         dwDSLockedBufferSize,
                                         &dwWavDataRead ) ) )
    {
        return -1;
    }
    if ( dwWavDataRead == 0 )
    {
        // Wav is blank, so just fill with silence
        FillMemory( (BYTE*) pDSLockedBuffer,
                    dwDSLockedBufferSize,
                    (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }
    else if ( dwWavDataRead < dwDSLockedBufferSize )
    {
        // If the wav file was smaller than the DirectSound buffer,
        // we need to fill the remainder of the buffer with data
        if ( bRepeatWavIfBufferLarger )
        {
            // Reset the file and fill the buffer with wav data
            DWORD dwReadSoFar = dwWavDataRead;    // From previous call above.
            while( dwReadSoFar < dwDSLockedBufferSize )
            {
                // This will keep reading in until the buffer is full
                // for very short files
                if ( FAILED( hr = m_pWaveFile->ResetFile() ) )
                {
                    return -1;
                }
                hr = m_pWaveFile->Read( (BYTE*)pDSLockedBuffer + dwReadSoFar,
                                        dwDSLockedBufferSize - dwReadSoFar,
                                        &dwWavDataRead );
                if ( FAILED(hr) )
                {
                    return -1;
                }
                dwReadSoFar += dwWavDataRead;
            }
        }
        else
        {
            // Don't repeat the wav file, just fill in silence
            FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead,
                        dwDSLockedBufferSize - dwWavDataRead,
                        (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
        }
    }
    // Unlock the buffer, we don't need it anymore.
    pDSB->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSound::RestoreBuffer()
// Desc: Restores the lost buffer. *pbWasRestored returns TRUE if the buffer was
//       restored.  It can also NULL if the information is not needed.
//-----------------------------------------------------------------------------
HRESULT CSound::RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored )
{
    HRESULT hr;
    if ( pDSB == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    if ( pbWasRestored )
    {
        *pbWasRestored = FALSE;
    }
    DWORD dwStatus = 0;
    if ( FAILED( hr = pDSB->GetStatus( &dwStatus ) ) )
    {
        return -1;
    }
    if ( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so
        // the restoring the buffer may fail.
        // If it does, sleep until DirectSound gives us control.
        do
        {
            hr = pDSB->Restore();
            if ( hr == DSERR_BUFFERLOST )
            {
                Sleep( 10 );
            }
        }
        while( hr = pDSB->Restore() );
        if ( pbWasRestored != NULL )
        {
            *pbWasRestored = TRUE;
        }
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}




//-----------------------------------------------------------------------------
// Name: CSound::Play()
// Desc: Plays the sound using voice management flags.  Pass in DSBPLAY_LOOPING
//       in the dwFlags to loop the sound
//-----------------------------------------------------------------------------

BOOL CSound::RePlay(int nLoop, int nRange, int nX, int nY)
{
    m_nWorldX = nX;
    m_nWorldY = nY;
    m_nRange = nRange;
    m_nLoop = nLoop;
    Continue();
    return 1;
}
HRESULT CSound::Play( DWORD dwPriority, DWORD dwFlags )
{
    HRESULT hr;
    BOOL    bRestored;
    if ( m_apDSBuffer == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    LPDIRECTSOUNDBUFFER pDSB = m_apDSBuffer;
    if ( pDSB == NULL )
    {
        return -1;
    }
    // Restore the buffer if it was lost
    if ( FAILED( hr = RestoreBuffer( pDSB, &bRestored ) ) )
    {
        return -1;
    }
    if ( bRestored )
    {
        // The buffer was restored, so we need to fill it with new data
        if ( FAILED( hr = FillBufferWithSound( pDSB, FALSE ) ) )
        {
            return -1;
        }
        m_apDSBuffer->SetCurrentPosition( 0 );
    }
    m_dwSleepTime = 0;
    m_n3DSoundActived = 1;
    return pDSB->Play( 0, dwPriority, dwFlags );
}




//-----------------------------------------------------------------------------
// Name: CSound::Stop()
// Desc: Stops the sound from playing
//-----------------------------------------------------------------------------
HRESULT CSound::Stop()
{
    if ( m_apDSBuffer == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    HRESULT hr = 0;
    hr |= m_apDSBuffer->Stop();
    m_n3DSoundActived = 0;
    m_dwSleepTime = 1;
    return hr;
}

HRESULT CSound::Continue()
{
    HRESULT hr;
    BOOL    bRestored;
    if ( m_apDSBuffer == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    LPDIRECTSOUNDBUFFER pDSB = m_apDSBuffer;
    if ( pDSB == NULL )
    {
        return -1;
    }
    // Restore the buffer if it was lost
    if ( FAILED( hr = RestoreBuffer( pDSB, &bRestored ) ) )
    {
        return -1;
    }
    if ( bRestored )
    {
        // The buffer was restored, so we need to fill it with new data
        if ( FAILED( hr = FillBufferWithSound( pDSB, FALSE ) ) )
        {
            return -1;
        }
    }
    DWORD flag;
    if ( (m_dwControl & DSBPLAY_LOOPING ) == 0 )
    {
        flag = 0;
    }
    else
    {
        flag = DSBPLAY_LOOPING;
    }
    m_apDSBuffer->SetCurrentPosition(0);
    hr |= m_apDSBuffer->Play(0, 0, flag);
    SetVolume(m_nVolume);
    m_dwSleepTime = 0;
    m_n3DSoundActived = 1;
    return hr;
}






//-----------------------------------------------------------------------------
// Name: CSound::IsSoundPlaying()
// Desc: Checks to see if a buffer is playing and returns TRUE if it is.
//-----------------------------------------------------------------------------
BOOL CSound::IsSoundPlaying()
{
    BOOL bIsPlaying = FALSE;
    if ( m_apDSBuffer )
    {
        DWORD dwStatus = 0;
        m_apDSBuffer->GetStatus( &dwStatus );
        bIsPlaying |= ( ( dwStatus & DSBSTATUS_PLAYING ) != 0 );
    }
    else
    {
        return FALSE;
    }
    return bIsPlaying;
}



//-----------------------------------------------------------------------------
// Name: CWaveFile::CWaveFile()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.
//       Then call Read() as needed.  Calling the destructor or Close()
//       will close the file.
//-----------------------------------------------------------------------------
CWaveFile::CWaveFile()
{
    m_pwfx    = NULL;
    m_hmmio   = NULL;
    m_dwSize  = 0;
    m_bIsReadingFromMemory = FALSE;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::~CWaveFile()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
CWaveFile::~CWaveFile()
{
    Close();
    if ( !m_bIsReadingFromMemory )
    {
        SAFE_DELETE_ARRAY( m_pwfx );
    }
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Open( LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags )
{
    HRESULT hr;
    m_dwFlags = dwFlags;
    m_bIsReadingFromMemory = FALSE;
    if ( m_dwFlags == WAVEFILE_READ )
    {
        if ( strFileName == NULL )
        {
            return E_INVALIDARG;
        }
        SAFE_DELETE_ARRAY( m_pwfx );
        m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );
        if ( NULL == m_hmmio )
        {
            HRSRC   hResInfo;
            HGLOBAL hResData;
            DWORD   dwSize;
            VOID*   pvRes;
            // Loading it as a file failed, so try it as a resource
            if ( NULL == ( hResInfo = FindResource( NULL, strFileName, TEXT("WAVE") ) ) )
            {
                if ( NULL == ( hResInfo = FindResource( NULL, strFileName, TEXT("WAV") ) ) )
                {
                    return -1;
                }
            }
            if ( NULL == ( hResData = LoadResource( NULL, hResInfo ) ) )
            {
                return -1;
            }
            if ( 0 == ( dwSize = SizeofResource( NULL, hResInfo ) ) )
            {
                return -1;
            }
            if ( NULL == ( pvRes = LockResource( hResData ) ) )
            {
                return -1;
            }
            CHAR* pData = new CHAR[ dwSize ];
            memcpy( pData, pvRes, dwSize );
            MMIOINFO mmioInfo;
            ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
            mmioInfo.fccIOProc = FOURCC_MEM;
            mmioInfo.cchBuffer = dwSize;
            mmioInfo.pchBuffer = (CHAR*) pData;
            m_hmmio = mmioOpen( NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
        }
        if ( FAILED( hr = ReadMMIO() ) )
        {
            // ReadMMIO will fail if its an not a wave file
            mmioClose( m_hmmio, 0 );
            return -1;
        }
        if ( FAILED( hr = ResetFile() ) )
        {
            return -1;
        }
        // After the reset, the size of the wav file is m_ck.cksize so store it now
        m_dwSize = m_ck.cksize;
    }
    else
    {
        m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF  |
                            MMIO_READWRITE |
                            MMIO_CREATE );
        if ( NULL == m_hmmio )
        {
            return -1;
        }
        if ( FAILED( hr = WriteMMIO( pwfx ) ) )
        {
            mmioClose( m_hmmio, 0 );
            return -1;
        }
        if ( FAILED( hr = ResetFile() ) )
        {
            return -1;
        }
    }
    return hr;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::OpenFromMemory()
// Desc: copy data to CWaveFile member variable from memory
//-----------------------------------------------------------------------------
HRESULT CWaveFile::OpenFromMemory( BYTE* pbData, ULONG ulDataSize,
                                   WAVEFORMATEX* pwfx, DWORD dwFlags )
{
    m_pwfx       = pwfx;
    m_ulDataSize = ulDataSize;
    m_pbData     = pbData;
    m_pbDataCur  = m_pbData;
    m_bIsReadingFromMemory = TRUE;
    if ( dwFlags != WAVEFILE_READ )
    {
        return E_NOTIMPL;
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_pwfx.
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ReadMMIO()
{
    MMCKINFO        ckIn;           // chunk info. for general use.
    PCMWAVEFORMAT   pcmWaveFormat;  // Temp PCM structure to load in.
    m_pwfx = NULL;
    if ( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) ) )
    {
        return -1;
    }
    // Check to make sure this is a valid wave file
    if ( (m_ckRiff.ckid != FOURCC_RIFF) ||
            (m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
    {
        return -1;
    }
    // Search the input file for for the 'fmt ' chunk.
    ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if ( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
    {
        return -1;
    }
    // Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
    // if there are extra parameters at the end, we'll ignore them
    if ( ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
    {
        return -1;
    }
    // Read the 'fmt ' chunk into <pcmWaveFormat>.
    if ( mmioRead( m_hmmio, (HPSTR) &pcmWaveFormat,
                   sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat) )
    {
        return -1;
    }
    // Allocate the waveformatex, but if its not pcm format, read the next
    // word, and thats how many extra bytes to allocate.
    if ( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
    {
        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
        if ( NULL == m_pwfx )
        {
            return -1;
        }
        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = 0;
    }
    else
    {
        // Read in length of extra bytes.
        WORD cbExtraBytes = 0L;
        if ( mmioRead( m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD) )
        {
            return -1;
        }
        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) + cbExtraBytes ];
        if ( NULL == m_pwfx )
        {
            return -1;
        }
        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = cbExtraBytes;
        // Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
        if ( mmioRead( m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize)) + sizeof(WORD)),
                       cbExtraBytes ) != cbExtraBytes )
        {
            SAFE_DELETE( m_pwfx );
            return -1;
        }
    }
    // Ascend the input file out of the 'fmt ' chunk.
    if ( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
    {
        SAFE_DELETE( m_pwfx );
        return -1;
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::GetSize()
// Desc: Retuns the size of the read access wave file
//-----------------------------------------------------------------------------
DWORD CWaveFile::GetSize()
{
    return m_dwSize;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::ResetFile()
// Desc: Resets the internal m_ck pointer so reading starts from the
//       beginning of the file again
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ResetFile()
{
    if ( m_bIsReadingFromMemory )
    {
        m_pbDataCur = m_pbData;
    }
    else
    {
        if ( m_hmmio == NULL )
        {
            return CO_E_NOTINITIALIZED;
        }
        if ( m_dwFlags == WAVEFILE_READ )
        {
            // Seek to the data
            if ( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
                                 SEEK_SET ) )
            {
                return -1;
            }
            // Search the input file for the 'data' chunk.
            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            if ( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
            {
                return -1;
            }
        }
        else
        {
            // Create the 'data' chunk that holds the waveform samples.
            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            m_ck.cksize = 0;
            if ( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
            {
                return -1;
            }
            if ( 0 != mmioGetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
            {
                return -1;
            }
        }
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Read()
// Desc: Reads section of data from a wave file into pBuffer and returns
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses m_ck to determine where to start reading from.  So
//       subsequent calls will be continue where the last left off unless
//       Reset() is called.
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
    if ( m_bIsReadingFromMemory )
    {
        if ( m_pbDataCur == NULL )
        {
            return CO_E_NOTINITIALIZED;
        }
        if ( pdwSizeRead != NULL )
        {
            *pdwSizeRead = 0;
        }
        if ( (BYTE*)(m_pbDataCur + dwSizeToRead) >
                (BYTE*)(m_pbData + m_ulDataSize) )
        {
            dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
        }
        CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );
        if ( pdwSizeRead != NULL )
        {
            *pdwSizeRead = dwSizeToRead;
        }
        return S_OK;
    }
    else
    {
        MMIOINFO mmioinfoIn; // current status of m_hmmio
        if ( m_hmmio == NULL )
        {
            return CO_E_NOTINITIALIZED;
        }
        if ( pBuffer == NULL || pdwSizeRead == NULL )
        {
            return E_INVALIDARG;
        }
        if ( pdwSizeRead != NULL )
        {
            *pdwSizeRead = 0;
        }
        if ( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
        {
            return -1;
        }
        UINT cbDataIn = dwSizeToRead;
        if ( cbDataIn > m_ck.cksize )
        {
            cbDataIn = m_ck.cksize;
        }
        m_ck.cksize -= cbDataIn;
        for( DWORD cT = 0; cT < cbDataIn; cT++ )
        {
            // Copy the bytes from the io to the buffer.
            if ( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
            {
                if ( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
                {
                    return -1;
                }
                if ( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
                {
                    return -1;
                }
            }
            // Actual copy.
            *((BYTE*)pBuffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
            mmioinfoIn.pchNext++;
        }
        if ( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
        {
            return -1;
        }
        if ( pdwSizeRead != NULL )
        {
            *pdwSizeRead = cbDataIn;
        }
        return S_OK;
    }
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Close()
// Desc: Closes the wave file
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Close()
{
    if ( m_dwFlags == WAVEFILE_READ )
    {
        mmioClose( m_hmmio, 0 );
        m_hmmio = NULL;
    }
    else
    {
        m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
        if ( m_hmmio == NULL )
        {
            return CO_E_NOTINITIALIZED;
        }
        if ( 0 != mmioSetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
        {
            return -1;
        }
        // Ascend the output file out of the 'data' chunk -- this will cause
        // the chunk size of the 'data' chunk to be written.
        if ( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
        {
            return -1;
        }
        // Do this here instead...
        if ( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
        {
            return -1;
        }
        mmioSeek( m_hmmio, 0, SEEK_SET );
        if ( 0 != (INT)mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) )
        {
            return -1;
        }
        m_ck.ckid = mmioFOURCC('f', 'a', 'c', 't');
        if ( 0 == mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
        {
            DWORD dwSamples = 0;
            mmioWrite( m_hmmio, (HPSTR)&dwSamples, sizeof(DWORD) );
            mmioAscend( m_hmmio, &m_ck, 0 );
        }
        // Ascend the output file out of the 'RIFF' chunk -- this will cause
        // the chunk size of the 'RIFF' chunk to be written.
        if ( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
        {
            return -1;
        }
        mmioClose( m_hmmio, 0 );
        m_hmmio = NULL;
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::WriteMMIO()
// Desc: Support function for reading from a multimedia I/O stream
//       pwfxDest is the WAVEFORMATEX for this new wave file.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_ck.
//-----------------------------------------------------------------------------
HRESULT CWaveFile::WriteMMIO( WAVEFORMATEX* pwfxDest )
{
    DWORD    dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
    MMCKINFO ckOut1;
    dwFactChunk = (DWORD) - 1;
    // Create the output file RIFF chunk of form type 'WAVE'.
    m_ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    m_ckRiff.cksize = 0;
    if ( 0 != mmioCreateChunk( m_hmmio, &m_ckRiff, MMIO_CREATERIFF ) )
    {
        return -1;
    }
    // We are now descended into the 'RIFF' chunk we just created.
    // Now create the 'fmt ' chunk. Since we know the size of this chunk,
    // specify it in the MMCKINFO structure so MMIO doesn't have to seek
    // back and set the chunk size after ascending from the chunk.
    m_ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
    m_ck.cksize = sizeof(PCMWAVEFORMAT);
    if ( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
    {
        return -1;
    }
    // Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type.
    if ( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
    {
        if ( mmioWrite( m_hmmio, (HPSTR) pwfxDest,
                        sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
        {
            return -1;
        }
    }
    else
    {
        // Write the variable length size.
        if ( (UINT)mmioWrite( m_hmmio, (HPSTR) pwfxDest,
                              sizeof(*pwfxDest) + pwfxDest->cbSize ) !=
                ( sizeof(*pwfxDest) + pwfxDest->cbSize ) )
        {
            return -1;
        }
    }
    // Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
    if ( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
    {
        return -1;
    }
    // Now create the fact chunk, not required for PCM but nice to have.  This is filled
    // in when the close routine is called.
    ckOut1.ckid = mmioFOURCC('f', 'a', 'c', 't');
    ckOut1.cksize = 0;
    if ( 0 != mmioCreateChunk( m_hmmio, &ckOut1, 0 ) )
    {
        return -1;
    }
    if ( mmioWrite( m_hmmio, (HPSTR)&dwFactChunk, sizeof(dwFactChunk)) !=
            sizeof(dwFactChunk) )
    {
        return -1;
    }
    // Now ascend out of the fact chunk...
    if ( 0 != mmioAscend( m_hmmio, &ckOut1, 0 ) )
    {
        return -1;
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Write()
// Desc: Writes data to the open wave file
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Write( UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote )
{
    UINT cT;
    if ( m_bIsReadingFromMemory )
    {
        return E_NOTIMPL;
    }
    if ( m_hmmio == NULL )
    {
        return CO_E_NOTINITIALIZED;
    }
    if ( pnSizeWrote == NULL || pbSrcData == NULL )
    {
        return E_INVALIDARG;
    }
    *pnSizeWrote = 0;
    for( cT = 0; cT < nSizeToWrite; cT++ )
    {
        if ( m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite )
        {
            m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
            if ( 0 != mmioAdvance( m_hmmio, &m_mmioinfoOut, MMIO_WRITE ) )
            {
                return -1;
            }
        }
        *((BYTE*)m_mmioinfoOut.pchNext) = *((BYTE*)pbSrcData + cT);
        (BYTE*)m_mmioinfoOut.pchNext++;
        (*pnSizeWrote)++;
    }
    return S_OK;
}




