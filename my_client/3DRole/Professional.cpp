
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Professional.cpp: implementation of the CProfessional class.
//
//////////////////////////////////////////////////////////////////////

#include "Professional.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfessional::CProfessional()
{
    this->CreateProfessionList();
    strcpy(m_szError, "ERROR");
}

CProfessional::~CProfessional()
{
    this->DestroyProfessionList();
}
//--------------------------------------------------------------------
void CProfessional::CreateProfessionList()
{
    this->DestroyProfessionList();
    const char IniFile[] = "ini/ProfessionalName.ini";
    FILE* fp = fopen(IniFile, "r");
    if (!fp)
    {
        return;
    }
    while(true)
    {
        char szProfessionalName[_MAX_NAMESIZE];
        DWORD dwIndex = 0;
        int nReturn = fscanf(fp, "%u,%s\n", &dwIndex, szProfessionalName);
        if (nReturn == EOF)
        {
            break;
        }
        if (nReturn != 2)
        {
            continue;
        }
        ProfessionalInfo* pInfo = new ProfessionalInfo;
        MYASSERT(pInfo);
        pInfo->dwIndex = dwIndex;
        strcpy(pInfo->szProfessionalName, szProfessionalName);
        m_setProfessional.push_back(pInfo);
    }
    fclose(fp);
}
//--------------------------------------------------------------------
void CProfessional::DestroyProfessionList()
{
    int nAmount = m_setProfessional.size();
    for(int i = nAmount - 1; i >= 0; i --)
    {
        ProfessionalInfo* pInfo = m_setProfessional[i];
        SAFE_DELETE(pInfo);
    }
    m_setProfessional.clear();
}
//--------------------------------------------------------------------
char* CProfessional::GetProfessionalName(DWORD dwIndex)
{
    int nAmount = m_setProfessional.size();
    for(int i = 0; i < nAmount; i ++)
    {
        ProfessionalInfo* pInfo = m_setProfessional[i];
        if (pInfo->dwIndex == dwIndex)
        {
            return pInfo->szProfessionalName;
        }
    }
    return m_szError;
}
//--------------------------------------------------------------------
