
# MySQL dump 8.16
#
# Host: 192.168.1.200    Database: cq
#--------------------------------------------------------
# Server version	3.23.32

#
# Table structure for table 'cq_item'
#

CREATE TABLE cq_item( 
	id int(4) unsigned NOT NULL auto_increment, 
	type int(4) unsigned NOT NULL default '0', 
	owner_id int(4) unsigned NOT NULL default '0', 
	player_id int(4) unsigned NOT NULL default '0', 
	amount smallint(2) unsigned NOT NULL default '100', 
	amount_limit smallint(2) unsigned NOT NULL default '100', 
	ident tinyint(1) unsigned NOT NULL default '0', 
	position tinyint(1) unsigned NOT NULL default '0', 
	gem1 tinyint(1) unsigned NOT NULL default '0', 
	gem2 tinyint(1) unsigned NOT NULL default '0', 
	magic1 tinyint(1) unsigned NOT NULL default '0', 
	magic2 tinyint(1) unsigned NOT NULL default '0', 
	magic3 tinyint(1) unsigned NOT NULL default '0', 
	data int(4) NOT NULL default '0', 
	PRIMARY KEY (id), 
	KEY package(owner_id, position, player_id)
) TYPE=MyISAM; 

#
# Table structure for table 'cq_itemtype'
#

DROP TABLE IF EXISTS cq_itemtype;
CREATE TABLE cq_itemtype ( 
	id int(4) unsigned NOT NULL default '0', 
	name varchar(15) binary NOT NULL default '', 
	req_profession tinyint(1) unsigned NOT NULL default '0', 
	level tinyint(1) unsigned NOT NULL default '0', 
	req_level tinyint(1) unsigned NOT NULL default '0', 
	req_sex tinyint(1) unsigned NOT NULL default '0', 
	req_force smallint(2) unsigned NOT NULL default '0', 
	req_speed smallint(2) unsigned NOT NULL default '0', 
	req_health smallint(2) unsigned NOT NULL default '0', 
	req_soul smallint(2) unsigned NOT NULL default '0', 
	monopoly tinyint(1) unsigned NOT NULL default '0', 
	weight smallint(2) unsigned NOT NULL default '100', 
	price int(4) unsigned NOT NULL default '0', 
	id_action int(4) unsigned NOT NULL default '0', 
	attack_max smallint(2) unsigned NOT NULL default '0', 
	attack_min smallint(2) unsigned NOT NULL default '0', 
	defense smallint(2) unsigned NOT NULL default '0', 
	magicatk_min smallint(2) unsigned NOT NULL default '0', 
	add_soul smallint(2) unsigned NOT NULL default '0', 
	life smallint(2) NOT NULL default '0', 
	mana smallint(2) NOT NULL default '0', 
	amount smallint(2) unsigned NOT NULL default '1', 
	amount_limit smallint(2) unsigned NOT NULL default '1', 
	ident tinyint(1) unsigned NOT NULL default '0', 
	gem1 tinyint(1) unsigned NOT NULL default '0', 
	gem2 tinyint(1) unsigned NOT NULL default '0', 
	magic1 tinyint(1) unsigned NOT NULL default '0', 
	magic2 tinyint(1) unsigned NOT NULL default '0', 
	magic3 tinyint(1) unsigned NOT NULL default '0', 
	magicatk_max smallint(2) unsigned NOT NULL default '0', 
	magic_def smallint(2) unsigned NOT NULL default '0', 
	atk_range smallint(2) unsigned NOT NULL default '1', 
	atk_speed smallint(2) unsigned NOT NULL default '800', 
	PRIMARY KEY (id)
) TYPE=MyISAM; 

##############################################################
# itemtype表针对精灵的字段转义：
# req_level：精灵等级
# attack_max：精灵原始成长率
# attack_min：精灵升级到下一级需要的经验值
##############################################################
#
# req_weaponskill字段无用，改名为level：物品等级
#
##############################################################

#
# Table structure for table 'cq_friend'
#

CREATE TABLE cq_friend (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  userid int(4) unsigned zerofill NOT NULL default '0000',
  friend int(4) unsigned zerofill NOT NULL default '0',
  friendname varchar(15) NOT NULL default '无',
  PRIMARY KEY (id),
  KEY userid(userid)
) TYPE=MyISAM;


#
# Table structure for table 'cq_enemy'
#

DROP TABLE IF EXISTS cq_enemy;
CREATE TABLE cq_enemy (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  userid int(4) unsigned zerofill NOT NULL default '0000',
  enemy int(4) unsigned zerofill NOT NULL default '0',
  enemyname varchar(15) NOT NULL default '无',
  time int(4) unsigned zerofill NOT NULL default '0',
  PRIMARY KEY (id),
  KEY userid(userid,enemy)
) TYPE=MyISAM;


#
# Table structure for table 'cq_generator'
#

DROP TABLE IF EXISTS cq_generator;
CREATE TABLE cq_generator (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  mapid int(4) unsigned zerofill NOT NULL default '0000',
  bound_x int(4) unsigned zerofill NOT NULL default '0000',
  bound_y int(4) unsigned zerofill NOT NULL default '0000',
  bound_cx int(4) unsigned zerofill NOT NULL default '0000',
  bound_cy int(4) unsigned zerofill NOT NULL default '0000',
  grid int(4) unsigned zerofill NOT NULL default '0009',
  rest_secs int(4) unsigned zerofill NOT NULL default '0000',
  max_per_gen int(4) unsigned zerofill NOT NULL default '0000',
  npctype int(4) unsigned zerofill NOT NULL default '0000',
  timer_begin int(4) unsigned zerofill NOT NULL default '0000',
  timer_end int(4) unsigned zerofill NOT NULL default '0000',
  born_x int(4) unsigned zerofill NOT NULL default '0000',
  born_y int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id),
  KEY mapid(mapid)
) TYPE=MyISAM;
# max_per_gen - 0: generat by game server
# npctype - only use for monster but pet
# (bound_x==0 && bound_y==0) : dynamic bound for pet
# timer_begin,timer_end - format: DDWWHHMMSS, DD:day of month(1-31), WW:day of week(1-7), HHMMSS:hour,minute,second. WW mast 00 when DD is not 00.
# born_x,born_y - born from bound when default 0, else born to corrd(x,y)


#
# Table structure for table 'cq_monstertype'
#

DROP TABLE IF EXISTS cq_monstertype;
CREATE TABLE cq_monstertype (
	id INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	name CHAR (15) NOT NULL  DEFAULT '无', 
	type INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	lookface INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	length tinyint (4) NOT NULL  DEFAULT 0, 
	fat tinyint (4) NOT NULL  DEFAULT 0, 
	life SMALLINT (2) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 00, 
	mana SMALLINT (2) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 00, 
	attack_max INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	attack_min INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	defence INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	dexterity INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	dodge INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	helmet_type INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	armor_type INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	weaponr_type INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	weaponl_type INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	attack_range INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	view_range INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	escape_life INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	attack_speed INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	move_speed INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	level INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	attack_user INTEGER (1) UNSIGNED  NOT NULL  DEFAULT 3, 
	drop_money_min INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	drop_money_max INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	size_add INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	action INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	run_speed INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	drop_armet tinyint (3) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 000, 
	drop_necklace tinyint (3) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 000, 
	drop_armor tinyint (3) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 000, 
	drop_ring tinyint (3) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 000, 
	drop_weapon tinyint (3) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 000, 
	drop_shield tinyint (3) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 000, 
	drop_shoes tinyint (3) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 000, 
	drop_money_chance INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	drop_item_chance INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	magic_type INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	magic_def INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	magic_hitrate INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0100, 
	atk_hitrate INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	ai_type INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	defence2 INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 10000, 
	explode_item_chance1 INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	explode_item_chance2 INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	explode_item_chance3 INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000, 
	drop_item_rule INTEGER (4) UNSIGNED  ZEROFILL  NOT NULL  DEFAULT 0000,
	PRIMARY KEY (id)
) TYPE=MyISAM;
# attack_user - 0: escape only;
				1: passive;		// 被动攻击
				2: active;		// 主动攻击
				4: good;		// 正义的(卫兵或玩家召唤和控制的怪物)
				8: guard;		// 卫兵
				16: pk_killer;		// 追杀黑名
				32: jump;		// 会跳
				64: fixed		// 不会动的
				128: fastback,		// 速归
				256: lockuser(auto_die), for task monster		// 锁定攻击指定玩家，玩家离开自动消失
				512: lockone,		// 锁定攻击首先攻击自己的玩家
				1024: add_life,		// 自动加血
				2048: evil_killer, 		// 白名杀手
				4096: wing,		// 飞行状态
				8192: neutral,		// 中立的
				16384: roar,		// 出生时全地图怒吼
				32768: no escape,		// 不会逃跑
				65536: equality,		// 不藐视
# attack_max/attack_min - attack with weapon, or power of magic when magic_type
# magic_type - send to client type, need not cq_magictype.
# recruit magic npc - magic_type=1020, magic_hitrate=percent(10 -> 10%)
# ai_type - for call pet ai, use by client only
# defence2 - danage = danage * defence2 / 10000

#
# Table structure for table 'cq_pet'
#

DROP TABLE IF EXISTS cq_pet;
CREATE TABLE cq_pet (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  ownerid int(4) unsigned zerofill NOT NULL default '0000',
  ownertype tinyint(3) unsigned zerofill NOT NULL default '000',
  generatorid int(4) unsigned zerofill NOT NULL default '0000',
  typeid int(4) unsigned zerofill NOT NULL default '0000',
  name char(15) binary NOT NULL default '无',
  life smallint(2) unsigned zerofill NOT NULL default '0000',
  mana smallint(2) unsigned zerofill NOT NULL default '0000',
  recordmap_id int(4) unsigned NOT NULL default '0000', 
  recordx smallint(2) unsigned NOT NULL default '0000', 
  recordy smallint(2) unsigned NOT NULL default '0000', 
  data int(4) unsigned NOT NULL default '0000', 
  PRIMARY KEY (id)
) TYPE=MyISAM;
# ownertype - 1: user, 2: syndicate, 3: npc
# typeid - id of cq_monstertype


#
# Table structure for table 'cq_map'
#

DROP TABLE IF EXISTS cq_map;
CREATE TABLE cq_map (
  id int(4) unsigned NOT NULL default '0',
  name varchar(15) binary NOT NULL default '未命名',
  describe_text varchar(127) binary NOT NULL default '',
  mapdoc int(4) unsigned NOT NULL default '0',
  type int(4) unsigned zerofill NOT NULL default '0',
  owner_id int(4) unsigned NOT NULL default '0',
  mapgroup int(4) unsigned zerofill NOT NULL default '0',
  idxserver int(4) NOT NULL default '-1',
  weather int(4) unsigned zerofill NOT NULL default '0',
  bgmusic int(4) unsigned zerofill NOT NULL default '0',
  bgmusic_show int(4) unsigned zerofill NOT NULL default '0',
  portal0_x int(4) unsigned zerofill NOT NULL default '0000',
  portal0_y int(4) unsigned zerofill NOT NULL default '0000',
  reborn_mapid int(4) unsigned zerofill NOT NULL default '0000',
  reborn_portal int(4) unsigned zerofill NOT NULL default '0000',
  res_lev tinyint(3) unsigned NOT NULL default '0',
  owner_type tinyint(3) unsigned NOT NULL default '0',
  link_map int(4) unsigned zerofill NOT NULL default '0000',
  link_x smallint(2) unsigned zerofill NOT NULL default '0000',
  link_y smallint(2) unsigned zerofill NOT NULL default '0000',
  del_flag tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY (id),
  KEY idx(idxserver,mapgroup)
) TYPE=MyISAM;

DROP TABLE IF EXISTS cq_dynamap;
CREATE TABLE cq_dynamap (
  id int(4) unsigned NOT NULL default '0' auto_increment,
  name varchar(15) binary NOT NULL default '未命名',
  describe_text varchar(127) binary NOT NULL default '',
  mapdoc smallint(4) unsigned NOT NULL default '0',
  type int(4) unsigned zerofill NOT NULL default '0',
  owner_id int(4) unsigned NOT NULL default '0',
  mapgroup tinyint(4) unsigned zerofill NOT NULL default '0',
  idxserver tinyint(4) NOT NULL default '-1',
  weather tinyint(4) unsigned zerofill NOT NULL default '0',
  bgmusic tinyint(4) unsigned zerofill NOT NULL default '0',
  bgmusic_show tinyint(4) unsigned zerofill NOT NULL default '0',
  portal0_x smallint(4) unsigned zerofill NOT NULL default '0000',
  portal0_y smallint(4) unsigned zerofill NOT NULL default '0000',
  reborn_mapid int(4) unsigned zerofill NOT NULL default '0000',
  reborn_portal tinyint(4) unsigned zerofill NOT NULL default '0000',
  res_lev smallint(4) unsigned NOT NULL default '0',
  owner_type tinyint(3) unsigned NOT NULL default '0',
  link_map int(4) unsigned zerofill NOT NULL default '0000',
  link_x smallint(2) unsigned zerofill NOT NULL default '0000',
  link_y smallint(2) unsigned zerofill NOT NULL default '0000',
  del_flag tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY (id)
) TYPE=MyISAM;
INSERT cq_dynamap SET id=1000000,name='RESERVE_ID_SPACE';
# enum ENUM_MAPTYPE {
	MAPTYPE_NORMAL				= 0x0000,
	MAPTYPE_PKFIELD				= 0x0001,
	MAPTYPE_CHGMAP_DISABLE		= 0x0002,
	MAPTYPE_RECORD_DISABLE		= 0x0004,
	MAPTYPE_PK_DISABLE			= 0x0008,
};


#
# Table structure for table 'cq_portal'
#

DROP TABLE IF EXISTS cq_portal;
CREATE TABLE cq_portal (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  mapid int(4) unsigned zerofill NOT NULL default '0000',
  portal_idx int(4) unsigned zerofill NOT NULL default '0000',
  pos_x int(4) unsigned zerofill NOT NULL default '0000',
  pos_y int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id),
  KEY mapid(mapid)
) TYPE=MyISAM;


#
# Table structure for table 'cq_passway'
#

DROP TABLE IF EXISTS cq_passway;
CREATE TABLE cq_passway (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  mapid int(4) unsigned zerofill NOT NULL default '0000',
  passway_idx int(4) unsigned zerofill NOT NULL default '0000',
  target_mapid int(4) unsigned zerofill NOT NULL default '0000',
  target_mapportal int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id),
  KEY mapid(mapid)
) TYPE=MyISAM;


#
# Table structure for table 'cq_region'
#

DROP TABLE IF EXISTS cq_region;
CREATE TABLE cq_region (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  mapid int(4) unsigned zerofill NOT NULL default '0000',
  type int(4) unsigned zerofill NOT NULL default '0000',
  bound_x int(4) unsigned zerofill NOT NULL default '0000',
  bound_y int(4) unsigned zerofill NOT NULL default '0000',
  bound_cx int(4) unsigned zerofill NOT NULL default '0000',
  bound_cy int(4) unsigned zerofill NOT NULL default '0000',
  datastr varchar(15) binary NOT NULL default '', 
  data0 int(4) unsigned zerofill NOT NULL default '0000',
  data1 int(4) unsigned zerofill NOT NULL default '0000',
  data2 int(4) unsigned zerofill NOT NULL default '0000',
  data3 int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id),
  KEY mapid(mapid)
) TYPE=MyISAM;
# mapid - is mapdoc
# type：
# 1 - city. record position disable, magic call player disable.
# 2 - weather. data0(2): Type, data1(200): Intensity(0-999), data2(10): dir(0-359), data3(0x00ffffff): nColor, data1<0: fast change.
#	type - enum { WEATHER_NONE=0, WEATHER_FINE, 
#					WEATHER_RAINY, WEATHER_SNOWY, WEATHER_SANDS, 
#					WEATHER_LEAF, WEATHER_BAMBOO, WEATHER_FLOWER, 
#					WEATHER_FLYING, WEATHER_DANDELION, WEATHER_WORM, WEATHER_CLOUDY,  
#					WEATHER_ALL};


#
# Table structure for table 'cq_magictype'
#

DROP TABLE IF EXISTS cq_magictype;
CREATE TABLE cq_magictype (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  type int(4) unsigned zerofill NOT NULL default '0000',
  sort int(4) unsigned zerofill NOT NULL default '0000',
  name varchar(15) binary NOT NULL default '', 
  crime tinyint(1) unsigned zerofill NOT NULL default '0',
  ground tinyint(1) unsigned zerofill NOT NULL default '0',
  multi tinyint(1) unsigned zerofill NOT NULL default '0',
  target int(4) unsigned zerofill NOT NULL default '0000',
  level int(4) unsigned zerofill NOT NULL default '0000',
  use_mp int(4) unsigned zerofill NOT NULL default '0000',
  power int(4) NOT NULL default '0000',
  intone_speed int(4) unsigned zerofill NOT NULL default '0000',
  percent int(4) unsigned zerofill NOT NULL default '0000',
  step_secs int(4) unsigned zerofill NOT NULL default '0000',
  range int(4) unsigned zerofill NOT NULL default '0000',
  distance int(4) unsigned zerofill NOT NULL default '0000',
  status int(4) unsigned zerofill NOT NULL default '0000',
  need_prof int(4) unsigned zerofill NOT NULL default '0000',
  need_exp int(4) NOT NULL default '0000',
  need_level int(4) unsigned zerofill NOT NULL default '0000',
  use_xp tinyint(3) unsigned zerofill NOT NULL default '000',
  weapon_subtype int(4) unsigned zerofill NOT NULL default '0000',
  active_times int(4) unsigned zerofill NOT NULL default '0000',
  auto_active tinyint(3) unsigned zerofill NOT NULL default '000',
  floor_attr int(4) unsigned zerofill NOT NULL default '0000',
  auto_learn tinyint(1) unsigned zerofill NOT NULL default '0',
  learn_level int(4) unsigned zerofill NOT NULL default '0000',
  drop_weapon tinyint(1) unsigned zerofill NOT NULL default '0',
  use_ep int(4) unsigned zerofill NOT NULL default '0000',
  weapon_hit tinyint(1) unsigned zerofill NOT NULL default '0',
  use_item int(4) unsigned zerofill NOT NULL default '0000',
  next_magic int(4) unsigned zerofill NOT NULL default '0000',
  delay_ms int(4) unsigned zerofill NOT NULL default '0000',
  use_item_num int(4) unsigned zerofill NOT NULL default '0001',
  width int(4) unsigned zerofill NOT NULL default '0000',
  durability int(4) unsigned zerofill NOT NULL default '0001',
  PRIMARY KEY (id)
) TYPE=MyISAM;

# type: 类型
# 10000~10255 - 武器魔法，即cq_itemtype中的magic1+10000

# sort: 行为分类。
# 1 - 攻击单人，对方去血, check floor attr, support drop_weapon in weapon skill。(support xp skill)
# 2 - 回复，加血。(support auto active), (support team member with multi flag)
# 3 - 十字型区域目标去血。
# 4 - 扇形区域目标去血。(support xp skill)(SizeAdd effective)(support auto active of random mode)
# 5 - 圆形区域目标去血。(support ground type)
# 6 - 攻击单人，对方加状态。
# 7 - 回复单人，对方去状态。
# 8 - range of square, multi target, (support xp skill)(support ground type)(SizeAdd effective)
# 9 - jump & attack, single target, (support xp skill)(support ground type)
# 10 - random transmit, power is max distance.
# 11 - dispatch xp
# 12 - collide, (support xp skill only)
# 13 - serial cut, ground type only. (auto active, support xp skill & weapon skill)
# 14 - line, (support weapon skill, power is number of life expended)(support auto active of random mode)
# 15 - add attack range, (auto active only, forever effect)
# 16 - attack with temp status, weapon skill only
#		support status: attack | defence | be damage with weapon | attack rate | stop | vampire (other status is attack target and attach this status), 
#		support auto active of random mode, only for weapon_hit=0
# 17 - call team member. 
# 18 - record map position to trans spell.
# 19 - transform to monster, (monster_type == power)
# 20 - add mana, (power is add number, support self target only)
# 21 - lay trap, power is trap_type.
# 22 - dance, (use for client)
# 23 - call pet, power is monster type. step_secs is alive secs for pet, 0 for keep alive.
# 24 - VAMPIRE. use for call pet.
# 25 - INSTEAD. use for call pet.
# 26 - decrement life. power is data or percent of current life.
# 27 - ground sting.

# range: 
# 106 - 表示主方向为6，次方向为1

# power:
# (-29999)-29999 - add or sub
# 30000-32767 - add or sub by percent of (num - 30000)
# -32768 - set to full
# (-30000)-(-32767) - set to (-1*num - 30000)
# 0 - mast be zero when detach

# status: 
# 2 - detach badly status.(support detach only)
# 4 - poison
# 8 - detach all status of magic.(support detach only)
# 16 - vampire(only support sort16)
# 32 - HitRate
# 64 - defence
# 128 - attack
# 512 - magic defence
# 1024 - bow defence
# 2048 - attack range
# 4096 - reflect weapon
# 8192 - super man status, with half of defence.
# 16384 - be damage with weapon, detach when move or attack in keep effect
# 32768 - be damage with magic
# 65536 - attack rate
# 131072 - invisible for monster
# 262144 - TORNADO
# 1048576 - reflect magic
# 2097152 - dodge
# 4194304 - wing
# 8388608 - keep_bow, add attack, cancel when move
# 16777216 - stop milli secs in power, have not keep effect. monster valid. (believe: power is add damage, step_secs is keep milli secs.)

# need_prof: 有多种职业要求时，值相加即可。最高级填0，表示没有一种职业可以再升级了。
# 1				- 魔法师
# 2				- 魔法师转职
# 4				- 战士
# 8				- 战士转职
# 16			- 弓箭手
# 32			- 弓箭手转职

# need_exp:
#  0 - 不升级(或通过任务系统升级)
# -1 - 自动升级。

# target:
# 0 - 敌人
# 1 - 允许对自己
# 2 - no target

# use_xp:
# 0 - 法术
# 1 - XP技
# 2 - 武器技(support up level with auto_uplevel & weaponskill_level, do not support up level with need_exp number)

# active_times:
# 0 - once times status class			(use for defence/hitrate)
# 1-x - more times status class			(use for poison)

# auto_active:
# 0 - none auto active
# 1 - active when kill target, enable sort 2, 13
# 2 - active when equipment, enable sort 15
# 4 - active in random, enable sort 4, 5, 14, 16

# learn_level:
# 1-99 - automatic learn weapon skill at this level.
# nnmm - automatic learn weapon skill between mm - nn level.

# drop_weapon:
# 1 - throw weapon_r to target around.

# weapon_hit:
# 0 - hit by weapon
# 1 - hit by magic
# 2 - hit mode by arrow

# use_item - use equip item by sub_type, 0 for no use.
# use_item_num - use equip item number, 0 for check item only.

# delay_ms - 0: for default delay.

# width - width of magic. 0-180 for degree of fan (sort 4), 0 for default.


#
# Table structure for table 'cq_magic'
#

CREATE TABLE cq_magic (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  ownerid int(4) unsigned zerofill NOT NULL default '0000',
  type smallint(4) unsigned zerofill NOT NULL default '0000',
  level smallint(4) unsigned zerofill NOT NULL default '0000',
  exp int(4) unsigned zerofill NOT NULL default '0000',
  unlearn tinyint(1) unsigned zerofill NOT NULL default '0',
  old_level smallint(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id),
  KEY user_magic(ownerid)
) TYPE=MyISAM;


#
# Table structure for table 'cq_syndicate'
#

CREATE TABLE cq_syndicate (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  name varchar(15) binary NOT NULL default '', 
  announce varchar(255) binary NOT NULL default '', 
  tenet varchar(255) binary NOT NULL default '', 
  member_title varchar(15) binary NOT NULL default '弟子', 
  leader_id int(4) unsigned zerofill NOT NULL default '0000',
  leader_name varchar(15) binary NOT NULL default '', 
  money int(4) zerofill NOT NULL default '0000',
  fealty_syn int(4) unsigned zerofill NOT NULL default '0000',
  del_flag tinyint(1) unsigned zerofill NOT NULL default '0',
  amount int(4) unsigned zerofill NOT NULL default '0000',
  enemy0 int(4) unsigned zerofill NOT NULL default '0000',
  enemy1 int(4) unsigned zerofill NOT NULL default '0000',
  enemy2 int(4) unsigned zerofill NOT NULL default '0000',
  enemy3 int(4) unsigned zerofill NOT NULL default '0000',
  enemy4 int(4) unsigned zerofill NOT NULL default '0000',
  ally0 int(4) unsigned zerofill NOT NULL default '0000',
  ally1 int(4) unsigned zerofill NOT NULL default '0000',
  ally2 int(4) unsigned zerofill NOT NULL default '0000',
  ally3 int(4) unsigned zerofill NOT NULL default '0000',
  ally4 int(4) unsigned zerofill NOT NULL default '0000',
  rank tinyint(1) unsigned zerofill NOT NULL default '0',
  saint tinyint(1) unsigned zerofill NOT NULL default '0',
  mantle tinyint(1) unsigned zerofill NOT NULL default '0',
  distime int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id),
  UNIQUE KEY name(name)
) TYPE=MyISAM;
# del_flag: delete by syndicate leader


#
# Table structure for table 'cq_synattr'
#

CREATE TABLE cq_synattr (
  id int(4) unsigned zerofill NOT NULL,
  syn_id int(4) unsigned zerofill NOT NULL default '0000',
  rank int(4) unsigned zerofill NOT NULL default '0000',
  proffer int(4) unsigned zerofill NOT NULL default '0000',
  title int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id)
) TYPE=MyISAM;


#
# Table structure for table 'cq_leaveword'
#

DROP TABLE IF EXISTS cq_leaveword;
CREATE TABLE cq_leaveword (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  user_name char(15) binary NOT NULL default '未知',
  send_name char(15) binary NOT NULL default '未知',
  time char(15) binary NOT NULL default '00000000000000',
  words char(255) binary NOT NULL default '未命名',
  PRIMARY KEY (id),
  KEY user_name(user_name)
) TYPE=MyISAM;


#
# Table structure for table 'cq_bonus'
#

CREATE TABLE cq_bonus (
  serianumber char(15) binary NOT NULL default '未知',
  password char(15) binary NOT NULL default '未知',
  action int(4) unsigned zerofill NOT NULL default '0000',
  id int(4) unsigned zerofill NOT NULL auto_increment,
  PRIMARY KEY (id),
  UNIQUE KEY serianumber(serianumber)
) TYPE=MyISAM;


#
# Table structure for table 'cq_traptype'
#

DROP TABLE IF EXISTS cq_traptype;
CREATE TABLE cq_traptype (
  id int(4) unsigned zerofill NOT NULL,
  sort tinyint(3) unsigned zerofill NOT NULL default '0',
  look int(4) unsigned zerofill NOT NULL default '0000',
  action_id int(4) unsigned zerofill NOT NULL default '0000',
  level int(4) unsigned zerofill NOT NULL default '0000',
  attack_max int(4) unsigned zerofill NOT NULL default '0000',
  attack_min int(4) unsigned zerofill NOT NULL default '0000',
  dexterity int(4) unsigned zerofill NOT NULL default '0000',
  attack_speed int(4) unsigned zerofill NOT NULL default '1000',
  active_times int(4) unsigned zerofill NOT NULL default '0000',
  magic_type int(4) unsigned zerofill NOT NULL default '0000',
  magic_hitrate int(4) unsigned zerofill NOT NULL default '0100',
  size int(4) unsigned zerofill NOT NULL default '0000',
  atk_mode int(3) unsigned zerofill NOT NULL default '0001',
  PRIMARY KEY  (id)
) TYPE=MyISAM;
insert cq_traptype values(1, 1,1091010,0,20,250,250,80,1000,0,1000,50,1,1);
insert cq_traptype values(2,10,1091010,0,20,25,25,80,1000,0,1000,50,7,1);

# sort:
	TRAPSORT_SYSTRAP	=1,					// 系统的陷阱，多次有效
	TRAPSORT_FIREWALL	=10,				// 火墙, 可由系统放置
# look - 0: invisible trap
# action_id - run action when active trap, only effect for TRAPSORT_SYSTRAP
# active_times - 0: forever effect
# magic_type - 0: hit by weapon mode
# atk_mode: 可叠加
	ATKMODE_USER		= 1,
	ATKMODE_MONSTER		= 2,
	ATKMODE_NOATTACK	= 4,				// 不攻击对象，只触发ACTION

CREATE TABLE cq_trap (
  id int(4) unsigned zerofill NOT NULL,
  type int(4) unsigned zerofill NOT NULL default '0000',
  look int(4) unsigned zerofill NOT NULL default '0000',
  owner_id int(4) unsigned zerofill NOT NULL default '0000',
  map_id int(4) unsigned zerofill NOT NULL default '0000',
  pos_x int(4) unsigned zerofill NOT NULL default '0000',
  pos_y int(4) unsigned zerofill NOT NULL default '0000',
  data int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY  (id)
) TYPE=MyISAM;
insert cq_trap values(990001,1,1091010,0,1002,404,400,0);
insert cq_trap values(990002,2,1091012,0,1002,439,400,0);

# id - 990001 ~ 999999
# look - 0: invisible trap


#
# Table structure for table 'cq_point_allot'
#

DROP TABLE IF EXISTS cq_point_allot;
CREATE TABLE cq_point_allot (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  prof_sort int(4) unsigned zerofill NOT NULL default '0000',
  level int(4) unsigned zerofill NOT NULL default '0000',
  force int(4) unsigned zerofill NOT NULL default '0000',
  Speed int(4) unsigned zerofill NOT NULL default '0000',
  health int(4) unsigned zerofill NOT NULL default '0000',
  soul int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY  (id),
  UNIQUE KEY idx(prof_sort,level)
) TYPE=MyISAM;

INSERT cq_point_allot VALUES(0,0,0,0,0,0,0);


#
# Table structure for table 'cq_superman'
#

CREATE TABLE cq_superman (
  id int(4) unsigned zerofill NOT NULL,
  number int(4) unsigned zerofill NOT NULL default '0000',
  name varchar(15) binary NOT NULL default '--',
  PRIMARY KEY  (id),
  KEY number(number)
) TYPE=MyISAM;


#
# Table structure for table 'cq_config'
#

CREATE TABLE cq_config (
  id int(4) unsigned zerofill NOT NULL,
  name varchar(15) binary NOT NULL default '',
  data int(4) unsigned zerofill NOT NULL default '0000',
  string varchar(255) binary NOT NULL default '',
  PRIMARY KEY  (id),
  KEY name(name)
) TYPE=MyISAM;


#
# Table structure for table 'cq_rebirth'
#

DROP TABLE IF EXISTS cq_rebirth;
CREATE TABLE cq_rebirth (
  id int(4) unsigned zerofill NOT NULL,
  need_prof int(4) unsigned zerofill NOT NULL default '0000',
  new_prof int(4) unsigned zerofill NOT NULL default '0000',
  need_level int(4) unsigned zerofill NOT NULL default '0000',
  new_level int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

#
# Table structure for table 'cq_dropitemrule'
#

DROP TABLE IF EXISTS cq_dropitemrule;
CREATE TABLE cq_dropitemrule(
	id int unsigned NOT NULL,
	RuleId int unsigned NOT NULL default '0',
	Chance int unsigned NOT NULL default '0',
	Item0 int unsigned NOT NULL default '0',
	Item1 int unsigned NOT NULL default '0',
	Item2 int unsigned NOT NULL default '0',
	Item3 int unsigned NOT NULL default '0',
	Item4 int unsigned NOT NULL default '0',
	Item5 int unsigned NOT NULL default '0',
	Item6 int unsigned NOT NULL default '0',
	Item7 int unsigned NOT NULL default '0',
	Item8 int unsigned NOT NULL default '0',
	Item9 int unsigned NOT NULL default '0',
	Item10 int unsigned NOT NULL default '0',
	Item11 int unsigned NOT NULL default '0',
	Item12 int unsigned NOT NULL default '0',
	Item13 int unsigned NOT NULL default '0',
	Item14 int unsigned NOT NULL default '0',

	PRIMARY KEY (id, RuleId)
) TYPE=MyISAM;


#
# Table structure for table 'cq_table'
#

DROP TABLE IF EXISTS cq_table;
CREATE TABLE cq_table (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  npc_id int(4) unsigned zerofill NOT NULL default '0000',
  type int(4) unsigned zerofill NOT NULL default '0000',
  key_id int(4) unsigned zerofill NOT NULL default '0000',
  datastr varchar(255) binary NOT NULL default '', 
  data0 int(4) unsigned zerofill NOT NULL default '0000',
  data1 int(4) unsigned zerofill NOT NULL default '0000',
  data2 int(4) unsigned zerofill NOT NULL default '0000',
  data3 int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY (id),
) TYPE=MyISAM;


