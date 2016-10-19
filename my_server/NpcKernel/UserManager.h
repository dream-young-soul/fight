
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// UserManager.h: interface for the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMANAGER_H__D248EA80_FC46_4ED1_8B7A_7A9931A469BD__INCLUDED_)
#define AFX_USERMANAGER_H__D248EA80_FC46_4ED1_8B7A_7A9931A469BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "User.h"
#include "GameObjSet.h"
#include <map>
using namespace std;

typedef	CGameObjSet<CUser>	IUserSet;
typedef CGameObjSet<CUser>	CUserSet;

class CUserManager
{
public:
    CUserManager();
    virtual ~CUserManager();
    bool	Create();
    virtual ULONG Release()				{ delete this; return 0; }
    virtual IUserSet* QuerySet()		{ CHECKF(m_pUserSet); return m_pUserSet; }
    CUser*	QueryUser(OBJID idUser)		{ return m_pUserSet->GetObj(idUser); }
    CUser*	QueryUser(LPCTSTR szUser);
    IRole*	QueryRole(OBJID idRole);

public:
    void OnTimer(DWORD nCurr);

public:
    static int	GetSizeAdd(int nLook);

protected:
    IUserSet*		m_pUserSet;

    typedef	map<int, int>	SIZEADD_SET;
    static SIZEADD_SET		m_setSizeAdd;
};

#endif // !defined(AFX_USERMANAGER_H__D248EA80_FC46_4ED1_8B7A_7A9931A469BD__INCLUDED_)
