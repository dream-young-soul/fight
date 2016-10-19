//--------------------------------------------------------------------------------------
// File Name: DnFile.h
// Create by: Huang Yuhang
// Create on: 2003/11/16 20:13
//--------------------------------------------------------------------------------------
#ifndef _DNFILE_H
#define _DNFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <map>

using namespace std;

class CDnFile  
{
public:
	CDnFile();
	virtual ~CDnFile();

public:
	void			BeforeUseDnFile();
	void			AfterUseDnFile();
	FILE*			GetFPtr(const char* pszFile, unsigned long& usFileSize);
	void*			GetMPtr(const char* pszFile, unsigned long& usFileSize);
	void			ClearPtr();
	bool			CheckDisperseFile(const char* pszFile);
	bool			CheckDisperseFile(const unsigned long uFileID);
	void			AddDisperseFile(const char* pszFile);
	bool			OpenFile(const char* pszFile);
	void			CloseFile(const char* pszFile);

private:
	void			Destroy();
	void			Create();
	unsigned long	GenerateID(const char* pszStr);	
	void			ProcessDir(const char* pszDir);
private:
	typedef struct 
	{
		unsigned long uSize;
		unsigned long uOffset;
	}FileIndexInfo;
	typedef struct 
	{
		FILE*	fpDnp;
		map<unsigned long, FileIndexInfo*> mapIndex;
	}DnpInfo;

private:
	map<unsigned long, DnpInfo*>		m_mapDnp;
	map<unsigned long, unsigned char>	m_mapDisperseFiles;
	unsigned char*						m_pBuffer;
	unsigned char*						m_pExtendBuffer;
	FILE*								m_fpExtend;
};
extern CDnFile g_objDnFile;
#endif
