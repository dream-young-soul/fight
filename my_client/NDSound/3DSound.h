
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include <dshow.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <tchar.h>
#include "dsutil.h"
#include "dmutil.h"


#define ORBIT_MAX_RADIUS        5.0f
#define IDT_MOVEMENT_TIMER      1


class C3DSound
{
private:
    CMusicManager*          m_pMusicManager ;
    CMusicSegment*          m_pMusicSegment ;
    IDirectMusicAudioPath*  m_p3DAudioPath  ;
    IDirectSound3DBuffer*   m_pDS3DBuffer   ;   // 3D sound buffer
    DS3DBUFFER              m_dsBufferParams;                  // 3D buffer properties
    HINSTANCE               m_hInst ;
    BOOL                    m_bAllowMovementTimer ;
    IDirectSound3DListener* m_pDSListener ;    // 3D listener object
    DS3DLISTENER            m_dsListenerParams;                // Listener properties
    int						m_nBeginTime;

    int m_nPosX;
    int m_nPosY;
    int m_nPosZ;
    int m_nVolume;//1-100

    VOID Set3DParameters( FLOAT fDopplerFactor, FLOAT fRolloffFactor, FLOAT fMinDistance,   FLOAT fMaxDistance );
    HRESULT LoadSegmentFile( HWND hDlg, TCHAR* strFileName );


public:
    TCHAR   m_szFileName[MAX_PATH];
    DWORD	m_dwSleepTime;


public:
    C3DSound() ;
    ~C3DSound() {Close();}
    HRESULT InitAudio( HWND hDlg , TCHAR* strFileName , int nRange);
    BOOL	PlaySound(HWND hwnd , char* szFileName, int nWorldX , int nWorldY, int nWorldZ, int nRange = 100, int nloop = 0);
    void	Close(void);
    void	Pause(void);
    void	Continue(void);
    void	Stop(void);
    int		GetVolume();
    void	SetVolume(int nVolume);
    BOOL	IsPlaying(void);
    BOOL	IsBeginning();
    int		GetWorldX() {return m_nPosX;}
    int		GetWorldY() {return m_nPosY;}
    int		GetWorldZ() {return m_nPosZ;}
    void SetSoundPos(int nX, int nY, int nZ);
    void ResetOrigPos(int nX, int nY, int nZ);
    BOOL RePlay(int nWorldX, int nWorldY, int nWorldZ, int nRange = 100, int nLoop = 0);
};
