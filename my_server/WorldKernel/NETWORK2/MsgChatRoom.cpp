
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// MsgChatRoom.cpp: implementation of the CMsgChatRoom class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "UserList.h"
#include "logfile.h"
using namespace world_kernel;
#include "WorldKernel.h"

#include "ChatRoom.h"
#include "ChatRoomManager.h"

#include "MsgChatRoom.h"


//extern CChatRoomManager g_objChatRoomManager;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgChatRoom::CMsgChatRoom()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_sString.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgChatRoom::~CMsgChatRoom()
{
}

//////////////////////////////////////////////////////////////////////
void CMsgChatRoom::Process(void* pSocket)
{
    CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());
    CHECK(pUser);
    char szString[_MAX_WORDSSIZE];
    m_sString.GetString(0, szString, sizeof(szString));
    switch( m_pInfo->usAction )
    {
    case ACTION_ENTERROOM_APPLY:	//玩家请求进入聊天室，客户端发送到服务器进行请求。
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoom(szString);
            CHECK(pChatRoom);
            if(pChatRoom->IsExist(pUser))
            {
                return;
            }
            if(pChatRoom->IsFull())
            {
                pChatRoom->SendSysMsg(pUser, STR_CHATROOM_MEMBERFULL);
                return;
            }
            int  nMoney = pChatRoom->GetFee();//得到收费金钱
            CHECK(Create(ACTION_ENTERROOM_CHECKMONEY, nMoney, szString));
            GameWorld()->SendMapGroupMsgForChatRoom(pUser->idProcess, pUser->idSocket, this);
        }
        break;
    case ACTION_LEAVEROOM:	//离开聊天室
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            CHECK(pChatRoom);
            if(!pChatRoom->IsExist(pUser))
            {
                return;
            }
            IF_NOT(this->Create(ACTION_LEAVEROOM, 0, pUser->szName)) //通知客户端玩家离开的消息
            return;
            if(pChatRoom->IsManager(pUser))
            {
                pChatRoom->ResetManager();
            }
            pChatRoom->SendMsgToAllPlayer(this);//通知聊天室所有玩家
            pChatRoom->LeaveRoom(pUser);
            char szBuff[128];
            sprintf(szBuff, STR_CHATROOM_LEAVE, pUser->szName, pChatRoom->GetRoomName());
            CMsgTalk msgTalk;
            if (msgTalk.Create(SYSTEM_NAME, ALLUSERS_NAME, szBuff, NULL, 0xff0000, _TXTATR_SYSTEM))
            {
                pChatRoom->SendMsgToAllPlayer(&msgTalk);
            }
        }
        break;
    case ACTION_ENTERROOM_CHECKMONEY: //从mapgroup返回的，允许进入
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoom(szString);
            CHECK(pChatRoom);
            if(pChatRoom->IsExist(pUser))
            {
                return ;
            }
            pChatRoom->EnterRoom(pUser);
        }
        break;
    case ACTION_APPLYMANAGER:	//申请聊天室管理员
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            CHECK(pChatRoom);
            if(!pChatRoom->IsExist(pUser))
            {
                return;
            }
            if(pChatRoom->IsCanApplyManager())
            {
                pChatRoom->ApplyManager(pUser);
            }
        }
        break;
    case ACTION_QUITMANAGER: //放弃聊天室管理员
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            CHECK(pChatRoom);
            if(!pChatRoom->IsExist(pUser))
            {
                return;
            }
            if(pChatRoom->IsManager(pUser))
            {
                pChatRoom->ResetManager();
                if(Create(ACTION_QUITMANAGER, 0, pUser->szName))
                {
                    pChatRoom->SendMsgToAllPlayer(this);
                    pChatRoom->LeaveRoom(pUser);
                }
            }
        }
        break;
    case ACTION_OFFLINE:
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            if(pChatRoom)
            {
                ULONG ulUserID = (ULONG)pUser->GetID();
                IF_NOT(this->Create(ACTION_LEAVEROOM, 0, pUser->szName)) //通知客户端玩家离开的消息
                return;
                pChatRoom->SendMsgToAllExcSelf(this, pUser); //通知聊天室所有玩家
                pChatRoom->LeaveRoom(pUser);
                char szBuff[128];
                sprintf(szBuff, STR_CHATROOM_LEAVE, pUser->szName, pChatRoom->GetRoomName());
                CMsgTalk msgTalk;
                if (msgTalk.Create(SYSTEM_NAME, ALLUSERS_NAME, szBuff, NULL, 0xff0000, _TXTATR_SYSTEM))
                {
                    pChatRoom->SendMsgToAllPlayer(&msgTalk);
                }
                //若是管理员，启动记时
                if(pChatRoom->IsManager(pUser))
                {
                    pChatRoom->StartupManagerOfflineTimer();
                }
            }
        }
        break;
    case ACTION_SETFEE:	//设置聊天费用
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            if(pChatRoom && pChatRoom->IsManager(pUser))
            {
                ULONG ulFee = m_pInfo->ulData;
                pChatRoom->SetFee(ulFee);
                pChatRoom->SendMsgToAllPlayer(this);//同步
            }
        }
        break;
    case ACTION_SETROOMNAME://设置聊天室名字
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            if(pChatRoom && pChatRoom->IsManager(pUser))
            {
                pChatRoom->SetName(szString);
                pChatRoom->SendMsgToAllPlayer(this);//同步
            }
        }
        break;
    case ACTION_KICKMEMBER: //踢
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            if(pChatRoom && pChatRoom->IsManager(pUser))
            {
                CPlayer* pPlayer = UserList()->GetPlayer(szString);
                if(pPlayer)
                {
                    if(pChatRoom->IsExist(pPlayer))
                    {
                        pChatRoom->DrawinFee(pPlayer);
                        IF_NOT(this->Create(ACTION_LEAVEROOM, 0, szString)) //通知客户端玩家离线的消息
                        return;
                        pChatRoom->SendMsgToAllPlayer(this);//通知聊天室所有玩家
                        pChatRoom->LeaveRoom(pUser);
                        char szBuff[128];
                        sprintf(szBuff, STR_CHATROOM_LEAVE, pUser->szName, pChatRoom->GetRoomName());
                        CMsgTalk msgTalk;
                        if (msgTalk.Create(SYSTEM_NAME, ALLUSERS_NAME, szBuff, NULL, 0xff0000, _TXTATR_SYSTEM))
                        {
                            pChatRoom->SendMsgToAllPlayer(&msgTalk);
                        }
                    }
                }
            }
        }
        break;
    case ACTION_TALK: //谈话
        {
            CChatRoom* pChatRoom = g_objChatRoomManager.GetRoomByPlayer(pUser);
            if(pChatRoom)
            {
                pChatRoom->SendMsgToAllExcSelf(this, pUser);
            }
        }
        break;
    default:
        break;
    }
}

bool CMsgChatRoom::Create(unsigned short usAction, ULONG ulData, const char* pString)
{
    // check param
    if(usAction < 0) //  || idTarget == ID_NONE
    {
        ASSERT(!"CMsgSyndicate::Create()");
        return false;
    }
    // init
    this->Init();
    m_pInfo->usAction	= usAction;
    m_pInfo->ulData	= ulData;
    if(pString)
    {
        m_sString.AddString(pString);
    }
    // fill info now
    m_unMsgType	= _MSG_CHATROOM;
    if(pString)
    {
        m_unMsgSize	= sizeof(MSG_Info) - 1 + m_sString.GetSize();
    }
    else
    {
        m_unMsgSize	= sizeof(MSG_Info) - 1;
    }
    return true;
}
