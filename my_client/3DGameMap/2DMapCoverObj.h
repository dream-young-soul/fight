
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------
//2DMapCoverObj.h
//----------------------------------------------------

#ifndef _2DMAPCOVEROBJ_H
#define _2DMAPCOVEROBJ_H
#include "2DMapObj.h"
//----------------------------------------------------
class C2DMapCoverObj : public C2DMapObj
{
public:
    static BOOL s_bStroke;
public:
    C2DMapCoverObj();
    ~C2DMapCoverObj();

private:
    char m_szFileName[_MAX_PATH];
    char m_szIndex[_MAX_TITLESIZE];
    CMyPos m_posOffset;
    CMySize m_sizeBase;

    int m_nFrameIndex;
    DWORD m_dwFrameInterval;
    DWORD m_dwTimePreFrame;
    DWORD m_dwShowWay;
    //--------------------------------------------
    // for mapobj
public:
    void Show(void* pInfo);
    void Process(void* pInfo);
    int	 GetObjType   () {return MAP_COVER;}
    BOOL IsFocus();
    void SetBase(CMySize infoSize);
    void GetBase(CMySize& infoSize);


    //--------------------------------------------
    //additional
public:
    static C2DMapCoverObj* CreateNew(char* pszFileName, char* pszIndex);

    void   LoadDataObj(FILE* fp);
    void   LoadTextObj(FILE* fp);

    void   SaveDataObj(FILE* fp);
    void   SaveTextObj(FILE* fp);
    void   Create(char* pszFileName, char* pszIndex);

    void   SetOffset(CMyPos posOffset);
    void   GetOffset(CMyPos& posOffset);

    void   SetFrameInterval(DWORD dwIntercal);
    DWORD  GetFrameInterval();
    void   SetShowWay(DWORD dwShowWay);

    const char* GetAniTitle() {return m_szIndex;}
};

//----------------------------------------------------
#endif

