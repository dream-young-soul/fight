
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: AniFileManager.cpp
// Create by: Huang Yuhang
// Create on: 2003/9/24 11:25
//--------------------------------------------------------------------------------------
#pragma warning(disable:4786)
#include "AniFileManager.h"
#include "AniFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniFileManager::CAniFileManager()
{
}

CAniFileManager::~CAniFileManager()
{
    this->Clear();
}

//--------------------------------------------------------------------------------------
unsigned long	CAniFileManager::Ani2Dai(const char* pszAniFileName, const char* pszDaiFileName, unsigned long& uLastErrorLine)
{
    if (!pszAniFileName && !pszDaiFileName)
    {
        return CODE_EXTERIOR_ERROR;
    }
    uLastErrorLine = 0;
    CAniFile objAniFile;
    unsigned long uCode;
    uCode = objAniFile.LoadAniByOrder(pszAniFileName);
    if (uCode != SUCCESS)
    {
        uLastErrorLine = objAniFile.GetLastErrorLine();
        return uCode;
    }
    uCode = objAniFile.SaveAni2Dai(pszDaiFileName);
    if (uCode != SUCCESS)
    {
        uLastErrorLine = objAniFile.GetLastErrorLine();
    }
    return uCode;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFileManager::Dai2Ani(const char* pszDaiFileName, const char* pszAniFileName, unsigned long& uLastErrorLine)
{
    if (!pszAniFileName && !pszDaiFileName)
    {
        return CODE_EXTERIOR_ERROR;
    }
    uLastErrorLine = 0;
    CAniFile objAniFile;
    unsigned long uCode;
    uCode = objAniFile.LoadDaiByOrder(pszDaiFileName);
    if (uCode != SUCCESS)
    {
        uLastErrorLine = objAniFile.GetLastErrorLine();
        return uCode;
    }
    uCode = objAniFile.SaveDai2Ani(pszAniFileName);
    if (uCode != SUCCESS)
    {
        uLastErrorLine = objAniFile.GetLastErrorLine();
    }
    return uCode;
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFileManager::Load(const char* pszFileName, BOOL bFroceBin)
{
    char szFileNameCopy[_MAX_PATH];
    strcpy(szFileNameCopy, pszFileName);
    if (bFroceBin)
    {
        int nStrlen = strlen(szFileNameCopy);
        szFileNameCopy[nStrlen-3] = 'D';
        szFileNameCopy[nStrlen-2] = 'A';
        szFileNameCopy[nStrlen-1] = 'I';
    }
    if (!pszFileName)
    {
        return CODE_EXTERIOR_ERROR;
    }
    int nAmount = m_setAniFile.size();
    // assay file name ...
    char szDir[_MAX_DIR]		=	"";
    char szDrive[_MAX_PATH]		=	"";
    char szFileName[_MAX_FNAME]	=	"";
    char szExt[_MAX_EXT]		=	"";
    _splitpath(szFileNameCopy, szDrive, szDir, szFileName, szExt);
    _strupr(szDir);
    _strupr(szFileName);
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CAniFile* pAniFile = m_setAniFile[i];
        if (pAniFile && pAniFile->CheckFile(szDir, szFileName))
        {
            return IDai::SUCCESS;
        }
    }
    CAniFile* pAniFile = new CAniFile;
    if (!pAniFile)
    {
        return CODE_INTERIOR_ERROR;
    }
    unsigned long uCode = pAniFile->Load(szFileNameCopy);
    if (uCode != IDai::SUCCESS)
    {
        delete(pAniFile);
    }
    else
    {
        m_setAniFile.push_back(pAniFile);
    }
    return uCode;
}
//--------------------------------------------------------------------------------------
void		CAniFileManager::UnLoad(const char* pszFileName)
{
    if (!pszFileName)
    {
        return;
    }
    int nAmount = m_setAniFile.size();
    // assay file name ...
    char szDir[_MAX_DIR]		=	"";
    char szDrive[_MAX_PATH]		=	"";
    char szFileName[_MAX_FNAME]	=	"";
    char szExt[_MAX_EXT]		=	"";
    _splitpath(pszFileName, szDrive, szDir, szFileName, szExt);
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CAniFile* pAniFile = m_setAniFile[i];
        if (pAniFile && pAniFile->CheckFile(szDir, szFileName))
        {
            delete pAniFile;
        }
        m_setAniFile.erase(m_setAniFile.begin() + i);
    }
}
//--------------------------------------------------------------------------------------
void		CAniFileManager::Clear()
{
    int nAmount = m_setAniFile.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CAniFile* pAniFile = m_setAniFile[i];
        if (pAniFile)
        {
            delete pAniFile;
        }
    }
    m_setAniFile.clear();
}
//--------------------------------------------------------------------------------------
unsigned long	CAniFileManager::Locate(const char* pszFileName, const char* pszindex, IFrameSet* pInfoFrame)
{
    if (!pszFileName || !pszindex)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    if (!pInfoFrame)
    {
        return IDai::CODE_INTERIOR_ERROR;
    }
    int nAmount = m_setAniFile.size();
    // assay file name ...
    char szDir[_MAX_DIR]		=	"";
    char szDrive[_MAX_PATH]		=	"";
    char szFileName[_MAX_FNAME]	=	"";
    char szExt[_MAX_EXT]		=	"";
    _splitpath(pszFileName, szDrive, szDir, szFileName, szExt);
    _strupr(szDir);
    _strupr(szFileName);
    for(int i = nAmount - 1; i >= 0; i --)
    {
        CAniFile* pAniFile = m_setAniFile[i];
        if (pAniFile && pAniFile->CheckFile(szDir, szFileName))
        {
            // find
            unsigned long uCode = pAniFile->GetAni(pszindex, pInfoFrame);
            return uCode;
        }
    }
    return CODE_INFO_NOTFOUND;
}
//--------------------------------------------------------------------------------------
void CFrameSet::InsertFrame(const char* pszFrame)
{
    if (!pszFrame)
    {
        return;
    }
    int nLength = strlen(pszFrame);
    char* pszNewFrame = new char[nLength+1];
    if (!pszNewFrame)
    {
        return;
    }
    strcpy(pszNewFrame, pszFrame);
    m_setFrame.push_back(pszNewFrame);
}
//--------------------------------------------------------------------------------------
void CFrameSet::Clear()
{
    int nAmount = m_setFrame.size();
    for(int i = 0; i < nAmount; i ++)
    {
        char* pszFrame = m_setFrame[i];
        if (pszFrame)
        {
            delete[] pszFrame;
        }
    }
    m_setFrame.clear();
}
//--------------------------------------------------------------------------------------
unsigned long	CFrameSet::GetFrameAmount()
{
    return m_setFrame.size();
}
//--------------------------------------------------------------------------------------
char*			CFrameSet::GetFrameByIndex(int nIndex)
{
    int nAmount = m_setFrame.size();
    if (nIndex < 0 && nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setFrame[nIndex];
}
//--------------------------------------------------------------------------------------
