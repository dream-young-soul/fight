
//**********************************************************
// 代码编辑器
//**********************************************************

// MapSound.cpp: implementation of the CMapSound class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)
#include "ndsound.h"

#include "MapSound.h"
#include "Nd_Bitmap.h"
#include "3DGameMap.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CMapSound::s_bStroke = false;

CMapSound::CMapSound()
{
    m_nType = MAP_SOUND;
    m_pSound = NULL;
    m_dwInterval = 0;
    m_bActive = false;
}
//-------------------------------------------------------------
CMapSound::~CMapSound()
{
    m_bFocus = false;
    if (m_pSound)
    {
        DXClose2DSound(m_pSound);
    }
}
//-------------------------------------------------------------

void CMapSound::Show(void* pInfo)
{
#ifdef _ARHUN_DEBUG
    if (!s_bStroke)
    {
        return;
    }
    static CMyBitmap* s_pObjBmp = NULL;
    if (!s_pObjBmp)
    {
        s_pObjBmp = new CMyBitmap;
        s_pObjBmp->Load("sound.tga");
    }
    CMySize sizeBmp;
    s_pObjBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
    CMyPos posScr;
    g_objGameMap.World2Screen(m_posWorld.x, m_posWorld.y, posScr.x, posScr.y);
    CMyPos posMouse;
    ::MouseCheck(posMouse.x, posMouse.y);
    // 判断是否focus
    if ((posMouse.x > posScr.x - sizeBmp.iWidth / 2) &&
            (posMouse.y > posScr.y - sizeBmp.iHeight / 2) &&
            (posMouse.x < posScr.x - sizeBmp.iWidth / 2 + sizeBmp.iWidth) &&
            (posMouse.y < posScr.y - sizeBmp.iHeight / 2 + sizeBmp.iHeight))
    {
        m_bFocus = true;
    }
    else
    {
        m_bFocus = false;
    }
    s_pObjBmp->Show(posScr.x - sizeBmp.iWidth / 2, posScr.y - sizeBmp.iHeight / 2);
#endif
}
//-------------------------------------------------------------
void CMapSound::Process(void* pInfo)
{
#ifdef _ARHUN_DEBUG
    CMyPos posCell;
    g_objGameMap.World2Cell(m_posWorld.x, m_posWorld.y, posCell.x, posCell.y);
    if (g_objGameMap.IsPosVisible(posCell))
    {
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (pLayer)
        {
            pLayer->ApplyShow(this);
        }
    }
    else
    {
        m_bFocus = false;
    }
#endif
    //	if (m_dwInterval == 0)
    //		return;
    CMyPos posHero, posHeroWorld;
    g_objHero.GetPos(posHero);
    g_objGameMap.Cell2World(posHero.x, posHero.y, posHeroWorld.x, posHeroWorld.y);
    BOOL bActive;
    if ((abs(posHeroWorld.x - m_posWorld.x) > m_nRange)// && abs(posHero.x - m_posWorld.x) > 1000)
            || (abs(posHeroWorld.y - m_posWorld.y) > m_nRange))// && abs(posHero.y - m_posWorld.y) > 1000))
    {
        bActive = false;
        if (bActive != m_bActive)
        {
            m_bActive = bActive;
            if (Check2DSound(m_pSound))
            {
                ::DXClose2DSound(m_pSound);
            }
        }
    }
    else
    {
        bActive = true;
        m_bActive = bActive;
    }
    if (!Check2DSound(m_pSound) && m_bActive)
    {
        if (!m_bWait)
        {
            m_bWait = true;
            m_dwWaitPoint = ::TimeGet();
        }
        if (::TimeGet() - m_dwWaitPoint > m_dwInterval)
        {
            this->Play();
            m_bWait = false;
        }
    }
}
//-------------------------------------------------------------
BOOL CMapSound::IsFocus()
{
    return m_bFocus;
}
//-------------------------------------------------------------
void CMapSound::GetWorldPos(CMyPos& posWorld)
{
    posWorld = m_posWorld;
}
//-------------------------------------------------------------
void CMapSound::SetWorldPos(CMyPos posWorld)
{
    m_posWorld = posWorld;
    //m_posCell - 30 保证在最前面显示
    if (m_pSound)
    {
        DXSet2DSoundPos(m_pSound, posWorld.x, posWorld.y);
    }
    g_objGameMap.World2Cell(posWorld.x, posWorld.y, m_posCell.x, m_posCell.y);
    m_posCell.x -= 30;
    m_posCell.x -= 30;
}
//-------------------------------------------------------------
void   CMapSound::LoadDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fread(m_szFile, sizeof(char), _MAX_PATH, fp);
    fread(&m_posWorld, sizeof(CMyPos), 1, fp);
    fread(&m_nRange, sizeof(int), 1, fp);
    fread(&m_nVolume, sizeof(int), 1, fp);
    fread(&m_dwInterval, sizeof(int), 1, fp);
}
//-------------------------------------------------------------
void   CMapSound::LoadTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fscanf(fp, "SoundFile=%s\n", m_szFile);
    fscanf(fp, "PosWorld=[%d,%d]\n", &m_posWorld.x, &m_posWorld.y);
    fscanf(fp, "Range=%d\n", &m_nRange);
    fscanf(fp, "Volume=%d\n", &m_nVolume);
    fscanf(fp, "Interval=%u\n", &m_dwInterval);
}
//-------------------------------------------------------------
void   CMapSound::SaveDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fwrite(m_szFile, sizeof(char), _MAX_PATH, fp);
    fwrite(&m_posWorld, sizeof(CMyPos), 1, fp);
    fwrite(&m_nRange, sizeof(int), 1, fp);
    fwrite(&m_nVolume, sizeof(int), 1, fp);
    fwrite(&m_dwInterval, sizeof(int), 1, fp);
}
//-------------------------------------------------------------
void   CMapSound::SaveTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fprintf(fp, "SoundFile=%s\n", m_szFile);
    fprintf(fp, "PosWorld=[%d,%d]\n", m_posWorld.x, m_posWorld.y);
    fprintf(fp, "Range=%d\n", m_nRange);
    fprintf(fp, "Volume=%d\n", m_nVolume);
    fprintf(fp, "Interval=%u\n", m_dwInterval);
}
//-------------------------------------------------------------
BOOL CMapSound::Create(CMyPos posWorld, int nRange, int nVolume, char* pszFile, DWORD dwInterval)
{
    if (!pszFile)
    {
        return false;
    }
    this->SetWorldPos(posWorld);
    m_nRange = nRange;
    m_nVolume = nVolume;
    strcpy(m_szFile, pszFile);
    m_dwInterval = dwInterval;
    //	DXPlaySound(m_szFile);
    //	m_pSound = (CSound*)DXPlaySound(m_szFile, 0, 0,
    //											m_nRange, -1, m_nVolume);
    return true;
}
//-------------------------------------------------------------
CMapSound* CMapSound::CreateNew(CMyPos posWorld, int nRange, int nVolume, char* pszFile, DWORD dwInterval)
{
    CMapSound* pSound = new CMapSound;
    MYASSERT(pSound);
    if (!pSound->Create(posWorld, nRange, nVolume, pszFile, dwInterval))
    {
        SAFE_DELETE(pSound);
        return NULL;
    }
    return pSound;
}
//-------------------------------------------------------------
void CMapSound::Play()
{
    if (m_pSound && Check2DSound(m_pSound))
    {
        DXClose2DSound(m_pSound);
    }
    if (m_dwInterval == 0)
        m_pSound = (CSound*)DXPlaySound(m_szFile, m_posWorld.x, m_posWorld.y,
                                        m_nRange, -1, m_nVolume);
    else
        m_pSound = (CSound*)DXPlaySound(m_szFile, m_posWorld.x, m_posWorld.y,
                                        m_nRange, 0, m_nVolume);
}
//-------------------------------------------------------------
