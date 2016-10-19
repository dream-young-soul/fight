
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// ChatRoom.h: interface for the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATROOM_H__BB957C1F_A8DF_4BEB_9859_89E17BECD25F__INCLUDED_)
#define AFX_CHATROOM_H__BB957C1F_A8DF_4BEB_9859_89E17BECD25F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "UserList.h"
#include <string>
#define MAX_FEE	200	//管理员设置的费用的最大值

class CChatRoom
{
private:
    struct stPlayerInfo
    {
        int nOrder ;
        CPlayer* pPlayer;
        int      nPoint;
        CTimeOut m_tmPoint;		//人气点增加的间隔时间
    };

    struct stApplyManagerEntity //申请管理员时，玩家信息
    {
        CPlayer* pPlayer;
        int nOrder;
        int nPoint;
    };

private:

    CTimeOut m_tmFee;			//收费时间间隔
    CTimeOut m_tmSetFee;		//允许进行设置收费参数的时间间隔
    CTimeOut m_tmCancelManager; //（6）	30分钟之内，聊天室人数没有达到30个人，系统将会取消当前管理员的管理资格。
    CTimeOut m_tmManagerOffline;//管理员掉线或者下线后开始检测3分钟后，若M没来，//则废掉这个玩家的管理员权限
    CTimeOut m_tmApplyManagerTimeLimit;
    bool m_bIsApplyManager;		//是否可以申请管理员
    std::string m_sRoomName;	//聊天室名字
    DWORD  m_dwManagerID;	//管理员
    std::string m_sManagerName;	//管理员的名字

    int m_nMaxContain;		//聊天室最大容量
    int m_nFee;				// 聊天室管理员所设置的收费金额

    int m_nRoomID;			//房间的ID

    std::vector<stPlayerInfo*> m_vtPlayerInfoSet;
    typedef std::vector<stPlayerInfo*>::iterator ITPLAYERINFO;

    std::vector<stApplyManagerEntity*> m_vtApplyManagerSet;	//1分钟内申请管理员的玩家集合
    typedef std::vector<stApplyManagerEntity*>::iterator ITAPPLYMANAGERSET;

private:
    void CheckFee();
    void CheckPoint();

public:
    CChatRoom();
    virtual ~CChatRoom();

public:
    static CChatRoom* CreateNew();

public:
    MYHEAP_DECLARATION(s_heap)

public:
    bool Release();
    bool DrawinFee(CPlayer* pPlayer);
    void SendMemberList(CPlayer* pPlayer);
    bool SendMsgToAllExcSelf(CNetMsg* pMsg, CPlayer* pPlayer);
    const char* GetRoomName();
    void ResetManager();
    void SetFee(int nFee);
    void StartupManagerOfflineTimer();
    bool IsManager(CPlayer* pPlayer);
    CPlayer* GetManager();
    bool IsCanApplyManager();
    bool ApplyManager(CPlayer* pPlayer);
    bool IsExist(CPlayer* pPlayer);
    bool SendMsgToAllPlayer(CNetMsg* pMsg);
    bool IsFull();
    bool SendSysMsg(CPlayer* pPlayer, char* szBuffer);
    int GetFee();
    int GetRoomID();
    void Init();
    void Clear();
    void OnTimer(time_t tCurr);
    void LeaveRoom(CPlayer* pPlayer);
    void EnterRoom(CPlayer* pPlayer);
    void SetName(const char* szName);
    void SetID(int nID);
};

#endif // !defined(AFX_CHATROOM_H__BB957C1F_A8DF_4BEB_9859_89E17BECD25F__INCLUDED_)
