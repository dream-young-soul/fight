
//**********************************************************
// 代码编辑器
//**********************************************************


#ifndef __MYDB_H__
#define __MYDB_H__

#include "I_mydb.h"
#include "MyHeap.h"

typedef char* HSTMT;
typedef bool ( * ENUMPROC)(char* );

static	int g_nMyDBError;
static	char m_szMyDBErrorMsg[256];
/////////////////////////////////////////////////////////////////////////////
class CMyDatabase : public IDatabase
{
    // Construct
public:
    CMyDatabase();
    virtual ~CMyDatabase();
public:
    static CMyDatabase*	CreateNew()	{ return new CMyDatabase; }		// { (CMyDatabase*)s_heap.Alloc(sizeof(CMyDatabase)); }
protected: // interface
    virtual	void	Release() { delete this; }				// { s_heap.Free(this); }
    virtual IRecordset*	CreateNewRecordset(LPCTSTR szSQL, bool bNoEmpty = true);
public: // interface
    virtual bool	ExecuteSQL(char* szSQL);
    //	virtual bool	CheckSQL(const char* pszSQL);
    virtual int		GetInsertId();
    virtual int		CountRecord	(const char* szSQL);

public:
    IDatabase*	GetInterface() { return (IDatabase*)this; }

    // Init & Final
    BOOL Init		(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);
    void Destroy	(void);

    bool	ExecuteSQL(char* szSQL, MYSQL_RES** pRes) { return ExecuteSQL(szSQL); }	//★ 仅用于兼容于幻灵代码
    //	int		GetInsertId();

    static	bool CheckSQL	(const char* pszSQL);

private:
    // Attributes
    bool	IsOpen()		{return m_bIsOpen;}

    bool	EnumDatabases	(ENUMPROC lpEnumFunc);
    bool	EnumTables		(ENUMPROC lpEnumFunc);
    void	GetDBErrorMsg	(char* szErrorMsg);

    // Operations
    bool Open(char* szHost, char* szUser, char* szPasswd, char* szDB,
              unsigned int uiPort = MYSQL_PORT, char* szSocket = NULL,
              unsigned int uiFlag = NULL);
    virtual void Close();

protected:
    MYSQL*	m_hdbc;
    int		m_nRefCount;
    HANDLE	m_hDbMutex;

    // friend classes that call protected CMyDatabase overridables
    friend class CMyRecordset;
    friend class CMyRecord;

private:
    bool 		m_bIsOpen;
    char		m_szDBName[32];

protected: // heap
    static CMyHeap	s_heap;
};

// extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);


/////////////////////////////////////////////////////////////////////////////
#endif //__MYDB_H__

