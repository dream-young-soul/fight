
//**********************************************************
// 代码编辑器
//**********************************************************

// Booth.h: interface for the CBooth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOTH_H__8DDAB97F_6B97_43F3_9EA8_8DC3E505968B__INCLUDED_)
#define AFX_BOOTH_H__8DDAB97F_6B97_43F3_9EA8_8DC3E505968B__INCLUDED_

#include "I_Role.h"
#include "NetworkDef.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGameMap;
class CBooth : IMapThing
{
protected:
    CBooth();
    virtual ~CBooth();
public:
    static CBooth*	CreateNew(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir);
protected:
    bool	Create(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir);
    IMapThing*	QueryMapThing()									{ return (IMapThing*)this; }

public:
    bool	AddItem(OBJID idItem, int nMoney);
    bool 	DelItem(OBJID idItem);
    int 	GetCost(OBJID idItem);
    bool	BuyItem(CUser* pTarget, OBJID idItem);
    void	SendGoods(CUser* pTarget);
    void	EnterMap(int nPosX, int nPosY, int nDir);
    void	LeaveMap();
    void	SetCryOut(LPCTSTR szWords)							{ SafeCopy(m_szCryOut, szWords, _MAX_WORDSSIZE); }
    void	SendCryOut(CUser* pUser);

public: // IMapThing
    virtual void	ReleaseByOwner()							{ delete this; }
    virtual OBJID	GetID()										{ return m_id; }
    virtual int 	GetObjType()								{ return OBJ_BOOTH; }
    virtual int		GetPosX()									{ return m_nPosX; }
    virtual int		GetPosY()									{ return m_nPosY; }
    virtual void	SendShow(IRole* pRole);

    // return true: ppv返回对象指针
    virtual bool	QueryObj(OBJID idObjType, void** ppv)		{ *ppv = NULL; if (idObjType == OBJ_BOOTH) { return *ppv = this, true; } return false; }
    virtual bool	QueryRole(void** ppv)						{ *ppv = NULL; return false; }

protected:
    struct	GOODS_ST
    {
        OBJID	idItem;
        int		nType;
        int		nMoney;
    };
    typedef	vector<GOODS_ST>	GOODS_SET;
    GOODS_SET		m_setGoods;

protected:
    CUser*			m_pUser;
    OBJID			m_id;
    CGameMap*		m_pMap;
    int				m_nPosX;
    int				m_nPosY;
    int				m_nDir;
    char			m_szCryOut[_MAX_WORDSSIZE];

protected: // ctrl
    PROCESS_ID		m_idProcess;
    bool			m_bEnterMap;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_BOOTH_H__8DDAB97F_6B97_43F3_9EA8_8DC3E505968B__INCLUDED_)
