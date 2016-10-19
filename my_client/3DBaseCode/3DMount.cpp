
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMount.cpp: implementation of the C3DMount class.
//
//////////////////////////////////////////////////////////////////////

#include "3DMount.h"
#include "GameDataSet.h"
#include "3DRoleData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DMount::C3DMount()
{
}

C3DMount::~C3DMount()
{
}

//--------------------------------------------------------------------
C3DMount*	C3DMount::CreateNew(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return NULL;
    }
    C3DMount* pData = new C3DMount;
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
BOOL	C3DMount::Create(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return false;
    }
    const char szIniFile[] = "ini/Mount.ini";
    char szTitle[256] = "";
    sprintf(szTitle, "Mount%u", idType);
    RolePartInfo* pPartInfo = g_obj3DRoleData.GetRoleRartInfo(ROLEPARTINFO_MOUNT, szTitle);
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
    m_idType = idType;
    return true;
}

//--------------------------------------------------------------------
