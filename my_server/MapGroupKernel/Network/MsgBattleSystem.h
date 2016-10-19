
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgBattleSystem.h: interface for the CMsgBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGBATTLESYSTEM_H__964FE72E_74E5_4275_8016_5EF914C05CA4__INCLUDED_)
#define AFX_MSGBATTLESYSTEM_H__964FE72E_74E5_4275_8016_5EF914C05CA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgBattleSystem : public CNetMsg
{
public:
    CMsgBattleSystem();
    virtual ~CMsgBattleSystem();

    BOOL	Create	(OBJID idAtker, OBJID idDefer, int nLifeLost, int nDeferLife,
                     DWORD dwAtkerStatus, DWORD dwDeferStatus);

public:
    BOOL		Create		(char* pMsgBuf, DWORD dwSize);
    void		Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID	idAtker;
        OBJID	idDefer;
        int		nDeferLifeLost;
        int		nDeferLife;
        DWORD	dwAtkerStatus;
        DWORD	dwDeferStatus;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGBATTLESYSTEM_H__964FE72E_74E5_4275_8016_5EF914C05CA4__INCLUDED_)
