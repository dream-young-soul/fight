
//**********************************************************
// 代码编辑器
//**********************************************************

#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include <DxErr8.h>
#include <dsound.h>
#include <math.h>
#include "DSUtil.h"
#include "NDsound.h"
#include "media.h"
#include "3dSound.h"

const	DWORD  DSOUND_3D_SOUND		= 0x00000008;       //３D音乐带有声音的Ｘ,Ｙ,ＲＡＮＧＥ
CNDsound sound;

int  CNDsound::init(HWND hWnd)
{
    if (m_binitOK)
    {
        return 0;
    }
    m_hWnd = hWnd;
    m_nMusicVolume = 80;
    m_nSoundVolume = 80;
    CoInitialize(NULL);
    int nRet = Initialize( hWnd, DSSCL_NORMAL, 2, 22050, 16 );
    switch(nRet)
    {
    case -1:
    case -2:
    case -3:
        m_binitOK = 0;
        return nRet;
    case S_OK:
        m_binitOK = 1;
        return 0;
    }
    return -1;
}

void CNDsound::Destroy()
{
    if (!m_binitOK)
    {
        return;
    }
    DWORD i;
    for(i = 0; i < m_DequepSound.size(); i++)
    {
        SAFE_DELETE( m_DequepSound[i]);
    }
    for(i = 0; i < m_DequepMedia.size(); i++)
    {
        SAFE_DELETE( m_DequepMedia[i]);
    }
    for(i = 0; i < m_Dequep3DSound.size(); i++)
    {
        SAFE_DELETE( m_Dequep3DSound[i]);
    }
    m_DequepSound.clear();
    m_DequepMedia.clear();
    m_Dequep3DSound.clear();
}


void CNDsound::Pause()
{
    if (!m_binitOK)
    {
        return;
    }
    DWORD i;
    for(i = 0; i < m_DequepMedia.size(); i++)
    {
        if (m_DequepMedia[i])
        {
            m_DequepMedia[i]->Pause();
        }
    }
    for(i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
        {
            m_DequepSound[i]->Stop();
        }
    }
    for(i = 0; i < m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
        {
            m_Dequep3DSound[i]->Stop();
        }
    }
}

void CNDsound::Continue()
{
    if (!m_binitOK)
    {
        return;
    }
    DWORD i;
    for(i = 0; i < m_DequepMedia.size(); i++)
    {
        if (m_DequepMedia[i])
        {
            m_DequepMedia[i]->Pause();
        }
    }
    for(  i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
        {
            m_DequepSound[i]->Continue();
        }
    }
    for(  i = 0; i < m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
        {
            m_Dequep3DSound[i]->Continue();
        }
    }
}
void CNDsound::SetMusicVolume(int nVolume)
{
    if (!m_binitOK)
    {
        return;
    }
    m_nMusicVolume = nVolume;
    DWORD i;
    for(i = 0; i < m_DequepMedia.size(); i++)
    {
        if (m_DequepMedia[i])
        {
            m_DequepMedia[i]->SetVolume(nVolume);
        }
    }
}

void CNDsound::SetSoundVolume(int nVolume)
{
    if (!m_binitOK)
    {
        return;
    }
    if (nVolume > 100)
    {
        nVolume = 100;
    }
    else if (nVolume < 0)
    {
        nVolume = 0;
    }
    m_nSoundVolume = nVolume;
    for(DWORD i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
        {
            m_DequepSound[i]->SetVolume(m_nSoundVolume);
        }
    }
    for(int i = 0; i < (int)m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
        {
            m_Dequep3DSound[i]->SetVolume(m_nSoundVolume);
        }
    }
}


BOOL CNDsound::CloseMedia(char* szFileName)
{
    if (!m_binitOK)
    {
        return 0;
    }
    for(DWORD i = 0; i < m_DequepMedia.size(); i++)
    {
        if (m_DequepSound[i])
        {
            if (strcmp(m_DequepMedia[i]->m_szFileName , szFileName) == 0)
            {
                SAFE_DELETE( m_DequepMedia[i]);
                m_DequepMedia.erase(m_DequepMedia.begin() + i);
                return 1;
            }
        }
    }
    return 0;
}

void CNDsound::SetVolume(char* szFileName, int nVolume, int nWorldX , int nWorldY)
{
    if (!m_binitOK)
    {
        return ;
    }
    for(DWORD i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
        {
            if (strcmp(m_DequepSound[i]->m_szFileName , szFileName) == 0)
                if (nWorldX || nWorldY)
                {
                    if (m_DequepSound[i]->m_dwControl & DSOUND_3D_SOUND)
                    {
                        if (m_DequepSound[i]->m_nWorldX == nWorldX && m_DequepSound[i]->m_nWorldY == nWorldY)
                        {
                            m_DequepSound[i]->SetSelfVolume(nVolume) ;
                            m_DequepSound[i]->SetVolume(m_nSoundVolume);
                            return ;
                        }
                    }
                }
                else
                {
                    m_DequepSound[i]->SetSelfVolume(nVolume) ;
                    m_DequepSound[i]->SetVolume(m_nSoundVolume);
                    return ;
                }
        }
    }
}

BOOL CNDsound::CloseSound(char* szFileName, int nX, int nY)
{
    if (!m_binitOK)
    {
        return 0;
    }
    for(DWORD i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
        {
            if (strcmp(m_DequepSound[i]->m_szFileName , szFileName) == 0)
                if (nX || nY)
                {
                    if (m_DequepSound[i]->m_dwControl & DSOUND_3D_SOUND)
                    {
                        if (m_DequepSound[i]->m_nWorldX == nX && m_DequepSound[i]->m_nWorldY == nY)
                        {
                            SAFE_DELETE( m_DequepSound[i]);
                            m_DequepSound.erase(m_DequepSound.begin() + i);
                            return 1;
                        }
                    }
                }
                else
                {
                    SAFE_DELETE( m_DequepSound[i]);
                    m_DequepSound.erase(m_DequepSound.begin() + i);
                    return 1;
                }
        }
    }
    return 0;
}

BOOL CNDsound::Close3DSound(char* szFileName, int nX, int nY)
{
    if (!m_binitOK)
    {
        return 0;
    }
    for(DWORD i = 0; i < m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
        {
            if (strcmp(m_Dequep3DSound[i]->m_szFileName , szFileName) == 0)
                if (m_Dequep3DSound[i]->GetWorldX() == nX && m_Dequep3DSound[i]->GetWorldZ() == nY)
                {
                    SAFE_DELETE( m_Dequep3DSound[i]);
                    m_Dequep3DSound.erase(m_Dequep3DSound.begin() + i);
                    return 1;
                }
        }
    }
    return 0;
}

const CSound* CNDsound::GetSound(char* szFileName, int nX, int nY)
{
    if (!m_binitOK)
    {
        return NULL;
    }
    for(DWORD i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
        {
            if (strcmp(m_DequepSound[i]->m_szFileName, szFileName) == 0)
                if (nX || nY)
                {
                    if (m_DequepSound[i]->m_dwControl & DSOUND_3D_SOUND)
                    {
                        if (m_DequepSound[i]->m_nWorldX == nX && m_DequepSound[i]->m_nWorldY == nY)
                        {
                            return m_DequepSound[i];
                        }
                    }
                }
                else
                {
                    return m_DequepSound[i];
                }
        }
    }
    return NULL;
}


const C3DSound* CNDsound::Get3DSound(char* szFileName, int nX, int nY)
{
    if (!m_binitOK)
    {
        return NULL;
    }
    for(DWORD i = 0; i < m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
        {
            if (strcmp(m_Dequep3DSound[i]->m_szFileName, szFileName) == 0)
                if (nX || nY)
                    if (m_Dequep3DSound[i]->GetWorldX() == nX && m_Dequep3DSound[i]->GetWorldZ() == nY)
                    {
                        return m_Dequep3DSound[i];
                    }
        }
    }
    return NULL;
}

BOOL	CNDsound::CloseSound(CSound* psound)
{
    if (!m_binitOK)
    {
        return 0;
    }
    for(DWORD i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
            if (m_DequepSound[i] == psound)
            {
                SAFE_DELETE( m_DequepSound[i]);
                m_DequepSound.erase(m_DequepSound.begin() + i);
                return 1;
            }
    }
    return 0;
}
BOOL	CNDsound::CloseSound(C3DSound* psound)
{
    if (!m_binitOK)
    {
        return 0;
    }
    for(DWORD i = 0; i < m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
            if (m_Dequep3DSound[i] == psound)
            {
                SAFE_DELETE( m_Dequep3DSound[i]);
                m_Dequep3DSound.erase(m_Dequep3DSound.begin() + i);
                return 1;
            }
    }
    return 0;
}

void	CNDsound::SetSoundPos(CSound* psound , int nX, int nY)
{
    if (!m_binitOK)
    {
        return ;
    }
    for(DWORD i = 0; i < m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
            if (m_DequepSound[i] == psound)
            {
                psound->SetSoundPos(nX, nY, psound->m_nRange);
                return ;
            }
    }
}

void	CNDsound::SetSoundPos(C3DSound* psound, int nX, int nY, int nZ)
{
    if (!m_binitOK)
    {
        return ;
    }
    for(DWORD i = 0; i < m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
            if (m_Dequep3DSound[i] == psound)
            {
                psound->ResetOrigPos(nX, nY, nZ);
                return ;
            }
    }
}

CMedia* CNDsound::PlayMedia(HWND hwnd , char* szFileName, int nLoop)
{
    if (!m_binitOK)
    {
        return NULL;
    }
    CMedia* pMedia;
    pMedia = new CMedia;
    if (!pMedia->PlayMedia(hwnd, szFileName, nLoop))
    {
        return NULL;
    }
    pMedia->SetVolume(m_nMusicVolume);
    m_DequepMedia.push_back(pMedia);
    return pMedia;
}

C3DSound* CNDsound::Play3DSound(HWND hwnd, char* szFileName, int nWorldX , int nWorldY, int nWorldZ, int nRange, int nLoop)
{
    if (!m_binitOK)
    {
        return NULL;
    }
    if (!szFileName || strcmp(szFileName, "") == 0)
    {
        return NULL;
    }
    for(int i = 0; i < (int)m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i]->m_dwSleepTime)
            if (strcmp(m_Dequep3DSound[i]->m_szFileName, szFileName) == 0)
            {
                m_Dequep3DSound[i]->RePlay(nWorldX, nWorldY, nWorldZ, nRange, nLoop);
                return m_Dequep3DSound[i];
            }
    }
    C3DSound* p3dSound = new C3DSound();
    if (!p3dSound)
    {
        return NULL;
    }
    p3dSound->PlaySound(hwnd, szFileName, nWorldX, nWorldY, nWorldZ, nRange, nLoop);
    m_Dequep3DSound.push_back(p3dSound);
    return p3dSound;
}

CSound* CNDsound::PlaySound(char* szFileName, int nWorldX, int nWorldY, int nRange, int nLoop, int nVolume)
{
    if (!m_binitOK)
    {
        return NULL;
    }
    if (!szFileName || strcmp(szFileName, " ") == 0 ||
		strcmp(szFileName,"NULL") == 0)
    {
        return NULL;
    }
	
    int i;
    char szExt[10];
    for(i = strlen(szFileName) - 1; i >= 0; i--)
    {
        if (szFileName[i] == '.')
        {
            strcpy(szExt, &szFileName[i]);
        }
    }
    _strupr(szExt);
    if (strcmp(szExt, ".WAV") != 0)
    {
        return NULL;    // not wav file
    }
    if ((m_nSoundVolume < 20 || nVolume < 20)  && nLoop != -1)
    {
        return NULL;
    }
    try
    {
        if ( nLoop != -1 ) // //!nRange ||  nLoop!=-1
        {
            for(i = 0; i < (int)m_DequepSound.size(); i++)
            {
                if (m_DequepSound[i])
                    if (strcmp(m_DequepSound[i]->m_szFileName, szFileName) == 0)
                    {
                        //如果附近有相同的音效，则忽略
                        if (m_DequepSound[i]->m_dwSleepTime == 0 )
                        {
                            if ( abs(nWorldX - m_DequepSound[i]->m_nWorldX ) < 300 && abs(nWorldY - m_DequepSound[i]->m_nWorldY ) < 300 )
                            {
                                return NULL;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            DWORD dwFlag = 0, dwControl = 0;
                            if (nRange)
                            {
                                dwControl |= DSOUND_3D_SOUND;
                            }
                            int iVolume = m_nSoundVolume;
                            if (dwControl & DSOUND_3D_SOUND)
                            {
                                int ndisX = m_dwWorldX - nWorldX;
                                int ndisY = m_dwWorldY - nWorldY;
                                int ndis =(int) sqrt(float(ndisX * ndisX + ndisY * ndisY));
                                if (ndis > nRange)
                                {
                                    return NULL;
                                }
                                nVolume =  abs(nRange - ndis) * 100 / nRange * 100 * m_nSoundVolume / 10000 ;
                            }
                            m_DequepSound[i]->SetInfo(dwControl, szFileName, nLoop);
                            m_DequepSound[i]->RePlay(nLoop, nRange, nWorldX, nWorldY);
                            m_DequepSound[i]->SetSelfVolume(nVolume);
                            m_DequepSound[i]->SetVolume( iVolume);
                            m_DequepSound[i]->m_dwSleepTime = 0;
                            m_DequepSound[i]->SetPan(0);
                            return m_DequepSound[i];
                        }
                    }
            }
        }
        DWORD dwControl = 0;
        if (nRange)
        {
            dwControl |= DSOUND_3D_SOUND;
        }
        DWORD dwFlag = 0;
        if (nLoop == -1 )
        {
            dwFlag = DSBPLAY_LOOPING;
            dwControl |= DSBPLAY_LOOPING;
        }
        int iVolume = m_nSoundVolume;
        if ((dwControl & DSOUND_3D_SOUND) && nLoop != -1 )
        {
            int ndisX = m_dwWorldX - nWorldX;
            int ndisY = m_dwWorldY - nWorldY;
            int ndis = (int)sqrt(float(ndisX * ndisX + ndisY * ndisY));
            if (ndis > nRange)
            {
                return NULL;
            }
            nVolume =  abs(nRange - ndis) * 100 / nRange * 100 * m_nSoundVolume / 10000 ;
        }
        CSound* pSound = NULL;
        Create(&pSound, szFileName, DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY);
        if (!pSound)
        {
            return NULL;
        }
        pSound->Play(0, dwFlag);
        pSound->SetSelfVolume(nVolume);
        pSound->SetVolume( iVolume );
        if ((dwControl & DSOUND_3D_SOUND) && nLoop == -1 )
        {
            pSound->Stop();
        }
        pSound->m_dwSleepTime = 0;
        pSound->SetSoundPos(nWorldX, nWorldY, nRange);
        pSound->SetInfo(dwControl, szFileName, nLoop);
        m_DequepSound.push_back(pSound);
        return pSound;
    }//end of try
    catch(...)
    {
        FILE* pFile;
        pFile = fopen("sound.log", "at");
        if (!pFile)
        {
            return NULL;
        }
        char szMsg[256];
        sprintf(szMsg, "Catch in PlaySound ,File:%s", szFileName);
        fprintf( pFile, "%s\n", szMsg );
        fclose(pFile);
        return NULL;
    }
}
void CNDsound::CloseAllSound()
{
    for(int i = 0; i < (int)m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
        {
            m_Dequep3DSound[i]->Stop();
            SAFE_DELETE( m_Dequep3DSound[i]);
        }
    }
    m_Dequep3DSound.clear();
    for(int i = 0; i < (int)m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i])
        {
            m_DequepSound[i]->Stop();
            SAFE_DELETE( m_DequepSound[i]);
        }
    }
    m_DequepSound.clear();
}
BOOL	CNDsound::Check2DSound(CSound* pSound)
{
    if (!pSound)
    {
        return false;
    }
    for(int i = 0; i < (int)m_DequepSound.size(); i++)
    {
        if (m_DequepSound[i] == pSound)
        {
            return true;
        }
    }
    return false;
}
void CNDsound::CloseAllMedia()
{
    for(int i = 0; i < (int)m_DequepMedia.size(); i++)
    {
        if (m_DequepMedia[i])
        {
            m_DequepMedia[i]->Stop();
            SAFE_DELETE( m_DequepMedia[i]);
        }
    }
    m_DequepMedia.clear();
}
void CNDsound::ProcessSound(int nWorldX, int nWorldY, int nDir)
{
    if (!m_binitOK)
    {
        return ;
    }
    m_dwWorldX = nWorldX;
    m_dwWorldY = nWorldY;
    DWORD i;
    if (::GetActiveWindow()	!= ::GetParent(m_hWnd) && ::GetParent(GetActiveWindow()) != ::GetParent(m_hWnd))
    {
        DWORD i;
        for(i = 0; i < m_DequepMedia.size(); i++)
        {
            if (m_DequepMedia[i])
            {
                m_DequepMedia[i]->SetVolume(0);
            }
        }
    }
    else
    {
        this->SetMusicVolume(m_nMusicVolume);
    }
    for(i = 0; i < m_DequepMedia.size(); i++)
    {
        if (m_DequepMedia[i])
            if (!m_DequepMedia[i]->IsPlaying())
            {
                SAFE_DELETE( m_DequepMedia[i]);
                m_DequepMedia.erase(m_DequepMedia.begin() + i);
            }
    }
    for(i = 0; i < m_Dequep3DSound.size(); i++)
    {
        if (m_Dequep3DSound[i])
        {
            if (!m_Dequep3DSound[i]->IsBeginning())
            {
                if (m_Dequep3DSound[i]->IsPlaying() )
                {
                    m_Dequep3DSound[i]->SetSoundPos(nWorldX, 0, nWorldY);
                }
                else
                {
                    if (!m_Dequep3DSound[i]->m_dwSleepTime)
                    {
                        m_Dequep3DSound[i]->Stop();
                    }
                    else if (m_Dequep3DSound[i]->m_dwSleepTime++ > 100)
                    {
                        SAFE_DELETE( m_Dequep3DSound[i]);
                        m_Dequep3DSound.erase(m_Dequep3DSound.begin() + i);
                    }
                }
            }
        }
    }
    try
    {
        for(i = 0; i < m_DequepSound.size(); i++)
        {
            if (m_DequepSound[i])
            {
                if (m_DequepSound[i]->m_dwControl & DSOUND_3D_SOUND)
                {
                    int ndisX = nWorldX - m_DequepSound[i]->m_nWorldX;
                    int ndisY = nWorldY - m_DequepSound[i]->m_nWorldY;
                    if (abs(ndisX) < m_DequepSound[i]->m_nRange && abs(ndisY) < m_DequepSound[i]->m_nRange)
                    {
                        if (!m_DequepSound[i]->m_n3DSoundActived)
                        {
                            if ( m_DequepSound[i]->m_nLoop)
                            {
                                m_DequepSound[i]->Continue();
                            }
                        }
                        int nPan =  ndisX * 100 / m_DequepSound[i]->m_nRange * -1;
                        if (abs(nPan) > 10)
                        {
                            m_DequepSound[i]->SetPan(nPan);
                        }
                        int ndis = (int)sqrt(float(ndisX * ndisX + ndisY * ndisY));
                        if (ndis > m_DequepSound[i]->m_nRange)
                        {
                            ndis = m_DequepSound[i]->m_nRange;
                        }
                        ndis =  abs(m_DequepSound[i]->m_nRange - ndis) * 100 / m_DequepSound[i]->m_nRange * 100 * m_nSoundVolume / 10000 ;
                        if (ndis > m_nSoundVolume)
                        {
                            ndis = m_nSoundVolume;
                        }
                        m_DequepSound[i]->SetVolume(ndis);
                    }
                    else
                    {
                        if (m_DequepSound[i]->m_n3DSoundActived)
                        {
                            m_DequepSound[i]->Stop();
                        }
                    }
                    if (m_DequepSound[i]->m_nLoop != -1)
                        if (!m_DequepSound[i]->IsSoundPlaying())
                        {
                            if (m_DequepSound[i]->m_nLoop > 0)
                            {
                                m_DequepSound[i]->m_nLoop --;
                                m_DequepSound[i]->Play(0, 0);
                                m_DequepSound[i]->SetVolume(m_DequepSound[i]->m_nVolume);
                            }
                            else
                            {
                                if (!m_DequepSound[i]->m_dwSleepTime)
                                {
                                    m_DequepSound[i]->Stop();
                                }
                                else if (m_DequepSound[i]->m_dwSleepTime++ > 100)
                                {
                                    SAFE_DELETE(m_DequepSound[i]);
                                    m_DequepSound.erase(m_DequepSound.begin() + i);
                                    i--;
                                }
                            }
                        }
                }
                else
                {
                    // is 2d sound
                    if (!m_DequepSound[i]->IsSoundPlaying())
                    {
                        if (m_DequepSound[i]->m_nLoop > 0)
                        {
                            m_DequepSound[i]->m_nLoop --;
                            m_DequepSound[i]->Play(0, 0);
                            m_DequepSound[i]->SetVolume(m_DequepSound[i]->m_nVolume);
                        }
                        else
                        {
                            if (!m_DequepSound[i]->m_dwSleepTime)
                            {
                                m_DequepSound[i]->Stop();
                            }
                            else if (m_DequepSound[i]->m_dwSleepTime++ > 100)
                            {
                                SAFE_DELETE( m_DequepSound[i]);
                                m_DequepSound.erase(m_DequepSound.begin() + i);
                                i--;
                            }
                        }
                    }
                }
            }
        }
    }//end of try
    catch(...)
    {
        FILE* pFile;
        pFile = fopen("sound.log", "at");
        if (!pFile)
        {
            return ;
        }
        char szMsg[256];
        sprintf(szMsg, "Catch in SoundProcess" );
        fprintf( pFile, "%s\n", szMsg );
        fclose(pFile);
    }
}






DLLAPI int __stdcall DXSoundinit(HWND hWnd)
{
    return sound.init(hWnd);
    ::DXSetSoundVolume(0);
    ::DXSetMusicVolume(0);
}


DLLAPI const CSound* __stdcall DXPlaySound(char* szFileName, int nWorldX, int nWorldY, int nRange, int nLoop, int nVolume)
{
    return sound.PlaySound(szFileName, nWorldX, nWorldY, nRange, nLoop, nVolume);
}


DLLAPI const C3DSound* __stdcall DXPlay3DSound(char* szFileName, int nWorldX, int nWorldY, int nWorldZ, int nRange, int nLoop)
{
    return sound.Play3DSound(sound.m_hWnd, szFileName, nWorldX, nWorldY, nWorldZ, nRange, nLoop);
}

DLLAPI const CMedia* __stdcall DXPlayMedia(char* szFileName, int nLoop)
{
    return  sound.PlayMedia(sound.m_hWnd, szFileName, nLoop);
}

DLLAPI void		__stdcall DXProcessSound(int nWorldX, int nWorldY)
{
    sound.ProcessSound(nWorldX, nWorldY, 0);
}

DLLAPI	BOOL	__stdcall DXCloseSound(char* szFileName, int nX, int nY)
{
    return sound.CloseSound(szFileName, nX, nY);
}

DLLAPI	BOOL	__stdcall DXCloseMedia(char* szFileName)
{
    return sound.CloseMedia(szFileName);
}
DLLAPI	void	__stdcall DXPause()
{
    sound.Pause();
}

DLLAPI	void	__stdcall DXContinue()
{
    sound.Continue();
}

DLLAPI	const	 CSound* __stdcall  DXGetSound(char* szFileName, int nX, int nY)
{
    return sound.GetSound(szFileName, nX, nY);
}
DLLAPI	const C3DSound* __stdcall	DXGet3DSound(char* szFileName, int nX, int nY)
{
    return sound.Get3DSound(szFileName, nX, nY);
}

DLLAPI	BOOL	__stdcall DXClose2DSound(CSound* pSound) { return sound.CloseSound(pSound);}
DLLAPI	BOOL	__stdcall DXClose3DSound(C3DSound* pSound) { return sound.CloseSound(pSound);}
DLLAPI  void	__stdcall DXSet3DSoundPos(C3DSound* pSound, int nX, int nY, int nZ)
{
    sound.SetSoundPos(pSound, nX, nY, nZ);
}
DLLAPI  void	__stdcall DXSet2DSoundPos(CSound* pSound, int nX, int nY)
{
    sound.SetSoundPos(pSound, nX, nY);
}

DLLAPI  void	__stdcall DXSetVolume(char* szFileName, int nVolume, int nWorldX, int nWorldY)
{
    sound.SetVolume(szFileName, nVolume, nWorldX, nWorldY);
}

DLLAPI	void	__stdcall DXSetMusicVolume(int nVolume)
{
    sound.SetMusicVolume(nVolume);
}

DLLAPI	void	__stdcall DXSetSoundVolume(int nVolume)
{
    sound.SetSoundVolume(nVolume);
}
DLLAPI	int		__stdcall DXGetMusicVolume()
{
    return sound.GetMusicVolume();
}

DLLAPI	int		__stdcall DXGetSoundVolume()
{
    return sound.GetSoundVolume();
}

DLLAPI	void		__stdcall DXDestroy()
{
    sound.~CNDsound();
}

DLLAPI  void	__stdcall DXCloseAllSound()
{
    sound.CloseAllSound();
}

DLLAPI  void	__stdcall DXCloseAllMedia()
{
    sound.CloseAllMedia();
}

DLLAPI	BOOL	__stdcall Check2DSound(CSound* pSound)
{
    return sound.Check2DSound(pSound);
}
