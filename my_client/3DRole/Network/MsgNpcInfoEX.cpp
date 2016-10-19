
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgNpcInfoEX.cpp: implementation of the CMsgNpcInfoEX class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgNpcInfoEX.h"
#include "GamePlayerset.h"
#include "Player.h"
#include "3DRoleData.h"
#include "3DGameMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNpcInfoEX::CMsgNpcInfoEX()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szName, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgNpcInfoEX::~CMsgNpcInfoEX()
{
}
//--------------------------------------------------------------------------------
BOOL CMsgNpcInfoEX::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_NPCINFOEX != this->GetType())
    {
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void CMsgNpcInfoEX::Process(void* pInfo)
{
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->id);
    if (pPlayer)
    {
        g_objPlayerSet.DelPlayer(m_pInfo->id);
        pPlayer = NULL;
    }
    if (!pPlayer)
    {
        switch(m_pInfo->usType)
        {
        case _ROLE_FURNITURE_NPC:
        case _ROLE_CITY_GATE_NPC:
        case _ROLE_CITY_WALL_NPC:
        case _ROLE_CITY_MOAT_NPC:
            {
                pPlayer	= new CPlayer;
                MYASSERT (pPlayer);
                pPlayer->SetID(m_pInfo->id);
                CMyPos posNpc;
                posNpc.x = (int)m_pInfo->usCellX;
                posNpc.y = (int)m_pInfo->usCellY;
                pPlayer->SetPos(posNpc);
                TerrainNpcTypeInfo* pInfo = g_obj3DRoleData.GetTerrainNpcInfo(m_pInfo->usLook / 10);
                pPlayer->Create(m_pInfo->usLook, _ROLE_VIEW_TERRAIN, m_pInfo->id, posNpc);
                MYASSERT(pInfo);
                if (m_pInfo->usType == _ROLE_CITY_GATE_NPC)
                {
                    pPlayer->SetAddSize(2);
                }
                pPlayer->SetDir(m_pInfo->usLook % 10);
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    pPlayer->SetName(pInfo->szNpcName);
                }
                pPlayer->SetRoleType(m_pInfo->usType);
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    if (m_StrPacker.GetStrAmount() > 0)
                    {
                        char szName[64];
                        m_StrPacker.GetString(0, szName, 16);
                        pPlayer->SetName(szName);
                    }
                }
                g_objPlayerSet.AddPlayer(pPlayer);
                pPlayer->SetMaxLife(m_pInfo->dwMaxLife);
                pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->dwLife);
            }
            break;
        case _ROLE_SYNFLAG_NPC:
            {
                pPlayer	= new CPlayer;
                MYASSERT (pPlayer);
                pPlayer->SetID(m_pInfo->id);
                pPlayer->SetName("");
                pPlayer->Create(m_pInfo->usLook, _ROLE_VIEW_SYNSTONE);
                pPlayer->SetRoleType(m_pInfo->usType);
                CMyPos posNpc;
                posNpc.x = (int)m_pInfo->usCellX;
                posNpc.y = (int)m_pInfo->usCellY;
                pPlayer->SetPos(posNpc);
                NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(m_pInfo->usLook / 10);
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    pPlayer->SetName(pInfo->szNpcName);
                }
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    if (m_StrPacker.GetStrAmount() > 0)
                    {
                        char szName[64];
                        m_StrPacker.GetString(0, szName, 16);
                        pPlayer->SetName(szName);
                    }
                }
                pPlayer->m_objEffect.Add(pInfo->szEffectIndex, false, 0, pPlayer->GetLook());
                g_objPlayerSet.AddPlayer(pPlayer);
                pPlayer->SetMaxLife(m_pInfo->dwMaxLife);
                pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->dwLife);
                //pPlayer->SetLife(m_pInfo->dwLife);
                pPlayer->SetAddSize(4);
            }
            break;
        case _ROLE_BOOTH_NPC:
            {
                pPlayer	= new CPlayer;
                MYASSERT (pPlayer);
                pPlayer->SetID(m_pInfo->id);
                pPlayer->SetName("");
                pPlayer->Create(m_pInfo->usLook, _ROLE_VIEW_BOOTH);
                pPlayer->SetRoleType(m_pInfo->usType);
                CMyPos posNpc;
                posNpc.x = (int)m_pInfo->usCellX;
                posNpc.y = (int)m_pInfo->usCellY;
                pPlayer->SetPos(posNpc);
                NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(m_pInfo->usLook / 10);
                MYASSERT(pInfo);
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    pPlayer->SetName(pInfo->szNpcName);
                }
                pPlayer->m_objEffect.Add(pInfo->szEffectIndex, false, 0, pPlayer->GetLook());
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    if (m_StrPacker.GetStrAmount() > 0)
                    {
                        char szName[64];
                        m_StrPacker.GetString(0, szName, 16);
                        pPlayer->SetName(szName);
                    }
                }
                g_objPlayerSet.AddPlayer(pPlayer);
            }
            break;
        default:
            {
                pPlayer	= new CPlayer;
                MYASSERT (pPlayer);
                pPlayer->SetID(m_pInfo->id);
                pPlayer->SetName("");
                pPlayer->Create(m_pInfo->usLook, _ROLE_VIEW_TASKNPC);
                pPlayer->SetRoleType(m_pInfo->usType);
                CMyPos posNpc;
                posNpc.x = (int)m_pInfo->usCellX;
                posNpc.y = (int)m_pInfo->usCellY;
                pPlayer->SetPos(posNpc);
                NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(m_pInfo->usLook / 10);
                MYASSERT(pInfo);
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    pPlayer->SetName(pInfo->szNpcName);
                }
                pPlayer->m_objEffect.Add(pInfo->szEffectIndex, false, 0, pPlayer->GetLook());
                if (_stricmp(pInfo->szNpcName, "NULL") != 0)
                {
                    if (m_StrPacker.GetStrAmount() > 0)
                    {
                        char szName[64];
                        m_StrPacker.GetString(0, szName, 16);
                        pPlayer->SetName(szName);
                    }
                }
                g_objPlayerSet.AddPlayer(pPlayer);
                pPlayer->SetMaxLife(m_pInfo->dwMaxLife);
                pPlayer->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->dwLife);
                //pPlayer->SetLife(m_pInfo->dwLife);
            }
            break;
        }
    }
}
//--------------------------------------------------------------------------------
