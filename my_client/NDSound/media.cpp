
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************


#include <tchar.h>
#include <windows.h>
#include <dshow.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <tchar.h>
#include "media.h"
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

void CMedia::SetVideoWindow(int nTop, int nLeft)
{
    LONG lHeight, lWidth;
    RECT TarRect;
    if (!pBV)
    {
        return  ;
    }
    //  Read the default video size
    pBV->GetVideoSize(&lWidth, &lHeight);
    TarRect.top		= nTop;
    TarRect.left	= nLeft;
    TarRect.right	= TarRect.left  + lWidth;
    TarRect.bottom	= TarRect.top  + lHeight;
    pVW->SetWindowPosition(TarRect.left, TarRect.top, TarRect.right,
                           TarRect.bottom);
    return  ;
}





void CMedia::CheckVisibility(void)
{
    long lVisible;
    HRESULT hr;
    m_bAudioOnly = FALSE;
    if ((!pVW) || (!pBV))
    {
        m_bAudioOnly = TRUE;
        return;
    }
    hr = pVW->get_Visible(&lVisible);
    if (FAILED(hr))
    {
        // If this is an audio-only clip, get_Visible() won't work.
        //
        // Also, if this video is encoded with an unsupported codec,
        // we won't see any video, although the audio will work if it
        // of a supported format.
        //
        if (hr == E_NOINTERFACE)
        {
            m_bAudioOnly = TRUE;
        }
    }
}


void CMedia::Pause(void)
{
    HRESULT hr;
    if (!pMC)
    {
        return;
    }
    // Toggle play/pause behavior
    if ((m_psCurrent == Paused) || (m_psCurrent == Stopped))
    {
        hr = pMC->Run();
        m_psCurrent = Running;
    }
    else
    {
        hr = pMC->Pause();
        m_psCurrent = Paused;
    }
}


void CMedia::Stop(void)
{
    HRESULT hr;
    if ((!pMC) || (!pMS))
    {
        return;
    }
    // Stop and reset postion to beginning
    if ((m_psCurrent == Paused) || (m_psCurrent == Running))
    {
        LONGLONG pos = 0;
        hr = pMC->Stop();
        m_psCurrent = Stopped;
        hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                               NULL, AM_SEEKING_NoPositioning);
        // Display the first frame to indicate the reset condition
        hr = pMC->Pause();
    }
}


BOOL CMedia::PlayMedia(HWND hwnd, char* szFileName, int nloop)
{
    HRESULT hr;
    m_hApp = hwnd;
    m_iLoopTime = nloop;
    lstrcpy(m_szFileName, szFileName);
    // Reset status variables
    m_psCurrent = Stopped;
    m_lVolume = 100;
    hr = PlayMovieInWindow(m_szFileName);
    // If we couldn't play the clip, clean up
    if (FAILED(hr))
    {
        Close();
        return 0;
    }
    return 1;
}



void CMedia::Close()
{
    HRESULT hr;
    if (pMC)
    {
        hr = pMC->Stop();
    }
    m_psCurrent = Stopped;
    m_bAudioOnly = TRUE;
    CloseInterfaces();
    // Clear file name to allow selection of new file with open dialog
    m_szFileName[0] = L'\0';
    // No current media state
    m_psCurrent = Init;
}


void CMedia::CloseInterfaces(void)
{
    HRESULT hr;
    // Relinquish ownership (IMPORTANT!) after hiding video window
    if (pVW)
    {
        hr = pVW->put_Visible(OAFALSE);
        hr = pVW->put_Owner(NULL);
    }
#ifdef REGISTER_FILTERGRAPH
    if (g_dwGraphRegister)
    {
        RemoveGraphFromRot(g_dwGraphRegister);
        g_dwGraphRegister = 0;
    }
#endif
    SAFE_RELEASE(pME);
    SAFE_RELEASE(pMS);
    SAFE_RELEASE(pMC);
    SAFE_RELEASE(pBA);
    SAFE_RELEASE(pBV);
    SAFE_RELEASE(pVW);
    SAFE_RELEASE(pGB);
}




int  CMedia::SetVolume(int nVolume)//0-100
{
    //	#define VOLUME_MAX     0L
    //	#define VOLUME_MIN  -3000L
    HRESULT hr = S_OK;
    if ((!pGB) || (!pBA))
    {
        return 0;
    }
    // Read current volume
    hr = pBA->get_Volume(&m_lVolume);
    if (hr == E_NOTIMPL)
    {
        // Fail quietly if this is a video-only media file
        return -1;
    }
    else if (FAILED(hr))
    {
        return -1;
    }
    int nVol = (nVolume - 100) * 30 ;
    m_lVolume = nVolume;
    pBA->put_Volume(nVol);
    return hr;
}

int CMedia::GetVolume()
{
    return m_lVolume;
}






BOOL CMedia::IsPlaying(void)
{
    LONG evCode, evParam1, evParam2;
    HRESULT hr = S_OK;
    while(SUCCEEDED(pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))
    {
        // Spin through the events
        hr = pME->FreeEventParams(evCode, evParam1, evParam2);
        if (EC_COMPLETE == evCode)
        {
            if (m_iLoopTime != 0)
            {
                LONGLONG pos = 0;
                // Reset to first frame of movie
                hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning , NULL, AM_SEEKING_NoPositioning);
                if (FAILED(hr))
                {
                    if (FAILED(hr = pMC->Stop()))
                    {
                        return 0;
                    }
                    if (FAILED(hr = pMC->Run()))
                    {
                        return 1;
                    }
                }
                if (m_iLoopTime != -1)
                {
                    m_iLoopTime--;
                }
            }
            else
            {
                return 0;
            }
        }
    }
    return 1;
}





HRESULT CMedia::PlayMovieInWindow(char* szFile)
{
    WCHAR wFile[MAX_PATH];
    HRESULT hr = 0;
    // Clear open dialog remnants before calling RenderFile()
    UpdateWindow(m_hApp);
#ifndef UNICODE
    MultiByteToWideChar(CP_ACP, 0, szFile, -1, wFile, MAX_PATH);
#else
    lstrcpy(wFile, szFile);
#endif
    // Get the interface for DirectShow's GraphBuilder
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGB);
    if (SUCCEEDED(hr))
    {
        // Have the graph construct its the appropriate graph
        pGB->RenderFile(wFile, NULL);
        // QueryInterface for DirectShow interfaces
        pGB->QueryInterface(IID_IMediaControl, (void**)&pMC);
        pGB->QueryInterface(IID_IMediaEventEx, (void**)&pME);
        pGB->QueryInterface(IID_IMediaSeeking, (void**)&pMS);
        // Query for video interfaces, which may not be relevant for audio files
        pGB->QueryInterface(IID_IVideoWindow, (void**)&pVW);
        pGB->QueryInterface(IID_IBasicVideo, (void**)&pBV);
        // Query for audio interfaces, which may not be relevant for video-only files
        pGB->QueryInterface(IID_IBasicAudio, (void**)&pBA);
        // Is this an audio-only file (no video component)?
        CheckVisibility();
        if (!m_bAudioOnly)
        {
            pVW->put_Owner((OAHWND)m_hApp);
            pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        }
        // Let's get ready to rumble!
        //   ShowWindow(m_hApp, SW_SHOWNORMAL);
        UpdateWindow(m_hApp);
        SetForegroundWindow(m_hApp);
        SetFocus(m_hApp);
        // Run the graph to play the media file
        pMC->Run();
        m_psCurrent = Running;
        SetFocus(m_hApp);
    }
    return hr;
}

