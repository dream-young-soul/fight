
//**********************************************************
// 代码编辑器
//**********************************************************

// 外壳接口文件
// 仙剑修，2002.12.13

#ifndef	INTERFACE_SHELL_HEAD_FILE
#define	INTERFACE_SHELL_HEAD_FILE

/////////////////////////////////////////////////////////////////////////////
const int STAT_MAX_MAPGROUP	= 5;			//??? @@@@可能地图组数量不足，只显示这么多
struct STAT_STRUCT
{
    char	szStartServer[20];
    char	szCurr[20];

    long	nMaxSocketMS;
    long	nAllSocketMS;
    long	nSocketTimes;
    long	nMaxWorldMS;
    long	nAllWorldMS;
    long	nWorldTimes;
    long	setMaxMapGroupMS[STAT_MAX_MAPGROUP];
    long	setAllMapGroupMS[STAT_MAX_MAPGROUP];
    long	setMapGroupTimes[STAT_MAX_MAPGROUP];
    long	setMapGroupTimerMS[STAT_MAX_MAPGROUP];

    // kernel
    long	nMaxPlayers;
    long	nAllPlayers;
    long	nLoginPlayers;
    long	nLogoutPlayers;

    long	nSocketBytes;
    long	nSocketPackets;
    long	nNpcSocketBytes;
    long	nNpcSocketPackets;

    long	nPacketID;

    // debug
    long	setDebug[5];
};

/////////////////////////////////////////////////////////////////////////////
class IMessagePort;
class ISocketKernel
{
public:
    virtual ~ISocketKernel() {}
    static ISocketKernel* CreateNew();
    virtual bool	Create(IMessagePort* pPort)				PURE_VIRTUAL_FUNCTION_0
    virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	ProcessSocket()							PURE_VIRTUAL_FUNCTION_0
    virtual bool	OnTimer(time_t tCurr)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	Release()								PURE_VIRTUAL_FUNCTION_0
};

class IMessagePort;
class IWorld
{
public:
    virtual ~IWorld() {}
    static IWorld* CreateNew();
    virtual bool	Create(IMessagePort* pPort)				PURE_VIRTUAL_FUNCTION_0
    virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	OnTimer(time_t tCurr)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	Release()								PURE_VIRTUAL_FUNCTION_0
};

class IMessagePort;
class IMapGroup
{
public:
    virtual ~IMapGroup() {}
    static IMapGroup* CreateNew();
    virtual bool	Create(IMessagePort* pPort)				PURE_VIRTUAL_FUNCTION_0
    virtual void	SynchroData()							PURE_VIRTUAL_FUNCTION
    virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)	PURE_VIRTUAL_FUNCTION_0
    virtual bool	OnTimer(time_t tCurr)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	Release()								PURE_VIRTUAL_FUNCTION_0
};

#endif // INTERFACE_SHELL_HEAD_FILE
