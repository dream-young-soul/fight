
//**********************************************************
// 代码编辑器
//**********************************************************

// SynStone.cpp: implementation of the CSynStone class.
//
//////////////////////////////////////////////////////////////////////

#include "SynStone.h"
#include "3DRoleData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSynStone::CSynStone()
{
    m_nLook	=	0;
    m_nDir	=	-1;
}

CSynStone::~CSynStone()
{
}
//--------------------------------------------------------------------
IRoleView* CSynStone::CreateNewView()
{
    CSynStone* pStone = new CSynStone;
    MYASSERT(pStone);
    return pStone;
}
//--------------------------------------------------------------------
void CSynStone::SetLook(int nLook)
{
    const char szIniFile[] = "ini/npcex.ini";
    char szTitle[64];
    char szSubTitle[64];
    sprintf(szTitle, "NpcType%d", nLook / 10);
    m_nDir = nLook % 10;
    strcpy(szSubTitle, "Amount");
    ::IniDataGet(szIniFile, szTitle, szSubTitle, m_nAmount, true);
    if (m_nAmount > _TARGET_LOOK_AMOUNT)
    {
        m_nAmount = _TARGET_LOOK_AMOUNT;
    }
    for(int i = 0; i < m_nAmount; i ++)
    {
        sprintf(szSubTitle, "Var%d", i);
        ::IniDataGet(szIniFile, szTitle, szSubTitle, m_nVarSet[i], true);
        sprintf(szSubTitle, "Look%d", i);
        ::IniDataGet(szIniFile, szTitle, szSubTitle, m_nLookSet[i], true);
    }
    this->SetTrueLook(m_nLookSet[0]);
}
//--------------------------------------------------------------------
void CSynStone::SetTrueLook(int nLook)
{
    if (nLook != m_nLook)
    {
        m_nLook = nLook;
        // 通过LOOK取必要信息...
        NpcTypeInfo* pInfo = g_obj3DRoleData.GetNpcTypeInfo(nLook / 10);
        MYASSERT(pInfo);
        m_nDir = nLook % 10;
        m_idSimpleObj = pInfo->idSimpleObj;
        strcpy(m_szName, pInfo->szNpcName);
        m_obj3DSimpleObj.Create(pInfo->idSimpleObj);
    }
}
//--------------------------------------------------------------------
BOOL CSynStone::SetVariable(int nVariableType, DWORD dwData)
{
    switch(nVariableType)
    {
    case _VAR_LOOK:
        this->SetLook((int)dwData);
        return true;
    case _VAR_MAX_LIFE:
        this->SetMaxLife(dwData);
        return true;
    case _VAR_LIFE:
        this->SetLife(dwData);
        return true;
    }
    return false;
}
//--------------------------------------------------------------------
void CSynStone::SetLife(DWORD dwLife)
{
    m_dwLife = dwLife;
    for(int i = m_nAmount - 1; i >= 0; i--)
    {
        int nVar = 100;
        if (m_dwMaxLife == 0)
        {
            nVar = 100;
        }
        else
        {
            nVar = m_dwLife * 100 / m_dwMaxLife;
        }
        if (nVar >= 100)
        {
            nVar = 100;
        }
        if (nVar <= m_nVarSet[i])
        {
            int nLook =  m_nLookSet[i];
            this->SetTrueLook(nLook);
            return;
        }
    }
}
//-------------------------------------------------------------------------------------------------------
void CSynStone::SetMaxLife(DWORD dwLife)
{
    m_dwMaxLife = dwLife;
    for(int i = m_nAmount - 1; i >= 0; i--)
    {
        int nVar = 100;
        if (m_dwMaxLife == 0)
        {
            nVar = 100;
        }
        else
        {
            nVar = m_dwLife * 100 / m_dwMaxLife;
        }
        if (nVar >= 100)
        {
            nVar = 100;
        }
        if (nVar <= m_nVarSet[i])
        {
            int nLook =  m_nLookSet[i];
            this->SetTrueLook(nLook);
            return;
        }
    }
}
//-------------------------------------------------------------------------------------------------------
void	CSynStone::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    int nMyRotate	= -45 * m_nDir - 45;
    m_obj3DSimpleObj.SetPos(nX, nY, nHeight, nMyRotate, fScale);
}
//-------------------------------------------------------------------------------------------------------
void	CSynStone::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
{
    m_obj3DSimpleObj.SetLightOffset(type, x, y, z, a, r, g, b);
}
//-------------------------------------------------------------------------------------------------------
void	CSynStone::SetRGBA ( float alpha, float red, float green, float blue )
{
    m_obj3DSimpleObj.SetARGB(alpha, red, green, blue);
}
//-------------------------------------------------------------------------------------------------------
BOOL	CSynStone::HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    return m_obj3DSimpleObj.HitTest(nHitScreenX, nHitScreenY, nMapX, nMapY);
}
//-------------------------------------------------------------------------------------------------------
BOOL  	CSynStone::Draw ( int nMapX, int nMapY )
{
    return m_obj3DSimpleObj.Draw2BG(nMapX, nMapY);
}
//-------------------------------------------------------------------------------------------------------
DWORD	CSynStone::GetVariable(int nVariableType)
{
    switch(nVariableType)
    {
    case _VAR_LOOK:
        return (m_nLook / 10) * 10 + m_nDir % 8;
    default:
        return false;
    }
}
//-------------------------------------------------------------------------------------------------------
int	CSynStone::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlayerSound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    return 5;
}
