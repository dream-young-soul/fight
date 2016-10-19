
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgName.cpp: implementation of the CMsgName class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "GamePlayerSet.h"
#include "Hero.h"
#include "3DGameMap.h"
#include "SkyLayer.h"
#include "NDSound.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgName::CMsgName()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgName::~CMsgName()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(int nType, const char* pszName, DWORD dwData)
{
    // check param
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_NAME;
    m_pInfo->dwData	= dwData;
    m_pInfo->ucType	= (unsigned char)nType;
    if (pszName)
    {
        m_StrPacker.AddString(pszName);
    }
    m_pInfo->unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(int nType, const char* pszName, USHORT usPosX, USHORT usPosY)
{
    // check param
    if (!pszName || strlen(pszName) >= _MAX_NAMESIZE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgType	= _MSG_NAME;
    m_pInfo->usPosX	= usPosX;
    m_pInfo->usPosY	= usPosY;
    m_pInfo->ucType	= (unsigned char)nType;
    m_StrPacker.AddString(pszName);
    m_pInfo->unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_NAME != this->GetType())
    {
        return false;
    }
    return true;
}
//////////////////////////////////////////////////////////////////////
void CMsgName::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogSave("Process CMsgName, data:%u, str:%s",
              m_pInfo->dwData,
              m_pInfo->szName);
#endif
    // get obj
    CPlayer* pPlayer	= g_objPlayerSet.GetPlayer((OBJID)m_pInfo->dwData);
    if (!pPlayer)
    {
        if (g_objHero.GetID() == (OBJID)m_pInfo->dwData)
        {
            pPlayer	= &g_objHero;
        }
    }
    if (m_pInfo->ucType == NAMEACT_MEMBERLIST || m_pInfo->ucType == NAMEACT_MEMBERLIST_SPECIFYSYN)
    {
        int nAmount = m_StrPacker.GetStrAmount();
        if (nAmount == 0)
        {
            return;
        }
        g_objHero.ClearSynMember();
        for(int i = 0; i < nAmount; i ++)
        {
            char szTemp[256];
            m_StrPacker.GetString(i, szTemp, 256);
            g_objHero.SetSynListIndex(m_pInfo->dwData);
            g_objHero.AddSynMember(szTemp);
        }
        ::PostCmd(CMD_OPEN_FLASH_SYN_MEMBER);
        return;
    }
    char szName[_MAX_NAMESIZE] = "";
    if (m_StrPacker.GetStrAmount() > 0)
    {
        m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
    }
    // actions
    switch(m_pInfo->ucType)
    {
    case NAMEACT_ROLEEFFECT:
        {
            IF_OK (pPlayer)
            pPlayer->m_objEffect.Add(szName, false, 0, pPlayer->GetLook());
        }
        break;
    case NAMEACT_MAPEFFECT:
        {
            CMyPos posWorld = { 0, 0 };
            g_objGameMap.Cell2World(m_pInfo->usPosX, m_pInfo->usPosY, posWorld.x, posWorld.y);
            g_objGameMap.Add3DMapEffect(posWorld, szName);
        }
        break;
    case NAMEACT_WANTED:
        strcpy(g_objHero.m_szWanted, szName);
        break;
    case NAMEACT_POLICEWANTED:
        strcpy(g_objHero.m_szPoliceWanted, szName);
        break;
    case NAMEACT_FIREWORKS:
        {
            int nPosX	= _SCR_WIDTH / 2 - 140 ;
            int nPosY	= _SCR_HEIGHT / 2 - 150;
            int nStrlen	= strlen(szName);
            if (nStrlen % 2 == 1)
            {
                nStrlen--;
            }
            CSkyLayer* pSky = g_objGameMap.GetSkyLayer();
            for (int i = 0; i < nStrlen; i += 2)
            {
                char szChar[_MAX_NAMESIZE];
                memset(szChar, 0L, _MAX_NAMESIZE);
                strncpy(szChar, szName + i, 2);
                pSky->CreateFireWork_Char(nPosX + rand() % 280 , nPosY + rand() % 120, szChar, (i % 3 + 1), i*(300 +::RandGet(150)));
            }
        }
        break;
    case NAMEACT_CHANGE_SYN:
        {
            char szSubName[_MAX_NAMESIZE] = "";
            if (m_StrPacker.GetStrAmount() > 1)
            {
                m_StrPacker.GetString(1, szSubName, _MAX_NAMESIZE);
            }
            if (strlen(szSubName) > 6)
            {
                strcpy(szSubName, "...");
            }
            g_objPlayerSet.SetSyndicateName(m_pInfo->dwData, szName, szSubName);
            if (g_objHero.GetSydicateID() == m_pInfo->dwData)
            {
                ::PostCmd(CMD_FLASH_SYNDICATE);
            }
        }
        break;
    case NAMEACT_CHANGE_SYNTITLE:
        {
        }
        break;
    case NAMEACT_MATE:
        {
            if (pPlayer)
            {
                pPlayer->SetMate(szName);
                if (pPlayer->GetID() == g_objHero.GetID())
                {
                    ::PostCmd(CMD_LEVELUP);
                }
            }
        }
        break;
    case NAMEACT_QUERY_MATE:
        {
            g_objHero.m_objPlayerDummy.SetMate(szName);
            ::PostCmd(CMD_FLASH_OTHERPLAYER_EQUIPMENT);
        }
        break;
    case NAMEACT_DICE_BONUS:
        {
            int nAmount = m_StrPacker.GetStrAmount();
            for(int i = 0; i < nAmount; i ++)
            {
                char szTemp[256];
                m_StrPacker.GetString(i, szTemp, _MAX_NAMESIZE);
                g_objHero.m_objDiceManager.UpdatePlayer(szTemp, m_pInfo->dwData);
            }
            ::PostCmd(CMD_DICE_FLASH_PLAYERLIST);
        }
        break;
    case NAMEACT_DELDICE_PLAYER:
        {
            int nAmount = m_StrPacker.GetStrAmount();
            for(int i = 0; i < nAmount; i ++)
            {
                char szTemp[256];
                m_StrPacker.GetString(i, szTemp, _MAX_NAMESIZE);
                if (strcmp(g_objHero.GetName(), szTemp) == 0)
                {
                    // close
                    g_objHero.m_objDiceManager.Close(false);
                    ::PostCmd(CMD_DICE_QUIT);
                }
                // del
                g_objHero.m_objDiceManager.DelPlayer(szTemp);
                ::PostCmd(CMD_DICE_FLASH_PLAYERLIST);
            }
        }
        break;
    case NAMEACT_ADDDICE_PLAYER:
        {
            int nAmount = m_StrPacker.GetStrAmount();
            for(int i = 0; i < nAmount; i ++)
            {
                char szTemp[256];
                m_StrPacker.GetString(i, szTemp, _MAX_NAMESIZE);
                if (strcmp(g_objHero.GetName(), szTemp) == 0)
                {
                    // join
                    g_objHero.m_objDiceManager.Open(m_pInfo->dwData, false);
                    ::PostCmd(CMD_DICE_JOIN);
                }
                // add list
                g_objHero.m_objDiceManager.UpdatePlayer(szTemp);
                ::PostCmd(CMD_DICE_FLASH_PLAYERLIST);
            }
        }
        break;
    case NAMEACT_PLAYER_WAVE:
        {
            if (m_StrPacker.GetStrAmount() > 0)
            {
                char szTemp[256];
                m_StrPacker.GetString(0, szTemp, 256);
                CMyPos posCell, posWorld;
                posCell.x = m_pInfo->usPosX;
                posCell.y = m_pInfo->usPosY;
                if (posCell.x != 0 || posCell.y != 0)
                {
                    g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
                    ::DXPlaySound(szTemp, posWorld.x, posWorld.y, 2000);
                }
                else
                {
                    ::DXPlaySound(szTemp, 0, 0, 0);
                }
            }
        }
        break;
    default:
        break;
    }
}
