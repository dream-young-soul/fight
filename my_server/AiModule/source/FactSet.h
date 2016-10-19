
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FactSet.h: interface for the CFactSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACTSET_H__C444973E_E282_4312_81CF_A23CFDA0660B__INCLUDED_)
#define AFX_FACTSET_H__C444973E_E282_4312_81CF_A23CFDA0660B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycom.h"
#include "T_SingleMap.h"
#include "GameObjSet.h"
#include "VarType.h"
#include "I_AiCenter.h"	// Added by ClassView
#include "TickOver.h"
#include "autoptr.h"
#include "myheap.h"
#include "basetype.h"
#include "Fact.h"
#include "t_myqueue.h"

//////////////////////////////////////////////////////////////////////
enum FACTDATA
{
    FACTDATA_ID_,
    FACTDATA_OWNERID,
    FACTDATA_FACT,
    FACTDATA_PRIORITY,
    FACTDATA_DATETIME,
    FACTDATA_DURABLE,
};

//////////////////////////////////////////////////////////////////////
class IDatabase;
class CSymbolTable;
class CFactObj
{
    COM_STYLE(CFactObj)
public:
    CFactObj()	{}
    MYHEAP_DECLARATION(s_heap)
public:
    bool		Create(CSymbolTable* pSymbolTable, OBJID id, OBJID idRecord, StringRef strFact, int nPriority, DWORD dwDateTime, int nDurable);
    bool		Create(CSymbolTable* pSymbolTable, OBJID id, StringRef strFact, int nPriority, int nNow, int nDurable, CFact* pModelFact);		// return pModelFact
    bool		Create(CSymbolTable* pSymbolTable, OBJID id, CFactRef cFact, int nPriority, int nNow, int nDurable);

public: // interface
    OBJID		GetID()				{ return m_id; }

public: // const
    OBJID		GetRecordID()		{ return m_idRecord; }
    OBJID		GetType()			const	{ return m_cFact.GetType(); }
    VarTypeSetRef	GetParamSet()	const	{ return m_cFact.GetParamSet(); }
    int			GetPriority()		{ return m_nPriority; }
    DWORD		GetDateTime()		{ return m_tDurable.GetBegin(); }

    void		GetFactString(String& strFact, CSymbolTable* pSymbolTable);
    bool		IsLikeModel(CFactRef cModelFact);
    bool		IsDirty()			{ return m_bDirty; }
    bool		IsAlive(int nNow)	{ return m_tDurable.GetDurable(nNow) > 0; }
    bool		IsLogicNot()		{ return m_cFact.IsLogicNot(); }
    CFactRef	QueryFact()			{ return m_cFact; }
    bool		IsValid();

public: // modify
    void		SetRecordID(OBJID id)		{ ASSERT(m_idRecord == ID_NONE); m_idRecord = id; }

public: // dump
    String		Dump()	const
    {
        String	str("CFactObj:");
        str += ::DumpString(m_cFact);
        str	+= " life:";
        str += String().FormatLong(m_tDurable.GetEnd());
        return str;
    }


protected:
    OBJID		m_id;				// serial
    OBJID		m_idRecord;
    bool		m_bDirty;

    //	bool		m_bLogicNot;
    //	OBJID		m_idType;
    //	VarTypeSet	m_setParam;
    CFact		m_cFact;

    int			m_nPriority;
    CTickOver	m_tDurable;
};
template<>
struct dump_traits<CFactObj> { static String Dump(const CFactObj& obj)		{ return obj.Dump(); } };


//////////////////////////////////////////////////////////////////////
class IDatabase;
class CAiCenter;
class CFactSet
{
    COM_STYLE(CFactSet)
protected:
    CFactSet();
    /*virtual*/
    ~CFactSet();

public: // construct
    bool	Create(CAiCenter* pOwner, IDatabase* pDb, int nNow);
    bool	Persistent(IDatabase* pDb, int nNow);

public:
    //	bool	AddFact(CFactRef cFact, int nPriority, int nNow, int nDurable);
    bool	AddFact(StringRef strFact, int nPriority, int nNow, int nDurable);
protected:
    void	ClearAlikeFact(CFactRef cModelFact);
    bool	AddFact(OBJID idRecord, StringRef strFact, int nPriority, DWORD dwDateTime, int nDurable);

public: // queue
    bool	AddFactQueue(CFactRef cFact, int nPriority, int nNow, int nDurable);
    void	CombineQueue();

public: // const
    CSymbolTable*	QuerySymbolTable();

public: // traversal
    typedef		CSingleMap<CFactObj>	FACT_SET;
    typedef		FACT_SET::Iterator		Iterator;
    Iterator	NewEnum		() 						{ return m_setFact->NewEnum(); }
    int			Size		() const				{ return m_setFact->Size(); }
    void		DelObj		(Iterator& iter)		{ m_setFact->DelObj(iter); }
protected:
    CAiCenter*		m_pOwner;
    CAutoPtr<FACT_SET>		m_setFact;

protected: // add fact queue
    //	typedef		CMyQueue<CFactObj*>	FACT_QUEUE;
    //	FACT_QUEUE	m_setFactQueue;

public:
    OBJID		SpawnFactID()						{ ++m_idLast; ASSERT(m_idLast != ID_NONE); return m_idLast; }

private: // id generator
    OBJID	m_idLast;
};

#endif // !defined(AFX_FACTSET_H__C444973E_E282_4312_81CF_A23CFDA0660B__INCLUDED_)
