
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMantle.cpp: implementation of the C3DMantle class.
//
//////////////////////////////////////////////////////////////////////

#include "3DMantle.h"
#include "GameDataSet.h"
#include "3DRoleData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DMantle::C3DMantle()
{
    m_idType = ID_NONE;
}

C3DMantle::~C3DMantle()
{
}
//-------------------------------------------------------------------------
C3DMantle* C3DMantle::CreateNew(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return NULL;
    }
    C3DMantle* pData = new C3DMantle;
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
BOOL C3DMantle::Create(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return false;
    }
    const char szIniFile[] = "ini/Mantle.ini";
    char szTitle[256] = "";
    sprintf(szTitle, "Mantle%010u", idType);
    RolePartInfo* pPartInfo = g_obj3DRoleData.GetRoleRartInfo(ROLEPARTINFO_MANTLE, szTitle);
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

