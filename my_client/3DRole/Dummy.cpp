
//**********************************************************
// 代码编辑器
//**********************************************************

// Dummy.cpp: implementation of the CDummy class.
//
//////////////////////////////////////////////////////////////////////

#include "Dummy.h"
#include "3DGameMap.h"
#include "3DRoleData.h"
#include "Hero.h"
#include "GamePlayerSet.h"
#include "AllMsg.h"
#include "GameMsg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDummy::CDummy()
{
    m_nCurrentDummyDir = 7;
    m_nDummyFrameIndex = 0;
    m_bDummyPlay = true;
    m_bLock = false;
    m_idDummyTemplate = ID_NONE;
    m_nCurrentDummyAction = _ACTION_STANDBY;
    for(int i = 0; i < _MAX_EQUIPMENT; i ++)
    {
        m_pEquipment[_MAX_EQUIPMENT] = NULL;
    }
}

CDummy::~CDummy()
{
    this->ClearEquipment();
}

//---------------------------------------------------------------------------------
void CDummy::ShowDummy(CMyPos posScr, int nZoomPercent, int nRotateX)
{
    CMyBitmap::ClearBuffer(true, false, 0);
    if (m_idDummyTemplate == ID_NONE)
    {
        this->LockDummy();
    }
    CMyPos posWorld;
    CMyPos posCenter = {_SCR_WIDTH / 2, _SCR_HEIGHT / 2};
    int nScrOffsetX = posCenter.x - posScr.x;
    int nScrOffsetY = posCenter.y - posScr.y;
    posScr.x = posCenter.x - nScrOffsetX;// * _NORMAL_SCALE / g_objGameMap.GetScale();
    posScr.y = posCenter.y - nScrOffsetY;// * _NORMAL_SCALE / g_objGameMap.GetScale();
    g_objGameMap.Screen2World(posScr.x, posScr.y, posWorld.x, posWorld.y);
    m_objDummy.SetWorldPos(posWorld);
    m_objDummy.SetZoomPercent(nZoomPercent);
    if (!m_objDummy.m_pIRoleView)
    {
        return;
    }
    int nRotate	= -45 * m_objDummy.GetDir() - 45;
    int nBgX, nBgY;
    g_objGameMap.World2Bg(posWorld.x, posWorld.y, nBgX, nBgY);
    int nOffsetY = 0;
    int nShadowY = 0;
    float fScale = 0.7f * nZoomPercent / 100.0f; //*_NORMAL_SCALE/(100.0f*g_objGameMap.GetScale());
    m_objDummy.m_pIRoleView->SetPos(nBgX, nBgY - nOffsetY - nShadowY, nShadowY, nRotate, fScale);
    m_objDummy.m_pIRoleView->SetVariable(_VAR_ROTATEX, nRotateX);
    CMyPos posView;
    g_objGameMap.GetViewPos(posView);
    int nViewportBgX, nViewportBgY;
    g_objGameMap.World2Bg(posView.x, posView.y, nViewportBgX, nViewportBgY);
    int nFrameInterval = g_obj3DRoleData.GetActionTimeInterval(m_nCurrentDummyAction);
    int nFrameAmount = m_objDummy.m_pIRoleView->SetAction(m_nCurrentDummyAction);
    if (m_bDummyPlay && ((int)(::TimeGet() - m_dwDummyTimer) >= nFrameInterval))
    {
        m_nDummyFrameIndex  ++;
        m_dwDummyTimer = ::TimeGet();
    }
    if (m_nDummyFrameIndex >= nFrameAmount)
    {
        m_nDummyFrameIndex = 0;
    }
    m_objDummy.m_pIRoleView->SetVariable(_VAR_FRAME, m_nDummyFrameIndex);
    m_objDummy.m_pIRoleView->SetLightOffset(CRole::SHADOW_NONE, 300, -300, -1000, 0.8f, 0.1f, 0.1f, 0.1f);
    m_objDummy.m_pIRoleView->Draw(nViewportBgX, nViewportBgY);
}
//---------------------------------------------------------------------------------
void CDummy::SetDummyAction(int nAction)
{
    m_nCurrentDummyAction = nAction;
    m_objDummy.m_pIRoleView->SetAction(m_nCurrentDummyAction);
    m_nDummyFrameIndex = 0;
    m_nCurrentDummyDir = 7;
}
//---------------------------------------------------------------------------------
void CDummy::SetDummyDir(int nDir)
{
    m_nCurrentDummyDir = nDir;
    m_objDummy.SetDir(nDir);
}
//---------------------------------------------------------------------------------
void CDummy::PlayDummy()
{
    m_bDummyPlay = true;
}
//---------------------------------------------------------------------------------
void CDummy::PauseDummy()
{
    m_bDummyPlay = false;
}
//---------------------------------------------------------------------------------

void CDummy::ProcessDummy()
{
    if (!m_objDummy.m_pIRoleView)
    {
        this->LockDummy();
    }
    m_objDummy.Process(NULL);
}
//---------------------------------------------------------------------------------
BOOL CDummy::LockDummy(OBJID idPlayer)
{
    m_idDummyTemplate = idPlayer;
    CPlayer* pDummyTemplate = NULL;
    BOOL bOK = true;
    // 容忍缺省
    if (idPlayer == ID_NONE)
    {
        pDummyTemplate = &g_objHero;
    }
    // 从超集中取
    if (!pDummyTemplate)
    {
        pDummyTemplate = g_objPlayerSet.GetPlayer(idPlayer, true);
    }
    // 未寻获容忍
    if (!pDummyTemplate)
    {
        return false;
    }
    if (!pDummyTemplate->IsPlayer())
    {
        return false;
    }
    // 类型检查
    if (pDummyTemplate->GetRoleType() != _ROLE_HERO
            && pDummyTemplate->GetRoleType() != _ROLE_PLAYER
            && pDummyTemplate->GetRoleType() != _ROLE_MONSTER)
        // 非法类型容忍
    {
        return false;
    }
    /*
    if (idPlayer != ID_NONE)
    {
    	// 权限检查 ...
    	// 配偶...
    	BOOL bPass = false;
    	if (strcmp(pDummyTemplate->GetName(), g_objHero.GetMate()) == 0)
    		bPass = true;
    	// 同帮
    	if (pDummyTemplate->GetSydicateID() != ID_NONE &&  g_objHero.GetSydicateID() != ID_NONE)
    	{
    		if (g_objPlayerSet.GetSyndicateStatus(pDummyTemplate->GetSydicateID()) == SYNDICATE_SELF)
    			bPass = true;
    	}
    	// 同队
    	if (g_objHero.GetTeamMemberInfoByID(pDummyTemplate->GetID()))
    		bPass = true;
    	// 好友
    	if (g_objHero.GetFriend(pDummyTemplate->GetID()))
    		bPass = true;
    	// 摆酷
    	if (pDummyTemplate->GetCommandType() == _COMMAND_EMOTION && pDummyTemplate->GetActionType() == _ACTION_COOLPOSE)
    		bPass = true;
    	if (!bPass)
    	{
    		const OBJID idStr = 100127;
    		g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
    		return false;
    	}
    }
    */
    if (!m_objDummy.m_pIRoleView)
    {
        m_objDummy.Create(pDummyTemplate->GetLook());
    }
    else
    {
        m_objDummy.SetLook(pDummyTemplate->GetLook());
    }
    m_objDummy.SetHair(pDummyTemplate->GetHair());
    m_objDummy.SetArmor(pDummyTemplate->GetArmor());
    m_objDummy.SetFace(pDummyTemplate->GetFace());
    m_objDummy.SetArmet(pDummyTemplate->GetArmet());
    m_objDummy.SetRWeapon(pDummyTemplate->GetRWeapon());
    m_objDummy.SetLWeapon(pDummyTemplate->GetLWeapon());
    m_objDummy.SetDir(7);
    m_objDummy.SetMate("");
    strcpy(m_szName, pDummyTemplate->GetName());
    if (idPlayer != ID_NONE)
    {
        // query ...
        CMsgAction msg;
        if (msg.Create(g_objHero.GetID(), 0, 0, 0, actionQueryEquipment, m_idDummyTemplate))
        {
            msg.Send();
            this->ClearEquipment();
        }
        CMsgName Msgname;
        if (Msgname.Create(NAMEACT_QUERY_MATE, "", m_idDummyTemplate))
        {
            Msgname.Send();
        }
    }
    return bOK;
}
//---------------------------------------------------------------------------------
void	CDummy::ClearEquipment()
{
    for(int i = 0; i < _MAX_EQUIPMENT; i ++)
    {
        SAFE_DELETE(m_pEquipment[i]);
    }
}
//---------------------------------------------------------------------------------
BOOL	CDummy::SetEquipment(int nPosition, CItem* pItem)
{
    if (!pItem)
    {
        return false;
    }
    if (pItem->GetID() != m_idDummyTemplate)
    {
        return false;
    }
    MYASSERT(nPosition - 1 >= 0 && nPosition - 1 < _MAX_EQUIPMENT);
    int nType = nPosition - 1;
    switch(nPosition)
    {
    case ITEMPOSITION_HELMET:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_NECKLACE:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_ARMOR:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_WEAPONR:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_WEAPONL:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_RINGR:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_TREASURE:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_SHOES:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_MOUNT:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_SPRITE:
        m_pEquipment[nType] = pItem;
        break;
    }
    return true;
}
