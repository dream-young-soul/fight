
//**********************************************************
// 代码编辑器
//**********************************************************

// 数据库底层的通用接口
// 仙剑修, 2002.9.27
//////////////////////////////////////////////////////////////////


#ifndef	I_MYDB_H
#define	I_MYDB_H

#define PURE_VIRTUAL_DECLARATION_DB	= 0;
#define PURE_VIRTUAL_DECLARATION_DB_0	= 0;

/*
	说明：为兼容旧系统，函数参数尽量与旧系统相同。
*/

class	CMyRecordset;
class	CMyRecord;
class	IRecordset;
class	IDatabase
{
public:
    virtual ~IDatabase() {}
    virtual IRecordset*	CreateNewRecordset(LPCTSTR szSQL)		PURE_VIRTUAL_DECLARATION_DB_0
    virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB
public:
    virtual bool	ExecuteSQL(char* szSQL)				PURE_VIRTUAL_DECLARATION_DB_0
    virtual int		GetInsertId()						PURE_VIRTUAL_DECLARATION_DB_0
};

class	IRecord
{
public:
    virtual ~IRecord() {}
    virtual IRecord*	CreateNewRecord(DWORD idNewKey = 0)		PURE_VIRTUAL_DECLARATION_DB_0
    virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB

    virtual bool	InsertRecord()								PURE_VIRTUAL_DECLARATION_DB
    virtual bool	UpdateRecord()								PURE_VIRTUAL_DECLARATION_DB
    virtual bool	DeleteRecord()								PURE_VIRTUAL_DECLARATION_DB

    virtual int		LoadInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
    virtual void	SetInt		(LPCTSTR szField, int nData)		PURE_VIRTUAL_DECLARATION_DB
    virtual UINT	LoadUInt	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
    virtual void	SetUInt		(LPCTSTR szField, UINT nData)		PURE_VIRTUAL_DECLARATION_DB
    virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize)		PURE_VIRTUAL_DECLARATION_DB
    virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
    virtual DWORD	LoadDWord	(LPCTSTR szField)					{ return LoadUInt(szField); }
    virtual void	SetDWord	(LPCTSTR szField, DWORD nData)		{ SetUInt(szField, nData); }
    // 注：暂时还需要遵守“先取后写”的规则，待具体实现改变后可不按此规则。

    virtual bool	AddInt(LPCTSTR szField, int nData)			PURE_VIRTUAL_DECLARATION_DB		// 立即写库。如无共享冲突，可不提供该接口
public:
//	virtual bool	IsOpen()									PURE_VIRTUAL_DECLARATION_DB_0
//	virtual bool	IsValidPt(void* p)							PURE_VIRTUAL_DECLARATION_DB_0
};

class	IRecordset : public IRecord
{
public:
    virtual ~IRecordset() {}
//	virtual IRecord*	GetRecordInterface()	PURE_VIRTUAL_DECLARATION_DB_0		// 取IRecord
    virtual UINT		RecordCount()			PURE_VIRTUAL_DECLARATION_DB_0		// return -1: error
    virtual bool		MoveNext()				PURE_VIRTUAL_DECLARATION_DB			// return false: no more data
};


// SetDatabase()、SetTableName()暂不提供接口，暂时直接从对象进行操作。

extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);



#endif // I_MYDB_H
