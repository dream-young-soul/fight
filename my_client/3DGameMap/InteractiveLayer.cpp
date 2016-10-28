
//**********************************************************
// 代码编辑器
//**********************************************************

//-------------------------------------------------------------
// InteractiveLayer.cpp
//-------------------------------------------------------------

#pragma warning(disable:4786)
#include "InteractiveLayer.h"
#include "2DMapCoverObj.h"
#include "2DMapTerrainObj.h"
#include "Hero.h"
#include "3DMapEffect.h"
#include "3DMapEffectNew.h"
#include "3DGameMap.h"
#include "3DMapSimpleObj.h"
#include "MapSound.h"
#include "3DMapItem.h"
//-------------------------------------------------------------
CInteractiveLayer::CInteractiveLayer()
{
    m_setMapObj.clear();
}
//-------------------------------------------------------------
CInteractiveLayer::~CInteractiveLayer()
{
    this->ClearMapObj();
}
//-------------------------------------------------------------
void CInteractiveLayer::Show(CMyPos posShow)
{
    BOOL bShowHero = false;
    int nAmount = m_setMapObjShow.size();
    for(int i = 0; i < nAmount; i++)
    {
        try
        {
            CMapObj* pObj = m_setMapObjShow[i];
            if (pObj)
            {
                /*			if (pObj->GetObjType() == MAP_HERO)
                				bShowHero = true;
                			if (pObj->GetObjType() == MAP_3DEFFECT)
                			{
                				if (bShowHero)
                				{
                //					CMyBitmap::ShowString(100, 100,  0x0000ff00, "人物先显示");
                				}
                				else
                				{
                //					CMyBitmap::ShowString(100, 100,  0x0000ff00, "特效先显示");
                				}
                			}
                */
                if (pObj->GetObjType() != MAP_ROLE &&
                        pObj->GetObjType() != MAP_HERO &&
                        pObj->GetObjType() != MAP_PLAYER &&
                        pObj->GetObjType() != MAP_PUZZLE &&
                        pObj->GetObjType() != MAP_3DEFFECT &&
                        pObj->GetObjType() != MAP_3DEFFECTNEW)
                {
                    pObj->SetARGB(g_objGameMap.GetARGB());
                }
                else
                {
                    pObj->SetARGB(0xffffffff);
                }
                pObj->Show(NULL);
            }
        }
        catch(...)
        {
            return;
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::Process(void* pInfo)
{
    m_setMapObjShow.clear();
    int nAmount = m_setMapObj.size();
    int i;
    for(i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj->GetObjType() == MAP_TERRAIN)
        {
            C2DMapTerrainObj* pTerrainObj =  (C2DMapTerrainObj*)pObj;
            if (pTerrainObj)
            {
                pTerrainObj->Process(pInfo);
            }
        }
    }
    for(i = nAmount - 1; i >= 0; i--)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj->GetObjType() == MAP_TERRAIN)
        {
            continue;
        }
        switch(pObj->GetObjType())
        {
        case MAP_COVER:
            {
                CMyPos pos;
                g_objHero.GetWorldPos(pos);
                C2DMapCoverObj* pCoverObj =  (C2DMapCoverObj*)pObj;
                if (pCoverObj)
                {
                    pCoverObj->Process(&pos);
                }
            }
            break;
        case MAP_TERRAIN:
            break;
        case MAP_HERO:
            {
                CHero* pHero =  (CHero*)pObj;
                if (pHero)
                {
                    pHero->Process(pInfo);
                }
            }
            break;
        case MAP_PLAYER:
            {
                CPlayer* pPlayer =  (CPlayer*)pObj;
                if (pPlayer)
                {
                    pPlayer->Process(pInfo);
                }
            }
            break;
        case MAP_3DSIMPLE:
            {
                C3DMapSimpleObj* p3DMapSimpleObj = (C3DMapSimpleObj*) pObj;
                pObj->Process(NULL);
                if (p3DMapSimpleObj->IsDie())
                {
                    this->DelObj(p3DMapSimpleObj);
                }
            }
            break;
        case MAP_3DEFFECT:
        case MAP_3DEFFECTNEW:
            {
                BOOL bOver;
                pObj->Process(&bOver);
                if (bOver)
                {
                    this->DelObj(pObj);
                }
            }
            break;
        case MAP_3DMAGICMAPITEM:
            {
                BOOL bOver;
                pObj->Process(&bOver);
                if (bOver)
                {
                    this->DelObj(pObj);
                }
            }
            break;
        case MAP_3DNPC:
            {
                pObj->Process(NULL);
            }
            break;
        case MAP_SOUND:
            {
                pObj->Process(NULL);
            }
            break;
        default:
            pObj->Process(NULL);
            break;
        }
    }
}
//-------------------------------------------------------------
int  CInteractiveLayer::GetType()
{
    return LAYER_INTERACTIVE;
}
//-------------------------------------------------------------
CInteractiveLayer* CInteractiveLayer::CreateNew()
{
    CInteractiveLayer* pLayer = new CInteractiveLayer;
    return pLayer;
}
//-------------------------------------------------------------
void CInteractiveLayer::ClearMapObj()
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (!pObj)
        {
            continue;
        }
        BOOL bDel = true;
        if ((pObj->GetObjType() == MAP_HERO) ||
                (pObj->GetObjType() == MAP_PLAYER))
        {
            bDel = false;
        }
        if (bDel)
        {
            SAFE_DELETE(pObj);
        }
    }
    m_setMapObj.clear();
}
//-------------------------------------------------------------
void CInteractiveLayer::AddMapObj(CMapObj* pObj)
{
    if (!pObj)
    {
        return;
    }
    m_setMapObj.push_back(pObj);
    if (pObj->GetObjType() == MAP_TERRAIN)
    {
        C2DMapTerrainObj* pTerrainObj = (C2DMapTerrainObj*)pObj;
        this->PlaceTerrainObj(pTerrainObj);
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::DelObj(CMapObj* pObj)
{
    if (!pObj)
    {
        return;
    }
    int nAmount = m_setMapObj.size();
    for(int i = nAmount - 1; i >= 0; i--)
    {
        CMapObj* pMyObj = m_setMapObj[i];
        if (pMyObj == pObj)
        {
            m_setMapObj.erase(m_setMapObj.begin() + i);
            if (pObj->GetObjType() == MAP_TERRAIN)
            {
                // todo ...
                C2DMapTerrainObj* pTerrainObj = (C2DMapTerrainObj*)pObj;
                this->DisplaceTerrainObj(pTerrainObj);
            }
            BOOL bDel = true;
            if ((pObj->GetObjType() == MAP_HERO) ||
                    (pObj->GetObjType() == MAP_PLAYER))
            {
                bDel = false;
            }
            if (bDel)
            {
                SAFE_DELETE(pObj);
            }
            return;
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::SaveDataLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount = m_setMapObj.size();
    int i;
    int nSaveAmount = 0;
    for(i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            switch(pObj->GetObjType())
            {
            case MAP_COVER:
            case MAP_TERRAIN:
            case MAP_SOUND:
                nSaveAmount ++;
                break;
            case MAP_3DEFFECT:
            case MAP_3DEFFECTNEW:
                {
                    C3DMapEffect* pEffect = (C3DMapEffect*)pObj;
                    if (pEffect->NeedSave())
                    {
                        nSaveAmount ++;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    fwrite(&nSaveAmount, sizeof(int), 1, fp);
    for(i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            switch(pObj->GetObjType())
            {
            case MAP_COVER:
                {
                    int nObjType = MAP_COVER;
                    fwrite(&nObjType, sizeof(int), 1, fp);
                    C2DMapCoverObj* pCoverObj = (C2DMapCoverObj*)pObj;
                    pCoverObj->SaveDataObj(fp);
                }
                break;
            case MAP_TERRAIN:
                {
                    int nObjType = MAP_TERRAIN;
                    fwrite(&nObjType, sizeof(int), 1, fp);
                    C2DMapTerrainObj* pTerrainObj = (C2DMapTerrainObj*)pObj;
                    pTerrainObj->SaveData(fp);
                }
                break;
            case MAP_SOUND:
                {
                    int nObjType = MAP_SOUND;
                    fwrite(&nObjType, sizeof(int), 1, fp);
                    CMapSound* pSoundObj = (CMapSound*)pObj;
                    pSoundObj->SaveDataObj(fp);
                }
                break;
            case MAP_3DEFFECT:
                {
                    C3DMapEffect* pEffect = (C3DMapEffect*)pObj;
                    if (pEffect->NeedSave())
                    {
                        int nObjType = MAP_3DEFFECT;
                        fwrite(&nObjType, sizeof(int), 1, fp);
                        pEffect->SaveDataObj(fp);
                    }
                }
                break;
            case MAP_3DEFFECTNEW:
                {
                    C3DMapEffectNew* pEffect = (C3DMapEffectNew*)pObj;
                    if (pEffect->NeedSave())
                    {
                        int nObjType = MAP_3DEFFECTNEW;
                        fwrite(&nObjType, sizeof(int), 1, fp);
                        pEffect->SaveDataObj(fp);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::SaveTextLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount = m_setMapObj.size();
    int i;
    int nSaveAmount = 0;
    for(i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            switch(pObj->GetObjType())
            {
            case MAP_COVER:
            case MAP_TERRAIN:
            case MAP_SOUND:
                nSaveAmount ++;
                break;
            case MAP_3DEFFECT:
            case MAP_3DEFFECTNEW:
                {
                    C3DMapEffect* pEffect = (C3DMapEffect*)pObj;
                    if (pEffect->NeedSave())
                    {
                        nSaveAmount ++;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    fprintf(fp, "========InteractiveLayer ObjAmount=%d========\n", nSaveAmount);
    for(i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj)
        {
            switch(pObj->GetObjType())
            {
            case MAP_COVER:
                {
                    fprintf(fp, "========ObjType=%d========\n", MAP_COVER);
                    C2DMapCoverObj* pCoverObj = (C2DMapCoverObj*)pObj;
                    pCoverObj->SaveTextObj(fp);
                }
                break;
            case MAP_TERRAIN:
                {
                    fprintf(fp, "========ObjType=%d========\n", MAP_TERRAIN);
                    C2DMapTerrainObj* pTerrainObj = (C2DMapTerrainObj*)pObj;
                    pTerrainObj->SaveText(fp);
                }
                break;
            case MAP_SOUND:
                {
                    fprintf(fp, "========ObjType=%d========\n", MAP_SOUND);
                    CMapSound* pSound = (CMapSound*)pObj;
                    pSound->SaveTextObj(fp);
                }
                break;
            case MAP_3DEFFECT:
                {
                    C3DMapEffect* pEffect = (C3DMapEffect*)pObj;
                    if (pEffect->NeedSave())
                    {
                        fprintf(fp, "========ObjType=%d========\n", MAP_3DEFFECT);
                        pEffect->SaveTextObj(fp);
                    }
                }
                break;
            case MAP_3DEFFECTNEW:
                {
                    C3DMapEffectNew* pEffect = (C3DMapEffectNew*)pObj;
                    if (pEffect->NeedSave())
                    {
                        fprintf(fp, "========ObjType=%d========\n", MAP_3DEFFECTNEW);
                        pEffect->SaveTextObj(fp);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::LoadDataLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount = 0;
    fread(&nAmount, sizeof(int), 1, fp);
    for(int i = 0; i < nAmount; i++)
    {
        // get obj type
        int nMapObjType;
        fread(&nMapObjType, sizeof(int), 1, fp);
		//兼容魔域1.0新地图
		if(nMapObjType == 0)
		{
			fread(&nMapObjType, sizeof(int), 1, fp);
		}
        switch(nMapObjType)
        {
		case MAP_NONE:
			break;
        case MAP_COVER:
            {
                C2DMapCoverObj* pCoverObj = new C2DMapCoverObj;
                if (pCoverObj)
                {
                    pCoverObj->LoadDataObj(fp);
                    this->AddMapObj(pCoverObj);
                }
            }
            break;
        case MAP_TERRAIN:
            {
                C2DMapTerrainObj* pTerrainObj = new C2DMapTerrainObj;
                if (pTerrainObj)
                {
                    pTerrainObj->LoadData(fp);
                    this->AddMapObj(pTerrainObj);
                }
            }
            break;
        case MAP_SOUND:
            {
                CMapSound* pObj = new CMapSound;
                if (pObj)
                {
                    pObj->LoadDataObj(fp);
                    this->AddMapObj(pObj);
                    //pObj->Play();
                }
            }
            break;
        case MAP_3DEFFECT:
            {
                C3DMapEffect* pEffect = new C3DMapEffect;
                if (pEffect)
                {
                    pEffect->LoadDataObj(fp);
                    this->AddMapObj(pEffect);
                }
            }
            break;
        case MAP_3DEFFECTNEW:
            {
                C3DMapEffectNew* pEffect = new C3DMapEffectNew;
                if (pEffect)
                {
                    pEffect->LoadDataObj(fp);
                    this->AddMapObj(pEffect);
                }
            }
            break;
        default:
            break;
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::LoadTextLayer(FILE* fp)
{
    if (!fp)
    {
        return;
    }
    int nAmount;
    fscanf(fp, "========InteractiveLayer ObjAmount=%d========\n", &nAmount);
    for(int i = 0; i < nAmount; i++)
    {
        int nObjType;
        fscanf(fp, "========ObjType=%d========\n", &nObjType);
        switch(nObjType)
        {
        case MAP_COVER:
            {
                C2DMapCoverObj* pCoverObj = new C2DMapCoverObj;
                if (pCoverObj)
                {
                    pCoverObj->LoadTextObj(fp);
                    this->AddMapObj(pCoverObj);
                }
            }
            break;
        case MAP_TERRAIN:
            {
                C2DMapTerrainObj* pTerrainObj = new C2DMapTerrainObj;
                if (pTerrainObj)
                {
                    pTerrainObj->LoadText(fp);
                    this->AddMapObj(pTerrainObj);
                    this->PlaceTerrainObj(pTerrainObj);
                }
            }
            break;
        case MAP_SOUND:
            {
                CMapSound* pSound = new CMapSound;
                if (pSound)
                {
                    pSound->LoadTextObj(fp);
                    this->AddMapObj(pSound);
                    pSound->Play();
                }
            }
            break;
        case MAP_3DEFFECT:
            {
                C3DMapEffect* pEffect = new C3DMapEffect;
                if (pEffect)
                {
                    pEffect->LoadTextObj(fp);
                    this->AddMapObj(pEffect);
                }
            }
            break;
        case MAP_3DEFFECTNEW:
            {
                C3DMapEffectNew* pEffect = new C3DMapEffectNew;
                if (pEffect)
                {
                    pEffect->LoadTextObj(fp);
                    this->AddMapObj(pEffect);
                }
            }
            break;
        default:
            break;
        }
    }
}
//-------------------------------------------------------------
CMapSound* CInteractiveLayer::GetFocusSound()
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (!pObj)
        {
            continue;
        }
        if (pObj->GetObjType() == MAP_SOUND)
        {
            CMapSound* pMyObj = (CMapSound*)pObj;
            if (pMyObj->IsFocus())
            {
                return pMyObj;
            }
        }
    }
    return NULL;
}
//-------------------------------------------------------------
C2DMapCoverObj* CInteractiveLayer::Get2DFocusMapCoverObj()
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (!pObj)
        {
            continue;
        }
        if (pObj->GetObjType() == MAP_COVER)
        {
            C2DMapCoverObj* p2DMapCoverObj = (C2DMapCoverObj*)pObj;
            if (p2DMapCoverObj->IsFocus())
            {
                return p2DMapCoverObj;
            }
        }
    }
    return NULL;
}
//-------------------------------------------------------------
C2DMapTerrainObj* CInteractiveLayer::Get2DFocusMapTerrainObj()
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (!pObj)
        {
            continue;
        }
        if (pObj->GetObjType() == MAP_TERRAIN)
        {
            C2DMapTerrainObj* p2DMapTerrainObj = (C2DMapTerrainObj*)pObj;
            if (p2DMapTerrainObj->IsFocus())
            {
                return p2DMapTerrainObj;
            }
        }
    }
    return NULL;
}
//-------------------------------------------------------------
C2DMapTerrainObjPart*  CInteractiveLayer::Get2DFocusMapTerrainObjPart()
{
    DEQUE_MAPOBJ setPartObj;
    setPartObj.clear();
    int nAmount = m_setMapObj.size();
    // 建立要测试队列
    for(int k = 0; k < nAmount; k++)
    {
        CMapObj* pObj = m_setMapObj[k];
        if (pObj && pObj->GetObjType() == MAP_TERRAIN)
        {
            C2DMapTerrainObj* pTerrainObj = (C2DMapTerrainObj*)pObj;
            int nPartAmount = pTerrainObj->GetPartAmount();
            for(int l = 0; l < nPartAmount; l++)
            {
                C2DMapTerrainObjPart* pPart = pTerrainObj->GetPartByIndex(l);
                if (pPart)
                {
                    CMyPos posPart;
                    pPart->GetPos(posPart);
                    if (g_objGameMap.IsPosVisible(posPart))
                    {
                        setPartObj.push_back(pPart);
                    }
                }
            }
        }
    }
    // 开始排序，放入setparttest
    DEQUE_MAPOBJ setPartTest;
    setPartTest.clear();
    int nPartAmount = setPartObj.size();
    for(int j = 0; j < nPartAmount; j++)
    {
        C2DMapTerrainObjPart* pPart = (C2DMapTerrainObjPart*)setPartObj[j];
        this->ApplyShow(pPart, setPartTest);
    }
    setPartObj.clear();
    C2DMapTerrainObjPart* pReturn = NULL;
    int nTestPartAmount = setPartTest.size();
    int nFocusAmount = 0;
    for(int n = 0; n < nTestPartAmount; n++)
    {
        C2DMapTerrainObjPart* pPart = (C2DMapTerrainObjPart*)setPartTest[n];
        if (pPart && pPart->IsFocus())
        {
            pReturn = pPart;
            nFocusAmount ++;
        }
    }
    setPartTest.clear();
    return pReturn;
}
//-------------------------------------------------------------

void CInteractiveLayer::ApplyShow(CMapObj* pNewObj)
{
    if (!pNewObj)
    {
        return;
    }
    UINT i;
    DWORD dwSize	= m_setMapObjShow.size();
    if (pNewObj->GetObjType() != MAP_TERRAIN_PART)
    {
        try
        {
            for(i = 0; i < dwSize; i++)
            {
                CMapObj* pObj =  m_setMapObjShow[i];
                if (pObj && pObj->GetObjType() == MAP_TERRAIN_PART)
                {
                    C2DMapTerrainObjPart* pPart = (C2DMapTerrainObjPart*)pObj;
                    CMyPos posCell;
                    pNewObj->GetPos(posCell);
                    if (pPart)
                    {
                        if (pPart->InMyBase(posCell))
                        {
                            pPart->AddObj(pNewObj);
                            return;
                        }
                    }
                }
            }
        }
        catch(...)
        {
            return;
        }
    }
    CMyPos posNewObj;
    pNewObj->GetPos(posNewObj);
    int nNewObjX = posNewObj.x;
    int nNewObjY = posNewObj.y;
    int nOverLayAfter = -1;
    for(i = 0; i < dwSize; i++)
    {
        CMapObj* pOldObj	= m_setMapObjShow[i];
        CMyPos posOldObj;
        pOldObj->GetPos(posOldObj);
        int nOldObjX = posOldObj.x;
        int nOldObjY = posOldObj.y;
        CMySize sizeOldObj, sizeNewObj;
        g_objGameMap.GetMapObjSize(sizeOldObj, pOldObj);
        g_objGameMap.GetMapObjSize(sizeNewObj, pNewObj);
        // sort now!
        if ((nNewObjX - sizeNewObj.iWidth + 1) - nOldObjX > 0 ||
                (nOldObjX - sizeOldObj.iWidth + 1) - nNewObjX > 0)
        {
            // x no overlay
            if ((nNewObjY - sizeNewObj.iHeight + 1) - nOldObjY > 0 ||
                    (nOldObjY - sizeOldObj.iHeight + 1) - nNewObjY > 0)
            {
                // y no overlay
            }
            else // got y overlay
            {
                if (nNewObjX < nOldObjX)
                {
                    int bInsert = i;
                    for(int n = i - 1; n >= 0; n--) ///插在与J同行的物件或比当前排序物更近的物件之前
                    {
                        CMapObj* pObj	= m_setMapObjShow[n];
                        if (!pObj)
                        {
                            continue;
                        }
                        CMyPos posOldObj;
                        pObj->GetPos(posOldObj);
                        int nObjX = posOldObj.x;
                        int nObjY = posOldObj.y;
                        if ((nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY < nObjX + nObjY) && n > nOverLayAfter)
                        {
                            bInsert = n;
                        }
                        else
                        {
                            break;
                        }
                    }
                    nOverLayAfter = -1;
                    m_setMapObjShow.insert(m_setMapObjShow.begin() + bInsert, pNewObj);
                    break;
                }
                else
                {
                    nOverLayAfter = i;
                    for(DWORD n = i + 1; n < dwSize; n++) ///插在与J同行的物件或比当前排序物更近的物件之后
                    {
                        CMapObj* pObj	= m_setMapObjShow[n];
                        if (!pObj)
                        {
                            continue;
                        }
                        CMyPos posOldObj;
                        pObj->GetPos(posOldObj);
                        int nObjX = posOldObj.x;
                        int nObjY = posOldObj.y;
                        if (nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY > nObjX + nObjY)
                        {
                            nOverLayAfter = n;
                        }
                        else
                        {
                            break;;
                        }
                    }
                }
            }// end of y overlay
        }
        else // got x overlay
        {
            if (nNewObjY < nOldObjY)
            {
                int bInsert = i;
                for(int n = i - 1; n >= 0; n--) ///插在与J同行的物件或比当前排序物更近的物件之前
                {
                    CMapObj* pObj	= m_setMapObjShow[n];
                    if (!pObj)
                    {
                        continue;
                    }
                    CMyPos posOldObj;
                    pObj->GetPos(posOldObj);
                    int nObjX = posOldObj.x;
                    int nObjY = posOldObj.y;
                    if ((nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY < nObjX + nObjY) && n > nOverLayAfter)
                    {
                        bInsert = n;
                    }
                    else
                    {
                        break;
                    }
                }
                nOverLayAfter = -1;
                m_setMapObjShow.insert(m_setMapObjShow.begin() + bInsert, pNewObj);
                break;
            }
            else
            {
                nOverLayAfter = i;
                for(DWORD n = i + 1; n < dwSize; n++) ///插在与J同行的物件或比当前排序物更近的物件之后
                {
                    CMapObj* pObj	= m_setMapObjShow[n];
                    if (!pObj)
                    {
                        continue;
                    }
                    CMyPos posOldObj;
                    pObj->GetPos(posOldObj);
                    int nObjX = posOldObj.x;
                    int nObjY = posOldObj.y;
                    if (nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY > nObjX + nObjY)
                    {
                        nOverLayAfter = n;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    if (nOverLayAfter != -1) //insert after nOverLayAfter
    {
        if (nOverLayAfter >= (int)m_setMapObjShow.size() - 1)
        {
            m_setMapObjShow.push_back(pNewObj);
        }
        else
        {
            m_setMapObjShow.insert(m_setMapObjShow.begin() + nOverLayAfter + 1, pNewObj);
        }
    }
    else
    {
        if (i >= dwSize)	// the last one...
        {
           DWORD n = 0;
            for (; n < dwSize; n++)
            {
                CMapObj* pOldObj	= m_setMapObjShow[n];
                if (!pOldObj)
                {
                    continue;
                }
                CMyPos posOldObj;
                pOldObj->GetPos(posOldObj);
                int nOldObjX = posOldObj.x;
                int nOldObjY = posOldObj.y;
                if (nNewObjX + nNewObjY < nOldObjX + nOldObjY)
                {
                    m_setMapObjShow.insert(m_setMapObjShow.begin() + n, pNewObj);
                    break;
                }
            }
            if (n >= dwSize)
            {
                m_setMapObjShow.push_back(pNewObj);
            }
        }
    }
}

//-------------------------------------------------------------
void CInteractiveLayer::ApplyShow(CMapObj* pNewObj, DEQUE_MAPOBJ& m_setApply)
{
    if (!pNewObj)
    {
        return;
    }
    UINT i;
    DWORD dwSize	= m_setApply.size();
    if (pNewObj->GetObjType() != MAP_TERRAIN_PART)
    {
        for(i = 0; i < dwSize; i++)
        {
            CMapObj* pObj =  m_setApply[i];
            if (pObj || pObj->GetObjType() == MAP_TERRAIN_PART)
            {
                C2DMapTerrainObjPart* pPart = (C2DMapTerrainObjPart*)pObj;
                CMyPos posCell;
                pNewObj->GetPos(posCell);
                if (pPart)
                {
                    if (pPart->InMyBase(posCell))
                    {
                        pPart->AddObj(pNewObj);
                        return;
                    }
                }
            }
        }
    }
    CMyPos posNewObj;
    pNewObj->GetPos(posNewObj);
    int nNewObjX = posNewObj.x;
    int nNewObjY = posNewObj.y;
    int nOverLayAfter = -1;
    for(i = 0; i < dwSize; i++)
    {
        CMapObj* pOldObj	= m_setApply[i];
        CMyPos posOldObj;
        pOldObj->GetPos(posOldObj);
        int nOldObjX = posOldObj.x;
        int nOldObjY = posOldObj.y;
        CMySize sizeOldObj, sizeNewObj;
        g_objGameMap.GetMapObjSize(sizeOldObj, pOldObj);
        g_objGameMap.GetMapObjSize(sizeNewObj, pNewObj);
        // sort now!
        if ((nNewObjX - sizeNewObj.iWidth + 1) - nOldObjX > 0 ||
                (nOldObjX - sizeOldObj.iWidth + 1) - nNewObjX > 0)
        {
            // x no overlay
            if ((nNewObjY - sizeNewObj.iHeight + 1) - nOldObjY > 0 ||
                    (nOldObjY - sizeOldObj.iHeight + 1) - nNewObjY > 0)
            {
                // y no overlay
            }
            else // got y overlay
            {
                if (nNewObjX < nOldObjX)
                {
                    int bInsert = i;
                    for(int n = i - 1; n >= 0; n--) ///插在与J同行的物件或比当前排序物更近的物件之前
                    {
                        CMapObj* pObj	= m_setApply[n];
                        if (!pObj)
                        {
                            continue;
                        }
                        CMyPos posOldObj;
                        pObj->GetPos(posOldObj);
                        int nObjX = posOldObj.x;
                        int nObjY = posOldObj.y;
                        if ((nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY < nObjX + nObjY) && n > nOverLayAfter)
                        {
                            bInsert = n;
                        }
                        else
                        {
                            break;
                        }
                    }
                    m_setApply.insert(m_setApply.begin() + bInsert, pNewObj);
                    break;
                }
                else
                {
                    nOverLayAfter = i;
                    for(DWORD n = i + 1; n < dwSize; n++) ///插在与J同行的物件或比当前排序物更近的物件之后
                    {
                        CMapObj* pObj	= m_setApply[n];
                        if (!pObj)
                        {
                            continue;
                        }
                        CMyPos posOldObj;
                        pObj->GetPos(posOldObj);
                        int nObjX = posOldObj.x;
                        int nObjY = posOldObj.y;
                        if (nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY > nObjX + nObjY)
                        {
                            nOverLayAfter = n;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
        else // got x overlay
        {
            if (nNewObjY < nOldObjY)
            {
                int bInsert = i;
                for(int n = i - 1; n >= 0; n--) ///插在与J同行的物件或比当前排序物更近的物件之前
                {
                    CMapObj* pObj	= m_setApply[n];
                    if (!pObj)
                    {
                        continue;
                    }
                    CMyPos posOldObj;
                    pObj->GetPos(posOldObj);
                    int nObjX = posOldObj.x;
                    int nObjY = posOldObj.y;
                    if ((nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY < nObjX + nObjY) && n > nOverLayAfter)
                    {
                        bInsert = n;
                    }
                    else
                    {
                        break;
                    }
                }
                m_setApply.insert(m_setApply.begin() + bInsert, pNewObj);
                break;
            }
            else
            {
                nOverLayAfter = i;
                for(DWORD n = i + 1; n < dwSize; n++) ///插在与J同行的物件或比当前排序物更近的物件之后
                {
                    CMapObj* pObj	= m_setApply[n];
                    if (!pObj)
                    {
                        continue;
                    }
                    CMyPos posOldObj;
                    pObj->GetPos(posOldObj);
                    int nObjX = posOldObj.x;
                    int nObjY = posOldObj.y;
                    if (nOldObjX + nOldObjY == nObjX + nObjY || nNewObjX + nNewObjY > nObjX + nObjY)
                    {
                        nOverLayAfter = n;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    if (nOverLayAfter != -1) //insert after nOverLayAfter
    {
        if (nOverLayAfter >= (int)(m_setApply.size() - 1))
        {
            m_setApply.push_back(pNewObj);
        }
        else
        {
            m_setApply.insert(m_setApply.begin() + nOverLayAfter + 1, pNewObj);
        }
    }
    else
    {
        if (i >= dwSize)	// the last one...
        {
            DWORD n = 0;
            for (; n < dwSize; n++)
            {
                CMapObj* pOldObj	= m_setApply[n];
                if (!pOldObj)
                {
                    continue;
                }
                CMyPos posOldObj;
                pOldObj->GetPos(posOldObj);
                int nOldObjX = posOldObj.x;
                int nOldObjY = posOldObj.y;
                if (nNewObjX + nNewObjY < nOldObjX + nOldObjY)
                {
                    m_setApply.insert(m_setApply.begin() + n, pNewObj);
                    break;
                }
            }
            if (n >= dwSize)
            {
                m_setApply.push_back(pNewObj);
            }
        }
    }
    /*
    // sort now!
    if ((nNewObjX-sizeNewObj.iWidth+1)-nOldObjX > 0 ||
    	(nOldObjX-sizeOldObj.iWidth+1)-nNewObjX > 0)
    {	// x no overlay

    	int nNewData =  nNewObjY-(int)sizeNewObj.iHeight+1;
    	int nOldData =  nOldObjY-(int)sizeOldObj.iHeight+1;
    	if (nNewData>nOldObjY || nOldData>nNewObjY)
    	{
    		// y no overlay
    		if (nNewObjX+nNewObjY < nOldObjX+nOldObjY)
    		{
    			m_setApply.insert(m_setApply.begin()+i, pNewObj);
    			return;
    		}
    		// do nothing in this condition
    	}
    	else // got y overlay
    	{
    		if (nNewObjX < nOldObjX)
    		{
    			m_setApply.insert(m_setApply.begin()+i, pNewObj);
    			return;
    		}
    	}
    }
    else // got x overlay
    {
    	if (nNewObjY < nOldObjY)
    	{
    		m_setApply.insert(m_setApply.begin()+i, pNewObj);
    		return;
    	}
    }

    	}

    	//if (i >= dwSize)
    	m_setApply.push_back(pNewObj);*/
}
//-------------------------------------------------------------
void CInteractiveLayer::PlaceTerrainObj(C2DMapTerrainObj* pTerrainObj)
{
    if (!pTerrainObj)
    {
        return;
    }
    if (!pTerrainObj->GetFixed())
    {
        return;
    }
    int nAmount = pTerrainObj->GetPartAmount();
    for(int i = 0; i < nAmount; i++)
    {
        C2DMapTerrainObjPart* pPart = pTerrainObj->GetPartByIndex(i);
        if (!pPart)
        {
            return;
        }
        CMySize sizePart;
        pPart->GetBase(sizePart);
        for(int j = 0; j < sizePart.iHeight; j++)
        {
            for(int k = 0; k < sizePart.iWidth; k++)
            {
                CLayer* pLayer = pPart->GetLayer(k, j);
                LayerInfo* pNewLayer = new LayerInfo;
                if (!pNewLayer)
                {
                    return;
                }
                CMyPos posCell;
                pPart->GetPos(posCell);
                posCell.x -= k;
                posCell.y -= j;
                CellInfo* pCellInfo = g_objGameMap.GetCell(posCell.x, posCell.y);
                int nAltitude = g_objGameMap.GetGroundAltitude(pCellInfo);
                if (pCellInfo)
                {
                    pNewLayer->usMask = pLayer->usMask;
                    pNewLayer->usTerrain = pLayer->usTerrain;
                    pNewLayer->sAltitude = nAltitude + pLayer->sAltitude;
                    pNewLayer->pLayer = NULL;
                    g_objGameMap.AddLayer(pCellInfo, pNewLayer);
                }
            }
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::DisplaceTerrainObj(C2DMapTerrainObj* pTerrainObj)
{
    if (!pTerrainObj)
    {
        return;
    }
    if (!pTerrainObj->GetFixed())
    {
        return;
    }
    int nAmount = pTerrainObj->GetPartAmount();
    for(int i = 0; i < nAmount; i++)
    {
        C2DMapTerrainObjPart* pPart = pTerrainObj->GetPartByIndex(i);
        if (!pPart)
        {
            return;
        }
        CMySize sizePart;
        pPart->GetBase(sizePart);
        for(int j = 0; j < sizePart.iHeight; j++)
        {
            for(int k = 0; k < sizePart.iWidth; k++)
            {
                CMyPos posCell;
                pPart->GetPos(posCell);
                posCell.x -= k;
                posCell.y -= j;
                CellInfo* pCellInfo = g_objGameMap.GetCell(posCell.x, posCell.y);
                if (pCellInfo)
                {
                    g_objGameMap.DelLastLayer(pCellInfo);
                }
            }
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::Del3DMapEffect(CMyPos posWorld)
{
    int nAmount = m_setMapObj.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj && (pObj->GetObjType() == MAP_3DEFFECT || pObj->GetObjType() == MAP_3DEFFECTNEW))
        {
            C3DMapEffect* pEffect = (C3DMapEffect*)pObj;
            CMyPos posWorld;
            pEffect->GetWorldPos(posWorld);
            if ((posWorld.x == posWorld.x) &&
                    (posWorld.y == posWorld.y))
            {
                SAFE_DELETE(pEffect);
                m_setMapObj.erase(m_setMapObj.begin() + i);
            }
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::Del3DMapEffect(C3DMapEffect* pEffect)
{
    int nAmount = m_setMapObj.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj && (pObj->GetObjType() == MAP_3DEFFECT || pObj->GetObjType() == MAP_3DEFFECTNEW) && pEffect == pObj)
        {
            pEffect->Die();
            return;
        }
    }
}
//-------------------------------------------------------------
void CInteractiveLayer::Del3DMapItem(OBJID idItem)
{
    int nAmount = m_setMapObj.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj && pObj->GetObjType() == MAP_3DITEM)
        {
            C3DMapItem* pItem = (C3DMapItem*)pObj;
            if (pItem->GetID() == idItem)
            {
                SAFE_DELETE(pItem);
                m_setMapObj.erase(m_setMapObj.begin() + i);
                return;
            }
        }
    }
}

void CInteractiveLayer::Del2DMapItem(OBJID idItemtype)
{
    int nAmount = m_setMapObj.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj && pObj->GetObjType() == MAP_2DITEM)
        {
            C2DMapItem* pItem = (C2DMapItem*)pObj;
            if (pItem->GetItemID() == idItemtype)
            {
                SAFE_DELETE(pItem);
                m_setMapObj.erase(m_setMapObj.begin() + i);
                return;
            }
        }
    }
}
//-------------------------------------------------------------
int CInteractiveLayer::GetObjAmount()
{
    return m_setMapObj.size();
}
//-------------------------------------------------------------
CMapObj* CInteractiveLayer::GetObjByIndex(int nIndex)
{
    if ((nIndex < 0) || (nIndex >= (int)m_setMapObj.size()))
    {
        return NULL;
    }
    return m_setMapObj[nIndex];
}
//-------------------------------------------------------------
C2DMapCoverObj* CInteractiveLayer::GetMapCoverObj(CMyPos posCell)
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj->GetObjType() == MAP_COVER)
        {
            CMyPos posMyCell;
            pObj->GetPos(posMyCell);
            if ((posMyCell.x == posCell.x) && (posMyCell.y == posCell.y))
            {
                return (C2DMapCoverObj*)pObj;
            }
        }
    }
    return NULL;
}
//-------------------------------------------------------------
C3DMapEffect* CInteractiveLayer::Get3DFocusMapEffect()
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMapObj* pObj = m_setMapObj[i];
        if (pObj->GetObjType() == MAP_3DEFFECT || pObj->GetObjType() == MAP_3DEFFECTNEW)
        {
            C3DMapEffect* pEffect = (C3DMapEffect*)pObj;
            if (pEffect->IsFocus())
            {
                return pEffect;
            }
        }
    }
    return NULL;
}
//-------------------------------------------------------------
OBJID CInteractiveLayer::Get3DMapItem(CMyPos posCell)
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMapObj* pObj = m_setMapObj[i];
        CMyPos posObj;
        pObj->GetPos(posObj);
        if ((pObj->GetObjType() == MAP_3DITEM || pObj->GetObjType() == MAP_2DITEM) && posObj.x == posCell.x && posObj.y == posCell.y)
        {
            C3DMapItem* pItem = (C3DMapItem*)pObj;
            return pItem->GetID();
        }
    }
    return ID_NONE;
}

OBJID CInteractiveLayer::Get2DMapItem(CMyPos posCell)
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMapObj* pObj = m_setMapObj[i];
        CMyPos posObj;
        pObj->GetPos(posObj);
        if (pObj->GetObjType() == MAP_2DITEM && posObj.x == posCell.x && posObj.y == posCell.y)
        {
            C2DMapItem* pItem = (C2DMapItem*)pObj;
            return pItem->GetItemType();
        }
    }
    return ID_NONE;
}
//-------------------------------------------------------------
OBJID CInteractiveLayer::GetFocus3DMapItem(CMyPos& posCell)
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMapObj* pObj = m_setMapObj[i];
        CMyPos posObj;
        pObj->GetPos(posObj);
        if (pObj->GetObjType() == MAP_3DITEM)
        {
            C3DMapItem* pItem = (C3DMapItem*)pObj;
            if (pItem->IsFocus())
            {
                pItem->GetPos(posCell);
                return pItem->GetID();
            }
        }
    }
    return ID_NONE;
}

OBJID CInteractiveLayer::GetFocus2DMapItem(CMyPos& posCell)
{
    int nAmount = m_setMapObj.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMapObj* pObj = m_setMapObj[i];
        CMyPos posObj;
        pObj->GetPos(posObj);
        if (pObj->GetObjType() == MAP_2DITEM)
        {
            C2DMapItem* pItem = (C2DMapItem*)pObj;
            if (pItem->IsFocus())
            {
                pItem->GetPos(posCell);
                return pItem->GetItemID();
            }
        }
    }
    return ID_NONE;
}
//-------------------------------------------------------------
