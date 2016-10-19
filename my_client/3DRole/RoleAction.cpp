#pragma warning(disable:4786)
#include "basefunc.H"
#include "3dgamemap.H"
#include "role.H"
#include "gameplayerset.H"
#include "hero.H"
#include "GameMsg.h"
#include "../NDSound/NDsound.h"

#define _USE_ASTAR

// const...
const DWORD _TIME_FAINT	=150;

//--------------------------------------------------------------
void CRole::StaticAction(int nAction, int nDir /*= -1*/)
{
	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;

	if (nDir != -1)
		m_Info.iDirection = nDir;

	m_Info.iActType = nAction;

	// attack?
	if (m_Info.iActType >= _ACTION_ATTACK0 && m_Info.iActType <= _ACTION_ATTACK2)
		m_tmLastAtk = ::TimeGet();
}

//--------------------------------------------------------------
BOOL CRole::StandBy(void)
{
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;

//	if (_COMMAND_STANDBY == this->GetCommandType() &&
//			::RandGet(10) == 1)
//	{
//		m_Info.iActType	= _ACTION_REST1;
///*		switch(::RandGet(3))
//		{
//			m_Info.iActType	= _ACTION_REST1;
//		case 0:
//			m_Info.iActType	= _ACTION_REST1;
//			break;
//		case 1:
//			m_Info.iActType	= _ACTION_REST2;
//			break;
//		default:
//			m_Info.iActType	= _ACTION_REST3;
//			break;
//		}*/
//	}
//	else

	m_Info.iActType	=_ACTION_STANDBY;
	// in battle?
	if (::TimeGet()-m_tmLastAtk < TIME_INBATTLE && this->IsPlayer())
		m_Info.iActType = _ACTION_ALERT;


	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;

	return true;
}

//--------------------------------------------------------------
BOOL CRole::Mine(void)
{
	MYASSERT (_COMMAND_MINE == m_Info.cmdProc.iType);

	m_Info.cmdProc.iStatus	= _CMDSTATUS_ACCOMPLISH;
	m_Info.iActType			= _ACTION_MINE;

	// set action pos
	m_Info.posActionBegin.x	= m_Info.posWorld.x;
	m_Info.posActionBegin.y	= m_Info.posWorld.y;
	m_Info.posActionEnd.x	= m_Info.posWorld.x;
	m_Info.posActionEnd.y	= m_Info.posWorld.y;

	return true;
}

//--------------------------------------------------------------
BOOL CRole::Bump(void)
{
	MYASSERT (m_Info.cmdProc.iType == _COMMAND_BUMP);

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_BEGIN)
		m_Info.cmdProc.iStatus = _CMDSTATUS_PROGRESS;

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS || 
			m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{	
		m_Info.objPath.ClearStep();

		CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_Info.cmdProc.idTarget);
		if (pTarget)
		{
			CMyPos posTarget;
			pTarget->GetPos(posTarget);
			
			CMyPos posMyself;
			this->GetPos(posMyself);

			int nAtkRange = 1;//this->GetAddSize()+pTarget->GetAddSize()+1; //this->GetAtkRange();
			if (abs(posMyself.x-posTarget.x) <= nAtkRange
					&& abs(posMyself.y-posTarget.y) <= nAtkRange)
			{
				if (!(posMyself.x == posTarget.x && posMyself.y == posTarget.y))
				{
					// set dir
					this->SetDir(this->GetDir(posTarget));
					
					// set act data
					m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
					m_Info.iActType	        =_ACTION_BUMP;
					
					// set action pos
					m_Info.posActionBegin.x	=m_Info.posWorld.x;
					m_Info.posActionBegin.y	=m_Info.posWorld.y;

					// target pos
					CMyPos posDes = { posTarget.x+_DELTA_X[m_Info.iDirection], 
						posTarget.y+_DELTA_Y[m_Info.iDirection] };

					if (g_objGameMap.TestPath(pTarget, posDes) && pTarget->IsBumpBackEnable())
					{									
						g_objGameMap.Cell2World(posTarget.x,
							posTarget.y,
							m_Info.posActionEnd.x,
							m_Info.posActionEnd.y);
					}
					else
						m_Info.posActionEnd = m_Info.posActionBegin;
					
					// attack time
					m_tmLastAtk = ::TimeGet();
					return true;
				}
			}
		}
		else
		{
			m_Info.objPath.ClearStep();

			StandBy();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			return false;
		}

		CMyPos posTarget;
		pTarget->GetPos(posTarget);

		g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, posTarget);
		if (m_Info.objPath.GetStepAmount() > 0)
		{
			StepInfo infoStep;
			m_Info.iDirection = m_Info.objPath.GetNextStep(infoStep);
			this->Step();

			// reset action to running
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
				m_Info.iActType	=_ACTION_RUNL;
				m_Info.cmdProc.iStatus =_CMDSTATUS_CONTINUE;
			}
			else if (m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
			{
				m_Info.iActType	=_ACTION_RUNR;
				m_Info.cmdProc.iStatus =_CMDSTATUS_PROGRESS;
			}		
		}
		else
		{
			m_Info.objPath.ClearStep();

			this->StandBy();

			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------
BOOL CRole::Action(void)
{
	// set act data
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
	m_Info.iActType	        =m_Info.cmdProc.nData;
	// set dir
	if (_ACTION_POPOFF != m_Info.iActType)
		this->SetDir(m_Info.cmdProc.nDir);
	else
		this->SetDir((m_Info.cmdProc.nDir));//+4)%8);

	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;

	m_Info.nPosActionBeginZ = m_Info.nZ;
	m_Info.nPosActionEndZ = m_Info.cmdProc.nTargetZ;
	if (m_Info.cmdProc.dwData == 0)
	{
		m_Info.posActionEnd.x	=m_Info.posWorld.x;
		m_Info.posActionEnd.y	=m_Info.posWorld.y;
	}
	else if (m_Info.cmdProc.dwData == 1)
	{
		g_objGameMap.Cell2World(m_Info.posMap.x+_DELTA_X[m_Info.cmdProc.nDir],
								m_Info.posMap.y+_DELTA_Y[m_Info.cmdProc.nDir],
								m_Info.posActionEnd.x,
								m_Info.posActionEnd.y);
	}
	else if (m_Info.cmdProc.dwData == 2)
	{
		g_objGameMap.Cell2World(m_Info.cmdProc.posTarget.x,
								m_Info.cmdProc.posTarget.y,
								m_Info.posActionEnd.x,
								m_Info.posActionEnd.y);
	}
	
	// attack?
	if (m_Info.iActType == _ACTION_ATTACK0 
			|| m_Info.iActType == _ACTION_ATTACK1
			|| m_Info.iActType == _ACTION_ATTACK2
			|| m_Info.iActType == _ACTION_BUMP
			|| m_Info.iActType == _ACTION_JUMP_ATK)
		m_tmLastAtk = ::TimeGet();
	if(this->GetID() == g_objHero.GetID())
	{
		CMyPos posCell;
		g_objGameMap.World2Cell(m_Info.posActionBegin.x, m_Info.posActionBegin.y, posCell.x, posCell.y);
		::DebugMsg("BeginPos(%d, %d)\n", posCell.x, posCell.y);
		g_objGameMap.World2Cell(m_Info.posActionEnd.x, m_Info.posActionEnd.y, posCell.x, posCell.y);
		::DebugMsg("EndPos(%d, %d)\n", posCell.x, posCell.y);
	}

	return true;
}

//--------------------------------------------------------------
BOOL CRole::Pose(void)
{
	// set dir
	this->SetDir(m_Info.cmdProc.nDir);
	
	// set act data
	m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	m_Info.iActType	        =m_Info.cmdProc.nData;

	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;
	
	return false;
}

//--------------------------------------------------------------
BOOL CRole::Kongfu(void)
{
	if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		// 重置为第0个动作
		m_Info.cmdProc.dwData = 0;
		m_Info.cmdProc.iStatus = _CMDSTATUS_CONTINUE;
	}
	if(m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{
		KongfuAction actionKongfu = m_setKongfu[m_Info.cmdProc.dwData];
		m_Info.iActType = actionKongfu.nAction;
		m_Info.cmdProc.nDir = (actionKongfu.cDir + this->GetDir())%8;
		m_Info.iDirection = (actionKongfu.cDir + this->GetDir())%8;
		m_Info.cmdProc.dwData ++;

		m_Info.posActionBegin.x	=m_Info.posWorld.x;
		m_Info.posActionBegin.y	=m_Info.posWorld.y;

		CMyPos posEnd = m_Info.posMap;
		for(int i = 0; i < (int)actionKongfu.setStep.size(); i ++)
		{
			posEnd.x += _DELTA_X[(actionKongfu.setStep[i] + this->GetDir())%8];
			posEnd.y += _DELTA_Y[(actionKongfu.setStep[i] + this->GetDir())%8];
		}
		g_objGameMap.Cell2World(posEnd.x, posEnd.y, m_Info.posActionEnd.x, m_Info.posActionEnd.y);
		this->m_objEffect.Add(actionKongfu.szEffect,false,0,this->GetLook());
		if(m_Info.cmdProc.dwData >= m_setKongfu.size())
		{
			m_Info.cmdProc.iStatus = _CMDSTATUS_ACCOMPLISH;
			return true;
		}
		this->SetDir(m_Info.cmdProc.nDir);
	}
	return false;
}
//--------------------------------------------------------------
BOOL CRole::Emotion(void)
{		
	// set dir
	this->SetDir(m_Info.cmdProc.nDir);

	// set action type
	BOOL bAccomplished	=false;
    switch(m_Info.cmdProc.nData)
    {
    case _ACTION_SALUTE:
	case _ACTION_BRUISE0:
	case _ACTION_DODGE0:
        m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
	    m_Info.iActType	        =m_Info.cmdProc.nData;
        bAccomplished           =true;
        break;

	case _ACTION_DANCE1:
	case _ACTION_DANCE2:
	case _ACTION_DANCE3:
	case _ACTION_DANCE4:
	case _ACTION_DANCE5:
	case _ACTION_DANCE6:
	case _ACTION_DANCE7:
	case _ACTION_DANCE8:
    case _ACTION_SAD:
    case _ACTION_LAUGH:
    case _ACTION_GENUFLECT:
	case _ACTION_ATTACK0:
	case _ACTION_ATTACK1:
	case _ACTION_ATTACK2:
	case _ACTION_REST1:
	case _ACTION_SITDOWN_STATIC:
        m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	    m_Info.iActType	        =m_Info.cmdProc.nData;
        break;
	case _ACTION_SITDOWN:
		{
            if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
            {
                m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
                m_Info.iActType	        =_ACTION_SITDOWN;
            }
            else if(_CMDSTATUS_PROGRESS == m_Info.cmdProc.iStatus)
            {
				m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
				m_Info.iActType	        =_ACTION_SITDOWN_STATIC;
            }
		}
		break;
	case _ACTION_INTONE:
		{
            if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
            {
                m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
                m_Info.iActType	        =_ACTION_INTONE;
				m_Info.nFlyOffsetGoal   = 0;
            }
            else if(_CMDSTATUS_PROGRESS == m_Info.cmdProc.iStatus)
            {
				m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
				m_Info.iActType	        =_ACTION_INTONE_DURATION;
            }
		}
		break;
    case _ACTION_FAINT:
        {
            if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
            {
                m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
                m_Info.iActType	        =_ACTION_FAINT;
            }
            else if(_CMDSTATUS_PROGRESS == m_Info.cmdProc.iStatus)
            {
              	m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
	            m_Info.iActType	        =_ACTION_LIE;
            }
        }
        break;

/*	case _ACTION_COOL:
        {
            if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
            {
                m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
                m_Info.iActType	        =_ACTION_COOL;
            }
            else if(_CMDSTATUS_PROGRESS == m_Info.cmdProc.iStatus)
            {
              	m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
	            m_Info.iActType	        =_ACTION_COOLPOSE;
            }
        }
        break;*/

	default:
        m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	    m_Info.iActType	        =_ACTION_STANDBY;
        bAccomplished           =true;
        break;
    }

	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;

    return bAccomplished;
}

//--------------------------------------------------------------
BOOL CRole::Attack(void)
{
	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;
	if(m_Info.idRWeaponType/1000 == 470)
		m_Info.iActType = _ACTION_MINE;
	else
		m_Info.iActType	        =(_ACTION_ATTACK0+::RandGet(3));
	m_Info.iDirection		=m_Info.cmdProc.nDir;
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;

	// record time
	m_tmLastAtk = ::TimeGet();
	return true;
}

//--------------------------------------------------------------
BOOL CRole::RushAtk(void)
{
	if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		CPlayer* pTarget	=g_objPlayerSet.GetPlayer(m_Info.cmdProc.idTarget);
		if (!pTarget)
		{
			if(g_objHero.GetID() == (OBJID)m_Info.cmdProc.idTarget)
				pTarget	=&g_objHero;
			else
			{
				this->StandBy();
				return true;
			}
		}

		// set dir
		m_Info.iDirection	=m_Info.cmdProc.nDir;

		// get jump target pos
		CMyPos posTarget;
		pTarget->GetPos(posTarget);
		posTarget.x	-=_DELTA_X[m_Info.cmdProc.nDir];
		posTarget.y	-=_DELTA_Y[m_Info.cmdProc.nDir];
		
		
		// set action pos
		m_Info.posActionBegin.x	=m_Info.posWorld.x;
		m_Info.posActionBegin.y	=m_Info.posWorld.y;

		g_objGameMap.Cell2World(posTarget.x, posTarget.y, m_Info.posActionEnd.x, m_Info.posActionEnd.y);

		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_DEPART;
		m_Info.cmdProc.posTarget=posTarget;

		m_Info.iActType			=_ACTION_JUMP;
		m_Info.iDirection		=this->GetDir(m_Info.cmdProc.posTarget);

		return false;
	}
	else if (m_Info.cmdProc.iStatus	== _CMDSTATUS_DEPART)
	{
		this->Attack();
		return true;
	}

	return true;
}
//--------------------------------------------------------------
BOOL CRole::InTone(void)
{
/*	if(_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		this->StaticAction(_ACTION_BEGININTONE);		
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}
	else if (_CMDSTATUS_PROGRESS == m_Info.cmdProc.iStatus)
	{
		this->StaticAction(_ACTION_INTONE);
		m_Info.cmdProc.iStatus = _CMDSTATUS_CONTINUE;
	}
*/	

	return false;
}

//--------------------------------------------------------------
BOOL CRole::LockBooth(void)
{
	assert(m_Info.cmdProc.iType==_COMMAND_LOCKBOOTH);

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_BEGIN)
		m_Info.cmdProc.iStatus = _CMDSTATUS_PROGRESS;

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS || 
			m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{	
		m_Info.objPath.ClearStep();

		CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_Info.cmdProc.idTarget);
		if (!pTarget)
		{
			m_Info.objPath.ClearStep();
			StandBy();
			this->SetDir(m_Info.cmdProc.nDir);
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			return false;
		}

		CMyPos posTarget;
		pTarget->GetPos(posTarget);

		CMyPos posBooth; 
		posBooth.x = posTarget.x + _DELTA_X[0]*0;
		posBooth.y = posTarget.y + _DELTA_Y[0];
		CPlayer* pPlayer=g_objPlayerSet.GetPlayer(posBooth.x, posBooth.y);
		if(pPlayer)
			return false;
		g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, posBooth);
		if(m_Info.objPath.GetStepAmount() > 0) // not reach target
		{
			StepInfo infoStep;
			m_Info.objPath.GetNextStep(infoStep);
			m_Info.iDirection = infoStep.cStepDir;
			Step();	
			
			// reset action type
			// reset action
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
				if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
					m_Info.iActType	=_ACTION_RUNL;
				else
					m_Info.iActType	=_ACTION_WALKL;
			}
			else
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
				if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
					m_Info.iActType	=_ACTION_RUNR;
				else
					m_Info.iActType	=_ACTION_WALKR;
			}
			/*
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
			m_Info.iActType	=_ACTION_RUNL;
			m_Info.cmdProc.iStatus =_CMDSTATUS_CONTINUE;
			}
			else if (m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
			{
			m_Info.iActType	=_ACTION_RUNR;
			m_Info.cmdProc.iStatus =_CMDSTATUS_PROGRESS;
			}
			*/
		}
		else
		{
			StandBy();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
		}
	}
	

	return true;
}
//--------------------------------------------------------------
BOOL CRole::LockAtk(void)
{
	assert(m_Info.cmdProc.iType==_COMMAND_LOCKATK);

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_BEGIN)
	{
		m_Info.cmdProc.iStatus = _CMDSTATUS_PROGRESS;
	}

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS || 
			m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{	
		m_Info.objPath.ClearStep();

		CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_Info.cmdProc.idTarget, true);
		if (pTarget)
		{
			CMyPos posTarget;
			pTarget->GetPos(posTarget);
			
			CMyPos posMyself;
			this->GetPos(posMyself);

			int nAtkRange = (this->GetAddSize()+pTarget->GetAddSize()+1)/2+this->GetAtkRange();
			BOOL bCheck = false;
			if(!this->IsMyPet())
				bCheck = true;
			else
			{
				CPlayer* pStandPlayer = g_objPlayerSet.GetPlayer(posMyself.x, posMyself.y);
				if(!pStandPlayer || pStandPlayer == this)
					bCheck = true;
			}

			if (abs(posMyself.x-posTarget.x) <= nAtkRange &&
					abs(posMyself.y-posTarget.y) <= nAtkRange && bCheck)
			{
				m_Info.cmdProc.nDir = this->GetDir(posTarget);
				if(this->HaveBow())
				{
					this->Attack();
					CMyPos posTargetWorld;
					g_objGameMap.Cell2World(posTarget.x, posTarget.y, posTargetWorld.x, posTargetWorld.y);
					m_Info.posDir.x = posTargetWorld.x;
					m_Info.posDir.y = posTargetWorld.y;
					m_Info.bAdjustDir	=	true;
					// create arrow
					if(pTarget)
					{
						CMagicEffect* pEffect = CMagicEffect::CreateNew(g_objHero.GetID(), pTarget->GetID(), g_objHero.GetRWeapon(), 0);
						g_objGameMap.AddMagicEffect(pEffect);
					}
				}
				else
				{
					// 如果是召唤兽，不可重叠攻击 ...
					if(this->IsMyPet())
					{
						CPlayer* pStandPlayer = g_objPlayerSet.GetPlayer(posMyself.x, posMyself.y);
						if(!pStandPlayer || pStandPlayer == this)
							this->Attack();
						else
						{
							StandBy();			
							m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
							return false;
						}
					}
					this->Attack();
				}
				return true;
			}
		}
		else
		{
			m_Info.objPath.ClearStep();

			StandBy();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			return false;
		}

		CMyPos posTarget;
		pTarget->GetPos(posTarget);
		
		// 如果是召唤兽，找合适地点攻击
		if(this->IsMyPet())
		{
			CMyPos posMyTarget; 
			int nDir = ::RandGet(8);
			posMyTarget.x = posTarget.x + _DELTA_X[nDir];
			posMyTarget.y = posTarget.y + _DELTA_Y[nDir];
			CPlayer* pStandPlayer = g_objPlayerSet.GetPlayer(posMyTarget.x, posMyTarget.y);
			if(!pStandPlayer || pStandPlayer == this)
				g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, posMyTarget);
			else
			{
				m_Info.objPath.ClearStep();
				
				StandBy();
				m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
				return false;
			}
		}
		else
		{
			g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, posTarget);
		}
		if (m_Info.objPath.GetStepAmount() > 0)
		{
			StepInfo infoStep;
			m_Info.objPath.GetNextStep(infoStep);
			m_Info.iDirection = infoStep.cStepDir;
			Step();	

			// reset action to running
			// reset action
			// reset action
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
				if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
					m_Info.iActType	=_ACTION_RUNL;
				else
					m_Info.iActType	=_ACTION_WALKL;
			}
			else
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
				if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
					m_Info.iActType	=_ACTION_RUNR;
				else
					m_Info.iActType	=_ACTION_WALKR;
			}
			
			/*
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
				m_Info.iActType	=_ACTION_RUNL;
				m_Info.cmdProc.iStatus =_CMDSTATUS_CONTINUE;
			}
			else if (m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
			{
				m_Info.iActType	=_ACTION_RUNR;
				m_Info.cmdProc.iStatus =_CMDSTATUS_PROGRESS;
			}*/		
		}
		else
		{
			m_Info.objPath.ClearStep();

			StandBy();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------
BOOL CRole::TeamFollow	(void)
{
	assert(_COMMAND_TEAMFOLLOW == m_Info.cmdProc.iType);
	CMyPos posOffset = m_Info.cmdProc.posTarget;

	// get follow target
	CRole*	pTarget	=g_objPlayerSet.GetPlayer(m_Info.cmdProc.idTarget);
	if(!pTarget)
	{
		if(g_objHero.GetID() == m_Info.cmdProc.idTarget)
			pTarget	=&g_objHero;
		else	// error, no target found
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			StandBy();
			m_Info.cmdProc.posTarget = posOffset;
			return false;
		}
	}
	if(!g_objHero.GetTeamMemberInfoByID(m_Info.cmdProc.idTarget))
	{
		m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
		StandBy();
		m_Info.cmdProc.posTarget = posOffset;
		return false;
	}
	// get follow pos
	CMyPos posTarget;
	pTarget->GetPos(posTarget);
	int nDir = pTarget->GetDir(this);
	//const int _DELTA_X[8]={0,-1,-1,-1,0,1,1,1};
	//const int _DELTA_Y[8]={1,1,0,-1,-1,-1,0,1};

	m_Info.cmdProc.posTarget.x = posTarget.x + _DELTA_X[nDir] * m_Info.cmdProc.dwData;
 	m_Info.cmdProc.posTarget.y = posTarget.y + _DELTA_Y[nDir] * m_Info.cmdProc.dwData;

	if(m_Info.posMap.x != m_Info.cmdProc.posTarget.x ||
			m_Info.posMap.y != m_Info.cmdProc.posTarget.y)
	{
		m_Info.cmdProc.iStatus		=_CMDSTATUS_BEGIN;
	}

	// step now...
	if(m_Info.cmdProc.iStatus==_CMDSTATUS_BEGIN)
	{
		//m_Info.objStepData.ClearStep();
		g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, m_Info.cmdProc.posTarget);

		BOOL bHavePath = false;
		if(m_Info.objPath.GetStepAmount() > 0)
			bHavePath =true;

		if (bHavePath)
			m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
		else
		{
			m_Info.objPath.ClearStep();
			StandBy();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
			m_Info.cmdProc.posTarget = posOffset;
			return false;
		}
	}

	if(m_Info.cmdProc.iStatus==_CMDSTATUS_PROGRESS)
	{
		if(m_Info.objPath.GetStepAmount() > 0) // not reach target
		{
			StepInfo infoStep;
			m_Info.objPath.GetNextStep(infoStep);
			m_Info.iDirection = infoStep.cStepDir;
			// reset action to running
	/*		if(m_Info.objPath.GetStepAmount() > 2)
			{
				if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
				{
					m_Info.iActType	=_ACTION_RUNL;
					m_Info.cmdProc.iStatus =_CMDSTATUS_CONTINUE;
				}
				else if (m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
				{
					m_Info.iActType	=_ACTION_RUNR;
					m_Info.cmdProc.iStatus =_CMDSTATUS_PROGRESS;
				}
			}	*/	
			Step();
		}
		else
		{
			StandBy();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
		}
	}


	if(m_Info.cmdProc.iStatus==_CMDSTATUS_ACCOMPLISH)
	{
		m_Info.cmdProc.posTarget = posOffset;
		return true;
	}
	else
	{
		m_Info.cmdProc.posTarget = posOffset;
		return false;
	}
}
//--------------------------------------------------------------
BOOL CRole::PickUp(void)
{
	MYASSERT (m_Info.cmdProc.iType == _COMMAND_PICKUP);
	CMyPos posMyself; 
	this->GetPos(posMyself);
	CMyPos posTarget = m_Info.cmdProc.posTarget;
	
	int nRange = 0; //this->GetAddSize()+1;
	if (m_Info.cmdProc.iStatus==_CMDSTATUS_BEGIN)
	{
		m_Info.objPath.ClearStep();
		
		g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, m_Info.cmdProc.posTarget);
		
		if (abs(posMyself.x-posTarget.x) <= nRange &&
			abs(posMyself.y-posTarget.y) <= nRange)
		{
			StandBy();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			this->StaticAction(_ACTION_PICKUP, this->GetDir());//this->GetDir(posTarget));
			return true;
		}
		else if (m_Info.objPath.GetStepAmount() > 0)
			m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
		else	// no way
		{
			m_Info.objPath.ClearStep();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			
			this->StandBy();
			return false;
		}
	}
	
	if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS ||
		m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{
		if (abs(posMyself.x-posTarget.x) <= nRange &&
			abs(posMyself.y-posTarget.y) <= nRange)
		{
			m_Info.cmdProc.iStatus = _CMDSTATUS_ACCOMPLISH;
			this->StaticAction(_ACTION_PICKUP, this->GetDir());//this->GetDir(posTarget));
			return true;
		}
		else
		{
			if(m_Info.objPath.GetStepAmount() > 0) // not reach target
			{
				StepInfo infoStep;
				m_Info.objPath.GetNextStep(infoStep);
				m_Info.iDirection = infoStep.cStepDir;
				Step();	
				
				// reset action type
				// reset action
				if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
				{
					m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
					if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
						m_Info.iActType	=_ACTION_RUNL;
					else
						m_Info.iActType	=_ACTION_WALKL;
				}
				else
				{
					m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
					if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
						m_Info.iActType	=_ACTION_RUNR;
					else
						m_Info.iActType	=_ACTION_WALKR;
				}
				/*
				if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
				{
				m_Info.iActType	=_ACTION_RUNL;
				m_Info.cmdProc.iStatus =_CMDSTATUS_CONTINUE;
				}
				else if (m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
				{
				m_Info.iActType	=_ACTION_RUNR;
				m_Info.cmdProc.iStatus =_CMDSTATUS_PROGRESS;
				}
				*/
			}
			else
			{
				StandBy();
				m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			}
		}
	}
	
	return false;
}

//--------------------------------------------------------------
// return the direction to step, -1 means fail to step...
int CRole::GetStepDir(void)
{
	double dMinDistance		=65535;
	int	iStepDir	=-1;
	for(int i=0;i<_MAX_DIRECTION;i++)
	{
		int iStepX	=m_Info.posMap.x+_DELTA_X[i];
		int iStepY	=m_Info.posMap.y+_DELTA_Y[i];

		if (IsMapAttainable(iStepX,iStepY))
		{
			int iTargetX	=m_Info.cmdProc.posTarget.x;
			int iTargetY	=m_Info.cmdProc.posTarget.y;
			double  dDistance	=CGameMap::GetPreciseDistance(iStepX,iStepY,iTargetX,iTargetY);
			if (dDistance<dMinDistance)
			{
				dMinDistance	=dDistance;
				iStepDir		=i;
			}
		}
	}

	return iStepDir;
}

//--------------------------------------------------------------
BOOL CRole::Jump(void)
{
	// clear a* path
	m_Info.objPath.ClearStep();

	// clear lstdir
	m_lstStepDir.clear();
	
	// set action type
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
	m_Info.iActType			=_ACTION_JUMP;
/*
	if(this->TestStatus(USERSTATUS_FLY))
		m_Info.iActType			=_ACTION_FLY_MOVE;
*/
	m_Info.iDirection		=this->GetDir(m_Info.cmdProc.posTarget);

	// set pos
	//m_Info.posMap.x	=m_Info.cmdProc.posTarget.x;
	//m_Info.posMap.y	=m_Info.cmdProc.posTarget.y;

	// g_objGameMap.SetObj(m_Info.posMap.x,m_Info.posMap.y,_OBJ_NULL);

	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;

	g_objGameMap.Cell2World(m_Info.cmdProc.posTarget.x, m_Info.cmdProc.posTarget.y, m_Info.posActionEnd.x, m_Info.posActionEnd.y);

	return true;
}
//--------------------------------------------------------------
BOOL	CRole::Float(void)
{
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;

//	if (_COMMAND_FLOAT == this->GetCommandType())
//		m_Info.iActType	=_ACTION_FLY_STANDBY;

	// in battle?
/*	if (::TimeGet()-m_tmLastAtk < TIME_INBATTLE)
		m_Info.iActType = _ACTION_FLY_ALERT;
*/
	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;

	return true;
}
//--------------------------------------------------------------
BOOL	CRole::TakeOff(void)
{
	// clear a* path
	m_Info.objPath.ClearStep();

	// clear lstdir
	m_lstStepDir.clear();
	
	// set action type
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
	m_Info.iActType			=_ACTION_FLY_UP;
	m_Info.iDirection		=this->GetDir(m_Info.cmdProc.posTarget);
	this->SetFlyOffsetGoal(100);
	return true;
}
//--------------------------------------------------------------
BOOL	CRole::Landing(void)
{
	if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		// clear a* path
		m_Info.objPath.ClearStep();

		// clear lstdir
		m_lstStepDir.clear();
	
		// set action type
		m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
		m_Info.iActType			=_ACTION_FLY_DOWN;
		m_Info.iDirection		=this->GetDir(m_Info.cmdProc.posTarget);
		this->SetFlyOffsetGoal(0);
	}
	return true;
}
//--------------------------------------------------------------
BOOL CRole::Die(void)
{
	enum 
	{
			DIE_NORMAL			= 1,
			DIE_CRITICALHIT		= 2,
			DIE_MAGIC			= 3,
			DIE_FLOAT			= 4,
			DIE_ARROWSHOOT		= 5,
	};

	BOOL bReturn = false;
//	if(m_Info.nFlyOffset != 0)
//		m_Info.cmdProc.dwData = DIE_FLOAT;
	switch(m_Info.cmdProc.dwData)
	{
//	case DIE_CRITICALHIT:
//		bReturn = this->DieFlying();
//		break;

	case DIE_MAGIC:
		bReturn = this->DieDelay();
		break;
	case DIE_FLOAT:
		bReturn = this->DieFloat();
		break;
	case DIE_ARROWSHOOT:
		bReturn = this->DieDelay();
		break;
	default:		
		bReturn = this->DieNormal();
		break;
	}

	return bReturn;
}

//--------------------------------------------------------------
BOOL CRole::DieNormal(void)
{
	if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		// set action
		m_Info.iActType		=_ACTION_DIE0;
/*		if (::RandGet(2) == 1)
			m_Info.iActType		=_ACTION_DIE0;
		else
			m_Info.iActType		=_ACTION_DIE1;
*/
		m_Info.iDirection	=m_Info.cmdProc.nDir;

		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
	}
	else if (m_Info.cmdProc.iStatus	== _CMDSTATUS_CONTINUE)
	{
		if (m_Info.iActType == _ACTION_DIE0)
			m_Info.iActType = _ACTION_BODY0;
		else
			m_Info.iActType = _ACTION_BODY1;

		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}

	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;

	// never end
	return false;
}

//--------------------------------------------------------------
BOOL CRole::DieFloat(void)
{
	if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		// set action
		m_Info.iActType		=_ACTION_FLY_DIE;

		m_Info.iDirection	=m_Info.cmdProc.nDir;
		m_Info.nFlyOffsetGoal = 0;

		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
	}
	else if (m_Info.cmdProc.iStatus	== _CMDSTATUS_CONTINUE)
	{
		m_Info.iActType = _ACTION_BODY0;
		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}

	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;

	// never end
	return false;
}

//--------------------------------------------------------------
BOOL CRole::DieDelay(void)
{
	if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		// set action
		m_Info.iActType		=_ACTION_BRUISE0;
		m_Info.iDirection	=m_Info.cmdProc.nDir;

		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_DEPART;
	}
	else if (m_Info.cmdProc.iStatus	== _CMDSTATUS_DEPART)
	{
		// set action
		m_Info.iActType		=_ACTION_DIE0;
/*		if (::RandGet(2) == 1)
			m_Info.iActType		=_ACTION_DIE0;
		else
			m_Info.iActType		=_ACTION_DIE1;
*/
		m_Info.iDirection	=m_Info.cmdProc.nDir;

		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
	}
	else if (m_Info.cmdProc.iStatus	== _CMDSTATUS_CONTINUE)
	{
		m_Info.iActType = _ACTION_BODY0;
/*		if (m_Info.iActType == _ACTION_DIE0)
			m_Info.iActType = _ACTION_BODY0;
		else
			m_Info.iActType = _ACTION_BODY1;
*/
		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}

	// set action pos
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;
	m_Info.posActionEnd.x	=m_Info.posWorld.x;
	m_Info.posActionEnd.y	=m_Info.posWorld.y;

	// never end
	return false;
}

//--------------------------------------------------------------
BOOL CRole::DieFlying(void)
{
	if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
	{
		// set action pos
		m_Info.posActionBegin.x	=m_Info.posWorld.x;
		m_Info.posActionBegin.y	=m_Info.posWorld.y;

		CMyPos posTarget;
		this->GetPos(posTarget);
		posTarget.x	-=2*_DELTA_X[m_Info.cmdProc.nDir];
		posTarget.y	-=2*_DELTA_Y[m_Info.cmdProc.nDir];
		posTarget.x	-=2*_DELTA_X[m_Info.cmdProc.nDir];
		posTarget.y	-=2*_DELTA_Y[m_Info.cmdProc.nDir];

		g_objGameMap.Cell2World(posTarget.x, posTarget.y, m_Info.posActionEnd.x, m_Info.posActionEnd.y);

		// set action
		if(this->IsPlayer())
			m_Info.iActType		=_ACTION_DIEFLY;
		else
			m_Info.iActType		=_ACTION_DIE0;

		m_Info.iDirection	=m_Info.cmdProc.nDir;
		m_Info.cmdProc.iStatus	= _CMDSTATUS_DEPART;
	}
	else if (m_Info.cmdProc.iStatus	== _CMDSTATUS_DEPART)
	{
		if(this->IsPlayer())
			m_Info.iActType	= _ACTION_DIEFLYEND;
		else
			m_Info.iActType = _ACTION_BODY0;

		// set action pos
		m_Info.posActionBegin.x	=m_Info.posWorld.x;
		m_Info.posActionBegin.y	=m_Info.posWorld.y;
		m_Info.posActionEnd.x	=m_Info.posWorld.x;
		m_Info.posActionEnd.y	=m_Info.posWorld.y;

		// change step
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}

	// never end
	return false;
}

//--------------------------------------------------------------
void CRole::ResetStepDir()
{
	if (true)
		return;
	/*
	if (this->GetID() == g_objHero.GetID())
		return;
	*/

	this->ResetActionData();

	int nSteps	=m_lstStepDir.size();
	if (nSteps <= 0)
		return;

	CMyPos posRole;
	this->GetPos(posRole);

	for (int i=0; i<nSteps; i++)
	{
		int nDirection	=m_lstStepDir.front();
		posRole.x	+=_DELTA_X[nDirection];
		posRole.y	+=_DELTA_Y[nDirection];
		
		m_lstStepDir.pop_front();
	}

	this->SetPos(posRole.x, posRole.y);
}

//--------------------------------------------------------------
// implement role step in map, set objid at new pos and del clr objid at old place
// must set m_Info.iDirection before call this function
BOOL CRole::Step(void)
{
	// set action type
	//  [1/29/2008]----------Chancy注释------------begin-----------------------------
//	Chancy暂时注释，因为此句逻辑不符合if(abs(posAligne.x - posHero.x) >= 2 || abs(posAligne.x - posHero.x) >= 2)永远不会进入
	/*if(m_Info.id == g_objHero.GetID())
	{
		CMyPos posHero, posAligne;
		g_objHero.GetPos(posHero);
		g_objHero.GetPos(posAligne);
		if(abs(posAligne.x - posHero.x) >= 2 || abs(posAligne.x - posHero.x) >= 2)
		{
			if(m_nWhichLeg == 1)
			{
				m_Info.iActType	=_ACTION_WALKL;
				m_nWhichLeg = 0;
			}
			else
			{
				m_Info.iActType	=_ACTION_WALKR;
				m_nWhichLeg = 1;
			}
			m_Info.posActionBegin.x	=m_Info.posWorld.x;
			m_Info.posActionBegin.y	=m_Info.posWorld.y;
			m_Info.posActionEnd.x =m_Info.posWorld.x;
			m_Info.posActionEnd.y =m_Info.posWorld.y;
			return true;
		}
	}*/
	BOOL bDoubleStep = false;
	if(m_Info.objPath.TestNextStep() != -1)
		bDoubleStep = true;
	int nStepAmount = 1;
	if(m_Info.bAdjustRun)
		bDoubleStep = false;
	BOOL bFollowRun = false;
	{
		CMyPos posHero, posAligne;
		g_objHero.GetPos(posHero);
		g_objHero.GetPos(posAligne);
		
		if(_COMMAND_TEAMFOLLOW == m_Info.cmdProc.iType && m_Info.objPath.GetStepAmount() >= 4)
			bFollowRun = true;
	}
	if(this->GetID() == g_objHero.GetID() && g_objHero.m_nFastRunSetp < 2
		&& this->TestStatus(USERSTATUS_PELT))
	{
		bDoubleStep = false;
		g_objHero.m_nFastRunSetp ++;
	}
	if(bDoubleStep && (m_Info.cmdProc.iType == _COMMAND_RUNFORWARD || _COMMAND_RUN == m_Info.cmdProc.iType
		|| _COMMAND_PICKUP == m_Info.cmdProc.iType || _COMMAND_LOCKATK ==  m_Info.cmdProc.iType || bFollowRun))
	{
		if(m_nWhichLeg == 1)
		{
			m_Info.iActType	=_ACTION_RUNL;
			m_nWhichLeg = 0;
		}
		else
		{
			m_Info.iActType	=_ACTION_RUNR;
			m_nWhichLeg = 1;
		}
		nStepAmount = 2;
	}
	else
	{
		if(m_nWhichLeg == 1)
		{
			m_Info.iActType	=_ACTION_WALKL;
			m_nWhichLeg = 0;
		}
		else
		{
			m_Info.iActType	=_ACTION_WALKR;
			m_nWhichLeg = 1;
		}
	}
	m_Info.posActionBegin.x	=m_Info.posWorld.x;
	m_Info.posActionBegin.y	=m_Info.posWorld.y;


	// 以下为获得m_Info.posActionEnd坐标,即完成一次走或跑(单腿)目标点坐标,它与m_Info.posActionBegin之间的差值就是要完成的位移距离添加注释 [Chancy,3/29/2008]
	if(nStepAmount == 2)
	{
		StepInfo info;
		m_Info.objPath.GetNextStep(info);
		m_Info.nNextStep = info.cStepDir%8;
		g_objGameMap.Cell2World(m_Info.posMap.x+_DELTA_X[m_Info.iDirection]+_DELTA_X[m_Info.nNextStep], 
			m_Info.posMap.y+_DELTA_Y[m_Info.iDirection]+_DELTA_Y[m_Info.nNextStep], 
			m_Info.posActionEnd.x, 
			m_Info.posActionEnd.y);
		const int RUNDIRIDADDITION = 20;
		m_Info.nNextStep += RUNDIRIDADDITION;
		
		m_Info.bAdjustDir = true;
		m_Info.posDir.x = m_Info.posActionEnd.x;
		m_Info.posDir.y = m_Info.posActionEnd.y;
	}
	else
	{
		
		g_objGameMap.Cell2World(m_Info.posMap.x+_DELTA_X[m_Info.iDirection], 
			m_Info.posMap.y+_DELTA_Y[m_Info.iDirection], 
			m_Info.posActionEnd.x, 
			m_Info.posActionEnd.y);
	}


	return true;
}

//--------------------------------------------------------------
// return true if reached target place
BOOL CRole::Walk(void)
{
	assert(m_Info.cmdProc.iType==_COMMAND_WALK);
	
#ifdef	_USE_ASTAR
	if (m_Info.cmdProc.iStatus==_CMDSTATUS_BEGIN)
	{
		m_Info.objPath.ClearStep();
		
		g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, m_Info.cmdProc.posTarget);
		
		if(m_Info.objPath.GetStepAmount() > 0)
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
		}
		else
		{
			m_Info.objPath.ClearStep();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			
			StandBy();
			return false;
		}
	}
	
	if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS ||
		m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{
		if(m_Info.objPath.GetStepAmount() > 0) // not reach target
		{
			StepInfo infoStep;
			m_Info.objPath.GetNextStep(infoStep);
			m_Info.iDirection = infoStep.cStepDir;
			Step();	
			
			// reset action
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
				if(m_Info.iActType == _ACTION_WALKR)
					m_Info.iActType	=_ACTION_WALKL;
				else
					m_Info.iActType = _ACTION_WALKR;
			}
			else
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
				if(m_Info.iActType == _ACTION_WALKR)
					m_Info.iActType	=_ACTION_WALKL;
				else
					m_Info.iActType = _ACTION_WALKR;
			}
		}
		else
		{
			m_Info.objPath.ClearStep();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			
			StandBy();
		}
	}
	
#else
	if(m_Info.cmdProc.iStatus==_CMDSTATUS_BEGIN)
	{
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}
	
	if(m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS ||
		m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{
		// reach the target?
		if(IsMapReached())
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			StandBy();
		}
		else
		{
			int iStepDir	=GetStepDir();
			if(iStepDir==-1)
				StandBy();
			else
			{
				m_Info.iDirection	=iStepDir;
				Step();
				
				// reset action
				if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
				{
					m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
					m_Info.iActType	=_ACTION_WALKL;
				}
				else
				{
					m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
					m_Info.iActType	=_ACTION_WALKR;
				}
			}
		}
	}
	
#endif
	
	if(m_Info.cmdProc.iStatus==_CMDSTATUS_ACCOMPLISH)
		return true;
	else
		return false;
}

//--------------------------------------------------------------
// return false always...
BOOL CRole::WalkForward(void)
{
	assert(m_Info.cmdProc.iType == _COMMAND_WALKFORWARD);
	
	/*
	if (m_Info.cmdProc.iStatus == _CMDSTATUS_BEGIN)
	m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	
	  if (m_lstStepDir.size() <= 0)
	  {
	  this->StandBy();
	  }
	  else
	  {
	  m_Info.iDirection	=m_lstStepDir.front();
	  m_lstStepDir.pop_front();
	  
		this->Step();
		}
		
		  return false;
	*/
	
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
	
	this->SetDir(m_Info.cmdProc.nDir);
	this->Step();
	
	return true;
}

//--------------------------------------------------------------
// Add step, continue run, not interrupt
void	CRole::AddStep(CMyPos posCell)
{
	CPath objPath;
	objPath.ClearStep();
	m_Info.objPath.ClearStep();
	CMyPos posHero;
	g_objGameMap.World2Cell(m_Info.posActionEnd.x, m_Info.posActionEnd.y, posHero.x, posHero.y);
	g_objGameMap.FindPath(objPath, posHero, posCell);
	int nAmount = objPath.GetStepAmount();
	for(int i= 0; i < nAmount; i ++)
	{
		StepInfo infoStep;
		objPath.GetNextStep(infoStep);
		m_Info.objPath.AddStep(infoStep.cStepDir, infoStep.ucStepLength);
	}
}
//--------------------------------------------------------------

// return true if reached target place
BOOL CRole::Run(void)
{
	assert(m_Info.cmdProc.iType==_COMMAND_RUN);

#ifdef	_USE_ASTAR
	if (m_Info.cmdProc.iStatus==_CMDSTATUS_BEGIN)
	{
		m_Info.objPath.ClearStep();

		g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, m_Info.cmdProc.posTarget);

		if(m_Info.objPath.GetStepAmount() > 0)
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
		}
		else
		{
			m_Info.objPath.ClearStep();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;

			StandBy();
			return false;
		}
	}

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS ||
			m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{
		if(m_Info.objPath.GetStepAmount() > 0) // not reach target
		{
			StepInfo infoStep;
			m_Info.objPath.GetNextStep(infoStep);
			m_Info.iDirection	=infoStep.cStepDir;
			Step();	

			// reset action
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
				if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
				{
/*					if(m_Info.iActType == _ACTION_RUNR)
						m_Info.iActType	=_ACTION_RUNL;
					else
						m_Info.iActType = _ACTION_RUNL;*/
				}
				else
				{
/*					if(m_Info.iActType == _ACTION_WALKR)
						m_Info.iActType	=_ACTION_WALKL;
					else
						m_Info.iActType = _ACTION_WALKL;*/
				}
			}
			else
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
				if(m_Info.iActType == _ACTION_RUNR || m_Info.iActType == _ACTION_RUNL)
				{
/*					if(m_Info.iActType == _ACTION_RUNR)
						m_Info.iActType	=_ACTION_RUNL;
					else
						m_Info.iActType = _ACTION_RUNL;*/
				}
				else
				{
/*					if(m_Info.iActType == _ACTION_WALKR)
						m_Info.iActType	=_ACTION_WALKL;
					else
						m_Info.iActType = _ACTION_WALKL;*/
				}
			}
		}
		else
		{
			m_Info.objPath.ClearStep();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;

			StandBy();
		}
	}

#else
	if(m_Info.cmdProc.iStatus==_CMDSTATUS_BEGIN)
	{
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}

	if(m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS ||
			m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{
		// reach the target?
		if(IsMapReached())
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			StandBy();
		}
		else
		{
			int iStepDir	=GetStepDir();
			if(iStepDir==-1)
				StandBy();
			else
			{
				m_Info.iDirection	=iStepDir;
				Step();

				// reset action
				if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
				{
					m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
					if(!m_Info.bAdjustRun)
						m_Info.iActType	=_ACTION_RUNL;
					else
						m_Info.iActType	=_ACTION_WALKL;
				}
				else
				{
					m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
					if(!m_Info.bAdjustRun)
						m_Info.iActType	=_ACTION_RUNR;
					else
						m_Info.iActType	=_ACTION_WALKR;
				}
			}
		}
	}

#endif

	if(m_Info.cmdProc.iStatus==_CMDSTATUS_ACCOMPLISH)
		return true;
	else
		return false;
}

//--------------------------------------------------------------
// return false always...
BOOL CRole::RunForward(void)
{
	assert(m_Info.cmdProc.iType == _COMMAND_RUNFORWARD);
	
	/*
	if (m_Info.cmdProc.iStatus == _CMDSTATUS_BEGIN)
	m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	
	  if (m_lstStepDir.size() <= 0)
	  {
	  this->StandBy();
	  }
	  else
	  {
	  m_Info.iDirection	=m_lstStepDir.front();
	  m_lstStepDir.pop_front();
	  
		this->Step();
		
		  // reset action
		  if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
		  {
		  m_Info.iActType	=_ACTION_RUNL;
		  m_Info.cmdProc.iStatus =_CMDSTATUS_CONTINUE;
		  }
		  else if (m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
		  {
		  m_Info.iActType	=_ACTION_RUNR;
		  m_Info.cmdProc.iStatus =_CMDSTATUS_PROGRESS;
		  }		
		  }
		  
			return false;
	*/
	
	m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
	
	int nPreAction = m_Info.iActType;
	this->SetDir(m_Info.cmdProc.nDir);
	m_Info.objPath.AddStep((char)m_Info.cmdProc.dwData, 32);
	this->Step();
	
	/*	if (_ACTION_RUNR == nPreAction)	// last action is runR
	m_Info.iActType	=_ACTION_RUNL;
	else
	m_Info.iActType	=_ACTION_RUNR;*/
	
	return true;
}

//--------------------------------------------------------------
BOOL CRole::Follow(void)
{
	assert(_COMMAND_FOLLOW == m_Info.cmdProc.iType);

	// get follow target
	CRole*	pTarget	=g_objPlayerSet.GetPlayer(m_Info.cmdProc.idTarget);
	if(!pTarget)
	{
		if(g_objHero.GetID() == m_Info.cmdProc.idTarget)
			pTarget	=&g_objHero;
		else	// error, no target found
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;
			StandBy();
			return false;
		}
	}

	// get follow pos
	int nTargetX=0, nTargetY=0;
	pTarget->GetPrePos(nTargetX, nTargetY);

	if(nTargetX != m_Info.cmdProc.posTarget.x ||
			nTargetY != m_Info.cmdProc.posTarget.y)
	{
		m_Info.cmdProc.iStatus		=_CMDSTATUS_BEGIN;
		m_Info.cmdProc.posTarget.x	=nTargetX;
		m_Info.cmdProc.posTarget.y	=nTargetY;
	}

	// step now...
#ifdef	_USE_ASTAR
	if (m_Info.cmdProc.iStatus==_CMDSTATUS_BEGIN)
	{
		m_Info.objPath.ClearStep();

		g_objGameMap.FindPath(m_Info.objPath, m_Info.posMap, m_Info.cmdProc.posTarget);

		if(m_Info.objPath.GetStepAmount() > 0)
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
		}
		else
		{
			m_Info.objPath.ClearStep();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;

			StandBy();
			return false;
		}
	}

	if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS ||
			m_Info.cmdProc.iStatus == _CMDSTATUS_CONTINUE)
	{
		if(m_Info.objPath.GetStepAmount() > 0) // not reach target
		{
			StepInfo infoStep;
			m_Info.objPath.GetNextStep(infoStep);
			m_Info.iDirection	=infoStep.cStepDir;

			// reset action
			if (m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
			}
			else
			{
				m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
			}
			Step();	
		}
		else
		{
			m_Info.objPath.ClearStep();
			m_Info.cmdProc.iStatus	=_CMDSTATUS_ACCOMPLISH;

			StandBy();
		}
	}
	
#else
	if(m_Info.cmdProc.iStatus == _CMDSTATUS_BEGIN)
	{
		m_Info.cmdProc.iStatus	=_CMDSTATUS_PROGRESS;
	}

	if(m_Info.cmdProc.iStatus == _CMDSTATUS_PROGRESS)
	{
		if(m_Info.posMap.x == m_Info.cmdProc.posData.x &&
				m_Info.posMap.y == m_Info.cmdProc.posData.y)
		{
			m_Info.cmdProc.iStatus	=_CMDSTATUS_CONTINUE;
			StandBy();
		}
		else
		{
			int iStepDir	=GetStepDir();
			if(iStepDir==-1)
				StandBy();
			else
			{
				m_Info.iDirection	=iStepDir;
				Step();
			}
		}
	}

#endif

	if(m_Info.cmdProc.iStatus==_CMDSTATUS_ACCOMPLISH)
		return true;
	else
		return false;
}


//--------------------------------------------------------------

void CRole::SetActionCmd(int nAction, int nDir/*=-1*/, DWORD dwType/*=0*/, BOOL bData/*=false*/)
{
	CCommand cmd;
	cmd.iType	= _COMMAND_ACTION;
	cmd.iStatus	= _CMDSTATUS_BEGIN;
	cmd.nData	= nAction;
	cmd.nDir	= (-1 == nDir) ? this->GetDir() : nDir;
	cmd.dwData	= dwType;
	cmd.bData	= bData;
	this->SetCommand(&cmd);
}

//--------------------------------------------------------------
void CRole::SetIntoneCmd(int nAction, int nDir/*=-1*/, DWORD dwType/*=0*/, BOOL bData/*=false*/)
{
	CCommand cmd;
	cmd.iType	= _COMMAND_INTONE;
	cmd.iStatus	= _CMDSTATUS_BEGIN;
	cmd.nData	= nAction;
	cmd.nDir	= (-1 == nDir) ? this->GetDir() : nDir;
	cmd.dwData	= dwType;
	cmd.bData	= bData;
	this->SetCommand(&cmd);
}
