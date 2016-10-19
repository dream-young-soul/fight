
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Path.h: interface for the CPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATH_H__B9591737_CA9C_42CD_AE06_A0F3D5BD89D5__INCLUDED_)
#define AFX_PATH_H__B9591737_CA9C_42CD_AE06_A0F3D5BD89D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapObj.h"

#include <deque>
#include <vector>
using namespace std;
typedef struct
{
    char cStepDir;
    unsigned char ucStepLength;
} StepInfo;
typedef deque<StepInfo>  DEQUE_STEP;
typedef vector<StepInfo> VECTOR_STEP;
enum {_ASTAR_NONE = 0, _ASTAR_CLOSE, _ASTAR_LEAVE};
class CPath
{
public:
    CPath();
    virtual ~CPath();

private:
    DEQUE_STEP	m_setStep;
	
    CMapObj*	m_pMapObj;
    int			m_nSearchMethod;
    int			m_nData;
    BOOL		m_bFound;

public:
    void	ClearStep();

    BOOL	GetNextStep(StepInfo& infoStep);
    void	AddStep(char cStep, unsigned char ucStepLength);

    int		TestNextStep();

    int		GetStepAmount();
    void	SetPlayer(CMapObj* pMapObj);
    void	SetSearchMethod(int nMethod);
    int		GetSearchMethod();
    void	SetPathData(int nData);
    int		GetData();
    int		IsReachable() {return m_bFound;}
    void	SetReachable(BOOL bReachable) {m_bFound = bReachable;}
};

#endif // !defined(AFX_PATH_H__B9591737_CA9C_42CD_AE06_A0F3D5BD89D5__INCLUDED_)
