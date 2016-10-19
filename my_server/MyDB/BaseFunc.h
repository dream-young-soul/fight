
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _BASEFUNC_H
#define _BASEFUNC_H

DWORD	TimeGet(void);
DWORD	SysTimeGet(void);
DWORD	SysTimeGetEx(void);

int		RandGet		(int nMax, BOOL bRealRand = false);

void	LogSave		(const char* fmt, ...);
void	MyLogSave	(const char* pszName, const char* fmt, ...);

void	DebugSave	(const char* fmt, ...);
void	GmLogSave	(const char* fmt, ...);
void	MsgLogSave	(const char* fmt, ...);

BOOL	StringCheck	(char* pszString);
BOOL	NameStrCheck(char* pszString);

double	RandomRateGet	(double dRange);
int		Double2Int		(double dValue);

#endif
