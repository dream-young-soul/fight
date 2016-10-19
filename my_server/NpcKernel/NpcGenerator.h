
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcGenerator.h: interface for the CNpcGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCGENERATOR_H__66F1D8C3_7833_4C77_AD36_F07544F0D2DE__INCLUDED_)
#define AFX_NPCGENERATOR_H__66F1D8C3_7833_4C77_AD36_F07544F0D2DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "GameData.h"
#include "Timeout.h"

//////////////////////////////////////////////////////////////////////
class IGenerator
{
protected:
    virtual ~IGenerator() {}
public:
    virtual	OBJID	GetID()											PURE_VIRTUAL_FUNCTION_0
    virtual	void	Release()										PURE_VIRTUAL_FUNCTION
    virtual POINT	GetCenter()										PURE_VIRTUAL_FUNCTION_(POINT)
    virtual bool	IsTooFar(int x, int y, int nRange)				PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsInRegion(int x, int y)						PURE_VIRTUAL_FUNCTION_0
    virtual int		GetWidth()										PURE_VIRTUAL_FUNCTION_0
    virtual int		GetHeight()										PURE_VIRTUAL_FUNCTION_0
    virtual int		GetPosX()										PURE_VIRTUAL_FUNCTION_0
    virtual int		GetPosY()										PURE_VIRTUAL_FUNCTION_0
    virtual void	GeneratorOK(OBJID idNpc)						PURE_VIRTUAL_FUNCTION
    virtual void	DeleteNpc(OBJID idNpc)							PURE_VIRTUAL_FUNCTION
};

//////////////////////////////////////////////////////////////////////
enum NPCGENDATA
{
    NPCGENDATA_MAPID = 1,
    NPCGENDATA_BOUNDX,
    NPCGENDATA_BOUNDY,
    NPCGENDATA_BOUNDCX,
    NPCGENDATA_BOUNDCY,

    NPCGENDATA_GRID,				// NPC的间隔
    NPCGENDATA_RESTSECS,			// 再生间隔秒数
    NPCGENDATA_MAXPERGEN,			// 最次最多再生数量

    NPCGENDATA_NPCTYPE,

    NPCGENDATA_BEGIN,
    NPCGENDATA_END,
    NPCGENDATA_BORNX,
    NPCGENDATA_BORNY,
};

char	szGenerator[];
typedef CGameData<NPCGENDATA, szGenerator, szID>	CNpcGenData;

//////////////////////////////////////////////////////////////////////
class CNpcGenerator : public CGameObj, IGenerator
{
public:
    CNpcGenerator();
    virtual ~CNpcGenerator();
    static CNpcGenerator*	CreateNew()	{ return new CNpcGenerator; }
    virtual ULONG	ReleaseByOwner()	{ delete this; return 0; }
    IGenerator*		QueryInterface()	{ return (IGenerator*)this; }

    virtual OBJID	GetID()						{ return m_pData->GetKey(); }
protected: // IGenerator
    virtual void	Release()					{}		// release by npc, no operate
    virtual POINT	GetCenter()
    {
        POINT pos;
        pos.x = GetInt(NPCGENDATA_BOUNDX) + GetInt(NPCGENDATA_BOUNDCX) / 2;
        pos.y = GetInt(NPCGENDATA_BOUNDY) + GetInt(NPCGENDATA_BOUNDCY) / 2;
        return pos;
    }
    virtual bool	IsTooFar(int x, int y, int nRange)
    {
        return !(x >= GetInt(NPCGENDATA_BOUNDX) - nRange
                 && x < GetInt(NPCGENDATA_BOUNDX) + GetInt(NPCGENDATA_BOUNDCX) + nRange
                 && y >= GetInt(NPCGENDATA_BOUNDY) - nRange
                 && y < GetInt(NPCGENDATA_BOUNDY) + GetInt(NPCGENDATA_BOUNDCY) + nRange);
    }
    virtual bool	IsInRegion(int x, int y)
    {
        return x >= GetInt(NPCGENDATA_BOUNDX) && x < GetInt(NPCGENDATA_BOUNDX) + GetInt(NPCGENDATA_BOUNDCX)
               && y >= GetInt(NPCGENDATA_BOUNDY) && y < GetInt(NPCGENDATA_BOUNDY) + GetInt(NPCGENDATA_BOUNDCY);
    }
    virtual int		GetWidth()					{ return m_pData->GetInt(NPCGENDATA_BOUNDCX); }
    virtual int		GetHeight()					{ return m_pData->GetInt(NPCGENDATA_BOUNDCY); }
    virtual int		GetPosX()					{ return m_pData->GetInt(NPCGENDATA_BOUNDX); }
    virtual int		GetPosY()					{ return m_pData->GetInt(NPCGENDATA_BOUNDY); }
    virtual void	GeneratorOK(OBJID idNpc);		// 2nd step, call from CMsgAiNpcInfo
    virtual void	DeleteNpc(OBJID idNpc);			// 3th step, dec npc count

public: // interface
    virtual bool	Create(IRecordset* pRes);

    int		Generator(int nNum);			// return : Generator num, 1st step

public:
    int		GetInt(NPCGENDATA idx)		{ return m_pData->GetInt(idx); }
    LPCTSTR	GetStr(NPCGENDATA idx)		{ return m_pData->GetStr(idx); }
    int		GetAmount()					{ return m_nAmount; }
    int		GetGenAmount()				{ return m_nGenAmount; }
    int		GetMaxNpc()					{ return m_nMaxNpc; }


protected:
    POINT	FindGenPos();

protected:
    int			m_nAmount;						// 与NPC中的m_pGenerator同步。
    int			m_nGenAmount;					// 申请创建了只个。当NPC死亡时将置为与m_nAmount相同。
    int			m_nCurrGen;						// 当前还剩几只需要再生
    CTimeOut	m_timeoutRest;
    int			m_idxLastGen;
    int			m_nMaxNpc;
    CGameMap*	m_pMap;

protected: // database
    CNpcGenData*		m_pData;
};

#endif // !defined(AFX_NPCGENERATOR_H__66F1D8C3_7833_4C77_AD36_F07544F0D2DE__INCLUDED_)
