
//**********************************************************
// 代码编辑器
//**********************************************************

// Package.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Package.h"
#include "User.h"
//#include "mapgroup.h"
MYHEAP_IMPLEMENTATION(CPackage, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPackage::CPackage()
{
    m_nType		= _STORE_NONE;
    m_setItem	= NULL;
}

//////////////////////////////////////////////////////////////////////
CPackage::~CPackage()
{
    if (m_setItem)
    {
        m_setItem->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CPackage::Create(int nType, OBJID idOwner, OBJID idPlayer, IDatabase* pDb)		// 可重复调用
{
    CHECKF(nType != _STORE_NONE);
    if (nType == m_nType && idOwner == m_idOwner && idPlayer == m_idPlayer)
    {
        return true;
    }
    m_nType		= nType;
    m_idOwner	= idOwner;
    m_idPlayer	= idPlayer;
    // add by zlong 2003-11-24 ---- 判断ucType
    switch (m_nType)
    {
    case ITEMPOSITION_STORAGE:
        m_ucType	= MSGPACKAGE_TYPE_STORAGE;
        break;
    case ITEMPOSITION_TRUNK:
        m_ucType	= MSGPACKAGE_TYPE_TRUNK;
        break;
    case ITEMPOSITION_CHEST:
        m_ucType	= MSGPACKAGE_TYPE_CHEST;
        break;
    case ITEMPOSITION_EUDEMON_BROODER:
        m_ucType	= MSGPACKAGE_TYPE_EUDEMON_BROODER;
        break;
    case ITEMPOSITION_EUDEMON_STORAGE:
        m_ucType	= MSGPACKAGE_TYPE_EUDEMON_STORAGE;
        break;
    case ITEMPOSITION_AUCTION_STORAGE:
    case ITEMPOSITION_AUCTION_SYS_STORAGE:
        m_ucType    = MSGPACKAGE_TYPE_AUCTION_STORAGE;
        break;
    default:
        m_ucType = MSGPACKAGE_TYPE_NONE;
        break;
    };
    if (m_setItem)
    {
        m_setItem->Release();
    }
    m_setItem	= CItemSet::CreateNew(true);
    CHECKF(m_setItem);
    int	nPosition = m_nType;		// 相同
    SQLBUF	szSQL;
    //-----------------------只有拍买系统用到ＬＷ-------------------------
    if (m_ucType == MSGPACKAGE_TYPE_AUCTION_STORAGE)
    {
        if (idPlayer == 0)//拍买ＮＰＣ仓库
        {
            sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && position=%d", _TBL_ITEM, idOwner, nPosition/*,AUCTION_CHIP*/);
        }
        else//玩家拍买仓库
        {
            sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && player_id=%u && position=%d LIMIT 6", _TBL_ITEM, idOwner, idPlayer, nPosition);
        }
    }
    //--------------------------------------------------------------------
    else
    {
        sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && player_id=%u && position=%d  LIMIT %d", _TBL_ITEM, idOwner, idPlayer, nPosition, PACKAGE_LIMIT);
    }
    IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
    if (!pRes)
    {
        return true;
    }
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        CItemPtr pItem = CItem::CreateNew();
        ASSERT(pItem);
        if (!pItem)
        {
            continue;
        }
        if (pItem->Create(pRes, pDb))
        {
            m_setItem->AddObj(pItem);
            // m_setItemEx->AddObj(pItem);
        }
        else
        {
            pItem->ReleaseByOwner();
        }
    }
    pRes->Release();
    return true;
}
//----------------------------------------------------------------------
//bool CPackage::AuctionGoodsBackOff
//LW 2005-01-12
//将拍买物品返还给玩家
//----------------------------------------------------------------------
CItem* CPackage::AuctionGoodsBackOff(OBJID idOwner, OBJID idItem, bool bUpdate/* = true*/)
{
    //	CHECKF(idPlayer != NULL);
    CHECKF(idOwner != NULL);
    CHECKF(m_nType != _STORE_NONE);
    CItemPtr pItem = m_setItem->GetObj(idItem);
    if (!pItem)
    {
        return false;
    }
    pItem->SetInt(ITEMDATA_OWNERID, idOwner, UPDATE_FALSE);
    pItem->SetInt(ITEMDATA_PLAYERID, idOwner, UPDATE_FALSE);
    pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_AUCTION_STORAGE, bUpdate);
    m_setItem->PopObj(idItem);
    return pItem;
}

//////////////////////////////////////////////////////////////////////
bool CPackage::CheckIn(CUser* pUser, CItem* pItem, bool bUpdate /*= true*/)
{
    DEBUG_TRY
    CHECKF(m_nType != _STORE_NONE);
    CHECKF(pUser);
    CHECKF(pItem);
    if (m_nType == ITEMPOSITION_CHEST && !pItem->IsExchangeEnable())
    {
        //		pUser->SendSysMsg("错误：此物品不宜存放!");
        return false;
    }
    int		nPosition = m_nType;		// 相同
    OBJID	idPlayer = (IsPlayerPackage() ? pUser->GetID() : ID_NONE);
    pItem->SetInt(ITEMDATA_OWNERID, m_idOwner, UPDATE_FALSE);
    pItem->SetInt(ITEMDATA_PLAYERID, idPlayer, UPDATE_FALSE);
    pItem->SetInt(ITEMDATA_POSITION, nPosition, bUpdate);
    m_setItem->AddObj(pItem);
    // synchro
    MsgPackageItemInfo	info;
    info.idItem		= pItem->GetID();
    info.idItemType	= pItem->GetInt(ITEMDATA_TYPE);
    info.usAmount	= pItem->GetInt(ITEMDATA_AMOUNT);
    info.usAmountLimit	= pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
    info.ucIdent	= pItem->GetInt(ITEMDATA_IDENT);
    info.ucGem1		= pItem->GetInt(ITEMDATA_GEM1);
    info.ucGem2		= pItem->GetInt(ITEMDATA_GEM2);
    info.ucMagic1	= pItem->GetInt(ITEMDATA_MAGIC1);
    info.ucMagic2	= pItem->GetInt(ITEMDATA_MAGIC2);
    info.ucMagic3	= pItem->GetInt(ITEMDATA_MAGIC3);
    info.nData		= pItem->GetInt(ITEMDATA_DATA);
    info.dwWarGhostExp		= pItem->GetInt(ITEMDATA_WARGHOSTEXP);
    info.dwGemAtkType		= pItem->GetInt(ITEMDATA_GEMTYPE);
    info.dwAvailableTime	= pItem->GetInt(ITEMDATA_AVAILABLETIME);
    ::SafeCopy(info.szName, pItem->GetStr(ITEMDATA_NAME), _MAX_NAMESIZE);
    // modified by zlong 2003-11-24 ---- 指定ucType类别
    CMsgPackage msg;
    CHECKF(msg.Create(MSGPACKAGE_CHECKIN, m_idOwner, &info, 1, m_ucType));
    pUser->SendMsg(&msg);
    DEBUG_CATCH("CPackage::CheckIn ERROR!")
    return true;
}
//----------------------------------------------------------------------
//CItem*  CPackage::CheckOut(OBJID idPlayer, OBJID idItem, bool bUpdate)
//LW 2005-01-12
//在玩家取出拍买仓库的物品时，删除在Ｎｐｃ拍买仓库中的Item内存变量，数据
//库在玩家取出物品时已修改，所以不用操作数据库
//----------------------------------------------------------------------
CItem*	CPackage::CheckOut(OBJID idItem)
{
    CItemPtr pItem = m_setItem->GetObj(idItem);
    if (!pItem)
    {
        return false;
    }
    m_setItem->PopObj(idItem);
    return pItem;
}
//////////////////////////////////////////////////////////////////////
CItem* CPackage::CheckOut(CUser* pUser, OBJID idItem, bool bUpdate)
{
    CHECKF(m_nType != _STORE_NONE);
    CItemPtr pItem = m_setItem->GetObj(idItem);
    if (!pItem)
    {
        return false;
    }
    pItem->SetInt(ITEMDATA_OWNERID, pUser->GetID(), UPDATE_FALSE);
    pItem->SetInt(ITEMDATA_PLAYERID, pUser->GetID(), UPDATE_FALSE);
    pItem->SetInt(ITEMDATA_POSITION, pUser->QueryPackage()->GetItemPosition(pItem), bUpdate);
    m_setItem->PopObj(idItem);
    // modified by zlong 2003-11-24 ---- 指定ucType类别
    // synchro
    CMsgPackage msg;
    CHECKF(msg.Create(MSGPACKAGE_CHECKOUT, m_idOwner, idItem, m_ucType));
    pUser->SendMsg(&msg);
    return pItem;
}
//////////////////////////////////////////////////////////////////////
bool CPackage::SendInfo(CUser* pUser)
{
    CHECKF(m_nType != _STORE_NONE);
    CHECKF(pUser);
    int nAction = MSGPACKAGE_QUERYLIST;
    MsgPackageItemInfo	buf[PACKAGE_LIMIT];
    int		nCount = 0;
    DWORD	dwCurrTime = ::TimeGet(TIME_SECOND);
    for(int i = 0; i < m_setItem->GetAmount(); i++)
    {
        CItemPtr pItem = m_setItem->GetObjByIndex(i);
        if (pItem && nCount < PACKAGE_LIMIT)
        {
            ///////////////////////////////////////////////////
            // 幻兽系统的幻兽蛋孵化在这里进行
            if (m_ucType == MSGPACKAGE_TYPE_EUDEMON_BROODER && pItem->IsEudemonEgg())
            {
                if (dwCurrTime > pItem->GetInt(ITEMDATA_AVAILABLETIME)
                        && dwCurrTime - pItem->GetInt(ITEMDATA_AVAILABLETIME) >= EUDEMON_HATCH_SECS)
                {
                    pUser->HatchEudemon(pItem);
                }
            }
            ///////////////////////////////////////////////////
            buf[nCount].usAmount	= pItem->GetInt(ITEMDATA_AMOUNT);
            buf[nCount].usAmountLimit	= pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
            buf[nCount].ucIdent		= pItem->GetInt(ITEMDATA_IDENT);
            buf[nCount].ucGem1		= pItem->GetInt(ITEMDATA_GEM1);
            buf[nCount].ucGem2		= pItem->GetInt(ITEMDATA_GEM2);
            buf[nCount].ucMagic1	= pItem->GetInt(ITEMDATA_MAGIC1);
            buf[nCount].ucMagic2	= pItem->GetInt(ITEMDATA_MAGIC2);
            buf[nCount].ucMagic3	= pItem->GetInt(ITEMDATA_MAGIC3);
            buf[nCount].idItem			= pItem->GetID();
            buf[nCount].idItemType	= pItem->GetInt(ITEMDATA_TYPE);
            buf[nCount].nData		= pItem->GetInt(ITEMDATA_DATA);
            buf[nCount].dwWarGhostExp	= pItem->GetInt(ITEMDATA_WARGHOSTEXP);
            buf[nCount].dwGemAtkType	= pItem->GetInt(ITEMDATA_GEMTYPE);
            // 如果是幻兽蛋，发送孵化剩余的时间
            if (pItem->IsEudemonEgg())
            {
                buf[nCount].dwAvailableTime	= __max(0, EUDEMON_HATCH_SECS - (int)(dwCurrTime - pItem->GetInt(ITEMDATA_AVAILABLETIME)));
            }
            else
            {
                buf[nCount].dwAvailableTime	= pItem->GetInt(ITEMDATA_AVAILABLETIME);
            }
            ::SafeCopy(buf[nCount].szName, pItem->GetStr(ITEMDATA_NAME), _MAX_NAMESIZE);
            ++nCount;
            // 为了避免一次数据过多，分多次发送
            const int	ITEMS_PER_PAGE	= 10;
            if (nCount >= ITEMS_PER_PAGE)
            {
                CMsgPackage msg;
                CHECKF(msg.Create(nAction, m_idOwner, buf, nCount, m_ucType));
                pUser->SendMsg(&msg);
                nCount = 0;
                nAction = MSGPACKAGE_QUERYLIST2;
            }
        }
        else
        {
            break;
        }
    }
    // modified by zlong 2003-11-24 ---- 指定ucType类别
    // synchro
    if (nCount > 0)
    {
        CMsgPackage msg;
        CHECKF(msg.Create(nAction, m_idOwner, buf, nCount, m_ucType));
        pUser->SendMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
int CPackage::GetItemType(int index)
{
    CHECKF(m_nType != _STORE_NONE);
    if (index >= 0 && index < m_setItem->GetAmount())
    {
        CItemPtr pItem = m_setItem->GetObjByIndex(index);
        if (pItem)
        {
            return  pItem->GetInt(ITEMDATA_TYPE);
        }
    }
    return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
int CPackage::GetWeight(OBJID idItem)
{
    CHECKF(m_nType != _STORE_NONE);
    CItemPtr pItem = m_setItem->GetObj(idItem);
    if (!pItem)
    {
        return ERROR_WEIGHT;
    }
    return pItem->GetWeight();
}

//////////////////////////////////////////////////////////////////////
OBJID CPackage::GetItemTypeByID(OBJID idItem)
{
    CHECKF(m_nType != _STORE_NONE);
    CItemPtr pItem = m_setItem->GetObj(idItem);
    if (!pItem)
    {
        return ID_NONE;
    }
    return pItem->GetInt(ITEMDATA_TYPE);
}
//----------------------------------------------------------------------
//CItem*  CPackage::CAuctionChip
//LW 2005-01-12
//给玩家一个拍买筹码，如果存在，取合并Ｄａｔａ，否则创建一个筹码
//当玩家不在线或不会拍买所在的地图组时，才会调用此函数。
//----------------------------------------------------------------------
bool  CPackage::CAuctionChip(/*OBJID idNpc,*/OBJID idUser, DWORD value, IRecordset* pRecordset, IDatabase* pDb)
{
    try  ////////
    {
        CHECKF(pDb);
        SQLBUF	szSQL;
        sprintf(szSQL, "SELECT * FROM %s WHERE type=%u && player_id=%d && position=%d  LIMIT 1", _TBL_ITEM, AUCTION_CHIP, idUser, ITEMPOSITION_AUCTION_STORAGE);
        IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
        if (pRes && pRes->RecordCount())
        {
            DEBUG_TRY
            pRes->SetInt(ITEMDATA_DATA, pRes->GetInt(ITEMDATA_DATA) + value);
            pRes->UpdateRecord();
            pRes->Release();
            return true;
            DEBUG_CATCH("CPackage::CAuctionChip UpdateRecord() ERROR!");
            return false;
        }
        CItemPtr pItem = NULL;
        pItem = CItem::CreateNew();
        if (pItem->Create(pRecordset, AUCTION_CHIP, idUser, ITEMPOSITION_AUCTION_STORAGE))
        {
            pItem->SetInt(ITEMDATA_DATA, value, UPDATE_TRUE);
            return true;
        }
        else
        {
            LOGERROR("Create DefaultItem Error!");
            return false;
        }
    }
    catch(...)
    {
        LOGERROR("CItem*  CPackage::CAuctionChip Error!");
        return false;
    }
}
DWORD CPackage::GetItemAmountLimitByID(OBJID idItem)
{
    CHECKF(m_nType != _STORE_NONE);
    CItemPtr pItem = m_setItem->GetObj(idItem);
    if (!pItem)
    {
        return 0;
    }
    return pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
}

