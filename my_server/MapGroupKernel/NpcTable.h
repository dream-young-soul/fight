
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcTable.h: interface for the CNpcTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCTABLE_H__8640C8B9_A80D_4390_93DA_CE64A2F86813__INCLUDED_)
#define AFX_NPCTABLE_H__8640C8B9_A80D_4390_93DA_CE64A2F86813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "gameobj.h"
#include "i_mydb.h"
#include "gamedata.h"
#include "t_GameDataMap.h"

///////////////////////////////////////////////////////
enum TABLEDATA
{
    TABLEDATA_NPCID = 1,
    TABLEDATA_TYPE,
    TABLEDATA_KEY,
    //	TABLEDATA_DATE_STAMP,
    TABLEDATA_DATASTR,
    TABLEDATA_DATA0,
    TABLEDATA_DATA1,
    TABLEDATA_DATA2,
    TABLEDATA_DATA3,
};
char	szNpcTableTable[];
typedef	CGameData<TABLEDATA, szNpcTableTable, szID>	CTableData;
typedef	CGameDataMap<CTableData>					CTableSet;
const int	TABLEDATA_SIZE		= 4;

///////////////////////////////////////////////////////
class CNpcTable
{
public:
    CNpcTable();
    virtual ~CNpcTable();
public:
    static CNpcTable* CreateNew()			{ return new CNpcTable; }
    void	Release()						{ delete this; }
    bool	Create(PROCESS_ID idProcess, CNpc* pOwner, OBJID idNpc);

public: // application
    bool	AddRecord(int nType, OBJID idKey, int setData[4], LPCTSTR szData);
    bool	DelRecord(int nType, OBJID idKey, int setData[4], LPCTSTR szData);	// del it when match all no 0 param
    bool	DelInvalid(int nType, int idxDateStamp);
    bool	DelRecord(OBJID idRecord);

public: // const
    CTableSet*	QuerySet()					{ return m_setTable; }
    bool	FindNext(int nType, int& nIter);			// order by ID, return false: no more syn
protected:
    CTableData*	GetRecord(int nType, OBJID idKey);

protected:
    PROCESS_ID	m_idProcess;
    CNpc*		m_pOwner;
    CTableSet*	m_setTable;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_NPCTABLE_H__8640C8B9_A80D_4390_93DA_CE64A2F86813__INCLUDED_)
