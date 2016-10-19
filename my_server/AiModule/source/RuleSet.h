
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// RuleSet.h: interface for the CRuleSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULESET_H__186F525F_61A3_40A7_8962_6A42A6D1A0D8__INCLUDED_)
#define AFX_RULESET_H__186F525F_61A3_40A7_8962_6A42A6D1A0D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycom.h"
#include "T_SingleMap.h"
#include "VarType.h"
#include "TickOver.h"
#include "myheap.h"
#include "autoptr.h"

//////////////////////////////////////////////////////////////////////
enum RULEDATA
{
    RULEDATA_ID,
    RULEDATA_OWNER_ID,
    RULEDATA_MODE,
    RULEDATA_CONDITION,
    RULEDATA_RESULT,
    RULEDATA_PRIORITY,
};

//////////////////////////////////////////////////////////////////////
class IDatabase;
class CRuleObj
{
    COM_STYLE(CRuleObj)
public:
    CRuleObj()	{}
    MYHEAP_DECLARATION(s_heap)
public: // interface
    OBJID	GetID()				const			{ return m_id; }

public: // application
    bool		Create(CSymbolTable* pSymbolTable, OBJID id, StringRef strMode, StringRef strCondition, StringRef strResult, int nPriority, int nNow, int nDurable);
    bool		ToNextTime(int nNow)		{ return m_tActive.ToNextTime(nNow); }

public: // const
    CFactRef	GetMode()		const			{ return m_cMode; }
    CFactArrayRef	GetCondition()	const			{ return m_setFacts; }
    CFactRef	GetResult()		const			{ return m_cResult; }
    CFactRef	GetPattern()	const			{ return m_cPattern; }
    int			GetPriority()	const			{ return m_nPriority; }

public: // dump
    String		Dump()	const
    {
        String str = "{";
        (str += m_cMode.Dump()) += ",";
        (str += m_setFacts.Dump()) += ",";
        (str += m_cResult.Dump()) += "}";
        return str;
    }

protected:
    OBJID		m_id;
    CFact		m_cMode;		// fact
    CFactArray	m_setFacts;
    CFact		m_cResult;
    CFact		m_cPattern;
    int			m_nPriority;
    CTickOver	m_tActive;
};
template<>
struct dump_traits<CRuleObj> { static String Dump(const CRuleObj& obj)		{ return obj.Dump(); } };

//////////////////////////////////////////////////////////////////////
class CAiCenter;
class CRuleSet
{
    COM_STYLE(CRuleSet)
protected:
    CRuleSet();
    /*virtual*/
    ~CRuleSet();

public:
    bool		Create(CAiCenter* pOwner, IDatabase* pDb, int nNow);

public:
    typedef		CSingleMap<CRuleObj>	RULE_SET;
    typedef		RULE_SET::Iterator		Iterator;
    Iterator	NewEnum		() 							{ return m_setRule->NewEnum(); }

protected:
    void		AddRule(OBJID idRule, StringRef strMode, StringRef strCondition, StringRef strResult, int nPriority);

protected:
    CAutoPtr<RULE_SET>	m_setRule;
    CAiCenter*	m_pOwner;
private: // temporary for create
    int			m_nStartNow;
};

#endif // !defined(AFX_RULESET_H__186F525F_61A3_40A7_8962_6A42A6D1A0D8__INCLUDED_)
