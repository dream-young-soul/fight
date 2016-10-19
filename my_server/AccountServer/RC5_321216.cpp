
//**********************************************************
// 代码编辑器
//**********************************************************

// RC5加密算法(32/12/16)
// 仙剑修，2001.11.16

#include "stdafx.h"
#include "stdlib.h"
#include "rc5_321216.h"


////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long	rotate_left(unsigned long nData, unsigned long nCount);
unsigned long	rotate_right(unsigned long nData, unsigned long nCount);

////////////////////////////////////////////////////////////////////////////////////////////////
void CRc5_321216::Rc5InitKey(const unsigned char bufKey[RC5_16])
{
    // 保存m_bufKey
    try
    {
        memcpy(m_bufKey, bufKey, RC5_16);
    }
    catch(...)
    {
#ifdef	_DEBUG
        ASSERT(!"InitRc5Key()");
#endif
    }

    // 初始化m_bufSub
    m_bufSub[0] = RC5_PW32;
    for(int i = 1; i < RC5_SUB; i++)
    {
        m_bufSub[i] = m_bufSub[i-1] + RC5_QW32;
    }

    // 生成m_bufSub
    int				/*i,*/ j;
    unsigned long	x, y;
    int i = j = x = y = 0;
    for (int k = 0; k < 3 * __max(RC5_KEY, RC5_SUB); k++)
    {
        m_bufSub[i] = rotate_left((m_bufSub[i] + x + y), 3);
        x = m_bufSub[i];
        i = (i + 1) % RC5_SUB;
        m_bufKey[j] = rotate_left((m_bufKey[j] + x + y), (x + y));
        y = m_bufKey[j];
        j = (j + 1) % RC5_KEY;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CRc5_321216::Rc5Encrypt(void* buf, int nLen8)
{
    ASSERT(nLen8 % 8 == 0);
    nLen8 = (nLen8 / 8) * 8;
    if (nLen8 <= 0)
    {
        return;
    }

    unsigned long* bufData = (unsigned long*)buf;
    for(int k = 0; k < nLen8 / 8; k++)
    {
        unsigned long	a = bufData[2*k];
        unsigned long	b = bufData[2*k + 1];

        // 加密a b
        unsigned long	le	= a + m_bufSub[0];
        unsigned long	re	= b + m_bufSub[1];
        for(int i = 1; i <= RC5_12; i++)
        {
            le = rotate_left((le ^ re), re) + m_bufSub[2*i];
            re = rotate_left((re ^ le), le) + m_bufSub[2*i + 1];
        }

        bufData[2*k]		= le;
        bufData[2*k + 1]	= re;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CRc5_321216::Rc5Decrypt(void* buf, int nLen8)
{
    ASSERT(nLen8 % 8 == 0);
    nLen8 = (nLen8 / 8) * 8;
    if (nLen8 <= 0)
    {
        return;
    }

    unsigned long* bufData = (unsigned long*)buf;
    for(int k = 0; k < nLen8 / 8; k++)
    {
        // 解密 ld rd
        unsigned long	ld	= bufData[2*k];
        unsigned long	rd	= bufData[2*k + 1];
        for(int i = RC5_12; i >= 1; i--)
        {
            rd = rotate_right((rd - m_bufSub[2*i + 1]),  ld) ^ ld;
            ld = rotate_right((ld - m_bufSub[2*i]),  rd) ^ rd;
        }

        unsigned long	b = rd - m_bufSub[1];
        unsigned long	a = ld - m_bufSub[0];

        bufData[2*k]		= a;
        bufData[2*k + 1]	= b;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 公用函数
////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long	rotate_left(unsigned long nData, unsigned long nCount)
{
    nCount %= 32;

    unsigned long	nHigh = nData >> (32 - nCount);
    return (nData << nCount) | nHigh;
}

////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long	rotate_right(unsigned long nData, unsigned long nCount)
{
    nCount %= 32;

    unsigned long	nLow = nData << (32 - nCount);
    return (nData >> nCount) | nLow;
}












