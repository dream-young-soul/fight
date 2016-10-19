
//**********************************************************
// 代码编辑器
//**********************************************************

// MultiObjSet.h: interface for the CMultiObjSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MULTIOBJSET_HEADER)
#define MULTIOBJSET_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Myheap.h"

#include <vector>
using namespace std;

template<class T>
class IMultiObjSet
{
public:
    virtual ~IMultiObjSet() {}
    virtual void		Release()				PURE_VIRTUAL_FUNCTION
    virtual int			GetAmount()				PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObjByIndex(int idx)	PURE_VIRTUAL_FUNCTION_0
    virtual bool		AddObj(T* pObj)			PURE_VIRTUAL_FUNCTION_0
    virtual bool		DelObj(OBJID idObj, int idType)		PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObj(OBJID idObj, int idType)		PURE_VIRTUAL_FUNCTION_0
    virtual T*			PopObj(OBJID idObj, int idType)		PURE_VIRTUAL_FUNCTION_0		// 用于从OWNER_SET中取出对象，不删除。
    virtual void		Clear()								PURE_VIRTUAL_FUNCTION
};

template<class T>
class CMultiObjSet : IMultiObjSet<T>
{
protected:
    CMultiObjSet(bool bOwner)								{ m_bOwner = bOwner; }
    virtual ~CMultiObjSet();
public:
    static IMultiObjSet<T>*	CreateNew(bool bOwner)			{ return new CMultiObjSet<T>(bOwner); }

public:
    void		Release()									{ delete this; }
    int			GetAmount()									{ return m_setObj.size(); }
    T*			GetObjByIndex(int idx)						{ CHECKF(idx >= 0 && idx < m_setObj.size()); return m_setObj[idx]; }
    bool		AddObj(T* pObj);
    bool		DelObj(OBJID idObj, int idType);
    T*			GetObj(OBJID idObj, int idType);
    T*			PopObj(OBJID idObj, int idType);
    void		Clear();

protected:
    typedef	vector<T*>	GAMEOBJ_SET;
    GAMEOBJ_SET			m_setObj;
    bool				m_bOwner;

    //	MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////
template<class T>
CMultiObjSet<T>::~CMultiObjSet()
{
    Clear();
}

//////////////////////////////////////////////////////////////////////
template<class T>
void CMultiObjSet<T>::Clear()
{
    if (m_bOwner)
    {
        for(int i = 0; i < m_setObj.size(); i++)
        {
            T* pObj = m_setObj[i];
            if (pObj)
            {
                pObj->ReleaseByOwner();
            }
        }
    }
    m_setObj.clear();
}

//////////////////////////////////////////////////////////////////////
template<class T>
bool CMultiObjSet<T>::AddObj(T* pObj)
{
    CHECKF(pObj && pObj->GetID());
    ASSERT(!DelObj(pObj->GetID(), pObj->GetObjType()));
    m_setObj.push_back(pObj);
    return true;
}

//////////////////////////////////////////////////////////////////////
template<class T>
bool CMultiObjSet<T>::DelObj(OBJID idObj, int idType)
{
    CHECKF(idObj);
    for(int i = 0; i < m_setObj.size(); i++)
    {
        T* pObj = m_setObj[i];
        if (pObj && pObj->GetObjType() == idType && pObj->GetID() == idObj )
        {
            if (m_bOwner)
            {
                pObj->ReleaseByOwner();
            }
            m_setObj.erase(m_setObj.begin() + i);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
template<class T>
T* CMultiObjSet<T>::GetObj(OBJID idObj, int idType)
{
    for(int i = 0; i < m_setObj.size(); i++)
    {
        T* pObj = m_setObj[i];
        if (pObj && pObj->GetObjType() == idType && pObj->GetID() == idObj)
        {
            return pObj;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
template<class T>
T* CMultiObjSet<T>::PopObj(OBJID idObj, int idType)
{
    ASSERT(m_bOwner);
    for(int i = 0; i < m_setObj.size(); i++)
    {
        T* pObj = m_setObj[i];
        if (pObj && pObj->GetObjType() == idType && pObj->GetID() == idObj)
        {
            if (m_bOwner)
            {
                m_setObj.erase(m_setObj.begin() + i);
            }
            return pObj;
        }
    }
    return NULL;
}



#endif // !defined(MULTIOBJSET_HEADER)
