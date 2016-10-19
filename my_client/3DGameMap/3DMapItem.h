
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMapItem.h: interface for the C3DMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DMAPITEM_H__AF8E7288_4A93_43D1_A281_EA3EF0C6C602__INCLUDED_)
#define AFX_3DMAPITEM_H__AF8E7288_4A93_43D1_A281_EA3EF0C6C602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DMapObj.h"
#include "3DSimpleObj.h"
#include "Game3DEffectEx.h"

class C3DMapItem	:	public C3DMapObj
{
public:
    C3DMapItem();
    virtual ~C3DMapItem();
private:
    OBJID			m_idType;
    OBJID			m_idItem;
    C3DSimpleObj	m_obj3DSimpleObj;
    CMyPos			m_posBg;
    DWORD			m_dwTimeBegin;
    int				m_nFrameIndex;
    unsigned short	m_usMapPercent;
    CGame3DEffectEx m_objEffect;
    BOOL			m_bDie;
public:
    void Show(void* pInfo);
    void Process(void* pInfo);
    int	 GetObjType() {return MAP_3DITEM;}
    BOOL IsFocus();
    BOOL Create(OBJID idType, OBJID idItem, CMyPos posWorld);
    OBJID GetID() {return m_idItem;}
};

#endif // !defined(AFX_3DMAPITEM_H__AF8E7288_4A93_43D1_A281_EA3EF0C6C602__INCLUDED_)
