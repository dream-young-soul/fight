
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAction.h: interface for the CMsgWalkEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MSG_WALKEX_HEAD)
#define MSG_WALKEX_HEAD

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "User.h"

class CMsgWalkEx : public CNetMsg
{
public:
    CMsgWalkEx();
    virtual ~CMsgWalkEx();

    BOOL	Create	(OBJID idUser, int nDir, int nMode, int x, int y);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		idUser;
        USHORT		usPosX;
        USHORT		usPosY;
        UCHAR		ucDir;
        UCHAR		ucMode;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(MSG_WALKEX_HEAD)
