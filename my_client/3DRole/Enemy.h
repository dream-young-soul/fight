
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Enemy.h: interface for the CEnemy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENEMY_H__CC5A9660_4AE9_429E_8E5E_6FC497EC6EEE__INCLUDED_)
#define AFX_ENEMY_H__CC5A9660_4AE9_429E_8E5E_6FC497EC6EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

class CEnemy
{
public:
    CEnemy();
    virtual ~CEnemy();

private:
    OBJID	m_idEnemy;
    char	m_szName[_MAX_NAMESIZE];
    BOOL	m_bFlagOnline;

    DWORD	m_dwFace;
    DWORD	m_dwLook;
    DWORD	m_dwLevel;
    DWORD	m_dwProfessional;
    int		m_nPK;
    DWORD	m_IDSyndicate;
    DWORD	m_dwRank;
    DWORD	m_dwRace;
    char	m_szMate[_MAX_NAMESIZE];
    UCHAR   m_ucNobilityRank;
    // the operate interface(s)
public:
    static CEnemy* CreateNew(OBJID id, char* pszName, DWORD dwLookFace, BOOL m_bFlagOnline);
    // get...
    OBJID	GetID() {return m_idEnemy;}
    DWORD	GetRace() {return m_dwRace;}
    DWORD	GetLook() {return m_dwLook;}
    char*	GetName() {return m_szName;}
    BOOL	IsOnline() {return m_bFlagOnline;}
    DWORD	GetFace() {return m_dwFace;}
    DWORD	GetLevel() {return m_dwLevel;}
    DWORD	GetProfessional();
    int		GetPK() {return m_nPK;}
    OBJID	GetSynDicateID() {return m_IDSyndicate;}
    DWORD	GetRank() {return m_dwRank;}
    char*	GetMate() {return m_szMate;}
    UCHAR   GetNobilityRank() {return m_ucNobilityRank;}
    // additional get ...
    char*	GetProfessionalName();
    char*	GetSyndicateName();
    char*	GetRankName();

    // set...
    void	SetID(OBJID id) {m_idEnemy = id;}
    void	SetName(char* pszName) {assert(pszName); strcpy(m_szName, pszName);}
    void	Online() {m_bFlagOnline = true;}
    void	Offline() {m_bFlagOnline = false;}
    void	SetFace(DWORD dwFace) {m_dwFace = dwFace;}
    void	SetLook(int nLook) {m_dwLook = nLook;}
    void	SetRace(int nRace) {m_dwRace = nRace;}
    void	SetLevel(DWORD dwLevel) {m_dwLevel = dwLevel;}
    void	SetProfessional(DWORD dwProfessional) {m_dwProfessional = dwProfessional;}
    void	SetPK(int nPK) {m_nPK = nPK;}
    void	SetSyndicateID(OBJID idSyndicate) {m_IDSyndicate = idSyndicate;}
    void	SetRank(DWORD dwRank) {m_dwRank = dwRank;}
    void	SetMate(char* pszMate) {strcpy(m_szMate, pszMate);}
    void	SetNobilityRank(UCHAR ucNobilityRank) {m_ucNobilityRank = ucNobilityRank;}
};
#include <deque>
using namespace std;
class CPlayer;
typedef deque<CEnemy* >  DEQUE_ENEMY;
#endif // !defined(AFX_ENEMY_H__CC5A9660_4AE9_429E_8E5E_6FC497EC6EEE__INCLUDED_)
