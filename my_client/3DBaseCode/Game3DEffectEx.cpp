
//**********************************************************
// 代码编辑器
//**********************************************************

// Game3DEffectEx.cpp: implementation of the CGame3DEffectEx class.
//
//////////////////////////////////////////////////////////////////////

#include "Game3DEffectEx.h"
#include "3DEffect.h"
#include "GamedataSet.h"
#include "3DRolePart.h"
#include "3DRoleData.h"
#include "3DRole.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame3DEffectEx::CGame3DEffectEx()
{
    m_setInfo.clear();
    this->Clear();
    m_nOffsetY = 0;
    m_Height = 0;
    m_bOblique = true;
    m_fX = 0.0;
    m_fY = 0.0;
    m_fZ = 0.0;
}

CGame3DEffectEx::~CGame3DEffectEx()
{
    this->Clear();
}

//-------------------------------------------------------------------
void CGame3DEffectEx::Show(CMyPos posWorld)
{
    int nAmount = m_setInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo)
        {
            CMyPos myPosWorld;
            myPosWorld = posWorld;
            myPosWorld.y += m_nOffsetY;
            this->Show(pInfo, myPosWorld);
        }
    }
}
//-------------------------------------------------------------------
DWORD CGame3DEffectEx::GetPartAmount(int nIndex)
{
    if (nIndex >= (int)m_setInfo.size() || nIndex < 0)
    {
        return 0;
    }
    Game3DEffectExInfo* pInfo = m_setInfo[nIndex];
    if (pInfo)
    {
        return pInfo->nInfoAmount;
    }
    return 0;
}
//-------------------------------------------------------------------
void  CGame3DEffectEx::SetBlend(int nIndex, int nPartIndex, int nASB, int nADB)
{
    if (nIndex >= (int)m_setInfo.size() || nIndex < 0)
    {
        return;
    }
    Game3DEffectExInfo* pInfo = m_setInfo[nIndex];
    if (pInfo)
    {
        if (nPartIndex < 0 || nPartIndex >= pInfo->nInfoAmount)
        {
            return;
        }
        if (!pInfo->pIDInfo)
        {
            return;
        }
        Game3DEffectIdInfo* pMyIDInfo = pInfo->pIDInfo + nPartIndex;
        pMyIDInfo->nASB = nASB;
        pMyIDInfo->nADB = nADB;
    }
}
//-------------------------------------------------------------------
void  CGame3DEffectEx::GetBlend(int nIndex, int nPartIndex, int& nASB, int& nADB)
{
    if (nIndex >= (int)m_setInfo.size() || nIndex < 0)
    {
        return;
    }
    Game3DEffectExInfo* pInfo = m_setInfo[nIndex];
    if (pInfo)
    {
        if (nPartIndex < 0 || nPartIndex >= pInfo->nInfoAmount)
        {
            return;
        }
        if (!pInfo->pIDInfo)
        {
            return;
        }
        Game3DEffectIdInfo* pMyIDInfo = pInfo->pIDInfo + nPartIndex;
        nASB = pMyIDInfo->nASB;
        nADB = pMyIDInfo->nADB;
    }
}
//-------------------------------------------------------------------
void CGame3DEffectEx::Show(Game3DEffectExInfo* pInfo,  C3DRolePart* pPart, const char* pszVMesh)
{
    if (!pInfo)
    {
        return;
    }
    if (!pInfo->bShow)
    {
        return;
    }
    if (!pPart)
    {
        return;
    }
    if (!pszVMesh)
    {
        return;
    }
    Game3DEffectExInfo* pMyInfo = pInfo;
    int nAmount = pInfo->nInfoAmount;
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectIdInfo* pIDInfo = pInfo->pIDInfo + i;
        // get effect
        C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIDInfo->idEffect);
        if (!pEffect)
        {
            return;
        }
        // get txeture of effect
        C3DTexture* pTexture = g_objGameDataSet.Get3DTexture(pIDInfo->idTexture);
        if (!pTexture)
        {
            return;
        }
        // set the texture
        pEffect->SetTexture(pTexture);
        pEffect->SetFrames(pMyInfo->dwCurrentFrameIndex);
        // restore shape info
        pEffect->RestoreShapeInfo(pIDInfo->pShapeInfoBuf);
        pEffect->m_nShapeAir = pMyInfo->nShapeAir;
        // setpos
        C3DObjPartInfo* pInfo = pPart->GetInfo(0);
        if (!pInfo)
        {
            return;
        }
        C3DObj* pObj = g_objGameDataSet.Get3DObj(pInfo->idMesh);
        if (!pObj)
        {
            return;
        }
        pEffect->Bind(pObj, (char*)pszVMesh);
        pEffect->Draw(false, pIDInfo->nASB, pIDInfo->nADB);
        pEffect->BackupShapeInfo(pIDInfo->pShapeInfoBuf);
    }
}

//-------------------------------------------------------------------
void CGame3DEffectEx::Show(C3DRole* p3DRole, CMyPos posWorld)
{
    if (!p3DRole && p3DRole->m_pArmor)
    {
        return;
    }
    for(int i = 0; i < (int)m_setInfo.size(); i++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        char szIndex[64];
        for(int i = 0; i < (int)strlen(pInfo->szIndex) + 1; i ++)
        {
            szIndex[i] = pInfo->szIndex[i];
            if (szIndex[i] == '+')
            {
                szIndex[i] = ' ';
            }
        }
        char szStr[64];
        char szVMesh[64];
        if (2 != sscanf(szIndex, "%s %s", szStr, szVMesh))
        {
            if (pInfo)
            {
                this->Show(pInfo, posWorld);
            }
        }
        else
        {
            if (pInfo)
            {
                C3Motion* pMotion = p3DRole->m_pArmor->GetVirtualMotion(szVMesh);
                if (pMotion)
                {
                    this->Show(pInfo, p3DRole->m_pArmor, szVMesh);
                }
            }
        }
    }
    this->Process();
}
//-------------------------------------------------------------------
void CGame3DEffectEx::Show(C3DRolePart* pPart)
{
    if (!pPart)
    {
        return;
    }
    for(int i = 0; i < (int)m_setInfo.size(); i++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo)
        {
            this->Show(pInfo, pPart);
        }
    }
    this->Process();
}
//-------------------------------------------------------------------
void CGame3DEffectEx::SetPos ( int nX, int nY, int nRotate, float fScale )
{
    /*effect->Move ( nX, 0, nY );
    effect->Rotate ( 0, 0, D3DXToRadian ( nRotate ) );
    effect->Scale ( fScale, fScale, fScale );*/
}

//-------------------------------------------------------------------
void CGame3DEffectEx::Rotate(float x, float y, float z)
{
    m_fX = x;
    m_fY = y;
    m_fZ = z;
    /*
    	int nAmount = m_setInfo.size();
    	for(int i = 0; i < nAmount; i ++)
    	{
    		Game3DEffectExInfo* pInfo = m_setInfo[i];
    		if (pInfo)
    		{
    			int nInfoAmount = pInfo->nInfoAmount;
    			for(int i = 0; i < nInfoAmount; i++)
    			{
    				Game3DEffectIdInfo* pIDInfo = pInfo->pIDInfo+i;

    				// get effect
    				C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIDInfo->idEffect);
    				if (pEffect)
    					pEffect->Rotate(x, y, z);
    			}
    		}
    	}*/
}

//-------------------------------------------------------------------
void CGame3DEffectEx::Scale(float x, float y, float z)
{
    int nAmount = m_setInfo.size();
    for(int i = 0; i < nAmount; i ++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo)
        {
            int nInfoAmount = pInfo->nInfoAmount;
            for(int i = 0; i < nInfoAmount; i++)
            {
                Game3DEffectIdInfo* pIDInfo = pInfo->pIDInfo + i;
                // get effect
                C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIDInfo->idEffect);
                if (pEffect)
                {
                    pEffect->Scale(x, y, z);
                }
            }
        }
    }
}
//-------------------------------------------------------------------

void CGame3DEffectEx::SetHeight ( float fHeight )
{
    m_Height = fHeight;
}
//-------------------------------------------------------------------
void CGame3DEffectEx::Show(Game3DEffectExInfo* pInfo, C3DRolePart* pPart)
{
    if (!pInfo)
    {
        return;
    }
    if (!pInfo->bShow)
    {
        return;
    }
    if (!pPart)
    {
        return;
    }
    Game3DEffectExInfo* pMyInfo = pInfo;
    int nAmount = pInfo->nInfoAmount;
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectIdInfo* pIDInfo = pInfo->pIDInfo + i;
        // get effect
        C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIDInfo->idEffect);
        if (!pEffect)
        {
            return;
        }
        // get txeture of effect
        C3DTexture* pTexture = g_objGameDataSet.Get3DTexture(pIDInfo->idTexture);
        if (!pTexture)
        {
            return;
        }
        // set the texture
        pEffect->SetTexture(pTexture);
        pEffect->SetFrames(pMyInfo->dwCurrentFrameIndex);
        // restore shape info
        pEffect->RestoreShapeInfo(pIDInfo->pShapeInfoBuf);
        pEffect->m_nShapeAir = pInfo->nShapeAir;
        // setpos
        C3DObjPartInfo* pInfo = pPart->GetInfo(0);
        if (!pInfo)
        {
            return;
        }
        C3DObj* pObj = g_objGameDataSet.Get3DObj(pInfo->idMesh);
        if (!pObj)
        {
            return;
        }
        pEffect->Bind(pObj);
        pEffect->Draw(false, pIDInfo->nASB, pIDInfo->nADB);
        pEffect->BackupShapeInfo(pIDInfo->pShapeInfoBuf);
    }
}
//-------------------------------------------------------------------

void CGame3DEffectEx::Show(Game3DEffectExInfo* pInfo, CMyPos posWorld)
{
    if (!pInfo)
    {
        return;
    }
    if (!pInfo->bShow)
    {
        return;
    }
    int nAmount = pInfo->nInfoAmount;
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectIdInfo* pIDInfo = pInfo->pIDInfo + i;
        // get effect
        C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIDInfo->idEffect);
        if (!pEffect)
        {
            return;
        }
        // get txeture of effect
        C3DTexture* pTexture = g_objGameDataSet.Get3DTexture(pIDInfo->idTexture);
        if (!pTexture)
        {
            return;
        }
        // set the texture
        pEffect->SetTexture(pTexture);
        pEffect->SetFrames(pInfo->dwCurrentFrameIndex);
        // restore shape info
        pEffect->RestoreShapeInfo(pIDInfo->pShapeInfoBuf);
        pEffect->m_nShapeAir = pInfo->nShapeAir;
        // setpos
        if (pInfo->bSelfRotate)
        {
            pEffect->Rotate(0, 0, pInfo->fDir);
        }
        else
        {
            pEffect->Rotate(m_fX, m_fY, m_fZ);
        }
        pEffect->Move(0, (float)pInfo->nOffsetZ, (float)(m_Height * 1.4));
        if (m_bOblique)
        {
            pEffect->Rotate ( D3DXToRadian ( -30.0f ), 0, 0 );
        }
        pEffect->Move((float)(posWorld.x + pInfo->nOffsetX), 0,(float)( posWorld.y + pInfo->nOffsetY));
        pEffect->Draw(false, pIDInfo->nASB, pIDInfo->nADB);
        pEffect->BackupShapeInfo(pIDInfo->pShapeInfoBuf);
    }
}

//-------------------------------------------------------------------
BOOL CGame3DEffectEx::Process(Game3DEffectExInfo* pInfo)
{
    int nAmount = pInfo->nInfoAmount;
    if (nAmount <= 0)
    {
        return true;
    }
    if (!pInfo)
    {
        return true;
    }
    int nMaxFrameAmount = 0;
    {
        Game3DEffectIdInfo* pIdInfo = pInfo->pIDInfo;
        if (pIdInfo)
        {
            C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIdInfo->idEffect);
            if (pEffect)
            {
                nMaxFrameAmount = pEffect->GetFrames();
            }
        }
    }
    /*
    for(int i = 0; i < pInfo->nInfoAmount; i++)
    {
    	Game3DEffectIdInfo* pIdInfo = pInfo->pIDInfo + i;
    	if (pIdInfo)
    	{
    		C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIdInfo->idEffect);
    		if (pEffect && pEffect->GetFrames() > nMaxFrameAmount)
    			nMaxFrameAmount = pEffect->GetFrames();
    	}
    }
    */
    // 看看死了没
    DWORD dwTimePass = ::TimeGet() - pInfo->dwTimeBegin;
    if (dwTimePass < pInfo->dwTimeDelay)
    {
        pInfo->bShow = false;
        return false;
    }
    dwTimePass -= pInfo->dwTimeDelay;
    DWORD dwAllTime = pInfo->dwLoopTime * (pInfo->dwFrameInterval * nMaxFrameAmount + pInfo->dwLoopInterval) - pInfo->dwLoopInterval;
    if (dwAllTime <= dwTimePass)
    {
        return true;
    }
    // 取祯数
    int nData = (pInfo->dwFrameInterval * nMaxFrameAmount + pInfo->dwLoopInterval);
    if (nData == 0)
    {
        nData = 1;
    }
    int nTime = (int)(dwTimePass / nData);
    DWORD dwTimePassThisLoop = dwTimePass - (pInfo->dwFrameInterval * nMaxFrameAmount + pInfo->dwLoopInterval) * nTime;
    int nFrameIndex = dwTimePassThisLoop / pInfo->dwFrameInterval;
    if (nFrameIndex >= nMaxFrameAmount)
    {
        pInfo->bShow = false;
    }
    else
    {
        pInfo->bShow = true;
        pInfo->dwCurrentFrameIndex = nFrameIndex;
    }
    return false;
}

//-------------------------------------------------------------------
BOOL CGame3DEffectEx::Process()
{
    int nAmount = m_setInfo.size();
    if (nAmount <= 0)
    {
        return true;
    }
    for(int i = nAmount - 1; i >= 0; i --)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo)
        {
            if (this->Process(pInfo))
            {
                // 结束
                this->Delete(pInfo->szIndex);
            }
        }
    }
    return false;
}

//-------------------------------------------------------------------
void CGame3DEffectEx::Clear()
{
    int nAmount = m_setInfo.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo->pIDInfo)
        {
            int nMyAmount = pInfo->nInfoAmount;
            for(int i = 0; i < nMyAmount; i ++)
            {
                Game3DEffectIdInfo* pMyInfo = pInfo->pIDInfo + i;
                if (pMyInfo)
                {
                    C3DEffect::DestroyShapeInfo(pMyInfo->pShapeInfoBuf, pMyInfo->nShapes);
                }
            }
            delete[] pInfo->pIDInfo;
            pInfo->pIDInfo = NULL;
        }
        SAFE_DELETE(pInfo);
    }
    m_setInfo.clear();
}

//-------------------------------------------------------------------
void CGame3DEffectEx::DeleteByVMesh(char* pszVMesh)
{
    if (!pszVMesh)
    {
        return;
    }
    int nAmount = m_setInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo && NULL != strstr(pInfo->szIndex, pszVMesh))
        {
            // delete
            m_setInfo.erase(m_setInfo.begin() + i);
            if (pInfo->pIDInfo)
            {
                int nMyAmount = pInfo->nInfoAmount;
                for(int i = 0; i < nMyAmount; i ++)
                {
                    Game3DEffectIdInfo* pMyInfo = pInfo->pIDInfo + i;
                    if (pMyInfo)
                    {
                        C3DEffect::DestroyShapeInfo(pMyInfo->pShapeInfoBuf, pMyInfo->nShapes);
                    }
                }
                delete[] pInfo->pIDInfo;
                pInfo->pIDInfo = NULL;
            }
            SAFE_DELETE(pInfo);
            return;
        }
    }
}
//-------------------------------------------------------------------
void CGame3DEffectEx::Delete(char* pszIndex)
{
    if (!pszIndex)
    {
        return;
    }
    int nAmount = m_setInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo && 0 == strcmp(pszIndex, pInfo->szIndex))
        {
            // delete
            m_setInfo.erase(m_setInfo.begin() + i);
            if (pInfo->pIDInfo)
            {
                int nMyAmount = pInfo->nInfoAmount;
                for(int i = 0; i < nMyAmount; i ++)
                {
                    Game3DEffectIdInfo* pMyInfo = pInfo->pIDInfo + i;
                    if (pMyInfo)
                    {
                        C3DEffect::DestroyShapeInfo(pMyInfo->pShapeInfoBuf, pMyInfo->nShapes);
                    }
                }
                delete[] pInfo->pIDInfo;
                pInfo->pIDInfo = NULL;
            }
            SAFE_DELETE(pInfo);
            return;
        }
    }
}

//-------------------------------------------------------------------
void CGame3DEffectEx::Add(Game3DEffectExInfo* pInfo)
{
    if (!pInfo)
    {
        return;
    }
    m_setInfo.push_back(pInfo);
}

//-------------------------------------------------------------------
void CGame3DEffectEx::Add(char* pszIndex, BOOL bSelfRotate, int nDir , DWORD dwLook)
{
    if (!pszIndex)
    {
        return;
    }
    if (strcmp(pszIndex, "none") == 0)
    {
        return;
    }
    if (strcmp(pszIndex, "null") == 0)
    {
        return;
    }
    const CMy3DEffectInfo* pEffectInfo = g_obj3DRoleData.GetMy3DEffectInfo(pszIndex);
    if (!pEffectInfo)
    {
        return;
    }
    Game3DEffectExInfo* pInfo = new Game3DEffectExInfo;
    MYASSERT (pInfo);
    //memset(pInfo, 0L, sizeof(Game3DEffectExInfo));
    if (!bSelfRotate)
    {
        pInfo->bSelfRotate = false;
    }
    else
    {
        pInfo->bSelfRotate = true;
        pInfo->fDir = (float)(3.14159 * 9 / 4 - 3.14159 * nDir / 4);
    }
    pInfo->nInfoAmount = pEffectInfo->nAmount;
    pInfo->pIDInfo = new Game3DEffectIdInfo[pInfo->nInfoAmount];
    MYASSERT(pInfo->pIDInfo);
    for(int i = 0; i < pInfo->nInfoAmount; i++)
    {
        Game3DEffectIdInfo* pIDInfo = pInfo->pIDInfo + i;
        //memset(pIDInfo, 0L, sizeof(Game3DEffectIdInfo));
        pIDInfo->idEffect		= pEffectInfo->idEffect[i];
        pIDInfo->idTexture		= pEffectInfo->idTexture[i];
        pIDInfo->nADB			= pEffectInfo->nADB[i];
        pIDInfo->nASB			= pEffectInfo->nASB[i];
        C3DEffect* pEffect = g_objGameDataSet.Get3DEffect(pIDInfo->idEffect);
        MYASSERT (pEffect);
        pEffect->ResetShape();
        pIDInfo->pShapeInfoBuf = pEffect->CreateShapeInfo();
        pIDInfo->nShapes = pEffect->GetShapeAmount();
        pEffect->BackupShapeInfo(pIDInfo->pShapeInfoBuf);
    }
    pInfo->dwTimeDelay	= pEffectInfo->nDelay;
    pInfo->dwLoopTime	= pEffectInfo->nLoopTime;
    pInfo->dwFrameInterval	= pEffectInfo->nFrameInterval;
    pInfo->dwLoopInterval	= pEffectInfo->nLoopInterval;
    pInfo->nOffsetX		= pEffectInfo->nOffsetX;
    pInfo->nOffsetY		= pEffectInfo->nOffsetY;
    pInfo->nOffsetZ		= pEffectInfo->nOffsetZ;
    pInfo->nShapeAir    = pEffectInfo->nShapeAir;
    pInfo->bShow		= true;
    strcpy(pInfo->szIndex, pszIndex);
    pInfo->dwTimeBegin = ::TimeGet();
    if (dwLook)
    {
        int x, y, z;
        if (g_obj3DRoleData.GetEffectLookSetInfo(dwLook, pInfo->szIndex, x, y, z))
        {
            pInfo->nOffsetX += x;
            pInfo->nOffsetY += y;
            pInfo->nOffsetZ += z;
        }
    }

    this->Add(pInfo);
}
//-----------------------------------------------------------------
void CGame3DEffectEx::EverPlay()
{
    int nAmount = m_setInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo)
        {
            pInfo->dwLoopTime = 99999999;
        }
    }
}
//-----------------------------------------------------------------
BOOL CGame3DEffectEx::TestEffect(char* pszIndex)
{
    if (!pszIndex)
    {
        return false;
    }
    int nAmount = m_setInfo.size();
    for(int i = 0; i < nAmount; i++)
    {
        Game3DEffectExInfo* pInfo = m_setInfo[i];
        if (pInfo)
        {
            if (strcmp(pInfo->szIndex, pszIndex) == 0)
            {
                return true;
            }
        }
    }
    return false;
}
//-----------------------------------------------------------------
