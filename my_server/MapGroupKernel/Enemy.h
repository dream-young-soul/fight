
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Enemy.h: interface for the CEnemy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENEMY_H__E9D8903D_D783_438B_B9F2_47B5BF6160B2__INCLUDED_)
#define AFX_ENEMY_H__E9D8903D_D783_438B_B9F2_47B5BF6160B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycom.h"
#include "array.h"
#include "vartype.h"

const int	MAX_ENEMYSIZE			= 10;		//??? info size is 10*(16+4)=200, close to 256 limit

struct CEnemyInfoStruct
{
    OBJID	setEnemy[MAX_ENEMYSIZE];
    char	setName[MAX_ENEMYSIZE][_MAX_NAMESIZE];
};

class IDatabase;
class CEnemy
{
    COM_STYLE(CEnemy)
protected:
    CEnemy();
    virtual ~CEnemy();

public:
    bool	Create(PROCESS_ID idProcess, CUser* pOwner, IDatabase* pDb);
    bool	Create(PROCESS_ID idProcess, CUser* pOwner);	// for chgmap

public:
    void	Add(OBJID idEnemy, LPCTSTR szName, bool bSynchro, bool bUpdate);
    void	Del(OBJID idEnemy, bool bUpdate);
    LPCTSTR	GetName(OBJID idEnemy);
    void	SendToClient();
    void	BeforeLogout();
public: // persistent
    bool	GetInfo(CEnemyInfoStruct* pInfo);
    bool	AppendInfo(const CEnemyInfoStruct* pInfo);

public:
    typedef	Array<pair<OBJID, String> >	ENEMY_SET;
protected:
    ENEMY_SET	m_set;
    CUser*		m_pOwner;
    PROCESS_ID	m_idProcess;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_ENEMY_H__E9D8903D_D783_438B_B9F2_47B5BF6160B2__INCLUDED_)
