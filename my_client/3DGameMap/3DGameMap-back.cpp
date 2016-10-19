
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
//---------------------------------------------------------------------------//
// Gamemap.cpp
//---------------------------------------------------------------------------//
// header ...
#pragma warning(disable:4786)

#include "3DGameMap.h"
#include "TerrainLayer.h"
#include "Interactivelayer.h"
#include "SurfaceLayer.h"
#include "SceneLayer.h"
#include "FloorLayer.h"
#include "SkyLayer.h"
#include "Hero.h"
#include "GamePlayerSet.h"
#include "2DMapPuzzleObj.h"
#include "GameDataSet.h"
#include "3DRole.h"
#include "InfoViewManager.h"
#include "Gamemsg.h"
#include "3DMagicMapItem.h"
#include "3DMapItem.h"
//---------------------------------------------------------------------------//
// gobal ...
CGameMap g_objGameMap;
#ifdef _ANALYSIS_ON
BOOL g_bShowAnalysisInfo = false;
#endif
//---------------------------------------------------------------------------//
CGameMap::CGameMap()
{
    m_setMapLayer.clear();
    CTerrainLayer* pTerrainLayer =  CTerrainLayer::CreateNew();
    this->AddLayer(pTerrainLayer);
    CFloorLayer* pFloorLayer = CFloorLayer::CreateNew();
    this->AddLayer(pFloorLayer);
    CSurfaceLayer* pSurfaceLayer =  CSurfaceLayer::CreateNew();
    this->AddLayer(pSurfaceLayer);
    CInteractiveLayer* pInteractiveLayer = CInteractiveLayer::CreateNew();
    this->AddLayer(pInteractiveLayer);
    CSkyLayer* pSkyLayer = CSkyLayer::CreateNew();
    this->AddLayer(pSkyLayer);
    //for a*
    m_setPath.clear();
    m_setCellAStar.clear();
    m_setNode.clear();
    m_setOpen.clear();
    m_nCloseNodeAmount = 0;
    m_posView.x = 0;
    m_posView.y = 0;
    //CMyBitmap::GameCameraCreate();
    m_pCellInfo = NULL;
    m_nPathMode = _ASTAR_CLOSE;
    m_idMap = ID_NONE;
    m_idDoc = ID_NONE;
    m_bShowMiniMap = true;
    m_posViewOffset.x = 0;
    m_posViewOffset.y = 0;
    m_bShowMapObjInfo = false;
    m_dwMapARGB = 0xffffffff;
    this->SetScale(_NORMAL_SCALE);
    this->CreateRegionData();
    m_setIndexLayerInfo.clear();
    m_bShowRegionEffect = false;
    m_bShowGobalRegionEffect = false;
    m_bShowExp = false;
    m_dwTimeExpBegin = ::TimeGet();
    m_dwShowExp = 0;
    m_bShowScreenInfo	=	false;
    m_bBrume = false;
    this->CreateRegionEffectInfo();
}
//---------------------------------------------------------------------------//
CGameMap::~CGameMap()
{
    m_setIndexLayerInfo.clear();
    this->ClearNodeSet();
    this->ClearOpenSet();
    this->ClearCloseSet();
    this->ClearPathSet();
    m_objTerrainEffectManager.Destroy();
    CMyBitmap::GameCameraDestroy();
    this->DestroyRegionData();
    this->ClearMapLayer();
    this->DestroyCell();
    this->ClearRes ();
    this->DestroyRegionEffectInfo();
}
//---------------------------------------------------------------------------//
void CGameMap::Reset()
{
    m_objMagicEffectManager.Destroy();
    m_objItemManager.Destroy();
    m_objTerrainObjManager.Destroy();
    m_objTerrainEffectManager.Destroy();
    this->ClearMapLayer();
    this->ClearNodeSet();
    this->ClearOpenSet();
    this->ClearCloseSet();
    this->ClearPathSet();
    this->ClearRes ();
    CTerrainLayer* pTerrainLayer =  CTerrainLayer::CreateNew();
    this->AddLayer(pTerrainLayer);
    CFloorLayer* pFloorLayer = CFloorLayer::CreateNew();
    this->AddLayer(pFloorLayer);
    CSurfaceLayer* pSurfaceLayer =  CSurfaceLayer::CreateNew();
    this->AddLayer(pSurfaceLayer);
    CInteractiveLayer* pInteractiveLayer = CInteractiveLayer::CreateNew();
    this->AddLayer(pInteractiveLayer);
    CSkyLayer* pSkyLayer = CSkyLayer::CreateNew();
    this->AddLayer(pSkyLayer);
    //for a*
    m_setPath.clear();
    m_setCellAStar.clear();
    m_setNode.clear();
    m_setOpen.clear();
    m_nCloseNodeAmount = 0;
    strcpy(m_szRegionName, "");
    m_dwRegionNameChangeBegin = ::TimeGet();
    m_setIndexLayerInfo.clear();
    this->ClearRes();
    m_nCurRegionEffectIndex = -1;
    m_dwTimeRegionEffect = 0;
}
//---------------------------------------------------------------------------//
void CGameMap::ClearMapLayer()
{
    int nAmount = m_setMapLayer.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        SAFE_DELETE (pLayer);
    }
    m_setMapLayer.clear();
}
//---------------------------------------------------------------------------//
void CGameMap::Show(BOOL bMapItemInfo)
{
    this->SetShowMapObjInfoFlag(bMapItemInfo);
    CMySize sizeScr = {m_nRealScreenWidth, m_nRealScreenHeight};
    CMyBitmap::GameCameraBuild(sizeScr);
    CMyPos posBgViewport;
    this->GetBgViewport(posBgViewport.x, posBgViewport.y);
    int nAmount = m_setMapLayer.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        if(pLayer)
        {
            pLayer->Show(posBgViewport);
        }
    }
    m_InfoViewManager.Show();
    m_InfoViewManager.Clear();
    // show mini map
    //	if (m_bShowMiniMap)
    //	{
    //		m_objRadar.Show(m_idDoc);
    //	}
    //	g_objHero.ShowArrow(sizeMiniMap.iWidth);
    CMyPos posTeam;
    posTeam.y = 128;
    //g_objHero.ShowTeam(posTeam);
    g_objHero.ShowSynWarReport();
    this->ShowRegionName();
    this->ShowExp();
    g_objHero.m_objDiceManager.Show();
    this->ShowSnow();
    this->ShowBrume();
    /*	char szDir[64];
    	sprintf(szDir, "DIR:%d ACTION:%d", g_objHero.GetDir(), g_objHero.GetActionType());
    	CMyBitmap::ShowString(100, 100, 0x00ffff00, szDir);*/
#ifdef _ARHUN_DEBUG
    //this->ShowPath();
    //this->ShowCell(false, true, false, false);
#endif
#ifdef _ANALYSIS_ON
    if(g_bShowAnalysisInfo)
    {
        char szTemp[256];
        DWORD dwLoadTotal = g_dwBitmapLoadTimeFrame + g_dw3DObjLoadTimeFrame + g_dw3DMotionLoadTimeFrame + g_dw3DTextureLoadTimeFrame + g_dw3DEffectLoadTimeFrame;
        static DWORD dwLoadBusy = 0;
        if(dwLoadTotal > 30)
        {
            sprintf(szTemp, "数据加载引发减祯:FrameCost:%u, Total:%u, Bitmap:%u, Phy:%u, Motion%u, Texture:%u, Effect:%u", g_dwFrameInterval, dwLoadTotal, g_dwBitmapLoadTimeFrame, g_dw3DObjLoadTimeFrame, g_dw3DMotionLoadTimeFrame, g_dw3DTextureLoadTimeFrame, g_dw3DEffectLoadTimeFrame);
            ::LogMsg("//-------------------------------------------------------------------");
            ::LogMsg(szTemp);
            ::LogMsg("//-------------------------------------------------------------------");
            dwLoadBusy ++;
        }
        sprintf(szTemp, "数据加载分析:(数据加载耗时超过30毫秒次数%u)", dwLoadBusy);
        CMyBitmap::ShowString(10, 80, 0xffff00ff, szTemp);
        sprintf(szTemp, "Bitmap加载:数量%u, 总耗时%u, 平均耗时%u, 峰值%u, 祯间峰值%u", g_dwBitmapLoadAmount, g_dwBitmapLoadTime, g_dwBitmapLoadTime / (g_dwBitmapLoadAmount + 1), g_dwBitmapLoadTimeMax, g_dwBitmapLoadTimeFrame);
        CMyBitmap::ShowString(10, 100, 0xffffff00, szTemp);
        sprintf(szTemp, "Phy加载:数量%u, 总耗时%u, 平均耗时%u, 峰值%u, 祯间峰值%u", g_dw3DObjLoadAmount, g_dw3DObjLoadTime, g_dw3DObjLoadTime / (g_dw3DObjLoadAmount + 1), g_dw3DObjLoadTimeMax, g_dw3DObjLoadTimeFrame);
        CMyBitmap::ShowString(10, 120, 0xffffff00, szTemp);
        sprintf(szTemp, "Motion加载:数量%u, 总耗时%u, 平均耗时%u, 峰值%u, 祯间峰值%u", g_dw3DMotionLoadAmount, g_dw3DMotionLoadTime, g_dw3DMotionLoadTime / (g_dw3DMotionLoadAmount + 1), g_dw3DMotionLoadTimeMax, g_dw3DMotionLoadTimeFrame);
        CMyBitmap::ShowString(10, 140, 0xffffff00, szTemp);
        sprintf(szTemp, "Texture加载:数量%u, 总耗时%u, 平均耗时%u, 峰值%u, 祯间峰值%u", g_dw3DTextureLoadAmount, g_dw3DTextureLoadTime, g_dw3DTextureLoadTime / (g_dw3DTextureLoadAmount + 1), g_dw3DTextureLoadTimeMax, g_dw3DTextureLoadTimeFrame);
        CMyBitmap::ShowString(10, 160, 0xffffff00, szTemp);
        sprintf(szTemp, "Effect加载:数量%u, 总耗时%u, 平均耗时%u, 峰值%u, 祯间峰值%u", g_dw3DEffectLoadAmount, g_dw3DEffectLoadTime, g_dw3DEffectLoadTime / (g_dw3DEffectLoadAmount + 1), g_dw3DEffectLoadTimeMax, g_dw3DEffectLoadTimeFrame);
        CMyBitmap::ShowString(10, 180, 0xffffff00, szTemp);
        g_dwBitmapLoadTimeFrame = g_dw3DObjLoadTimeFrame = g_dw3DMotionLoadTimeFrame = g_dw3DTextureLoadTimeFrame = g_dw3DEffectLoadTimeFrame = 0;
        CMyBitmap::ShowString(10, 200, 0xffff00ff, "文本搜索分析:");
        sprintf(szTemp, "搜索次数%u，搜索行数%u，祯间搜索次数%u，祯间搜索行数%u", g_dwIniGetAmount, g_dwIniSearchLineAmount, g_dwIniGetAmountFrame, g_dwIniSearchLineAmountFrame);
        g_dwIniGetAmountFrame			=	0;
        g_dwIniSearchLineAmountFrame	=	0;
        CMyBitmap::ShowString(10, 220, 0xffffff00, szTemp);
        CMyBitmap::ShowString(10, 240, 0xffff00ff, "GamedataSet分析:");
        DWORD dw3DObj, dw3DMotion, dw3DWMotion, dwTexture, dw3DEffect, dwDataAni;
        g_objGameDataSet.GetObjAmount(dw3DObj, dw3DMotion, dw3DWMotion, dwTexture, dw3DEffect, dwDataAni);
        sprintf(szTemp, "物件数量:3DObj %u, 3DMotion %u, 3DWMotion %u, 3DTexture %u, 3DEffect %u, DataAni %u", dw3DObj, dw3DMotion, dw3DWMotion, dwTexture, dw3DEffect, dwDataAni);
        CMyBitmap::ShowString(10, 260, 0xffffff00, szTemp);
        sprintf(szTemp, "物件祯搜索数量:3DObj %u, 3DMotion %u, 3DWMotion %u, 3DTexture %u, 3DEffect %u, DataAni %u", g_dw3DObj, g_dw3DMotion, g_dw3DWMotion, g_dwTexture, g_dw3DEffect, g_dwDataAni);
        CMyBitmap::ShowString(10, 280, 0xffffff00, szTemp);
        g_dw3DObj = 0, g_dw3DMotion = 0, g_dw3DWMotion = 0, g_dwTexture = 0, g_dw3DEffect = 0, g_dwDataAni = 0;
        CMyBitmap::ShowString(10, 300, 0xffff00ff, "显示分析:");
        if(g_dw3DRoleDrawAmount == 0) { g_dw3DRoleDrawAmount = 1; }
        if(g_dw3DRoleDrawAmountFrame == 0) { g_dw3DRoleDrawAmountFrame = 1; }
        sprintf(szTemp, "3DRole: 总数量%u，总耗时%u，均值%u，祯间数量%u，祯间耗时%u，祯间均值%u，峰值%u", g_dw3DRoleDrawAmount, g_dw3DRoleDrawTime, g_dw3DRoleDrawTime / g_dw3DRoleDrawAmount, g_dw3DRoleDrawAmountFrame, g_dw3DRoleDrawTimeFrame, g_dw3DRoleDrawTimeFrame / g_dw3DRoleDrawAmountFrame, g_dw3DRoleDrawTimeMax);
        g_dw3DRoleDrawAmountFrame  = g_dw3DRoleDrawTimeFrame = 0;
        CMyBitmap::ShowString(10, 320, 0xffffff00, szTemp);
        sprintf(szTemp, "Gamemsg显示TimeCost: %u", g_dwTxtMsgTimeCost);
        CMyBitmap::ShowString(10, 340, 0xffffff00, szTemp);
        g_dwTxtMsgTimeCost = 0;
        sprintf(szTemp, "大贴图数量: %u", g_dwBigTexture);
        CMyBitmap::ShowString(10, 360, 0xffff00ff, szTemp);
    }
#endif
    sizeScr.iWidth = _SCR_WIDTH;
    sizeScr.iHeight = _SCR_HEIGHT;
    CMyBitmap::ClearBuffer(true, false, 0x0);
    CMyBitmap::GameCameraBuild(sizeScr);
    this->ShowScreenInfo();
}

//---------------------------------------------------------------------------//
void CGameMap::ShowMiniMap(CMySize& sizeMap)
{
    const char szAniFile[] = "ani/MiniMap.ani";
    char szAniName[256] = "";
    sprintf(szAniName, "%u", m_idDoc);
}
//---------------------------------------------------------------------------//
void CGameMap::ProcessManager()
{
    m_objTerrainObjManager.Process();
    m_objItemManager.Process();
    return;
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(pLayer)
    {
        // 判断删除
        CMyPos posHero = g_objHero.GetAlignPos();
        int nAmount = pLayer->GetObjAmount();
        for(int i = nAmount - 1; i >= 0; i --)
        {
            CMapObj* pObj = pLayer->GetObjByIndex(i);
            if(pObj && (pObj->GetObjType() == MAP_3DMAGICMAPITEM || pObj->GetObjType() == MAP_3DITEM
                        || pObj->GetObjType() == MAP_2DITEM))
            {
                CMyPos posCell;
                pObj->GetPos(posCell);
                if(g_objGameMap.Outof9Block(posHero, posCell))
                {
                    pLayer->DelObj(pObj);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------//

void CGameMap::Process(void* pInfo)
{
    // ToDo
    m_objMagicEffectManager.Process();
    m_objTerrainEffectManager.Process();
    int nAmount = m_setMapLayer.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        if(pLayer)
        {
            pLayer->Process(NULL);
        }
    }
    m_nRealScreenWidth = _SCR_WIDTH * _NORMAL_SCALE / m_nScale;
    m_nRealScreenHeight = _SCR_HEIGHT * _NORMAL_SCALE / m_nScale;
}
//---------------------------------------------------------------------------//
void CGameMap::AddLayer(CMapLayer* pLayer, int nIndex)
{
    if(!pLayer)
    {
        return;
    }
    int nAmount = m_setMapLayer.size();
    if((nIndex < 0) || (nIndex >= nAmount))
    {
        m_setMapLayer.push_back(pLayer);
        return;
    }
    else
    {
        m_setMapLayer.insert(m_setMapLayer.begin() + nIndex, 1, pLayer);
    }
}
//---------------------------------------------------------------------------//
CTerrainLayer* CGameMap::GetTerrainLayer()
{
    int nAmount = m_setMapLayer.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        if(pLayer)
        {
            if(pLayer->GetType() == CMapLayer::LAYER_TERRAIN)
            {
                return (CTerrainLayer*)pLayer;
            }
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------//
CInteractiveLayer* CGameMap::GetInteractiveLayer()
{
    int nAmount = m_setMapLayer.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        if(pLayer)
        {
            if(pLayer->GetType() == CMapLayer::LAYER_INTERACTIVE)
            {
                return (CInteractiveLayer*)pLayer;
            }
        }
    }
    return NULL;
}

CFloorLayer* CGameMap::GetFloorLayer()
{
    int nAmount = m_setMapLayer.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        if(pLayer)
        {
            if(pLayer->GetType() == CMapLayer::LAYER_FLOOR)
            {
                return (CFloorLayer*)pLayer;
            }
        }
    }
    return NULL;
}
CSkyLayer* CGameMap::GetSkyLayer()
{
    int nAmount = m_setMapLayer.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = m_setMapLayer[i];
        if(pLayer)
        {
            if(pLayer->GetType() == CMapLayer::LAYER_SKY)
            {
                return (CSkyLayer*)pLayer;
            }
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------//
void CGameMap::World2Cell(int iWorldX, int iWorldY, int& iCellX, int& iCellY)
{
    iWorldX	-= m_posOrigin.x;
    iWorldY	-= m_posOrigin.y;
    double dWorldX  = (double)iWorldX;
    double dWorldY  = (double)iWorldY;
    double dCellWidth   = (double)_CELL_WIDTH;
    double dCellHeight  = (double)_CELL_HEIGHT;
    double dTemp0   = (1.0 * dWorldX) / (1.0 * dCellWidth) + (1.0 * dWorldY) / (1.0 * dCellHeight);
    double dTemp1   = (1.0 * dWorldY) / (1.0 * dCellHeight) - (1.0 * dWorldX) / (1.0 * dCellWidth);
    iCellX  = Double2Int(dTemp0);
    iCellY  = Double2Int(dTemp1);
}
//---------------------------------------------------------------------------//
void CGameMap::World2Screen(int iWorldX, int iWorldY, int& iScrX, int& iScrY)
{
    iScrX	= iWorldX - m_posView.x;
    iScrY	= iWorldY - m_posView.y;
}
//---------------------------------------------------------------------------//
void CGameMap::Cell2World(int iCellX, int iCellY, int& iWorldX, int& iWorldY)
{
    iWorldX	= _CELL_WIDTH * (iCellX - iCellY) / 2 + m_posOrigin.x;
    iWorldY	= _CELL_HEIGHT * (iCellX + iCellY) / 2 + m_posOrigin.y;
}
//---------------------------------------------------------------------------//
void CGameMap::Cell2Screen(int iCellX, int iCellY, int& iScreenX, int& iScreenY)
{
    iScreenX = _CELL_WIDTH * (iCellX - iCellY) / 2 + m_posOrigin.x - m_posView.x;
    iScreenY = _CELL_HEIGHT * (iCellX + iCellY) / 2 + m_posOrigin.y - m_posView.y;
}
//---------------------------------------------------------------------------//
void CGameMap::Screen2World(int iScrX, int iScrY, int& iWorldX, int& iWorldY)
{
    iWorldX =   m_posView.x +  iScrX;
    iWorldY =   m_posView.y +  iScrY;
}
//---------------------------------------------------------------------------//
void CGameMap::Screen2Cell(int iScrX, int iScrY, int& iCellX, int& iCellY)
{
    int iWorldX, iWorldY;
    iWorldX	= iScrX + m_posView.x;
    iWorldY	= iScrY + m_posView.y;
    World2Cell(iWorldX, iWorldY, iCellX, iCellY);
    if(iCellX < 0)
    {
        iCellX	= 0;
    }
    else if(iCellX >= this->m_sizeMap.iWidth)
    {
        iCellX	= this->m_sizeMap.iWidth - 1;
    }
    if(iCellY < 0)
    {
        iCellY	= 0;
    }
    else if(iCellY >= this->m_sizeMap.iHeight)
    {
        iCellY	= this->m_sizeMap.iHeight - 1;
    }
    return;
}
//---------------------------------------------------------------------------//
void	CGameMap::Cell2Map( int nCellX, int nCellY, int& nMapX, int& nMapY )
{
    int nWorldX, nWorldY, nBgX, nBgY ;
    g_objGameMap.Cell2World( nCellX, nCellY, nWorldX, nWorldY ) ;
    g_objGameMap.World2Bg( nWorldX, nWorldY, nBgX, nBgY ) ;
    CMySize BgSize ;
    g_objGameMap.GetBgSize( BgSize ) ;
    nMapX = nBgX ;
    nMapY = BgSize.iHeight - nBgY ;
    nMapY /= 32 ;
    nMapX /= 64 ;
}
//---------------------------------------------------------------------------//
void	CGameMap::Map2Cell( int nMapX, int nMapY, int& nCellX, int& nCellY )
{
    int nWorldX, nWorldY, nBgX, nBgY ;
    nBgX = nMapX * 64 ;
    nBgY = nMapY * 32 ;
    CMySize BgSize ;
    g_objGameMap.GetBgSize( BgSize ) ;
    nBgY = BgSize.iHeight - nBgY ;
    g_objGameMap.Bg2World( nBgX, nBgY, nWorldX, nWorldY ) ;
    g_objGameMap.World2Cell( nWorldX, nWorldY, nCellX, nCellY ) ;
}
//---------------------------------------------------------------------------//
void CGameMap::GetMapSize(CMySize& infoSize)
{
    infoSize.iWidth = m_sizeMap.iWidth;
    infoSize.iHeight = m_sizeMap.iHeight;
}
//---------------------------------------------------------------------------//
void CGameMap::SetMapSize(CMySize infoSize)
{
    // set info ...
    m_sizeMap.iWidth        = infoSize.iWidth;
    m_sizeMap.iHeight       = infoSize.iHeight;
    m_posOrigin.x	= _CELL_WIDTH * m_sizeMap.iWidth / 2;
    m_posOrigin.y	= _CELL_HEIGHT / 2;
    CMySize sizeBmp;
    this->GetMapSize(sizeBmp);
    CMyPos posCenter;
    posCenter.x         = m_sizeMap.iWidth * _CELL_WIDTH / 2;
    posCenter.y         = m_sizeMap.iHeight * _CELL_HEIGHT / 2;
    m_szieWorld.iWidth = m_sizeMap.iWidth * _CELL_WIDTH;
    m_szieWorld.iHeight = m_sizeMap.iHeight * _CELL_HEIGHT;
    this->CreateCell();
    m_setCellAStar.clear();
    this->ClearNodeSet();
}
//---------------------------------------------------------------------------//
void CGameMap::GetWorldSize(CMySize& infoSize)
{
    infoSize = m_szieWorld;
}

//---------------------------------------------------------------------------//
void CGameMap::GetViewPos(CMyPos& posShow)
{
    posShow = m_posView;
}
//---------------------------------------------------------------------------//
CellInfo* CGameMap::GetCell(int iPosX, int iPosY)
{
    if(iPosX >= m_sizeMap.iWidth)
    {
        return NULL;
    }
    if(iPosY >= m_sizeMap.iHeight)
    {
        return NULL;
    }
    if(iPosX < 0)
    {
        return NULL;
    }
    if(iPosY < 0)
    {
        return NULL;
    }
    /*
    if(m_sizeMap.iWidth*iPosY+iPosX < 0)
    return NULL;
    if(m_sizeMap.iWidth*iPosY+iPosX >= m_sizeMap.iWidth*m_sizeMap.iHeight)
    return NULL;
    	*/
    if(!m_pCellInfo)
    {
        return NULL;
    }
    return m_pCellInfo + m_sizeMap.iWidth * iPosY + iPosX;
}
//---------------------------------------------------------------------------//
int  CGameMap::GetExitIndex(CMyPos posCell)
{
    CTerrainLayer* pLayer = this->GetTerrainLayer();
    if(pLayer)
    {
        return pLayer->GetExitIndex(posCell);
    }
    return -1;
}

//---------------------------------------------------------------------------//
BOOL CGameMap::Create(DWORD idDoc)
{
    // load map
    char szSection[256] = "";
    sprintf(szSection, "Map%u", idDoc);
    char szMapFile[_MAX_FNAME] = "";
    MYASSERT (::IniStrGet("ini/GameMap.ini", szSection, "File", szMapFile));
    MYASSERT( ::IniStrGet("ini/GameMap.ini", szSection, "Name", m_szMapName ) ) ;
    MYASSERT( ::IniDataGet("ini/GameMap.ini", szSection, "Snow", m_bSnow ) ) ;
    int dwAlpha = 255, dwRed = 255, dwGreen = 255, dwBlue = 255;
    ::IniDataGet("ini/GameMap.ini", szSection, "Alpha", dwAlpha);
    ::IniDataGet("ini/GameMap.ini", szSection, "Red", dwRed);
    ::IniDataGet("ini/GameMap.ini", szSection, "Green", dwGreen);
    ::IniDataGet("ini/GameMap.ini", szSection, "Blue", dwBlue);
    DWORD dwColor = (dwAlpha << 24) + (dwRed << 16) + (dwGreen << 8) + dwBlue;
    g_objGameMap.SetARGB(dwColor);
    if (!this->LoadDataMap(szMapFile))
    {
        ::ErrorMsg("game map %s load failed.", szMapFile);
        return false;
    }
    g_objPlayerSet.DelAllPlayer(true);
    g_objHero.m_objBoothManager.Close();
    this->AddInteractiveObj(&g_objHero);
    int nAmount = g_objPlayerSet.GetPlayerAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
        if(pPlayer)
        {
            this->AddInteractiveObj(pPlayer);
        }
    }
    m_idDoc = idDoc;
    ::PostCmd(CMD_CHANGE_MAP);
    g_objHero.LoadMusicInfo();
    return true;
}

//---------------------------------------------------------------------------//
void CGameMap::SaveTextMap(char* pszFileName)
{
    if(!pszFileName)
    {
        return;
    }
    FILE* fp = fopen(pszFileName, "w");
    if(!fp)
    {
        return;
    }
    char szCurDir[_MAX_PATH];
    char szFileName[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szCurDir);
    CTerrainLayer* pTerrainLayer = this->GetTerrainLayer();
    MYASSERT(pTerrainLayer);
    char szPuzzleFile[_MAX_PATH] = "";
    strcpy(szPuzzleFile, pTerrainLayer->GetPuzleFileName());
    int nLen = strlen(szCurDir);
    for(int k = 0; k < _MAX_PATH - nLen - 1; k++)
    {
        szFileName[k] =  szPuzzleFile[nLen+k+1];
    }
    // save puzzle file ...
    fprintf(fp, "PuzzleFile=%s\n", szFileName);
    // save map size ...
    CMySize infoSize;
    g_objGameMap.GetMapSize(infoSize);
    fprintf(fp, "Width=%d\n", infoSize.iWidth);
    fprintf(fp, "Height=%d\n", infoSize.iHeight);
    int i, j;
    // save mask ...
    // save terrain ...
    // save altitude ..
    for(i = 0; i < infoSize.iHeight; i++)
    {
        char szTemp[1024];
        for(j = 0; j < infoSize.iWidth; j++)
        {
            CellInfo* pCellInfo = g_objGameMap.GetCell(j, i);
            LayerInfo* pLayerInfo = &pCellInfo->m_infoLayer;
            if(pLayerInfo)
            {
                sprintf(szTemp, "Cell[%d,%d]=[%u,%d,%d]",
                        j, i, pLayerInfo->usMask, pLayerInfo->usTerrain,
                        pLayerInfo->sAltitude);
                fprintf(fp, "%s\n", szTemp);
            }
        }
    }
    pTerrainLayer->SaveTextPassage(fp);
    CInteractiveLayer* pInteractiveLayer = this->GetInteractiveLayer();
    if(pInteractiveLayer)
    {
        pInteractiveLayer->SaveTextLayer(fp);
    }
    // save others ...
    int nAmount = this->GetLayerAmount();
    // write amount ...
    fprintf(fp, "========Other Layers Amount=%d========\n", this->GetLayerAmount() - 3);
    for(i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = this->GetLayerByIndex(i);
        if(!pLayer)
        {
            continue;
        }
        switch(pLayer->GetType())
        {
        case CMapLayer::LAYER_SCENE:
            {
                // save index type
                fprintf(fp, "========Index=%d, Type=%d========\n", i, pLayer->GetType());
                CSceneLayer* pSceneLayer = (CSceneLayer*)pLayer;
                pSceneLayer->SaveTextLayer(fp);
            }
            break;
        }
    }
    //pTerrainLayer->SaveTextRegion(fp);
    fclose(fp);
}
//---------------------------------------------------------------------------//
void CGameMap::SaveDataMap(char* pszFileName)
{
    if(!pszFileName)
    {
        return;
    }
    FILE* fp = fopen(pszFileName, "wb");
    if(!fp)
    {
        return;
    }
    // header ...
    DWORD dwVersion = 1004;
    DWORD dwData	= 0;
    fwrite(&dwVersion, sizeof(DWORD), 1, fp);
    fwrite(&dwData, sizeof(DWORD), 1, fp);
    // save puzzle file
    char szCurDir[_MAX_PATH];
    char szFileName[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szCurDir);
    CTerrainLayer* pTerrainLayer = this->GetTerrainLayer();
    MYASSERT(pTerrainLayer);
    char szPuzzleFile[_MAX_PATH] = "";
    strcpy(szPuzzleFile, pTerrainLayer->GetPuzleFileName());
    int nLen = strlen(szCurDir);
    for(int k = 0; k < _MAX_PATH - nLen - 1; k++)
    {
        szFileName[k] =  szPuzzleFile[nLen+k+1];
    }
    fwrite(szFileName, sizeof(char), _MAX_PATH, fp);
    // save map size ...
    fwrite(&m_sizeMap.iWidth, sizeof(int), 1, fp);
    fwrite(&m_sizeMap.iHeight, sizeof(int), 1, fp);
    int i, j;
    // save mask ...
    // save terrain ...
    // save altitude ..
    for(i = 0; i < m_sizeMap.iHeight; i++)
    {
        DWORD dwCheckData = 0;
        for(j = 0; j < m_sizeMap.iWidth; j++)
        {
            CellInfo* pCellInfo = g_objGameMap.GetCell(j, i);
            LayerInfo* pLayerInfo = &pCellInfo->m_infoLayer;
            if(pLayerInfo)
            {
                unsigned short usData = pLayerInfo->usMask;
                unsigned short usTerrain = pLayerInfo->usTerrain;
                short sAltiute = pLayerInfo->sAltitude;
                if(pLayerInfo->sAltitude > 0)
                {
                    int kkk = 0;
                }
                fwrite(&usData, sizeof(unsigned short), 1, fp);
                fwrite(&usTerrain, sizeof(unsigned short), 1, fp);
                fwrite(&sAltiute, sizeof(short), 1, fp);
                dwCheckData += pLayerInfo->usMask * (pLayerInfo->usTerrain + i + 1) + (pLayerInfo->sAltitude + 2) * (j + 1 + pLayerInfo->usTerrain);
            }
        }
        fwrite(&dwCheckData, sizeof(DWORD), 1, fp);
    }
    pTerrainLayer->SaveDataPassage(fp);
    //pTerrainLayer->SaveDataRegion(fp);
    CInteractiveLayer* pInteractiveLayer = this->GetInteractiveLayer();
    if(pInteractiveLayer)
    {
        pInteractiveLayer->SaveDataLayer(fp);
    }
    // save others ...
    int nAmount = this->GetLayerAmount();
    // save Other Amount ...
    int nOtherAmount =  nAmount - 3;
    fwrite(&nOtherAmount, sizeof(int), 1, fp);
    for(i = 0; i < nAmount; i++)
    {
        CMapLayer* pLayer = this->GetLayerByIndex(i);
        if(!pLayer)
        {
            continue;
        }
        switch(pLayer->GetType())
        {
        case CMapLayer::LAYER_SCENE:
            {
                // save index
                fwrite(&i, sizeof(int), 1, fp);
                // save type
                int nType = pLayer->GetType();
                fwrite(&nType, sizeof(int), 1, fp);
                CSceneLayer* pSceneLayer = (CSceneLayer*)pLayer;
                pSceneLayer->SaveDataLayer(fp);
            }
            break;
        }
    }
    fclose(fp);
}
//---------------------------------------------------------------------------//
BOOL CGameMap::LoadTextMap(char* pszFileName)
{
    if(!pszFileName)
    {
        return false;
    }
    FILE* fp = fopen(pszFileName, "r");
    if(!fp)
    {
        return false;
    }
    this->Reset();
    this->DestroyCell();
    // get puzzle file name ...
    char szCurDir[_MAX_PATH];
    char szFileName[_MAX_PATH];
    char szFullFileName[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szCurDir);
    // begin read ...
    fscanf(fp, "PuzzleFile=%s\n", szFileName);
    sprintf(szFullFileName, "%s\\%s", szCurDir, szFileName);
    CTerrainLayer* pLayer = this->GetTerrainLayer();
    if(!pLayer)
    {
        return false;
    }
    pLayer->LoadPuzzle(szFullFileName);
    CMySize infoSize = {0, 0};
    fscanf(fp, "Width=%i\n", &infoSize.iWidth);
    fscanf(fp, "Height=%i\n", &infoSize.iHeight);
    this->SetMapSize(infoSize);
    int i, j;
    for(i = 0; i < infoSize.iHeight; i++)
    {
        for(j = 0; j < infoSize.iWidth; j++)
        {
            CellInfo* pCellInfo = g_objGameMap.GetCell(j, i);
            LayerInfo* pLayerInfo = &pCellInfo->m_infoLayer;
            if(pLayer)
            {
                int nPosX, nPosY;
                DWORD dwMask;
                int nTer, nAltitude;
                fscanf(fp, "Cell[%d,%d]=[%u,%d,%d]\n",
                       &nPosX, &nPosY, &dwMask, &nTer, &nAltitude);
                pLayerInfo->usMask =  dwMask;
                pLayerInfo->usTerrain = nTer;
                pLayerInfo->sAltitude = nAltitude;
            }
        }
    }
    pLayer = this->GetTerrainLayer();
    if(pLayer)
    {
        pLayer->LoadTextPassage(fp);
    }
    CInteractiveLayer* pInteractiveLayer = this->GetInteractiveLayer();
    if(pInteractiveLayer)
    {
        pInteractiveLayer->LoadTextLayer(fp);
    }
    // other layers
    // read amount ...
    int nAmount;
    fscanf(fp, "========Other Layers Amount=%d========\n", &nAmount);
    for(i = 0; i < nAmount; i++)
    {
        int nIndex, nType;
        fscanf(fp, "========Index=%d, Type=%d========\n", &nIndex, &nType);
        switch(nType)
        {
        case CMapLayer::LAYER_SCENE:
            {
                CSceneLayer* pLayer = new CSceneLayer;
                if(!pLayer)
                {
                    return false;
                }
                m_setMapLayer.insert(m_setMapLayer.begin() + nIndex, pLayer);
                pLayer->LoadTextLayer(fp);
            }
            break;
        }
    }
    /*pLayer = this->GetTerrainLayer();
    if(pLayer)
    {
    pLayer->LoadTextRegion(fp);
    }
    */
    fclose(fp);
    return true;
}
//---------------------------------------------------------------------------//

BOOL CGameMap::LoadDataMap(char* pszFileName)
{
    if(!pszFileName)
    {
        return false;
    }
    FILE* fp = fopen(pszFileName, "rb");
    if(!fp)
    {
        return false;
    }
    this->Reset();
    this->DestroyCell();
    DWORD dwVersion;
    DWORD dwData;
    fread(&dwVersion, sizeof(DWORD), 1, fp);
    fread(&dwData, sizeof(DWORD), 1, fp);
    // get puzzle file name ...
    char szCurDir[_MAX_PATH];
    char szFileName[_MAX_PATH];
    char szFullFileName[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szCurDir);
    fread(szFileName, sizeof(char), _MAX_PATH, fp);
    sprintf(szFullFileName, "%s/%s", szCurDir, szFileName);
    //	strcpy(szFullFileName,szFileName);
    CTerrainLayer* pLayer = this->GetTerrainLayer();
    if(!pLayer)
    {
        return false;
    }
    pLayer->LoadPuzzle(szFullFileName);
    CMySize infoSize = {0, 0};
    fread(&infoSize.iWidth, sizeof(UINT), 1, fp);
    fread(&infoSize.iHeight, sizeof(UINT), 1, fp);
    this->SetMapSize(infoSize);
    int i, j;
    for(i = 0; i < infoSize.iHeight; i++)
    {
        DWORD dwCheckData = 0;
        for(j = 0; j < infoSize.iWidth; j++)
        {
            CellInfo* pCellInfo = g_objGameMap.GetCell(j, i);
            MYASSERT(pCellInfo);
            LayerInfo* pLayerInfo = &pCellInfo->m_infoLayer;
            if(pLayerInfo)
            {
                //int nPosX, nPosY;
                fread(&pLayerInfo->usMask, sizeof(unsigned short), 1, fp);
                fread(&pLayerInfo->usTerrain, sizeof(unsigned short), 1, fp);
                fread(&pLayerInfo->sAltitude, sizeof(short), 1, fp);
                dwCheckData += pLayerInfo->usMask * (pLayerInfo->usTerrain + i + 1) + (pLayerInfo->sAltitude + 2) * (j + 1 + pLayerInfo->usTerrain);
            }
        }
        DWORD dwMapCheckData;
        fread(&dwMapCheckData, sizeof(DWORD), 1, fp);
        if(dwMapCheckData != dwCheckData)
        {
            return false;
        }
    }
    pLayer = this->GetTerrainLayer();
    if(pLayer)
    {
        pLayer->LoadDataPassage(fp);
        /*if(dwVersion == 1003)
        pLayer->LoadDataRegion(fp);*/
    }
    CInteractiveLayer* pInteractiveLayer = this->GetInteractiveLayer();
    if(pInteractiveLayer)
    {
        pInteractiveLayer->LoadDataLayer(fp);
    }
    // other layers
    // read amount ...
    int nAmount;
    fread(&nAmount, sizeof(int), 1, fp);
    for(i = 0; i < nAmount; i++)
    {
        int nIndex, nType;
        fread(&nIndex, sizeof(int), 1, fp);
        fread(&nType, sizeof(int), 1, fp);
        switch(nType)
        {
        case CMapLayer::LAYER_SCENE:
            {
                CSceneLayer* pLayer = new CSceneLayer;
                if(!pLayer)
                {
                    return false;
                }
                m_setMapLayer.insert(m_setMapLayer.begin() + nIndex, pLayer);
                pLayer->LoadDataLayer(fp);
            }
            break;
        }
    }
    fclose(fp);
    m_idDoc = 1;
    return true;
}
//---------------------------------------------------------------------------//
int  CGameMap::GetLayerAmount()
{
    return m_setMapLayer.size();
}
//----------------------------------------------------//
CMapLayer* CGameMap::GetLayerByIndex(int nIndex)
{
    int nAmount = m_setMapLayer.size();
    if((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return  m_setMapLayer[nIndex];
}
//----------------------------------------------------//
void CGameMap::DelLayer(int nIndex)
{
    int nAmount = this->GetLayerAmount();
    if((nIndex < 0) || (nIndex >= nAmount))
    {
        return;
    }
    CMapLayer* pLayer = this->GetLayerByIndex(nIndex);
    if((pLayer->GetType() == CMapLayer::LAYER_TERRAIN) ||
            (pLayer->GetType() == CMapLayer::LAYER_INTERACTIVE) ||
            (pLayer->GetType() == CMapLayer::LAYER_SURFACE))
    {
        return;
    }
    SAFE_DELETE(pLayer);
    m_setMapLayer.erase(m_setMapLayer.begin() + nIndex);
}
//----------------------------------------------------//
void CGameMap::LayerUp(int nIndex)
{
    if(nIndex <= 0)
    {
        return;
    }
    CMapLayer* pLayer = this->GetLayerByIndex(nIndex);
    if(!pLayer)
    {
        return;
    }
    if((pLayer->GetType() == CMapLayer::LAYER_TERRAIN) ||
            (pLayer->GetType() == CMapLayer::LAYER_INTERACTIVE) ||
            (pLayer->GetType() == CMapLayer::LAYER_SURFACE))
    {
        return;
    }
    CMapLayer* pUpLayer = this->GetLayerByIndex(nIndex - 1);
    if(!pUpLayer)
    {
        return;
    }
    m_setMapLayer[nIndex] = pUpLayer;
    m_setMapLayer[nIndex-1] = pLayer;
}
//----------------------------------------------------//
void CGameMap::LayerDn(int nIndex)
{
    if(nIndex < 0)
    {
        return;
    }
    int nAmount = m_setMapLayer.size();
    if(nIndex > nAmount)
    {
        return;
    }
    CMapLayer* pLayer = this->GetLayerByIndex(nIndex);
    if(!pLayer)
    {
        return;
    }
    if((pLayer->GetType() == CMapLayer::LAYER_TERRAIN) ||
            (pLayer->GetType() == CMapLayer::LAYER_INTERACTIVE) ||
            (pLayer->GetType() == CMapLayer::LAYER_SURFACE))
    {
        return;
    }
    CMapLayer* pDnLayer = this->GetLayerByIndex(nIndex + 1);
    if(!pDnLayer)
    {
        return;
    }
    m_setMapLayer[nIndex] = pDnLayer;
    m_setMapLayer[nIndex+1] = pLayer;
}
//----------------------------------------------------//

BOOL CGameMap::IsPosVisible(CMyPos posCell, int nExtendSize/*=8*/)
{
    int nSumPos     = posCell.x + posCell.y;
    int nDeltaPos	= posCell.x - posCell.y;
    int nMyExtendSize = nExtendSize * _NORMAL_SCALE / this->GetScale();
    // get top-left cell point of visible area
    int nStartX, nStartY;
    this->World2Cell(m_posView.x, m_posView.y, nStartX, nStartY);
    nStartX	-= nMyExtendSize;
    int nSumStartPos	= nStartX + nStartY;
    int nDeltaStartPos	= nStartX - nStartY;
    // get width and height of visible area
    int nWidth	= _SCR_WIDTH / _CELL_WIDTH + nMyExtendSize;
    int nHeight	= _SCR_HEIGHT / _CELL_HEIGHT + 2 * nMyExtendSize;
    // judge now
    if(nSumPos >= nSumStartPos && nSumPos <= nSumStartPos + 2 * nHeight &&
            nDeltaPos >= nDeltaStartPos && nDeltaPos <= nDeltaStartPos + 2 * nWidth)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//----------------------------------------------------//
void CGameMap::GetMapObjSize(CMySize& infoSize, CMapObj* pObj)
{
    if(!pObj)
    {
        infoSize.iWidth = 1;
        infoSize.iHeight = 1;
        return;
    }
    switch(pObj->GetObjType())
    {
    case MAP_TERRAIN_PART:
        {
            C2DMapTerrainObjPart* pTerrainObjPart =  (C2DMapTerrainObjPart*)pObj;
            if(pTerrainObjPart)
            {
                pTerrainObjPart->GetBase(infoSize);
            }
        }
        break;
    case MAP_COVER:
        {
            C2DMapCoverObj* pCoverObj =  (C2DMapCoverObj*)pObj;
            if(pCoverObj)
            {
                pCoverObj->GetBase(infoSize);
            }
        }
        break;
    default:
        {
            infoSize.iWidth = 1;
            infoSize.iHeight = 1;
        }
        break;
    }
}
//----------------------------------------------------//
void CGameMap::ResetViewPos()
{
    m_posViewOffset.x = 0;
    m_posViewOffset.y = 0;
}
//----------------------------------------------------//
void CGameMap::SetViewPos(CMyPos posView, BOOL bShiftViewPos, BOOL bBgLimit)
{
    CMyPos posPos;
    ::MouseCheck(posPos.x, posPos.y);
    if(bShiftViewPos)
    {
        if(posPos.y <= 120)
        {
            m_posViewOffset.y -= 6 - posPos.y / 20;
        }
        if(posPos.y >= _SCR_HEIGHT - 170)
        {
            m_posViewOffset.y += 6 - (_SCR_HEIGHT - posPos.y - 50) / 20;
        }
        if(posPos.x <= 120)
        {
            m_posViewOffset.x -= 6 - posPos.x / 20;
        }
        if(posPos.x >= _SCR_WIDTH - 120)
        {
            m_posViewOffset.x += 6 - (_SCR_WIDTH - posPos.x) / 20;
        }
    }
    // 移动屏幕的限制，保证人在屏幕内
    if(bBgLimit)
    {
        int nData = 356;
        nData = nData *  _NORMAL_SCALE / this->GetScale();
        if(m_posViewOffset.x > nData)
        {
            m_posViewOffset.x = nData;
        }
        if(m_posViewOffset.x < -nData)
        {
            m_posViewOffset.x = -nData;
        }
        nData = 192;
        nData = nData *  _NORMAL_SCALE / this->GetScale();
        if(m_posViewOffset.y > nData)
        {
            m_posViewOffset.y = nData;
        }
        nData = 172;
        nData = nData *  _NORMAL_SCALE / this->GetScale();
        if(m_posViewOffset.y < -nData)
        {
            m_posViewOffset.y = -nData;
        }
    }
    this->ProcessShakeEffect();
    this->ProcessScaleEffect();
    this->ProcessColorEffect();
    posView.x += m_posViewOffset.x;
    posView.y += m_posViewOffset.y;
    int iWorldX = posView.x;
    int iWorldY = posView.y;
    CMyPos posLimitBg, posLimitWorld;
    CMySize sizeBg;
    this->GetBgSize(sizeBg);
    posLimitBg.x = sizeBg.iWidth - _SCR_WIDTH + (_SCR_WIDTH - m_nRealScreenWidth) / 2;
    posLimitBg.y = sizeBg.iHeight - _SCR_HEIGHT + (_SCR_HEIGHT - m_nRealScreenHeight) / 2;
    this->Bg2World(posLimitBg.x, posLimitBg.y, posLimitWorld.x, posLimitWorld.y);
    UINT uBgWorldX, uBgWorldY;
    this->GetBgWorldPos(uBgWorldX, uBgWorldY);
    if(bBgLimit)
    {
        if(iWorldX > posLimitWorld.x)
        {
            iWorldX = posLimitWorld.x;
        }
        if(iWorldY > posLimitWorld.y)
        {
            iWorldY = posLimitWorld.y;
        }
        if(iWorldX < (int)uBgWorldX - (_SCR_WIDTH - m_nRealScreenWidth) / 2)
        {
            iWorldX = uBgWorldX - (_SCR_WIDTH - m_nRealScreenWidth) / 2;
        }
        if(iWorldY < (int)uBgWorldY - (_SCR_HEIGHT - m_nRealScreenHeight) / 2)
        {
            iWorldY = uBgWorldY - (_SCR_HEIGHT - m_nRealScreenHeight) / 2;
        }
    }
    m_posView.x = iWorldX;
    m_posView.y = iWorldY;
    this->SetCamera(m_posView);
}
//----------------------------------------------------//
void CGameMap::World2Bg	(int iWorldX, int iWorldY, int& iBgX, int& iBgY)
{
    UINT uBgWorldX, uBgWorldY;
    this->GetBgWorldPos(uBgWorldX, uBgWorldY);
    iBgX	= iWorldX - uBgWorldX;
    iBgY	= iWorldY - uBgWorldY;
}

//--------------------------------------------------------------
void CGameMap::Bg2World	(int iBgX, int iBgY, int& iWorldX, int& iWorldY)
{
    UINT uBgWorldX, uBgWorldY;
    this->GetBgWorldPos(uBgWorldX, uBgWorldY);
    iWorldX	= uBgWorldX + iBgX;
    iWorldY	= uBgWorldY + iBgY;
}
//--------------------------------------------------------------
void CGameMap::GetBgWorldPos(UINT& uWorldX, UINT& uWorldY)
{
    CMySize sizePuzzle;
    this->GetBgSize(sizePuzzle);
    uWorldX	= m_posOrigin.x - sizePuzzle.iWidth / 2;
    uWorldY	= m_posOrigin.y + _CELL_HEIGHT * m_sizeMap.iHeight / 2 - sizePuzzle.iHeight / 2;
    uWorldY	-= ((m_sizeMap.iHeight + 1) % 2) * _CELL_HEIGHT / 2;
}

//--------------------------------------------------------------
void CGameMap::GetBgViewport(int& nViewportX, int& nViewportY)
{
    UINT uBgWorldX, uBgWorldY;
    this->GetBgWorldPos(uBgWorldX, uBgWorldY);
    nViewportX	= m_posView.x - uBgWorldX;
    nViewportY	= m_posView.y - uBgWorldY;
}

//--------------------------------------------------------------
void CGameMap::GetBgSize(CMySize& infoSize)
{
    CTerrainLayer* pLayer = this->GetTerrainLayer();
    if(pLayer)
    {
        pLayer->GetPuzzleSize(infoSize);
    }
    infoSize.iWidth = infoSize.iWidth * pLayer->GetPuzzleGrideSize() / CPuzzleBmp::GetNormalGrideSize();
    infoSize.iHeight = infoSize.iHeight * pLayer->GetPuzzleGrideSize() / CPuzzleBmp::GetNormalGrideSize();
}
//--------------------------------------------------------------
int CGameMap::GetDirection(int x0, int y0, int x1, int y1)
{
    int Dir;
    long Tan[4]	= { -241, -41, 41, 241};
    long DeltaX	= x1 - x0;
    long DeltaY	= y1 - y0;
    if(DeltaX == 0)
    {
        if(DeltaY > 0)
        {
            Dir	= 0;
        }
        else
        {
            Dir	= 4;
        }
    }
    else if(DeltaY == 0)
    {
        if(DeltaX > 0)
        {
            Dir	= 6;
        }
        else
        {
            Dir	= 2;
        }
    }
    else
    {
        int Flag = abs(DeltaX) / DeltaX;
        DeltaY	*= (100 * Flag);
        int i;
        for(i = 0; i < 4; i++)
        {
            Tan[i]	*= abs(DeltaX);
        }
        for(i = 0; i < 3; i++)
            if(DeltaY >= Tan[i] && DeltaY < Tan[i+1])
            {
                break;
            }
        //** note :
        //   i=0    ------- -241 -- -41
        //   i=1    ------- -41  --  41
        //   i=2    -------  41  -- 241
        //   i=3    -------  241 -- -241
        DeltaX = x1 - x0;
        DeltaY = y1 - y0;
        if(DeltaX > 0)
        {
            if(i == 0) { Dir	= 5; }
            else if(i == 1) { Dir	= 6; }
            else if(i == 2) { Dir	= 7; }
            else if(i == 3)
            {
                if(DeltaY > 0)	{ Dir	= 0; }
                else { Dir	= 4; }
            }
        }
        else
        {
            if(i == 0) { Dir	= 1; }
            else if(i == 1) { Dir	= 2; }
            else if(i == 2) { Dir	= 3; }
            else if(i == 3)
            {
                if(DeltaY > 0)	{ Dir	= 0; }
                else { Dir	= 4; }
            }
        }
    }
    Dir	= (Dir + 7) % 8;
    return Dir;
}

//--------------------------------------------------------------
double CGameMap::GetPreciseDistance(int x0, int y0, int x1, int y1)
{
    long Value = (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
    return sqrt(Value);
}

//--------------------------------------------------------------
//--- get distance with the size of role
int CGameMap::GetDistance(int x0, int y0, int x1, int y1, int size)
{
    // adjust caculate point
    if(size == 2)
    {
        int Dir = GetDirection(x0, y0, x1, y1);
        MYASSERT (Dir >= 0 && Dir < _MAX_DIRECTION);
        switch(Dir)
        {
        case 6:
        case 7:
        case 0:
            break;	//** no change
        case 1:
        case 2:
            x0 -= 1;
            break;
        case 3:
            x0 -= 1;
            y0 -= 1;
            break;
        case 4:
        case 5:
            y0 -= 1;
            break;
        default:
            break;
        }
    }
    else if(size == 3)
    {
        x0--;
        y0--;
        int Dir = GetDirection(x0, y0, x1, y1);
        x0 += _DELTA_X[Dir];
        y0 += _DELTA_Y[Dir];
    }
    long Value = (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
    long i = 1;
    while(1)
    {
        if(Value < (i * i))
        {
            break;
        }
        i++;
    }
    long a = (i - 1) * (i - 1);
    if((i * i - a) > (Value - a) * 2)
    {
        return int(i - 1);
    }
    else
    {
        return int(i);
    }
}


//--------------------------------------------------------------
void CGameMap::SetCamera(CMyPos posView)
{
    int iWorldX = posView.x;
    int iWorldY = posView.y;
    UINT uBgWidth, uBgHeight;
    CMySize infoSize;
    this->GetBgSize(infoSize);
    uBgWidth = infoSize.iWidth;
    uBgHeight = infoSize.iHeight;
    UINT uBgWorldX, uBgWorldY;
    this->GetBgWorldPos(uBgWorldX, uBgWorldY);
    /*	if(iWorldX < (int)uBgWorldX)
    iWorldX =uBgWorldX;
    else if(iWorldX+m_nRealScreenWidth > uBgWorldX+uBgWidth)
    iWorldX =uBgWorldX+uBgWidth-m_nRealScreenWidth;

      if(iWorldY < (int)uBgWorldY)
      iWorldY =uBgWorldY;
      else if(iWorldY+m_nRealScreenHeight > uBgWorldY+uBgHeight)
      iWorldY =uBgWorldY+uBgHeight-m_nRealScreenHeight;
    */
    CMyPos posCamera;
    posCamera.x = iWorldX - uBgWorldX;
    posCamera.y = iWorldY - uBgWorldY;
    float fX, fZ;
    fX = (float)_SCR_WIDTH / 2 + (float)posCamera.x;
    fZ = (float)_SCR_HEIGHT / 2 + (float)posCamera.y;
    CMyBitmap::GameCameraSet(fX, fZ);
}
//--------------------------------------------------------------
void CGameMap::ApplyObjShow(CMapObj* pMapObj)
{
    if(!pMapObj)
    {
        return;
    }
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(!pLayer)
    {
        return;
    }
    pLayer->ApplyShow(pMapObj);
}
//--------------------------------------------------------------
void CGameMap::AddInteractiveObj(CMapObj* pMapObj)
{
    if(!pMapObj)
    {
        return;
    }
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(!pLayer)
    {
        return;
    }
    pLayer->AddMapObj(pMapObj);
}
//--------------------------------------------------------------
void CGameMap::DelInteractiveObj(CMapObj* pMapObj)
{
    if(!pMapObj)
    {
        return;
    }
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(!pLayer)
    {
        return;
    }
    pLayer->DelObj(pMapObj);
}
//--------------------------------------------------------------
int  CGameMap::GetAltitude(CMyPos posCell)
{
    CellInfo* pCellInfo = this->GetCell(posCell.x, posCell.y);
    if(!pCellInfo)
    {
        return 0;
    }
    LayerInfo* pInfo = g_objGameMap.GetLastLayerInfo(pCellInfo);
    if(!pInfo)
    {
        return 0;
    }
    return pInfo->sAltitude;
}
//--------------------------------------------------------------
int  CGameMap::GetBaseAltitude(CMyPos posCell)
{
    CellInfo* pCellInfo = this->GetCell(posCell.x, posCell.y);
    if(!pCellInfo)
    {
        return 0;
    }
    return pCellInfo->m_infoLayer.sAltitude;
}
//--------------------------------------------------------------
void CGameMap::Sampling(DEQUE_CELL& setCell, CMyPos& posHighest, CMyPos posStart, CMyPos posEnd, int nHits)
{
    setCell.clear();
    vector<POINT> setPoint;
    this->DDALine(posStart.x, posStart.y, posEnd.x, posEnd.y, setPoint);
    int nHighestAltitude = -65535;
    int nAmount = setPoint.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CellInfo* pCellInfo = this->GetCell(setPoint[i].x, setPoint[i].y);
        if(!pCellInfo)
        {
            continue;
        }
        setCell.push_back(pCellInfo);
        if(this->GetGroundAltitude(pCellInfo) > nHighestAltitude)
        {
            posHighest.x = setPoint[i].x;
            posHighest.y = setPoint[i].y;
            nHighestAltitude = this->GetGroundAltitude(pCellInfo);
        }
    }
    setPoint.clear();
    /*	CMyPos posStartWorld, posEndWorld;
    	this->Cell2World(posStart.x, posStart.y, posStartWorld.x, posStartWorld.y);
    	this->Cell2World(posEnd.x, posEnd.y, posEndWorld.x, posEndWorld.y);

    	double dbChangeX = 1.0*(posEndWorld.x - posStartWorld.x) /nHits;
    	double dbChangeY = 1.0*(posEndWorld.y - posStartWorld.y) /nHits;

    	CellInfo* pPreCell = NULL;

    	int nHighestAltitude = -65535;
    	posHighest = posStart;

    	for(int i = 0; i < nHits; i++)
    	{
    		CMyPos posCurrentWorld;
    		posCurrentWorld.x = (int)(i*dbChangeX + posStartWorld.x);
    		posCurrentWorld.y = (int)(i*dbChangeY + posStartWorld.y);

    		CMyPos posCurrentCell;
    		this->World2Cell(posCurrentWorld.x, posCurrentWorld.y,
    			posCurrentCell.x, posCurrentCell.y);

    		CellInfo* pCellInfo = this->GetCell(posCurrentCell.x, posCurrentCell.y);
    		if(!pCellInfo)
    			continue;

    		if(pCellInfo != pPreCell)
    		{
    			setCell.push_back(pCellInfo);
    			if(this->GetGroundAltitude(pCellInfo) > nHighestAltitude)
    			{
    				posHighest = posCurrentCell;
    				nHighestAltitude = this->GetGroundAltitude(pCellInfo);
    			}
    			pPreCell = pCellInfo;
    		}
    	}	setCell.clear();
    	CMyPos posStartWorld, posEndWorld;
    	this->Cell2World(posStart.x, posStart.y, posStartWorld.x, posStartWorld.y);
    	this->Cell2World(posEnd.x, posEnd.y, posEndWorld.x, posEndWorld.y);

    	double dbChangeX = 1.0*(posEndWorld.x - posStartWorld.x) /nHits;
    	double dbChangeY = 1.0*(posEndWorld.y - posStartWorld.y) /nHits;

    	CellInfo* pPreCell = NULL;

    	int nHighestAltitude = -65535;
    	posHighest = posStart;

    	for(int i = 0; i < nHits; i++)
    	{
    		CMyPos posCurrentWorld;
    		posCurrentWorld.x = (int)(i*dbChangeX + posStartWorld.x);
    		posCurrentWorld.y = (int)(i*dbChangeY + posStartWorld.y);

    		CMyPos posCurrentCell;
    		this->World2Cell(posCurrentWorld.x, posCurrentWorld.y,
    			posCurrentCell.x, posCurrentCell.y);

    		CellInfo* pCellInfo = this->GetCell(posCurrentCell.x, posCurrentCell.y);
    		if(!pCellInfo)
    			continue;

    		if(pCellInfo != pPreCell)
    		{
    			setCell.push_back(pCellInfo);
    			if(this->GetGroundAltitude(pCellInfo) > nHighestAltitude)
    			{
    				posHighest = posCurrentCell;
    				nHighestAltitude = this->GetGroundAltitude(pCellInfo);
    			}
    			pPreCell = pCellInfo;
    		}
    	}*/
}
//--------------------------------------------------------------
void CGameMap::Mouse2Cell(int nMouseX, int nMouseY, int& nCellX, int& nCellY)
{
    /*	CMyPos posReference = {_SCR_WIDTH/2, _SCR_HEIGHT/2};
    nX = (nMouseX - posReference.x)

      nMouseX = posReference.x + (nX - posReference.x) * this->GetScale()/_NORMAL_SCALE;
      nMouseY = posReference.y + (nY - posReference.y) * this->GetScale()/_NORMAL_SCALE;
    	*/
    CMyPos posMouse;
    posMouse.x = nMouseX;
    posMouse.y = nMouseY;
    int nDistX = (posMouse.x - _SCR_WIDTH / 2) * _NORMAL_SCALE / this->GetScale();
    int nDistY = (posMouse.y - _SCR_HEIGHT / 2) * _NORMAL_SCALE / this->GetScale();
    posMouse.x = _SCR_WIDTH / 2 + nDistX;
    posMouse.y = _SCR_HEIGHT / 2 + nDistY;
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(pLayer)
    {
        C2DMapTerrainObjPart* pObj = pLayer->Get2DFocusMapTerrainObjPart();
        // 调试代码
        if(pObj)
        {
            int nOffset;
            if(pObj->GetMouseOffset(posMouse, nOffset))
            {
                posMouse.y +=  nOffset;
            }
        }
    }
    this->Screen2Cell(posMouse.x, posMouse.y, nCellX, nCellY);
}
//--------------------------------------------------------------

void CGameMap::DestroyCell()
{
    if(!m_pCellInfo)
    {
        return;
    }
    DWORD dwAmount =  m_sizeMap.iWidth * m_sizeMap.iHeight;
    CellInfo* pMyCellInfo = m_pCellInfo;
    for(DWORD i = 0; i < dwAmount; i ++)
    {
        if(pMyCellInfo->m_infoLayer.pLayer)
        {
            this->DelLayerInfo(pMyCellInfo->m_infoLayer.pLayer);
        }
        pMyCellInfo ++;
    }
    delete[] m_pCellInfo;
    m_pCellInfo = NULL;
}
//--------------------------------------------------------------
void CGameMap::DelLayerInfo(LayerInfo* pLayerInfo)
{
    if(!pLayerInfo)
    {
        return;
    }
    while (true)
    {
        LayerInfo* pMyLayerInfo =  pLayerInfo;
        if(!pMyLayerInfo->pLayer)
        {
            SAFE_DELETE(pMyLayerInfo);
            break;
        }
        else
        {
            // 找到最后一个 CellInfo 删除
            LayerInfo* pMyLastLayerInfo = pLayerInfo;
            while(true)
            {
                if(pMyLastLayerInfo->pLayer->pLayer)
                {
                    pMyLastLayerInfo = pMyLastLayerInfo->pLayer;
                }
                else
                {
                    SAFE_DELETE(pMyLastLayerInfo->pLayer);
                    break;
                }
            }
        }
    }
    pLayerInfo = NULL;
}
//--------------------------------------------------------------
void CGameMap::CreateCell()
{
    DWORD dwAmount =  m_sizeMap.iWidth * m_sizeMap.iHeight;
    m_pCellInfo = new CellInfo[dwAmount];
    CellInfo* pMyCellInfo = m_pCellInfo;
    for(DWORD i = 0; i < dwAmount; i ++)
    {
        pMyCellInfo->m_infoLayer.pLayer = NULL;
        pMyCellInfo->m_infoLayer.usTerrain = 0;
        pMyCellInfo->m_infoLayer.usMask = 0;
        pMyCellInfo->m_infoLayer.sAltitude = 0;
        pMyCellInfo->m_bSearched = false;
        pMyCellInfo ++;
    }
}
//--------------------------------------------------------------
LayerInfo* CGameMap::GetLastLayerInfo(CellInfo* pCellInfo)
{
    if(!pCellInfo)
    {
        return NULL;
    }
    if(!pCellInfo->m_infoLayer.pLayer)
    {
        return &pCellInfo->m_infoLayer;
    }
    LayerInfo* pLayerInfo = pCellInfo->m_infoLayer.pLayer;
    while (true)
    {
        if(!pLayerInfo->pLayer)
        {
            return  pLayerInfo;
        }
        else { pLayerInfo = pLayerInfo->pLayer; }
    }
    return NULL;
}

//--------------------------------------------------------------
void CGameMap::AddLayer(CellInfo* pCellInfo, LayerInfo* pLayerInfo)
{
    if(!pCellInfo || !pLayerInfo)
    {
        return;
    }
    LayerInfo* infoLastLayer = this->GetLastLayerInfo(pCellInfo);
    if(infoLastLayer)
    {
        infoLastLayer->pLayer =  pLayerInfo;
    }
}

//--------------------------------------------------------------
void CGameMap::DelLastLayer(CellInfo* pCellInfo)
{
    if(!pCellInfo || !pCellInfo->m_infoLayer.pLayer)
    {
        return;
    }
    LayerInfo* pLayerInfo = &pCellInfo->m_infoLayer;
    while (true)
    {
        if(!pLayerInfo->pLayer->pLayer)
        {
            SAFE_DELETE(pLayerInfo->pLayer);
            break;
        }
        else
        {
            pLayerInfo = pLayerInfo->pLayer;
        }
    }
    return;
}
//--------------------------------------------------------------
int CGameMap::GetGroundAltitude(CellInfo* pCellInfo)
{
    if(!pCellInfo)
    {
        return 0;
    }
    LayerInfo* pLayerInfo = this->GetLastLayerInfo(pCellInfo);
    if(!pLayerInfo)
    {
        return 0;
    }
    return pLayerInfo->sAltitude;
}
//--------------------------------------------------------------
DWORD CGameMap::GetGroundMask(CellInfo* pCellInfo)
{
    if(!pCellInfo)
    {
        return 1;
    }
    LayerInfo* pLayerInfo = this->GetLastLayerInfo(pCellInfo);
    if(!pLayerInfo)
    {
        return 1;
    }
    return pLayerInfo->usMask;
}
//--------------------------------------------------------------
void  CGameMap::SetGroundMask(CellInfo* pCellInfo, DWORD dwMask)
{
    if(!pCellInfo)
    {
        return;
    }
    LayerInfo* pLayerInfo = this->GetLastLayerInfo(pCellInfo);
    if(!pLayerInfo)
    {
        return;
    }
    pLayerInfo->usMask = (unsigned short)dwMask;
}
//--------------------------------------------------------------
int	 CGameMap::GetMask(LayerInfo* pInfo)
{
    if(!pInfo)
    {
        return 1;
    }
}
//--------------------------------------------------------------
int	 CGameMap::GetTerrain(CellInfo* pCellInfo)
{
    if(!pCellInfo)
    {
        return 0;
    }
    LayerInfo* pLayerInfo = CGameMap::GetLastLayerInfo(pCellInfo);
    if(!pLayerInfo)
    {
        return 0;
    }
    return pLayerInfo->usTerrain;
}
//--------------------------------------------------------------
int	 CGameMap::GetAltitude(LayerInfo* pInfo)
{
    if(!pInfo)
    {
        return 1;
    }
}
//--------------------------------------------------------------
void CGameMap::SetMask(int nMask, LayerInfo* pInfo)
{
    if(!pInfo)
    {
        return;
    }
}
//--------------------------------------------------------------
void CGameMap::SetTerrain(int nTerrain, LayerInfo* pInfo)
{
    if(!pInfo)
    {
        return;
    }
}
//--------------------------------------------------------------
void CGameMap::SetAltitude(int nAltitude, LayerInfo* pInfo)
{
    if(!pInfo)
    {
        return;
    }
}
//--------------------------------------------------------------
int CGameMap::GetTerrain(CMyPos posCell)
{
    return this->GetTerrain(posCell.x, posCell.y);
}
//--------------------------------------------------------------
int CGameMap::GetTerrain(int nPosX, int nPosY)
{
    CellInfo* pInfo = this->GetCell(nPosX, nPosY);
    return CGameMap::GetTerrain(pInfo);
}
//--------------------------------------------------------------
void CGameMap::AddMagicEffect(CMagicEffect* pEffect)
{
    if(pEffect)
    {
        m_objMagicEffectManager.AddMagicEffect(pEffect);
    }
}
//--------------------------------------------------------------
void CGameMap::Add3DMapMagicItem(OBJID idType, OBJID id, CMyPos posCell)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(pLayer)
    {
        int nAmount = pLayer->GetObjAmount();
        for(int i = 0; i < nAmount; i ++)
        {
            CMapObj* pObj = pLayer->GetObjByIndex(i);
            if(pObj && pObj->GetObjType() == MAP_3DMAGICMAPITEM)
            {
                C3DMagicMapItem* pItem = (C3DMagicMapItem*) pObj;
                if(pItem->GetID() ==  idType)
                {
                    return;
                }
            }
        }
        C3DMagicMapItem* pItem = C3DMagicMapItem::CreateNew(idType, id);
        if(pItem)
        {
            pItem->SetPos(posCell);
            pLayer->AddMapObj(pItem);
        }
    }
}
//--------------------------------------------------------------
void CGameMap::SetARGB(DWORD dwARGB)
{
    ColorEffectInfo.dwOldColor = dwARGB;
    m_dwMapARGB = dwARGB;
}
//--------------------------------------------------------------
BOOL CGameMap::Outof9Block(CMyPos posHero, CMyPos posTarget)
{
    /*
    CMyPos posBlock;
    posBlock.x = posHero.x/_BLOCK_SIZE;
    posBlock.y = posHero.y/_BLOCK_SIZE;

      CMyPos posBegin, posEnd;
      posBegin.x = posBlock.x * _BLOCK_SIZE - _BLOCK_SIZE;
      posBegin.y = posBlock.y * _BLOCK_SIZE - _BLOCK_SIZE;

    	posEnd.x = posBlock.x * _BLOCK_SIZE + 2*_BLOCK_SIZE;
    	posEnd.y = posBlock.y * _BLOCK_SIZE + 2*_BLOCK_SIZE;

    	  if((posTarget.x >= posBegin.x) &&
    	  (posTarget.x < posEnd.x) &&
    	  (posTarget.y >= posBegin.y) &&
    	  (posTarget.y < posEnd.y))
    	  return false;

    		return true;
    	*/
    if((abs(posHero.x - posTarget.x) > _BLOCK_SIZE) ||
            (abs(posHero.y - posTarget.y) > _BLOCK_SIZE))
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------
void CGameMap::SetScale(int nScale)
{
    m_nScale = nScale;
    ScaleEffectInfo.nOldScale = nScale;
    m_nRealScreenWidth = _SCR_WIDTH * _NORMAL_SCALE / m_nScale;
    m_nRealScreenHeight = _SCR_HEIGHT * _NORMAL_SCALE / m_nScale;
}
//--------------------------------------------------------------
void CGameMap::MapScaleShowPos(int& nX, int& nY)
{
    // 基准点为视口中心点
    CMyPos posReference = {_SCR_WIDTH / 2, _SCR_HEIGHT / 2};
    nX = posReference.x + (nX - posReference.x) * this->GetScale() / _NORMAL_SCALE;
    nY = posReference.y + (nY - posReference.y) * this->GetScale() / _NORMAL_SCALE;
}
//--------------------------------------------------------------
void CGameMap::MapScaleMousePos(int& nX, int& nY)
{
    CMyPos posReference = {_SCR_WIDTH / 2, _SCR_HEIGHT / 2};
    nX = posReference.x + (nX - posReference.x) * _NORMAL_SCALE / this->GetScale();
    nY = posReference.y + (nY - posReference.y) * _NORMAL_SCALE / this->GetScale();
}
//--------------------------------------------------------------
void	CGameMap::SetID(OBJID idMap)
{
    if(m_idMap != idMap)
    {
        m_bShowGobalRegionEffect = true;
    }
    m_idMap = idMap;
    this->SelectRegionData();
    g_objGameMap.ProcessRegion(g_objHero.GetAlignPos());
}
//--------------------------------------------------------------
OBJID	CGameMap::GetID()
{
    return m_idMap;
}
//--------------------------------------------------------------
void	CGameMap::SetIDDoc(OBJID idMapDoc)
{
    m_idDoc = idMapDoc;
}
//--------------------------------------------------------------
OBJID	CGameMap::GetIDDoc()
{
    return m_idDoc;
}
//--------------------------------------------------------------
void	CGameMap::SetType(DWORD dwType)
{
    m_dwType = dwType;
}
//--------------------------------------------------------------
DWORD	CGameMap::GetType()
{
    return m_dwType;
}
//--------------------------------------------------------------
void	CGameMap::CreateRegionData()
{
    this->DestroyRegionData();
    const char szIniFile[64] = "ini/region.ini";
    FILE* fp = fopen(szIniFile, "r");
    if(!fp)
    {
        return;
    }
    RegionInfo infoRegion;
    while (true)
    {
        int nReturn = fscanf(fp, "%u %u %u %u %u %u %s %s %u %u %u %u\n",
                             &infoRegion.m_idMap,
                             &infoRegion.m_dwType,
                             &infoRegion.m_posCell.x,
                             &infoRegion.m_posCell.y,
                             &infoRegion.m_dwCx,
                             &infoRegion.m_dwCy,
                             infoRegion.m_szString[0],
                             infoRegion.m_szString[1],
                             &infoRegion.m_dwData[0],
                             &infoRegion.m_dwData[1],
                             &infoRegion.m_dwData[2],
                             &infoRegion.m_dwData[3]
                            );
        if(nReturn == EOF)
        {
            break;
        }
        else if(nReturn != 12)
        {
            ::ErrorMsg("invalid line found in %s.", szIniFile);
            continue;
        }
        RegionInfo* pInfo = new RegionInfo;
        MYASSERT(pInfo);
        memcpy(pInfo, &infoRegion, sizeof(RegionInfo));
        m_setRegionData.push_back(pInfo);
    }
    fclose(fp);
}
//--------------------------------------------------------------
void	CGameMap::DestroyRegionData()
{
    int nAmount = m_setRegionData.size();
    for(int i = 0; i < nAmount; i ++)
    {
        RegionInfo* pInfo = m_setRegionData[i];
        SAFE_DELETE(pInfo);
    }
    m_setRegionData.clear();
}
//--------------------------------------------------------------
void	CGameMap::SelectRegionData()
{
    CTerrainLayer* pLayer = this->GetTerrainLayer();
    MYASSERT(pLayer);
    pLayer->ClearRegion();
    int nAmount = m_setRegionData.size();
    for(int i = 0; i < nAmount; i ++)
    {
        RegionInfo* pInfo = m_setRegionData[i];
        if(pInfo && pInfo->m_idMap == m_idMap)
        {
            C2DMapRegion* pRegion = C2DMapRegion::CreateNew(pInfo);
            if(pRegion)
            {
                pLayer->AddRegion(pRegion);
            }
        }
    }
}
//--------------------------------------------------------------
void	CGameMap::BeginShakeEffect(int nEffectType)
{
    ShakeEffectInfo.dwFrameAmount = 60;
    ShakeEffectInfo.dwTimeBegin = ::TimeGet();
    ShakeEffectInfo.dwFrameInterval = 50;
    ShakeEffectInfo.dwSwing = 8;
    ShakeEffectInfo.bOn = true;
}
//--------------------------------------------------------------
void	CGameMap::BeginScaleEffect(int nEffectType)
{
    if(!ScaleEffectInfo.bOn)
    {
        ScaleEffectInfo.nOldScale = this->GetScale();
    }
    ScaleEffectInfo.dwFrameAmount = 30;
    ScaleEffectInfo.dwTimeBegin = ::TimeGet();
    ScaleEffectInfo.dwFrameInterval = 150;
    ScaleEffectInfo.dwSwing = 64;
    ScaleEffectInfo.bOn = true;
}
//--------------------------------------------------------------
void	CGameMap::BeginColorEffect(int nEffectType)
{
    if(!ColorEffectInfo.bOn)
    {
        ColorEffectInfo.dwOldColor = m_dwMapARGB;
    }
    ColorEffectInfo.bRed = (ColorEffectInfo.dwOldColor & 0x00ffffff) >> 16;
    ColorEffectInfo.bGreen = (ColorEffectInfo.dwOldColor & 0x0000ffff) >> 8;
    ColorEffectInfo.bBlue = ColorEffectInfo.dwOldColor & 0x000000ff;
    ColorEffectInfo.bRedE = 0x22;
    ColorEffectInfo.bGreenE = 0x22;
    ColorEffectInfo.bBlueE = 0x22;
    ColorEffectInfo.bOn = true;
    ColorEffectInfo.dwChangeDuration = 2000;
    ColorEffectInfo.dwChangeBackDuration = 2000;
    ColorEffectInfo.dwDuration	= 2000;
    ColorEffectInfo.dwTimeBegin = ::TimeGet();
}
//--------------------------------------------------------------
void	CGameMap::ProcessShakeEffect()
{
    if(!ShakeEffectInfo.bOn)
    {
        return;
    }
    // get current frame index
    DWORD dwFrameIndex = (::TimeGet() - ShakeEffectInfo.dwTimeBegin) / ShakeEffectInfo.dwFrameInterval;
    if(dwFrameIndex > ShakeEffectInfo.dwFrameAmount)
    {
        ShakeEffectInfo.bOn = false;
        m_posViewOffset.x = m_posViewOffset.y = 0;
        return;
    }
    switch(dwFrameIndex % 4)
    {
    case  0:
        m_posViewOffset.x -= ShakeEffectInfo.dwSwing;
        break;
    case 1:
        m_posViewOffset.x += ShakeEffectInfo.dwSwing;
        break;
    case 2:
        m_posViewOffset.y -= ShakeEffectInfo.dwSwing;
        break;
    case 3:
        m_posViewOffset.y += ShakeEffectInfo.dwSwing;
        break;
    }
}
//--------------------------------------------------------------
void	CGameMap::ProcessScaleEffect()
{
    if(!ScaleEffectInfo.bOn)
    {
        return;
    }
    // get current frame index
    DWORD dwTimePass = ::TimeGet() - ScaleEffectInfo.dwTimeBegin;
    DWORD dwFrameIndex = dwTimePass / ScaleEffectInfo.dwFrameInterval;
    if(dwFrameIndex > ScaleEffectInfo.dwFrameAmount)
    {
        ScaleEffectInfo.bOn = false;
        this->SetScale(ScaleEffectInfo.nOldScale);
        return;
    }
    int nScaleOffset = ScaleEffectInfo.dwSwing * (dwTimePass - dwFrameIndex * ScaleEffectInfo.dwFrameInterval) / ScaleEffectInfo.dwFrameInterval;
    if(dwFrameIndex % 2 == 0)
    {
        m_nScale = _NORMAL_SCALE + nScaleOffset;
    }
    else
    {
        m_nScale = _NORMAL_SCALE + (ScaleEffectInfo.dwSwing - nScaleOffset);
    }
}
//--------------------------------------------------------------
void	CGameMap::ProcessColorEffect()
{
    if(!ColorEffectInfo.bOn)
    {
        return;
    }
    DWORD dwTimePass = ::TimeGet() - ColorEffectInfo.dwTimeBegin;
    if(dwTimePass > ColorEffectInfo.dwChangeDuration + ColorEffectInfo.dwDuration + ColorEffectInfo.dwChangeBackDuration)
    {
        m_dwMapARGB = ColorEffectInfo.dwOldColor;
        ColorEffectInfo.bOn = false;
        return;
    }
    if(dwTimePass < ColorEffectInfo.dwChangeDuration)
    {
        DWORD dwCurrentR = ColorEffectInfo.bRed + (ColorEffectInfo.bRedE - ColorEffectInfo.bRed) * (int)dwTimePass / (int)ColorEffectInfo.dwChangeDuration;
        DWORD bCurrentG = ColorEffectInfo.bGreen + (ColorEffectInfo.bGreenE - ColorEffectInfo.bGreen) * (int)dwTimePass / (int)ColorEffectInfo.dwChangeDuration;
        DWORD bCurrentB = ColorEffectInfo.bBlue + (ColorEffectInfo.bBlueE - ColorEffectInfo.bBlue) * (int)dwTimePass / (int)ColorEffectInfo.dwChangeDuration;
        DWORD dwCurrentColor = ColorEffectInfo.dwOldColor & 0xff000000 + (dwCurrentR << 16) + (bCurrentG << 8) + bCurrentB;
        m_dwMapARGB = dwCurrentColor;
    }
    else if(dwTimePass < ColorEffectInfo.dwChangeDuration + ColorEffectInfo.dwDuration)
    {
        DWORD dwCurrentColor = ColorEffectInfo.dwOldColor & 0xff000000 + (ColorEffectInfo.bRedE << 16) + (ColorEffectInfo.bGreenE << 8) + ColorEffectInfo.bBlueE;
        m_dwMapARGB = dwCurrentColor;
    }
    else
    {
        DWORD dwMyTimePass = dwTimePass - ColorEffectInfo.dwChangeDuration - ColorEffectInfo.dwDuration;
        DWORD dwCurrentR = ColorEffectInfo.bRedE - (ColorEffectInfo.bRedE - ColorEffectInfo.bRed) * (int)dwMyTimePass / (int)ColorEffectInfo.dwChangeBackDuration;
        DWORD bCurrentG = ColorEffectInfo.bGreenE - (ColorEffectInfo.bGreenE - ColorEffectInfo.bGreen) * (int)dwMyTimePass / (int)ColorEffectInfo.dwChangeBackDuration;
        DWORD bCurrentB = ColorEffectInfo.bBlueE - (ColorEffectInfo.bBlueE - ColorEffectInfo.bBlue) * (int)dwMyTimePass / (int)ColorEffectInfo.dwChangeBackDuration;
        DWORD dwCurrentColor = ColorEffectInfo.dwOldColor & 0xff000000 + (dwCurrentR << 16) + (bCurrentG << 8) + bCurrentB;
        m_dwMapARGB = dwCurrentColor;
    }
}
//--------------------------------------------------------------
void CGameMap::LastMapMagicItem(OBJID id)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(pLayer)
    {
        int nAmount = pLayer->GetObjAmount();
        for(int i = 0; i < nAmount; i ++)
        {
            CMapObj* pObj = pLayer->GetObjByIndex(i);
            if(pObj && pObj->GetObjType() == MAP_3DMAGICMAPITEM)
            {
                C3DMagicMapItem* pItem = (C3DMagicMapItem*) pObj;
                if(pItem->GetID() ==  id)
                {
                    pItem->Last();
                }
            }
        }
    }
}
//--------------------------------------------------------------
void CGameMap::EndMapMagicItem(OBJID id)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    if(pLayer)
    {
        int nAmount = pLayer->GetObjAmount();
        for(int i = 0; i < nAmount; i ++)
        {
            CMapObj* pObj = pLayer->GetObjByIndex(i);
            if(pObj && pObj->GetObjType() == MAP_3DMAGICMAPITEM)
            {
                C3DMagicMapItem* pItem = (C3DMagicMapItem*) pObj;
                if(pItem->GetID() ==  id)
                {
                    pItem->End();
                }
            }
        }
    }
}
//--------------------------------------------------------------
void CGameMap::SetShowExpFlag(BOOL bShow)
{
    if(m_bShowExp != bShow)
    {
        m_dwOldExp = g_objHero.GetExp();
    }
    m_bShowExp = bShow;
}

//--------------------------------------------------------------
void CGameMap::ShowExp()
{
    int nExp				= g_objHero.GetExp();
    int nExpNextLevel	= g_objHero.GetNextLevelExp();
    CMyPos posShow = {50, _SCR_HEIGHT - 110};
    if(nExpNextLevel == 0)
    {
        if(m_bShowExp)
        {
            CMyBitmap::ShowString(posShow.x, posShow.y, 0xff01f546, g_objGameDataSet.GetStr(100138), "楷体", 18, true);
        }
        return;
    }
    double dRate			= (double)nExp * 100 / nExpNextLevel;
    char szRate[256];
    OBJID idStr = 100135;
    sprintf(szRate, g_objGameDataSet.GetStr(idStr), (float)dRate);
    if(m_bShowExp)
    {
        CMyBitmap::ShowString(posShow.x, posShow.y, 0xff01f546, szRate, "楷体", 18, true);
    }
    int nShowX = 60 + strlen(szRate) * 9;
    if(m_bShowExp && m_dwShowExp > 0 && (::TimeGet() - m_dwTimeExpBegin) < 2000)
    {
        char szExp[64];
        sprintf(szExp, "+%u", m_dwShowExp);
        CMyBitmap::ShowString(nShowX, posShow.y, 0xff01f546, szExp, "楷体", 18, true);
    }
}
//--------------------------------------------------------------
void CGameMap::SetShowExp(DWORD dwExp)
{
    m_dwTimeExpBegin	= ::TimeGet();
    m_dwShowExp			= dwExp;
}
//--------------------------------------------------------------
TerrainWeatherInfo* CGameMap::GetDefaultWeatherInfo()
{
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    int nTerrain = this->GetTerrain(posHero);
    return g_obj3DRoleData.GetWeatherInfo(nTerrain);
}
//--------------------------------------------------------------
void	CGameMap::DDALine(int x0, int y0, int x1, int y1, vector<POINT>& vctPoint)
{
    vctPoint.clear();
    if (x0 == x1 && y0 == y1)
    {
        return;
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int abs_dx = abs(dx);
    int abs_dy = abs(dy);
    int flag_dx = dx > 0 ? 1 : -1;
    int flag_dy = dy > 0 ? 1 : -1;
    if(abs_dx > abs_dy)
    {
        // x 增益
        if(dx > 0)
        {
            // x0 ++
            for(int i = x0 + 1; i <= x1; i ++)
            {
                POINT point;
                point.x = i;
                point.y = y0 + (dy * (i - x0) * 2 + abs_dx * flag_dy) / (abs_dx * 2);
                vctPoint.push_back(point);
            }
        }
        else if(dx < 0)
        {
            // x0 --
            for(int i = x0 - 1; i >= x1; i --)
            {
                POINT point;
                point.x = i;
                point.y = y0 + (dy * (x0 - i) * 2 + abs_dx * flag_dy) / (abs_dx * 2);
                vctPoint.push_back(point);
            }
        }
    }
    else
    {
        // y 增益
        if(dy > 0)
        {
            // y0 ++
            for(int i = y0 + 1; i <= y1; i ++)
            {
                POINT point;
                point.y = i;
                point.x = x0 + (dx * (i - y0) * 2 + abs_dy * flag_dx) / (abs_dy * 2);
                int nAmount = vctPoint.size();
                vctPoint.push_back(point);
            }
        }
        else if(dy < 0)
        {
            // y0 --
            for(int i = y0 - 1; i >= y1; i --)
            {
                POINT point;
                point.y = i;
                point.x = x0 + (dx * (y0 - i) * 2 + abs_dy * flag_dx) / (abs_dy * 2);
                vctPoint.push_back(point);
            }
        }
    }
}
//--------------------------------------------------------------
void CGameMap::Add3DMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    C3DMapItem* pItem = new C3DMapItem;
    MYASSERT(pItem);
    CMyPos posWorld;
    this->Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
    pItem->Create(idItemType, idMapItem, posWorld);
    pLayer->AddMapObj(pItem);
}

void CGameMap::Add2DMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell, char* szName)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    C2DMapItem* pItem = C2DMapItem::CreateNew(posCell, idMapItem, idItemType, szName);
    if(pItem)
    {
        pLayer->AddMapObj(pItem);
    }
}

//--------------------------------------------------------------
void CGameMap::Del3DMapItem(OBJID idMapItem)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    pLayer->Del3DMapItem(idMapItem);
}


void CGameMap::Del2DMapItem(OBJID idMapItem)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    pLayer->Del2DMapItem(idMapItem);
}
//--------------------------------------------------------------
OBJID CGameMap::Get3DMapItem(CMyPos posCell)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    return pLayer->Get3DMapItem(posCell);
}

OBJID CGameMap::Get2DMapItem(CMyPos posCell)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    return pLayer->Get2DMapItem(posCell);
}

//--------------------------------------------------------------
OBJID CGameMap::Get3DFocusMapItem(CMyPos& posCell)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    return pLayer->GetFocus3DMapItem(posCell);
}

//--------------------------------------------------------------
OBJID CGameMap::Get2DFocusMapItem(CMyPos& posCell)
{
    CInteractiveLayer* pLayer = this->GetInteractiveLayer();
    MYASSERT(pLayer);
    return pLayer->GetFocus2DMapItem(posCell);
}
//--------------------------------------------------------------
BOOL CGameMap::IsPkRegion(CMyPos posCell)
{
    int nAmount = m_setRegionData.size();
    for(int i = 0; i < nAmount; i ++)
    {
        RegionInfo* pInfo = m_setRegionData[i];
        if(!pInfo)
        {
            continue;
        }
        if(pInfo->m_idMap == m_idDoc && pInfo->m_dwType == REGION_PK_PROTECTED)
        {
            if(posCell.x >= pInfo->m_posCell.x &&
                    posCell.y >= pInfo->m_posCell.y &&
                    posCell.x <= pInfo->m_posCell.x + pInfo->m_dwCx &&
                    posCell.y <= pInfo->m_posCell.y + pInfo->m_dwCy)
            {
                return false;
            }
        }
    }
    return true;
}
//--------------------------------------------------------------
bool CGameMap::CreateRegionEffectInfo()
{
    const char szIniFile[] = "ini/RegionWeather.ini";
    FILE* fp = fopen(szIniFile, "r");
    if (!fp)
    {
        ::ErrorOut("error on open %s.", szIniFile);
        return false;
    }
    // get info ...
    RegionEffectInfo RegionEffect;
    int nIndex = 0;
    while (true)
    {
        int nReturn = fscanf(fp, "%u %u %u %u %u %u %s %s %u\n",
                             &RegionEffect.idMap,
                             &RegionEffect.nRegionType,
                             &RegionEffect.nX,
                             &RegionEffect.nY,
                             &RegionEffect.nW,
                             &RegionEffect.nH,
                             RegionEffect.szEffect[0],
                             RegionEffect.szEffect[1],
                             &RegionEffect.nDelayTime
                            );
        if(nReturn == EOF)
        {
            break;
        }
        else if(nReturn != 9)
        {
            ::ErrorMsg("invalid line found in %s.", szIniFile);
            return false;
        }
        RegionEffect.nIndex = nIndex;
        RegionEffectInfo* pInfo = new RegionEffectInfo;
        MYASSERT(pInfo);
        memcpy(pInfo, &RegionEffect, sizeof(RegionEffectInfo));
        // keep it
        m_setRegionEffect.push_back(pInfo);
        nIndex++;
    }
    fclose(fp);
    return true;
}
//--------------------------------------------------------------
void CGameMap::DestroyRegionEffectInfo()
{
    int nAmount = m_setRegionEffect.size();
    for(int i = 0 ; i < nAmount; i++)
    {
        RegionEffectInfo* pInfo = m_setRegionEffect[i];
        SAFE_DELETE(pInfo);
    }
    m_setRegionEffect.clear();
}
//--------------------------------------------------------------
int CGameMap::GetRegionEffectInfo(CMyPos posCell)
{
    for(int i = 0; i < m_setRegionEffect.size(); i++)
    {
        RegionEffectInfo* pInfo = m_setRegionEffect[i];
        if (pInfo && pInfo->idMap == this->GetID())
        {
            if((pInfo->nRegionType == 2
                    && posCell.x >= pInfo->nX && posCell.x < (pInfo->nX + pInfo->nW)
                    && posCell.y >= pInfo->nY && posCell.y < (pInfo->nY + pInfo->nH))
                    || (pInfo->nRegionType == 1))
            {
                return pInfo->nIndex;
            }
        }
    }
    return -1;
}
//--------------------------------------------------------------
void CGameMap::ProcessRegionEffect(CMyPos posCell)
{
    int nIndex = this->GetRegionEffectInfo(posCell);
    if (nIndex == m_nCurRegionEffectIndex)
    {
        return;
    }
    if (m_setRegionEffect.size() <= 0)
    {
        return;
    }
    else if (nIndex == -1 && m_nCurRegionEffectIndex != -1)
    {
        RegionEffectInfo* pInfo = m_setRegionEffect[m_nCurRegionEffectIndex];
        if (pInfo == NULL)
        {
            return;
        }
        if (m_dwTimeRegionEffect  == 0)
        {
            m_dwTimeRegionEffect = ::TimeGet();
        }
        if (::TimeGet() - m_dwTimeRegionEffect > pInfo->nDelayTime)
        {
            m_objSnow.Clear();
            m_nCurRegionEffectIndex = -1;
            m_dwTimeRegionEffect = 0;
        }
    }
    else if(nIndex != -1 &&  nIndex  != m_nCurRegionEffectIndex)
    {
        RegionEffectInfo* pInfo = m_setRegionEffect[nIndex];
        if (pInfo == NULL)
        {
            return;
        }
        if (m_dwTimeRegionEffect == 0)
        {
            m_dwTimeRegionEffect = ::TimeGet();
        }
        else if (::TimeGet() - m_dwTimeRegionEffect >= pInfo->nDelayTime)
        {
            if (m_nCurRegionEffectIndex != -1)
            {
                m_objSnow.Clear();
                m_dwTimeRegionEffect = 0;
                m_nCurRegionEffectIndex = -1;
                return;
            }
            m_nCurRegionEffectIndex = nIndex;
            m_dwTimeRegionEffect = 0;
            if (pInfo->szEffect[1] && strlen(pInfo->szEffect[1]) > 0)
            {
                m_objSnow.Add(pInfo->szEffect[0]);
            }
            else
            {
                return;
            }
            if (pInfo->szEffect[1] && strlen(pInfo->szEffect[1]) > 0)
            {
                m_objSnow.Add(pInfo->szEffect[1]);
            }
            CMyPos posBg, posWorld;
            this->Screen2World(0, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
            this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
            m_posSnow[0] = posBg;
            m_posSnow[2].x = posBg.x;
            m_posSnow[2].y = posBg.y + _SCR_HEIGHT;
            this->Screen2World(_SCR_WIDTH, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
            this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
            m_posSnow[1] = posBg;
            m_posSnow[3].x = posBg.x;
            m_posSnow[3].y = posBg.y + _SCR_HEIGHT;
        }
    }
}
//--------------------------------------------------------------
void CGameMap::ShowSnow()
{
    // 左右判断
    g_objGameMap.ProcessRegionEffect(g_objHero.GetAlignPos());
    if (m_nCurRegionEffectIndex == -1)
    {
        return;
    }
    // center ...
    CMyPos posBgCenter, posWorldCenter;
    this->Screen2World(_SCR_WIDTH / 2, _SCR_HEIGHT - 100, posWorldCenter.x, posWorldCenter.y);
    this->World2Bg(posWorldCenter.x, posWorldCenter.y, posBgCenter.x, posBgCenter.y);
    enum {_NONE_OUT = 0, _POS0_OUT = 1, _POS1_OUT = 2, _POS12_OUT = 3,
          _POS0_OUT_Y = 4, _POS2_OUT_Y = 5, POS02_OUT = 6
         };
    int nWhitchIsOutX = 	_NONE_OUT;
    int nWhitchIsOutY = 	_NONE_OUT;
    if(abs(posBgCenter.x - m_posSnow[0].x) > _SCR_WIDTH)
    {
        nWhitchIsOutX = _POS0_OUT;
    }
    if(abs(posBgCenter.x - m_posSnow[1].x) > _SCR_WIDTH)
    {
        if(nWhitchIsOutX == _POS0_OUT)
        {
            nWhitchIsOutX = _POS12_OUT;
        }
        else
        {
            nWhitchIsOutX = _POS1_OUT;
        }
    }
    if(abs(posBgCenter.y - m_posSnow[0].y) > _SCR_HEIGHT)
    {
        nWhitchIsOutY = _POS0_OUT_Y;
    }
    if(abs(posBgCenter.y - m_posSnow[2].y) > _SCR_HEIGHT)
    {
        if(nWhitchIsOutY == _POS0_OUT_Y)
        {
            nWhitchIsOutY = POS02_OUT;
        }
        else
        {
            nWhitchIsOutY = _POS2_OUT_Y;
        }
    }
    if(nWhitchIsOutX == _POS12_OUT || nWhitchIsOutY == POS02_OUT)
    {
        CMyPos posBg, posWorld;
        this->Screen2World(0 - 20, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
        this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
        m_posSnow[0] = posBg;
        m_posSnow[2].x = posBg.x;
        m_posSnow[2].y = posBg.y + _SCR_HEIGHT;
        this->Screen2World(_SCR_WIDTH + 20, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
        this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
        m_posSnow[1] = posBg;
        m_posSnow[3].x = posBg.x;
        m_posSnow[3].y = posBg.y + _SCR_HEIGHT;
    }
    if(nWhitchIsOutX == _POS0_OUT)
    {
        //根据1 位置调整
        m_posSnow[0].x = m_posSnow[1].x + _SCR_WIDTH;
        if(abs(posBgCenter.x - m_posSnow[0].x) > _SCR_WIDTH)
        {
            m_posSnow[0].x = m_posSnow[1].x - _SCR_WIDTH;
        }
        m_posSnow[2].x = m_posSnow[0].x;
    }
    if(nWhitchIsOutX == _POS1_OUT)
    {
        //根据1 位置调整
        m_posSnow[1].x = m_posSnow[0].x + _SCR_WIDTH;
        if(abs(posBgCenter.x - m_posSnow[1].x) > _SCR_WIDTH)
        {
            m_posSnow[1].x = m_posSnow[0].x - _SCR_WIDTH;
        }
        m_posSnow[3].x = m_posSnow[1].x;
    }
    // 上下判断
    if(nWhitchIsOutY == _POS0_OUT_Y)
    {
        //根据1 位置调整
        m_posSnow[0].y = m_posSnow[2].y + _SCR_HEIGHT;
        if(abs(posBgCenter.y - m_posSnow[0].y) > _SCR_HEIGHT)
        {
            m_posSnow[0].y = m_posSnow[2].y - _SCR_HEIGHT;
        }
        m_posSnow[1].y = m_posSnow[0].y;
    }
    if(nWhitchIsOutY == _POS2_OUT_Y)
    {
        //根据1 位置调整
        m_posSnow[2].y = m_posSnow[0].y + _SCR_HEIGHT;
        if(abs(posBgCenter.y - m_posSnow[2].y) > _SCR_HEIGHT)
        {
            m_posSnow[2].y = m_posSnow[0].y - _SCR_HEIGHT;
        }
        m_posSnow[3].y = m_posSnow[2].y;
    }
    m_objSnow.Process();
    m_objSnow.Show(m_posSnow[0]);
    m_objSnow.Show(m_posSnow[1]);
    m_objSnow.Show(m_posSnow[2]);
    m_objSnow.Show(m_posSnow[3]);
}
//--------------------------------------------------------------
void	CGameMap::ShowScreenInfo()
{
    if(!m_bShowScreenInfo)
    {
        return;
    }
    char szTemp[256];
    CMyPos	posHero, posAlign;
    g_objHero.GetPos(posHero);
    posAlign = g_objHero.GetAlignPos();
    sprintf(szTemp, "Hero坐标:%03d_%03d, 对齐坐标:%03d_%03d", posHero.x, posHero.y, posAlign.x, posAlign.y);
    CMyBitmap::ShowString(10, 10, 0xffffffff, szTemp);
    int nStep = __max(abs(posAlign.x - posHero.x), abs(posAlign.y - posHero.y));
    if(nStep > 255)
    {
        nStep = 255;
    }
    char szTemp1[600] = "";
    for(int i = 0; i < nStep; i ++)
    {
        strcat(szTemp1, "－");
    }
    if(nStep <= 1)
    {
        CMyBitmap::ShowString(10, 30, 0xffffffff, szTemp1);
    }
    else if(nStep <= 2)
    {
        CMyBitmap::ShowString(10, 30, 0xff00ff00, szTemp1);
    }
    else if(nStep <= 3)
    {
        CMyBitmap::ShowString(10, 30, 0xffff0000, szTemp1);
    }
    else
    {
        CMyBitmap::ShowString(10, 30, 0xff000000, szTemp1);
    }
}
//---------------------------------------------------------------------------//
void CGameMap::ShowBrume()
{
    if(!m_bBrume)
    {
        return;
    }
    CMyBitmap::ClearBuffer(true, false, 0);
    static 	BOOL m_bInitDandelion = false;
    if(!m_bInitDandelion)
    {
        m_objBrume.Add("brume");
        m_objBrume.Add("brume");
        m_bInitDandelion = true;
        CMyPos posBg, posWorld;
        this->Screen2World(0, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
        this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
        m_posBrume[0] = posBg;
        m_posBrume[2].x = posBg.x;
        m_posBrume[2].y = posBg.y + _SCR_HEIGHT;
        this->Screen2World(_SCR_WIDTH, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
        this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
        m_posBrume[1] = posBg;
        m_posBrume[3].x = posBg.x;
        m_posBrume[3].y = posBg.y + _SCR_HEIGHT;
    }
    // 左右判断
    // center ...
    CMyPos posBgCenter, posWorldCenter;
    this->Screen2World(_SCR_WIDTH / 2, _SCR_HEIGHT - 100, posWorldCenter.x, posWorldCenter.y);
    this->World2Bg(posWorldCenter.x, posWorldCenter.y, posBgCenter.x, posBgCenter.y);
    enum {_NONE_OUT = 0, _POS0_OUT = 1, _POS1_OUT = 2, _POS12_OUT = 3,
          _POS0_OUT_Y = 4, _POS2_OUT_Y = 5, POS02_OUT = 6
         };
    int nWhitchIsOutX = 	_NONE_OUT;
    int nWhitchIsOutY = 	_NONE_OUT;
    if(abs(posBgCenter.x - m_posBrume[0].x) > _SCR_WIDTH)
    {
        nWhitchIsOutX = _POS0_OUT;
    }
    if(abs(posBgCenter.x - m_posBrume[1].x) > _SCR_WIDTH)
    {
        if(nWhitchIsOutX == _POS0_OUT)
        {
            nWhitchIsOutX = _POS12_OUT;
        }
        else
        {
            nWhitchIsOutX = _POS1_OUT;
        }
    }
    if(abs(posBgCenter.y - m_posBrume[0].y) > _SCR_HEIGHT)
    {
        nWhitchIsOutY = _POS0_OUT_Y;
    }
    if(abs(posBgCenter.y - m_posBrume[2].y) > _SCR_HEIGHT)
    {
        if(nWhitchIsOutY == _POS0_OUT_Y)
        {
            nWhitchIsOutY = POS02_OUT;
        }
        else
        {
            nWhitchIsOutY = _POS2_OUT_Y;
        }
    }
    if(nWhitchIsOutX == _POS12_OUT || nWhitchIsOutY == POS02_OUT)
    {
        CMyPos posBg, posWorld;
        this->Screen2World(0 - 20, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
        this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
        m_posSnow[0] = posBg;
        m_posSnow[2].x = posBg.x;
        m_posSnow[2].y = posBg.y + _SCR_HEIGHT;
        this->Screen2World(_SCR_WIDTH + 20, _SCR_HEIGHT - 100, posWorld.x, posWorld.y);
        this->World2Bg(posWorld.x, posWorld.y, posBg.x, posBg.y);
        m_posSnow[1] = posBg;
        m_posSnow[3].x = posBg.x;
        m_posSnow[3].y = posBg.y + _SCR_HEIGHT;
    }
    if(nWhitchIsOutX == _POS0_OUT)
    {
        //根据1 位置调整
        m_posBrume[0].x = m_posBrume[1].x + _SCR_WIDTH;
        if(abs(posBgCenter.x - m_posBrume[0].x) > _SCR_WIDTH)
        {
            m_posBrume[0].x = m_posBrume[1].x - _SCR_WIDTH;
        }
        m_posBrume[2].x = m_posBrume[0].x;
    }
    if(nWhitchIsOutX == _POS1_OUT)
    {
        //根据1 位置调整
        m_posBrume[1].x = m_posBrume[0].x + _SCR_WIDTH;
        if(abs(posBgCenter.x - m_posBrume[1].x) > _SCR_WIDTH)
        {
            m_posBrume[1].x = m_posBrume[0].x - _SCR_WIDTH;
        }
        m_posBrume[3].x = m_posBrume[1].x;
    }
    // 上下判断
    if(nWhitchIsOutY == _POS0_OUT_Y)
    {
        //根据1 位置调整
        m_posBrume[0].y = m_posBrume[2].y + _SCR_HEIGHT;
        if(abs(posBgCenter.y - m_posBrume[0].y) > _SCR_HEIGHT)
        {
            m_posBrume[0].y = m_posBrume[2].y - _SCR_HEIGHT;
        }
        m_posBrume[1].y = m_posBrume[0].y;
    }
    if(nWhitchIsOutY == _POS2_OUT_Y)
    {
        //根据1 位置调整
        m_posBrume[2].y = m_posBrume[0].y + _SCR_HEIGHT;
        if(abs(posBgCenter.y - m_posBrume[2].y) > _SCR_HEIGHT)
        {
            m_posBrume[2].y = m_posBrume[0].y - _SCR_HEIGHT;
        }
        m_posBrume[3].y = m_posBrume[2].y;
    }
    m_objBrume.Process();
    m_objBrume.Show(m_posBrume[0]);
    m_objBrume.Show(m_posBrume[1]);
    m_objBrume.Show(m_posBrume[2]);
    m_objBrume.Show(m_posBrume[3]);
}
//--------------------------------------------------------------
