
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgPlayerTask.cpp
// Create by: Huang Yuhang
// Create on: 2004/10/9 10:39
//--------------------------------------------------------------------------------------
#include "AllMsg.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgPlayerTask::CMsgPlayerTask()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgPlayerTask::~CMsgPlayerTask()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayerTask::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_PLAYERTASK != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void			CMsgPlayerTask::WantPk(const char* pszName, const char* pszTitle, const char* pszDesc,
                                       DWORD dwMoney, UCHAR ucRankReq, OBJID idItem0,
                                       OBJID idItem1, OBJID idItem2, OBJID idItem3,
                                       OBJID idItem4)
{
    if (!pszName || !pszTitle || !pszDesc)
    {
        return;
    }
    // init
    this->Init();
    m_StrPacker.AddString(pszTitle);
    m_StrPacker.AddString(pszDesc);
    m_StrPacker.AddString(pszName);
    m_pInfo->ucType  = _WANT_PK;
    m_pInfo->dwMoney = dwMoney;
    m_pInfo->ucRankReq = ucRankReq;
    m_pInfo->infoItem[0].idItem = idItem0;
    m_pInfo->infoItem[1].idItem = idItem1;
    m_pInfo->infoItem[2].idItem = idItem2;
    m_pInfo->infoItem[3].idItem = idItem3;
    m_pInfo->infoItem[4].idItem = idItem4;
    m_pInfo->unMsgType	= _MSG_PLAYERTASK;
    m_pInfo->unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    this->Send();
}
//////////////////////////////////////////////////////////////////////
void			CMsgPlayerTask::WantItem(const char* pszName,  const char* pszTitle, const char* pszDesc, UCHAR ucType,
        DWORD dwDurability, UCHAR ucAddition, UCHAR ucDegree,
        WORD dwMoney, UCHAR ucRankReq, OBJID idItem0,
        OBJID idItem1, OBJID idItem2, OBJID idItem3,
        OBJID idItem4)
{
    if (!pszName || !pszTitle || !pszDesc)
    {
        return;
    }
    // init
    this->Init();
    m_StrPacker.AddString(pszTitle);
    m_StrPacker.AddString(pszDesc);
    m_StrPacker.AddString(pszName);
    m_pInfo->ucType = ucType;
    m_pInfo->dwMoney = dwMoney;
    m_pInfo->ucRankReq = ucRankReq;
    m_pInfo->dwDurability = dwDurability;
    m_pInfo->ucAddition = ucAddition;
    m_pInfo->ucDegree = ucDegree;
    m_pInfo->infoItem[0].idItem = idItem0;
    m_pInfo->infoItem[1].idItem = idItem1;
    m_pInfo->infoItem[2].idItem = idItem2;
    m_pInfo->infoItem[3].idItem = idItem3;
    m_pInfo->infoItem[4].idItem = idItem4;
    m_pInfo->unMsgType	= _MSG_PLAYERTASK;
    m_pInfo->unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    this->Send();
}
//////////////////////////////////////////////////////////////////////
void CMsgPlayerTask::Process(void* pInfo)
{
    char szStr[512];
    m_StrPacker.GetString(0, szStr, 512);
    g_objHero.m_infoTask.strName = szStr;
    m_StrPacker.GetString(1, szStr, 512);
    g_objHero.m_infoTask.strDesc = szStr;
    m_StrPacker.GetString(2, szStr, 512);
    g_objHero.m_infoTask.strWant = szStr;
    g_objHero.m_infoTask.idTask = m_pInfo->idTask;
    g_objHero.m_infoTask.ucType = m_pInfo->ucType;
    g_objHero.m_infoTask.dwMoney = m_pInfo->dwMoney;
    g_objHero.m_infoTask.ucRankReq = m_pInfo->ucRankReq;
    if (g_objHero.m_infoTask.ucType == _WANT_ITEM)
    {
        g_objHero.m_infoTask.dwDurability = m_pInfo->dwDurability;
        g_objHero.m_infoTask.ucDegree = m_pInfo->ucDegree;
        g_objHero.m_infoTask.ucAddition = m_pInfo->ucAddition;
    }
    for(int i = 0; i < _MAX_PRIZE; i ++)
    {
        g_objHero.m_infoTask.infoItem[i].idItemType = m_pInfo->infoItem[i].idItemType;
        g_objHero.m_infoTask.infoItem[i].ucAddition = m_pInfo->infoItem[i].ucAddition;
        g_objHero.m_infoTask.infoItem[i].ucDegree = m_pInfo->infoItem[i].ucDegree;
        g_objHero.m_infoTask.infoItem[i].usDurability = m_pInfo->infoItem[i].usDurability;
    }
    ::PostCmd(CMD_FLASH_PLAYERTASKDETIAL);
}
