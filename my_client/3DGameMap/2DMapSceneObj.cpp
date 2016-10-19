
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------
//2DMapSceneObj.cpp
//----------------------------------------------------
#pragma warning(disable:4786)
#include "2DMapSceneObj.h"
#include "3DGameMap.h"
#include "GameDataSet.h"
//----------------------------------------------------
BOOL C2DMapSceneObj::s_bStroke = false;
//----------------------------------------------------
C2DMapSceneObj::C2DMapSceneObj()
{
    m_dwTimePreFrame = ::TimeGet();
}
//----------------------------------------------------
C2DMapSceneObj::~C2DMapSceneObj()
{
}
//----------------------------------------------------
void   C2DMapSceneObj::SetOffset(CMyPos posOffset)
{
    m_posOffset = posOffset;
}
//----------------------------------------------------
void   C2DMapSceneObj::GetOffset(CMyPos& posOffset)
{
    posOffset = m_posOffset;
}
//----------------------------------------------------
void C2DMapSceneObj::Show(void* pInfo)
{
    CAni* pAni = g_objGameDataSet.GetDataAni(m_szFileName, m_szIndex);
    if (!pAni)
    {
        return;
    }
    CMyPos posScr, posWorld, posShow;
    this->GetWorldPos(posWorld);
    g_objGameMap.World2Screen(posWorld.x, posWorld.y, posScr.x, posScr.y);
    posShow.x = posScr.x - m_posOffset.x;
    posShow.y = posScr.y - m_posOffset.y;
    g_objGameMap.MapScaleShowPos(posShow.x, posShow.y);
    CMyBitmap* pBmp = pAni->GetFrame(m_nFrameIndex);
    if (pBmp)
    {
        CMySize sizeBmp;
        pBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
        sizeBmp.iWidth = sizeBmp.iWidth * g_objGameMap.GetScale() / _NORMAL_SCALE;
        sizeBmp.iHeight = sizeBmp.iHeight * g_objGameMap.GetScale() / _NORMAL_SCALE;
        pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeBmp.iWidth, sizeBmp.iHeight);
    }
    pAni->Show(m_nFrameIndex, posShow.x, posShow.y);
    if (s_bStroke)
    {
        if (pBmp)
        {
            CMySize sizeBmp;
            pBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
            sizeBmp.iWidth = sizeBmp.iWidth * g_objGameMap.GetScale() / _NORMAL_SCALE;
            sizeBmp.iHeight = sizeBmp.iHeight * g_objGameMap.GetScale() / _NORMAL_SCALE;
            CMyBitmap::ShowRect(posShow.x, posShow.y,
                                posShow.x + sizeBmp.iWidth, posShow.y + sizeBmp.iHeight,
                                0xffffff00);
        }
    }
}
//----------------------------------------------------
void C2DMapSceneObj::Process(void* pInfo)
{
    if (g_objGameMap.IsPosVisible(m_posCell))
    {
        CAni* pAni = g_objGameDataSet.GetDataAni(m_szFileName, m_szIndex);
        if (!pAni)
        {
            return;
        }
        m_nFrameIndex = ((::TimeGet() - m_dwTimePreFrame) / m_dwFrameInterval) % pAni->GetFrameAmount();
        CInteractiveLayer* pLayer = g_objGameMap.GetInteractiveLayer();
        if (pLayer)
        {
            pLayer->ApplyShow(this);
        }
    }
}
//----------------------------------------------------
C2DMapSceneObj* C2DMapSceneObj::CreateNew(char* pszFileName, char* pszIndex)
{
    if (!pszFileName || !pszIndex)
    {
        return NULL;
    }
    if (strlen(pszFileName) >= _MAX_PATH)
    {
        return NULL;
    }
    if (strlen(pszFileName) >= _MAX_TITLESIZE)
    {
        return NULL;
    }
    C2DMapSceneObj* pObj = new C2DMapSceneObj;
    if (pObj)
    {
        pObj->Create(pszFileName, pszIndex);
    }
    return pObj;
}
//----------------------------------------------------
void   C2DMapSceneObj::Create(char* pszFileName, char* pszIndex)
{
    if (!pszFileName || !pszIndex)
    {
        return;
    }
    if (strlen(pszFileName) >= _MAX_PATH)
    {
        return;
    }
    if (strlen(pszFileName) >= _MAX_TITLESIZE)
    {
        return;
    }
    strcpy(m_szFileName, pszFileName);
    strcpy(m_szIndex, pszIndex);
}
//----------------------------------------------------
void   C2DMapSceneObj::SetFrameInterval(DWORD dwInterval)
{
    m_dwFrameInterval = dwInterval;
}
//----------------------------------------------------
DWORD  C2DMapSceneObj::GetFrameInterval()
{
    return m_dwFrameInterval;
}
//----------------------------------------------------
BOOL   C2DMapSceneObj::IsFocus()
{
    return false;
}
//----------------------------------------------------
void C2DMapSceneObj::SaveDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fwrite(m_szFileName, sizeof(char), _MAX_PATH, fp);
    fwrite(m_szIndex, sizeof(char), _MAX_TITLESIZE, fp);
    fwrite(&m_posCell.x, sizeof(int), 1, fp);
    fwrite(&m_posCell.y, sizeof(int), 1, fp);
    fwrite(&m_posOffset.x, sizeof(int), 1, fp);
    fwrite(&m_posOffset.y, sizeof(int), 1, fp);
    fwrite(&m_dwFrameInterval, sizeof(DWORD), 1, fp);
    fwrite(&m_dwShowWay, sizeof(DWORD), 1, fp);
}
//----------------------------------------------------
void C2DMapSceneObj::SaveTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fprintf(fp, "AniFile=%s\n", m_szFileName);
    fprintf(fp, "AniTitle=%s\n", m_szIndex);
    fprintf(fp, "PosCell=[%d,%d]\n", m_posCell.x, m_posCell.y);
    fprintf(fp, "PosOffset=[%d,%d]\n", m_posOffset.x, m_posOffset.y);
    fprintf(fp, "FrameInterval=%u\n", m_dwFrameInterval);
    fprintf(fp, "ShowWay=%u\n", m_dwShowWay);
}
//----------------------------------------------------

void C2DMapSceneObj::LoadDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fread(m_szFileName, sizeof(char), _MAX_PATH, fp);
    fread(m_szIndex, sizeof(char), _MAX_TITLESIZE, fp);
    fread(&m_posCell.x, sizeof(int), 1, fp);
    fread(&m_posCell.y, sizeof(int), 1, fp);
    fread(&m_posOffset.x, sizeof(int), 1, fp);
    fread(&m_posOffset.y, sizeof(int), 1, fp);
    fread(&m_dwFrameInterval, sizeof(DWORD), 1, fp);
    fread(&m_dwShowWay, sizeof(DWORD), 1, fp);
}
//----------------------------------------------------
void  C2DMapSceneObj::LoadTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fscanf(fp, "AniFile=%s\n", m_szFileName);
    fscanf(fp, "AniTitle=%s\n", m_szIndex);
    fscanf(fp, "PosCell=[%d,%d]\n", &m_posCell.x, &m_posCell.y);
    fscanf(fp, "PosOffset=[%d,%d]\n", &m_posOffset.x, &m_posOffset.y);
    fscanf(fp, "FrameInterval=%u\n", &m_dwFrameInterval);
    fscanf(fp, "ShowWay=%u\n", &m_dwShowWay);
}
//----------------------------------------------------
