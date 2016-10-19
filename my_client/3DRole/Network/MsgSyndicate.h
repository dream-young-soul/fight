
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgSyndicate.h: interface for the CMsgSyndicate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNDICATE_H__6D5AF55F_20CC_4A86_931B_878B4A787FEC__INCLUDED_)
#define AFX_MSGSYNDICATE_H__6D5AF55F_20CC_4A86_931B_878B4A787FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"
#include "NetStringPacker.h"

enum
{
    ACTION_NONE			= 0,			// 保留值
    APPLY_JOIN			= 1,			// 申请加入黑社会, id
    INVITE_JOIN			= 2,			// 邀请加入黑社会, id
    LEAVE_SYN			= 3,			// 脱离黑社会
    KICKOUT_MEMBER		= 4,			// 开除黑社会成员,id	(注意：改成ID了)
    QUERY_SYN_NAME		= 6,			// 查询帮派名字
    ALLY_APPLY			= 7,			// 申请/同意结盟
    CLEAR_ALLY			= 8,			// 解除结盟
    ANTAGONIZE			= 9,			// 树敌
    CLEAR_ANTAGONIZE	= 10,			// 解除树敌
    DONATE_MONEY		= 11,			// 帮众捐钱
    QUERY_SYNATTR		= 12,			// 查询帮派资料
    //		SET_MAINSYN			= 13,			// 添加主帮派的ID
    SET_SYN				= 14,			// 添加军团的ID
    //		SET_SUBSUBSYN		= 15,			// 添加小队的ID
    //		UNITE_SUBSYN		= 15,			// 合并堂口 // to client // dwData被合并, target是主人
    //		UNITE_SYN			= 16,			// 合并帮派 // to client // dwData被合并, target是主人
    SET_WHITE_SYN		= 17,			// 白帮帮派ID // 未被占领请发ID_NONE
    SET_BLACK_SYN		= 18,			// 黑帮帮派ID // 未被占领请发ID_NONE
    DESTROY_SYN			= 19,			// 解散帮派 // 自同步
    SET_MANTLE			= 20,			// 设置披风

    APPLY_ALLY			= 21,			// 申请结盟			// to server client
    _CLEAR_ALLY			= 22,			//清除结盟//client to server

    _SET_ANTAGONIZE		= 23,			//树敌 client to server
    _CLEAR_ANTAGONIZE   = 24,			//解除树敌 client to server

    _CREATE_SYN			= 101,			// 通知NPC服务器添加帮派	// to npc
    _DESTROY_SYN		= 102,			// 通知NPC服务器删除帮派	// to npc

    SYN_SET_PUBLISHTIME = 119, //设置公告时间

    //add time:2005-3-21
    KICKOUT_MEMBER_INFO_QUERY = 110, //帮主查询申请开除的成员
    KICKOUT_MEMBER_AGREE = 111,   //帮主同意开除会员
    KICKOUT_MEMBER_NOTAGREE = 112,   //帮主同意开除会员

    SYNMEMBER_ASSIGN	= 113,	//帮派成员编制

    SYN_CHANGE_SUBNAME = 114, //分团改名
    SYN_CHANGE_SUBSUBNAME = 115, //分队改名
    SYN_DEMISE = 116,	 //禅让
    SYN_SET_ASSISTANT = 117, //设置副帮主
    SYN_SET_TEAMLEADER = 118, //设置帮派队长
};

class CMsgSyndicate  : public CNetMsg
{
public:
    CMsgSyndicate();
    virtual ~CMsgSyndicate();

public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);
    BOOL			Create		(unsigned short usAction, OBJID idTarget);
    BOOL			Create		(unsigned short usAction, char* pMsgBuf);
    BOOL			Create      (unsigned short usAction, char* szName, OBJID idTarget);
protected:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        unsigned short	usAction;
        union
        {
            OBJID idTarget;
            DWORD dwData;
        };
        union
        {
            OBJID idFealty; // 上级帮派ID
            OBJID idUser;
        };

        char	szBuf[1];
    } MSG_Info;

    CNetStringPacker m_sName;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSYNDICATE_H__6D5AF55F_20CC_4A86_931B_878B4A787FEC__INCLUDED_)
