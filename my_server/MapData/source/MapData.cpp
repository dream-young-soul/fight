
//**********************************************************
// 代码编辑器
//**********************************************************

//----------------------------------------------------//
// InterActiveLayer.cpp
//----------------------------------------------------//
#include "typedef.h"
#include "MapData.h"
#include "TerrainObj.h"
#include "common.h"
#include "inifile.h"
#include "I_MapData.h"
#include "LOGFILE.h"
#include <math.h>

const int	_MAX_TITLESIZE			= 128;

//----------------------------------------------------//


//----------------------------------------------------//
CMapData::CMapData()
{
    //	m_setCell.clear();
    m_setPassage.clear();
}
//----------------------------------------------------//
CMapData::~CMapData()
{
    this->ClearNodeSet();
    this->ClearCell();
    this->ClearPassage();
    this->ClearMapObj();
    this->ClearLayerSet();
}
//----------------------------------------------------//
CMapData* CMapData::CreateNew(LPCTSTR pszFileName, LPCTSTR pszVersion)
{
    if (!pszFileName)
    {
        return NULL;
    }
    FILE* fp = fopen(pszFileName, "rb");
    if (!fp)
    {
        LOGERROR("无法打开地图文件[%s]", pszFileName);
        return NULL;
    }
    CMapData* ptr = new CMapData;
    if (!ptr)
    {
        fclose(fp);
        return NULL;
    }
    char szHeader[8];
    fread(&szHeader, sizeof(char), 8, fp);
    if (pszVersion)
    {
        ptr->m_bDDVersion = true;
    }
    /*if (pszVersion && stricmp(szHeader, pszVersion) != 0)
    {
    	LOGERROR("地图文件[%s]版本检查错误！", pszFileName);
    	return NULL;
    }*/
    // get puzzle file name ...
    char szFileName[_MAX_PATH];
    fread(szFileName, sizeof(char), _MAX_PATH, fp);
    if (	!ptr->LoadSurfaceCellData(fp)
            || !ptr->LoadDataPassage(fp)
            || !ptr->LoadTerrainItemData(fp)
            //		|| !ptr->LoadSenceData(fp)
      )
    {
        LOGERROR("地图文件[%s]数据装载错误！", pszFileName);
        SAFE_DELETE(ptr);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    if (!ptr->LoadPuzzle(szFileName))
    {
        LOGERROR("背景文件[%s]数据装载错误！", szFileName);
        SAFE_DELETE(ptr);
        return NULL;
    }
    return ptr;
}
//-------------------------------------------------------------
bool CMapData::LoadPuzzle(char* pszFile)
{
    if (!pszFile)
    {
        return false;
    }
    FILE* fp = fopen(pszFile, "rb");
    if (!fp)
    {
        return false;
    }
    // header..(8 bytes)
    char szHeader[8] = "";
    fread(szHeader, 1, 8, fp);
    // check header ...
    /*if (strcmp(szHeader, "PUZZLE") != 0)
    {
    	fclose(fp);
    	return false;
    }*/
    // AniFile.. (256 bytes)
    char szAni[256];
    fread(szAni, 1, 256, fp);
    //	this->SetAniFile(szAni);
    // Size...
    SIZE infoSize;
    fread(&infoSize.cx, sizeof(UINT), 1, fp);
    fread(&infoSize.cy, sizeof(UINT), 1, fp);
    //	this->SetSize(infoSize);
    /*/ Grid...
    int nAmount = infoSize.iWidth*infoSize.iHeight;
    m_setAniInfo.clear();
    for(int i = 0; i < nAmount; i++)
    {
    	unsigned short usData;
    	fread(&usData, sizeof(unsigned short), 1, fp);
    	m_setAniInfo.push_back(usData);
    }*/
    fclose(fp);
#define FULL_CELL
#ifdef	FULL_CELL
    // full cell out of puzzle to sign mask flag.
    ASSERT(m_sizeMap.cx == m_sizeMap.cy);
    const int _CELL_WIDTH	= 64;
    const int _CELL_HEIGHT	= 32;
    int		_BLOCK_SIZE	= 256;
    if (m_bDDVersion)
    {
        _BLOCK_SIZE	= 128;
    }
    infoSize.cx	= (infoSize.cx * _BLOCK_SIZE) / _CELL_WIDTH;
    infoSize.cy	= (infoSize.cy * _BLOCK_SIZE) / _CELL_HEIGHT;
    int nDiagonalCells = m_sizeMap.cx / 2;
    int nTopFulCells	= (infoSize.cx / 2) + ((nDiagonalCells - infoSize.cy) / 2);
    int nLeftFulCells	= (infoSize.cy / 2) + ((nDiagonalCells - infoSize.cx) / 2);
    ASSERT(nTopFulCells + nLeftFulCells == nDiagonalCells);
    // full top & bottom
    for(int y = 0; y < nTopFulCells; y++)
    {
        for(int x = 0; x < nTopFulCells - y; x++)
        {
            CCell* pCell = GetCell(x, y);
            IF_OK(pCell)
            pCell->FullMask();
            pCell = GetCell((m_sizeMap.cx - 1) - x, (m_sizeMap.cy - 1) - y);
            IF_OK(pCell)
            pCell->FullMask();
        }
    }
    // full left & right
    for(int y = 0; y < nLeftFulCells; y++)
    {
        for(int x = 0; x < nLeftFulCells - y; x++)
        {
            CCell* pCell = GetCell(x, (m_sizeMap.cy - 1) - y);
            IF_OK(pCell)
            pCell->FullMask();
            pCell = GetCell((m_sizeMap.cx - 1) - x, y);
            IF_OK(pCell)
            pCell->FullMask();
        }
    }
#endif // FULL_CELL
    return true;
}
//-------------------------------------------------------------
bool CMapData::LoadSurfaceCellData(FILE* fp)
{
    SIZE infoSize = {0, 0};
    fread(&infoSize.cx, sizeof(UINT), 1, fp);
    fread(&infoSize.cy, sizeof(UINT), 1, fp);
    m_sizeMap		= infoSize;
    IF_NOT(m_setCell.Create(infoSize.cx * infoSize.cy))
    return false;
    int i, j;
    int	idx = 0;
    for(i = 0; i < infoSize.cy; i++)
    {
        DWORD dwCheckData = 0;
        for(j = 0; j < infoSize.cx; j++)
        {
            SHORT	nAlt	= 0;
            USHORT	dwMask	= 0;
            USHORT	nTerrian	= 0;
            fread(&dwMask, sizeof(unsigned short), 1, fp);
            fread(&nTerrian, sizeof(unsigned short), 1, fp);
            fread(&nAlt, sizeof(short), 1, fp);
            dwCheckData += dwMask * (nTerrian + i + 1) + (nAlt + 2) * (j + 1 + nTerrian);
            IF_OK(idx < m_setCell.size())
            {
                CCell* pCell = m_setCell[idx++];
                pCell->Create(nAlt, dwMask);
            }
        }
        DWORD dwMapCheckData;
        fread(&dwMapCheckData, sizeof(DWORD), 1, fp);
        if (dwMapCheckData != dwCheckData)
        {
            return false;
        }
    }
    return true;
}
//----------------------------------------------------//
bool CMapData::LoadDataPassage(FILE* fp)
{
    if (!fp)
    {
        return false;
    }
    int nAmount;
    fread(&nAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nAmount; i++)
    {
        PassageInfo info;
        fread(&info, sizeof(PassageInfo), 1, fp);
        AddPassage(&info);
    }
    return true;
}
//-------------------------------------------------------------
bool CMapData::LoadTerrainItemData(FILE* fp)
{
    if (!fp)
    {
        return false;
    }
    int nAmount = 0;
    fread(&nAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nAmount; i++)
    {
        // get obj type
        int nMapObjType;
        fread(&nMapObjType, sizeof(int), 1, fp);
        switch(nMapObjType)
        {
        case MAP_COVER:
            {
                char	data[_MAX_PATH];
                CHECKF(_MAX_TITLESIZE <= _MAX_PATH);
                fread(data, sizeof(char), _MAX_PATH, fp);
                fread(data, sizeof(char), _MAX_TITLESIZE, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(UINT), 1, fp);
                fread(data, sizeof(UINT), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(DWORD), 1, fp);
            }
            break;
        case MAP_TERRAIN:
            {
                char	szFileName[_MAX_PATH];
                POINT	posCell;
                fread(szFileName, sizeof(char), _MAX_PATH, fp);
                fread(&posCell.x, sizeof(int), 1, fp);
                fread(&posCell.y, sizeof(int), 1, fp);
                CTerrainObj* pTerrainObj = CTerrainObj::CreateNew(szFileName);
                if (!pTerrainObj)
                {
                    return false;
                }
                pTerrainObj->SetPos(posCell);
                if (!this->AddMapObj(pTerrainObj))
                {
                    return false;
                }
            }
            break;
        case MAP_SCENE:
            {
                char	data[_MAX_PATH];
                CHECKF(_MAX_TITLESIZE <= _MAX_PATH);
                fread(data, sizeof(char), _MAX_PATH, fp);
                fread(data, sizeof(char), _MAX_TITLESIZE, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(DWORD), 1, fp);
                fread(data, sizeof(DWORD), 1, fp);
            }
            break;
        case MAP_3DEFFECT:
            {
                char	data[_MAX_PATH];
                CHECKF(64 <= _MAX_PATH);
                fread(data, sizeof(char), 64, fp);
                fread(data, sizeof(POINT), 1, fp);
            }
            break;
        case MAP_SOUND:
            {
                char	data[_MAX_PATH];
                CHECKF(_MAX_TITLESIZE <= _MAX_PATH);
                fread(data, sizeof(char), _MAX_PATH, fp);
                fread(data, sizeof(POINT), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(int), 1, fp);
                fread(data, sizeof(int), 1, fp);
            }
            break;
        case MAP_3DEFFECTNEW:
            {
                char	data[_MAX_PATH];
                CHECKF(64 <= _MAX_PATH);
                fread(data, sizeof(char), 64, fp);
                fread(data, sizeof(POINT), 1, fp);
                fread(data, sizeof(float), 1, fp);
                fread(data, sizeof(float), 1, fp);
                fread(data, sizeof(float), 1, fp);
                fread(data, sizeof(float), 1, fp);
                fread(data, sizeof(float), 1, fp);
                fread(data, sizeof(float), 1, fp);
            }
            break;
        default:
			{
				break; //兼容新地图 2016.10.20
				//return false;
			}
        }
    }
    return true;
}
//-------------------------------------------------------------
bool CMapData::PlaceTerrainObj(CTerrainObj* pTerrainObj)
{
    if (!pTerrainObj)
    {
        return false;
    }
    int nAmount = pTerrainObj->GetPartAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CTerrainObjPart* pPart = pTerrainObj->GetPartByIndex(i);
        if (!pPart)
        {
            return false;
        }
        SIZE sizePart = pPart->GetBase();
        for(int j = 0; j < sizePart.cy; j++)
        {
            for(int k = 0; k < sizePart.cx; k++)
            {
                CLayer* pLayer = pPart->GetLayer(k, j);
                CLayer* pNewLayer = CLayer::CreateNew();
                if (!pLayer || !pNewLayer)
                {
                    return false;
                }
                POINT posCell = pPart->GetPos();
                posCell.x -= k;
                posCell.y -= j;
                CCell* pCell = GetCell(posCell.x, posCell.y);
                if (!pCell)
                {
                    return false;
                }
                pNewLayer->dwMask = pLayer->dwMask;
                pNewLayer->nTerrian = pLayer->nTerrian;
                pNewLayer->nAltitude = pLayer->nAltitude + pCell->GetFloorAlt(m_set2Layer);
                pCell->AddLayer(m_set2Layer, pNewLayer);
            }
        }
    }
    return true;
}
//-------------------------------------------------------------
bool CMapData::DisplaceTerrainObj(CTerrainObj* pTerrainObj)
{
    CHECKF(pTerrainObj);
    int nAmount = pTerrainObj->GetPartAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CTerrainObjPart* pPart = pTerrainObj->GetPartByIndex(i);
        ASSERT(pPart);
        if (!pPart)
        {
            return false;
        }
        SIZE sizePart = pPart->GetBase();
        for(int j = 0; j < sizePart.cy; j++)
        {
            for(int k = 0; k < sizePart.cx; k++)
            {
                POINT posCell = pPart->GetPos();
                posCell.x -= k;
                posCell.y -= j;
                CCell* pCell = GetCell(posCell.x, posCell.y);
                ASSERT(pCell);
                if (!pCell)
                {
                    return false;
                }
                ASSERT(pCell->DelLayer(m_set2Layer));
            }
        }
    }
    return true;
}
//----------------------------------------------------//
void CMapData::ClearCell()
{
    /*	int nAmount = m_setCell.size();
    	for(int i = 0; i < nAmount; i++)
    	{
    		CCell* pCell = m_setCell[i];
    		SAFE_DELETE(pCell);
    	}
    */	m_setCell.clear();
}
//----------------------------------------------------//
void CMapData::ClearPassage()
{
    int nAmount =  m_setPassage.size();
    for(int i = 0; i < nAmount; i++)
    {
        PassageInfo* pInfo = m_setPassage[i];
        SAFE_DELETE(pInfo);
    }
    m_setPassage.clear();
}
//----------------------------------------------------//
void CMapData::ClearMapObj()
{
    int nAmount =  m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CTerrainObj* pInfo = m_setMapObj[i];
        SAFE_DELETE(pInfo);
    }
    m_setMapObj.clear();
}
//----------------------------------------------------//
void CMapData::ClearLayerSet()
{
    int nAmount =  m_set2Layer.size();
    for(int i = 0; i < nAmount; i++)
    {
        for(int j = 0; j < m_set2Layer[i].size(); j++)
        {
            if (m_set2Layer[i][j])
            {
                delete	m_set2Layer[i][j];
            }
        }
    }
    m_set2Layer.clear();
}
//----------------------------------------------------//
CCell*  CMapData::GetCell(int nIndex)
{
    int nAmount = m_setCell.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setCell[nIndex];
}
//----------------------------------------------------//
CCell*  CMapData::GetCell(int nCellX, int nCellY)
{
    return GetCell(POS2INDEX(nCellX, nCellY, m_sizeMap.cx, m_sizeMap.cy));
}
//----------------------------------------------------//
void CMapData::AddPassage(const PassageInfo* pInfo)
{
    CHECK(pInfo);
    POINT	posCell;
    posCell.x	= pInfo->nPosX;
    posCell.y	= pInfo->nPosY;
    AddPassage(posCell, pInfo->nIndex);
}
//----------------------------------------------------//
void CMapData::AddPassage(POINT posMap, int nIndex)
{
    // 兼容于地图编辑器
    {
        int nAmount =  m_setPassage.size();
        for(int i = 0; i < nAmount; i++)
        {
            PassageInfo* pInfo = m_setPassage[i];
            if (pInfo)
            {
                if ((pInfo->nPosX == posMap.x) && (pInfo->nPosY == posMap.y))
                {
                    pInfo->nIndex = nIndex;
                    return;
                }
            }
        }
    }
    PassageInfo* pInfo = new PassageInfo;
    if (pInfo)
    {
        pInfo->nPosX = posMap.x;
        pInfo->nPosY = posMap.y;
        pInfo->nIndex = nIndex;
        m_setPassage.push_back(pInfo);
    }
}
//----------------------------------------------------//
void CMapData::DelPassage(POINT posMap)
{
    int nAmount =  m_setPassage.size();
    for(int i = 0; i < nAmount; i++)
    {
        PassageInfo* pInfo = m_setPassage[i];
        if (pInfo)
        {
            if ((pInfo->nPosX == posMap.x) && (pInfo->nPosY == posMap.y))
            {
                SAFE_DELETE(pInfo);
                m_setPassage.erase(m_setPassage.begin() + i);
                // 兼容于编辑器	return ;
            }
        }
    }
}
//----------------------------------------------------//
int  CMapData::GetExitIndex(POINT posCell)			// return -1 : error
{
    int nAmount =  m_setPassage.size();
    for(int i = 0; i < nAmount; i++)
    {
        PassageInfo* pInfo = m_setPassage[i];
        if (pInfo)
        {
            if ((pInfo->nPosX == posCell.x) && (pInfo->nPosY == posCell.y))
            {
                return pInfo->nIndex;
            }
        }
    }
    return -1;
}
//----------------------------------------------------//
bool CMapData::AddMapObj(CTerrainObj* pObj)
{
    CHECKF(pObj);
    m_setMapObj.push_back(pObj);
    return PlaceTerrainObj(pObj);
}

bool CMapData::DelMapObj(int idx)
{
    CHECKF(idx >= 0 && idx < m_setMapObj.size());
    CTerrainObj* pObj = m_setMapObj[idx];
    if (pObj && DisplaceTerrainObj(pObj))
    {
        SAFE_DELETE(pObj);
        m_setMapObj.erase(m_setMapObj.begin() + idx);
        return true;
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////////////
// interface
///////////////////////////////////////////////////////////////////////////////////////
bool CMapData::AddTerrainItem(OBJID idOwner, int nCellX, int nCellY, OBJID idTerrainItemType)
{
    CHECKF(idTerrainItemType != ID_NONE);
    char	szKey[256];
    sprintf(szKey, "NpcType%u", idTerrainItemType / 10);
    CIniFile	ini("ini\\TerrainNpc.ini", szKey);
    char	szField[256];
    sprintf(szField, "Dir%u", idTerrainItemType % 10);
    char	szFileName[256];
    if (!ini.GetString(szFileName, szField, 256))
    {
        LOGERROR("Can't find TerrainNpc[%d] in ini\\TerrainNpc.ini", idTerrainItemType);
        return NULL;
    }
    CTerrainObj* pObj = CTerrainObj::CreateNew(szFileName, idOwner);
    if (!pObj)
    {
        return false;
    }
    POINT	posCell;
    posCell.x	= nCellX;
    posCell.y	= nCellY;
    pObj->SetPos(posCell);
    if (!AddMapObj(pObj))
    {
        SAFE_DELETE(pObj);
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMapData::DelTerrainItem(OBJID idOwner)
{
    for(int i = m_setMapObj.size() - 1; i >= 0; i--)
    {
        CTerrainObj* pObj = m_setMapObj[i];
        if (pObj && pObj->GetOwnerID() == idOwner)
        {
            if (!DelMapObj(i))		// 注意：如果一个OWNER有多个物件时，要先放关键物件，否则会部分删除。
            {
                return false;
            }
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
int  CMapData::GetMapWidth()
{
    return m_sizeMap.cx;
}

///////////////////////////////////////////////////////////////////////////////////////
int	 CMapData::GetMapHeight()
{
    return m_sizeMap.cy;
}

///////////////////////////////////////////////////////////////////////////////////////
CCell* CMapData::QueryCell(int nCellX, int nCellY)
{
    CHECKF(nCellX >= 0 && nCellX < m_sizeMap.cx);
    CHECKF(nCellY >= 0 && nCellY < m_sizeMap.cy);
    return GetCell(nCellX, nCellY);
}

///////////////////////////////////////////////////////////////////////////////////////
CCell* CMapData::QueryCell(POINT posCell)
{
    return QueryCell(posCell.x, posCell.y);
}

///////////////////////////////////////////////////////////////////////////////////////
int CMapData::GetPassage(int x, int y)
{
    for(int i = 0; i < m_setPassage.size(); i++)
    {
        if (m_setPassage[i]->nPosX == x && m_setPassage[i]->nPosY == y)
        {
            return m_setPassage[i]->nIndex;
        }
    }
    return PASSAGE_NONE;
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
IMapData* IMapData::CreateNew(int nMapDoc, LPCTSTR pszVersion/*=0*/)
{
    CHECKF(nMapDoc != ID_NONE);
    char	szField[256];
    sprintf(szField, "Map%d", nMapDoc);
    CIniFile	ini("ini\\gamemap.ini", szField);
    char	szFileName[256];
    if (!ini.GetString(szFileName, "File", 256))
    {
        return NULL;
    }
    IMapData*	pObj = CMapData::CreateNew(szFileName, pszVersion);
    if (!pObj)
    {
        return false;
    }
    return pObj;
}

