
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// PrcHeap.h: interface for the CPrcHeap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJHEAP_H__5F62F501_11AD_4962_B7A1_BED361A28533__INCLUDED_)
#define AFX_OBJHEAP_H__5F62F501_11AD_4962_B7A1_BED361A28533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPrcHeap
{
public:
    CPrcHeap();
    virtual ~CPrcHeap();

    void	Init	(void);

    void*	New		(size_t size);
    void	Delete	(void* pObj);
    BOOL	IsSafe	(void* pObj);

private:
    HANDLE		m_hHeap;

};

// global func
BOOL IsValidPtr(void* p);

// external...
extern CPrcHeap g_heapSystem;

#endif // !defined(AFX_OBJHEAP_H__5F62F501_11AD_4962_B7A1_BED361A28533__INCLUDED_)
