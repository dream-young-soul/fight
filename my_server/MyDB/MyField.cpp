
//**********************************************************
// 代码编辑器
//**********************************************************


#ifdef	MULTITHREAD_SAFE
#include <afxmt.h>
#define	LOCKOBJ		CSingleLock xLock(&m_xCtrl, true)
#include <winsock2.h>
#else
#define	LOCKOBJ
#endif

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "mydb.h"
#include "BaseFunc.h"

// static init
HANDLE	CMyField::s_hHeap				= NULL;
UINT	CMyField::s_uNumAllocsInHeap	= 0;
#ifdef	MULTITHREAD_SAFE
CCriticalSection	CMyField::m_xCtrl;
#endif

//////////////////////////////////////////////////////////////////////////////
// Implementation of CMyField

CMyField::CMyField()
{
    // Initialize type and value
    m_ulType = FIELD_TYPE_NULL;
    m_szVal = NULL;
}		//★ 未初始化 m_Field,m_nIndex,m_pMyRecordset,m_nNumAllocsInHeap
//paled - m_pMyRecordset为回指指针，由OWNER对象初始化。m_Field、m_nIndex也是。m_szVal的空间也由OWNER分配.。

void CMyField::Clear()
{

}

CMyField::~CMyField()
{
    //Clear();
}

void* CMyField::operator new(size_t size)
{
    LOCKOBJ;

    // got heap?
    if (!s_hHeap)
    {
        s_hHeap	=::HeapCreate(0, 0, 0);
        if (!s_hHeap)
        {
            return NULL;
        }
    }

    // alloc mem for new obj
    void* p	=::HeapAlloc(s_hHeap, HEAP_ZERO_MEMORY, size);

    if (p)
    {
        s_uNumAllocsInHeap++;
    }

    return p;
}

void CMyField::operator delete(void* p)
{
    LOCKOBJ;

    // check param
    if (!s_hHeap || s_uNumAllocsInHeap <= 0)
    {
        return;
    }

    // free it...
    if (::HeapFree(s_hHeap, 0, p))
    {
        s_uNumAllocsInHeap--;
    }

    // time to destroy heap?
    if (s_uNumAllocsInHeap <= 0)
    {
        if (::HeapDestroy(s_hHeap))
        {
            s_hHeap	= NULL;
        }
    }
}

void* CMyField::operator new[](size_t size)
{
    LOCKOBJ;

    // got heap?
    if (!s_hHeap)
    {
        s_hHeap	=::HeapCreate(0, 0, 0);
        if (!s_hHeap)
        {
            return NULL;
        }
    }

    // alloc mem for new obj
    void* p	=::HeapAlloc(s_hHeap, HEAP_ZERO_MEMORY, size);

    if (p)
    {
        s_uNumAllocsInHeap++;
    }

    return p;
}

void CMyField::operator delete[](void* p)
{
    LOCKOBJ;

    // check param
    if (!s_hHeap || s_uNumAllocsInHeap <= 0)
    {
        return;
    }

    // free it...
    if (::HeapFree(s_hHeap, 0, p))
    {
        s_uNumAllocsInHeap--;
    }

    // time to destroy heap?
    if (s_uNumAllocsInHeap <= 0)
    {
        if (::HeapDestroy(s_hHeap))
        {
            s_hHeap	= NULL;
        }
    }
}

BOOL CMyField::IsValidPt(void* p)
{
    LOCKOBJ;

    if (!s_hHeap || !p)
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

    return ::HeapValidate(s_hHeap, 0, p);
}

//////////////////////////////////////////////////////////////////
//	Operator

void CMyField::operator = (unsigned char chOp)
{
    if (m_pMyField->m_ulType == FIELD_TYPE_TINY)
    {
        m_pMyField->m_ucVal = chOp;
    }
}

void CMyField::operator = (char chOp)
{
    if (m_pMyField->m_ulType == FIELD_TYPE_TINY)
    {
        m_pMyField->m_cVal = chOp;
    }
}

void CMyField::operator = ( int nOp)
{
    if (m_pMyField->m_ulType == FIELD_TYPE_LONG)
    {
        m_pMyField->m_iVal = nOp;
    }
}

void CMyField::operator = ( long lOp)
{
    if (m_pMyField->m_ulType == FIELD_TYPE_LONG)
    {
        m_pMyField->m_lVal = lOp;
    }
}

void CMyField::operator = ( float fltOp )
{
    if (m_pMyField->m_ulType == FIELD_TYPE_FLOAT)
    {
        m_pMyField->m_fVal = fltOp;
    }
}

void CMyField::operator = ( double dbOp )
{
    if (m_pMyField->m_ulType == FIELD_TYPE_DOUBLE)
    {
        m_pMyField->m_dVal = dbOp;
    }
}

void CMyField::operator = ( unsigned long lOp )
{
    if (m_pMyField->m_ulType == FIELD_TYPE_LONG ||
            m_pMyField->m_ulType == FIELD_TYPE_SHORT )
    {
        m_pMyField->m_lVal = lOp;
    }
}


void CMyField::FieldName(char* szFileName)
{
    strcpy(szFileName, m_Field.name);
}

unsigned int CMyField::FieldLen() const
{
    return m_Field.length;
}

unsigned int CMyField::FieldAttr() const
{
    return m_Field.flags;
}


void CMyField::CopyFrom(const char* szField, int nLen)			//★ 不应该操作本地变量(结果：字符串指针已回指到CMyRecordset)
{
    try
    {
        assert(szField != NULL);
        memset(m_szVal, 0, nLen);
        strncpy(m_szVal, szField, nLen - 1);		//★★ 没检查长度超长。丢失了尾0，暂未解决
    }
    catch(...) { LogSave("*DATABASE* ERROR: CMyField::CopyFrom() illegal access."); }	//paled debug
}

void CMyField::CopyTo(char* szField, int nLen)			//★ 不应该操作本地变量(结果：字符串指针已回指到CMyRecordset)
{
    try
    {
        assert(szField != NULL);
        memset(szField, 0, nLen);
        strncpy(szField, m_szVal, nLen - 1);		//★★ 没检查长度超长。丢失了尾0，暂未解决
    }
    catch(...) { LogSave("*DATABASE* ERROR: CMyField::CopyTo() illegal access."); }	//paled debug
}


#ifdef	EXTERN_C_DEFINE
void CMyField::operator = (char* szVal)
{
    try
    {
        if (m_pMyField->m_ulType == FIELD_TYPE_STRING)		//paled debug
        {
            strcpy(m_pMyField->m_szVal , szVal);    //★★ BUG: 长度可能溢出，暂未解决
        }
    }
    catch(...) 				//paled debug
    {
        LogSave("*DATABASE* ERROR: CMyField::operator char*() illegal access.");
    }
}
#else
CMyField::operator = (CString strOp)
{
    strcpy(m_pMyField->m_szVal, strOp);
}
#endif

