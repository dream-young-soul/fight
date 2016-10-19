
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTeam.h: interface for the CMsgTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTEAM_H__3E7E3CFB_3522_4444_941B_BFDCABD1EFB7__INCLUDED_)
#define AFX_MSGTEAM_H__3E7E3CFB_3522_4444_941B_BFDCABD1EFB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    _MSGTEAM_CREATE			=	0,	//TS
    _MSGTEAM_APPLYJOIN		=	1,  //TS	//TC
    _MSGTEAM_LEAVE			=	2,	//TS	//TC
    _MSGTEAM_ACCEPTINVEIT	=	3,	//TS

    _MSGTEAM_INVITE			=	4,	//TS	//TC
    _MSGTEAM_AGREEJOIN		=	5,  //TS
    _MSGTEAM_DISMISS		=	6,	//TS	//TC
    _MSGTEAM_KICKOUT		=	7,	//TS	//TC
    _MSGTEAM_CLOSETEAM		=	8,	//TS
    _MSGTEAM_OPENTEAM		=	9,	//TS
};

class CMsgTeam : public CNetMsg
{
public:
    CMsgTeam();
    virtual ~CMsgTeam();

public:
    BOOL	Create	(char* pbufMsg, DWORD dwMsgSize);

    void	Process	(void* pInfo);
    BOOL	Create	(OBJID idPlayer, int nAction);
private:
    typedef struct
    {
        MSGHEAD_DEFINE

        unsigned short	unAction;
        OBJID			idPlayer;
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTEAM_H__3E7E3CFB_3522_4444_941B_BFDCABD1EFB7__INCLUDED_)
