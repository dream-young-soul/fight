
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
    EVENT_BEACTIVED = 0,				// to server		// ´¥·¢
    EVENT_ADDNPC,						// no use
    EVENT_LEAVEMAP,						// to client		// É¾³ý
    EVENT_DELNPC,						// to server
    EVENT_CHANGEPOS,					// to client/server
    EVENT_LAYNPC,						// to client(id=region,data=lookface), answer MsgNpcInfo(CMsgPlayer for statuary)
};

class CMsgNpc   : public CNetMsg
{
public:
    CMsgNpc();
    virtual ~CMsgNpc();
    BOOL	Create	(int nAction, OBJID id, int nType = 0, DWORD dwData = 0);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID id;								// may be ID_NONE, EVENT_LAYNPC: region
        union
        {
            DWORD			dwData;
            struct
            {
                USHORT			usPosX;
                USHORT			usPosY;
            };
        };
        unsigned short usAction;
        unsigned short usType;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGNPC_H__B662BA21_4E11_41B2_B997_584B4833F266__INCLUDED_)
