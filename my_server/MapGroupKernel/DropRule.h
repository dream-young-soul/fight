
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DropRule.h: interface for the CDropRule class.
//
// Àä×ÏÁú 2003-11-15
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPRULE_H__1A26CA92_A4A3_42C6_81F9_DD1A70FA5E38__INCLUDED_)
#define AFX_DROPRULE_H__1A26CA92_A4A3_42C6_81F9_DD1A70FA5E38__INCLUDED_

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
#include "Myheap.h"
#include "I_mydb.h"
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////
/*
const int _MAX_DROPRULE_ITEMTYPE_NUM	= 15;

struct RuleInfoStruct
{
	OBJID	id;
	int		nChance;
	OBJID	idItemType[_MAX_DROPRULE_ITEMTYPE_NUM];
};
*/
//////////////////////////////////////////////////////////////////////

enum DROPRULEDATA
{
    DROPRULEDATA_ID_	= 0,
    DROPRULEDATA_RULEID	= 1,
    DROPRULEDATA_CHANCE,
    DROPRULEDATA_ITEM0,
    DROPRULEDATA_ITEM1,
    DROPRULEDATA_ITEM2,
    DROPRULEDATA_ITEM3,
    DROPRULEDATA_ITEM4,
    DROPRULEDATA_ITEM5,
    DROPRULEDATA_ITEM6,
    DROPRULEDATA_ITEM7,
    DROPRULEDATA_ITEM8,
    DROPRULEDATA_ITEM9,
    DROPRULEDATA_ITEM10,
    DROPRULEDATA_ITEM11,
    DROPRULEDATA_ITEM12,
    DROPRULEDATA_ITEM13,
    DROPRULEDATA_ITEM14,
};

//////////////////////////////////////////////////////////////////////
class CDropRule
{
public:
    CDropRule();
    virtual ~CDropRule();

protected:
    typedef	std::vector<OBJID> ITEMTYPE_SET;

    OBJID			m_idRule;
    int				m_nChance;
    ITEMTYPE_SET	m_setItemType;

public:
    static	CDropRule*	CreateNew()	{ return new CDropRule; }
    ULONG	Release()	{ delete this; return 0; }

    bool	Create(IRecordset*	pRes);
    bool	LoadInfo(IRecord*	pRes);

    OBJID	GetDropItem(int nTotalChance, int nRand);

    bool	IsValid()	{ return (m_nChance > 0) && (m_setItemType.size() > 0); }
    int		GetChance()	{ return m_nChance; }

protected:
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_DROPRULE_H__1A26CA92_A4A3_42C6_81F9_DD1A70FA5E38__INCLUDED_)
