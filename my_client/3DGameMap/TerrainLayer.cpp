
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------//
// InterActiveLayer.cpp
//----------------------------------------------------//
#include "TerrainLayer.h"
#include "3DGameMap.h"
#include "Hero.h"
//----------------------------------------------------//
CTerrainLayer::CTerrainLayer()
{
    m_setPassage.clear();
    m_setRegion.clear();
}
//----------------------------------------------------//
CTerrainLayer::~CTerrainLayer()
{
    this->ClearPassage();
    this->ClearRegion();
}
//----------------------------------------------------//
CTerrainLayer* CTerrainLayer::CreateNew()
{
    CTerrainLayer* pLayer = new CTerrainLayer;
    return  pLayer;
}
//----------------------------------------------------//
void CTerrainLayer::Show(CMyPos posShow)
{
    // È¡HeroµÄÆÁÄ»×ø±ê×ø±ê
    //	posShow.x = posShow.x * m_bmpPuzzle.GetGrideSize() / CPuzzleBmp::GetNormalGrideSize();
    //	posShow.y = posShow.y * m_bmpPuzzle.GetGrideSize() / CPuzzleBmp::GetNormalGrideSize();
    m_bmpPuzzle.SetColor(g_objGameMap.GetARGB());
    m_bmpPuzzle.Show(-posShow.x, -posShow.y);
}
//----------------------------------------------------//
void CTerrainLayer::Process(void* pInfo)
{
}
//----------------------------------------------------//
int  CTerrainLayer::GetType()
{
    return LAYER_TERRAIN;
}
//----------------------------------------------------//
void CTerrainLayer::LoadPuzzle(char* pszFile)
{
    if (!pszFile)
    {
        return;
    }
    strcpy(m_szPuzzleFile, pszFile);
    m_bmpPuzzle.Load(pszFile);
}
//----------------------------------------------------//
void CTerrainLayer::ClearPassage()
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
void CTerrainLayer::AddPassage(CMyPos posMap, int nIndex)
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
void CTerrainLayer::DelPassage(CMyPos posMap)
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
                return;
            }
        }
    }
}
//----------------------------------------------------//
int  CTerrainLayer::GetExitIndex(CMyPos posCell)
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
void CTerrainLayer::LoadDataPassage(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount;
    fread(&nAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nAmount; i++)
    {
        PassageInfo infoPassage;
        fread(&infoPassage, sizeof(PassageInfo), 1, fp);
        CMyPos posCell;
        posCell.x = infoPassage.nPosX;
        posCell.y = infoPassage.nPosY;
        this->AddPassage(posCell, infoPassage.nIndex);
        CMyPos posWorld;
        g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
        g_objGameMap.Add3DMapEffect(posWorld, "Exit");
    }
}
//----------------------------------------------------//
void CTerrainLayer::LoadTextPassage(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount;
    fscanf(fp, "PassageAmount=%d\n", &nAmount);
    for(int i = 0; i < nAmount; i++)
    {
        CMyPos posPassage;
        int nPassageIndex;
        int nIndex;
        fscanf(fp, "Passage%d=[%d,%d,%d]\n", &nIndex, &posPassage.x,
               &posPassage.y, &nPassageIndex);
        this->AddPassage(posPassage, nPassageIndex);
    }
}
//----------------------------------------------------//
void CTerrainLayer::SaveTextPassage(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    // amount ...
    int nAmount = m_setPassage.size();
    fprintf(fp, "PassageAmount=%d\n", nAmount);
    for(int i = 0; i < nAmount; i++)
    {
        PassageInfo* pInfo = m_setPassage[i];
        if (pInfo)
        {
            fprintf(fp, "Passage%d=[%d,%d,%d]\n", i, pInfo->nPosX, pInfo->nPosY, pInfo->nIndex);
        }
    }
}
//----------------------------------------------------//
void CTerrainLayer::GetPuzzleSize(CMySize& infoSize)
{
    m_bmpPuzzle.GetSize(infoSize);
}
//----------------------------------------------------//
void CTerrainLayer::SaveDataPassage(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    // amount ...
    int nAmount = m_setPassage.size();
    fwrite(&nAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nAmount; i++)
    {
        PassageInfo* pInfo = m_setPassage[i];
        if (pInfo)
        {
            fwrite(pInfo, sizeof(PassageInfo), 1, fp);
        }
    }
}
//----------------------------------------------------//
int CTerrainLayer::GetPuzzleGrideSize()
{
    return m_bmpPuzzle.GetGrideSize();
}
//----------------------------------------------------//
void CTerrainLayer::ClearRegion()
{
    int nAmount = m_setRegion.size();
    for(int i = 0; i < nAmount; i ++)
    {
        C2DMapRegion* pRegion = m_setRegion[i];
        SAFE_DELETE(pRegion);
    }
    m_setRegion.clear();
}
//----------------------------------------------------//
C2DMapRegion* CTerrainLayer::GetRegion(CMyPos posRegion, int nType)
{
    int nAmount = m_setRegion.size();
    for(int i = 0; i < nAmount; i ++)
    {
        C2DMapRegion* pRegion = m_setRegion[i];
        if (pRegion)
        {
            RegionInfo* pInfo = pRegion->GetInfo();
            if (pInfo->m_posCell.x == posRegion.x && pInfo->m_posCell.y == posRegion.y && pInfo->m_dwType == nType)
            {
                return 	pRegion;
            }
        }
    }
    return NULL;
}
//----------------------------------------------------//
void CTerrainLayer::DelRegion(CMyPos posRegion, int nType)
{
    int nAmount = m_setRegion.size();
    for(int i = 0; i < nAmount; i ++)
    {
        C2DMapRegion* pRegion = m_setRegion[i];
        if (pRegion)
        {
            RegionInfo* pInfo = pRegion->GetInfo();
            if (pInfo->m_posCell.x == posRegion.x && pInfo->m_posCell.y == posRegion.y && pInfo->m_dwType == nType)
            {
                SAFE_DELETE(pRegion);
                m_setRegion.erase(m_setRegion.begin() + i);
                return;
            }
        }
    }
}
//----------------------------------------------------//
void CTerrainLayer::AddRegion(C2DMapRegion* pRegion)
{
    m_setRegion.push_back(pRegion);
}
//----------------------------------------------------//
void CTerrainLayer::ShowRegion(int nType)
{
    int nAmount = m_setRegion.size();
    for(int i = 0; i < nAmount; i ++)
    {
        C2DMapRegion* pRegion = m_setRegion[i];
        RegionInfo* pInfo = pRegion->GetInfo();
        if (pRegion && pInfo && pInfo->m_dwType == nType)
        {
            pRegion->Show(NULL);
        }
    }
}
//----------------------------------------------------//
void CTerrainLayer::LoadDataRegion(FILE* fp)
{
    /*
    	MYASSERT(fp);
    	// load amount
    	DWORD dwAmount = 0;
    	fread(&dwAmount, sizeof(DWORD), 1, fp);
    	// load region Info
    	for(int i = 0; i < dwAmount; i ++)
    	{
    		RegionInfo infoRegion;
    		fread(&infoRegion, sizeof(RegionInfo), 1, fp);
    		C2DMapRegion* pRegion = C2DMapRegion::CreateNew(&infoRegion);
    		this->AddRegion(pRegion);
    	}*/
}
//----------------------------------------------------//
void CTerrainLayer::LoadTextRegion(FILE* fp)
{
    /*
    	MYASSERT(fp);
    	int nAmount;
    	if ( 1 != fscanf(fp, "RegionAmount=%d\n", &nAmount))
    		return;
    	for(int i = 0; i < nAmount; i++)
    	{
    		RegionInfo infoRegion;
    		fprintf(fp, "Region%d=[%u,%u,%u,%u,%u,%u,%u,%u,%u,%s]\n", i,
    			&infoRegion.m_posCell.x, &infoRegion.m_posCell.y,
    			&infoRegion.m_dwCx, &infoRegion.m_dwCy, &infoRegion.m_dwType, &infoRegion.m_dwData[0],
    			&infoRegion.m_dwData[1], &infoRegion.m_dwData[2], &infoRegion.m_dwData[3], infoRegion.m_szString);
    		C2DMapRegion* pRegion = C2DMapRegion::CreateNew(&infoRegion);
    		this->AddRegion(pRegion);

    	}*/
}
//----------------------------------------------------//
void CTerrainLayer::SaveDataRegion(FILE* fp)
{
    /*
    	MYASSERT(fp);
    	// save amount
    	DWORD dwAmount = m_setRegion.size();
    	fwrite(&dwAmount, sizeof(DWORD), 1, fp);
    	//save region info
    	for(int i = 0; i < dwAmount; i ++)
    	{
    		C2DMapRegion* pRegion = m_setRegion[i];
    		if (pRegion)
    		{
    			RegionInfo* pInfo = pRegion->GetInfo();
    			fwrite(pInfo, sizeof(RegionInfo), 1, fp);
    		}
    	}*/
}
//----------------------------------------------------//
void CTerrainLayer::SaveTextRegion(FILE* fp)
{
    /*
    	MYASSERT(fp);
    	// save amount
    	int nAmount = m_setRegion.size();
    	fprintf(fp, "RegionAmount=%d\n", nAmount);
    	for(int i = 0; i < nAmount; i++)
    	{
    		PassageInfo* pInfo = m_setPassage[i];
    		C2DMapRegion* pRegion = m_setRegion[i];
    		if (pRegion)
    		{
    			RegionInfo* pInfo = pRegion->GetInfo();
    			fprintf(fp, "Region%d=[%u,%u,%u,%u,%u,%u,%u,%u,%u,%s]\n", i,
    				pInfo->m_posCell.x, pInfo->m_posCell.y,
    					pInfo->m_dwCx, pInfo->m_dwCy, pInfo->m_dwType, pInfo->m_dwData[0],
    						pInfo->m_dwData[1], pInfo->m_dwData[2], pInfo->m_dwData[3], pInfo->m_szString);
    		}
    	}*/
}
//----------------------------------------------------//
int  CTerrainLayer::GetRegionAmount()
{
    return m_setRegion.size();
}
//----------------------------------------------------//
C2DMapRegion* CTerrainLayer::GetRegionByINdex(int nIndex)
{
    int nAmount = m_setRegion.size();
    if (nIndex < 0 || nIndex >= nAmount)
    {
        return NULL;
    }
    else
    {
        return m_setRegion[nIndex];
    }
}
//----------------------------------------------------//
