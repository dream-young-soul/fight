
# MySQL dump 8.16
#
# Host: 192.168.1.200    Database: cq
#--------------------------------------------------------
# Server version	3.23.32

#
# Table structure for table 'cq_rule'
#

DROP TABLE IF EXISTS cq_rule;
CREATE TABLE cq_rule (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  owner_id int(4) unsigned zerofill NOT NULL default '0000',
  mode varchar(255) binary NOT NULL default '',
  condition varchar(255) binary NOT NULL default '',
  result varchar(255) binary NOT NULL default '',
  possible tinyint(3) unsigned zerofill NOT NULL default '0050',
  PRIMARY KEY  (id)
) TYPE=MyISAM;
INSERT cq_rule VALUES(0,0,'!Mode(1_Dead,_)',	'keep(0),MyID(MY),!IsAlive(MY)',	'Mode(1_Dead,0)',80);
INSERT cq_rule VALUES(0,0,'Mode(1_Dead,_)',		'keep(0),MyID(MY),IsAlive(MY),!MyLeader(_)',	'Mode(2_Idle,1_Wait)',80);
INSERT cq_rule VALUES(0,0,'Mode(1_Dead,_)',		'keep(0),MyID(MY),IsAlive(MY),MyLeader(_)',		'Mode(2_Idle,2_Follow)',80);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,1_Wait)',	'keep(0),MyLeader(_)',			'Mode(2_Idle,2_Follow)',80);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)',	'keep(0),!MyLeader(_)',			'Mode(2_Idle,1_Wait)',80);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)',	'keep(0),!MyLeader(_)',				'Mode(2_Idle,1_Wait)',80);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)',	'keep(0),MyLeader(_),ItemCount(0)',	'Mode(2_Idle,2_Follow)',90);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)',	'LookTarget<ROLE>,!Enemy(_),EvilValue(ROLE,EVIL),!less(EVIL,80)','Enemy(ROLE)',90);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)',	'keep(0),Enemy(_),!ItemCount(0)',		'Mode(3_Fight,0)',90);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)',		'LookTarget<ROLE>,EvilValue(ROLE,EVIL),!less(EVIL,80)','Enemy<ROLE>',90);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)',		'keep(0),!Enemy(_),MyLeader(_)',		'Mode(2_Idle,2_Follow)',90);

INSERT cq_rule VALUES(0,0,'Mode(1_Dead,_)','MyLeader(_),percent(80)','LeaveTeam',50);
INSERT cq_rule VALUES(0,0,'Mode(1_Dead,_)','','Reborn',50);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','TalkToMe(ROLE,TALK),strstr(TALK,"你好")','TalkToName<ROLE,"你好。">',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','TalkToMe(ROLE,TALK),MyName(NAME),strstr(TALK,NAME)','TalkToName<ROLE,"你说什么？不懂。">',50);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','!MyLeader(_),TeamInvite(ROLE)','AcceptTeamInvite<ROLE>',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','!MyLeader(_),LookTarget<ROLE>,IsLeader(ROLE)','AllyTeamJoin<ROLE>',50);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','TalkToMe(ROLE,TALK),strstr(TALK,"药")','TalkToAbout<ROLE>(1_Item)',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','TalkToAbout(ROLE,1_Item),ItemCount(40)','TalkToName<ROLE,"我买了很多药。">',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','TalkToAbout(ROLE,1_Item),!ItemCount(40),ItemCount(NUM),!less(NUM,10)','TalkToName<ROLE,"我还有一些药。">',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','TalkToAbout(ROLE,1_Item),ItemCount(NUM),less(NUM,10)','TalkToName<ROLE,"快没药了，我得去买点。">',50);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','percent(50)','RandomWalk',20);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','ItemCount(NUM),less(NUM,30),ShopPos(X,Y),MyDistance(X,Y,DIST),!less_equ(DIST,8)','JumpClose(X,Y,4)',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','!ItemCount(40),ShopPos(X,Y),MyDistance(X,Y,DIST),less(DIST,8),MedicineID(TYPE)','FullMedicine(TYPE)',50);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','MyLeader(ROLE),RolePos(ROLE,X,Y),MyDistance(X,Y,DIST),!less_equ(DIST,6)','JumpClose(X,Y,2)',80);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','MyLeader<ROLE>,ShopPos(X,Y),MyDistance(X,Y,DIST),less(DIST,15)','TalkToID<ROLE,"快走吧，我们去打怪。">',20);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,1_Wait)','LookTarget<ROLE>,EvilValue(ROLE,EVIL),!less(EVIL,80)','KeepOutEnemy(16)',80);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','LookTarget<ROLE>,EvilValue(ROLE,EVIL),!less(EVIL,80),ItemCount(0)','KeepOutEnemy(16)',80);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','MyLeader<ROLE>,ItemCount(0),percent(50)','TalkToID<ROLE,"我没药了，要回去买药。">',50);


INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','Enemy<ROLE>,RolePos(ROLE,X,Y),MyDistance(X,Y,DIST)','EnemyDistance<ROLE>(DIST)',90);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','EnemyDistance<ROLE>(DIST),!CloseEnemy(_,_)','CloseEnemy(ROLE,DIST)',90);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','EnemyDistance<ROLE>(DIST),CloseEnemy(ROLE2,DIST2),less(DIST,DIST2)','CloseEnemy(ROLE,DIST)',90);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','CloseEnemy(ROLE,DIST),AttackDistance(ROLE,ATK),!less_equ(DIST,ATK),RolePos(ROLE,X,Y)','JumpClose(X,Y,ATK)',90);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','CloseEnemy(ROLE,DIST),AttackDistance(ROLE,ATK),less_equ(DIST,ATK)','AttackRole(ROLE)',90);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','TalkToMe(ROLE,TALK),strstr(TALK,"来"),MyLeader(ROLE),RolePos(ROLE,X,Y)','JumpClose(X,Y,2)',50);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','TalkToMe(ROLE,TALK),strstr(TALK,"走"),MyLeader(ROLE),RolePos(ROLE,X,Y)','JumpClose(X,Y,2)',50);

INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','MyLife(LIFE),less(LIFE,12000),!ItemCount(0),MedicineID(TYPE)','UseItem(TYPE)',100);











/*
# INSERT cq_rule VALUES(0,0,'Mode(2_Idle,1_Wait)','TeamInvite(ROLE),CoolValue(ROLE,VAL),less(50,VAL)','AcceptTeamInvite<ROLE>',50);
# INSERT cq_rule VALUES(0,0,'Mode(2_Idle,1_Wait)','LookTarget<ROLE>,IsLeader(ROLE),CoolValue(ROLE,VAL),less(50,VAL)','AllyTeamJoin<ROLE>',50);
# INSERT cq_rule VALUES(0,0,'Mode(2_Idle,1_Wait)','TeamInvite<ROLE>,CoolValue(ROLE,VAL),less(VAL,50),percent(20)','TalkToID<ROLE,"别烦我。">',50);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','ItemCount(NUM),less(NUM,10),CityPos(X,Y),GetDistance(X,Y,DIST),!less(DIST,5),ShopMap(ID)','ChangeMapTo<ID,X,Y>',20);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','MyLeader(ROLE),Distance(ROLE,NUM),less(15,NUM)','MoveLookTarget(ROLE)',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','MyLeader<ROLE>,ItemCount(NUM),less(NUM,5),ShopDistance(DIST),less(5,DIST),percent(50)','TalkTo<ROLE,"我没药了，要回去买药。">',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,2_Follow)','ShopDistance(DIST),less(DIST,5),percent(20)','LeaveTeam',20);

INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','MyLeader(ROLE),Distance(ROLE,NUM),less(15,NUM),percent(50)','MoveLookTarget(ROLE)',50);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','LookTarget<ROLE>,EvilValue(ROLE,EVIL),less(50,EVIL),!AttackEnemy(ROLE,_),Distance(ROLE,DIST)','AttackEnemy(ROLE,DIST)',80);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','LookTarget<ROLE>,EvilValue(ROLE,EVIL),less(50,EVIL),AttackEnemy(ROLE,DIST0),Distance(ROLE,DIST),less(DIST,DIST0)','AttackEnemy(ROLE,DIST)',80);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','AttackEnemy(ROLE,DIST)','MoveCloseTarget(ROLE)',100);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','AttackEnemy(ROLE,DIST)','AttackTarget(ROLE)',100);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','MyLife(LIFE),less(LIFE,1000),ItemCount(NUM),less(0,NUM)','EatMedicine',100);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','MyLife(LIFE),less(LIFE,1000),ItemCount(0),IsTeamOK','Mode(2_Idle,2_Follow)',80);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','MyLife(LIFE),less(LIFE,1000),ItemCount(0),!IsTeamOK','Mode(2_Idle,1_Wait)',80);
INSERT cq_rule VALUES(0,0,'Mode(3_Fight,_)','DangerValue(DANG),!less(DANG,80)','JumpToSafePos',100);

INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','MyLife(LIFE),less(LIFE,100),ItemCount(0)','ChangeMapToShop',50);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','DangerValue(DANG),!less(DANG,10)','JumpToSafePos',100);
INSERT cq_rule VALUES(0,0,'Mode(2_Idle,_)','DangerValue(DANG),!less(DANG,1)','KeepOutEnemy(10)',80);
*/


#
# Table structure for table 'cq_fact'
#

DROP TABLE IF EXISTS cq_fact;
CREATE TABLE cq_fact (
  id int(4) unsigned zerofill NOT NULL auto_increment,
  owner_id int(4) unsigned zerofill NOT NULL default '0000',
  fact varchar(255) binary NOT NULL default '',
  priority int(4) unsigned zerofill NOT NULL default '0000',
  datetime int(4) unsigned zerofill NOT NULL default '0000',
  durable int(4) unsigned zerofill NOT NULL default '0000',
  PRIMARY KEY  (id),
  KEY owner_id(owner_id)
) TYPE=MyISAM;
INSERT cq_fact VALUES(0,1002420,'Mode(2_Idle,1_Wait)',20,0,2147483647);
INSERT cq_fact VALUES(0,1002420,'ShopMap(1000)',0,0,2147483647);
INSERT cq_fact VALUES(0,1002420,'ShopPos(511,630)',0,0,2147483647);
INSERT cq_fact VALUES(0,1002420,'MedicineID(1002010)',0,0,2147483647);
INSERT cq_fact VALUES(0,1002420,'MyName("石榴裙")',0,0,2147483647);
INSERT cq_fact VALUES(0,1002420,'MyID(1002420)',0,0,2147483647);

DELETE FROM cq_user WHERE id=1002420;
INSERT cq_user SET id=1002420,account_id=0,level=60,name='石榴裙[PM]',lookface=2012001,hair=510,money=999,force=1111,speed=555,health=3333,life=55755,profession=11,recordmap_id=1000,recordx=515,recordy=636;
