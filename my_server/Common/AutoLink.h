
//**********************************************************
// 代码编辑器
//**********************************************************

// 自动链接类
// 仙剑修, 2003.7.7
////////////////////////////////////////////////////////////////
// 说明:“主AutoLink”必须要用父对象的指针初始化自己，Init()，不可为空。
//		“从AutoLink”可使用赋值的方式关联到主LINK。(注意：最好不要出现太多的临时对象，否则会有一定代价)
//		“从AutoLink”内部的指针可能随时被“主AutoLink”清空，所以每次使用“从AutoLink”时必须先检查是否为空。(!)
//		“AutoLink”可以用Clear()清空对象。“主AutoLink”会同时清空所有链。“从AutoLink”只会从链中删除自己。
////////////////////////////////////////////////////////////////
//		父对象必须实现一个函数：CAutoLink<T>& QueryLink(const CAutoLink<T>&)，返回“主AutoLink”的引用。(参数只有类型，无意义)


#ifndef	AUTO_LINK_H
#define	AUTO_LINK_H

#include "common.h"
#include "basetype.h"

////////////////////////////////////////////////////////////////
template<class T>
class	CAutoLink;
template<class T>
class	CAutoLink
{
public:
    CAutoLink()										{ Init(); }
    ~CAutoLink()									{ Break(); }
    CAutoLink(const CAutoLink<T>& link)				{ Init(); Insert(&const_cast<CAutoLink<T>&>(link)); }
    CAutoLink(T* ptr)						{ Init(); Insert(&ptr->QueryLink(CAutoLink<T>())); }
    CAutoLink&	operator=(const CAutoLink<T>& link)	{ Break(); Insert(&const_cast<CAutoLink<T>&>(link)); return *this; }
    CAutoLink&	operator=(T* ptr)					{ Break(); Insert(&ptr->QueryLink(CAutoLink<T>())); return *this; }
    void	Init(T* pOwner)							{ ASSERT(!IsValid() && pOwner); m_pOwner = pOwner; }
    void	Clear()									{ Break(); }

public: // const
    operator T*()			const					{ return m_pOwner; }
    POINT_NO_RELEASE<T>*		operator->()	const					{ ASSERT(IsValid()); return static_cast<POINT_NO_RELEASE<T>* >(m_pOwner); }

public: // modify

protected:
    void	Init();
    void	Insert(CAutoLink<T>* pPrev);		// insert to next, only for !IsMaster
    void	Erase();							// erase from list, only for !IsMaster
    void	Break();
    bool	IsValid()		const					{ return m_pOwner != NULL; }
    bool	IsMaster()		const					{ return IsValid() && m_pPrev == NULL; }

protected:
    T*				m_pOwner;
    CAutoLink<T>*	m_pPrev;
    CAutoLink<T>*	m_pNext;
};

////////////////////////////////////////////////////////////////
template<class T>
inline void CAutoLink<T>::Init()						// Init all
{
    m_pOwner	= NULL;
    m_pPrev		= NULL;
    m_pNext		= NULL;
}

////////////////////////////////////////////////////////////////
template<class T>
void CAutoLink<T>::Break()
{
    if (IsMaster())		// master
    {
        CAutoLink<T>* pNext = m_pNext;
        int	nCount = 0;
        while(pNext)
        {
            CAutoLink<T>* pCurr = pNext;
            pNext	= pNext->m_pNext;			// ++
            ASSERT(pCurr->IsValid());
            pCurr->Init();
            DEAD_LOOP_BREAK(nCount, 100)
        }
        Init();
    }
    else
    {
        Erase();
    }
}

////////////////////////////////////////////////////////////////
template<class T>
void CAutoLink<T>::Insert(CAutoLink<T>* pPrev)		// insert to next
{
    CHECK(pPrev);
    CHECK(pPrev != this);
    CHECK(!IsMaster());
    Erase();
    this->m_pOwner			= pPrev->m_pOwner;
    if (IsValid())
    {
        this->m_pPrev			= pPrev;
        this->m_pNext			= pPrev->m_pNext;
        if (pPrev->m_pNext)
        {
            pPrev->m_pNext->m_pPrev	= this;
        }
        pPrev->m_pNext			= this;
    }
}

////////////////////////////////////////////////////////////////
template<class T>
void CAutoLink<T>::Erase()						// erase from list
{
    if (IsValid())
    {
        CHECK(!IsMaster());
        m_pPrev->m_pNext	= this->m_pNext;
        if (m_pNext)
        {
            m_pNext->m_pPrev	= this->m_pPrev;
        }
        Init();
    }
}


#endif // AUTO_LINK_H
