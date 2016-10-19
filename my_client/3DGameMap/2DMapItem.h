
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 2DMapItem.h: interface for the C2DMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DMAPITEM_H__BEB266A1_58BE_4541_8B61_DC18A5BF9C9A__INCLUDED_)
#define AFX_2DMAPITEM_H__BEB266A1_58BE_4541_8B61_DC18A5BF9C9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DMapObj.h"
#include "AniEffect.h"
#include "Game3DEffectEx.h"

class C2DMapItem  : public C2DMapObj
{
public:
    C2DMapItem(OBJID idItemType);
    C2DMapItem() {}
    virtual ~C2DMapItem();

    OBJID	GetItemType() {return m_idItemType;}
    OBJID	GetItemID() {return m_idItem;}
private:
    DWORD	m_dwBegin;
    DWORD	m_dwFrame;
    OBJID   m_idItem ;
    OBJID	m_idItemType;
    CMyPos	m_posWorld;
    BOOL	m_bShowName;
    char	m_szName[_MAX_NAMESIZE];
    CGame3DEffectEx m_objEffectEx;

public:
    void SetShowName(char* szName) {strcpy(m_szName, szName);}
    void Show(void* pInfo);
    void Process(void* pInfo);
    int	 GetObjType() {return MAP_2DITEM;}
    BOOL IsFocus();
    void BeShowName(BOOL bShow) {m_bShowName = bShow;}

public:
    static C2DMapItem* CreateNew(CMyPos posCell, OBJID idItem, OBJID idItemType, char* szName = NULL);
};

#endif // !defined(AFX_2DMAPITEM_H__BEB266A1_58BE_4541_8B61_DC18A5BF9C9A__INCLUDED_)
