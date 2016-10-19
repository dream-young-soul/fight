
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcManager.h: interface for the CNpcManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMANAGER_H__6F4AFF3F_CAF1_499D_A8AD_DAF034223CE6__INCLUDED_)
#define AFX_NPCMANAGER_H__6F4AFF3F_CAF1_499D_A8AD_DAF034223CE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "npc.h"
#include "GameObjSet.h"
#include "NpcGenerator.h"
#include "T_SingleMap.h"
#include "T_MyQueue.h"

//////////////////////////////////////////////////////////////////////
const int	MAXNPC_PER_ONTIMER			= 20;		// 每次ONTIMER调用生成几个

#ifdef	_DEBUG
const int	AGENT_GEN_DELAY_SECS		= 1;		//
#else
const int	AGENT_GEN_DELAY_SECS		= 10;		//
#endif

//////////////////////////////////////////////////////////////////////
typedef	IGameObjSet<INpc>	INpcSet;
typedef	CGameObjSet<INpc>	CNpcSet;
typedef	CSingleMap<INpc>	INpcBigSet;
typedef	CSingleMap<INpc>	CNpcBigSet;

typedef	IGameObjSet<CNpcGenerator>	INpcGenSet;
typedef	CGameObjSet<CNpcGenerator>	CNpcGenSet;

typedef	IGameObjSet<CNpcType>	INpcTypeSet;
typedef	CGameObjSet<CNpcType>	CNpcTypeSet;

struct NpcInfoStruct;
class CNpcManager
{
public:
    CNpcManager();
    virtual ~CNpcManager();
    bool			Create();
    virtual ULONG	Release()		{ delete this; return 0; }

    INpc*		QueryNpc(OBJID id)	{ return QuerySet()->GetObj(id); }
    CAgent*		QueryAgent(OBJID id)	{ INpc* pNpc = QuerySet()->GetObj(id); if (pNpc) { return pNpc->QueryInterface(TYPE_OF(CAgent)); } return NULL; }
    CAgent*		QueryAgent(LPCTSTR szName);
    INpcBigSet*	QuerySet()			{ CHECKF(m_pNpcSet); return m_pNpcSet; }
    INpcGenSet*	QueryGenSet()		{ CHECKF(m_pNpcGenSet); return m_pNpcGenSet; }
    INpcTypeSet*	QueryNpcTypeSet()		{ CHECKF(m_pNpcTypeSet); return m_pNpcTypeSet; }
    typedef	CMyQueue<OBJID>	NPCID_RECYCLE;
    NPCID_RECYCLE*	QueryRecycle()	{ return &m_setRecycle; }

public:
    void OnTimer(DWORD nCurr);
    int	CountActiveNpc();

public: // for generator
    OBJID	SpawnNewNpcID();

protected:
    INpcBigSet*	m_pNpcSet;
    INpcGenSet*	m_pNpcGenSet;
    INpcTypeSet*	m_pNpcTypeSet;
    CMyQueue<CAgent*>	m_setAgentGen;

private: // ctrl
    OBJID		m_idLastNewNpc;
    int			m_idxCurrGen;
    int			m_nStep;
    CMyQueue<OBJID>	m_setRecycle;
    CTimeOut	m_tAgentDelay;
};

#endif // !defined(AFX_NPCMANAGER_H__6F4AFF3F_CAF1_499D_A8AD_DAF034223CE6__INCLUDED_)
