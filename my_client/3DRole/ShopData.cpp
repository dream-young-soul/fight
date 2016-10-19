
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ShopData.cpp: implementation of the CShopData class.
//
//////////////////////////////////////////////////////////////////////

#include "ShopData.h"
#include "GameMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopData::CShopData()
{
    m_setShopData.clear();
    this->Create();
}

CShopData::~CShopData()
{
    this->Destroy();
}

//-------------------------------------------------------------------

BOOL CShopData::GetItemList(OBJID idShop, DEQUE_ITEM& setItem, char* pszName, int& nType)
{
    if (idShop == ID_NONE)
    {
        g_objGameMsg.AddMsg("GetItemList ERROR ID=ID_NONE£¡");
        return false;
    }
    int nAmount = setItem.size();
    int i = 0;
    for(i = nAmount - 1; i >= 0; i --)
    {
        CItem* pItem = setItem[i];
        SAFE_DELETE(pItem);
    }
    setItem.clear();
    ShopDataInfo* pInfo = NULL;
    nAmount = m_setShopData.size();
    for(i = 0; i < nAmount; i++)
    {
        ShopDataInfo* pMyInfo = m_setShopData[i];
        if (pMyInfo && pMyInfo->idShop == idShop)
        {
            pInfo = pMyInfo;
            break;
        }
    }
    if (!pInfo)
    {
        char szMsg[128];
        sprintf(szMsg, "Cannot found Shop£¬ID=%u£¡", idShop);
        g_objGameMsg.AddMsg(szMsg);
        return false;
    }
    nType = pInfo->nType;
    strcpy(pszName, pInfo->szName);
    for(i = 0; i < (int)pInfo->dwItemAmount; i++)
    {
        OBJID* pCurrentId = pInfo->pidItem + i;
        CItem* pItem = new CItem;
        MYASSERT(pItem);
        if (!pItem->Create(96120059, *pCurrentId))
        {
            char szMsg[128];
            sprintf(szMsg, "create item for shop error£¬IdType=%u£¡", *pCurrentId);
            g_objGameMsg.AddMsg(szMsg);
            SAFE_DELETE(pItem);
            return false;
        }
        setItem.push_back(pItem);
    }
    return true;
}

//-------------------------------------------------------------------

void CShopData::Create()
{
    char szShopFile[] = "ini/Shop.dat";
    FILE* fp = fopen(szShopFile, "rb");
    if (!fp)
    {
        char szMsg[128];
        sprintf(szMsg, "open shop file %s error£¡", szShopFile);
        g_objGameMsg.AddMsg(szMsg);
        return;
    }
    // get Amount ...
    DWORD dwAmount;
    fread(&dwAmount, sizeof(DWORD), 1, fp);
    // get Shopdata
    for(DWORD i = 0; i < dwAmount; i ++)
    {
        ShopDataInfo* pInfo = new ShopDataInfo;
        MYASSERT(pInfo);
        // get shop ID
        fread(&pInfo->idShop, sizeof(OBJID), 1, fp);
        // get shop name
        fread(pInfo->szName, sizeof(char), _MAX_NAMESIZE, fp);
        // get shop type
        fread(&pInfo->nType, sizeof(int), 1, fp);
        // get id amount
        fread(&pInfo->dwItemAmount, sizeof(DWORD), 1, fp);
        // create id buf
        pInfo->pidItem = new OBJID[pInfo->dwItemAmount];
        fread(pInfo->pidItem, sizeof(OBJID), pInfo->dwItemAmount, fp);
        m_setShopData.push_back(pInfo);
    }
    fclose(fp);
}

//-------------------------------------------------------------------

void CShopData::Destroy()
{
    int nAmount = m_setShopData.size();
    for(int i = 0; i < nAmount; i++)
    {
        ShopDataInfo* pInfo = m_setShopData[i];
        delete[] pInfo->pidItem;
        SAFE_DELETE(pInfo);
    }
    m_setShopData.clear();
}

//-------------------------------------------------------------------

