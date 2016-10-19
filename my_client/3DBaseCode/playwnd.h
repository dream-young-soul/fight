
//**********************************************************\\
// ´úÂë±à¼­Æ÷\\
//**********************************************************\\
\\
//------------------------------------------------------------------------------
// File: PlayWnd.h
//
// Desc: DirectShow sample code - header file for video in window movie
//       player application.
//
// Copyright (c) 1998-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------
#ifndef _DXPLAYER_H
#define _DXPLAYER_H
//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
#define VOLUME_FULL     0L
#define VOLUME_SILENCE  -10000L
// Defaults used with audio-only files
#define DEFAULT_AUDIO_WIDTH     240
#define DEFAULT_AUDIO_HEIGHT    120
#define DEFAULT_VIDEO_WIDTH     320
#define DEFAULT_VIDEO_HEIGHT    240
#define MINIMUM_VIDEO_WIDTH     200
#define MINIMUM_VIDEO_HEIGHT    120

#define WM_GRAPHNOTIFY  WM_USER+13

enum PLAYSTATE { _STOPPED, _PAUSED, _RUNNING, _INIT };
//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

#define JIF(x) if (FAILED(hr=(x))) \
    {Msg(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n"), hr); return hr;}

#define LIF(x) if (FAILED(hr=(x))) \
    {Msg(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n"), hr);}

//------------------------------------------------------------------------------
struct IGraphBuilder;
struct IMediaControl;
struct IMediaEventEx;
struct IVideoWindow;
struct IBasicAudio;
struct IBasicVideo;
struct IMediaSeeking;
struct IMediaPosition;
struct IVideoFrameStep;
//------------------------------------------------------------------------------

class CDXPlayer
{
public:
    CDXPlayer();
    ~CDXPlayer();
    BOOL IsRepeat() { return m_bRepeat; }
    void SetRepeat(BOOL bRepeat) { m_bRepeat = bRepeat; }
    void SetPlayerWindow(HWND hWnd);
    void OpenClip(TCHAR* pszFileName);
    HRESULT HandleGraphEvent();
private:
    HRESULT InitVideoWindow(int nMultiplier, int nDivider);
    HRESULT StepOneFrame();
    HRESULT StepFrames(int nFramesToStep);
    HRESULT ModifyRate(double dRateAdjust);
    HRESULT SetRate(double dRate);

    HRESULT PlayMovieInWindow(LPTSTR szFile);
    HRESULT ToggleMute();
    HRESULT ToggleFullScreen(HWND hWnd);

    BOOL GetFrameStepInterface();

    void PaintAudioWindow();
    void MoveVideoWindow(HWND hWnd);
    void CheckVisibility();
    void CloseInterfaces();

    void PauseClip();
    void StopClip();
    void CloseClip();

    void UpdateMainTitle();
    void EnablePlaybackMenu(BOOL bEnable);
    void GetFilename(TCHAR* pszFull, TCHAR* pszFile);
    void Msg(TCHAR* szFormat, ...);

    HRESULT AddGraphToRot(IUnknown* pUnkGraph, DWORD* pdwRegister);
    void RemoveGraphFromRot(DWORD pdwRegister);
private:
    HWND m_hWnd;
    // DirectShow interfaces
    IGraphBuilder*	m_pGraphBuilder;
    IMediaControl*	m_pMediaControl;
    IMediaEventEx*	m_pMediaEventEx;
    IVideoWindow*	m_pVideoWindow;
    IBasicAudio*	m_pBaseAudio;
    IBasicVideo*	m_pBaseVidio;
    IMediaSeeking*	m_pMediaSeeking;
    IMediaPosition*	m_pMediaPosition;
    IVideoFrameStep* m_pVideoFrameStep;
    // controls
    BOOL m_bRepeat;
    BOOL m_bAudioOnly;
    BOOL m_bFullscreen;
    LONG m_lVolume;
    DWORD m_dwGraphRegister;
    PLAYSTATE	m_psCurrent;
    double m_PlaybackRate;
};
//------------------------------------------------------------------------------
extern CDXPlayer g_objMediaPlayer;
//------------------------------------------------------------------------------
#endif
