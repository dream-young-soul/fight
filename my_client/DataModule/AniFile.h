
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: AniFile.h
// Create by: Huang Yuhang
// Create on: 2003/9/24 14:39
//--------------------------------------------------------------------------------------
#ifndef _ANIFILE_H
#define _ANIFILE_H

#include <DEQUE>
#include <MAP>
#include <VECTOR>
#include "DataAniModule.h"
using namespace std;

class CAniFile
{
public:
    CAniFile();
    virtual ~CAniFile();

private:
    // 文件索引信息
    typedef struct
    {
        char	szPath	[_MAX_DIR];
        char	szFile	[_MAX_FNAME];
    } AniFileIndex;

    // Ani动画
    typedef struct
    {
        deque<char*>		setFrame;
    } AniInfo;

    typedef struct
    {
        char*				pszIndex;
        deque<char*>		setFrame;
    } AniInfoByOrder;

    // AniFile
    typedef struct
    {
        map<unsigned long, AniInfo*> setAniInfo;
    } AniFileInfo;

    // AniFileByOrder
    typedef struct
    {
        deque<AniInfoByOrder*> setAniInfo;
    } AniFileInfoByOrder;


    // Dai帧信息
    typedef struct
    {
        unsigned short		usPath;
        unsigned char		ucExtendName;
        unsigned short		usFileNameIndex;
        char				cFileNameAdjust1;
        char				cFileNameAdjust2;
    } DaiFrameInfo;

    // Dai动画
    typedef struct
    {
        unsigned char		ucFrameAmount;
        unsigned short		usIndexNameIndex;
        char				cIndexNameAdjust1;
        char				cIndexNameAdjust2;
        DaiFrameInfo*		pFrameInfo;
    } DaiInfo;

    // DaiFile
    typedef struct
    {
        deque<char*>			setStr;
        deque<char*>			setExt;
        deque<char*>			setDir;
        map<unsigned long, DaiInfo*>	setDaiInfo;
    } DaiFileInfo;

    // DaiFileByOrder
    typedef struct
    {
        deque<char*>			setStr;
        deque<char*>			setExt;
        deque<char*>			setDir;
        deque<DaiInfo*>			setDaiInfo;
    } DaiFileInfoByOrder;

private:
    AniFileInfo*			m_pAniFile;
    DaiFileInfo*			m_pDaiFile;
    AniFileInfoByOrder*		m_pAniFileByOrder;
    DaiFileInfoByOrder*		m_pDaiFileByOrder;
    unsigned long			m_uLastErrorLine;
    AniFileIndex			m_infoIndex;

    // for id generator
    unsigned		m_idGen[70];

private:
    void			Clear();
    unsigned long	LoadAni(const char* pszFile);
    unsigned long	LoadDai(const char* pszFile);
    unsigned long	Str2Id(const char* pszStr);
    unsigned long	ZipStr(deque<char*>& setStr, const char* pszStr, unsigned short& usStrIndex, char& cAdjustA, char& cAdjustB);
    unsigned long	ZipFrame(deque<char*>& setStr, deque<char*>& setExt, deque<char*>& setDir,
                             const char* pszFrame, unsigned short& usPathIndex, unsigned char& ucExtIndex,
                             unsigned short& usNameIndex, char& cNameAdjustA, char& cNameAdjustB);
    unsigned long	UnZipStr(deque<char*>& setStr, char* pszBuffer, unsigned long uBufferSize,
                             unsigned short& usStrIndex, char& cAdjustA, char& cAdjustB);
    unsigned long	UnZipFrame(deque<char*>& setStr, deque<char*>& setExt, deque<char*>& setDir,
                               char* pszBuffer, unsigned long dwBufferSize, unsigned short& usPathIndex, unsigned char& ucExtIndex,
                               unsigned short& usNameIndex, char& cNameAdjustA, char& cNameAdjustB);

public:
    unsigned long	Load(const char* pszFile);
    unsigned long	UnLoad();
    unsigned long	GetLastErrorLine() {return m_uLastErrorLine;}

public:
    unsigned long	LoadAniByOrder(const char* pszFile);
    unsigned long	LoadDaiByOrder(const char* pszFile);
    unsigned long	SaveDai2Ani(const char* pszFile);
    unsigned long	SaveAni2Dai(const char* pszFile);
    unsigned long	GetAni(const char* pszIndex, IFrameSet* pInfoFrame);
    bool			CheckFile(const char* pszDir, const char* pszFileName);
};

#endif
