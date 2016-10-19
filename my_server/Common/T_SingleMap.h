
//**********************************************************
// 代码编辑器
//**********************************************************

// 索引类，用于建立数据表之间的快速查询。加入的KEY不能重复
// 仙剑修，2001.12.18
//////////////////////////////////////////////////////////////////////
// 说明：该模板需要GetID()和ReleaseByOwner()函数支持，同时也需要定义OBJID和ULONG

#ifndef	TSINGLEMAP_H
#define TSINGLEMAP_H

//#include "Define.h"
#include "basefunc.h"
#include "logfile.h"

#pragma warning(disable:4786)
#include <map>
using namespace std;

template<class T>
class ISingleMap
{
protected:
    virtual ~ISingleMap() {}

public:
    typedef	map<OBJID, T*>			MapIndex_t;
    typedef	typename MapIndex_t::iterator	Iter_t;

    virtual void		Release()	= 0;//						PURE_VIRTUAL_FUNCTION
    virtual Iter_t		Begin() 		= 0;//						PURE_VIRTUAL_FUNCTION_0
    virtual Iter_t		End()			= 0;//						PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObjByIter(Iter_t i)	= 0;//				PURE_VIRTUAL_FUNCTION_0
    virtual bool		AddObj(T* pObj)		= 0;//					PURE_VIRTUAL_FUNCTION_0
    virtual bool		DelObj(OBJID key)	= 0;//					PURE_VIRTUAL_FUNCTION_0
    virtual T*			GetObj(OBJID key)	= 0;//					PURE_VIRTUAL_FUNCTION_0
    virtual int			GetAmount()							PURE_VIRTUAL_FUNCTION_0
    //	virtual OBJID			Key(iterator Iter_t)					PURE_VIRTUAL_FUNCTION_0
    //	virtual T*			PopObj(OBJID idObj, int idType)		PURE_VIRTUAL_FUNCTION_0		// 用于从OWNER_SET中取出对象，不删除。
};

template<class T>
class CSingleMap : public ISingleMap<T>
{
protected:
    typedef	map<OBJID, T*>		MAP_SET;
    typedef	typename MAP_SET::iterator	MAP_ITER;

public:
    class	Iterator
    {
    private: // create by parent class
        Iterator(MAP_SET* pSet) : m_pSet(pSet), m_iter(pSet->begin()), m_pObj(0) {}
        friend class CSingleMap<T>;
    public:
        //		Iterator(const Iterator& iter);		// need not define this ^_^

    public: // application
        bool	Next()		{ if (m_iter == m_pSet->end()) { return m_pObj = NULL, false; } ASSERT(m_pObj = m_iter->second); m_iter++; return true; }
        void	Reset()		{ m_iter = m_pSet->begin(); m_pObj = NULL; }
        operator T*()		{ return m_pObj; }
        T* operator->()		{ return m_pObj; }

    protected:
        MAP_SET*	m_pSet;
        MAP_ITER	m_iter;
        T*			m_pObj;
    };
public: // traverse
    Iterator	NewEnum		() 						{ return Iterator(&m_map); }
    int			Size		() const				{ return m_map.size(); }

    //////////////////////////////////////////////////////
protected:
    CSingleMap(bool bOwner) { m_bOwner = bOwner; }
    virtual ~CSingleMap();
public:
    static CSingleMap<T>*	CreateNew(bool bOwner) { return new CSingleMap<T>(bOwner); }
public:
    //	typedef	map<OBJID, T*> MapIndex_t;	typedef	MapIndex_t::iterator	Iter_t;
    void	Release() { delete this; }
    Iter_t	Begin() 					{ return m_map.begin(); }
    Iter_t	End()						{ return m_map.end(); }
    T*		GetObjByIter(Iter_t i)		{ return i->second; }
    bool	AddObj(T* pObj);
    bool	DelObj(OBJID key);
    bool	DelObj(Iterator& iter);
    T*		PopObj(Iterator& iter);
    T*		GetObj(OBJID key);
    int		GetAmount()										{ return m_map.size(); }
    ///	OBJID		Key(iterator Iter_t)								{ return Iter_t->first; }
    //	bool	IsIn(OBJID key);
protected:
    void		Clear			();
protected:
    bool		m_bOwner;
    MapIndex_t	m_map;
};

template<class T>
CSingleMap<T>::~CSingleMap()
{
    Clear();
}

template<class T>
void CSingleMap<T>::Clear()
{
    if (m_bOwner && m_map.size())
    {
        for(Iter_t i = Begin(); i != End(); i++)
        {
            GetObjByIter(i)->ReleaseByOwner();
        }
    }
    m_map.clear();
}

template<class T>
bool	CSingleMap<T>::AddObj(T* pObj)
{
    CHECKF(pObj && pObj->GetID());
    if (m_bOwner)
    {
        ASSERT(!DelObj(pObj->GetID()));
    }
    m_map[pObj->GetID()] = pObj;
    return true;
}

template<class T>
bool	CSingleMap<T>::DelObj(Iterator& iter)
{
    if (iter.m_pObj)
    {
        if (m_bOwner)
        {
            iter.m_pObj->ReleaseByOwner();
        }
        return PopObj(iter) != NULL;
    }
    return false;
}

template<class T>
T*	CSingleMap<T>::PopObj(Iterator& iter)
{
    if (iter.m_pObj)
    {
        T* pObj = iter.m_pObj;
        iter.m_pObj	= NULL;
        IF_OK(iter.m_iter != m_map.begin())
        {
            MAP_ITER	temp = iter.m_iter;
            temp--;
            m_map.erase(temp);
            return pObj;
        }
    }
    return NULL;
}

template<class T>
bool	CSingleMap<T>::DelObj(OBJID key)
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

template<class T>
T*		CSingleMap<T>::GetObj(OBJID key)
{
    Iter_t i;
    if ((i = m_map.find(key)) != m_map.end())
    {
        return i->second;
    }
    return NULL;
}

/*template<class T>
bool	CSingleMap<T>::IsIn(OBJID key)
{
	return (m_map.find(key) != m_map.end());
}*/


#endif // TSINGLEMAP_H
