
//**********************************************************
// 代码编辑器
//**********************************************************

// EncryptData.cpp: implementation of the CEncryptData class.
//
//////////////////////////////////////////////////////////////////////

#include "EncryptData.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#include "../std_defs.h"


#define f_rnd(i,a,b,c,d)                    \
        u = _rotl(d * (d + d + 1), 5);       \
        t = _rotl(b * (b + b + 1), 5);       \
        a = _rotl(a ^ t, u) + l_key[i];      \
        c = _rotl(c ^ u, t) + l_key[i + 1]

#define i_rnd(i,a,b,c,d)                    \
        u = _rotl(d * (d + d + 1), 5);       \
        t = _rotl(b * (b + b + 1), 5);       \
        c = _rotr(c - l_key[i + 1], t) ^ u;  \
        a = _rotr(a - l_key[i], u) ^ t



/* initialise the key schedule from the user supplied key   */

DWORD* CEncryptData::set_key(const DWORD in_key[], const DWORD key_len)
{
    DWORD  i, j, k, a, b, l[8], t;
    l_key[0] = 0xb7e15163;
    for(k = 1; k < 44; ++k)
    {
        l_key[k] = l_key[k - 1] + 0x9e3779b9;
    }
    for(k = 0; k < key_len / 32; ++k)
    {
        l[k] = in_key[k];
    }
    t = (key_len / 32) - 1; // t = (key_len / 32);
    if (t > key_len)
    {
        t = 0;
    }
    a = b = i = j = 0;
    for(k = 0; k < 132; ++k)
    {
        a = _rotl(l_key[i] + a + b, 3);
        b += a;
        b = _rotl(l[j] + b, b);
        l_key[i] = a;
        l[j] = b;
        i = (i == 43 ? 0 : i + 1);  //i = (i + 1) % 44;
        j = (j == t ? 0 : j + 1);   //j = (j + 1) % t;
    }
    return l_key;
};

/* encrypt a block of text  */

inline void CEncryptData::encrypt(const DWORD in_blk[4], DWORD out_blk[4])
{
    DWORD  a, b, c, d, t, u;
    a = in_blk[0];
    b = in_blk[1] + l_key[0];
    c = in_blk[2];
    d = in_blk[3] + l_key[1];
    f_rnd( 2, a, b, c, d);
    f_rnd( 4, b, c, d, a);
    f_rnd( 6, c, d, a, b);
    f_rnd( 8, d, a, b, c);
    f_rnd(10, a, b, c, d);
    f_rnd(12, b, c, d, a);
    f_rnd(14, c, d, a, b);
    f_rnd(16, d, a, b, c);
    f_rnd(18, a, b, c, d);
    f_rnd(20, b, c, d, a);
    f_rnd(22, c, d, a, b);
    f_rnd(24, d, a, b, c);
    f_rnd(26, a, b, c, d);
    f_rnd(28, b, c, d, a);
    f_rnd(30, c, d, a, b);
    f_rnd(32, d, a, b, c);
    f_rnd(34, a, b, c, d);
    f_rnd(36, b, c, d, a);
    f_rnd(38, c, d, a, b);
    f_rnd(40, d, a, b, c);
    out_blk[0] = a + l_key[42];
    out_blk[1] = b;
    out_blk[2] = c + l_key[43];
    out_blk[3] = d;
};

/* decrypt a block of text  */

inline void CEncryptData::decrypt(const DWORD in_blk[4], DWORD out_blk[4])
{
    DWORD  a, b, c, d, t, u;
    d = in_blk[3];
    c = in_blk[2] - l_key[43];
    b = in_blk[1];
    a = in_blk[0] - l_key[42];
    i_rnd(40, d, a, b, c);
    i_rnd(38, c, d, a, b);
    i_rnd(36, b, c, d, a);
    i_rnd(34, a, b, c, d);
    i_rnd(32, d, a, b, c);
    i_rnd(30, c, d, a, b);
    i_rnd(28, b, c, d, a);
    i_rnd(26, a, b, c, d);
    i_rnd(24, d, a, b, c);
    i_rnd(22, c, d, a, b);
    i_rnd(20, b, c, d, a);
    i_rnd(18, a, b, c, d);
    i_rnd(16, d, a, b, c);
    i_rnd(14, c, d, a, b);
    i_rnd(12, b, c, d, a);
    i_rnd(10, a, b, c, d);
    i_rnd( 8, d, a, b, c);
    i_rnd( 6, c, d, a, b);
    i_rnd( 4, b, c, d, a);
    i_rnd( 2, a, b, c, d);
    out_blk[3] = d - l_key[1];
    out_blk[2] = c;
    out_blk[1] = b - l_key[0];
    out_blk[0] = a;
};


CEncryptData::CEncryptData()
{
    DWORD dwPassWord[4] = {::RandGet(INT_MAX, true), ::RandGet(INT_MAX, true), ::RandGet(INT_MAX, true), ::RandGet(INT_MAX, true)};
    this->set_key(dwPassWord, 4);
    this->SetValue(0);
}

CEncryptData::~CEncryptData()
{
}

void CEncryptData::Reset()
{
	memset(l_key, 0, sizeof(l_key));
	memset(m_dwValue, 0, sizeof(m_dwValue));
}

//---------------------------------------------------------------------
void CEncryptData::SetValue(int nData)
{
    // 使用RC6来加大强度
    if ( nData > 0)
    {
        int fda = 0;
    }
    DWORD dwTemp[4] = {nData & 0x000000ff, (nData >> 8) & 0x000000ff, (nData >> 16) & 0x000000ff, (nData >> 24) & 0x000000ff};
    this->encrypt(dwTemp, m_dwValue);
}
//---------------------------------------------------------------------
int CEncryptData::GetValue(void)
{
    DWORD dwTemp[4] = {0, 0, 0, 0};
    this->decrypt(m_dwValue, dwTemp);
    DWORD dwLife = (dwTemp[0] + ((dwTemp[1] << 8) & 0x0000ff00) + ((dwTemp[2] << 16) & 0x00ff0000) + ((dwTemp[3] << 24) & 0xff000000));
    return (int)(dwTemp[0] + ((dwTemp[1] << 8) & 0x0000ff00) + ((dwTemp[2] << 16) & 0x00ff0000) + ((dwTemp[3] << 24) & 0xff000000));
}
//---------------------------------------------------------------------
