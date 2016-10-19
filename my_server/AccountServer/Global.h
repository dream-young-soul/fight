
//**********************************************************
// 代码编辑器
//**********************************************************

// 全局变量
// 仙剑修，2001.11.19

#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
using namespace std;

extern HANDLE	g_xDatabase;

//////////////////////////////////////////////////////////////////
// 用于统计
extern long	s_nSocketCount;
extern long	s_nLoginCount;
extern long	s_nMaxServerTime;
extern long	s_nAvgServerTime;
extern long	s_nPointCount;
extern long s_nLoginAccept;
extern long s_nPointFee;
extern long s_nPointSum;
extern long s_nAllTimeSum;
extern long s_nDatabaseTimeSum;

void	PrintText(const char* szFormat, ...);
bool	LockedGetText(char* buf);

enum	{ c_flagNone = '.', c_flagOffline = ',', c_flagSocket = ':', c_flagAccount = '-', c_flagStop = '#', c_flagNormal = '=', c_flagNormal91U = 'U',
          c_flagBaseMax = '@',					// 区分基本符号和临时符号的分界 ==============================================
          c_flagBegin = 'S', c_flagPoint = 'Y', c_flagEnd = 'X', c_flagHeartbeat = '?',
          c_flagBegin2 = 's', c_flagPoint2 = 'y', c_flagEnd2 = 'x'
     };
void	SetServerState(int nServer, int nState);
void	GetServerState(char* buf);
//////////////////////////////////////////////////////////////////

extern	time_t	g_tStartServerTime;				// log need

//////////////////////////////////////////////////////////////////
// IPS列表常量
typedef	char	NAMESTR[_MAX_NAMESIZE];
struct	ISP_ST
{
    NAMESTR		szName;
    NAMESTR		szFirstIP;
    NAMESTR		szLastIP;
    NAMESTR		szAccount;
    NAMESTR		szPassword;
};
typedef vector<ISP_ST>		ISP_SET;

//////////////////////////////////////////////////////////////////
// 全局对象

extern COnlineTable* 	g_pOnlineTable;		// 在线玩家表
extern CPointThread* 	g_pPointThread;		// (POINTLISTENPORT);
extern CLoginThread* 	g_pLoginThread;		// (LOGINLISTENPORT);
extern CAccount			g_cDatabase;
extern bool				g_bEnableLogin;

extern CServerAccount	g_aServerAccount[];		// 游戏服务器列表
extern int				g_nServerAccount;		// 游戏服务器数量
extern char				g_szCountHeadLine[];	// 游戏服务器列表的标题行
extern ISP_SET			g_setISPList;			// IPS的IP地址列表


//////////////////////////////////////////////////////////////////
// 全局函数

DWORD	NewAuthenID(unsigned long nClientIP);
bool	SafeCopy(char* pTarget, const char* pSource, int nBufLen = 0);		// return false: 非法。或源串超长(成功拷贝部分串)
bool	FormatStr(char* pszBuf, int nBufLen = 0);

//////////////////////////////////////////////////////////////////
enum
{
    c_errPassword		= 1,		// 帐号名或者密码错
    c_errNotPoint		= 6,		// 计点帐号，没点了
    c_errNotTime		= 7, 		// 包月帐号，过期了
    c_errUnknowServer	= 10,		// GAME服务器名不存在
    c_errOnline			= 11,		// 已经有另一玩家登录了游戏(可再次登录)
    c_errBan			= 12, 		// 已被封号
    c_errBarPassword	= 13, 		// 网吧登录模式，帐号名或者密码错
    c_errBarOverflow	= 14, 		// 网吧登录模式，超出了许可登录人数
    c_errServerBusy		= 20,		// 服务器忙
    c_errServerFull		= 21,		// 服务器已满
    c_errUnknown		= 999		// 未知
};

bool	GetAccount(LPCTSTR szAccount, LPCTSTR szPassword,
                   OBJID& idAccount, int& nType, int& nPoint, DWORD& nPointTime, int& nBlock);			// 返回值
bool	IsFreeServer(LPCTSTR szServer);
bool	CheckISP(OBJID idAccount, unsigned long nClientIP, LPCTSTR pClientIP,
                 OBJID& idFeeAccount, char* szFeeAccount, int& nPoint, DWORD& nPointTime, char* szNotifyMsg);	// 返回值
bool	Check7DaysNodify(DWORD nPointTime, char* szNotifyMsg);		// return szNotifyMsg





#endif // GLOBAL_H
