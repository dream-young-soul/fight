
//**********************************************************
// 代码编辑器
//**********************************************************

////////////////////////////////////////////////////////////////////////////////////////////////////
// 服务端SOCKET用加密模板
////////////////////////////////////////////////////////////////////////////////////////////////////
// 仙剑修, 2001.10.26
// 修改为模板：2001.12.11

#ifndef	ENCRYPTSERVER_H
#define ENCRYPTSERVER_H

#include <stdlib.h>
#include "Logfile.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// 3个随机数, 用于隐藏服务端INI中的SERVER KEY。不变
#define		aa	0x7E
#define		bb	0x33
#define		cc	0xA1

////////////////////////////////////////////////////////////////////////////////////////////////////
template <unsigned long key1, unsigned long key2>
class	CEncryptServer
{
public:
    CEncryptServer() { Init(); }
//default	CEncryptServer(CEncryptServer & cEncrypt);
public:
    void Init() { m_nPos1 = m_nPos2 = 0; }
    void Encrypt(unsigned char* bufMsg, int nLen, bool bMove = true);
protected:
    int		m_nPos1;
    int		m_nPos2;
protected:
    class	CEncryptCode
    {
    public:
        CEncryptCode()
        {
            try
            {
                // 生成 ABC
                int	a1, b1, c1, fst1;
                a1		= ((key1 >> 0) & 0xFF) ^ aa;
                b1		= ((key1 >> 8) & 0xFF) ^ bb;
                c1		= ((key1 >> 24) & 0xFF) ^ cc;
                fst1	= (key1 >> 16) & 0xFF;

                int	a2, b2, c2, fst2;
                a2		= ((key2 >> 0) & 0xFF) ^ aa;
                b2		= ((key2 >> 8) & 0xFF) ^ bb;
                c2		= ((key2 >> 24) & 0xFF) ^ cc;
                fst2	= (key2 >> 16) & 0xFF;

                /*
                		unsigned char	nCode = fst1;
                		for(int i = 0; i < 256; i++)
                		{
                			m_cGlobalEncrypt.m_bufEncrypt1[i] = nCode;
                			int	nTemp = a1*512;
                			nCode = (nTemp*128 + c1 + b1*256) % 256;
                		}
                		assert(nCode == fst1);

                			nCode = fst2;
                		for( i = 0; i < 256; i++)
                		{
                			m_cGlobalEncrypt.m_bufEncrypt2[i] = nCode;
                			int	nTemp = a2*1024;
                			nCode = ((b2<<8) + nTemp*64 + c2) & 0xFF;
                		}
                		assert(nCode == fst2);
                //*/
                //*else
                unsigned char	nCode = fst1;
                for(int i = 0; i < 256; i++)
                {
                    m_bufEncrypt1[i] = nCode;
                    //			printf("%02X ", nCode);		//?
                    nCode = (a1 * nCode * nCode + b1 * nCode + c1) % 256;
                }
                //		printf("[%02X]\n", nCode);		//?
                assert(nCode == fst1);

                nCode = fst2;
                for(int i = 0; i < 256; i++)
                {
                    m_bufEncrypt2[i] = nCode;
                    nCode = (a2 * nCode * nCode + b2 * nCode + c2) % 256;
                }
                assert(nCode == fst2);
                //*/
            }
            catch(...) { LOGCATCH("Encrypt init fail."); exit(3); }
        }
//	protected:
        unsigned char m_bufEncrypt1[256];
        unsigned char m_bufEncrypt2[256];
    } m_cGlobalEncrypt;
};

//template <unsigned long key1, unsigned long key2>
//CEncryptServer<key1, key2>::CEncryptCode	CEncryptServer<key1, key2>::m_cGlobalEncrypt;


template <unsigned long key1, unsigned long key2>
inline void CEncryptServer<key1, key2>::Encrypt(unsigned char* bufMsg, int nLen, bool bMove /*= true*/)
{
    try
    {
        int		nOldPos1 = m_nPos1;
        int		nOldPos2 = m_nPos2;
        for(int i = 0; i < nLen; i++)
        {
            bufMsg[i] ^= m_cGlobalEncrypt.m_bufEncrypt1[m_nPos1];
            bufMsg[i] ^= m_cGlobalEncrypt.m_bufEncrypt2[m_nPos2];
            if (++m_nPos1 >= 256)
            {
                m_nPos1 = 0;
                if (++m_nPos2 >= 256)
                {
                    m_nPos2 = 0;
                }
            }
            assert(m_nPos1 >= 0 && m_nPos1 < 256);
            assert(m_nPos2 >= 0 && m_nPos2 < 256);
        }

        if (!bMove)
        {
            // 恢复指针
            m_nPos1 = nOldPos1;
            m_nPos2 = nOldPos2;
        }
    }
    catch(...) { LOGCATCH("Encrypt fail."); exit(3); }
}


#endif	// ENCRYPTSERVER_H
