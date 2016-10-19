
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************


#ifndef FIREBUG_HEAD_KATTY
#define FIREBUG_HEAD_KATTY

#include <windows.h>
#include <Windef.h>
#include "Ani.h"
#include <deque>
using namespace std;

enum {_FIREBUGSTATUS_NONE	= 0, _FIREBUGSTATUS_CREATE, _FIREBUGSTATUS_STOP, _FIREBUGSTATUS_BEGIN, FIREBUGSTATUS_PAUSE, FIREBUGSTATUS_RESET};


class CFireBug
{
public:
    CFireBug() { m_iStatus	= _FIREBUGSTATUS_NONE;}
    ~CFireBug()	{Destroy();}
    void Initial();
    void  InitialLayer();
    BOOL	Create	( int nCurFrame);
    void	Show	(void* pInfo);
    void	Destroy	(void);
    BOOL	IsCreated	(void) {if (m_iStatus) { return TRUE; } else { return FALSE; }}
    int GetY(int nx);

private:
    int		m_nDirChanged;
    int		m_iStatus;
    POINT	m_nCurWorldPoint;
    POINT	m_nShowWorldPoint;
    int		m_nLayer;
    int		m_nCurState;
    int		m_nCurFrame;
    int		m_nCurX;
    int		m_nCurY;
    int		m_nSpeed;
    int		m_nLoopCount;

};



typedef deque<CFireBug*> DequeCFireBug;
class CSysFireBug
{
public:

    CSysFireBug(void)	{m_nCreated = 0; m_iStatu = _FIREBUGSTATUS_NONE; }
    ~CSysFireBug(void) {Destroy();}

    BOOL Create	( int nNum, DWORD dwDelayTime);
    void Process(int& nStatus);
    void Destroy();
    void Stop();
    void ReSet(int inum);
    void Pause();
    void Continue();


private:
    int			m_iCurNum;
    int			m_iTargetNum;
    int			m_iStatu;
    DWORD		m_dwCreateTime;
    DWORD		m_dwDelayTime;
    DWORD		m_dwBeginTime;
    int			m_nSpeed;
    DequeCFireBug		m_dequeFlyBug;

public:
    int m_nCreated;

};

#endif
