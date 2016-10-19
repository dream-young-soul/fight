
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _BASEFUNC_H
#define _BASEFUNC_H

#pragma	warning(disable:4786)
#include <time.h>
#include "common.h"
#include "BaseType.h"
#include <vector>
using namespace std;


String	DumpBuffer(const char* buf, int nSize);
void	DateTime(char* buf20, time_t tInput = 0);		// 0 : curr time
bool	FormatDateTime(char* buf, const char* szFormat, time_t tInput = 0);		// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
void	DateTimeStamp(char* buf15, time_t tInput = 0);		// 0 : curr time
int		DateStamp(time_t tTime = 0);
int		DateStampPass(int nDate, int nDays);	// return -1: error
bool	IsActiveTime(time_t tCurr, unsigned long nFormat);
bool	IsTimeRange(unsigned long nBegin, unsigned long nEnd, time_t tCurr = 0);
inline long	GetCurrTime()				{ return time(NULL); }
char*	StrStrCh(const char* string, const char* strCharSet);

enum TIME_TYPE { TIME_MILLISECOND = 0, TIME_SECOND, TIME_MINUTE, TIME_DAY, TIME_DAYTIME, TIME_STAMP };

DWORD	TimeGet(TIME_TYPE type = TIME_MILLISECOND);
//DWORD	TimeGet(void);
DWORD	SysTimeGet(void);
DWORD	SysTimeGetEx(void);

inline long	GetClock()		{ return clock(); }
int		RandGet		(int nMax, BOOL bReset = false);
void	DDALineEx(int x0, int y0, int x1, int y1, vector<POINT>& vctPoint);

void	LogSave		(const char* fmt, ...);
void	MyLogSave	(const char* pszName, const char* fmt, ...);

void	DebugSave	(const char* fmt, ...);
void	GmLogSave	(const char* fmt, ...);
void	MsgLogSave	(const char* pszFileName, const char* fmt, ...);

BOOL	StringCheck	(char* pszString, BOOL bNewLineEnable = false);
BOOL	TalkStrCheck(char* pszString, BOOL bNewLineEnable = false);
BOOL	DisableWordCheck(char* pszString, BOOL bNewLineEnable = false);
BOOL	NameStrCheck(char* pszString);

double	RandomRateGet	(double dRange);
int		Double2Int		(double dValue);

inline int	AddToTop(int nData, int nAdd, int nTop) { int res = nData + nAdd; if (nAdd > 0 && res < nData || res > nTop) { return nTop; } else { return res; } }

/////////////////////////////////////////////////////////////////////////////
class CTimeCostChk
{
public:
    CTimeCostChk(const char* pszStr, DWORD tmChk) : m_tmChk(tmChk)
    {
        m_tmBegin	= ::TimeGet();
        m_pszStr	= pszStr;
    }

    ~CTimeCostChk(void)
    {
        DWORD tmCost = ::TimeGet() - m_tmBegin;
        if (tmCost > m_tmChk)
        {
            if (m_pszStr)
            {
                ::LogSave("Overtime (%u) when chk %s", tmCost, m_pszStr);
            }
            else
            {
                ::LogSave("Overtime (%u)", tmCost);
            }
        }
    }

private:
    DWORD m_tmChk;
    const char* m_pszStr;

    DWORD m_tmBegin;
};

/////////////////////////////////////////////////////////////////////////////
template<class T>
inline T	CutOverflow(T x, T y) { return (x <= y) ? x : y; }
template<class T>
inline T	CutTrail(T x, T y) { return (x >= y) ? x : y; }
template<class T>
inline T	CutRange(T n, T min, T max) { return (n < min) ? min : ((n > max) ? max : n); }

/////////////////////////////////////////////////////////////////////////////
inline unsigned int	ExchangeShortBits(unsigned long nData, int nBits)
{
    ASSERT(nBits >= 0 && nBits < 16);
    nData	&= 0xFFFF;
    return ((nData >> nBits) | (nData << (16 - nBits))) & 0xFFFF;
}

inline unsigned int	ExchangeLongBits(unsigned long nData, int nBits)
{
    ASSERT(nBits >= 0 && nBits < 32);
    return (nData >> nBits) | (nData << (32 - nBits));
}

//////////////////////////////////////////////////////////////////////
inline int pos2index(int x, int y, int cx, int cy) { return (x + y * cx); }
inline int index2x(int idx, int cx, int cy) { return (idx % cy); }
inline int index2y(int idx, int cx, int cy) { return (idx / cy); }
inline void pos2lt(POINT* pTarget, const POINT& pos, int nRange) { pTarget->x = pos.x - nRange; pTarget->y = pos.y - nRange; }
inline void posadd(POINT* pTarget, int nOffsetX, int nOffsetY) { pTarget->x += nOffsetX; pTarget->y += nOffsetY; }
inline void possub(POINT* pTarget, int nOffsetX, int nOffsetY) { pTarget->x -= nOffsetX; pTarget->y -= nOffsetY; }


#endif

