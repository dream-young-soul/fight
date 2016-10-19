
//**********************************************************
// 代码编辑器
//**********************************************************


#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "mydb.h"
#include "basefunc.h"
#include "myrecordset.h"
#include "myrecord.h"

#ifdef	MULTITHREAD_SAFE
#define	LOCKADD_TIMESUM
#else
extern	long	s_nDatabaseTimeSum;			//?? 用于统计数据库操作所消耗的时间
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)
#endif

// gloable..
CMyDatabase		g_db;

#ifdef	NO_MUTEX
#define		RELEASE_MUTEX(x)
#else
#define		RELEASE_MUTEX(x)		::ReleaseMutex(x)
#endif

//////////////////////////////////////////////////////////////////////
IRecordset* CMyDatabase::CreateNewRecordset(LPCTSTR szSQL)
{
    CMyRecordset* pRes = (CMyRecordset::CreateNew(this));
    if (pRes)
    {
        if (pRes->Open(szSQL))
        {
            return (IRecordset*)pRes;
        }

        delete pRes;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////
CMyDatabase::CMyDatabase()
{
    m_hdbc = NULL;
    m_bIsOpen = FALSE;
    m_nRefCount = 0;
    m_szDBName[0]	= 0;	//paled debug
}			//★ 未初始化 m_szDBName、m_bIsConnect、m_hstmt

CMyDatabase::~CMyDatabase()
{

}

///////////////////////////////////////////////////////////////////////
bool CMyDatabase::CheckSQL(const char* pszSQL)
{
    if (!pszSQL)
    {
        return false;
    }

    if (strstr(pszSQL, "UPDATE"))	// update sql
    {
        if (!strstr(pszSQL, "WHERE") || !strstr(pszSQL, "LIMIT"))
        {
            // invalid sql
            ::LogSave("Invalid update SQL [%s].", pszSQL);
            return false;
        }
    }

    if (strstr(pszSQL, "DELETE"))	// delete sql
    {
        if (!strstr(pszSQL, "WHERE"))
        {
            // invalid sql
            ::LogSave("Invalid SQL delete [%s].", pszSQL);
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMyDatabase::Open(char* szHost, char* szUser, char* szPasswd, char* szDB,
                       unsigned int uiPort, char* szSocket, unsigned int uiFlag )
{
    if (m_bIsOpen)
    {
        if (m_nRefCount == 0 )
        {
            Close();
        }
        else
        {
            // message:please close all recordset
            g_nMyDBError = 1000;
            return FALSE;
        }
    }

    m_hdbc = mysql_init(NULL);
    if (m_hdbc == NULL)
    {
        g_nMyDBError = 1001;
        return FALSE;
    }

    if (szDB != NULL)
    {
        strcpy(m_szDBName , szDB);
    }

    if ( mysql_real_connect(m_hdbc, szHost, szUser, szPasswd,
                            szDB, uiPort, szSocket , uiFlag) == NULL )
    {
        const char* msg = mysql_error(m_hdbc);
        MessageBox(NULL, "err", msg, MB_OK);
        g_nMyDBError = 1002;
        return FALSE;
    }
    m_bIsOpen = TRUE;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
void CMyDatabase::Close()
{
    if ( m_hdbc != NULL )
    {
        mysql_close( m_hdbc );
    }
    m_bIsOpen = false;
    m_hdbc = NULL;

    //paled debug
    if (m_nRefCount)
    {
        ::OutputDebugString("*DATABASE* warning: Found unclosed CRecordset.");
    }
}					//★ 未检查 m_nRefCount

//////////////////////////////////////////////////////////////////////
bool CMyDatabase::ExecuteSQL(char* szSQL)		//, MYSQL_RES ** pRes
{
    if (!CMyDatabase::CheckSQL(szSQL))
    {
        return false;
    }

    try
    {
        if (m_hdbc == NULL)
        {
            g_nMyDBError = 1003;
            return false;
        }

        time_t	tStart = clock();
#ifdef	NO_MUTEX
        if (true)
#else
        if (WAIT_OBJECT_0 ==::WaitForSingleObject(m_hDbMutex, _MAX_DBACCESSTIME))
#endif
        {
            if (mysql_query(m_hdbc, szSQL) != NULL)
            {
                LOCKADD_TIMESUM;
                RELEASE_MUTEX(m_hDbMutex);
                g_nMyDBError = 1004;
                return false;
            }
//			else if (NULL == pRes)
            {
                LOCKADD_TIMESUM;
                RELEASE_MUTEX(m_hDbMutex);
                return true;
            }
            /*			else
            			{
            				LOCKADD_TIMESUM;
            				MYSQL_RES * res=mysql_store_result(m_hdbc);

            				if (res == NULL)
            				{	if (mysql_field_count(m_hdbc) >0 )
            					{	g_nMyDBError = 1005;
            						RELEASE_MUTEX(m_hDbMutex);
            						return false;
            					}
            					else
            					{
            						*pRes = NULL;
            						RELEASE_MUTEX(m_hDbMutex);
            						return true;
            					}
            				}
            				else
            				{	*pRes = res;
            					RELEASE_MUTEX(m_hDbMutex);
            					return true;
            				}
            			}*/
        }
        else
        {
            ::LogSave("*DATABASE* WARNING: ExecuteSQL() overtime");
            return false;
        }
    }
    catch (...)
    {
        ::LogSave("MyDataBase Exe Sql error: %s", szSQL);
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
int CMyDatabase::GetInsertId()
{
    if (m_hdbc)
    {
        return (int)mysql_insert_id(m_hdbc);
    }
    else
    {
        return 0;
    }
}

/*/////////////////////////////////////////////////////////////////////
bool CMyDatabase::EnumDatabases( ENUMPROC lpEnumFunc)
{
	MYSQL_RES * res;
	MYSQL_ROW	row;
	char * szSQL = (char *) &SQL_STMT_SHOWDATABASES[0];
	if (ExecuteSQL(szSQL, &res))
	{	for (int i=0;i<mysql_num_rows(res);i++)
		{	row=mysql_fetch_row(res) ;
			if (!row || !row[0] || !lpEnumFunc(row[0])) break;
		}
		mysql_free_result(res);
		return true;
	}
	else return false;
}*/

/*/////////////////////////////////////////////////////////////////////
bool CMyDatabase::EnumTables( ENUMPROC lpEnumFunc)
{
	MYSQL_RES * res;
	MYSQL_ROW	row;
	char * szSQL = (char *) &SQL_STMT_SHOWTABLES[0];
	if (ExecuteSQL(szSQL, &res))
	{	for (int i=0;i<mysql_num_rows(res);i++)
		{	row=mysql_fetch_row(res) ;
			if (!row || !row[0] || !lpEnumFunc(row[0])) break;
		}
		mysql_free_result(res);
		return true;
	}
	else return false;
}*/


//////////////////////////////////////////////////////////////////////
void CMyDatabase::GetDBErrorMsg(char* szErrorMsg)
{
    strcpy(szErrorMsg, mysql_error(m_hdbc));
}


//////////////////////////////////////////////////////////////////////
// globle init function
//////////////////////////////////////////////////////////////////////
DWORD g_dwServerID	= 1;
DWORD g_dwMaxPlayer = 800;

BOOL CMyDatabase::Init(const char* pszDBServer, const char* pszDBUser, const char* pszPassword, const char* pszDBName)
{
    // init db
    char szDBServer[256], szDBName[256], szDBUser[256], szPassword[256];

#ifdef _COLOR_ENABLE
    if (pszDBServer)
    {
        strcpy(szDBServer, pszDBServer);
    }
    else
    {
        strcpy(szDBServer,	"192.168.1.200");
    }

    if (pszDBName)
    {
        strcpy(szDBName, pszDBName);
    }
    else
    {
        strcpy(szDBName,	"hl2");
    }

#else
    if (pszDBServer)
    {
        strcpy(szDBServer, pszDBServer);
    }
    else
    {
        strcpy(szDBServer,	"61.131.96.97");
    }

    if (pszDBName)
    {
        strcpy(szDBName, pszDBName);
    }
    else
    {
        strcpy(szDBName,	"games");
    }

#endif

    if (pszDBUser)
    {
        strcpy(szDBUser, pszDBUser);
    }
    else
    {
        strcpy(szDBUser,	"blacknull");
    }

    if (pszPassword)
    {
        strcpy(szPassword, pszPassword);
    }
    else
    {
        strcpy(szPassword,	"monstermaster");
    }

    // for local debug
    {
        FILE* fp = fopen("gameserver.cfg", "r");
        if (fp)
        {
            fscanf(fp, "%u %u %s %s %s %s\n", &g_dwMaxPlayer, &g_dwServerID, szDBServer, szDBUser, szPassword, szDBName);
            fclose(fp);
        }
    }

    if (Open(szDBServer, szDBUser, szPassword, szDBName))
    {
        m_hDbMutex	=::CreateMutex(NULL, false, NULL);			// 允许多个连接 "GameDBMutex"
//@@@		m_hDbMutex	=::CreateMutex(NULL, false, "GameDBMutex");			// 允许多个连接 "GameDBMutex"
        if (m_hDbMutex)
        {
            return true;
        }
        else
        {
            ::MessageBox(NULL, "m_hDbMutex产生失败。", "Error", MB_OK | MB_ICONERROR);
            return false;
        }
    }
    else
    {
        ::MessageBox(NULL, "数据库对象open失败。", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
void CMyDatabase::Destroy	(void)
{
    if (m_hDbMutex)
    {
        ::CloseHandle(m_hDbMutex);
        m_hDbMutex	= NULL;
    }

    Close();
}

//////////////////////////////////////////////////////////////////////
IDatabase* CreateDatabase(const char* szDBServer /*= NULL*/, const char* szLoginName /*= NULL*/, const char* szPassword /*= NULL*/, const char* szDBName /*= NULL*/)
{
    CMyDatabase* pDb = new CMyDatabase;
    if (!pDb)
    {
        return NULL;
    };

    if (pDb->Init(szDBServer, szLoginName, szPassword, szDBName))
    {
        return pDb;
    }

    delete pDb;
    return NULL;
}


