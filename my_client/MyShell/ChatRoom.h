
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// ChatRoom.h: interface for the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

//Create By:	Wen Yiku
//Create Date:	2005-6-29
#if !defined(AFX_CHATROOM_H__E61FC286_64C3_404C_8322_7C335C42406A__INCLUDED_)
#define AFX_CHATROOM_H__E61FC286_64C3_404C_8322_7C335C42406A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "basefunc.h"

#include "IMSdk.h"

#include <deque>

using namespace std;

typedef deque<CGroupSummary* >  DEQUE_GROUPLIST;

typedef struct
{
    DWORD dwUin;
    DWORD dwStatus;
    DWORD dwEntryTime;
    char  szNick[16];
    char  szDomain[16];
    char  szIdiograph[256];
} GroupMember;
typedef deque<GroupMember* >  DEQUE_GROUPMEMBER;

class CChatRoom
{
public:
    void DeInit();
    void Init();
    void Logout();
    void Login();
    void SendMsg(UINT dwFriendID = 0);
    //	UINT GetMyUin(){ return im_GetMyUin();}
    void ChangeRoomInfo();
    void ChangeMyInfo();
    void AbandonManger();
    void ApplyManager();
    void KickMember();
    void GetRoomInfo();
    void GetRoomList();
    void EnterRoom(int nIndex);		//
    void EnterRoomByID(UINT GroupID);	//进入xx聊天室
    void LeaveRoom();	//退出当前聊天室
    CChatRoom();
    virtual ~CChatRoom();

    //
    void Reset();
    //聊天室列表
    void SetRoomListInfo(CGroupSummary* pGroupSummary, int iCount);
    CGroupSummary* GetRoomInfoList(int nIndex);
    CGroupSummary* GetRoomInfoListByID(UINT GroupID);
    int	 GetRoomInfoAmount() {return m_setGroupList.size();}

    //当前聊天室信息
    void SetCurRoomInfo(IGroupInfo* pGroup) {m_curGroup = pGroup;}
    IGroupInfo* GetCurRoomInfo() {return m_curGroup;}
    //当前聊天室成员列表
    BOOL AddRoomMember(IGroupInfo* pGroupInfo, IGroupMember* pGroupMember);
    BOOL DelRoomMember(IGroupInfo* pGroupInfo, IGroupMember* pGroupMember);
    BOOL UpdateRoomMember(IGroupMember* pGroupMember);
    int  GetRoomMemberAmount() {return m_setGroupMember.size();}
    GroupMember* GetRoomMemberByIndex(int nIndex);
    GroupMember* GetRoomMemberByUin(int nUin);

    void ClearRoomMembers() ;

private:
    void ClearRoomListInfo();
private:
    DEQUE_GROUPLIST m_setGroupList;				//聊天室列表
    DEQUE_GROUPMEMBER m_setGroupMember;			//当前聊天室成员列表
    IGroupInfo* m_curGroup;						//当前聊天室
    char m_strMsg[256];
    UINT m_dwUin;
    char m_strGroupDomain[64];
    char m_strValue[64];
    UINT m_dwAttrib;
    char m_strGameCode[64];
    char m_strPasswd[32];
    char m_strAccount[32];
    UINT m_dwMyUin;
public:
    UINT m_dwGroupID;

public:
    void SetMsg(char* strMsg) {strcpy(m_strMsg, strMsg);}

};


#ifndef WM_91U_MESSAGE
#define WM_91U_MESSAGE (WM_USER+1000)

const int CMD_91U_LOGIN									= 1000;					//登陆成功
const int CMD_91U_EXIST_UIN								= CMD_91U_LOGIN + 1;	//Uin在别处登录了！
const int CMD_91U_LOGINFAILED							= CMD_91U_LOGIN + 2;	//登录失败
const int CMD_91U_STATUSCHANGED							= CMD_91U_LOGIN + 3; 	//在线状态改变了
const int CMD_91U_OFFLINEERROR							= CMD_91U_LOGIN + 4;	//不正常掉线了
const int CMD_91U_OFFLINE								= CMD_91U_LOGIN + 5;	//正常下线了

const int CMD_91U_SEARCHROOMBACK						= CMD_91U_LOGIN + 6;	//查询聊天室列表返回

const int CMD_91U_ENTERROOMOK							= CMD_91U_LOGIN + 7;	//进入聊天室
const int CMD_91U_ROOMOUTAMOUNT							= CMD_91U_LOGIN + 8;	//超出群允许的最大成员数！
const int CMD_91U_HASENTERED							= CMD_91U_LOGIN + 9;	//该成员已经在群的列表之中
const int CMD_91U_NOTALLOW								= CMD_91U_LOGIN + 10;	//不允许添加群管理员到群中
const int CMD_91U_ENTEROUTTIME							= CMD_91U_LOGIN + 11;	//网络超时
const int CMD_91U_ENTERFAILED							= CMD_91U_LOGIN + 12;	//进入群失败
const int CMD_91U_KICKOUT								= CMD_91U_LOGIN + 15;    //被踢出聊天室

const int CMD_91U_GETROOMINFOBACK						= CMD_91U_LOGIN + 13;	//从服务端取群信息
const int CMD_91U_MODIFYROOMNAME						= CMD_91U_LOGIN + 14;	//修改群信息 -名称
const int CMD_91U_MODIFYROOMBULLETIN					= CMD_91U_LOGIN + 15;	//修改群信息 -改公告
const int CMD_91U_MODIFYROOMSYNOPSIS					= CMD_91U_LOGIN + 16;	//修改群信息 -群简介


const int CMD_91U_MEMBERIN								= CMD_91U_LOGIN + 16;	//成员进入聊天室
const int CMD_91U_MEMBEROUT								= CMD_91U_LOGIN + 18;	//成员退出聊天室
const int CMD_91U_MEMBERKICKOUT							= CMD_91U_LOGIN + 19;	//成员被踢出聊天室
const int CMD_91U_MEMBEROFFLINE							= CMD_91U_LOGIN + 20;   //成员掉线
const int CMD_91U_MEMBERONLINE							= CMD_91U_LOGIN + 21;   //成员上线
const int CMD_91U_MODIFYMEMBERNAME						= CMD_91U_LOGIN + 20;	//修改群成员name
const int CMD_91U_MODIFYMEMBERIDIOGRAPH					= CMD_91U_LOGIN + 21;	//修改群成员签名

const int CMD_91U_CHANGEGROUPMANAGER					= CMD_91U_LOGIN + 21;   //改变群管理员
const int CMD_91U_ABANDONGROUPMANAGER					= CMD_91U_LOGIN + 22;	//放弃群管理员


void Post91UCmd(DWORD dwCommand, int nData = 0);
#endif
extern CChatRoom g_obj91Sdk;
#endif // !defined(AFX_CHATROOM_H__E61FC286_64C3_404C_8322_7C335C42406A__INCLUDED_)
