
//**********************************************************
// 代码编辑器
//**********************************************************

////////////////////////////////////////////
// T_CompactObjSet.h
// paled, 2003.2.17
// 说明：该SET只能放“非堆分配”对象，且构造函数和析购函数不会被调用。
//		 此SET内部的对象只能调用成员函数(如Create(...))来显式初始化和析购。
///////////////////////////////////////////

#ifndef T_COMPACTOBJSET_H
#define T_COMPACTOBJSET_H

#include "common.h"


template<class T>
class CompactObjSet
{
public:
    CompactObjSet()	{ m_buf = NULL; }
    ~CompactObjSet() { if (m_buf) { delete []m_buf; }}
public:
    bool	Create(int nSize)						{ CHECKF(!m_buf); m_nSize = nSize; m_buf = new char[sizeof(T)*m_nSize]; clear(); return m_buf != NULL; }
public:
    int		size()									{ return m_nSize; }
    T*		operator [](int idx)					{ return (T*)(m_buf + sizeof(T) * idx); }
    void	clear()									{ memset(m_buf, 0, sizeof(T)*m_nSize); }
protected:
    char*	m_buf;
    int		m_nSize;
};


#endif
