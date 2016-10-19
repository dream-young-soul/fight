
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ChatLog.cpp: implementation of the CChatLog class.
//
//////////////////////////////////////////////////////////////////////

#include "ChatLog.h"
#include "Hero.h"
#include "GameMsg.h"
#include "NetMsg.h"
#include <direct.h>
#include "GameDataSet.h"
//extern CHAR	g_szServerName[256];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatLog::CChatLog()
{
    strcpy(m_szCurrentLogFile, "ERROR");
    m_bLog = true;
}

CChatLog::~CChatLog()
{
    this->DestroyLogFileSet();
    this->DestroyLogLineSet();
    this->DestroyLeaveWordLogLineSet();
    this->ClearLogBuffer();
}

//-------------------------------------------------------------------
void CChatLog::CheckDirectory()
{
    // the char log directory is LOG/SERVERNAME/USERNAME/CHAT/
    char szDirectory[128] = "LOG";
    _mkdir(szDirectory);
    sprintf(szDirectory, "LOG/%s", g_szServerName);
    _mkdir(szDirectory);
    sprintf(szDirectory, "LOG/%s/%s", g_szServerName, g_objHero.GetName());
    _mkdir(szDirectory);
    sprintf(szDirectory, "LOG/%s/%s/CHAT",  g_szServerName, g_objHero.GetName());
    _mkdir(szDirectory);
}
//-------------------------------------------------------------------
void CChatLog::Log(CGameMsgUnit* pMsgUnit)
{
    if (!pMsgUnit)
    {
        return;
    }
    // the file name is YEAR_MONTH_DAY.LOG
    time_t ltime;
    time( &ltime );
    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */
    //-------------------------------------------------------
    // search ...
    BOOL bFound = false;
    int nAmount = m_setLogFile.size();
    for(int k = 0; k < nAmount; k++)
    {
        ChatLogFile* pInfo = m_setLogFile[k];
        if (pInfo && pInfo->nDay == pTime->tm_mday && pInfo->nMonth == (pTime->tm_mon + 1) % 12 && pInfo->nYear == 2000 + pTime->tm_year % 100)
        {
            bFound = true;
        }
    }
    if (!bFound)
    {
        // add to filename log
        ChatLogFile* InfoLogFile = new ChatLogFile;
        MYASSERT(InfoLogFile);
        InfoLogFile->nDay = pTime->tm_mday;
        InfoLogFile->nMonth = (pTime->tm_mon + 1) % 12;
        InfoLogFile->nYear = 2000 + pTime->tm_year % 100;
        char szLogFile[256];
        sprintf(szLogFile, "%d_%d_%d.log", InfoLogFile->nYear, InfoLogFile->nMonth, InfoLogFile->nDay);
        strcpy(InfoLogFile->szFileName, szLogFile);
        m_setLogFile.push_back(InfoLogFile);
        ::PostCmd(CMD_FLASHCHATLOGFILELIST);
    }
    //-------------------------------------------------------
    char szTemp[_MAX_WORDSSIZE * 2] = "aa";
    DWORD dwTimeBak = pMsgUnit->m_dwTime;
    pMsgUnit->m_dwTime = pTime->tm_hour * 100 + pTime->tm_min;
    pMsgUnit->GetText(szTemp);
    pMsgUnit->m_dwTime = dwTimeBak;
    char szLogFile[256];
    sprintf(szLogFile, "%d_%d_%d.log", 2000 + pTime->tm_year % 100, (pTime->tm_mon + 1) % 12, pTime->tm_mday);
    if (strcmp(m_szCurrentLogFile, szLogFile) == 0)
    {
        ChatLogLine* pLine = new ChatLogLine;
        MYASSERT(pLine);
        strcpy(pLine->szSender, pMsgUnit->m_szSender);
        strcpy(pLine->szTxt, szTemp);
        m_setLogLine.push_back(pLine);
        ::PostCmd(CMD_FLASHCURRENTCHATLOGLINELIST);
    }
    if (m_bLog)
    {
        // add to save buffer
        ChatLogLineBuffer* pBuffer = new ChatLogLineBuffer;
        if (pBuffer)
        {
            pBuffer->nDay = pTime->tm_mday;
            pBuffer->nYear = 2000 + pTime->tm_year % 100;
            pBuffer->nMonth = (pTime->tm_mon + 1) % 12;
            strcpy(pBuffer->szName, pMsgUnit->m_szSender);
            strcpy(pBuffer->szMsg, szTemp);
            m_setLogBuffer.push_back(pBuffer);
        }
    }
}
//-------------------------------------------------------------------
void CChatLog::DestroyLogFileSet()
{
    int nAmount = m_setLogFile.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        ChatLogFile* pInfo = m_setLogFile[i];
        SAFE_DELETE(pInfo);
    }
    m_setLogFile.clear();
}
//-------------------------------------------------------------------
void CChatLog::DestroyLeaveWordLogLineSet()
{
    int nAmount = m_setLeaveWordLogLine.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        ChatLogLine* pLine = m_setLeaveWordLogLine[i];
        SAFE_DELETE(pLine);
    }
    m_setLeaveWordLogLine.clear();
}
//-------------------------------------------------------------------
void CChatLog::DestroyLogLineSet()
{
    int nAmount = m_setLogLine.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        ChatLogLine* pLine = m_setLogLine[i];
        SAFE_DELETE(pLine);
    }
    m_setLogLine.clear();
}
//-------------------------------------------------------------------
void CChatLog::SearchFile()
{
    this->DestroyLogFileSet();
    WIN32_FIND_DATA FindFileData;
    char szDirectory[128];
    sprintf(szDirectory, "LOG\\%s\\%s\\CHAT\\*.LOG", g_szServerName, g_objHero.GetName());
    // find first ...
    HANDLE hFind = FindFirstFile(szDirectory, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }
    if ((strcmp(FindFileData.cFileName, ".") != 0) &&
            (strcmp(FindFileData.cFileName, "..") != 0))
    {
        int	nYear, nMonth, nDay;
        char szFileName[_MAX_PATH] = "";
        if (3 == sscanf(FindFileData.cFileName, "%d_%d_%d.log", &nYear, &nMonth, &nDay))
        {
            ChatLogFile* InfoLogFile = new ChatLogFile;
            MYASSERT(InfoLogFile);
            InfoLogFile->nDay = nDay;
            InfoLogFile->nMonth = nMonth;
            InfoLogFile->nYear = nYear;
            strcpy(InfoLogFile->szFileName, FindFileData.cFileName);
            m_setLogFile.push_back(InfoLogFile);
        }
    }
    while(true)
    {
        if (FindNextFile(hFind, &FindFileData) == 0)
        {
            FindClose(hFind);
            break;
        }
        if ((strcmp(FindFileData.cFileName, ".") != 0) &&
                (strcmp(FindFileData.cFileName, "..") != 0))
        {
            int	nYear, nMonth, nDay;
            char szFileName[_MAX_PATH] = "";
            if (3 == sscanf(FindFileData.cFileName, "%d_%d_%d.log", &nYear, &nMonth, &nDay))
            {
                ChatLogFile* InfoLogFile = new ChatLogFile;
                MYASSERT(InfoLogFile);
                InfoLogFile->nDay = nDay;
                InfoLogFile->nMonth = nMonth;
                InfoLogFile->nYear = nYear;
                strcpy(InfoLogFile->szFileName, FindFileData.cFileName);
                m_setLogFile.push_back(InfoLogFile);
            }
        }
    }
    ::PostCmd(CMD_FLASHCHATLOGFILELIST);
}
//-------------------------------------------------------------------
int CChatLog::GetLogFileAmount()
{
    return m_setLogFile.size();
}
//-------------------------------------------------------------------
ChatLogFile* CChatLog::GetLogFileByIndex(int nIndex)
{
    int nAmount = m_setLogFile.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setLogFile[nIndex];
}
//-------------------------------------------------------------------
void CChatLog::LoadLog(char* pFileName)
{
    this->ClearLogBuffer();
    this->DestroyLogLineSet();
    if (!pFileName)
    {
        return;
    }
    strcpy(m_szCurrentLogFile, pFileName);
    char szFullFilePath[_MAX_PATH];
    sprintf(szFullFilePath, "LOG\\%s\\%s\\CHAT\\%s", g_szServerName, g_objHero.GetName(), pFileName);
    FILE* fp = fopen(szFullFilePath, "rb");
    if (!fp)
    {
        return;
    }
    char szHeroName[_MAX_NAMESIZE];
    strcpy(szHeroName, g_objHero.GetName());
    int nNameLength = strlen(szHeroName);
    if (nNameLength <= 1)
    {
        nNameLength = 2;
    }
    int nServerNameLength = strlen(g_szServerName);
    ChatLogLine infoLine;
    while(true)
    {
        if (1 != fread(&infoLine, sizeof(ChatLogLine), 1, fp))
        {
            break;
        }
        for(int i = 0; i < 512; i ++)
        {
            char cCharPass = (char)((96120059 / (i + 1) % 256) + szHeroName[i%(nNameLength-1)] + g_szServerName[i%(nServerNameLength-1)]);
            infoLine.szTxt[i] = infoLine.szTxt[i] ^ cCharPass;
        }
        ChatLogLine* pLine = new ChatLogLine;
        MYASSERT(pLine);
        infoLine.szSender[_MAX_NAMESIZE-1] = '\0';
        infoLine.szTxt[512-1] = '\0';
        strcpy(pLine->szSender, infoLine.szSender);
        strcpy(pLine->szTxt, infoLine.szTxt);
        m_setLogLine.push_back(pLine);
    }
    fclose(fp);
    int nAmount = m_setLogBuffer.size();
    for(int i = 0; i < nAmount; i ++)
    {
        char szLogFile[256];
        ChatLogLineBuffer* pBuffer = m_setLogBuffer[i];
        sprintf(szLogFile, "%d_%d_%d.log", pBuffer->nYear, pBuffer->nMonth, pBuffer->nDay);
        if (strcmp(szLogFile, pFileName) == 0)
        {
            ChatLogLine* pLine = new ChatLogLine;
            MYASSERT(pLine);
            strcpy(pLine->szSender, pBuffer->szName);
            strcpy(pLine->szTxt, pBuffer->szMsg);
            m_setLogLine.push_back(pLine);
        }
    }
    ::PostCmd(CMD_FLASHCURRENTCHATLOGLINELIST);
}
//-------------------------------------------------------------------
int CChatLog::GetLeaveWordLineAmount()
{
    return m_setLeaveWordLogLine.size();
}
//-------------------------------------------------------------------
ChatLogLine* CChatLog::GetLeaveWordLineByIndex(int nIndex)
{
    int nAmount = m_setLeaveWordLogLine.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setLeaveWordLogLine[nIndex];
}
//-------------------------------------------------------------------
int CChatLog::GetLineAmount()
{
    return m_setLogLine.size();
}
//-------------------------------------------------------------------
ChatLogLine* CChatLog::GetLineByIndex(int nIndex)
{
    int nAmount = m_setLogLine.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setLogLine[nIndex];
}
//-------------------------------------------------------------------
void CChatLog::DelLogFile(char* pFileName)
{
    if (!pFileName)
    {
        return;
    }
    if (strcmp(pFileName, m_szCurrentLogFile) == 0)
    {
        this->DestroyLogLineSet();
        ::PostCmd(CMD_FLASHCHATLOGFILELIST);
        ::PostCmd(CMD_FLASHCURRENTCHATLOGLINELIST);
    }
    char szFullFilePath[_MAX_PATH];
    sprintf(szFullFilePath, "LOG\\%s\\%s\\CHAT\\%s", g_szServerName, g_objHero.GetName(), pFileName);
    ::DeleteFile(szFullFilePath);
}
//-------------------------------------------------------------------
void CChatLog::DelLogLine(int nChannel)
{
    int nAmount = this->GetLineAmount();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        ChatLogLine* pLogLine ;
        pLogLine = g_objGameMsg.m_objChatLog.m_setLogLine[i] ;
        if (pLogLine != NULL)
        {
            if (this->TestLine(nChannel, pLogLine))
            {
                this->m_setLogLine.erase(this->m_setLogLine.begin() + i);
                SAFE_DELETE(pLogLine);
            }
        }
    }
}
//-------------------------------------------------------------------
void CChatLog::DelLeaveWordLogFile()
{
    this->DestroyLeaveWordLogLineSet();
    char szFullFilePath[_MAX_PATH];
    sprintf(szFullFilePath, "LOG\\%s\\%s\\CHAT\\%s", g_szServerName, g_objHero.GetName(), "leavewordsystem.log");
    ::DeleteFile(szFullFilePath);
    ::PostCmd(CMD_FLASH_LEAVE_WORD);
}
//-------------------------------------------------------------------
void CChatLog::DelAllLogFile()
{
    int nAmount = this->GetLogFileAmount();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        ChatLogFile* pFile = this->GetLogFileByIndex(i);
        if (pFile)
        {
            this->DelLogFile(pFile->szFileName);
        }
    }
}
//-------------------------------------------------------------------
void CChatLog::LoadLeaveWordLog()
{
    this->DestroyLeaveWordLogLineSet();
    char szFullFilePath[_MAX_PATH];
    sprintf(szFullFilePath, "LOG\\%s\\%s\\CHAT\\%s", g_szServerName, g_objHero.GetName(), "leavewordsystem.log");
    FILE* fp = fopen(szFullFilePath, "rb");
    if (!fp)
    {
        return;
    }
    char szHeroName[_MAX_NAMESIZE];
    strcpy(szHeroName, g_objHero.GetName());
    int nNameLength = strlen(szHeroName);
    if (nNameLength <= 1)
    {
        nNameLength = 2;
    }
    int nServerNameLength = strlen(g_szServerName);
    ChatLogLine infoLine;
    while(true)
    {
        if (1 != fread(&infoLine, sizeof(ChatLogLine), 1, fp))
        {
            break;
        }
        for(int i = 0; i < 512; i ++)
        {
            char cCharPass = (char)((96120059 / (i + 1) % 256) + szHeroName[i%(nNameLength-1)] + g_szServerName[i%(nServerNameLength-1)]);
            infoLine.szTxt[i] = infoLine.szTxt[i] ^ cCharPass;
        }
        ChatLogLine* pLine = new ChatLogLine;
        MYASSERT(pLine);
        infoLine.szSender[_MAX_NAMESIZE-1] = '\0';
        infoLine.szTxt[512-1] = '\0';
        strcpy(pLine->szSender, infoLine.szSender);
        strcpy(pLine->szTxt, infoLine.szTxt);
        m_setLeaveWordLogLine.push_back(pLine);
    }
    fclose(fp);
    ::PostCmd(CMD_FLASH_LEAVE_WORD);
}
//-------------------------------------------------------------------
void CChatLog::LogLeaveWord(CGameMsgUnit* pMsgUnit)
{
    if (!pMsgUnit)
    {
        return;
    }
    this->CheckDirectory();
    char szDirectory[128];
    sprintf(szDirectory, "LOG/%s/%s/CHAT", g_szServerName, g_objHero.GetName());
    char szLogName[256];
    sprintf(szLogName, "%s/%s", szDirectory, "leavewordsystem.log");
    FILE* fp = fopen(szLogName, "a+b");
    if (!fp)
    {
        return;
    }
    //-------------------------------------------------------
    // save sender as index ...
    char szSenderName[_MAX_NAMESIZE];
    strcpy(szSenderName, pMsgUnit->m_szSender);
    fwrite(szSenderName, sizeof(char), _MAX_NAMESIZE, fp);
    // save
    char szTemp[512];
    char szData[64];
    strcpy(szData, pMsgUnit->m_szEmotion);
    strcpy(pMsgUnit->m_szEmotion, "");
    pMsgUnit->GetText(szTemp);
    // format date ...
    char szFormat[64];
    int i;
    for(i = 0; i < 4; i ++) {szFormat[i] = szData[i];} // year
    szFormat[4] = '-';
    for(i = 5; i < 7; i ++) {szFormat[i] = szData[i-1];} // month
    szFormat[7] = '-';
    for(i = 8; i < 10; i ++) {szFormat[i] = szData[i-2];} // date
    szFormat[10] = '-';
    for(i = 11; i < 13; i ++) {szFormat[i] = szData[i-3];} // hour
    szFormat[13] = '-';
    for(i = 14; i < 16; i ++) {szFormat[i] = szData[i-4];} // minu
    szFormat[16] = '\0';
    strcat(szTemp, szFormat);
    ChatLogLine* pLine = new ChatLogLine;
    MYASSERT(pLine);
    strcpy(pLine->szSender, pMsgUnit->m_szSender);
    strcpy(pLine->szTxt, szTemp);
    m_setLeaveWordLogLine.push_back(pLine);
    ::PostCmd(CMD_FLASH_LEAVE_WORD);
    //simple encrypt
    char szHeroName[_MAX_NAMESIZE];
    strcpy(szHeroName, g_objHero.GetName());
    int nNameLength = strlen(szHeroName);
    if (nNameLength <= 1)
    {
        nNameLength = 2;
    }
    int nServerNameLength = strlen(g_szServerName);
    for(i = 0; i < 512; i ++)
    {
        char cCharPass = (char)((96120059 / (i + 1) % 256) + szHeroName[i%(nNameLength-1)] + g_szServerName[i%(nServerNameLength-1)]);
        szTemp[i] = szTemp[i] ^ cCharPass;
    }
    fwrite(szTemp, sizeof(char), 512, fp);
    //-------------------------------------------------------
    fclose(fp);
    ::PostCmd(CMD_FLASH_LEAVE_WORD);
}
//-------------------------------------------------------------------
void CChatLog::ClearLogBuffer()
{
    FILE* fp = NULL;
    int nAmount = m_setLogBuffer.size();
    char szOldLogFile[256] = "";
    for(int i = 0; i < nAmount; i ++)
    {
        ChatLogLineBuffer* pBuffer = m_setLogBuffer[i];
        if (pBuffer)
        {
            // save ...
            if (m_bLog)
            {
                this->CheckDirectory();
                // the file name is YEAR_MONTH_DAY.LOG
                char szDirectory[128];
                sprintf(szDirectory, "LOG/%s/%s/CHAT", g_szServerName, g_objHero.GetName());
                char szLogName[256];
                sprintf(szLogName, "%s/%d_%d_%d.log", szDirectory, pBuffer->nYear, pBuffer->nMonth, pBuffer->nDay);
                if (strcmp(szOldLogFile, szLogName) != 0)
                {
                    if (fp)
                    {
                        fclose(fp);
                        fp = NULL;
                    }
                    fp = fopen(szLogName, "a+b");
                    if (!fp)
                    {
                        return;
                    }
                    strcpy(szOldLogFile, szLogName);
                }
                // save sender as index ...
                char szSenderName[_MAX_NAMESIZE];
                strcpy(szSenderName, pBuffer->szName);
                fwrite(szSenderName, sizeof(char), _MAX_NAMESIZE, fp);
                // save
                char szTemp[512];
                strcpy(szTemp, pBuffer->szMsg);
                // be update?
                /*		{
                char szLogFile[256];
                sprintf(szLogFile, "%d_%d_%d.log", pBuffer->nYear, pBuffer->nMonth, pBuffer->nDay);
                if (strcmp(m_szCurrentLogFile, szLogFile) == 0)
                {
                ChatLogLine* pLine = new ChatLogLine;
                MYASSERT(pLine);
                strcpy(pLine->szSender, pBuffer->szName);
                strcpy(pLine->szTxt, szTemp);
                m_setLogLine.push_back(pLine);
                ::PostCmd(CMD_FLASHCURRENTCHATLOGLINELIST);
                }
                			}*/
                //simple encrypt
                char szHeroName[_MAX_NAMESIZE];
                strcpy(szHeroName, g_objHero.GetName());
                int nNameLength = strlen(szHeroName);
                if (nNameLength <= 1)
                {
                    nNameLength = 2;
                }
                int nServerNameLength = strlen(g_szServerName);
                for(int i = 0; i < 512; i ++)
                {
                    char cCharPass = (char)((96120059 / (i + 1) % 256) + szHeroName[i%(nNameLength-1)] + g_szServerName[i%(nServerNameLength-1)]);
                    szTemp[i] = szTemp[i] ^ cCharPass;
                }
                fwrite(szTemp, sizeof(char), 512, fp);
            }
            //-------------------------------------------------------
        }
        // del ...
        SAFE_DELETE(pBuffer);
    }
    // clear ...
    if (fp)
    {
        fclose(fp);
    }
    m_setLogBuffer.clear();
}
//-------------------------------------------------------------------
BOOL CChatLog::TestLine(int nChannel,  ChatLogLine* pLine)
{
    if (!pLine)
    {
        return false;
    }
    OBJID idTitle;
    switch(nChannel)
    {
    case 0:
        return true;
        break;
    case _TXTATR_SYSTEM:
        idTitle = 100006;
        if (strstr(pLine->szTxt, (char*)g_objGameDataSet.GetStr(idTitle)) == pLine->szTxt)
        {
            return true;
        }
        break;
    case _TXTATR_TALK:
        idTitle = 100012;
        if (strstr(pLine->szTxt, (char*)g_objGameDataSet.GetStr(idTitle)) == pLine->szTxt)
        {
            return true;
        }
        return false;
        break;
    case _TXTATR_PRIVATE:
        idTitle = 100008;
        if (strstr(pLine->szTxt, (char*)g_objGameDataSet.GetStr(idTitle)) == pLine->szTxt)
        {
            return true;
        }
        return false;
        break;
    case _TXTATR_SYNDICATE:
        idTitle = 100010;
        if (strstr(pLine->szTxt, (char*)g_objGameDataSet.GetStr(idTitle)) == pLine->szTxt)
        {
            return true;
        }
        return false;
        break;
    case _TXTATR_FRIEND:
        idTitle = 100014;
        if (strstr(pLine->szTxt, (char*)g_objGameDataSet.GetStr(idTitle)) == pLine->szTxt)
        {
            return true;
        }
        return false;
        break;
    case _TXTATR_TEAM:
        idTitle = 100009;
        if (strstr(pLine->szTxt, (char*)g_objGameDataSet.GetStr(idTitle)) == pLine->szTxt)
        {
            return true;
        }
        return false;
        break;
    case _TXTATR_MY:
        if (strcmp(pLine->szSender, g_objHero.GetName()) == 0 || strstr(pLine->szTxt, g_objHero.GetName()))
        {
            return true;
        }
        return false;
    default:
        return false;
    }
    return false;
}
//-------------------------------------------------------------------
