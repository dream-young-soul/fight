
//**********************************************************
// 代码编辑器
//**********************************************************

// action list
// paled, 2003.1.25

////////////////////////////////////////////////////////////////////////
// "%%" 表示可显示的 "%" 符号
// "%nnXXX" 表示显示宽度，一律左对齐。超长会按实际长度显示。(例id为1234的NPC的"%6id"显示为"1234  ")

// 系统部分 //////////////////
const char PARAM_DATE_STAMP[]			= "date_stamp";					// 仅用于计算
const char PARAM_TIME[]					= "time";						// 仅用于计算
const char ACCEPT_STR_[]				= "accept";						// 客户端上传的一个子串 // %accept0 ~ %accept3

// ITEM部分 //////////////////
const char	PARA_ITEM_TYPE[]			= "item_type";
const char	PARA_ITEM_DATA[]			= "item_data";

// NPC部分 //////////////////
const char	PARA_DATASTR[]				= "datastr";
const char	PARA_DATA_[]				= "data";							// %data0 ~ %data3
const char	PARA_NAME[]					= "name";							// npc name
const char	PARA_NPC_ID[]				= "id";								// npc id in database
const char	PARA_NPC_X[]				= "npc_x";
const char	PARA_NPC_Y[]				= "npc_y";
const char	PARA_NPC_OWNERID[]			= "npc_ownerid";

// USER部分 ////////////////
const char	PARA_USER_ID[]				= "user_id";
const char	PARA_USER_MAPID[]			= "user_map_id";
const char	PARA_USER_MAPX[]			= "user_map_x";
const char	PARA_USER_MAPY[]			= "user_map_y";
const char	PARA_USER_HOME[]			= "user_home_id";
const char	PARA_SYN_ID[]				= "syn_id";
const char	PARA_SYN_NAME[]				= "syn_name";
const char	PARA_USER_NAME[]			= "user_name";
const char	PARA_MATE_NAME[]			= "mate_name";
const char	PARA_MAP_OWNER_ID[]			= "map_owner_id";
const char	PARA_MAP_OWNER_TYPE[]		= "map_owner_type";
const char	PARA_ALLY_SYN_[]			= "ally_syn";						// %ally_syn0 ~ %ally_syn4
const char	PARA_ENEMY_SYN_[]			= "enemy_syn";						// %enemy_syn0 ~ %enemy_syn4
const char	PARA_TUTOREXP_[]			= "tutor_exp";						// 导师经验
const char	PARA_STUDENT_EXP_[]			= "student_exp";					// 徒弟贡献的经验
const char	PARA_EXPLOIT_[]				= "exploit";						// 功勋值
//帮派部分
const char	PARA_AVAILABLE_FUND[]		= "available_fund";					// 提取帮派的可以分配基金
// user 的 iter 部分 ////////////////
const char	PARA_ITER_VALUE[]			= "iter_value";
const char	PARA_ITER_SYN_NAME[]		= "iter_syn_name";
const char	PARA_ITER_SYN_LEADER[]		= "iter_syn_leader";
const char	PARA_ITER_SYN_MONEY[]		= "iter_syn_money";
const char	PARA_ITER_SYN_AMOUNT[]		= "iter_syn_amount";
const char	PARA_ITER_SYN_FEALTY[]		= "iter_syn_fealty";
const char	PARA_ITER_MEMBER_NAME[]		= "iter_member_name";				// %iter为玩家ID，返回玩家名字
const char	PARA_ITER_MEMBER_RANK[]		= "iter_member_rank";				// %iter为玩家ID，返回玩家等级称号
const char	PARA_ITER_MEMBER_PROFFER[]	= "iter_member_proffer";
const char	PARA_ITER_WANTED[]			= "iter_wanted";
const char	PARA_ITER_POLICEWANTED[]	= "iter_police_wanted";
const char	PARA_ITER_GEMSUPQUALITY[]	= "iter_upquality_gem";
const char	PARA_ITER_GEMSUPLEVEL[]		= "iter_uplevel_gem";
const char	PARA_ITER_COST_DURRECOVER[]	= "iter_cost_durrecover";			//
const char	PARA_ITER_GAME_CARD[]		= "iter_game_card";					//
const char	PARA_ITER_GAME_CARD2[]		= "iter_game_card2";				//

const char	PARA_ITER_TABLE_DATASTR[]	= "iter_table_datastr";
const char	PARA_ITER_TABLE_DATA_[]		= "iter_table_data";				// %iter_table_data0 ~ %iter_table_data3

const char	PARA_ITER_ITEM_DATA[]		= "iter_item_data";

//---任务系统的iter---begin
const char	PARA_ITER_TASK_USERNAME[]		= "iter_task_username";		//返回玩家的cq_user表中的name字段
const char	PARA_ITER_TASK_COMPLETENUM[]	= "iter_task_completenum";  //对任务完成次数取值，返回任务完成次数
const char	PARA_ITER_TASK_BEGINTIME[]		= "iter_task_begintime";    //对任务开始时间取值，返回任务开始时间
//---任务系统的iter---end

////////////////////////////////////////////////////////////////////////
enum
{
    // system part, need user ptr --------------------------------------------------------------------------
    ACTION_SYS_FIRST			= 100,
    ACTION_MENUTEXT				= 101,			// 菜单文本。data:显示行数(缺省为0), param="text"(可选): 显示文本，此类型可包含空格，也可为空行。
    ACTION_MENULINK				= 102,			// 菜单超链接。"text task_id align": align(可选): 对齐模式(缺省/0:左对齐; 5:居中; 9:右对齐; a(1-9)b(2-8):不换行,从a/b行宽处开始显示,9为右对齐)
    ACTION_MENUEDIT				= 103,			// 菜单输入框。"len task_id text": len:可输入的长度；text(可选)：显示的文字，align：对齐模式(缺省：文字在左，正整数：输入框嵌入在文字的第几字符处)。★说明：如有多个输入框，由界面用圆按钮区分，每次只上传一个。
    ACTION_MENUPIC				= 104,			// 菜单图片。"x y pic_id task_id"：task_id(可选)：表示图片可以“按”。图片所在区域不显示文本。
    ACTION_MENUBUTTON			= 110,			// 菜单按钮，格式同超链接。
    ACTION_MENULISTPART			= 111,			// 菜单列表项。"task_id iter text...", 玩家点选列表后，会触发task_id，并且iter会填到玩家的%iter变量中。
    ACTION_MENUCREATE			= 120,			// 菜单创建。"cancel_task_id"(可选): 强行关闭菜单时触发的TASK
    ACTION_RAND					= 121,			// 检测随机率。"data1 data2"。"10 100"表示有1/10的机会是true。
    ACTION_RANDACTION			= 122,			// 随机Action "action0 action1... action7"共8个，随机挑一个执行
    ACTION_CHKTIME				= 123,			// data为时间类型 0-检查当前服务器详细时间 "%d-%d-%d %d:%d %d-%d-%d %d:%d"; 1-检查年某天时间"%d-%d %d:%d %d-%d %d:%d", 2-检查月某天时间"%d %d:%d %d %d:%d", 3-检查周某天时间"%d %d:%d %d %d:%d", 4-检查日时间"%d:%d %d:%d", 5-检查小时时间"%d %d"(每个小时的第几分到底几分)
    ACTION_POSTCMD				= 124,			// 向客户端发送界面命令，data为命令编号
    ACTION_BROCASTMSG			= 125,			// 全服务器广播文字消息，data为频道，para为内容
    ACTION_MESSAGEBOX			= 126,			// 让客户端弹出对话框。data为对话框类型(0-99)，param=文字串参数。
    ACTION_SYS_LIMIT			= 199,

    // npc part, need npc ptr --------------------------------------------------------------------------
    ACTION_NPC_FIRST			= 200,
    ACTION_NPC_ATTR				= 201,			// 修改或检测任务NPC的属性。"attr opt data npc_id"，至少3个参数。如果指定npc_id，该NPC必须在本地图组中。attr可选择"ownerid"(=,==)、"ownertype"(=,==)、"lookface"(=,==)、"data?"(=,+=,==,<,<=,>,>=,pass. pass可让字段中存的DateStamp增减一定的天数，. data可为"accept")、"datastr"(=,==)、"life"(=)、"maxlife"(=)
    //		ACTION_NPC_REQUESTSHIFT		= 202,			// 通知客户端平移一个NPC。param="idNextTask"。
    //		ACTION_NPC_SHIFT			= 203,			// 平移当前NPC。限同地图移动。限动态NPC。
    ACTION_NPC_ERASE			= 205,			// 删除当前NPC。限动态NPC。注意：删除后，不要再对此NPC进行操作。dwData不为0，表示删除本地图的所有type为dwData的NPC。param="idMap type": 删除指定地图的某些NPC。
    ACTION_NPC_MODIFY			= 206,			// 跨线程修改指定任务NPC的属性。"npc_id attr opt data"。attr可选择"lookface"(=)、"data?"(=)、"datastr"(=)
    ACTION_NPC_RESETSYNOWNER	= 207,			// 重置帮派地图的主人。仅用于帮派标志NPC。统计帮派战绩第一名填入OWNER_ID，同时清除所有战绩。所有攻击自动停止。(未中止地图的战斗标志)
    ACTION_NPC_FIND_NEXT_TABLE	= 208,			// 查找下一个列表项，将ID写入TASK_ITERATOR。仅用于带列表的NPC。param="type", 对应cq_table的type字段。
    ACTION_NPC_ADD_TABLE		= 209,			// 在列表中添加一条, type和idKey相同的会被预先删除(idKey为0时，不删除)。仅用于带列表的NPC。param="type idKey data0 data1 data2 data3 szData", 至少2个参数。
    ACTION_NPC_DEL_TABLE		= 210,			// 从列表中删除所有符合条件的项，不存在的也返回true。仅用于带列表的NPC。param="type idKey data0 data1 data2 data3 szData", 至少2个参数, 参数为0表示不匹配，所有不为0的与列表完全相同的会删除。无param时，删除当前记录(iterator指定的记录)，总是返回true。
    ACTION_NPC_DEL_INVALID		= 211,			// 从列表中删除所有过期的项，不存在的也返回true。仅用于带列表的NPC。param="type idx", idx表示日期(%date_stamp)存放在哪个data中。[例如idx为3表示日期存放在data3中，所有data3中的日期小于今天日期的项将被删除。]
    ACTION_NPC_TABLE_AMOUNT		= 212,			// 检查列表的项数，>=data时返回false，<data时返回true。param无意义。
    ACTION_NPC_SYS_AUCTION      = 213,          // LW让NPC开始系统拍买,DATA为NPC的ID,param 为系统正式开始的提示
    ACTION_NPC_DRESS_SYNCLOTHING = 214,			//穿帮派服装
    ACTION_NPC_TAKEOFF_SYNCLOTHING = 215,		//脱帮派服装
    ACTION_NPC_AUCTIONING        = 216,         //判断是否还有物品正在拍卖中DATA为NPC的ID，PARAM：type：0.表示查看系统拍卖物品，1.表示查看玩家物品
    ACTION_NPC_LIMIT			= 299,

    // map part, need curr map --------------------------------------------------------------------------
    ACTION_MAP_FIRST			= 300,
    ACTION_MAP_MOVENPC			= 301,			// 把npc移动到指定的地图、位置(仅用于固定NPC)，data 为指定的npc的ID, param为 "idMap nPosX nPosY"。注意：可移动到该地图的(0,0)坐标，以便隐藏NPC。
    ACTION_MAP_MAPUSER			= 302,			// 判断指定地图中的用户人数, data 为指定地图ID, param为 "cmd opt data",
    //  cmd支持"map_user"和"alive_user", opt可以为"==, <=, >="，data为数量
    ACTION_MAP_BROCASTMSG		= 303,			// 广播消息, data 为地图id, szParam为广播的消息
    ACTION_MAP_DROPITEM			= 304,			// 地图产生指定物品, szParam为"idItemType idMap nPosX nPosY"
    ACTION_MAP_SETSTATUS		= 305,			// 设置地图状态，支持EVENT。param="mapid status_bit data", status_bit = { STATUS_WAR=1, }, data=0或1。
    ACTION_MAP_ATTRIB			= 306,			// 检查、修改地图的属性。param="field opt data idMap", 参数至少3个，缺省为当前地图。field="synid"(opt="=="、"=")。field="status"(opt="test"、"set"、"reset")。field="type"(opt="test")。field="res_lev"(opt="="、"=="、"<")。field="mapdoc"(opt="="、"==")， portal0_x(=), portal0_y(=),field ="castle"(opt="==")
    ACTION_MAP_REGION_MONSTER	= 307,			// 检查指定地图或当前地图的某区域内怪物数量。param="map_id region_x region_y region_cx region_cy monster_type opt data"。map_id为0时表示取当前地图，monster_type为0表示不检查类型，opt支持“==”和“<”。

    ACTION_MAP_CHANGEWEATHER	= 310,			// 修改玩家所在REGION的天气。param="Type Intensity Dir Color KeepSecs", Type, Intensity=0~999, Dir=0~359, Color=0x00RRGGBB, KeepSecs=seconds
    ACTION_MAP_CHANGELIGHT		= 311,			// 修改玩家地图的亮度。param="idmap light secs", light=0xAARRGGBB(0xFFFFFFFF表示恢复), secs为0 : 表示永久改变
    ACTION_MAP_MAPEFFECT		= 312,			// 在指定地图的指定地点显示地图特效, param = "idMap x y EffectName"
    ACTION_MAP_CREATEMAP		= 313,			// 创建一张地图，链接到当前NPC上(npc必须为LINK_NPC)，需要玩家对象。param="name owner_type owner_id mapdoc type portal_x portal_y reborn_map reborn_portal res_lev"。partal指入口点坐标，res_lev表示地图等级(用于升级)。
    ACTION_MAP_FIREWORKS		= 314,			// 放焰火

    ACTION_MAP_LIMIT			= 399,

    // item action only part, need pUser+pItem ptr --------------------------------------------------------------------------
    ACTION_ITEMONLY_FIRST		= 400,
    ACTION_ITEM_REQUESTLAYNPC	= 401,			// 通知客户端放置一个NPC。param="idNextTask type sort lookface region", 至少4个参数。region表示cq_region的type
    ACTION_ITEM_COUNTNPC		= 402,			// 检查同地图的NPC数量。param="field data opt num", field="name"(按名字)、"type"(按类型)、"all"(所有NPC)、"furniture"(家具), data=要统计的名字或类型(all和furniture填0), opt="<"、"=="。
    ACTION_ITEM_LAYNPC			= 403,			// 创建一个NPC，创建成功后，该NPC即是当前任务NPC，owner_id会自动设置为帮派ID或玩家ID。param="name type sort lookface ownertype  life region base linkid  task0 task0 ... task7  data0 data1 data2 data3 datastr"。至少5个参数。靶子的等级放在data3中。
    ACTION_ITEM_DELTHIS			= 498,			// 删除当前任务物品。注意：必须为最后一个ACTION。
    ACTION_ITEMONLY_LIMIT		= 499,

    // user item part, need user ptr --------------------------------------------------------------------------
    ACTION_ITEM_FIRST			= 500,
    ACTION_ITEM_ADD				= 501,			// 添加物品。data=itemtype_id, param="amount amount_limit ident gem1 gem2 magic1 magic2 magic3 data warghostexp gemtype availabletime", param可省略，所有缺省值为0(表示不修改)
    ACTION_ITEM_DEL				= 502,			// 删除物品。data=itemtype_id, param不为0时，可叠加类物品会同时删除多个。或者data为0，param表示要删除的物品名。
    ACTION_ITEM_CHECK			= 503,			// 检测物品。data=itemtype_id, param不为0时，同时检查物品的数量(或耐久度)，此物品必须达到要求的数量(或耐久度)才行。或者data为0，param表示要找的物品名。
    ACTION_ITEM_HOLE			= 504,			// 武器打洞。param 支持 "ChkHole HoleNum" 或者"MakeHole HoleNum"，Num为1或者2
    ACTION_ITEM_REPAIR			= 505,			// 装备修理。data为指定装备的位置。
    ACTION_ITEM_MULTIDEL		= 506,			// 删除多种物品, param为 "idType0 idType1 num"，即删除num个idType0-idType1的物品。
    ACTION_ITEM_MULTICHK		= 507,			// 检测多种物品, param为 "idType0 idType1 num"，即检测num个idType0-idType1的物品。
    ACTION_ITEM_LEAVESPACE		= 508,			// 检查背包剩余空间。param="space weight packtype"
    // 其中packtype的取值范围为50 ~ 53
    // 50：普通物品背包
    // 51：魔魂宝石背包
    // 52：幻兽蛋背包
    // 53：幻兽背包

    ACTION_ITEM_UPEQUIPMENT		= 509,			// 装备操作, param格式为"cmd position",
    // cmd支持"up_lev", "up_quality", "recover_dur"
    // position 为装备位置, 定义如下
    /*	ITEMPOSITION_HELMET		= 1;
    	ITEMPOSITION_NECKLACE	= 2;
    	ITEMPOSITION_ARMOR		= 3;
    	ITEMPOSITION_WEAPONR	= 4;
    	ITEMPOSITION_WEAPONL	= 5;
    	ITEMPOSITION_RINGR		= 6;
    	ITEMPOSITION_RINGL		= 7;
    	ITEMPOSITION_SHOES		= 8;
    	ITEMPOSITION_MOUNT		= 9*/

    ACTION_ITEM_EQUIPTEST		= 510,			// 物品品质检验,
    // param "equip_pos cmd opt num",
    // equip_pos同上position定义,
    // cmd支持"level", "quality", "durability", "max_dur"
    // opt支持"==, >=, <=",
    // num为数据, cmd为"durability"和"max_dur"时, -1为最大值
    ACTION_ITEM_EQUIPEXIST		= 511,			// 装备存在检验, data为装备位置
    ACTION_ITEM_EQUIPCOLOR		= 512,			// 装备颜色改变, param = "equip_pos color", equip_pos支持如下
    /*	ITEMPOSITION_HELMET		= 1;
    	ITEMPOSITION_ARMOR		= 3;
    	ITEMPOSITION_WEAPONL	= 5; */	// ITEMPOSITION_WEAPONL 必须是盾牌才起作用
    ACTION_ITEM_FIND			= 513,			// 查找一个物品，type存在user的iterator中。data=itemtype_id。或者data为0，param表示要查找的物品名。
    ACTION_ENCASH_CHIP          = 514,          // 用筹码换钱，钱数在Ｉｔｅｍ中的Ｄａｔａ字段
    ACTION_ITEM_LIMIT			= 599,

    // user npc only part, need not user ptr --------------------------------------------------------------------------
    ACTION_NPCONLY_FIRST		= 600,
    ACTION_NPCONLY_CREATENEW_PET = 601,			// 创建一个MONSTER，OWNERID、OWNERTYPE与NPC相同。param="x y generator_id type data name", 至少4个参数, 如有name则改名。generator用于控制怪物活动范围，cq_generator中的type无意义。x、y本地图的绝对坐标。
    ACTION_NPCONLY_DELETE_PET	= 602,			// 删除本地图的所有MONSTER，OWNERID、OWNERTYPE与NPC相同。param="type data name", 至少1个参数, data不为0则同时匹配data, 如有name则同时匹配名字。
    ACTION_NPCONLY_MAGICEFFECT	= 603,			// NPC发一个魔法效果。param="source_id magic_type magic_level target_id data"
    ACTION_NPCONLY_MAGICEFFECT2	= 604,			// NPC发一个地效魔法。param="source_id magic_type magic_level x y target_id data", 至少5个参数。
    ACTION_NPCONLY_LIMIT		= 699,

    // user syndicate part --------------------------------------------------------------------------
    ACTION_SYN_FIRST			= 700,
    ////////////////////////////////////////////////////////
    // 帮派Action整理
    ACTION_SYN_CREATE			= 701,			// 创帮，玩家要输入帮派名字。param="level money leave_money", 三个参数为需要的玩家等级，需要的现金数，帮派成立后所余现金数。
    ACTION_SYN_DESTROY			= 702,			// 解散。执行Action的玩家为团长、分团长、分队长的时候，分别表示解散帮派、分团、分队
    ACTION_SYN_DONATE			= 703,			// 捐款，需要一个输入框。
    ACTION_SYN_CREATE_SUB		= 708,			// 创子帮(军团长执行就是创建分团，分团长执行就是创建分队)，
    // 玩家要输入帮派名字(长度不大于16BYTE)。
    ACTION_SYN_COMBINE_SUB		= 710,			// 合并子帮派。执行Action的玩家是子帮派的帮主，合并到父帮派
    ACTION_SYN_ATTR				= 717,			// 检查和修改帮派属性，参数不小于3个，缺省帮派ID是当前玩家的帮派ID。
    // param="szField szOpt data syn_id", szField可选：
    // 基金："money"	(opt可选"+=", "<"),
    // 声望："repute"	(opt可选"+=", "<"),
    // 人数："membernum"(opt为"<"),
    // 父帮派："fealty"	(opt为"=="),
    // 等级："level"	(opt可选"=", "+=", "<", "==")
    ACTION_SYN_ALLOCATE_SYNFUND	= 729,			// 分配帮派基金。需要玩家数据具体金额(最多不超过总基金的50%)
    ACTION_SYN_RENAME			= 731,			// 重命名帮派。必须是子帮派，由子帮派的帮主执行
    ////////////////////////////////////////////////////////

    ACTION_SYN_DEMISE			= 704,			// 禅让, 只允许军团长禅让，分团长和分队长都不允许。
    // 玩家要输入帮众名字。param= "level"，表示接受禅让的玩家需要的等级
    ACTION_SYN_SET_ASSISTANT	= 705,			// 提升为副帮主, 玩家要输入帮众名字。
    ACTION_SYN_CLEAR_RANK		= 706,			// 解除职务, 玩家要输入帮众名字。
    ACTION_SYN_PRESENT_MONEY	= 707,			// 送钱给其它帮派。帮主要输入金钱数量，其它帮派的ID为TASK_ITERATOR(参见ACTION_SYN_FIND_BY_NAME)。送钱不能少于10000
    ACTION_SYN_CHANGE_LEADER	= 709,			// 更新子帮派的帮主。帮主和子帮派新帮主组队，输入子帮派名字。param= level，要求的新帮主等级
    ACTION_SYN_ANTAGONIZE		= 711,			// 树敌, 玩家要输入帮派名字。
    ACTION_SYN_CLEAR_ANTAGONIZE	= 712,			// 清除树敌，玩家要输入帮派名字。
    ACTION_SYN_ALLY				= 713,			// 结盟，要求两个帮主组队
    ACTION_SYN_CLEAR_ALLY		= 714,			// 解除结盟,玩家要输入帮派名字。
    ACTION_SYN_KICKOUT_MEMBER	= 715,			// 按名字开除帮众，玩家要输入帮众名字。
    ACTION_SYN_CREATENEW_PET	= 716,			// (作废)创建一个帮派守护兽。param="generator_id type data", 至少两个参数, 如有accept则改名。generator用于控制怪物活动范围，cq_generator中的type无意义。
    ACTION_SYN_CHANGESYN		= 718,			// 帮众转堂口。堂口转堂口，堂口和总帮互转。需要帮众和堂主(或帮主)组队，一次转一人。需要输入要转入的堂口名字(或帮派名字)。转过去之前职位会被自动取消。
    ACTION_SYN_CHANGE_SUBNAME	= 719,			// 修改堂口名，只限于名字超过6个字节的堂口。否则返回FALSE。(临时功能)

    ACTION_SYN_FIND_NEXT_SYN	= 720,			// 查找下一个帮派，将ID写入TASK_ITERATOR
    ACTION_SYN_FIND_BY_NAME		= 721,			// 按名字查找帮派，玩家要输入帮派名字。将ID写入TASK_ITERATOR
    ACTION_SYN_FIND_NEXT_SYNMEMBER	= 722,		// 查找下一个帮众，将ID写入TASK_ITERATOR
    ACTION_SYN_SAINT            = 724,          // 圣骑士团的升级的操作 "=, >="
    ACTION_SYN_RANK				= 726,			// 修改RANK，ACCEPT="rank name"。只修改RANK=50及以下。param="RANK50的等级限制 RANK40的 30的 20 10"，param为空则不限制。

    ACTION_SYN_UPMEMBERLEVEL		= 728,
    ACTION_SYN_APPLLY_ATTACKSYN		= 730,		//申请攻打帮派

    ACTION_SYN_LIMIT			= 799,

    // monster part --------------------------------------------------------------------------
    ACTION_MST_FIRST			= 800,
    ACTION_MST_DROPITEM			= 801,			// monster被杀死后掉物品或者钱, param "dropitem itemtype"或者"dropmoney money"
    //monster被杀死后掉陷阱, param "droptrap traptype lifeperiod"。
    ACTION_MST_MAGIC			= 802,			// 检查法术。
    // param "check type"(学过type类型的法术),
    // "check type level"(学过type类型的法术，且等级为level级),
    // "learn type"(学会type类法术，等级为0)，
    // "uplevel type"(type类法术升一级)
    ACTION_MST_LIMIT			= 899,

    // user attr part --------------------------------------------------------------------------
    ACTION_USER_FIRST			= 1000,
    ACTION_USER_ATTR			= 1001,			// 玩家属性的修改和检查。"attr opt data"。attr可选择
	//		"life"(+=,==,<)、
	//		"mana"(+=,==,<)、
	//		"money"(+=,==,<)、
	//		"exp"(+=,==,<)、
	//		"pk"(+=,==,<)、
	//		"profession"(==, set, >=, <=)、
	//		"level"、(+=,==,<)、
	//		"force"、(+=,==,<)、
	//		"dexterity"、(+=,==,<)
	//		"speed"、(+=,==,<)、
	//		"health"、(+=,==,<)、
	//		"soul"、(+=,==,<)、
	//		"rank"、(==,<)、
	//		"rankshow"、(==,<)、
	//		"iterator"、(=, <=, +=, ==)、
	//		"crime" (==, set)
	//		"gamecard"(==, >=, <=)
	//		"gamecard2"(==, >=, <=)
	//		"xp"(+=)
	//		"metempsychosis"(==, <)
	//													//      "nobility_rank"(==, <, +=, =)	//
	//		"mercenary_rank"(==, <, +=)		// 佣兵等级
	//		"mercenary_exp"(==, <, +=)		// 佣兵经验
	//		"exploit"(==, <, +=)			// 功勋值
	//		"maxlifepercent"(+=,==,<)		// 最大生命千分比
	//		"turor_exp"(==,<,+=,=)
	//		"tutor_level"(==,<,+=,=)
	//		"syn_proffer"(<,+=,=)	//帮派贡献度
	//		"maxeudemon"(<,+=,=)	//最大可召唤幻兽数

    ACTION_USER_FULL			= 1002,			// 将玩家的属性加满。"attr"。attr可选"life","mana"
    ACTION_USER_CHGMAP			= 1003,			// 切地图 param "idMap nPosX nPosY bPrisonChk", bPrisonChk为可选参数，缺省不可以出监狱，设置为1可以出
    ACTION_USER_RECORDPOINT		= 1004,			// 存记录点 param "idMap nMapX nMapY"
    ACTION_USER_HAIR			= 1005,			// "color num"
    // "style num"
    ACTION_USER_CHGMAPRECORD	= 1006,			// 切地图到记录点
    ACTION_USER_CHGLINKMAP		= 1007,			// 切地图到NPC链接的地图。需要有NPC对象。

    ACTION_USER_TALK			= 1010,			// 向玩家发MSGTALK消息。param中为消息内容, data为频道，
    // 	const unsigned short _TXTATR_NORMAL		=2000;
    // 	const unsigned short _TXTATR_ACTION		=_TXTATR_NORMAL+2;	// 动作
    // 	const unsigned short _TXTATR_SYSTEM		=_TXTATR_NORMAL+5;	// 系统
    // 	const unsigned short _TXTATR_TALK		=_TXTATR_NORMAL+7;	// 交谈
    // 	const unsigned short _TXTATR_GM 		=_TXTATR_NORMAL+11;	// GM频道
    //	const unsigned short _TXTATR_WEBPAGE 	=_TXTATR_NORMAL+105;	// 打开URL
    ACTION_USER_MAGIC			= 1020,			// 检查法术。param可以为：
    // "check type"(玩家学过type类型的法术),
    // "check type level"(玩家学过type类型的法术，且等级为level级),
    // "learn type"(玩家学会type类法术，等级为0)，
    // "uplevel type"(玩家的type类法术升一级)
    // "addexp type exp"(玩家的type类法术增加exp点经验)
    ACTION_USER_WEAPONSKILL		= 1021,			// "check type level", 检查武器技能的类型和等级，是否>=等级
    // "learn type level", 学习指定类型和等级的技能
    ACTION_USER_LOG				= 1022,			// 保存指定信息到gm log并带入触发者信息（名字和id）,信息指定在param中
    // 比如 "%s 完成天剑任务并获得宝石", param中%s就是保存触发者信息的位置
    ACTION_USER_BONUS			= 1023,			// 取一个奖品。
    ACTION_USER_DIVORCE			= 1024,			// 离婚
    ACTION_USER_MARRIAGE		= 1025,			// 婚姻检查, 已婚返回1，未婚返回0
    ACTION_USER_SEX				= 1026,			// 性别检查, 男返回1，女返回0
    ACTION_USER_EFFECT			= 1027,			// 触发action的人物附加指定特效，param为"opt effect", opt支持"self", "couple", "team", "target", effect为特效名称
    ACTION_USER_TASKMASK		= 1028,			// task mask相关操作，param为"opt idx", opt为操作，支持"chk", "add", "clr", idx为任务编号，取值(0-31)
    ACTION_USER_MEDIAPLAY		= 1029,			// 媒体播放, param 为"opt media", opt支持 "play, broacast", "media"为媒体文件名
    ACTION_USER_SUPERMANLIST	= 1030,			// 查询无双列表, 起始值存在TASK_ITERATOR中。param="idNextTask number", idNextTask是下一个TASK的值，number是每次下传几个列表项。
    ACTION_USER_CHKIN_CARD		= 1031,			// 删除玩家身上的游戏卡物品，添加一个游戏卡记录
    ACTION_USER_CHKOUT_CARD		= 1032,			// 添加一个游戏卡物品给玩家，删除一个游戏卡记录
    ACTION_USER_CREATEMAP		= 1033,			// 创建一张地图，链接到玩家的home_id上，需要玩家对象。param="name owner_type owner_id mapdoc type portal_x portal_y reborn_map reborn_portal res_lev"。partal指入口点坐标，res_lev表示地图等级(用于升级)。
    ACTION_USER_ENTER_HOME		= 1034,			// 回到自己的家。
    ACTION_USER_ENTER_MATE_HOME	= 1035,			// 回到配偶的家。
    ACTION_USER_CHKIN_CARD2		= 1036,			// 删除玩家身上的游戏卡2物品，添加一个游戏卡2记录
    ACTION_USER_CHKOUT_CARD2	= 1037,			// 添加一个游戏卡2物品给玩家，删除一个游戏卡2记录
    ACTION_USER_FLY_NEIGHBOR	= 1038,			// 在地图组中查找一个_ROLE_NEIGHBOR_DOOR类型的NPC，立即切屏到该NPC处。param="serial"，serial是指data3的值。
    ACTION_USER_UNLEARN_MAGIC	= 1039,			// 转世时，忘却魔法技能，该技能将来可以再“顿悟”。param="type1 type2 ..."，至少一个参数，最多20个参数。
    ACTION_USER_REBIRTH			= 1040,			// 转世。如果已转过或者等级不够人，会失败。检查玩家是否已经转职、职业要求、等级要求。自动修改玩家职业、等级、身上装备等级、重新分配点数。param="nProf nLook"
    // 以下功能由其它ACTION完成：任务奖励；15、40、100级的就职奖励；坐骑奖励；随时回城；技能奖励。
    ACTION_USER_WEBPAGE			= 1041,			// 通知客户端打开网页。param="http://....."
    ACTION_USER_BBS				= 1042,			// 在BBS公告板中，添加一条SYSTEM频道的消息，留言人为玩家名字。需要USER对象，每个USER仅保留一条。param是消息内容。
    ACTION_USER_UNLEARN_SKILL	= 1043,			// 转世时，忘却所有武器技能，该技能将来可以再“顿悟”。
    ACTION_USER_DROP_MAGIC		= 1044,			// 转世时，删除魔法技能。param="type1 type2 ..."，至少一个参数，最多20个参数。
    ACTION_USER_OPEN_DIALOG		= 1046,			// 通知客户端打开一个界面。data=idDialog。param="task_id0 task_id1 task_id2 task_id3 ..."，可以无param，最多20个task_id, task_id不能为0, 允许客户端选择下一个TASK。无param时，客户端只能上传“客户端可触发的TASK”。有param时，cq_task.client_active必须为0。
    ACTION_USER_CHGMAP_REBORN	= 1047,			// 切屏到复活点。
    //		ACTION_USER_ADD_WPG_BADGE	= 1048,			// 添加周PK赛信物，按身上找到的第一件信物类型添加。该物品必须是可叠加的。最多只给两个。
    ACTION_USER_DEL_WPG_BADGE	= 1049,			// 删除所有周PK赛信物。
    ACTION_USER_CHK_WPG_BADGE	= 1050,			// 检查玩家身上只有一个param类型的物品(数量只能有一个)，没有其它周PK赛的信物。param为空表示不能有任何周PK赛信物。
    ACTION_USER_TAKESTUDENTEXP	= 1051,			// 提取徒弟贡献的经验。玩家返回pszAccept指定需要提取的经验值，自动扣除导师经验。

    ACTION_USER_CHGTO_MAINMAP	= 1052,			// 到主地图的复活点复活
    ACTION_USER_CHGTO_RANDOMPOS	= 1053,			// 人物随机飞到当前地图上任意一个坐标（该点不能有掩码）


    //---任务系统，记录任务的细节---begin

    ACTION_USER_TASK_MANAGER	= 1080,			/////////////////////////////////////////////////
    ////data:任务编号，
    //param: 'new' (为创建新记录)
    //       'delete' (删除记录)
    //		 'isexit' (任务是否存在)
    /////////////////////////////////////////////////

    ACTION_USER_TASK_OPE		= 1081,			/////////////////////////////////////////////////
    //data: 任务编号，  若data == -1 ,则下面的操作是针对 findnext 进行的
    //param: 'ope opt data',  data(值)
    //		 ope(phase)		 opt(>=, ==, +=,=) 对任务阶段操作
    //		 ope(completenum) opt(>=, ==, +=,=) 对任务完成次数操作
    //       ope(begintime)	 opt(>=, ==, +=,=,reset) 对任务开始时间操作, 对于 += 时 以秒为参数；对于">=,==,="时以"yyyy-mm-dd hh:mm:ss"为格式
    //                           reset表示 将任务的开始时间设置为当前时间
    ACTION_USER_TASK_LOCALTIME = 1082,          /////////////////////////////////////////////////////////
    //data: 任务编号，
    //param: ‘秒数’ , 当前时间与任务开始时间比较的action；若当前时间与任务的开始时间之差 大于 param ，则返回true.否则返回false
    //////////////////////////////////////////////////////////////////////

    ACTION_USER_TASK_FIND		= 1083,			//对玩家任务进行查询 ,记录是按照 userid , taskid 升序的集合
    //param: 'find taskid phase completenum ';根据任务ID，阶段，完成次数查询具体记录; phase 与 cocompletenum同时为-1时，只查询符合taskid的记录
    //		 'findnext'	;查询下一条记录
    //---任务系统，记录任务的细节---end


    // team part. ★ 没有队伍时，会返回 false。以下ACTION必须由队长触发，
    // 操作对象为每个队员(通常不包括队长)，队员必须在一屏范围内。
    // 注意：必须所有的队员都true时，才返回true；否则返回false
    // ★ ---------------------------------------------------------------
    ACTION_TEAM_BROADCAST		= 1101,			// 向在队伍频道中广播一条消息。param=消息。
    ACTION_TEAM_ATTR			= 1102,			// 检查或操作队员属性。
    // param="field opt data",
    // field="money"(+=、<、>、==),
    // field="level"(<、>、==),
    // field="count"(含队长的队员数量，<、==),
    // field="count_near"(含队长的队员数量，本地图中，必须活着，<、==),
    // field="mate"(只需要field，必须活着),
    // field="friend"(只需要field，必须活着),
    ACTION_TEAM_LEAVESPACE		= 1103,			// 检查背包剩余空间, param="space weight packtype"。
    // packtype为需要检查的背包类型，取值范围50~53
    ACTION_TEAM_ITEM_ADD		= 1104,			// 添加物品。data=itemtype_id
    ACTION_TEAM_ITEM_DEL		= 1105,			// 删除物品。data=itemtype_id
    ACTION_TEAM_ITEM_CHECK		= 1106,			// 检测物品。data=itemtype_id
    ACTION_TEAM_CHGMAP			= 1107,			// 组队切屏(包含队长)，仅用于同一个地图组切屏，所有人必须活着。param="mapid x y"

    ACTION_TEAM_CHK_ISLEADER	= 1501,			// 检查是否为队长，不需要参数
    ACTION_USER_LIMIT			= 1999,

    // event part, need not any ptr --------------------------------------------------------------------------
    ACTION_EVENT_FIRST			= 2000,
    ACTION_EVENT_SETSTATUS		= 2001,			// 设置地图状态。param="mapid status_bit data", status_bit = { STATUS_WAR=1, }, data=0或1。
    ACTION_EVENT_DELNPC_GENID	= 2002,			// (作废)删除MONSTER。param="idMap idGen"。
    ACTION_EVENT_COMPARE		= 2003,			// 比较各种属性。"data1 opt data2"。data1、data2为带%的通用参数, 按带符号数比较。opt可选"=="、"<"、"<="
    ACTION_EVENT_COMPARE_UNSIGNED	= 2004,			// 比较各种属性。"data1 opt data2"。data1、data2为带%的通用参数, 按无符号数比较。opt可选"=="、"<"、"<="
    ACTION_EVENT_CHANGEWEATHER	= 2005,			// 修改指定REGION的天气。param="idMap idRegion Type Intensity Dir Color KeepSecs", Type, Intensity=0~999, Dir=0~359, Color=0x00RRGGBB, KeepSecs=seconds
    ACTION_EVENT_CREATEPET		= 2006,			// 创建一个MONSTER。param="nOwnerType idOwner idMap nPosX nPosY idGen idType nData szName", 至少7个参数, 如有accept则改名，否则用name命名。generator用于控制怪物活动范围，cq_generator中的type无意义。idOwner为0时，不存盘。
    ACTION_EVENT_CREATENEW_NPC	= 2007,			// 创建一个NPC。param="name type sort lookface ownertype ownerid  mapid posx posy  life base linkid  task0 task0 ... task7  data0 data1 data2 data3 datastr"。至少9个参数。
    ACTION_EVENT_COUNTMONSTER	= 2008,			// 检查同地图的MONSTER数量。param="idMap field data opt num", field="name"(按名字)、"gen_id"(按类型), data=要统计的名字或类型, opt="<"、"=="。
    ACTION_EVENT_DELETEMONSTER	= 2009,			// 删除某地图的MONSTER。param="idMap type data name", 至少两个参数。如果data不为0则同时匹配data, 如果有name则同时匹配name。
    ACTION_EVENT_BBS			= 2010,			// 在BBS公告板中，添加一条SYSTEM频道的消息，留言人为“SYSTEM”。param是消息内容。
    ACTION_EVENT_ERASE			= 2011,			// 删除指定NPC。限动态NPC。注意：删除后，不要再对此类NPC进行操作。param="idMap type": 删除指定地图的所有类型为type的NPC。
    ACTION_EVENT_LIMIT			= 2099,

    // event part, need not any ptr --------------------------------------------------------------------------
    ACTION_TRAP_FIRST			= 2100,
    ACTION_TRAP_CREATE			= 2101,			// 创建一个陷阱。param="type look owner_id map_id pos_x pos_y data"。
    ACTION_TRAP_ERASE			= 2102,			// 删除一个陷阱。param="", 删除当前陷阱。注意：删除后不要再操作该陷阱。
    ACTION_TRAP_COUNT			= 2103,			// 检测类型为type的陷阱数量, 小于count返回true。param="map_id pos_x pos_y pos_cx pos_cy count type"。
    ACTION_TRAP_ATTR			= 2104,			// 修改陷阱的属性(不存盘)。param="id field opt num"。field: "type"(opt: "="), "look"(opt: "=")。
    ACTION_TRAP_LIMIT			= 2199,

    // wanted list part  --------------------------------------------------------------------------
    ACTION_WANTED_FIRST			= 3000,
    ACTION_WANTED_NEXT			= 3001,			// 搜寻下个悬赏，将idx写入TASK_ITERATOR
    ACTION_WANTED_NAME			= 3002,			// 玩家返回pszAccept指定被悬赏者名字, 和ACTION_WANTED_NEW联用。
    ACTION_WANTED_BONUTY		= 3003,			// 玩家返回pszAccept指定金额，和ACTION_WANTED_NEW联用。
    ACTION_WANTED_NEW			= 3004,			// 通过CUser::m_WantedInfo生产新的悬赏记录，和上2条action联合使用。
    ACTION_WANTED_ORDER			= 3005,			// 领取pszAccept中指定的悬赏
    ACTION_WANTED_CANCEL		= 3006,			// 以2倍的价钱取消pszAccept中指定的悬赏
    ACTION_WANTED_MODIFYID		= 3007,			// 玩家返回pszAccept指定修改的悬赏id。
    ACTION_WANTED_SUPERADD		= 3008,			// 玩家返回pszAccept指定追加悬赏的金钱，与ACTION_WANTED_ID联合使用。
    ACTION_POLICEWANTED_NEXT	= 3010,			// 搜寻下个官方悬赏，将idx写入TASK_ITERATOR
    ACTION_POLICEWANTED_ORDER	= 3011,			// 揭榜(pszAccept中指定编号)
    ACTION_POLICEWANTED_CHECK	= 3012,			// 检查触发人是否被官府通缉
    ACTION_WANTED_LIMIT			= 3099,

    // ghost gem magic part  ----------------------------------------------------------------------------------
    ACTION_MAGIC_FIRST			= 4000,
    ACTION_MAGIC_ATTACHSTATUS	= 4001,			// 追加状态，szParam="status power secs times"

    ACTION_MAGIC_ATTACK			= 4002,			// 魔法攻击，data=magictype, szParam="magiclevel"
    // 要求magictype表中存在相应的数据
    // 目前支持的魔法类型有：
    //		MAGICSORT_DETACHSTATUS
    //		MAGICSORT_STEAL
    ACTION_MAGIC_LIMIT			= 4099,

};

