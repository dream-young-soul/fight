
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgEudemonInfo.cpp: implementation of the CMsgEudemonInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "Player.h"
#include "GamePlayerSet.h"
#include "Hero.h"
#include "3droledata.H"
#include "3dGameMap.h"
#include "GameMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgEudemonInfo::CMsgEudemonInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgEudemonInfo::~CMsgEudemonInfo()
{
}

BOOL CMsgEudemonInfo::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_EUDEMONINFO != this->GetType())
    {
        return false;
    }
    return true;
}


void CMsgEudemonInfo::Process(void* pInfo)
{
#ifdef _DEBUG
    ::LogMsg("Process CMsgEudemonInfo, id:%u, pos(%u,%u)", m_pInfo->id, m_pInfo->usPosX, m_pInfo->usPosY);
#endif
    if (m_pInfo->id == g_objHero.GetID())
    {
        //::LogMsg("Got a player with hero id! %u", m_pInfo->id);
        return;
    }
    if (m_pInfo->dwLookFace == 0)
    {
        return;
    }
    char szName[_MAX_NAMESIZE]		= "";
    m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
    if (m_pInfo->id == 0)
    {
        return;
    }
    CPet* pPet	= (CPet*)g_objPlayerSet.GetPlayer(m_pInfo->id);
    if (!pPet)
    {
        pPet	= new CPet;
        MYASSERT (pPet);
        pPet->SetID(m_pInfo->id);
        pPet->SetName(szName);
        pPet->SetRoleType(_ROLE_PLAYER);
        pPet->Create(m_pInfo->dwLookFace % 1000);
        MYASSERT (g_objPlayerSet.AddPlayer(pPet));
    }
    pPet->SetName(szName);
    const CMonsterInfo* pPetInfo = g_obj3DRoleData.GetMonsterInfo(m_pInfo->idMonsterType);
    if (pPetInfo)
    {
        pPet->SetMonsterTypeId(m_pInfo->idMonsterType);
        pPet->SetMaxLife(m_pInfo->usMaxLife);
        pPet->SetData(CPlayer::_PLAYER_LIFE, m_pInfo->usCurLife);
        pPet->SetAddSize(pPetInfo->nAddSize);
        pPet->SetZoomPercent(pPetInfo->nZoomPercent);
    }
    __int64 i64Status = m_pInfo->dwStatus[1];
    i64Status = i64Status << 32;
    i64Status += m_pInfo->dwStatus[0];
    pPet->ReplaceStatus(i64Status);
    pPet->SetDir(m_pInfo->ucDir);
    pPet->m_pIRoleView->SetVariable(_VAR_DIR, m_pInfo->ucDir);
    if (m_pInfo->ucPose == _ACTION_SITDOWN)
    {
        m_pInfo->ucPose = _ACTION_SITDOWN_STATIC;
    }
    pPet->SetPose(m_pInfo->ucPose);
    //-----------------------------------------------------------------
    // 消耗Cpu的操作
    if (pPet->TestStatus(USERSTATUS_GHOST))
    {
        pPet->SetLook(999);
    }
    else
    {
        pPet->SetLook(m_pInfo->dwLookFace % 1000);    // set default body here
    }
    pPet->Transform(m_pInfo->dwLookFace / 10000000);
    pPet->SetPetOwnerId(m_pInfo->idOwner);
    // set pose of player
    if (pPet->IsDead() && !pPet->TestStatus(USERSTATUS_GHOST))
    {
        CCommand cmd;
        cmd.iType		= _COMMAND_POSE;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nData		= _ACTION_BODY0;
        cmd.nDir		= m_pInfo->ucDir;
        pPet->SetCommand(&cmd);
    }
    // set pos
    int nPosX = m_pInfo->usPosX, nPosY = m_pInfo->usPosY;
    pPet->SetPos(nPosX, nPosY);
}
