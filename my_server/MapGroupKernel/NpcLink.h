
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NpcLink.h: interface for the CNpcLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCLINK_H__65ABD347_A49B_4CDB_B369_49BF7785A599__INCLUDED_)
#define AFX_NPCLINK_H__65ABD347_A49B_4CDB_B369_49BF7785A599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNpcLink
{
protected:
    CNpcLink();
    virtual ~CNpcLink();
public:
    static CNpcLink* CreateNew(PROCESS_ID idProcess, CNpc* pOwner);
    void	Release()						{ delete this; }

public: // application
    void	SetLinkMap(OBJID idMap)			{ m_pOwner->SetInt(NPCDATA_LINKID, idMap); }
    CGameMap* GetLinkMap();				// return null: data error
    bool	EraseMap();

protected:
    PROCESS_ID	m_idProcess;
    CNpc*		m_pOwner;
    CGameMap*	m_pLinkMap;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_NPCLINK_H__65ABD347_A49B_4CDB_B369_49BF7785A599__INCLUDED_)
