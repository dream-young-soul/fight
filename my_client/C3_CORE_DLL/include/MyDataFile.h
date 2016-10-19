#ifndef _MyDataFile_h_
#define _MyDataFile_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

// Open a datafile
C3_CORE_DLL_API
BOOL MyDataFileOpen ( const char *filename );

C3_CORE_DLL_API
void MyDataFileClose ();

// Load a file from a datafile
C3_CORE_DLL_API
void* MyDataFileLoad ( const char *filename, DWORD &size );

//------------------------------------------------------
C3_CORE_DLL_API
BOOL MyDnpFileOpen ( const char *filename );

C3_CORE_DLL_API
void* OpenFilePointer(const char* pszFile,unsigned long &usFileSize);
#endif