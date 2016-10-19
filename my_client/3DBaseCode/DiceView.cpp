
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DiceView.cpp: implementation of the CDiceView class.
//
//////////////////////////////////////////////////////////////////////

#include "DiceView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiceView::CDiceView()
{
    m_objDesk.Create(999);
    for(int i = 0; i < 3; i ++)
    {
        m_objDiceRoll[i].Create(998);
        m_objDiceStatic[i].Create(997);
        m_dwDice[i] = i;
        m_bDiceRun[i] = false;
        m_bRun = false;
    }
    m_nDir = 0;
    m_bRun = false;
}

CDiceView::~CDiceView()
{
}
//-----------------------------------------------------------------------------------------------------------
BOOL	CDiceView::SetVariable(int nVariableType, DWORD dwData)
{
    switch(nVariableType)
    {
    case _VAR_LOOK:
        this->SetLook((int)dwData);
        return true;
    case _VAR_DICE0:
        m_bRun		 = true;
        m_bDiceRun[0] = true;
        m_dwDice[0] = dwData;
        m_dwDiceEnd[0] = ::TimeGet() + 3000 + ::RandGet(1000);
        m_dwDiceFrame[0] = ::RandGet(16);
        m_dwOffset[0] = 0;
        m_bUp[0] = 0;
        return true;
    case _VAR_DICE1:
        m_bRun		 = true;
        m_bDiceRun[1] = true;
        m_dwDice[1] = dwData;
        m_dwDiceEnd[1] = ::TimeGet() + 3000 + ::RandGet(1000);
        m_dwDiceFrame[1] = ::RandGet(16);
        m_dwOffset[1] = 0;
        m_bUp[1] = 1;
        return true;
    case _VAR_DICE2:
        m_bRun		 = true;
        m_bDiceRun[2] = true;
        m_dwDice[2] = dwData;
        m_dwDiceEnd[2] = ::TimeGet() + 3000 + ::RandGet(1000);
        m_dwDiceFrame[2] = ::RandGet(16);
        m_dwOffset[2] = 0;
        m_bUp[2] = 2;
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------------------------------------
int		CDiceView::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlayerSound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    return 10;
}
//-----------------------------------------------------------------------------------------------------------
void	CDiceView::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    m_nDir = 0;
    int nMyRotate	= -45 * m_nDir - 45;
    m_obj3DSimpleObj.SetPos(nX, nY, nHeight, nMyRotate, fScale);
    const CMyPos posDeskOffset = { -30, 40};
    m_objDesk.SetPos(nX + posDeskOffset.x, nY + posDeskOffset.y, nHeight, nMyRotate, fScale);
    const CMyPos posDiceOffset[3] = {{ -30, 30}, { -20, 34}, { -10, 38}};
    const CMyPos posDiceRollOffset[3] = {{ -30, 30}, { -20, 34}, { -10, 38}};
    for(int i = 0; i < 3; i ++)
    {
        m_objDiceRoll[i].SetPos(nX + posDiceRollOffset[i].x, nY + posDiceRollOffset[i].y, -20 - m_dwOffset[i], nMyRotate, 1.2f);
        m_objDiceStatic[i].SetPos(nX + posDiceOffset[i].x, nY + posDiceOffset[i].y, -20, nMyRotate, 1.2f);
    }
}
//-----------------------------------------------------------------------------------------------------------
void	CDiceView::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
{
    m_obj3DSimpleObj.SetLightOffset(type, x, y, z, a, r, g, b);
    const CMyPos posDeskOffset = { -30, 40};
    m_objDesk.SetLightOffset(type, x + posDeskOffset.x, y + posDeskOffset.y, z, a, r, g, b);
    const CMyPos posDiceOffset[3] = {{ -30, 30}, { -20, 34}, { -10, 38}};
    const CMyPos posDiceRollOffset[3] = {{ -30, 30}, { -20, 34}, { -10, 38}};
    for(int i = 0; i < 3; i ++)
    {
        m_objDiceRoll[i].SetLightOffset(0, x + posDiceRollOffset[i].x, y + posDiceRollOffset[i].y, z, a, r, g, b);
        m_objDiceStatic[i].SetLightOffset(0, x + posDiceOffset[i].x, y + posDiceOffset[i].y, z, a, r, g, b);
    }
}
//-----------------------------------------------------------------------------------------------------------
void	CDiceView::SetRGBA ( float alpha, float red, float green, float blue )
{
    C3DTaskNpc::SetRGBA(alpha, red, green, blue);
    m_objDesk.SetARGB(alpha, red, green, blue);
    for(int i = 0; i < 3; i ++)
    {
        m_objDiceRoll[i].SetARGB(alpha, red, green, blue);
        m_objDiceStatic[i].SetARGB(alpha, red, green, blue);
    }
}
//-----------------------------------------------------------------------------------------------------------
BOOL	CDiceView::HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    return C3DTaskNpc::HitTest(nHitScreenX, nHitScreenY, nMapX, nMapY);
}
//-----------------------------------------------------------------------------------------------------------
BOOL	CDiceView::Draw ( int nMapX, int nMapY )
{
    C3DTaskNpc::Draw(nMapX, nMapY);
    m_objDesk.Draw2BG(nMapX, nMapY);
    for(int i = 0; i < 3; i ++)
    {
        if (m_bDiceRun[i])
        {
            if (m_bUp[i])
            {
                m_dwOffset[i] ++;
                if (m_dwOffset[i] > 10)
                {
                    m_dwOffset[i] = 10;
                    m_bUp[i] = false;
                }
            }
            else
            {
                m_dwOffset[i] --;
                if (m_dwOffset[i] == 0 || m_dwOffset[i] > 200)
                {
                    m_dwOffset[i] = 0;
                    m_bUp[i] = true;
                }
            }
            m_dwDiceFrame[i] ++;
            m_objDiceRoll[i].SetFrame(m_dwDiceFrame[i]);
            m_objDiceRoll[i].Draw2BG(nMapX, nMapY);
            if (m_dwDiceEnd[i] < ::TimeGet())
            {
                m_bDiceRun[i] = false;
            }
        }
        else
        {
            m_objDiceStatic[i].SetFrame(m_dwDice[i] - 1);
            m_objDiceStatic[i].Draw2BG(nMapX, nMapY);
        }
    }
    return true;
}
//-----------------------------------------------------------------------------------------------------------
DWORD	CDiceView::GetVariable(int nVariableType)
{
    return C3DTaskNpc::GetVariable(nVariableType);
}
//-----------------------------------------------------------------------------------------------------------
IRoleView* CDiceView::CreateNewView()
{
    CDiceView* pView = new CDiceView;
    MYASSERT(pView);
    return pView;
}
//-----------------------------------------------------------------------------------------------------------
