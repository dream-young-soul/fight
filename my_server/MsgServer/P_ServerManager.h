
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// P_ServerManager.h
// paled, 2003,5.12

const int	QUERY_BASE		= 100;
const int	QUERY_STATUS		= QUERY_BASE + 1;					// INT: send id. query all status
const int	QUERY_COMMOND		= QUERY_BASE + 2;					// STRING: cmd.
const int	QUERY_MAX		= 199;

const int	ACK_BASE		= 200;
const int	ACK_TITLE			= ACK_BASE + 1;						// STRING: title.
const int	ACK_SHELLMSG		= ACK_BASE + 2;						// STRING: shell msg
const int	ACK_KERNELMSG		= ACK_BASE + 3;						// STRING: shell msg
const int	ACK_TEXT			= ACK_BASE + 4;						// STRING: text list
