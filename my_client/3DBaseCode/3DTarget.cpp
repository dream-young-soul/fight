
//**********************************************************
// 代码编辑器
//**********************************************************

// 3DTarget.cpp: implementation of the C3DTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "3DTarget.h"
#include "3DRoleData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DTarget::C3DTarget()
{
    m_nLook	=	0;
    m_nDir	=	-1;
    m_idStandByMotion	=	ID_NONE;
    m_idWoundMotion		=	ID_NONE;
    m_bHit = false;
    m_dwTimer = ::TimeGet();
    m_nFrameAmount = 1;
}

C3DTarget::~C3DTarget()
{
}
//-------------------------------------------------------------------------------------------------------
BOOL C3DTarget::SetVariable(int nVariableType, DWORD dwData)
{
    switch(nVariableType)
    {
    case _VAR_LOOK:
        this->SetLook((int)dwData);
        return true;
    case _VAR_DIR:
        if (m_nDir == -1)
        {
            m_nDir = dwData;
        }
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------
int C3DTarget::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlayerSound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    if (nActionType == _ACTION_BRUISE0)
    {
        m_bHit = true;
    }
    return 10;
}
//-------------------------------------------------------------------------------------------------------
void C3DTarget::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    int nMyRotate	= -45 * m_nDir - 45;
    m_obj3DSimpleObj.SetPos(nX, nY, nHeight, nMyRotate, fScale);
}
//-------------------------------------------------------------------------------------------------------
void C3DTarget::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
{
    m_obj3DSimpleObj.SetLightOffset(type, x, y, z, a, r, g, b);
}
//-------------------------------------------------------------------------------------------------------
void C3DTarget::SetRGBA ( float alpha, float red, float green, float blue )
{
    m_obj3DSimpleObj.SetARGB(alpha, red, green, blue);
}
//-------------------------------------------------------------------------------------------------------
BOOL C3DTarget::HitTest (int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    return m_obj3DSimpleObj.HitTest(nHitScreenX, nHitScreenY, nMapX, nMapY);
}
//-------------------------------------------------------------------------------------------------------
BOOL C3DTarget::Draw ( int nMapX, int nMapY )
{
    const DWORD dwFrameInterval = 33;
    // 取祯数
    int nFrameIndex = (::TimeGet() - m_dwTimer) / dwFrameInterval;
    if ((nFrameIndex >= m_nFrameAmount) && m_bHit)
    {
        m_dwTimer = ::TimeGet();
        nFrameIndex = 0;
        OBJID idMotion = m_idStandByMotion;
        if (m_bHit)
        {
            idMotion = m_idWoundMotion;
            m_bHit = false;
        }
        C3DMotion* pMotion = g_objGameDataSet.Get3DMotion(idMotion);
        if (pMotion)
        {
            m_nFrameAmount = pMotion->GetFrameAmount();
            m_obj3DSimpleObj.SetMotion(pMotion);
        }
        m_idCurrentMotion = idMotion;
    }
    m_obj3DSimpleObj.SetMotion(g_objGameDataSet.Get3DMotion(m_idCurrentMotion));
    m_obj3DSimpleObj.SetFrame(nFrameIndex);
    return m_obj3DSimpleObj.Draw2BG(nMapX, nMapY);
}
//-------------------------------------------------------------------------------------------------------
DWORD C3DTarget::GetVariable(int nVariableType)
{
    return 0;
}
//-------------------------------------------------------------------------------------------------------
IRoleView* C3DTarget::CreateNewView()
{
    C3DTarget* pTarget = new C3DTarget;
    MYASSERT(pTarget);
    return pTarget;
}
//-------------------------------------------------------------------------------------------------------

void C3DTarget::SetLook(int nLook)
{
    m_nLook = nLook;
    // 通过LOOK取必要信息...
    NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(nLook / 10);
    MYASSERT(pInfo);
    m_idStandByMotion = pInfo->idMotionStandBy;
    m_idCurrentMotion = pInfo->idMotionStandBy;
    m_idWoundMotion = pInfo->idMotionBlaze;
    m_idSimpleObj = pInfo->idSimpleObj;
    strcpy(m_szName, pInfo->szNpcName);
    m_obj3DSimpleObj.Create(pInfo->idSimpleObj);
    m_nDir = nLook % 10;
}
//-------------------------------------------------------------------------------------------------------
