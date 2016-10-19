
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef CLOUDDEFINE_HEADER
#define CLOUDDEFINE_HEADER

#include <windows.h>
#include <Windef.h>
#include <deque>
#include "ani.h"

using namespace std;

enum {CLOUD_NONE, CLOUD_CREATE, CLOUD_RESET, CLOUD_STOP, CLOUD_PAUSE};
const int  MAXCLOUD = 30;

struct CLOUDOFF
{
    char nSportDirX;
    char nSportDirY;
    char nCurSport;
    char AlphaChange;
    WORD nSportRadian;
    int  nCurAlpha;
    int  nCurFrame;

    int nWorldX;
    int nWorldY;
};

class CCloud
{
public:

    CCloud() {}
    ~CCloud() {Destroy();}
    BOOL Create(int nCloud0, int nCloud1, int nCloud2, int nCloud3);
    void Destroy() {}
    void Process();

private:

    int			m_nCloudResource[4];
    CLOUDOFF	m_infoCloud[4];
    BYTE		m_bCount;

};

typedef deque<CCloud* > CLOUDDEQUE;

class CCloudSys
{
public:
    CCloudSys() {m_bStatus = CLOUD_NONE;}
    ~CCloudSys() {Destroy();}
    BOOL Create(int nCloudNum);
    void Destroy();
    void Process(int& nStatus);
    void Stop();
    void ReSet(int inum);
    void Pause();
    void Continue();
private:
    CLOUDDEQUE	m_dequeCloud;
    BYTE		m_bStatus;
    BYTE		m_nCurAmound;
    BYTE		m_nTargetAmound;
};

#endif
