
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcStorage.h: interface for the CNpcTrunk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCSTORAGE_H__09B6F357_449A_4DDC_95BD_7A73C05D8104__INCLUDED_)
#define AFX_NPCSTORAGE_H__09B6F357_449A_4DDC_95BD_7A73C05D8104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
#include "Myheap.h"
#include "Package.h"

class CPackage;
class CNpcTrunk
{
protected:
    CNpcTrunk();
    virtual ~CNpcTrunk();
public:
    static CNpcTrunk* CreateNew()			{ return new CNpcTrunk; }
    ULONG	Release()						{ delete this; return 0; }

public:
    bool	Create(PROCESS_ID, OBJID idRecordNpc, int nSize, int nPosition = ITEMPOSITION_TRUNK);
    CPackage*	QueryPackage(OBJID idPlayer = ID_NONE);	//				{ ASSERT(m_pPackage); return m_pPackage; }
    bool	IsPackageFull(OBJID idPlayer = ID_NONE)				{ return QueryPackage(idPlayer)->GetAmount() >= m_nSize; }
    bool	IsEmpty(OBJID idPlayer = ID_NONE)						{ return QueryPackage(idPlayer)->GetAmount() == 0; }

protected:
    CPackage*	m_pPackage;
    int			m_nSize;
    int			m_nPosition;
    OBJID		m_idRecordNpc;

private: // ctrl
    PROCESS_ID	m_idProcess;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_NPCSTORAGE_H__09B6F357_449A_4DDC_95BD_7A73C05D8104__INCLUDED_)
