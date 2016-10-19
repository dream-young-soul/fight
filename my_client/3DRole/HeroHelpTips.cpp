
//**********************************************************
// 代码编辑器
//**********************************************************

//---------------------------------------------------------------------------
// HeroHelpTips.cpp
//---------------------------------------------------------------------------
#include "hero.H"
#include "3DGameMap.h"
#include "3dRoleData.h"
#include "GamePlayerSet.h"

const int _LOGIN_TIME = 15000;
const int _MAX_LOOPTIME = 3;
const int _MAX_STAYTIME = 3000;
const int _MOVE_SPEED = 33;

void CHero::ClearHelpTip()
{
    m_nShowIndex = 0;
    m_nResetIndex = 0;
    m_dwLoginTime = 0;
    m_dwStartTime = 0;
    m_dwStayTime = 0;
    m_nLoopTime = 0;
    m_bSetHelpTip = FALSE;
    m_bShowFlyHelp = true;
    m_bCanSetHelpTips = false;
    int nAmount = m_setHelpTip.size();
    for(int i = 0; i < nAmount; i ++)
    {
        SAFE_DELETE(m_setHelpTip[i]);
    }
    m_setHelpTip.clear();
}

void CHero::ShowHelpTips(int posShowX, int posShowY, int nWidth, DWORD dwColor, DWORD dwBkColor)
{
    if (!m_bCanSetHelpTips)
    {
        if (m_dwLoginTime == 0)
        {
            m_dwLoginTime = ::TimeGet();
        }
        if ( ::TimeGet() - m_dwLoginTime >= _LOGIN_TIME)
        {
            this->SetHelpTip();
            m_bCanSetHelpTips = true;
        }
        else
        {
            return;
        }
    }
    int nAmountTip = this->GetFlyHelpAmount();
    if (nAmountTip == 0 || m_nShowIndex >= nAmountTip)
    {
        return;
    }
    CMySize nFontSize;
    CMyBitmap::GetFontSize(nFontSize);
    CMyPos posB = {posShowX, posShowY};
    CMyPos posE = {posShowX + nWidth, posShowY + nFontSize.iHeight};
    char strTemp[256] = "";
    strcpy(strTemp, this->GetFlyHelpByIndex(m_nShowIndex));
    if (strTemp && strlen(strTemp) > 0)
    {
        if (m_dwStartTime == 0)
        {
            m_dwStartTime = ::TimeGet();
        }
        int nStrlen = strlen(strTemp);
        int MoveX = posE.x - (::TimeGet() - m_dwStartTime) / _MOVE_SPEED;
        if (MoveX <= posShowX)
        {
            MoveX = posShowX;
            if (m_dwStayTime == 0)
            {
                m_dwStayTime = ::TimeGet();
            }
            if ((::TimeGet() - m_dwStayTime) >= _MAX_STAYTIME)
            {
                if (m_nShowIndex < nAmountTip - 1)
                {
                    if (m_nResetIndex != 0 && m_nShowIndex < m_nResetIndex)
                    {
                        m_nShowIndex = m_nResetIndex;
                    }
                    else
                    {
                        m_nShowIndex++;
                    }
                }
                else
                {
                    if (m_nLoopTime < _MAX_LOOPTIME)
                    {
                        m_nLoopTime++;
                    }
                    else
                    {
                        ActivedHelpTips(nAmountTip);
                        ResetFlyHelp();
                    }
                    m_nShowIndex = m_nResetIndex;
                }
                m_dwStartTime = 0;
                m_dwStayTime = 0;
            }
        }
        int nShowlen = (posE.x - MoveX  ) / nFontSize.iWidth;
        char strShow[256] = "";
        if (nShowlen > nStrlen)
        {
            nShowlen = nStrlen;
        }
        strncpy(strShow, strTemp, nShowlen);
        CMyBitmap::ShowBlock(MoveX - 1, posShowY - 1,
                             MoveX + nShowlen * nFontSize.iWidth ,
                             posShowY + nFontSize.iHeight,
                             dwBkColor);
        CMyBitmap::ShowString(MoveX, posShowY, dwColor, strShow);
    }
}

void CHero::SetTips()
{
    m_setTips.clear();
    int nProfessional = this->GetProfession() / 10;
    int nLevel = this->GetLev();
    int nSex =   this->GetSex();
    OBJID IdMap =	g_objGameMap.GetID();
    int nAmount = g_obj3DRoleData.GetUserHelpInfoAmount();
    for(int i = 0; i < nAmount; i++)
    {
        UserHelpInfo* pInfo = g_obj3DRoleData.GetUserHelpInfoByIndex(i);
        if (pInfo
                && ((nLevel <= pInfo->nMaxLevel
                     && nLevel >= pInfo->nMinLevel)
                    || (pInfo->nMaxLevel == 0 && pInfo->nMinLevel == 0))
                && (nProfessional == pInfo->nProfession || pInfo->nProfession == 0)
                && (nSex == pInfo->nSex || pInfo->nSex == 0)
                && (IdMap == pInfo->nMapId || pInfo->nMapId == 0))
        {
            BOOL bSetFlag = false;
            //查询物品
            if (pInfo->nItemType)
            {
                int nAmount = g_objHero.GetItemAmount(PACKAGE_BAG);
                int i = 0;
                for(i = 0; i < nAmount; i++)
                {
                    CItem* pItem = g_objHero.GetItemByIndex(i, PACKAGE_BAG);
                    if (pItem != NULL && pItem->GetTypeID() == pInfo->nItemType)
                    {
                        bSetFlag = true;
                        break;
                    }
                }
                if (!bSetFlag)
                {
                    nAmount = g_objHero.GetItemAmount(PACKAGE_PET);
                    for(i = 0; i < nAmount; i++)
                    {
                        CItem* pItem = g_objHero.GetItemByIndex(i, PACKAGE_PET);
                        if (pItem != NULL && pItem->GetTypeID() == pInfo->nItemType)
                        {
                            bSetFlag = true;
                            break;
                        }
                    }
                }
                if (!bSetFlag)
                {
                    nAmount = g_objHero.GetItemAmount(PACKAGE_EGG);
                    for(i = 0; i < nAmount; i++)
                    {
                        CItem* pItem = g_objHero.GetItemByIndex(i, PACKAGE_EGG);
                        if (pItem != NULL && pItem->GetTypeID() == pInfo->nItemType)
                        {
                            bSetFlag = true;
                            break;
                        }
                    }
                }
                if (!bSetFlag)
                {
                    nAmount = g_objHero.GetItemAmount(PACKAGE_SOUL);
                    for(i = 0; i < nAmount; i++)
                    {
                        CItem* pItem = g_objHero.GetItemByIndex(i, PACKAGE_SOUL);
                        if (pItem != NULL && pItem->GetTypeID() == pInfo->nItemType)
                        {
                            bSetFlag = true;
                            break;
                        }
                    }
                }
                if (!bSetFlag)
                {
                    return;
                }
            }
            //查询技能
            if (pInfo->nMagicId)
            {
                int	nAmount = g_objHero.GetMagicAmount();
                int i = 0;
                for(i = 0; i < nAmount; i++)
                {
                    CMagic* pMagic = g_objHero.GetMagicByIndex(i);
                    if (pMagic != NULL
                            && pMagic->GetIDType() == pInfo->nMagicId
                            && pMagic->GetLevel() == pInfo->nMagicLevel)
                    {
                        bSetFlag = true;
                        break;
                    }
                }
                if (!bSetFlag)
                {
                    nAmount = g_objHero.GetTeamMagicAmount();
                    for(i = 0; i < nAmount; i++)
                    {
                        CMagic* pMagic = g_objHero.GetTeamMagicByIndex(i);
                        if (pMagic != NULL
                                && pMagic->GetIDType() == pInfo->nMagicId
                                && pMagic->GetLevel() == pInfo->nMagicLevel)
                        {
                            bSetFlag = true;
                            break;
                        }
                    }
                }
                if (!bSetFlag)
                {
                    return;
                }
            }
            int nStatus = pInfo->nStatus;
            switch(nStatus)
            {
            case 0:	//无要求
                bSetFlag = true;
                break;
            case 1:	// 无任何身份
                {
                    if (!this->IsHaveStudent()
                            && !this->IsHaveTeacher()
                            && strcmp(this->GetMate(), g_objGameDataSet.GetStr(100122)) != 0
                            && this->GetNobilityRank() == 0
                            && this->GetMercenaryLevel() == 0)
                    {
                        bSetFlag = true;
                    }
                }
                break;
            case 2: // 师傅
                if (this->IsHaveStudent()
                        && this->GetTutorLevel() == pInfo->nStatusLevel)
                {
                    bSetFlag = true;
                }
                break;
            case 3: // 徒弟
                if ((this->IsHaveTeacher() && pInfo->nStatusLevel == 1)
                        || pInfo->nStatusLevel == 0)
                {
                    bSetFlag = true;
                }
                break;
            case 4: //爵位
                if (this->GetNobilityRank() == pInfo->nStatusLevel)
                {
                    bSetFlag = true;
                }
                break;
            case 5:	//婚姻
                if ((strcmp(this->GetMate(), g_objGameDataSet.GetStr(100122)) == 0 && pInfo->nStatusLevel == 1)
                        || pInfo->nStatusLevel == 0)
                {
                    bSetFlag = true;
                }
                break;
            case 6: // 佣兵
                if (this->GetMercenaryLevel() == pInfo->nStatusLevel)
                {
                    bSetFlag = true;
                }
                break;
            default:
                break;
            }
            if (bSetFlag)
            {
                int nLine = pInfo->m_setStr.size();
                char strTip[_MAX_WORDSSIZE] = "";
                for(int j = 0; j < nLine; j++)
                {
                    strcpy(strTip, pInfo->m_setStr[j].c_str());
                    m_setTips.push_back(strTip);
                }
                strcpy(strTip, "$");
                m_setTips.push_back(strTip);
            }
        }
    }
}


void CHero::SetHelpTip(OBJID IdItemType, OBJID IdMagic, int nMagicLevel, BOOL bLevelUp)
{
    int nProfessional = this->GetProfession() / 10;
    int nLevel = this->GetLev();
    int nSex =   this->GetSex();
    OBJID IdMap = 0;
    if (m_bCanSetHelpTips)
    {
        IdMap =	g_objGameMap.GetID();
    }
    int nAmount = g_obj3DRoleData.GetUserHelpInfoAmount();
    for(int i = 0; i < nAmount; i++)
    {
        UserHelpInfo* pInfo = g_obj3DRoleData.GetUserHelpInfoByIndex(i);
        if (pInfo
                && ((nLevel <= pInfo->nMaxLevel
                     && nLevel >= pInfo->nMinLevel)
                    || (pInfo->nMaxLevel == 0 && pInfo->nMinLevel == 0))
                && (nProfessional == pInfo->nProfession || pInfo->nProfession == 0)
                && (nSex == pInfo->nSex || pInfo->nSex == 0)
                && (IdMap == pInfo->nMapId || pInfo->nMapId == 0)
                && IdItemType == pInfo->nItemType
                && IdMagic == pInfo->nMagicId
                && nMagicLevel == pInfo->nMagicLevel
                && this->CheckHelpTip(i))
        {
            if (!m_bCanSetHelpTips && pInfo->nStatus != 0)
            {
                continue;
            }
            if (!bLevelUp
                    && IdItemType == 0
                    && IdMagic == 0
                    && pInfo->nMaxLevel > 1
                    && pInfo->nMinLevel > 2)
            {
                continue;
            }
            BOOL bSetFlag = false;
            int nStatus = pInfo->nStatus;
            switch(nStatus)
            {
            case 0:	//无要求
                bSetFlag = true;
                break;
            case 1:	// 无任何身份
                {
                    if (!this->IsHaveStudent()
                            && !this->IsHaveTeacher()
                            && strcmp(this->GetMate(), g_objGameDataSet.GetStr(100122)) != 0
                            && this->GetNobilityRank() == 0
                            && this->GetMercenaryLevel() == 0)
                    {
                        bSetFlag = true;
                    }
                }
                break;
            case 2: // 师傅
                if (this->IsHaveStudent()
                        && this->GetTutorLevel() == pInfo->nStatusLevel)
                {
                    bSetFlag = true;
                }
                break;
            case 3: // 徒弟
                if ((this->IsHaveTeacher() && pInfo->nStatusLevel == 1)
                        || pInfo->nStatusLevel == 0)
                {
                    bSetFlag = true;
                }
                break;
            case 4: //爵位
                if (this->GetNobilityRank() == pInfo->nStatusLevel)
                {
                    bSetFlag = true;
                }
                break;
            case 5:	//婚姻
                if ((strcmp(this->GetMate(), g_objGameDataSet.GetStr(100122)) == 0 && pInfo->nStatusLevel == 1)
                        || pInfo->nStatusLevel == 0)
                {
                    bSetFlag = true;
                }
                break;
            case 6: // 佣兵
                if (this->GetMercenaryLevel() == pInfo->nStatusLevel)
                {
                    bSetFlag = true;
                }
                break;
            default:
                break;
            }
            if (bSetFlag)
            {
                UserHelpTip* pHelpTip = new UserHelpTip;
                int nLine = pInfo->m_setStr.size();
                for(int j = 0; j < nLine; j++)
                {
                    pHelpTip->m_setStr.push_back(pInfo->m_setStr[j]);
                    pHelpTip->nHelpIndex = i;
                    pHelpTip->bActived = false;
                }
                m_setHelpTip.push_back(pHelpTip);
            }
        }
    }
    m_bSetHelpTip = TRUE;
}


const char* CHero::GetFlyHelpByIndex(int nIndex)
{
    int nAmount = m_setHelpTip.size();
    int nAmountRe = 0;
    if (nIndex < (int)m_setHelpTip.size())
    {
        for(int i = 0; i < nAmount; i++)
        {
            UserHelpTip* pHelpTip = m_setHelpTip[i];
            if (pHelpTip && !pHelpTip->bActived)
            {
                if (nIndex == nAmountRe)
                {
                    return pHelpTip->m_setStr[0].c_str();
                }
                nAmountRe++;
            }
        }
    }
    return NULL;
}

int CHero::GetFlyHelpAmount()
{
    int nAmount = m_setHelpTip.size();
    int nAmountRe = 0;
    for(int i = 0; i < nAmount; i++)
    {
        UserHelpTip* pUserHelpTip = m_setHelpTip[i];
        if (pUserHelpTip && !pUserHelpTip->bActived)
        {
            nAmountRe++;
        }
    }
    return nAmountRe;
}

void CHero::ResetFlyHelp()
{
    m_nShowIndex = 0;
    m_nShowIndex = 0;
    m_dwStartTime = 0;
    m_dwStayTime = 0;
    m_nLoopTime = 0;
    m_nResetIndex = 0;
}

const char* CHero::GetHelpTipLineByIndex(int nIndex, int nLine)
{
    int nAmount = m_setHelpTip.size();
    int nAmountRe = 0;
    for (int i = 0; i < nAmount; i++)
    {
        UserHelpTip* pHelpTip = m_setHelpTip[i];
        if (pHelpTip && !pHelpTip->bActived)
        {
            if (nIndex == nAmountRe)
            {
                return pHelpTip->m_setStr[nLine].c_str();
            }
            nAmountRe++;
        }
    }
    return NULL;
}

int CHero::GetHelpTipAmount(int nIndex)
{
    int nAmount = m_setHelpTip.size();
    int nAmountRe = 0;
    for (int i = 0; i < nAmount; i++)
    {
        UserHelpTip* pHelpTip = m_setHelpTip[i];
        if (pHelpTip && !pHelpTip->bActived)
        {
            if (nAmountRe == nIndex)
            {
                return pHelpTip->m_setStr.size();
            }
            nAmountRe++;
        }
    }
    return 0;
}

bool CHero::IsFlyHelpShow()
{
    int nAmount = this->GetFlyHelpAmount();
    if (nAmount > 0 )
    {
        m_bShowFlyHelp = true;
    }
    else
    {
        m_bShowFlyHelp = false;
    }
    return m_bShowFlyHelp;
}

void CHero::ActivedHelpTips(int nAmount)
{
    int nTAmount = m_setHelpTip.size();
    int nAmountRe = 0;
    if (nAmount <= nTAmount)
    {
        for(int i = 0; i < nTAmount; i++)
        {
            UserHelpTip* pHelpTip = m_setHelpTip[i];
            if (pHelpTip && !pHelpTip->bActived)
            {
                pHelpTip->bActived = true;
                nAmountRe++;
                if (nAmountRe == nAmount + 1)
                {
                    m_nResetIndex = 0;
                    return;
                }
            }
        }
    }
}

bool CHero::CheckHelpTip(int nIndex)
{
    int nAmount = m_setHelpTip.size();
    for(int i = 0; i < nAmount; i++)
    {
        UserHelpTip* pHelpTip = m_setHelpTip[i];
        if (pHelpTip && pHelpTip->nHelpIndex == nIndex)
        {
            return false;
        }
    }
    return true;
}

void CHero::ClearTips()
{
    m_setTips.clear();
}

const char* CHero::GetTipsByLine(int nLine)
{
    int nAmount = m_setTips.size();
    if (nAmount <= 0)
    {
        return NULL;
    }
    return m_setTips[nLine].c_str();
}

int CHero::GetTipsAmount()
{
    int nAmount = m_setTips.size();
    return nAmount;
}
