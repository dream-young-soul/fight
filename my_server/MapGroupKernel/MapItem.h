
//**********************************************************
// 代码编辑器
//**********************************************************

// MapItem.h: interface for the CMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPITEM_H__9FB2A993_BFD6_47A9_B21A_4378262CCC0B__INCLUDED_)
#define AFX_MAPITEM_H__9FB2A993_BFD6_47A9_B21A_4378262CCC0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gameobj.h"
#include "ItemType.h"
#include "GameBlock.h"
#include "Timeout.h"
#include "Myheap.h"
#include "Item.h"

//////////////////////////////////////////////////////////////////////
const int	MAPITEM_ONTIMER_SECS		= 5;		// MAPMANAGER中的定时检查时间(太大不好-paled)
const int	MAPITEM_MONSTER_ALIVESECS	= 3 * 60;		// 怪物掉的地面物品的生存时间
const int	MAPITEM_USERMAX_ALIVESECS	= 3 * 60;		// 玩家扔的地面物品的生存时间MAX
const int	MAPITEM_USERMIN_ALIVESECS	= 1 * 60;		// 玩家扔的地面物品的生存时间MIN
const int	MAPITEM_ALIVESECS_PERPRICE	= 1000 / (MAPITEM_USERMAX_ALIVESECS - MAPITEM_USERMIN_ALIVESECS);		// 玩家扔的地面物品的生存时间(每多少钱加1秒)
const int	MAPITEM_PRIV_SECS			= 30;		// 打怪玩家优先时间
//const int	DROPMONEY_LIMIT				= 10000000;	// 扔钱的限制数量	—— 圣战中取消
const int	PICKMAPITEMDIST_LIMIT		= 0;		// 捡钱的限制距离

//////////////////////////////////////////////////////////////////////
class CMapItem : public CGameObj, IMapThing
{
protected:
    CMapItem();
    virtual ~CMapItem();

public:
    static CMapItem* CreateNew()			{ return new CMapItem; }
    void	ReleaseByOwner()				{ delete this; }
    bool	Create(OBJID idNewMapItem, CGameMap* pMap, POINT pos, OBJID idType, OBJID idOwner, int nMagic2 = 0, int nMagic3 = 0, bool bUnIdent = false, int nLuck = 0);				// 怪物掉
    bool	Create(OBJID idNewMapItem, CGameMap* pMap, POINT pos, ItemInfoStruct* pInfo, OBJID idOwner = ID_NONE);	// 玩家掉
    bool	CreateMoney(OBJID idNewMapItem, CGameMap* pMap, POINT pos, DWORD dwMoney, OBJID idOwner = ID_NONE);

public: // interface
    virtual IMapThing*	QueryMapThing()		{ return (IMapThing*)this; }

public: // IMapThing
    //	virtual ULONG	ReleaseByOwner()							PURE_VIRTUAL_FUNCTION_0
    virtual OBJID	GetID()									{ return m_id; }
    virtual int 	GetObjType()							{ return CGameObj::GetObjType(); }
    virtual int		GetPosX()								{ return m_nPosX; }
    virtual int		GetPosY()								{ return m_nPosY; }
    virtual CGameMap*	GetMap()								{ CHECKF(m_pMap); return m_pMap; }
    virtual void	SendShow(IRole* pRole);
protected:
    // return true: ppv返回对象指针
    virtual bool	QueryObj(OBJID idObjType, void** ppv)			{ if (idObjType == OBJ_MAPITEM) { return *ppv = this, true; } return *ppv = NULL, false; }
    virtual bool	QueryRole(void** ppv)					{ return *ppv = NULL, false; }

public: // application
    OBJID	GetType()										{ return m_Info.idType; }
    int		GetAmount()										{ return m_Info.nAmount; }
    OBJID	GetPlayerID()									{ return m_Info.idPlayer; }
    ItemInfoStruct*	GetInfo()								{ return &m_Info; }
    bool	IsPriv()										{ return !m_tPriv.IsTimeOut(); }
    bool	IsAlive()										{ return !m_tAlive.IsTimeOut(); }
    bool	IsMoney()										{ return GetType() >= ITEMTYPEMONEY_MIN && GetType() <= ITEMTYPEMONEY_MAX; }
    bool	IsGem()											{ return CItem::IsGhostGem(GetType());}
    void	BroadcastShow();
    int		GetWeight()										{ return CItem::GetWeight(GetType(), m_Info.nAmount); }
    void	BroadcastSet(bool bEnter);

protected:
    OBJID		m_id;
    ItemInfoStruct	m_Info;
    int			m_nPosX;
    int			m_nPosY;
    CTimeOut	m_tPriv;
    CTimeOut	m_tAlive;
    CGameMap*	m_pMap;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAPITEM_H__9FB2A993_BFD6_47A9_B21A_4378262CCC0B__INCLUDED_)
