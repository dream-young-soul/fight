
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgNpc.h: interface for the CMsgNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNPC_H__B662BA21_4E11_41B2_B997_584B4833F266__INCLUDED_)
#define AFX_MSGNPC_H__B662BA21_4E11_41B2_B997_584B4833F266__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

//define constant ..
enum
{
    MSGNPC_DELNPC = 1,						// É¾³ýNPC
};

class CMsgNpc   : public CNetMsg
{
public:
    CMsgNpc();
    virtual ~CMsgNpc();
    BOOL	Create	(OBJID id, int nAction, int nType = 0, DWORD dwData = 0);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID id;
        unsigned short usAction;
        unsigned short usType;
        union
        {
            DWORD		   dwData;
            struct
            {
                unsigned short usCellX;
                unsigned short usCellY;
            };
        };
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGNPC_H__B662BA21_4E11_41B2_B997_584B4833F266__INCLUDED_)
