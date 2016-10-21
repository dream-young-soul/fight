
// SingleMap64.h: interface for the CSingleMap64 class.
//
// 以int64类型为键的索引类，用于建立数据表之间的快速查询。加入的KEY不能重复
// 冷紫龙，2003.12.03
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINGLEMAP64_H__D34B0F90_17D0_403C_9ACD_DD44380CA6A2__INCLUDED_)
#define AFX_SINGLEMAP64_H__D34B0F90_17D0_403C_9ACD_DD44380CA6A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"
#include "basefunc.h"
#include "logfile.h"
#include "I_mydb.h"
#include "Myheap.h"

#pragma warning(disable:4786)
#include <map>
using namespace std;

template<class T, class K, K KEY1, K KEY2>
class ISingleMap64
{
protected:
    virtual ~ISingleMap64() {}
public:
    typedef	map<_int64, T*> MapIndex_t;
    typedef	typename MapIndex_t::iterator	Iter_t;
    virtual bool		Create(LPCTSTR szSQL, IDatabase* pDb) = 0;//	PURE_VIRTUAL_FUNCTION_0
    virtual ULONG		Release()				 = 0;//				PURE_VIRTUAL_FUNCTION_0
    virtual Iter_t		Begin() 				 = 0;//				PURE_VIRTUAL_FUNCTION_0
    virtual Iter_t		End()						 = 0;//			PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObjByIter(Iter_t i)		 = 0;//			PURE_VIRTUAL_FUNCTION_0
    virtual bool		AddObj(T* pObj)				 = 0;//			PURE_VIRTUAL_FUNCTION_0
    virtual bool		DelObj(_int64 key)			 = 0;//			PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObj(_int64 key)			 = 0;//			PURE_VIRTUAL_FUNCTION_0

    //	MYHEAP_DECLARATION(s_heap)
};


template<class T, class K, K KEY1, K KEY2>
class CSingleMap64  :	ISingleMap64<T, K, KEY1, KEY2>
{
protected:
    CSingleMap64(bool bOwner) { m_bOwner = bOwner; }
    virtual ~CSingleMap64();

public:
    static ISingleMap64<T, K, KEY1, KEY2>*	CreateNew(bool bOwner)	{ return new CSingleMap64<T, K, KEY1, KEY2>(bOwner); }

public:
    //	typedef	map<_int64, T*> MapIndex_t;	typedef	MapIndex_t::iterator	Iter_t;
    bool	Create(LPCTSTR szSQL, IDatabase* pDb);
    ULONG	Release() { delete this; return 0; }
    Iter_t	Begin() 										{ return m_map.begin(); }
    Iter_t	End()											{ return m_map.end(); }
    T*		GetObjByIter(Iter_t i)							{ return i->second; }
    bool	AddObj(T* pObj);
    bool	DelObj(_int64 key);
    T*		GetObj(_int64 key);

protected:
    bool		m_bOwner;
    MapIndex_t	m_map;
};

//////////////////////////////////////////////////////////////////////
template<class T, class K, K KEY1, K KEY2>
CSingleMap64<T, K, KEY1, KEY2>::~CSingleMap64()
{
    DEBUG_TRY
    if (m_bOwner && m_map.size())
    {
        for(Iter_t i = Begin(); i != End(); i++)
        {
            GetObjByIter(i)->ReleaseByOwner();
        }
    }
    DEBUG_CATCH("CSingleMap64::~CSingleMap64()")
}

//////////////////////////////////////////////////////////////////////
// 此函数需要数据库支持
template<class T, class K, K KEY1, K KEY2>
bool CSingleMap64<T, K, KEY1, KEY2>::Create(LPCTSTR szSQL, IDatabase* pDb)
{
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
    if (!pRes)
    {
        return false;
    }
    DEBUG_TRY
    for(UINT i = 0; i < pRes->RecordCount(); i++)
    {
        T* pData = T::CreateNew();
        if (!pData)
        {
            pRes->Release();
            return false;
        }
        if (!pData->Create(pRes))
        {
            pData->ReleaseByOwner();
            pRes->Release();
            return false;
        }
        AddObj(pData);
        pRes->MoveNext();
    }
    DEBUG_CATCH("CSingleMap64::Create()")
    pRes->Release();
    return true;
}

//////////////////////////////////////////////////////////////////////
template<class T, class K, K KEY1, K KEY2>
bool	CSingleMap64<T, K, KEY1, KEY2>::AddObj(T* pObj)
{
    _int64	key = 0;
    if (m_bOwner)
    {
        key = pObj->GetInt(KEY1);
        key = (key << 32) + pObj->GetInt(KEY2);
        ASSERT(!DelObj(key));
        //		ASSERT(!DelObj(pObj->GetID()));
    }
    m_map[key] = pObj;
    return true;
}

//////////////////////////////////////////////////////////////////////
template<class T, class K, K KEY1, K KEY2>
bool	CSingleMap64<T, K, KEY1, KEY2>::DelObj(_int64 key)
{
    if (m_bOwner)
    {
        Iter_t i = m_map.find(key);
        if (i != m_map.end())
        {
            GetObjByIter(i)->ReleaseByOwner();
            m_map.erase(i);
            return true;
        }
        return false;
    }
    else
    {
        return m_map.erase(key) != 0;
    }
}

//////////////////////////////////////////////////////////////////////
template<class T, class K, K KEY1, K KEY2>
T*		CSingleMap64<T, K, KEY1, KEY2>::GetObj(_int64 key)
{
    Iter_t i;
    if ((i = m_map.find(key)) != m_map.end())
    {
        return i->second;
    }
    return NULL;
}



#endif // !defined(AFX_SINGLEMAP64_H__D34B0F90_17D0_403C_9ACD_DD44380CA6A2__INCLUDED_)
