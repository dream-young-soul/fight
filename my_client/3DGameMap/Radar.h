
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Radar.h: interface for the CRadar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADAR_H__F603F447_D77F_48B4_A419_C0DD98DD7917__INCLUDED_)
#define AFX_RADAR_H__F603F447_D77F_48B4_A419_C0DD98DD7917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include "Ani.h"
#include "Hero.h"

enum
{
    _MODE_128 = 0,
    _MODE_256 = 1,
    _MODE_512 = 2,
};
class CRadar
{
public:
    CRadar();
    virtual ~CRadar();
public:
    BOOL IsOnlyTeamMember(TeamMemberInfo* pInfo);
    void LoadNpc();
    void CellPos2RaderPos(CAni* pAni, CMyPos posCell, CMyPos& posRader);
    void ShowBigMap(CMyPos posScr, OBJID idMap, CMySize sizeDlgBg);
    void Show(CMyPos posScr, OBJID idMap, int nMode);
private:
    typedef struct
    {
        OBJID idMapDoc;
        CMyPos posNpc;
        int	   NpcType;
    } NpcInfo;
    vector<NpcInfo> m_setNpcInfo;
};

#endif // !defined(AFX_RADAR_H__F603F447_D77F_48B4_A419_C0DD98DD7917__INCLUDED_)
