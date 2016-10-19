
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DropRuleSet.h: interface for the CDropRuleMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPRULESET_H__54C6A5DF_23F2_45F2_9774_E0D8CAC05E9B__INCLUDED_)
#define AFX_DROPRULESET_H__54C6A5DF_23F2_45F2_9774_E0D8CAC05E9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "define.h"
/*
#include "ConstGameData.h"
#include "GameData.h"
#include "GameObj.h"
#include "T_SingleMap2.h"
*/
#include "DropRuleGroup.h"
#include "Myheap.h"
#include "I_mydb.h"
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
class IDatabase;
class CDropRuleMap
{
public:
    CDropRuleMap();
    virtual ~CDropRuleMap();

public:
    static CDropRuleMap*	CreateNew()	{ return new CDropRuleMap; }
    ULONG	Release()	{ delete this; return 0; }

    bool	Create(IDatabase*	pDb);
    void	Destroy();

    CDropRuleGroup*	GetObjByIndex(OBJID	idRuleGroup)	{ if (m_map[idRuleGroup]) { return m_map[idRuleGroup]; } return NULL; }

protected:
    typedef map<OBJID, CDropRuleGroup*>				MAP_RULE;
    typedef map<OBJID, CDropRuleGroup*>::iterator	Iter_t;

    MAP_RULE	m_map;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_DROPRULESET_H__54C6A5DF_23F2_45F2_9774_E0D8CAC05E9B__INCLUDED_)
