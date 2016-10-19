
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// GameBlock.cpp: implementation of the CGameBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "GameBlock.h"

//MYHEAP_IMPLEMENTATION(CGameBlock,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameBlock::CGameBlock()
{
    m_pSet = NULL;
}

//////////////////////////////////////////////////////////////////////
CGameBlock::~CGameBlock()
{
    if (m_pSet)
    {
        m_pSet->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameBlock::Create()
{
    if (m_pSet)
    {
        return true;
    }
    m_pSet			= CThingSet::CreateNew(false);
    CHECKF(m_pSet);
    m_bDormancy		= true;
    return true;
}

