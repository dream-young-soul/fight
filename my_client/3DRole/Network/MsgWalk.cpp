
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAction.cpp: implementation of the CMsgWalk class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "3DGameMap.h"
#include "GamePlayerSet.h"
#include "Player.h"
#include "Hero.h"
#include "GameMsg.h"
#include "Pet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWalk::CMsgWalk()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgWalk::~CMsgWalk()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWalk::Create(OBJID idPlayer, int nDir, int nScondDir /*= -1*/)
{
    // param check
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_WALK;
    m_pInfo->idUser		= idPlayer;
    m_pInfo->ucDir		= nDir;
    m_pInfo->ucRun		= nScondDir;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWalk::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_WALK != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgWalk::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgWalk, idUser:%u, dir:%u",
             m_pInfo->idUser,
             m_pInfo->ucDir);
#endif
    m_pInfo->ucDir %= 8;
    // get obj
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->idUser);
    if (!pPlayer)
    {
        if (m_pInfo->idUser == g_objHero.GetID())
        {
            //			CMyPos posHero	=g_objHero.GetAlignPos();
            //			g_objHero.SetAlignPos(posHero.x+_DELTA_X[m_pInfo->ucDir], posHero.y+_DELTA_Y[m_pInfo->ucDir]);
        }
        else
        {
            CMsgAction msg;
            if (msg.Create(g_objHero.GetID(), 0, 0, g_objHero.GetDir(), actionQueryPlayer, m_pInfo->idUser))
            {
                msg.Send();
            }
        }
        return;
    }
    if (pPlayer->IsMyPet())
    {
        return;
    }
    CCommand cmd;
    //	cmd.iType		=m_pInfo->ucRun ? _COMMAND_RUNFORWARD : _COMMAND_WALKFORWARD;
    if (m_pInfo->ucRun >= 20)
    {
        cmd.iType		= _COMMAND_RUNFORWARD;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nDir		= m_pInfo->ucDir;
        cmd.dwData		= m_pInfo->ucRun - 20;
        pPlayer->SetCommand(&cmd);
    }
    else
    {
        cmd.iType		= _COMMAND_WALKFORWARD;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nDir		= m_pInfo->ucDir;
        pPlayer->SetCommand(&cmd);
    }
    if (pPlayer && pPlayer->GetID() != g_objHero.GetID())
    {
        CMyPos posHeroAlign	= g_objHero.GetAlignPos();
        g_objPlayerSet.ProcessManager(posHeroAlign.x, posHeroAlign.y);
    }
}
