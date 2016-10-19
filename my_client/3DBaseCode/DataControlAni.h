
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef DATACONTROLANI_HEADER_KATTY
#define DATACONTROLANI_HEADER_KATTY

#include <windows.h>
#include <deque>
#include "basefunc.h"
using namespace std;

typedef struct OFFSET
{
    int* nOffsetX;
    int* nOffsetY;
    BYTE  nFrame;
    char  szSection[32];
} strOFFSET;

typedef deque<OFFSET*> OFFSETDEQUE;


class CDataControlAni
{

public:
    CDataControlAni() {}
    ~CDataControlAni() {Destroy();}
    BOOL Create(const char* pszDataFile);
    void Destroy();
    void Show(int nFrame, int nCurAction, int nX, int nY, int nAlpha);
    DWORD GetBornTime() {return m_dwBornTime;}
    void SetBornTime(DWORD dwTime) { m_dwBornTime = dwTime;}
    int  GetControlFrame(int nAction);
    BOOL CheckByFileName(char* szFilename) {return _strcmpi(m_szDataFilename, szFilename) == 0;}
private:
    char m_szAniFilename[256];
    char m_szDataFilename[256];
    char m_bCanPlayRand;
    OFFSETDEQUE m_dequeAction;
    DWORD m_dwBornTime ;

};


#endif
