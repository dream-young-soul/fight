
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgBetOutside.h: interface for the CMsgAtk class.
// 仙剑修，2002.1.8
//////////////////////////////////////////////////////////////////////

#ifndef	MSGSYNDICATE_H
#define MSGSYNDICATE_H

#include "NetMsg.h"

enum
{
    // 已实现
    APPLY_JOIN			= 1,			// 申请加入黑社会, id
    INVITE_JOIN			= 2,			// 邀请加入黑社会, id
    LEAVE_SYN			= 3,			// 脱离黑社会
    KICKOUT_MEMBER		= 4,			// 开除黑社会成员,id	(注意：改成ID了)
    QUERY_SYN_NAME		= 6,			// 查询帮派名字
    SET_ALLY			= 7,			// 结盟				// to client, npc(npc_id is syn_id, same follow)
    CLEAR_ALLY			= 8,			// 解除结盟			// to client, npc
    SET_ANTAGONIZE		= 9,			// 树敌				// to client, npc
    CLEAR_ANTAGONIZE	= 10,			// 解除树敌			// to client, npc
    DONATE_MONEY		= 11,			// 帮众捐钱
    QUERY_SYNATTR		= 12,			// 查询帮派信息		// to server
    SET_MAINSYN			= 13,			// 添加主帮派的ID	// to client
    SET_SUBSYN			= 14,			// 添加子帮派的ID	// to client
    //UNITE_SUBSYN		= 15,			// 合并堂口 // to client // dwData被合并, target是主人
    //UNITE_SYN			= 16,			// 合并帮派 // to client // dwData被合并, target是主人
    SET_WHITE_SYN		= 17,			// 白帮帮派ID // 未被占领请发ID_NONE
    SET_BLACK_SYN		= 18,			// 黑帮帮派ID // 未被占领请发ID_NONE
    DESTROY_SYN			= 19,			// 世界广播，删除帮派。

    NPCMSG_CREATE_SYN	= 101,			// 通知NPC服务器添加帮派	// to npc
    NPCMSG_DESTROY_SYN	= 102,			// 通知NPC服务器删除帮派	// to npc
};


class CMsgSyndicate : public CNetMsg
{
public:
    CMsgSyndicate();
    virtual ~CMsgSyndicate();

    BOOL	Create (unsigned short usAction, OBJID idTarget);

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        unsigned short	usAction;
        OBJID			idTarget;
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // MSGSYNDICATE_H
