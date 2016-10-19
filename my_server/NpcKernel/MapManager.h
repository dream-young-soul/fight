
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MapManager.h: interface for the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPMANAGER_H__46D53D47_F3AD_48DC_B7CD_5C861DF99F47__INCLUDED_)
#define AFX_MAPMANAGER_H__46D53D47_F3AD_48DC_B7CD_5C861DF99F47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "GameMap.h"
#include "GameObjSet.h"


typedef	IGameObjSet<CGameMap>	IGameMapSet;
typedef	CGameObjSet<CGameMap>	CGameMapSet;

class CMapManager
{
public:
    CMapManager();
    virtual ~CMapManager();
    ULONG		Release() { delete this; return 0; }

    bool		Create();
    CGameMap*	QueryMap(OBJID idMap);
    IGameMapSet*	QuerySet() { CHECKF(m_pMapSet); return m_pMapSet; }

public:
    void OnTimer(DWORD nCurr);

protected:
    IGameMapSet*		m_pMapSet;
};

#endif // !defined(AFX_MAPMANAGER_H__46D53D47_F3AD_48DC_B7CD_5C861DF99F47__INCLUDED_)
