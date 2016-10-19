
//**********************************************************
// 代码编辑器
//**********************************************************

// MyRecord.cpp: implementation of the CMyConstRecord class.
// 仙剑修，2002.9.12
// 仙剑修 修改于 2003.8.26
//////////////////////////////////////////////////////////////////////

#ifdef	MULTITHREAD_SAFE
#include <afxmt.h>
#define	LOCKOBJ		CSingleLock xLock(&m_xCtrl, true)
#include <winsock2.h>
#else
#define	LOCKOBJ
#endif

//#ifdef	MULTITHREAD_SAFE
//#define	LOCKADD_TIMESUM
//#else
extern	long	s_nDatabaseTimeSum;			//?? 用于统计数据库操作所消耗的时间
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)
// #endif

#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "mydb.h"
#include "basefunc.h"
#include "MyConstRecord.h"
#include "common.h"

#ifdef	NO_MUTEX
#define		RELEASE_MUTEX(x)
#else
#define		RELEASE_MUTEX(x)		::ReleaseMutex(x)
#endif


// static init
CMyHeap	CMyConstRecord::s_heapString("CMyConstRecord::s_heapString");
#ifdef	MULTITHREAD_SAFE
CCriticalSection	CMyConstRecord::m_xCtrl;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 构建 UpdateSQLs

MYHEAP_IMPLEMENTATION(CMyConstRecord, s_heap)
///////////////////////////////////////////////////////////////////////
CMyConstRecord::CMyConstRecord(CMyDatabase*   pMyDatabase )
{
}			//★ 未初始化其它成员变量(其它成员变量由open()初始化)

///////////////////////////////////////////////////////////////////////
CMyConstRecord* CMyConstRecord::CreateNew(CMyDatabase*   pMyDatabase)
{
    return (new CMyConstRecord(pMyDatabase));
}

///////////////////////////////////////////////////////////////////////
bool CMyConstRecord::Open(CMyRecordset*  pMyRecordset, DWORD idNewKey /*= NEWKEY_NONE*/)
{
    int	nFieldsCount	= pMyRecordset->FieldsCount();
    for(int i = 0; i < nFieldsCount; i++)
    {
        //		m_setName.Push(pMyRecordset->m_Fields[i].name);
    }
    for(int i = 0; i < nFieldsCount; i++)
    {
        pMyRecordset->GetFieldValue(i);
        VarType	data;
        if (pMyRecordset->m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING
                || pMyRecordset->m_objFields[i].m_ulType == FIELD_TYPE_STRING)
        {
            data = VarType(pMyRecordset->m_objFields[i].m_szVal);
        }
        else
        {
            data = VarType(pMyRecordset->m_objFields[i].m_iVal);
        }
        m_setField.Push(data);
    }
    return true;
}

///////////////////////////////////////////////////////////////////////
CMyConstRecord::~CMyConstRecord()
{
}

///////////////////////////////////////////////////////////////////////
// record
///////////////////////////////////////////////////////////////////////
IRecord* CMyConstRecord::CreateNewRecord(DWORD idNewKey /*= NEWKEY_NONE*/)
{
    return new CMyConstRecord(NULL);
}

///////////////////////////////////////////////////////////////////////
DWORD CMyConstRecord::InsertRecord()
{
    ASSERT(!"const");
    return	false;
}

///////////////////////////////////////////////////////////////////////
bool CMyConstRecord::UpdateRecord()
{
    ASSERT(!"const");
    return	false;
}

///////////////////////////////////////////////////////////////////////
bool CMyConstRecord::DeleteRecord()
{
    ASSERT(!"const");
    return	false;
}

///////////////////////////////////////////////////////////////////////
// new interface
///////////////////////////////////////////////////////////////////////
int CMyConstRecord::LoadInt		(LPCTSTR szField)
{
    /*	for(int i = 0; i < m_setName.Size(); i++)
    	{
    		if (strcmp(szField, m_setName[i]) == 0)
    			return GetInt(i);
    	}
    */	ASSERT(!"no field");
    return 0;
}

///////////////////////////////////////////////////////////////////////
void CMyConstRecord::SetInt		(LPCTSTR szField, int nData)
{
    ASSERT(!"const");
}

///////////////////////////////////////////////////////////////////////
void CMyConstRecord::LoadString	(char* szData, LPCTSTR szField, int nSize)
{
    /*	for(int i = 0; i < m_setName.Size(); i++)
    	{
    		if (strcmp(szField, m_setName[i]) == 0)
    		{
    			SafeCopy(szData, GetStr(i), nSize);
    			return;
    		}
    	}
    */	ASSERT(!"no field");
}

///////////////////////////////////////////////////////////////////////
void CMyConstRecord::SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)
{
    ASSERT(!"const");
}

///////////////////////////////////////////////////////////////////////
int CMyConstRecord::GetInt		(int idx)
{
    IF_NOT(idx >= 0 && idx < m_setField.Size())
    {
        LOGERROR("invalid index[%d] in table [?s].", idx);
        return INT_NONE;
    }
    return static_cast<int>(m_setField[idx]);
}

///////////////////////////////////////////////////////////////////////
LPCTSTR CMyConstRecord::GetStr		(int idx)
{
    IF_NOT(idx >= 0 && idx < m_setField.Size())
    return STR_NONE;
    return static_cast<const String&>(m_setField[idx]);
}

///////////////////////////////////////////////////////////////////////
void CMyConstRecord::SetInt		(int idx, int nData)
{
    ASSERT(!"const");
}

///////////////////////////////////////////////////////////////////////
void CMyConstRecord::SetStr		(int idx, LPCTSTR szData, int nSize)
{
    ASSERT(!"const");
}














