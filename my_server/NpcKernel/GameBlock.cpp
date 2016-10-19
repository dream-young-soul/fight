
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// GameBlock.cpp: implementation of the CGameBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "GameBlock.h"
#include "GameMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameBlock::CGameBlock()
{
    m_pUserSet		= NULL;
    m_pNpcSet		= NULL;
    m_bDormancy		= true;
}

//////////////////////////////////////////////////////////////////////
CGameBlock::~CGameBlock()
{
    if (m_pUserSet)
    {
        m_pUserSet->Release();
    }
    if (m_pNpcSet)
    {
        m_pNpcSet->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CGameBlock::Create()
{
    if (m_pUserSet)
    {
        return true;
    }
    m_pUserSet		= CUserSet::CreateNew(false);
    m_pNpcSet		= CNpcSet::CreateNew(false);
    if (!m_pUserSet || !m_pNpcSet)
    {
        return false;
    }
    return true;
}

