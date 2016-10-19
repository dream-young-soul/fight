
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Radar.cpp: implementation of the CRadar class.
//
//////////////////////////////////////////////////////////////////////

#include "Radar.h"
#include "GameDataSet.h"
#include "3DGameMap.h"
#include "GamePlayerSet.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const int _MAP_FRAME_SIZEX = 128;
const int _MAP_FRAME_SIZEY = 128;
const int _POS_FRAME_SIZE = 8;

CRadar::CRadar()
{
    m_setNpcInfo.clear();
    this->LoadNpc();
}

CRadar::~CRadar()
{
    m_setNpcInfo.clear();
}
//--------------------------------------------------------------------------------------
void CRadar::LoadNpc()
{
    FILE* fp = fopen("ini/Radar.ini", "r");
    if (!fp)
    {
        return;
    }
    while(true)
    {
        NpcInfo info;
        int nResult = fscanf(fp, "%u %u %u %d\n", &info.idMapDoc, &info.posNpc.x, &info.posNpc.y, &info.NpcType);
        if (nResult == EOF)
        {
            fclose(fp);
            return;
        }
        if (nResult == 4)
        {
            m_setNpcInfo.push_back(info);
        }
    }
}
//--------------------------------------------------------------------------------------
void CRadar::Show(CMyPos posScr, OBJID idMap, int nMode)
{
    //	return;
    const char szAniFile[] = "ani/MiniMap.ani";
    char szAniName[256] = "";
    sprintf(szAniName, "%u", idMap);
    CAni* pAni = g_objGameDataSet.GetDataAni((char*)szAniFile, szAniName);
    CAni* pHeroAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "Hero");
    CAni* pAniLeader = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamLeader");
    CAni* pAniNpc = g_objGameDataSet.GetDataAni((char*)szAniFile, "Npc");
    CAni* pAniTransPoint = g_objGameDataSet.GetDataAni((char*)szAniFile, "TransPoint");
    if (!pAni)
    {
        return;
    }
    if (pAni->GetFrameAmount() != 3)
    {
        return;
    }
    CMySize sizeAni;
    float   fMapMode;
    switch(nMode)
    {
    case _MODE_128:
        fMapMode = 128;
        sizeAni = pAni->GetSize(0);
        break;
    case _MODE_256:
        fMapMode = 256;
        sizeAni = pAni->GetSize(1);
        break;
    case _MODE_512:
        fMapMode = 512;
        sizeAni = pAni->GetSize(2);
        break;
    }
    CMyPos posScreenCenter;
    posScreenCenter.x = _SCR_WIDTH / 2;
    posScreenCenter.y = _SCR_HEIGHT / 2;
    CMyPos posWorldCenter;
    g_objGameMap.Screen2World(posScreenCenter.x, posScreenCenter.y,
                              posWorldCenter.x, posWorldCenter.y);
    CMyPos posBgCenter;
    g_objGameMap.World2Bg(posWorldCenter.x, posWorldCenter.y, posBgCenter.x, posBgCenter.y);
    // get BgSize
    CMySize sizeBg;
    g_objGameMap.GetBgSize(sizeBg);
    // get scale
    float fScaleX = (float) sizeAni.iWidth / sizeBg.iWidth;
    float fScaleY = (float) sizeAni.iHeight / sizeBg.iHeight;
    // get ani center pos
    CMyPos posAniCenter;
    posAniCenter.x = (int)(posBgCenter.x * fScaleX);
    posAniCenter.y = (int)(posBgCenter.y * fScaleY);
    CMyPos posAni;
    posAni.x = posAniCenter.x - _MAP_FRAME_SIZEX / 2;
    posAni.y = posAniCenter.y - _MAP_FRAME_SIZEY / 2;
    if (posAni.x < 0)
    {
        posAni.x = 0;
    }
    if (posAni.y < 0)
    {
        posAni.y = 0;
    }
    if (posAni.x > sizeAni.iWidth - _MAP_FRAME_SIZEX)
    {
        posAni.x = sizeAni.iWidth - _MAP_FRAME_SIZEX;
    }
    if (posAni.y > sizeAni.iHeight - _MAP_FRAME_SIZEY)
    {
        posAni.y = sizeAni.iHeight - _MAP_FRAME_SIZEY;
    }
    CMyBitmap* pBmp = pAni->GetFrame(nMode);
    if (!pBmp)
    {
        return;
    }
    RECT rect = {posAni.x, posAni.y, posAni.x + _MAP_FRAME_SIZEX, posAni.y + _MAP_FRAME_SIZEY};
    pBmp->ShowEx(posScr.x, posScr.y, &rect, _MAP_FRAME_SIZEX, _MAP_FRAME_SIZEY);
    posScr.x -= _POS_FRAME_SIZE;
    posScr.y -= _POS_FRAME_SIZE;
    if (pAniNpc)
    {
        int nAmount = m_setNpcInfo.size();
        for(int i = 0; i < nAmount; i ++)
        {
            NpcInfo info = m_setNpcInfo[i];
            if (info.idMapDoc == g_objGameMap.GetIDDoc())
            {
                CMyPos posNpcCell, posNpcBg, posNpcScr, posNpcWorld;
                posNpcCell = info.posNpc;
                g_objGameMap.Cell2World(posNpcCell.x, posNpcCell.y, posNpcWorld.x, posNpcWorld.y);
                g_objGameMap.World2Bg(posNpcWorld.x, posNpcWorld.y, posNpcBg.x, posNpcBg.y);
                if (info.NpcType == 1)
                {
                    posNpcScr.x =(int)( posScr.x - posAni.x + posNpcBg.x * fScaleX);
                    posNpcScr.y = (int)(posScr.y - posAni.y + posNpcBg.y * fScaleY);
                    if (posNpcScr.x > posScr.x && posNpcScr.y > posScr.y
                            && posNpcScr.x < posScr.x + _MAP_FRAME_SIZEX
                            && posNpcScr.y < posScr.y + _MAP_FRAME_SIZEY
                       )
                    {
                        pAniNpc->Show(0, posNpcScr.x, posNpcScr.y);
                    }
                }
                else if (info.NpcType == 2)
                {
                    posNpcScr.x = (int)(posScr.x - posAni.x + posNpcBg.x * fScaleX) ;
                    posNpcScr.y = (int)(posScr.y - posAni.y + posNpcBg.y * fScaleY) ;
                    if (posNpcScr.x > posScr.x && posNpcScr.y > posScr.y
                            && posNpcScr.x < posScr.x + _MAP_FRAME_SIZEX
                            && posNpcScr.y < posScr.y + _MAP_FRAME_SIZEY
                       )
                    {
                        pAniTransPoint->Show(0, posNpcScr.x, posNpcScr.y);
                    }
                }
            }
        }
    }
    //MATE
    CPlayer* pPlayer  = g_objPlayerSet.GetPlayer(g_objHero.GetMate());
    if (pPlayer )
    {
        CMyPos posMateCell, posMateBg, posMateScr, posMateWorld;
        pPlayer->GetPos(posMateCell);
        g_objGameMap.Cell2World(posMateCell.x, posMateCell.y, posMateWorld.x, posMateWorld.y);
        g_objGameMap.World2Bg(posMateWorld.x, posMateWorld.y, posMateBg.x, posMateBg.y);
        CAni* pMateAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "Mate", EXIGENCE_IMMEDIATE);
        if (pMateAni)
        {
            posMateScr.x = (int)(posScr.x - posAni.x + posMateBg.x * fScaleX) ;
            posMateScr.y = (int)(posScr.y - posAni.y + posMateBg.y * fScaleY) ;
            if (posMateScr.x > posScr.x && posMateScr.y > posScr.y
                    && posMateScr.x < posScr.x + _MAP_FRAME_SIZEX
                    && posMateScr.y < posScr.y + _MAP_FRAME_SIZEY
               )
            {
                pMateAni->Show(0, posMateScr.x, posMateScr.y);
            }
        }
    }
    int i;		// team member ...
    int nTeamMemberAmount = g_objHero.GetTeamMemberAmount();
    for(i = 0; i < nTeamMemberAmount; i ++)
    {
        TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo(i);
        if (!pInfo)
        {
            continue;
        }
        if (pInfo->id == g_objHero.GetID())
        {
            continue;
        }
        CMyPos posTeamCell, posTeamBg, posTeamScr, posTeamWorld;
        CPlayer* pPlayer  = g_objPlayerSet.GetPlayer(pInfo->id);
        if (i == 0)
        {
            if (!pPlayer)
            {
                const DWORD dwTeamLeaderPosTimeout = 6 * 1000;
                posTeamCell = g_objHero.GetTeamLeaderPos();
                if (::TimeGet() - g_objHero.GetTeamLeaderFlashTime() >= dwTeamLeaderPosTimeout)
                {
                    continue;
                }
            }
            else
            {
                if (!IsOnlyTeamMember(pInfo))
                {
                    continue;
                }
                pPlayer->GetPos(posTeamCell);
            }
        }
        else
        {
            if (!pPlayer)
            {
                continue;
            }
            if (!IsOnlyTeamMember(pInfo))
            {
                continue;
            }
            pPlayer->GetPos(posTeamCell);
        }
        g_objGameMap.Cell2World(posTeamCell.x, posTeamCell.y, posTeamWorld.x, posTeamWorld.y);
        g_objGameMap.World2Bg(posTeamWorld.x, posTeamWorld.y, posTeamBg.x, posTeamBg.y);
        CAni* pTeamAni;
        if (i == 0)
        {
            pTeamAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamLeader", EXIGENCE_IMMEDIATE);
        }
        else
        {
            pTeamAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamMember", EXIGENCE_IMMEDIATE);
        }
        if (pTeamAni)
        {
            posTeamScr.x =(int)( posScr.x - posAni.x + posTeamBg.x * fScaleX) ;
            posTeamScr.y = (int)(posScr.y - posAni.y + posTeamBg.y * fScaleY );
            if (posTeamScr.x > posScr.x && posTeamScr.y > posScr.y
                    && posTeamScr.x < posScr.x + _MAP_FRAME_SIZEX
                    && posTeamScr.y < posScr.y + _MAP_FRAME_SIZEY)
            {
                pTeamAni->Show(0, posTeamScr.x, posTeamScr.y);
            }
        }
        //		pPlayer->SetShowIcon(true);
    }
    BOOL bShowTeam = true;
    if (g_objHero.GetTeamMemberAmount() == 0)
    {
        bShowTeam = false;
    }
    if (::TimeGet() - g_objGameMap.GetFriendBeginShow() > 3 * 1000)
    {
        bShowTeam = false;
    }
    if (bShowTeam)
    {
        CMyPos posTeamCell, posTeamBg, posTeamScr, posTeamWorld;
        g_objGameMap.GetFriendPos(posTeamCell);
        g_objGameMap.Cell2World(posTeamCell.x, posTeamCell.y, posTeamWorld.x, posTeamWorld.y);
        g_objGameMap.World2Bg(posTeamWorld.x, posTeamWorld.y, posTeamBg.x, posTeamBg.y);
        CAni* pTeamAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamMember", EXIGENCE_IMMEDIATE);
        if (pTeamAni)
        {
            posTeamScr.x = (int)(posScr.x - posAni.x + posTeamBg.x * fScaleX );
            posTeamScr.y =(int)( posScr.y - posAni.y + posTeamBg.y * fScaleY );
            int nFrame = 0;
            if ((::TimeGet() - g_objGameMap.GetFriendBeginShow()) % 100 < 20)
            {
                nFrame = 1;
            }
            if (posTeamScr.x > posScr.x && posTeamScr.y > posScr.y
                    && posTeamScr.x < posScr.x + _MAP_FRAME_SIZEX
                    && posTeamScr.y < posScr.y + _MAP_FRAME_SIZEY )
            {
                pTeamAni->Show(nFrame, posTeamScr.x, posTeamScr.y);
            }
        }
    }
    // friend ...
    int nFriendAmount = g_objHero.GetFriendAmount();
    for(i = 0; i < nFriendAmount; i ++)
    {
        CFriend* pFriend = g_objHero.GetFriendByIndex(i);
        if (!pFriend)
        {
            continue;
        }
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pFriend->GetID());
        if (!pPlayer)
        {
            continue;
        }
        //				if (pPlayer->GetShowIcon())
        //					continue;
        CMyPos posFriendCell, posFriendBg, posFriendScr, posFriendWorld;
        pPlayer->GetPos(posFriendCell);
        g_objGameMap.Cell2World(posFriendCell.x, posFriendCell.y, posFriendWorld.x, posFriendWorld.y);
        g_objGameMap.World2Bg(posFriendWorld.x, posFriendWorld.y, posFriendBg.x, posFriendBg.y);
        CAni* pFriendAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "Friend", EXIGENCE_IMMEDIATE);
        if (pFriendAni)
        {
            posFriendScr.x = (int)(posScr.x  - posAni.x + posFriendBg.x * fScaleX) ;
            posFriendScr.y = (int)(posScr.y  - posAni.y + posFriendBg.y * fScaleY) ;
            if (posFriendScr.x > posScr.x && posFriendScr.y > posScr.y
                    && posFriendScr.x < posScr.x + _MAP_FRAME_SIZEX
                    && posFriendScr.y < posScr.y + _MAP_FRAME_SIZEY
               )
            {
                pFriendAni->Show(0, posFriendScr.x, posFriendScr.y);
            }
        }
        //	pPlayer->SetShowIcon(true);
    }
    // enemy ...
    int nEnemyAmount = g_objHero.GetEnemyAmount();
    for(i = 0; i < nEnemyAmount; i ++)
    {
        CEnemy* pEnemy = g_objHero.GetEnemyByIndex(i);
        if (!pEnemy)
        {
            continue;
        }
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pEnemy->GetID());
        if (!pPlayer)
        {
            continue;
        }
        //if (pPlayer->GetShowIcon())
        //	continue;
        CMyPos posEnemyCell, posEnemyBg, posEnemyScr, posEnemyWorld;
        pPlayer->GetPos(posEnemyCell);
        g_objGameMap.Cell2World(posEnemyCell.x, posEnemyCell.y, posEnemyWorld.x, posEnemyWorld.y);
        g_objGameMap.World2Bg(posEnemyWorld.x, posEnemyWorld.y, posEnemyBg.x, posEnemyBg.y);
        CAni* pEnemyAni = g_objGameDataSet.GetDataAni("ani/Minimap.ani", "Enemy", EXIGENCE_IMMEDIATE);
        if (pEnemyAni)
        {
            posEnemyScr.x =(int)( posScr.x  - posAni.x + posEnemyBg.x * fScaleX );
            posEnemyScr.y =(int)( posScr.y  - posAni.y + posEnemyBg.y * fScaleY) ;
            if (posEnemyScr.x > posScr.x && posEnemyScr.y > posScr.y
                    && posEnemyScr.x < posScr.x + _MAP_FRAME_SIZEX
                    && posEnemyScr.y < posScr.y + _MAP_FRAME_SIZEY
               )
            {
                pEnemyAni->Show(0, posEnemyScr.x, posEnemyScr.y);
            }
        }
        //pPlayer->SetShowIcon(true);
    }
    if (pHeroAni)
    {
        CMyPos posHeroCell, posHeroBg, posHeroScr, posHeroWorld;
        g_objHero.GetPos(posHeroCell);
        g_objGameMap.Cell2World(posHeroCell.x, posHeroCell.y, posHeroWorld.x, posHeroWorld.y);
        g_objGameMap.World2Bg(posHeroWorld.x, posHeroWorld.y, posHeroBg.x, posHeroBg.y);
        posHeroScr.x = (int)(posScr.x - posAni.x + posHeroBg.x * fScaleX) ;
        posHeroScr.y =(int)( posScr.y - posAni.y + posHeroBg.y * fScaleY );
        if (posHeroScr.x > posScr.x + _MAP_FRAME_SIZEX - _POS_FRAME_SIZE)
        {
            posHeroScr.x = posScr.x + _MAP_FRAME_SIZEY - _POS_FRAME_SIZE;
        }
        else if (posHeroScr.x < posScr.x)
        {
            posHeroScr.x = posScr.x;
        }
        if (posHeroScr.y < posScr.y )
        {
            posHeroScr.y = posScr.y ;
        }
        else if (posHeroScr.y > posScr.y + _MAP_FRAME_SIZEX - _POS_FRAME_SIZE)
        {
            posHeroScr.y = posScr.y + _MAP_FRAME_SIZEY - _POS_FRAME_SIZE;
        }
        pHeroAni->Show(0, posHeroScr.x, posHeroScr.y);
    }
}

//--------------------------------------------------------------------------------------

void CRadar::ShowBigMap(CMyPos posScr, OBJID idMap, CMySize sizeDlgBg)
{
    const char szAniFile[] = "ani/MiniMap.ani";
    char szAniName[256] = "";
    sprintf(szAniName, "%u", idMap);
    CAni* pAni = g_objGameDataSet.GetDataAni((char*)szAniFile, szAniName);
    CAni* pHeroAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "Hero");
    CAni* pAniLeader = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamLeader");
    CAni* pAniNpc = g_objGameDataSet.GetDataAni((char*)szAniFile, "Npc");
    CAni* pAniTransPoint = g_objGameDataSet.GetDataAni((char*)szAniFile, "TransPoint");
    if (!pAni)
    {
        return;
    }
    if (pAni->GetFrameAmount() != 3)
    {
        return;
    }
    if (pAni)
    {
        CMySize sizeAni;
        sizeAni = pAni->GetSize(2);
        if (sizeAni.iWidth < sizeDlgBg.iWidth)
        {
            posScr.x = posScr.x + (sizeDlgBg.iWidth - sizeAni.iWidth) / 2;
        }
        if (sizeAni.iHeight < sizeDlgBg.iHeight)
        {
            posScr.y = posScr.y + (sizeDlgBg.iHeight - sizeAni.iHeight) / 2;
        }
        CMyPos posScreenCenter;
        posScreenCenter.x = _SCR_WIDTH / 2;
        posScreenCenter.y = _SCR_HEIGHT / 2;
        CMyPos posWorldCenter;
        g_objGameMap.Screen2World(posScreenCenter.x, posScreenCenter.y,
                                  posWorldCenter.x, posWorldCenter.y);
        CMyPos posBgCenter;
        g_objGameMap.World2Bg(posWorldCenter.x, posWorldCenter.y, posBgCenter.x, posBgCenter.y);
        // get BgSize
        CMySize sizeBg;
        g_objGameMap.GetBgSize(sizeBg);
        // get scale
        float fScaleX = (float)sizeAni.iWidth / sizeBg.iWidth;
        float fScaleY = (float)sizeAni.iHeight / sizeBg.iHeight;
        //			if (sizeBg.iWidth > sizeBg.iHeight)
        //			{
        //				fScaleY = fScaleY * sizeBg.iHeight / sizeBg.iWidth;
        //			}
        //			if (sizeBg.iWidth < sizeBg.iHeight)
        //			{
        //				fScaleX = fScaleX * sizeBg.iWidth / sizeBg.iHeight;
        //			}
        // get ani center pos
        CMyPos posAniCenter;
        posAniCenter.x =(int)( posBgCenter.x * fScaleX);
        posAniCenter.y =(int)( posBgCenter.y * fScaleY);
        CMyBitmap* pBmp = pAni->GetFrame(2);
        if (!pBmp)
        {
            return;
        }
        CMyPos posAni;
        posAni.x = posAniCenter.x - sizeDlgBg.iWidth / 2;
        posAni.y = posAniCenter.y - sizeDlgBg.iHeight / 2;
        if (posAni.x > sizeAni.iWidth - sizeDlgBg.iWidth )
        {
            posAni.x = sizeAni.iWidth - sizeDlgBg.iWidth;
        }
        if (posAni.x < 0)
        {
            posAni.x = 0;
        }
        if (posAni.y >   sizeAni.iHeight - sizeDlgBg.iHeight)
        {
            posAni.y =   sizeAni.iHeight - sizeDlgBg.iHeight;
        }
        if (posAni.y < 0)
        {
            posAni.y = 0;
        }
        RECT rect ;
        rect.left = posAni.x;
        rect.top =  posAni.y;
        if (sizeDlgBg.iWidth > sizeAni.iWidth)
        {
            rect.right = posAni.x + sizeAni.iWidth;
        }
        else
        {
            rect.right = posAni.x + sizeDlgBg.iWidth;
        }
        if (sizeDlgBg.iHeight > sizeAni.iHeight)
        {
            rect.bottom = posAni.y + sizeAni.iHeight;
        }
        else
        {
            rect.bottom = posAni.y + sizeDlgBg.iHeight;
        }
        pBmp->ShowEx(posScr.x, posScr.y, &rect, rect.right - rect.left, rect.bottom - rect.top);
        posScr.x -= _POS_FRAME_SIZE;
        posScr.y -= _POS_FRAME_SIZE;
        if (pAniNpc)
        {
            int nAmount = m_setNpcInfo.size();
            for(int i = 0; i < nAmount; i ++)
            {
                NpcInfo info = m_setNpcInfo[i];
                if (info.idMapDoc == g_objGameMap.GetIDDoc())
                {
                    CMyPos posNpcCell, posNpcBg, posNpcScr, posNpcWorld;
                    CMySize sizeAni;
                    posNpcCell = info.posNpc;
                    g_objGameMap.Cell2World(posNpcCell.x, posNpcCell.y, posNpcWorld.x, posNpcWorld.y);
                    g_objGameMap.World2Bg(posNpcWorld.x, posNpcWorld.y, posNpcBg.x, posNpcBg.y);
                    if (info.NpcType == 1)
                    {
                        sizeAni = pAniNpc->GetSize(0);
                        posNpcScr.x = (int)(posScr.x - posAni.x + posNpcBg.x * fScaleX) ;
                        posNpcScr.y = (int)(posScr.y - posAni.y + posNpcBg.y * fScaleY );
                        if (posNpcScr.x > posScr.x && posNpcScr.y > posScr.y
                                && posNpcScr.x < posScr.x + sizeDlgBg.iWidth
                                && posNpcScr.y < posScr.y + sizeDlgBg.iHeight
                           )
                        {
                            pAniNpc->Show(0, posNpcScr.x, posNpcScr.y);
                        }
                    }
                    else if (info.NpcType == 2)
                    {
                        sizeAni = pAniTransPoint->GetSize(0);
                        posNpcScr.x = (int)(posScr.x - posAni.x + posNpcBg.x * fScaleX) ;
                        posNpcScr.y = (int)(posScr.y - posAni.y + posNpcBg.y * fScaleY );
                        if (posNpcScr.x > posScr.x && posNpcScr.y > posScr.y
                                && posNpcScr.x < posScr.x + sizeDlgBg.iWidth
                                && posNpcScr.y < posScr.y + sizeDlgBg.iHeight
                           )
                        {
                            pAniTransPoint->Show(0, posNpcScr.x, posNpcScr.y);
                        }
                    }
                }
            }
        }
        //MATE
        CPlayer* pPlayer  = g_objPlayerSet.GetPlayer(g_objHero.GetMate());
        if (pPlayer )
        {
            CMyPos posMateCell, posMateBg, posMateScr, posMateWorld;
            pPlayer->GetPos(posMateCell);
            g_objGameMap.Cell2World(posMateCell.x, posMateCell.y, posMateWorld.x, posMateWorld.y);
            g_objGameMap.World2Bg(posMateWorld.x, posMateWorld.y, posMateBg.x, posMateBg.y);
            CAni* pMateAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "Mate", EXIGENCE_IMMEDIATE);
            if (pMateAni)
            {
                posMateScr.x = (int)(posScr.x - posAni.x + posMateBg.x * fScaleX) ;
                posMateScr.y =(int)( posScr.y - posAni.y + posMateBg.y * fScaleY);
                if (posMateScr.x > posScr.x && posMateScr.y > posScr.y
                        && posMateScr.x < posScr.x + sizeDlgBg.iWidth
                        && posMateScr.y < posScr.y + sizeDlgBg.iHeight
                   )
                {
                    pMateAni->Show(0, posMateScr.x, posMateScr.y);
                }
            }
        }
        int i;
        // team member ...
        int nTeamMemberAmount = g_objHero.GetTeamMemberAmount();
        for(i = 0; i < nTeamMemberAmount; i ++)
        {
            TeamMemberInfo* pInfo = g_objHero.GetTeamMemberInfo(i);
            if (!pInfo)
            {
                continue;
            }
            if (pInfo->id == g_objHero.GetID())
            {
                continue;
            }
            CMyPos posTeamCell, posTeamBg, posTeamScr, posTeamWorld;
            CPlayer* pPlayer  = g_objPlayerSet.GetPlayer(pInfo->id);
            if (i == 0)
            {
                if (!pPlayer)
                {
                    const DWORD dwTeamLeaderPosTimeout = 6 * 1000;
                    posTeamCell = g_objHero.GetTeamLeaderPos();
                    if (::TimeGet() - g_objHero.GetTeamLeaderFlashTime() >= dwTeamLeaderPosTimeout)
                    {
                        continue;
                    }
                }
                else
                {
                    if (!IsOnlyTeamMember(pInfo))
                    {
                        continue;
                    }
                    pPlayer->GetPos(posTeamCell);
                }
            }
            else
            {
                if (!pPlayer)
                {
                    continue;
                }
                if (!IsOnlyTeamMember(pInfo))
                {
                    continue;
                }
                pPlayer->GetPos(posTeamCell);
            }
            g_objGameMap.Cell2World(posTeamCell.x, posTeamCell.y, posTeamWorld.x, posTeamWorld.y);
            g_objGameMap.World2Bg(posTeamWorld.x, posTeamWorld.y, posTeamBg.x, posTeamBg.y);
            CAni* pTeamAni;
            if (i == 0)
            {
                pTeamAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamLeader", EXIGENCE_IMMEDIATE);
            }
            else
            {
                pTeamAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamMember", EXIGENCE_IMMEDIATE);
            }
            if (pTeamAni)
            {
                posTeamScr.x =(int)( posScr.x - posAni.x + posTeamBg.x * fScaleX) ;
                posTeamScr.y =(int)( posScr.y - posAni.y + posTeamBg.y * fScaleY );
                if (posTeamScr.x > posScr.x && posTeamScr.y > posScr.y
                        && posTeamScr.x < posScr.x + sizeDlgBg.iWidth
                        && posTeamScr.y < posScr.y + sizeDlgBg.iHeight
                   )
                {
                    pTeamAni->Show(0, posTeamScr.x, posTeamScr.y);
                }
            }
            //		pPlayer->SetShowIcon(true);
        }
        BOOL bShowTeam = true;
        if (g_objHero.GetTeamMemberAmount() == 0)
        {
            bShowTeam = false;
        }
        if (::TimeGet() - g_objGameMap.GetFriendBeginShow() > 3 * 1000)
        {
            bShowTeam = false;
        }
        if (bShowTeam)
        {
            CMyPos posTeamCell, posTeamBg, posTeamScr, posTeamWorld;
            g_objGameMap.GetFriendPos(posTeamCell);
            g_objGameMap.Cell2World(posTeamCell.x, posTeamCell.y, posTeamWorld.x, posTeamWorld.y);
            g_objGameMap.World2Bg(posTeamWorld.x, posTeamWorld.y, posTeamBg.x, posTeamBg.y);
            CAni* pTeamAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "TeamMember", EXIGENCE_IMMEDIATE);
            if (pTeamAni)
            {
                posTeamScr.x = (int)(posScr.x - posAni.x + posTeamBg.x * fScaleX) ;
                posTeamScr.y =(int)( posScr.y - posAni.y + posTeamBg.y * fScaleY );
                int nFrame = 0;
                if ((::TimeGet() - g_objGameMap.GetFriendBeginShow()) % 100 < 20)
                {
                    nFrame = 1;
                }
                if (posTeamScr.x > posScr.x && posTeamScr.y > posScr.y
                        && posTeamScr.x < posScr.x +  sizeDlgBg.iWidth
                        && posTeamScr.y < posScr.y + sizeDlgBg.iHeight )
                {
                    pTeamAni->Show(nFrame, posTeamScr.x, posTeamScr.y);
                }
            }
        }
        // friend ...
        int nFriendAmount = g_objHero.GetFriendAmount();
        for(i = 0; i < nFriendAmount; i ++)
        {
            CFriend* pFriend = g_objHero.GetFriendByIndex(i);
            if (!pFriend)
            {
                continue;
            }
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pFriend->GetID());
            if (!pPlayer)
            {
                continue;
            }
            //				if (pPlayer->GetShowIcon())
            //					continue;
            CMyPos posFriendCell, posFriendBg, posFriendScr, posFriendWorld;
            pPlayer->GetPos(posFriendCell);
            g_objGameMap.Cell2World(posFriendCell.x, posFriendCell.y, posFriendWorld.x, posFriendWorld.y);
            g_objGameMap.World2Bg(posFriendWorld.x, posFriendWorld.y, posFriendBg.x, posFriendBg.y);
            CAni* pFriendAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "Friend", EXIGENCE_IMMEDIATE);
            if (pFriendAni)
            {
                posFriendScr.x = (int)(posScr.x  - posAni.x + posFriendBg.x * fScaleX) ;
                posFriendScr.y = (int)(posScr.y  - posAni.y + posFriendBg.y * fScaleY );
                if (posFriendScr.x > posScr.x && posFriendScr.y > posScr.y
                        && posFriendScr.x < posScr.x + sizeDlgBg.iWidth
                        && posFriendScr.y < posScr.y + sizeDlgBg.iHeight
                   )
                {
                    pFriendAni->Show(0, posFriendScr.x, posFriendScr.y);
                }
            }
            //	pPlayer->SetShowIcon(true);
        }
        // enemy ...
        int nEnemyAmount = g_objHero.GetEnemyAmount();
        for(i = 0; i < nEnemyAmount; i ++)
        {
            CEnemy* pEnemy = g_objHero.GetEnemyByIndex(i);
            if (!pEnemy)
            {
                continue;
            }
            CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pEnemy->GetID());
            if (!pPlayer)
            {
                continue;
            }
            //if (pPlayer->GetShowIcon())
            //	continue;
            CMyPos posEnemyCell, posEnemyBg, posEnemyScr, posEnemyWorld;
            pPlayer->GetPos(posEnemyCell);
            g_objGameMap.Cell2World(posEnemyCell.x, posEnemyCell.y, posEnemyWorld.x, posEnemyWorld.y);
            g_objGameMap.World2Bg(posEnemyWorld.x, posEnemyWorld.y, posEnemyBg.x, posEnemyBg.y);
            CAni* pEnemyAni = g_objGameDataSet.GetDataAni((char*)szAniFile, "Enemy", EXIGENCE_IMMEDIATE);
            if (pEnemyAni)
            {
                posEnemyScr.x = (int)(posScr.x  - posAni.x + posEnemyBg.x * fScaleX );
                posEnemyScr.y =(int)( posScr.y  - posAni.y + posEnemyBg.y * fScaleY );
                if (posEnemyScr.x > posScr.x && posEnemyScr.y > posScr.y
                        && posEnemyScr.x < posScr.x + sizeDlgBg.iWidth
                        && posEnemyScr.y < posScr.y + sizeDlgBg.iHeight
                   )
                {
                    pEnemyAni->Show(0, posEnemyScr.x, posEnemyScr.y);
                }
            }
            //pPlayer->SetShowIcon(true);
        }
        if (pHeroAni)
        {
            CMyPos posHeroCell, posHeroBg, posHeroScr, posHeroWorld;
            g_objHero.GetPos(posHeroCell);
            g_objGameMap.Cell2World(posHeroCell.x, posHeroCell.y, posHeroWorld.x, posHeroWorld.y);
            g_objGameMap.World2Bg(posHeroWorld.x, posHeroWorld.y, posHeroBg.x, posHeroBg.y);
            posHeroScr.x = (int)(posScr.x - posAni.x + posHeroBg.x * fScaleX) ;
            posHeroScr.y =(int)( posScr.y - posAni.y + posHeroBg.y * fScaleY );
            CMySize sizeMap;
            sizeMap = pAni->GetSize(2);
            if (posHeroScr.x > posScr.x + sizeMap.iWidth - _POS_FRAME_SIZE)
            {
                posHeroScr.x = posScr.x + sizeMap.iWidth - _POS_FRAME_SIZE;
            }
            else if (posHeroScr.x < posScr.x)
            {
                posHeroScr.x = posScr.x;
            }
            if (posHeroScr.y < posScr.y )
            {
                posHeroScr.y = posScr.y ;
            }
            else if (posHeroScr.y > posScr.y + sizeMap.iHeight - _POS_FRAME_SIZE)
            {
                posHeroScr.y = posScr.y + sizeMap.iHeight - _POS_FRAME_SIZE;
            }
            pHeroAni->Show(0, posHeroScr.x, posHeroScr.y);
        }
    }
}

//--------------------------------------------------------------------------------------
void CRadar::CellPos2RaderPos(CAni* pAni, CMyPos posCell, CMyPos& posRader)
{
}
//--------------------------------------------------------------------------------------

BOOL CRadar::IsOnlyTeamMember(TeamMemberInfo* pInfo)
{
    if (pInfo)
    {
        if (strcmp(pInfo->szName, g_objHero.GetMate()) == 0)
        {
            return false;
        }
        if (g_objHero.GetFriend(pInfo->id) != NULL)
        {
            return false;
        }
        if (g_objHero.GetEnemy(pInfo->id) != NULL)
        {
            return false;
        }
    }
    return true;
}
