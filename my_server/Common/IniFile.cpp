
//**********************************************************
// 代码编辑器
//**********************************************************

// INI文件
// 仙剑修，2001.11.19


#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "IniFile.h"
#include "logfile.h"
#include "common.h"

CIniFile::CIniFile(const char* pFilename, const char* pSection)
{
    if (pFilename && strlen(pFilename) < INIWORDSIZE)
    {
        strcpy(m_bufFilename, pFilename);
    }
    else
    {
        m_bufFilename[0] = 0;
        LOGERROR("CIniFile::CIniFile() parameter error.");
    }
    if (pSection && strlen(pSection) < INIWORDSIZE)
    {
        strcpy(m_bufSection, pSection);
    }
    else
    {
        m_bufSection[0] = 0;
        LOGERROR("CIniFile::CIniFile() parameter error.");
    }
}

CIniFile::~CIniFile()
{
}

void	CIniFile::SetSection(const char* pSect)
{
    if (pSect && strlen(pSect) < INIWORDSIZE)
    {
        strcpy(m_bufSection, pSect);
    }
    else
    {
        m_bufSection[0] = 0;
        LOGERROR("CIniFile::SetSection()  parameter error.");
    }
}

bool	CIniFile::GetString(char* bufString, const char* pKey, int nBufSize)
{
    if (bufString)
    {
        bufString[0] = 0;
    }
    else
    {
        LOGERROR("CIniFile::GetString() parameter error.");
        return false;
    }
    int	nSectLen = strlen(m_bufSection);
    ASSERT(nSectLen < INIWORDSIZE);
    int	nKeyLen = strlen(pKey);
    ASSERT(nKeyLen < INIWORDSIZE);
    FILE* pFile = fopen(m_bufFilename, "r");
    if (nSectLen && nKeyLen && pFile)
    {
        char buf[INIWORDSIZE+INIDATASIZE];
        do
        {
            fgets(buf, INIWORDSIZE + INIDATASIZE - 1, pFile);
            buf[INIWORDSIZE+INIDATASIZE-1] = 0;
            if (buf[0] == '[' && buf[nSectLen+1] == ']' && strnicmp(buf + 1, m_bufSection, nSectLen) == 0)
            {
                do
                {
                    fgets(buf, INIWORDSIZE + INIDATASIZE - 1, pFile);
                    buf[INIWORDSIZE+INIDATASIZE-1] = 0;
                    if (buf[0] && buf[0] != ';'
                            && (buf[nKeyLen] == '=' || buf[nKeyLen] == ' ' || buf[nKeyLen] == '\t')
                            && strnicmp(buf, pKey, nKeyLen) == 0)
                    {
                        int		pos = nKeyLen;
                        while(buf[pos] == ' ' || buf[pos] == '\t')		// 去掉前空
                        {
                            pos++;
                        }
                        if (buf[pos] == '=')		// 去掉'='
                        {
                            pos++;
                        }
                        while(buf[pos] == ' ' || buf[pos] == '\t')		// 去掉前空
                        {
                            pos++;
                        }
                        strncpy(bufString, buf + pos, nBufSize - 1);
                        bufString[nBufSize-1] = 0;
                        if (bufString[0] && bufString[strlen(bufString)-1] == '\n')		// fgets 会多取一个 '\n' 字符
                        {
                            bufString[strlen(bufString)-1] = 0;
                        }
                        while(bufString[0] && (bufString[strlen(bufString)-1] == ' '
                                               || bufString[strlen(bufString)-1] == '\t'))		// 去掉尾空
                        {
                            bufString[strlen(bufString)-1] = 0;
                        }
                        if (pFile)
                        {
                            fclose(pFile);
                        }
                        return true;
                    }
                }
                while(!feof(pFile) && buf[0] != '[');
            }
        }
        while(!feof(pFile));
    }
    if (pFile)
    {
        fclose(pFile);
    }
    return false;
}

int		CIniFile::GetInt(const char* pKey)
{
    char buf[INIWORDSIZE];		//??
    if (GetString(buf, pKey, INIWORDSIZE))
    {
        return atoi(buf);
    }
    else
    {
        return 0;
    }
}


