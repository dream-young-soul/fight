
//**********************************************************
// 代码编辑器
//**********************************************************

// 数据库底层的通用接口
// 仙剑修, 2002.9.27
//////////////////////////////////////////////////////////////////


#ifndef	I_MYDB_H
#define	I_MYDB_H

#include "common.h"

#define PURE_VIRTUAL_DECLARATION_DB		PURE_VIRTUAL_FUNCTION
#define PURE_VIRTUAL_DECLARATION_DB_0	PURE_VIRTUAL_FUNCTION_0

/*
	说明：为兼容旧系统，函数参数尽量与旧系统相同。
*/

class	CMyRecordset;
class	CMyRecord;
class	IRecordset;
class	IDatabase
{
protected:
    virtual ~IDatabase() {}
public:
    virtual IRecordset*	CreateNewRecordset(LPCTSTR szSQL, bool bNoEmpty = true)		PURE_VIRTUAL_DECLARATION_DB_0
    virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB
public:
    virtual bool	ExecuteSQL	(char* szSQL)				PURE_VIRTUAL_DECLARATION_DB_0
    virtual int		GetInsertId	()						PURE_VIRTUAL_DECLARATION_DB_0
    virtual int		CountRecord	(const char* szSQL)		PURE_VIRTUAL_DECLARATION_DB_0
};

const int		NEWKEY_NONE	= (DWORD) - 1;
class	IRecord
{
protected:
    virtual ~IRecord() {}
public:
    virtual IRecord*	CreateNewRecord(DWORD idNewKey = NEWKEY_NONE)	PURE_VIRTUAL_DECLARATION_DB_0
    virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB

    // 注：目前只有CMyRecord支持在InsertRecord()后继续UpdateRecord()。
    virtual DWORD	InsertRecord()								PURE_VIRTUAL_DECLARATION_DB_0
    virtual bool	UpdateRecord()								PURE_VIRTUAL_DECLARATION_DB_0
    virtual bool	DeleteRecord()								PURE_VIRTUAL_DECLARATION_DB_0

    virtual int		LoadInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
    virtual void	SetInt		(LPCTSTR szField, int nData)		PURE_VIRTUAL_DECLARATION_DB
    virtual UINT	LoadUInt	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
    virtual void	SetUInt		(LPCTSTR szField, UINT nData)		PURE_VIRTUAL_DECLARATION_DB
    virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize)		PURE_VIRTUAL_DECLARATION_DB
    virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
    virtual DWORD	LoadDWord	(LPCTSTR szField)					{ return LoadUInt(szField); }
    virtual void	SetDWord	(LPCTSTR szField, DWORD nData)		{ SetUInt(szField, nData); }
    // 注：暂时还需要遵守“先取后写”的规则，待具体实现改变后可不按此规则。

public:
    // 新模式的接口，用于代替应用程序的DATA层。
    virtual int		GetInt		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
    virtual LPCTSTR	GetStr		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
    virtual void	SetInt		(int nIdx, int nData)				PURE_VIRTUAL_DECLARATION_DB
    virtual void	SetStr		(int nIdx, LPCTSTR szData, int nSize)			PURE_VIRTUAL_DECLARATION_DB
    virtual void	ClearUpdateFlags	()							PURE_VIRTUAL_DECLARATION_DB

    virtual bool	AddInt(LPCTSTR szField, int nData)			PURE_VIRTUAL_DECLARATION_DB_0		// 立即写库。如无共享冲突，可不提供该接口
};

class	IRecordset : public IRecord
{
protected:
    virtual ~IRecordset() {}
public:
    virtual IRecord*	CreateNewConstRecord(DWORD idNewKey = NEWKEY_NONE)	PURE_VIRTUAL_DECLARATION_DB_0
    //	virtual IRecord*	GetRecordInterface()	PURE_VIRTUAL_DECLARATION_DB_0		// 取IRecord
    virtual UINT		RecordCount()			PURE_VIRTUAL_DECLARATION_DB_0		// return -1: error
    virtual bool		MoveNext()				PURE_VIRTUAL_DECLARATION_DB_0			// return false: no more data
};

template<class T>
class	IGameData
{
protected:
    virtual ~IGameData() {}
public:
    virtual	void		Release()											PURE_VIRTUAL_DECLARATION_DB
    virtual	DWORD		GetID()												PURE_VIRTUAL_DECLARATION_DB_0
public:
    virtual	int			GetInt(T nDataName)									PURE_VIRTUAL_DECLARATION_DB_0
    virtual	LPCTSTR		GetStr(T nDataName)									PURE_VIRTUAL_DECLARATION_DB_0
    virtual	void		SetInt(T nDataName, int nData)						PURE_VIRTUAL_DECLARATION_DB
    virtual	void		SetStr(T nDataName, LPCTSTR szData, int nSize)		PURE_VIRTUAL_DECLARATION_DB
public:
    virtual	bool		Create(DWORD id, IDatabase* pDb)					PURE_VIRTUAL_DECLARATION_DB_0
    virtual	bool		Create(IRecordset* pRecordset)						PURE_VIRTUAL_DECLARATION_DB_0
    virtual	bool		Create(IRecord* pDefaultRes, DWORD id)				PURE_VIRTUAL_DECLARATION_DB_0
    virtual	bool		Update()											PURE_VIRTUAL_DECLARATION_DB_0
    virtual	DWORD		InsertRecord()										PURE_VIRTUAL_DECLARATION_DB_0
    virtual	bool		DeleteRecord()										PURE_VIRTUAL_DECLARATION_DB_0
    virtual	void		ClearUpdateFlags()									PURE_VIRTUAL_DECLARATION_DB
};


// SetDatabase()、SetTableName()暂不提供接口，暂时直接从对象进行操作。

extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);


/////////////////////////////////////////////////////////////////////////////
// 数据库表名定义
//
#define		_TBL_PREHEAD			"cq_"

#define		_TBL_ACCOUNT			"account"
#define		_TBL_ACTION				_TBL_PREHEAD "action"
#define		_TBL_BONUS				_TBL_PREHEAD "bonus"
#define		_TBL_CARD				_TBL_PREHEAD "card"
#define		_TBL_CARD2				_TBL_PREHEAD "card2"
#define		_TBL_CONFIG				_TBL_PREHEAD "config"
//#define		_TBL_CROSSLOGIN			_TBL_PREHEAD "cross_login"
#define		_TBL_DELUSER			_TBL_PREHEAD "deluser"
#define		_TBL_DROPITEMRULE		_TBL_PREHEAD "dropitemrule"
#define		_TBL_DYNAMAP			_TBL_PREHEAD "dynamap"
#define		_TBL_DYNANPC			_TBL_PREHEAD "dynanpc"
#define		_TBL_ENEMY				_TBL_PREHEAD "enemy"
#define		_TBL_FACT				_TBL_PREHEAD "fact"
//#define		_TBL_FORGEGEM			_TBL_PREHEAD "forge_gem"
#define		_TBL_FRIEND				_TBL_PREHEAD "friend"
#define		_TBL_GENERATOR			_TBL_PREHEAD "generator"
//#define		_TBL_GM_MESSAGE			_TBL_PREHEAD "gm_message"
#define		_TBL_GOODS				_TBL_PREHEAD "goods"
#define		_TBL_ITEM				_TBL_PREHEAD "item"
#define		_TBL_ITEMTYPE			_TBL_PREHEAD "itemtype"
#define		_TBL_ITEMADDITION		_TBL_PREHEAD "itemaddition"
#define		_TBL_LEAVEWORD			_TBL_PREHEAD "leaveword"
#define		_TBL_LEVEXP				_TBL_PREHEAD "levexp"
#define		_TBL_LOG				_TBL_PREHEAD "log"
#define		_TBL_MAGIC				_TBL_PREHEAD "magic"
#define		_TBL_MAGICTYPE			_TBL_PREHEAD "magictype"
#define		_TBL_MAP				_TBL_PREHEAD "map"
#define		_TBL_MERCENARYTASK		_TBL_PREHEAD "mercenary_task"
#define     _TBL_ANNOUNCE           _TBL_PREHEAD "announce"
//#define		_TBL_MISSION_ITEM		_TBL_PREHEAD "mission_item"
#define		_TBL_MONSTERTYPE		_TBL_PREHEAD "monstertype"
#define		_TBL_NPC				_TBL_PREHEAD "npc"
#define		_TBL_PASSWAY			_TBL_PREHEAD "passway"
#define		_TBL_PET				_TBL_PREHEAD "pet"
#define		_TBL_EUDEMON			_TBL_PREHEAD "eudemon"
//#define		_TBL_PK					_TBL_PREHEAD "pk"
#define		_TBL_POINTALLOT			_TBL_PREHEAD "point_allot"
#define		_TBL_PORTAL				_TBL_PREHEAD "portal"
#define		_TBL_REBIRTH			_TBL_PREHEAD "rebirth"
#define		_TBL_REGION				_TBL_PREHEAD "region"
#define		_TBL_RULE				_TBL_PREHEAD "rule"
#define		_TBL_SUPERMAN			_TBL_PREHEAD "superman"
#define		_TBL_SYNATTR			_TBL_PREHEAD "synattr"
#define		_TBL_SYNDICATE			_TBL_PREHEAD "syndicate"
#define		_TBL_TABLE				_TBL_PREHEAD "table"
#define		_TBL_TASK				_TBL_PREHEAD "task"
//#define		_TBL_TEACHER			_TBL_PREHEAD "teacher"
#define		_TBL_TRACK				_TBL_PREHEAD "track"
#define		_TBL_TRAP				_TBL_PREHEAD "trap"
#define		_TBL_TRAPTYPE			_TBL_PREHEAD "traptype"
#define		_TBL_TUTOR				_TBL_PREHEAD "tutor"
#define     _TBL_AUCTION            _TBL_PREHEAD  "auction"
#define     _TBL_AUCTION_SYSTEM     _TBL_PREHEAD "auction_system_item"
#define		_TBL_TUTOREXP			_TBL_PREHEAD "tutorexp"
#define		_TBL_USER				_TBL_PREHEAD "user"
#define		_TBL_WANTED				_TBL_PREHEAD "wanted"
#define		_TBL_WEAPONSKILL		_TBL_PREHEAD "weapon_skill"
#define		_TBL_FEE				_TBL_PREHEAD "fee"
//#define		_TBL_LOGON				_TBL_PREHEAD "logon"
//#define		_TBL_SERIAL				_TBL_PREHEAD "serial"
#define		_TBL_TASKDETAIL			_TBL_PREHEAD "taskdetail"

/////////////////////////////////////////////////////////////////////////////


#endif // I_MYDB_H
