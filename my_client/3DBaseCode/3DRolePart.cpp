
//**********************************************************
// 代码编辑器
//**********************************************************

// 3DRolePart.cpp: implementation of the C3DRolePart class.
//
//////////////////////////////////////////////////////////////////////

#include "3DRolePart.h"
#include "GameDataSet.h"
#include "3DRoleData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DRolePart::C3DRolePart()
{
    m_idType = ID_NONE;
    m_nAction = -1;
    m_ucSAlpha	=	255;
    m_ucSRed	=	255;
    m_ucSGreen	=	255;
    m_ucSBlue	=	255;
    m_nLightIndex = 0;
    m_dwTotalCost = 0;
}

C3DRolePart::~C3DRolePart()
{
    this->Destroy();
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::Destroy(void)
{
    m_objGame3DEffectEx.Clear();
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::Move(float x, float y, float z)
{
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->Move(x, y, z);
    }
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::Rotate(float x, float y, float z)
{
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->Rotate(x, y, z);
    }
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::Scale(float x, float y, float z)
{
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->Scale(x, y, z);
    }
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::SetARGB(float alpha, float red, float green, float blue)
{
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->SetARGB (alpha, red, green, blue);
    }
    m_ucSAlpha	=	(unsigned char)(alpha * 255);
    m_ucSRed	=	(unsigned char)(red * 255);
    m_ucSGreen	=	(unsigned char)(green * 255);
    m_ucSBlue	=	(unsigned char)(blue * 255);
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::NextFrame(int nStep)
{
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->NextFrame(nStep);
    }
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::SetFrame(DWORD dwFrame)
{
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->SetFrame(dwFrame);
    }
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::Draw(int type, float lightx, float lighty, float lightz, float sa, float sr, float sg, float sb)
{
    this->ProcessLight();
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->Draw(type, lightx, lighty, lightz, sa, sr, sg, sb);
    }
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::DrawAlpha(int type, float lightx, float lighty, float lightz, float sa, float sr, float sg, float sb, float height)
{
    m_objGame3DEffectEx.Process();
    m_objGame3DEffectEx.Show(this);
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->DrawAlpha(type, lightx, lighty, lightz, sa, sr, sg, sb, height);
    }
}
//-----------------------------------------------------------------------------
inline C3DObjPartInfo*	C3DRolePart::GetInfo(int nIndex)
{
    return &m_infoPart;
}

//-----------------------------------------------------------------------------
inline int	C3DRolePart::GetInfoAmount()
{
    return 1;
}

//-----------------------------------------------------------------------------
inline void C3DRolePart::Clear3DEffect(char* szIndex)
{
    if (!szIndex)
    {
        m_objGame3DEffectEx.Clear();
    }
    else
    {
        m_objGame3DEffectEx.Delete(szIndex);
    }
}

//-----------------------------------------------------------------------------
inline void C3DRolePart::AddEffect(char* pszIndex, bool bOnlyOnce)
{
    if (pszIndex && strcmp(pszIndex, "none") != 0)
    {
        if (bOnlyOnce && !m_objGame3DEffectEx.TestEffect(pszIndex))
        {
            m_objGame3DEffectEx.Add(pszIndex);
        }
        else if (!bOnlyOnce)
        {
            m_objGame3DEffectEx.Add(pszIndex);
        }
    }
}
//-----------------------------------------------------------------------------
inline void C3DRolePart::SetMotion(C3DMotion* pMotion)
{
    if (!pMotion)
    {
        return;
    }
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->SetMotion(pMotion);
    }
}

//--------------------------------------------------------------------
void C3DRolePart::TransformCoord(D3DXVECTOR3* pobjMin, D3DXVECTOR3* pobjMax, _D3DXMATRIXA16* pobjMatrix)
{
    if (!pobjMin || !pobjMax || !pobjMatrix)
    {
        MYASSERT(false);
    }
    if (m_infoPart.p3DMesh)
    {
        D3DXVec3TransformCoord (pobjMin,
                                &m_infoPart.p3DMesh->m_phy[m_infoPart.p3DMesh->GetIndexByName ( "v_body" )]->bboxMin,
                                pobjMatrix);
        D3DXVec3TransformCoord (pobjMax,
                                &m_infoPart.p3DMesh->m_phy[m_infoPart.p3DMesh->GetIndexByName ( "v_body" )]->bboxMax,
                                pobjMatrix);
    }
}
//--------------------------------------------------------------------
void C3DRolePart::TransformCoordforSimpleObj(D3DXVECTOR3* pobjMin, D3DXVECTOR3* pobjMax, _D3DXMATRIXA16* pobjMatrix)
{
    if (!pobjMin || !pobjMax || !pobjMatrix)
    {
        MYASSERT(false);
    }
    if (m_infoPart.p3DMesh)
    {
        D3DXVec3TransformCoord (pobjMin,
                                &m_infoPart.p3DMesh->m_phy[0]->bboxMin,
                                pobjMatrix);
        D3DXVec3TransformCoord (pobjMax,
                                &m_infoPart.p3DMesh->m_phy[0]->bboxMax,
                                pobjMatrix);
    }
}
//--------------------------------------------------------------------
C3Motion* C3DRolePart::GetVirtualMotion(const char* pszName)
{
    if (!pszName)
    {
        return NULL;
    }
    if (m_infoPart.p3DMesh)
    {
        int nIndex = m_infoPart.p3DMesh->GetIndexByName((char*)pszName);
        if (-1 == nIndex)
        {
            //			::LogMsg("RolePart %s not found in mesh:%u", pszName, m_infoPart.idMesh);
            return NULL;
        }
        return m_infoPart.p3DMesh->m_phy[nIndex]->lpMotion;
    }
    return NULL;
}

//--------------------------------------------------------------------
void C3DRolePart::SetVirtualMotion(C3Motion* pMotion)
{
    if (!pMotion || !m_infoPart.p3DMesh || !m_infoPart.p3DMesh->m_phy[0] || !m_infoPart.p3DMesh->m_phy[0]->lpMotion)
    {
        return;
    }
    for (DWORD n = 0; n < m_infoPart.p3DMesh->m_phy[0]->lpMotion->dwBoneCount; n++)
    {
        _D3DXMATRIXA16 mm;
        Motion_GetMatrix ( pMotion,
                           0,
                           &mm );
        CopyMemory ( &m_infoPart.p3DMesh->m_phy[0]->lpMotion->matrix[n],
                     &mm,
                     sizeof ( _D3DXMATRIXA16 ) );
        /*	CopyMemory ( &m_infoPart.p3DMesh->m_phy[0]->lpMotion->matrix[n],
        			&pMotion->lpFrames[pMotion->nFrame],
        			sizeof ( _D3DXMATRIXA16 ) );
        		*/
        D3DXMatrixMultiply ( &m_infoPart.p3DMesh->m_phy[0]->lpMotion->matrix[n],
                             &m_infoPart.p3DMesh->m_phy[0]->lpMotion->matrix[n],
                             &pMotion->matrix[0] );
    }
    for (DWORD n = 0; n < m_infoPart.p3DMesh->m_dwPhyNum; n++ )
    {
        m_infoPart.p3DMesh->m_x[n] = pMotion->matrix[0]._41;
        m_infoPart.p3DMesh->m_y[n] = pMotion->matrix[0]._42;
        m_infoPart.p3DMesh->m_z[n] = pMotion->matrix[0]._43;
    }
}

//--------------------------------------------------------------------
void C3DRolePart::MuliplyPhy(_D3DXMATRIXA16* pobjMatrix)
{
    if (!pobjMatrix || !m_infoPart.p3DMesh)
    {
        return;
    }
    for ( DWORD n = 0; n < m_infoPart.p3DMesh->m_dwPhyNum; n++ )
    {
        Phy_Muliply ( m_infoPart.p3DMesh->m_phy[n], -1, pobjMatrix );
        m_infoPart.p3DMesh->m_x[n] = m_infoPart.p3DMesh->m_phy[n]->lpMotion->matrix[0]._41;
        m_infoPart.p3DMesh->m_y[n] = m_infoPart.p3DMesh->m_phy[n]->lpMotion->matrix[0]._42;
        m_infoPart.p3DMesh->m_z[n] = m_infoPart.p3DMesh->m_phy[n]->lpMotion->matrix[0]._43;
    }
}

//------------------------------------------------------------------
void C3DRolePart::ClearMatrix()
{
    if (m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->ClearMatrix();
    }
}

//--------------------------------------------------------------------
inline void C3DRolePart::SetTexture()
{
    //_V_BODY
    C3DTexture* pTexture = g_objGameDataSet.Get3DTexture(m_infoPart.idTexture);
    //MYASSERT(pTexture);
    if (pTexture && m_infoPart.p3DMesh)
    {
        m_infoPart.p3DMesh->ChangeTexture(pTexture);
    }
}

//-------------------------------------------------------------------------
inline void C3DRolePart::Set3DEffect(int nLook, int nAction, int nVariable, CGame3DEffectEx* pEffect,  char* pszVmesh)
{
    if (m_nAction == nAction)
    {
        return;
    }
    m_nAction = nAction;
    // 根据 action, variable 取特效索引
    DWORD idxPart = (m_idType % 1000000) / 1000;
    if (pEffect)
    {
        if (!pEffect->TestEffect((char*)g_obj3DRoleData.GetRolePart3DEffect(nLook, nAction, idxPart, nVariable)))
        {
            if (pszVmesh)
            {
                pEffect->DeleteByVMesh(pszVmesh);
            }
            pEffect->Add((char*)g_obj3DRoleData.GetRolePart3DEffect(nLook, nAction, idxPart, nVariable));
        }
    }
    else if (!this->m_objGame3DEffectEx.TestEffect((char*)g_obj3DRoleData.GetRolePart3DEffect(nLook, nAction, idxPart, nVariable)))
    {
        if (pszVmesh)
        {
            m_objGame3DEffectEx.DeleteByVMesh(pszVmesh);
        }
        m_objGame3DEffectEx.Add((char*)g_obj3DRoleData.GetRolePart3DEffect(nLook, nAction, idxPart, nVariable));
    }
}

//-----------------------------------------------------------------------------
void C3DRolePart::SetDefaultMotion(void)
{
    if (m_infoPart.p3DMesh)
    {
        C3DObjInfo* p3DObjInfo = g_objGameDataSet.Get3DObjInfo(m_infoPart.idMesh);
        MYASSERT(p3DObjInfo);
        p3DObjInfo->p3DObj->SetMotion(p3DObjInfo->pDefaultMotion);
    }
}
//-----------------------------------------------------------------------------
void	C3DRolePart::CreateLight(OBJID idLight)
{
    m_nLightIndex = g_obj3DRoleData.GetRolePartLightInfo(idLight);
    LightInfo* pInfo = g_obj3DRoleData.GetLightInfoByIndex(m_nLightIndex);
    if (!pInfo)
    {
        m_nLightIndex = 0;
        return;
    }
    m_dwTotalCost = 0;
    int nAmount = pInfo->m_setLightPartInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        LightInfoPart* pPart = pInfo->m_setLightPartInfo[i];
        if (!pPart)
        {
            m_nLightIndex = 0;
            return;
        }
        m_dwTotalCost += pPart->dwTimeCost;
    }
}
//-----------------------------------------------------------------------------
void	C3DRolePart::ProcessLight()
{
    if (m_nLightIndex == 0)
    {
        m_ucAlpha	=	m_ucSAlpha;
        m_ucRed		=	m_ucSRed;
        m_ucGreen	=	m_ucSGreen;
        m_ucBlue	=	m_ucSBlue;
        return;
    }
    // 当前时段
    DWORD dwCurrentTimer = ::TimeGet() % m_dwTotalCost;
    // 当前比率
    DWORD dwCost = 0;
    DWORD dwEndCost = 0;
    LightInfo* pInfo = g_obj3DRoleData.GetLightInfoByIndex(m_nLightIndex);
    if (!pInfo)
    {
        m_nLightIndex = 0;
        return;
    }
    unsigned char nAmount = pInfo->m_setLightPartInfo.size();
    unsigned char ucOrgAlphaRate = 100;
    unsigned char ucOrgRedRate   = 100;
    unsigned char ucOrgGreenRate = 100;
    unsigned char ucOrgBlueRate	 = 100;
    unsigned char ucCurAlphaRate = 100;
    unsigned char ucCurRedRate   = 100;
    unsigned char ucCurGreenRate = 100;
    unsigned char ucCurBlueRate	 = 100;
    for(int i = 0; i < nAmount; i ++)
    {
        LightInfoPart* pPart = pInfo->m_setLightPartInfo[i];
        if (!pPart)
        {
            m_nLightIndex = 0;
            return;
        }
        dwCost += pPart->dwTimeCost;
        if (dwCurrentTimer < dwCost)
        {
            if (i == 0)
            {
                int kkkkk = 0;
            }
            if (i > 0)
            {
                LightInfoPart* pOrgPart = pInfo->m_setLightPartInfo[i-1];
                if (!pOrgPart)
                {
                    return;
                }
                ucOrgAlphaRate = pOrgPart->ucAlphaRate;
                ucOrgRedRate   = pOrgPart->ucRedRate;
                ucOrgGreenRate = pOrgPart->ucGreenRate;
                ucOrgBlueRate  = pOrgPart->ucBlueRate;
            }
            else
            {
                ucOrgAlphaRate = 100;
                ucOrgRedRate   = 100;
                ucOrgGreenRate = 100;
                ucOrgBlueRate  = 100;
            }
            if (pPart->ucAlphaRate > ucOrgAlphaRate)
            {
                ucCurAlphaRate = (unsigned char )(ucOrgAlphaRate + (pPart->ucAlphaRate - ucOrgAlphaRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            else
            {
                ucCurAlphaRate =(unsigned char )( ucOrgAlphaRate - (ucOrgAlphaRate - pPart->ucAlphaRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            if (pPart->ucRedRate > ucOrgRedRate)
            {
                ucCurRedRate = (unsigned char )(ucOrgRedRate + (pPart->ucRedRate - ucOrgRedRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            else
            {
                ucCurRedRate = (unsigned char )(ucOrgRedRate - (ucOrgRedRate - pPart->ucRedRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            if (pPart->ucGreenRate > ucOrgGreenRate)
            {
                ucCurGreenRate = (unsigned char )(ucOrgGreenRate + (pPart->ucGreenRate - ucOrgGreenRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            else
            {
                ucCurGreenRate = (unsigned char )(ucOrgGreenRate - (ucOrgGreenRate - pPart->ucGreenRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            if (pPart->ucBlueRate > ucOrgBlueRate)
            {
                ucCurBlueRate = (unsigned char )(ucOrgBlueRate + (pPart->ucBlueRate - ucOrgBlueRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            else
            {
                ucCurBlueRate =(unsigned char )( ucOrgBlueRate - (ucOrgBlueRate - pPart->ucBlueRate) * (pPart->dwTimeCost - (dwCost - dwCurrentTimer)) / pPart->dwTimeCost);
            }
            m_ucAlpha = (unsigned char )(m_ucSAlpha * ucCurAlphaRate / 100);
            m_ucRed =(unsigned char )( m_ucSRed * ucCurRedRate / 100);
            m_ucGreen = (unsigned char )(m_ucSGreen * ucCurGreenRate / 100);
            m_ucBlue = (unsigned char )(m_ucSBlue * ucCurBlueRate / 100);
            break;
        }
    }
    if (m_infoPart.p3DMesh)
    {
		float a =(float)(1.0f * m_ucAlpha / 255) ;
		float r =(float)( 1.0f * m_ucRed / 255);
		float g = (float)(1.0f * m_ucGreen / 255);
		float b =  (float)(1.0f * m_ucBlue / 255);
        m_infoPart.p3DMesh->SetARGB (a, r, g,b);
    }
}
