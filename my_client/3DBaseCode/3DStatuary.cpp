
//**********************************************************
// 代码编辑器
//**********************************************************

// 3DStatuary.cpp: implementation of the C3DStatuary class.
//
//////////////////////////////////////////////////////////////////////

#include "3DStatuary.h"
#include "GameDataSet.h"
#include "3DRoleData.h"
#include "../ndsound/ndsound.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DStatuary::C3DStatuary()
{
    m_nActionType	=	-1;
    m_fScale		=	-1;
    m_nFrame		=	-1;
    m_nDir			=	-1;
}

C3DStatuary::~C3DStatuary()
{
}
//-----------------------------------------------------------------------
BOOL C3DStatuary::SetVariable(int nVariableType, DWORD dwData)
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
        if (m_nFrame == -1)
        {
            m_nFrame = dwData;
            this->SetFrame(dwData);
        }
        return true;
    case _VAR_ROTATEX:
        this->SetRotateX(dwData);
        return true;
    case _VAR_DIR:
        if (m_nDir == -1)
        {
            m_nDir = dwData;
        }
        return true;
    case _VAR_HEAD:
        this->SetHead(dwData);
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------
int C3DStatuary::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlaySound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    if (m_nActionType == -1)
    {
        m_nActionType = nActionType;
    }
    else
    {
        nActionType = m_nActionType;
    }
    bPlaySound = bSetEffect = false;
    MYASSERT (m_pArmor);
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
                const OBJID idActionError =  500000;
                ::ErrorOut(g_objGameDataSet.GetStr(idActionError), idBodyMotion, idDefaultMotion);
                return 0;
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
            m_pArmor->Set3DEffect(m_nLook, nActionType, m_pArmor->GetTypeID() % 1000);
        }
        m_pArmor->SetMotion(m_pMotionArmor);
    }
    // Mount motion
    if (m_pMount)
    {
        if (bSetEffect)
        {
            m_pMount->Set3DEffect(m_nLook, nActionType, m_pMount->GetTypeID() % 1000);
        }
    }
    // Armet motion
    if (m_pArmet)
    {
        if (bSetEffect)
        {
            m_pArmet->Set3DEffect(m_nLook, nActionType, m_pArmet->GetTypeID() % 1000);
        }
    }
    // right weapon
    if (m_pRWeapon)
    {
        if (bSetEffect)
        {
            m_pRWeapon->Set3DEffect(m_nLook, nActionType, m_pRWeapon->GetTypeID() % 1000);
        }
        if (bResetMotion || !m_pMotionWeaponR)
        {
            m_pMotionWeaponR = this->GetWeaponMotion(m_pRWeapon->GetTypeID(), nActionType);
        }
        if (m_pMotionWeaponR)
        {
            m_pRWeapon->SetMotion(m_pMotionWeaponR);
        }
        else
        {
            m_pRWeapon->SetDefaultMotion();
        }
    }
    // left weapon
    if (m_pLWeapon)
    {
        if (bSetEffect)
        {
            m_pLWeapon->Set3DEffect(m_nLook, nActionType, m_pLWeapon->GetTypeID() % 1000);
        }
        // 为了加快速度 做一些判断
        // 左手武器只有弓类武器有Motion
        if (m_pLWeapon->GetTypeID() / 1000 == 500)
        {
            if (bResetMotion || !m_pMotionWeaponL)
            {
                m_pMotionWeaponL = this->GetWeaponMotion(m_pLWeapon->GetTypeID(), nActionType);
            }
            if (m_pMotionWeaponL)
            {
                m_pLWeapon->SetMotion(m_pMotionWeaponL);
            }
            else
            {
                m_pLWeapon->SetDefaultMotion();
            }
        }
    }
    // clone armor
    if (m_pArmorClone)
    {
        return this->SetCloneAction(nActionType, bPlaySound, nWorldX, nWorldY, bSetEffect);
    }
    // player sound
    /*	if (bPlaySound)
    	{
    		const char* pszSound = g_obj3DRoleData.GetActionSound(m_nLook, this->GetWeaponActionData(), nActionType);
    		if (pszSound)
    		{
    			try {
    				// player sound here
    				const int nRange = 3000;
    				::DXPlaySound ((char*)pszSound, nWorldX, nWorldY, nRange);
    			}catch(...)
    			{
    				::LogMsg("dxplayersound play action sound:%s", pszSound);
    			}
    		}
    	}
    */
    return m_pMotionArmor->GetFrameAmount();
}
//-----------------------------------------------------------------------
void C3DStatuary::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    m_nPosX = nX;
    m_nPosY = nY;
    m_nRotateZ = -45 * m_nDir - 45;
    m_fScale = fScale;
    m_nPosH = nHeight * 2;
}
//-----------------------------------------------------------------------
void C3DStatuary::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
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
//-----------------------------------------------------------------------
void C3DStatuary::SetRGBA ( float alpha, float red, float green, float blue )
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
}
//-----------------------------------------------------------------------
BOOL C3DStatuary::HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
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
    from.x =(float)( nMapX + _SCR_WIDTH / 2);
    from.y = -1000;
    from.z =(float)( nMapY + _SCR_HEIGHT / 2);
    to.x = (float)(nHitScreenX + nMapX);
    to.y = 0;
    to.z = (float)(nHitScreenY + nMapY);
    dir = to - from;
    D3DXVec3Normalize ( &dir, &dir );
    return D3DXBoxBoundProbe ( &min, &max, &from, &dir );
}
//-----------------------------------------------------------------------
BOOL C3DStatuary::Draw ( int nMapX, int nMapY )
{
    this->SetRGBA(1.0, 0.5, 0.5, 0.5);
    this->SetFrame(m_nFrame);
    if (CMyBitmap::s_nShowMode != modeMesh)
    {
        C3DObj::Prepare();
    }
    float br = D3DXToRadian ( ( float )( m_nRotateZ % 45 ) );
    _D3DXMATRIXA16 bmatrix;
    D3DXMatrixIdentity ( &bmatrix );
    Common_Translate ( &bmatrix, 0, 0, (float)m_nPosH );
    Common_Rotate ( &bmatrix, 0, 0, br );
    //Common_Rotate ( &bmatrix, D3DXToRadian ( -45 ), 0, 0 );
    Common_Scale ( &bmatrix, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.2f, m_fScale - m_fScale * 0.3f );
    Common_Translate ( &bmatrix, (float)m_nPosX, 0, (float)m_nPosY );
    D3DXVECTOR3 min, max;
    m_pArmor->TransformCoord(&min, &max, &bmatrix);
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
    RECT rect = { (LONG)min.x, (LONG)min.z, (LONG)max.x, (LONG)max.z };
    RECT mode = { nMapX, nMapY, _SCR_WIDTH + nMapX, _SCR_HEIGHT + nMapY };
    /*if ( !IntersectRect ( &rect, &mode, &rect ) )
    	return false;*/
    _D3DXMATRIXA16 matrix;
    D3DXMatrixIdentity ( &matrix );
    Common_Translate ( &matrix, 0, 0,(float) m_nPosH );
    Common_Rotate ( &matrix, 0, 0, D3DXToRadian ( ( float )m_nRotateZ ) );
    Common_Rotate ( &matrix, D3DXToRadian ( m_nRotateX ), 0, 0 );
    Common_Scale ( &matrix, m_fScale, m_fScale, m_fScale );
    Common_Translate ( &matrix, (float)m_nPosX, 0, (float)m_nPosY );
    // 如果有复制体，只画复制体
    if (m_pArmorClone)
    {
        m_pArmorClone->SetTexture();
        m_pArmorClone->MuliplyPhy(&matrix);
        m_pArmorClone->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
    }
    else
    {
        // 先画身体
        if (m_pArmor)
        {
            m_pArmor->SetTexture();
            m_pArmor->MuliplyPhy(&matrix);
            m_pArmor->Draw(m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b);
        }
        // 画武器
        if (m_pLWeapon )
        {
            // 武器跟随 v_l_weapon
            m_pLWeapon->SetTexture();
            m_pLWeapon->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_l_weapon"));// v_l_shield v_l_weapon
            m_pLWeapon->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
        }
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
        }
        if (m_pMount)
        {
            // 坐骑跟随 v_pet
            m_pMount->SetTexture();
            m_pMount->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_pet"));
            m_pMount->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
        }
        if (m_pArmet)
        {
            // 头盔跟随 v_armet
            m_pArmet->SetTexture();
            m_pArmet->SetVirtualMotion(m_pArmor->GetVirtualMotion("v_armet"));
            m_pArmet->Draw ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b );
        }
    }
    // 如果有复制体，只画复制体alpha
    if (m_pArmorClone)
    {
        m_pArmorClone->SetTexture();
        m_pArmorClone->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
    }
    else
    {
        // 先画身体 alpha
        if (m_pArmor)
        {
            m_pArmor->SetTexture();
            m_pArmor->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
        }
        // 画武器 alpha
        if (m_pLWeapon)
        {
            m_pLWeapon->SetTexture();
            m_pLWeapon->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
        }
        if (m_pRWeapon)
        {
            m_pRWeapon->SetTexture();
            m_pRWeapon->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
        }
        if (m_pMount)
        {
            m_pMount->SetTexture();
            m_pMount->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b,(float) m_nPosH );
        }
        if (m_pArmet)
        {
            m_pArmet->SetTexture();
            m_pArmet->DrawAlpha ( m_nShadowType, m_LightOffset.x, m_LightOffset.y, m_LightOffset.z, m_a, m_r, m_g, m_b, (float)m_nPosH );
        }
    }
    return true;
}
//-----------------------------------------------------------------------
DWORD C3DStatuary::GetVariable(int nVariableType)
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
    }
    return 0;
}
//-----------------------------------------------------------------------
IRoleView* C3DStatuary::CreateNewView()
{
    C3DStatuary* pStatuary = new C3DStatuary;
    MYASSERT(pStatuary);
    return pStatuary;
}
//-----------------------------------------------------------------------
