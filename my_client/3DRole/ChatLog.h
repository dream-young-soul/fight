
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ChatLog.h: interface for the CChatLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATLOG_H__08070572_BE3A_422E_B18E_79689144FC73__INCLUDED_)
#define AFX_CHATLOG_H__08070572_BE3A_422E_B18E_79689144FC73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

class CGameMsgUnit;

typedef struct
{
    int		nYear;
    int		nMonth;
    int		nDay;
    char	szFileName[_MAX_PATH];
} ChatLogFile;

typedef struct
{
    int		nYear;
    int		nMonth;
    int		nDay;
    char	szName[_MAX_NAMESIZE];
    char	szMsg[512];
} ChatLogLineBuffer;


typedef struct
{
    char	szSender[_MAX_NAMESIZE];
    char	szTxt[512];
} ChatLogLine;

#include <vector>
using namespace std;
typedef vector<ChatLogFile*> VECTOR_CHARLOGFILE;
typedef vector<ChatLogLine*> VECTOR_CHARLOGLINE;
typedef vector<ChatLogLineBuffer*> VECTOR_CHARLOGBUFFER;


class CChatLog
{
public:
    CChatLog();
    virtual ~CChatLog();

private:
    VECTOR_CHARLOGFILE		m_setLogFile;
    VECTOR_CHARLOGLINE		m_setLogLine;
    char m_szCurrentLogFile[_MAX_PATH];

    VECTOR_CHARLOGLINE		m_setLeaveWordLogLine;
    VECTOR_CHARLOGBUFFER	m_setLogBuffer;
private:
    void DestroyLogFileSet();
    void DestroyLogLineSet();

    void DestroyLeaveWordLogLineSet();

    void CheckDirectory();
    BOOL m_bLog;
public:
    void SearchFile();

    void Log(CGameMsgUnit* pMsgUnit);
    int GetLogFileAmount();
    ChatLogFile* GetLogFileByIndex(int nIndex);
    BOOL TestLine(int nChannel, ChatLogLine* pLine);
    void LoadLog(char* pFileName);
    int GetLineAmount();
    ChatLogLine* GetLineByIndex(int nIndex);
    void DelLogFile(char* pFileName);
    void DelLogLine(int nChannel);
    void DelAllLogFile();
    void LogOn() {m_bLog = true;}
    void LogOff() {m_bLog = false;}

    int GetLeaveWordLineAmount();
    ChatLogLine* GetLeaveWordLineByIndex(int nIndex);
    void LoadLeaveWordLog();
    void LogLeaveWord(CGameMsgUnit* pMsgUnit);
    void DelLeaveWordLogFile();
    void ClearLogBuffer();
};


#endif // !defined(AFX_CHATLOG_H__08070572_BE3A_422E_B18E_79689144FC73__INCLUDED_)
