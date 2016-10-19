
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgPlayer.cpp: implementation of the CMsgPlayer class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "mapgroup.h"
//#include "UserData.h"
#include "User.h"
#include "AiNpc.h"
//#include "Booth.h"

const int	STATUARY_LIFE_SCALE					= 100;			//? ²¹¶¡£ºµñÏñÏÂ´«LIFEµÄ±ÈÀý
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgPlayer::CMsgPlayer()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgPlayer::~CMsgPlayer()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayer::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_PLAYER != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayer::Create(IRole* pRole)
{
    DEBUG_CREATEMSG("CMsgPlayer", pRole->GetID(), pRole->IsUser(), "", pRole->GetPosX(), pRole->GetPosY())
    // param check
    if (!pRole)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->id			= pRole->GetID();
    //m_pInfo->i64Status	= pRole->GetEffect();
    m_pInfo->dwStatus[0]	= pRole->GetEffect() & 0xFFFFFFFF;
    m_pInfo->dwStatus[1]	= (pRole->GetEffect() >> 32) & 0xFFFFFFFF;
    m_pInfo->dwLookFace	= pRole->GetLookFace();
    m_pInfo->usHair		= (USHORT)pRole->GetHair();
    m_pInfo->cLength	= pRole->GetLength();
    m_pInfo->cFat		= pRole->GetFat();
    m_pInfo->dwArmorType	= pRole->GetArmorTypeID();
    m_pInfo->dwWeaponRType	= pRole->GetWeaponRTypeID();
    m_pInfo->dwMountType	= pRole->GetMountTypeID();
    CMonster* pMonster = NULL;
    if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->IsEudemon() && pMonster->QueryOwnerUser())
    {
        m_pInfo->idOwner	= pMonster->QueryOwnerUser()->GetID();
    }
    else
    {
        m_pInfo->dwSynID_Rank	= (pRole->GetSynRankShow() << MASK_RANK_SHIFT) | (pRole->GetSynID()&MASK_SYNID);
    }
    CUser* pUser = NULL;
    if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        m_pInfo->dwMantleType		= pUser->GetMantleTypeID();
        m_pInfo->ucActionSpeed		= pUser->AdjustSpeed(pUser->GetSpeed());
        m_pInfo->ucTutorLevel		= pUser->GetTutorLevel();
        m_pInfo->ucMercenaryRank	= pUser->GetMercenaryRank();
        m_pInfo->ucNobilityRank		= pUser->GetNobilityRank();
    }
    else
    {
        m_pInfo->usMaxLife		= pRole->GetMaxLife();
        m_pInfo->usMonsterLife	= pRole->GetLife();
        if (pMonster)
        {
            m_pInfo->idMonsterType	= pMonster->GetType();
        }
    }
    m_pInfo->usPosX		= pRole->GetPosX();
    m_pInfo->usPosY		= pRole->GetPosY();
    m_pInfo->ucDir		= pRole->GetDir();
    m_pInfo->ucPose		= pRole->GetPose();
    if (IsSceneID(pRole->GetID()))
    {
        CNpc* pNpc;
        IF_OK(pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
        {
            if (pNpc->GetType() == _STATUARY_NPC)
            {
                m_pInfo->usStatuaryLife	 = pRole->GetLife() / STATUARY_LIFE_SCALE;
                m_pInfo->usStatuaryFrame = pNpc->GetInt(STATUARYDATA_FRAME);
            }
            else
            {
                m_pInfo->usLife	 = pRole->GetLife();
                m_pInfo->usLevel = pNpc->GetInt(NPCDATA_MAXLIFE);
            }
        }
    }
    else if (IsNpcID(pRole->GetID()))
    {
        m_pInfo->usLife	 = pRole->GetLife();
        m_pInfo->usLevel = pRole->GetLev();
    }
    /*	else
    	{
    		CUser*	pUser;
    		if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
    		{
    			m_pInfo->ucSprite	= pUser->GetSpriteFace();
    		}
    	}*/
    BOOL bSucMake	= true;
    bSucMake	&= m_StrPacker.AddString((char*)pRole->GetName());
    m_unMsgType	= _MSG_PLAYER;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return true;
}

/*//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayer::Create((CBooth* pBooth))
{
	// param check
	if (idPlayer == ID_NONE)
		return false;

	// init
	this->Init();

	// fill info now
	m_pInfo->id					= pBooth->GetID();
	m_pInfo->dwLookFace			= pBooty->GetLook();
	m_pInfo->dwEffect			= 0;
	m_pInfo->dwSynID_Rank		= 0;

	m_pInfo->dwHelmetType		= pInfo->ucFace;
	m_pInfo->dwArmorType		= pInfo->dwExp;
	m_pInfo->dwWeaponRType		= pInfo->nRepute;
	m_pInfo->dwWeaponLType		= pInfo->nRepute;
	m_pInfo->dwMountType		= pInfo->ucRank;

	m_pInfo->dwMedalSelect		= 0;

	m_pInfo->usPosX				= pBooth->GetPosX();
	m_pInfo->usPosY				= pBooth->GetPosY();
	m_pInfo->usHair				= 0;
	m_pInfo->ucDir				= pBooth->GetDir();
	m_pInfo->ucPose				= 0;

	if (!m_StrPacker.AddString(pBooth->GetOwnerName())
		return false;

	m_unMsgType	=_MSG_PLAYER;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}*/

//////////////////////////////////////////////////////////////////////
void CMsgPlayer::Process(void* pInfo)
{
    return;
    CUser* pUser = g_UserManager.GetUser(this);
    pUser->BroadcastRoomMsg(this, EXCLUDE_SELF);
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgPlayer, id:%u", m_pInfo->id);
#endif
    char szName[_MAX_NAMESIZE]		= "";
    char szMate[_MAX_NAMESIZE]		= "";
    char szSyndicate[_MAX_NAMESIZE]	= "";
    char szTitle[_MAX_NAMESIZE]		= "";
    m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
    m_StrPacker.GetString(1, szMate, _MAX_NAMESIZE);
    m_StrPacker.GetString(2, szSyndicate, _MAX_NAMESIZE);
    m_StrPacker.GetString(3, szTitle, _MAX_NAMESIZE);
}
