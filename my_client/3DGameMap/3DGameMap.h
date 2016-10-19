
//**********************************************************
// 代码编辑器
//**********************************************************

// GameMap.h ...

#ifndef _GAMEMAP_H
#define _GAMEMAP_H

//-----------------------------
// header ...
#include "Radar.h"
#include "MapLayer.h"
#include "InteractiveLayer.h"
#include "TerrainLayer.h"
#include "MapLayer.h"

#include "PuzzleBmp.h"
#include "Layer.h"
#include "MapItemManager.h"
#include "Path.h"
#include "MagicEffectManager.h"
#include "TerrainEffectManager.h"
#include "InfoViewManager.h"
#include "3DRoleData.h"
#include "TerrainObjManager.h"
//-----------------------------
// Constant ...
const int _NORMAL_SCALE = 128;
const BOOL _SHOW_MASK = false;
const BOOL _SHOW_TERRAIN = false;
const BOOL _SHOW_ALTITUDE = false;
const BOOL _SHOW_EXIT = false;

const int _MAX_STEP = 360;
const int _MAX_DIRECTION	= 8;
const int _DELTA_X[8] = {0, -1, -1, -1, 0, 1, 1, 1};
const int _DELTA_Y[8] = {1, 1, 0, -1, -1, -1, 0, 1};

const int _CELL_WIDTH	= 64;
const int _CELL_HEIGHT	= 32;
const int _BLOCK_SIZE	= 18;
int  GetBaseAltitude(CMyPos posCell);
//-----------------------------

enum ENUM_MAPTYPE
{
    MAPTYPE_NORMAL				= 0x0000,
    MAPTYPE_PKFIELD				= 0x0001,
    MAPTYPE_CHGMAP_DISABLE		= 0x0002,
    MAPTYPE_RECORD_DISABLE		= 0x0004,
    MAPTYPE_PK_DISABLE			= 0x0008,
    MAPTYPE_BOOTH_ENABLE		= 0x0010,
    MAPTYPE_TEAM_DISABLE		= 0x0020,	// 32
    MAPTYPE_TELEPORT_DISABLE	= 0x0040,	// 64					// chgmap by action
    MAPTYPE_SYN_MAP				= 0x0080,	// 128
    MAPTYPE_PRISON_MAP			= 0x0100,	// 256
    MAPTYPE_FLY_DISABLE			= 0x00200,
    MAPTYPE_FAMILY				= 0x00400,
};

struct CellRes
{
    DWORD X;
    DWORD Y;
    DWORD pResTmpID;
};
typedef map<__int64, CellRes*> MAP_CELLRES;

struct  NodeInfo
{
    int nX;
    int nY;
    int nG; // 目前代价
    int nH; // 估计代价, 改为总代价，减少计算量
    int nStep;
    int nDir;
    NodeInfo* pNode;
    NodeInfo* pChildNode;
};
enum REGION_TYPE
{
    REGION_NONE					= 0,
    REGION_CITY					= 1,
    REGION_WEATHER				= 2,
    REGION_STATUARY				= 3,
    REGION_DESC					= 4,
    REGION_GOBALDESC			= 5,
    REGION_DANCE				= 6,
    REGION_PK_PROTECTED			= 7,
};

struct  RegionEffectInfo
{
    int	  nIndex;
    DWORD idMap;
    int	  nRegionType;
    int   nX;
    int   nY;
    int	  nW;
    int   nH;
    char  szEffect[2][64];
    int   nDelayTime;
};
typedef vector<RegionEffectInfo*> VECTOR_REGIONEFFECT;

#include <deque>
using namespace std;
typedef deque<CMapLayer* >  DEQUE_MAPLAYER;
typedef deque<CellInfo* >  DEQUE_CELL;
typedef deque<NodeInfo*>  DEQUE_NODE;
typedef deque<RegionInfo*>  DEQUE_REGIONINFO;
//-----------------------------
class CTerrainLayer;
class CFloorLayer;
class CSkyLayer;

class CGameMap
{
private:
    DEQUE_MAPLAYER		m_setMapLayer;
    CMySize				m_sizeMap;      // by cell
    CMySize				m_szieWorld;    // by World
    BOOL				m_bCreate;

    CMyPos				m_posOrigin;
    CMyPos				m_posView;
    CellInfo*			m_pCellInfo;
    CMyPos				m_posViewOffset;
    DWORD				m_dwVersion;
    CMyPos				m_posPlayerMap;
    BOOL				m_bPlayerMap[37][37];

private: // the interaface for A*
    DEQUE_NODE m_setNode;
    int m_nNodeIndex;
    DEQUE_NODE m_setOpen;
    DEQUE_NODE m_setClose;
    int m_nCloseNodeAmount;
    DEQUE_NODE m_setPath;

    CMyPos m_posStart;
    CMyPos m_posTarget;
    CMyPos m_posRealTarget;
    DEQUE_CELL  m_setCellAStar;
    int		m_nPathMode;
    int		m_nLeaveDisc;
    BOOL	m_bShowMapObjInfo;

    //----------------------------------------------------------------------
    // A*
private:
    BOOL CheckPlayerMap(CMyPos posCell);
    void BuildPlayerMap(CMyPos posStart);
    void ClearNodeSet();
    NodeInfo* CreateNewNode();
    void ClearCloseSet();
    void ClearOpenSet();
    void ClearPathSet();
    void FindPath(CMyPos posStart, CMyPos posTarget);
    void InitAStar();
    void GetPath(NodeInfo* pNode);
    int  GetPathAmount();
    NodeInfo* GetNodeByIndex(int nIndex);
    int  GetStep(CMyPos posCell);
    int GetHValue(CMyPos pos);
    BOOL AddSucNode(NodeInfo* pNode);
    NodeInfo* CheckBestNode();
    NodeInfo* CheckOpenList();
    BOOL CheckNode(CMyPos posNode, int nHight);
    int GetNodeSetAmount() {return m_setNode.size();}
    void ShowPath();
    void GetBestTarget(CMyPos& posCell);

    //		void FindPath(CMyPos posStart, CMyPos posTarget);

    //----------------------------------------------------------------------
    // others
private:
    void SetCamera(CMyPos posView);

    //----------------------------------------------------------------------
    // public Interface
public:
    CGameMap();
    virtual ~CGameMap();

public:
    void Reset();
    void Show(BOOL bMapItemInfo = false);
    void ShowMiniMap(CMySize& sizeMap);
    void SetShowExpFlag(BOOL bShow);
    void AddExp(int nExp);
    void ShowExp();
    void ClearExpSet();
    void Process(void* pInfo);


    void GetMapSize(CMySize& infoSize);
    void SetMapSize(CMySize infoSize);

    void GetWorldSize(CMySize& infoSize);

    void ResetViewPos();
    void SetViewPos(CMyPos posView, BOOL bShiftViewPos = false, BOOL bBgLimit = true);
    void GetViewPos(CMyPos& posShow);

    void GetBgWorldPos	(UINT& uWorldX, UINT& uWorldY);
    void GetBgViewport	(int& nViewportX, int& nViewportY);
    void GetBgSize(CMySize& infoSize);

    int  GetExitIndex(CMyPos posCell);
    BOOL IsPosVisible(CMyPos posCell, int nExtendSize = 8);

    BOOL LoadDataMap(char* pszFileName);
    void SaveDataMap(char* pszFileName);

    BOOL LoadTextMap(char* pszFileName);
    void SaveTextMap(char* pszFileName);

    BOOL Create(DWORD idDoc);
    void FindPath(CPath& objPath, CMyPos posStart, CMyPos posTarget);
    BOOL TestPath(CMapObj* pMapObj, CMyPos posTarget);

    int  GetAltitude(CMyPos posCell);
    int  GetBaseAltitude(CMyPos posCell);
    void Sampling(DEQUE_CELL& setCell, CMyPos& posHighest, CMyPos posStart, CMyPos posEnd, int nHits);

    // for cell
public:
    void DestroyCell();
    void DelLayerInfo(LayerInfo* pLayerInfo);
    void CreateCell();
    void AddLayer(CellInfo* pCellInfo, LayerInfo* pLayerInfo);
    void DelLastLayer(CellInfo* pCellInfo);
    int  GetGroundAltitude(CellInfo* pCellInfo);
    DWORD GetGroundMask(CellInfo* pCellInfo);
    void  SetGroundMask(CellInfo* pCellInfo, DWORD dwMask);
    //----------------------------------------------------------------------
    // 坐标转换
public:
    void	World2Cell	(int iWorldX, int iWorldY, int& iCellX, int& iCellY);
    void	World2Screen(int iWorldX, int iWorldY, int& iScrX, int& iScrY);
    void	Cell2World	(int iCellX, int iCellY, int& iWorldX, int& iWorldY);
    void	Cell2Screen	(int iCellX, int iCellY, int& iScreenX, int& iScreenY);
    void	Screen2World(int iScrX, int iScrY, int& iWorldX, int& iWorldY);
    void	Screen2Cell	(int iScrX, int iScrY, int& iCellX, int& iCellY);
    void	World2Bg	(int iWorldX, int iWorldY, int& iBgX, int& iBgY);
    void	Bg2World	(int iBgX, int iBgY, int& iWorldX, int& iWorldY);
    void	Mouse2Cell(int nMouseX, int nMouseY, int& nCellX, int& nCellY);
    void	Cell2Map( int nCellX, int nCellY, int& nMapX, int& nMapY ) ;
    void	Map2Cell( int nMapX, int nMapY, int& nCellX, int& nCellY ) ;
    //----------------------------------------------------------------------
    // static
public:
    static LayerInfo* GetLastLayerInfo(CellInfo* pCellInfo);

    static int		GetDirection		(int x0, int y0, int x1, int y1);
    static double	GetPreciseDistance	(int x0, int y0, int x1, int y1);
    static int		GetDistance			(int x0, int y0, int x1, int y1, int size = 1);

    inline static int	GetMask				(LayerInfo* pInfo);
    inline static int	GetTerrain			(CellInfo* pCellInfo);
    inline static int	GetAltitude			(LayerInfo* pInfo);

    inline static void SetMask(int nMask, LayerInfo* pInfo);
    inline static void	SetTerrain(int nTerrain, LayerInfo* pInfo);
    inline static void SetAltitude(int nAltitude, LayerInfo* pInfo);
    //----------------------------------------------------------------------
    // layer operate..
public:
    int GetTerrain(CMyPos posCell);
    int GetTerrain(int nPosX, int nPosY);
    CellInfo* GetCell(int iPosX, int iPosY);
    int  GetLayerAmount();
    CMapLayer* GetLayerByIndex(int nIndex);
    void DelLayer(int nIndex);
    void LayerUp(int nIndex);
    void LayerDn(int nIndex);

    void GetMapObjSize(CMySize& infoSize, CMapObj* pObj);

    void ApplyObjShow(CMapObj* pMapObj);
    void AddInteractiveObj(CMapObj* pMapObj);
    void DelInteractiveObj(CMapObj* pMapObj);

    void ClearMapLayer();
    void AddLayer(CMapLayer* pLayer, int nIndex = -1); //nIndex表示插入以后的位置

    CTerrainLayer* GetTerrainLayer();
    CInteractiveLayer* GetInteractiveLayer();
    CFloorLayer* GetFloorLayer();
    CSkyLayer* GetSkyLayer();

private:
    CMapItemManager m_objItemManager;
    CTerrainObjManager m_objTerrainObjManager;

    CMagicEffectManager	m_objMagicEffectManager;
    CTerrainEffectManager m_objTerrainEffectManager;
public:
    void AddMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell);
    void DelMapItem(OBJID idMapItem);
    OBJID GetMapItem(CMyPos posCell);
    OBJID GetFocusMapItem(CMyPos& posCell);

    void Add2DMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell, char* szName = NULL);
    void Del2DMapItem(OBJID idMapItem);
    OBJID Get2DMapItem(CMyPos posCell);
    OBJID Get2DFocusMapItem(CMyPos& posCell);

    void Add3DMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell);
    void Del3DMapItem(OBJID idMapItem);
    OBJID Get3DMapItem(CMyPos posCell);
    OBJID Get3DFocusMapItem(CMyPos& posCell);

    C3DMapEffect*  Add3DMapEffect(CMyPos posWorld, char* pszIndex, BOOL bDelself = true);
    void Del3DMapEffect(CMyPos posWorld);
    void Del3DMapEffect(C3DMapEffect* pEffect);
    void AddMagicEffect(CMagicEffect* pEffect);

    void Add3DMapMagicItem(OBJID idType, OBJID id, CMyPos posCell);
    void LastMapMagicItem(OBJID id);
    void EndMapMagicItem(OBJID id);

    // data memeber
    OBJID		m_idMap;
    OBJID		m_idDoc;
    DWORD		m_dwType;
    BOOL		m_bShowMiniMap;
    CRadar		m_objRadar;
    DWORD		m_dwMapARGB;
    char		m_szMapName[64];
    BOOL		m_bSnow;
public:
    void	SetID(OBJID idMap);
    OBJID	GetID();
    void	SetIDDoc(OBJID idMapDoc);
    OBJID	GetIDDoc();
    void	SetType(DWORD dwType);
    DWORD	GetType();
    const char*	GetMapName() { return m_szMapName;}


    void ShowCellRange();
    void ShowCell(BOOL bGride, BOOL bMask, BOOL bArea, BOOL bAltitude);
    void InitMask();
    void SetShowMapObjInfoFlag(BOOL bShow) {m_bShowMapObjInfo = bShow;}
    BOOL GetShowMapObjInfoFlag() {return m_bShowMapObjInfo;}
    DWORD GetARGB() {return m_dwMapARGB;}
    void SetARGB(DWORD dwARGB);
    void CleanForEditor();
    void GetMapSizeByPuzzle(CMySize& sizeMap);
    void CreateMapByPuzzle(char* pszPuzzleFile);
    void AddTerrainMagicUnit(CTerainEffectUnit* pUnit) {m_objTerrainEffectManager.AddUnit(pUnit);}
    BOOL Outof9Block(CMyPos posHero, CMyPos posTarget);
    void ProcessManager();


    // map Scale Show pos
    void MapScaleShowPos(int& nX, int& nY);
    void MapScaleMousePos(int& nX, int& nY);
    void SetShowExp(DWORD dwExp);
private:
    int		m_nScale;
    int		m_nRealScreenWidth;
    int		m_nRealScreenHeight;

public:
    void	SetScale(int nScale);
    int		GetScale() {return m_nScale;}

    // search friend and so on ...
private:
    DWORD	m_dwBeginShowFriendPos;
    CMyPos	m_posFriend;

    // region and so on
private:
    BOOL	m_bRegionNameChangeCount;
    char	m_szRegionName[64];
    char	m_szRegionEffect[64];
    char	m_szRegionDes[128];
    int		m_RegionNShowType;
    int		m_RegionDShowType;
    int		m_RegionDPos;
    DWORD	m_dwDesColor;
    DWORD	m_dwNameColor;

    DWORD	m_dwRegionEffectBegin;
    BOOL	m_bShowRegionEffect;
    BOOL	m_bShowGobalRegionEffect;
    DWORD	m_dwRegionNameChangeBegin;
    BOOL	m_bShowExp;
    DWORD	m_dwOldExp;
    DWORD	m_dwTimeExpBegin;
    DWORD	m_dwShowExp;
    CGame3DEffectEx	 m_objRegionEffect;
    DEQUE_REGIONINFO m_setRegionData;
    VECTOR_INDEXLAYER m_setIndexLayerInfo;

public:
    void	ProcessRegion(CMyPos posCell);
    void	ShowRegionName();

    BOOL	ShowRegionNameFade();
    BOOL	ShowRegionDesFade(BOOL bTline);

    BOOL	ShowRegionNameRoll();
    BOOL	ShowRegionDesRoll(BOOL bTline);

    BOOL	ShowRegionNameWindow();
    BOOL	ShowRegionDesWindow(BOOL bTline);

    void	CreateRegionData();
    void	DestroyRegionData();
    void	SelectRegionData();
    BOOL	IsPkRegion(CMyPos posCell);
public:
    void	SetFriendPos(CMyPos posFriend) {m_dwBeginShowFriendPos = ::TimeGet(); m_posFriend = posFriend;}
    void	GetFriendPos(CMyPos& posFriend) {posFriend = m_posFriend;}
    DWORD	GetFriendBeginShow() {return m_dwBeginShowFriendPos;}

    DWORD	CellIndexCounter();
    void	CellIndexUniteCounter(DWORD& dwSame, DWORD& dwDif);

    // screen effect ...
private:
    struct
    {
        DWORD	dwTimeBegin;
        DWORD	dwFrameInterval;
        DWORD	dwFrameAmount;
        DWORD	dwSwing;
        BOOL	bOn;
    } ShakeEffectInfo;

    struct
    {
        int		nOldScale;
        DWORD	dwTimeBegin;
        DWORD	dwFrameInterval;
        DWORD	dwFrameAmount;
        DWORD	dwSwing;
        BOOL	bOn;
    } ScaleEffectInfo;

    struct
    {
        DWORD	dwOldColor;
        DWORD	dwTimeBegin;
        DWORD	dwChangeDuration;
        DWORD	dwChangeBackDuration;
        DWORD	dwDuration;
        BOOL	bOn;
        BYTE	bRed;
        BYTE	bGreen;
        BYTE	bBlue;
        BYTE	bRedE;
        BYTE	bGreenE;
        BYTE	bBlueE;
    } ColorEffectInfo;

public:
    enum {_SCREEN_EFFECT_NONE = 0, _SCREEN_EFFECT_NORMAL = 1};
    void	BeginShakeEffect(int nEffectType = _SCREEN_EFFECT_NORMAL);
    void	BeginScaleEffect(int nEffectType = _SCREEN_EFFECT_NORMAL);
    void	BeginColorEffect(int nEffectType = _SCREEN_EFFECT_NORMAL);
    void	ProcessShakeEffect();
    void	ProcessScaleEffect();
    void	ProcessColorEffect();
public:
    CInfoViewManager m_InfoViewManager;
public:
    TerrainWeatherInfo* GetDefaultWeatherInfo();

    // for terrain npc
public:
    void AddTerrainNpc(int nType, int nDir, OBJID id, CMyPos posCell);
    void DeleteTerrainNpc(OBJID id);
    void SetTerrainNpcDir(OBJID id, int nDir);
    void SetTerrainNpcPos(OBJID id, CMyPos posCell);
    void GetTerrainNpcPos(OBJID id, CMyPos& posCell);
    int	GetTerrainNpcLook(OBJID id);
    OBJID GetFocusTerrainNpc();
    BOOL IsTerrainNpcFocus(OBJID id);
public:
    void	DDALine(int x0, int y0, int x1, int y1, vector<POINT>& vctPoint);
    char*	GetRegionName() {return m_szRegionName;}

    // for resource edit
public:
    void SetShowRes(BOOL bShow) {m_bShowRes = bShow;}

    void LoadMapRes(char* szFileName);
    void SaveMapRes(char* szFileName);

    void PutCellRes(DWORD x, DWORD y, DWORD idTmp);
    void ClearRes();
    void DelCellRes(DWORD x, DWORD y);
private:
    MAP_CELLRES		m_setCellRes;
    BOOL			m_bShowRes;
    CellRes*		GetCellRes(DWORD x, DWORD y);
private:

    VECTOR_REGIONEFFECT	m_setRegionEffect;

    DWORD			m_dwTimeRegionEffect;
    int				m_nCurRegionEffectIndex;

    CGame3DEffectEx m_objSnow;
    CMyPos			m_posSnow[4];

    BOOL			m_bMapShowWeather;
    void			ShowSnow();

    BOOL			m_bBrume;
    CGame3DEffectEx m_objBrume;
    CMyPos			m_posBrume[4];
    void			ShowBrume();

    void			ProcessRegionEffect(CMyPos posCell);
    bool			CreateRegionEffectInfo();
    void			DestroyRegionEffectInfo();
    int 			GetRegionEffectInfo(CMyPos posCell);
private:
    BOOL			m_bShowScreenInfo;
    void			ShowScreenInfo();
public:
    void			SwitchScreenInfo() {m_bShowScreenInfo = !m_bShowScreenInfo;}
    void			BuildMapResSql(const char* pszFileName, const char* pszTemplate);
};

//------------------------------------------
extern CGameMap g_objGameMap;

#ifdef _ANALYSIS_ON
extern BOOL g_bShowAnalysisInfo;
#endif

#endif
