
//**********************************************************
// 代码编辑器
//**********************************************************

//----------------------------------------------------
//2DMapCoverObj.cpp
//----------------------------------------------------
#pragma warning(disable:4786)
#include "2DMapCOverObj.h"
#include "3DGameMap.h"
#include "GameDataSet.h"
//----------------------------------------------------
BOOL C2DMapCoverObj::s_bStroke = false;
//----------------------------------------------------
C2DMapCoverObj::C2DMapCoverObj()
{
    m_dwTimePreFrame = ::TimeGet();
    m_dwShowWay = _SHOWWAY_NORMAL;
}
//----------------------------------------------------
C2DMapCoverObj::~C2DMapCoverObj()
{
}
//----------------------------------------------------
void C2DMapCoverObj::Show(void* pInfo)
{
    CAni* pAni = g_objGameDataSet.GetDataAni(m_szFileName, m_szIndex, EXIGENCE_NORMAL, 60 * 1000);
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
    if (!pBmp)
    {
        return;
    }
    CMySize sizeBmp;
    pBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
    sizeBmp.iWidth = sizeBmp.iWidth * g_objGameMap.GetScale() / _NORMAL_SCALE;
    sizeBmp.iHeight = sizeBmp.iHeight * g_objGameMap.GetScale() / _NORMAL_SCALE;
    if (!s_bStroke)
    {
        unsigned char ucA, ucR, ucG, ucB;
        ucA = m_dwARGB >> 24;
        ucR =(unsigned char)( (m_dwARGB & 0x00ff0000) >> 16);
        ucG = (unsigned char)((m_dwARGB & 0x0000ff00) >> 8);
        ucB = m_dwARGB & 0x000000ff;
        pBmp->SetColor(ucA, ucR, ucG, ucB);
        pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeBmp.iWidth, sizeBmp.iHeight, 0, m_dwShowWay);
    }
    else
    {
        CMyPos posMouse;
        ::MouseCheck(posMouse.x, posMouse.y);
        CMyPos posCell;
        g_objGameMap.Mouse2Cell(posMouse.x, posMouse.y, posCell.x, posCell.y);
        CMyBitmap* pBmp = pAni->GetFrame(m_nFrameIndex);
        if ((posCell.x == m_posCell.x) && (posCell.y == m_posCell.y))
        {
            pBmp->SetColor(0x77, 0xff, 0xff, 0xff);
            pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeBmp.iWidth, sizeBmp.iHeight, 0, m_dwShowWay);
            pBmp->SetColor(0xff, 0xff, 0xff, 0xff);
        }
        else
        {
            pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeBmp.iWidth, sizeBmp.iHeight, 0, m_dwShowWay);
        }
        if (pBmp)
        {
            CMyBitmap::ShowRect(posShow.x, posShow.y,
                                posShow.x + sizeBmp.iWidth, posShow.y + sizeBmp.iHeight,
                                0xffffff00);
        }
        CMyPos posScr;
        g_objGameMap.Cell2Screen(m_posCell.x, m_posCell.y, posScr.x, posScr.y);
        g_objGameMap.MapScaleShowPos(posScr.x, posScr.y);
        // 显示重心点
        CMyPos posA, posB, posC, posD;
        posA.x = posScr.x - _CELL_WIDTH * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
        posA.y = posScr.y;
        posB.x = posScr.x;
        posB.y = posScr.y - _CELL_HEIGHT * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
        posC.x = posScr.x;
        posC.y = posScr.y + _CELL_HEIGHT * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
        posD.x = posScr.x + _CELL_WIDTH * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
        posD.y = posScr.y;
        CMyBitmap::ShowBlock(posA, posB, posC, posD, 0xff00ff00);
    }
}
//----------------------------------------------------
void C2DMapCoverObj::Process(void* pInfo)
{
    if (g_objGameMap.IsPosVisible(m_posCell, 12))
    {
        CAni* pAni = g_objGameDataSet.GetDataAni(m_szFileName, m_szIndex, EXIGENCE_NORMAL, 60 * 1000);
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
C2DMapCoverObj* C2DMapCoverObj::CreateNew(char* pszFileName, char* pszIndex)
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
    C2DMapCoverObj* pObj = new C2DMapCoverObj;
    if (pObj)
    {
        pObj->Create(pszFileName, pszIndex);
    }
    return pObj;
}
//----------------------------------------------------
void   C2DMapCoverObj::Create(char* pszFileName, char* pszIndex)
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
void   C2DMapCoverObj::SetOffset(CMyPos posOffset)
{
    m_posOffset = posOffset;
}
//----------------------------------------------------
void   C2DMapCoverObj::GetOffset(CMyPos& posOffset)
{
    posOffset = m_posOffset;
}
//----------------------------------------------------
void   C2DMapCoverObj::SetFrameInterval(DWORD dwInterval)
{
    m_dwFrameInterval = dwInterval;
}
//----------------------------------------------------
DWORD  C2DMapCoverObj::GetFrameInterval()
{
    return m_dwFrameInterval;
}
//----------------------------------------------------
void   C2DMapCoverObj::SetBase(CMySize infoSize)
{
    m_sizeBase = infoSize;
}
//----------------------------------------------------
void   C2DMapCoverObj::GetBase(CMySize& infoSize)
{
    infoSize = m_sizeBase;
}
//----------------------------------------------------
void C2DMapCoverObj::SaveDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fwrite(m_szFileName, sizeof(char), _MAX_PATH, fp);
    fwrite(m_szIndex, sizeof(char), _MAX_TITLESIZE, fp);
    fwrite(&m_posCell.x, sizeof(int), 1, fp);
    fwrite(&m_posCell.y, sizeof(int), 1, fp);
    fwrite(&m_sizeBase.iWidth, sizeof(UINT), 1, fp);
    fwrite(&m_sizeBase.iHeight, sizeof(UINT), 1, fp);
    fwrite(&m_posOffset.x, sizeof(int), 1, fp);
    fwrite(&m_posOffset.y, sizeof(int), 1, fp);
    fwrite(&m_dwFrameInterval, sizeof(DWORD), 1, fp);
    //	fwrite(&m_dwShowWay, sizeof(DWORD), 1, fp);
}
//----------------------------------------------------
void C2DMapCoverObj::LoadDataObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fread(m_szFileName, sizeof(char), _MAX_PATH, fp);
    fread(m_szIndex, sizeof(char), _MAX_TITLESIZE, fp);
	::DebugMsg("%s %s",m_szFileName,m_szIndex);
    fread(&m_posCell.x, sizeof(int), 1, fp);
    fread(&m_posCell.y, sizeof(int), 1, fp);
    fread(&m_sizeBase.iWidth, sizeof(UINT), 1, fp);
    fread(&m_sizeBase.iHeight, sizeof(UINT), 1, fp);
    fread(&m_posOffset.x, sizeof(int), 1, fp);
    fread(&m_posOffset.y, sizeof(int), 1, fp);
    fread(&m_dwFrameInterval, sizeof(DWORD), 1, fp);
    //fread(&m_dwShowWay, sizeof(DWORD), 1, fp);
}
//----------------------------------------------------
void C2DMapCoverObj::SaveTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fprintf(fp, "AniFile=%s\n", m_szFileName);
    fprintf(fp, "AniTitle=%s\n", m_szIndex);
    fprintf(fp, "PosCell=[%d,%d]\n", m_posCell.x, m_posCell.y);
    fprintf(fp, "SizeBase=[%u,%u]\n", m_sizeBase.iWidth, m_sizeBase.iHeight);
    fprintf(fp, "PosOffset=[%d,%d]\n", m_posOffset.x, m_posOffset.y);
    fprintf(fp, "FrameInterval=%u\n", m_dwFrameInterval);
}
//----------------------------------------------------
void C2DMapCoverObj::LoadTextObj(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    fscanf(fp, "AniFile=%s\n", m_szFileName);
    fscanf(fp, "AniTitle=%s\n", m_szIndex);
    fscanf(fp, "PosCell=[%d,%d]\n", &m_posCell.x, &m_posCell.y);
    fscanf(fp, "SizeBase=[%u,%u]\n", &m_sizeBase.iWidth, &m_sizeBase.iHeight);
    fscanf(fp, "PosOffset=[%d,%d]\n", &m_posOffset.x, &m_posOffset.y);
    fscanf(fp, "FrameInterval=%u\n", &m_dwFrameInterval);
}
//----------------------------------------------------
BOOL   C2DMapCoverObj::IsFocus()
{
    CMyPos posMouse;
    ::MouseCheck(posMouse.x, posMouse.y);
    CAni* pAni = g_objGameDataSet.GetDataAni(m_szFileName, m_szIndex, EXIGENCE_NORMAL, 60 * 1000);
    CMyBitmap* pBmp = pAni->GetFrame(m_nFrameIndex);
    if (pBmp)
    {
        CMyPos posScr, posWorld, posShow;
        this->GetWorldPos(posWorld);
        g_objGameMap.World2Screen(posWorld.x, posWorld.y, posScr.x, posScr.y);
        posShow.x = posScr.x - m_posOffset.x;
        posShow.y = posScr.y - m_posOffset.y;
        CMyPos posTransparent;
        posTransparent.x = posMouse.x - posShow.x;
        posTransparent.y = posMouse.y - posShow.y;
        if (pBmp->IsTransparent(posTransparent))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------
void  C2DMapCoverObj::SetShowWay(DWORD dwShowWay)
{
    m_dwShowWay = dwShowWay;
}
