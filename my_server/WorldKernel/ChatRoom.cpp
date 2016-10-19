
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// ChatRoom.cpp: implementation of the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "ChatRoom.h"

#define TMPOINT 5*60		//（4）	玩家在线5分钟，增加一点人气值
#define MANAGEROFFLINE_CHECKINTERVAL	3*60
#define APPLYMANAGER_TIMELIMIT	60
#define GETDEPOSIT_TIMEINTERVAL	10*60 //收费用的时间间隔
#define SETROOMFEE_TIMEINTERVAL 5*60 //设置收费的时间间隔
#define CANCELMANAGER_CONDITION_TIME	30*60  // （6）	30分钟之内，聊天室人数没有达到30个人，一分钟之后，系统将会取消当前管理员的管理资格。
#define CANCELMANAGER_CONDITION_MEMNUM	30

#include "AllMsg.h"
#include "UserList.h"
#include "logfile.h"
#include "WorldKernel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CChatRoom, s_heap)

CChatRoom::CChatRoom()
{
    Init();
}

CChatRoom::~CChatRoom()
{
    Clear();
}

CChatRoom* CChatRoom::CreateNew()
{
    return new CChatRoom;
}

void CChatRoom::SetID(int nID)
{
    m_nRoomID = nID;
}

void CChatRoom::SetName(const char* szName)
{
    m_sRoomName = szName;
}

void CChatRoom::EnterRoom(CPlayer* pPlayer)
{
    if(pPlayer)
    {
        stPlayerInfo* pInfo = new stPlayerInfo;
        if(pInfo)
        {
            pInfo->nPoint = 0;
            pInfo->pPlayer = pPlayer;
            m_vtPlayerInfoSet.push_back(pInfo);
            pInfo->m_tmPoint.Startup(TMPOINT);
            //发送欢迎消息
            char szBuffer[128];
            sprintf(szBuffer, STR_CHATROOM_WELCOME, pPlayer->szName, m_sRoomName.c_str(), m_sManagerName.c_str());
            this->SendSysMsg(pPlayer, szBuffer);
            //通知各个成员更新列表
            CMsgChatRoom msg;
            CHECK(msg.Create(ACTION_ENTERROOM, 0, pPlayer->szName));
            pInfo = NULL;
            for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
            {
                pInfo = (*itInfo);
                if(pInfo)
                {
                    CPlayer* pCurPlayer = pInfo->pPlayer;
                    if(pCurPlayer)
                    {
                        pCurPlayer->SendMsg(&msg);
                    }
                }
            }
            //向刚进入的成员发送成员列表
            this->SendMemberList(pPlayer);
            //判断是不是管理员
            if(pPlayer->idUser == this->m_dwManagerID)
            {
                if(m_tmManagerOffline.IsActive())
                {
                    m_tmManagerOffline.Clear();
                }
                //向玩家发送 管理员回来信息
                CMsgChatRoom msg;
                CHECK(msg.Create(ACTION_SETMANAGER, 0, pPlayer->szName));
                this->SendMsgToAllPlayer(&msg);
                m_tmFee.Startup(GETDEPOSIT_TIMEINTERVAL);
            }
        }
    }
}

void CChatRoom::LeaveRoom(CPlayer* pPlayer)
{
    if(pPlayer)
    {
        stPlayerInfo* pInfo = NULL;
        for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
        {
            pInfo = (*itInfo);
            if(pInfo)
            {
                if (pInfo->pPlayer == pPlayer)
                {
                    m_vtPlayerInfoSet.erase(itInfo);
                    delete pInfo;
                    break;
                }
            }
        }
    }
}

void CChatRoom::OnTimer(time_t tCurr)
{
    //---人气值---玩家在线5分钟，增加一点人气值（该数值做为聊天室的中使用），不足5分钟的不计算。
    CheckPoint();
    //---收管理费用---，10分钟收一次费用，
    CheckFee();
    //3分钟到了，清楚管理员信息
    if(m_tmManagerOffline.IsActive())
    {
        //假如管理员离线，停止收费用
        if(m_tmFee.IsActive())
        {
            m_tmFee.Clear();
        }
        if(m_tmManagerOffline.ToNextTime())
        {
            //管理员在规定时间内没有回来，取消他的权限
            char szBuff[128];
            sprintf(szBuff, STR_CHATROOM_CANAPPLYMANAGER, m_sManagerName);
            CMsgTalk msgTalk;
            if (msgTalk.Create(SYSTEM_NAME, ALLUSERS_NAME, szBuff, NULL, 0xff0000, _TXTATR_SYSTEM))
            {
                SendMsgToAllPlayer(&msgTalk);
            }
            this->m_dwManagerID = 0;
            this->m_sManagerName = "";
            m_tmManagerOffline.Clear();
        }
    }
    //申请管理员,允许一分钟内竞争
    if(m_tmApplyManagerTimeLimit.IsActive())
    {
        if(m_tmApplyManagerTimeLimit.ToNextTime())
        {
            //对竞争集合进行排序查询
            int nSize = m_vtApplyManagerSet.size();
            int k = 0;
            stApplyManagerEntity* pK = NULL, *pJ = NULL, *pI = NULL;
            for(int i = 0; i < nSize; i++)
            {
                k = i;
                for(int j = i + 1; j < nSize - 1; j++)
                {
                    pK = m_vtApplyManagerSet[k];
                    pJ = m_vtApplyManagerSet[j];
                    if( pK->nPoint < pJ->nPoint)
                    {
                        k = j;
                    }
                }
                if(k != i)
                {
                    pI = m_vtApplyManagerSet[i];
                    m_vtApplyManagerSet[i] = m_vtApplyManagerSet[k];
                    m_vtApplyManagerSet[k] = pI;
                }
            }
            stApplyManagerEntity* pCurEty = m_vtApplyManagerSet[0];
            if(pCurEty)
            {
                this->m_sManagerName = pCurEty->pPlayer->szName;
                this->m_dwManagerID = pCurEty->pPlayer->idUser;
                CMsgChatRoom msg;
                ULONG ulID = pCurEty->pPlayer->idUser;
                CHECK(msg.Create(ACTION_SETMANAGER, 0, pCurEty->pPlayer->szName));
                this->SendMsgToAllPlayer(&msg);
            }
            m_tmApplyManagerTimeLimit.Clear();
            //清除内存
            stApplyManagerEntity* pAMTmp;
            for(i = 0; i < nSize; i++)
            {
                pAMTmp = m_vtApplyManagerSet[i];
                SAFE_DELETE(pAMTmp);
            }
            m_vtApplyManagerSet.clear();
            m_tmFee.Startup(GETDEPOSIT_TIMEINTERVAL);//启动收费用记时
            m_tmCancelManager.Startup(CANCELMANAGER_CONDITION_TIME); //启动 人员最低限制记时
        }
    }
    //（6）	30分钟之内，聊天室人数没有达到30个人，一分钟之后，系统将会取消当前管理员的管理资格。
    if(m_tmCancelManager.IsActive())
    {
        if(m_tmCancelManager.ToNextTime())
        {
            if(this->m_vtPlayerInfoSet.size() < CANCELMANAGER_CONDITION_MEMNUM)
            {
                //向玩家发送同步消息，放弃管理员
                CMsgChatRoom msg;
                if(msg.Create(ACTION_QUITMANAGER, 0, this->m_sManagerName.c_str()))
                {
                    this->SendMsgToAllPlayer(&msg);
                }
                this->ResetManager();
                m_tmCancelManager.Clear();
            }
        }
    }
}

void CChatRoom::Clear()
{
    stPlayerInfo* pInfo = NULL;
    for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
    {
        pInfo = (*itInfo);
        SAFE_DELETE(pInfo);
    }
    m_vtPlayerInfoSet.clear();
}

void CChatRoom::Init()
{
    m_bIsApplyManager = true;
    m_nFee = 0;
    m_nMaxContain = 200;
    m_dwManagerID = 0;
    m_sRoomName = "";
}

void CChatRoom::CheckPoint()
{
    stPlayerInfo* pInfo = NULL;
    for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
    {
        pInfo = (*itInfo);
        if(pInfo)
        {
            if(pInfo->pPlayer->GetID() != m_dwManagerID)
            {
                if(pInfo->m_tmPoint.IsActive())
                {
                    if(pInfo->m_tmPoint.ToNextTime())
                    {
                        pInfo->nPoint++;
                        //给客户端发送同步消息发送,更新人气值
                        CMsgChatRoom msg;
                        if(msg.Create(ACTION_UPDATEPOINT, pInfo->nPoint, pInfo->pPlayer->szName))
                        {
                            pInfo->pPlayer->SendMsg(&msg);
                        }
                    }
                }
            }
        }
    }
}
//收管理费用
void CChatRoom::CheckFee()
{
    if(m_tmFee.IsActive())
    {
        if(m_tmFee.ToNextTime())
        {
            CPlayer* pUser = UserList()->GetPlayerBySocket(m_dwManagerID);
            if(!pUser)
            {
                return;
            }
            DWORD dwFeeSum = 0;
            stPlayerInfo* pInfo = NULL;
            for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
            {
                pInfo = (*itInfo);
                if(pInfo)
                {
                    //向非管理员的MAP线程发送 收费消息
                    if(pInfo->pPlayer->GetID() != m_dwManagerID)
                    {
                        dwFeeSum = CutTrail(dwFeeSum, dwFeeSum + m_nFee);
                        //发送消息...
                        CMsgChatRoom msg;
                        if(msg.Create(ACTION_FEE_DEDUCT, m_nFee))
                        {
                            GameWorld()->SendMapGroupMsgForChatRoom(pInfo->pPlayer->idProcess, pInfo->pPlayer->idSocket, &msg);
                        }
                    }
                }
            }
            //---向管理员发送得到的费用消息 , dwFeeSum
            pUser = UserList()->GetPlayerBySocket(m_dwManagerID);
            if(pUser)
            {
                CMsgChatRoom msg;
                if(msg.Create(ACTION_FEE_INCOME, dwFeeSum))
                {
                    GameWorld()->SendMapGroupMsgForChatRoom(pUser->idProcess, pUser->idSocket, &msg);
                }
            }
        }
    }
}

int CChatRoom::GetRoomID()
{
    return m_nRoomID;
}

int CChatRoom::GetFee()
{
    return m_nFee;
}

bool CChatRoom::SendSysMsg(CPlayer* pPlayer, char* szBuffer)
{
    CMsgTalk msgTalk;
    if (msgTalk.Create(SYSTEM_NAME, ALLUSERS_NAME, szBuffer, NULL, 0xff0000, _TXTATR_SYSTEM))
    {
        pPlayer->SendMsg(&msgTalk);
        return true;
    }
    return false;
}

bool CChatRoom::IsFull()
{
    if(m_vtPlayerInfoSet.size() < m_nMaxContain)
    {
        return false;
    }
    return true;
}

bool CChatRoom::SendMsgToAllPlayer(CNetMsg* pMsg)
{
    stPlayerInfo* pInfo = NULL;
    for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
    {
        pInfo = (*itInfo);
        if(pInfo)
        {
            pInfo->pPlayer->SendMsg(pMsg);
        }
    }
    return true;
}

bool CChatRoom::IsExist(CPlayer* pPlayer)
{
    if(pPlayer)
    {
        stPlayerInfo* pInfo = NULL;
        for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
        {
            pInfo = (*itInfo);
            if(pInfo)
            {
                if (pInfo->pPlayer == pPlayer)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
//申请管理员
bool CChatRoom::ApplyManager(CPlayer* pPlayer)
{
    if(!m_tmApplyManagerTimeLimit.IsActive())
    {
        m_tmApplyManagerTimeLimit.Startup(APPLYMANAGER_TIMELIMIT);
    }
    CHECKF(pPlayer);
    stApplyManagerEntity* pAMEty = new stApplyManagerEntity();
    pAMEty->pPlayer = pPlayer;
    pAMEty->nOrder = m_vtApplyManagerSet.size();
    m_vtApplyManagerSet.push_back(pAMEty);
    return true;
}

bool CChatRoom::IsCanApplyManager()
{
    if(this->m_tmManagerOffline.IsActive() || this->m_dwManagerID != 0)
    {
        return false;
    }
    return true;
}

CPlayer* CChatRoom::GetManager()
{
    stPlayerInfo* pInfo = NULL;
    for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
    {
        pInfo = (*itInfo);
        if(pInfo)
        {
            //向非管理员的MAP线程发送 收费消息
            if(pInfo->pPlayer->GetID() == m_dwManagerID)
            {
                return pInfo->pPlayer;
            }
        }
    }
    return NULL;
}

bool CChatRoom::IsManager(CPlayer* pPlayer)
{
    if(pPlayer->idUser == this->m_dwManagerID)
    {
        return true;
    }
    return false;
}

void CChatRoom::StartupManagerOfflineTimer()
{
    m_tmManagerOffline.Startup(MANAGEROFFLINE_CHECKINTERVAL);
}

void CChatRoom::SetFee(int nFee)
{
    if(m_tmSetFee.IsActive())
    {
        return ;
    }
    this->m_nFee = __min(MAX_FEE, nFee);
    m_tmSetFee.Startup(SETROOMFEE_TIMEINTERVAL);
    //发送系统消息
    char szBuff[128];
    sprintf(szBuff, STR_CHATROOM_ROOMFEE_MODIFY, m_nFee);
    CMsgTalk msgTalk;
    if (msgTalk.Create(SYSTEM_NAME, ALLUSERS_NAME, szBuff, NULL, 0xff0000, _TXTATR_SYSTEM))
    {
        SendMsgToAllPlayer(&msgTalk);
    }
}

void CChatRoom::ResetManager()
{
    this->m_dwManagerID = 0;
    this->m_sManagerName = "";
    m_tmFee.Clear(); //管理员不在不收费用
}

const char* CChatRoom::GetRoomName()
{
    return this->m_sRoomName.c_str();
}

bool CChatRoom::SendMsgToAllExcSelf(CNetMsg* pMsg, CPlayer* pPlayer)
{
    stPlayerInfo* pInfo = NULL;
    for(ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin(); itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
    {
        pInfo = (*itInfo);
        if(pInfo)
        {
            if(pInfo->pPlayer != pPlayer)
            {
                pInfo->pPlayer->SendMsg(pMsg);
            }
        }
    }
    return true;
}

void CChatRoom::SendMemberList(CPlayer* pPlayer)
{
    CMsgChatRoom msg;
    ITPLAYERINFO itInfo = m_vtPlayerInfoSet.begin();
    stPlayerInfo* pInfo = NULL;
    for( ; itInfo != m_vtPlayerInfoSet.end(); ++itInfo)
    {
        pInfo = (*itInfo);
        if(pInfo)
        {
            if(pInfo->pPlayer != pPlayer)
            {
                CHECK(msg.Create(ACTION_MEMBERINFO, 0, (const char*)pInfo->pPlayer->szName));
                pPlayer->SendMsg(&msg);
            }
        }
    }
}

bool CChatRoom::DrawinFee(CPlayer* pPlayer)
{
    CMsgChatRoom msg;
    if(msg.Create(ACTION_FEE_DEDUCT, m_nFee))
    {
        GameWorld()->SendMapGroupMsgForChatRoom(pPlayer->idProcess, pPlayer->idSocket, &msg);
        return true;
    }
    return false;
}

bool CChatRoom::Release()
{
    delete this;
    return true;
}
