
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcShop.h: interface for the CNpcShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCSHOP_H__92EE4D73_A647_49A7_9F33_9168AA574529__INCLUDED_)
#define AFX_NPCSHOP_H__92EE4D73_A647_49A7_9F33_9168AA574529__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GameObjSet.h"
#include "ItemType.h"
#include "Myheap.h"

typedef	IGameObjSet<CItemTypeData>	IGoodsSet;
typedef	CGameObjSet<CItemTypeData>	CGoodsSet;

class CUser;
class CNpcShop
{
protected:
    CNpcShop();
    virtual ~CNpcShop();
public:
    static CNpcShop* CreateNew()						{ return new CNpcShop; }
    ULONG	Release()									{ delete this; return 0; }

public:
    bool	Create(PROCESS_ID idProcess, CNpc* pOwner, OBJID idNpc);
    CItemTypeData*	QueryItemType(OBJID idType)			{ return m_setGoods->GetObj(idType); }
    bool	IsOpen();
    int		Rebate(int nPrice, OBJID idSyn, int nRankShow);

protected: // data
    IGoodsSet*		m_setGoods;

private: // ctrl
    CNpc*			m_pOwner;
    PROCESS_ID		m_idProcess;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_NPCSHOP_H__92EE4D73_A647_49A7_9F33_9168AA574529__INCLUDED_)
