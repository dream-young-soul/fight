
//**********************************************************
// 代码编辑器
//**********************************************************

#include <windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////
DWORD SysTimeGet(void)
{
    time_t long_time;
    time( &long_time );                /* Get time as long integer. */

    struct tm* pTime;
    pTime = localtime( &long_time ); /* Convert to local time. */

    DWORD dwTime	= pTime->tm_hour * 100 + pTime->tm_min;
    return dwTime;
}

//////////////////////////////////////////////////////////////////////
DWORD SysTimeGetEx(void)
{
    time_t long_time;
    time( &long_time );                /* Get time as long integer. */

    struct tm* pTime;
    pTime = localtime( &long_time ); /* Convert to local time. */

    DWORD dwTime	=	pTime->tm_mon * 100000000 +
                        pTime->tm_mday * 1000000 +
                        pTime->tm_hour * 10000 +
                        pTime->tm_min * 100 +
                        pTime->tm_sec;
    return dwTime;
}

/////////////////////////////////////////////////////////////////////////////
void LogSave (const char* fmt, ...)
{
    const DWORD _MAX_LOGLINE	= 1000000;
    static DWORD dwLineCount	= 0;

    time_t ltime;
    time( &ltime );

    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */

    char buffer[1024];
    vsprintf( buffer, fmt, (char*) ((&fmt) + 1) );

    char szLogName[256];
    sprintf(szLogName, "syslog/sys %d-%d %u.log", (pTime->tm_mon + 1) % 12, pTime->tm_mday, ++dwLineCount / _MAX_LOGLINE);

    FILE* fp	= fopen(szLogName, "a+");
    if (!fp)
    {
        return;
    }

    fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
    fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
void MyLogSave (const char* pszName, const char* fmt, ...)
{
    if (!pszName || !fmt)
    {
        return;
    }

    time_t ltime;
    time( &ltime );

    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */

    char buffer[1024];
    vsprintf( buffer, fmt, (char*) ((&fmt) + 1) );

    char szLogName[256];
    sprintf(szLogName, "%s %d-%d.log", pszName, (pTime->tm_mon + 1) % 12, pTime->tm_mday);

    FILE* fp	= fopen(szLogName, "a+");
    if (!fp)
    {
        return;
    }

    fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
    fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
void DebugSave (const char* fmt, ...)
{
    char szMsg[1024];
    vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
    strcat(szMsg, "\n");

    ::OutputDebugString(szMsg);
}

//////////////////////////////////////////////////////////////////////
void MsgLogSave	(const char* fmt, ...)
{
    time_t ltime;
    time( &ltime );

    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */

    char buffer[1024];
    vsprintf( buffer, fmt, (char*) ((&fmt) + 1) );

    char szLogName[256];
    sprintf(szLogName, "msglog/gm %d-%d.log", (pTime->tm_mon + 1) % 12, pTime->tm_mday);

    FILE* fp	= fopen(szLogName, "a+");
    if (!fp)
    {
        return;
    }


    fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
    fclose(fp);
}

//////////////////////////////////////////////////////////////////////
void GmLogSave	(const char* fmt, ...)
{
    time_t ltime;
    time( &ltime );

    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */

    char buffer[1024];
    vsprintf( buffer, fmt, (char*) ((&fmt) + 1) );

    char szLogName[256];
    sprintf(szLogName, "gmlog/gm %d-%d.log", (pTime->tm_mon + 1) % 12, pTime->tm_mday);

    FILE* fp	= fopen(szLogName, "a+");
    if (!fp)
    {
        return;
    }


    fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
    fclose(fp);
}

//////////////////////////////////////////////////////////////////////
int	Double2Int(double dValue)
{
    if ((int)(dValue + 0.5) > (int)dValue)
    {
        return int(dValue) + 1;
    }
    else
    {
        return int(dValue);
    }
}


//////////////////////////////////////////////////////////////////////
DWORD TimeGet(void)
{
    return timeGetTime();
}

//////////////////////////////////////////////////////////////////////
int	RandGet(int nMax, BOOL bRealRand)
{
    if (nMax <= 0)
    {
        nMax	= 1;
    }

    if (bRealRand)
    {
        ::srand( (unsigned)time( NULL ) );
    }

    return ::rand() % nMax;
}

//////////////////////////////////////////////////////////////////////
double RandomRateGet(double dRange)
{
    double pi = 3.1415926;

    int nRandom	= RandGet(999, false) + 1;
    double a	= sin(nRandom * pi / 1000);
    double b;
    if (nRandom >= 90)
    {
        b	= (1.0 + dRange) - sqrt(sqrt(a)) * dRange;
    }
    else
    {
        b	= (1.0 - dRange) + sqrt(sqrt(a)) * dRange;
    }

    return b;
}


//////////////////////////////////////////////////////////////////////
BOOL StringCheck(char* pszString)
{
    if (!pszString)
    {
        return false;
    }

    int nLen	= strlen(pszString);
    for (int i = 0; i < nLen; i++)
    {
        unsigned char c	= (unsigned char)pszString[i];
        if (c >= 0x81 && c <= 0xfe)
        {
            if (i + 1 >= nLen)
            {
                return false;
            }

            unsigned char c2	= (unsigned char)pszString[i+1];
            if (c2 < 0x40 && c2 > 0x7e && c2 < 0x80 && c2 > 0xfe)
            {
                return false;
            }
            else
            {
                i++;
            }
        }
        else
        {
            if (c == 0x80)
            {
                return false;
            }
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL NameStrCheck(char* pszString)
{
    if (!pszString)
    {
        return false;
    }

    int nLen	= strlen(pszString);
    for (int i = 0; i < nLen; i++)
    {
        unsigned char c	= pszString[i];
        switch(c)
        {
        case ' ':
        case ';':
        case ',':
        case '/':
        case '\\':
        case '=':
        case '%':
        case '@':
        case '\'':
            return false;
        }
    }

    if (strcmp(pszString, "无") == 0)
    {
        return false;
    }

    if (strstr(pszString, "※"))
    {
        return false;
    }

    if (strstr(pszString, "GM"))
    {
        return false;
    }

    if (strstr(pszString, "PM"))
    {
        return false;
    }

    if (strstr(pszString, "法轮"))
    {
        return false;
    }

    if (strstr(pszString, "台独"))
    {
        return false;
    }

    if (strstr(pszString, "独立"))
    {
        return false;
    }

    if (strstr(pszString, "民进"))
    {
        return false;
    }

    if (strstr(pszString, "宏志"))
    {
        return false;
    }

    if (strstr(pszString, "泽民"))
    {
        return false;
    }

    if (strstr(pszString, "天晴"))
    {
        return false;
    }

    if (strstr(pszString, "系统"))
    {
        return false;
    }

    if (strstr(pszString, "策划"))
    {
        return false;
    }

    if (strstr(pszString, "服务"))
    {
        return false;
    }

    if (strstr(pszString, "管理"))
    {
        return false;
    }

    if (strstr(pszString, "客服"))
    {
        return false;
    }

    if (strstr(pszString, "公告"))
    {
        return false;
    }

    return ::StringCheck(pszString);
}
