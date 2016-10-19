
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Magic.h: interface for the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGIC_H__9CC51658_212A_4D02_BD47_A2FA5B3378C3__INCLUDED_)
#define AFX_MAGIC_H__9CC51658_212A_4D02_BD47_A2FA5B3378C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicData.h"

class CMagic
{
public:
    CMagic();
    virtual ~CMagic();
private:
    BOOL m_bEnable;
public:
    DWORD m_dwExp;
    OBJID m_idOwner;
    MagicTypeInfo m_infoMagicType;
    static CMagicData s_objMagicData;
    DWORD m_dwTimeBegin;
public:
    BOOL Create(OBJID idType, DWORD dwLevel, DWORD dwExp);
    static CMagic* CreateNew(OBJID idType, DWORD dwLevel, DWORD dwExp);

    char* GetName();
    char* GetDescribe();
    DWORD GetExp();
    DWORD GetNextLevelExp();
    DWORD GetNeedLevel() {return m_infoMagicType.dwNeedLevel;}
    DWORD GetLevel();
    OBJID GetIDType();

    BOOL  TestTarget(DWORD dwMask);
    DWORD GetBusyDuration();
    DWORD GetWeaponRequired	(void) { return m_infoMagicType.dwWeaponSubType; }
    DWORD GetPower	(void) { return m_infoMagicType.dwPower; }
    BOOL  IsTransformMagic();
    BOOL  IsEnable() {return m_bEnable;}
    BOOL  Process();
    BOOL  PetMagicProcess();
    BOOL  HeroMagicProcess();
    void  StartBusy() {m_dwTimeBegin = ::TimeGet();}
};

#include <vector>
using namespace std;
typedef vector<CMagic* > VECTOR_MAGIC;
#endif // !defined(AFX_MAGIC_H__9CC51658_212A_4D02_BD47_A2FA5B3378C3__INCLUDED_)
