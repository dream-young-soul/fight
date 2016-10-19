
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMapSimpleObj.h: interface for the C3DMapSimpleObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DMAPSIMPLEOBJ_H__8769B046_2EB4_46E8_B672_FA6FADBC0571__INCLUDED_)
#define AFX_3DMAPSIMPLEOBJ_H__8769B046_2EB4_46E8_B672_FA6FADBC0571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DMapObj.h"
#include "3DSimpleObj.h"

class C3DMapSimpleObj	:	public C3DMapObj
{
public:
    C3DMapSimpleObj();
    virtual ~C3DMapSimpleObj();

private:
    C3DSimpleObj	m_obj3DSimpleObj;
    CMyPos			m_posWorld;
    DWORD			m_dwTimeBegin;
    DWORD			m_dwFrameInterval;
    BOOL			m_bDie;
    BOOL			m_bRole;
    float			m_fHorizontal;
    float			m_fVertical;

public:
    void SetRotate(float fHorizontal, float fVertical) {m_fHorizontal = fHorizontal, m_fVertical = fVertical;}
    void SetWorldPos(CMyPos posWorld);
    void GetWorldPos(CMyPos& posWorld);
    void SetFrameInterval(DWORD dwFrameInterval);
    void AddEffect(char* pszIndex);
    void Die() {m_bDie = true;}
    BOOL IsDie() {return m_bDie;}
public:
    void Show(void* pInfo);
    void Process(void* pInfo);
    void SetPos();
    int GetObjType() {return MAP_3DSIMPLE;}
    BOOL IsFocus() {return false;}
    BOOL Create(OBJID idType, CMyPos posWorld, BOOL bRole = true);

public:
    static C3DMapSimpleObj* CreateNew(OBJID idType, CMyPos posWorld, BOOL bRole = true);
};

#endif // !defined(AFX_3DMAPSIMPLEOBJ_H__8769B046_2EB4_46E8_B672_FA6FADBC0571__INCLUDED_)
