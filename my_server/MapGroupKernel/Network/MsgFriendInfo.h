
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgFriendInfo.h: interface for the CMsgFriendInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFRIENDINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_)
#define AFX_MSGFRIENDINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NetMsg.h"
#include "Npc.h"

enum {_FRIEND_INFO = 0, _ENEMY_INFO = 1};

class CNpc;
class CMsgFriendInfo  : public CNetMsg
{
public:
    CMsgFriendInfo();
    virtual ~CMsgFriendInfo();

    BOOL	Create(int nAction, OBJID idFriend, int nLook, int nLevel, int nProfession, int nPk, OBJID idSyn, int nRank, LPCTSTR szMate, UCHAR ucNobilityRank);
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID			idFriend;
        DWORD			dwLook;
        UCHAR			ucLevel;
        UCHAR			ucProfession;
        SHORT			sPk;
        DWORD			dwSynID_Rank;
        char			szMate[_MAX_NAMESIZE];
        UCHAR			ucNobilityRank;
        unsigned char	ucAction;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGFRIENDINFO_H__50F4AAD1_342A_4D6C_A161_33CDE0158D5B__INCLUDED_)
