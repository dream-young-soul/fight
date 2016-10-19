
//**********************************************************
// 代码编辑器
//**********************************************************

// 队列
// 仙剑修，2003.5.24
//////////////////////////////////////////////////////////////////////
// 说明：pop不成功时返回0。
//////////////////////////////////////////////////////////////////////


#ifndef	MYQUEUE_H
#define MYQUEUE_H

#pragma warning(disable:4786)
#include <deque>
using namespace std;

template<class T>
class CMyQueue
{
public:
    void	push(T obj)					{ m_set.push_back(obj); }
    T		pop()						{ if (size()) { T obj = *m_set.begin(); m_set.pop_front(); return obj; } return 0; }
    int		size()						{ return m_set.size(); }
    void	clear()						{ m_set.clear(); }
    //	deque<T>&	QueryList()				{ return m_set; }
protected:
    typedef	deque<T>		QUEUE_SET;
    QUEUE_SET		m_set;
};



#endif // MYQUEUE_H
