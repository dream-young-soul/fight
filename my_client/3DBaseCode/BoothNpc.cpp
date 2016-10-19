
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// BoothNpc.cpp: implementation of the CBoothNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "BoothNpc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoothNpc::CBoothNpc()
{
    m_dwItem1 = m_dwItem0 = ID_NONE;
}

CBoothNpc::~CBoothNpc()
{
}
//----------------------------------------------------------------------------------
BOOL CBoothNpc::HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    CMyPos posNpc;
    posNpc.x = nHitScreenX;
    posNpc.y = nHitScreenY;
    for(int i = -4; i < 3; i ++)
    {
        for(int j = -1; j < 4; j ++)
        {
            int nX = nMapX + j;
            int nY = nMapY + i;
            if (nX == nHitScreenX && nY == nHitScreenY)
            {
                return true;
            }
        }
    }
    return false;
}
//----------------------------------------------------------------------------------
IRoleView* CBoothNpc::CreateNewView()
{
    CBoothNpc* pNpc  = new CBoothNpc;
    MYASSERT(pNpc);
    return pNpc;
}
//----------------------------------------------------------------------------------
BOOL CBoothNpc::Draw (int nMapX, int nMapY)
{
    return true;
    return C3DTaskNpc::Draw(nMapX, nMapY);
}
//----------------------------------------------------------------------------------
BOOL CBoothNpc::SetVariable(int nVariableType, DWORD dwData)
{
    switch(nVariableType)
    {
    case _VAR_ITEM0:
        m_dwItem0 = nVariableType;
        break;
    case _VAR_ITEM1:
        m_dwItem1 = nVariableType;
        break;
    default:
        return C3DTaskNpc::SetVariable(nVariableType, dwData);
        break;
    }
    return true;
}
//----------------------------------------------------------------------------------
