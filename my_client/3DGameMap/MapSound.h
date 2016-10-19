
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MapSound.h: interface for the CMapSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPSOUND_H__CC3E4C02_E21B_40D0_9B9C_7A55E2337029__INCLUDED_)
#define AFX_MAPSOUND_H__CC3E4C02_E21B_40D0_9B9C_7A55E2337029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//----------------------------------------------------
#include "MapObj.h"
//----------------------------------------------------

class CSound;
class CMapSound : public CMapObj
{
public:
    CMapSound();
    virtual ~CMapSound();
private:
    CMyPos	m_posWorld;
    CMyPos	m_posCell;
    BOOL	m_bFocus;
    CSound* m_pSound;
    // info
    int		m_nRange;
    int		m_nVolume;
    char	m_szFile[_MAX_PATH];
    DWORD	m_dwInterval;
    BOOL	m_bWait;
    DWORD	m_dwWaitPoint;
    BOOL	m_bActive;
public:
    static BOOL s_bStroke;

public:
    virtual void GetPos(CMyPos& posCell) {posCell = m_posCell;}
    virtual void SetPos(CMyPos posCell) {m_posCell = posCell;}


    virtual void Show(void* pInfo);
    virtual void Process(void* pInfo);
    virtual int  GetObjType() {return MAP_SOUND;}
    virtual BOOL IsFocus();
    virtual void GetWorldPos(CMyPos& posWorld);
    virtual void SetWorldPos(CMyPos posWorld);

    void   LoadDataObj(FILE* fp);
    void   LoadTextObj(FILE* fp);
    void   SaveDataObj(FILE* fp);
    void   SaveTextObj(FILE* fp);
    void   Play();

    BOOL   Create(CMyPos posWorld, int nRange, int nVolume, char* pszFile, DWORD dwInterval);
    static CMapSound* CreateNew(CMyPos posWorld, int nRange, int nVolume, char* pszFile, DWORD dwInterval);
};

#endif // !defined(AFX_MAPSOUND_H__CC3E4C02_E21B_40D0_9B9C_7A55E2337029__INCLUDED_)
