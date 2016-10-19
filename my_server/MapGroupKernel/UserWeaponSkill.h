
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// UserWeaponSkill.h: interface for the CUserWeaponSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERWEAPONSKILL_H__28E1B0C3_1380_4E65_8DCF_BA7094D5B62E__INCLUDED_)
#define AFX_USERWEAPONSKILL_H__28E1B0C3_1380_4E65_8DCF_BA7094D5B62E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "windows.h"
#include "MyHeap.h"
#include "GameData.h"
#include "GameObj.h"

//////////////////////////////////////////////////////////////////////
enum WPSKILLDATA
{
    WPSKILLDATA_NAME = 1,
};

char	szWpSkillTable[];
typedef	CGameData<WPSKILLDATA, szWpSkillTable, szID>	CUserWeaponSkillData;

//////////////////////////////////////////////////////////////////////
struct	UserWeaponSkillInfoStruct
{
    OBJID		id;
    OBJID		idowner;
    int			nType;
    UCHAR		ucLevel;
    int			nExp;
    UCHAR		ucOldLev;
};

class IRecord;
class IRecordset;
class IDatabase;

class CUserWeaponSkill
{
public:
    CUserWeaponSkill();
    virtual ~CUserWeaponSkill();

public:
    static		CUserWeaponSkill*		CreateNew			(IRecordset* pRes);
    static		CUserWeaponSkill*		CreateNew			(UserWeaponSkillInfoStruct* pInfo, IRecordset* pDefault);
    static		CUserWeaponSkill*		InsertNewRecord		(UserWeaponSkillInfoStruct* pInfo, IRecordset* pDefault, OBJID idNew);
    static		BOOL					CreateNewRecordset	(UserWeaponSkillInfoStruct& info, IDatabase* pDb);

public:
    BOOL	SaveInfo			(void)			{ return m_pRes->UpdateRecord(); }
protected:
    BOOL	Create				(IRecordset* pRes);
    BOOL	LoadInfo			(void);
    MYHEAP_DECLARATION(m_sHeap)

public:
    UserWeaponSkillInfoStruct*	GetInfo()		{ return &m_Info; }
    ULONG	ReleaseByOwner		()		{delete this; return 0;}

    int		GetType				();
    void	SetType				(int nType, BOOL bUpdate = FALSE);

    int		GetLevel			();
    void	SetLevel			(int nLevel, BOOL bUpdate = FALSE);

    int		GetExp				();
    void	SetExp				(int nExp, BOOL bUpdate = FALSE);

    int		GetOldLev			();
    void	SetOldLev			(int nOldLev, BOOL bUpdate = TRUE);

    OBJID	GetID				();
    void	SetID				(OBJID id);

    OBJID	GetOwnerID			();
    void	SetOwnerID			(OBJID idOwner, BOOL bUpdate = TRUE);

    void	SetUnlearn			(int nData, BOOL bUpdate = TRUE);
    bool	DeleteRecord		();

protected:
    UserWeaponSkillInfoStruct		m_Info;
    IRecord*						m_pRes;
};

#endif // !defined(AFX_USERWEAPONSKILL_H__28E1B0C3_1380_4E65_8DCF_BA7094D5B62E__INCLUDED_)
