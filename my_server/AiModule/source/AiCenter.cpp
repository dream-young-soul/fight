
//**********************************************************
// 代码编辑器
//**********************************************************

// AiCenter.cpp: implementation of the CAiCenter class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "Fact.h"
#include "SymbolTable.h"
#include "VarType.h"
#include "RuleSet.h"
#include "FactSet.h"
#include "AiCenter.h"
#include "Syntax.h"

//////////////////////////////////////////////////////////////////////
bool IsAllWildParam(VarTypeSetRef setParam)
{
    VarTypeSet::ConstIterator pParam = setParam.NewEnum();
    while(pParam.Next())
    {
        if (!( pParam->IsSymbolType() && strcmp(static_cast<String>(*pParam), ANY_PARAM_STRING) == 0 ))
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CAiCenter::CallFunction(OBJID idFact, VarTypeSetRef setParam, ARGUMENT_SET* psetArgument)
{
    switch(idFact - SYSTEM_FUNCTION_ID_BEGIN)
    {
    case	SYS_FUNC_DUMP:		// "dump",					// for debug, dump this rule
        {
            //CHECKF(setParam.Size() == 1);
            m_bDumpRule = true;
            return true;
        }
        break;
    case	SYS_FUNC_KEEP:		// "keep",					// (int), only for change mode, keep(0) for keep fact forever
        {
            CHECKF(setParam.Size() == 1);
            m_nNewPriority = GetValue(setParam[0], psetArgument);
            return true;
        }
        break;
    case	SYS_FUNC_NOW:		// "now",					// (int), 当前时间
        {
            CHECKF(setParam.Size() == 1);
            VarTypeSet	setFactParam;
            setFactParam.Push(Now());
            return MatchParam(setFactParam, setParam, psetArgument);
        }
        break;
    case	SYS_FUNC_LESS:		// "less",					// (left,right), 如果　left < right　则正确
        {
            CHECKF(setParam.Size() == 2);
            int left	= GetValue(setParam[0], psetArgument);
            int right	= GetValue(setParam[1], psetArgument);
            if (left < right)
            {
                return true;
            }
        }
        break;
    case	SYS_FUNC_LESS_EQU:	// "less_equ",				// (left,right), 如果　left <= right　则正确
        {
            CHECKF(setParam.Size() == 2);
            int left	= GetValue(setParam[0], psetArgument);
            int right	= GetValue(setParam[1], psetArgument);
            if (left <= right)
            {
                return true;
            }
        }
        break;
    case	SYS_FUNC_STRSTR:		// "strstr",				// (str,substr), 如果str中包含有子串substr，正确
        {
            CHECKF(setParam.Size() == 2);
            String left		= GetValue(setParam[0], psetArgument);
            String right	= GetValue(setParam[1], psetArgument);
            if (::StrStrCh(left, right))
            {
                return true;
            }
        }
        break;
    case	SYS_FUNC_PERCENT:	// "percent",				// (int), 0 ~ 100, 有n%的机会为TRUE。
        {
            CHECKF(setParam.Size() == 1);
            int nPercent	= GetValue(setParam[0], psetArgument);
            if (::RandGet(100) < nPercent)
            {
                return true;
            }
        }
        break;
    default:
        ASSERT(!"CallFunction");
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
IAiCenter* IAiCenter::CreateNew()
{
    return CAiCenter::CreateNew();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAiCenter::CAiCenter()
{
}

CAiCenter::~CAiCenter()
{
}

bool CAiCenter::Create(IAiCenterOwner* pOwner, IDatabase* pDb, const Array<String>& setGameFunction, const Array<String>& setGameAction, int nTimeOfLife)
{
    CHECKF(pOwner && pDb);
    m_pOwner	= pOwner;
    m_nNow		= nTimeOfLife;
    CSymbolTable* pTable = QuerySymbolTable();
    CHECKF(pTable && pTable->Create(SYSTEM_FACT_ID_BEGIN));
    // sys func
    OBJID idCurr = SYSTEM_FUNCTION_ID_BEGIN;
    int i = 0;
    while(SystemFunction(i))
    {
        pTable->AddSymbol(SystemFunction(i), idCurr++);
        i++;
    }
    // game function
    idCurr = GAME_FUNCTION_ID_BEGIN;
    Array<String>::ConstIterator pStr = setGameFunction.NewEnum();
    while(pStr.Next())
    {
        pTable->AddSymbol(*pStr, idCurr++);
    }
    // game action
    idCurr = GAME_ACTION_ID_BEGIN;
    pStr = setGameAction.NewEnum();
    while(pStr.Next())
    {
        pTable->AddSymbol(*pStr, idCurr++);
    }
    if (!m_pFactSet->Create(this, pDb, nTimeOfLife))
    {
        return false;
    }
    if (!m_pRuleSet->Create(this, pDb, m_nNow))
    {
        return false;
    }
    return true;
}

void CAiCenter::Thinking(int nStep)
{
    CRuleSet::Iterator pRule = m_pRuleSet->NewEnum();
    while(pRule.Next())
    {
        if (pRule->ToNextTime(Now()))
        {
            DEBUG_TRY /////////////////////
            //			LOGDUMP("Process Rule %u: ", pRule->GetID());
            ProcessRule(pRule);
            DEBUG_CATCH2("Process Rule %u", pRule->GetID()) ///////////////////////
        }
        //		if (--nStep <= 0)
        //			return;
    }
}

void CAiCenter::OnTimer()
{
    DEBUG_TRY /////////////////////
    Thinking(10000);
    // clear
    m_nNow++;
    ClearInvalidRule();
    DEBUG_CATCH("CAiCenter::OnTimer()") ///////////////////////
}

void CAiCenter::AddFact(StringRef strFact, int nPriority)
{
    m_pFactSet->AddFact(strFact, nPriority, Now(), m_pOwner->Priority2Durable(nPriority));
}

bool CAiCenter::ProcessRule(const CRuleObj* pRule)
{
    CFactRef	cMode = pRule->GetMode();
    m_bDumpRule		= false;
    m_nNewPriority	= PRIORITY_NONE;
    ARGUMENT_SET	setArgument;
    if (!ProcessRuleNest(cMode.IsLogicNot(), cMode.GetType(), cMode.GetParamSet(), setArgument, pRule, pRule->GetCondition().NewEnum()))
    {
        m_pFactSet->CombineQueue();
        if (m_bDumpRule)
        {
            LOGDUMP("--- failed ---");
        }
        return false;
    }
    m_pFactSet->CombineQueue();
    LOGDUMP("process rule ok: %u", pRule->GetID());
    return true;
}

bool CAiCenter::ProcessRuleNest(bool bLogicNot, OBJID idFact, VarTypeSetRef setParam, ARGUMENT_SET& setArgument, const CRuleObj* pRule, CFactArray::ConstIterator iterFact)
{
#ifdef	DUMP_DEBUG_MSG
    String strFact;
    strFact += bLogicNot ? String("!") : String();
    strFact += m_pFactSet->QuerySymbolTable()->FindSymbolByID(idFact);
    strFact += setParam.Dump();
    String strDumpArgu = setArgument.Dump();
    String strDumpRule = pRule->Dump();
#endif
    if (m_bDumpRule)
    {
        LOGDUMP("*** system function: %c %s,%s - %s", bLogicNot ? '!' : ' ', m_pSymbolTable->FindSymbolByID(idFact), (LPCTSTR)::DumpString(setParam), (LPCTSTR)setArgument.Dump());
    }
    // check function implement by ai module
    if (IsFunction(idFact))
    {
        if ((bLogicNot == 0) == (CallFunction(idFact, setParam, &setArgument) != 0))
        {
            if (iterFact.Next())
            {
                return ProcessRuleNest(iterFact->IsLogicNot(), iterFact->GetType(), iterFact->GetParamSet(), setArgument, pRule, iterFact);    // nest call self
            }
            else
            {
                return ProcessResult(pRule->GetResult(), pRule->GetPattern(), pRule->GetPriority(), setArgument);
            }
        }
        return false;
    }
    // check fact function implement by game kernel
    if (IsFactFunction(idFact))
    {
        int		idxFactFunction	= idFact - GAME_FUNCTION_ID_BEGIN;
        if (m_pOwner->CheckCondition(bLogicNot, idxFactFunction, setParam, &setArgument))
        {
            if (iterFact.Next())
            {
                return ProcessRuleNest(iterFact->IsLogicNot(), iterFact->GetType(), iterFact->GetParamSet(), setArgument, pRule, iterFact);    // nest call self
            }
            else
            {
                return ProcessResult(pRule->GetResult(), pRule->GetPattern(), pRule->GetPriority(), setArgument);
            }
        }
        return false;
    }
    // collect facts
    bool bWildParam = IsAllWildParam(setParam);
    CFactArray	setFacts;
    CFactSet::Iterator pFact = m_pFactSet->NewEnum();
    while(pFact.Next())
    {
        ARGUMENT_SET	setArg = setArgument;
        if (pFact->GetType() == idFact)
        {
            ASSERT(pFact->IsAlive(Now()));
#ifdef	DUMP_DEBUG_MSG
            String strSet1 = ::DumpString(pFact->GetParamSet());
            String strSet2 = ::DumpString(setParam);
#endif
            if (IAiCenter::MatchParam(pFact->GetParamSet(), setParam, &setArg))
            {
                if (!bLogicNot)
                {
                    setFacts.Push(pFact->QueryFact());
                    if (bWildParam)
                    {
                        break;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
    } // while all fact
    // check facts
    bool bNext = iterFact.Next();
    if (!bLogicNot && setFacts.Size())
    {
        bool ret = false;
        CFactArray::Iterator pFact = setFacts.NewEnum();
        while(pFact.Next())
        {
            ARGUMENT_SET	setArg = setArgument;
            ASSERT(IAiCenter::MatchParam(pFact->GetParamSet(), setParam, &setArg));
            if (bNext)
            {
                ret |= ProcessRuleNest(iterFact->IsLogicNot(), iterFact->GetType(), iterFact->GetParamSet(), setArg, pRule, iterFact);    // nest call self
            }
            else
            {
                ret |= ProcessResult(pRule->GetResult(), pRule->GetPattern(), pRule->GetPriority(), setArg);
            }
            if (bWildParam)
            {
                break;
            }
        }
        return ret;
    }
    else if (bLogicNot && setFacts.Size() == 0)
    {
        if (bNext)
        {
            return ProcessRuleNest(iterFact->IsLogicNot(), iterFact->GetType(), iterFact->GetParamSet(), setArgument, pRule, iterFact);    // nest call self
        }
        else
        {
            return ProcessResult(pRule->GetResult(), pRule->GetPattern(), pRule->GetPriority(), setArgument);
        }
    }
    return false;
}

VarTypeRef IAiCenter::GetValue(VarTypeRef cParam, ARGUMENT_SET* psetArgument)
{
    if (cParam.IsSymbolType())
    {
        IF_OK(psetArgument->IsIn(cParam))
        {
            return (*psetArgument)[cParam];
        }
    }
    ASSERT(!cParam.IsSymbolType());
    return cParam;
}

bool IAiCenter::MatchParam(VarTypeSetRef setFactParam, VarTypeSetRef setRuleParam, ARGUMENT_SET* psetArgument)
{
    // check fact param set
    VarTypeSet::ConstIterator pFactParam	= setFactParam.NewEnum();
    VarTypeSet::ConstIterator pRuleArg		= setRuleParam.NewEnum();
    while(pFactParam.Next())
    {
        CHECKF(pRuleArg.Next());
        if (*pFactParam == *pRuleArg)
        {
            continue;
        }
        else if (!pRuleArg->IsSymbolType())
        {
            return false;
        }
        else // if (pRuleArg->IsSymbolType())
        {
            if (static_cast<String>(*pRuleArg) == static_cast<String>(ANY_PARAM_STRING))
            {
                continue;
            }
            else if (!psetArgument->IsIn(*pRuleArg))
            {
                psetArgument->Add(static_cast<String>(*pRuleArg), *pFactParam);
            }
            else // if (psetArgument->IsIn(*pRuleArg))
            {
                if (*pFactParam != (*psetArgument)[*pRuleArg])
                {
                    return false;
                }
            }
        }
    } // while param list
    return true;
}

bool CAiCenter::ProcessResult(CFactRef cResult, CFactRef cPattern, int nPriority, ARGUMENT_SET_REF setArgument)
{
    if (m_bDumpRule)
    {
        LOGDUMP("=== success result: %s - %s", (LPCTSTR)::DumpString(cResult), (LPCTSTR)::DumpString(cPattern));
    }
    if (m_nNewPriority != PRIORITY_NONE)
    {
        nPriority	= m_nNewPriority;
        LOGDUMP("Change Mode: %s ===========================================", (LPCTSTR)::DumpString(cResult));
    }
    CHECKF(cPattern.IsLogicNot());
    CFact cNewPattern = cPattern;
    ::ReplaceSymbol(&cNewPattern, setArgument);
    m_pFactSet->AddFactQueue(cNewPattern, nPriority, Now(), m_pOwner->Priority2Durable(nPriority));
    // action
    if (IsAction(cNewPattern.GetType()))
    {
        ClearAlikeAction(cNewPattern);
    }
    if (!cResult.IsLogicNot())
    {
        CFact cNewFact	= cResult;
        ::ReplaceSymbol(&cNewFact, setArgument);
        m_pFactSet->AddFactQueue(cNewFact, nPriority, Now(), m_pOwner->Priority2Durable(nPriority));
        // action
        if (IsAction(cNewFact.GetType()))
        {
            m_setAction.Push(cNewFact);
            LOGDUMP("push action: %s", (const char*)cNewFact.Dump(), nPriority, Now());
        }
    }
    return true;
}

bool IsLikeModel(CFactRef cFact, CFactRef cPattern)
{
    if (cPattern.GetType() != cFact.GetType())
    {
        return false;
    }
    if (cPattern.GetParamSet().Size() != cFact.GetParamSet().Size())
    {
        return false;
    }
    for(int i = 0; i < cFact.GetParamSet().Size(); i++)
    {
        VarTypeRef obj = cPattern.GetParamSet().At(i);
        if (obj.IsValid())
        {
            if (obj != cFact.GetParamSet().At(i))
            {
                return false;
            }
        }
    }
    return true;
}

void CAiCenter::ClearAlikeAction(CFactRef cPattern)
{
    CFactArray::Iterator pFact = m_setAction.NewEnum();
    while(pFact.Next())
    {
        if (IsLikeModel(*pFact, cPattern))
        {
            LOGDUMP("Del Alike Action: %s", (const char*)pFact->Dump());
            m_setAction.DelObj(pFact);
        }
    }
}

bool CAiCenter::FetchAction(int* pidxAction, VarTypeSet* psetParam)
{
    CFactArray::Iterator pAction = m_setAction.NewEnum();
    if (pAction.Next())
    {
        *pidxAction	= pAction->GetType() - GAME_ACTION_ID_BEGIN;
        *psetParam	= pAction->GetParamSet();
        LOGDUMP("Agent FetchAction type: %u", pAction->GetType());
        m_setAction.DelObj(pAction);
        return true;
    }
    return false;
}

void CAiCenter::Persistent(IDatabase* pDb)						{ m_pFactSet->Persistent(pDb, Now()); }

void CAiCenter::ClearInvalidRule()
{
    CFactSet::Iterator pFact = m_pFactSet->NewEnum();
    while(pFact.Next())
    {
        if (!pFact->IsAlive(Now()))
        {
            LOGDUMP("Clear fact: %s, Now:%d", (LPCTSTR)(pFact->Dump()), Now());
            m_pFactSet->DelObj(pFact);
        }
    }
}
