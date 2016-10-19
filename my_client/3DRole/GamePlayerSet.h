
//**********************************************************
// 代码编辑器
//**********************************************************

// GamePlayerSet.h: interface for the CGamePlayerSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPLAYERSET_H__002C9957_B925_44B4_A009_7335AD638B68__INCLUDED_)
#define AFX_GAMEPLAYERSET_H__002C9957_B925_44B4_A009_7335AD638B68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basefunc.h"
#include "Role.h"
#include "Game3DEffectEx.h"
#include <deque>
#include "Player.h"

using namespace std;

class CPlayer;
typedef deque<CPlayer* >  DEQUE_PLAYER;

enum
{
    SYNDICATE_ALLY			= 0,	// 结盟
    SYNDICATE_ANTAGONIZE	= 1,	// 敌对
    SYNDICATE_SELF			= 2,	// 本帮
    SYNDICATE_NORMAL		= 3,	// 其他帮派
    SYNDICATE_SUB			= 4,	// 分团
    SYNDICATE_SUBSUB		= 5,	// 分队
};

enum
{
    PLAYER_NORMAL			= 0x0000,
    PLAYER_INCLUDESELF		= 0x0001,
    PLAYER_INCLUDEDEAD		= 0x0002,
    PLAYER_MONSTERPRI		= 0x0004,
    PLAYER_PLAYERPRI		= 0x0008,
    PLAYER_INCLUDEDEADMONSTER	= 0x0010,
    PLAYER_DEADONLY				= 0x0020,
};

typedef struct
{
    OBJID	idTopSyndicate;
    OBJID	idSyndicate;
    char	szSyndicateName[_MAX_NAMESIZE];
    char	szSyndicateLeaderName[_MAX_NAMESIZE];
    DWORD	dwStatus;
} InfoSyndicate;

#include <vector>
typedef vector<InfoSyndicate*> VECTOR_SYNDICATEINFO;

class CGamePlayerSet
{
public:
    CGamePlayerSet(void);
    virtual ~CGamePlayerSet(void);

    void	Init	(void);
    void	Destroy	(void);

    BOOL	AddPlayer	(CPlayer* pPlayer);
    BOOL	DelPlayer	(OBJID id, BOOL bDelobj = true);
    void	DelAllPlayer(BOOL bExcludeMyPet = false);

    int		GetPlayerAmount	(void) const {return m_dequePlayer.size();}
    CPlayer*	GetPlayerByIndex(DWORD dwIndex) const;

    CPlayer*	GetPlayer	(const char* pszName) const;
    CPlayer*	GetPlayer	(OBJID id, BOOL bIncludeSelf = false) const;
    CPlayer*	GetPlayer	(int nMapX, int nMapY, int nRoleType = _ROLE_NPC_NONE) const;

    void	ShowX	(void);
    void	Process	(int nHeroX, int nHeroY);
    void	ProcessManager(int nHeroX, int nHeroY);
    CPlayer*	GetFocusPlayer	(DWORD dwFlag = PLAYER_NORMAL);

private:
    DEQUE_PLAYER	m_dequePlayer;
    CPlayer*		m_pFocusPlayer;
    DWORD			m_dwTimeFor3DRole;
private:
    VECTOR_SYNDICATEINFO m_setSyndicateInfo;
    void	DestroySyndicateInfo();
    OBJID	m_idWhiteSyn;
    OBJID	m_idBlackSyn;
public:
    CGame3DEffectEx m_objFaceEffectWhite;
    CGame3DEffectEx m_objFaceEffectBlack;

public:
    BOOL	AddSyndicate(OBJID idSyndaicate, OBJID idTopSyndaicate, char* pszName, char* pszLeaderName, DWORD dwStatus);
    void	UpdateSyndicateName(OBJID idSyndaicate);
    char*	GetSyndicateName(OBJID idSyndaicate);
    OBJID	GetSyndicateIdByName(char* pszSynName);
    void	SetSyndicateName(OBJID idSyndaicate, char* pszName, char* pszSubName);
    char*	GetSyndicateLeaderName(OBJID idSyndaicate);
    void	SetSyndicateStatus(OBJID idSyndaicate, DWORD dwStatus);
    void	ClearAllSyndicateStatus();
    DWORD	GetSyndicateStatus(OBJID idSyndaicate);
    DWORD	GetAllySyndicateAmount();
    char*	GetAllySyndicateNameByIndex(int nIndex);
    DWORD	GetAntagonizeSyndicateAmount();
    char*	GetAntagonizeNameByIndex(int nIndex);

    DWORD	GetSubSyndicateId(OBJID idSyndicate, int nIndex);
    int 	GetSubSyndicateAmount(OBJID idSyndicate);

    void	SetWhiteSynId(OBJID idWhiteSyn);
    OBJID	GetWhiteSynId();
    void	SetBlackSynId(OBJID idBlackSyn);
    OBJID	GetBlackSynId();
    void	DestroySyndicate(OBJID idSyndicate);
    void	SetMantle(OBJID idSyndicate, DWORD dwMantle);
    static CPlayer* CreateNewPlayer() {CPlayer* pPlayer = new CPlayer; return pPlayer;}
};

extern CGamePlayerSet	g_objPlayerSet;

#endif // !defined(AFX_GAMEPLAYERSET_H__002C9957_B925_44B4_A009_7335AD638B68__INCLUDED_)
