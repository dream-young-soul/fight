
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// EventPack.cpp: implementation of the CEventPack class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "limits.h"
#include "common.h"
#include "EventPack.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEventPack::CEventPack(DWORD idEvent, ULONG idKey, DWORD idAction/*=ID_NONE*/)
{
    ASSERT(idEvent < USHRT_MAX);
    ASSERT(idAction < USHRT_MAX);
    m_idEventType	= (USHORT)idEvent;
    m_idKey			= idKey;
    m_idAction		= (USHORT)idAction;
    m_bSplit		= false;
    m_ptr			= m_setParam;
}

//////////////////////////////////////////////////////////////////////
CEventPack::CEventPack(const char* pPackBuf, int nPackSize)
{
    memset(m_buf, 0, EVENTPACK_SIZE);
    IF_OK(nPackSize > 0 && nPackSize <= EVENTPACK_SIZE)
    memcpy(m_buf, pPackBuf, nPackSize);
    m_bSplit		= true;
    m_ptr			= m_setParam;
}

//////////////////////////////////////////////////////////////////////
CEventPack::~CEventPack()
{
}

//////////////////////////////////////////////////////////////////////
// appilcation
//////////////////////////////////////////////////////////////////////
CEventPack& CEventPack::operator <<(const int nData)
{
    IF_NOT(IsUnite())
    return *this;
    IF_NOT(CheckLen(sizeof(long)))
    return *this;
    *((long*)m_setParam)	= nData;
    m_ptr	+= sizeof(long);
    return *this;
}

//////////////////////////////////////////////////////////////////////
CEventPack& CEventPack::operator <<(const String& str)
{
    IF_NOT(IsUnite())
    return *this;
    IF_NOT(CheckLen(strlen((const char*)str) + 1))
    return *this;
    SafeCopy(m_ptr, (const char*)str);
    m_ptr	+= strlen((const char*)str) + 1;
    return *this;
}

//////////////////////////////////////////////////////////////////////
CEventPack& CEventPack::operator >>(int& nData)
{
    IF_NOT(IsSplit())
    return *this;
    IF_NOT(CheckLen(sizeof(long)))
    return *this;
    nData	= *((long*)m_setParam);
    m_ptr	+= sizeof(long);
    return *this;
}

//////////////////////////////////////////////////////////////////////
CEventPack& CEventPack::operator >>(String& str)
{
    IF_NOT(IsSplit())
    return *this;
    IF_NOT(CheckLen(1))
    return *this;
    str		= m_ptr;
    m_ptr	+= strlen((const char*)str) + 1;
    return *this;
}
