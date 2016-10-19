
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// WeaponSkill.h: interface for the CWeaponSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONSKILL_H__A18A22A1_D85C_4E83_87A2_0E24119332B4__INCLUDED_)
#define AFX_WEAPONSKILL_H__A18A22A1_D85C_4E83_87A2_0E24119332B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

const int SINGLEHAND_WEAPON_SKILL = 0;
const int DOUBLEHAND_WEAPON_SKILL = 1;
const int OTHER_WEAPON_SKILL	  = 2;

typedef struct
{
    DWORD	dwWeaponSkillIndex;
    char	szWeaponSkillName[_MAX_NAMESIZE];
} WeaponSkillInfo;

typedef struct
{
    DWORD	dwLevel;
    DWORD	dwExp;
} WeaponSkillLevelExp;


#include <deque>
using namespace std;
typedef deque<WeaponSkillInfo*>DEQUE_WEAPONSKILLINFO;
typedef deque<WeaponSkillLevelExp*>DEQUE_WEAPONSKILLLEVELEXPINFO;

class CWeaponSkillData
{
public:
    CWeaponSkillData();
    virtual ~CWeaponSkillData();

private:
    DEQUE_WEAPONSKILLINFO m_setWeaponSkillInfo;
    DEQUE_WEAPONSKILLLEVELEXPINFO m_setWeaponSkillLevelExp;
    char m_szError[_MAX_NAMESIZE];

private:
    void Create();
    void Destroy();

public:
    char* GetWeaponSkillName(DWORD dwIndex);
    DWORD GetExpRequired(DWORD dwLevel);
};

class CWeaponSkill
{
public:
    static CWeaponSkillData s_Data;
public:
    CWeaponSkill();
    virtual ~CWeaponSkill();
private:
    int		m_nWeaponSkillType;
    DWORD	m_dwLevel;
    DWORD	m_dwExp;
    char	m_szDescribe[_MAX_TITLESIZE];

public:
    int		GetType() {return m_nWeaponSkillType;}
    void	SetType(int nWeaponSkillType) {m_nWeaponSkillType = nWeaponSkillType;}
    DWORD	GetLevel() {return m_dwLevel;}
    void	SetLevel(DWORD dwLevel) {m_dwLevel = dwLevel;}
    DWORD	GetExp() {return m_dwExp;}
    void	SetExp(DWORD dwExp) {m_dwExp = dwExp;}
    int		GetWeaponType();
    int		GetWeaponSubType();
    char*	GetName();
    DWORD	GetNextLevelExpRequired();
    char*	GetDescribe();
};
#include <deque>
using namespace std;
typedef deque<CWeaponSkill*>DEQUE_WEAPONSKILL;

#endif // !defined(AFX_WEAPONSKILL_H__A18A22A1_D85C_4E83_87A2_0E24119332B4__INCLUDED_)
