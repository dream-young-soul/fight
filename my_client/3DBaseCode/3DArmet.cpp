
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DArmet.cpp: implementation of the C3DArmet class.
//
//////////////////////////////////////////////////////////////////////

#include "3DArmet.h"
#include "GameDataSet.h"
#include "3DRoleData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DArmet::C3DArmet()
{
}

C3DArmet::~C3DArmet()
{
}
//--------------------------------------------------------------------
C3DArmet*	C3DArmet::CreateNew(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return NULL;
    }
    C3DArmet* pData = new C3DArmet;
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
BOOL	C3DArmet::Create(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return false;
    }
    const char szIniFile[] = "ini/Armet.ini";
    char szTitle[256] = "";
    sprintf(szTitle, "Armet%09u", idType);
    RolePartInfo* pPartInfo = g_obj3DRoleData.GetRoleRartInfo(ROLEPARTINFO_ARMET, szTitle);
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
//--------------------------------------------------------------------

