
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// MsgChatRoom.h: interface for the CMsgChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCHATROOM_H__1B68DBCB_769A_43F8_9FC3_1A3E09873759__INCLUDED_)
#define AFX_MSGCHATROOM_H__1B68DBCB_769A_43F8_9FC3_1A3E09873759__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
    //---jinggy---聊天室系统---begin
    //规定：5个聊天室的ID: 1 ,2,3,4,5

    ACTION_POINT	= 0,			  //向客户端更新玩家的人气值,客户端收到这个消息后，进行处理

    ACTION_FEE_DEDUCT = 1,		  //从玩家那里扣除聊天费用，这个是服务器间处理的消息。
    ACTION_FEE_INCOME = 2,		  //通知聊天室管理员收取费用

    ACTION_ENTERROOM_APPLY = 3,	  //玩家请求进入聊天室，客户端发送到服务器进行请求。ucAmount(为1),usShort
    ACTION_ENTERROOM_CHECKMONEY = 4, //允许玩家进入聊天室，服务器发给客户端
    ACTION_ENTERROOM	 = 5,   //通知客户端玩家进入聊天室
    ACTION_LEAVEROOM	 = 6,   //离开聊天室

    ACTION_APPLYMANAGER = 7,    //申请聊天室管理员
    ACTION_SETMANAGER   = 8,		//	设置聊天室管理员
    ACTION_QUITMANAGER = 10,        //放弃聊天室管理员

    ACTION_KICKMEMBER = 11,        //踢聊天室成员

    ACTION_SETFEE = 12,			  //设置聊天室费用

    ACTION_SETROOMNAME = 13,		 //设置聊天室名字

    ACTION_OFFLINE = 14,			  //玩家离线

    ACTION_TALK = 15,			  //玩家谈话

    ACTION_MEMBERINFO = 16,		//成员信息
    ACTION_UPDATEPOINT = 17,		//更新玩家人气值

    //---jinggy---聊天室系统---end

};
#include "NetMsg.h"
WORLDKERNEL_BEGIN
class CMsgChatRoom  : public CNetMsg
{
public:
    CMsgChatRoom();
    virtual ~CMsgChatRoom();
public:
    void			Process		(void* pSocket);

public:		//接口函数
    bool Create(unsigned short usAction, ULONG ulData, const char*   pString = NULL);

private:
    typedef struct
    {
        MSGHEAD_DEFINE
        unsigned short	usAction;
        ULONG ulData;
        char	szBuf[1];
    } MSG_Info;

    CNetStringPacker m_sString;

    MSG_Info*	m_pInfo;
};
WORLDKERNEL_END

#endif // !defined(AFX_MSGCHATROOM_H__1B68DBCB_769A_43F8_9FC3_1A3E09873759__INCLUDED_)
