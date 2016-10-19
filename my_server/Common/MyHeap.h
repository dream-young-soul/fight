
//**********************************************************
// ¥˙¬Î±‡º≠∆˜
//**********************************************************

// MyHeap.h: interface for the CMyHeap class.
// paled, 2002.?.?
//////////////////////////////////////////////////////////////////////
// put line in *.h:		MYHEAP_DECLARATION(s_heap)
// put line in *.cpp:	MYHEAP_IMPLEMENTATION(TypeName,s_heap)

#if !defined(AFX_MYHEAP_H__CFFE0003_9790_4E6B_B0C8_16CF55F89B19__INCLUDED_)
#define AFX_MYHEAP_H__CFFE0003_9790_4E6B_B0C8_16CF55F89B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

const int	CLASSNAME_SIZE = 32;

class CMyHeap
{
public:
    CMyHeap(LPCTSTR pszClassName = NULL);
    ~CMyHeap();
    //	static CMyHeap* CreateNew()		{ return new CMyHeap; }
public:
    void*	Alloc(size_t size);
    void	Free(void* ptr);
    BOOL	IsValidPt(void* p);
protected:
    HANDLE			m_hHeap;
    long			m_uNumAllocsInHeap;
    char			m_szClassName[CLASSNAME_SIZE];		// ”√”⁄DEBUG
};

#ifdef	_DEBUG ////////////////////////////////////////////////////////

#define	MYHEAP_DECLARATION(VAR)
#define	MYHEAP_IMPLEMENTATION(T,VAR)

#else // _DEBUG ////////////////////////////////////////////////////////

#define	MYHEAP_DECLARATION(VAR)					\
	void   operator delete	(void* p);			\
	void*  operator new	(size_t size);			\
public:											\
	static BOOL		IsValidPt(void* p);			\
protected:										\
	static CMyHeap	VAR;


#define	MYHEAP_IMPLEMENTATION(T,VAR)			\
CMyHeap	T::VAR(#T);								\
void * T::operator new(size_t size)				\
	{ return VAR.Alloc(size); }					\
void T::operator delete(void* p)				\
	{ VAR.Free(p); }							\
BOOL T::IsValidPt(void* p)						\
	{ return VAR.IsValidPt(p); }

#endif // _DEBUG ////////////////////////////////////////////////////////


#endif // !defined(AFX_MYHEAP_H__CFFE0003_9790_4E6B_B0C8_16CF55F89B19__INCLUDED_)
