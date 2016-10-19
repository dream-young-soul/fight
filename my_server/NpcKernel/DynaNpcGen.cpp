
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DynaNpcGen.cpp: implementation of the CDynaNpcGen class.
//
//////////////////////////////////////////////////////////////////////

#include "DynaNpcGen.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDynaNpcGen::CDynaNpcGen()
{
}

//////////////////////////////////////////////////////////////////////
CDynaNpcGen::~CDynaNpcGen()
{
}

//////////////////////////////////////////////////////////////////////
CDynaNpcGen* CDynaNpcGen::CreateNew(IGenerator* pGen, OBJID idMap, int x, int y)
{
    CDynaNpcGen* pObj = new CDynaNpcGen;
    CHECKF(pObj);
    pObj->m_pGen		= pGen;
    pObj->m_idMap		= idMap;
    pObj->m_pos.x		= x;
    if (x > pGen->GetWidth() / 2)
    {
        pObj->m_pos.x		= x - pGen->GetWidth() / 2;
    }
    pObj->m_pos.y		= y;
    if (y > pGen->GetHeight() / 2)
    {
        pObj->m_pos.y		= y - pGen->GetHeight() / 2;
    }
    return pObj;
}

