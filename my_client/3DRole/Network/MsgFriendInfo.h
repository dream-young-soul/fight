
//**********************************************************
// ¥˙¬Î±‡º≠∆˜
//**********************************************************

// MsgFriendInfo.h: interface for the CMsgFriendInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFRIENDINFO_H__51533268_8792_44EB_8E15_6BBDDC9D5AF0__INCLUDED_)
#define AFX_MSGFRIENDINFO_H__51533268_8792_44EB_8E15_6BBDDC9D5AF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum {_FRIEND_INFO = 0, _ENEMY_INFO = 1};
class CMsgFriendInfo  : public CNetMsg
{
public:
    CMsgFriendInfo();
    virtual ~CMsgFriendInfo();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        OBJID			idFriend;
        DWORD			dwLook;

        UCHAR			ucLevel;
        UCHAR			ucProfession;
        SHORT			sPk;
        DWORD			dwSynID_Rank;
        char			szMate[_MAX_NAMESIZE];
        UCHAR			ucNobilityRank;	//æÙŒª Ù–‘
        unsigned char	ucAction;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGFRIENDINFO_H__51533268_8792_44EB_8E15_6BBDDC9D5AF0__INCLUDED_)
