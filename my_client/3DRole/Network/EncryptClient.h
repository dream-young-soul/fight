
//**********************************************************
// 代码编辑器
//**********************************************************

////////////////////////////////////////////////////////////////////////////////////////////////////
// 客户端SOCKET用加密模板
////////////////////////////////////////////////////////////////////////////////////////////////////
// 仙剑修, 2001.10.26
// 修改为模板：2001.12.11

#ifndef	ENCRYPTCLIENT_H
#define ENCRYPTCLIENT_H

#include <stdlib.h>
//#include "Profile.h"
#include "gamemsg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//#define		LOGCATCH	LOGERROR
//@@@	#define		LOGCATCH	LogSave

////////////////////////////////////////////////////////////////////////////////////////////////////
template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
class	CEncryptClient
{
public:
    CEncryptClient() { Init(); }
    //default	CEncryptClient(CEncryptClient & cEncrypt);
public:
    void Init() { m_nPos1 = m_nPos2 = 0; }
    void Encrypt(unsigned char* bufMsg, int nLen, bool bMove = true);
    void ChangeCode(DWORD dwData);
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
                //* 与下面ELSE算法相同，只为加强复杂性：)
                unsigned char	nCode = fst1;
                int i;
                for(i = 0; i < 256; i++)
                {
                    m_bufEncrypt1[i] = nCode;
                    int	nTemp = (a1 * nCode) % 256;
                    nCode = (c1 + nTemp * nCode + b1 * nCode) % 256;
                }
                //@@@	assert(nCode == fst1);
                nCode = fst2;
                for( i = 0; i < 256; i++)
                {
                    m_bufEncrypt2[i] = nCode;
                    int	nTemp = a2 * nCode;
                    nCode = ((b2 + nTemp) * nCode + c2) & 0xFF;
                }
                //@@@	assert(nCode == fst2);
                //*/
                /*else
                		unsigned char	nCode = fst1;
                		for(int i = 0; i < 256; i++)
                		{
                			m_bufEncrypt1[i] = nCode;
                //			printf("%02X ", nCode);		//???
                			nCode = (a1*nCode*nCode + b1*nCode + c1) % 256;
                		}
                //		printf("[%02X]\n", nCode);		//???
                		assert(nCode == fst1);

                			nCode = fst2;
                		for( i = 0; i < 256; i++)
                		{
                			m_bufEncrypt2[i] = nCode;
                			nCode = (a2*nCode*nCode + b2*nCode + c2) % 256;
                		}
                		assert(nCode == fst2);
                //*/
            }
            catch(...) { /*@@@	LOGCATCH("Encrypt init fail."); exit(3);*/ }
        }
        //	protected:
        unsigned char m_bufEncrypt1[256];
        unsigned char m_bufEncrypt2[256];
    } m_cGlobalEncrypt;		//??? 应改成静态共享的成员对象，以节约资源
};

//template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
//					unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
//CEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::CEncryptCode	CEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::m_cGlobalEncrypt;


template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
inline void CEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::Encrypt(unsigned char* bufMsg, int nLen, bool bMove /*= true*/)
{
    /*
    	typedef struct {
    		USHORT	unMsgSize;
    		USHORT	unMsgType;
    	}MsgInfo;

    	MsgInfo* pInfo = (MsgInfo*)bufMsg;
    	if (_MSG_INTERACT == pInfo->unMsgType)
    		g_objGameMsg.AddMsg(_TXTATR_NORMAL, "INTERACT msg encrypt.");
    //*/
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
            //@@@	assert(m_nPos1 >=0 && m_nPos1 < 256);
            //@@@	assert(m_nPos2 >=0 && m_nPos2 < 256);
        }
        if (!bMove)
        {
            // 恢复指针
            m_nPos1 = nOldPos1;
            m_nPos2 = nOldPos2;
        }
    }
    catch(...) { /*@@@	LOGCATCH("Encrypt fail."); exit(3);*/ }
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
inline void CEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::ChangeCode(DWORD dwData)
{
    try
    {
        DWORD	dwData2 = dwData * dwData;
        for(int i = 0; i < 256; i += 4)
        {
            *(DWORD*)(&m_cGlobalEncrypt.m_bufEncrypt1[i]) ^= dwData;
            *(DWORD*)(&m_cGlobalEncrypt.m_bufEncrypt2[i]) ^= dwData2;
        }
    }
    catch(...) { /*LOGCATCH("ChangeCode fail.");*/ }
}



#endif	// ENCRYPTCLIENT_H
