
//**********************************************************
// 代码编辑器
//**********************************************************

// AlphaMask.cpp: implementation of the CAlphaMask class.
//
//////////////////////////////////////////////////////////////////////


#include "basefunc.h"
#include "MyDataFile.h"
#include "AlphaMask.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//----------------------------------------
CAlphaMask::CAlphaMask()
{
    m_pBuffer = NULL;
}
//----------------------------------------
CAlphaMask::~CAlphaMask()
{
    SAFE_DELETE(m_pBuffer);
}
//----------------------------------------
BOOL CAlphaMask::CreateBuffer()
{
    SAFE_DELETE(m_pBuffer);
    int nBufferSize = (m_sizeInfo.iWidth * m_sizeInfo.iHeight) / 8 + 1;
    m_pBuffer = new BYTE[nBufferSize];
    if (!m_pBuffer)
    {
        return false;
    }
    return true;
}
//----------------------------------------
BOOL CAlphaMask::IsTransparent(CMyPos posPoint)
{
    if	((posPoint.x < 0) ||
            (posPoint.y < 0) ||
            (posPoint.x >= m_sizeInfo.iWidth) ||
            (posPoint.y >= m_sizeInfo.iHeight))
    {
        return true;
    }
    int nIndex = posPoint.x + posPoint.y * m_sizeInfo.iWidth;
    int nByteOffset = 0; //要偏移几个Byte
    int nBitOffset = 0;  //的几个Bit
    nByteOffset = nIndex / 8;
    nBitOffset = nIndex % 8;
    BYTE* bData = m_pBuffer + nByteOffset;
    BYTE bMask = 1 << nBitOffset;
    if (bMask & *bData)
    {
        return false;
    }
    return true;
}
//----------------------------------------
BOOL CAlphaMask::Load(CMySize sizeBmp, char* pszFileName)
{
    if (!pszFileName)
    {
        return false;
    }
    m_sizeInfo = sizeBmp;
    if (!this->CreateBuffer())
    {
        return false;
    }
    // read ...
    FILE* fp = fopen(pszFileName, "rb");
    if (!fp)
    {
        DWORD dwSize = 0;
        void* pBuf = ::MyDataFileLoad(pszFileName, dwSize);
        if (!pBuf)
        {
            return false;
        }
        return this->Load(sizeBmp, pBuf);
    }
    int nBufferSize = (m_sizeInfo.iWidth * m_sizeInfo.iHeight) / 8 + 1;
    fread(m_pBuffer, 1, nBufferSize, fp);
    fclose(fp);
    return true;
}
//----------------------------------------
BOOL CAlphaMask::Load(CMySize sizeBmp, void* pMaskData)
{
    if (!pMaskData)
    {
        return false;
    }
    m_sizeInfo = sizeBmp;
    if (!this->CreateBuffer())
    {
        return false;
    }
    int nBufferSize = (m_sizeInfo.iWidth * m_sizeInfo.iHeight) / 8 + 1;
    memcpy(m_pBuffer,  pMaskData, nBufferSize);
    return true;
}
