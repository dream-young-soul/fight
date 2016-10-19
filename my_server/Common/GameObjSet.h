
//**********************************************************
// 代码编辑器
//**********************************************************

// GameObjSet.h: interface for the CGameObjSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOBJSET_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
#define AFX_GAMEOBJSET_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// 说明：该模板需要GetID()和ReleaseByOwner()函数支持，同时也需要定义OBJID和ULONG

#include <vector>
using namespace std;

template<class T>
class IGameObjSet
{
protected:
    virtual ~IGameObjSet() {}
public:
    virtual void		Release()				PURE_VIRTUAL_FUNCTION
    virtual int			GetAmount()	const			PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObjByIndex(int idx)	PURE_VIRTUAL_FUNCTION_0
    virtual bool		AddObj(T* pObj)			PURE_VIRTUAL_FUNCTION_0
    virtual bool		DelObj(OBJID idObj)		PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObj(OBJID idObj)		PURE_VIRTUAL_FUNCTION_0
    virtual T*			PopObj(OBJID idObj)		PURE_VIRTUAL_FUNCTION_0		// 用于从OWNER_SET中取出对象，不删除。
    //	virtual void		Clear	()				PURE_VIRTUAL_FUNCTION
};

template<class T>
class CGameObjSet : public IGameObjSet<T>
{
protected:
    typedef	vector<T*>	VECTOR_SET;
protected:
    CGameObjSet(bool bOwner) { m_bOwner = bOwner; }
    virtual ~CGameObjSet();
public:
    static CGameObjSet<T>*	CreateNew(bool bOwner = true) { return new CGameObjSet<T>(bOwner); }		//? default is owner set
    void		Release			()				{ delete this; }

    //////////////////////////////////////////////////////
public:	// new traversal
    class	Iterator
    {
    private: // create by parent class
        Iterator(VECTOR_SET* pSet) : m_pSet(pSet), m_idx(pSet->size() - 1), m_pObj(0) {}
        friend class CGameObjSet<T>;
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
    //////////////////////////////////////////////////////

public:	// old traversal
    int			GetAmount		() const		{ return m_setObj.size(); }
    T*			GetObjByIndex	(int idx)		{ CHECKF(idx >= 0 && idx < (int)m_setObj.size()); return m_setObj[idx]; }

public: // application
    bool		AddObj			(T* pObj);
    bool		DelObj			(OBJID idObj);
    bool		DelObj			(Iterator& iter);
    T*			GetObj			(OBJID idObj);
    T*			PopObj			(OBJID idObj);
    T*			PopObj			(Iterator& iter);
protected:
    void		Clear			();

protected:
    VECTOR_SET			m_setObj;
    bool				m_bOwner;
};

//////////////////////////////////////////////////////////////////////
template<class T>
CGameObjSet<T>::~CGameObjSet()
{
    Clear();
}

//////////////////////////////////////////////////////////////////////
template<class T>
void CGameObjSet<T>::Clear()
{
    if (m_bOwner)
    {
        for(size_t i = 0; i < m_setObj.size(); i++)
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
bool CGameObjSet<T>::AddObj(T* pObj)
{
    CHECKF(pObj && pObj->GetID());
    ASSERT(!DelObj(pObj->GetID()));
    m_setObj.push_back(pObj);
    return true;
}

//////////////////////////////////////////////////////////////////////
template<class T>
bool CGameObjSet<T>::DelObj(Iterator& iter)
{
    if (iter.m_pObj)
    {
        T*	pObj = iter.m_pObj;
        if (PopObj(iter) == NULL)
        {
            return false;
        }
        if (m_bOwner)
        {
            pObj->ReleaseByOwner();
        }
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
template<class T>
T* CGameObjSet<T>::PopObj(Iterator& iter)
{
    if (iter.m_pObj)
    {
        T* pObj = iter.m_pObj;
        iter.m_pObj	= NULL;
        int	idx = iter.m_idx + 1;
        IF_OK(idx < (int)m_setObj.size())
        {
            m_setObj.erase(m_setObj.begin() + idx);
            return pObj;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
template<class T>
bool CGameObjSet<T>::DelObj(OBJID idObj)
{
    CHECKF(idObj);
    for(UINT i = 0; i < m_setObj.size(); i++)
    {
        T* pObj = m_setObj[i];
        if (pObj && pObj->GetID() == idObj)
        {
            m_setObj.erase(m_setObj.begin() + i);
            if (m_bOwner)
            {
                pObj->ReleaseByOwner();
            }
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
template<class T>
T* CGameObjSet<T>::GetObj(OBJID idObj)
{
    for(UINT i = 0; i < m_setObj.size(); i++)
    {
        T* pObj = m_setObj[i];
        if (pObj && pObj->GetID() == idObj)
        {
            return pObj;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
template<class T>
T* CGameObjSet<T>::PopObj(OBJID idObj)
{
    ASSERT(m_bOwner);
    for(UINT i = 0; i < m_setObj.size(); i++)
    {
        T* pObj = m_setObj[i];
        if (pObj && pObj->GetID() == idObj)
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



#endif // !defined(AFX_GAMEOBJSET_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
