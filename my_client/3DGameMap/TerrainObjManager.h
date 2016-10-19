
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TerrainObjManager.h: interface for the CTerrainObjManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAINOBJMANAGER_H__DB124777_3CC5_43E1_8AA1_D92EA75013D8__INCLUDED_)
#define AFX_TERRAINOBJMANAGER_H__DB124777_3CC5_43E1_8AA1_D92EA75013D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include "2DMapTerrainObj.h"
#include <vector>
using namespace std;

class CTerrainObjManager
{
public:
    CTerrainObjManager();
    virtual ~CTerrainObjManager();
public:
    typedef struct
    {
        char				szName[_MAX_NAMESIZE];
        int					nDir;
        int					nType;
        OBJID				idNpc;
        C2DMapTerrainObj*	pObj;
        CMyPos				posCell;
    } TerrainObjInfo;
private:
    vector<TerrainObjInfo*> m_setTerrainObjInfo;
public:
    void	Process();
    void	Destroy();
    void	AddNpc(int nType, int nDir, OBJID id, CMyPos posCell);
    void	Delete(OBJID id);
    void	SetDir(OBJID id, int nDir);
    void	SetPos(OBJID id, CMyPos posNpc);
    OBJID	GetFocus();
    int		GetLook(OBJID id);
    void	GetPos(OBJID id, CMyPos& posCell);
    BOOL	IsFocus(OBJID id);
};

#endif // !defined(AFX_TERRAINOBJMANAGER_H__DB124777_3CC5_43E1_8AA1_D92EA75013D8__INCLUDED_)
