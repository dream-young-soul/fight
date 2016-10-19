
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DArmor.cpp: implementation of the C3DArmor class.
//
//////////////////////////////////////////////////////////////////////

#include "3DArmor.h"
#include "GameDataSet.h"
#include "3DRoleData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DArmor::C3DArmor()
{
}

C3DArmor::~C3DArmor()
{
}

//--------------------------------------------------------------------
C3DArmor*	C3DArmor::CreateNew(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return NULL;
    }
    C3DArmor* pData = new C3DArmor;
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

//--------------------------------------------------------------------
BOOL C3DArmor::Create(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return false;
    }
    const char szIniFile[] = "ini/Armor.ini";
    char szTitle[256] = "";
    sprintf(szTitle, "Armor%09u", idType);
    RolePartInfo* pPartInfo = g_obj3DRoleData.GetRoleRartInfo(ROLEPARTINFO_AEMOR, szTitle);
    if (!pPartInfo)
    {
        ::ErrorOut("%s not found in %s!", szTitle, szIniFile);
		return false;
    }
    m_infoPart.idMesh = pPartInfo->idMesh;
    m_infoPart.idTexture = pPartInfo->idTexture;
    m_infoPart.p3DMesh = g_objGameDataSet.Get3DObj(pPartInfo->idMesh);
    m_infoPart.idTexture2 = pPartInfo->idTexture2;
    m_infoPart.fCurrentX = 0.0f;
    m_infoPart.fCurrentY = 0.0f;
    m_infoPart.fSpeedX = pPartInfo->fMoveRateX;
    m_infoPart.fSpeedY = pPartInfo->fMoveRateY;
    // keep idtype
    m_idType = idType;
    return true;
}

//--------------------------------------------------------------------
