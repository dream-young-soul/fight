# MySQL dump 8.16
#
# Host: 192.168.1.175    Database: games
#--------------------------------------------------------
# Server version	3.23.42-nt

#
# Table structure for table 'account'
#

CREATE TABLE account (
 	id int(4) unsigned zerofill NOT NULL  auto_increment, 
	name char(32) binary NOT NULL  , 
	sex int(4) unsigned zerofill NOT NULL  , 
	age int(4) unsigned zerofill NOT NULL  , 
	phone char(16) binary NOT NULL  , 
	email char(128) binary NOT NULL  , 
	address char(128) binary NOT NULL  , 
	idnumber char(32) binary NOT NULL  , 
	password char(16) binary NOT NULL   ,
	PRIMARY KEY (id),
	KEY ix_name (name)	
) TYPE=MyISAM;


#
# Table structure for table 'fee'
#

CREATE TABLE fee (
  time_stamp char(14) binary NOT NULL default '',
  server_name char(15) binary NOT NULL default '',
  user_name char(15) binary NOT NULL default '',
  user_id int(4) unsigned zerofill NOT NULL default '0000',
  start_time char(19) binary NOT NULL default '',
  last_point char(19) binary NOT NULL default '',
  end_time char(19) binary NOT NULL default '',
  fee_name char(15) binary NOT NULL default '',
  fee_id int(4) unsigned zerofill NOT NULL default '0000',
  fee_type char(15) binary NOT NULL default '',
  point int(4) unsigned zerofill NOT NULL default '0000',
  end_type char(15) binary NOT NULL default '',
  ip_addr char(15) binary NOT NULL default ''
) TYPE=MyISAM;

# NOTE: time_stamp mast NOT be PRIMARY KEY.
# you can add some INDEX for this table.



#
# Table structure for table 'logon'
#

CREATE TABLE logon (
  time_stamp char(14) binary NOT NULL default '',
  account char(15) binary NOT NULL default '',
  account_id int(4) unsigned zerofill NOT NULL default '0000',
  fee_type int(4) unsigned zerofill NOT NULL default '0000',
  server char(15) binary NOT NULL default '',
  authen int(4) unsigned zerofill NOT NULL default '0000',
  client_ip char(15) binary NOT NULL default '',
  fee_account char(15) binary NOT NULL default '',
  notify char(15) binary NOT NULL default '',
  rejoin tinyint(1) unsigned zerofill NOT NULL default '0000'
) TYPE=MyISAM;



#
# Table structure for table 'serial'
#

CREATE TABLE serial (
  time_stamp char(14) binary NOT NULL default '',
  account_id int(4) unsigned zerofill NOT NULL default '0000',
  serial int(4) unsigned zerofill NOT NULL default '0000',
  serial_str varchar(15) binary NOT NULL default ''
) TYPE=MyISAM;

#表名在config.ini中[Special]配置POINTTABLE配置项指定
#
# Table structure for table 'point001'
#
#通过account_id与account关联

CREATE TABLE point001 (
 	account_id int(4) unsigned zerofill NOT NULL  , 
	type int(4) unsigned zerofill NOT NULL  , 
	point int(4) unsigned zerofill NOT NULL  , 
	pointtime int(4) unsigned zerofill NOT NULL  , 
	online int(4) unsigned zerofill NOT NULL  , 
	licence int(4) unsigned NOT NULL  , 
	netbar_ip char(15)   , 
	ip_mask char(15)    ,
	PRIMARY KEY (account_id)	
) TYPE=MyISAM;


