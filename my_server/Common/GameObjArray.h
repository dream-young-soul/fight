
//**********************************************************
// 代码编辑器
//**********************************************************

// GameObjArray.h: interface for the CGameObjArray class.
// 仙剑修，2004.1.25
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOBJARRAY_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
#define AFX_GAMEOBJARRAY_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// 说明：该模板需要GetID()和Release()函数支持，同时也需要定义OBJID和ULONG

#include <vector>
using namespace std;

template<class T>
class CGameObjArray
{
protected:
    typedef	vector<T*>	VECTOR_SET;
public:
    CGameObjArray(int nSize = 0, bool bOwner = true) { m_bOwner = bOwner; m_setObj.resize(nSize, NULL); }
    virtual ~CGameObjArray();
public:
    static CGameObjArray<T>*	CreateNew(int nSize = 0, bool bOwner = true) { return new CGameObjArray<T>(nSize, bOwner); }		//? default is owner set
    void		Release			()				{ delete this; }

    //////////////////////////////////////////////////////
public:	// new traversal
    class	Iterator
    {
    private: // create by parent class
        Iterator(VECTOR_SET* pSet) : m_pSet(pSet), m_idx(pSet->size() - 1), m_pObj(0) {}
        friend class CGameObjArray<T>;
    public:
        //		Iterator(const Iterator& iter);		// need not define this ^_^

    public: // application
        bool	Next()		{ if (m_idx < 0) { return m_pObj = NULL, false; } ASSERT(m_pObj = m_pSet->at(m_idx)); m_idx--; return true; }
        void	Reset()		{ m_idx = m_pSet->size() - 1; m_pObj = NULL; }
        operator T*()							{ return m_pObj; }
        T* operator->()						{ return m_pObj; }

    protected:
        const VECTOR_SET*	m_pSet;
        int			m_idx;
        T*			m_pObj;
    };
public: // traverse
    Iterator	NewEnum		() 						{ return Iterator(&m_setObj); }
    int			Size		() const				{ return m_setObj.size(); }
    bool		Resize		(int nSize)				{ CHECKF(nSize >= Size()); m_setObj.resize(nSize, NULL); return true; }
    //////////////////////////////////////////////////////

public:	// old traversal
    int			GetAmount		() const		{ return m_setObj.size(); }
    T*			GetObjByIndex	(int idx)		{ CHECKF(idx >= 0 && idx < m_setObj.size()); return m_setObj[idx]; }

public: // application
    bool		PushObj			(int idx, T* pObj);
    bool		DelObj			(Iterator& iter);
    T*			PopObj			(Iterator& iter);
    T*			GetObj			(OBJID idObj);
    T*&			operator []		(int idx)		{ ASSERT(idx >= 0 && idx < m_setObj.size()); return m_setObj[idx]; }
protected:
    void		Clear			();

protected:
    VECTOR_SET			m_setObj;
    bool				m_bOwner;
};

//////////////////////////////////////////////////////////////////////
template<class T>
CGameObjArray<T>::~CGameObjArray()
{
    Clear();
}

//////////////////////////////////////////////////////////////////////
template<class T>
void CGameObjArray<T>::Clear()
{
    if (m_bOwner)
    {
        for(int i = 0; i < m_setObj.size(); i++)
        {
            T* pObj = m_setObj[i];
            if (pObj)
            {
                pObj->Release();
            }
        }
    }
    m_setObj.clear();
}

//////////////////////////////////////////////////////////////////////
template<class T>
bool CGameObjArray<T>::PushObj(int idx, T* pObj)
{
    CHECKF(idx >= 0 && idx < m_setObj.size());
    T* pObj = m_setObj[idx];
    if (pObj && m_bOwner)
    {
        pObj->Release();
    }
    m_setObj[idx]	= pObj;
    return true;
}

//////////////////////////////////////////////////////////////////////
template<class T>
bool CGameObjArray<T>::DelObj(Iterator& iter)
{
    if (iter.m_pObj)
    {
        if (m_bOwner)
        {
            iter.m_pObj->Release();
        }
        return PopObj(iter) != NULL;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
template<class T>
T* CGameObjArray<T>::PopObj(Iterator& iter)
{
    if (iter.m_pObj)
    {
        T* pObj = iter.m_pObj;
        iter.m_pObj	= NULL;
        int	idx = iter.m_idx + 1;
        IF_OK(idx < m_setObj.size())
        {
            m_setObj[idx]	= NULL;
            return pObj;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
template<class T>
T* CGameObjArray<T>::GetObj(OBJID idObj)
{
    for(int i = 0; i < m_setObj.size(); i++)
    {
        T* pObj = m_setObj[i];
        if (pObj && pObj->GetID() == idObj)
        {
            return pObj;
        }
    }
    return NULL;
}



#endif // !defined(AFX_GAMEOBJARRAY_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
