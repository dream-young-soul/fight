
//**********************************************************
// 代码编辑器
//**********************************************************

// NPC服务器单线程版
// 仙剑修，2002.12.2



#ifndef	TYPEDEF_HEADFILE_H
#define	TYPEDEF_HEADFILE_H

#include <limits.h>

/////////////////////////////////////////////////////////////////////////////////////
#define	NPCSERVER_TITLE				"NPC服务器"

#define	LOGFILE_DIRECTION			"syslog"
#define	LOGFILE_FILENAME			"syslog\\NpcServer"

/////////////////////////////////////////////////////////////////////////////////////
// DIALOG控制常量
const int	MAX_TEXTLINESIZE		= 500;
const DWORD	END_OF_TEXT				= INT_MAX;
const int	SAVESTATEWIN_SECS		= 5 * 60;				// 每5分钟更新一次界面
const int	UPDATESTATEWIN_SECS		= 10;				// 每5分钟更新一次界面



const int	CONNECT_DELAY_MS		= 900;
const int	SYSTEM_ONTIMER_MS		= 500;		// ONTIMER间隔
const int	TIMER_OFFSET_LIMIT		= 10;		//



/////////////////////////////////////////////////////////////////////////////////////
// 核心定义
/////////////////////////////////////////////////////////////////////////////////////
typedef		unsigned long			OBJID;
const OBJID	ID_NONE					= 0;

const int	IPSTR_SIZE				= 16;
typedef		char					IPSTR[IPSTR_SIZE];

const int	MAX_PACKETSIZE			= 1024;			// 网络消息包尺寸



#endif // TYPEDEF_HEADFILE_H
