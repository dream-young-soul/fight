
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgFlushExp.h: interface for the CMsgFlushExp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFLUSHEXP_H__C68B182B_68CB_4D0D_ACDC_8A87BAEB1AA7__INCLUDED_)
#define AFX_MSGFLUSHEXP_H__C68B182B_68CB_4D0D_ACDC_8A87BAEB1AA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    MSGFLUSHEXP_WEAPONSKILL,
    MSGFLUSHEXP_MAGIC,
    MSGFLUSHEXP_SKILL,
};

class CMsgFlushExp : public CNetMsg
{
public:
    CMsgFlushExp();
    virtual ~CMsgFlushExp();

public:
    BOOL	Create	(int nAction, int nType, DWORD dwExp);
    BOOL	Create	(char* pbufMsg, DWORD dwMsgSize);
    void	Process	(void* pInfo);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        DWORD			dwExp;
        USHORT			usType;
        UCHAR			ucAction;
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGFLUSHEXP_H__C68B182B_68CB_4D0D_ACDC_8A87BAEB1AA7__INCLUDED_)
