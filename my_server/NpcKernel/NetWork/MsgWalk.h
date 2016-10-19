
//**********************************************************
// 代码编辑器
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

enum {	WALKMODE_MOVE = 0,					// PathMove()的模式
        WALKMODE_RUN,
        WALKMODE_SHIFT,
        WALKMODE_JUMP,

        WALKMODE_RUN_DIR0 = 20,
        WALKMODE_RUN_DIR7 = 27,
     };


class CMsgWalk : public CNetMsg
{
public:
    CMsgWalk();
    virtual ~CMsgWalk();

    BOOL	Create	(OBJID idUser, int nDir, int nMode = WALKMODE_MOVE);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		idUser;
        UCHAR		ucDir;
        UCHAR		ucMode;
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(MSG_WALK_HEAD)
