
//**********************************************************
// 代码编辑器
//**********************************************************


#ifndef  _MYDB_
#define  _MYDB_

#include "mysql.h"
#include "I_mydb.h"

//------ for bcb use ------
#ifdef _BCB
typedef __int64 _int64;
#endif
//--------------------------

#include "mydatabase.h"
#include "myfield.h"
#include "myrecordset.h"
#include "myrecord.h"
#include "MyConstRecord.h"

const int			INT_NONE	= 0;			// 非法的整数返回值
const LPCTSTR		STR_NONE	= "";			// 非法的串返回值

static const char	DEFALT_SQL_STMT[]			=	"SELECT * FROM ";
static const char	SQL_STMT_SHOWDATABASES[]	=	"SHOW  DATABASES";
static const char	SQL_STMT_SHOWTABLES[]		=	"SHOW  TABLES";

static const char	SQL_STMT_DELETE[]			=	"DELETE FROM %s %s WHERE %s";
static const char	SQL_STMT_UPDATE[]			=	"UPDATE %s SET %s WHERE %s LIMIT 1";	//paled add "LIMIT 1"
static const char	SQL_STMT_INSERT[]			=	"INSERT %s SET %s";		//paled - 丢弃了一个“条件”参数

static const DWORD	_MAX_DBACCESSTIME			=	10000;

extern CMyDatabase		g_db;

#endif
