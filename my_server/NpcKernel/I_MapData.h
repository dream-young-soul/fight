
//**********************************************************
// 代码编辑器
//**********************************************************

// MapData的接口
// 仙剑修，2002.12.6

#include "common.h"
#include "windows.h"

#ifndef	I_MAPDATA_H
#define	I_MAPDATA_H

///////////////////////////////////////////////////////////////////////////////////////
// MapData
///////////////////////////////////////////////////////////////////////////////////////
const int	PASSAGE_NONE	= -1;

class IRoleAttr;
struct PASSPATH0;
class IMapData
{
public:
    ~IMapData() {}
    static IMapData*	CreateNew(int nMapDoc, LPCTSTR pszVersion = 0);
    virtual ULONG	Release()												PURE_VIRTUAL_FUNCTION_0
public: // modify. idTerrainItemType = NpcType*10 + Dir
    virtual bool	AddTerrainItem(OBJID idOwner, int nCellX, int nCellY, OBJID idTerrainItemType)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	DelTerrainItem(OBJID idOwner)							PURE_VIRTUAL_FUNCTION_0
public: // get
    virtual int		GetMapWidth()											PURE_VIRTUAL_FUNCTION_0
    virtual int		GetMapHeight()											PURE_VIRTUAL_FUNCTION_0

public: // application
    virtual int		GetPassage(int x, int y)								PURE_VIRTUAL_FUNCTION_0
    virtual bool	FindPath(PASSPATH0* pPathBuf, int nBufSize, POINT posSource, POINT posTarget, IRoleAttr* pAttr, int nEscapeSteps = 0)		PURE_VIRTUAL_FUNCTION_0

public: // cell
    virtual int		GetFloorAttr	(int nCellX, int nCellY)				PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetFloorMask	(int nCellX, int nCellY)				PURE_VIRTUAL_FUNCTION_0
    virtual int		GetFloorAlt		(int nCellX, int nCellY)				PURE_VIRTUAL_FUNCTION_0
    virtual int		GetSurfaceAlt	(int nCellX, int nCellY)				PURE_VIRTUAL_FUNCTION_0
    //...
public: // role
    virtual void	IncRole			(int nCellX, int nCellY, int nVal = 1)				PURE_VIRTUAL_FUNCTION
    virtual void	DecRole			(int nCellX, int nCellY, int nVal = 1)				PURE_VIRTUAL_FUNCTION
    virtual int		GetRoleAmount	(int nCellX, int nCellY)				PURE_VIRTUAL_FUNCTION_0
};



///////////////////////////////////////////////////////////////////////////////////////
class IRoleAttr
{
protected:
    //	~IRoleAttr() {}
    //	virtual ULONG	Release()												PURE_VIRTUAL_FUNCTION_0
public: // get
    //@
};


///////////////////////////////////////////////////////////////////////////////////////
struct PASSPATH0
{
    POINT	posStart;
    POINT	posStop;
    int		nSize;
    char	pPath[1];		// 变长
    int		GetSize() { return (sizeof(POINT) * 2 + sizeof(int) + nSize); }
};

///////////////////////////////////////////////////////////////////////////////////////
const int	_MAX_PATHSTEP		= 64;			// 最多搜索64步


#endif // I_MAPDATA_H
