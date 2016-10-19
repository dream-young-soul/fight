
//**********************************************************
// 代码编辑器
//**********************************************************

// MyHeap.cpp: implementation of the CMyHeap class.
//
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "MyHeap.h"

#define		CHECK	ASSERT
#define		CHECKF	ASSERT

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyHeap::CMyHeap(LPCTSTR pszClassName)
{
    m_hHeap	=::HeapCreate(0, 0, 0);

    m_uNumAllocsInHeap = 0;
    if (pszClassName)
    {
        SafeCopy(m_szClassName, pszClassName, CLASSNAME_SIZE);
    }
}

//////////////////////////////////////////////////////////////////////
CMyHeap::~CMyHeap()
{
    if (m_hHeap)
    {
        ::HeapDestroy(m_hHeap);
    }
    if (m_uNumAllocsInHeap != 0)
    {
        if (strlen(m_szClassName) > 0)
        {
            LOGWARNING("CMyHeap::~CMyHeap() 的[%s]有[%d]个内存块没有释放", m_szClassName, m_uNumAllocsInHeap);
        }
        else
        {
            LOGWARNING("CMyHeap::~CMyHeap() 有[%d]个内存块没有释放", m_uNumAllocsInHeap);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void* CMyHeap::Alloc(size_t size)
{
    CHECKF(m_hHeap);
    if (size == 0)
    {
        return false;
    }

    // alloc mem for new obj
    void* p	=::HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY, size);
    if (p)
    {
        m_uNumAllocsInHeap++;
    }

    return p;
}

//////////////////////////////////////////////////////////////////////
void CMyHeap::Free(void* p)
{
    // check param
    CHECK(m_hHeap && m_uNumAllocsInHeap > 0);

    // free it...
    if (::HeapFree(m_hHeap, 0, p))
    {
        m_uNumAllocsInHeap--;
    }
}

//////////////////////////////////////////////////////////////////////
BOOL CMyHeap::IsValidPt(void* p)
{
    if (!m_hHeap || !p)
    {
        return false;
    }

    if ((DWORD)p < 0x00010000 || (DWORD)p >= 0x7FFEFFFF)	// user address range from 1M--2G-64k
    {
        return false;
    }

    if (::IsBadCodePtr((FARPROC)p))
    {
        return false;
    }

    return ::HeapValidate(m_hHeap, 0, p);
}







