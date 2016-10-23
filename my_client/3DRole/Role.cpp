#pragma warning(disable:4786)
#include "basefunc.H"
#include "nd_bitmap.H"
#include "ani.H"
#include "3dgamemap.H"
#include "3droledata.H"
#include "3DRole.h"
#include "role.H"
#include "Hero.H"
#include "GameMsg.H"
#include "GameDataSet.H"
#include "GamePlayerSet.h"
#include "3DStatuary.h"
#include "3DTarget.h"
#include "3DTaskNpc.h"
#include "SynStone.h"
#include "BoothNpc.h"
#include "MagicItem.h"
#include "DiceView.h"
#include "TerrainNpcView.h"
#include "Item.h"
#include "Physics.h"

const int _MAX_ROLEDATA		=2;
const int _DEFAULT_JUMP_HEIGH = 120;

DWORD g_dwCurTime = TimeGet() ;
// static member
int CRole::s_nShadowType = SHADOW_REAL;//SHADOW_SIMPLE;//CRole::SHADOW_REAL;//SHADOW_SIMPLE;
float CRole::s_fBaseScale = 0.75f;
//--------------------------------------------------------------
void CRole::Init(void)
{
	m_Info.Reset();
	this->InitCommand();

	m_Info.iSize = 1;
	m_Info.iAddSize = 0;
	m_Info.iZoomPercent = 100;
	m_Info.objPath.SetSearchMethod(_ASTAR_CLOSE);	
	m_pIRoleView	=NULL;
	m_dwStatus	=USERSTATUS_NORMAL;

	m_tmLastAtk		= 0;
	m_tmDisappear	= 0;
	m_dwRoleAttribute = 0;
	m_Info.nFlyOffset = 0;
	m_nWhichLeg = 0;
	m_Info.bAdjustRun = false;
	m_Info.ucSprite = 255 ;
	m_Info.dwSpeed = 100;
	m_setCmd.clear();
	m_dwDisappear = 0; 
	m_dwDisappearLast = 0;
	m_dwCommandIndex = 0;
}

//--------------------------------------------------------------
BOOL CRole::Create(UINT uLook, int nViewType)
{
	if(uLook%2 == 0)
		m_Info.nSex = 1;
	else
		m_Info.nSex = 2;
	if(uLook > 6)
		this->SetRace(RACE_NONE);
	else
		this->SetRace((uLook+1)/2);
	m_Info.usLook = uLook;
	m_nViewType = nViewType;

	this->Disappear(500);
				
	switch(nViewType)
	{
	case _ROLE_VIEW_ROLE:
		{
			if (m_pIRoleView)
				SAFE_DELETE(m_pIRoleView);
			C3DRole* pRole =(C3DRole* )C3DRole::CreateNewView();
			pRole->SetGame3DEffectEx(&m_objEffect);
			m_pIRoleView = pRole; 
			if (!m_pIRoleView)
				return false;
			m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		}
		return true;
	case _ROLE_VIEW_STATUARY:
		if (m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView	=C3DStatuary::CreateNewView();
		if (!m_pIRoleView)
			return false;
		m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		return true;
	case _ROLE_VIEW_TARGET:
		if (m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView = C3DTarget::CreateNewView();
		m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		return true;
	case _ROLE_VIEW_TASKNPC:
		if (m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView = C3DTaskNpc::CreateNewView();
		m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		return true;
	case _ROLE_VIEW_DICENPC:
		if (m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView = CDiceView::CreateNewView();
		m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		return true;
	case _ROLE_VIEW_SYNSTONE:
		if (m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView = CSynStone::CreateNewView();
		m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		return true;
	case _ROLE_VIEW_BOOTH:
		if (m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView = CBoothNpc::CreateNewView();
		m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		return true;
	case _ROLE_VIEW_MAPMAGICOBJ:
		if(m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView = CMagicItem::CreateNewView();
		m_pIRoleView->SetVariable(_VAR_LOOK, uLook);
		return true;
	default:
		return false;
	}
	return true;
}


//--------------------------------------------------------------

BOOL CRole::Create(UINT uLook, int nViewType, OBJID id, CMyPos pos)
{
	m_Info.usLook = uLook;
	m_nViewType = nViewType;
	switch(nViewType)
	{
	case _ROLE_VIEW_TERRAIN:
		if(m_pIRoleView)
			SAFE_DELETE(m_pIRoleView);
		m_pIRoleView = CTerrainNpcView::CreateNewView(uLook, id, pos);
		break;
	}
	this->Disappear(500);
	return true;
}

//--------------------------------------------------------------
void CRole::Destroy(void)
{
	if (m_pIRoleView)
		SAFE_DELETE(m_pIRoleView);

	m_objEffect.Clear();
	

	this->ClrAniEffect(NULL);
	int nAmount = m_setCmd.size();
	for(int i = 0; i < nAmount; i ++)
	{
		delete m_setCmd[i];
	}
	m_setCmd.clear();
	this->Init();
}

//--------------------------------------------------------------
void    CRole::SetFace(USHORT usFace)	
{
	m_Info.usFace=usFace;
	if(m_pIRoleView)
		m_pIRoleView->SetVariable(_VAR_HEAD, usFace);
}
//--------------------------------------------------------------
void CRole::AllotRes()
{
	if(!m_pIRoleView)
		m_pIRoleView = C3DRole::CreateNewView();
	MYASSERT(m_pIRoleView);
	this->SetLook(m_Info.usLook);

	this->Transform(m_Info.nTransformLook);

	this->SetHair(m_Info.usHair);

	this->SetArmor(m_Info.idArmorType);
	this->SetRWeapon(m_Info.idRWeaponType);
	this->SetLWeapon(m_Info.idLWeaponType);
	this->SetMount(m_Info.idMountType);
	this->SetArmet(m_Info.idArmetType);

}

//--------------------------------------------------------------
void CRole::SetLook(USHORT usLook)
{
	m_Info.usLook	=usLook;

	if(m_pIRoleView)
		m_pIRoleView->SetVariable(_VAR_LOOK, usLook);
}

//--------------------------------------------------------------
void CRole::SetHair(USHORT usHair)
{
//	if ((usHair%100) == 0)
//		usHair = 0;

	m_Info.usHair	=usHair;
	if(m_pIRoleView)
		m_pIRoleView->SetVariable(_VAR_HAIR, usHair);
}

//--------------------------------------------------------------
void CRole::InitCommand(void)
{
	m_Info.cmdProc.iType	=_COMMAND_STANDBY;
	m_Info.cmdProc.iStatus	=_CMDSTATUS_BEGIN;
	m_Info.cmdProc.posTarget.x	=0;
	m_Info.cmdProc.posTarget.y	=0;
}

//--------------------------------------------------------------
DWORD CRole::SetCommand(CCommand* pCommand)
{
	if (!pCommand)
		return 0;
	m_dwCommandIndex++;
	pCommand->dwIndex = m_dwCommandIndex;
	/*
	// reset action...
	if (_COMMAND_STANDBY != pCommand->iType)
	{
		// emotion, standby and action can be interrupted at any time.
		if (_COMMAND_STANDBY == m_Info.cmdProc.iType
				|| _COMMAND_EMOTION == m_Info.cmdProc.iType)
			this->ResetActionData();


		// cmd accomplished can be interrupted at andy time
		if (_CMDSTATUS_ACCOMPLISH == m_Info.cmdProc.iStatus)
		{
			if (_ACTION_STANDBY == m_Info.iActType 
					|| _ACTION_ALERT == m_Info.iActType			
					|| _ACTION_REST1 == m_Info.iActType
					|| _ACTION_REST2 == m_Info.iActType
					|| _ACTION_REST3 == m_Info.iActType)
				this->ResetActionData();
		}

		// die command can interrupt any command
		if (_COMMAND_DIE == pCommand->iType)
			this->ResetActionData(); 
	}
	*/

	// set command now
	if(pCommand->iType == _COMMAND_INTONE || pCommand->iType == _COMMAND_EMOTION || pCommand->iType == _COMMAND_ACTION)
	{
		if(((pCommand->nData == _ACTION_BRUISE0) || (pCommand->nData == _ACTION_DODGE0)) && (this->m_Info.cmdProc.iType == _COMMAND_EMOTION) && 
			(m_Info.cmdProc.nData == _ACTION_INTONE || m_Info.cmdProc.nData == _ACTION_INTONE_DURATION))
			return m_dwCommandIndex;
	}
	memcpy(&(m_Info.cmdProc), pCommand, sizeof(CCommand));
	return m_dwCommandIndex;
}

//--------------------------------------------------------------
int	CRole::GetCommandType(void)
{
	if (_CMDSTATUS_ACCOMPLISH == this->GetCommandStatus())
	{
		if(m_Info.nFlyOffset == 0)
			return _COMMAND_STANDBY;
		else
			return _COMMAND_FLOAT;
	}

	return m_Info.cmdProc.iType;
}

//--------------------------------------------------------------
DWORD CRole::GetFrameInterval(int nActType)
{
	DWORD dwMyIndex, dwInterval;
	if(m_pIRoleView)
	{
		dwMyIndex = this->GetLook()*1000000 + m_pIRoleView->GetVariable(_VAR_WEAPON_ACTION_DATA)*1000 + nActType;
		dwInterval = g_obj3DRoleData.GetActionTimeInterval(dwMyIndex);
	}
	else
	{
		dwInterval = 33;
	}

//	if (this->IsAttacking())
//	{
//		if (this->TestStatus(USERSTATUS_ATKSPEED) 
//			|| this->TestStatus(USERSTATUS_XPFULL)
			/*|| this->TestStatus(USERSTATUS_TORNADO))*/

//			dwInterval = dwInterval*2/3;
//	}
	/*else*/ if (this->IsMoving())
	{
		if(this->GetActionType() == _ACTION_JUMP)
		{
			// 取得跳跃距离
			double dbDicAll = sqrt(
				(m_Info.posActionBegin.x- m_Info.posActionEnd.x) * 
				(m_Info.posActionBegin.x - m_Info.posActionEnd.x) +
				(m_Info.posActionBegin.y - m_Info.posActionEnd.y) * 
				(m_Info.posActionBegin.y - m_Info.posActionEnd.y)
				);

			dwInterval = __max(33, (DWORD) (dwInterval*dbDicAll/300+1));
		}
		if(m_dwSpeedRate == 0)
			m_dwSpeedRate = 100;
		if(IsRunning())
		{
			if(this->TestStatus(USERSTATUS_TEAMLEADER)&&
				(
					this->TestStatus(USERSTATUS_TEAM_HEALTH)||
					this->TestStatus(USERSTATUS_TEAM_ATTACK)||
					this->TestStatus(USERSTATUS_TEAM_DEFENCE)||
					this->TestStatus(USERSTATUS_TEAM_SPEED)||
					this->TestStatus(USERSTATUS_TEAM_EXP)||
					this->TestStatus(USERSTATUS_TEAM_SPIRIT)||
					this->TestStatus(USERSTATUS_TEAM_CLEAN)
				)
			  )
				dwInterval = dwInterval*100/80;
			else
				dwInterval = dwInterval*100/m_dwSpeedRate;
		}
	}
	if(	this->GetActionType() == _ACTION_RUNL ||
		this->GetActionType() == _ACTION_RUNR)
		dwInterval = dwInterval*100/m_Info.dwSpeed;
//	if(this->TestStatus(USERSTATUS_XPFULL) && this->GetCommandType() == _COMMAND_LOCKATK)
//	{
//		dwInterval = dwInterval*1/2;
//	}
	if(dwInterval < 5)
		dwInterval = 5;
	return dwInterval; 
}
DWORD CRole::GetActionTotalTime(int nActType)
{   



	int Diraction= GetDir();//取方向

	DWORD dwMyIndex, dwInterval,dwRunaction;
	if(m_pIRoleView)
	{
		//  [11/2/2007]-----------Chancy添加-----begin--------------------------------------------------
		//	获得帧间隔时
		//	NPC的GetLook()为四位数，再*1000000,已经超过了DWORD的最大值，出错了，所以去掉最后一位，这一位
		//	其实是NPC的方向
		if( this->IsNpc() && !this ->IsMonster() )
		{
			dwMyIndex = this->GetLook() / 10 * 1000000 + m_pIRoleView->GetVariable(_VAR_WEAPON_ACTION_DATA)*1000 + nActType;
		}
		else
			//---------------------------------------end-------------------------------------------------------
		{
			//	if( m_Info.nTransformLook != 0 )
			//		dwMyIndex = dwMyIndex = m_Info.nTransformLook * 1000000 + 0 * 1000 + nActType;
			//	else
			dwMyIndex = this->GetLook()*1000000 + m_pIRoleView->GetVariable(_VAR_WEAPON_ACTION_DATA)*1000 + nActType;
		}
		//		dwInterval = g_obj3DRoleData.GetActionTimeInterval(dwMyIndex);	// 原系统调用 [THEONE 2008-8-21 23:20]
		dwInterval = _FRAMEINTERVAL;
	}
	else
	{
		dwInterval = 50;
	}
	if (this->IsMonster()&&this->IsMoving()&&this->TestStatus(USERSTATUS_SLOWDOWN1))//如果是中了减速状态,那么怪就行走与跑变慢
	{
		
		return 125 * m_Info.iActFrameAmount;  

	} 

	if (this->IsMoving())										//  [11/1/2007]--Chancy添加，在移动(走、跑)--
	{
		if(this->GetActionType() == _ACTION_JUMP &&this->IsMonster())//  [12/10/2007]--Chancy添加注释，跳
		{
			// 取得跳跃距离           gongfan改,跳只有怪有可以，其他基本上都没有了。
			double dbDicAll = sqrt((double)
				(m_Info.posActionBegin.x- m_Info.posActionEnd.x) * 
				(m_Info.posActionBegin.x - m_Info.posActionEnd.x) +
				(m_Info.posActionBegin.y - m_Info.posActionEnd.y) * 
				(m_Info.posActionBegin.y - m_Info.posActionEnd.y)
				);

			dwInterval = __max(50, (DWORD) (dwInterval*dbDicAll/300+1));
		}
	
	}

	if(dwInterval < 5)
		dwInterval = 5;

	if( this->IsPlayer() )		//人物走、跑不是固定    主要是这一块，人物的走和跑
	{
		if( _ACTION_RUNL == nActType   
			||_ACTION_RUNR == nActType 
			||_ACTION_WALKL == nActType 
			||_ACTION_WALKR == nActType )

		{	 

			{
				dwRunaction=dwInterval-5;  //除了左右跑动.其他动作都改成加快
				return dwRunaction * m_Info.iActFrameAmount;
			}
		}
		if(	_ACTION_ATTACK0 == nActType   
			||_ACTION_ATTACK1 == nActType 
			||_ACTION_ATTACK2 == nActType )
			return dwInterval * m_Info.iActFrameAmount;

	}

//	return 50 * m_Info.iActFrameAmount;		//  [1/14/2008]--默认值50
	return 30 * m_Info.iActFrameAmount;	
}
//--------------------------------------------------------------
void CRole::SetActionData(void)
{

	if( NULL == m_pIRoleView )
		return ;
	CMyPos posRole;
	g_objGameMap.World2Cell(m_Info.posWorld.x, m_Info.posWorld.y, posRole.x, posRole.y);

	BOOL bPlaySound = g_objGameMap.IsPosVisible(posRole, 4);

	m_Info.iActFrameIndex	=0;
	BOOL bAccelerate = false;
/*	if(this->TestStatus(USERSTATUS_FLY))
	{
		switch(m_Info.iActType)
		{
		case _ACTION_JUMP:
			m_Info.iActType = _ACTION_FLY_MOVE;
			break;
		case _ACTION_STANDBY:
			m_Info.iActType = _ACTION_FLY_STANDBY;
			break;
		case _ACTION_WALKL:
		case _ACTION_WALKR:
		case _ACTION_WALKR_I:
		case _ACTION_WALKL_I:
		case _ACTION_RUNL:
		case _ACTION_RUNR:
			bAccelerate = true;
			m_Info.iActType = _ACTION_FLY_MOVE;
			break;

		case _ACTION_ATTACK0:
		case _ACTION_ATTACK1:
		case _ACTION_ATTACK2:
			m_Info.iActType = _ACTION_FLY_ATTACK;
			break;
		case _ACTION_FLY_STANDBY:
		case _ACTION_FLY_ALERT:
		case _ACTION_FLY_MOVE:
		case _ACTION_FLY_ATTACK:
		case _ACTION_FLY_DOWN:
		case _ACTION_FLY_UP:
		case _ACTION_FLY_DIE:
		case _ACTION_FLY_WOUND:
		case _ACTION_BODY0:
			break;
		case _ACTION_BRUISE0:
			m_Info.iActType = _ACTION_FLY_WOUND;
			break;
		default:
			m_Info.iActType = _ACTION_FLY_STANDBY;
			break;
		}
	}
	else if(this->TestStatus(USERSTATUS_WOUND))
	{
		switch(m_Info.iActType)
		{
			case _ACTION_WALKL:
				m_Info.iActType = _ACTION_WALKL_I;
				break;
			case _ACTION_WALKR:
				m_Info.iActType = _ACTION_WALKR_I;
				break;
		}
	}
*/
	DWORD dwTerrain = g_objGameMap.GetTerrain(m_Info.posMap.x, m_Info.posMap.y);
	
	m_Info.iActFrameAmount	=m_pIRoleView->SetAction(m_Info.iActType, true, bPlaySound, m_Info.posWorld.x, m_Info.posWorld.y, true, dwTerrain);
	if(m_Info.cmdProc.nFrameEnd != 0 && m_Info.cmdProc.nFrameStart < m_Info.cmdProc.nFrameEnd)
	{
		m_Info.iActFrameAmount = m_Info.cmdProc.nFrameEnd - m_Info.cmdProc.nFrameStart;
		m_pIRoleView->SetVariable(_VAR_FRAME, m_Info.cmdProc.nFrameStart);
	}

	

	if(this->IsUseFrameInterval())
	{
		m_Info.timeFrameInterval = this->GetFrameInterval(m_Info.iActType);
		if(m_Info.cmdProc.nFrameInterval != 0)	
			m_Info.timeFrameInterval = m_Info.cmdProc.nFrameInterval;
		if(bAccelerate)
			m_Info.timeFrameInterval = m_Info.timeFrameInterval/4;
	}else
	{
			m_Info.timeActionTime = GetActionTotalTime(m_Info.iActType);
		if( m_Info.cmdProc.timeActionTime != 0  )
			m_Info.timeActionTime = m_Info.cmdProc.timeActionTime;
	}


	//设置第一帧开始的时间
//	m_Info.timeFirstFrame	= ::TimeGet();
	m_Info.timeFirstFrame = g_dwCurTime;
	// map 3d effect of action 
	int nTerrain = g_objGameMap.GetTerrain(m_Info.posMap);
	const ActionMapEffectInfo* pInfo = g_obj3DRoleData.GetActionMapEffect(m_Info.usLook, m_Info.iActType, nTerrain);
	if (pInfo && pInfo->nShowTime == 0)	// effect show at begin of action
	{
		if (pInfo->nType == 0) 
		{
			C3DMapEffect* pEffect = g_objGameMap.Add3DMapEffect(m_Info.posWorld, (char*)pInfo->szEffect);
			if(pEffect && pInfo->bRotate)
			{
				pEffect->SetDir(pInfo->bRotate);
			}
		}
		else if (pInfo->nType == 1) 
		{
			m_objEffect.Add((char*)pInfo->szEffect,pInfo->bRotate,this->GetDir(),this->GetLook());
		}
	}

	// 极品帽子的特效
	/*ItemTypeInfo infoItem;
	if(!CItem::s_ItemData.GetItemTypeInfo(this->GetArmet(), infoItem))
	{
		this->m_objEffect.Delete("elf");
		return;
	}
	if(infoItem.ucRequiredLevel < 67)
	{
		this->m_objEffect.Delete("elf");
		return;
	}
	if(infoItem.uID%10 != 9)
	{
		this->m_objEffect.Delete("elf");
		return;
	}
	if(infoItem.uID/10000 != 11)
	{
		this->m_objEffect.Delete("elf");
		return;
	}
	if((m_Info.iActType != _ACTION_STANDBY) && 
		(m_Info.iActType != _ACTION_REST1) && 
		(m_Info.iActType != _ACTION_REST2) &&
		(m_Info.iActType != _ACTION_REST3) &&
		(m_Info.iActType != _ACTION_ALERT) &&		
		(m_Info.iActType != _ACTION_FLY_STANDBY))
	{
		this->m_objEffect.Delete("elf");
		return;
	}
	if(this->m_objEffect.TestEffect("elf"))
		return;
	this->m_objEffect.Add("elf",false,0,this->GetLook());*/

}
BOOL  CRole::IsUseFrameInterval()
{
	if(m_Info.cmdProc.bUseFrameInterval)return TRUE;
	switch(m_Info.iActType)
	{
		case 700: //星魔剑
			{
				return true;
			}
		case 262://拾取
			{
				return true;
			}
	}
	return FALSE;
}
//--------------------------------------------------------------
void CRole::ResetActionData(void)
{
	if(m_Info.cmdProc.iType == _COMMAND_DIE)
		return;
	if ((this->GetID() == g_objHero.GetID() || this->IsMyPet())
			&& this->IsMoving())
		return;

	if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
		this->ProcessCommand();

	m_Info.iActFrameIndex	= m_Info.iActFrameAmount;
	m_Info.posWorld			= m_Info.posActionEnd;
	m_Info.nZ				= m_Info.nZ;
	g_objGameMap.World2Cell(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posMap.x, m_Info.posMap.y);
	g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
}

//--------------------------------------------------------------
// return true means this action is end
BOOL CRole::ProcessAction(void)
{	
	if(this->GetID() == g_objHero.GetID() && 
		m_Info.cmdProc.iType != _COMMAND_RUN)
	{
		g_objHero.m_nFastRunSetp = 0; 
	}
	if(m_Info.cmdProc.nFrameEnd != 0 && m_Info.cmdProc.nFrameStart < m_Info.cmdProc.nFrameEnd)
	{
		if(m_Info.iActFrameIndex >= m_Info.cmdProc.nFrameEnd)
			return true;
	}
	if(m_Info.iActFrameIndex >= m_Info.iActFrameAmount)
	{
		m_Info.bAdjustDir = false;
		return true;
	}

	DWORD timeCurrent	=::TimeGet();
	if(this->IsUseFrameInterval())
	{
			// get the current frame index
		
		m_Info.iActFrameIndex	=(timeCurrent-m_Info.timeFirstFrame+m_Info.timeFrameInterval/2)/m_Info.timeFrameInterval;
		if (m_Info.iActFrameIndex > m_Info.iActFrameAmount)
			m_Info.iActFrameIndex	=m_Info.iActFrameAmount;
	}else
	{
		 timeCurrent = g_dwCurTime ;
		if( m_Info.timeActionTime == 0 )
		{
			m_Info.timeActionTime = 1;
		}
		m_Info.iActFrameIndex = ( timeCurrent - m_Info.timeFirstFrame ) * m_Info.iActFrameAmount / m_Info.timeActionTime ;
		if (m_Info.iActFrameIndex > m_Info.iActFrameAmount)
			m_Info.iActFrameIndex	=m_Info.iActFrameAmount;
	}



	// set world pos
	m_Info.posWorld.x	=m_Info.posActionBegin.x;
	m_Info.posWorld.y	=m_Info.posActionBegin.y;


	m_Info.nZ = m_Info.nPosActionBeginZ;
	// now get the world pos of role
	int nDeltaX	=m_Info.posActionEnd.x-m_Info.posActionBegin.x;
	int nDeltaY	=m_Info.posActionEnd.y-m_Info.posActionBegin.y;
	int nDeltaZ =m_Info.nPosActionEndZ-m_Info.nPosActionBeginZ;

	if (nDeltaX != 0 || nDeltaY != 0)
	{
		DWORD timeCost	=__max(1, m_Info.timeFrameInterval*m_Info.iActFrameAmount);
		if(!this->IsUseFrameInterval())
		{
			 timeCost	=__max(1, m_Info.timeActionTime);//总共需要的时间
		}
		
		DWORD timePass	=__min(timeCost, timeCurrent-m_Info.timeFirstFrame);//已经过了多长时间(从开始表现第一帧时)
		
		DWORD dwKey = m_pIRoleView ? m_pIRoleView->GetVariable(_VAR_ACTION_DATA) : 0;
		CONST CActionCtrlInfo* pInfo = g_obj3DRoleData.GetActionCtrlInfo(dwKey);
		if (pInfo)
		{
			// 求当前的运动位移百分比
			int nTimePercent = __min(100, 100 * timePass / timeCost);
			int i = 0;
			for (; i < pInfo->nSection; i++)
			{
				if (nTimePercent < pInfo->nTimePercent[i])
					break;
			}

			if (i < pInfo->nSection)
			{
				// 求上一段的绝对时间
				int nPreSectionTimePercent = (i <= 0) ? 0 : pInfo->nTimePercent[i - 1];
				int nPreSectionTime = timeCost * nPreSectionTimePercent / 100;

				// 此段的间隔时间
				int nSectionTime = timeCost * pInfo->nTimePercent[i] / 100 - nPreSectionTime;

				// 当前时间在段时间中的百分比例, 注意加上0.5是为了4舍5入
				int nSectionPassTimePercent = (int)(0.5f + 100.0f * (timePass - nPreSectionTime) / __max(1, nSectionTime));
				if (nSectionPassTimePercent > 100)
					nSectionPassTimePercent = 100;

				
				// 上一段的位移百分比
				int nPreSectionMovePercent = (i <= 0) ? 0 : pInfo->nMovePercent[i - 1];

				// 求出当前时间的位移百分比
				int nMovePercent = nPreSectionMovePercent + (pInfo->nMovePercent[i] - nPreSectionMovePercent) * nSectionPassTimePercent / 100;


				// 得出当前世界坐标
				DOUBLE dbChangeX = 1.0 * nDeltaX * nMovePercent / 100;
				DOUBLE dbChangeY = 1.0 * nDeltaY * nMovePercent / 100;
				
				m_Info.posWorld.x = m_Info.posActionBegin.x + ::Double2Int(dbChangeX);
				m_Info.posWorld.y = m_Info.posActionBegin.y + ::Double2Int(dbChangeY);
			}
			else
			{
				m_Info.posWorld = m_Info.posActionEnd;
			}
		}
		else
		{
			DOUBLE dbChangeX	=1.0*nDeltaX*timePass/timeCost;
			DOUBLE dbChangeY	=1.0*nDeltaY*timePass/timeCost;

			m_Info.posWorld.x	=m_Info.posActionBegin.x + ::Double2Int(dbChangeX);
			m_Info.posWorld.y	=m_Info.posActionBegin.y + ::Double2Int(dbChangeY);
		}
		DOUBLE dbChangeZ	=1.0*nDeltaZ*timePass/timeCost;

		m_Info.nZ	=m_Info.nPosActionBeginZ + ::Double2Int(dbChangeZ);
		if(m_Info.nZ != 0)
			int fdafa=1;

	}
	else
	{
	}
	
	// get map pos according world pos
	g_objGameMap.World2Cell(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posMap.x, m_Info.posMap.y);

	// get screen pos of role
	g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);


	// shall we show the last frame of action?
	if(m_Info.cmdProc.nFrameEnd != 0 && m_Info.cmdProc.nFrameStart < m_Info.cmdProc.nFrameEnd
		&& m_Info.iActFrameIndex >= m_Info.cmdProc.nFrameEnd)
	{
		// map 3d effect of action 
		int nTerrain = g_objGameMap.GetTerrain(m_Info.posMap);
		const ActionMapEffectInfo* pInfo = g_obj3DRoleData.GetActionMapEffect(m_Info.usLook, m_Info.iActType, nTerrain);
		if (pInfo && pInfo->nShowTime == 1)	// effect show at end of action
		{
			if (pInfo->nType == 0) 
			{
				C3DMapEffect* pEffect = g_objGameMap.Add3DMapEffect(m_Info.posWorld, (char*)pInfo->szEffect);
				if(pEffect && pInfo->bRotate)
				{
					pEffect->SetDir(pInfo->bRotate);
				}
			}
			else if (pInfo->nType == 1) 
			{
				m_objEffect.Add((char*)pInfo->szEffect,pInfo->bRotate,this->GetDir(),this->GetLook());
			}
		}

		{
			m_Info.bAdjustDir = false;
			return true;
		}

	}
	else
	if(m_Info.iActFrameIndex >= m_Info.iActFrameAmount)
	{
		// map 3d effect of action 
		int nTerrain = g_objGameMap.GetTerrain(m_Info.posMap);
		const ActionMapEffectInfo* pInfo = g_obj3DRoleData.GetActionMapEffect(m_Info.usLook, m_Info.iActType, nTerrain);
		if (pInfo && pInfo->nShowTime == 1)	// effect show at end of action
		{
			if (pInfo->nType == 0) 
			{
				C3DMapEffect* pEffect = g_objGameMap.Add3DMapEffect(m_Info.posWorld, (char*)pInfo->szEffect);
				if(pEffect && pInfo->bRotate)
				{
					pEffect->SetDir(pInfo->bRotate);
				}
			}
			else if (pInfo->nType == 1) 
			{
				m_objEffect.Add((char*)pInfo->szEffect,pInfo->bRotate,this->GetDir(),this->GetLook());
			}
		}

		{
			m_Info.bAdjustDir = false;
			return true;
		}
	}
	else
		return false;
}

//--------------------------------------------------------------
// return true if accomplished the command processed
BOOL CRole::ProcessCommand(void)
{
	if (m_Info.cmdProc.iStatus == _CMDSTATUS_ACCOMPLISH)
	{
		// set default cmd
		if(m_Info.nFlyOffset == 0)
			m_Info.cmdProc.iType	= _COMMAND_STANDBY;
		else
			m_Info.cmdProc.iType	= _COMMAND_FLOAT;
		m_Info.cmdProc.iStatus  = _CMDSTATUS_BEGIN;
	}

	BOOL bAccomlished	=true;
	switch(m_Info.cmdProc.iType)
	{
	case _COMMAND_WALK:
		bAccomlished	=Walk();
		break;

	case _COMMAND_RUN:
		bAccomlished	=Run();
		break;

	case _COMMAND_JUMP:
		bAccomlished	=Jump();
		break;

	case _COMMAND_FOLLOW:
		bAccomlished	=Follow();
		break;

	case _COMMAND_EMOTION:
		bAccomlished	=Emotion();
		break;

	case _COMMAND_POSE:
		bAccomlished	=Pose();
		break;
	case _COMMAND_INTONE:
	case _COMMAND_ACTION:
		bAccomlished	=Action();
		break;

	case _COMMAND_ATTACK:
	case _COMMAND_SPATTACK:
		bAccomlished	=Attack();
		break;

	case _COMMAND_LOCKBOOTH:
		bAccomlished	=LockBooth();
		break;

	case _COMMAND_LOCKATK:
		bAccomlished	=LockAtk();
		break;

	case _COMMAND_BUMP:
		bAccomlished	=Bump();
		break;

	case _COMMAND_TEAMFOLLOW:
		bAccomlished	=TeamFollow();
		break;

	case _COMMAND_RUSHATK:
		bAccomlished	=RushAtk();
		break;

	case _COMMAND_DIE:
		bAccomlished	=Die();
		break;

	case _COMMAND_WALKFORWARD:
		bAccomlished	=WalkForward();
		break;

	case _COMMAND_RUNFORWARD:
		bAccomlished	=RunForward();
		break;

	case _COMMAND_PICKUP:
		bAccomlished	=PickUp();
		break;

/*	case _COMMAND_INTONE:
		bAccomlished	=InTone();
		break;
*/
	case _COMMAND_MINE:
		bAccomlished	=Mine();
		break;
	case _COMMAND_TAKEOFF:
		bAccomlished	=TakeOff();
		break;
	case _COMMAND_LANDING:
		bAccomlished	=Landing();
		break;
	case _COMMAND_FLOAT:
		bAccomlished	=Float();
		break;
	case _COMMAND_KONGFU:
		bAccomlished	=Kongfu();
		break;
	default:
		bAccomlished	=StandBy();
		break;

	}

	/*
	if (bAccomlished)
		this->ClrRushPos();
	*/

	return bAccomlished;
}

//--------------------------------------------------------------
void CRole::Process(void* pInfo)
{
	this->ProcessAniEffect();

	if (this->ProcessAction())
	{
		this->ProcessCommand();
		this->SetActionData();
	}

	if (this->GetActionType() == _ACTION_WALKL ||
			this->GetActionType() == _ACTION_WALKR ||
				this->GetActionType() == _ACTION_RUNL ||
					this->GetActionType() == _ACTION_RUNR)
	{
		CMyPos posCurrent = m_Info.posMap;
		CMyPos posPreStep;
		int nDir = this->GetDir();
		if(nDir > 3)
			g_objGameMap.World2Cell(m_Info.posActionEnd.x, m_Info.posActionEnd.y, posPreStep.x, posPreStep.y);
		else
			g_objGameMap.World2Cell(m_Info.posActionBegin.x, m_Info.posActionBegin.y, posPreStep.x, posPreStep.y);

		m_Info.posMap = posPreStep;

		if (g_objGameMap.IsPosVisible(m_Info.posMap, 2))
			g_objGameMap.ApplyObjShow(this);

		m_Info.posMap = posCurrent;
	}
	else
	{
		if (g_objGameMap.IsPosVisible(m_Info.posMap, 2))
			g_objGameMap.ApplyObjShow(this);
	}

	m_objFStr.Process();
}

//--------------------------------------------------------------
void CRole::Show()
{
	if(m_dwDisappear != 0 && (::TimeGet() - m_dwDisappear) < m_dwDisappearLast && m_dwDisappearLast != 0)
	{
		int nOffsetY = 0;
		nOffsetY += m_Info.nFlyOffset;
		int nBgX, nBgY;
		g_objGameMap.World2Bg(m_Info.posWorld.x, m_Info.posWorld.y, nBgX, nBgY);
		CMyPos posWorld;
		posWorld.x = nBgX;
		posWorld.y = nBgY-nOffsetY;
		m_objEffect.Show(posWorld);
		return;
	}
	if(!m_pIRoleView) return;
	if ((m_dwStatus & USERSTATUS_INVISIBLE) == USERSTATUS_INVISIBLE)
		return;

	if (!m_pIRoleView)
		return;

	{
		int nBgX, nBgY;
		g_objGameMap.World2Bg(m_Info.posWorld.x, m_Info.posWorld.y, nBgX, nBgY);

		int nOffsetY = 0;
		int nShadowY = 0;
		switch(this->GetActionType())
		{
		case _ACTION_JUMP:
			{
				nOffsetY = this->GetJumpHeight();
				nShadowY = this->GetJumpShadowHeight();
			}
			break;
		case _ACTION_WALKL:
		case _ACTION_WALKR:
		case _ACTION_RUNL:
		case _ACTION_RUNR:
			{
			
			}
			break;
		default:
			break;
		}
		nOffsetY += m_Info.nFlyOffset;
		nShadowY -= m_Info.nFlyOffset;
		int nRotate	= -45*m_Info.iDirection-45;
		if(m_Info.bAdjustDir && (_ACTION_RUNL == m_Info.iActType || _ACTION_RUNR == m_Info.iActType))
		{
			int nDeltaX = (m_Info.posActionEnd.x - m_Info.posDir.x);
			int nDeltaY = (m_Info.posActionEnd.y - m_Info.posDir.y);
			double dAtan = atan2((double)nDeltaX, (double)nDeltaY);
			nRotate = (int)((180.0 * dAtan)/3.1415926 + 90);
		}
		if (m_Info.bAdjustDir &&(m_Info.posActionBegin.x != m_Info.posDir.x 
			|| m_Info.posActionBegin.y != m_Info.posDir.y))
		{
			if (_ACTION_RUNL != m_Info.iActType 
				&& _ACTION_RUNR != m_Info.iActType && 
				_ACTION_WALKL != m_Info.iActType
				&& _ACTION_WALKR != m_Info.iActType)
			{
				int nDeltaX = -(m_Info.posActionEnd.x - m_Info.posDir.x);
				int nDeltaY = m_Info.posActionEnd.y - m_Info.posDir.y;
				
				double dAtan = atan2((double)nDeltaY, (double)nDeltaX);
				nRotate = (int)((180.0 * dAtan)/3.1415926 + 90);
			}
			
			if(_ACTION_RUNL == m_Info.iActType 
				|| _ACTION_RUNR == m_Info.iActType)
			{
				int nDeltaX = -(m_Info.posActionBegin.x - m_Info.posDir.x);
				int nDeltaY = m_Info.posActionBegin.y - m_Info.posDir.y;
				
				double dAtan = atan2((double)nDeltaY, (double)nDeltaX);
				nRotate =(int)((180.0 * dAtan)/3.1415926 + 90);
			}
		}
		
		float fScale = s_fBaseScale*this->GetZoomPercent()/100.0f;
		static float rscale = s_fBaseScale;
		/*if(this->GetLook() == 1 && this->TestStatus(USERSTATUS_ATTACK))
		{
			if ( rscale < 0.85)
				rscale += 0.01f;
			else
				rscale = 0.85f;
			fScale = rscale*this->GetZoomPercent()/100.0f;
		}
		if (this->GetLook() == 1&& rscale > s_fBaseScale && !this->TestStatus(USERSTATUS_ATTACK)) 
		{
			if (rscale > s_fBaseScale) 
				rscale -= 0.01f;
			else
				rscale = s_fBaseScale;
			fScale = rscale*this->GetZoomPercent()/100.0f;
		}*/

		if(this->GetCommandType() == _COMMAND_INTONE)
			nShadowY = -m_Info.nZ;
		else 
			m_Info.nZ = 0;
		m_pIRoleView->SetPos(nBgX, nBgY-nOffsetY, nShadowY, nRotate, fScale);

		// shadow...
//		s_nShadowType = SHADOW_REAL;
		if (s_nShadowType == SHADOW_SIMPLE && 
				!this->TestStatus(USERSTATUS_GHOST) && 
				this->GetViewType() != _ROLE_VIEW_TERRAIN)
		{
			CAni* pAni = g_objGameDataSet.GetDataAni("ani/common.ani", "SimpleShadow");
			if (pAni)
			{
				int nShowX = 0, nShowY =0;
				g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, nShowX, nShowY);

				CMyBitmap* pBmp = pAni->GetFrame(0);
				if (pBmp)
				{
					int nAlpha = 255;
					if (m_tmDisappear != 0)
					{
						DWORD dwTimeGoesBy = ::TimeGet()-m_tmDisappear;
						nAlpha = 256-50*dwTimeGoesBy/1000;
						if (nAlpha < 0)
							nAlpha = 0;
					}
					CMyPos posShadow = {nShowX-32, nShowY-16-nShadowY-nOffsetY};
					g_objGameMap.MapScaleShowPos(posShadow.x, posShadow.y);
					CMySize sizeShadow;
					pBmp->GetSize(sizeShadow.iWidth, sizeShadow.iHeight);
					sizeShadow.iWidth = sizeShadow.iWidth*g_objGameMap.GetScale()/_NORMAL_SCALE;
					sizeShadow.iHeight = sizeShadow.iHeight*g_objGameMap.GetScale()/_NORMAL_SCALE;
					pBmp->ShowEx(posShadow.x, posShadow.y, NULL, sizeShadow.iWidth, sizeShadow.iHeight, nAlpha);
				}
			}
		}

		CMyPos posView;
		g_objGameMap.GetViewPos(posView);
	
		int nViewportBgX, nViewportBgY;
		g_objGameMap.World2Bg(posView.x, posView.y, nViewportBgX, nViewportBgY);
		
		m_pIRoleView->SetAction(this->GetActionType(), false);
		m_pIRoleView->SetVariable(_VAR_STATUS, (DWORD)this->GetStatus());
		m_pIRoleView->SetVariable(_VAR_FRAME, m_Info.iActFrameIndex);


		float alpha			= 1.0f;
		float alphaShadow	= 0.8f;

		if (m_tmDisappear != 0)
		{
			DWORD dwTimeGoesBy = ::TimeGet()-m_tmDisappear;

			alpha = (float)(1.0-0.2*dwTimeGoesBy/1000);
			if (alpha < 0.0)
				alpha = 0.0;

			alphaShadow = (float)(0.8-0.15*dwTimeGoesBy/1000);
			if (alphaShadow < 0.0)
				alphaShadow = 0.0;
		}

		union
		{
			DWORD dwColor;
			struct
			{
				unsigned char ucAlpha;
				unsigned char ucRed;
				unsigned char ucGreen;
				unsigned char ucBlue;
			}InfoColor;
		};
		dwColor = g_objGameMap.GetARGB();
		float fColor = (float)(0.1 * (InfoColor.ucRed + InfoColor.ucGreen + InfoColor.ucBlue)/(255*3));

		if (this->TestStatus(USERSTATUS_GHOST) && !this->IsPet())
			m_pIRoleView->SetLightOffset(SHADOW_NONE, 300, -300, -1000, 0.8f, fColor, fColor, fColor);
		else
			m_pIRoleView->SetLightOffset(CRole::s_nShadowType, 300, -300, -1000, alphaShadow, fColor, fColor, fColor);


		unsigned char ucA, ucR, ucG, ucB;
			
		ucA = m_dwARGB >> 24;
		ucR = (unsigned char)((m_dwARGB & 0x00ff0000) >> 16);
		ucG = (unsigned char)((m_dwARGB & 0x0000ff00) >> 8);
		ucB = (unsigned char)(m_dwARGB & 0x000000ff);
		if ((this->TestStatus(USERSTATUS_GHOST) || this->TestStatus(USERSTATUS_LURKER)) && !this->IsPet())
			m_pIRoleView->SetRGBA((float)(1.0*ucA/(255*alpha*2)), (float)(1.0*ucR/255), (float)(1.0*ucG/255),(float)(1.0*ucB/255));
		else
			m_pIRoleView->SetRGBA((float)(1.0*ucA/255*alpha), (float)(1.0*ucR/255), (float)(1.0*ucG/255), (float)(1.0*ucB/255));
		

		m_pIRoleView->Draw(nViewportBgX, nViewportBgY); //画角色-- 

		// show effect
		this->ShowAniEffect(m_Info.posScr.x, m_Info.posScr.y-nOffsetY);

		// show dialog ...
		int nDialogDir = this->GetDir();
		if(m_Info.posScr.x < 100)
			nDialogDir = 1;

		if(m_Info.posScr.x > _SCR_WIDTH - 100)
			nDialogDir = 5;

		g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
		m_objSayHello.ShowWords(m_Info.posScr.x, m_Info.posScr.y-nOffsetY, nDialogDir);

		CMyPos posFStr;
		posFStr.x = m_Info.posScr.x;
		posFStr.y = m_Info.posScr.y-nOffsetY-100;
		m_objFStr.Show(posFStr);


		CMyPos posWorld;
		posWorld.x = nBgX;
		posWorld.y = nBgY-nOffsetY;		

		// Test code
		int test = 0;
		if (test == 1)
		if ( this->GetID() == g_objHero.GetID() )
		{
			posWorld.y -= 100;
			
			static BOOL s_bOK = true;
			static CMyPos s_posEnd = posWorld;
			static CMyPos s_posBegin;
			static float s_fRadii;
			static DWORD s_dwBeginTime;
			static float s_fTime;
			static BOOL s_bDir;
			
			// Get a random position
			if ( s_bOK )
			{
				s_posBegin = s_posEnd;
				
				int nRandX = ::RandGet( 60 );
				int nRandY = ::RandGet( 60 );
				switch ( ::RandGet( 4 ) )
				{
				case 0:
					s_posEnd.x = posWorld.x + nRandX;
					s_posEnd.y = posWorld.y + nRandY;
					break;
				case 1:
					s_posEnd.x = posWorld.x - nRandX;
					s_posEnd.y = posWorld.y - nRandY;
					break;
				case 2:
					s_posEnd.x = posWorld.x + nRandX;
					s_posEnd.y = posWorld.y - nRandY;
					break;
				case 3:
					s_posEnd.x = posWorld.x - nRandX;
					s_posEnd.y = posWorld.y + nRandY;
					break;
				default:
					break;
				}
				
				s_fRadii = (float)(( 60 + ::RandGet( 60 ) ) / 100);
				s_dwBeginTime = ::TimeGet();
				s_bDir = ::TimeGet() % 2;
				s_bOK = false;
				posWorld = s_posBegin;
			}
			
			s_fTime = ( ::TimeGet() - s_dwBeginTime ) * 3.0f / 100;
			if ( s_fTime != 0 )
				s_bOK = CPhysics::S2DCMGetPos( 30, 0, s_fTime, s_fRadii, s_posBegin, s_posEnd, posWorld, s_bDir );
		}
		if(this->GetViewType() != _ROLE_VIEW_ROLE)
			m_objEffect.Show(posWorld);
		
	}
	//CMyBitmap::ClearBuffer(true, false, 0);
}

//--------------------------------------------------------------
int CRole::GetWalkHeight()
{
	CMyPos posBegin, posEnd;
	g_objGameMap.World2Cell(m_Info.posActionBegin.x, m_Info.posActionBegin.y, 
								posBegin.x, posBegin.y);
	g_objGameMap.World2Cell(m_Info.posActionEnd.x, m_Info.posActionEnd.y, 
								posEnd.x, posEnd.y);
	CellInfo* pCellBegin = g_objGameMap.GetCell(posBegin.x, posBegin.y);
	CellInfo* pCellEnd = g_objGameMap.GetCell(posEnd.x, posEnd.y);
	
	int nOffsetY = 0;
	if(pCellBegin && pCellEnd)
	{

		int nHeightDef = g_objGameMap.GetGroundAltitude(pCellBegin) -
						 g_objGameMap.GetBaseAltitude(posBegin) -
						 g_objGameMap.GetGroundAltitude(pCellEnd) +
						 g_objGameMap.GetBaseAltitude(posEnd);
		
		double dbDicAll = sqrt(
			(m_Info.posActionBegin.x- m_Info.posActionEnd.x) * 
			(m_Info.posActionBegin.x - m_Info.posActionEnd.x) +
			(m_Info.posActionBegin.y - m_Info.posActionEnd.y) * 
			(m_Info.posActionBegin.y - m_Info.posActionEnd.y)
			);
		CMyPos posWorld; 
		this->GetWorldPos(posWorld);
		double dbDicNow = sqrt(
			(m_Info.posActionBegin.x- posWorld.x) * 
			(m_Info.posActionBegin.x - posWorld.x) +
			(m_Info.posActionBegin.y - posWorld.y) * 
			(m_Info.posActionBegin.y - posWorld.y)
			);
		double dbOffsetY;
		dbOffsetY = nHeightDef * dbDicNow / dbDicAll; 
		nOffsetY = (int)dbOffsetY;
	}
	int nAltitude = g_objGameMap.GetGroundAltitude(pCellBegin)-g_objGameMap.GetBaseAltitude(posBegin) - nOffsetY;
	return nAltitude;
}
//--------------------------------------------------------------

int	CRole::GetJumpHeight()
{
	DEQUE_CELL setCell;
	CMyPos posHighest;

	CMyPos posBegin, posEnd;
	g_objGameMap.World2Cell(m_Info.posActionBegin.x, m_Info.posActionBegin.y, 
								posBegin.x, posBegin.y);
	g_objGameMap.World2Cell(m_Info.posActionEnd.x, m_Info.posActionEnd.y, 
								posEnd.x, posEnd.y);


	g_objGameMap.Sampling(setCell, posHighest, posBegin, posEnd, 32);

	// Get the highest altitude
	CellInfo* pBeginCell = g_objGameMap.GetCell(posBegin.x, posBegin.y);
	if(!pBeginCell)
		return 0;

	CellInfo* pEndCell = g_objGameMap.GetCell(posEnd.x, posEnd.y);
	if(!pEndCell)
		return 0;

	CellInfo* pHighestCell = g_objGameMap.GetCell(posHighest.x, posHighest.y);
	if(!pHighestCell)
		return 0;


	// 取三个点
	int nHightestAltitude = g_objGameMap.GetGroundAltitude(pHighestCell) - g_objGameMap.GetBaseAltitude(posHighest);

	int nBeginAltitude = g_objGameMap.GetGroundAltitude(pBeginCell) - g_objGameMap.GetBaseAltitude(posBegin);
	
	int nEndAltitude = g_objGameMap.GetGroundAltitude(pEndCell) - g_objGameMap.GetBaseAltitude(posEnd);
	
	// 判断有没有最高点或者最高点小于缺省高度, 取中间点为最高点, 
	BOOL bHaveHighestPoint = false;
	if(nHightestAltitude > nBeginAltitude)
		bHaveHighestPoint = true;



	// 计算最高点的世界坐标
	CMyPos posHighestWorldPos, posBeginWorldPos, posEndWorldPos;
	posBeginWorldPos = m_Info.posActionBegin;
	posEndWorldPos = m_Info.posActionEnd;
	if(bHaveHighestPoint)
	{
		g_objGameMap.Cell2World(posHighest.x, posHighest.y, posHighestWorldPos.x, posHighestWorldPos.y);
	}

	nHightestAltitude = nHightestAltitude + (int)(sqrt((posBeginWorldPos.x - posEndWorldPos.x) * (posBeginWorldPos.x - posEndWorldPos.x) +
			(posBeginWorldPos.y - posEndWorldPos.y) * (posBeginWorldPos.y - posEndWorldPos.y))/3);

	posHighestWorldPos.x = (posBeginWorldPos.x + posEndWorldPos.x)/2;
	posHighestWorldPos.y = (posBeginWorldPos.y + posEndWorldPos.y)/2;

	nHightestAltitude += 20;
	// 上升阶段还是下降阶段
	BOOL beUp = false;
	CMyPos posCurrentWorld;
	this->GetWorldPos(posCurrentWorld);

	DWORD dwB2N = (posBeginWorldPos.x - posCurrentWorld.x) * 
					(posBeginWorldPos.x - posCurrentWorld.x) + 
					(posBeginWorldPos.y - posCurrentWorld.y) * 
					(posBeginWorldPos.y - posCurrentWorld.y);

	DWORD dwB2H = (posBeginWorldPos.x - posHighestWorldPos.x) * 
					(posBeginWorldPos.x - posHighestWorldPos.x) + 
					(posBeginWorldPos.y - posHighestWorldPos.y) * 
					(posBeginWorldPos.y - posHighestWorldPos.y);
	if(dwB2H > dwB2N)
		beUp = true;

	// 计算高度
	int nHeight = 0;
	if(beUp)
	{
		int nHeightDef =  nHightestAltitude - nBeginAltitude;
		double dbDisNow = sqrt((posBeginWorldPos.x - posCurrentWorld.x) * (posBeginWorldPos.x - posCurrentWorld.x) +
 					   (posBeginWorldPos.y - posCurrentWorld.y) * (posBeginWorldPos.y - posCurrentWorld.y));
		double dbDisEnd = sqrt((posBeginWorldPos.x - posHighestWorldPos.x) * (posBeginWorldPos.x - posHighestWorldPos.x) +
 					   (posBeginWorldPos.y - posHighestWorldPos.y) * (posBeginWorldPos.y - posHighestWorldPos.y));
		if(dbDisEnd == 0)
			dbDisEnd = 1;
		double pi = 3.1415926535;
		double dbAngle = pi*dbDisNow/dbDisEnd/2;
		nHeight = nBeginAltitude + (int)(nHeightDef*sin(dbAngle));

	}
	else
	{
		int nHeightDef =  nHightestAltitude - nEndAltitude;
		double dbDisNow = sqrt((posEndWorldPos.x - posCurrentWorld.x) * (posEndWorldPos.x - posCurrentWorld.x) +
 					   (posEndWorldPos.y - posCurrentWorld.y) * (posEndWorldPos.y - posCurrentWorld.y));
		double dbDisEnd = sqrt((posEndWorldPos.x - posHighestWorldPos.x) * (posEndWorldPos.x - posHighestWorldPos.x) +
 					   (posEndWorldPos.y - posHighestWorldPos.y) * (posEndWorldPos.y - posHighestWorldPos.y));
		if(dbDisEnd == 0)
			dbDisEnd = 1;

		double pi = 3.1415926535;
		double dbAngle = pi*dbDisNow/dbDisEnd/2;
		nHeight = nEndAltitude + (int)(nHeightDef*sin(dbAngle));
	}
			
	return nHeight;	
}

//--------------------------------------------------------------
int CRole::GetJumpShadowHeight()
{
	int nHeight = this->GetJumpHeight();
	CMyPos posCell;
	g_objGameMap.World2Cell(m_Info.posWorld.x, m_Info.posWorld.y,
							posCell.x, posCell.y);
	CellInfo* pCellInfo = g_objGameMap.GetCell(posCell.x, posCell.y);
	if(!pCellInfo)
		return 0;	
	int nJumpShadowHeight = g_objGameMap.GetGroundAltitude(pCellInfo) - g_objGameMap.GetBaseAltitude(posCell) - nHeight;
	if(nJumpShadowHeight > 0)
		nJumpShadowHeight = 0;
	return nJumpShadowHeight;
}
//--------------------------------------------------------------
DWORD CRole::GetJumpFrameInterval()
{
	// 取得跳跃距离
	double dbDicAll = sqrt(
		(m_Info.posActionBegin.x- m_Info.posActionEnd.x) * 
		(m_Info.posActionBegin.x - m_Info.posActionEnd.x) +
		(m_Info.posActionBegin.y - m_Info.posActionEnd.y) * 
		(m_Info.posActionBegin.y - m_Info.posActionEnd.y)
		);
	// 取得标准帧间隔
	DWORD dwInteraval = this->GetFrameInterval(_ACTION_JUMP);
	DWORD dwMyInteraval = (DWORD) (dwInteraval*dbDicAll/300+1);
	if(dwMyInteraval < 36)
			dwMyInteraval = 36;
	return dwMyInteraval;
}
//--------------------------------------------------------------
int	CRole::GetOffsetY()
{
	int nOffsetY = 0;
	switch(this->GetActionType())
	{
	case _ACTION_JUMP:
		{
			nOffsetY = this->GetJumpHeight();
		}
		break;
	case _ACTION_WALKL:
	case _ACTION_WALKR:
	case _ACTION_RUNL:
	case _ACTION_RUNR:
		{
			nOffsetY = this->GetWalkHeight();
		}
		break;
	default:
		nOffsetY = g_objGameMap.GetAltitude(m_Info.posMap)- g_objGameMap.GetBaseAltitude(m_Info.posMap);
		break;
	}
	return nOffsetY;
}
//--------------------------------------------------------------
void CRole::AddFStr(char* pszStr, int nMaxHeight, DWORD dwColor, int nStepHeight, int nDelayTime)
{
	m_objFStr.AddStr(pszStr, nMaxHeight, dwColor, nStepHeight , nDelayTime);
}
//--------------------------------------------------------------
void CRole::AddZoomNum(int nNum, DWORD dwColor, DWORD dwDelay, int nRandOffset)
{	
	if (nNum > 0) 
		m_objZoomNum.Add(nNum, dwColor, dwDelay, nRandOffset);
	else
	{
		if (this->IsPlayer ()) 
			m_objEffect.Add("PMISS",false,0,this->GetLook());
		else 
			m_objEffect.Add("MISS",false,0,this->GetLook());
	}
}
//--------------------------------------------------------------
void CRole::Transform(int nIndex)
{
	CTransformInfo* pInfo = g_obj3DRoleData.GetTransformInfo(nIndex);
	if(!pInfo)
		return;
	if(!this->IsNpc())
	{
		this->SetAddSize(pInfo->nAddSize);
		this->SetZoomPercent(pInfo->nScale);
	}
	m_Info.nTransformLook = pInfo->nLook;
	if(m_pIRoleView)
		m_pIRoleView->SetVariable(_VAR_TRANSFORM, pInfo->nLook);
}
//--------------------------------------------------------------
void CRole::ShootArrow(OBJID idTarget, int nHpLose)
{
	CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idTarget, true);
	if(!pPlayer)
		return;
	CMyPos posTarget;
	pPlayer->GetPos(posTarget);

	CCommand cmd;
	cmd.iType		=_COMMAND_ATTACK;
	cmd.iStatus		=_CMDSTATUS_BEGIN;
	cmd.idTarget	=idTarget;
	cmd.dwData		=nHpLose;
	cmd.nDir	=	this->GetDir(posTarget);
	this->SetCommand(&cmd);
	CMagicEffect* pMagicEffect = CMagicEffect::CreateNewArrow(this->GetID(), idTarget, 500000);
	g_objGameMap.AddMagicEffect(pMagicEffect);
}
//--------------------------------------------------------------
void CRole::SetDir(int nDir)
{
/*	if(this->GetID() == g_objHero.GetID() && nDir%8 == 4)
		int kk = 0;
*/
	m_Info.iDirection=nDir%8;
	if(m_Info.cmdProc.iType == _COMMAND_EMOTION)
		m_Info.cmdProc.nDir = m_Info.iDirection;
}
//--------------------------------------------------------------
void CRole::SetFlyOffset(int nOffset)
{
	m_Info.nFlyOffset = nOffset;
}
//--------------------------------------------------------------
int	CRole::GetFlyOffset()
{
	return m_Info.nFlyOffset;
}
//--------------------------------------------------------------
void CRole::SetFlyOffsetGoal(int nOffset)
{
	m_Info.nFlyOffsetGoal = nOffset;
}
//--------------------------------------------------------------
int	CRole::GetFlyOffsetGoal()
{
	return m_Info.nFlyOffsetGoal; 
}
//--------------------------------------------------------------
BOOL CRole::HaveBow()
{
	if(this->m_pIRoleView->GetVariable(_VAR_LOOKCLONE) != 0)
		return false;
	if(this->GetRWeapon()/1000%1000 == 500 || this->GetRWeapon()/1000%1000 == 501)
		return true;
	else
		return false;
}
//--------------------------------------------------------------
BOOL	CRole::IsGuard(void)
{
	if (this->IsNpc() && this->GetMonsterTypeId() >= 100 && this->GetMonsterTypeId() <= 199)
		return true;
	return false;
}
//--------------------------------------------------------------
BOOL	CRole::IsMyPet(void)
{
	if(!this->IsPet())
		return false;
	if(g_objHero.CheckPet(this->GetID()))
		return true;
	return false;
}
//--------------------------------------------------------------
BOOL	CRole::IsHypersensitive()
{
	int nRoleType = this->GetRoleType();
	if(nRoleType == _ROLE_SHOPKEEPER_NPC || 
		nRoleType == _ROLE_TASK_NPC ||
		nRoleType == _ROLE_STORAGE_NPC ||
		nRoleType == _ROLE_TRUNCK_NPC ||
		nRoleType == _ROLE_FACE_NPC ||
		nRoleType == _ROLE_FORGE_NPC ||
		nRoleType == _ROLE_EMBED_NPC ||
		nRoleType == _ROLE_STATUARY_NPC ||
		nRoleType == _ROLE_SYNFLAG_NPC ||
		nRoleType == _ROLE_DICE_NPC
		)
	{
		return true;
	}
	return false;
}
//--------------------------------------------------------------

void CRole::SetSpriteEffect(UCHAR ucSprite)
{
	if ( ucSprite == 255 )
	// not sprite, test if the player unequip it
	{
		// The player had a sprite delete the effect
		if ( m_Info.ucSprite != 255 )
		{
			char strName[128] ;
			sprintf( strName, "Sprite_%u", m_Info.ucSprite ) ;
			if ( m_objEffect.TestEffect( strName ) )
			{
				m_objEffect.Delete( strName ) ;
			}
			m_Info.ucSprite = 255 ;
		}
	}
	else
	{
		// Not sprite yet,just add it
		if ( m_Info.ucSprite == 255 )
		{
			m_Info.ucSprite = ucSprite ;
			char strName[128] ;
			sprintf( strName, "Sprite_%u", m_Info.ucSprite ) ;
			m_objEffect.Add( strName ,false,0,this->GetLook()) ;
		}
		else
		{
			// The sprite changes, del the old one and add the new one
			if ( m_Info.ucSprite != ucSprite )
			{
				char strName[128] ;
				sprintf( strName, "Sprite_%u", m_Info.ucSprite ) ;
				if ( m_objEffect.TestEffect( strName ) )
				{
					m_objEffect.Delete( strName ) ;
				}

				m_Info.ucSprite = ucSprite ;
				sprintf( strName, "Sprite_%u", m_Info.ucSprite ) ;
				m_objEffect.Add( strName,false,0,this->GetLook() ) ;
			}
		}
	}
}

//--------------------------------------------------------------
BOOL CRole::LoadKongfu(const char* pszFileName)	
{
	if(!pszFileName)
		return false;
	FILE* fp = fopen(pszFileName, "rb");
	if(!fp)
		return false;
	m_setKongfu.clear();
	char szHead[16]="NDKONGFU";
	fread(szHead, sizeof(char), 16, fp);
	DWORD dwVersion = 1000;
	fread(&dwVersion, sizeof(DWORD), 1, fp);
	DWORD dwFormat = 0;
	fread(&dwFormat, sizeof(DWORD), 1, fp);
	DWORD dwActionAmount = 0;
	fread(&dwActionAmount, sizeof(DWORD), 1, fp);
	for(DWORD i = 0; i  < dwActionAmount; i ++)	
	{
		KongfuAction actionKongfu;
		fread(&actionKongfu.cDir, sizeof(char), 1, fp);
		fread(&actionKongfu.dwFrameInterval, sizeof(DWORD), 1, fp);
		
		int nSize = 0;
		fread(&nSize, sizeof(int), 1, fp);
		fread(&actionKongfu.szEffect, sizeof(char), nSize, fp);
		fread(&actionKongfu.nAction, sizeof(int), 1, fp);
		int nStepAmount = 0;
		fread(&nStepAmount, sizeof(int), 1, fp);
		for(int j = 0; j < nStepAmount; j ++)
		{
			char cStep;
			fread(&cStep, sizeof(char), 1, fp);
			actionKongfu.setStep.push_back(cStep);
		}
		m_setKongfu.push_back(actionKongfu);
	}
	fclose(fp);
	return true;
}
//--------------------------------------------------------------
void CRole::Disappear(DWORD dwLast)
{
	m_dwDisappear = ::TimeGet();
	m_dwDisappearLast = dwLast;
}
//--------------------------------------------------------------
void CRole::AddBeAttackCommandInfo(OBJID idSender, int nSenderCommandIndex, int nAction, int nFrameIndex, int nLife, DWORD dwColor)
{
	BeAttackCmdInfo* pInfo = new BeAttackCmdInfo;
	if(pInfo)
	{
		pInfo->idPlayer = idSender;
		pInfo->dwSenderCommandIndex = nSenderCommandIndex;
		pInfo->nAction = nAction;
		pInfo->nFrameIndex = nFrameIndex;
		pInfo->nLife = nLife;
		pInfo->dwColor = dwColor;
		m_setBeAttackCmdInfo.push_back(pInfo);
	}
}
//--------------------------------------------------------------
void CRole::PerformBeAttackCmd(BeAttackCmdInfo* pInfo)
{
	if(!pInfo)
		return;
	// target show
	CPlayer* pTarget = (CPlayer*)this;
	int nDir = pTarget->GetDir();
	CPlayer* pSender = g_objPlayerSet.GetPlayer(pInfo->idPlayer, true);
	if(pSender)
		nDir = pTarget->GetDir(pSender);
	DWORD dwColor = _COLOR_YELLOW;
	if (pTarget->GetID() == g_objHero.GetID())
	{
		if (pInfo->nLife > 0)
		{
			dwColor = _COLOR_RED;
			g_objHero.AddZoomNum(pInfo->nLife, dwColor, 300, 30);
			
			if (!g_objHero.IsMoving() 
				&& !g_objHero.IsAttacking())
				//&& !g_objHero.TestStatus(USERSTATUS_SUPERSOLDIER) 
				//&& !g_objHero.TestStatus(USERSTATUS_TORNADO))
				g_objHero.SetActionCmd(_ACTION_BRUISE0, nDir);//(nDir+4)%8);
		}
		else
		{
			if (!g_objHero.IsMoving() 
				&& !g_objHero.IsAttacking())
				//&& !g_objHero.TestStatus(USERSTATUS_SUPERSOLDIER) 
				//&& !g_objHero.TestStatus(USERSTATUS_TORNADO))
				g_objHero.SetActionCmd(_ACTION_DODGE0, nDir);//(nDir+4)%8);
		}
	}
	else
	{
		if (pInfo->nLife > 0)
		{
			pTarget->AddZoomNum(pInfo->nLife, dwColor, 300, 30);
			
			if (!pTarget->IsAttacking())
			{
				pTarget->ResetStepDir();
				pTarget->SetActionCmd(_ACTION_BRUISE0, nDir);
			}						
			
			if (pTarget->IsNpc())
			{
				int nLife = (int)pTarget->GetData(CPlayer::_PLAYER_LIFE)-(int)pInfo->nLife;
				pTarget->SetData(CPlayer::_PLAYER_LIFE, __max(0, nLife));
				//pTarget->SetLife(__max(0, nLife));
			}					
		}
		else
		{
			if (!pTarget->IsAttacking())
			{
				pTarget->ResetStepDir();
				if(pTarget->IsPlayer())
					pTarget->SetActionCmd(_ACTION_DODGE0, nDir);
			}						
		}
	}
}
//--------------------------------------------------------------
