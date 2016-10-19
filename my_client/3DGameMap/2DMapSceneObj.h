
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------
//2DMapSceneObj.h
//----------------------------------------------------

#ifndef _2DMAPSCENEOBJ_H
#define _2DMAPSCENEOBJ_H
#include "2DMapObj.h"
#include "BaseFunc.h"
//----------------------------------------------------
class C2DMapSceneObj : public C2DMapObj
{
public:
    static BOOL s_bStroke;
public:
    C2DMapSceneObj();
    ~C2DMapSceneObj();
private:
    char m_szFileName[_MAX_PATH];
    char m_szIndex[_MAX_TITLESIZE];
    CMyPos m_posOffset;

    int m_nFrameIndex;
    DWORD m_dwFrameInterval;
    DWORD m_dwTimePreFrame;
    DWORD m_dwShowWay;
    //--------------------------------------------
    // for mapobj
public:
    void Show(void* pInfo);
    void Process(void* pInfo);
    int  GetObjType   () {return MAP_SCENE;}
    BOOL IsFocus();

    //--------------------------------------------
    // additional
public:
    static C2DMapSceneObj* CreateNew(char* pszFileName, char* pszIndex);
    void   Create(char* pszFileName, char* pszIndex);

    void   LoadDataObj(FILE* fp);
    void   LoadTextObj(FILE* fp);

    void   SaveDataObj(FILE* fp);
    void   SaveTextObj(FILE* fp);

    void   SetFrameInterval(DWORD dwIntercal);
    DWORD  GetFrameInterval();

    void   SetOffset(CMyPos posOffset);
    void   GetOffset(CMyPos& posOffset);
};

//----------------------------------------------------
#endif

