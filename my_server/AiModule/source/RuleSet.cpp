
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// RuleSet.cpp: implementation of the CRuleSet class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "mycom.h"
#include "Fact.h"
#include "SymbolTable.h"
#include "FactSet.h"
#include "RuleSet.h"
#include "i_mydb.h"
#include "Syntax.h"
#include "aicenter.h"

MYHEAP_IMPLEMENTATION(CRuleObj, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CRuleObj::Create(CSymbolTable* pSymbolTable, OBJID id, StringRef strMode, StringRef strCondition, StringRef strResult, int nPriority, int nNow, int nDurable)
{
    LOGDUMP("Add Rule: %s; %s; %s; %d", (LPCTSTR)strMode, (LPCTSTR)strCondition, (LPCTSTR)strResult, nPriority);
    m_id			= id;
    AnalyzeFact(pSymbolTable, strMode, &m_cMode);
    AnalyzeFactSet(pSymbolTable, strCondition, &m_setFacts);
    AnalyzeFact(pSymbolTable, strResult, &m_cResult, &m_cPattern);
    m_nPriority		= nPriority;
    m_tActive.Startup(nNow, nDurable);
    LOGDUMP("Add rule: mode:%s, condition:%s, rule:%s, %s", (LPCTSTR)m_cMode.Dump(), (LPCTSTR)m_setFacts.Dump(), (LPCTSTR)m_cResult.Dump(), (LPCTSTR)m_cPattern.Dump());
    return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRuleSet::CRuleSet()
{
    m_setRule	= RULE_SET::CreateNew(true);
}

CRuleSet::~CRuleSet()
{
}

bool CRuleSet::Create(CAiCenter* pOwner, IDatabase* pDb, int nNow)
{
    m_pOwner	= pOwner;
    m_nStartNow	= nNow;
    char	buf[256];
    sprintf(buf, "SELECT * FROM %s WHERE owner_id=%u || owner_id=0", _TBL_RULE, pOwner->QueryAgent()->GetID());
    IRecordset* pRes = pDb->CreateNewRecordset(buf, false);
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        AddRule(pRes->GetInt(RULEDATA_ID),
                pRes->GetStr(RULEDATA_MODE),
                pRes->GetStr(RULEDATA_CONDITION),
                pRes->GetStr(RULEDATA_RESULT),
                pRes->GetInt(RULEDATA_PRIORITY)	);
    }
    pRes->Release();
    return true;
}

void CRuleSet::AddRule(OBJID idRule, StringRef strMode, StringRef strCondition, StringRef strResult, int nPriority)
{
    CSymbolTable* pSymbolTable	= m_pOwner->QuerySymbolTable();
    CRuleObj* pRule = CRuleObj::CreateNew();
    IF_OK(pRule)
    {
        int	nDurable = m_pOwner->QueryAgent()->Priority2Durable(nPriority);
        IF_OK(pRule->Create(pSymbolTable, idRule, strMode, strCondition, strResult, nPriority, m_nStartNow, nDurable))
        {
            m_setRule->AddObj(pRule);
            return ;
        }
        else
        {
            pRule->ReleaseByOwner();
        }
    }
}
