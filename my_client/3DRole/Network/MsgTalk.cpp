
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgTalk.cpp: implementation of the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "GamePlayerSet.h"
#include "Register.h"
#include "Entrance.h"
#include "Shop.h"
#include "Hero.h"
#include "GameMsg.h"
#include "GameDataSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgTalk::CMsgTalk()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgTalk::~CMsgTalk()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(char* pszSpeaker, char* pszHearer, char* pszWords, char* pszEmotion, DWORD dwColor, unsigned short unAttribute, unsigned short unStyle)
{
    if (!pszSpeaker || strlen(pszSpeaker) > _MAX_NAMESIZE)
    {
        return false;
    }
    if (!pszHearer || strlen(pszHearer) > _MAX_NAMESIZE)
    {
        return false;
    }
    if (!pszWords || strlen(pszWords) > _MAX_WORDSSIZE)
    {
        return false;
    }
    // can be null
    if (pszEmotion && strlen(pszEmotion) > _MAX_NAMESIZE)
    {
        return false;
    }
    // fill
    this->Init();
    // fill structure
    m_pInfo->dwWordsColor	= dwColor;
    m_pInfo->unTxtAttribute	= unAttribute;
    m_pInfo->unTxtStyle		= unStyle;
    m_pInfo->dwTime			=::SysTimeGet();
    BOOL bSucMake	= true;
    bSucMake	&= m_StrPacker.AddString(pszSpeaker);
    bSucMake	&= m_StrPacker.AddString(pszHearer);
    bSucMake	&= m_StrPacker.AddString(pszEmotion);
    bSucMake	&= m_StrPacker.AddString(pszWords);
    m_pInfo->unMsgType	= _MSG_TALK;
    m_pInfo->unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return bSucMake;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    if ((unsigned short)dwMsgSize != GetSize(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TALK != GetType(pbufMsg, dwMsgSize))
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTalk::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process MsgTalk, Sender:%s, Receiver:%s, Words:%s",
             szSender,
             szReceiver,
             szWords);
#endif
    char szSender[_MAX_NAMESIZE];
    char szReceiver[_MAX_NAMESIZE];
    char szEmotion[_MAX_NAMESIZE];
    char szWords[_MAX_WORDSSIZE];
    m_StrPacker.GetString(0, szSender, sizeof(szSender));
    m_StrPacker.GetString(1, szReceiver, sizeof(szReceiver));
    m_StrPacker.GetString(2, szEmotion, sizeof(szEmotion));
    m_StrPacker.GetString(3, szWords, sizeof(szWords));
    if (m_pInfo->unTxtAttribute == _TXTATR_NORMAL)
    {
        m_pInfo->unTxtAttribute = _TXTATR_TALK;
    }
    switch(m_pInfo->unTxtAttribute)
    {
    case _TXTATR_MESSAGEBOX:
        ::PostCmd(CMD_ACTION_MESSAGEBOX);
        g_objHero.SetActionMessage(szWords);
        break;
    case _TXTATR_REGISTER:
        g_objRegister.AddMsg(szWords);
        break;
    case _TXTATR_ENTRANCE:
        g_objEntrance.AddMsg(szWords);
        break;
    case _TXTATR_SHOP:
        //g_objShop.AddMsg(szWords);
        break;
    case _TXTATR_WEBPAGE:
        break;
    case _TXTATR_GHOST:
        if (g_objHero.TestStatus(USERSTATUS_GHOST) ||
                g_objHero.GetProfession() / 10 == 13 ||
                g_objHero.IsTeamMember(szSender))
        {
            if (!g_objGameMsg.IsBlackName(szSender) && !g_objGameMsg.IsBlackName(szReceiver)
                    && g_objGameMsg.GetChannelStatus(m_pInfo->unTxtAttribute))
            {
                BOOL bAdd = true;
                if (m_pInfo->unTxtAttribute == _TXTATR_PRIVATE && !g_objGameMsg.TestPriveteChannel(szSender))
                {
                    bAdd = false;
                }
                if (bAdd)
                {
                    g_objGameMsg.AddMsg(szSender,
                                        szReceiver,
                                        szEmotion,
                                        szWords,
                                        m_pInfo->dwWordsColor,
                                        m_pInfo->unTxtStyle,
                                        m_pInfo->unTxtAttribute,
                                        m_pInfo->dwTime);
                    {
                        CPlayer* pPlayer	= g_objPlayerSet.GetPlayer( szSender );
                        if (pPlayer)
                        {
                            pPlayer->Say(szWords);
                        }
                    }
                }
            }
        }
        break;
    case _TXTATR_SYNANNOUNCEL:	//登陆时同步
    case _TXTATR_SYNANNOUNCEG: //游戏中同步
        g_objHero.SetSyndicateAnnounce(szWords);
        break;
    case _TXTATR_LEAVEWORD:
        {
            g_objGameMsg.AddMsg(szSender,
                                szReceiver,
                                szEmotion,
                                szWords,
                                m_pInfo->dwWordsColor,
                                m_pInfo->unTxtStyle,
                                m_pInfo->unTxtAttribute,
                                m_pInfo->dwTime);
            CMsgAction msg;
            if (msg.Create(g_objHero.GetID(), 0, 0, 0, actionQueryLeaveWord))
            {
                msg.Send();
            }
            ::PostCmd(CMD_NEW_LEAVE_WORD);
        }
        break;
    case _TXT_SYNWAR_FIRST:
        {
            g_objHero.ClearSynWarReport();
            g_objHero.AddSynWarReport(szWords);
            ::PostCmd(CMD_FLASH_SYN_WAR_REPORT);
        }
        break;
    case _TXT_SYNWAR_NEXT:
        {
            g_objHero.AddSynWarReport(szWords);
            ::PostCmd(CMD_FLASH_SYN_WAR_REPORT);
        }
        break;
    case _TXTATR_CRYOUT:
        {
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(szSender);
            if (!pPlayer)
            {
                if (strcmp(g_objHero.GetName(), szSender) == 0)
                {
                    pPlayer = &g_objHero;
                }
            }
            if (pPlayer)
            {
                pPlayer->SetCryOut(szWords);
            }
        }
        break;
    case _TXTATR_REJECT:
        {
            if (strcmp(szWords, "a") == 0)
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100162));
            }
            if (strcmp(szWords, "b") == 0)
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100161));
            }
            if (strcmp(szWords, "c") == 0)
            {
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100160));
            }
            if (strcmp (szWords, "d") == 0)
            {
                g_objGameMsg.AddMsg (g_objGameDataSet.GetStr(100176));
            }
            if (strcmp (szWords, "e") == 0)
            {
                g_objGameMsg.AddMsg (g_objGameDataSet.GetStr(100177));
            }
        }
        break;
    case _TXTATR_MSG_TRADE:
    case _TXTATR_MSG_FRIEND:
    case _TXTATR_MSG_TEAM:
    case _TXTATR_MSG_SYN:
    case _TXTATR_MSG_OTHER:
        {
            g_objHero.m_objBulletin.SetNote(szWords);
        }
        break;
    default:
        if (!g_objGameMsg.IsBlackName(szSender) && !g_objGameMsg.IsBlackName(szReceiver)
                && g_objGameMsg.GetChannelStatus(m_pInfo->unTxtAttribute))
        {
            BOOL bAdd = true;
            if (m_pInfo->unTxtAttribute == _TXTATR_PRIVATE && !g_objGameMsg.TestPriveteChannel(szSender))
            {
                bAdd = false;
            }
            if (bAdd)
            {
                g_objGameMsg.AddMsg(szSender,
                                    szReceiver,
                                    szEmotion,
                                    szWords,
                                    m_pInfo->dwWordsColor,
                                    m_pInfo->unTxtStyle,
                                    m_pInfo->unTxtAttribute,
                                    m_pInfo->dwTime);
                {
                    CPlayer* pPlayer	= g_objPlayerSet.GetPlayer( szSender );
                    if (pPlayer)
                    {
                        pPlayer->Say(szWords);
                    }
                }
            }
        }
        break;
    }
}
