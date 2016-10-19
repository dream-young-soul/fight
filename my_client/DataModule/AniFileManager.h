
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: AniFileManager.h
// Create by: Huang Yuhang
// Create on: 2003/9/24 11:25
//--------------------------------------------------------------------------------------
#ifndef _ANIFILEMANAGER_H
#define _ANIFILEMANAGER_H

#include "DataAniModule.h"
#include "AniFile.h"
#include <VECTOR>

using namespace std;
class CFrameSet  : public IFrameSet
{
public:
    CFrameSet(void) {}
    virtual ~CFrameSet(void) {this->Clear();}
private:
    vector<char*> m_setFrame;
public:
    void InsertFrame(const char* pszFrame);
    void Clear();
    unsigned long	GetFrameAmount();
    char*			GetFrameByIndex(int nIndex);
};

class CAniFileManager  : public IDai
{
public:
    CAniFileManager();
    virtual ~CAniFileManager();
public:
    unsigned long	Load(const char* pszFileName, BOOL bFroceBin);
    void			UnLoad(const char* pszFileName);
    void			Clear();
    vector<CAniFile*>	m_setAniFile;
    // for editor
public:
    unsigned long	Ani2Dai(const char* pszAniFileName, const char* pszDaiFileName, unsigned long& uLastErrorLine);
    unsigned long	Dai2Ani(const char* pszDaiFileName, const char* pszAniFileName, unsigned long& uLastErrorLine);
private:
    unsigned long	LoadAni(const char* pszFileName);
    unsigned long	LoadDai(const char* pszFileName);
public:
    unsigned long	Locate(const char* pszFileName, const char* pszindex, IFrameSet* pInfoFrame);
};

#endif
