
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MapItemManager.h: interface for the MapItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPITEMMANAGER_H__A03972B3_DB33_4491_A021_94FAE46E1E79__INCLUDED_)
#define AFX_MAPITEMMANAGER_H__A03972B3_DB33_4491_A021_94FAE46E1E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DMapItem.h"

typedef struct
{
    OBJID	idMapItem;
    OBJID	idItemType;
    CMyPos	posCell;
    C2DMapItem* pMapItem;
} MapItemInfo;

#include <vector>
using namespace std;
typedef vector<MapItemInfo*> VECTOR_MAPITEMINFO;


class CMapItemManager
{
public:
    CMapItemManager();
    virtual ~CMapItemManager();

private:
    VECTOR_MAPITEMINFO m_setItemInfo;

public:
    void  Process();
    void  AddMapItem(OBJID idMapItem, OBJID idItemType, CMyPos posCell);
    void  DelMapItem(OBJID idMapItem);
    OBJID GetMapItem(CMyPos posCell);
    OBJID GetFocusItem(CMyPos& posCell);
    void  Destroy();
};

#endif // !defined(AFX_MAPITEMMANAGER_H__A03972B3_DB33_4491_A021_94FAE46E1E79__INCLUDED_)
