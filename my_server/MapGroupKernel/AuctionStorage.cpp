
//**********************************************************
// 代码编辑器
//**********************************************************

// AuctionStorage.cpp: implementation of the CAuctionStorage class.
// 拍买系统仓库，只由Auction 调用   －LW-   2004-11-26
//////////////////////////////////////////////////////////////////////

#include "AuctionStorage.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuctionStorage::CAuctionStorage()
{
    //memset(m_Info, 0L, sizeof(AUCTION_STORAGE_INFO));
    m_pInfo =  NULL;
}

CAuctionStorage::~CAuctionStorage()
{
    DEBUG_TRY
    pAUCTION_STORAGE_INFO pInfo;
    for(int i = 0; i < m_SetAuctionQueue.size(); i++)
    {
        pInfo = m_SetAuctionQueue.pop();
        if (pInfo)
        {
            SAFE_DELETE(pInfo);
        }
    }
    m_SetAuctionQueue.clear();
    for(int i = 0; i < m_SetSysAuction.size(); i++)
    {
        pInfo = m_SetSysAuction.pop();
        if (pInfo)
        {
            SAFE_DELETE(pInfo);
        }
    }
    m_SetSysAuction.clear();
    DEBUG_CATCH("CAuctionStorage::~CAuctionStorage() ERROR!")
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//--------------------------------------------
//LW --2004-01-18--
//bool CAuctionStorage::Create(OBJID idNpc,int type,IDatabase* pDb)
//返回false失败 true表示成功
//--------------------------------------------
bool CAuctionStorage::Create(OBJID idNpc, int iType, IDatabase* pDb)
{
    ASSERT(idNpc);
    ASSERT(pDb);
    SQLBUF	szSQL;
    DEBUG_TRY
    if (iType == PLAYER_ITEM)//非系统物品
    {
        sprintf(szSQL, "SELECT * FROM %s WHERE auction_id=%u and state=%d LIMIT %d", _TBL_AUCTION, idNpc, PLAYER_ITEM, AUCTION_PACKAGE_MAX);
    }
    else if (iType == SYSTEM_ITEM)//系统物品
    {
        sprintf(szSQL, "SELECT * FROM %s WHERE auction_id=%u and state=%d LIMIT %d", _TBL_AUCTION, idNpc, SYSTEM_ITEM, AUCTION_SYSTEM_MAX);
    }
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
    if (!pRes)
    {
        return true;
    }
    for(int i = 0; i < pRes->RecordCount(); i++)
    {
        if (pRes)
        {
            pAUCTION_STORAGE_INFO pInfo = new AUCTION_STORAGE_INFO;
            if (LoadInfo(pRes, pInfo))
            {
                if (iType == PLAYER_ITEM)
                {
                    m_SetAuctionQueue.push(pInfo);
                }
                else
                {
                    pInfo->idPlayer = 0;
                    if (!strcmp(pInfo->szPlayer, ""))
                    {
                        strcpy(pInfo->szPlayer, SYSTEM_NAME);
                    }
                    m_SetSysAuction.push(pInfo);
                }
            }
            pRes->MoveNext();
        }
    }
    pRes->Release();
    DEBUG_CATCH("CAuctionStorage::Create Error!")
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CAuctionStorage::Create(IRecordset* pRes)
{
    /*
        CHECKF(pRes);
    	IRecord* pRecord = pRes->CreateNewRecord();
        bool ret = LoadInfo(pRecord);
    	pRecord->Release();*/
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CAuctionStorage::LoadInfo(IRecord* pRes, pAUCTION_STORAGE_INFO pInfo)
{
    if (!pRes)
    {
        return false;
    }
    pInfo->id				 = pRes->LoadDWord("id");
    pInfo->idAuction         = pRes->LoadDWord("auction_id");
    pRes->LoadString(pInfo->szPlayer, "auction_player", _MAX_NAMESIZE);
    pInfo->idItem		   	 = pRes->LoadDWord("item_id");
    pInfo->dwValue           = pRes->LoadDWord("value");
    pInfo->iState            = pRes->LoadDWord("state");
    //	info->itype              = pRes->LoadDWord("type");
    return true;
}
//////////////////////////////////////////////////////////////////////
OBJID CAuctionStorage::CreateRecord(OBJID idAuction, const char* szPlayer, OBJID idItem, DWORD dwValue, UCHAR nType, IDatabase* pDb)
{
    CHECKF( pDb);
    m_pInfo  = new AUCTION_STORAGE_INFO;
    m_pInfo->idAuction    = idAuction;
    strcpy(m_pInfo->szPlayer, szPlayer);
    m_pInfo->idItem		 = idItem;
    m_pInfo->dwValue      = dwValue;
    m_pInfo->iState       = nType;
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT %s SET auction_id = %d,auction_player='%s',item_id=%d, value=%u,state = %d",
            _TBL_AUCTION, idAuction, szPlayer, idItem, dwValue, nType);
    pDb->ExecuteSQL(szSQL);
    m_pInfo->id	= pDb->GetInsertId();
    return m_pInfo->id;
}

//////////////////////////////////////////////////////////////////////
bool CAuctionStorage::DeleteRecord(OBJID id, IDatabase* pDb)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "DELETE FROM %s WHERE id=%u", _TBL_AUCTION, id);
    return(pDb->ExecuteSQL(szSQL));
}
