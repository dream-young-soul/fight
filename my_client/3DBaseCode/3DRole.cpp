
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------------------------------
//	3DRole.cpp
//-------------------------------------------------------------

#include "3DRole.h"
#include "GameDataSet.h"
#include "3DRoleData.h"
#include "../ndsound/ndsound.h"
const char HAIR_INI[] = "ini/hair.ini";

//需要特殊处理的几种状态表现
const __int64	PLAYERTATUS_ATTACK			= 0x00010000;			// 攻击提升
const __int64	PLAYERTATUS_ADJUST_XP		= 0x0000010000000000;	// 调节每次增加XP值


#ifdef _ANALYSIS_ON
DWORD g_dw3DRoleDrawTime = 0;
DWORD g_dw3DRoleDrawAmount = 0;
DWORD g_dw3DRoleDrawTimeMax = 0;
DWORD g_dw3DRoleDrawTimeFrame = 0;
DWORD g_dw3DRoleDrawAmountFrame = 0;
#endif
//-------------------------------------------------------------
C3DRole::C3DRole ()
{
    m_nPosX = m_nPosY = 0;
    m_nRotateZ = 0;
    m_nRotateX = -37;
    m_nRotateY = 0;
    m_fScale = 1.0f;
    m_LightOffset.x = 0;
    m_LightOffset.y = 0;
    m_LightOffset.z = -1000;
    m_nShadowType = 2;
    m_a = 1.0f;
    m_r = 0.3f;
    m_g = 0.3f;
    m_b = 0.3f;
    m_dwPlayerStatus = 0;
    m_pArmor	= NULL;
    m_pLWeapon	= NULL;
    m_pRWeapon	= NULL;
    m_pLShield	= NULL;
    m_pRShield	= NULL;
    m_pMount	= NULL;
    m_pArmet	= NULL;
    m_pMantle	= NULL;
    m_pHead		= NULL;
    m_pArmorClone	= NULL;
    m_nLook		= 0;
    m_nHair		= 0;
    m_nLookClone = 0;
    m_usStatus  = 0;
    m_pMotionArmor	= NULL;
    m_pMotionArmet	= NULL;
    m_pMotionWeaponR = NULL;
    m_pMotionWeaponL = NULL;
    m_pMotionMount	= NULL;
    m_pMotionMantle	= NULL;
    m_min.x = 0;
    m_min.y = 0;
    m_min.z = 0;
    m_max.x = 0;
    m_max.y = 0;
    m_max.z = 0;
    m_pEffect = NULL;
}

C3DRole::~C3DRole ()
{
    SAFE_DELETE(m_pArmor);
    SAFE_DELETE(m_pLWeapon);
    SAFE_DELETE(m_pRWeapon);
    SAFE_DELETE(m_pLShield);
    SAFE_DELETE(m_pRShield);
    SAFE_DELETE(m_pMount);
    SAFE_DELETE(m_pArmet);
    SAFE_DELETE(m_pMantle);
    SAFE_DELETE(m_pArmorClone);
    SAFE_DELETE(m_pHead);
}

//------------------------------------------------------
void C3DRole::ResetMotion	(void)
{
    m_pMotionArmor	= NULL;
    m_pMotionArmet	= NULL;
    m_pMotionWeaponR = NULL;
    m_pMotionWeaponL = NULL;
    m_pMotionMount	= NULL;
    m_pMotionMantle = NULL;
}

//------------------------------------------------------
void C3DRole::Move ( float x, float y, float z )
{
    if (m_pArmor)
    {
        m_pArmor->Move(x, y, z);
    }
    if (m_pLWeapon)
    {
        m_pLWeapon->Move(x, y, z);
    }
    if (m_pRWeapon)
    {
        m_pRWeapon->Move(x, y, z);
    }
    if (m_pMount)
    {
        m_pMount->Move(x, y, z);
    }
    if (m_pArmet)
    {
        m_pArmet->Move(x, y, z);
    }
    if (m_pArmorClone)
    {
        m_pArmorClone->Move(x, y, z);
    }
    if (m_pMantle)
    {
        m_pMantle->Move(x, y, z);
    }
    if (m_pHead)
    {
        m_pHead->Move(x, y, z);
    }
}

//------------------------------------------------------
void C3DRole::Rotate ( float x, float y, float z )
{
    if (m_pArmor)
    {
        m_pArmor->Rotate(x, y, z);
    }
    if (m_pLWeapon)
    {
        m_pLWeapon->Rotate(x, y, z);
    }
    if (m_pRWeapon)
    {
        m_pRWeapon->Rotate(x, y, z);
    }
    if (m_pMount)
    {
        m_pMount->Rotate(x, y, z);
    }
    if (m_pArmet)
    {
        m_pArmet->Rotate(x, y, z);
    }
    if (m_pArmorClone)
    {
        m_pArmorClone->Rotate(x, y, z);
    }
    if (m_pMantle)
    {
        m_pMantle->Rotate(x, y, z);
    }
    if (m_pHead)
    {
        m_pHead->Rotate(x, y, z);
    }
}

//------------------------------------------------------
void C3DRole::Scale ( float x, float y, float z )
{
    if (m_pArmor)
    {
        m_pArmor->Scale(x, y, z);
    }
    if (m_pLWeapon)
    {
        m_pLWeapon->Scale(x, y, z);
    }
    if (m_pRWeapon)
    {
        m_pRWeapon->Scale(x, y, z);
    }
    if (m_pMount)
    {
        m_pMount->Scale(x, y, z);
    }
    if (m_pArmet)
    {
        m_pArmet->Scale(x, y, z);
    }
    if (m_pArmorClone)
    {
        m_pArmorClone->Scale(x, y, z);
    }
    if (m_pMantle)
    {
        m_pMantle->Scale(x, y, z);
    }
    if (m_pHead)
    {
        m_pHead->Scale(x, y, z);
    }
}

//------------------------------------------------------
DWORD C3DRole::GetWeaponActionData (void)
{
    if (m_pRWeapon && !m_pLWeapon)	// single or two hand weapon
    {
        OBJID idType = m_pRWeapon->GetTypeID();
        return (idType % 1000000) / 1000;
    }
    if (!m_pRWeapon && m_pLWeapon)	// shield only
    {
        OBJID idType = m_pLWeapon->GetTypeID();
        return 741;
    }
    if (m_pRWeapon && m_pLWeapon)	// two weapon
    {
        if (((m_pLWeapon->GetTypeID() % 1000000) / 100000) == 9)		// shield
        {
            OBJID idTypeR = m_pRWeapon->GetTypeID();
            OBJID idTypeL = m_pLWeapon->GetTypeID();
            return 7 * 100 + ((idTypeR % 1000000) / 10000);
        }
        else if (((m_pLWeapon->GetTypeID() % 1000000) / 100000) == 4)	// single hand weapon
        {
            OBJID idTypeR = m_pRWeapon->GetTypeID();
            OBJID idTypeL = m_pLWeapon->GetTypeID();
            return 6 * 100 + ((idTypeR % 100000) / 10000) * 10 + ((idTypeL % 100000) / 10000);
        }
        if (m_pRWeapon->GetTypeID() / 1000 == 500) // bow
        {
            return 500;
        }
    }
    return 0;	// default
}

//------------------------------------------------------
DWORD C3DRole::GetMountActionData (void)
{
    if (!m_pMount)
    {
        return 0;
    }
    // temp code
    return 1;
}


//------------------------------------------------------
int  C3DRole::SetCloneAction (int nActionType, BOOL bPlaySound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    MYASSERT (m_pArmor);
    // body motion
    OBJID idBodyMotion = m_nLookClone * 1000000 + nActionType;
    // load motion
    C3DMotion* pBodyMotion = g_objGameDataSet.Get3DMotion(idBodyMotion);
    if (!pBodyMotion)
    {
        ::LogMsg("Error: motion(%u) can't found.", idBodyMotion);
        // load default motion if error
        OBJID idDefaultMotion = m_nLook * 1000000 + nActionType;
        pBodyMotion	 = g_objGameDataSet.Get3DMotion(idDefaultMotion);
        if (!pBodyMotion)
        {
            const OBJID idActionError =  500000;
            ::ErrorOut(g_objGameDataSet.GetStr(idActionError), idBodyMotion, idDefaultMotion);
            return 0;
        }
        else
        {
            return pBodyMotion->GetFrameAmount();
        }
    }
    if (m_pArmorClone)
    {
        if (bSetEffect)
        {
            m_pArmorClone->Set3DEffect(m_nLook, nActionType, m_pArmor->GetTypeID() % 1000, m_pEffect, "v_body");
        }
        m_pArmorClone->SetMotion(pBodyMotion);
    }
    return pBodyMotion->GetFrameAmount();
}


//------------------------------------------------------
C3DMotion* C3DRole::GetWeaponMotion	(OBJID idWeapon, int nActionType)
{
    idWeapon = (idWeapon / 10) * 10;
    OBJID idMotion = idWeapon * 1000 + nActionType;
    C3DMotion* pMotion  = g_objGameDataSet.Get3DWeaponMotion(idMotion);
    if (pMotion)
    {
        return pMotion;
    }
    idMotion = idWeapon * 1000 + 999;
    pMotion  = g_objGameDataSet.Get3DWeaponMotion(idMotion);
    if (pMotion)
    {
        return pMotion;
    }
    idMotion = ((idWeapon / 1000) * 1000 + 999) * 1000 + nActionType;
    pMotion  = g_objGameDataSet.Get3DWeaponMotion(idMotion);
    if (pMotion)
    {
        return pMotion;
    }
    idMotion = ((idWeapon / 1000) * 1000 + 999) * 1000 + 999;
    pMotion  = g_objGameDataSet.Get3DWeaponMotion(idMotion);
    if (pMotion)
    {
        return pMotion;
    }
    return NULL;
}

//------------------------------------------------------
void C3DRole::NextFrame ( int nStep )
{
    m_pArmor->NextFrame(nStep);
}

//------------------------------------------------------
BOOL C3DRole::SetLWeapon(OBJID idType)
{
    // reset motion
    this->ResetMotion();
    SAFE_DELETE(m_pLWeapon);
    if (m_pEffect != NULL)
    {
        m_pEffect->DeleteByVMesh("v_l_weapon");
    }
    if (idType == ID_NONE)
    {
        return false;
    }
    m_pLWeapon = C3DWeapon::CreateNew(idType);
    m_pLWeapon->CreateLight(idType);
    return true;
}

//------------------------------------------------------
BOOL C3DRole::SetRWeapon(OBJID idType)
{
    // reset motion
    this->ResetMotion();
    idType %= 1000000;
    SAFE_DELETE(m_pRWeapon);
    if (m_pEffect != NULL)
    {
        m_pEffect->DeleteByVMesh("v_r_weapon");
    }
    if (idType == ID_NONE)
    {
        return false;
    }
    m_pRWeapon = C3DWeapon::CreateNew(idType);
    m_pRWeapon->CreateLight(idType);
    return true;
}
//------------------------------------------------------
void C3DRole::SetHead(OBJID idFace)
{
    // reset motion
    this->ResetMotion();
    SAFE_DELETE(m_pHead);
    if (m_nLook == 1)
    {
        m_pHead = C3DHead::CreateNew(1000000 + idFace);
    }
    else if (m_nLook == 2)
    {
        m_pHead = C3DHead::CreateNew(2000000 + idFace);
    }
    if (m_pEffect != NULL)
    {
        m_pEffect->DeleteByVMesh("v_head");
    }
    //MYASSERT (m_pHead);
}
//------------------------------------------------------

void C3DRole::SetArmor(OBJID idType)
{
    // reset motion
    this->ResetMotion();
    idType %= 1000000;
    SAFE_DELETE(m_pArmor);
    /*
    if (idType == ID_NONE)
    	return;
    */
    m_pArmor = C3DArmor::CreateNew(m_nLook * 1000000 + idType / 10 * 10);
    m_pArmor->CreateLight(idType);
    if (m_pEffect != NULL)
    {
        m_pEffect->DeleteByVMesh("v_body");
    }
    MYASSERT (m_pArmor);
}
//------------------------------------------------------
void C3DRole::SetMantle	(OBJID idType)
{
    // reset motion
    this->ResetMotion();
    SAFE_DELETE(m_pMantle);
    if (m_pEffect != NULL)
    {
        m_pEffect->DeleteByVMesh("v_mantle");
    }
    if (idType == ID_NONE)
    {
        return;
    }
    idType %= 1000000;
    idType = m_nLook * 1000000 + idType;
    m_pMantle = C3DMantle::CreateNew(idType);
    m_pMantle->CreateLight(idType);
    MYASSERT(m_pMantle);
}
//------------------------------------------------------
void C3DRole::SetMount(OBJID idMountType)
{
    // reset motion
    this->ResetMotion();
    idMountType %= 1000000;
    SAFE_DELETE(m_pMount);
    if (m_pEffect != NULL)
    {
        m_pEffect->DeleteByVMesh("v_pet");
    }
    if (idMountType == ID_NONE)
    {
        return;
    }
    m_pMount = C3DMount::CreateNew(idMountType);
    m_pMount->CreateLight(idMountType);
    MYASSERT (m_pMount);
}

//------------------------------------------------------
void C3DRole::SetLook(int nLook)
{
    MYASSERT (nLook > 0 && nLook < 1000);
    m_nLook = nLook;
    if (!m_pArmor)
    {
        m_pArmor = C3DArmor::CreateNew(m_nLook * 1000000);
        MYASSERT (m_pArmor);
    }
    else
    {
        OBJID idType = m_pArmor->GetTypeID();
        if ((idType % 1000000) == 0)	// no real armor
        {
            SAFE_DELETE(m_pArmor);
            m_pArmor = C3DArmor::CreateNew(m_nLook * 1000000);
            MYASSERT (m_pArmor);
        }
    }
    //	this->SetHead(0);	//fot TerrainEffectEditor
}

//------------------------------------------------------
void C3DRole::SetArmet(OBJID idArmetType)
{
    // reset motion
    this->ResetMotion();
    // auto correct type id
    idArmetType = idArmetType % 1000000;
    if (119 != (idArmetType % 1000000) / 1000)
    {
        idArmetType = (idArmetType / 10) * 10;
    }
    // sub type...
    int nSubType = (idArmetType % 1000000) / 1000;
    if (nSubType == 117 || nSubType == 118 || (nSubType == 112 && (idArmetType / 10) % 10 == 8))
    {
        // invisible head equipment
        if (m_pArmet)	// got armet already
        {
            int nSubType = (m_pArmet->GetTypeID() % 1000000) / 1000;
            if (nSubType == 119)	// current armet is hair
            {
                return;
            }
            else					// current armet is helmet, so replace it
            {
                SAFE_DELETE(m_pArmet);
                if (m_nHair != 0)	// got hair
                {
                    OBJID idNewType = 119000 + m_nHair;
                    m_pArmet = C3DArmet::CreateNew(m_nLook * 1000000 + idNewType);
                    MYASSERT (m_pArmet);
                }
            }
        }
        return;
    }
    else if (nSubType == 119)	// new hair
    {
        if (m_pArmet)	// got armet already
        {
            int nSubType = (m_pArmet->GetTypeID() % 1000000) / 1000;
            if (nSubType == 119)	// current armet is hair, so replace it
            {
                SAFE_DELETE(m_pArmet);
                if ((idArmetType % 1000) != 0)	// not zero
                {
                    m_pArmet = C3DArmet::CreateNew(m_nLook * 1000000 + idArmetType);
                    MYASSERT (m_pArmet);
                }
            }
        }
        else	// no armet, so create it
        {
            if (idArmetType % 1000)	// not zero
            {
                m_pArmet = C3DArmet::CreateNew(m_nLook * 1000000 + idArmetType);
                MYASSERT (m_pArmet);
            }
        }
        return;
    }
    else	// new helmet or ID_NONE
    {
        if (idArmetType == ID_NONE)
        {
            if (m_pArmet)	// got armet already
            {
                int nSubType = (m_pArmet->GetTypeID() % 1000000) / 1000;
                if (nSubType == 119)	// current armet is hair
                {
                    // do nothing
                    //SAFE_DELETE(m_pArmet);
                }
                else	// is not hair, so delete it; and restore hair if it got one
                {
                    SAFE_DELETE(m_pArmet);
                    if (m_nHair != 0)
                    {
                        m_pArmet = C3DArmet::CreateNew(m_nLook * 1000000 + 119000 + m_nHair);
                        MYASSERT (m_pArmet);
                    }
                }
            }
        }
        else	// new helmet
        {
            SAFE_DELETE(m_pArmet);
            m_pArmet = C3DArmet::CreateNew(m_nLook * 1000000 + idArmetType);
            MYASSERT (m_pArmet);
        }
    }
}

//------------------------------------------------------
void C3DRole::SetHair(int nHair)
{
    m_nHair = nHair;
    this->SetArmet(nHair + 119000);
}

//------------------------------------------------------
void C3DRole::Transform(int nLook)
{
    m_nLookClone = nLook;
    SAFE_DELETE(m_pArmorClone);
    if (nLook == 99 || nLook == 98)
    {
        return;
    }
    else if (nLook > 0)
    {
        m_pArmorClone = C3DArmor::CreateNew(nLook * 1000000);
    }
}
//------------------------------------------------------
void C3DRole::SetFrame (DWORD dwFrame)
{
    if (m_pArmorClone)
    {
        m_pArmorClone->SetFrame(dwFrame);
    }
    else
    {
        if (m_pArmor)
        {
            m_pArmor->SetFrame(dwFrame);
        }
        if (m_pMount)
        {
            m_pMount->SetFrame(dwFrame);
        }
        if (m_pArmet)
        {
            m_pArmet->SetFrame(dwFrame);
        }
        if (m_pRWeapon)
        {
            m_pRWeapon->SetFrame(dwFrame);
        }
        if (m_pLWeapon)
        {
            m_pLWeapon->SetFrame(dwFrame);
        }
        if (m_pMantle)
        {
            m_pMantle->SetFrame(dwFrame);
        }
    }
}
//------------------------------------------------------

BOOL	C3DRole::SetVariable(int nVariableType, DWORD dwData)
{
    switch(nVariableType)
    {
    case _VAR_LOOK:
        this->SetLook((int)dwData);
        return true;
    case _VAR_HAIR:
        this->SetHair((int)dwData);
        return true;
    case _VAR_TRANSFORM:
        this->Transform((int)dwData);
        return true;
    case _VAR_ARMOR:
        this->SetArmor(dwData);
        return true;
    case _VAR_ARMET:
        this->SetArmet(dwData);
        return true;
    case _VAR_MANTLE:
        this->SetMantle(dwData);
        return true;
    case _VAR_RWEAPON:
        return this->SetRWeapon(dwData);
    case _VAR_LWEAPON:
        return this->SetLWeapon(dwData);
    case _VAR_MOUNT:
        this->SetMount(dwData);
        return true;
    case _VAR_FRAME:
        this->SetFrame(dwData);
        return true;
    case _VAR_ROTATEX:
        this->SetRotateX(dwData);
        return true;
    case _VAR_HEAD:
        this->SetHead(dwData);
        return true;
    case _VAR_STATUS:
        this->SetPlayerStatus(dwData);
        return true;
    }
    return false;
}
//------------------------------------------------------
DWORD	C3DRole::GetVariable(int nVariableType)
{
    switch(nVariableType)
    {
    case _VAR_RWEAPON_ID:
        if (!m_pRWeapon)
        {
            return ID_NONE;
        }
        return m_pRWeapon->GetTypeID();
    case _VAR_WEAPON_ACTION_DATA:
        return this->GetWeaponActionData();
    case _VAR_LOOKCLONE:
        return m_nLookClone;
    case _VAR_ACTION_DATA:
        return this->GetActionData();
    }
    return 0;
}
//------------------------------------------------------
DWORD	C3DRole::GetActionData (void)
{
    return (this->GetMountActionData() * 1000000000 +
            m_nLook * 1000000 +
            this->GetWeaponActionData() * 1000 +
            m_nActionType);
}
//------------------------------------------------------
int		C3DRole::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlaySound, int nWorldX, int nWorldY, BOOL bSetEffect, DWORD dwTerrain)
{
    if (!m_pArmor)
    {
        return 10;
    }
    m_nActionType = nActionType;
    // body motion
    if (bResetMotion || !m_pMotionArmor)
    {
        OBJID idBodyMotion = this->GetMountActionData() * 1000000000 +
                             m_nLook * 1000000 +
                             this->GetWeaponActionData() * 1000 +
                             nActionType;
        m_pMotionArmor = g_objGameDataSet.Get3DMotion(idBodyMotion);
        if (!m_pMotionArmor)
        {
            ::LogMsg("Error: motion(%u) can't found.", idBodyMotion);
            // load default motion if error
            OBJID idDefaultMotion = m_nLook * 1000000 + nActionType;
            m_pMotionArmor	 = g_objGameDataSet.Get3DMotion(idDefaultMotion);
            if (!m_pMotionArmor)
            {
                idDefaultMotion = m_nLook * 1000000 + 100; //standby
                m_pMotionArmor	 = g_objGameDataSet.Get3DMotion(idDefaultMotion);
                if (!m_pMotionArmor)
                {
                    const OBJID idActionError =  500000;
                    ::ErrorOut(g_objGameDataSet.GetStr(idActionError), idBodyMotion, idDefaultMotion);
                    return 0;
                }
            }
            else
            {
                return m_pMotionArmor->GetFrameAmount();
            }
        }
    }
    if (m_pArmor)
    {
        if (bSetEffect)
        {
            m_pArmor->Set3DEffect(m_nLook, nActionType, m_pArmor->GetTypeID() % 1000, m_pEffect, "v_body");
        }
        m_pArmor->SetMotion(m_pMotionArmor);
        if (m_pHead)
        {
            m_pHead->SetMotion(m_pMotionArmor);
        }
    }
    // Mount motion
    if (m_pMount)
    {
        if (bSetEffect)
        {
            m_pMount->Set3DEffect(m_nLook, nActionType, m_pMount->GetTypeID() % 1000, m_pEffect, "v_pet");
        }
        if (bResetMotion || !m_pMotionMount)
        {
            m_pMotionMount = g_objGameDataSet.Get3DMountMotion(m_pMount->GetTypeID() % 1000 / 100 * 1000 + nActionType);
        }
        if (bResetMotion || m_pMotionMount)
        {
            m_pMount->SetMotion(m_pMotionMount);
        }
        /*
        char szMountMotionIndex[64];
        sprintf(szMountMotionIndex, "%s.%s", m_pMount->GetMountActionName(), g_obj3DRoleData.GetActionName(nActionType));
        pMotion = g_obj3DRoleData.GetGameMotion(szMountMotionIndex);
        MYASSERT(pMotion);
        m_pMount->SetMotion(pMotion);
        		*/
    }
    // Armet motion
    if (m_pArmet)
    {
        if (bSetEffect)
        {
            m_pArmet->Set3DEffect(m_nLook, nActionType, m_pArmet->GetTypeID() % 1000, m_pEffect, "v_head");
        }
        m_pArmet->SetMotion(m_pMotionArmor);
    }
    // clone armor
    if (m_pArmorClone)
    {
        return this->SetCloneAction(nActionType, bPlaySound, nWorldX, nWorldY, bSetEffect);
    }
    if (m_pRWeapon)
    {
        if (bSetEffect)
        {
            if (this->TestPlayerStatus(PLAYERTATUS_ATTACK))
            {
                m_pRWeapon->Clear3DEffect((char*)g_obj3DRoleData.GetRolePart3DEffect(m_nLook, nActionType, (m_pRWeapon->GetTypeID() % 1000000) / 1000, m_pRWeapon->GetTypeID() % 1000));
                OBJID idType = m_pRWeapon->GetTypeID();
                int nSort = (idType % 10000000) / 100000;
                if (nSort == 4)	//ITEMSORT_WEAPON_SINGLE_HAND;
                {
                    int nType = idType / 10000;
                    switch(nType)
                    {
                    case 41:
                    case 42:
                        m_pRWeapon->AddEffect("Xattack04+v_r_weapon", true);
                        break;
                    case 44:
                        m_pRWeapon->AddEffect("Xattack01+v_r_weapon", true);
                        break;
                    case 49:
                        m_pRWeapon->AddEffect("Xattack03+v_r_weapon", true);
                        break;
                    default:
                        break;
                    }
                }
            }
            else
            {
                m_pRWeapon->Clear3DEffect("Xattack04+v_r_weapon");
                m_pRWeapon->Clear3DEffect("Xattack01+v_r_weapon");
                m_pRWeapon->Clear3DEffect("Xattack03+v_r_weapon");
                m_pRWeapon->Set3DEffect(m_nLook, nActionType, m_pRWeapon->GetTypeID() % 1000, m_pEffect, "v_r_weapon");
            }
        }
    }
    // player sound
    if (bPlaySound)
    {
        const char* pszSound = g_obj3DRoleData.GetActionSound(dwTerrain, m_nLook, this->GetWeaponActionData(), nActionType);
        if (pszSound)
        {
            try
            {
                // player sound here
                const int nRange = 3000;
                ::DXPlaySound ((char*)pszSound, nWorldX, nWorldY, nRange);
            }
            catch(...)
            {
                ::LogMsg("dxplayersound play action sound:%s", pszSound);
            }
        }
    }
    return m_pMotionArmor->GetFrameAmount();
}
//------------------------------------------------------
void	C3DRole::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    m_nPosX = nX;
    m_nPosY = nY;
    m_nRotateZ = nRotate;
    m_fScale = fScale;
    m_nPosH = nHeight * 2;
}
//------------------------------------------------------
void	C3DRole::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
{
    m_nShadowType = type;
    m_LightOffset.x = x;
    m_LightOffset.y = -y;
    m_LightOffset.z = -z;
    m_a = a;
    m_r = r;
    m_g = g;
    m_b = b;
}
//------------------------------------------------------
void	C3DRole::SetRGBA ( float alpha, float red, float green, float blue )
{
    if (m_pArmor)
    {
        m_pArmor->SetARGB(alpha, red, green, blue);
    }
    if (m_pLWeapon)
    {
        m_pLWeapon->SetARGB(alpha, red, green, blue);
    }
    if (m_pRWeapon)
    {
        m_pRWeapon->SetARGB(alpha, red, green, blue);
    }
    if (m_pMount)
    {
        m_pMount->SetARGB(alpha, red, green, blue);
    }
    if (m_pArmet)
    {
        m_pArmet->SetARGB(alpha, red, green, blue);
    }
    if (m_pArmorClone)
    {
        m_pArmorClone->SetARGB(alpha, red, green, blue);
    }
    if (m_pMantle)
    {
        m_pMantle->SetARGB(alpha, red, green, blue);
    }
}
//------------------------------------------------------
void   C3DRole::SetPlayerStatus(__int64 dwStatus)
{
    m_dwPlayerStatus = dwStatus;
}
//------------------------------------------------------
BOOL   C3DRole::TestPlayerStatus(__int64 dwStatus)
{
    return ((m_dwPlayerStatus & dwStatus) == dwStatus);
}
//------------------------------------------------------
BOOL	C3DRole::HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    float br = D3DXToRadian ( ( float )( m_nRotateZ % 45 ) );
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Rotate ( &matrix, 0, 0, br );
    //Common_Rotate ( &matrix, D3DXToRadian ( -45 ), 0, 0 );
    Common_Scale ( &matrix, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.3f );
    Common_Translate ( &matrix, (float)m_nPosX, 0, (float)m_nPosY );
    D3DXVECTOR3 min, max;
    m_pArmor->TransformCoord(&min, &max, &matrix);
    float temp;
    if ( min.x > max.x )
    {
        temp = min.x;
        min.x = max.x;
        max.x = temp;
    }
    if ( min.y > max.y )
    {
        temp = min.y;
        min.y = max.y;
        max.y = temp;
    }
    if ( min.z > max.z )
    {
        temp = min.z;
        min.z = max.z;
        max.z = temp;
    }
    D3DXVECTOR3 from, to, dir;
    from.x = (float)(nMapX + _SCR_WIDTH / 2);
    from.y = -1000.0f;
    from.z = (float)(nMapY + _SCR_HEIGHT / 2);
    to.x = (float)(nHitScreenX + nMapX);
    to.y = 0;
    to.z = (float)(nHitScreenY + nMapY);
    dir = to - from;
    D3DXVec3Normalize ( &dir, &dir );
    return D3DXBoxBoundProbe ( &min, &max, &from, &dir );
    /*

    	float temp;
    	if ( m_min.x > m_max.x )
    	{
    		temp = m_min.x;
    		m_min.x = m_max.x;
    		m_max.x = temp;
    	}
    	if ( m_min.y > m_max.y )
    	{
    		temp = m_min.y;
    		m_min.y = m_max.y;
    		m_max.y = temp;
    	}
    	if ( m_min.z > m_max.z )
    	{
    		temp = m_min.z;
    		m_min.z = m_max.z;
    		m_max.z = temp;
    	}


    	RECT rect;
    	rect.left	= int(m_min.x-nMapX);
    	rect.right	= int(m_max.x-nMapX);
    	rect.top	= int(m_min.z-nMapY);
    	rect.bottom = int(m_max.z-nMapY);


    	if (rect.bottom - rect.top < 70)
    	{
    		rect.bottom += 35;
    		rect.top -= 35;
    	}
    	if (rect.right - rect.left < 70)
    	{
    		rect.right += 35;
    		rect.left -= 35;
    	}

    //	CMyBitmap::ShowRect(rect.left, rect.top, rect.right, rect.bottom, 0xffffffff);

    	if (nHitScreenX >= rect.left && nHitScreenY >= rect.top &&
    		nHitScreenX <= rect.right && nHitScreenY <= rect.bottom)
    		return true;
    	return false;
    //	return D3DXBoxBoundProbe ( &m_min, &m_max, &from, &dir );
    	*/
}
//------------------------------------------------------
BOOL	C3DRole::Draw ( int nMapX, int nMapY )
{
#ifdef _ANALYSIS_ON
    DWORD dwAnalysisTime = ::TimeGet();
#endif
    if (CMyBitmap::s_nShowMode != modeMesh)
    {
        C3DObj::Prepare();
    }
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Translate ( &matrix, 0, 0, (float)m_nPosH );
    Common_Rotate ( &matrix, 0, 0, D3DXToRadian ( ( float )m_nRotateZ ) );
    Common_Rotate ( &matrix, D3DXToRadian ( m_nRotateX ), 0, 0 );
    Common_Scale ( &matrix, m_fScale, m_fScale, m_fScale );
    Common_Translate ( &matrix, (float)m_nPosX, 0, (float)m_nPosY );
    // 如果有复制体，只画复制体
    if (m_pArmorClone)
    {
        m_pArmorClone->SetTexture();
        m_pArmorClone->MuliplyPhy(&matrix);
        m_pArmorClone->TransformCoord(&m_min, &m_max, &matrix);
        m_pArmorClone->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
        m_pArmorClone->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
    }
    else
    {
        if (m_pHead)
        {
            // 头盔跟随 共享骨骼
            m_pHead->SetTexture();
            m_pHead->MuliplyPhy(&matrix);
            m_pHead->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
            m_pHead->DrawAlpha(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH);
            m_pHead->ClearMatrix();
        }
        if (m_pArmet)
        {
            // 头盔跟随
            m_pArmet->SetTexture();
            m_pArmet->MuliplyPhy(&matrix);
            //m_pArmet->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_armet"));
            m_pArmet->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
            m_pArmet->DrawAlpha(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH);
            m_pArmet->ClearMatrix();
        }
        // 先画身体
        if (m_pArmor)
        {
            m_pArmor->SetTexture();
            m_pArmor->MuliplyPhy(&matrix);
            m_pArmor->TransformCoord(&m_min, &m_max, &matrix);
            m_pArmor->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
            m_pArmor->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH);
            CMyPos posBg;
            posBg.x = m_nPosX;
            posBg.y = m_nPosY;
            /*	if (m_pArmet)
            	{
            		// 头盔跟随
            		m_pArmet->SetTexture();
            		m_pArmet->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_armet"));
            		m_pArmet->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
            		m_pArmet->DrawAlpha(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH);
            		//m_pArmet->ClearMatrix();
            	}*/
            if (m_pRWeapon)
            {
                // 武器跟随 v_r_weapon
                m_pRWeapon->SetTexture();
                if ((m_pRWeapon->GetTypeID() / 1000) % 1000 == 500)
                {
                    m_pRWeapon->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_l_weapon"));
                }
                else
                {
                    m_pRWeapon->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_r_weapon"));
                }
                m_pRWeapon->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
                m_pRWeapon->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
            }
            if (m_pMount)
            {
                m_pMount->SetTexture();
                if ((m_pMount->GetTypeID() / 1000) % 1000 == 500)
                {
                    m_pMount->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_pet"));
                }
                else
                {
                    m_pMount->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_pet"));
                }
                m_pMount->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
                m_pMount->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
            }
            if (m_pEffect)
            {
                m_pEffect->Show(this, posBg);
            }
            m_pArmor->ClearMatrix();
        }
        /*		if (m_pArmet)
        		{
        			// 头盔跟随 共享骨骼
        			m_pArmet->SetTexture();
        			m_pArmet->MuliplyPhy(&matrix);
        			m_pArmet->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
        			m_pArmet->DrawAlpha(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH);
        			m_pArmet->ClearMatrix();
        		}*/
    }
    /*
    		// 画武器
    		if (m_pLWeapon )
    		{
    			// 武器跟随 v_l_weapon

    			m_pLWeapon->SetTexture();
    			if (m_pLWeapon->GetTypeID()/10000 == 105)
    			{
    				m_pLWeapon->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_r_weapon"));// v_l_shield v_l_weapon
    				if (m_nActionType == _ACTION_ATTACK0 ||
    					m_nActionType == _ACTION_ATTACK1 ||
    					m_nActionType == _ACTION_ATTACK2 ||
    					m_nActionType == _ACTION_FLY_ATTACK ||
    					m_nActionType == _ACTION_FLY_ATTACK ||
    					m_nActionType == _ACTION_FAST_SHOOT)
    				{
    					m_pLWeapon->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
    					m_pLWeapon->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH );
    				}
    			}
    			else
    			{
    				m_pLWeapon->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_l_weapon"));// v_l_shield v_l_weapon
    				m_pLWeapon->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
    				m_pLWeapon->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH );
    			}
    		}

    		if (m_pRWeapon)
    		{
    			// 武器跟随 v_r_weapon
    			m_pRWeapon->SetTexture();
    			if ((m_pRWeapon->GetTypeID()/1000)%1000 == 500)
    				m_pRWeapon->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_l_weapon"));
    			else
    				m_pRWeapon->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_r_weapon"));
    			m_pRWeapon->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
    			m_pRWeapon->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH );
    		}
    		if (m_pMantle)
    		{
    			// 披风跟随Back
    			m_pMantle->SetTexture();
    			m_pMantle->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_back"));
    			m_pMantle->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
    		}
    		if (m_pMount)
    		{
    			// 坐骑跟随 v_pet
    			m_pMount->SetTexture();
    			m_pMount->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_pet"));
    			m_pMount->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
    		}

    	}


    	// 如果有复制体，只画复制体alpha
    	if (m_pArmorClone)
    	{
    		//m_pArmorClone->SetTexture();
    		m_pArmorClone->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH );
    	}
    	else
    	{
    		if (m_pMantle)
    		{
    			//m_pArmet->SetTexture();
    			m_pMantle->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH );
    		}
    		if (m_pMount)
    		{
    			//m_pMount->SetTexture();
    			m_pMount->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, m_nPosH );
    		}
    	}*/
#ifdef _ANALYSIS_ON
    DWORD dwTimePass = ::TimeGet()  - dwAnalysisTime;
    g_dw3DRoleDrawTime += dwTimePass;
    g_dw3DRoleDrawAmount ++;
    g_dw3DRoleDrawTimeFrame += dwTimePass;
    g_dw3DRoleDrawAmountFrame ++;
    if (dwTimePass > g_dw3DRoleDrawTimeMax)
    {
        g_dw3DRoleDrawTimeMax = dwTimePass;
    }
#endif
    return true;
}
//------------------------------------------------------
IRoleView* C3DRole::CreateNewView()
{
    C3DRole* p3DRole = new C3DRole;
    MYASSERT(p3DRole);
    return p3DRole;
}


