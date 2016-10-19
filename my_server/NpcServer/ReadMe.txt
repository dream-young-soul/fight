// NPC服务器单线程版
// 仙剑修，2002.12.2


核心对象管理关系：
	全局对象：CNpcWorld
		CMapManager
			CMap
				CMapItem
				CGameMap(与客户端同步的地图数据对象)
				(*)CNpc
				(*)CUser
		CNpcManager
			CNpc
		CUserManager
			CUser
注：(*)部分为refrence


核心全局接口：
	NpcWorld(), 
	MapManager(), 
	NpcManager(), 
	UserManager()


线程：
	外壳线程
	核心线程


线程间接口：
	(参见protocol.h)




