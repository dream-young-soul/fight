
//**********************************************************
// 代码编辑器
//**********************************************************

// PlayerLeaveWord.h: interface for the CLeaveWord class.
// 仙剑修, 2002.10.11
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYERLEAVEWORD_H__6547133F_17B9_4978_93AB_BAA88B109312__INCLUDED_)
#define AFX_PLAYERLEAVEWORD_H__6547133F_17B9_4978_93AB_BAA88B109312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "define.h"
#include "TimeOut.h"
#include "Myheap.h"

////////////////////////////////////////////////////////////////////////////////////////////
enum LEAVEWORDDATA
{
    LEAVEWORDDATA_ID = 0,		//
    LEAVEWORDDATA_USER = 1,
    LEAVEWORDDATA_SENDER,
    LEAVEWORDDATA_TIME,
    LEAVEWORDDATA_WORDS,
};

////////////////////////////////////////////////////////////////////////////////////////////
const int	PLAYER_WORDS_PER_TIME	= 1;				// 玩家每次读取5条留言
const int	NEW_WORDS_PER_MINUTE	= 5;				// 每分钟自动读取几条新留言
const int	DELETE_LEAVEWORD_DAYS	= 31;				// 31天以上留言会自动删除
const int	SECS_PER_AUTOFLUSH		= 60;				// 每分钟自动刷新一次

////////////////////////////////////////////////////////////////////////////////////////////
class	CUser;
class CLeaveWord
{
public:
    CLeaveWord();
    virtual ~CLeaveWord();

public:
    bool	Create			(PROCESS_ID idProcess);
    ULONG	Release			()							{ delete this; return 0; }
    bool	InsertWords		(LPCTSTR szSender, LPCTSTR szRecvUser, LPCTSTR szWords);
    //	int		CountWords		(LPCTSTR szRecvUser);
    bool	ShowWords		(CUser* pUser, int nMaxNum = PLAYER_WORDS_PER_TIME);
    void	OnTimer			(DWORD tCurr);

protected:
    DWORD			m_idLastMsg;
    CTimeOut		m_tAutoFlush;

protected: // ctrl
    PROCESS_ID		m_idProcess;

public: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_PLAYERLEAVEWORD_H__6547133F_17B9_4978_93AB_BAA88B109312__INCLUDED_)
