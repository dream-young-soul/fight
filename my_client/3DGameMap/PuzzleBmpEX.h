
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// PuzzleBmpEX.h: interface for the CPuzzleBmpEX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUZZLEBMPEX_H__FC364010_759C_4257_AE69_E86FE7644037__INCLUDED_)
#define AFX_PUZZLEBMPEX_H__FC364010_759C_4257_AE69_E86FE7644037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PuzzleBmp.h"
class CPuzzleBmpEX : public CPuzzleBmp
{
public:
    CPuzzleBmpEX();
    virtual ~CPuzzleBmpEX();
private:
    int			m_nLayerCount;
    DWORD*		m_pdwDiffuse;
    int			m_nVetexAmount;
public:
    void		Save(char* pszFile);
    BOOL		Load(char* pszFile);
    void		Destroy();
    void		SetSize(CMySize infoSize, int nLayer);
    void		PreLoad(int iPosX, int iPosY);
    void		Show(int iPosX, int iPosY, BOOL bGrid = false, BOOL bPos = false);
    int			GetGrid(int iPosX, int iPosY, int iLayer = 0);
    int			GetGrid(CMyPos myPos, int iLayer = 0);
    DWORD		GetDiffuse(int iPosX, int iPosY);
    void		SetGrid(int iPosX, int iPosY, unsigned short usIndex, int iLayer = 0);
    void		SetDiffuse(int iPosX, int iPosY, DWORD dwDiffuse[4]);
};

#endif // !defined(AFX_PUZZLEBMPEX_H__FC364010_759C_4257_AE69_E86FE7644037__INCLUDED_)
