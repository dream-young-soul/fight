// 设计说明
// 仙剑修，2002.10.23
///////////////////////////////////////////////////////////////////////

核心代码请在核心工程的“TODO”部分添加。

工程模块：
	MsgServer
		SocketKernel
		WorldKernel
		MapGroupKernel
		GameBaseCode(该工程从属于以上三个工程)
		Comm
		Common(工具库，从属于所有工程)
		MapData


线程模块：
	shell
	socket
	world
	*map_group


登录过程：
。玩家登录时先在WORLD_KERNEL进行网络消息处理。
。MSG_LOGIN时，将玩家转到MAPGROUP_KERNEL。
。退出时过程相反。


同步处理：
。玩家从一个核心转到另一个核心时，客户端要等待服务器回应后才能再上传消息，否则服务器可能会丢失该消息。


NPC消息广播：
。TARGET_NPC_ID指定为BROADCAST_NPC_ID(可直接为ID_NONE)，这时只需要向NPC服务器发送一个消息，由NPC服务器自己
决定转发给哪几个NPC。


玩家进入步骤：
。建立连接，玩家属世界核心。
。登录世界核心，转到地图组。
。登录地图组。


玩家转地图组：
。玩家登出地图组。
。通知世界和网络玩家切换。
。向另一地图组发送玩家数据。
。通知另一地图组让玩家登录。


玩家退出步骤：
。从核心开始退出：
	。WORLD或者MAPGROUP调用CloseSocket接口，玩家LOGOUT，同时发给SOCKET核心 SOCKET_BREAKCONNECT。SOCKET核心断开SOCKET(或者按1-1发消息)。执行“从网络层退出”。
。从网络层开始退出：
	。SOCKET核心的行为：
		1-1。发送给最后一个MAPGROUP核心KERNEL_CLOSEKERNEL消息。
		1-6。接收到MAPGROUP的SETPROCESSID消息时，回复KERNEL_CLOSEKERNEL。
		1-7。接收到SOCKET_CLOSEKERNEL时，完成退出操作!
	。WORLD核心的行为：
		2-1。KERNEL_CLOSEKERNEL：玩家退出WORLD，转发给SOCKET核心SOCKET_CLOSESOCKET。
	。MAPGROUP核心的行为：
		3-1。KERNEL_CLOSEKERNEL：如果不在本MAPGROUP，转发给前一个线程(MAPGROUP或WORLD) KERNEL_CLOSEKERNEL消息。
		3-2。KERNEL_CLOSEKERNEL：玩家从MAPGROUP核心退出时，直接通知WORLD核心KERNEL_CLOSEKERNEL消息。
		3-6。如果玩家正在从WORLD到MAPGROUP，或者从MAPGROUP到MAPGROUP。则玩家到新MAPGROUP时，会调用SETPROCESSID，这时SOCKET会回复KERNEL_CLOSEKERNEL(即再次发此消息)。这时，也能正常退出。


帮派：
。CSynData：数据层，所有成员变量使用private，防止CSyndicate类存取。所有外部接口必须是原子的。
	。QuerySynchro()：内部接口。修改自己的状态。

。CSyndicate：从CSynData派生。只提供只读接口。
	。QueryChange()：提供修改的接口。

。CSynShell：提供给地图组对象的代理程序。
	。QueryChange()：将修改消息转发到主对象。
	。QuerySynchro()：调用基类接口。

。CSynWorldShell：提供给地图组对象的代理程序。
	。QueryChange()：调用基类接口。
	。QuerySynchro()：内部接口。修改自己的状态，同时广播给所有MapGroup。

。CSynManager：
	。创建新帮派：向WORLD申请，完成后向所有MAPGROUP广播。由申请的MAPGROUP创建数据库记录。
	。解散帮派：向WORLD申请，完成后向所有MAPGROUP广播。都可以删除数据库记录。

。CSynWorldManager：
	。新建帮派。
	。删除帮派。


登录消息流程：
	。客户端先登录到帐号服务器，上传CMsgAccount登录帐号服务器。帐号服务器会返回给客户端XXX消息。
	。帐号服务器会向游戏服务器下传CMsgConnect_A
	。同时客户端会连接SOCKET，并上传CMsgConnect
	。世界核心收到CMsgConnect_A和CMsgConnect后，向客户端返回CTalk的_TXTATR_ENTRANCE频道消息，同时向地图组发MAPGROUP_LOGIN内部消息，让玩家登录地图组。
	。

