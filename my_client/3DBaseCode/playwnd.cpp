
//**********************************************************\\
// ´úÂë±à¼­Æ÷\\
//**********************************************************\\
\\
//------------------------------------------------------------------------------
// File: PlayWnd.cpp
//
// Desc: DirectShow sample code - a simple audio/video media file player
//       application.  Pause, stop, mute, and fullscreen mode toggle can
//       be performed via keyboard commands.
//
// Copyright (c) 1996-2001 Microsoft Corporation.  All rights reserved.
// Modify by Longee
//------------------------------------------------------------------------------
#include <dshow.h>
#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#include "playwnd.h"
//-----------------------------------------------------------------------------
// An application can advertise the existence of its filter graph
// by registering the graph with a global Running Object Table (ROT).
// The GraphEdit application can detect and remotely view the running
// filter graph, allowing you to 'spy' on the graph with GraphEdit.
//
// To enable registration in this sample, define REGISTER_FILTERGRAPH.
//-----------------------------------------------------------------------------
#define REGISTER_FILTERGRAPH
//-----------------------------------------------------------------------------
CDXPlayer g_objMediaPlayer;
//-----------------------------------------------------------------------------
CDXPlayer::CDXPlayer()
{
    m_pGraphBuilder		= NULL;
    m_pMediaControl		= NULL;
    m_pMediaEventEx		= NULL;
    m_pVideoWindow		= NULL;
    m_pBaseAudio		= NULL;
    m_pBaseVidio		= NULL;
    m_pMediaSeeking		= NULL;
    m_pMediaPosition	= NULL;
    m_pVideoFrameStep	= NULL;
    m_bAudioOnly		= FALSE;
    m_bFullscreen		= FALSE;
    m_bRepeat			= TRUE;
    m_lVolume			= VOLUME_FULL;
    m_dwGraphRegister	= 0;
    m_psCurrent			= _STOPPED;
    m_PlaybackRate		= 1.0;
    m_hWnd				= NULL;
    // Initialize COM
    //if(FAILED(CoInitialize(NULL)))
    //{
    //    Msg(TEXT("CoInitialize Failed!\r\n"));
    //    exit(1);
    //}
}
//-----------------------------------------------------------------------------
CDXPlayer::~CDXPlayer()
{
    // Finished with COM
    // CoUninitialize();
}
//-----------------------------------------------------------------------------
HRESULT CDXPlayer::PlayMovieInWindow(LPTSTR szFile)
{
    USES_CONVERSION;
    WCHAR wFile[MAX_PATH];
    HRESULT hr;
    // Clear open dialog remnants before calling RenderFile()
    UpdateWindow(m_hWnd);
    // Convert filename to wide character string
    wcscpy(wFile, T2W(szFile));
    // Get the interface for DirectShow's GraphBuilder
    JIF(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
                         IID_IGraphBuilder, (void**)&m_pGraphBuilder));
    if(SUCCEEDED(hr))
    {
        // Have the graph builder construct its the appropriate graph automatically
        JIF(m_pGraphBuilder->RenderFile(wFile, NULL));
        // QueryInterface for DirectShow interfaces
        JIF(m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl));
        JIF(m_pGraphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&m_pMediaEventEx));
        JIF(m_pGraphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&m_pMediaSeeking));
        JIF(m_pGraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&m_pMediaPosition));
        // Query for video interfaces, which may not be relevant for audio files
        JIF(m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&m_pVideoWindow));
        JIF(m_pGraphBuilder->QueryInterface(IID_IBasicVideo, (void**)&m_pBaseVidio));
        // Query for audio interfaces, which may not be relevant for video-only files
        JIF(m_pGraphBuilder->QueryInterface(IID_IBasicAudio, (void**)&m_pBaseAudio));
        // Is this an audio-only file (no video component)?
        CheckVisibility();
        // Have the graph signal event via window callbacks for performance
        JIF(m_pMediaEventEx->SetNotifyWindow((OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0));
        if (!m_bAudioOnly)
        {
            JIF(m_pVideoWindow->put_Owner((OAHWND)m_hWnd));
            JIF(m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN));
            JIF(InitVideoWindow(1, 1));
            GetFrameStepInterface();
        }
        // Let's get ready to rumble!
        //ShowWindow(ghApp, SW_SHOWNORMAL);
        //UpdateWindow(ghApp);
        //SetForegroundWindow(ghApp);
        //SetFocus(ghApp);
        //m_bFullscreen = FALSE;
        m_PlaybackRate = 1.0;
        //UpdateMainTitle();
#ifdef REGISTER_FILTERGRAPH
        hr = AddGraphToRot(m_pGraphBuilder, &m_dwGraphRegister);
        if (FAILED(hr))
        {
            Msg(TEXT("Failed to register filter graph with ROT!  hr=0x%x"), hr);
            m_dwGraphRegister = 0;
        }
#endif
        // Run the graph to play the media file
        JIF(m_pMediaControl->Run());
        m_psCurrent = _RUNNING;
        //SetFocus(m_hWnd);
    }
    return hr;
}
//-----------------------------------------------------------------------------
HRESULT CDXPlayer::InitVideoWindow(int nMultiplier, int nDivider)
{
    LONG lHeight, lWidth;
    HRESULT hr = S_OK;
    RECT rect;
    if (!m_pBaseVidio)
    {
        return S_OK;
    }
    // Read the default video size
    hr = m_pBaseVidio->GetVideoSize(&lWidth, &lHeight);
    if (hr == E_NOINTERFACE)
    {
        return S_OK;
    }
    //EnablePlaybackMenu(TRUE);
    // Account for requests of normal, half, or double size
    lWidth  = lWidth  * nMultiplier / nDivider;
    lHeight = lHeight * nMultiplier / nDivider;
    //SetWindowPos(ghApp, NULL, 0, 0, lWidth, lHeight,
    //             SWP_NOMOVE | SWP_NOOWNERZORDER);
    int nTitleHeight  = GetSystemMetrics(SM_CYCAPTION);
    int nBorderWidth  = GetSystemMetrics(SM_CXBORDER);
    int nBorderHeight = GetSystemMetrics(SM_CYBORDER);
    // Account for size of title bar and borders for exact match
    // of window client area to default video size
    //SetWindowPos(ghApp, NULL, 0, 0, lWidth + 2*nBorderWidth,
    //        lHeight + nTitleHeight + 2*nBorderHeight,
    //        SWP_NOMOVE | SWP_NOOWNERZORDER);
    GetClientRect(m_hWnd, &rect);
    JIF(m_pVideoWindow->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom));
    return hr;
}
//-----------------------------------------------------------------------------
void CDXPlayer::MoveVideoWindow(HWND hWnd)
{
    HRESULT hr;
    // Track the movement of the container window and resize as needed
    if(m_pVideoWindow)
    {
        RECT client;
        GetClientRect(hWnd, &client);
        hr = m_pVideoWindow->SetWindowPosition(client.left, client.top,
                                               client.right, client.bottom);
    }
}
//-----------------------------------------------------------------------------
void CDXPlayer::CheckVisibility(void)
{
    long lVisible;
    HRESULT hr;
    if ((!m_pVideoWindow) || (!m_pBaseVidio))
    {
        // Audio-only files have no video interfaces.  This might also
        // be a file whose video component uses an unknown video codec.
        m_bAudioOnly = TRUE;
        return;
    }
    else
    {
        // Clear the global flag
        m_bAudioOnly = FALSE;
    }
    hr = m_pVideoWindow->get_Visible(&lVisible);
    if (FAILED(hr))
    {
        // If this is an audio-only clip, get_Visible() won't work.
        //
        // Also, if this video is encoded with an unsupported codec,
        // we won't see any video, although the audio will work if it is
        // of a supported format.
        //
        if (hr == E_NOINTERFACE)
        {
            m_bAudioOnly = TRUE;
        }
        else
        {
            Msg(TEXT("Failed(%08lx) in m_pVideoWindow->get_Visible()!\r\n"), hr);
        }
    }
}
//-----------------------------------------------------------------------------
void CDXPlayer::PauseClip(void)
{
    if (!m_pMediaControl)
    {
        return;
    }
    // Toggle play/pause behavior
    if((m_psCurrent == _PAUSED) || (m_psCurrent == _STOPPED))
    {
        if (SUCCEEDED(m_pMediaControl->Run()))
        {
            m_psCurrent = _RUNNING;
        }
    }
    else
    {
        if (SUCCEEDED(m_pMediaControl->Pause()))
        {
            m_psCurrent = _PAUSED;
        }
    }
    UpdateMainTitle();
}
//-----------------------------------------------------------------------------
void CDXPlayer::StopClip(void)
{
    HRESULT hr;
    if ((!m_pMediaControl) || (!m_pMediaSeeking))
    {
        return;
    }
    // Stop and reset postion to beginning
    if((m_psCurrent == _PAUSED) || (m_psCurrent == _RUNNING))
    {
        LONGLONG pos = 0;
        hr = m_pMediaControl->Stop();
        m_psCurrent = _STOPPED;
        // Seek to the beginning
        hr = m_pMediaSeeking->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                                           NULL, AM_SEEKING_NoPositioning);
        // Display the first frame to indicate the reset condition
        hr = m_pMediaControl->Pause();
    }
    UpdateMainTitle();
}
//-----------------------------------------------------------------------------
void CDXPlayer::OpenClip(TCHAR* pszFileName)
{
    HRESULT hr;
    // Reset status variables
    m_psCurrent = _STOPPED;
    m_lVolume = VOLUME_FULL;
    // Start playing the media file
    hr = PlayMovieInWindow(pszFileName);
    // If we couldn't play the clip, clean up
    if (FAILED(hr))
    {
        CloseClip();
    }
}
//-----------------------------------------------------------------------------
void CDXPlayer::CloseClip()
{
    HRESULT hr;
    // Stop media playback
    if(m_pMediaControl)
    {
        hr = m_pMediaControl->Stop();
    }
    // Clear global flags
    m_psCurrent = _STOPPED;
    m_bAudioOnly = TRUE;
    m_bFullscreen = FALSE;
    // Free DirectShow interfaces
    CloseInterfaces();
    // Clear file name to allow selection of new file with open dialog
    // m_szFileName[0] = L'\0';
    // No current media state
    m_psCurrent = _INIT;
    // Reset the player window
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    InvalidateRect(m_hWnd, &rect, TRUE);
    UpdateMainTitle();
}
//-----------------------------------------------------------------------------

void CDXPlayer::CloseInterfaces(void)
{
    HRESULT hr;
    // Relinquish ownership (IMPORTANT!) after hiding video window
    if(m_pVideoWindow)
    {
        hr = m_pVideoWindow->put_Visible(OAFALSE);
        hr = m_pVideoWindow->put_Owner(NULL);
    }
    // Disable event callbacks
    if (m_pMediaEventEx)
    {
        hr = m_pMediaEventEx->SetNotifyWindow((OAHWND)NULL, 0, 0);
    }
#ifdef REGISTER_FILTERGRAPH
    if (m_dwGraphRegister)
    {
        RemoveGraphFromRot(m_dwGraphRegister);
        m_dwGraphRegister = 0;
    }
#endif
    // Release and zero DirectShow interfaces
    SAFE_RELEASE(m_pMediaEventEx);
    SAFE_RELEASE(m_pMediaSeeking);
    SAFE_RELEASE(m_pMediaPosition);
    SAFE_RELEASE(m_pMediaControl);
    SAFE_RELEASE(m_pBaseAudio);
    SAFE_RELEASE(m_pBaseVidio);
    SAFE_RELEASE(m_pVideoWindow);
    SAFE_RELEASE(m_pVideoFrameStep);
    SAFE_RELEASE(m_pGraphBuilder);
}
//-----------------------------------------------------------------------------

#ifdef REGISTER_FILTERGRAPH
//-----------------------------------------------------------------------------
HRESULT CDXPlayer::AddGraphToRot(IUnknown* pUnkGraph, DWORD* pdwRegister)
{
    IMoniker* pMoniker;
    IRunningObjectTable* pROT;
    if (FAILED(GetRunningObjectTable(0, &pROT)))
    {
        return E_FAIL;
    }
    WCHAR wsz[128];
    wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph,
              GetCurrentProcessId());
    HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr))
    {
        hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
        pMoniker->Release();
    }
    pROT->Release();
    return hr;
}
//-----------------------------------------------------------------------------
void CDXPlayer::RemoveGraphFromRot(DWORD pdwRegister)
{
    IRunningObjectTable* pROT;
    if (SUCCEEDED(GetRunningObjectTable(0, &pROT)))
    {
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

void CDXPlayer::Msg(TCHAR* szFormat, ...)
{
    TCHAR szBuffer[512];  // Large buffer for very long filenames (like HTTP)
    // Format the input string
    va_list pArgs;
    va_start(pArgs, szFormat);
    _vstprintf(szBuffer, szFormat, pArgs);
    va_end(pArgs);
    // Display a message box with the formatted string
    MessageBox(NULL, szBuffer, TEXT("PlayWnd Sample"), MB_OK);
}
//-----------------------------------------------------------------------------

HRESULT CDXPlayer::ToggleMute(void)
{
    HRESULT hr = S_OK;
    if ((!m_pGraphBuilder) || (!m_pBaseAudio))
    {
        return S_OK;
    }
    // Read current volume
    hr = m_pBaseAudio->get_Volume(&m_lVolume);
    if (hr == E_NOTIMPL)
    {
        // Fail quietly if this is a video-only media file
        return S_OK;
    }
    else if (FAILED(hr))
    {
        Msg(TEXT("Failed to read audio volume!  hr=0x%x\r\n"), hr);
        return hr;
    }
    // Switch volume levels
    if (m_lVolume == VOLUME_FULL)
    {
        m_lVolume = VOLUME_SILENCE;
    }
    else
    {
        m_lVolume = VOLUME_FULL;
    }
    // Set new volume
    JIF(m_pBaseAudio->put_Volume(m_lVolume));
    UpdateMainTitle();
    return hr;
}
//-----------------------------------------------------------------------------
void CDXPlayer::UpdateMainTitle(void)
{
    /*
        TCHAR szTitle[MAX_PATH]={0}, szFile[MAX_PATH]={0};

        // If no file is loaded, just show the application title
        if (m_szFileName[0] == L'\0')
        {
            wsprintf(szTitle, TEXT("%s"), APPLICATIONNAME);
        }

        // Otherwise, show useful information
        else
        {
            // Get file name without full path
            GetFilename(m_szFileName, szFile);

            char szPlaybackRate[16];
            if (m_PlaybackRate == 1.0)
                szPlaybackRate[0] = '\0';
            else
                sprintf(szPlaybackRate, "(Rate:%2.2f)", m_PlaybackRate);

            TCHAR szRate[20];

    #ifdef UNICODE
            MultiByteToWideChar(CP_ACP, 0, szPlaybackRate, -1, szRate, 20);

    #else
            lstrcpy(szRate, szPlaybackRate);
    #endif

            // Update the window title to show filename and play state
            wsprintf(szTitle, TEXT("%s [%s] %s%s%s\0\0"),
                    szFile,
                    m_bAudioOnly ? TEXT("Audio\0") : TEXT("Video\0"),
                    (m_lVolume == VOLUME_SILENCE) ? TEXT("(Muted)\0") : TEXT("\0"),
                    (m_psCurrent == Paused) ? TEXT("(Paused)\0") : TEXT("\0"),
                    szRate);
        }

        SetWindowText(ghApp, szTitle);
    */
}
//-----------------------------------------------------------------------------

void CDXPlayer::GetFilename(TCHAR* pszFull, TCHAR* pszFile)
{
    int nLength;
    TCHAR szPath[MAX_PATH] = {0};
    BOOL bSetFilename = FALSE;
    // Strip path and return just the file's name
    _tcscpy(szPath, pszFull);
    nLength = (int) _tcslen(szPath);
    for (int i = nLength - 1; i >= 0; i--)
    {
        if ((szPath[i] == '\\') || (szPath[i] == '/'))
        {
            szPath[i] = '\0';
            lstrcpy(pszFile, &szPath[i+1]);
            bSetFilename = TRUE;
            break;
        }
    }
    // If there was no path given (just a file name), then
    // just copy the full path to the target path.
    if (!bSetFilename)
    {
        _tcscpy(pszFile, pszFull);
    }
}
//-----------------------------------------------------------------------------

HRESULT CDXPlayer::ToggleFullScreen(HWND hWnd)
{
    HRESULT hr = S_OK;
    /*
        LONG lMode;
        static HWND hDrain=0;

        // Don't bother with full-screen for audio-only files
        if ((m_bAudioOnly) || (!m_pVideoWindow))
            return S_OK;

        // Read current state
        JIF(m_pVideoWindow->get_FullScreenMode(&lMode));

        if (lMode == OAFALSE)
        {
            // Save current message drain
            LIF(m_pVideoWindow->get_MessageDrain((OAHWND *) &hDrain));

            // Set message drain to application main window
            LIF(m_pVideoWindow->put_MessageDrain((OAHWND) hWnd));

            // Switch to full-screen mode
            lMode = OATRUE;
            JIF(m_pVideoWindow->put_FullScreenMode(lMode));
            m_bFullscreen = TRUE;
        }
        else
        {
            // Switch back to windowed mode
            lMode = OAFALSE;
            JIF(m_pVideoWindow->put_FullScreenMode(lMode));

            // Undo change of message drain
            LIF(m_pVideoWindow->put_MessageDrain((OAHWND) hDrain));

            // Reset video window
            LIF(m_pVideoWindow->SetWindowForeground(-1));

            // Reclaim keyboard focus for player application
            UpdateWindow(ghApp);
            SetForegroundWindow(ghApp);
            SetFocus(ghApp);
            m_bFullscreen = FALSE;
        }
    */
    return hr;
}
//-----------------------------------------------------------------------------
//
// Some video renderers support stepping media frame by frame with the
// IVideoFrameStep interface.  See the interface documentation for more
// details on frame stepping.
//
//-----------------------------------------------------------------------------
BOOL CDXPlayer::GetFrameStepInterface(void)
{
    HRESULT hr;
    IVideoFrameStep* pFSTest = NULL;
    // Get the frame step interface, if supported
    hr = m_pGraphBuilder->QueryInterface(__uuidof(IVideoFrameStep), (PVOID*)&pFSTest);
    if (FAILED(hr))
    {
        return FALSE;
    }
    // Check if this decoder can step
    hr = pFSTest->CanStep(0L, NULL);
    if (hr == S_OK)
    {
        m_pVideoFrameStep = pFSTest;  // Save interface to global variable for later use
        return TRUE;
    }
    else
    {
        pFSTest->Release();
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
HRESULT CDXPlayer::StepOneFrame(void)
{
    HRESULT hr = S_OK;
    // If the Frame Stepping interface exists, use it to step one frame
    if (m_pVideoFrameStep)
    {
        // The graph must be paused for frame stepping to work
        if (m_psCurrent != State_Paused)
        {
            PauseClip();
        }
        // Step the requested number of frames, if supported
        hr = m_pVideoFrameStep->Step(1, NULL);
    }
    return hr;
}
//-----------------------------------------------------------------------------
HRESULT CDXPlayer::StepFrames(int nFramesToStep)
{
    HRESULT hr = S_OK;
    // If the Frame Stepping interface exists, use it to step frames
    if (m_pVideoFrameStep)
    {
        // The renderer may not support frame stepping for more than one
        // frame at a time, so check for support.  S_OK indicates that the
        // renderer can step nFramesToStep successfully.
        if ((hr = m_pVideoFrameStep->CanStep(nFramesToStep, NULL)) == S_OK)
        {
            // The graph must be paused for frame stepping to work
            if (m_psCurrent != State_Paused)
            {
                PauseClip();
            }
            // Step the requested number of frames, if supported
            hr = m_pVideoFrameStep->Step(nFramesToStep, NULL);
        }
    }
    return hr;
}
//-----------------------------------------------------------------------------
HRESULT CDXPlayer::ModifyRate(double dRateAdjust)
{
    HRESULT hr = S_OK;
    double dRate;
    // If the IMediaPosition interface exists, use it to set rate
    if ((m_pMediaPosition) && (dRateAdjust != 0))
    {
        if ((hr = m_pMediaPosition->get_Rate(&dRate)) == S_OK)
        {
            // Add current rate to adjustment value
            double dNewRate = dRate + dRateAdjust;
            hr = m_pMediaPosition->put_Rate(dNewRate);
            // Save global rate
            if (SUCCEEDED(hr))
            {
                m_PlaybackRate = dNewRate;
                UpdateMainTitle();
            }
        }
    }
    return hr;
}
//-----------------------------------------------------------------------------
HRESULT CDXPlayer::SetRate(double dRate)
{
    HRESULT hr = S_OK;
    // If the IMediaPosition interface exists, use it to set rate
    if (m_pMediaPosition)
    {
        hr = m_pMediaPosition->put_Rate(dRate);
        // Save global rate
        if (SUCCEEDED(hr))
        {
            m_PlaybackRate = dRate;
            UpdateMainTitle();
        }
    }
    return hr;
}
//-----------------------------------------------------------------------------
HRESULT CDXPlayer::HandleGraphEvent(void)
{
    LONG evCode, evParam1, evParam2;
    HRESULT hr = S_OK;
    // Make sure that we don't access the media event interface
    // after it has already been released.
    if (!m_pMediaEventEx)
    {
        return S_OK;
    }
    // Process all queued events
    while(SUCCEEDED(m_pMediaEventEx->GetEvent(&evCode, (LONG_PTR*) &evParam1,
                    (LONG_PTR*) &evParam2, 0)))
    {
        // Free memory associated with callback, since we're not using it
        hr = m_pMediaEventEx->FreeEventParams(evCode, evParam1, evParam2);
        // If this is the end of the clip, reset to beginning
        if(EC_COMPLETE == evCode)
        {
            LONGLONG pos = 0;
            // Reset to first frame of movie
            hr = m_pMediaSeeking->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                                               NULL, AM_SEEKING_NoPositioning);
            if (FAILED(hr))
            {
                // Some custom filters (like the Windows CE MIDI filter)
                // may not implement seeking interfaces (IMediaSeeking)
                // to allow seeking to the start.  In that case, just stop
                // and restart for the same effect.  This should not be
                // necessary in most cases.
                if (FAILED(hr = m_pMediaControl->Stop()))
                {
                    Msg(TEXT("Failed(0x%08lx) to stop media clip!\r\n"), hr);
                    break;
                }
                if (FAILED(hr = m_pMediaControl->Run()))
                {
                    Msg(TEXT("Failed(0x%08lx) to reset media clip!\r\n"), hr);
                    break;
                }
            }
        }
    }
    return hr;
}
//-----------------------------------------------------------------------------
void CDXPlayer::SetPlayerWindow(HWND hWnd)
{
    m_hWnd = hWnd;
}
//-----------------------------------------------------------------------------
