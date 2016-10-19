
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Bulletin.cpp: implementation of the CBulletin class.
//
//////////////////////////////////////////////////////////////////////

#include "Bulletin.h"
#include "Hero.h"
#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBulletin::CBulletin()
{
    strcpy(m_szMsg, "");
    m_nCurrentListIndex = 0;
}

CBulletin::~CBulletin()
{
    int nAmount = m_setTitle.size();
    for(int i = 0; i < nAmount; i ++)
    {
        BulletinTitleInfo* pInfo = m_setTitle[i];
        SAFE_DELETE(pInfo);
    }
    m_setTitle.clear();
}

//------------------------------------------------------------------------------------------------------
void	CBulletin::QueryList(unsigned short usIndex, unsigned short usChannel)
{
    int nAmount = m_setTitle.size();
    for(int i = 0; i < nAmount; i ++)
    {
        BulletinTitleInfo* pInfo = m_setTitle[i];
        SAFE_DELETE(pInfo);
    }
    m_setTitle.clear();
    m_nCurrentListIndex = usIndex;
    m_nCurrentChannel = usChannel;
    CMsgMessageBoard msg;
    if (msg.Create(usIndex, MESSAGEBOARD_GETLIST, usChannel))
    {
        msg.Send();
    }
    ::PostCmd(CMD_FLASH_BULLETIN_LIST);
}
//------------------------------------------------------------------------------------------------------
void	CBulletin::SetList(unsigned short usIndex, unsigned short usChannel, CNetStringPacker& StrPacker)
{
    int nAmount = m_setTitle.size();
    int i = 0;
    for(i = 0; i < nAmount; i ++)
    {
        BulletinTitleInfo* pInfo = m_setTitle[i];
        SAFE_DELETE(pInfo);
    }
    m_setTitle.clear();
    m_nCurrentListIndex = usIndex;
    m_nCurrentChannel = usChannel;
    int nStrigAmount = StrPacker.GetStrAmount();
    int nRecordAmount = nStrigAmount / 3;
    char szName[_MAX_NAMESIZE];
    char szWord[64];
    char szTime[32];
    for(i = 0; i < nRecordAmount; i ++)
    {
        StrPacker.GetString(i * 3, szName, _MAX_NAMESIZE);
        StrPacker.GetString(i * 3 + 1, szWord, 64);
        StrPacker.GetString(i * 3 + 2, szTime, _MAX_NAMESIZE);
        // format date ...
        char szFormat[64];
        int i;
        for(i = 0; i < 4; i ++) {szFormat[i] = szTime[i];} // year
        szFormat[4] = '-';
        for(i = 5; i < 7; i ++) {szFormat[i] = szTime[i-1];} // month
        szFormat[7] = '-';
        for(i = 8; i < 10; i ++) {szFormat[i] = szTime[i-2];} // date
        szFormat[10] = '-';
        for(i = 11; i < 13; i ++) {szFormat[i] = szTime[i-3];} // hour
        szFormat[13] = '-';
        for(i = 14; i < 16; i ++) {szFormat[i] = szTime[i-4];} // minu
        szFormat[16] = '\0';
        // format words ...
        ::RepairString(szWord);
        // ok, now format the msg
        BulletinTitleInfo* pInfo = new 	BulletinTitleInfo;
        MYASSERT(pInfo);
        strcpy(pInfo->szName, szName);
        strcpy(pInfo->szTitle, szWord);
        strcpy(pInfo->szTime, szFormat);
        m_setTitle.push_back(pInfo);
    }
    ::PostCmd(CMD_FLASH_BULLETIN_LIST);
}
//------------------------------------------------------------------------------------------------------
int		CBulletin::GetListAmount()
{
    return m_setTitle.size();
}
//------------------------------------------------------------------------------------------------------
BulletinTitleInfo*	CBulletin::GetListLine(int nIndex)
{
    int nAmount = m_setTitle.size();
    if (nIndex >= nAmount || nIndex < 0)
    {
        return NULL;
    }
    return m_setTitle[nIndex];
}
//------------------------------------------------------------------------------------------------------
void	CBulletin::QueryNote(char* pszSenderName, unsigned short usChannel)
{
    strcpy(m_szMsg, "");
    m_nCurrentChannel = usChannel;
    CMsgMessageBoard msg;
    if (msg.Create(0, MESSAGEBOARD_GETWORDS, usChannel, pszSenderName))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------------------------------------------
void	CBulletin::SetNote(char* pszMsg)
{
    if (!pszMsg || strlen(pszMsg) >= 512)
    {
        return;
    }
    strcpy(m_szMsg, pszMsg);
    ::PostCmd(CMD_FLASH_BULLETIN_NOTE);
}
//------------------------------------------------------------------------------------------------------
char*	CBulletin::GetNote()
{
    return m_szMsg;
}
//------------------------------------------------------------------------------------------------------
void CBulletin::SendBulletin(char* pszMsg,  unsigned short usChannel)
{
    CMsgTalk msg;
    if (msg.Create((char*)g_objHero.GetName(), "ALL", pszMsg, NULL, 0xffffff, usChannel))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------------------------------------------
void CBulletin::DelBulletin(unsigned short usChannel)
{
    CMsgMessageBoard msg;
    if (msg.Create(0, MESSAGEBOARD_DEL, usChannel))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------------------------------------------
