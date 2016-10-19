
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ImgNumCounter.cpp: implementation of the CImgNumCounter class.
//
//////////////////////////////////////////////////////////////////////

#include "ImgNumCounter.h"

// globle...
CImgNumCounter g_objImgNumCounter;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImgNumCounter::CImgNumCounter()
{
    m_nNumber		= 0;
    m_nStep			= 0;
    m_dwBeginTime	= 0;
    m_dwInterval	= 0;
}

CImgNumCounter::~CImgNumCounter()
{
}

//////////////////////////////////////////////////////////////////////
void CImgNumCounter::Start	(int nNumber, int nStep/*=-1*/, DWORD dwInterval/*=1000*/)
{
    MYASSERT (nNumber > 0);
    m_nNumber		= nNumber;
    m_nStep			= nStep;
    m_dwInterval	= dwInterval;
    m_dwBeginTime	= ::TimeGet();
}

//////////////////////////////////////////////////////////////////////
void CImgNumCounter::End(void)
{
    m_dwBeginTime	= 0;
}
//////////////////////////////////////////////////////////////////////
void CImgNumCounter::Reset (void)
{
    m_dwBeginTime	= ::TimeGet();
}

//////////////////////////////////////////////////////////////////////
BOOL CImgNumCounter::Show (void)
{
    if (m_nNumber == 0 || m_dwBeginTime <= 0 || m_dwInterval <= 0)
    {
        return true;
    }
    // show...
    int nPassedStep = (::TimeGet() - m_dwBeginTime) / m_dwInterval;
    int nShowNum	= m_nNumber + m_nStep * nPassedStep;
    if (nShowNum < 0)
    {
        return true;
    }
    char szStrShow[256] = "";
    sprintf(szStrShow, "%d", nShowNum);
    m_objImgString.SetString(szStrShow);
    m_objImgString.Show();
    return (nShowNum <= 0);
}
