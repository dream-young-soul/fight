
//**********************************************************
// 代码编辑器
//**********************************************************

// StrRes.cpp: implementation of the CStrRes class.
//
//////////////////////////////////////////////////////////////////////

#include "StrRes.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrRes::CStrRes()
{
    m_setStr.clear();
}

CStrRes::~CStrRes()
{
}
//-------------------------------------------------------------------
void CStrRes::Init()
{
    const char	szFile[] = "ini/StrRes.ini";
    FILE* fp = fopen(szFile, "r");
    if (!fp)
    {
        ::ErrorOut("can't open file:%s！", szFile);
        return;
    }
    this->Destroy();
    char szLine[1024] = "";
    char szLineCopy[1024] = "";
    while(true)
    {
        if (NULL == fgets(szLine, 1023, fp))
        {
            break;
        }
        if (strlen(szLine) == 0)
        {
            continue;
        }
        if (szLine[0] == ';')
        {
            continue;
        }
        if (szLine[strlen(szLine)-1] == 10)
        {
            szLine[strlen(szLine)-1] = '\0';
        }
        OBJID id = ID_NONE;
        static char szFile[1024]	= "";
        if (2 == sscanf(szLine, "%u=%s", &id, szFile))
        {
            // 搜索第一个'='的位置
            int nLength = strlen(szLine);
            int nPos = 0;
            for(int i = 0; i < nLength; i ++)
            {
                if (szLine[i] == '=')
                {
                    nPos = i + 1;
                    break;
                }
            }
            // 从nPos开始拷贝
            int j = 0;
            for(; j < nLength - nPos; j ++)
            {
                szLineCopy[j] = szLine[nPos+j];
            }
            szLineCopy[j] = '\0';
            char* pszStr = new char[strlen(szLineCopy)+1];
            strcpy(pszStr, szLineCopy);
            m_setStr[id] = pszStr;
        }
    }
    fclose(fp);
}
//-------------------------------------------------------------------
void CStrRes::Destroy()
{
    DWORD dwAmount = m_setStr.size();
    map<OBJID, char*>::iterator iter = m_setStr.begin();
    for(DWORD dwCounter = 0; dwCounter < dwAmount; dwCounter++)
    {
        char* pszStr = iter->second;
        iter++;
        SAFE_DELETE(pszStr);
    }
    m_setStr.clear();
}
//-------------------------------------------------------------------
const char*	CStrRes::GetStr(OBJID idRes)
{
    map<OBJID, char*>::iterator iter = m_setStr.find(idRes);
    if (iter != m_setStr.end())
    {
        return iter->second;
    }
    return g_szStrResError;
}
//-------------------------------------------------------------------
