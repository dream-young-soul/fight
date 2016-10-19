
//**********************************************************
// 代码编辑器
//**********************************************************

// AuctionSystemItemData.cpp: implementation of the CAuctionSystemItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "AuctionSystemItemData.h"
#include "MapGroup.h"
#include "ItemType.h"

char szAuctionsystemTable[]	= _TBL_AUCTION_SYSTEM;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CAuctionSystemItemData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuctionSystemItemData::CAuctionSystemItemData()
{
    m_SystemData_set.empty();
}
CAuctionSystemItemData::~CAuctionSystemItemData()
{
    this->CleanDataSet();
}
void CAuctionSystemItemData::CleanDataSet()
{
    AUCTION_DATA_SET::iterator it = m_SystemData_set.begin();
    for (; it != m_SystemData_set.end(); it++)
    {
        CGameAuctionSystemData* pData = *it;
        SAFE_RELEASE (pData);
    }
    m_SystemData_set.clear();
}
CGameAuctionSystemData* CAuctionSystemItemData::GetItemDataByID(int id)
{
    AUCTION_DATA_SET::iterator it = m_SystemData_set.begin();
    for (; it != m_SystemData_set.end(); it++)
    {
        CGameAuctionSystemData* pData = *it;
        if (pData && pData->GetID() == id)
        {
            return pData;
        }
    }
    return NULL;
}
CGameAuctionSystemData* CAuctionSystemItemData::GetItemDataByIndex(int nIndex)
{
    DEBUG_TRY
    AUCTION_DATA_SET::iterator it = m_SystemData_set.begin();
    for(int i = 0; it != m_SystemData_set.end() && i < nIndex; it++, i++);
    CGameAuctionSystemData* pData = *it;
    if (pData)
    {
        return pData;
    }
    return NULL;
    DEBUG_CATCH("CAuctionSystemItemData::GetItemDataByIndex(int nIndex) ERROR!")
    return NULL;
}
//根据要拍卖的物品的日期类型查找相配的物品
void CAuctionSystemItemData::GetItemDataByDateType(AUCTION_DATA_SET& Data_set, int nType, int nAmount)
{
    DEBUG_TRY
    AUCTION_DATA_SET::iterator it = m_SystemData_set.begin();
    for (int i = 0; i < nAmount && it != m_SystemData_set.end(); it++, i++)
    {
        CGameAuctionSystemData* pData = *it;
        if (pData && pData->GetInt(AUCTION_SYSTEMITEM_DATE_TYPE) == nType)
        {
            Data_set.push_back(pData);
        }
    }
    DEBUG_CATCH("CAuctionSystemItemData::GetItemDataByDateType ERRORF")
}

/*--------------------------------------------------------------------
*根据拍卖物品的类型和日期参数读入系统拍卖物品
*idNpc：为拍买NPC的ID,nDateType：为筛选的日期类型 szDate：为指定的日期
*nAmount:为当前系统拍卖的物品最大数量，nMaxType：系统拍卖分类的最大类型
*物品的列表（在重新开始一次新的系统拍买时要清除）
*-------------------------------------------------------------------*/
bool CAuctionSystemItemData::Create(IDatabase* pDb, OBJID idNpc, int nAmount, int nMaxType)
{
    DEBUG_TRY
    IF_NOT (pDb)
    return false;
    time_t ltime;
    time( &ltime );
    struct tm* pTime;
    pTime = localtime( &ltime ); /* Convert to local time. */
    int nCurYear, nCurMonth, nCurDay, nCurWeekDay;//, nCurYearDay;
    nCurYear	= pTime->tm_year + 1900;
    nCurMonth	= pTime->tm_mon + 1;
    nCurDay		= pTime->tm_mday;
    nCurWeekDay	= pTime->tm_wday;
    //	nCurYearDay	=pTime->tm_yday;
    int nCount = 0, nDateTypeTemp = 1;
    //	if (isClean)
    this->CleanDataSet();
    char szSQL[1024], szDateTemp[64];
    IRecordset*	pRes = NULL;
    while(nCount < nAmount && nDateTypeTemp <= nMaxType)
    {
        ZeroMemory(szDateTemp, sizeof(szDateTemp));
        switch(nDateTypeTemp)
        {
        case 1://筛选条件为某年某月某日的物品
            sprintf(szDateTemp, "%d-%d-%d", nCurYear, nCurMonth, nCurDay);
            break;
        case 2://筛选条件为某月某日的物品
            sprintf(szDateTemp, "%d-%d", nCurMonth, nCurDay);
            break;
        case 3://筛选条件为某日的物品
            sprintf(szDateTemp, "%d", nCurDay);
            break;
        case 4://筛选条件为某周几的物品
            sprintf(szDateTemp, "%d", nCurWeekDay);
            break;
        case 5://其他物品
            strcpy(szDateTemp, "0");
            break;
        }
        sprintf(szSQL, "SELECT * FROM %s WHERE auction_id=%d and date_type = %d and date_param = '%s' LIMIT %d", _TBL_AUCTION_SYSTEM, idNpc, nDateTypeTemp, szDateTemp, nAmount);
        pRes = pDb->CreateNewRecordset(szSQL);
        if (pRes)
        {
            nCount += pRes->RecordCount();
            for(int i = 0; i < pRes->RecordCount(); i++)
            {
                OBJID idItemType = pRes->GetInt(AUCTION_SYSTEMITEM_TYPE);
                CItemTypeData* pTypeData = ItemType()->QueryItemType(idItemType);
                IF_NOT (pTypeData)
                {
                    LOGWARNING(STR_AUCTION_SYS_ITEM_INVALID, idNpc, idItemType);
                    pRes->MoveNext();
                    continue;
                }
                CGameAuctionSystemData* pData = CGameAuctionSystemData::CreateNew();
                IF_NOT (pData)
                {
                    pRes->Release();
                    return false;
                }
                IF_NOT (pData->Create(pRes))
                {
                    pData->Release();
                    pRes->Release();
                    return false;
                }
                m_SystemData_set.push_back(pData);
                pRes->MoveNext();
            }
        }
        nDateTypeTemp++;
    }
    pRes->Release();
    return true;
    DEBUG_CATCH("CAuctionSystemItemData::Create")
    return false;
}
//////////////////////////////////////////////////////////////////////
bool CAuctionSystemItemData::DeleteRecord(int id)
{
    DEBUG_TRY
    AUCTION_DATA_SET::iterator it = m_SystemData_set.begin();
    for (; it != m_SystemData_set.end(); it++)
    {
        CGameAuctionSystemData* pData = *it;
        if (pData->GetID() == id)
        {
            if (!pData->DeleteRecord())
            {
                return false;
            }
            m_SystemData_set.erase(it);
            SAFE_RELEASE (pData);
            return true;
        }
    }
    return false;
    DEBUG_CATCH("CAuctionSystemItemData::DeleteRecord ERROR")
    return false;
}
