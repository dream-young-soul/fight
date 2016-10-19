
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// GamePlayerSet.cpp: implementation of the CGamePlayerSet class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)
#include "player.h"
#include "GamePlayerSet.h"
#include "3DGameMap.h"
#include "Hero.h"
#include "AllMsg.h"
#include "GameDataSet.h"
#include "BoothNpc.h"
// globle...
CGamePlayerSet	g_objPlayerSet;
const DWORD CREATE_3DROLE_INTERVAL = 300;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGamePlayerSet::CGamePlayerSet()
{
    this->Init();
    m_dwTimeFor3DRole = ::TimeGet();
    m_idWhiteSyn = m_idBlackSyn = ID_NONE;
}

CGamePlayerSet::~CGamePlayerSet()
{
    this->Destroy();
    this->DestroySyndicateInfo();
}

//////////////////////////////////////////////////////////////////////
void CGamePlayerSet::DestroySyndicateInfo()
{
    int nAmount = m_setSyndicateInfo.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        SAFE_DELETE(pInfo);
    }
    m_setSyndicateInfo.clear();
}
//////////////////////////////////////////////////////////////////////
void CGamePlayerSet::Init(void)
{
    m_dequePlayer.clear();
    m_pFocusPlayer	= NULL;
}

//////////////////////////////////////////////////////////////////////
void CGamePlayerSet::Destroy(void)
{
    this->DelAllPlayer();
    m_pFocusPlayer	= NULL;
}


//////////////////////////////////////////////////////////////////////
BOOL CGamePlayerSet::AddPlayer(CPlayer* pPlayer)
{
    if (!CPlayer::IsValidPt(pPlayer))
    {
        return false;
    }
    if (this->GetPlayer(pPlayer->GetID()))
    {
        return false;
    }
    switch(pPlayer->GetRoleType())
    {
    case _ROLE_BOOTH_NPC:
        {
            CMyPos posNpc;
            pPlayer->GetPos(posNpc);
            int nDir = pPlayer->GetDir() % 8;
            for(int i = 0; i < 5; i ++)
            {
                CMyPos posMask;
                posMask.x = posNpc.x + BoothMaskOffset[nDir][i][0];
                posMask.y = posNpc.y + BoothMaskOffset[nDir][i][1];
                CellInfo* pInfo = g_objGameMap.GetCell(posMask.x, posMask.y);
                if (pInfo)
                {
                    LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pInfo);
                    pLayerInfo->usMask = 1;
                }
            }
        }
        break;
        /*	case _ROLE_SHOPKEEPER_NPC:
        	case _ROLE_TASK_NPC:
        	case _ROLE_STORAGE_NPC:
        	case _ROLE_TRUNCK_NPC:
        	case _ROLE_FACE_NPC:
        	case _ROLE_FORGE_NPC:
        	case _ROLE_EMBED_NPC:
        	case _ROLE_STATUARY_NPC:
        	case _SYNTRANS_NPC:
        	case _ROLE_BOOTH_FLAG_NPC:
        	case _ROLE_WEAPONGOAL_NPC:
        	case _ROLE_MAGICGOAL_NPC:
        	case _ROLE_BOWGOAL_NPC:
        		{
        			CMyPos posNpc;
        			pPlayer->GetPos(posNpc);
        			CMyPos posMask;
        			posMask.x = posNpc.x;
        			posMask.y = posNpc.y;
        			CellInfo* pInfo = g_objGameMap.GetCell(posMask.x, posMask.y);
        			if (pInfo)
        			{
        				LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pInfo);
        				pLayerInfo->usMask = 1;
        			}
        		}
        		break;*/
    default:
        {
            if (pPlayer->IsNpc())
            {
                NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(pPlayer->GetLook() / 10);
                if (pInfo)
                {
                    const int _DELTA_X[8] = {0, -1, -1, -1, 0, 1, 1, 1};
                    const int _DELTA_Y[8] = {1, 1, 0, -1, -1, -1, 0, 1};
                    int nDirFront = (pPlayer->GetDir() + 8) % 8;
                    int nDirBack = (nDirFront + 4 + 8) % 8;
                    int nDirLeft = (nDirFront - 2 + 8) % 8;
                    int nDirRight = (nDirFront + 2 + 8) % 8;
                    CMyPos posOrg, posFront, posBack, posLeft, posRight;
                    pPlayer->GetPos(posOrg);
                    posFront.x = posOrg.x + _DELTA_X[nDirFront] * pInfo->nFront;
                    posFront.y = posOrg.y + _DELTA_Y[nDirFront] * pInfo->nFront;
                    posBack.x = posOrg.x + _DELTA_X[nDirBack] * pInfo->nBack;
                    posBack.y = posOrg.y + _DELTA_Y[nDirBack] * pInfo->nBack;
                    posLeft.x = posOrg.x + _DELTA_X[nDirLeft] * pInfo->nLeft;
                    posLeft.y = posOrg.y + _DELTA_Y[nDirLeft] * pInfo->nLeft;
                    posRight.x = posOrg.x + _DELTA_X[nDirRight] * pInfo->nRight;
                    posRight.y = posOrg.y + _DELTA_Y[nDirRight] * pInfo->nRight;
                    int nMinX = __min(__min(posFront.x, posBack.x), __min(posLeft.x, posRight.x));
                    int nMinY = __min(__min(posFront.y, posBack.y), __min(posLeft.y, posRight.y));
                    int nMaxX = __max(__max(posFront.x, posBack.x), __max(posLeft.x, posRight.x));
                    int nMaxY = __max(__max(posFront.y, posBack.y), __max(posLeft.y, posRight.y));
                    for(int i = nMinY; i <= nMaxY; i ++)
                    {
                        for(int j = nMinX; j <= nMaxX; j ++)
                        {
                            CellInfo* pInfo = g_objGameMap.GetCell(j, i);
                            if (pInfo)
                            {
                                LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pInfo);
                                if (pLayerInfo)
                                {
                                    pLayerInfo->usMask = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        break;
    }
    m_dequePlayer.push_back(pPlayer);
    g_objGameMap.AddInteractiveObj(pPlayer);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CGamePlayerSet::DelPlayer(OBJID id, BOOL bDelObj)
{
    int nPlayerAmount	= m_dequePlayer.size();
    CPlayer* pMyPlayer = this->GetPlayer(id);
    if (pMyPlayer && pMyPlayer->IsMyPet())
    {
        g_objHero.DelPet(id);
    }
    for (int i = nPlayerAmount - 1; i >= 0; i--)
    {
        CPlayer* pPlayer = m_dequePlayer[i];
        if (id == pPlayer->GetID())
        {
            if (m_pFocusPlayer == pPlayer)
            {
                m_pFocusPlayer = NULL;
            }
            if (pPlayer->GetRoleType() == _ROLE_BOOTH_NPC)
            {
                // 1*1 ...
                CMyPos posNpc;
                pPlayer->GetPos(posNpc);
                for(int i = 0; i < 5; i ++)
                {
                    CMyPos posMask;
                    int nDir = pPlayer->GetDir() % 8;
                    posMask.x = posNpc.x + BoothMaskOffset[nDir][i][0];
                    posMask.y = posNpc.y + BoothMaskOffset[nDir][i][1];
                    CellInfo* pInfo = g_objGameMap.GetCell(posMask.x, posMask.y);
                    if (pInfo)
                    {
                        LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pInfo);
                        pLayerInfo->usMask = 0;
                    }
                }
                // reset cry out
                {
                    CPlayer* pOwner = this->GetPlayer(pPlayer->GetName());
                    if (pOwner)
                    {
                        pOwner->SetCryOut("");
                    }
                    else
                    {
                        if (0 == strcmp(pPlayer->GetName(), g_objHero.GetName()))
                        {
                            g_objHero.SetCryOut("");
                        }
                    }
                }
            }
            else
            {
                if (pPlayer->IsNpc())
                {
                    NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(pPlayer->GetLook() / 10);
                    if (pInfo)
                    {
                        const int _DELTA_X[8] = {0, -1, -1, -1, 0, 1, 1, 1};
                        const int _DELTA_Y[8] = {1, 1, 0, -1, -1, -1, 0, 1};
                        int nDirFront = (pPlayer->GetDir() + 8) % 8;
                        int nDirBack = (nDirFront + 4 + 8) % 8;
                        int nDirLeft = (nDirFront - 2 + 8) % 8;
                        int nDirRight = (nDirFront + 2 + 8) % 8;
                        CMyPos posOrg, posFront, posBack, posLeft, posRight;
                        pPlayer->GetPos(posOrg);
                        posFront.x = posOrg.x + _DELTA_X[nDirFront] * pInfo->nFront;
                        posFront.y = posOrg.y + _DELTA_Y[nDirFront] * pInfo->nFront;
                        posBack.x = posOrg.x + _DELTA_X[nDirBack] * pInfo->nBack;
                        posBack.y = posOrg.y + _DELTA_Y[nDirBack] * pInfo->nBack;
                        posLeft.x = posOrg.x + _DELTA_X[nDirLeft] * pInfo->nLeft;
                        posLeft.y = posOrg.y + _DELTA_Y[nDirLeft] * pInfo->nLeft;
                        posRight.x = posOrg.x + _DELTA_X[nDirRight] * pInfo->nRight;
                        posRight.y = posOrg.y + _DELTA_Y[nDirRight] * pInfo->nRight;
                        int nMinX = __min(__min(posFront.x, posBack.x), __min(posLeft.x, posRight.x));
                        int nMinY = __min(__min(posFront.y, posBack.y), __min(posLeft.y, posRight.y));
                        int nMaxX = __max(__max(posFront.x, posBack.x), __max(posLeft.x, posRight.x));
                        int nMaxY = __max(__max(posFront.y, posBack.y), __max(posLeft.y, posRight.y));
                        for(int i = nMinY; i <= nMaxY; i ++)
                        {
                            for(int j = nMinX; j <= nMaxX; j ++)
                            {
                                CellInfo* pInfo = g_objGameMap.GetCell(j, i);
                                if (pInfo)
                                {
                                    LayerInfo* pLayerInfo = g_objGameMap.GetLastLayerInfo(pInfo);
                                    if (pLayerInfo)
                                    {
                                        pLayerInfo->usMask = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (bDelObj)
            {
                g_objGameMap.DelInteractiveObj(pPlayer);
                SAFE_DELETE (pPlayer);
            }
            m_dequePlayer.erase(m_dequePlayer.begin() + i);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
/*
BOOL CGamePlayerSet::DelPlayer	(const char* pszName, BOOL bDelObj)
{
	CPlayer* pPlayer	=this->GetPlayer(pszName);
	if (!pPlayer)
		return false;

	return this->DelPlayer(pPlayer->GetID(), bDelObj);
}
*/

//////////////////////////////////////////////////////////////////////
void CGamePlayerSet::DelAllPlayer(BOOL bExcludeMyPet)
{
    if (bExcludeMyPet)
    {
        int nPlayerAmount	= m_dequePlayer.size();
        for (int i = nPlayerAmount - 1; i >= 0; i--)
        {
            CPlayer* pPlayer = m_dequePlayer[i];
            if (!pPlayer->IsMyPet())
            {
                if (CPlayer::IsValidPt(pPlayer))
                {
                    g_objGameMap.DelInteractiveObj(pPlayer);
                    SAFE_DELETE (pPlayer);
                    m_dequePlayer.erase(m_dequePlayer.begin() + i);
                }
                else
                {
                    g_objGameMap.DelInteractiveObj(pPlayer);
                    m_dequePlayer.erase(m_dequePlayer.begin() + i);
                }
            }
        }
    }
    else
    {
        int nPlayerAmount	= m_dequePlayer.size();
        for (int i = nPlayerAmount - 1; i >= 0; i--)
        {
            CPlayer* pPlayer = m_dequePlayer[i];
            if (CPlayer::IsValidPt(pPlayer))
            {
                g_objGameMap.DelInteractiveObj(pPlayer);
                SAFE_DELETE (pPlayer);
            }
            else
            {
                g_objGameMap.DelInteractiveObj(pPlayer);
                m_dequePlayer.erase(m_dequePlayer.begin() + i);
            }
        }
        m_dequePlayer.clear();
    }
    m_pFocusPlayer = NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CGamePlayerSet::GetPlayerByIndex(DWORD dwIndex) const
{
    if (dwIndex >= m_dequePlayer.size())
    {
        return NULL;
    }
    return m_dequePlayer[dwIndex];
}

//////////////////////////////////////////////////////////////////////
CPlayer* CGamePlayerSet::GetPlayer	(const char* pszName) const
{
    if (!pszName)
    {
        return NULL;
    }
    int nPlayerAmount	= m_dequePlayer.size();
    for (int i = nPlayerAmount - 1; i >= 0; i--)
    {
        CPlayer* pPlayer = m_dequePlayer[i];
        if (!pPlayer)
        {
            continue;
        }
        if (pPlayer->IsPlayer() &&
                0 == strcmp(pPlayer->GetName(), pszName))
        {
            return pPlayer;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CGamePlayerSet::GetPlayer	(OBJID id, BOOL bIncludeSelf) const
{
    if (bIncludeSelf && g_objHero.GetID() == id)
    {
        return &g_objHero;
    }
    int nPlayerAmount	= m_dequePlayer.size();
    for (int i = nPlayerAmount - 1; i >= 0; i--)
    {
        CPlayer* pPlayer	= m_dequePlayer[i];
        if (!pPlayer)
        {
            continue;
        }
        if (id == pPlayer->GetID())
        {
            return pPlayer;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CGamePlayerSet::GetPlayer	(int nMapX, int nMapY, int nRoleType) const
{
    int nPlayerAmount	= m_dequePlayer.size();
    for (int i = nPlayerAmount - 1; i >= 0; i--)
    {
        CPlayer* pPlayer	= m_dequePlayer[i];
        if (!pPlayer)
        {
            continue;
        }
        CMyPos posPlayer;
        pPlayer->GetPos(posPlayer);
        if (nMapX == posPlayer.x && nMapY == posPlayer.y)
        {
            if (nRoleType == _ROLE_NPC_NONE)
            {
                return pPlayer;
            }
            else if (pPlayer->GetRoleType() == nRoleType)
            {
                return pPlayer;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CGamePlayerSet::GetFocusPlayer(DWORD dwFlag/*=PLAYER_NORMAL*/)
{
    if (PLAYER_NORMAL == dwFlag)
    {
        if (CPlayer::IsValidPt(m_pFocusPlayer))
        {
            return m_pFocusPlayer;
        }
        else
        {
            m_pFocusPlayer = NULL;
            return NULL;
        }
    }
    CPlayer* pFocusPlayer = NULL;
    if ((dwFlag & PLAYER_INCLUDESELF) && g_objHero.IsMouseFocus())
    {
        pFocusPlayer = &g_objHero;
    }
    int iPlayerAmount = this->GetPlayerAmount();
    for(int i = iPlayerAmount - 1; i >= 0; i--)
    {
        CPlayer* pPlayer = m_dequePlayer[i];
        if (!CPlayer::IsValidPt(pPlayer))
        {
            continue;
        }
        if (pPlayer->GetID() == MOUSE_PLAYER_ID)
        {
            continue;
        }
        /*
        		if (pPlayer->IsDead() && !(((dwFlag&PLAYER_INCLUDEDEAD) == 0)||
        							((dwFlag&PLAYER_INCLUDEDEADMONSTER)==0) ||
        							((dwFlag&PLAYER_DEADONLY)==0)))
        			continue;
        */
        if (pPlayer->IsDead())
        {
            BOOL bCanDead = false;
            if (dwFlag & PLAYER_INCLUDEDEAD)
            {
                bCanDead = true;
            }
            if (dwFlag & PLAYER_INCLUDEDEADMONSTER)
            {
                bCanDead = true;
            }
            if (!bCanDead)
            {
                continue;
            }
        }
        if (dwFlag & PLAYER_DEADONLY)
            if (!pPlayer->IsDead())
            {
                continue;
            }
        if (pPlayer->IsMouseFocus())
        {
            if (pPlayer->IsHypersensitive())
            {
                m_pFocusPlayer = pPlayer;
                return pPlayer;
            }
            if (pFocusPlayer)	// got one already
            {
                CMyPos posFocusPlayer;
                pFocusPlayer->GetPos(posFocusPlayer);
                CMyPos posPlayer;
                pPlayer->GetPos(posPlayer);
                if ((posPlayer.x + posPlayer.y >= posFocusPlayer.x + posFocusPlayer.y) ||
                        (g_objHero.GetPkMode() == PKMODE_ARRESTMENT && !pFocusPlayer->IsBaddy() && pPlayer->IsBaddy()))
                {
                    if (dwFlag & PLAYER_MONSTERPRI)
                    {
                        if (pFocusPlayer->IsNpc())
                        {
                            if (pPlayer->IsNpc())
                            {
                                pFocusPlayer = pPlayer;
                            }
                        }
                        else
                        {
                            if (pPlayer->IsNpc())
                            {
                                pFocusPlayer = pPlayer;
                            }
                            else
                            {
                                if (g_objHero.GetFriend(pFocusPlayer->GetID())
                                        || g_objHero.GetTeamMemberInfoByID(pFocusPlayer->GetID())
                                        || g_objHero.GetSydicateID() == pFocusPlayer->GetSydicateID())
                                {
                                    pFocusPlayer = pPlayer;
                                }
                                else
                                {
                                    if (g_objHero.GetFriend(pFocusPlayer->GetID())
                                            || g_objHero.GetTeamMemberInfoByID(pFocusPlayer->GetID())
                                            || g_objHero.GetSydicateID() == pFocusPlayer->GetSydicateID())
                                    {
                                    }
                                    else
                                    {
                                        pFocusPlayer = pPlayer;
                                    }
                                }
                            }
                        }
                    }
                    else if (dwFlag & PLAYER_PLAYERPRI)
                    {
                        if (!pFocusPlayer->IsNpc())
                        {
                            if (!pPlayer->IsNpc())
                            {
                                pFocusPlayer = pPlayer;
                            }
                        }
                        else
                        {
                            pFocusPlayer = pPlayer;
                        }
                    }
                    else
                    {
                        pFocusPlayer = pPlayer;
                    }
                }
            }
            else
            {
                pFocusPlayer = pPlayer;
            }
        }
    }
    return pFocusPlayer;
}

//////////////////////////////////////////////////////////////////////
void CGamePlayerSet::ShowX(void)
{
    if (CPlayer::IsValidPt(m_pFocusPlayer))
    {
        m_pFocusPlayer->ShowInfo();
    }
}

//////////////////////////////////////////////////////////////////////
void CGamePlayerSet::ProcessManager(int nHeroX, int nHeroY)
{
    return;
    int iPlayerAmount = this->GetPlayerAmount();
    for(int i = iPlayerAmount - 1; i >= 0; i--)
    {
        CPlayer* pPlayer	= m_dequePlayer[i];
        if (CPlayer::IsValidPt(pPlayer) && _ROLE_CALL_PET != pPlayer->GetRoleType())
        {
            if (nHeroX > 0 && nHeroY > 0)
            {
                // del player by distance from hero
                int nPosX = 0, nPosY = 0;
                CMyPos posHero, posPlayer;
                posHero.x = nHeroX;
                posHero.y = nHeroY;
                pPlayer->GetPos(posPlayer);
                //				int nDelta = __max(abs(posHero.x - posPlayer.x), abs(posHero.y - posPlayer.y));
                //				::DebugMsg("player des %s %d", pPlayer->GetName(), nDelta);
                if (g_objGameMap.Outof9Block(posHero, posPlayer))
                {
                    if (m_pFocusPlayer == pPlayer)
                    {
                        m_pFocusPlayer = NULL;
                    }
                    g_objGameMap.DelInteractiveObj(pPlayer);
                    //					::DebugMsg("delete player %s", pPlayer->GetName());
                    m_dequePlayer.erase(m_dequePlayer.begin() + i);
                    SAFE_DELETE(pPlayer);
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////

void CGamePlayerSet::Process(int nHeroX, int nHeroY)
{
    m_pFocusPlayer	= NULL;	// reset
    DWORD dwDistance = 999;
    int iPlayerAmount = this->GetPlayerAmount();
    for(int i = iPlayerAmount - 1; i >= 0; i--)
    {
        CPlayer* pPlayer	= m_dequePlayer[i];
        if (CPlayer::IsValidPt(pPlayer))
        {
            if (!pPlayer->IsDead() && pPlayer->IsMouseFocus())
            {
                if (pPlayer->IsHypersensitive())
                {
                    m_pFocusPlayer = pPlayer;
                    break;
                }
                if (m_pFocusPlayer)	// got one already
                {
                    CMyPos posFocusPlayer;
                    m_pFocusPlayer->GetPos(posFocusPlayer);
                    CMyPos posPlayer;
                    pPlayer->GetPos(posPlayer);
                    if (posPlayer.x + posPlayer.y >= posFocusPlayer.x + posFocusPlayer.y)
                    {
                        m_pFocusPlayer = pPlayer;
                    }
                }
                else
                {
                    m_pFocusPlayer = pPlayer;
                }
            }
        }
        else
        {
            if (m_pFocusPlayer == pPlayer)
            {
                m_pFocusPlayer = NULL;
            }
            g_objGameMap.DelInteractiveObj(pPlayer);
            m_dequePlayer.erase(m_dequePlayer.begin() + i);
        }
    }
    m_objFaceEffectWhite.Process();
    m_objFaceEffectBlack.Process();
}
//--------------------------------------------------------------
BOOL	CGamePlayerSet::AddSyndicate(OBJID idSyndaicate, OBJID idTopSyndaicate, char* pszName, char* pszLeaderName, DWORD dwStatus)
{
    if (idSyndaicate == ID_NONE)
    {
        return FALSE;
    }
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndaicate)
        {
            return FALSE;
        }
    }
    const OBJID idStrNone = 100021;
    InfoSyndicate* pInfoSyndicate = new InfoSyndicate;
    pInfoSyndicate->idSyndicate = idSyndaicate;
    pInfoSyndicate->idTopSyndicate = idTopSyndaicate;
    strcpy(pInfoSyndicate->szSyndicateName, pszName);
    strcpy(pInfoSyndicate->szSyndicateLeaderName, pszLeaderName);
    pInfoSyndicate->dwStatus = dwStatus;
    m_setSyndicateInfo.push_back(pInfoSyndicate);
    return TRUE;
}

char*	CGamePlayerSet::GetSyndicateLeaderName(OBJID idSyndaicate)
{
    // seek ...
    const OBJID idStrNone = 100021;
    InfoSyndicate infoNoSyndicate = {ID_NONE, ID_NONE, "NONE", "NONE", SYNDICATE_NORMAL};
    strcpy(infoNoSyndicate.szSyndicateLeaderName, g_objGameDataSet.GetStr(idStrNone));
    if (idSyndaicate == ID_NONE)
    {
        return infoNoSyndicate.szSyndicateLeaderName;
    }
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndaicate)
        {
            return pInfo->szSyndicateLeaderName;
        }
    }
    return "";
}
//--------------------------------------------------------------
char* CGamePlayerSet::GetSyndicateName(OBJID idSyndaicate)
{
    // seek ...
    const OBJID idStrNone = 100021;
    InfoSyndicate infoNoSyndicate = {ID_NONE, ID_NONE, "NONE", "NONE", SYNDICATE_NORMAL};
    strcpy(infoNoSyndicate.szSyndicateName, g_objGameDataSet.GetStr(idStrNone));
    if (idSyndaicate == ID_NONE)
    {
        return infoNoSyndicate.szSyndicateName;
    }
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndaicate)
        {
            return pInfo->szSyndicateName;
        }
    }
    // query ...
    CMsgSyndicate msg;
    if (msg.Create(QUERY_SYN_NAME, idSyndaicate))
    {
        msg.Send();
    }
    // insert a virtual record...
    InfoSyndicate* pInfo = new InfoSyndicate;
    MYASSERT(pInfo);
    pInfo->idSyndicate = idSyndaicate;
    pInfo->idTopSyndicate = idSyndaicate;
    strcpy(pInfo->szSyndicateName, "");
    strcpy(pInfo->szSyndicateLeaderName, "");
    pInfo->dwStatus	= SYNDICATE_NORMAL;
    m_setSyndicateInfo.push_back(pInfo);
    return pInfo->szSyndicateName;
}
//--------------------------------------------------------------
void CGamePlayerSet::SetSyndicateName(OBJID idSyndaicate, char* pszName, char* pszLeaderName)
{
    if (!pszName || !pszLeaderName)
    {
        return;
    }
    // seek ...
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndaicate)
        {
            strcpy(pInfo->szSyndicateName, pszName);
            strcpy(pInfo->szSyndicateLeaderName, pszLeaderName);
            return;
        }
    }
    // server error ...
    // skip ...
    return;
}
//--------------------------------------------------------------
void  CGamePlayerSet::SetSyndicateStatus(OBJID idSyndaicate, DWORD dwStatus)
{
    CGamePlayerSet::GetSyndicateName(idSyndaicate);
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndaicate)
        {
            pInfo->dwStatus = dwStatus;
            ::PostCmd(CMD_FLASH_SYNALLY);
            return;
        }
    }
}
//--------------------------------------------------------------
void CGamePlayerSet::ClearAllSyndicateStatus()
{
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo)
        {
            pInfo->dwStatus = SYNDICATE_NORMAL;
        }
    }
}
//--------------------------------------------------------------
OBJID   CGamePlayerSet::GetSyndicateIdByName(char* pszSynName)
{
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && strcmp(pInfo->szSyndicateName, pszSynName) == 0)
        {
            return pInfo->idSyndicate;
        }
    }
    return ID_NONE;
}
//--------------------------------------------------------------
int		CGamePlayerSet::GetSubSyndicateAmount(OBJID idSyndicate)
{
    int nAmount = m_setSyndicateInfo.size();
    int nSubAmount = 0;
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idTopSyndicate == idSyndicate
                && (pInfo->dwStatus == SYNDICATE_SUB || pInfo->dwStatus == SYNDICATE_SUBSUB) )
        {
            nSubAmount++;
        }
    }
    return nSubAmount;
}
//--------------------------------------------------------------
DWORD	CGamePlayerSet::GetSubSyndicateId(OBJID idSyndicate, int nIndex)
{
    int nAmount = m_setSyndicateInfo.size();
    int nSubAmount = 0;
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idTopSyndicate == idSyndicate
                && (pInfo->dwStatus == SYNDICATE_SUB || pInfo->dwStatus == SYNDICATE_SUBSUB) )
        {
            if (nSubAmount == nIndex)
            {
                return pInfo->idSyndicate;
            }
            nSubAmount++;
        }
    }
    return ID_NONE;
}
//--------------------------------------------------------------
DWORD	CGamePlayerSet::GetSyndicateStatus(OBJID idSyndaicate)
{
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndaicate)
        {
            return pInfo->dwStatus;
        }
    }
    return SYNDICATE_NORMAL;
}
//--------------------------------------------------------------
DWORD	CGamePlayerSet::GetAllySyndicateAmount()
{
    int nAmount = m_setSyndicateInfo.size();
    int nAllySyndicateAmount = 0;
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo->dwStatus == SYNDICATE_ALLY)
        {
            nAllySyndicateAmount ++;
        }
    }
    return nAllySyndicateAmount;
}
//--------------------------------------------------------------
char*	CGamePlayerSet::GetAllySyndicateNameByIndex(int nIndex)
{
    int nAmount = m_setSyndicateInfo.size();
    int nAllySyndicateAmount = 0;
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo->dwStatus == SYNDICATE_ALLY)
        {
            if (nAllySyndicateAmount == nIndex)
            {
                return pInfo->szSyndicateName;
            }
            nAllySyndicateAmount ++;
        }
    }
    return NULL;
}
//--------------------------------------------------------------
DWORD	CGamePlayerSet::GetAntagonizeSyndicateAmount()
{
    int nAmount = m_setSyndicateInfo.size();
    int nAntagonizeSyndicateAmount = 0;
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo->dwStatus == SYNDICATE_ANTAGONIZE)
        {
            nAntagonizeSyndicateAmount ++;
        }
    }
    return nAntagonizeSyndicateAmount;
}
//--------------------------------------------------------------
char*	CGamePlayerSet::GetAntagonizeNameByIndex(int nIndex)
{
    int nAmount = m_setSyndicateInfo.size();
    int nAntagonizeSyndicateAmount = 0;
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo->dwStatus == SYNDICATE_ANTAGONIZE)
        {
            if (nAntagonizeSyndicateAmount == nIndex)
            {
                return pInfo->szSyndicateName;
            }
            nAntagonizeSyndicateAmount ++;
        }
    }
    return NULL;
}
//--------------------------------------------------------------
void	CGamePlayerSet::UpdateSyndicateName(OBJID idSyndaicate)
{
    int nAmount = m_setSyndicateInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        InfoSyndicate* pInfo = m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndaicate)
        {
            SAFE_DELETE(pInfo);
        }
        m_setSyndicateInfo.erase(m_setSyndicateInfo.begin() + i);
        return;
    }
}
//--------------------------------------------------------------
void	CGamePlayerSet::SetWhiteSynId(OBJID idWhiteSyn)
{
    m_idWhiteSyn = idWhiteSyn;
    m_objFaceEffectWhite.Clear();
    m_objFaceEffectWhite.Add("White_Frame");
}
//--------------------------------------------------------------
OBJID	CGamePlayerSet::GetWhiteSynId()
{
    return m_idWhiteSyn;
}
//--------------------------------------------------------------
void	CGamePlayerSet::SetBlackSynId(OBJID idBlackSyn)
{
    m_idBlackSyn = idBlackSyn;
    m_objFaceEffectBlack.Clear();
    m_objFaceEffectBlack.Add("Black_Frame");
}
//--------------------------------------------------------------
OBJID	CGamePlayerSet::GetBlackSynId()
{
    return m_idBlackSyn;
}
//--------------------------------------------------------------
void	CGamePlayerSet::DestroySyndicate(OBJID idSyndicate)
{
    // process hero ...
    if (g_objHero.GetSydicateID() == idSyndicate)
    {
        g_objHero.SetSyndicateAnnounce("");
        g_objHero.SetSyndicateFund(0);
        g_objHero.SetSyndicateLeaderName("");
        g_objHero.SetSyndicatePopulation(0);
        g_objHero.SetSyndicateProffer(0);
        g_objHero.SetSyndicateRank(RANK_NONE);
        g_objHero.SetSyndicateID(ID_NONE);
    }
    // process playerset ...
    int nAmount = this->GetPlayerAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        CPlayer* pPlayer = this->GetPlayerByIndex(i);
        if (pPlayer && pPlayer->GetSydicateID() == idSyndicate)
        {
            pPlayer->SetSyndicateID(ID_NONE);
            pPlayer->SetSyndicateFund(0);
            pPlayer->SetSyndicateLeaderName("");
            pPlayer->SetSyndicatePopulation(0);
            pPlayer->SetSyndicateProffer(0);
            pPlayer->SetSyndicateRank(ID_NONE);
        }
    }
    nAmount  = m_setSyndicateInfo.size();
    for(int i = nAmount - 1 ; i >= 0; i--)
    {
        InfoSyndicate* pInfo =  m_setSyndicateInfo[i];
        if (pInfo && pInfo->idSyndicate == idSyndicate)
        {
            SAFE_DELETE(pInfo);
            m_setSyndicateInfo.erase(m_setSyndicateInfo.begin() + i);
        }
    }
    ::PostCmd(CMD_FLASH_SYNALLY);
}

//--------------------------------------------------------------
void	CGamePlayerSet::SetMantle(OBJID idSyndicate, DWORD dwMantle)
{
    int nAmount  = this->GetPlayerAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        CPlayer* pPlayer = this->GetPlayerByIndex(i);
        if (pPlayer && pPlayer->GetSydicateID() == idSyndicate)
        {
            pPlayer->SetMantle(dwMantle);
        }
    }
    if (g_objHero.GetSydicateID() == idSyndicate)
    {
        g_objHero.SetMantle(dwMantle);
    }
}
