
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
    ACTION_NONE = 0,				// 保留值

    APPLY_JOIN			= 1,			// 申请加入黑社会, id
    INVITE_JOIN			= 2,			// 邀请加入黑社会, id
    LEAVE_SYN			= 3,			// 脱离黑社会
    KICKOUT_MEMBER		= 4,			// 开除黑社会成员, name
    QUERY_SYN_NAME		= 6,			// 查询帮派名字
    SET_ALLY			= 7,			// 结盟				// to client, npc(npc_id is syn_id, same follow)
    CLEAR_ALLY			= 8,			// 解除结盟			// to client, npc
    SET_ANTAGONIZE		= 9,			// 树敌				// to client, npc
    CLEAR_ANTAGONIZE	= 10,			// 解除树敌			// to client, npc
    DONATE_MONEY		= 11,			// 帮众捐钱
    QUERY_SYNATTR		= 12,			// 查询帮派信息		// to server

    SET_SYN				= 14,			// 添加帮派ID		// to client
    //UNITE_SUBSYN		= 15,			// 合并堂口 // to client // dwData被合并, target是主人
    //UNITE_SYN			= 16,			// 合并帮派 // to client // dwData被合并, target是主人
    SET_WHITE_SYN		= 17,			// 白帮帮派ID // 未被占领请发ID_NONE
    SET_BLACK_SYN		= 18,			// 黑帮帮派ID // 未被占领请发ID_NONE
    DESTROY_SYN			= 19,			// 世界广播，删除帮派。
    SET_MANTLE          = 20,            // 世界广播，披风 // add huang 2004.1.1

    _APPLY_ALLY			= 21,			// 申请结盟			// to server&client, idTarget=SynLeaderID
    _CLEAR_ALLY			= 22,			// 清除结盟			// to server

    _SET_ANTAGONIZE		= 23,			//树敌 client to server
    _CLEAR_ANTAGONIZE   = 24,			//解除树敌 client to server

    NPCMSG_CREATE_SYN	= 101,			// 通知NPC服务器添加帮派	// to npc
    NPCMSG_DESTROY_SYN	= 102,			// 通知NPC服务器删除帮派	// to npc

    KICKOUT_MEMBER_INFO_QUERY	= 110,	//帮主查询申请开除的成员
    KICKOUT_MEMBER_AGREE		= 111,	//帮主同意开除会员
    KICKOUT_MEMBER_NOAGREE		= 112,	//帮主不同意开除会员

    SYNMEMBER_ASSIGN	= 113,			//帮派成员编制

    SYN_CHANGE_NAME		= 114,			// 帮派改名




    SYN_CHANGE_SUBNAME		= 114,		//分团改名
    SYN_CHANGE_SUBSUBNAME	= 115,		//分队改名

    SYN_DEMISE				= 116,		//禅让
    SYN_SET_ASSISTANT		= 117,		//设置副帮主
    SYN_SET_TEAMLEADER		= 118,		//设置帮派队长
    SYN_SET_PUBLISHTIME		= 119,		//设置公告时间

};

#define SYN_CONDITION_NEWBIE 20 //20级以下为预备团员



class CMsgSyndicate : public CNetMsg
{
public:
    CMsgSyndicate();
    virtual ~CMsgSyndicate();

    BOOL	Create(unsigned short usAction, OBJID idTarget, OBJID idFealty = ID_NONE);

public:

    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(CGameSocket* pSocket);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        unsigned short	usAction;
        union
        {
            OBJID	idTarget;
            DWORD	dwData;
        };
        union
        {
            OBJID	idFealty;				// 上级帮派ID
            OBJID	idUser;					// 仅用于修改帮派编制时
        };
        char	szBuf[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;

    CNetStringPacker m_StrPacker;

};

#endif // MSGSYNDICATE_H
