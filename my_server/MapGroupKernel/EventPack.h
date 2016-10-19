
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// EventPack.h: interface for the CEventPack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTPACK_H__7565C398_0FA3_4E78_9344_860F1D15C840__INCLUDED_)
#define AFX_EVENTPACK_H__7565C398_0FA3_4E78_9344_860F1D15C840__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basefunc.h"

const int EVENTPACK_SIZE		= 1024 + 2 * sizeof(USHORT) + sizeof(ULONG);

class CEventPack
{
public:
    CEventPack(DWORD idEvent, ULONG idKey, DWORD idAction = 0);
    CEventPack(const char* pPackBuf, int nPackSize);
    virtual ~CEventPack();

public: // appilcation
    CEventPack& operator <<(const int nData);
    CEventPack& operator <<(const String& str);
    CEventPack& operator >>(int& nData);
    CEventPack& operator >>(String& str);

public: // const
    int		GetEventType()	const		{ return m_idEventType; }
    ULONG	GetObjID()		const		{ return m_idKey; }
    int		GetAction()		const		{ return m_idAction; }

    const char*	GetBuf()	const		{ return m_buf; }
    int		GetSize()		const		{ return m_ptr - m_buf; }

protected:
    union
    {
        char	m_buf[EVENTPACK_SIZE];
        struct
        {
            USHORT	m_idEventType;
            USHORT	m_idAction;
            ULONG	m_idKey;
            char	m_setParam[1];
        };
    };

protected: // ctrl
    bool	IsUnite()				{ return !m_bSplit; }
    bool	IsSplit()				{ return m_bSplit; }
    bool	CheckLen(int nSize)		{ return m_ptr + nSize <= m_setParam; }
protected:
    bool	m_bSplit;
    char*	m_ptr;
};

#endif // !defined(AFX_EVENTPACK_H__7565C398_0FA3_4E78_9344_860F1D15C840__INCLUDED_)
