
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTick.h: interface for the CMsgTick class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTICK_H__C68B182B_68CB_4D0D_ACDC_8A87BAEB1AA7__INCLUDED_)
#define AFX_MSGTICK_H__C68B182B_68CB_4D0D_ACDC_8A87BAEB1AA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgTick : public CNetMsg
{
public:
    CMsgTick();
    virtual ~CMsgTick();

public:
    BOOL	Create	(char* pbufMsg, DWORD dwMsgSize);
    BOOL	Create	(OBJID idPlayer);
    void	Process	(void* pInfo);

private:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID			idPlayer;
        DWORD			dwData;
        DWORD			dwChkData;
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTICK_H__C68B182B_68CB_4D0D_ACDC_8A87BAEB1AA7__INCLUDED_)
