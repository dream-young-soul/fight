
//**********************************************************
// 代码编辑器
//**********************************************************

// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "basefunc.h"
#include "Player.h"
#include "Hero.h"
#include "gamedataset.h"
#include "GamePlayerSet.h"
#include "../NdSound/NdSound.h"
#include "3DGameMap.h"
#include "3DRoleData.h"
#include "pet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// static...
BOOL	CPlayer::s_bShowName	= false;
HANDLE	CPlayer::s_hHeap		= NULL;
UINT	CPlayer::s_uNumAllocsInHeap	= 0;

//////////////////////////////////////////////////////////////////////
void* CPlayer::operator new(size_t size)
{
    // got heap?
    if (!s_hHeap)
    {
        s_hHeap	=::HeapCreate(0, 0, 0);
        if (!s_hHeap)
        {
            return NULL;
        }
    }
    // alloc mem for new obj
    void* p	=::HeapAlloc(s_hHeap, HEAP_ZERO_MEMORY, size);
    if (p)
    {
        s_uNumAllocsInHeap++;
    }
    return p;
}

//////////////////////////////////////////////////////////////////////
void CPlayer::operator delete(void* p)
{
    // check param
    if (!s_hHeap || s_uNumAllocsInHeap <= 0)
    {
        return;
    }
    // free it...
    if (::HeapFree(s_hHeap, 0, p))
    {
        s_uNumAllocsInHeap--;
    }
    // time to destroy heap?
    if (s_uNumAllocsInHeap <= 0)
    {
        if (::HeapDestroy(s_hHeap))
        {
            s_hHeap	= NULL;
        }
    }
}

//////////////////////////////////////////////////////////////////////
BOOL CPlayer::IsValidPt(void* p)
{
    if (!s_hHeap || !p)
    {
        return false;
    }
    if ((DWORD)p < 0x00010000 || (DWORD)p >= 0x7FFEFFFF)	// user address range from 1M--2G-64k
    {
        return false;
    }
    if (::IsBadCodePtr((FARPROC)p))
    {
        return false;
    }
    BOOL bOk = ::HeapValidate(s_hHeap, 0, p);
    if (bOk == false)
    {
        int kkk = 1;
    }
    return bOk;
}

//////////////////////////////////////////////////////////////////////
CPlayer::CPlayer()
{
	m_PlayerInfo.Reset();
	m_PlayerInfo.dwMaxPhysicalForce = MaxPhysicalForce;

    m_nType = MAP_PLAYER;
    strcpy(m_szCryOut, "");
    m_dwKoScale = 0;
    m_pFD = CFDword::CreateNew(::GetDate(), 16);
    strcpy(m_szNpcTitle, "");
    m_bFlashCrime = FALSE;
    m_dwFlashCrimeTime = 0;
    m_nAttackTimes = 0;
}

CPlayer::~CPlayer()
{
    SAFE_DELETE(m_pFD);
}

//////////////////////////////////////////////////////////////////////
void CPlayer::Show(void* pInfo)
{
    //	if (this->TestStatus(USERSTATUS_LURKER))
    //		this->SetARGB(0x55000000 | (this->GetARGB() & 0x00ffffff));
    CRole::Show();
    //show teammember life
    if (this->GetID() != g_objHero.GetID() && g_objHero.GetTeamMemberAmount() > 0
            && g_objHero.GetTeamMemberInfoByID(this->GetID()) != NULL)
    {
        this->ShowLife();
    }
    // show info
    if (this->IsMouseFocus())
    {
        this->ShowInfo();
        //show hero life
        //		if (this->GetID () == g_objHero.GetID ())
        //			this->ShowLife ();
    }
    else
    {
        if (CPlayer::s_bShowName || g_objGameMap.GetShowMapObjInfoFlag())
        {
            if ((this->GetID() >= MONSTERID_FIRST  && this->GetID() <= MONSTERID_LAST)
                    && (this->TestStatus(USERSTATUS_DISAPPEARING) || this->TestStatus(USERSTATUS_DIE)))
            {
                // idle
            }
            else
            {
                if (this->GetID () == g_objHero.GetID ())
                {
                    this->ShowLife ();
                }
                this->ShowName();
                this->ShowSyndicate();
                this->ShowKoAmount(false);
            }
        }
    }
    // show zoom number
    CMyPos posNum;
    posNum.x = m_Info.posScr.x;
    posNum.y = m_Info.posScr.y - 100;
    m_objZoomNum.Show(posNum);
}

//////////////////////////////////////////////////////////////////////
void CPlayer::ShowLife()
{
    CMySize sizeFont;
    sizeFont.iHeight	= CMyBitmap::GetFontSize();
    sizeFont.iWidth		= sizeFont.iHeight / 2;
    if (this->GetRoleType() == _ROLE_SYNFLAG_NPC &&
            this->GetData(CPlayer::_PLAYER_LIFE) > 0 && this->GetMaxLife() > 0)
    {
        DWORD dwBloodColor;
        DWORD dwLife = this->GetData(CPlayer::_PLAYER_LIFE);
        DWORD dwMaxLife = this->GetMaxLife();
        if (dwLife > 20000000)
        {
            dwBloodColor = _COLOR_GREEN;
        }
        else if (dwLife > 10000000)
        {
            dwBloodColor = _COLOR_YELLOW;
        }
        else
        {
            dwBloodColor = _COLOR_RED;
        }
        int nBarAmount = dwMaxLife / 2000000;
        if ((DWORD)(nBarAmount * 2000000) < dwMaxLife)
        {
            nBarAmount ++;
        }
        if (nBarAmount > 5)
        {
            nBarAmount = 5;
        }
        int nBarWidth = 50, nBarHeight = 4;
        int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
        int nShowX	= m_Info.posScr.x - nBarWidth / 2;
        int nShowY	= m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 2;
        dwLife = dwLife % (10000000 + 1);
        for(int i = 0; i < nBarAmount; i ++)
        {
            CMyPos posBegin, posEnd;
            posBegin.x = nShowX;
            posBegin.y = nShowY - nBarHeight * i * 2;
            posEnd.x   = nShowX + nBarWidth;
            posEnd.y   = nShowY + nBarHeight - nBarHeight * i * 2;
            g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
            g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
            CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posEnd.x, posEnd.y, 0x55000000 + _COLOR_GRAY);
            DWORD dwCurrentMaxLife = 2000000;
            DWORD dwCurrentLife;
            if (dwLife > dwCurrentMaxLife * i)
            {
                dwCurrentLife = dwLife - dwCurrentMaxLife * i;
            }
            else
            {
                dwCurrentLife = 0;
            }
            if (dwCurrentLife > dwCurrentMaxLife)
            {
                dwCurrentLife = dwCurrentMaxLife;
            }
            int nLifeBarLen;
            if (dwCurrentMaxLife == 0)
            {
                nLifeBarLen = 0;
            }
            else
            {
                nLifeBarLen = nBarWidth * dwCurrentLife / dwCurrentMaxLife;
            }
            posBegin.x = nShowX;
            posBegin.y = nShowY - nBarHeight * i * 2;
            posEnd.x   = nShowX + nLifeBarLen;
            posEnd.y   = nShowY + nBarHeight - nBarHeight * i * 2;
            g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
            g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
            CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posEnd.x, posEnd.y, 0xff000000 + dwBloodColor);
        }
    }
    else if (this->IsMyPet() && this->GetData(CPlayer::_PLAYER_LIFE) > 0 && this->GetMaxLife() > 0)
    {
        g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
        int nBarWidth = 50, nBarHeight = 6;
        int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
        int nShowX	= m_Info.posScr.x - nBarWidth / 2;
        int nShowY	= m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 3;
        CMyPos posBegin, posEnd;
        posBegin.x = nShowX;
        posBegin.y = nShowY;
        posEnd.x   = nShowX + nBarWidth;
        posEnd.y   = nShowY + nBarHeight;
        g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
        g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
        CMyBitmap::ShowBlock(posBegin.x - 1, posBegin.y - 1, posEnd.x + 1, posEnd.y + 1, 0x77000000 + _COLOR_GRAY);
        CMyBitmap::ShowRect(posBegin.x - 1, posBegin.y - 1, posEnd.x, posEnd.y, 0x77000000 + _COLOR_RED);
        int nLifeBarLen = nBarWidth * this->GetData(CPlayer::_PLAYER_LIFE) / this->GetMaxLife();
        posBegin.x = nShowX;
        posBegin.y = nShowY;
        posEnd.x   = nShowX + nLifeBarLen;
        posEnd.y   = nShowY + nBarHeight;
        g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
        g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
        CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posEnd.x, posEnd.y, 0xffffff00);
    }
    else if (g_objHero.GetID() == this->GetID()
             && this->GetData(CPlayer::_PLAYER_LIFE) > 0 && this->GetMaxLife() > 0)
    {
        g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
        int nBarWidth = 50, nBarHeight = 6;
        int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
        CCommand* pCommand = this->GetCommand();
        if (pCommand
                && pCommand->nData == _ACTION_SITDOWN
                && pCommand->iType == _COMMAND_EMOTION
                && pCommand->iStatus == _CMDSTATUS_CONTINUE)
        {
            nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 200;
        }
        int nShowX	= m_Info.posScr.x - nBarWidth / 2;
        int nShowY	= m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 3;
        CMyPos posBegin, posEnd;
        posBegin.x = nShowX;
        posBegin.y = nShowY;
        posEnd.x   = nShowX + nBarWidth;
        posEnd.y   = nShowY + nBarHeight;
        g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
        g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
        CMyBitmap::ShowBlock(posBegin.x - 1, posBegin.y - 1, posEnd.x + 1, posEnd.y + 1, 0x77000000 + _COLOR_GRAY);
        CMyBitmap::ShowRect(posBegin.x - 1, posBegin.y - 1, posEnd.x, posEnd.y, 0x77000000 + _COLOR_RED);
        int nLifeBarLen = nBarWidth * this->GetData(CPlayer::_PLAYER_LIFE) / this->GetMaxLife();
        posBegin.x = nShowX;
        posBegin.y = nShowY;
        posEnd.x   = nShowX + nLifeBarLen;
        posEnd.y   = nShowY + nBarHeight;
        g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
        g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
        CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posEnd.x, posEnd.y, 0xffffff00);
    }
    else if (g_objHero.GetID() != this->GetID()
             && g_objHero.GetTeamMemberInfoByID(this->GetID()) != NULL)
    {
        TeamMemberInfo* pTeamMemberInfo = g_objHero.GetTeamMemberInfoByID(this->GetID());
        if (pTeamMemberInfo == NULL)
        {
            return;
        }
        else if (pTeamMemberInfo->dwHp >= 0 && pTeamMemberInfo->dwMaxHp > 0)
        {
            g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
            int nBarWidth = 50, nBarHeight = 6;
            int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
            CCommand* pCommand = this->GetCommand();
            if (pCommand
                    && pCommand->nData == _ACTION_SITDOWN
                    && pCommand->iType == _COMMAND_EMOTION
                    && pCommand->iStatus == _CMDSTATUS_CONTINUE)
            {
                nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 200;
            }
            int nShowX	= m_Info.posScr.x - nBarWidth / 2;
            int nShowY	= m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 3;
            CMyPos posBegin, posEnd;
            posBegin.x = nShowX;
            posBegin.y = nShowY;
            posEnd.x   = nShowX + nBarWidth;
            posEnd.y   = nShowY + nBarHeight;
            g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
            g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
            CMyBitmap::ShowBlock(posBegin.x - 1, posBegin.y - 1, posEnd.x + 1, posEnd.y + 1, 0x77000000 + _COLOR_GRAY);
            CMyBitmap::ShowRect(posBegin.x - 1, posBegin.y - 1, posEnd.x, posEnd.y, 0x77000000 + _COLOR_RED);
            int nLifeBarLen = nBarWidth * pTeamMemberInfo->dwHp / pTeamMemberInfo->dwMaxHp;
            posBegin.x = nShowX;
            posBegin.y = nShowY;
            posEnd.x   = nShowX + nLifeBarLen;
            posEnd.y   = nShowY + nBarHeight;
            g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
            g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
            CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posEnd.x, posEnd.y, 0xffffff00);
        }
    }
    else if (this->IsNpc() &&
             this->GetData(CPlayer::_PLAYER_LIFE) > 0 && this->GetMaxLife() > 0)
    {
        // npc will show life bar
        int nBarWidth = 50, nBarHeight = 4;
        int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
        int nShowX	= m_Info.posScr.x - nBarWidth / 2;
        int nShowY	= m_Info.posScr.y - nNameShowOffset - 12;
        CMyPos posBegin, posEnd;
        posBegin.x = nShowX;
        posBegin.y = nShowY;
        posEnd.x   = nShowX + nBarWidth;
        posEnd.y   = nShowY + nBarHeight;
        g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
        g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
        CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posEnd.x, posEnd.y, 0x55000000 + _COLOR_GRAY);
        int nLifeBarLen = nBarWidth * this->GetData(CPlayer::_PLAYER_LIFE) / this->GetMaxLife();
        posBegin.x = nShowX;
        posBegin.y = nShowY;
        posEnd.x   = nShowX + nLifeBarLen;
        posEnd.y   = nShowY + nBarHeight;
        g_objGameMap.MapScaleShowPos(posBegin.x, posBegin.y);
        g_objGameMap.MapScaleShowPos(posEnd.x, posEnd.y);
        CMyBitmap::ShowBlock(posBegin.x, posBegin.y, posEnd.x, posEnd.y, 0xffB80303);
    }
}

//////////////////////////////////////////////////////////////////////

void CPlayer::ShowName(void)
{
    if (this->GetRoleType() == _ROLE_STONE_MINE)
    {
        return;
    }
    if (this->GetID() != g_objHero.GetID()
            && g_objHero.GetTeamMemberInfoByID(this->GetID()) == NULL)
    {
        this->ShowLife();
    }
    CMySize sizeFont;
    sizeFont.iHeight	= CMyBitmap::GetFontSize();
    sizeFont.iWidth		= sizeFont.iHeight / 2;
    g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
    const char* pszString	= this->GetName();
#ifdef _DEBUG
    char szID[256] = "";
    if (this->IsNpc())
    {
        sprintf(szID, "%s[%u]", pszString, this->GetID());
        pszString = szID;
    }
    if (this->IsMyPet())
    {
        CPet* pPet = (CPet*)this;
        CMyPos posC;
        pPet->GetPos(posC);
        //pPet->GetAlignPos(posA);
        //sprintf(szID, "%s[%d,%d][%d,%d]", pszString, posC.x, posC.y, posA.x, posA.y);
        pszString = szID;
    }
#endif
    int iInfoLen	= strlen(pszString);
    if (iInfoLen <= 0)
    {
        return;
    }
    static int dwNameFlashTimer = ::TimeGet();
    DWORD dwColor = _COLOR_WHITE;
    if (this->IsNpc())
    {
        dwColor = g_objHero.GetMonsterNameColor(this->GetLev());
    }
    else
    {
        if (this->TestStatus(USERSTATUS_RED))
        {
            dwColor = _COLOR_RED;
        }
        if (this->TestStatus(USERSTATUS_DARKRED))
        {
            dwColor = _COLOR_BLACK;
        }
        if (this->TestStatus(USERSTATUS_CRIME))
        {
            int nFrame = (::TimeGet() - dwNameFlashTimer) / 200;
            if (nFrame > nFrame / 2 * 2)
            {
                dwColor = _COLOR_BLUE;
            }
            else
            {
                dwColor = _COLOR_WHITE;
            }
        }
        /*		if (this->TestStatus(KEEPEFFECT_SELFDEFENCE))
        		{
        			int nFrame = (::TimeGet() - dwNameFlashTimer)/200;
        			if (nFrame > nFrame/2*2)
        				dwColor = _COLOR_BLUE;
        			else
        				dwColor = _COLOR_WHITE;
        		}*/
    }
    int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
    CCommand* pCommand = this->GetCommand();
    if (pCommand
            && pCommand->nData == _ACTION_SITDOWN
            && pCommand->iType == _COMMAND_EMOTION
            && pCommand->iStatus == _CMDSTATUS_CONTINUE)
    {
        nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 200;
    }
    int nShowX	= m_Info.posScr.x; //-sizeFont.iWidth*iInfoLen/2;
    int nShowY	= m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 2;
    if (g_objHero.GetID() == this->GetID())
    {
        nShowY = m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 2;
    }
    if (this->GetRoleType() == _ROLE_BOOTH_NPC)
    {
        char szTemp[64];
        const OBJID idStr = 100124;
        sprintf(szTemp, "%s%s", pszString, g_objGameDataSet.GetStr(idStr));
        g_objGameMap.MapScaleShowPos(nShowX, nShowY);
        nShowX -= sizeFont.iWidth * iInfoLen / 2;
        {
            DWORD dwBkColor = 0;
            if (((dwColor & 0x00ff0000) >> 16) < 0x20
                    && ((dwColor & 0x00ff00) >> 8) < 0x20
                    && (dwColor & 0x00ff) < 0x20)
            {
                dwBkColor = 0xaaaaaa;
            }
            CMyPos posShow;
            posShow.x = nShowX + 1;
            posShow.y = nShowY + 1 - this->GetOffsetY();
            g_objGameMap.m_InfoViewManager.AddStringView(szTemp, posShow, dwBkColor);
            posShow.x = nShowX;
            posShow.y = nShowY - this->GetOffsetY();
            g_objGameMap.m_InfoViewManager.AddStringView(szTemp, posShow, dwColor);
        }
    }
    else
    {
        g_objGameMap.MapScaleShowPos(nShowX, nShowY);
        {
            DWORD dwBkColor = 0;
            if (((dwColor & 0x00ff0000) >> 16) < 0x20
                    && ((dwColor & 0x00ff00) >> 8) < 0x20
                    && (dwColor & 0x00ff) < 0x20)
            {
                dwBkColor = 0xaaaaaa;
            }
            nShowX -= sizeFont.iWidth * iInfoLen / 2;
            if (this->IsPlayer())
            {
                CMyPos posShow;
                posShow.x = nShowX + 1;
                posShow.y = nShowY + 1 - this->GetOffsetY();
                g_objGameMap.m_InfoViewManager.AddStringView((char*)pszString, posShow, dwBkColor);
                posShow.x = nShowX;
                posShow.y = nShowY - this->GetOffsetY();
                g_objGameMap.m_InfoViewManager.AddStringView((char*)pszString, posShow, dwColor);
            }
            else
            {
                CMyPos posShow;
                posShow.x = nShowX + 1;
                posShow.y = nShowY + 1 - this->GetOffsetY() - 4;
                g_objGameMap.m_InfoViewManager.AddStringView((char*)pszString, posShow, dwBkColor);
                posShow.x = nShowX;
                posShow.y = nShowY - this->GetOffsetY() - 4;
                g_objGameMap.m_InfoViewManager.AddStringView((char*)pszString, posShow, dwColor);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CPlayer::ShowSyndicate (void)
{
    if (CRole::GetIDType(this->GetID()) == _PLAYER_ID_SYSNPC && this->GetRoleType() != _ROLE_STONE_MINE)
    {
        if (strlen(m_szNpcTitle) > 0)
        {
            CMySize sizeFont;
            sizeFont.iHeight	= CMyBitmap::GetFontSize();
            sizeFont.iWidth		= sizeFont.iHeight / 2;
            g_objGameMap.World2Screen(m_Info.posWorld.x, m_Info.posWorld.y, m_Info.posScr.x, m_Info.posScr.y);
            int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
            int nShowX	= m_Info.posScr.x; //-sizeFont.iWidth*iInfoLen/2;
            int nShowY	= m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 2;
            g_objGameMap.MapScaleShowPos(nShowX, nShowY);
            nShowX -= sizeFont.iWidth * strlen(m_szNpcTitle) / 2;
            CMyPos posShow;
            int noffset = 6;
            posShow.x = nShowX;
            posShow.y = nShowY - this->GetOffsetY() - sizeFont.iHeight - noffset;
            g_objGameMap.m_InfoViewManager.AddStringView((char*)m_szNpcTitle, posShow, _COLOR_BLACK);
            posShow.x = nShowX + 1;
            posShow.y = nShowY - this->GetOffsetY() - sizeFont.iHeight + 1 - noffset;
            g_objGameMap.m_InfoViewManager.AddStringView((char*)m_szNpcTitle, posShow, _COLOR_YELLOW);
            //			CMyBitmap::ShowString(posShow.x, posShow.y, _COLOR_YELLOW, m_szNpcTitle);
        }
        return;
    }
    if (this->GetSydicateID() == 0)
    {
        return;
    }
    CMySize sizeFont;
    sizeFont.iHeight	= CMyBitmap::GetFontSize();
    sizeFont.iWidth		= sizeFont.iHeight / 2;
    int iInfoLen	= strlen(this->GetSyndicate());
    if (iInfoLen <= 0)
    {
        return;
    }
    char szSyndicate[256] = "";
    sprintf(szSyndicate, "%s", this->GetSyndicate());
    char szSyndicateInfo[256] = "";
    if (this->GetSyndicateRank() >= RANK_SUBLEADER)
    {
        sprintf(szSyndicateInfo, "%s %s", this->GetSyndicate(), this->GetSyndicateTitle());
    }
    else
    {
        sprintf(szSyndicateInfo, "%s", this->GetSyndicate());
    }
    iInfoLen = strlen(szSyndicateInfo);
    DWORD dwColor = _COLOR_WHITE;
    DWORD dwSyndicateColor;
    switch(g_objPlayerSet.GetSyndicateStatus(this->GetSydicateID()) )
    {
    case SYNDICATE_ALLY:
        dwSyndicateColor = _COLOR_GREEN;
        break;
    case SYNDICATE_ANTAGONIZE:
        dwSyndicateColor = _COLOR_RED;
        break;
    case SYNDICATE_SELF:
        dwSyndicateColor = _COLOR_YELLOW;
        break;
    case _COLOR_WHITE:
        dwSyndicateColor = SYNDICATE_NORMAL;
        break;
    default:
        dwSyndicateColor = _COLOR_WHITE;
        break;
    }
    if (g_objHero.GetSydicateID() == ID_NONE)
    {
        dwSyndicateColor = _COLOR_WHITE;
    }
    int nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 100;
    CCommand* pCommand = this->GetCommand();
    if (pCommand
            && pCommand->nData == _ACTION_SITDOWN
            && pCommand->iType == _COMMAND_EMOTION
            && pCommand->iStatus == _CMDSTATUS_CONTINUE)
    {
        nNameShowOffset = this->GetZoomPercent() * OFFSET_NAMESHOW / 200;
    }
    int nShowX	= m_Info.posScr.x; //-sizeFont.iWidth*iInfoLen/2;
    int nShowY	= m_Info.posScr.y - nNameShowOffset - sizeFont.iHeight * 2;
    g_objGameMap.MapScaleShowPos(nShowX, nShowY);
    nShowX -= sizeFont.iWidth * iInfoLen / 2;
    nShowY += sizeFont.iHeight;
    CMyPos posShow;
    posShow.x = nShowX + 1;
    posShow.y = nShowY + 1 - this->GetOffsetY();
    g_objGameMap.m_InfoViewManager.AddStringView(szSyndicate, posShow, 0x0);
    // show syndicate ...
    posShow.x = nShowX;
    posShow.y = nShowY - this->GetOffsetY();
    g_objGameMap.m_InfoViewManager.AddStringView(szSyndicate, posShow, dwSyndicateColor);
    // 总堂，或无堂口
    if (strlen(this->GetSubSyndicate()) == 0 && this->GetSyndicateRank() >= RANK_SUBLEADER)
    {
        // show rank ...
        nShowX += (strlen(szSyndicate)) * sizeFont.iWidth;
        posShow.x = nShowX + 1;
        posShow.y = nShowY + 1 - this->GetOffsetY();
        g_objGameMap.m_InfoViewManager.AddStringView(this->GetSyndicateTitle(), posShow, 0x0);
        posShow.x = nShowX;
        posShow.y = nShowY - this->GetOffsetY();
        g_objGameMap.m_InfoViewManager.AddStringView(this->GetSyndicateTitle(), posShow, 0x6699ff);
    }
    // show subsybcidate ...
    if (strlen(this->GetSubSyndicate()) != 0)
    {
        posShow.x = nShowX + strlen(szSyndicate) * sizeFont.iWidth;
        posShow.y = nShowY + 1 - this->GetOffsetY();
        g_objGameMap.m_InfoViewManager.AddAniView("ani/common.ani", "Syndicate", posShow, 0);
        // show rank ...
        nShowX += (2 + strlen(szSyndicate)) * sizeFont.iWidth;
        posShow.x = nShowX + 1;
        posShow.y = nShowY + 1 - this->GetOffsetY();
        g_objGameMap.m_InfoViewManager.AddStringView(this->GetSubSyndicate(), posShow, 0x0);
        posShow.x = nShowX;
        posShow.y = nShowY - this->GetOffsetY();
        g_objGameMap.m_InfoViewManager.AddStringView(this->GetSubSyndicate(), posShow, dwSyndicateColor);
    }
    // show subsybcidate title...
    if (strlen(this->GetSubSyndicate()) != 0 && this->GetSyndicateRank() != RANK_NEWBIE)
    {
        // show rank ...
        nShowX += (strlen(this->GetSubSyndicate())) * sizeFont.iWidth;
        posShow.x = nShowX + 1;
        posShow.y = nShowY + 1 - this->GetOffsetY();
        g_objGameMap.m_InfoViewManager.AddStringView(this->GetSyndicateTitle(), posShow, 0x0);
        posShow.x = nShowX;
        posShow.y = nShowY - this->GetOffsetY();
        g_objGameMap.m_InfoViewManager.AddStringView(this->GetSyndicateTitle(), posShow, 0x6699ff);
    }
}

//////////////////////////////////////////////////////////////////////
void CPlayer::ShowInfo(void)
{
    if (this->IsNpc())
    {
        this->ShowName();
        this->ShowSyndicate();
        return;
    }
    if (this->GetRoleType() == _ROLE_MOUSE_NPC)
    {
        return;
    }
    this->ShowName();
    this->ShowSyndicate();
    //	this->ShowKoAmount();
}

//////////////////////////////////////////////////////////////////////
DWORD CPlayer::SetCommand(CCommand* pCommand)
{
    if (!pCommand)
    {
        return false;
    }
    switch(pCommand->iType)
    {
    case _COMMAND_DIE:
        {
            // 清掉延时命令
            m_setCmdList.clear();
            // 清除当前命令
            this->ResetActionData();
            // 快速执行队列中所有命令
            int nCmds = m_PlayerInfo.setCmd.size();
            for (int i = 0; i < nCmds; i++)
            {
                CCommand& cmd = m_PlayerInfo.setCmd[i];
                CRole::SetCommand(&cmd);
                CRole::ProcessCommand();
                this->ResetActionData();
            }
            m_PlayerInfo.setCmd.clear();
            // 设置指令
            CRole::SetCommand(pCommand);
        }
        break;
    case _COMMAND_SHITHAPPEN:
    case _COMMAND_INTONE:
    case _COMMAND_ATTACK:
    case _COMMAND_SHOOT:
    case _COMMAND_SPATTACK:
    case _COMMAND_RUSHATK:
    case _COMMAND_LOCKATK:
    case _COMMAND_BUMP:
        {
            // 清掉延时命令
            m_setCmdList.clear();
            // 快速执行队列中所有命令
            int nCmds = m_PlayerInfo.setCmd.size();
            for (int i = 0; i < nCmds; i++)
            {
                CCommand& cmd = m_PlayerInfo.setCmd[i];
                CRole::SetCommand(&cmd);
                CRole::ProcessCommand();
                this->ResetActionData();
            }
            m_PlayerInfo.setCmd.clear();
            // 清除当前动作
            this->ResetActionData();
            // 设置指令
            CRole::SetCommand(pCommand);
        }
        break;
    case _COMMAND_TAKEOFF:
    case _COMMAND_LANDING:
    case _COMMAND_JUMP:
    case _COMMAND_WALK:
    case _COMMAND_RUN:
    case _COMMAND_WALKFORWARD:
    case _COMMAND_RUNFORWARD:
        {
            // 清除掉所有延时的命令
            m_setCmdList.clear();
            // 查询序列中的命令, 如果是低优先级别, 则清除掉
            int nCmds = m_PlayerInfo.setCmd.size();
            for (int i = nCmds - 1; i >= 0 ; i--)
            {
                CONST CCommand& cmd = m_PlayerInfo.setCmd[i];
                switch (cmd.iType)
                {
                case _COMMAND_EMOTION:
                case _COMMAND_ACTION:
                case _COMMAND_STANDBY:
                case _COMMAND_POSE:
                case _COMMAND_MINE:
                case _COMMAND_WOUND:
                case _COMMAND_DEFEND:
                    //case _COMMAND_LOCKBOOTH:
                    m_PlayerInfo.setCmd.erase(m_PlayerInfo.setCmd.begin() + i);
                    break;
                default:
                    break;
                }
            }
            // 查询当前执行的命令, 如果是低优先级别, 则重置
            if (this->IsIdle())
            {
                this->ResetActionData();
            }
            // 加入新的指令
            m_PlayerInfo.setCmd.push_back(*pCommand);
        }
        break;
    case _COMMAND_ACTION:
        {
            this->ResetActionData();
            // reset cmd in queue
            int nCmds = m_PlayerInfo.setCmd.size();
            for (int i = 0; i < nCmds; i++)
            {
                CCommand cmd = m_PlayerInfo.setCmd[i];
                CRole::SetCommand(&cmd);
                CRole::ProcessCommand();
                this->ResetActionData();
            }
            m_PlayerInfo.setCmd.clear();
            m_setCmdList.clear();
            // set command now
            return CRole::SetCommand(pCommand);
        }
        break;
    default:
        m_PlayerInfo.setCmd.push_back(*pCommand);
        break;
    }
    return TRUE;
    /*
    	switch(pCommand->iType)
    	{
    	case _COMMAND_POSE:
    	case _COMMAND_EMOTION:
    	case _COMMAND_ACTION:
    	case _COMMAND_INTONE:
    	case _COMMAND_DIE:
    	case _COMMAND_JUMP:
    		{
    			// reset cur cmd first
    			this->ResetActionData();

    			// reset cmd in queue
    			int nCmds = m_PlayerInfo.setCmd.size();
    			for (int i=0; i<nCmds; i++)
    			{
    				CCommand cmd = m_PlayerInfo.setCmd[i];
    				CRole::SetCommand(&cmd);
    				CRole::ProcessCommand();

    				this->ResetActionData();
    			}

    			m_PlayerInfo.setCmd.clear();
    			m_setCmdList.clear();

    			// set command now
    			return CRole::SetCommand(pCommand);
    		}
    		break;

    	default:
    		{
    			m_dwCommandIndex++;
    			pCommand->dwIndex = m_dwCommandIndex;
    			m_PlayerInfo.setCmd.push_back(*pCommand);
    			return m_dwCommandIndex;
    		}
    		break;
    	}
    	return 0;*/
}

//////////////////////////////////////////////////////////////////////
void    CPlayer::SynTrack(BOOL bMove)
{
    if (!bMove)
    {
        if (m_setLife.size() > 0)
        {
            int nLife = m_setLife.front();
            m_setLife.erase(m_setLife.begin());
            this->AddZoomNum(nLife, 0xff0000ff, 300, 30);
            int nRealLife = (int)this->GetData(CPlayer::_PLAYER_LIFE) - nLife;
            this->SetData(CPlayer::_PLAYER_LIFE, __max(0, nRealLife));
            this->SetActionCmd(_ACTION_BRUISE0);
        }
        return;
    }
    if (this->IsDead())
    {
        this->ResetActionData();
        // reset cmd in queue
        int nCmds = m_PlayerInfo.setCmd.size();
        for (int i = 0; i < nCmds; i++)
        {
            CCommand cmd = m_PlayerInfo.setCmd[i];
            CRole::SetCommand(&cmd);
            CRole::ProcessCommand();
            this->ResetActionData();
        }
        m_PlayerInfo.setCmd.clear();
        while(true)
        {
            if (m_setLife.size() > 0)
            {
                int nLife = m_setLife.front();
                m_setLife.erase(m_setLife.begin());
                this->AddZoomNum(nLife, 0xff0000ff, 300, 30);
                int nRealLife = (int)this->GetData(CPlayer::_PLAYER_LIFE) - nLife;
                this->SetData(CPlayer::_PLAYER_LIFE, __max(0, nRealLife));
            }
            else
            {
                break;
            }
        }
        return;
    }
    this->SetWorldPos(m_Info.posActionEnd);
    CMyPos posCell;
    g_objGameMap.World2Cell(m_Info.posActionEnd.x, m_Info.posActionEnd.y, posCell.x, posCell.y);
    this->SetPos(posCell);
    if (_CMDSTATUS_BEGIN == m_Info.cmdProc.iStatus)
    {
        this->ProcessCommand();
    }
    m_Info.iActFrameIndex	= m_Info.iActFrameAmount;
    if (m_setCmd.size() > 0)
    {
        CCommand* pCmd = m_setCmd.front();
        if (m_setLife.size() > 0)
        {
            int nLife = m_setLife.front();
            m_setLife.erase(m_setLife.begin());
            this->AddZoomNum(nLife, 0xff0000ff, 300, 30);
            int nRealLife = (int)this->GetData(CPlayer::_PLAYER_LIFE) - nLife;
            this->SetData(CPlayer::_PLAYER_LIFE, __max(0, nRealLife));
        }
        CRole::SetCommand(pCmd);
        if (strcmp(pCmd->szString0, "NULL") != 0)
        {
            m_objEffect.Add(pCmd->szString0, false, 0, this->GetLook());
        }
        if (strcmp(pCmd->szString1, "NULL") != 0)
        {
			if(strlen(pCmd->szString1) > 0)
			{
				::DXPlaySound(pCmd->szString1);
			}
            
        }
        delete pCmd;
        m_setCmd.erase(m_setCmd.begin());
    }
    else
    {
        if (m_setLife.size() > 0)
        {
            int nLife = m_setLife.front();
            m_setLife.erase(m_setLife.begin());
            this->AddZoomNum(nLife, 0xff0000ff, 300, 30);
            int nRealLife = (int)this->GetData(CPlayer::_PLAYER_LIFE) - nLife;
            this->SetData(CPlayer::_PLAYER_LIFE, __max(0, nRealLife));
        }
    }
}//////////////////////////////////////////////////////////////////////
void CPlayer::SetTimerCommand(CCommand& cmd, DWORD tmExecute, const char* pszEffect, const char* pszSound)
{
    TimerCmdInfo info;
    info.cmd = cmd;
    info.tmExecute = tmExecute;
    if (pszEffect)
    {
        info.strEffect = pszEffect;
    }
    if (pszSound)
    {
        info.strSound = pszSound;
    }
    m_setCmdList.push_back(info);
}

//////////////////////////////////////////////////////////////////////
void CPlayer::ClrTimerCommand()
{
    m_setCmdList.clear();
}

//////////////////////////////////////////////////////////////////////
bool CPlayer::ProcessTimerCommand(void)
{
    if (m_setCmdList.size() <= 0)
    {
        return false;
    }
    std::list< TimerCmdInfo >::iterator iter = m_setCmdList.begin();
    for ( ; iter != m_setCmdList.end(); iter++)
    {
        TimerCmdInfo& info = (*iter);
        if (::TimeGet() >= info.tmExecute)
        {
            CRole::SetCommand(&info.cmd);
            // 播放特效
            if (info.strEffect.size())
            {
                // g_objGameMap.Add3DMapEffect(m_Info.posWorld, info.strEffect.c_str(), TRUE, this->GetDir());
                m_objEffect.Add((char*)info.strEffect.c_str(), false, 0, this->GetLook());
            }
            // 播放声效
            if (info.strSound.size())
            {
                CMyPos pos;
                this->GetWorldPos(pos);
                ::DXPlaySound((char*)info.strSound.c_str(), pos.x, pos.y, 2000);
            }
            // 从时间命令列表中删除
            m_setCmdList.erase(iter);
            return true;
        }
    }
    return false;
}
//////////////////////////////////////////////////////////////////////
void CPlayer::Process(void* pInfo)
{
    if (!m_pIRoleView)
    {
        return;
    }
    if (!(m_Info.cmdProc.iType == _COMMAND_EMOTION && (m_Info.cmdProc.nData == _ACTION_INTONE
            || m_Info.cmdProc.nData == _ACTION_INTONE_DURATION) || (m_Info.cmdProc.iType == _COMMAND_INTONE &&
                    m_Info.cmdProc.nData == _ACTION_INTONE_LAUNCH)))
    {
        m_Info.nFlyOffsetGoal = 0;
    }
    /*	if (!this->TestStatus(USERSTATUS_FLY))
    		this->SetFlyOffsetGoal(0);
    	else
    		this->SetFlyOffsetGoal(100);
    */
    this->ProcessAniEffect();

    if (this->ProcessAction())
    {
        BOOL bProcess = true;
        if (g_objPlayerSet.GetPlayer(m_idLocker, true) && m_idLocker != this->GetID())
        {
            bProcess = false;
        }
        if (m_setCmd.size() > 0 && bProcess)
        {
            CCommand* pCmd = m_setCmd.front();
            if (m_setLife.size() > 0)
            {
                int nLife = m_setLife.front();
                m_setLife.erase(m_setLife.begin());
                this->AddZoomNum(nLife, 0xff0000ff, 300, 30);
            }
            if (strcmp(pCmd->szString0, "NULL") != 0)
            {
                m_objEffect.Add(pCmd->szString0, false, 0, this->GetLook());
            }
            if (strcmp(pCmd->szString1, "NULL") != 0)
            {
                ::DXPlaySound(pCmd->szString1);
            }
            CRole::SetCommand(pCmd);
            delete pCmd;
            m_setCmd.erase(m_setCmd.begin());
            if (m_idLocker == this->GetID())
            {
                // 告诉所有被我锁定者，同步动作
                int nAmount = g_objPlayerSet.GetPlayerAmount();
                for(int i = 0; i < nAmount + 1; i ++)
                {
                    CPlayer* pPlayer;
                    if (i < nAmount)
                    {
                        pPlayer = g_objPlayerSet.GetPlayerByIndex(i);
                    }
                    else
                    {
                        pPlayer = &g_objHero;
                    }
                    if (pPlayer && pPlayer->m_idLocker == this->GetID()
                            && pPlayer->GetID() != this->GetID())
                    {
                        pPlayer->SynTrack();
                    }
                    /*					else if (pPlayer	&& pPlayer->GetID() != this->GetID())
                    					{
                    						pPlayer->SynTrack(false);
                    					}*/
                }
            }
        }
        else if (m_PlayerInfo.setCmd.size() > 0)
        {
            CCommand cmd1 = m_PlayerInfo.setCmd.front();
            CRole::SetCommand(&cmd1);
			m_PlayerInfo.setCmd.pop_front();
        }
        this->ProcessCommand();
        this->SetActionData();
    }
    else
    {
        // 只有在没有命令执行的时候才去找延时的命令执行
        if (this->IsIdle())
        {
            if (this->ProcessTimerCommand())
            {
                this->ProcessCommand();
                this->SetActionData();
            }
        }
    }
    if (this->GetActionType() == _ACTION_WALKL ||
        this->GetActionType() == _ACTION_WALKR ||
        this->GetActionType() == _ACTION_RUNL ||
        this->GetActionType() == _ACTION_RUNR)
    {
        CMyPos posCurrent = m_Info.posMap;
        CMyPos posPreStep;
        int nDir = this->GetDir();
        if (nDir > 3)
        {
            g_objGameMap.World2Cell(m_Info.posActionEnd.x, m_Info.posActionEnd.y, posPreStep.x, posPreStep.y);
        }
        else
        {
            g_objGameMap.World2Cell(m_Info.posActionBegin.x, m_Info.posActionBegin.y, posPreStep.x, posPreStep.y);
        }
        m_Info.posMap = posPreStep;
        if (g_objGameMap.IsPosVisible(m_Info.posMap, 2))
        {
            g_objGameMap.ApplyObjShow(this);
        }
        m_Info.posMap = posCurrent;
    }
    else
    {
        if (g_objGameMap.IsPosVisible(m_Info.posMap, 2))
        {
            g_objGameMap.ApplyObjShow(this);
        }
    }
    if (strlen(m_szCryOut) > 0)
    {
        if (RandGet(100) == 1)
        {
            this->Say(m_szCryOut);
        }
    }
    m_objFStr.Process();
    // flyoffset ...
    const int nStepUp = 1;
    const int nStepDn = 4;
    if (m_Info.nFlyOffset < m_Info.nFlyOffsetGoal)
    {
        m_Info.nFlyOffset += nStepUp;
        if (m_Info.nFlyOffset > m_Info.nFlyOffsetGoal)
        {
            m_Info.nFlyOffset = m_Info.nFlyOffsetGoal;
        }
    }
    if (m_Info.nFlyOffset > m_Info.nFlyOffsetGoal)
    {
        m_Info.nFlyOffset -= nStepDn;
        if (m_Info.nFlyOffset < m_Info.nFlyOffsetGoal)
        {
            m_Info.nFlyOffset = m_Info.nFlyOffsetGoal;
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CPlayer::SetMate(const char* pszMate)
{
    if (!pszMate || _MAX_NAMESIZE < strlen(pszMate))
    {
        return;
    }
    strcpy(m_PlayerInfo.szMate, pszMate);
}


//-------------------------------------------------------------------------------
void CPlayer::DoLevelUp()
{
    m_PlayerInfo.nExp = 0;
    if (this->IsPet())
    {
        this->m_objEffect.Add("petLevelup", false, 0, this->GetLook());
    }
    else
    {
        if (this->GetID() == g_objHero.GetID())
        {
            this->m_objEffect.Add("Levelup_self", false, 0, this->GetLook());
        }
        this->m_objEffect.Add("Levelup", false, 0, this->GetLook());
    }
    if (this->IsVisible())
    {
        CMyPos posWorld = { 0, 0 };
        this->GetWorldPos(posWorld);
        if (this->IsPet())
        {
            ::DXPlaySound("sound/monster_lev_up.wav", posWorld.x, posWorld.y, 2000);
        }
        else if (this->IsPlayer())
        {
            ::DXPlaySound("sound/role_lev_up.wav", posWorld.x, posWorld.y, 2000);
        }
    }
}

//-------------------------------------------------------------------------------
void CPlayer::SetFlashCrime ()
{
    if (!this->TestStatus(USERSTATUS_CRIME))
    {
        return;
    }
    if (!m_bFlashCrime && m_dwFlashCrimeTime == 0)
    {
        m_dwFlashCrimeTime = ::TimeGet();
        m_bFlashCrime = TRUE;
    }
    else if (m_dwFlashCrimeTime != 0
             && ::TimeGet () - m_dwFlashCrimeTime < FLASHCRIME_TIME
             && m_bFlashCrime)
    {
        m_dwFlashCrimeTime = ::TimeGet ();
    }
}
//-------------------------------------------------------------------------------
BOOL CPlayer::IsFlashCrime ()
{
    if (!this->TestStatus(USERSTATUS_CRIME))
    {
        m_dwFlashCrimeTime = 0;
        m_bFlashCrime = FALSE;
    }
    else if (::TimeGet () - m_dwFlashCrimeTime <= FLASHCRIME_TIME
             && m_bFlashCrime)
    {
        m_dwFlashCrimeTime = 0;
        m_bFlashCrime = FALSE;
    }
    return m_bFlashCrime;
}
//-------------------------------------------------------------------------------
void CPlayer::ReplaceStatus	(__int64 dwStatus)
{
    // team leader
    this->ProcessStatusEffect(dwStatus);
    // XP FULL
    if (this->TestStatus(USERSTATUS_XPFULL))
    {
        if ((dwStatus & USERSTATUS_XPFULL))
        {
            m_dwKoScale = 100;
            m_PlayerInfo.dwKoAmount = 0;
        }
    }
    // tornado
    /*	if (dwStatus&USERSTATUS_TORNADO)
    	{
    		if (!this->TestStatus(USERSTATUS_TORNADO))
    		{
    			m_dwKoScale = 100;
    		}
    	}
    	else
    	{
    		if (this->TestStatus(USERSTATUS_TORNADO))
    		{
    			m_PlayerInfo.dwKoAmount = 0;
    		}
    	}*/
    // body shield
    /*	if (dwStatus&USERSTATUS_WEAPONDAMAGE)
    	{
    		if (!this->TestStatus(USERSTATUS_WEAPONDAMAGE))
    		{

    			this->ResetStepDir();
    			this->SetActionCmd(_ACTION_BODYSHIELD);
    		}
    	}*/
    // god believe
    if (dwStatus & USERSTATUS_MAGICDAMAGE)
    {
        if (!this->TestStatus(USERSTATUS_MAGICDAMAGE))
        {
            this->ResetStepDir();
            //			this->SetActionCmd(_ACTION_GODBELIEVE);
        }
    }
    // die
    if (dwStatus & USERSTATUS_DIE)
    {
    }
    else
    {
        // reborn
        if (TestStatus(USERSTATUS_DIE))
        {
            this->StandBy();
        }
    }
    // disapear...
    if (dwStatus & USERSTATUS_DISAPPEARING && !this->TestStatus(USERSTATUS_DISAPPEARING))
    {
        m_tmDisappear = ::TimeGet();
        CMonsterInfo* pInfo = (CMonsterInfo*)g_obj3DRoleData.GetMonsterInfo(this->GetMonsterTypeId());
        if (pInfo && strcmp(pInfo->szDisappearEffect, "NULL") != 0)
        {
            this->m_objEffect.Add(pInfo->szDisappearEffect, false, 0, this->GetLook());
        }
        if (pInfo && strcmp(pInfo->szDisappearSound, "NULL") != 0)
        {
            CMyPos posWorld;
            this->GetWorldPos(posWorld);
            ::DXPlaySound(pInfo->szDisappearSound, posWorld.x, posWorld.y, 1000);
        }
    }
    // set status.
    m_dwStatus = dwStatus;
}
//-------------------------------------------------------------------------------
OBJID CPlayer::GetSydicateID()
{
    return m_PlayerInfo.idSyndicate;
}
//-------------------------------------------------------------------------------
void CPlayer::SetSyndicateID(OBJID idSyndicate)
{
    m_PlayerInfo.idSyndicate = idSyndicate;
}
//-------------------------------------------------------------------------------
DWORD CPlayer::GetSyndicateRank()
{
    return m_PlayerInfo.dwSyndicateRank;
}
//-------------------------------------------------------------------------------
void CPlayer::SetSyndicateRank(DWORD dwRank)
{
    m_PlayerInfo.dwSyndicateRank = dwRank;
}
//-------------------------------------------------------------------------------
int CPlayer::GetSyndicateProffer()
{
    return m_PlayerInfo.nSyndicateProffer;
}
//-------------------------------------------------------------------------------
void CPlayer::SetSyndicateProffer(int nSyndicateProffer)
{
    m_PlayerInfo.nSyndicateProffer = nSyndicateProffer;
}
//-------------------------------------------------------------------------------
char* CPlayer::GetSyndicate()
{
    return g_objPlayerSet.GetSyndicateName(m_PlayerInfo.idSyndicate);
}
//-------------------------------------------------------------------------------
char* CPlayer::GetSubSyndicate()
{
    return g_objPlayerSet.GetSyndicateLeaderName(m_PlayerInfo.idSyndicate);
}
//-------------------------------------------------------------------------------
char* CPlayer::GetSyndicateTitle()
{
    int i;
    for(i = 0; i < RANKAMOUNT; i++)
    {
        if (SYNDICATERANKINFOSET[i].dwRank == m_PlayerInfo.dwSyndicateRank)
        {
            return (char*)g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[i].idTitle);
        }
    }
    // error ...
    for(i = 0; i < RANKAMOUNT; i++)
    {
        if (SYNDICATERANKINFOSET[i].dwRank == RANK_NONE)
        {
            return (char*)g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[i].idTitle);
        }
    }
    return NULL;
}
//-------------------------------------------------------------------------------
char*	CPlayer::GetSyndicateLeaderName()
{
    return m_PlayerInfo.szSyndicateLeader;
}
//------------------------------------------------------------------
void	CPlayer::SetSyndicateLeaderName(char* pszLeader)
{
    if (pszLeader)
    {
        strcpy(m_PlayerInfo.szSyndicateLeader, pszLeader);
    }
}
//------------------------------------------------------------------
DWORD	CPlayer::GetSyndicateFund()
{
    return m_PlayerInfo.dwSyndicateFund;
}
//------------------------------------------------------------------
void	CPlayer::SetSyndicateFund(DWORD dwFund)
{
    m_PlayerInfo.dwSyndicateFund = dwFund;
}
//------------------------------------------------------------------
void	CPlayer::SetSyndicateFamer(DWORD dwSyndicateFamer)
{
    m_PlayerInfo.dwSyndicateFamer = dwSyndicateFamer;
}
//------------------------------------------------------------------
DWORD	CPlayer::GetSyndicateFamer()
{
    return m_PlayerInfo.dwSyndicateFamer;
}
//------------------------------------------------------------------
DWORD	CPlayer::GetSyndicatePopulation()
{
    return m_PlayerInfo.dwSyndicatePopulation;
}
//------------------------------------------------------------------
void	CPlayer::SetSyndicatePopulation(DWORD dwPopulation)
{
    m_PlayerInfo.dwSyndicatePopulation = dwPopulation;
}
//------------------------------------------------------------------

void CPlayer::SetMaxLife(DWORD dwMaxLife)
{
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_MAX_LIFE, dwMaxLife);
    }
    m_PlayerInfo.dwMaxLife = dwMaxLife;
}

//------------------------------------------------------------------
DWORD CPlayer::GetLife()
{
    return m_PlayerInfo.dataLife.GetValue();
}

//------------------------------------------------------------------
void CPlayer::SetLife(DWORD dwLife)
{
    if (m_pIRoleView)
    {
        m_pIRoleView->SetVariable(_VAR_LIFE, dwLife);
    }
    //m_PlayerInfo.dwLife = dwLife;
    m_PlayerInfo.dataLife.SetValue(dwLife);
}

//------------------------------------------------------------------
DWORD CPlayer::GetMana()
{
    return m_PlayerInfo.dataMana.GetValue();
}

//------------------------------------------------------------------
void CPlayer::SetMana(DWORD dwMana)
{
    int nMana = m_PlayerInfo.dataMana.GetValue();
    if (nMana > (int)dwMana && g_objHero.GetID() == this->GetID())
    {
        g_objHero.m_objCheck.DecMana();
    }
    m_PlayerInfo.dataMana.SetValue(dwMana);
}

//------------------------------------------------------------------
char* CPlayer::GetPkTitle()
{
    return g_obj3DRoleData.GetPkTitle(m_PlayerInfo.nPk);
}
//------------------------------------------------------------------
void CPlayer::AddProfessionalCoolposEffect(DWORD dwProfessional, BOOL bAllNosuch/* = true*/)
{
    /*	switch(dwProfessional)
    	{
    		case PROFSORT_WARRIOR:
    			if (bAllNosuch)
    				this->m_objEffect.Add("warrior");
    			else
    				this->m_objEffect.Add("warrior-s");
    			break;
    		case PROFSORT_SOLDIER:
    			if (bAllNosuch)
    				this->m_objEffect.Add("fighter");
    			else
    				this->m_objEffect.Add("fighter-s");
    			break;
    		case PROFSORT_XIAKE:
    			// to do ...
    			break;
    		case PROFSORT_ARCHER:
    			if (bAllNosuch)
    				this->m_objEffect.Add("archer");
    			else
    				this->m_objEffect.Add("archer-s");
    			break;
    		case PROFSORT_DAOIST:
    		case PROFSORT_DAOISTGOLD:
    		case PROFSORT_DAOISTWOOD:
    		case PROFSORT_DAOISTWATER:
    		case PROFSORT_DAOISTFIRE:
    		case PROFSORT_DAOISTEARTH:
    			if (bAllNosuch)
    				this->m_objEffect.Add("taoist");
    			else
    				this->m_objEffect.Add("taoist-s");
    			break;
    	}*/
}
//--------------------------------------------------------------------------------------
void CPlayer::AddMoneyEffect(DWORD dwMoney)
{
    char szStr[64];
    sprintf(szStr, "$ %u", dwMoney);
    if (dwMoney >= 1000 || this->TestStatus(USERSTATUS_ADJUST_DROPMONEY))
    {
        this->m_objEffect.Add("muchmoney", false, 0, this->GetLook());
        this->AddFStr(szStr, 64, 0x00ffffff, 3, 1000);
    }
    else
    {
        this->m_objEffect.Add("money", false, 0, this->GetLook());
        this->AddFStr(szStr);
    }
}

//------------------------------------------------------------------
BOOL CPlayer::IsBaddy()
{
    if (this->TestStatus(USERSTATUS_CRIME) || this->TestStatus(USERSTATUS_RED)
            || this->TestStatus(USERSTATUS_DARKRED))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------
DWORD CPlayer::GetCurLife()
{
    return m_PlayerInfo.dataCurLife.GetValue();
}
//------------------------------------------------------------------
void CPlayer::SetCurLife(int nLife)
{
    m_PlayerInfo.dataCurLife.SetValue(nLife);
}
//------------------------------------------------------------------
DWORD CPlayer::GetCurMana()
{
    return m_PlayerInfo.dataCurMana.GetValue();
}
//------------------------------------------------------------------
void CPlayer::SetCurMana(int nMana)
{
    m_PlayerInfo.dataCurMana.SetValue(nMana);
}
//------------------------------------------------------------------
void CPlayer::ShowKoAmount(BOOL bImmedia)
{
    if (!this->TestStatus(USERSTATUS_XPFULL))
    {
        return;
    }
    int nNameShowOffset = this->GetZoomPercent() * (OFFSET_NAMESHOW + 50) / 100 + this->GetOffsetY();
    int nShowX	= m_Info.posScr.x;
    int nShowY	= m_Info.posScr.y - nNameShowOffset;
    g_objGameMap.MapScaleShowPos(nShowX, nShowY);
    CAni* pAni = g_objGameDataSet.GetDataAni("ani/common.ani", "Ko", EXIGENCE_IMMEDIATE);
    if (pAni)
    {
        CMyBitmap* pBmp = pAni->GetFrame(0);
        if (pBmp)
        {
            CMySize sizeKO;
            pBmp->GetSize(sizeKO.iWidth, sizeKO.iHeight);
            sizeKO.iWidth = sizeKO.iWidth * 2 / 3;
            sizeKO.iHeight = sizeKO.iHeight * 2 / 3;
            CMyPos posShow = {_SCR_WIDTH - 60, _SCR_HEIGHT - 220};
            posShow.x = nShowX - sizeKO.iWidth * 4 * this->GetZoomPercent() / 3 / 100;
            posShow.y = nShowY - sizeKO.iHeight * this->GetZoomPercent() / 2 / 100;
            pBmp->ShowEx(posShow.x, posShow.y, NULL, sizeKO.iWidth, sizeKO.iHeight);
            CMyPos posKo = {posShow.x + sizeKO.iWidth* this->GetZoomPercent() * 2 / 100, posShow.y + sizeKO.iHeight* this->GetZoomPercent() / 2 / 100};
            m_objImgNumber.SetShowPos(posKo);
            char szInfo[256] = "";
            sprintf(szInfo, "%02u", this->GetKoAmount());
            m_objImgNumber.SetString(szInfo);
            if (m_dwKoScale > 50)
            {
                m_dwKoScale -= 10;
            }
            m_objImgNumber.SetScalePercent(100);
            if (this->TestStatus(USERSTATUS_XPFULL))
            {
                m_objImgNumber.SetColor(_COLOR_RED, 0xff);
            }
            else
            {
                m_objImgNumber.SetColor(_COLOR_GREEN, 0xff);
            }
            m_objImgNumber.Show();
        }
    }
}
//------------------------------------------------------------------
void	CPlayer::ProcessStatusEffect(__int64 dwStatus)
{
    int nAmount = g_obj3DRoleData.GetStatusEffectInfoAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        StatusEffectInfo* pInfo = g_obj3DRoleData.GetStatusEffectInfoByIndex(i);
        if (!pInfo)
        {
            continue;
        }
        if (dwStatus & pInfo->dwStatus)
        {
            if (!this->TestStatus(pInfo->dwStatus))
            {
                if (strcmp(pInfo->sz2DEffect, "NULL") != 0 && (!this->TestAniEffect(pInfo->sz2DEffect)))
                {
                    this->AddAniEffect(pInfo->sz2DEffect);
                }
                if (strcmp(pInfo->sz3DEffect, "NULL") != 0)
                {
                    m_objEffect.Add(pInfo->sz3DEffect);
                }
            }
        }
        else
        {
            if (this->TestStatus(pInfo->dwStatus))
            {
                if (strcmp(pInfo->sz2DEffect, "NULL") != 0)
                {
                    this->ClrAniEffect(pInfo->sz2DEffect);
                }
                if (strcmp(pInfo->sz3DEffect, "NULL") != 0)
                {
                    m_objEffect.Delete(pInfo->sz3DEffect);
                }
            }
        }
    }
}
//------------------------------------------------------------------
DWORD	CPlayer::GetData(DWORD dwDataType)
{
    if (m_pFD)
    {
        return m_pFD->GetValue(dwDataType);
    }
    return 0;
}
//------------------------------------------------------------------
void	CPlayer::SetData(DWORD dwDataType, DWORD dwData)
{
    if (g_objHero.GetID() == this->GetID() && _PLAYER_LIFE == dwDataType)
    {
        int nLife = this->GetData(_PLAYER_LIFE);
        if (nLife > (int)dwData)
        {
            g_objHero.m_objCheck.DecLife();
        }
    }
    if (m_pFD)
    {
        m_pFD->SetValue(dwData, dwDataType);
    }
}
//------------------------------------------------------------------
DWORD	CPlayer::GetMercenaryLevel()
{
    return m_PlayerInfo.dwMercenaryLevel;
}
//------------------------------------------------------------------
void	CPlayer::SetMercenaryLevel(DWORD dwMercenaryLevel)
{
    m_PlayerInfo.dwMercenaryLevel = dwMercenaryLevel;
}
//------------------------------------------------------------------
DWORD	CPlayer::GetMercenaryExp()
{
    return m_PlayerInfo.dwMercenaryExp;
}
//------------------------------------------------------------------
void	CPlayer::SetMercenaryExp(DWORD dwMercenaryExp)
{
    m_PlayerInfo.dwMercenaryExp = dwMercenaryExp;
}
//------------------------------------------------------------------
DWORD	CPlayer::GetSelectedHonorTitle()
{
    return m_PlayerInfo.dwSelectHonorTitle;
}
//------------------------------------------------------------------
void	CPlayer::SetSelectedHonorTitle(DWORD dwSelectedTitle)
{
    m_PlayerInfo.dwSelectHonorTitle = dwSelectedTitle;
}
//------------------------------------------------------------------
DWORD	CPlayer::GetNobilityRank()
{
    return m_PlayerInfo.dwNobilityRank;
}
//------------------------------------------------------------------
void	CPlayer::SetNobilityRank(DWORD dwNobilityRank)
{
    m_PlayerInfo.dwNobilityRank = dwNobilityRank;
}
//------------------------------------------------------------------
DWORD			CPlayer::GetHonorTitleAmount()
{
    return m_PlayerInfo.setHonorTitle.size();
}
//------------------------------------------------------------------
const char*			CPlayer::GetHororTitle(int nIndex)
{
    if (nIndex < 0 && nIndex >= (int)m_PlayerInfo.setHonorTitle.size())
    {
        return NULL;
    }
    return g_obj3DRoleData.GetHonorTitle(nIndex);
}
//------------------------------------------------------------------
unsigned char	CPlayer::GetHororTitleIndex(int nIndex)
{
    if (nIndex < 0 && nIndex >= (int)m_PlayerInfo.setHonorTitle.size())
    {
        return 0;
    }
    return m_PlayerInfo.setHonorTitle[nIndex];
}
//------------------------------------------------------------------
void			CPlayer::AddHororTitle(int nIndex)
{
    int i = 0;
    for(; i < (int)m_PlayerInfo.setHonorTitle.size(); i ++)
    {
        if (nIndex = m_PlayerInfo.setHonorTitle[i])
        {
            break;
        }
    }
    if (i == (int)m_PlayerInfo.setHonorTitle.size())
    {
        m_PlayerInfo.setHonorTitle.push_back(nIndex);
    }
}
//------------------------------------------------------------------
DWORD			CPlayer::GetMedalAmount()
{
    return (DWORD)m_PlayerInfo.setMedal.size();
}
//------------------------------------------------------------------
const char*			CPlayer::GetMedale(int nIndex)
{
    if (nIndex < 0 && nIndex >= (int)m_PlayerInfo.setMedal.size())
    {
        return NULL;
    }
    return g_obj3DRoleData.GetMedal(nIndex);
}
//------------------------------------------------------------------
unsigned char	CPlayer::GetMedaleIndex(int nIndex)
{
    if (nIndex < 0 && nIndex >= (int)m_PlayerInfo.setMedal.size())
    {
        return 0;
    }
    return m_PlayerInfo.setMedal[nIndex];
}
//------------------------------------------------------------------
void	CPlayer::AddMedaleTitle(int nIndex)
{
    int i = 0;
    for(; i < (int)m_PlayerInfo.setMedal.size(); i ++)
    {
        if (nIndex = m_PlayerInfo.setMedal[i])
        {
            break;
        }
    }
    if (i == m_PlayerInfo.setMedal.size())
    {
        m_PlayerInfo.setMedal.push_back(nIndex);
    }
}
//------------------------------------------------------------------
char*	CPlayer::GetNobilityTitle()
{
    return (char*)g_obj3DRoleData.GetNobilityTitle(this->GetSelectedHonorTitle());
}
//------------------------------------------------------------------
void	CPlayer::SetTutorLevel(DWORD dwTutorLevel)
{
    m_PlayerInfo.dwTutorLevel = dwTutorLevel;
}
//------------------------------------------------------------------
void	CPlayer::SetTutorExp(int dwTutorExp)
{
    m_PlayerInfo.dwTutorExp = dwTutorExp;
}
//------------------------------------------------------------------
DWORD   CPlayer::GetTutorLevel()
{
    return m_PlayerInfo.dwTutorLevel;
}
//------------------------------------------------------------------
int		CPlayer::GetTutorExp()
{
    return m_PlayerInfo.dwTutorExp;
}
//------------------------------------------------------------------
void	CPlayer::SetNpcTitle(const char* pszTitle)
{
    if (pszTitle)
    {
        strcpy(m_szNpcTitle, pszTitle);
    }
}

void CPlayer::SetSyndicateLevel(UINT uiLevel)
{
    m_PlayerInfo.uiSyndicateLevel = uiLevel;
}

UINT CPlayer::GetSyndicateLevel()
{
    return m_PlayerInfo.uiSyndicateLevel;
}
