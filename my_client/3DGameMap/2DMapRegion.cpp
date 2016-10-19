
//**********************************************************
// 代码编辑器
//**********************************************************

// 2DMapRegion.cpp: implementation of the C2DMapRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "2DMapRegion.h"
#include "3DGameMap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DMapRegion::C2DMapRegion()
{
    ZeroMemory(&m_InfoRegion, sizeof(RegionInfo));
}

C2DMapRegion::~C2DMapRegion()
{
}
//-----------------------------------------------------------------------
void C2DMapRegion::Show(void* pInfo)
{
    for(int j = m_InfoRegion.m_posCell.x; j < (int)(m_InfoRegion.m_posCell.x + m_InfoRegion.m_dwCx - 1); j++)
    {
        for(int i = m_InfoRegion.m_posCell.y; i < (int)(m_InfoRegion.m_posCell.y + m_InfoRegion.m_dwCy - 1); i++)
        {
            CellInfo* pInfo = g_objGameMap.GetCell(i, j);
            if (pInfo)
            {
                //				if ((j ==  m_posCell.y) || (j == nEndY-1) || (i ==  m_posCell.x) || (i == nEndX-1))
                {
                    CMyPos posScr;
                    g_objGameMap.Cell2Screen(j, i, posScr.x, posScr.y);
                    CMySize sizeFont;
                    CMyBitmap::GetFontSize(sizeFont);
                    g_objGameMap.MapScaleShowPos(posScr.x, posScr.y);
                    posScr.x -= sizeFont.iWidth / 2;
                    posScr.y -= sizeFont.iHeight / 2;
                    CMyBitmap::ShowString(posScr.x, posScr.y, 0xffff00, "⊙");
                }
            }
        }
    }
    CMyPos posScr;
    g_objGameMap.Cell2Screen(m_InfoRegion.m_posCell.x, m_InfoRegion.m_posCell.y, posScr.x, posScr.y);
    g_objGameMap.MapScaleShowPos(posScr.x, posScr.y);
    // 显示重心点
    CMyPos posA, posB, posC, posD;
    posA.x = posScr.x - _CELL_WIDTH * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
    posA.y = posScr.y;
    posB.x = posScr.x;
    posB.y = posScr.y - _CELL_HEIGHT * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
    posC.x = posScr.x;
    posC.y = posScr.y + _CELL_HEIGHT * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
    posD.x = posScr.x + _CELL_WIDTH * g_objGameMap.GetScale() / _NORMAL_SCALE / 2;
    posD.y = posScr.y;
    CMyBitmap::ShowBlock(posA, posB, posC, posD, 0xff00ff00);
}
//-----------------------------------------------------------------------
C2DMapRegion* C2DMapRegion::CreateNew(int x, int y, int cx, int cy)
{
    C2DMapRegion* pRegion = new C2DMapRegion;
    MYASSERT(pRegion);
    RegionInfo* pInfo = pRegion->GetInfo();
    pInfo->m_posCell.x = x;
    pInfo->m_posCell.y = y;
    pInfo->m_dwCx = cx;
    pInfo->m_dwCy = cy;
    pInfo->m_dwAccessTime = ::TimeGet();
    pInfo->m_bAccess = false;
    return pRegion;
}
//-----------------------------------------------------------------------
C2DMapRegion* C2DMapRegion::CreateNew(RegionInfo* pInfo)
{
    C2DMapRegion* pRegion = new C2DMapRegion;
    MYASSERT(pRegion);
    RegionInfo* pMyInfo = pRegion->GetInfo();
    memcpy(pMyInfo,  pInfo, sizeof(RegionInfo));
    pMyInfo->m_dwAccessTime = ::TimeGet();
    pMyInfo->m_bAccess = false;
    return pRegion;
}
//-----------------------------------------------------------------------
