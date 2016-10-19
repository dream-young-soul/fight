
//**********************************************************
// 代码编辑器
//**********************************************************

// 场外赌博
// 仙剑修，2002.1.7
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "BetOutside.h"
#include "MsgBetOutside.h"
#include "logfile.h"
#include "GameMap.h"
#include "MapGroup.h"


const int		MAXBOMONEY		= 30000;
const int		MINBOMONEY		= 100;
const int		BOCOUNTDOWNSECS	= 50;
const int		BOOPENDELAYSECS	= 2;
const int		BOPLAYDICESECS	= 8;

// 仅本模块使用
const int	PAYFOR_SIZE		= 22;
const int	g_aPayFor[PAYFOR_SIZE] = { 2, 2,
                                       216, 216, 216, 216, 216, 216,
                                       72, 36, 21, 14,
                                       10, 8, 8, 8, 8, 10,
                                       14, 21, 36, 72
                                   };

MYHEAP_IMPLEMENTATION(CBet, s_heap)
MYHEAP_IMPLEMENTATION(CNpcBet, s_heap)
// public:
CNpcBet::CNpcBet()
{
    m_nState		= c_betNone;
    m_nCountDown	= 0;
    m_nSysGain		= 0;
    m_nSysGainAll	= 0;
    m_tGain			= clock() + BOSTAT_SECS * CLOCKS_PER_SEC;
    ASSERT(PAYFOR_SIZE == CBet::DICE_VALUE_END);
    m_setBet		= CBetSet::CreateNew(true);	// true: Obj Owner
}

CNpcBet::~CNpcBet()
{
}

CNpcBet* CNpcBet::CreateNew(PROCESS_ID idProcess, CNpc* pNpc)
{
    CNpcBet* pBet = new CNpcBet;
    IF_OK(pBet)
    {
        pBet->m_idProcess	= idProcess;
        pBet->m_pOwner		= pNpc;
        return pBet;
    }
    return NULL;
}

// public:
bool	CNpcBet::JoinBet(CUser* pUser)
{
    CHECKF(pUser);
    if (m_setPlayer.GetObj(pUser->GetID()))		// 重复加入
    {
        m_setPlayer.DelObj(pUser->GetID());
    }
    // join answer
    CMsgName	msg;
    IF_OK(msg.Create(NAMEACT_ADDDICE_PLAYER, pUser->GetName(), m_pOwner->GetID()))
    pUser->SendMsg(&msg);
    // 状态同步
    switch(m_nState)
    {
    case	c_betNone:
        m_nState		= c_betWait;
        m_nCountDown	= BOCOUNTDOWNSECS;
        m_setBet->Clear();
        // break;
    case	c_betWait:
        {
            CMsgBODice	cMsg;
            IF_OK(cMsg.Create(_ACTION_BEGINCHIP, m_pOwner->GetID(), m_nCountDown))
            pUser->SendMsg(&cMsg);
        }
        break;
    case	c_betRun:
        {
            CMsgBODice	cMsg;
            IF_OK(cMsg.Create(_ACTION_ENDCHIP, m_pOwner->GetID()))
            pUser->SendMsg(&cMsg);
        }
        break;
    default:
        LOGERROR("CNpcBet::JoinBet() 异常");
    }
    PLAYER_SET::Iterator pPlayer = m_setPlayer.NewEnum();
    while(pPlayer.Next())
    {
        IF_OK(pPlayer)
        {
            CMsgName	cMsg;
            if (cMsg.Create(NAMEACT_ADDDICE_PLAYER, pPlayer->GetName(), m_pOwner->GetID()))
            {
                pUser->SendMsg(&cMsg);
            }
        }
        CMsgBODice	cMsg;
        CHECKF(cMsg.Create(_ACTION_CHIPIN, m_pOwner->GetID()));
        bool	bOK = false;
        for(int i = 0; i < m_setBet->GetAmount(); i++)
        {
            CBet*	pBet = m_setBet->GetObjByIndex(i);
            ASSERT(pBet);
            if (pBet && pBet->GetID() == pPlayer->GetID())
            {
                if (cMsg.Append(pBet->GetObjType(), pBet->GetMoney()))
                {
                    bOK = true;
                }
            }
        }
        if (bOK)
        {
            pUser->SendMsg(&cMsg);
        }
    }
    // 添加玩家
    m_setPlayer.AddObj(pUser->QueryLink());
    return true;
}

// 切屏时也调用该函数
bool	CNpcBet::LeaveBet(CUser* pUser)
{
    CHECKF(pUser);
    // leave msg in CMsgName
    if (!m_setPlayer.GetObj(pUser->GetID()))		// 未加入
    {
        return false;
    }
    // 状态同步
    switch(m_nState)
    {
    case	c_betWait:
        {
            for(int i = m_setBet->GetAmount() - 1; i >= 0 ; i--)		// 要删，倒着计数
            {
                CBet*	pBet = m_setBet->GetObjByIndex(i);
                ASSERT(pBet);
                if (pBet && pBet->GetID() == pUser->GetID())
                {
                    CMsgBODice	cMsg;
                    if (cMsg.Create(_ACTION_CANCELCHIP, m_pOwner->GetID()) && cMsg.Append(pBet->GetObjType(), pBet->GetMoney()))
                    {
                        BroadcastListMsg(&cMsg, pUser);
                    }
                    int	nMoney = pBet->GetMoney();
                    ASSERT(nMoney > 0);
                    DEBUG_TRY
                    m_setBet->DelObj(pUser->GetID(), pBet->GetObjType());		// 无副作用
                    pUser->GainMoney(nMoney, SYNCHRO_TRUE);
                    DEBUG_CATCH("leave bet")
                    m_nSysGain  -= nMoney;
                }
            }
        }
        break;
    case	c_betRun:
        break;
    default:
        LOGERROR("CNpcBet::LeaveBet() 内部异常");
    }
    // 删除玩家
    m_setPlayer.DelObj(pUser->GetID());
    if (m_setPlayer.Size() == 0)
    {
        m_nState = c_betNone;
    }
    PLAYER_SET::Iterator pPLayer = m_setPlayer.NewEnum();
    while(pPLayer.Next())
    {
        IF_OK(pPLayer)
        {
            CMsgName	cMsg;
            if (cMsg.Create(NAMEACT_DELDICE_PLAYER, pUser->GetName(), m_pOwner->GetID()))
            {
                pPLayer->SendMsg(&cMsg);
            }
        }
    }
    return true;
}

bool	CNpcBet::ChipIn(CUser* pUser, int nType, int nMoney)		//? nMoney==0: for cancel
{
    // 检查参数
    CHECKF(pUser && m_setPlayer.GetObj(pUser->GetID()));
    CHECKF(m_nState != c_betNone);
    CHECKF(nMoney == 0 && nType == CBet::DICE_VALUE_ALL || nMoney > 0 && nType >= 0 && nType < CBet::DICE_VALUE_END);
    CHECKF(nMoney == 0 || nMoney == 100 || nMoney == 1000 || nMoney == 10000);
    char bufText[1024];
    // 时机错误
    if (m_nState != c_betWait)
    {
        // 回传NPC文字消息
        sprintf(bufText, "错失良机，请留意下次...");
        CMsgTalk msg;
        if (msg.Create(m_pOwner->GetName(), pUser->GetName(), bufText))
        {
            pUser->SendMsg(&msg);
        }
        return false;
    }
    // 取上次押的注数量
    int		nAllMoney	= 0;
    int		nThisMoney	= 0;
    for(int i = 0; i < m_setBet->GetAmount(); i++)
    {
        CBet*	pBet = m_setBet->GetObjByIndex(i);
        ASSERT(pBet);
        if (pBet && pBet->GetID() == pUser->GetID())
        {
            nAllMoney += pBet->GetMoney();
            if (pBet->GetObjType() == nType)
            {
                nThisMoney = pBet->GetMoney();
            }
        }
    }
    ASSERT(nAllMoney >= 0);
    ASSERT(nThisMoney >= 0);
    // 过滤多余消息
    if ( nMoney > 0 && pUser->GetMoney() < nMoney
            //|| nMoney == 0 && nThisMoney == 0
            || nAllMoney + nMoney > MAXBOMONEY )
    {
        return false;
    }
    // 同意下注
    ASSERT(nThisMoney + nMoney >= 0);
    // 添加
    if (nMoney > 0)
    {
        CBet* pBet = new CBet(pUser->GetID(), nType, nThisMoney + nMoney);
        ASSERT(pBet);
        if (!pBet)
        {
            return false;
        }
        m_setBet->DelObj(pUser->GetID(), nType);
        DEBUG_TRY
        pUser->SpendMoney(nMoney, SYNCHRO_TRUE);			// 押注, nMoney可为负
        m_setBet->AddObj(pBet);
        DEBUG_CATCH("add bet")
        m_nSysGain		+= nMoney;
    }
    else
    {
        if (nType == CBet::DICE_VALUE_ALL)
        {
            for(int i = m_setBet->GetAmount() - 1; i >= 0 ; i--)		// 要删，倒着计数
            {
                CBet*	pBet = m_setBet->GetObjByIndex(i);
                ASSERT(pBet);
                if (pBet && pBet->GetID() == pUser->GetID())
                {
                    CMsgBODice	cMsg;
                    if (cMsg.Create(_ACTION_CANCELCHIP, m_pOwner->GetID()) && cMsg.Append(pBet->GetObjType(), pBet->GetMoney()))
                    {
                        BroadcastListMsg(&cMsg, pUser);
                    }
                    if (cMsg.Create(_ACTION_CANCELCHIP_CONFIRM, m_pOwner->GetID()) && cMsg.Append(pBet->GetObjType(), pBet->GetMoney()))
                    {
                        pUser->SendMsg(&cMsg);
                    }
                    int	nMoney = pBet->GetMoney();
                    ASSERT(nMoney > 0);
                    DEBUG_TRY
                    m_setBet->DelObj(pUser->GetID(), pBet->GetObjType());		// 无副作用
                    pUser->GainMoney(nMoney, SYNCHRO_TRUE);
                    DEBUG_CATCH("del bet")
                    m_nSysGain		-= nMoney;
                }
            }
        }
        else
        {
            m_setBet->DelObj(pUser->GetID(), nType);    //? m_nSysGain		-= nThisMoney;
        }
    }
    // 广播
    if (nMoney > 0)
    {
        CMsgBODice	cMsg;
        if (cMsg.Create(_ACTION_CHIPIN_CONFIRM, m_pOwner->GetID()) && cMsg.Append(nType, nMoney))
        {
            pUser->SendMsg(&cMsg);
        }
        if (cMsg.Create(_ACTION_CHIPIN, m_pOwner->GetID()) && cMsg.Append(nType, nMoney))
        {
            BroadcastListMsg(&cMsg, pUser);
        }
    }
    else
    {
        CMsgBODice	cMsg;
        if (cMsg.Create(_ACTION_CANCELCHIP_CONFIRM, m_pOwner->GetID()) && cMsg.Append(nType, nMoney))
        {
            pUser->SendMsg(&cMsg);
        }
        if (cMsg.Create(_ACTION_CANCELCHIP, m_pOwner->GetID()) && cMsg.Append(nType, nThisMoney))
        {
            BroadcastListMsg(&cMsg, pUser);
        }
    }
    return true;
}

void	CNpcBet::OnTimer()
{
    if (!m_tClock.ToNextTime(1))
    {
        return ;
    }
    switch(m_nState)
    {
    case	c_betNone:
        break;
    case	c_betWait:
        --m_nCountDown;
        if (m_nCountDown <= 0)
        {
            ProcessRun();
            m_nState = c_betRun;
        }
        break;
    case	c_betRun:
        --m_nCountDown;
        if (m_nCountDown == -BOOPENDELAYSECS)
        {
            ProcessOpen();
            // 输出LOG
            if (m_nSysGain && clock() > m_tGain)
            {
                m_nSysGainAll	+= m_nSysGain;
                LOGMSG("BET STAT: 骰子赌场累计收入[%d], 半小时收入[%d]", m_nSysGainAll, m_nSysGain);
                m_nSysGain		= 0;
                m_tGain		= clock() + BOSTAT_SECS * CLOCKS_PER_SEC;
            }
        }
        else if (m_nCountDown <= -(BOOPENDELAYSECS + BOPLAYDICESECS))
        {
            ProcessNew();
            // 重新初始化
            m_nState		= c_betWait;
            m_nCountDown	= BOCOUNTDOWNSECS;
            m_setBet->Clear();
        }
        break;
    default:
        LOGERROR("CNpcBet::OnTimer() 内部异常");
    }
}

void CNpcBet::BroadcastListMsg(CNetMsg* pMsg, CUser* pExcludeUser)
{
    PLAYER_SET::Iterator pUser = m_setPlayer.NewEnum();
    while(pUser.Next())
    {
        if (pUser && (pExcludeUser == NULL || pUser->GetID() != pExcludeUser->GetID()))
        {
            pUser->SendMsg(pMsg);
        }
    }
}

void CNpcBet::ProcessRun()
{
    // 停手
    // 通知所有客户端停止下注
    CMsgBODice	cMsg;
    if (cMsg.Create(_ACTION_ENDCHIP, m_pOwner->GetID()))
    {
        BroadcastListMsg(&cMsg, NULL);
    }
}

void CNpcBet::ProcessOpen()
{
    // 开局
    // 掷色子
    srand(clock() + m_setPlayer.Size() + m_setBet->GetAmount());
    for(int i = 0; i < 4; i++)
    {
        m_aDice[i] = (rand() % 6) + 1;
    }
    CMsgBODice		cMsg;
    if (cMsg.Create(_ACTION_DICE, m_pOwner->GetID(), m_aDice))
    {
        m_pOwner->BroadcastRoomMsg(&cMsg);
    }
    //				m_setPlayer.BroadcastMsg(&cMsg);
    // 更新并广播胜局
    //			CUser*	pDelUser = NULL;	// 指向上一个输家
    for(int i = m_setBet->GetAmount() - 1; i >= 0 ; i--)			// 要删除，倒着计数
    {
        CBet*	pBet = m_setBet->GetObjByIndex(i);
        IF_NOT(pBet)
        continue;
        OBJID	idUser	= pBet->GetID();
        int		nType	= pBet->GetObjType();
        int		nMoney	= pBet->GetMoney();
        CUser*	pUser	= UserManager()->GetUser(idUser);
        ASSERT(pUser && nMoney >= MINBOMONEY && nMoney <= MAXBOMONEY);
        if (!(pUser && nMoney >= MINBOMONEY && nMoney <= MAXBOMONEY))
        {
            LOGWARNING("赌骰子的玩家掉线或下注错误。玩家ID[%u]，钱[%d]", idUser, nMoney);
            m_setBet->DelObj(idUser, nType);
            continue;
        }
        int		nDiceSum = m_aDice[0] + m_aDice[1] + m_aDice[2];
        switch(nType)
        {
        case	CBet::DICE_VALUE_SMALL:
            if (nDiceSum <= 10)
            {
                nMoney *= g_aPayFor[nType];
            }
            else
            {
                nMoney = 0;
            }
            break;
        case	CBet::DICE_VALUE_BIG:
            if (nDiceSum >= 11)
            {
                nMoney *= g_aPayFor[nType];
            }
            else
            {
                nMoney = 0;
            }
            break;
        case	CBet::DICE_VALUE_111:
        case	CBet::DICE_VALUE_222:
        case	CBet::DICE_VALUE_333:
        case	CBet::DICE_VALUE_444:
        case	CBet::DICE_VALUE_555:
        case	CBet::DICE_VALUE_666:
            if (m_aDice[0] == m_aDice[1]
                    && m_aDice[1] == m_aDice[2]
                    && m_aDice[2] == nType - CBet::DICE_VALUE_111 + 1)
            {
                nMoney *= g_aPayFor[nType];
                //						::GmLogSave("玩家[%s]赌豹子[%d]赢了[%d]两银子", pUser->GetName(), nType, nMoney);
            }
            else
            {
                nMoney = 0;
            }
            break;
        case	CBet::DICE_VALUE_4:
        case	CBet::DICE_VALUE_5:
        case	CBet::DICE_VALUE_6:
        case	CBet::DICE_VALUE_7:
        case	CBet::DICE_VALUE_8:
        case	CBet::DICE_VALUE_9:
        case	CBet::DICE_VALUE_10:
        case	CBet::DICE_VALUE_11:
        case	CBet::DICE_VALUE_12:
        case	CBet::DICE_VALUE_13:
        case	CBet::DICE_VALUE_14:
        case	CBet::DICE_VALUE_15:
        case	CBet::DICE_VALUE_16:
        case	CBet::DICE_VALUE_17:
            if (nDiceSum == nType - CBet::DICE_VALUE_4 + 4)
            {
                nMoney *= g_aPayFor[nType];
            }
            else
            {
                nMoney = 0;
            }
            break;
        default:
            m_setBet->DelObj(idUser, nType);
            LOGERROR("CNpcBet::OnTimer() switch 内部异常");
        }
        // 更新下注表
        if (nMoney)
        {
            CBet* pBet = m_setBet->GetObj(idUser, nType);
            if (pBet)
            {
                pBet->SetMoney(nMoney);
            }
            ASSERT(nMoney > 0);
            if (nMoney > 0)
            {
                pUser->GainMoney(nMoney, UPDATE_TRUE);	// 发钱
                m_nSysGain		-= nMoney;
            }
            // 广播
            CMsgName	cMsg;
            if (cMsg.Create(NAMEACT_DICE_BONUS, pUser->GetName(), nMoney))
            {
                BroadcastListMsg(&cMsg, NULL);
            }
            // 大赢家广播
            if (nMoney > 2 * MAXBOMONEY)
            {
                char bufText[1024];
                sprintf(bufText, STRING_BET_WIN, pUser->GetName(), nMoney);
                pUser->BroadcastRoomMsg(bufText, INCLUDE_SELF);
            }
            //					pDelUser = NULL;	// 不删除该下注项
        }
        else
        {
            m_setBet->DelObj(idUser, nType);
        }
    } // for(i--)
}

void CNpcBet::ProcessNew()
{
    // 重新下注
    // 通知所有客户端开始下注
    CMsgBODice	cMsg;
    if (cMsg.Create(_ACTION_BEGINCHIP, m_pOwner->GetID(), BOCOUNTDOWNSECS))
    {
        BroadcastListMsg(&cMsg, NULL);
    }
}
