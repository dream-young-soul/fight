
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DynaNpcGen.h: interface for the CDynaNpcGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DYNANPCGEN_H__BBC14A3A_D833_4059_A3FD_016B3B7A55E2__INCLUDED_)
#define AFX_DYNANPCGEN_H__BBC14A3A_D833_4059_A3FD_016B3B7A55E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NpcGenerator.h"
#include "mycom.h"

class CDynaNpcGen : public IGenerator
{
    NO_COPY(CDynaNpcGen)
protected:
    CDynaNpcGen();
    virtual ~CDynaNpcGen();
public:
    static CDynaNpcGen* CreateNew(IGenerator* pGen, OBJID idMap, int x, int y);

protected: // IGenerator
    virtual	OBJID	GetID()											{ return 1234567890; }		// for log
    virtual	void	Release()										{ delete this; }
    virtual POINT	GetCenter()										{ POINT pos; pos.x = m_pos.x + GetWidth() / 2; pos.y = m_pos.y + GetHeight() / 2; return pos; }
    virtual bool	IsTooFar(int x, int y, int nRange)
    {
        return !(x >= GetPosX() - nRange && x < GetPosX() + GetWidth() + nRange
                 && y >= GetPosY() - nRange && y < GetPosY() + GetHeight() + nRange);
    }
    virtual bool	IsInRegion(int x, int y)						{ return x >= GetPosX() && x < GetPosX() + GetWidth() && y >= GetPosY() && y < GetPosY() + GetHeight(); }
    virtual int		GetWidth()										{ return m_pGen->GetWidth(); }
    virtual int		GetHeight()										{ return m_pGen->GetHeight(); }
    virtual int		GetPosX()										{ return m_pos.x; }
    virtual int		GetPosY()										{ return m_pos.y; }
    virtual void	GeneratorOK(OBJID idNpc)						{}
    virtual void	DeleteNpc(OBJID idNpc)							{}

protected:
    IGenerator*	m_pGen;
    OBJID		m_idMap;
    POINT		m_pos;
};

#endif // !defined(AFX_DYNANPCGEN_H__BBC14A3A_D833_4059_A3FD_016B3B7A55E2__INCLUDED_)
