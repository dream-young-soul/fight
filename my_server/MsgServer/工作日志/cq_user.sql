DROP TABLE IF EXISTS cq_user;
CREATE TABLE cq_user ( 
	name varchar(15) binary NOT NULL default '', 
	mate varchar(15) binary NOT NULL default '', 
	lookface int(4) unsigned NOT NULL default '0', 
	hair smallint(2) unsigned NOT NULL default '0', 
	money int(4) unsigned NOT NULL default '0', 
	money_saved int(4) unsigned NOT NULL default '0', 
	task_data int(4) unsigned NOT NULL default '0', 
	level tinyint(1) unsigned NOT NULL default '0', 
	exp int(4) NOT NULL default '0', 
	force smallint(2) unsigned NOT NULL default '0', 
	physique smallint(2) unsigned NOT NULL default '0', 
	Speed smallint(2) unsigned NOT NULL default '0', 
	health smallint(2) unsigned NOT NULL default '0', 
	soul smallint(2) unsigned NOT NULL default '0', 
	additional_point smallint(2) unsigned NOT NULL default '0',
	auto_allot tinyint(1) unsigned NOT NULL default '0', 
	life smallint(2) unsigned NOT NULL default '0', 
	mana smallint(2) unsigned NOT NULL default '0', 
	profession tinyint(2) unsigned NOT NULL default '0', 
	deed int(4) NOT NULL default '0', 
	pk smallint(2) NOT NULL default '0', 
	nobility tinyint(1) unsigned NOT NULL default '0000', 
	medal varchar(64) binary NOT NULL default '',
	medal_select tinyint(4) unsigned NOT NULL default '0000', 
	metempsychosis tinyint(1) unsigned NOT NULL default '0000', 
	syndicate_id int(4) unsigned NOT NULL default '0000', 
	recordmap_id int(4) unsigned NOT NULL default '0000', 
	recordx smallint(2) unsigned NOT NULL default '0000', 
	recordy smallint(2) unsigned NOT NULL default '0000', 
	account_id int(4) unsigned NOT NULL default '0000', 
	id int(4) unsigned NOT NULL auto_increment, 
	last_login int(8) unsigned NOT NULL default '0', 
	task_mask int(4) unsigned NOT NULL default '0000', 
	time_of_life int(4) unsigned NOT NULL default '0000', 
	virtue int(4) unsigned NOT NULL default '0000', 
	home_id int(4) unsigned NOT NULL default '0000', 
	title varchar(64) binary NOT NULL default '',
	title_select tinyint(4) unsigned NOT NULL default '0000', 
	lock_key int(4) unsigned NOT NULL default '0000', 
	PRIMARY KEY (id), 
	KEY index_account_id(account_id), 
	KEY index_name(name), 
	KEY index_level(level) 
) TYPE=MyISAM; 
INSERT cq_user SET id=1000000,name='RESERVE_ID_SPACE',account_id=4111222333;


DROP TABLE IF EXISTS cq_weapon_skill;
CREATE TABLE cq_weapon_skill (
  type int(4) unsigned zerofill NOT NULL default '0000',
  level tinyint(2) unsigned zerofill NOT NULL default '0000',
  exp int(4) unsigned zerofill NOT NULL default '0000',
  old_level  tinyint(2) unsigned zerofill NOT NULL default '0000',
  owner_id int(4) unsigned zerofill NOT NULL default '0000',
  id int(4) unsigned zerofill NOT NULL auto_increment,
  unlearn tinyint(1) unsigned zerofill NOT NULL default '0',
  PRIMARY KEY (id),
  KEY owner_id(owner_id)
) TYPE=MyISAM;

#
# Table structure for table 'cq_levexp'
#

DROP TABLE IF EXISTS cq_levexp;
CREATE TABLE cq_levexp (
  level int(4) unsigned zerofill NOT NULL default '0000',
  exp int(4) unsigned NOT NULL default '0',
  PRIMARY KEY  (level)
) TYPE=MyISAM;

#
# Dumping data for table 'cq_levexp'
#

INSERT INTO cq_levexp VALUES (0001,36);
INSERT INTO cq_levexp VALUES (0002,51);
INSERT INTO cq_levexp VALUES (0003,71);
INSERT INTO cq_levexp VALUES (0004,100);
INSERT INTO cq_levexp VALUES (0005,164);
INSERT INTO cq_levexp VALUES (0006,357);
INSERT INTO cq_levexp VALUES (0007,523);
INSERT INTO cq_levexp VALUES (0008,765);
INSERT INTO cq_levexp VALUES (0009,1115);
INSERT INTO cq_levexp VALUES (0010,2705);
INSERT INTO cq_levexp VALUES (0011,2841);
INSERT INTO cq_levexp VALUES (0012,3052);
INSERT INTO cq_levexp VALUES (0013,3275);
INSERT INTO cq_levexp VALUES (0014,3511);
INSERT INTO cq_levexp VALUES (0015,2461);
INSERT INTO cq_levexp VALUES (0016,3156);
INSERT INTO cq_levexp VALUES (0017,4213);
INSERT INTO cq_levexp VALUES (0018,5397);
INSERT INTO cq_levexp VALUES (0019,6396);
INSERT INTO cq_levexp VALUES (0020,4712);
INSERT INTO cq_levexp VALUES (0021,6036);
INSERT INTO cq_levexp VALUES (0022,8049);
INSERT INTO cq_levexp VALUES (0023,10300);
INSERT INTO cq_levexp VALUES (0024,12200);
INSERT INTO cq_levexp VALUES (0025,7828);
INSERT INTO cq_levexp VALUES (0026,9996);
INSERT INTO cq_levexp VALUES (0027,13291);
INSERT INTO cq_levexp VALUES (0028,16961);
INSERT INTO cq_levexp VALUES (0029,20034);
INSERT INTO cq_levexp VALUES (0030,12703);
INSERT INTO cq_levexp VALUES (0031,16183);
INSERT INTO cq_levexp VALUES (0032,21470);
INSERT INTO cq_levexp VALUES (0033,27338);
INSERT INTO cq_levexp VALUES (0034,32225);
INSERT INTO cq_levexp VALUES (0035,20031);
INSERT INTO cq_levexp VALUES (0036,25475);
INSERT INTO cq_levexp VALUES (0037,33740);
INSERT INTO cq_levexp VALUES (0038,42892);
INSERT INTO cq_levexp VALUES (0039,50478);
INSERT INTO cq_levexp VALUES (0040,37296);
INSERT INTO cq_levexp VALUES (0041,47367);
INSERT INTO cq_levexp VALUES (0042,62654);
INSERT INTO cq_levexp VALUES (0043,79550);
INSERT INTO cq_levexp VALUES (0044,93507);
INSERT INTO cq_levexp VALUES (0045,58310);
INSERT INTO cq_levexp VALUES (0046,73960);
INSERT INTO cq_levexp VALUES (0047,97707);
INSERT INTO cq_levexp VALUES (0048,123902);
INSERT INTO cq_levexp VALUES (0049,145465);
INSERT INTO cq_levexp VALUES (0050,90536);
INSERT INTO cq_levexp VALUES (0051,114712);
INSERT INTO cq_levexp VALUES (0052,151385);
INSERT INTO cq_levexp VALUES (0053,191773);
INSERT INTO cq_levexp VALUES (0054,224919);
INSERT INTO cq_levexp VALUES (0055,139669);
INSERT INTO cq_levexp VALUES (0056,176805);
INSERT INTO cq_levexp VALUES (0057,233123);
INSERT INTO cq_levexp VALUES (0058,295061);
INSERT INTO cq_levexp VALUES (0059,345764);
INSERT INTO cq_levexp VALUES (0060,223040);
INSERT INTO cq_levexp VALUES (0061,282232);
INSERT INTO cq_levexp VALUES (0062,371987);
INSERT INTO cq_levexp VALUES (0063,470642);
INSERT INTO cq_levexp VALUES (0064,551316);
INSERT INTO cq_levexp VALUES (0065,336936);
INSERT INTO cq_levexp VALUES (0066,426076);
INSERT INTO cq_levexp VALUES (0067,561214);
INSERT INTO cq_levexp VALUES (0068,709603);
INSERT INTO cq_levexp VALUES (0069,830715);
INSERT INTO cq_levexp VALUES (0070,502587);
INSERT INTO cq_levexp VALUES (0071,635204);
INSERT INTO cq_levexp VALUES (0072,836218);
INSERT INTO cq_levexp VALUES (0073,1056753);
INSERT INTO cq_levexp VALUES (0074,1236460);
INSERT INTO cq_levexp VALUES (0075,735967);
INSERT INTO cq_levexp VALUES (0076,929744);
INSERT INTO cq_levexp VALUES (0077,1223418);
INSERT INTO cq_levexp VALUES (0078,1545383);
INSERT INTO cq_levexp VALUES (0079,1807392);
INSERT INTO cq_levexp VALUES (0080,1175412);
INSERT INTO cq_levexp VALUES (0081,1484085);
INSERT INTO cq_levexp VALUES (0082,1951814);
INSERT INTO cq_levexp VALUES (0083,2464176);
INSERT INTO cq_levexp VALUES (0084,2880473);
INSERT INTO cq_levexp VALUES (0085,1655624);
INSERT INTO cq_levexp VALUES (0086,2089765);
INSERT INTO cq_levexp VALUES (0087,2747551);
INSERT INTO cq_levexp VALUES (0088,3467760);
INSERT INTO cq_levexp VALUES (0089,4052400);
INSERT INTO cq_levexp VALUES (0090,2304544);
INSERT INTO cq_levexp VALUES (0091,2908073);
INSERT INTO cq_levexp VALUES (0092,3822426);
INSERT INTO cq_levexp VALUES (0093,4823128);
INSERT INTO cq_levexp VALUES (0094,5634816);
INSERT INTO cq_levexp VALUES (0095,3234913);
INSERT INTO cq_levexp VALUES (0096,4081025);
INSERT INTO cq_levexp VALUES (0097,5362786);
INSERT INTO cq_levexp VALUES (0098,6765013);
INSERT INTO cq_levexp VALUES (0099,7901491);
INSERT INTO cq_levexp VALUES (0100,4518997);
INSERT INTO cq_levexp VALUES (0101,5699613);
INSERT INTO cq_levexp VALUES (0102,7487970);
INSERT INTO cq_levexp VALUES (0103,9443671);
INSERT INTO cq_levexp VALUES (0104,11027591);
INSERT INTO cq_levexp VALUES (0105,6136098);
INSERT INTO cq_levexp VALUES (0106,7737731);
INSERT INTO cq_levexp VALUES (0107,10163682);
INSERT INTO cq_levexp VALUES (0108,12815845);
INSERT INTO cq_levexp VALUES (0109,14962606);
INSERT INTO cq_levexp VALUES (0110,17410153);
INSERT INTO cq_levexp VALUES (0111,19151169);
INSERT INTO cq_levexp VALUES (0112,21066285);
INSERT INTO cq_levexp VALUES (0113,23172914);
INSERT INTO cq_levexp VALUES (0114,25490205);
INSERT INTO cq_levexp VALUES (0115,28039226);
INSERT INTO cq_levexp VALUES (0116,30843149);
INSERT INTO cq_levexp VALUES (0117,33927463);
INSERT INTO cq_levexp VALUES (0118,37320210);
INSERT INTO cq_levexp VALUES (0119,41052231);
INSERT INTO cq_levexp VALUES (0120,45157454);


DROP TABLE IF EXISTS cq_dropitemrule;
CREATE TABLE cq_dropitemrule(
	id int unsigned NOT NULL,　　　　　　　　　　　# 规则组ID
	RuleId int unsigned NOT NULL default '0',　　　　　# 规则ID
	Chance int unsigned NOT NULL default '0',　　　　　# 掉物品概率
	Item0 int unsigned NOT NULL default '0',　　　　　　# 以下为此概率可能掉的物品类别ID
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
# id=0为默认规则：不掉任何物品的规则——此规则不生效，正常的规则不允许id=0
#
INSERT cq_dropitemrule set id=0, ruleid=0, chance=0;

