
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicItem.cpp: implementation of the CMagicItem class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)
#include "MagicItem.h"
#include "3DRoleData.h"
//#include ""

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMagicItem::CMagicItem()
{
    strcpy(m_szStart, "NULL");
    strcpy(m_szLast, "NULL");
    strcpy(m_szEnd, "NULL");
    m_dwTimeBegin = ::TimeGet();
    m_nMoment = 0;
}

CMagicItem::~CMagicItem()
{
}

//-------------------------------------------------------------------

BOOL	CMagicItem::Load(int nLook)
{
    MapMagicItemInfo* pInfo = g_obj3DRoleData.GetMapMagicItemInfo(nLook);
    if (pInfo)
    {
        strcpy(m_szStart, pInfo->szStart);
        strcpy(m_szLast, pInfo->szLast);
        strcpy(m_szEnd, pInfo->szEnd);
    }
    m_dwTimeBegin = ::TimeGet();
    return true;
}

//-------------------------------------------------------------------

BOOL	CMagicItem::SetVariable(int nVariableType, DWORD dwData)
{
    switch(nVariableType)
    {
    case _VAR_LOOK:
        this->Load((int)dwData);
        return true;
    case _VAR_MOMENT:
        return true;
    }
    return false;
}

//-------------------------------------------------------------------
int		CMagicItem::SetAction(int nActionType, BOOL bResetMotion, BOOL bPlayerSound, int nWorldX, int nWorldY, BOOL bSetEffect)
{
    return 100;
}

//-------------------------------------------------------------------
void	CMagicItem::SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale )
{
    return;
}

//-------------------------------------------------------------------
void	CMagicItem::SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b )
{
    return;
}

//-------------------------------------------------------------------
void	CMagicItem::SetRGBA ( float alpha, float red, float green, float blue )
{
    return;
}

//-------------------------------------------------------------------
BOOL	CMagicItem::HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY )
{
    return false;
}

//-------------------------------------------------------------------
BOOL	CMagicItem::Draw ( int nMapX, int nMapY )
{
    switch(m_nMoment)
    {
    case 0: // 初始化Tme, 从创建开始播放...
        //		g_objGameMap.AddTerrainMagicUnit();
        break;
    case 1: // 初始化Tme, 从持续开始播放...
        break;
    case 2: // 初始化Tme, 播放结束
        break;
    }
    return true;
}

//-------------------------------------------------------------------
DWORD	CMagicItem::GetVariable(int nVariableType)
{
    return false;
}

//-------------------------------------------------------------------

IRoleView* CMagicItem::CreateNewView()
{
    CMagicItem* pItem = new CMagicItem;
    MYASSERT(pItem);
    return pItem;
}

//-------------------------------------------------------------------
