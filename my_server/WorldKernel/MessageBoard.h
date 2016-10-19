
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MessageBoard.h: interface for the CMessageBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEBOARD_H__7D053D4B_8CD7_4D1B_B8D7_8C2965DA9A4B__INCLUDED_)
#define AFX_MESSAGEBOARD_H__7D053D4B_8CD7_4D1B_B8D7_8C2965DA9A4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "NetWorkDef.h"
#include "windows.h"
#include "basefunc.h"
#include <time.h>
#include <deque>
using namespace std;


class CMessageBoard
{
public:
    struct MessageInfo
    {
        OBJID			id;
        NAMESTR			name;
        WORDSSTR		words;
        time_t			time;
    };
public:
    typedef	deque<MessageInfo>	MESSAGE_SET;

    /////////////////////////////////////
protected:
    CMessageBoard(int nChannel, int nSize);
    virtual ~CMessageBoard();
public:
    static CMessageBoard* CreateNew(int nChannel, int nSize = 500)	{ return new CMessageBoard(nChannel, nSize); }
    void	Release()									{ delete this; }

public:
    bool	AddMsg(OBJID id, LPCTSTR szName, LPCTSTR szMsg);
    bool	DelMsg(OBJID id);
    bool	IsValid(int idx)							{ return idx >= 0 && idx < m_setMsg.size(); }
    OBJID	GetMsgID(int idx)							{ if (!IsValid(idx)) { return ID_NONE; } return m_setMsg[idx].id; }
    LPCTSTR	GetMsgName(int idx)							{ if (!IsValid(idx)) { return NULL; } return m_setMsg[idx].name; }
    LPCTSTR	GetMsgWords(int idx)						{ if (!IsValid(idx)) { return NULL; } return m_setMsg[idx].words; }
    bool	GetMsgTime(int idx, char* szTime15)			{ if (!IsValid(idx)) { return szTime15[0] = 0, false; } ::DateTimeStamp(szTime15, m_setMsg[idx].time); return true; }
    MessageInfo*	GetMsgInfo(LPCTSTR szName);

protected:
    MESSAGE_SET			m_setMsg;

    const int			m_nChannel;
    const int			m_nSize;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MESSAGEBOARD_H__7D053D4B_8CD7_4D1B_B8D7_8C2965DA9A4B__INCLUDED_)
