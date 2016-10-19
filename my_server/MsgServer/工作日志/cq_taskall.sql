#
# Table structure for table 'cq_npc'
#

DROP TABLE IF EXISTS cq_npc;
CREATE TABLE cq_npc (
  id int(4) unsigned zerofill NOT NULL default '0000',
  ownerid int(4) unsigned NOT NULL default '0',
  ownertype int(4) unsigned NOT NULL default '0',
  name char(15) binary NOT NULL default '未命名',
  type int(4) unsigned zerofill NOT NULL default '0',
  lookface int(4) unsigned zerofill NOT NULL default '0',
  idxserver int(4) NOT NULL default '-1',
  mapid int(4) unsigned zerofill NOT NULL default '0',
  cellx int(4) unsigned zerofill NOT NULL default '0',
  celly int(4) unsigned zerofill NOT NULL default '0',
  task0 int(4) unsigned zerofill NOT NULL default '0',
  task1 int(4) unsigned zerofill NOT NULL default '0',
  task2 int(4) unsigned zerofill NOT NULL default '0',
  task3 int(4) unsigned zerofill NOT NULL default '0',
  task4 int(4) unsigned zerofill NOT NULL default '0',
  task5 int(4) unsigned zerofill NOT NULL default '0',
  task6 int(4) unsigned zerofill NOT NULL default '0',
  task7 int(4) unsigned zerofill NOT NULL default '0',
  data0 int(4) NOT NULL default '0',
  data1 int(4) NOT NULL default '0',
  data2 int(4) NOT NULL default '0',
  data3 int(4) NOT NULL default '0',
  datastr char(15) binary NOT NULL default '无',
  linkid int(4) unsigned zerofill NOT NULL default '0',
  life int(4) unsigned zerofill NOT NULL default '0',
  maxlife int(4) unsigned zerofill NOT NULL default '0',
  base int(4) unsigned zerofill NOT NULL default '0',
  sort smallint(2) unsigned zerofill NOT NULL default '0',
  itemid int(4) unsigned zerofill NOT NULL default '0',
  defence smallint(2) unsigned zerofill NOT NULL default '0',
  magic_def smallint(2) unsigned zerofill NOT NULL default '0',
  PRIMARY KEY (id)
) TYPE=MyISAM;
INSERT cq_npc VALUES(9999,0,0,'赌场',19,26,-1,1002,404,398,0,0,0,0,0,0,0,0,0,0,0,0,'',0,0,0,0,0,0);

CREATE TABLE cq_dynanpc (
  id int(4) unsigned zerofill NOT NULL default '0000' auto_increment, 
  ownerid int(4) unsigned NOT NULL default '0',
  ownertype tinyint(3) unsigned NOT NULL default '0',
  name varchar(15) binary NOT NULL default '未命名',
  type smallint(2) unsigned zerofill NOT NULL default '0',
  lookface int(4) unsigned zerofill NOT NULL default '0',
  idxserver tinyint(3) NOT NULL default '-1',
  mapid int(4) unsigned zerofill NOT NULL default '0',
  cellx smallint(2) unsigned zerofill NOT NULL default '0',
  celly smallint(2) unsigned zerofill NOT NULL default '0',
  task0 int(4) unsigned zerofill NOT NULL default '0',
  task1 int(4) unsigned zerofill NOT NULL default '0',
  task2 int(4) unsigned zerofill NOT NULL default '0',
  task3 int(4) unsigned zerofill NOT NULL default '0',
  task4 int(4) unsigned zerofill NOT NULL default '0',
  task5 int(4) unsigned zerofill NOT NULL default '0',
  task6 int(4) unsigned zerofill NOT NULL default '0',
  task7 int(4) unsigned zerofill NOT NULL default '0',
  data0 int(4) NOT NULL default '0',
  data1 int(4) NOT NULL default '0',
  data2 int(4) NOT NULL default '0',
  data3 int(4) NOT NULL default '0',
  datastr varchar(255) binary NOT NULL default '无',
  linkid int(4) unsigned zerofill NOT NULL default '0',
  life int(4) unsigned zerofill NOT NULL default '0',
  maxlife int(4) unsigned zerofill NOT NULL default '0',
  base int(4) unsigned zerofill NOT NULL default '0',
  sort smallint(2) unsigned zerofill NOT NULL default '0',
  itemid int(4) unsigned zerofill NOT NULL default '0',
  defence smallint(2) unsigned zerofill NOT NULL default '0',
  magic_def smallint(2) unsigned zerofill NOT NULL default '0',
  PRIMARY KEY (id)
) TYPE=MyISAM;
####################
##  INSERT VALUES ##
####################
#ownertype: 0: system, 1: user, 2: syndicate, 3: npc
#type - 
enum NPCTYPE{
	  _NPC_NONE				= 0,			// 通用NPC
      _SHOPKEEPER_NPC		= 1,			// 商店NPC
	  _TASK_NPC				= 2,			// 任务NPC
	  _STORAGE_NPC			= 3,			// 寄存处NPC
	  _TRUNCK_NPC			= 4,			// 箱子NPC
	  _FACE_NPC				= 5,			// 变头像NPC
	  _FORGE_NPC			= 6,			// 锻造NPC
	  _STATUARY_NPC			= 7,			// 雕像NPC
	  _WEAPONGOAL_NPC		= 8,			// 武器靶子NPC
	  _MAGICGOAL_NPC		= 9,			// 武器靶子NPC
	  _BOWGOAL_NPC			= 10,			// 武器靶子NPC
	  _SYNFLAG_NPC			= 11,			// 帮派标记NPC
};
#sort - 
enum NPCSORT{
		NPCSORT_TASK		= 1,			// 任务类
		NPCSORT_RECYCLE		= 2,			// 可回收类
		NPCSORT_SCENE		= 4,			// 场景类(带地图物件，LookFace为地图物件的TYPE)
		NPCSORT_LINKMAP		= 8,			// 挂地图类(使用LINKID，与其它使用LINKID的互斥)
		NPCSORT_DIEACTION	= 16,			// 带死亡任务(使用LINKID，与其它使用LINKID的互斥)
		NPCSORT_DELENABLE	= 32,			// 可以手动删除(不是指通过任务)
};
#maxlife - 0: can not be attack
#base - npc type in "TerrainNpc.ini".



DROP TABLE IF EXISTS cq_goods;
CREATE TABLE cq_goods (
  id int(4) unsigned zerofill NOT NULL default '0000',
  ownerid int(4) unsigned NOT NULL default '0',
  itemtype int(4) unsigned NOT NULL default '0',
  PRIMARY KEY (id),
  KEY ownerid(ownerid)
) TYPE=MyISAM;


#
# Table structure for table 'cq_task'
#

DROP TABLE IF EXISTS cq_task;
CREATE TABLE cq_task (
  id int(4) unsigned zerofill NOT NULL default '0000',
  id_next int(4) unsigned zerofill NOT NULL default '0000',
  id_nextfail int(4) unsigned zerofill NOT NULL default '0000',
  itemname1 char(15) binary NOT NULL default '',
  itemname2 char(15) binary NOT NULL default '',
  money int(4) unsigned NOT NULL default '0',
  profession int(4) unsigned NOT NULL default '0',
  sex int(4) unsigned NOT NULL default '0',
  min_pk int(4) NOT NULL default '0',
  max_pk int(4) NOT NULL default '0',
  team int(4) unsigned zerofill NOT NULL default '0',
  metempsychosis int(4) unsigned zerofill NOT NULL default '0',
  query tinyint(1) unsigned zerofill NOT NULL default '0',
  marriage tinyint(1) NOT NULL default '0',
  client_active tinyint(1) unsigned zerofill NOT NULL default '0',
  PRIMARY KEY (id)
) TYPE=MyISAM;

#
# Table structure for table 'cq_action'
#

DROP TABLE IF EXISTS cq_action;
CREATE TABLE cq_action (
  id int(4) unsigned zerofill NOT NULL default '0000',
  id_next int(4) unsigned zerofill NOT NULL default '0000',
  id_nextfail int(4) unsigned zerofill NOT NULL default '0000',
  type int(4) unsigned zerofill NOT NULL default '0000',
  data int(4) NOT NULL default '0',
  param char(127) binary NOT NULL default '',
  PRIMARY KEY (id)
) TYPE=MyISAM;


