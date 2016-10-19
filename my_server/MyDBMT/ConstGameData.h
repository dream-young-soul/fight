
//**********************************************************
// 代码编辑器
//**********************************************************

// ConstGameData.h: interface for the CGameMapData class.
// paled, modify in 2003.8.26
//////////////////////////////////////////////////////////////////////

#if !defined(CONST_GAMEDATA_HEAD3)
#define CONST_GAMEDATA_HEAD3

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "I_mydb.h"

//////////////////////////////////////////////////////////////////////
template<class T, char* TBL, T KEY>
class CConstGameData //: IGameData<T>
{
protected:
    CConstGameData()									{ m_pRecord = NULL; m_idKey = ID_NONE; }
    virtual ~CConstGameData()							{ if (m_pRecord) { m_pRecord->Release(); } }
public: // construct
    //	static bool			Init(IDatabase* pDb);
    static CConstGameData*	CreateNew()					{ return new CConstGameData; }
    virtual void		Release()						{ delete this; }
    //	static void			Final()							{ if (m_pDefaultRes) m_pDefaultRes->Release(); m_pDefaultRes=NULL; }	// 可重复调用
    //IGameData*			QueryInterface()				{ return (IGameData*)this; }
public: // by set
    virtual void		ReleaseByOwner()				{ delete this; }
    OBJID		GetID()									{ return m_idKey; }
public:
    OBJID		GetKey()								{ return m_idKey; }
    int			GetInt(T nDataName)						{ return m_pRecord->GetInt(nDataName); }
    LPCTSTR		GetStr(T nDataName)						{ return m_pRecord->GetStr(nDataName); }
    /*public: // IGameData
    	int			GetInt(int nDataName)						{ return GetInt_((T)nDataName); }
    	LPCTSTR		GetStr(int nDataName)						{ return GetStr_((T)nDataName); }
    	void		SetInt(int nDataName, int nData)			{ SetInt_((T)nDataName, nData); }
    	void		SetStr(int nDataName, LPCTSTR szData, int nSize)		{ SetStr_((T)nDataName, szData, nSize); }
    */
public:
    bool		Create(OBJID id, IDatabase* pDb);				// 装入数据
    bool		Create(IRecordset* pRecordset)
    {
        m_pRecord = pRecordset->CreateNewConstRecord();
        if (m_pRecord) { m_idKey	= m_pRecord->GetInt(KEY); }
        return m_pRecord != NULL;
    }
    IRecord*	GetRecord()								{ ASSERT(m_pRecord); return m_pRecord; }
protected:
    IRecord*	m_pRecord;
    OBJID		m_idKey;


    //	MYHEAP_DECLARATION(s_heap)
};

template<class T, char* TBL, T KEY>
bool CConstGameData<T, TBL, KEY>::Create(OBJID id, IDatabase* pDb)
{
    CHECKF(pDb);
    char	szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s WHERE %s=%u LIMIT 1", TBL, KEY, id);
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
    m_pRecord	= pRes->CreateNewRecord();
    pRes->Release();			//?? IRecordset暂不支持修改KEY值
    if (m_pRecord)
    {
        m_idKey	= m_pRecord->GetInt(KEY);
    }
    return m_pRecord != NULL;
}


#endif // !defined(CONST_GAMEDATA_HEAD3)
