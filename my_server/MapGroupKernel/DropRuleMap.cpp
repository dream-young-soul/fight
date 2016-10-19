
//**********************************************************
// 代码编辑器
//**********************************************************

// DropRuleSet.cpp: implementation of the CDropRuleMap class.
//
//////////////////////////////////////////////////////////////////////


#include "DropRuleMap.h"
#include "AutoPtr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CDropRuleMap, s_heap)

CDropRuleMap::CDropRuleMap()
{
}

CDropRuleMap::~CDropRuleMap()
{
    Destroy();
}

//////////////////////////////////////////////////////////////////////

bool	CDropRuleMap::Create(IDatabase*	pDb)
{
    CHECKF(pDb);
    char szSQL[1024];
    sprintf(szSQL, "SELECT DISTINCT id FROM %s ORDER BY id", _TBL_DROPITEMRULE);
    CAutoPtr<IRecordset> pRes = pDb->CreateNewRecordset(szSQL, false);
    if (pRes)
    {
        CDropRuleGroup*	pRuleGroup = NULL;
        OBJID	idRuleGroup;
        DEBUG_TRY
        for (int i = 0; i < pRes->RecordCount(); i++)
        {
            idRuleGroup = pRes->LoadDWord("id");
            pRuleGroup = CDropRuleGroup::CreateNew();
            if (!pRuleGroup)
            {
                return false;
            }
            if (!pRuleGroup->Create(idRuleGroup, pDb))
            {
                pRuleGroup->Release();
                return false;
            }
            m_map[idRuleGroup] = pRuleGroup;
            pRes->MoveNext();
        }
        //pRes->Release();
        return true;
        DEBUG_CATCH("CDropRuleMap::Create()")
    }
    return true;	//?? pRes为空应该返回false
}
//////////////////////////////////////////////////////////////////////

void	CDropRuleMap::Destroy()
{
    DEBUG_TRY
    for (Iter_t it = m_map.begin(); it != m_map.end(); it++)
    {
        CDropRuleGroup*	pRule = it->second;
        if (pRule)
        {
            pRule->Release();
        }
    }
    DEBUG_CATCH("CDropRuleMap::Destroy()")
    m_map.clear();
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
