
//**********************************************************
// 代码编辑器
//**********************************************************

// 索引类，用于建立数据表之间的快速查询。加入的KEY不能重复
// 仙剑修，2001.12.18
//////////////////////////////////////////////////////////////////////

#ifndef	INDEX_H
#define INDEX_H

#include "Define.h"
//#include "basefunc.h"

#pragma warning(disable:4786)
#include <map>
using namespace std;

///////////////////////////////////////////////////////////////////////
//#define	MORECHECK		//??? 加强检测, 但降低效率。重复添加会出错
///////////////////////////////////////////////////////////////////////

//#define	LogSave	printf
//#define	class K, class D/*, D ERR*/	class K, class D/*, D ERR*/
//#define	K, D/*, ERR*/	K, D/*, ERR*/

template < class K, class D/*, D ERR*/ >
class CIndex
{
public:
    CIndex() {}
    virtual ~CIndex() { if (m_map.size()) { LogSave("WARNING: CIndex destruction in not empty[%d].", m_map.size()); } }

public:
    typedef	map<K, D> MapIndex_t;
    typedef	typename  map<K, D>::iterator	Iter_t;

    bool		Add(K key, D data);
    bool		Del(K key);
    bool		IsIn(K key);
    D			operator[](K key);
    void		ClearAll() { m_map.clear(); }

    K			Key(Iter_t iter)	{ return iter->first; }
    D			Data(Iter_t iter)	{ return iter->second; }
    Iter_t		Begin() { return m_map.begin(); }
    Iter_t		End()	{ return m_map.end(); }
    DWORD		Size()	{ return m_map.size();}
protected:
    MapIndex_t	m_map;
};

template < class K, class D/*, D ERR*/ >
bool	CIndex < K, D/*, ERR*/ >::Add(K key, D data)
{
#ifdef	MORECHECK
    if (m_map.find(key) == m_map.end())
#endif
    {
        m_map[key] = data;
        return true;
    }
    LogSave("ERROR: CIndex::Add() attempt add again. refused.");
    return false;
}

template < class K, class D/*, D ERR*/ >
bool	CIndex < K, D/*, ERR*/ >::Del(K key)
{
#ifdef	MORECHECK
    if (m_map.find(key) != m_map.end())
#endif
    {
        m_map.erase(key);
        return true;
    }
    LogSave("ERROR: CIndex::Del() attempt del again. refused.");
    return false;
}

template < class K, class D/*, D ERR*/ >
bool	CIndex < K, D/*, ERR*/ >::IsIn(K key)
{
    if (m_map.find(key) != m_map.end())
    {
        return true;
    }
    return false;
}

template < class K, class D/*, D ERR*/ >
D		CIndex < K, D/*, ERR*/ >::operator[](K key)
{
    MapIndex_t::iterator iter;
    if ((iter = m_map.find(key)) != m_map.end())
    {
        return iter->second;
    }
    //? 因为有时会用不存在的ID为参数	LogSave("ERROR: CIndex operator[]() not found key.");
    return 0;
}


#endif // INDEX_H
