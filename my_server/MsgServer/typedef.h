
//**********************************************************
// 代码编辑器
//**********************************************************

// typedef.h
// 仙剑修，2002.10.23
//////////////////////////////////////////////////////////////////////

#ifndef	TYPEDEF_H
#define	TYPEDEF_H

#include "define.h"

#define	BETA		// 内部测试版本

#define	GAME_TITLE			"消息服务器"


//////////////////////////////////////////////////////////////////////
// 控制变量
const int	THREADDELAY_MS			= 400;			// 等待内部消息的最大时间
const int	MAPGROUP_ONTIMER_MS		= 500;			// 地图组线程的ONTIMER频率
const int	SOCKETKERNEL_ONTIMER_MS	= 1000;			// 网络线程的ONTIMER频率
const int	WORLDKERNEL_ONTIMER_MS	= 1000;			// 世界线程的ONTIMER频率
const int	TIMER_OFFSET_LIMIT		= 10;			// TIMER时钟误差，大于该值时会自动纠正
const int	SOCKET_PROCESS_INTERVAL	= 40;	//100;			// SOCKET轮循的频率
const int	CLOSETHREAD_MILLISECS	= 30000;			// 等待线程关闭的毫秒数


//////////////////////////////////////////////////////////////////////
// 界面常量
const int	TEXTWINDOW_SIZE		= 8;			// 消息窗行数

#define		LOGFILE_DIR				"syslog"
#define		LOGFILE_FILENAME		"syslog\\SZ_SERVER"
#define		ONLINE_FILENAME			"Game server online.txt"


//////////////////////////////////////////////////////////////////////
struct ST_CONFIG
{
    int		MAPGROUP_SIZE;					// 地图组数量
    int		CURRENT_PORTID;
    int		PORT_SIZE;
    char	MASTER_IP[IPSTR_SIZE];
    int		MASTER_PORT;
    char	LOGIN_KEY[256];
};

extern ST_CONFIG	CONFIG;



#endif // TYPEDEF_H
