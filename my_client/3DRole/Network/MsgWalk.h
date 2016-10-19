
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAction.h: interface for the CMsgWalk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MSG_WALK_HEAD)
#define MSG_WALK_HEAD

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgWalk : public CNetMsg
{
public:
    CMsgWalk();
    virtual ~CMsgWalk();

    BOOL	Create	(OBJID idUser, int nDir, int nScondDir = 0);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        USHORT		unMsgSize;
        USHORT		unMsgType;

        OBJID		idUser;
        UCHAR		ucDir;
        UCHAR		ucRun;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(MSG_WALK_HEAD)
