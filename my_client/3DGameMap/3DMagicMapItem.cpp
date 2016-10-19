
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMagicMapItem.cpp: implementation of the C3DMagicMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "3DMagicMapItem.h"
#include "3DGameMap.h"
#include "3DRoleData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DMagicMapItem::C3DMagicMapItem()
{
    m_pUnit = NULL;
    m_nMoment = 0;
}

C3DMagicMapItem::~C3DMagicMapItem()
{
    SAFE_DELETE(m_pUnit);
}

//-------------------------------------------------------------------
void C3DMagicMapItem::Show(void* pInfo)
{
    return;
}
//-------------------------------------------------------------------
void C3DMagicMapItem::Process(void* pInfo)
{
    BOOL* pbOver = (BOOL*)pInfo;
    *pbOver = false;
    switch(m_nMoment)
    {
    case 0:// start
        {
            if (strcmp(m_szStart, "NULL") == 0)
            {
                m_nMoment = 1;
            }
            if (!m_pUnit)
            {
                m_pUnit = CTerainEffectUnit::CreateNew(m_szStart);
                if (!m_pUnit)
                {
                    m_nMoment = 1;
                }
                else
                {
                    CMyPos posWorld;
                    g_objGameMap.Cell2World(m_posCell.x, m_posCell.y, posWorld.x, posWorld.y);
                    m_pUnit->SetPos(posWorld);
                    if (m_pUnit->Process())
                    {
                        m_nMoment = 1;
                        SAFE_DELETE(m_pUnit);
                    }
                }
            }
            else
            {
                if (m_pUnit->Process())
                {
                    m_nMoment = 1;
                    SAFE_DELETE(m_pUnit);
                }
            }
        }
        break;
    case 1:// last
        {
            if (strcmp(m_szLast, "NULL") == 0)
            {
                m_nMoment = 2;
            }
            if (!m_pUnit)
            {
                m_pUnit = CTerainEffectUnit::CreateNew(m_szLast);
                if (!m_pUnit)
                {
                    m_nMoment = 2;
                }
                else
                {
                    CMyPos posWorld;
                    g_objGameMap.Cell2World(m_posCell.x, m_posCell.y, posWorld.x, posWorld.y);
                    m_pUnit->SetPos(posWorld);
                    if (m_pUnit->Process())
                    {
                        m_nMoment = 2;
                        SAFE_DELETE(m_pUnit);
                    }
                }
            }
            else
            {
                if (m_pUnit->Process())
                {
                    m_nMoment = 2;
                    SAFE_DELETE(m_pUnit);
                }
            }
        }
        break;
    case 2:// end
        {
            if (strcmp(m_szLast, "NULL") == 0)
            {
                m_nMoment = 3;
            }
            if (!m_pUnit)
            {
                m_pUnit = CTerainEffectUnit::CreateNew(m_szEnd);
                if (!m_pUnit)
                {
                    m_nMoment = 3;
                }
                else
                {
                    CMyPos posWorld;
                    g_objGameMap.Cell2World(m_posCell.x, m_posCell.y, posWorld.x, posWorld.y);
                    m_pUnit->SetPos(posWorld);
                    if (m_pUnit->Process())
                    {
                        m_nMoment = 3;
                        SAFE_DELETE(m_pUnit);
                    }
                }
            }
            else
            {
                if (m_pUnit->Process())
                {
                    m_nMoment = 3;
                    SAFE_DELETE(m_pUnit);
                }
            }
        }
        break;
    case 3:// bye
        {
            *pbOver = true;
        }
    }
}

//-------------------------------------------------------------------
void C3DMagicMapItem::SetBgPos(CMyPos posBg)
{
    m_posBg = posBg;
}
//-------------------------------------------------------------------
void C3DMagicMapItem::GetBgPos(CMyPos& posBg)
{
    posBg = m_posBg;
}
//-------------------------------------------------------------------
BOOL C3DMagicMapItem::IsFocus()
{
    return false;
}
//-------------------------------------------------------------------
void C3DMagicMapItem::SetPos(CMyPos posCell)
{
    CMyPos posWorld;
    g_objGameMap.Cell2World(posCell.x, posCell.y, posWorld.x, posWorld.y);
    g_objGameMap.World2Bg(posWorld.x, posWorld.y, m_posBg.x, m_posBg.y);
    C3DMapObj::SetPos(posCell);
}
//-------------------------------------------------------------------
BOOL C3DMagicMapItem::Create(OBJID idType, OBJID id)
{
    MapMagicItemInfo* pInfo = g_obj3DRoleData.GetMapMagicItemInfo(idType);
    if (!pInfo)
    {
        return false;
    }
    char szDir[] = "ini/tme/";
    m_id = id;
    sprintf(m_szStart, "%s%s", szDir, pInfo->szStart);
    sprintf(m_szLast, "%s%s", szDir, pInfo->szLast);
    sprintf(m_szEnd, "%s%s", szDir, pInfo->szEnd);
    return true;
}
//-------------------------------------------------------------------
C3DMagicMapItem* C3DMagicMapItem::CreateNew(OBJID idType, OBJID id)
{
    C3DMagicMapItem* pItem = new C3DMagicMapItem;
    if (!pItem)
    {
        return NULL;
    }
    if (!pItem->Create(idType, id))
    {
        SAFE_DELETE(pItem);
        return NULL;
    }
    return pItem;
}
//-------------------------------------------------------------------
