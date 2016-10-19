
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DWeapon.cpp: implementation of the C3DWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "3DWeapon.h"
#include "GameDataSet.h"
#include "3DRoleData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DWeapon::C3DWeapon()
{
    m_idType = ID_NONE;
}

C3DWeapon::~C3DWeapon()
{
}

//-------------------------------------------------------------------------
C3DWeapon* C3DWeapon::CreateNew(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return NULL;
    }
    C3DWeapon* pData = new C3DWeapon;
    if (!pData)
    {
        return NULL;
    }
    if (!pData->Create(idType))
    {
        SAFE_DELETE(pData);
        return NULL;
    }
    return pData;
}
//-------------------------------------------------------------------------
BOOL C3DWeapon::Create(OBJID idType)
{
    //	if (idType / 1000 == 490) // ½äÖ¸
    //	{
    //		m_idType = idType;
    //		m_infoPart.idMesh = ID_NONE;
    //		m_infoPart.idTexture = ID_NONE;
    //		m_infoPart.p3DMesh = NULL;
    //		return true;
    //	}
    if (idType == ID_NONE)
    {
        return false;
    }
    const char szIniFile[] = "ini/Weapon.ini";
    char szTitle[256] = "";
    sprintf(szTitle, "Weapon%u", idType);
    RolePartInfo* pPartInfo = g_obj3DRoleData.GetRoleRartInfo(ROLEPARTINFO_WEAPON, szTitle);
    if (!pPartInfo)
    {
        ::ErrorOut("%s not found in %s!", szTitle, szIniFile);
    }
    m_infoPart.idMesh = pPartInfo->idMesh;
    m_infoPart.idTexture = pPartInfo->idTexture;
    m_infoPart.p3DMesh = g_objGameDataSet.Get3DObj(pPartInfo->idMesh);
    m_infoPart.idTexture2 = pPartInfo->idTexture2;
    m_infoPart.fCurrentX = 0.0f;
    m_infoPart.fCurrentY = 0.0f;
    m_infoPart.fSpeedX = pPartInfo->fMoveRateX;
    m_infoPart.fSpeedY = pPartInfo->fMoveRateY;
    // set default motion
    this->SetDefaultMotion();
    // keep idtype
    m_idType = idType;
    return true;
}

//-------------------------------------------------------------------------

