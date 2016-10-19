
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// LocalEncrypt
// ÏÉ½£ÐÞ£¬2002.3.27

#ifndef	LOCALENCRYPT_H
#define	LOCALENCRYPT_H

bool		SaveUserName(LPCTSTR szUserName);		// return false: can't save.

bool		GetUserName(char* buf16);		// return false: Don't Found UserName.

bool		SavePassword(LPCTSTR szPassword);		// return false: can't save.

bool		GetPassword(char* buf16);		// return false: Don't Found password.



#endif	// LOCALENCRYPT_H
