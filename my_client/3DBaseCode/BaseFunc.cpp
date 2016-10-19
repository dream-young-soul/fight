
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//////////////////////////////////////////////////////////////////////
// Purpose:		base function for game
//
// WriteBy:		blacknull	10-16-2002
//
// ModifyBy:	blacknull	10-16-2002
//
// CopyRight:	Acme Entertrainment, copyright 2002
//////////////////////////////////////////////////////////////////////

#include "basefunc.H"

// global...
HWND g_hGameWnd = NULL;
BOOL g_bLocalVersion = false;

#ifdef _ANALYSIS_ON
DWORD g_dwIniGetAmount = 0;
DWORD g_dwIniSearchLineAmount = 0;
DWORD g_dwIniGetAmountFrame = 0;
DWORD g_dwIniSearchLineAmountFrame = 0;
#endif
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
BOOL IniStrGet(const char* pszFileName, const char* pszTitle, const char* pszSubTitle, char* pszStr, BOOL bCritical/*=false*/)
{
#ifdef _ANALYSIS_ON
    g_dwIniGetAmount ++;
    g_dwIniGetAmountFrame++;
#endif
    if (!pszFileName || !pszTitle || !pszSubTitle || !pszStr)
    {
        return false;
    }
    FILE* fp = fopen(pszFileName, "r");
    if (!fp)
    {
        return false;
    }
    char szSection[256]	= "";
    sprintf(szSection, "[%s]", pszTitle);
    bool bSucFound	= false;
    while(true)
    {
        char szLine[1024]	= "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
#ifdef _ANALYSIS_ON
        g_dwIniSearchLineAmount ++;
        g_dwIniSearchLineAmountFrame++;
#endif
        if (0 != _stricmp(szSection, szLine))
        {
            continue;
        }
        // section found
        char szFormat[256] = "";
        sprintf(szFormat, "%s=%s\n", pszSubTitle, "%s");
        while (true)
        {
            char szLine[1024]	= "";
#ifdef _ANALYSIS_ON
            g_dwIniSearchLineAmount ++;
            g_dwIniSearchLineAmountFrame++;
#endif
            if (EOF == fscanf(fp, "%s\n", szLine))
            {
                goto out;
            }
            if (strrchr(szLine, '[') && strrchr(szLine, ']'))
            {
                goto out;
            }
            if (sscanf(szLine, szFormat, pszStr))
            {
                bSucFound = true;
                goto out;
            }
        }
    }
out:
    fclose(fp);
    if (!bSucFound)
    {
        if (bCritical)
        {
            char szMsg[1024] = "";
            sprintf(szMsg, "Error: [%s] %s not found in %s.", pszTitle, pszSubTitle, pszFileName);
            ::MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
            ::LogMsg(szMsg);
        }
        else
        {
            ::DebugMsg("Error: [%s] %s not found in %s.", pszTitle, pszSubTitle, pszFileName);
        }
    }
    return bSucFound;
}

//////////////////////////////////////////////////////////////////////
BOOL IniDataGet(const char* pszFileName, const char* pszTitle, const char* pszSubTitle, int& iData, BOOL bCritical/*=false*/)
{
    if (!pszFileName || !pszTitle || !pszSubTitle)
    {
        return false;
    }
#ifdef _ANALYSIS_ON
    g_dwIniGetAmount ++;
    g_dwIniGetAmountFrame++;
#endif
    FILE* fp = fopen(pszFileName, "r");
    if (!fp)
    {
        return false;
    }
    char szSection[256]	= "";
    sprintf(szSection, "[%s]", pszTitle);
    bool bSucFound	= false;
    while(true)
    {
        char szLine[1024]	= "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
#ifdef _ANALYSIS_ON
        g_dwIniSearchLineAmount ++;
        g_dwIniSearchLineAmountFrame++;
#endif
        if (0 != _stricmp(szSection, szLine))
        {
            continue;
        }
        // section found
        char szFormat[256] = "";
        sprintf(szFormat, "%s=%s\n", pszSubTitle, "%d");
        while (true)
        {
#ifdef _ANALYSIS_ON
            g_dwIniSearchLineAmount ++;
            g_dwIniSearchLineAmountFrame++;
#endif
            char szLine[1024]	= "";
            if (EOF == fscanf(fp, "%s\n", szLine))
            {
                goto out;
            }
            if (strrchr(szLine, '[') && strrchr(szLine, ']'))
            {
                goto out;
            }
            if (sscanf(szLine, szFormat, &iData))
            {
                bSucFound = true;
                goto out;
            }
        }
    }
out:
    fclose(fp);
    if (!bSucFound)
    {
        if (bCritical)
        {
            char szMsg[1024] = "";
            sprintf(szMsg, "Error: [%s] %s not found in %s.", pszTitle, pszSubTitle, pszFileName);
            ::MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
            ::LogMsg(szMsg);
        }
        else
        {
            ::DebugMsg("Error: [%s] %s not found in %s.", pszTitle, pszSubTitle, pszFileName);
        }
    }
    return bSucFound;
}

//////////////////////////////////////////////////////////////////////
BOOL TxtStrGet(const char* pszFileName, const char* pszTitle, char* pszStr, BOOL bCritical/*=false*/)
{
    if (!pszFileName || !pszTitle || !pszStr)
    {
        return false;
    }
#ifdef _ANALYSIS_ON
    g_dwIniGetAmount ++;
    g_dwIniGetAmountFrame++;
#endif
    FILE* fp = fopen(pszFileName, "r");
    if (!fp)
    {
        return false;
    }
    char szFormat[256]	= "";
    sprintf(szFormat, "%s=%s", pszTitle, "%s");
    bool bSucFound	= false;
    while(true)
    {
#ifdef _ANALYSIS_ON
        g_dwIniSearchLineAmount ++;
        g_dwIniSearchLineAmountFrame++;
#endif
        char szLine[1024]	= "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        if (sscanf(szLine, szFormat, pszStr))
        {
            bSucFound	= true;
            break;
        }
    }
    fclose(fp);
    if (!bSucFound)
    {
        if (bCritical)
        {
            ::LogMsg("Error: %s not found in %s.", pszTitle, pszFileName);
        }
        else
        {
            ::DebugMsg("Error: %s not found in %s.", pszTitle, pszFileName);
        }
    }
    return bSucFound;
}

//////////////////////////////////////////////////////////////////////
BOOL TxtDataGet(const char* pszFileName, const char* pszTitle, int& nData, BOOL bCritical/*=false*/)
{
    if (!pszFileName || !pszTitle)
    {
        return false;
    }
#ifdef _ANALYSIS_ON
    g_dwIniGetAmount ++;
    g_dwIniGetAmountFrame++;
#endif
    FILE* fp = fopen(pszFileName, "r");
    if (!fp)
    {
        return false;
    }
    char szFormat[256]	= "";
    sprintf(szFormat, "%s=%s\n", pszTitle, "%d");
    bool bSucFound	= false;
    while(true)
    {
#ifdef _ANALYSIS_ON
        g_dwIniSearchLineAmount ++;
        g_dwIniSearchLineAmountFrame++;
#endif
        char szLine[1024] = "";
        if (EOF == fscanf(fp, "%s\n", szLine))
        {
            break;
        }
        if (sscanf(szLine, szFormat, &nData))
        {
            bSucFound	= true;
            break;
        }
    }
    fclose(fp);
    if (bCritical && !bSucFound)
    {
        ::LogMsg("Error: %s not found in %s.", pszTitle, pszFileName);
    }
    return bSucFound;
}

//////////////////////////////////////////////////////////////////////
DWORD TimeGet(void)
{
    return timeGetTime();
}

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

/////////////////////////////////////////////////////////////////////////////
void LogMsg (const char* fmt, ...)
{
    if (!fmt)
    {
        return;
    }
    char szFileName[]	= "sys";
    time_t ltime;
    time( &ltime );
    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */
    char buffer[1024];
    vsprintf( buffer, fmt, (char*) ((&fmt) + 1) );
    char szLogName[256];
    sprintf(szLogName, "%s %d-%d.log", szFileName, (pTime->tm_mon + 1) % 12, pTime->tm_mday);
    FILE* fp	= fopen(szLogName, "a+");
    if (!fp)
    {
        return;
    }
    fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
    fclose(fp);
}

/////////////////////////////////////////////////////////////////////////////
void DebugMsg (const char* fmt, ...)
{
    char szMsg[1024];
    vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
    strcat(szMsg, "\n");
    ::OutputDebugString(szMsg);
}

/////////////////////////////////////////////////////////////////////////////
void ErrorOut (const char* fmt, ...)
{
    static BOOL bShowing	= false;
    if (bShowing)
    {
        return;
    }
    char szMsg[1024];
    vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
    bShowing	= true;
    ::MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
    bShowing	= false;
    //	exit(-1);
}

//////////////////////////////////////////////////////////////////////
void ErrorMsg(const char* fmt, ...)
{
    static BOOL bShowing	= false;
    if (bShowing)
    {
        ::abort();
        return;
    }
    char szMsg[1024];
    vsprintf( szMsg, fmt, (char*) ((&fmt) + 1) );
    bShowing	= true;
    ::MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
    bShowing	= false;
}

//////////////////////////////////////////////////////////////////////
int	RandGet(int nMax, BOOL bRealRand)
{
    if (nMax == 0)
    {
        return 0;
    }
    if (bRealRand)
    {
        ::srand( (unsigned)TimeGet() );
    }
    return ::rand() % nMax;
}


//////////////////////////////////////////////////////////////////////
void	RepairString	(char* pszString)
{
    if (!pszString)
    {
        return;
    }
    int nLen	= strlen(pszString);
    for (int i = 0; i < nLen; i++)
    {
        unsigned char c	= (unsigned char)pszString[i];
        if (c >= 0x81 && c <= 0xfe)
        {
            // the last char is cut improperly
            if (i + 1 >= nLen)
            {
                pszString[i] = '\0';
                return;
            }
            // the second char is outof range
            unsigned char c2	= (unsigned char)pszString[i+1];
            if (c2 < 0x40 && c2 > 0x7e && c2 < 0x80 && c2 > 0xfe)
            {
                pszString[i] = '\0';
                return;
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
                pszString[i] = '\0';
                return;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
BOOL StringCheck(char* pszString)
{
    if (!pszString)
    {
        return false;
    }
    if (strstr(pszString, "£ "))
    {
        return false;
    }
    if (strstr(pszString, "¡¡"))
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
            else
            {
                bool bNewLineEnable = true;
                if (bNewLineEnable)
                {
                    if (c < ' ' && c != 0x0A && c != 0x0D)
                    {
                        return false;
                    }
                }
                else
                {
                    if (c < ' ')
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
    /*	if (!pszString)
    		return false;

    	int nLen	=strlen(pszString);
    	for (int i=0; i < nLen; i++)
    	{
    		unsigned char c	=(unsigned char)pszString[i];
    		if (c >= 0x81 && c <= 0xfe)
    		{
    			if (i+1 >= nLen)
    				return false;

    			unsigned char c2	=(unsigned char)pszString[i+1];
    			if (c2 < 0x40 && c2 > 0x7e && c2 < 0x80 && c2 > 0xfe)
    				return false;
    			else
    				i++;
    		}
    		else
    		{
    			if (c==0x80)
    				return false;
    		}
    	}

    	return true;*/
}
//////////////////////////////////////////////////////////////////////
// Ì½²â×Ö·û´®ÖÐµÚidx¸ö×Ö·ûÊÇ·ñÊÇDBCSµÄLEAD BYTE
BOOL IsDBCSLeadByte	(const char* pszString, int idx)
{
    if (!pszString || idx >= (int)strlen(pszString))
    {
        return FALSE;
    }
    BOOL bLeadByte = FALSE;
    for (int i = 0; i < idx; i++)
    {
        if (bLeadByte)
        {
            bLeadByte = FALSE;
            continue;
        }
        bLeadByte = ::IsDBCSLeadByte(pszString[i]);
    }
    return bLeadByte ? FALSE : IsDBCSLeadByte(pszString[idx]);
}

//////////////////////////////////////////////////////////////////////
void	ReplaceString(char* pszString, char cFind, char cReplace)
{
    if (!pszString)
    {
        return;
    }
    DWORD dwLength = strlen(pszString);
    if (dwLength > 1024)
    {
        return;
    }
    for(DWORD i = 0; i < dwLength; i ++)
    {
        if (pszString[i] == cFind)
        {
            pszString[i] = cReplace;
        }
    }
}
//////////////////////////////////////////////////////////////////////
DWORD GetDate()
{
    time_t ltime;
    time(&ltime);
    struct tm* pTime;
    pTime = localtime( &ltime );
    return pTime->tm_mday;
}


//////////////////////////////////////////////////////////////////////
// mouse functions
//////////////////////////////////////////////////////////////////////
static CMouseInfo infoMouse;

void MouseInit(void)
{
    infoMouse.iEvent	= _MOUSE_NONE;
}

void MouseSet(int x, int y, int event)
{
    infoMouse.iEvent	= event;
    infoMouse.iPosX		= x;
    infoMouse.iPosY		= y;
}

int MouseCheck(int& iMouseX, int& iMouseY)
{
    iMouseX	= infoMouse.iPosX;
    iMouseY	= infoMouse.iPosY;
    return infoMouse.iEvent;
}

void MouseProcess(void)
{
    infoMouse.iEvent	= _MOUSE_NONE;
}

#ifdef WM_MY_MESSAGE
void PostCmd(DWORD dwCommand, int nData)
{
    if (g_hGameWnd)
    {
        ::PostMessage(g_hGameWnd, WM_MY_MESSAGE, dwCommand, nData);
    }
}
#endif

