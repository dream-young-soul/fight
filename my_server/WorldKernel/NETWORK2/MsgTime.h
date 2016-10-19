
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTime.h: interface for the CMsgTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTIME_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_)
#define AFX_MSGTIME_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

WORLDKERNEL_BEGIN
class CMsgTime : public CNetMsg
{
public:
    CMsgTime();
    virtual ~CMsgTime();

    BOOL	Create	(int nHour);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        UCHAR			ucHour;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};
WORLDKERNEL_END

#endif // !defined(AFX_MSGTIME_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_)
