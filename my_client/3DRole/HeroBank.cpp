
//**********************************************************
// 代码编辑器
//**********************************************************

//---------------------------------------------------------------
// HeroBank.cpp
//---------------------------------------------------------------
#include "Hero.h"
#include "AllMsg.h"
#include "GameMsg.h"
#include "3dGameMap.h"
#include "GameDataSet.h"
#include "GamePlayerSet.h"

//---------------------------------------------------------------
void CHero::QueryMoney()
{
    CMsgItem msg;
    if (msg.Create(m_objBank.GetId(), 0, ITEMACT_QUERYMONEYSAVED))
    {
        msg.Send();
    }
}
//---------------------------------------------------------------
void CHero::DrawMoney(DWORD dwMoney)
{
    CMsgItem msg;
    if (msg.Create(m_objBank.GetId(), dwMoney, ITEMACT_DRAWMONEY))
    {
        msg.Send();
    }
    this->QueryMoney();
}
//---------------------------------------------------------------
void CHero::SaveMoney(DWORD dwMoney)
{
    if (dwMoney > g_objHero.GetMoney())
    {
        return;
    }
    CMsgItem msg;
    if (msg.Create(m_objBank.GetId(), dwMoney, ITEMACT_SAVEMONEY))
    {
        msg.Send();
    }
    this->QueryMoney();
}
//---------------------------------------------------------------
BOOL CHero::DropMoney(DWORD dwMoney, int x, int y)
{
    if (g_objGameMap.GetType() & MAPTYPE_FAMILY)
    {
        OBJID idCanNotDrop = 100146;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotDrop));
        return false;
    }
    if (dwMoney < 100)
    {
        OBJID idDropMoneyLimit = 100038;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idDropMoneyLimit));
        return false;
    }
    if (this->GetMoney() < dwMoney)
    {
        OBJID idMoneyNotEnough = 100039;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMoneyNotEnough));
        return false;
    }
    CMyPos posCell;
    g_objGameMap.Screen2Cell(x, y, posCell.x, posCell.y);
    CMyPos posHero = g_objHero.GetAlignPos();
    int nDir = g_objGameMap.GetDirection(posHero.x, posHero.y, posCell.x, posCell.y);
    /*
    posCell.x = posHero.x+_DELTA_X[nDir];
    posCell.y = posHero.y+_DELTA_Y[nDir];

    CellInfo* pInfo = g_objGameMap.GetCell(posCell.x, posCell.y);
    if (!pInfo || g_objGameMap.GetMapItem(posCell) || g_objGameMap.GetGroundMask(pInfo) != 0)
    */
    {
        // 八格随机方向
        int nX[8] = {1, -1, 0, 1, 1, -1, -1, 0};
        int nY[8] = {1, -1, 1, 0, -1, 1, 0, -1};
        int i = 0;
        for(; i < 8; i ++)
        {
            CMyPos posItem;
            posItem.x = posHero.x + nX[i];
            posItem.y = posHero.y + nY[i];
            CellInfo* pInfo = g_objGameMap.GetCell(posItem.x, posItem.y);
            if (pInfo)
            {
                if (!g_objGameMap.Get3DMapItem(posItem) && g_objGameMap.GetGroundMask(pInfo) == 0
                        && !g_objPlayerSet.GetPlayer(posItem.x, posItem.y))
                {
                    posCell = posItem;
                    break;
                }
            }
        }
        if (i >= 8)
        {
            OBJID idCanNotDrop = 100040;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotDrop));
            return false;
        }
    }
    CMsgItem msg;
    if (msg.Create(dwMoney, posCell.x, posCell.y, ITEMACT_DROPMONEY))
    {
        msg.Send();
    }
    return true;
}
//---------------------------------------------------------------
DWORD CHero::GetSavedMoney()
{
    return m_objBank.GetMoney();
}
//---------------------------------------------------------------
void CHero::SetSavedMoney(DWORD dwMoney)
{
    m_objBank.SetMoney(dwMoney);
    ::PostCmd(CMD_FLASHBANKMONEY);
}
//---------------------------------------------------------------
void CHero::OpenBank(OBJID idBank)
{
    m_objBank.Open(idBank);
    this->QueryMoney();
}
//---------------------------------------------------------------
void CHero::CloseBank()
{
    m_objBank.Close();
}
//---------------------------------------------------------------
