
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DSimpleObj.cpp: implementation of the C3DSimpleObj class.
//
//////////////////////////////////////////////////////////////////////

#include "3DSimpleObj.h"
#include "3DRoleData.h"
#include "GameDataSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DSimpleObj::C3DSimpleObj()
{
    m_nPosX = m_nPosY = 0;
    m_nRotate = 0;
    m_fScale = 1.0f;
    m_LightOffset.x = 0;
    m_LightOffset.y = 0;
    m_LightOffset.z = -1000;
    m_nShadowType = 2;
    m_a = 1.0f;
    m_r = 0.3f;
    m_g = 0.3f;
    m_b = 0.3f;
    m_bOblique = true;
}

C3DSimpleObj::~C3DSimpleObj()
{
}

//------------------------------------------------------------------
void C3DSimpleObj::SetPos(int nX, int nY, int nHeight, int nRotate, float fScale)
{
    m_nPosX = nX;
    m_nPosY = nY;
    m_nRotate = nRotate;
    m_fScale = fScale;
    m_nPosH = nHeight * 2;
}

//------------------------------------------------------------------
BOOL C3DSimpleObj::Draw2BG(int nMapX, int nMapY)
{
    if (CMyBitmap::s_nShowMode != modeMesh)
    {
        C3DObj::Prepare();
    }
    float br = D3DXToRadian ( ( float )( m_nRotate % 45 ) );
    _D3DXMATRIXA16 bmatrix;
    D3DXMatrixIdentity ( &bmatrix );
    Common_Translate ( &bmatrix, 0, 0, (float)m_nPosH );
    Common_Rotate ( &bmatrix, 0, 0, br );
    Common_Scale ( &bmatrix, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.3f );
    Common_Translate ( &bmatrix, (float)m_nPosX, 0, (float)m_nPosY );
    D3DXVECTOR3 min, max;
    this->TransformCoordforSimpleObj(&min, &max, &bmatrix);
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
    // temp
    if ((max.z  - min.z) < 1)
    {
        max.z += 1;
    }
    RECT rect = { (LONG)min.x, (LONG)min.z, (LONG)max.x, (LONG)max.z };
    if (rect.left == rect.right)
    {
        rect.right ++;
    }
    if (rect.top == rect.bottom)
    {
        rect.bottom ++;
    }
    RECT mode = { nMapX, nMapY, _SCR_WIDTH + nMapX, _SCR_HEIGHT + nMapY };
    if ( !IntersectRect ( &rect, &mode, &rect ) )
    {
        return false;
    }
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Translate ( &matrix, 0, 0, (float)m_nPosH );
    Common_Rotate ( &matrix, 0, 0, D3DXToRadian ( ( float )m_nRotate ) );
    if (m_bOblique)
    {
        Common_Rotate ( &matrix, D3DXToRadian ( -45 ), 0, 0 );
    }
    Common_Scale ( &matrix, m_fScale, m_fScale, m_fScale );
    Common_Translate ( &matrix,(float) m_nPosX, 0, (float)m_nPosY );
    //this->SetDefaultMotion();
    this->SetTexture();
    this->MuliplyPhy(&matrix);
    C3DRolePart::Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
    C3DRolePart::DrawAlpha(m_nShadowType, (float)m_LightOffset.x, (float)m_LightOffset.y, (float)m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH);
    this->ClearMatrix();
    return true;
}
//------------------------------------------------------------------
BOOL C3DSimpleObj::HitTest(int nHitScreenX, int nHitScreenY, int nMapX, int nMapY)
{
    float br = D3DXToRadian ( ( float )( m_nRotate % 45 ) );
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Rotate ( &matrix, 0, 0, br );
    Common_Scale ( &matrix, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.3f );
    Common_Translate ( &matrix, (float)m_nPosX, 0,(float) m_nPosY );
    D3DXVECTOR3 min, max;
    this->TransformCoordforSimpleObj(&min, &max, &matrix);
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
    from.y = -1000;
    from.z = (float)(nMapY + _SCR_HEIGHT / 2);
    to.x = (float)(nHitScreenX + nMapX);
    to.y = 0;
    to.z = (float)(nHitScreenY + nMapY);
    dir = to - from;
    D3DXVec3Normalize ( &dir, &dir );
    return D3DXBoxBoundProbe ( &min, &max, &from, &dir );
}
//------------------------------------------------------------------
void C3DSimpleObj::SetLightOffset(int type, float x, float y, float z, float a, float r, float g, float b)
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

//------------------------------------------------------------------
C3DSimpleObj* C3DSimpleObj::CreateNew(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return NULL;
    }
    C3DSimpleObj* pData = new C3DSimpleObj;
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
BOOL C3DSimpleObj::Create(OBJID idType)
{
    if (idType == ID_NONE)
    {
        return false;
    }
    this->Destroy();
    C3DSimpleObjInfo* pInfo = g_obj3DRoleData.Get3DSimpleObjInfo(idType);
    if (!pInfo || pInfo->nParts <= 0)
    {
        return false;
    }
    m_infoPart.idMesh = pInfo->idMesh[0];
    m_infoPart.idTexture = pInfo->idTexture[0];
    m_infoPart.p3DMesh = g_objGameDataSet.Get3DObj(m_infoPart.idMesh);
    this->SetDefaultMotion();
    return true;
}
//--------------------------------------------------------------------
void C3DSimpleObj::Create(OBJID idMesh, OBJID idTexture)
{
    m_infoPart.idMesh = idMesh;
    m_infoPart.idTexture = idTexture;
    m_infoPart.p3DMesh = g_objGameDataSet.Get3DObj(m_infoPart.idMesh);
    this->SetDefaultMotion();
}
