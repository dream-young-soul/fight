
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// UserList.h: interface for the CMapList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MAPLIST_HEADFILE)
#define MAPLIST_HEADFILE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "define.h"
#include "windows.h"
#include "NetworkDef.h"
#include <time.h>
#include <vector>
using namespace std;
using namespace world_kernel;

struct CMapSt
{
    OBJID		idMap;
    OBJID		idxMapGroup;
    int			nPortalX;
    int			nPortalY;

    CMapSt() { idMap = ID_NONE; }
    OBJID	GetID() { return idMap; }
};

class IDatabase;
class CMapList
{
public:
    CMapList();
    virtual ~CMapList();
    static CMapList* CreateNew()		{ return new CMapList; }
    bool	Create(IDatabase* pDb);
    ULONG	Release()			{ delete this; return 0; }

public:
    PROCESS_ID	GetMapProcessID(OBJID idMap);
    CMapSt*		GetMap(OBJID idMap);

protected:
    typedef vector<CMapSt*>	MAP_SET;
    MAP_SET	m_setMap;
};



#endif // !defined(MAPLIST_HEADFILE)
