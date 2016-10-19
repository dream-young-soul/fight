
//**********************************************************
// 代码编辑器
//**********************************************************

// ItemObserver.cpp: implementation of the ItemObserver class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemObserver.h"
#include "Ani.h"
#include "ND_Bitmap.h"
#include "3DGameMap.h"
#include "Item.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItemObserver g_objObserver;
//--------------------------------------------------------------------------------------

CItemObserver::CItemObserver()
{
    m_bShowItem = false;
    m_bShowAction = true;
    m_nIndex = 0;
}

CItemObserver::~CItemObserver()
{
}
//--------------------------------------------------------------------------------------
void CItemObserver::Show()
{
    if (m_bShowItem)
    {
        this->ShowBg();
    }
    if (m_bShowAction)
    {
        this->ShowAction();
    }
}
//--------------------------------------------------------------------------------------
void CItemObserver::ShowBg()
{
    /*
    	CMyPos posBegin = {_SCR_WIDTH - 37 * 6, 30};
    	for(int i = 0; i < 6; i ++)
    		CMyBitmap::ShowLine(posBegin.x + 37 * i, posBegin.y, posBegin.x + 37 * i,
    									30 + 37 * 8, 0xff00ffff);
    	for(int j = 0; j < 9; j ++ )
    		CMyBitmap::ShowLine(posBegin.x, posBegin.y + 37 * j, posBegin.x + 37 * 5,
    		posBegin.y + 37 * j, 0xff00ffff);

    	CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posBegin.x + 37 * 5 + 1, posBegin.y + 37 * 8 + 1, 0xaa0000ff);
    	CMyBitmap::ShowRect(posBegin.x-129, posBegin.y, posBegin.x - 1, posBegin.y + 128, 0xff00ffff);
    	CMyBitmap::ShowBlock(posBegin.x-129, posBegin.y, posBegin.x, posBegin.y + 129, 0xaa0000ff);

    	CMyBitmap::ShowRect(posBegin.x-65, posBegin.y + 130, posBegin.x - 1, posBegin.y + 65 + 130, 0xff00ffff);
    	CMyBitmap::ShowBlock(posBegin.x-65, posBegin.y + 130, posBegin.x, posBegin.y + 65 + 130 + 1, 0xaa0000ff);
    	CMyPos posMouse;
    	MouseCheck(posMouse.x, posMouse.y);
    	static CItem objItem[5][8];
    	BOOL bFC=false;
    	for(int m = 0; m < 5; m ++)
    	{
    		for(int n = 0; n < 8; n ++)
    		{
    			objItem[m][n].Create(100 + RandGet(100000), 1090010);
    			CMyPos posLT, posRD;
    			posLT.x = posBegin.x + 37 * m;
    			posLT.y = posBegin.y + 37 * n;
    			posRD.x = posLT.x + 37;
    			posRD.y = posLT.y + 37;

    			BOOL bF = false;
    			if ((posMouse.x > posLT.x) &&
    				(posMouse.y > posLT.y) &&
    				(posMouse.x < posRD.x) &&
    				(posMouse.y < posRD.y))
    			{
    				CMyBitmap::ShowBlock(posLT.x, posLT.y, posRD.x, posRD.y, 0x77777700);
    				bF = true;
    				bFC = true;
    			}
    			CMyPos posShow;
    			posShow.x = (posLT.x + posRD.x)/2;
    			posShow.y = posRD.y;
    			objItem[m][n].ShowMiniItem(posShow.x, posShow.y, bF);
    		}
    	}
    	if (bFC)
    	{
    		static CItem objMItem;
    		objMItem.Create(100 + RandGet(100000), 1090010);
    		if (objMItem.GetPercent() <= 50)
    			objMItem.ShowMiniItem((posBegin.x-65 + posBegin.x - 1)/2, posBegin.y + 65 + 130, true, false);
    		static CItem objBItem;
    		objBItem.Create(100 + RandGet(100000), 1090010);
    		if (objBItem.GetPercent() > 50)
    			objBItem.ShowMiniItem((posBegin.x-129 + posBegin.x - 1)/2, posBegin.y + 129, true, false);
    	}*/
}
//--------------------------------------------------------------------------------------

int nFoc = -1;
int nRate[100] = {0};
void CItemObserver::ShowAction()
{
    char szAction[20][8] =
    {
        "高兴",
        "悲伤",
        "行礼",
        "半跪",
        "坐下",
        "躺下",
        "轻伤",
        "重伤",
        "格挡",

        "死一",
        "死二",
        "物品",
        "受伤",

        "攻一",
        "攻二",
        "攻三",
        "小动",
        "警戒",
        "重置",
        "翻页"
    };
    char szArmor[10][8] =
    {
        "甲一",
        "甲二",
        "甲三",
        "甲四",
        "甲五",
        "甲六",
        "发一",
        "弓箭",
        "单兵",
        "翻页",
    };
    CMyPos posMouse;
    int nEvent = ::MouseCheck(posMouse.x, posMouse.y);
    // show rect;
    BOOL bFoc = false;
    CMyPos posBegin = {5, 5};
    if (m_nIndex == 0)
    {
        for(int i = 0; i < 20; i ++)
        {
            CMyBitmap::ShowRect(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0xffffffff);
            CMyBitmap::ShowBlock(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0x770000ff);
            if ((posMouse.x > posBegin.x + 50 * i) &&
                    (posMouse.y > posBegin.y) &&
                    (posMouse.x < posBegin.x + 50 * (i + 1)) &&
                    (posMouse.y < posBegin.y + 20))
            {
                bFoc = true;
                CMyBitmap::ShowBlock(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0x77ffffff);
                if (nFoc != i)
                {
                    nFoc = i;
                    switch(i)
                    {
                    case 0:
                        g_objHero.Emotion(_ACTION_LAUGH);
                        break;
                    case 1:
                        g_objHero.Emotion(_ACTION_SAD);
                        break;
                    case 2:
                        g_objHero.Emotion(_ACTION_SALUTE);
                        break;
                    case 3:
                        g_objHero.Emotion(_ACTION_GENUFLECT);
                        break;
                    case 4:
                        g_objHero.Emotion(_ACTION_SITDOWN);
                        break;
                    case 5:
                        g_objHero.Emotion(_ACTION_FAINT);
                        break;
                    case 6:
                        g_objHero.Emotion(_ACTION_BRUISE0);
                        break;
                    case 7:
                        g_objHero.Emotion(_ACTION_BRUISE1);
                        break;
                    case 8:
                        g_objHero.Emotion(_ACTION_DODGE0);
                        break;
                    case 9:
                        {
                            CMyPos posHero;
                            g_objHero.GetPos(posHero);
                            CCommand cmd;
                            cmd.iType			= _COMMAND_DIE;
                            cmd.iStatus			= _CMDSTATUS_BEGIN;
                            cmd.posTarget.x		= posHero.x;
                            cmd.posTarget.y		= posHero.y;
                            g_objHero.SetCommand(&cmd);
                        }
                        break;
                    case 10:
                        {
                            CMyPos posHero;
                            g_objHero.GetPos(posHero);
                            CCommand cmd;
                            cmd.iType			= _COMMAND_DIE;
                            cmd.iStatus			= _CMDSTATUS_BEGIN;
                            cmd.posTarget.x		= posHero.x;
                            cmd.posTarget.y		= posHero.y;
                            cmd.dwData = 2;
                            cmd.nDir = (g_objHero.GetDir() + 8) % 8;
                            g_objHero.SetCommand(&cmd);
                        }
                        break;
                    case 11:
                        {
                            g_objObserver.Switch();
                        }
                        break;
                    case 12:
                        {
                            /*
                            							if (g_objHero.TestStatus(USERSTATUS_WOUND))
                            								g_objHero.ReplaceStatus(0);
                            							else
                            								g_objHero.ReplaceStatus(USERSTATUS_WOUND);
                            */
                        }
                        break;
                    case 13:
                        g_objHero.Emotion(_ACTION_ATTACK0);
                        break;
                    case 14:
                        g_objHero.Emotion(_ACTION_ATTACK1);
                        break;
                    case 15:
                        g_objHero.Emotion(_ACTION_ATTACK2);
                        break;
                    case 16:
                        g_objHero.Emotion(_ACTION_REST1);
                        break;
                    case 17:
                        g_objHero.FlashLastAttackTime();
                        break;
                    case 18:
                        {
                        }
                        break;
                    case 19:
                        m_nIndex = 1;
                        break;
                    }
                }
            }
            else
            {
                CMyBitmap::ShowBlock(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0x770000ff);
            }
            CMySize sizeFont;
            CMyBitmap::GetFontSize(sizeFont);
            sizeFont.iWidth = sizeFont.iWidth * 2;
            CMyBitmap::ShowString(posBegin.x + 50 * i + 25 - sizeFont.iWidth, posBegin.y + 10 - sizeFont.iHeight / 2, 0xffffff00, szAction[i]);
        }
    }
    else
    {
        for(int i = 0; i < 10; i ++)
        {
            CMyBitmap::ShowRect(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0xffffffff);
            CMyBitmap::ShowBlock(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0x770000ff);
            if ((posMouse.x > posBegin.x + 50 * i) &&
                    (posMouse.y > posBegin.y) &&
                    (posMouse.x < posBegin.x + 50 * (i + 1)) &&
                    (posMouse.y < posBegin.y + 20))
            {
                bFoc = true;
                CMyBitmap::ShowBlock(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0x77ffffff);
                if (nFoc != i)
                {
                    nFoc = i;
                    switch(i)
                    {
                    case 0:
                        g_objHero.SetArmor(2000010);
                        break;
                    case 1:
                        g_objHero.SetArmor(2000020);
                        break;
                    case 2:
                        g_objHero.SetArmor(2000030);
                        break;
                    case 3:
                        g_objHero.SetArmor(2000040);
                        break;
                    case 4:
                        g_objHero.SetArmor(2000050);
                        break;
                    case 5:
                        g_objHero.SetArmor(2000060);
                        break;
                    case 6:
                        g_objHero.SetHair(101);
                        break;
                    case 7:
                        g_objHero.SetRWeapon(500000);
                        g_objHero.SetLWeapon(1050000);
                        break;
                    case 8:
                        g_objHero.SetRWeapon(410000);
                        break;
                    case 9:
                        m_nIndex = 0;
                        break;
                    }
                }
            }
            else
            {
                CMyBitmap::ShowBlock(posBegin.x + 50 * i, posBegin.y, posBegin.x + 50 * (i + 1), posBegin.y + 20, 0x770000ff);
            }
            CMySize sizeFont;
            CMyBitmap::GetFontSize(sizeFont);
            sizeFont.iWidth = sizeFont.iWidth * 2;
            CMyBitmap::ShowString(posBegin.x + 50 * i + 25 - sizeFont.iWidth, posBegin.y + 10 - sizeFont.iHeight / 2, 0xffffff00, szArmor[i]);
        }
    }
    if (!bFoc)
    {
        nFoc = -1;
    }
}
//--------------------------------------------------------------------------------------
