
//**********************************************************
// 代码编辑器
//**********************************************************

// MapManager.h: interface for the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPMANAGER_H__4DC38FE0_1341_4D20_8C0A_47517229F518__INCLUDED_)
#define AFX_MAPMANAGER_H__4DC38FE0_1341_4D20_8C0A_47517229F518__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameMap.h"
#include "MapItem.h"
#include "T_SingleObjSet2.h"
#include "T_MyQueue.h"
#include <vector>
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////
const OBJID	MAPITEM_FIRSTID			= 1;
const OBJID	MAPITEM_LASTID			= 4012345678;

//////////////////////////////////////////////////////////////////////
typedef	IGameObjSet<CGameMap>	IMapSet;
typedef	CGameObjSet<CGameMap>	CMapSet;
typedef	IGameObjSet<CMapItem>	IMapItemSet;
typedef	CGameObjSet<CMapItem>	CMapItemSet;
typedef	ISingleObjSet2<CGameMapData>	IStaticMapSet;
typedef	CSingleObjSet2<CGameMapData>	CStaticMapSet;

//////////////////////////////////////////////////////////////////////
enum PORTALDATA
{
    PORTALDATA_MAPID = 1,
    PORTALDATA_INDEX,
    PORTALDATA_X,
    PORTALDATA_Y,
};
char	szPortalTable[];
typedef	CGameData<PORTALDATA, szPortalTable, szID>		CPortalData;
typedef	ISingleObjSet2<CPortalData>			IPortalSet;
typedef	CSingleObjSet2<CPortalData>			CPortalSet;

//////////////////////////////////////////////////////////////////////
enum PASSWAYDATA
{
    PASSWAYDATA_MAPID = 1,
    PASSWAYDATA_INDEX,
    PASSWAYDATA_PROTALMAP,
    PASSWAYDATA_PROTALIDX,
};
char	szPasswayTable[];
typedef	CGameData<PASSWAYDATA, szPasswayTable, szID>	CPasswayData;
typedef	ISingleObjSet2<CPasswayData>		IPasswaySet;
typedef	CSingleObjSet2<CPasswayData>		CPasswaySet;

//////////////////////////////////////////////////////////////////////
class CMapItem;
class IMapManager
{
public:
    virtual ~IMapManager() {}
    virtual void		Release()							PURE_VIRTUAL_FUNCTION
    virtual CGameMap*	QueryMap(OBJID idMap, BOOL bLoad = true)				PURE_VIRTUAL_FUNCTION_0
    virtual IMapSet*	QuerySet()							PURE_VIRTUAL_FUNCTION_0
    virtual CGameMap*	GetGameMap(OBJID idMap, BOOL bLoad = true)				PURE_VIRTUAL_FUNCTION_0
    virtual int			GetGameMapAmount()					PURE_VIRTUAL_FUNCTION_0
    virtual CGameMap*	GetGameMapByIndex(int idx)			PURE_VIRTUAL_FUNCTION_0
    virtual PROCESS_ID	GetProcessID(OBJID idMap)			PURE_VIRTUAL_FUNCTION_0
    virtual void		OnTimer(time_t tCurr)				PURE_VIRTUAL_FUNCTION
    virtual CMapItem*	QueryMapItem(OBJID idMapItem)		PURE_VIRTUAL_FUNCTION_0
    virtual CMapItemSet*	QueryMapItemSet()				PURE_VIRTUAL_FUNCTION_0
    virtual OBJID		SpawnMapItemID()					PURE_VIRTUAL_FUNCTION_0
    virtual bool		FindPortal(POINT* pPos, OBJID idMap, int idxPortal)		PURE_VIRTUAL_FUNCTION_0
    virtual bool		FindPassway(OBJID* pidTarget, int* pidxPortal, OBJID idMap, int idxPortal)		PURE_VIRTUAL_FUNCTION_0
    virtual OBJID		CreateDynaMap(const NewMapInfo* pInfo)	PURE_VIRTUAL_FUNCTION_0
    virtual IStaticMapSet*	QuerySystemMapSet()				PURE_VIRTUAL_FUNCTION_0
    virtual OBJID	SpawnNewPetID()								PURE_VIRTUAL_FUNCTION_0
    virtual void	RecyclePetID(OBJID id)						PURE_VIRTUAL_FUNCTION
    virtual OBJID	SpawnNewNpcID()								PURE_VIRTUAL_FUNCTION_0
    virtual void	RecycleDynaNpcID(OBJID id)					PURE_VIRTUAL_FUNCTION
    virtual OBJID	SpawnNewTrapID()							PURE_VIRTUAL_FUNCTION_0
    virtual void	RecycleTrapID(OBJID id)						PURE_VIRTUAL_FUNCTION
    virtual OBJID	SpawnNewCallPetID()							PURE_VIRTUAL_FUNCTION_0
    virtual void	RecycleCallPetID(OBJID id)					PURE_VIRTUAL_FUNCTION
};

class CMapManager : IMapManager
{
public:
    CMapManager(PROCESS_ID idProcess);
    virtual ~CMapManager();
    IMapManager*	GetInterface() { return (IMapManager*)this; }
    bool			Create();
    //	virtual CGameMap*	CreateGameMap();
    virtual void	OnTimer(time_t tCurr);

public:
    virtual void		Release();
    virtual CGameMap*	QueryMap(OBJID idMap, BOOL bLoad = true)			{ CHECKF(m_pMapSet); return GetGameMap(idMap, bLoad); }
    virtual IMapSet*	QuerySet()						{ CHECKF(m_pMapSet); return m_pMapSet; }
    virtual CGameMap*	GetGameMap(OBJID idMap, BOOL bLoad = true);
    virtual int			GetGameMapAmount()				{ return m_pMapSet->GetAmount(); }
    virtual CGameMap*	GetGameMapByIndex(int idx)		{ return m_pMapSet->GetObjByIndex(idx); }
    virtual bool		FindPortal(POINT* pPos, OBJID idMap, int idxPortal);
    virtual bool		FindPassway(OBJID* pidTarget, int* pidxPortal, OBJID idMap, int idxPortal);
    virtual OBJID		CreateDynaMap(const NewMapInfo* pInfo);

    // 预定义地图集，不包含玩家地图。
    virtual IStaticMapSet*	QuerySystemMapSet()			{ extern IStaticMapSet* g_setStaticMap; CHECKF(g_setStaticMap); return g_setStaticMap; }

protected: // internal port
    bool	IsDynaMap(OBJID idMap)						{ return idMap > DYNAMIC_MAP_ID; }



public: // mapitem
    virtual CMapItem*	QueryMapItem(OBJID idMapItem)	{ CHECKF(m_pMapItemSet); return m_pMapItemSet->GetObj(idMapItem); }
    virtual CMapItemSet*	QueryMapItemSet()			{ CHECKF(m_pMapItemSet); return m_pMapItemSet; }
    virtual OBJID		SpawnMapItemID();

public:
    virtual PROCESS_ID	GetProcessID(OBJID idMap);

public: // spawn id
    OBJID	SpawnNewPetID();
    void	RecyclePetID(OBJID id)						{ if (id != ID_NONE) { m_setPetRecycle.push(id); } }
    OBJID	SpawnNewNpcID();
    void	RecycleDynaNpcID(OBJID id)					{ if (id != ID_NONE) { m_setDynaNpcRecycle.push(id); } }
    OBJID	SpawnNewTrapID();
    void	RecycleTrapID(OBJID id)						{ if (id != ID_NONE) { m_setTrapRecycle.push(id); } }
    OBJID	SpawnNewCallPetID();
    void	RecycleCallPetID(OBJID id)					{ if (id != ID_NONE) { m_setCallPetRecycle.push(id); } }
protected:
    OBJID				m_idLastPet;
    CMyQueue<OBJID>		m_setPetRecycle;
    OBJID				m_idLastDynaNpc;
    CMyQueue<OBJID>		m_setDynaNpcRecycle;
    OBJID				m_idLastTrap;
    CMyQueue<OBJID>		m_setTrapRecycle;
    OBJID				m_idLastCallPet;
    CMyQueue<OBJID>		m_setCallPetRecycle;

protected: // ctrl
    PROCESS_ID	m_idProcess;

protected:
    IMapSet*		m_pMapSet;
    IPortalSet*		m_setPortal;
    IPasswaySet*	m_setPassway;

protected: // mapitem
    CMapItemSet*	m_pMapItemSet;
    OBJID			m_idLastMapItem;
    CTimeOut		m_tMapItem;

public:
    MYHEAP_DECLARATION(s_heap)
};




#endif // !defined(AFX_MAPMANAGER_H__4DC38FE0_1341_4D20_8C0A_47517229F518__INCLUDED_)
