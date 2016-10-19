#ifndef _c3_datafile_h_
#define _c3_datafile_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#include "c3_common.h"

#define MAXDATAFILE 16
//#define HANDLE DWORD

struct C3DataFileIndex {
	DWORD uid;
	DWORD offset;
	DWORD size;
	DWORD space;
};

struct C3DataFileHeader {
	DWORD id;
	int number;
	unsigned offset;
};

/* datafile */
struct C3DataFile
{
	void* m_File ; 
	C3DataFileIndex *m_Index;
	DWORD m_Id;
	int m_Number;
	C3DataFile () { m_Index = 0, m_File = INVALID_HANDLE_VALUE, m_Id = 0 ; }
};

extern C3DataFile _WDF[MAXDATAFILE];

// close a datafile
C3_CORE_DLL_API
void DataFile_Close ( C3DataFile *lpDataFile );

// test if the datafile is already open
C3_CORE_DLL_API
BOOL DataFile_IsOpen ( C3DataFile *lpDataFile, DWORD id ) ;

// test if the datafile is valid or not
C3_CORE_DLL_API
BOOL DataFile_IsValid ( C3DataFile *lpDataFile ) ;

// use the id to search the datafile
C3_CORE_DLL_API	
C3DataFileIndex* DataFile_SearchFile ( C3DataFile *lpDataFile, DWORD id ) ;

// get the specify file handle
C3_CORE_DLL_API
HANDLE DataFile_GetFileHandle( C3DataFile *lpDataFile ) ;

C3_CORE_DLL_API
void* DataFile_Load ( const char* pszFile, DWORD& dwSize );

// returen the pack name id
C3_CORE_DLL_API
DWORD pack_name ( const char *filename ) ;

// returen the real file name id
C3_CORE_DLL_API
DWORD real_name ( const char *filename ) ;


#endif