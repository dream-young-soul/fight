
//**********************************************************
// 代码编辑器
//**********************************************************

// PlayerLeaveWord.cpp: implementation of the CLeaveWord class.
// 仙剑修, 2002.10.11
//////////////////////////////////////////////////////////////////////

#include "LeaveWord.h"
#include "AllMsg.h"
#include "I_mydb.h"
#include "MapGroup.h"
#include "TimeOut.h"

MYHEAP_IMPLEMENTATION(CLeaveWord, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLeaveWord::CLeaveWord()
{
    m_idLastMsg		= 0;
}

//////////////////////////////////////////////////////////////////////
CLeaveWord::~CLeaveWord()
{
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
// 本地函数，仅本文件使用
void GetTimeStamp(char* pBuf15, time_t tTime = 0)
{
    ASSERT(pBuf15);
    if (!tTime)
    {
        tTime	= time(NULL);
    }
    tm*	pTm		= localtime(&tTime);
    if (pTm)
    {
        sprintf(pBuf15, "%04d%02d%02d%02d%02d%02d",
                pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
    }
}

//////////////////////////////////////////////////////////////////////
bool CLeaveWord::Create(PROCESS_ID idProcess)
{
    m_idProcess = idProcess;
    char	szSQL[1024];
    // 清除过期留言
    time_t	tDelTime = time(NULL);
    tDelTime -= DELETE_LEAVEWORD_DAYS * 86400;
    char	szDelTime[_MAX_NAMESIZE];
    GetTimeStamp(szDelTime, tDelTime);
    sprintf(szSQL, "DELETE FROM %s WHERE time < %s", _TBL_LEAVEWORD, szDelTime);	// need not '%s'
    g_db.ExecuteSQL(szSQL);
    m_idLastMsg	= 0;
    sprintf(szSQL, "SELECT MAX(id) as max_id FROM %s", _TBL_LEAVEWORD);
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);		//? may be failed.
    if (pRes)
    {
        m_idLastMsg = pRes->GetInt(0);
        pRes->Release();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CLeaveWord::InsertWords(LPCTSTR szSender, LPCTSTR szRecvUser, LPCTSTR szWords)
{
    if (!szSender || strlen(szSender) == 0)
    {
        return false;
    }
    if (!szRecvUser || strlen(szRecvUser) == 0)
    {
        return false;
    }
    if (!szWords || strlen(szWords) == 0)
    {
        return false;
    }
    CHECKF(!UserManager()->GetUser(szRecvUser));
    char	szTime[_MAX_NAMESIZE] = "00000000000000";		// 要填入EMOTION位置
    GetTimeStamp(szTime);
    char	szSQL[1024];
    sprintf(szSQL, "INSERT %s SET user_name='%s', send_name='%s', time='%s', words='%s'",
            _TBL_LEAVEWORD,
            szRecvUser, szSender, szTime, szWords);
    return g_db.ExecuteSQL(szSQL);
}

/*/////////////////////////////////////////////////////////////////////
int CLeaveWord::CountWords(LPCTSTR szRecvUser)
{
	if (!szRecvUser || strlen(szRecvUser) == 0)
		return false;

	char	szSQL[1024];
	sprintf(szSQL, "SELECT COUNT(*) as count_star FROM %s WHERE user_name='%s'", _TBL_LEAVEWORD, szRecvUser);
	IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
	if (!pRes)
		return 0;

	int nCount = pRes->GetInt(0);
	pRes->Release();
	return nCount;
}*/

//////////////////////////////////////////////////////////////////////
bool CLeaveWord::ShowWords(CUser* pUser, int nMaxNum /*= PLAYER_WORDS_PER_TIME*/)
{
    char	szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s WHERE user_name='%s' ORDER BY id LIMIT %d", _TBL_LEAVEWORD, pUser->GetName(), nMaxNum);
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
    if (!pRes)
    {
        pUser->SetLeaveWord(false);
        return false;
    }
    for(int i = 0; i < pRes->RecordCount(); i++)
    {
        char	szSender[_MAX_NAMESIZE];
        char	szRecvUser[_MAX_NAMESIZE];
        char	szTime[_MAX_NAMESIZE];
        char	szWords[_MAX_WORDSSIZE];
        SafeCopy(szSender, pRes->GetStr(LEAVEWORDDATA_SENDER), _MAX_NAMESIZE);
        SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
        SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
        SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
        CMsgTalk	msg;
        if (msg.Create(szSender, szRecvUser, szWords, szTime, 0, _TXTATR_LEAVEWORD))
        {
            pUser->SendMsg(&msg);
        }
        OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
        sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
        g_db.ExecuteSQL(szSQL);
        pRes->MoveNext();
    }
    pRes->Release();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CLeaveWord::OnTimer(DWORD tCurr)
{
    if (!m_tAutoFlush.ToNextTime(SECS_PER_AUTOFLUSH))		// 每分钟一次
    {
        return ;
    }
    char	szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s WHERE id>%u ORDER BY id LIMIT %d", _TBL_LEAVEWORD, m_idLastMsg, NEW_WORDS_PER_MINUTE);
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++)
        {
            char	szRecvUser[_MAX_NAMESIZE];
            SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
            CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
            if (pUser && !pUser->IsMoreLeaveWord())
            {
                char	szSender[_MAX_NAMESIZE];
                char	szTime[_MAX_NAMESIZE];
                char	szWords[_MAX_WORDSSIZE];
                SafeCopy(szSender, pRes->GetStr(LEAVEWORDDATA_SENDER), _MAX_NAMESIZE);
                SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
                SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
                CMsgTalk	msg;
                if (msg.Create(szSender, pUser->GetName(), szWords, szTime, 0, _TXTATR_LEAVEWORD))
                {
                    pUser->SendMsg(&msg);
                }
                pUser->SetLeaveWord(true);
                OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
                sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
                g_db.ExecuteSQL(szSQL);
            }
            pRes->MoveNext();
        }
        pRes->Release();
    }
}






