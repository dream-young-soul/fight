
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FactSet.cpp: implementation of the CFactSet class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "Fact.h"
#include "SymbolTable.h"
#include "FactSet.h"
#include "i_mydb.h"
#include "Syntax.h"
#include "ruleset.h"
#include "AiCenter.h"

MYHEAP_IMPLEMENTATION(CFactObj, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool CFactObj::Create(CSymbolTable* pSymbolTable, OBJID id, OBJID idRecord, StringRef strFact, int nPriority, DWORD dwDateTime, int nDurable)
{
    m_id				= id;
    m_idRecord			= idRecord;
    m_bDirty			= false;
    if (!::AnalyzeFact(pSymbolTable, strFact, &m_cFact))
    {
        return false;
    }
    m_nPriority			= nPriority;
    m_tDurable.Startup(dwDateTime, nDurable);
    return true;
}

bool CFactObj::Create(CSymbolTable* pSymbolTable, OBJID id, StringRef strFact, int nPriority, int nNow, int nDurable, CFact* pModelFact)		// return pModelFact
{
    m_id				= id;
    m_idRecord			= ID_NONE;
    m_bDirty			= true;
    if (!::AnalyzeFact(pSymbolTable, strFact, &m_cFact, pModelFact))		// return pModelFact
    {
        return false;
    }
    m_nPriority			= nPriority;
    m_tDurable.Startup(nNow, nDurable);
    VarTypeSet::ConstIterator pParam = GetParamSet().NewEnum();
    while(pParam.Next())
    {
        IF_NOT(pParam->IsIntType() || pParam->IsStringType())
        return false;
    }
    return true;
}

bool CFactObj::Create(CSymbolTable* pSymbolTable, OBJID id, CFactRef cFact, int nPriority, int nNow, int nDurable)
{
    m_id				= id;
    m_idRecord			= ID_NONE;
    m_bDirty			= true;
    m_cFact				= cFact;
    m_nPriority			= nPriority;
    m_tDurable.Startup(nNow, nDurable);
    if (!cFact.IsLogicNot())
    {
        VarTypeSet::ConstIterator pParam = GetParamSet().NewEnum();
        while(pParam.Next())
        {
            IF_NOT(pParam->IsIntType() || pParam->IsStringType())
            return false;
        }
    }
    return true;
}

bool CFactObj::IsValid()
{
    VarTypeSet::ConstIterator pParam = GetParamSet().NewEnum();
    while(pParam.Next())
    {
        IF_NOT(pParam->IsIntType() || pParam->IsStringType())
        return false;
    }
    return true;
}

bool CFactObj::IsLikeModel(CFactRef cPattern)
{
    if (cPattern.GetType() != this->GetType())
    {
        return false;
    }
    if (cPattern.GetParamSet().Size() != this->GetParamSet().Size())
    {
        return false;
    }
    for(int i = 0; i < this->GetParamSet().Size(); i++)
    {
        VarTypeRef obj = cPattern.GetParamSet().At(i);
        if (obj.IsValid())
        {
            if (obj != this->GetParamSet().At(i))
            {
                return false;
            }
        }
    }
    return true;
}

void CFactObj::GetFactString(String& strFact, CSymbolTable* pSymbolTable)
{
    // logic not
    if (IsLogicNot())
    {
        strFact.QueryString() += LOGIC_NOT_CHAR;
    }
    // type
    LPCTSTR ptr = pSymbolTable->FindSymbolByID(GetType());
    IF_OK(ptr)
    strFact.QueryString() += ptr;
    // <
    strFact.QueryString() += KEY_PARAM_CHAR;
    // param set
    bool bSeparator = false;
    VarTypeSet::ConstIterator pParam = GetParamSet().NewEnum();
    while(pParam.Next())
    {
        // ,
        if (bSeparator)
        {
            strFact.QueryString() += SEPARATOR_CHAR;
        }
        bSeparator	= true;
        IF_OK(pParam->IsValid())
        {
            if (!pParam->IsValid())
            {
                strFact	+= "(null)";
            }
            else if (pParam->IsIntType())
            {
                strFact	+= String().FormatLong(static_cast<int>(*pParam));
            }
            else
            {
                strFact	+= static_cast<String>(*pParam);
            }
        }
    }
    // >
    strFact.QueryString() += KEY_PARAM_CHAR2;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool CFactSet::AddFactQueue(CFactRef cFact, int nPriority, int nNow, int nDurable)
{
    if (!cFact.IsLogicNot())
    {
        CFactObj* pFact = CFactObj::CreateNew();
        IF_OK(pFact)
        {
            IF_OK(pFact->Create(QuerySymbolTable(), SpawnFactID(), cFact, nPriority, nNow, nDurable))
            {
                CHECKF(pFact->GetID() >= FACT_ID_FIRST);
                m_setFact->AddObj(pFact);
                LOGDUMP("add fact: %s", (LPCTSTR)::DumpString(*pFact));
                return true;
            }
            else
            {
                pFact->Release();
            }
        }
    }
    else
    {
        ClearAlikeFact(cFact);
        return true;
    }
    return false;
}

void CFactSet::CombineQueue()
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFactSet::CFactSet()
{
    m_setFact	= FACT_SET::CreateNew(true);
    m_idLast	= FACT_ID_FIRST;
}

CFactSet::~CFactSet()
{
}

bool CFactSet::Create(CAiCenter* pOwner, IDatabase* pDb, int nNow)
{
    m_pOwner	= pOwner;
    char	buf[256];
    sprintf(buf, "SELECT * FROM %s WHERE owner_id=%u", _TBL_FACT, pOwner->QueryAgent()->GetID());
    IRecordset* pRes = pDb->CreateNewRecordset(buf, false);
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        if (nNow < pRes->GetInt(FACTDATA_DATETIME) + pRes->GetInt(FACTDATA_DURABLE))
        {
            AddFact(pRes->GetInt(FACTDATA_ID_),
                    pRes->GetStr(FACTDATA_FACT),
                    pRes->GetInt(FACTDATA_PRIORITY),
                    pRes->GetInt(FACTDATA_DATETIME),
                    pRes->GetInt(FACTDATA_DURABLE) );
            LOGDUMP("Load All Facts: %s", pRes->GetStr(FACTDATA_FACT));
        }
    }
    pRes->Release();
    return true;
}

bool CFactSet::Persistent(IDatabase* pDb, int nNow)
{
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_FACT);
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
    FACT_SET::Iterator pFact = m_setFact->NewEnum();
    while(pFact.Next())
    {
        if (pFact && pFact->IsDirty() && pFact->IsAlive(nNow))
        {
            String strFact;
            pFact->GetFactString(strFact, QuerySymbolTable());
            pRes->SetStr(FACTDATA_FACT,		strFact, MAX_FIELDSIZE);
            pRes->SetInt(FACTDATA_PRIORITY,	pFact->GetPriority());
            pRes->SetInt(FACTDATA_DATETIME,	pFact->GetDateTime());
            if (pFact->GetRecordID() != ID_NONE)
            {
                pRes->SetInt(FACTDATA_ID_,		pFact->GetRecordID());
                pRes->UpdateRecord();
            }
            else
            {
                OBJID id = pRes->InsertRecord();
                pFact->SetRecordID(id);
            }
        }
    }
    pRes->Release();
    return true;
}

bool CFactSet::AddFact(OBJID idRecord, StringRef strFact, int nPriority, DWORD dwDateTime, int nDurable)
{
    CFactObj* pFact = CFactObj::CreateNew();
    IF_OK(pFact)
    {
        IF_OK(pFact->Create(QuerySymbolTable(), SpawnFactID(), idRecord, strFact, nPriority, dwDateTime, nDurable))
        {
            CHECKF(!pFact->IsLogicNot());
            m_setFact->AddObj(pFact);
#ifdef	DUMP_DEBUG_MSG
            String	str;
            pFact->GetFactString(str, QuerySymbolTable());
            LOGDUMP("Add Fact: %s,%s,%s,%s", (LPCTSTR)str, (LPCTSTR)::DumpString(nPriority), (LPCTSTR)::DumpString(dwDateTime), (LPCTSTR)::DumpString(nDurable));
#endif
            return true;
        }
        else
        {
            pFact->ReleaseByOwner();
        }
    }
    return false;
}

bool CFactSet::AddFact(StringRef strFact, int nPriority, int nNow, int nDurable)
{
    CFactObj* pFact = CFactObj::CreateNew();
    IF_OK(pFact)
    {
        CFact cModelFact;
        IF_OK(pFact->Create(QuerySymbolTable(), SpawnFactID(), strFact, nPriority, nNow, nDurable, &cModelFact))		// return faceModel
        {
            LOGDUMP("add fact: %s - %s", (const char*)strFact, (LPCTSTR)::DumpString(cModelFact));
            ClearAlikeFact(cModelFact);
            if (!pFact->IsLogicNot())
            {
                m_setFact->AddObj(pFact);
#ifdef	DUMP_DEBUG_MSG
                String	str;
                pFact->GetFactString(str, QuerySymbolTable());
                //				LOGDUMP("Add Fact: %s,%s,%s,%s:", (LPCTSTR)str, (LPCTSTR)::DumpString(nPriority), (LPCTSTR)::DumpString(nNow), (LPCTSTR)::DumpString(nDurable));
#endif
            }
            return true;
        }
        else
        {
            pFact->ReleaseByOwner();
        }
    }
    return false;
}
/*
bool CFactSet::AddFact(CFactRef cFact, int nPriority, int nNow, int nDurable)
{
	CFactObj* pFact = CFactObj::CreateNew();
	IF_OK(pFact)
	{
		CFact cModelFact;
		IF_OK(pFact->Create(QuerySymbolTable(), SpawnFactID(), cFact, nPriority, nNow, nDurable))
		{
			if (!pFact->IsLogicNot())
			{
				m_setFact->AddObj(pFact);
#ifdef	DUMP_DEBUG_MSG
				String	str;
				pFact->GetFactString(str, QuerySymbolTable());
				LOGDUMP("Add Fact: %s,%s,%s,%s+", (LPCTSTR)str, (LPCTSTR)::DumpString(nPriority), (LPCTSTR)::DumpString(nNow), (LPCTSTR)::DumpString(nDurable));
#endif
			}
			return true;
		}
		else
		{
			pFact->ReleaseByOwner();
		}
	}

	return false;
}
*/
void CFactSet::ClearAlikeFact(CFactRef cModelFact)
{
    FACT_SET::Iterator pFact = m_setFact->NewEnum();
    while(pFact.Next())
    {
        if (pFact->IsLikeModel(cModelFact))
        {
            LOGDUMP("Del alike fact: %s", (const char*)pFact->Dump());
            m_setFact->DelObj(pFact);
        }
    }
}

CSymbolTable* CFactSet::QuerySymbolTable()				{ return m_pOwner->QuerySymbolTable(); }
