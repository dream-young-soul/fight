
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// MsgChatRoom.cpp: implementation of the CMsgChatRoom class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "mapgroup.h"
#include "transformation.h"
#include "Agent.h"

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
    CUserPtr pUser = UserManager()->GetUser(this);
    CHECK(pUser);
    switch( m_pInfo->usAction )
    {
    case ACTION_ENTERROOM_APPLY://玩家申请进入聊天室
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    case ACTION_LEAVEROOM:	//离开聊天室
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    case ACTION_APPLYMANAGER:	//申请聊天室管理员
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    case ACTION_QUITMANAGER: //放弃聊天室管理员
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    case ACTION_ENTERROOM_CHECKMONEY: //检查钱
        {
            ULONG dwMoney = m_pInfo->ulData;
            if(pUser->SpendMoney(dwMoney, true))
            {
                MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this); //可以进入聊天室
            }
            else
            {
                pUser->SendSysMsg(STR_CHATROOM_CANNOT_INMOREROOM_FORNOMONEY);
            }
        }
        break;
    case ACTION_FEE_DEDUCT://抠钱--对非管理员
        {
            ULONG dwMoney = m_pInfo->ulData;
            if(!pUser->SpendMoney(dwMoney, true))
            {
                //假如没有钱了，从聊天室剔出
                if(this->Create(ACTION_LEAVEROOM))
                {
                    MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
                }
            }
        }
        break;
    case ACTION_FEE_INCOME://收管理费用--对管理员
        {
            ULONG dwMoney = m_pInfo->ulData;
            pUser->AddMoney(dwMoney);
        }
        break;
    case ACTION_SETFEE:	//设置聊天费用
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    case ACTION_SETROOMNAME:
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    case ACTION_OFFLINE:
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    case ACTION_KICKMEMBER: //踢
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
    case ACTION_TALK: //谈话
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pUser->GetSocketID(), this);
        }
        break;
    default:
        break;
    }
}


bool CMsgChatRoom::Create(unsigned short usAction, ULONG ulData, const char* pString)
{
    // check param
    if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
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
