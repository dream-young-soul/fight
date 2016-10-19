
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Path.cpp: implementation of the CPath class.
//
//////////////////////////////////////////////////////////////////////

#include "Path.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPath::CPath()
{
    m_setStep.clear();
    m_nSearchMethod = _ASTAR_CLOSE;
    m_nData = 15;
}

CPath::~CPath()
{
    m_setStep.clear();
}

//--------------------------------------------------------------------
int CPath::TestNextStep()
{
    if (m_setStep.size() > 0)
    {
		return m_setStep[0].cStepDir % 8;
    }
	return -1; 
}
//--------------------------------------------------------------------
BOOL  CPath::GetNextStep(StepInfo& infoStep)
{
    if (m_setStep.size() > 0)
    {
		infoStep = m_setStep[0];
		m_setStep.pop_front();
		return true;
    }
    return false;
}
//--------------------------------------------------------------------
void CPath::ClearStep()
{
    m_setStep.clear();
}
//--------------------------------------------------------------------
void CPath::AddStep(char cStep, unsigned char ucStepLength)
{
    StepInfo infoStep;
	infoStep.cStepDir = cStep;
	infoStep.ucStepLength = ucStepLength;
    m_setStep.push_back(infoStep);
}
//--------------------------------------------------------------------
int  CPath::GetStepAmount()
{
    return m_setStep.size();
}
//--------------------------------------------------------------------
void CPath::SetPlayer(CMapObj* pMapObj)
{
    m_pMapObj = pMapObj;
}
//--------------------------------------------------------------------
void CPath::SetSearchMethod(int nMethod)
{
    m_nSearchMethod = nMethod;
}
//--------------------------------------------------------------------
int  CPath::GetSearchMethod()
{
    return m_nSearchMethod;
}
//--------------------------------------------------------------------
void CPath::SetPathData(int nData)
{
    m_nData = nData;
}
//--------------------------------------------------------------------
int	CPath::GetData()
{
    return m_nData;
}
//--------------------------------------------------------------------
