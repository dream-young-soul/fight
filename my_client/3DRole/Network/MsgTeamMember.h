
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTeamMember.h: interface for the CMsgTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTEAMMEMBER_H__CAE19AEF_4BC7_41CD_9CC5_05AAE358EA74__INCLUDED_)
#define AFX_MSGTEAM_H__CAE19AEF_4BC7_41CD_9CC5_05AAE358EA74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
const int _MAX_TEAMMEMBER = 5;
enum
{
    _MSG_TEAMMEMBER_ADDMEMBER = 0,
    _MSG_TEAMMEMBER_DROP_ADDMEMBER = 1,
};
class CMsgTeamMember : public CNetMsg
{
protected:
    typedef struct
    {
        char			szName[_MAX_NAMESIZE];
        OBJID			id;
        DWORD			dwFace;
        unsigned short	usMaxHp;
        unsigned short	usHp;
    } MsgTeamMemberInfo;

public:
    CMsgTeamMember();
    virtual ~CMsgTeamMember();

public:
    BOOL	Create	(char* pbufMsg, DWORD dwMsgSize);
    void	Process	(void* pInfo);

private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        unsigned char		ucAction;
        unsigned char		ucAmount;
        MsgTeamMemberInfo	setMember[_MAX_TEAMMEMBER];
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTEAM_H__CAE19AEF_4BC7_41CD_9CC5_05AAE358EA74__INCLUDED_)
