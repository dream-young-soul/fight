
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAnnounceList.h: interface for the CMsgAnnounceList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGANNOUNCELIST_H__2EC71D3F_5C0C_4690_9DEF_AB84E97996E9__INCLUDED_)
#define AFX_MSGANNOUNCELIST_H__2EC71D3F_5C0C_4690_9DEF_AB84E97996E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"


//////////////////////////////////////////////////////////////////////
const int	_MAX_ANNOUNCE_PER_PAGE	= 20;	// 每页发送20条公告标题

struct ST_ANNOUNCE_TITLE
{
    OBJID	idAnnounce;			// 公告ID
    char	szName[32];	// 发布公告人名字
};
/*enum {
	ANNOUNCELIST_NONE		= 0,
	ANNOUNCELIST_TITLE,	        // 查看公告标题
	ANNOUNCELIST_SELF,          // 查看自己的公告
	ANNOUNCELIST_CONTENT,		// 查看公告内容
	ANNOUNCELIST_ISSUE,         // 发布公告
};*/
enum
{
    CHECK_BY_INDEX,
    CHECK_BY_ID,
    CHECK_BY_OWNER,
};
class CMsgAnnounceList : public CNetMsg
{
public:
    CMsgAnnounceList();
    virtual ~CMsgAnnounceList();
    BOOL	Create(char* pbufMsg, DWORD dwMsgSize);

    BOOL	Create(USHORT usType, USHORT usAmount = 0, ST_ANNOUNCE_TITLE* setAnnounce = NULL);
    BOOL	Append(OBJID idAnnounce,  const char* pszTitle, int type = 1);
    void	Process	(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE;

        USHORT	usType;				// 类型
        //	OBJID   user_id;
        union
        {
            USHORT	usAmount;			// 数量
            OBJID	idAnnounce;
            int		nIndex;
        };
        ST_ANNOUNCE_TITLE	setAnnounce[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGANNOUNCELIST_H__2EC71D3F_5C0C_4690_9DEF_AB84E97996E9__INCLUDED_)
