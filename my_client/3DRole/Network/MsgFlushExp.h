
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgFlushExp.h: interface for the CMsgFlushExp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFLUSHEXP_H__E922536D_FB70_4EA3_B76E_21BA86C49657__INCLUDED_)
#define AFX_MSGFLUSHEXP_H__E922536D_FB70_4EA3_B76E_21BA86C49657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    MSGFLUSHEXP_WEAPONSKILL			=	0,
    MSGFLUSHEXP_MAGIC				=	1,
    MSGFLUSHEXP_SKILL				=	2,
};
class CMsgFlushExp : public CNetMsg
{
public:
    CMsgFlushExp();
    virtual ~CMsgFlushExp();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
    typedef struct
    {
        USHORT			unMsgSize;
        USHORT			unMsgType;
        DWORD			dwExp;
        USHORT			usType;
        UCHAR			ucAction;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGFLUSHEXP_H__E922536D_FB70_4EA3_B76E_21BA86C49657__INCLUDED_)
