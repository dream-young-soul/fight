
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 2DMapPuzzleObj.h: interface for the C2DMapPuzzleObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DMAPPUZZLEOBJ_H__022895C6_F922_49BE_B930_CA3E4AE2C1CD__INCLUDED_)
#define AFX_2DMAPPUZZLEOBJ_H__022895C6_F922_49BE_B930_CA3E4AE2C1CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DMapObj.h"
#include "PuzzleBmp.h"

class C2DMapPuzzleObj  : public C2DMapObj
{
public:
    C2DMapPuzzleObj();
    virtual ~C2DMapPuzzleObj();

    // for mapobj
public:
    void Show(void* pInfo);
    void Process(void* pInfo) {;}
    int	 GetObjType   () {return MAP_PUZZLE;}
    BOOL IsFocus() {return false;}
    void SetBase(CMySize infoSize) {;}
    void GetBase(CMySize& infoSize) {;}
    void LoadData(FILE* fp);
    void SaveData(FILE* fp);
    void LoadText(FILE* fp);
    void SaveText(FILE* fp);
    void LoadFile(char* pszFile);

private:
    char m_szFileName[_MAX_PATH];

public:
    CPuzzleBmp	m_bmpPuzzle;
};

#endif // !defined(AFX_2DMAPPUZZLEOBJ_H__022895C6_F922_49BE_B930_CA3E4AE2C1CD__INCLUDED_)
