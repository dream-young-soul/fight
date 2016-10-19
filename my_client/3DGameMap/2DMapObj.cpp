
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------
//2DMapObj.cpp
//----------------------------------------------------
#pragma warning(disable:4786)
#include "2DMapObj.h"
#include "BaseFunc.h"
#include "3DGameMap.h"
//----------------------------------------------------
C2DMapObj::C2DMapObj()
{
}
//----------------------------------------------------
C2DMapObj::~C2DMapObj()
{
}
//----------------------------------------------------
void C2DMapObj::GetWorldPos(CMyPos& posWorld)
{
    g_objGameMap.Cell2World(m_posCell.x, m_posCell.y, posWorld.x, posWorld.y);
}
//----------------------------------------------------
void C2DMapObj::GetPos(CMyPos& posCell)
{
    posCell = m_posCell;
}
//----------------------------------------------------
void C2DMapObj::SetPos(CMyPos posCell)
{
    m_posCell = posCell;
}
//----------------------------------------------------

