
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ImgNumCounter.h: interface for the CImgNumCounter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGNUMCOUNTER_H__82CC27B5_6D53_4F12_9107_176944BCD986__INCLUDED_)
#define AFX_IMGNUMCOUNTER_H__82CC27B5_6D53_4F12_9107_176944BCD986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageString.h"

class CImgNumCounter
{
public:
    CImgNumCounter();
    virtual ~CImgNumCounter();

    void	Start		(int nNumber, int nStep = -1, DWORD dwInterval = 1000);
    void	Reset		(void);

    void	Scale		(int nPercent)		{ m_objImgString.SetScalePercent(nPercent); }
    void	SetShowPos	(CMyPos posShow)	{ m_objImgString.SetShowPos(posShow); }
    BOOL	Show		(void);	// return true means counter over
    void	End			(void);
private:
    int		m_nNumber, m_nStep;
    DWORD	m_dwBeginTime;
    DWORD	m_dwInterval;

    CImageString	m_objImgString;
};

extern CImgNumCounter g_objImgNumCounter;

#endif // !defined(AFX_IMGNUMCOUNTER_H__82CC27B5_6D53_4F12_9107_176944BCD986__INCLUDED_)
