
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMagicMapItem.h: interface for the C3DMagicMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DMAGICMAPITEM_H__302FCD51_7B21_490B_9B78_764817BA83EC__INCLUDED_)
#define AFX_3DMAGICMAPITEM_H__302FCD51_7B21_490B_9B78_764817BA83EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DMapObj.h"
#include "TerrainEffectManager.h"

class C3DMagicMapItem   :	public C3DMapObj
{
public:
    C3DMagicMapItem();
    virtual ~C3DMagicMapItem();
private:
    CMyPos	m_posBg;
    char	m_szStart[_MAX_PATH];
    char	m_szLast[_MAX_PATH];
    char	m_szEnd[_MAX_PATH];
    OBJID	m_id;
    CTerainEffectUnit* m_pUnit;
    int		m_nMoment;
public:
    virtual void Show(void* pInfo);
    virtual void Process(void* pInfo);
    virtual void SetBgPos(CMyPos posBg);
    virtual void GetBgPos(CMyPos& posWorld);
    virtual int  GetObjType() {return MAP_3DMAGICMAPITEM;}
    virtual BOOL IsFocus();
    virtual void SetPos(CMyPos posCell);

private:
    BOOL	Create(OBJID idType, OBJID id);
public:
    static C3DMagicMapItem* CreateNew(OBJID idType, OBJID id);
    OBJID	GetID() {return m_id;}
    void	Last() {m_nMoment = 1;}
    // set id = id_none. this obj has been deleted by server
    void	End() {m_nMoment = 2; m_id = ID_NONE; SAFE_DELETE(m_pUnit);}
};

#endif // !defined(AFX_3DMAGICMAPITEM_H__302FCD51_7B21_490B_9B78_764817BA83EC__INCLUDED_)
