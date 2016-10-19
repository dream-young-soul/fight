
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// FDword.cpp: implementation of the CFDword class.
//
//////////////////////////////////////////////////////////////////////

#include "FDword.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFDword::CFDword()
{
    m_pData = NULL;
}

CFDword::~CFDword()
{
    SAFE_DELETE(m_pData);
}
//--------------------------------------------------------------------------------------
void	CFDword::SetValue(DWORD dwValue, int nIndex)
{
    int nOrder = this->Index2Order(nIndex);
    this->CheckOrder(nOrder);
    MYASSERT(m_pData);
    DWORD* pCurrentBlock = m_pData + nOrder;
    *pCurrentBlock = dwValue;
    this->Encrypt(*pCurrentBlock, nOrder);
}
//--------------------------------------------------------------------------------------
DWORD	CFDword::GetValue(int nIndex)
{
    int nOrder = this->Index2Order(nIndex);
    this->CheckOrder(nOrder);
    MYASSERT(m_pData);
    DWORD* pCurrentBlock = m_pData + nOrder;
    DWORD dwMyValue = *pCurrentBlock;
    this->DecryptFile(dwMyValue, nOrder);
    return dwMyValue;
}
//--------------------------------------------------------------------------------------
BOOL		CFDword::Create(int nOrder, DWORD dwSize)
{
    const int _MAX_BLOCK_SIZE = 256;
    if (dwSize > _MAX_BLOCK_SIZE)
    {
        MYASSERT(false);
    }
    SAFE_DELETE(m_pData);
    m_pData = new DWORD[dwSize];
    MYASSERT(m_pData);
    m_nOrder = nOrder % 4;
    m_dwSize = dwSize;
    return true;
}
//--------------------------------------------------------------------------------------
CFDword*	CFDword::CreateNew(int nOrder, DWORD dwSize)
{
    CFDword* pData = new CFDword;
    MYASSERT(pData);
    if (pData->Create(nOrder, dwSize))
    {
        return pData;
    }
    else
    {
        SAFE_DELETE(pData);
    }
    return NULL;
}

//--------------------------------------------------------------------------------------
DWORD		CFDword::Index2Order(int nIndex)
{
    switch(m_nOrder)
    {
    case 0:
        return nIndex;
    case 1:
        return m_dwSize - nIndex - 1;
    case 2:
        if (m_dwSize % 2 == 0)
        {
            return nIndex / 2;
        }
        else
        {
            int nOffset = 0;
            if (m_dwSize % 2 == 0)
            {
                nOffset = m_dwSize / 2;
            }
            else
            {
                nOffset = m_dwSize / 2 + 1;
            }
            return nOffset + nIndex / 2;
        }
    case 3:
        if (m_dwSize % 2 == 1)
        {
            return nIndex / 2;
        }
        else
        {
            int nOffset = 0;
            if (m_dwSize % 2 == 0)
            {
                nOffset = m_dwSize / 2;
            }
            else
            {
                nOffset = m_dwSize / 2 + 1;
            }
            return nOffset + nIndex / 2;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------------
void		CFDword::CheckOrder(int nOrder)
{
    if ((nOrder < 0) && (nOrder >= (int)m_dwSize))
    {
        MYASSERT(false);
    }
}
//--------------------------------------------------------------------------------------
void		CFDword::Encrypt(DWORD& dwData, int nOrder)
{
    DWORD dwCpoy = dwData;
    switch(m_nOrder)
    {
    case 0:
        dwData = _rotl(dwCpoy, nOrder);
        break;
    case 1:
        dwData = _rotr(dwCpoy, nOrder);
        break;
    case 2:
        dwData = _rotl(dwCpoy, nOrder);
        break;
    case 3:
        dwData = _rotr(dwCpoy, nOrder);
        break;
    }
}
//--------------------------------------------------------------------------------------
void		CFDword::DecryptFile(DWORD& dwData, int nOrder)
{
    DWORD dwCpoy = dwData;
    switch(m_nOrder)
    {
    case 0:
        dwData = _rotr(dwCpoy, nOrder);
        break;
    case 1:
        dwData = _rotl(dwCpoy, nOrder);
        break;
    case 2:
        dwData = _rotr(dwCpoy, nOrder);
        break;
    case 3:
        dwData = _rotl(dwCpoy, nOrder);
        break;
    }
}
//--------------------------------------------------------------------------------------
