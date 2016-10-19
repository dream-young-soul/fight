
//**********************************************************
// 代码编辑器
//**********************************************************

// Array.h: interface for the Array class.
// paled, 2003.8.5
//////////////////////////////////////////////////////////////////////
// 包含对象的容器(非包含指针)，支持COPY。
// 正向顺序叠代，可在叠代时删除对象，也可在叠代时增加对象，增加的对象不参加此次叠代。
// 程序最大允许SIZE为20亿，但通常用于小规模的SET
// 注意：多个iterator同时操作时，如果有DelObj()调用，会有BUG。
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAFEARRAY_H__D9E42F53_A4BD_4BB9_9745_D3FE8666BA41__INCLUDED_)
#define AFX_SAFEARRAY_H__D9E42F53_A4BD_4BB9_9745_D3FE8666BA41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basetype.h"
#include "mycom.h"
#pragma warning(disable:4786)
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////
template <class T>
class Array
{
    //-----------------------------------------------
public:	// new traversal
    class	ConstIterator
    {
        enum { VALID_MASK = 0x80000000, SIZE_MASK = 0x7FFFFFFF };
    protected: // create by parent class
        ConstIterator(const vector<T>* pSet) : m_pSet(pSet), m_nValid_Size(pSet->size()), m_idx(0) {}
        friend class Array<T>;

    public: // application
        bool	Next()								{ if (m_idx >= Size()) { return false; } if (!IsValid()) { m_nValid_Size += VALID_MASK; } else { m_idx++; } return m_idx < Size(); }
        void	Reset()								{ m_idx = 0; m_nValid_Size = pSet->size(); }
        operator const T*()							{ ASSERT(IsValid()); return &m_pSet->at(m_idx); }
        const T* operator->()						{ ASSERT(IsValid()); return &m_pSet->operator[](m_idx); }
    protected:
        bool	IsValid()							{ return (m_nValid_Size & VALID_MASK) && m_idx < Size(); }
        int		Size()								{ return m_nValid_Size & SIZE_MASK; }
        void	DelObj()							{ ASSERT(Size()); m_nValid_Size = Size() - 1; }	// set invalid

    protected:
        const vector<T>*	m_pSet;
        unsigned long		m_nValid_Size;
        int					m_idx;
    };
    class	Iterator
    {
        enum { VALID_MASK = 0x80000000, SIZE_MASK = 0x7FFFFFFF };
    protected: // create by parent class
        Iterator(vector<T>* pSet) : m_pSet(pSet), m_nValid_Size(pSet->size()), m_idx(0) {}
        friend class Array<T>;

    public: // application
        bool	Next()								{ if (m_idx >= Size()) { return false; } if (!IsValid()) { m_nValid_Size += VALID_MASK; } else { m_idx++; } return m_idx < Size(); }
        void	Reset()								{ m_idx = 0; m_nValid_Size = m_pSet->size(); }
        operator T*()								{ ASSERT(IsValid()); return &m_pSet->at(m_idx); }
        T* operator->()							{ ASSERT(IsValid()); return &m_pSet->operator[](m_idx); }
    protected:
        bool	IsValid()							{ return (m_nValid_Size & 0x80000000) && m_idx < (int)m_pSet->size(); }
        int		Size()								{ return m_nValid_Size & 0x7FFFFFFF; }
        void	DelObj()							{ ASSERT(Size()); m_nValid_Size = Size() - 1; }	// set invalid

    protected:
        vector<T>*			m_pSet;
        unsigned long		m_nValid_Size;
        int					m_idx;
    };
public: // traverse
    const ConstIterator	NewEnum	() 	const			{ return ConstIterator(&m_set); }
    Iterator	NewEnum	() 							{ return Iterator(&m_set); }
    int			Size	() 	const					{ return m_set.size(); }
    //-----------------------------------------------

public:
    explicit Array(int nSize = 0)					{ m_set.resize(nSize); }
    Array(const Array& obj)							{ m_set = obj.m_set; }
    Array& operator=(const Array& obj)				{ if (this == &obj) { return *this; } m_set = obj.m_set; return *this; }

public:
    T&		operator[](int idx)						{ ASSERT(idx >= 0 && idx < (int)m_set.size()); return m_set[idx]; }
    T&		At(int idx)								{ ASSERT(idx >= 0 && idx < (int)m_set.size()); return m_set[idx]; }
    const T& operator[](int idx) const				{ ASSERT(idx >= 0 && idx < (int)m_set.size()); return m_set[idx]; }
    const T& At(int idx) const						{ ASSERT(idx >= 0 && idx < (int)m_set.size()); return m_set[idx]; }
    void	Clear()									{ m_set.clear(); }
    void	Resize(int nSize)						{ m_set.resize(nSize); }
    void	Push(const T& obj)						{ m_set.push_back(obj); }
    bool	DelObj(Iterator& iter)					{ if (!iter.IsValid()) { return false; } m_set.erase(m_set.begin() + iter.m_idx); iter.DelObj(); return true; }
    bool	DelObj(int idx)							{ ASSERT(idx < (int)m_set.size()); m_set.erase(m_set.begin() + idx); return true; }

public: // dump
    String	Dump()	const							{ String str = "{ "; for(UINT i = 0; i < m_set.size(); i++) { if (i) { str += ", "; } str +=::DumpString(m_set[i]); } return str += " }"; }

protected:
    vector<T>		m_set;
};

#endif // !defined(AFX_SAFEARRAY_H__D9E42F53_A4BD_4BB9_9745_D3FE8666BA41__INCLUDED_)
