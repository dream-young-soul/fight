
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcManager.cpp: implementation of the CNpcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "NpcWorld.h"
#include "NpcGenerator.h"
#include "NpcType.h"
#include "Agent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcManager::CNpcManager()
{
    m_pNpcSet		= CNpcBigSet::CreateNew(true);
    m_pNpcGenSet	= CNpcGenSet::CreateNew(true);
    m_pNpcTypeSet	= CNpcTypeSet::CreateNew(true);
    m_idLastNewNpc	= MONSTERID_FIRST - 1;
    m_idxCurrGen	= 0;
    m_nStep			= 1;						// 刚开始时，每个生成器出一个
}

//////////////////////////////////////////////////////////////////////
CNpcManager::~CNpcManager()
{
    if (m_pNpcSet)
    {
        m_pNpcSet->Release();
    }
    if (m_pNpcGenSet)
    {
        m_pNpcGenSet->Release();
    }
    if (m_pNpcTypeSet)
    {
        m_pNpcTypeSet->Release();
    }
    //	CNpcGenData::Final();
    //	CNpcTypeData::Final();
}

//////////////////////////////////////////////////////////////////////
bool CNpcManager::Create()
{
    CHECKF(m_pNpcSet);
    CHECKF(m_pNpcGenSet);
    CHECKF(m_pNpcTypeSet);
    // generator
    //	if (!CNpcGenData::Init(NpcWorld()->GetDatabase()))
    //		return false;
    char szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s", _TBL_GENERATOR);
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);		// VVVVVVVVVVV
    if (!pRes)
    {
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        CGenPtr pGen = CNpcGenerator::CreateNew();		// VVVVVVVVV
        if (!pGen)
        {
            pRes->Release();
            return false;			// AAAAAAAAAA
        }
        if (!pGen->Create(pRes))
        {
            pGen->ReleaseByOwner();
#ifdef	PALED_DEBUG
            continue;
#endif
            pRes->Release();
            return false;			// AAAAAAAAAA
        }
        QueryGenSet()->AddObj(pGen);
    }
    pRes->Release();
    // npctype
    //	if (!CNpcTypeData::Init(NpcWorld()->GetDatabase()))
    //		return false;
    sprintf(szSQL, "SELECT * FROM %s", _TBL_MONSTERTYPE);
    pRes = Database()->CreateNewRecordset(szSQL, false);		// VVVVVVVVVVV
    if (!pRes)
    {
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        CNpcTypePtr pType = CNpcType::CreateNew();		// VVVVVVVVV
        if (!pType)
        {
            pRes->Release();
            return false;			// AAAAAAAAAA
        }
        if (!pType->Create(pRes))
        {
            pRes->Release();
            pType->ReleaseByOwner();
            return false;			// AAAAAAAAAA
        }
        QueryNpcTypeSet()->AddObj(pType);
    }
    pRes->Release();
    sprintf(szSQL, "SELECT id,time_of_life FROM %s WHERE account_id=0", _TBL_USER);
    pRes = Database()->CreateNewRecordset(szSQL, false);		// VVVVVVVVVVV
    if (!pRes)
    {
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        CAgent* pAgent = CAgent::CreateNew();		// VVVVVVVVV
        if (!pAgent)
        {
            pRes->Release();
            return false;			// AAAAAAAAAA
        }
        if (!pAgent->Create(pRes->LoadDWord("id"), pRes->LoadDWord("time_of_life")))
        {
            pRes->Release();
            pAgent->ReleaseByOwner();
            return false;			// AAAAAAAAAA
        }
        m_setAgentGen.push(pAgent);
    }
    pRes->Release();
    return true;			// AAAAAAAAAA
}

//////////////////////////////////////////////////////////////////////
OBJID CNpcManager::SpawnNewNpcID()
{
    if (m_idLastNewNpc < MONSTERID_LAST)
    {
        return ++m_idLastNewNpc;
    }
    else if (m_setRecycle.size())
    {
        return m_setRecycle.pop();
    }
    else
    {
        m_idLastNewNpc = MONSTERID_FIRST;
        LOGERROR("NPC的ID空间不足，强行回卷! NPC再生系统出现故障!");
        return m_idLastNewNpc;
    }
}

//////////////////////////////////////////////////////////////////////
void CNpcManager::OnTimer(DWORD nCurr)
{
    // 动态再生NPC
    int	nMaxSpawn	= MAXNPC_PER_ONTIMER;		// 每次调用最多生成10个
    for(int i = 0; i < QueryGenSet()->GetAmount(); i++)
    {
        DEBUG_TRY	// VVVVVVVV
        int nSpawn = QueryGenSet()->GetObjByIndex(m_idxCurrGen)->Generator(::CutOverflow(nMaxSpawn, m_nStep));
        nMaxSpawn	-= nSpawn;
        if (nMaxSpawn <= 0)
        {
            break;
        }
        DEBUG_CATCH("CNpcManager::OnTimer(1)")		// AAAAAAAAAAA
        // next
        m_idxCurrGen++;
        if (m_idxCurrGen >= QueryGenSet()->GetAmount())
        {
            m_idxCurrGen = 0;
        }
    }
    // calc m_nStep
    if (nMaxSpawn <= 0)
    {
        m_nStep = ::CutTrail(1, m_nStep - 1);
    }
    else
    {
        ASSERT(m_nStep != 0);
        int nActiveGen = (MAXNPC_PER_ONTIMER - nMaxSpawn) / m_nStep;
        if (nActiveGen)
        {
            m_nStep = ::CutRange(MAXNPC_PER_ONTIMER / nActiveGen, 1, MAXNPC_PER_ONTIMER);
        }
        else
        {
            m_nStep = MAXNPC_PER_ONTIMER;
        }
    }
    // 每个NPC的行为
    for(CNpcBigSet::Iter_t iter = QuerySet()->Begin(); iter != QuerySet()->End(); iter++)
    {
        DEBUG_TRY	// VVVVVVVV
        CNpcPtr pNpc = QuerySet()->GetObjByIter(iter);
        CNpc* pMonster;
        if (pNpc && !(Cast(&pMonster, pNpc) && pMonster->IsDormancy()))
        {
            pNpc->OnTimer();
        }
        DEBUG_CATCH("CNpcManager::OnTimer(1)")		// AAAAAAAAAAA
    }
    // AGENT
    if (!m_tAgentDelay.IsActive())
    {
        m_tAgentDelay.Startup(AGENT_GEN_DELAY_SECS);
    }
    else if (m_tAgentDelay.IsTimeOut(AGENT_GEN_DELAY_SECS))
    {
        if (m_setAgentGen.size())
        {
            CAgent* pAgent = m_setAgentGen.pop();
            IF_OK(pAgent)
            {
                IF_OK(pAgent->Login())
                m_pNpcSet->AddObj(Cast<INpc>(pAgent));
                else
                {
                    pAgent->ReleaseByOwner();
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
int CNpcManager::CountActiveNpc()
{
    int nCount = 0;
    for(CNpcBigSet::Iter_t iter = QuerySet()->Begin(); iter != QuerySet()->End(); iter++)
    {
        CNpcPtr pNpc = QuerySet()->GetObjByIter(iter);
        CNpc* pMonster;
        if (pNpc && !(Cast(&pMonster, pNpc) && pMonster->IsDormancy()))
        {
            nCount++;
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
CAgent* CNpcManager::QueryAgent(LPCTSTR szName)
{
    for(CNpcBigSet::Iter_t iter = QuerySet()->Begin(); iter != QuerySet()->End(); iter++)
    {
        CNpcPtr pNpc = QuerySet()->GetObjByIter(iter);
        CAgent* pAgent;
        if (pNpc && Cast(&pAgent, pNpc))
        {
            if (strcmp(pAgent->GetName(), szName) == 0)
            {
                return pAgent;
            }
        }
    }
    return NULL;
}
