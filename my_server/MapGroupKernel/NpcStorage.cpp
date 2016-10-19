
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcStorage.cpp: implementation of the CNpcTrunk class.
//
//////////////////////////////////////////////////////////////////////

#include "define.h"
#include "NpcStorage.h"
#include "MapGroup.h"
#include "Package.h"

MYHEAP_IMPLEMENTATION(CNpcTrunk, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcTrunk::CNpcTrunk()
{
    m_pPackage = NULL;
}

//////////////////////////////////////////////////////////////////////
CNpcTrunk::~CNpcTrunk()
{
    if (m_pPackage)
    {
        m_pPackage->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpcTrunk::Create(PROCESS_ID idProcess, OBJID idRecordNpc, int nSize, int nPosition /* = ITEMPOSITION_TRUNK*/)
{
    m_idProcess = idProcess;
    m_nSize		= nSize;
    m_nPosition	= nPosition;
    m_idRecordNpc	= idRecordNpc;
    m_pPackage	= CPackage::CreateNew();
    CHECKF(m_pPackage && m_pPackage->Create(nPosition, idRecordNpc, ID_NONE, Database()));
    return true;
}

//////////////////////////////////////////////////////////////////////
CPackage* CNpcTrunk::QueryPackage(OBJID idPlayer/*=ID_NONE*/)
{
    CHECKF(m_pPackage);
    IF_NOT (m_pPackage->Create(m_nPosition, m_idRecordNpc, idPlayer, Database()))
    return NULL;
    return m_pPackage;
}
