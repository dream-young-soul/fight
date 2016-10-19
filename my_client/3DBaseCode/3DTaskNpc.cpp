
//**********************************************************
// 代码编辑器
//**********************************************************

// 3DTaskNpc.cpp: implementation of the C3DTaskNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "3DTaskNpc.h"
#include "3DRoleData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DTaskNpc::C3DTaskNpc()
{
    m_nLook	=	0;
    m_nDir	=	-1;
    m_idStandByMotion	= ID_NONE;
    m_idRestMotion		= ID_NONE;
    m_idBlazeMotion		= ID_NONE;
    m_dwTimer = ::TimeGet();
    m_nFrameAmount = 1;
}

C3DTaskNpc::~C3DTaskNpc()
{
}
//-------------------------------------------------------------------------------------------------------
BOOL C3DTaskNpc::SetVariable(int nVariableType, DWORD dwData)
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
    case _VAR_DIRECT_DIR:
        m_nDir = dwData;
        return true;
    case _VAR_CLICK:
        {
            if (m_idCurrentMotion != m_idBlazeMotion)
            {
                // 激发
                m_idCurrentMotion = m_idBlazeMotion;
                C3DMotion* pMotion = g_objGameDataSet.Get3DMotion(m_idBlazeMotion);
                if (pMotion)
                {
                    m_nFrameAmount = pMotion->GetFrameAmount();
                    m_obj3DSimpleObj.SetMotion(pMotion);
                }
                m_dwTimer = ::TimeGet();
            }
        }
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------
int C3DTaskNpc::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlayerSound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    return 10;
}
//-------------------------------------------------------------------------------------------------------
void C3DTaskNpc::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    int nMyRotate	= -45 * m_nDir - 45;
    m_obj3DSimpleObj.SetPos(nX, nY, nHeight, nMyRotate, fScale);
}
//-------------------------------------------------------------------------------------------------------
void C3DTaskNpc::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
{
    m_obj3DSimpleObj.SetLightOffset(type, x, y, z, a, r, g, b);
}
//-------------------------------------------------------------------------------------------------------
void C3DTaskNpc::SetRGBA ( float alpha, float red, float green, float blue )
{
    m_obj3DSimpleObj.SetARGB(alpha, red, green, blue);
}
//-------------------------------------------------------------------------------------------------------
BOOL C3DTaskNpc::HitTest (int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    return m_obj3DSimpleObj.HitTest(nHitScreenX, nHitScreenY, nMapX, nMapY);
}
//-------------------------------------------------------------------------------------------------------
BOOL C3DTaskNpc::Draw ( int nMapX, int nMapY )
{
    const DWORD dwFrameInterval = 33;
    CMyPos posMouse;
    int nEvent = ::MouseCheck(posMouse.x, posMouse.y);
    if (m_idCurrentMotion != m_idBlazeMotion  && m_obj3DSimpleObj.HitTest(posMouse.x, posMouse.y, nMapX, nMapY)
            && nEvent == _MOUSE_CLICK)
    {
        // 激发
        m_idCurrentMotion = m_idBlazeMotion;
        C3DMotion* pMotion = g_objGameDataSet.Get3DMotion(m_idBlazeMotion);
        if (pMotion)
        {
            m_nFrameAmount = pMotion->GetFrameAmount();
            m_obj3DSimpleObj.SetMotion(pMotion);
        }
        m_dwTimer = ::TimeGet();
    }
    // 取祯数
    int nFrameIndex = (::TimeGet() - m_dwTimer) / dwFrameInterval;
    if ((nFrameIndex >= m_nFrameAmount))
    {
        m_dwTimer = ::TimeGet();
        nFrameIndex = 0;
        OBJID idMotion = m_idStandByMotion;
        if (::RandGet(30) == 15)
        {
            idMotion = m_idRestMotion;
        }
        C3DMotion* pMotion = g_objGameDataSet.Get3DMotion(idMotion);
        m_idCurrentMotion = idMotion;
        if (pMotion)
        {
            m_nFrameAmount = pMotion->GetFrameAmount();
            m_obj3DSimpleObj.SetMotion(pMotion);
        }
    }
    m_obj3DSimpleObj.SetMotion(g_objGameDataSet.Get3DMotion(m_idCurrentMotion));
    //m_obj3DSimpleObj.SetDefaultMotion();
    m_obj3DSimpleObj.SetFrame(nFrameIndex);
    return m_obj3DSimpleObj.Draw2BG(nMapX, nMapY);
}
//-------------------------------------------------------------------------------------------------------
DWORD C3DTaskNpc::GetVariable(int nVariableType)
{
    switch(nVariableType)
    {
    case _VAR_LOOK:
        return (m_nLook / 10) * 10 + m_nDir % 8;
    case _VAR_DIR:
        return m_nDir;
    default:
        return false;
    }
}
//-------------------------------------------------------------------------------------------------------
IRoleView* C3DTaskNpc::CreateNewView()
{
    C3DTaskNpc* pNpc = new C3DTaskNpc;
    MYASSERT(pNpc);
    return pNpc;
}
//-------------------------------------------------------------------------------------------------------
void C3DTaskNpc::SetLook(int nLook)
{
    m_nLook = nLook;
    // 通过LOOK取必要信息...
    NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(nLook / 10);
    MYASSERT(pInfo);
    m_idStandByMotion	=	pInfo->idMotionStandBy;
    m_idRestMotion		=	pInfo->idMotionRest;
    m_idBlazeMotion		=	pInfo->idMotionBlaze;
    m_idCurrentMotion = m_idStandByMotion;
    m_nDir = nLook % 10;
    m_idSimpleObj = pInfo->idSimpleObj;
    m_nZoomPercent = pInfo->nZoomPercent;
    strcpy(m_szName, pInfo->szNpcName);
    m_obj3DSimpleObj.Create(pInfo->idSimpleObj);
}
//-------------------------------------------------------------------------------------------------------
