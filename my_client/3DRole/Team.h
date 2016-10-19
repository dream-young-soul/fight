
//**********************************************************
// 代码编辑器
//**********************************************************

// Team.h: interface for the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEAM_H__72157DB5_CD53_47C3_90DC_649832C0F995__INCLUDED_)
#define AFX_TEAM_H__72157DB5_CD53_47C3_90DC_649832C0F995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include "Game3DEffectEx.h"

typedef struct
{
    char	szName[_MAX_NAMESIZE];
    OBJID	id;
    DWORD	dwFace;
    DWORD	dwMaxHp;
    DWORD	dwHp;
    BOOL	bActive;
    DWORD	m_dwQueryBegin;
    BOOL	bBeginSearch;	// 是开始查找
    DWORD	dwLook;
} TeamMemberInfo;

#include <deque>
using namespace std;
typedef deque<TeamMemberInfo*>DEQUE_TEAMMEMBER;


class CTeam
{
public:
    CTeam();
    virtual ~CTeam();

private:
    DEQUE_TEAMMEMBER m_setMember;
    DWORD	m_dwTimeBegin;
    DWORD	m_dwTimeFocusBegin;
    OBJID	m_idPlayerLastApply;
    char	m_szNamePlayerLastApply[_MAX_NAMESIZE];
    BOOL	m_bClosed;
    int		m_nFocusIndex;
    int		m_nFocusIndexFrame;

public:
    void Destroy();
    void AddMember(TeamMemberInfo* pInfo);
    void DelMember(OBJID idMember);
    TeamMemberInfo* GetMemberInfo(OBJID idMember);
    TeamMemberInfo* GetMemberInfoByIndex(int nIndex);
    int GetMemberInfoAmount();

    BOOL SetLastApplyID(OBJID idPlayer);
    OBJID GetLastApplyID() {return m_idPlayerLastApply;}
    void CloseTeam() {m_bClosed = true;}
    void OpenTeam() {m_bClosed = false;}
    BOOL IsTeamClosed() {return m_bClosed;}
    char* GetLastApplyName() {return m_szNamePlayerLastApply;}

    CMyPos	m_posLeader;
public:
    static TeamMemberInfo* CreateNewMemberInfo(char* pszName, OBJID id,
            DWORD dwFace, DWORD dwHP, DWORD dwMaxHp, DWORD dwLook);
public:
    void Process();
    void Show(CMyPos posBegin);
};

#endif // !defined(AFX_TEAM_H__72157DB5_CD53_47C3_90DC_649832C0F995__INCLUDED_)
