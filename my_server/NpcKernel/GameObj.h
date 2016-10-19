
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// GameObj.h: interface for the CGameObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOBJ_H__0E59B91E_AF23_466F_B7FC_63963D268670__INCLUDED_)
#define AFX_GAMEOBJ_H__0E59B91E_AF23_466F_B7FC_63963D268670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>

#include "define.h"
#include "basefunc.h"

char	szID[];

class CGameObj
{
public:
    CGameObj();
    virtual ~CGameObj();

    virtual	OBJID	GetID()	{::LogSave("Fatal error in CGameObj::GetID()."); return ID_NONE;}//=0;

    virtual int		GetObjType()	{return m_nObjType;}
    virtual void	SetObjType(int nType)	{m_nObjType = nType;}

    static BOOL SafeCheck	(CGameObj* pObj);

private:
    int		m_nObjType;
};


#endif // !defined(AFX_GAMEOBJ_H__0E59B91E_AF23_466F_B7FC_63963D268670__INCLUDED_)
