
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Fact.h: interface for the CFact class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACT_H__264FE0AB_AE25_4C9C_9271_847F2F5DF91E__INCLUDED_)
#define AFX_FACT_H__264FE0AB_AE25_4C9C_9271_847F2F5DF91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "VarType.h"
#include "myheap.h"

class CFact
{
public:
    CFact();
    CFact(bool bLogicNot, OBJID idType, VarTypeSetRef setParam)				{ Create(bLogicNot, idType, setParam); }
    /*virtual*/ ~CFact();
    //	MYHEAP_DECLARATION(s_heap)

public: // construct
    bool	Create(bool bLogicNot, OBJID idType, VarTypeSetRef setParam)	{ m_bLogicNot = bLogicNot; m_idType = idType; m_setParam = setParam; return true; }

public: // const
    bool			IsLogicNot()	const		{ return m_bLogicNot; }
    OBJID			GetType()		const		{ return m_idType; }
    VarTypeSetRef	GetParamSet()	const		{ return m_setParam; }
    VarTypeSet&		GetParamSet()				{ return m_setParam; }
    bool			IsValid()		const		{ return GetType() != ID_NONE; }

public: // dump
    String			Dump() const
    {
        String	str;
        if (m_bLogicNot)
        {
            str	+= '!';
        }
        str	+=	String().FormatLong(m_idType);
        str	+= '(';
        for(int i = 0; i < m_setParam.Size(); i++)
        {
            if (i) { str += ","; }
            if (!m_setParam[i].IsValid())
            {
                str += String("(null)");
            }
            else if (m_setParam[i].IsIntType())
            {
                str	+= String().FormatLong(static_cast<int>(m_setParam[i]));
            }
            else
            {
                str	+= static_cast<String>(m_setParam[i]);
            }
        }
        str	+= ')';
        return str;
    }

protected:
    bool		m_bLogicNot;
    OBJID		m_idType;
    VarTypeSet	m_setParam;
};
template<>
struct dump_traits<CFact> { static String Dump(const CFact& obj)		{ return obj.Dump(); } };

typedef	const CFact&	CFactRef;
typedef	Array<CFact>	CFactArray;
typedef	const CFactArray&	CFactArrayRef;

#endif // !defined(AFX_FACT_H__264FE0AB_AE25_4C9C_9271_847F2F5DF91E__INCLUDED_)
