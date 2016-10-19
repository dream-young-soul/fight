
//**********************************************************
// 代码编辑器
//**********************************************************

//----------------------------------------------------//
// TerrainLayer.h
//----------------------------------------------------//
#ifndef MAPDATA_HEAD
#define MAPDATA_HEAD
//----------------------------------------------------//
#include "I_MapData.h"
#include "Cell.h"
#include "TerrainObj.h"
#include "T_CompactObjSet.h"
#include <deque>
using namespace std;
//----------------------------------------------------//
enum {_ASTAR_NONE = 0, _ASTAR_CLOSE, _ASTAR_LEAVE};
//----------------------------------------------------//
typedef struct
{
    int nPosX;
    int nPosY;
    int nIndex;
} PassageInfo;
//----------------------------------------------------//
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
//----------------------------------------------------//
#include <vector>
using namespace std;
typedef vector<CCell* >  DEQUE_CELL;
typedef vector<PassageInfo* >  DEQUE_PASSAGE;
//----------------------------------------------------//

inline int POS2INDEX(int x, int y, int cx, int cy) { return (x + y * cx); }
inline int INDEX2X(int idx, int cx, int cy) { return (idx % cy); }
inline int INDEX2Y(int idx, int cx, int cy) { return (idx / cy); }

class CTerrainObj;
class CMapData : public IMapData
{
public:
    CMapData();
    ~CMapData();
    static CMapData* CreateNew(LPCTSTR pszFileName, LPCTSTR pszVersion);

protected: // interface ////////////////////////////////////////////////////
    ULONG	Release()	{ delete this; return 0; }
protected: // modify
    bool	AddTerrainItem(OBJID idOwner, int nCellX, int nCellY, OBJID idTerrainItemType);
    bool	DelTerrainItem(OBJID idOwner);
protected: // get
    int		GetMapWidth();
    int		GetMapHeight();
    CCell*	QueryCell(int nCellX, int nCellY);
    CCell*	QueryCell(POINT posCell);
protected: // application
    int		GetPassage(int x, int y);
    bool	FindPath(PASSPATH0* pPathBuf, int nBufSize, POINT posSource, POINT posTarget, IRoleAttr* pAttr, int nEscapeSteps = 0);
    // nEscapeSteps == 0 : Move Forward
protected: // cell
    virtual int		GetFloorAttr	(int x, int y)				{ CCell* ptr = QueryCell(x, y); CHECKF(ptr); return ptr->GetFloorAttr(m_set2Layer); }
    virtual DWORD	GetFloorMask	(int x, int y)				{ CCell* ptr = QueryCell(x, y); IF_NOT(ptr) return MASK_MASK; return ptr->GetFloorMask(m_set2Layer); }
    virtual int		GetFloorAlt		(int x, int y)				{ CCell* ptr = QueryCell(x, y); CHECKF(ptr); return ptr->GetFloorAlt(m_set2Layer); }
    virtual int		GetSurfaceAlt	(int x, int y)				{ CCell* ptr = QueryCell(x, y); CHECKF(ptr); return ptr->GetSurfaceAlt(); }
    //...
protected: // role
    virtual void	IncRole			(int x, int y, int nVal = 1)				{ CCell* ptr = QueryCell(x, y); CHECK(ptr); ptr->IncRole(nVal); }
    virtual void	DecRole			(int x, int y, int nVal = 1)				{ CCell* ptr = QueryCell(x, y); CHECK(ptr); ptr->DecRole(nVal); }
    virtual int		GetRoleAmount	(int x, int y)				{ CCell* ptr = QueryCell(x, y); CHECKF(ptr); return ptr->GetRoleAmount(); }

private: // a_start ///////////////////////////////////////////////////////
    void ClearNodeSet();
    NodeInfo* CreateNewNode();
    void ClearCloseSet()							{ m_nCloseNodeAmount = 0; }
    void ClearOpenSet()								{ m_setOpen.clear(); }
    void ClearPathSet()								{ m_setPath.clear(); }
    void FindPath(POINT posStart, POINT posTarget);
    void InitAStar();
    void GetPath(NodeInfo* pNode);
    int  GetPathAmount();
    NodeInfo* GetNodeByIndex(int nIndex);
    int  GetStep(POINT posCell);
    int GetHValue(POINT pos);
    BOOL AddSucNode(NodeInfo* pNode);
    NodeInfo* CheckBestNode();
    NodeInfo* CheckOpenList();
    BOOL CheckNode(POINT posNode, int nHight);
    int GetNodeSetAmount()							{ return m_setNode.size(); }
private: // member
    typedef deque<NodeInfo*>  DEQUE_NODE;
    DEQUE_NODE		m_setNode;
    int				m_nNodeIndex;
    DEQUE_NODE		m_setOpen;
    DEQUE_NODE		m_setClose;
    int				m_nCloseNodeAmount;
    DEQUE_NODE		m_setPath;
    POINT			m_posStart;
    POINT			m_posTarget;
    POINT			m_posRealTarget;
    DEQUE_CELL		m_setCellAStar;
    int				m_nPathMode;
    int				m_nLeaveDisc;

private:
    SIZE			m_sizeMap;
    typedef	CompactObjSet<CCell>	SELL_SET;
    SELL_SET		m_setCell;
    DEQUE_PASSAGE	m_setPassage;
    DEQUE_TERRAINOBJ	m_setMapObj;
    DEQUE2_LAYER	m_set2Layer;

protected: // ctrl
    auto_long		m_bDDVersion;

    // for maplayer
public:
    int  GetExitIndex(POINT posCell);			// return -1 : error

    //others
protected: // destroy
    void ClearCell();
    void ClearPassage();
    void ClearMapObj();
    void ClearLayerSet();
protected:
    void AddPassage(const PassageInfo* pInfo);
    void AddPassage(POINT posMap, int nIndex);
    void DelPassage(POINT posMap);
    CCell* GetCell(int nCellX, int nCellY);
    CCell* GetCell(int nIndex);
    bool LoadSurfaceCellData(FILE* fp);
    bool LoadDataPassage(FILE* fp);
    bool LoadTerrainItemData(FILE* fp);
    bool LoadPuzzle(char* pszFile);
    bool AddMapObj(CTerrainObj* pObj);
    bool DelMapObj(int idx);
private:
    bool PlaceTerrainObj(CTerrainObj* pTerrainObj);
    bool DisplaceTerrainObj(CTerrainObj* pTerrainObj);
};
//----------------------------------------------------//
#endif
//----------------------------------------------------//
