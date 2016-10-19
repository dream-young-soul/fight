
//**********************************************************
// 代码编辑器
//**********************************************************

// 诊断程序
// 仙剑修, 2001.10.10

#ifndef	LOGFILE_H
#define LOGFILE_H

#define		BUFSIZE		1024

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "time.h"

////////////////////////////////////////////////////////////////////////////////////
// 使用说明

// 使用前需要调用 InitLog()

////////////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#define PROFILE		//? 该宏定义为剖面系统开关，通常在RELEASE版中也可能会打开剖面系统
#endif

void InitLog(const char* pTitle, const char* pFilename, time_t tStart);
void log_Save(const char* pFilename, const char* pBuf);
void LOSEDATA(const char* sFormat, ...);



/*
#define SAVELOG(x)		log_SaveLog(x)
#define SAVELOG1(x,y)	log_SaveLog(x,(y))
#define SAVELOG2(x,y,z)	log_SaveLog(x,(y),(z))
//*/

//void	log_SaveLog(const char * sFormat, ...);
//void	InitProfile();

#define log_InlineLog(x,y)\
inline void	log_InlineLog##y(const char * sFormat, ...)\
{\
	va_list argptr;\
	va_start( argptr, sFormat);     /* Initialize variable arguments. */\
\
	time_t ltime;\
	time( &ltime );\
	tm *	pTm = localtime(&ltime);\
	char	buf[4096] = "";					/* ?????空间可能不足 */\
	sprintf(buf, "%02d:%02d:%02d " #y ": ", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);\
	int		ret = vsprintf(buf + strlen(buf), sFormat, argptr);\
	strcpy(buf + strlen(buf), "\n");\
	if (!(ret < 4096-2)) log_Save(x, "ASSERT(ret < 4096-2)");	/* ?????空间可能不足 */\
\
	va_end( argptr );              /* Reset variable arguments.      */\
\
	log_Save(x,buf);\
}

//#else	// PROFILE

//#define SAVELOG(x)
//#define SAVELOG1(x,y)
//#define SAVELOG2(x,y,z)

//#define log_InlineLog(x) inline void	log_InlineLog##x(const char * sFormat, ...) { }


/////////////////////////////////////////////////////////////////
extern char	log_szFilename[BUFSIZE];		// 内部使用

log_InlineLog(log_szFilename, WARNING)
log_InlineLog(log_szFilename, Message)
log_InlineLog(log_szFilename, ERROR)
log_InlineLog(log_szFilename, CATCH_ERROR)
//log_InlineLog("server", server)
//log_InlineLog("account", account)
//log_InlineLog(log_szFilename, Other)
log_InlineLog("gmlog\\event", event)
log_InlineLog("gmlog\\card", card)
log_InlineLog("syslog\\dump", DUMP)
log_InlineLog("syslog\\cheat", CHEAT)
inline void log_InlineLogNULL(const char* sFormat, ...) {}

#define LOGWARNING		log_InlineLogWARNING
#define LOGMSG			log_InlineLogMessage
#define LOGERROR		log_InlineLogERROR
#define LOGCATCH		log_InlineLogCATCH_ERROR
//#define LOGSERVER		log_InlineLogserver
//#define LOGACCOUNT		log_InlineLogaccount
//#define LogSave			log_InlineLogOther
#define LOGEVENT		log_InlineLogevent
#define LOGCARD			log_InlineLogcard
#define LOGCHEAT		log_InlineLogCHEAT
#ifdef	DUMP_DEBUG_MSG
#define LOGDUMP			log_InlineLogDUMP
#else
#define LOGDUMP			log_InlineLogNULL
#endif

/////////////////////////////////////////////////////////
#ifndef	PROFILE
#undef	log_InlineLog
#define log_InlineLog(x,y) inline void	log_InlineLog##y(const char * sFormat, ...) { }
#endif	// PROFILE
log_InlineLog(log_szFilename, Profile)
#define LOGPROFILE			log_InlineLogProfile

/////////////////////////////////////////////////////////
#include <windows.h>

class CLogFile
{
public:
    CLogFile(LPCTSTR szFile, LPCTSTR szTitle = NULL, int nMaxSize = 500 * 1000 * 1000);
public:
    void SaveLog(LPCTSTR szHead, LPCTSTR szFormat, ...);
    void SaveRaw(LPCTSTR szLog);
protected:
    long			m_tLastTimeStamp;

    //-------------------------------------
protected:
    static long GetTick()	{ return InterlockedIncrement(&m_nLogTick); }
private: // use by function, need mutex.
    static long	m_nLogTick;
};



#endif	// LOGFILE_H
