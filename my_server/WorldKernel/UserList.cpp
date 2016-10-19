
//**********************************************************
// 代码编辑器
//**********************************************************

// UserList.cpp: implementation of the CUserList class.
//
//////////////////////////////////////////////////////////////////////
#include "MessagePort.h"
#include "common.h"
#include "NetMsg.h"
#include "I_mydb.h"
#include "protocol.h"
#include "inifile.h"
#include "MsgFee_A.h"
using namespace world_kernel;
#include "AllMsg.h"
#include "supermanlist.h"
#include "UserList.h"
#include "MessageBoard.h"
#include "WorldKernel.h"

const int	SECS_PER_GAMEHOUR		= 300;			// send CMsgTime to all user
const int	NEWBIE_ATTR_POINT		= 10;			// 新建角色属性点总和
//////////////////////////////////////////////////////////////////////
void CPlayer::OnTimer(time_t tCurr)
{
    if (IsAgent())
    {
        return;
    }
    if (m_nLevel < LOGIN_FREE_LEVEL)
    {
        m_tPoint	= time(NULL);
        if (m_tTick.ToNextTime(POINTFEE_SECS))
        {
            GameWorld()->SendFee(m_idAccount, CMsgFee_A::FEE_TICK);
        }
        return;
    }
    // account server
    if (GameWorld()->IsAccountLoginOK() && time(NULL) > m_tPoint + POINTFEE_DELAY_SECS)
    {
        m_tPoint += POINTFEE_SECS;
        GameWorld()->SendFee(m_idAccount, CMsgFee_A::FEE_POINT);
    }
}

bool CPlayer::SendMsg(CNetMsg* pMsg)
{
    if (IsAgent())
    {
        return GameWorld()->SendNpcMsg(GetID(), pMsg);
    }
    else
    {
        return GameWorld()->SendClientMsg(idSocket, pMsg);
    }
}


MYHEAP_IMPLEMENTATION(CUserList, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserList::CUserList()
{
    m_nPlayerAmount		= 0;
    m_nMaxPlayerAmount	= 0;
    m_nHour				= 6;		// start server in 8 am.
    m_setAgent			= AGENT_SET::CreateNew(true);
}

//////////////////////////////////////////////////////////////////////
CUserList::~CUserList()
{
}

//////////////////////////////////////////////////////////////////////
bool CUserList::Create(ISocket* pSocket, IDatabase* pDb)
{
    CHECKF(pSocket);
    m_pSocket	= pSocket;
    m_pSupermanSet = CSupermanList::CreateNew();
    CHECKF(m_pSupermanSet && m_pSupermanSet->Create(pDb));
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserList::LoginUser(LPCTSTR szAccount, LPCTSTR szPassword, SOCKET_ID idSocket)	// 仅用于非帐号服务器启动
{
#ifdef	ACCOUNT_ENABLE
    return false;
#endif
    bool	bRet = false;
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id, name, account_id, recordmap_id FROM %s WHERE account='%s' && password='%s'", _TBL_USER, szAccount, szPassword);
    IRecordset*	pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);		//VVVVVVVVVVVVVVVVVVVVVVVV
    if (pRes)
    {
        OBJID	idUser	= pRes->LoadDWord("id");
        NAMESTR	szName = "无";
        pRes->LoadString(szName, "name", _MAX_NAMESIZE);
        OBJID	idAccount	= pRes->LoadDWord("account_id");
        OBJID	idMap	= pRes->LoadDWord("recordmap_id");
        PROCESS_ID	idProcess	= MapList()->GetMapProcessID(idMap);
        ASSERT (idProcess != PROCESS_NONE);
        int		nLevel	= 0;		// 未帐号服务器登录，不需要等级
        CreateUser(idProcess, idUser, szName, idAccount, idSocket, nLevel, szAccount);
        bRet = true;
        pRes->Release();;		//AAAAAAAAAAAAAAAAAAAAAAAAAA
        GameWorld()->SendFee(idAccount, CMsgFee_A::FEE_BEGIN);
    }
    {
        m_nPlayerAmount++;
        if (m_nMaxPlayerAmount < m_nPlayerAmount)
        {
            m_nMaxPlayerAmount = m_nPlayerAmount;
        }
        extern struct STAT_STRUCT	g_stat;
        InterlockedExchange(&g_stat.nAllPlayers, m_nPlayerAmount);
        InterlockedExchange(&g_stat.nMaxPlayers, m_nMaxPlayerAmount);
        InterlockedIncrement(&g_stat.nLoginPlayers);
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////
// enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };
int CUserList::LoginUser(OBJID idAccount, SOCKET_ID idSocket, LPCTSTR szInfo)
{
    int	nRet = LOGIN_NEW;
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id, name, account_id, recordmap_id, level FROM %s WHERE account_id='%u'", _TBL_USER, idAccount);
    IRecordset*	pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);		//VVVVVVVVVVVVVVVVVVVVVVVV
    if (pRes)
    {
        OBJID	idUser	= pRes->LoadDWord("id");
        NAMESTR	szName = "无";
        pRes->LoadString(szName, "name", _MAX_NAMESIZE);
        OBJID	idAccount	= pRes->LoadDWord("account_id");
        OBJID	idMap	= pRes->LoadDWord("recordmap_id");
        PROCESS_ID	idProcess	= MapList()->GetMapProcessID(idMap);
        int		nLevel	= pRes->LoadInt("level");
        if (!(strcmp(szInfo, FLAG_NO_POINT) == 0 && nLevel >= LOGIN_FREE_LEVEL)
                && CreateUser(idProcess, idUser, szName, idAccount, idSocket, nLevel, szInfo))
        {
            nRet = LOGIN_OK;
            GameWorld()->SendFee(idAccount, CMsgFee_A::FEE_BEGIN);
        }
        else
        {
            nRet = LOGIN_BAN;
            GameWorld()->SendFee(idAccount, CMsgFee_A::FEE_OFFLINE);
        }
        pRes->Release();		//AAAAAAAAAAAAAAAAAAAAAAAAAA
    }
    return nRet;
}

//////////////////////////////////////////////////////////////////////
bool CUserList::LoginAgent(OBJID idAgent)
{
    int	nRet = LOGIN_NEW;
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id, name, recordmap_id, level FROM %s WHERE id=%u && account_id=0", _TBL_USER, idAgent);
    IRecordset*	pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);		//VVVVVVVVVVVVVVVVVVVVVVVV
    if (pRes)
    {
        OBJID	idUser	= pRes->LoadDWord("id");
        NAMESTR	szName = "无";
        pRes->LoadString(szName, "name", _MAX_NAMESIZE);
        OBJID	idAccount	= ID_NONE;
        SOCKET_ID	idSocket	= SOCKET_NONE;
        OBJID	idMap	= pRes->LoadDWord("recordmap_id");
        PROCESS_ID	idProcess	= MapList()->GetMapProcessID(idMap);
        int		nLevel	= pRes->LoadInt("level");
        pRes->Release();		//AAAAAAAAAAAAAAAAAAAAAAAAAA
        if (CreateAgent(idProcess, idAgent, szName))
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUserList::LogoutUser(SOCKET_ID idSocket)		// return false: no this user
{
    if (!( idSocket != SOCKET_NONE && idSocket < m_setUser.size() && m_setUser[idSocket] ))
    {
        return false;
    }
    // account server
    CPlayer* pPlayer = m_setUser[idSocket];
    CHECKF(pPlayer);
    GameWorld()->SendFee(pPlayer->m_idAccount, CMsgFee_A::FEE_END);
    int	tCurr = time(NULL);
    if (tCurr > pPlayer->m_tPoint + POINTFEE_SECS)
    {
        LOGWARNING("玩家[%s][%u]退出时有[%d]个点没有成功上传!",
                   pPlayer->szName, pPlayer->GetID(), (tCurr - pPlayer->m_tPoint) / POINTFEE_SECS);
    }
    // gm ?
    if (pPlayer->IsGM())
    {
        vector<OBJID>::iterator iter;
        for (iter = m_setGm.begin(); iter != m_setGm.end(); iter++)
        {
            if (pPlayer->GetID() == *iter)
            {
                m_setGm.erase(iter);
                break;
            }
        }
    }
    {
        // stat
        m_nPlayerAmount--;
        extern struct STAT_STRUCT	g_stat;
        InterlockedExchange(&g_stat.nAllPlayers, m_nPlayerAmount);
        InterlockedIncrement(&g_stat.nLogoutPlayers);
    }
    {
        SAFE_DELETE(m_setUser[idSocket]);
        m_setUser[idSocket] = NULL;
        return true;
    }
}

//////////////////////////////////////////////////////////////////////
bool CUserList::KickOutAccount(OBJID idAccount, LPCTSTR szText)		// return false: 不在线
{
    CHECKF(szText);
    CPlayer* pPlayer = GetPlayerByAccountID(idAccount);
    if (!pPlayer)
    {
        return false;
    }
    LOGDEBUG("DEBUG：世界核心踢出帐号[%u]，SOCKET_ID[%d]", idAccount, pPlayer->idSocket);
    {
        CMsgTalk	msg;
        if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, (char*)szText, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
        {
            pPlayer->SendMsg(&msg);
        }
    }
    return GameWorld()->GetSocketInterface()->CloseSocket(pPlayer->idSocket);
}

//////////////////////////////////////////////////////////////////////
bool CUserList::KickOutSocket(SOCKET_ID idSocket)		// return false: 不在线
{
    LOGDEBUG("DEBUG：世界核心踢出SOCKET_ID[%d]", idSocket);
    return GameWorld()->GetSocketInterface()->CloseSocket(idSocket);
}

//////////////////////////////////////////////////////////////////////
void CUserList::OnTimer(time_t tCurr)
{
    for(int i = 0; i < m_setUser.size(); i++)
    {
        if (m_setUser[i])
        {
            m_setUser[i]->OnTimer(tCurr);
        }
    }
    if (m_tHour.ToNextTick(SECS_PER_GAMEHOUR))
    {
        m_nHour = (++m_nHour) % 24;
        /*
        		CMsgTime	msg;
        		IF_OK(msg.Create(m_nHour))
        		{
        			for(int i = 0; i < m_setUser.size(); i++)
        			{
        				if (m_setUser[i])
        					GameWorld()->SendClientMsg(i, &msg);
        			}
        		}
        */
    }
}

//////////////////////////////////////////////////////////////////////
void CUserList::SendTimeToSocket(SOCKET_ID idSocket)
{
    /*	CMsgTime	msg;
    	IF_OK(msg.Create(m_nHour))
    		GameWorld()->SendClientMsg(idSocket, &msg);
    */
}

//////////////////////////////////////////////////////////////////////
void CUserList::LogoutAllUser()
{
}

//////////////////////////////////////////////////////////////////////
bool CUserList::CreateUser(PROCESS_ID idProcess, OBJID idUser, LPCTSTR szName, OBJID idAccount, SOCKET_ID idSocket, int nLevel, LPCTSTR szNotify)
{
    if (idSocket >= m_setUser.size())
    {
        m_setUser.resize(idSocket + 1, NULL);    // +1 : 保证有m_setUser[idSocket]这个成员
    }
    IF_NOT(!m_setUser[idSocket])
    {
        LOGERROR("玩家使用相同SOCKET登录，新玩家[%s][%u]，SOCKET[%d]，旧玩家[%s][%d]",
                 szName, idUser, idSocket, m_setUser[idSocket]->szName, m_setUser[idSocket]->idUser);
        // gm ?
        if (m_setUser[idSocket]->IsGM())
        {
            vector<OBJID>::iterator iter;
            for (iter = m_setGm.begin(); iter != m_setGm.end(); iter++)
            {
                if (m_setUser[idSocket]->GetID() == *iter)
                {
                    m_setGm.erase(iter);
                    break;
                }
            }
        }
        KickOutSocket(idSocket);
        return false;
    }
    m_setUser[idSocket] = new CPlayer;
    if (!m_setUser[idSocket])
    {
        return false;
    }
    m_setUser[idSocket]->idUser		= idUser;
    SafeCopy(m_setUser[idSocket]->szName, szName, _MAX_NAMESIZE);
    m_setUser[idSocket]->idProcess	= idProcess;
    SafeCopy(m_setUser[idSocket]->szAccount, szNotify, _MAX_NAMESIZE);
    m_setUser[idSocket]->idSocket	= idSocket;
    m_setUser[idSocket]->m_idAccount	= idAccount;
    m_setUser[idSocket]->m_nLevel		= nLevel;
    SafeCopy(m_setUser[idSocket]->szNotify, szNotify, _MAX_NAMESIZE);
    m_setUser[idSocket]->m_tPoint	= time(NULL);
    // gm ?
    if (m_setUser[idSocket]->IsGM())
    {
        m_setGm.push_back(m_setUser[idSocket]->GetID());
    }
    {
        // stat
        m_nPlayerAmount++;
        if (m_nMaxPlayerAmount < m_nPlayerAmount)
        {
            m_nMaxPlayerAmount = m_nPlayerAmount;
        }
        extern struct STAT_STRUCT	g_stat;
        InterlockedExchange(&g_stat.nAllPlayers, m_nPlayerAmount);
        InterlockedExchange(&g_stat.nMaxPlayers, m_nMaxPlayerAmount);
        InterlockedIncrement(&g_stat.nLoginPlayers);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserList::CreateAgent(PROCESS_ID idProcess, OBJID idAgent, LPCTSTR szName)
{
    CPlayer* pAgent = new CPlayer;
    if (!pAgent)
    {
        return false;
    }
    pAgent->idUser			= idAgent;
    SafeCopy(pAgent->szName, szName, _MAX_NAMESIZE);
    pAgent->idProcess		= idProcess;
    pAgent->szAccount[0]	= 0;
    pAgent->idSocket		= SOCKET_NONE;
    pAgent->m_idAccount		= ID_NONE;
    pAgent->m_nLevel		= 0;
    pAgent->szNotify[0]		= 0;
    m_setAgent->AddObj(pAgent);
    return true;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CUserList::GetPlayer(LPCTSTR szName)
{
    for(int i = 0; i < m_setUser.size(); i++)
    {
        if (m_setUser[i])
        {
            if (strcmp(m_setUser[i]->szName, szName) == 0)
            {
                return m_setUser[i];
            }
        }
    }
    AGENT_SET::Iterator pAgent = m_setAgent->NewEnum();
    while(pAgent.Next())
    {
        if (strcmp(pAgent->szName, szName) == 0)
        {
            return pAgent;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CUserList::GetPlayer(OBJID idUser)
{
    for(int i = 0; i < m_setUser.size(); i++)
    {
        if (m_setUser[i])
        {
            if (m_setUser[i]->idUser == idUser)
            {
                return m_setUser[i];
            }
        }
    }
    AGENT_SET::Iterator pAgent = m_setAgent->NewEnum();
    while(pAgent.Next())
    {
        if (pAgent->GetID() == idUser)
        {
            return pAgent;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CUserList::GetPlayerByAccount(LPCTSTR szAccount)
{
    for(int i = 0; i < m_setUser.size(); i++)
    {
        if (m_setUser[i])
        {
            if (strcmp(m_setUser[i]->szAccount, szAccount) == 0)
            {
                return m_setUser[i];
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CUserList::GetPlayerByAccountID(OBJID idAccount)
{
    for(int i = 0; i < m_setUser.size(); i++)
    {
        if (m_setUser[i])
        {
            if (m_setUser[i]->m_idAccount == idAccount)
            {
                return m_setUser[i];
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUserList::CreateNewPlayer(LPCTSTR szAccount, LPCTSTR szName, LPCTSTR szPassword,
                                int unLook, int nData, OBJID idAccount, char cLength, char cFat)
{
    int idMap	= DEFAULT_LOGIN_MAPID;								// 缺省的登录点地图
    CMapSt* pMap = MapList()->GetMap(idMap);
    if (!pMap)
    {
        return false;
    }
    // default attribute
    int nProfession = nData;
    CHECKF(nProfession == 10 || nProfession == 20 || nProfession == 30);
    int nForce	= 0;
    int nHealth	= 0;
    int nDex	= 0;
    int nSoul	= 0;
    if (nProfession == 20)	// 战
    {
        nForce	= 15;
        nHealth	= 10;
        nDex	= 1;
        nSoul	= 0;
    }
    else if (nProfession == 30)	// 异能者
    {
        nForce	= 15;
        nHealth	= 8;
        nDex	= 1;
        nSoul	= 3;
    }
    else //if (nProfession == 10)	// 魔
    {
        nForce	= 15;
        nHealth	= 5;
        nDex	= 2;
        nSoul	= 4;
    }
    //	CHECKF(nForce + nHealth + nDex + nSoul == NEWBIE_ATTR_POINT);
    int nMaxLife = nHealth * 10;		// nHealth*10;
    int nMaxMana = nSoul * 20;			// nSoul*20;
    int nDefaultMoney = 0;
    // default icon
    int nSex = (unLook / 1000) % 10;
    //	unLook = (unLook%10000)+10000;
    int nDefaultHair = 101;
    const int DEFAULT_ALLOT_POINT = 0;
	cLength = 1; //为1 2016.10.14
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT %s SET account_id=%u,name='%s',lookface=%d,hair=%d,length=%d,fat=%d,life=%d,mana=%d,money=%u,recordmap_id=%u, recordx=%u, recordy=%u,forceex=%d,dexterity=%d,health=%d,soul=%d,profession=%d,additional_point=%d",
            _TBL_USER,
            idAccount, szName, unLook, nDefaultHair, cLength, cFat,
            nMaxLife, nMaxMana, nDefaultMoney,
            idMap, DEFAULT_LOGIN_POSX, DEFAULT_LOGIN_POSY,
            nForce, nDex, nHealth, nSoul, nProfession, DEFAULT_ALLOT_POINT);
    return GameWorld()->GetDatabase()->ExecuteSQL(szSQL);		// 不检查重名和重帐号，由UNI_KEY保证
}

//////////////////////////////////////////////////////////////////////
void CUserList::ChangeProcessID(SOCKET_ID idSocket, PROCESS_ID idProcess)
{
    CHECK(idProcess >= MSGPORT_MAPGROUP_FIRST && idProcess < GameWorld()->GetMsgPort()->GetSize());
    CHECK(idSocket >= 0 && idSocket < m_setUser.size());
    m_setUser[idSocket]->idProcess = idProcess;
}

//////////////////////////////////////////////////////////////////////
void CUserList::LoginToMapGroup(CPlayer* pPlayer)
{
    CHECK(pPlayer);
    // 转到地图组中进行登录。
    IMessagePort* pPort = GameWorld()->GetMsgPort();
    ST_LOGIN	cLogin;
    cLogin.idSocket	= pPlayer->idSocket;
    cLogin.idUser	= pPlayer->GetID();
    PROCESS_ID	idMapGroupProcess = pPlayer->GetProcessID();
    pPort->Send(idMapGroupProcess, MAPGROUP_LOGIN, STRUCT_TYPE(ST_LOGIN), &cLogin);
    // 转换PROCESS_ID，标志玩家进入了MAPGROUP。(@帐号服务器是在MsgConnect中)
    pPlayer->idProcess	= idMapGroupProcess;	// 已设置，无意义		// readme.txt(2-9)
    if (!pPlayer->IsAgent())
    {
        CMsgTalk	msg;
        if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, REPLAY_OK_STR, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
        {
            pPlayer->SendMsg(&msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUserList::BroadcastMsg(CNetMsg* pMsg, CPlayer* pExcludeSender/*=NULL*/)
{
    CHECK(pMsg);
    for(int i = 0; i < m_setUser.size(); i++)
    {
        CPlayer* pPlayer = m_setUser[i];
        if (pPlayer && (!pExcludeSender || pExcludeSender->GetID() != pPlayer->GetID()))
        {
            pPlayer->SendMsg(pMsg);
        }
    }
    AGENT_SET::Iterator pAgent = m_setAgent->NewEnum();
    while(pAgent.Next())
    {
        if (pAgent && (!pExcludeSender || pExcludeSender->GetID() != pAgent->GetID()))
        {
            pAgent->SendMsg(pMsg);
            return;		// only one msg
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUserList::BroadcastMsg(LPCTSTR szMsg, CPlayer* pExcludeSender/*=NULL*/, LPCTSTR pszEmotion, DWORD dwColor, int nAttribute)
{
    CHECK(szMsg);
    CMsgTalk	msg;
    if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, (char*)szMsg, (char*)pszEmotion, dwColor, nAttribute))
    {
        BroadcastMsg(&msg, pExcludeSender);
    }
}

//////////////////////////////////////////////////////////////////////
void CUserList::DelAllAgent()
{
    AGENT_SET::Iterator pAgent	= m_setAgent->NewEnum();
    while(pAgent.Next())
    {
        m_setAgent->DelObj(pAgent);
    }
}
