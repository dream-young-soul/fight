
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TerrainNpcView.cpp: implementation of the CTerrainNpcView class.
//
//////////////////////////////////////////////////////////////////////

#include "TerrainNpcView.h"
#include "3DGameMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrainNpcView::CTerrainNpcView()
{
    m_idNpc = ID_NONE;
}

CTerrainNpcView::~CTerrainNpcView()
{
    g_objGameMap.DeleteTerrainNpc(m_idNpc);
}

//-----------------------------------------------------------------------------------------------
void	CTerrainNpcView::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    /*	CMyPos posCell, posWorld;
    	g_objGameMap.Bg2World(nX, nY, posWorld.x, posWorld.y);
    	g_objGameMap.World2Cell(posWorld.x, posWorld.y, posCell.x, posCell.y);
    	g_objGameMap.SetTerrainNpcPos(m_idNpc, posCell);*/
}
//-----------------------------------------------------------------------------------------------
void	CTerrainNpcView::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
{
    return;
}
//-----------------------------------------------------------------------------------------------
void	CTerrainNpcView::SetRGBA ( float alpha, float red, float green, float blue )
{
    return;
}
//-----------------------------------------------------------------------------------------------
BOOL	CTerrainNpcView::HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    return g_objGameMap.IsTerrainNpcFocus(m_idNpc);
}
//-----------------------------------------------------------------------------------------------
BOOL	CTerrainNpcView::Draw ( int nMapX, int nMapY )
{
    return true;
}
//-----------------------------------------------------------------------------------------------
DWORD	CTerrainNpcView::GetVariable(int nVariableType)
{
    return 0;
}
//-----------------------------------------------------------------------------------------------
int		CTerrainNpcView::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlayerSound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    return 100;
}
//-----------------------------------------------------------------------------------------------
BOOL	CTerrainNpcView::SetVariable(int nVariableType, DWORD dwData)
{
    return false;
}
//-----------------------------------------------------------------------------------------------
IRoleView* CTerrainNpcView::CreateNewView(int nLook, OBJID idNpc, CMyPos posCell)
{
    CTerrainNpcView* pView = new CTerrainNpcView;
    MYASSERT(pView);
    if (pView->Create(idNpc))
    {
        g_objGameMap.AddTerrainNpc(nLook / 10, nLook % 10, idNpc, posCell);
    }
    return pView;
}
//-----------------------------------------------------------------------------------------------
BOOL	CTerrainNpcView::Create(OBJID idNpc)
{
    if (idNpc == ID_NONE)
    {
        return false;
    }
    m_idNpc = idNpc;
    return true;
}
