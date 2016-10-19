
//**********************************************************
// 代码编辑器
//**********************************************************

// Team.cpp: implementation of the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "Team.h"
#include "Player.h"
#include "GamePlayerSet.h"
#include "Hero.h"
#include "gamedataset.h"
#include "3DGameMap.h"
#include "AllMsg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeam::CTeam()
{
    m_setMember.clear();
    this->OpenTeam();
    m_nFocusIndex = 0;
    m_dwTimeBegin = ::TimeGet();
    m_nFocusIndex = -1;
}

CTeam::~CTeam()
{
    this->Destroy();
}
//------------------------------------------------------------------
void CTeam::Destroy()
{
    int nAmount = m_setMember.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        TeamMemberInfo* pInfo = m_setMember[i];
        SAFE_DELETE(pInfo);
    }
    m_setMember.clear();
}
//------------------------------------------------------------------
void CTeam::AddMember(TeamMemberInfo* pInfo)
{
    if (!pInfo)
    {
        return;
    }
    // 搜索，如果存在，替换之
    int nAmount = m_setMember.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TeamMemberInfo* pMyInfo = m_setMember[i];
        if (pMyInfo && (pMyInfo->id == pInfo->id))
        {
            memcpy(pMyInfo, pInfo, sizeof(TeamMemberInfo));
            return;
        }
    }
    // 没有搜索到，添加之
    pInfo->m_dwQueryBegin = ::TimeGet();
    m_setMember.push_back(pInfo);
}
//------------------------------------------------------------------
void CTeam::DelMember(OBJID idMember)
{
    int nAmount = m_setMember.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        TeamMemberInfo* pInfo = m_setMember[i];
        if (pInfo && (pInfo->id == idMember))
        {
            SAFE_DELETE(pInfo);
            m_setMember.erase(m_setMember.begin() + i);
        }
    }
}
//------------------------------------------------------------------
TeamMemberInfo* CTeam::GetMemberInfo(OBJID idMember)
{
    int nAmount = m_setMember.size();
    for(int i = 0; i < nAmount; i++)
    {
        TeamMemberInfo* pInfo = m_setMember[i];
        if (pInfo && (pInfo->id == idMember))
        {
            return pInfo;
        }
    }
    return NULL;
}
//------------------------------------------------------------------
TeamMemberInfo* CTeam::GetMemberInfoByIndex(int nIndex)
{
    int nAmount = m_setMember.size();
    if ((nIndex >= nAmount) || (nIndex < 0))
    {
        return NULL;
    }
    return m_setMember[nIndex];
}
//------------------------------------------------------------------
int CTeam::GetMemberInfoAmount()
{
    return m_setMember.size();
}
//------------------------------------------------------------------
BOOL CTeam::SetLastApplyID(OBJID idPlayer)
{
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idPlayer);
    if (!pPlayer)
    {
        return false;
    }
    m_idPlayerLastApply = pPlayer->GetID();
    strcpy(m_szNamePlayerLastApply, pPlayer->GetName());
    return true;
}
//------------------------------------------------------------------
TeamMemberInfo* CTeam::CreateNewMemberInfo(char* pszName, OBJID id, DWORD dwFace, DWORD dwHp, DWORD dwMaxHp, DWORD dwLook)
{
    if (!pszName)
    {
        return NULL;
    }
    TeamMemberInfo* pInfo = new TeamMemberInfo;
    if (!pInfo)
    {
        return NULL;
    }
    strcpy(pInfo->szName, pszName);
    pInfo->id = id;
    pInfo->dwFace = dwFace;
    pInfo->dwHp = dwHp;
    pInfo->dwMaxHp = dwMaxHp;
    pInfo->dwLook = dwLook;
    // 判断是否Active
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pInfo->id);
    if (!pPlayer)
    {
        pInfo->bActive = false;
    }
    else
    {
        pInfo->bActive = true;
    }
    return pInfo;
}
//------------------------------------------------------------------
void CTeam::Process()
{
    int nAmount = m_setMember.size();
    for(int i = 0; i < nAmount; i ++)
    {
        TeamMemberInfo* pInfo = m_setMember[i];
        if (pInfo)
        {
            if (pInfo->id != g_objHero.GetID())
            {
                // 判断是否Active
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer(pInfo->id);
                if (!pPlayer)
                {
                    pInfo->bActive = false;
                }
                else
                {
                    pInfo->bActive = true;
                }
            }
        }
    }
}
//------------------------------------------------------------------
void CTeam::Show(CMyPos posBegin)
{
    int nAmount = m_setMember.size();
    CMySize sizeBmp = {46, 46};
    CMySize sizeHpBar = {5, 46};
    int nIconAmount = 0;
    // show arrow first
    if (!(g_objHero.IsTeamLeader() && m_nFocusIndexFrame == 0) && (this->GetMemberInfoAmount() != 0))
    {
        TeamMemberInfo* pInfo = this->GetMemberInfoByIndex(m_nFocusIndexFrame);
        if (pInfo)
        {
            CPlayer* pLeader = g_objPlayerSet.GetPlayer(pInfo->id);
            if (pLeader)
            {
                char szCommonAni[] = "ani/common.ani";
                char szArrow[] = "Arrow";
                CAni* pAni = g_objGameDataSet.GetDataAni(szCommonAni, szArrow);
                CMyPos posHero = {0, 0};
                CMyPos posLeader = {0, 0};
                g_objHero.GetPos(posHero);
                pLeader->GetPos(posLeader);
                if (abs(posLeader.x - posHero.x) > 4 || abs(posLeader.y - posHero.y) > 4)
                {
                    int nDir = CGameMap::GetDirection(posLeader.x, posLeader.y, posHero.x, posHero.y);
                    nDir--;
                    if (nDir < 0)
                    {
                        nDir = 7;
                    }
                    if (pAni)
                    {
                        CMyBitmap* pBmp = pAni->GetFrame(nDir);
                        if (pBmp)
                        {
                            CMyPos posShow;
                            posShow.x = _SCR_WIDTH - 48;
                            posShow.y = posBegin.y - 20;
                            pBmp->Show(posShow.x, posShow.y);
                        }
                    }
                }
            }
        }
    }
    int nFocusIndex = -1;
    m_nFocusIndexFrame = -1;
    for(int i = 0; i < nAmount; i ++)
    {
        TeamMemberInfo* pInfo = m_setMember[i];
        if (pInfo && pInfo->id != g_objHero.GetID())
        {
            // 计算位置
            CMyPos posShow;
            posShow.x = _SCR_WIDTH - sizeBmp.iWidth;
            posShow.y = posBegin.y + nIconAmount * (sizeBmp.iHeight + 20) + 48;
            CAni* pAni = g_objGameDataSet.GetPlayerFaceIcon(pInfo->dwFace, pInfo->dwLook);
            if (pAni)
            {
                CMyBitmap* pBmp = pAni->GetFrame(0);
                if (pBmp)
                {
                    CMyPos posMouse;
                    BOOL bFocus = false;
                    ::MouseCheck(posMouse.x, posMouse.y);
                    if ((posMouse.x > posShow.x) &&
                            (posMouse.y > posShow.y) &&
                            (posMouse.x < posShow.x + sizeBmp.iWidth) &&
                            (posMouse.y < posShow.y + sizeBmp.iHeight))
                    {
                        // show name...
                        bFocus = true;
                        nFocusIndex = i;
                        m_nFocusIndexFrame = i;
                        CMySize sizeFont;
                        CMyBitmap::GetFontSize(sizeFont);
                        CMyBitmap::ShowString(_SCR_WIDTH - sizeFont.iWidth * strlen(pInfo->szName), posShow.y - sizeFont.iHeight, 0xffff00, pInfo->szName);
                    }
                    if (pInfo->bActive)
                    {
                        pBmp->SetColor(0xff, 0xff, 0xff, 0xff);
                    }
                    else
                    {
                        pBmp->SetColor(0xff, 0xff, 0xff, 0xff);
                        CMyBitmap::ShowBlock(posShow.x, posShow.y, posShow.x + sizeBmp.iWidth, posShow.y + sizeBmp.iHeight, 0x77000000);
                    }
                    pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeBmp.iWidth, sizeBmp.iHeight);
                    if (bFocus && !pInfo->bActive)
                    {
                        CAni* pAni = g_objGameDataSet.GetDataAni("ani/common.ani", "Magnifier");
                        if (pAni)
                        {
                            int nFrameAmount = pAni->GetFrameAmount();
                            int nFrameIndex = ((::TimeGet() - m_dwTimeBegin) / 300) % nFrameAmount;
                            CMyBitmap* pBmp =  pAni->GetFrame(nFrameIndex);
                            if (pBmp)
                            {
                                pBmp->Show(posShow.x, posShow.y);
                            }
                        }
                        if (m_nFocusIndex != i)
                        {
                            m_nFocusIndex = i;
                            m_dwTimeFocusBegin = ::TimeGet();
                        }
                        else
                        {
                            if ((::TimeGet() - m_dwTimeFocusBegin > 500) && (::TimeGet() - pInfo->m_dwQueryBegin > 2000))
                            {
                                // begin query ...
                                CMsgAction msg;
                                m_nFocusIndex = -1;
                                if (msg.Create(pInfo->id, 0, 0, 0, actionQueryTeamMember, pInfo->id))
                                {
                                    msg.Send();
                                }
                                pInfo->m_dwQueryBegin = ::TimeGet();
                            }
                        }
                    }
                    if (pInfo->bActive)
                    {
                        int nHeight = pInfo->dwHp * sizeBmp.iHeight / pInfo->dwMaxHp;
                        posShow.x = _SCR_WIDTH - sizeHpBar.iWidth;
                        posShow.y = posShow.y + sizeHpBar.iHeight - nHeight;
                        if (nHeight * 3 < sizeHpBar.iHeight)
                        {
                            CMyBitmap::ShowBlock(posShow.x, posShow.y, posShow.x + sizeHpBar.iWidth, posShow.y + nHeight, 0xffff0000);
                        }
                        else
                        {
                            CMyBitmap::ShowBlock(posShow.x, posShow.y, posShow.x + sizeHpBar.iWidth, posShow.y + nHeight, 0xffffff00);
                        }
                    }
                }
            }
            nIconAmount ++;
        }
    }
    if (nFocusIndex == -1)
    {
        m_nFocusIndex = -1;
        m_dwTimeFocusBegin = ::TimeGet();
    }
}
//------------------------------------------------------------------
