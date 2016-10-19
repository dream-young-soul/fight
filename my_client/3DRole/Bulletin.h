
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Bulletin.h: interface for the CBulletin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BULLETIN_H__5AC7A4CB_CB01_44E0_871D_8E490ADF5364__INCLUDED_)
#define AFX_BULLETIN_H__5AC7A4CB_CB01_44E0_871D_8E490ADF5364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include "NetStringPacker.h"
#include <vector>

typedef struct
{
    char szName[_MAX_NAMESIZE];
    char szTitle[64];
    char szTime[32];
} BulletinTitleInfo;

using namespace std;
typedef vector<BulletinTitleInfo*> STRING_SET;

class CBulletin
{
public:
    CBulletin();
    virtual ~CBulletin();
    // bulletin
public:
    void				SendBulletin(char* pszMsg,  unsigned short usChannel);
    void				DelBulletin(unsigned short usChannel);

    void				QueryList(unsigned short usIndex, unsigned short usChannel);
    void				SetList(unsigned short usIndex, unsigned short usChannel, 	CNetStringPacker& StrPacker);
    int					GetListAmount();
    BulletinTitleInfo*	GetListLine(int nIndex);

    void				QueryNote(char* pszSenderName, unsigned short usChannel);
    void				SetNote(char* pszMsg);
    char*				GetNote();

    unsigned short		GetCurrentListIndex() {return m_nCurrentListIndex;}
    unsigned short		GetCurrentChannel() {return m_nCurrentChannel;}
private:
    STRING_SET			m_setTitle;
    char				m_szMsg[512];
    int					m_nCurrentListIndex;
    int					m_nCurrentChannel;
};

#endif // !defined(AFX_BULLETIN_H__5AC7A4CB_CB01_44E0_871D_8E490ADF5364__INCLUDED_)
