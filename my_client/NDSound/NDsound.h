
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************


//-----------------------------------------------------------------------------
#ifndef NDSOUND_H
#define NDSOUND_H

#include <windows.h>
#include <deque>
#include "dsutil.h"
using namespace std;
class CSound;
class CMedia;
class C3DSound;




typedef deque<CSound*> dequeSound;
typedef deque<CMedia*> dequeMedia;
typedef deque<C3DSound*> deque3DSound;
//-----------------------------------------------------------------------------------
const int SETPRIMARYBUFFERFORMATERR = -3;
const int SETCOOPERACTIVELEVELERR	= -2;
const int DIRECTSOUNDCREATEERR	= -1;
const int INITOK		= 0;

class CNDsound : public CSoundManager
{
public:
    CNDsound() {m_binitOK = 0; m_hWnd = NULL; m_dwWorldX = 0; m_dwWorldY = 0; m_nSoundVolume = 80; m_nMusicVolume = 80;}
    int  init(HWND hWnd);
    ~CNDsound() {Destroy();  CoUninitialize();}
    void	Destroy();

    CSound* PlaySound(char* szFileName, int nWorldX = 0, int nWorldY = 0, int nRange = 0, int nLoop = 0, int nVolume = 100);
    C3DSound* Play3DSound(HWND hwnd, char* szFileName, int nWorldX = 0, int nWorldY = 0, int nWorldZ = 0 , int nRange = 100, int nLoop = 0);
    CMedia* PlayMedia(HWND hwnd , char* szFileName, int nLoop = 0);

    void	ProcessSound(int nWorldX, int nWorldY, int nDir);
    BOOL	Close3DSound(char* szFileName, int nX, int nY);
    BOOL	CloseSound(char* szFileName, int nX, int nY);
    BOOL	CloseSound(CSound* psound);
    BOOL	CloseSound(C3DSound* psound);
    void	CloseAllSound();
    void    CloseAllMedia();
    void	SetSoundPos(CSound* psound, int nX, int nY);
    void	SetSoundPos(C3DSound* psound, int nX, int nY, int nZ);
    void	SetVolume(char* szFileName, int nVolume, int nWorldX = 0, int nWorldY = 0);
    BOOL	CloseMedia(char* szFileName);
    void	Pause();
    void	Continue();
    const	CSound*  GetSound(char* szFileName, int nX, int nY);
    const	C3DSound* Get3DSound(char* szFileName, int nX, int nY);

    void	SetMusicVolume(int nVolume);
    void	SetSoundVolume(int nVolume);
    int		GetMusicVolume() {return m_nMusicVolume;}
    int		GetSoundVolume() {return m_nSoundVolume;}
    BOOL	Check2DSound(CSound* pSound);
    HWND		m_hWnd;

private:
    dequeSound  m_DequepSound;
    dequeMedia  m_DequepMedia;
    deque3DSound m_Dequep3DSound;
    int			m_nMusicVolume;
    int			m_nSoundVolume;
    bool		m_binitOK;
    DWORD		m_dwWorldX, m_dwWorldY;
};




#ifdef NDSOUND_EXPORTS
#define DLLAPI extern "C" _declspec(dllexport)
#else
#define DLLAPI extern "C" _declspec(dllimport)
#endif



DLLAPI int   __stdcall DXSoundinit(HWND hWnd);

DLLAPI const CSound* __stdcall DXPlaySound(char* szFileName, int nWorldX = 0, int nWorldY = 0, int nRange = 0, int nLoop = 0, int nVolume = 100);
DLLAPI const C3DSound* __stdcall DXPlay3DSound(char* szFileName, int nWorldX = 0, int nWorldY = 0, int nWorldZ = 0, int nRange = 100, int nLoop = 0);
DLLAPI const CMedia* __stdcall DXPlayMedia(char* szFileName, int nLoop = 0);

DLLAPI void		__stdcall DXProcessSound(int nWorldX, int nWorldY);

DLLAPI	BOOL	__stdcall DXCloseSound(char* szFileName, int nX = 0, int nY = 0);
DLLAPI	BOOL	__stdcall DXClose2DSound(CSound* pSound);
DLLAPI	BOOL	__stdcall DXClose3DSound(C3DSound* pSound);
DLLAPI  void	__stdcall DXCloseAllSound();
DLLAPI  void	__stdcall DXCloseAllMedia();
DLLAPI  void	__stdcall DXSet3DSoundPos(C3DSound* pSound, int nX, int nY, int nZ);
DLLAPI  void	__stdcall DXSet2DSoundPos(CSound* pSound, int nX, int nY);
DLLAPI  void	__stdcall DXSetVolume(char* szFileName, int nVolume, int nWorldX = 0, int nWorldY = 0);


DLLAPI	BOOL	__stdcall DXCloseMedia(char* szFileName);
DLLAPI	void	__stdcall DXPause();
DLLAPI	void	__stdcall DXContinue();

DLLAPI	const CSound* 	__stdcall	DXGetSound(char* szFileName, int nX, int nY);
DLLAPI	const C3DSound* __stdcall	DXGet3DSound(char* szFileName, int nX, int nY);

DLLAPI	void	__stdcall DXSetMusicVolume(int nVolume);
DLLAPI	void	__stdcall DXSetSoundVolume(int nVolume);
DLLAPI	int		__stdcall DXGetMusicVolume();
DLLAPI	int		__stdcall DXGetSoundVolume();

DLLAPI	void	__stdcall DXDestroy();
DLLAPI	BOOL	__stdcall Check2DSound(CSound* pSound);

#endif
