
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include <stdio.h>
#include <windows.h>

#include "Basefunc.h"
#include "MsgPacket.h"

// static init
HANDLE	CMsgPacket::s_hHeap				= NULL;
UINT	CMsgPacket::s_uNumAllocsInHeap	= 0;

//---------------------------------------------------------------
CMsgPacket::CMsgPacket(void)
{
    Reset();
}

//---------------------------------------------------------------
void* CMsgPacket::operator new(size_t size)
{
    // got heap?
    if (!s_hHeap)
    {
        s_hHeap	=::HeapCreate(0, 0, 0);
        if (!s_hHeap)
        {
            return NULL;
        }
    }

    // alloc mem for new obj
    void* p	=::HeapAlloc(s_hHeap, HEAP_ZERO_MEMORY, size);

    if (p)
    {
        s_uNumAllocsInHeap++;
    }

    return p;
}

//---------------------------------------------------------------
void CMsgPacket::operator delete(void* p)
{
    // check param
    if (!s_hHeap || s_uNumAllocsInHeap <= 0)
    {
        return;
    }

    // free it...
    if (::HeapFree(s_hHeap, 0, p))
    {
        s_uNumAllocsInHeap--;
    }

    // time to destroy heap?
    if (s_uNumAllocsInHeap <= 0)
    {
        if (::HeapDestroy(s_hHeap))
        {
            s_hHeap	= NULL;
        }
    }
}

//---------------------------------------------------------------
BOOL CMsgPacket::IsValidPt(void* p)
{
    if (!s_hHeap || !p)
    {
        return false;
    }

    if ((DWORD)p < 0x00010000 || (DWORD)p >= 0x7FFEFFFF)	// user address range from 1M--2G-64k
    {
        return false;
    }

    if (::IsBadCodePtr((FARPROC)p))
    {
        return false;
    }

    return ::HeapValidate(s_hHeap, 0, p);
}

//---------------------------------------------------------------
void CMsgPacket::Reset()
{
    memset(m_bufMsg, 0L, _MAX_PACKETSIZE);
    m_nCurSize	= 0;
}

//---------------------------------------------------------------
BOOL CMsgPacket::Create(void* pbuf, int nSize)
{
    if (!pbuf || nSize <= 0 || nSize > _MAX_PACKETSIZE)	// input param error
    {
        return false;
    }

    memcpy(m_bufMsg, pbuf, nSize);	//_MAX_PACKETSIZE);	//
    if ((int)m_unMsgSize != nSize)   // bad packet
    {
        return false;
    }
    else
    {
        return true;
    }
}

//---------------------------------------------------------------
int CMsgPacket::AddContent(void* pbuf, int nSize)
{
    if (!pbuf || nSize <= 0 || nSize > _MAX_PACKETSIZE)	// input param error
    {
        return -1;
    }

    memcpy(m_bufMsg + m_nCurSize, pbuf, nSize);
    m_nCurSize	+= nSize;

    if (m_nCurSize < sizeof(m_unMsgSize))
    {
        return sizeof(m_unMsgSize) - m_nCurSize;
    }
    else if (m_nCurSize == sizeof(m_unMsgSize))
    {
        return m_unMsgSize - sizeof(m_unMsgSize);
    }
    else
    {
        return m_unMsgSize - m_nCurSize;
    }
}

//---------------------------------------------------------------
void CMsgPacket::Dump(void)
{
    char szName[256];
    sprintf(szName, "dump%u.pkt", ::SysTimeGetEx());

    FILE* fp	= fopen(szName, "wb");
    if (fp)
    {
        try
        {
            fwrite(m_bufMsg, m_unMsgSize, 1L, fp);
        }
        catch(...)
        {
            ::LogSave("invalid dump pkt.");
        }

        fclose(fp);
    }
}






//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgPacketCache::CMsgPacketCache()
{
    m_dwTimeLastSend    = 0;
    m_dwTimeInterval	= 20;

    m_dequePacketToSend.clear();
}

CMsgPacketCache::~CMsgPacketCache()
{
    DWORD dwSize	= m_dequePacketToSend.size();
    for(DWORD i = 0; i < dwSize; i++)
    {
        delete m_dequePacketToSend[i];
    }

    m_dequePacketToSend.clear();
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPacketCache::AddPacket	(const void* pbuf, DWORD dwSize)
{
    if (!pbuf || dwSize <= 0)
    {
        return false;
    }

    CMsgPacket* pNewPacket	= new CMsgPacket;
    if (!pNewPacket)
    {
        return false;
    }

    if (!pNewPacket->Create((void*)pbuf, dwSize))
    {
        return false;
    }

    // set add time
    pNewPacket->m_dwAnyData	=::TimeGet();

    m_dequePacketToSend.push_back(pNewPacket);
    return true;
}

//////////////////////////////////////////////////////////////////////
CMsgPacket* CMsgPacketCache::GetFirstPacket(void)
{
    if (m_dequePacketToSend.size() <= 0)
    {
        return NULL;
    }

    CMsgPacket* pPacket	= m_dequePacketToSend[0];
    if (!pPacket)
    {
        return NULL;
    }

    DWORD dwTimeNow		=::TimeGet();
    DWORD dwTimeMsgAdd	= pPacket->m_dwAnyData;
    if (dwTimeNow - dwTimeMsgAdd < m_dwTimeInterval)
    {
        return NULL;
    }

    return pPacket;
}

//////////////////////////////////////////////////////////////////////
void CMsgPacketCache::DelFirstPacket(void)
{
    if (m_dequePacketToSend.size() <= 0)
    {
        return;
    }

    delete m_dequePacketToSend[0];
    m_dequePacketToSend.pop_front();

    m_dwTimeLastSend	=::TimeGet();
}
