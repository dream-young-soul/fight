
//**********************************************************
// 代码编辑器
//**********************************************************

// 计费消息类
// 仙剑修，2001.11.22
//////////////////////////////////////////////////////////////////////

#ifndef	MSGFEE_HA
#define MSGFEE_HA

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "NetMsg.h"

class CMsgFee_A	// : public CNetMsg
{
public:
    CMsgFee_A();
    virtual ~CMsgFee_A();

    BOOL	Create	(OBJID idAccount, unsigned char ucType);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    //	void			Process		(CGameSocket* pSocket);

    enum {FEE_BEGIN = 0, FEE_END, FEE_POINT, HEARTBEAT, FEE_KICKOUT,
          FEE_OFFLINE, SERVER_BUSY, SERVER_FULL, SERVER_CLOSE, FEE_TICK
         };
    //private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        OBJID			idAccount;			// 帐号id
        unsigned char	ucType;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // MSGFEE_HA
