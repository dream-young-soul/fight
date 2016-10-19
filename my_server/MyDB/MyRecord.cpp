
//**********************************************************
// 代码编辑器
//**********************************************************

// MyRecord.cpp: implementation of the CMyRecord class.
// 仙剑修，2002.9.12
//////////////////////////////////////////////////////////////////////

#ifdef	MULTITHREAD_SAFE
#include <afxmt.h>
#define	LOCKOBJ		CSingleLock xLock(&m_xCtrl, true)
#include <winsock2.h>
#else
#define	LOCKOBJ
#endif

#ifdef	MULTITHREAD_SAFE
#define	LOCKADD_TIMESUM
#else
extern	long	s_nDatabaseTimeSum;			//?? 用于统计数据库操作所消耗的时间
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)
#endif

#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "mydb.h"
#include "basefunc.h"
#include "PrcHeap.h"
#include "MyRecord.h"

#ifdef	NO_MUTEX
#define		RELEASE_MUTEX(x)
#else
#define		RELEASE_MUTEX(x)		::ReleaseMutex(x)
#endif


// static init
HANDLE	CMyRecord::s_hHeap				= NULL;
UINT	CMyRecord::s_uNumAllocsInHeap	= 0;
#ifdef	MULTITHREAD_SAFE
CCriticalSection	CMyRecord::m_xCtrl;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 构建 UpdateSQLs

///////////////////////////////////////////////////////////////////////
bool CMyRecord::FormatField(char* szFormat, const CMyField& objField)
{
    switch (objField.m_ulType)
    {
    case FIELD_TYPE_STRING:
    case FIELD_TYPE_VAR_STRING:
        if (strlen(objField.m_szVal) == NULL)			//paled 不能向数据库写入空串!!!
        {
            return false;
        }
        else
        {
            sprintf(szFormat, "'%s'", objField.m_szVal);
        }
        break;

    case FIELD_TYPE_TINY:
    case FIELD_TYPE_SHORT:
        sprintf(szFormat, "%d", objField.m_iVal);
        break;

    case FIELD_TYPE_FLOAT:
        sprintf(szFormat, "%.2f", objField.m_fVal);
        break;

    case FIELD_TYPE_DOUBLE:
        sprintf(szFormat, "%.2f", objField.m_dVal);
        break;

    case FIELD_TYPE_LONG:
        if ((objField.m_Field.flags & UNSIGNED_FLAG) != 0)
        {
            sprintf(szFormat, "%u", objField.m_ulVal);
        }
        else
        {
            sprintf(szFormat, "%ld", objField.m_lVal);
        }
        break;

    default:
        ::LogSave("Error: unknow type in CMyRecord::FieldToStr()");
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::BuildUpdateCondition()
{
    sprintf(m_szUpdateCondition, "%s=%s", m_szKey, m_szKeyData);
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::BuildUpdateOpration()
{
    char szFormat[1024];

    m_szUpdateOpration[0] = '\0';

    BOOL bFirst = true;

    switch ( m_nEditMode )
    {
    case modeEdit:
    case modeAddNew:
        for (UINT i = 0; i < m_uiFieldsCount; i++ )
        {
            if ( m_pbDirty[i] )
            {
                if (FormatField(szFormat, m_objFields[i]))
                {
                    if (!bFirst)
                    {
                        strcat(m_szUpdateOpration, ",");
                    }
                    else
                    {
                        bFirst = false;
                    }

                    strcat(m_szUpdateOpration, m_Fields[i].name);
                    strcat(m_szUpdateOpration, "=");
                    strcat(m_szUpdateOpration, szFormat);
                    if (strlen(m_szUpdateOpration) >= 1024)
                    {
                        ::LogSave("ERROR: BuildUpdateOpration() 溢出!!!");
                    }
                }

            } // if ( m_pbDirty[i] )
        } // for ( i=0; i<m_uiFieldsCount; i++ )
        break;
    }
}


///////////////////////////////////////////////////////////////////////
void CMyRecord::BuildUpdateSQL()
{
    if (!m_bCanUpdate)
    {
        return;
    }

    BuildUpdateOpration();
    BuildUpdateCondition();

    char* psz;

    switch (m_nEditMode)
    {
    case modeDelete:
        psz = (char* )SQL_STMT_DELETE;
        break;

    case modeEdit:
        psz = (char* )SQL_STMT_UPDATE;
        break;

    case modeAddNew:
        psz = (char* )SQL_STMT_INSERT;
        break;
    }

    sprintf(m_szUpdateSQL, psz, m_szTableName,			//paled - SQL_STMT_DELETE的第2个参数为空串
            m_szUpdateOpration, m_szUpdateCondition);		//paled - SQL_STMT_INSERT只有两个参数，将丢弃第3个参数
    if (strlen(m_szUpdateSQL) >= 1024)
    {
        ::LogSave("ERROR: BuildUpdateSQL() 溢出!!!");
    }
}


///////////////////////////////////////////////////////////////////////
bool CMyRecord::Update()
{
    if (!m_bIsOpen)
    {
        return false;
    }

    m_bEdit = false;

    bool ret	= true;
    if (this->UpdateFieldsCount() > 0)
    {
        // build sql
        this->BuildUpdateSQL();

        // need change the local row;
        MYSQL_RES* res;
        ret = Query(m_szUpdateSQL, &res);
        if (ret && res)
        {
            mysql_free_result(res);
        }
    }

    if (m_nEditMode == modeAddNew)
    {
        if (m_objFields[m_uiKeyIndex].m_lVal == 0 && (m_objFields[m_uiKeyIndex].FieldAttr() & AUTO_INCREMENT_FLAG) )
        {
            m_objFields[m_uiKeyIndex].m_lVal	= m_pMyDatabase->GetInsertId();
            FormatField(m_szKeyData, m_objFields[m_uiKeyIndex]);
        }
    }

    // clear edit flag
    this->ClsEditFlag();

    return	ret;
}


///////////////////////////////////////////////////////////////////////
CMyField* CMyRecord::GetFieldRef(UINT uiIndex)
{
    return m_objFields + uiIndex;
}

///////////////////////////////////////////////////////////////////////
CMyRecord::CMyRecord(CMyDatabase*   pMyDatabase )
{
    m_bIsOpen = false;

    m_pMyDatabase = pMyDatabase;
    if (pMyDatabase == NULL )
    {
        return;
    }

    if (m_pMyDatabase != NULL && m_pMyDatabase->IsOpen())
    {
        m_pMyDatabase->m_nRefCount++;
    }
}			//★ 未初始化其它成员变量(其它成员变量由open()初始化)

///////////////////////////////////////////////////////////////////////
CMyRecord* CMyRecord::CreateNew(CMyDatabase*   pMyDatabase)
{
    if (pMyDatabase == NULL ||
            !pMyDatabase->IsOpen())
    {
        return NULL;
    }

    return (new CMyRecord(pMyDatabase));
}

///////////////////////////////////////////////////////////////////////
void* CMyRecord::operator new(size_t size)
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

///////////////////////////////////////////////////////////////////////
void CMyRecord::operator delete(void* p)
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

///////////////////////////////////////////////////////////////////////
BOOL CMyRecord::IsValidPt(void* p)
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

///////////////////////////////////////////////////////////////////////
bool CMyRecord::Open(CMyRecordset*  pMyRecordset, DWORD idNewKey /*= 0*/)
{
    if (pMyRecordset == NULL ||	!pMyRecordset->IsOpen())
    {
        return false;
    }

    if (!m_pMyDatabase->IsOpen()) 
	{ 
		return false; 
	}

    m_uiFieldsCount	= pMyRecordset->m_uiFieldsCount;
    strcpy(m_szKey, pMyRecordset->m_szKey);
    m_uiKeyIndex	= pMyRecordset->m_uiKeyIndex;
    if (idNewKey != 0)
    {
        sprintf(m_szKeyData, "%u", idNewKey);
    }
    else
    {
        FormatField(m_szKeyData, pMyRecordset->m_objFields[pMyRecordset->m_uiKeyIndex]);
    }
    m_bEdit			= pMyRecordset->m_bEdit;
    m_bIsOpen			= pMyRecordset->m_bIsOpen;
    m_nEditMode		= pMyRecordset->m_nEditMode;
    strcpy(m_szTableName, pMyRecordset->m_szTableName);
    m_bCanUpdate		= pMyRecordset->m_bCanUpdate;

#ifdef	USE_NEW
    m_pbDirty	= new bool[pMyRecordset->m_uiFieldsCount];
#else
    m_pbDirty	= (bool*)g_heapSystem.New(sizeof(bool) * pMyRecordset->m_uiFieldsCount);		//★ 全局堆分配，可能产生内存碎片。
#endif
    memcpy(m_pbDirty, pMyRecordset->m_pbDirty, pMyRecordset->m_uiFieldsCount);

#ifdef	USE_NEW
    m_Fields		= new MYSQL_FIELD[m_uiFieldsCount];
#else
    m_Fields		= (MYSQL_FIELD*)g_heapSystem.New(sizeof(MYSQL_FIELD) * m_uiFieldsCount);
#endif
    for(int i = 0; i < (int)m_uiFieldsCount; i++)
    {
        m_Fields[i]	= pMyRecordset->m_Fields[i];
#ifdef	USE_NEW
        m_Fields[i].name = new char[strlen(pMyRecordset->m_Fields[i].name) + 1];
#else
        m_Fields[i].name = (char*)g_heapSystem.New(sizeof(char) * (strlen(pMyRecordset->m_Fields[i].name) + 1));
#endif
        strcpy(m_Fields[i].name, pMyRecordset->m_Fields[i].name);		//VVVVV 注意删除。table、def不复制，从未用到过
    }

    m_objFields		= new CMyField [m_uiFieldsCount];
    for(int i = 0; i < (int)m_uiFieldsCount; i++)
    {
        pMyRecordset->GetFieldValue(i);
        m_objFields[i]		= pMyRecordset->m_objFields[i];
        m_objFields[i].m_Field = m_Fields[i];					// 指向本地name串
        if (m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING
                || m_objFields[i].m_ulType == FIELD_TYPE_STRING)
        {
#ifdef	USE_NEW
            m_objFields[i].m_szVal = new char[m_Fields[i].length + 1];
#else
            m_objFields[i].m_szVal = (char*) g_heapSystem.New(sizeof(char) * (m_Fields[i].length + 1));
#endif
            //★ 全局堆分配，会有内存碎片
            strcpy(m_objFields[i].m_szVal, pMyRecordset->m_objFields[i].m_szVal);
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::Spawn(CMyRecord* pMyRecord, DWORD idNewKey /*= 0*/)
{
    *this	= *pMyRecord;

    if (idNewKey != 0)
    {
        sprintf(m_szKeyData, "%u", idNewKey);
    }

#ifdef	USE_NEW
    m_pbDirty	= new bool[pMyRecord->m_uiFieldsCount];
#else
    m_pbDirty	= (bool*)g_heapSystem.New(sizeof(bool) * pMyRecord->m_uiFieldsCount);		//★ 全局堆分配，可能产生内存碎片。
#endif
    memcpy(m_pbDirty, pMyRecord->m_pbDirty, pMyRecord->m_uiFieldsCount);

#ifdef	USE_NEW
    m_Fields		= new MYSQL_FIELD[m_uiFieldsCount];
#else
    m_Fields		= (MYSQL_FIELD*)g_heapSystem.New(sizeof(MYSQL_FIELD) * m_uiFieldsCount);
#endif
    for(int i = 0; i < (int)m_uiFieldsCount; i++)
    {
        m_Fields[i]	= pMyRecord->m_Fields[i];
#ifdef	USE_NEW
        m_objFields[i].m_Field.name = new char[strlen(pMyRecord->m_objFields[i].m_Field.name) + 1];
#else
        m_objFields[i].m_Field.name	= (char*)g_heapSystem.New(sizeof(char) * (strlen(pMyRecord->m_objFields[i].m_Field.name) + 1));
#endif
        strcpy(m_objFields[i].m_Field.name, pMyRecord->m_objFields[i].m_Field.name);		//VVVVV 注意删除。table、def不复制，从未用到过
    }

    m_objFields		= new CMyField [m_uiFieldsCount];
    for(int i = 0; i < (int)m_uiFieldsCount; i++)
    {
        m_objFields[i]		= pMyRecord->m_objFields[i];
        m_objFields[i].m_Field	= m_Fields[i];					// 指向本地name串
        if (m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING
                || m_objFields[i].m_ulType == FIELD_TYPE_STRING)
        {
#ifdef	USE_NEW
            m_objFields[i].m_szVal = new char[m_Fields[i].length + 1];
#else
            m_objFields[i].m_szVal = (char*) g_heapSystem.New(sizeof(char) * (m_Fields[i].length + 1));
#endif
            //★ 全局堆分配，会有内存碎片
            strcpy(m_objFields[i].m_szVal, pMyRecord->m_objFields[i].m_szVal);
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::Close()
{
    try
    {
        if (m_bIsOpen)
        {
#ifdef	USE_NEW
            delete [] m_pbDirty;
#else
            g_heapSystem.Delete(m_pbDirty);
#endif
            for (unsigned int i = 0; i < m_uiFieldsCount; i++)
            {
                if (m_objFields[i].m_ulType == FIELD_TYPE_STRING
                        || m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING)
#ifdef	USE_NEW
                    delete [] m_objFields[i].m_szVal;
#else
                    g_heapSystem.Delete(m_objFields[i].m_szVal);
#endif

#ifdef	USE_NEW
                delete [] m_Fields[i].name;			// AAAAAAAAAAAAAAAAAA
#else
                g_heapSystem.Delete(m_Fields[i].name);
#endif
            }
            delete [] m_objFields;
#ifdef	USE_NEW
            delete [] m_Fields;
#else
            g_heapSystem.Delete(m_Fields);
#endif
            m_pMyDatabase->m_nRefCount --;
        }
        m_bIsOpen = false;
    }
    catch(...)
    {
        char szMsg[]	= "Mysql RecordSet close error.";
        ::OutputDebugString(szMsg);
    }
}

///////////////////////////////////////////////////////////////////////
CMyRecord::~CMyRecord()
{
    if (m_bIsOpen) { Close(); }
}


///////////////////////////////////////////////////////////////////////
// 数据集操作
void CMyRecord::Edit()
{
    m_bEdit = true;
    if (! m_bIsOpen )
    {
        return;
    }
    m_nEditMode = modeEdit;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::AddNew()
{
    m_bEdit = true;
    if (! m_bIsOpen )
    {
        return;
    }
    m_nEditMode = modeAddNew;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::Delete()
{
    m_bEdit = true;
    if (! m_bIsOpen )
    {
        return;
    }
    m_nEditMode = modeDelete;
}

///////////////////////////////////////////////////////////////////////
// SQL
bool CMyRecord::Query(char* strSQL, MYSQL_RES * * res)
{
    if (!CMyDatabase::CheckSQL(strSQL))
    {
        return false;
    }

    try
    {
        if (!m_bIsOpen) { return false; }

        time_t	tStart = clock();
#ifdef	NO_MUTEX
        if (true)
#else
        if (WAIT_OBJECT_0 ==::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME))
#endif
        {
            MYSQL* hdbc = m_pMyDatabase->m_hdbc;
            if (mysql_query(hdbc, strSQL) != NULL)
            {
                LOCKADD_TIMESUM;
                //error
                RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
                return false;
            }
            else
            {
                LOCKADD_TIMESUM;
                *res = mysql_store_result(hdbc);			//paled - 当仅有INSERT、DELETE、UPDATE等查询时，可使用 mysql_affected_rows() 检查是否成功
                if (*res == NULL)
                {
                    if (mysql_field_count(hdbc) > 0 )
                    {
                        //error
                        RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
                        return false;
                    }
                    else
                    {
                        RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
                        return true;
                    }
                }
                else
                {
                    RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
                    return true;
                }
            }
        }
        else
        {
            ::LogSave("*DATABASE* WARNING: Qurey() overtime");
            return false;
        }
    }
    catch(...)
    {
        char szMsg[1024];
        sprintf(szMsg, "Mysql Query error: %s", strSQL);
        ::OutputDebugString(szMsg);

        return false;
    }
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::IsOpen()
{
    //assert(this);
    return (m_bIsOpen);
}

///////////////////////////////////////////////////////////////////////
int CMyRecord::FieldsCount()
{
    if (m_bIsOpen)
    {
        return m_uiFieldsCount;
    }
    else
    {
        return -1;
    }

}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::CanUpdate() const     // Can Edit/AddNew/Delete be called?
{
    if (!m_bIsOpen ) { return false; }
    return m_bCanUpdate;
}

///////////////////////////////////////////////////////////////////////
//
CMyField CMyRecord::Fields(UINT uiIndex)
{
    if (uiIndex > m_uiFieldsCount)
    {
        ::LogSave("*DATABASE* ERROR: Illegal Fields() parameter.");
        uiIndex = 0;			//★ 0号对象是非法对象么?
    }

    if (m_bEdit)
    {
        m_pbDirty[uiIndex] = true;
        CMyField MyField = m_objFields[uiIndex];
        MyField.m_pMyField = &m_objFields[uiIndex];	//paled
        MyField.m_nIndex = uiIndex;
        MyField.m_szVal = m_objFields[uiIndex].m_szVal;
        return MyField;				//★ BUG: 没有初始化 m_Field 成员变量。没有初始化数值类成员变量
    }

    m_pbDirty[uiIndex] = false;
//	GetFieldValue(uiIndex);
    return m_objFields[uiIndex];
}

///////////////////////////////////////////////////////////////////////
CMyField CMyRecord::Fields(char* szField)
{
    int nIndex = -1 ;
    for (UINT i = 0; i < m_uiFieldsCount; i++)
    {
        if ( strcmp(szField, m_Fields[i].name) == 0)
        {
            nIndex = i;
            break;
        }
    }

    if (nIndex != -1)
    {
        return Fields(nIndex);
    }
    else
    {
        ::LogSave("Error: field %s not found in CMyRecord::Fields", szField);
        return Fields((unsigned int)0);
    }
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::ClsEditFlag()
{
    for ( int i = 0; i < (int)m_uiFieldsCount; i++ )
    {
        m_pbDirty[i] = false;
    }
}

///////////////////////////////////////////////////////////////////////
int CMyRecord::UpdateFieldsCount()
{
    int nCount	= 0;
    for ( int i = 0; i < (int)m_uiFieldsCount; i++ )
    {
        if (m_pbDirty[i])
        {
            nCount++;
        }
    }

    return nCount;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::AddField(LPCTSTR szField, int nData)
{
    sprintf(m_szUpdateSQL, "UPDATE %s SET %s=%s+%d WHERE %s=%s LIMIT 1",
            m_szTableName, szField, szField, nData, m_szKey, m_szKeyData);

    MYSQL_RES* res;
    bool ret = Query(m_szUpdateSQL, &res);
    if (ret && res)
    {
        mysql_free_result(res);
    }
    return	ret;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::SetTableName(LPCTSTR szName, DWORD idNewKey /*= 0*/)
{
    if (!szName || strlen(szName) == 0 || strlen(szName) >= 32)
    {
        return false;
    }

    strcpy(m_szTableName, szName);

    if (idNewKey != 0)
    {
        sprintf(m_szKeyData, "%u", idNewKey);
    }

    return true;
}

///////////////////////////////////////////////////////////////////////
// record
///////////////////////////////////////////////////////////////////////
IRecord* CMyRecord::CreateNewRecord(DWORD idNewKey /*= 0*/)
{
    CMyRecord* pRecord = CMyRecord::CreateNew(m_pMyDatabase);		//VVVVVVVVVVVVVV
    if (pRecord)
    {
        if (pRecord->Spawn(this, idNewKey))
        {
            return (IRecord*)pRecord;
        }

        delete pRecord;
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::InsertRecord()
{
    if (!m_bIsOpen || !m_bCanUpdate)
    {
        return false;
    }

    m_bEdit		= false;
    m_nEditMode	= modeAddNew;

    bool ret	= true;
    if (this->UpdateFieldsCount() > 0)
    {
        // build sql
        this->BuildUpdateSQL();

        // need change the local row;
        MYSQL_RES* res;
        ret = Query(m_szUpdateSQL, &res);
        if (ret && res)
        {
            mysql_free_result(res);
        }
    }
    else
    {
        ::LogSave("WARNING: CMyRecord::InsertRecord() no found field.");
    }

    // clear edit flag
    this->ClsEditFlag();

    return	ret;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::UpdateRecord()
{
    if (!m_bIsOpen || !m_bCanUpdate)
    {
        return false;
    }

    m_bEdit		= false;
    m_nEditMode	= modeEdit;

    bool ret	= true;
    if (this->UpdateFieldsCount() > 0)
    {
        // build sql
        this->BuildUpdateSQL();

        // need change the local row;
        MYSQL_RES* res;
        ret = Query(m_szUpdateSQL, &res);
        if (ret && res)
        {
            mysql_free_result(res);
        }
    }

    // clear edit flag
    this->ClsEditFlag();

    return	ret;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::DeleteRecord()
{
    if (!m_bIsOpen || !m_bCanUpdate)
    {
        return false;
    }

    m_bEdit		= false;
    m_nEditMode	= modeDelete;

    bool ret	= true;
//	if (this->UpdateFieldsCount() > 0)
    {
        // build sql
        this->BuildUpdateSQL();

        // need change the local row;
        MYSQL_RES* res;
        ret = Query(m_szUpdateSQL, &res);
        if (ret && res)
        {
            mysql_free_result(res);
        }
    }
//	else
//		::LogSave("WARNING: CMyRecord::InsertRecord() no found field.");

    // clear edit flag
    this->ClsEditFlag();

    return	ret;
}














