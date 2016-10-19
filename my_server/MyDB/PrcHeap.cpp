
//**********************************************************
// 代码编辑器
//**********************************************************

// PrcHeap.cpp: implementation of the CPrcHeap class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "PrcHeap.h"

// global...
CPrcHeap g_heapSystem;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrcHeap::CPrcHeap()
{
    Init();
}

CPrcHeap::~CPrcHeap()
{
}

//////////////////////////////////////////////////////////////////////

void CPrcHeap::Init(void)
{
    m_hHeap	=::GetProcessHeap();			//★ 使用系统堆并不能防止内存碎片。应使用::HeapCreate()
}

//////////////////////////////////////////////////////////////////////

void* CPrcHeap::New(size_t size)
{
    if (!m_hHeap)
    {
        return NULL;
    }

    return ::HeapAlloc(m_hHeap, 0, size);
}

//////////////////////////////////////////////////////////////////////

void CPrcHeap::Delete(void* pObj)
{
    if (!m_hHeap || !pObj)
    {
        return;
    }

    ::HeapFree(m_hHeap, 0, pObj);
}

//////////////////////////////////////////////////////////////////////

BOOL CPrcHeap::IsSafe(void* pObj)
{
    if (!m_hHeap || !pObj)
    {
        return false;
    }

    if ((DWORD)pObj < 0x00010000 || (DWORD)pObj >= 0x7FFEFFFF)	// user address range from 1M--2G-64k
    {
        return false;
    }

    if (::IsBadCodePtr((FARPROC)pObj))
    {
        return false;
    }

    return ::HeapValidate(m_hHeap, 0, pObj);
}

//////////////////////////////////////////////////////////////////////
// global functions
BOOL IsValidPtr(void* p)
{
    return g_heapSystem.IsSafe(p);
}
