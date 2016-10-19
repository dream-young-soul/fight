
//**********************************************************
// 代码编辑器
//**********************************************************

// AutoAllot.cpp: implementation of the CAutoAlloc class.
//
//////////////////////////////////////////////////////////////////////

#include "AutoAllot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoAllot::CAutoAllot()
{
    m_pdwProfessionalIndex	= NULL;
    this->BulidIndex();
}

CAutoAllot::~CAutoAllot()
{
    if (m_pdwProfessionalIndex)
    {
        delete[] m_pdwProfessionalIndex;
        m_pdwProfessionalIndex = NULL;
    }
}

//--------------------------------------------------------------------

void CAutoAllot::BulidIndex()
{
    char szAutoAllotFile[] = "ini/AutoAllot.dat";
    FILE* fp = fopen(szAutoAllotFile, "rb");
    MYASSERT(fp);
    // read ProfessionalAmount
    fread(&m_dwProfessionalAmount, sizeof(DWORD), 1, fp);
    // get level amount
    fread(&m_dwLevelAmount, sizeof(DWORD), 1, fp);
    // Allot Memeory for Index
    if (m_pdwProfessionalIndex)
    {
        delete[] m_pdwProfessionalIndex;
        m_pdwProfessionalIndex = NULL;
    }
    m_pdwProfessionalIndex = new DWORD[m_dwProfessionalAmount];
    // fill data
    fread(m_pdwProfessionalIndex, sizeof(DWORD), m_dwProfessionalAmount, fp);
    fclose(fp);
}

//--------------------------------------------------------------------

BOOL CAutoAllot::GetAllot(DWORD dwLevel, DWORD dwProfessional, FourAttribInfo& infoAttrib)
{
    /*
    定位公式：DWORD + DWORD +
    			unsigned short * m_dwProfessionalAmount +
    				ProfessionalIndex * m_dwLevelAmount * 4 * unsigned short +
    				(Level-1) * 4 * unsigned short;
    */
    if (!m_pdwProfessionalIndex)
    {
        return false;
    }
    // locate professional and get the professional index
    DWORD dwProfessionalIndex = 0;
    BOOL bFound = false;
    for(DWORD dwCounter = 0; dwCounter < m_dwProfessionalAmount; dwCounter ++)
    {
        DWORD* pdwProfessional = m_pdwProfessionalIndex + dwCounter;
        if (*pdwProfessional == dwProfessional / 10)
        {
            bFound = true;
            dwProfessionalIndex = dwCounter;
            break;
        }
    }
    IF_NOT(bFound)
    return false;
    DWORD dwOffset = 2 * sizeof(DWORD) +
                     sizeof(DWORD) * m_dwProfessionalAmount +
                     dwProfessionalIndex * m_dwLevelAmount * 4 * sizeof(DWORD) +
                     (dwLevel - 1) * 4 * sizeof(DWORD);
    char szAutoAllotFile[] = "ini/AutoAllot.dat";
    FILE* fp = fopen(szAutoAllotFile, "rb");
    MYASSERT(fp);
    MYASSERT(fseek(fp, dwOffset, SEEK_SET) == 0);
    fread(&infoAttrib.dwForce, sizeof(DWORD), 1, fp);
    fread(&infoAttrib.dwSpeed, sizeof(DWORD), 1, fp);
    fread(&infoAttrib.dwHealth, sizeof(DWORD), 1, fp);
    fread(&infoAttrib.dwSoul, sizeof(DWORD), 1, fp);
    fclose(fp);
    return true;
}

//--------------------------------------------------------------------
