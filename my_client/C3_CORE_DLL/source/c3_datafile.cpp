#pragma warning(disable:4786)
#include "..\include\c3_datafile.h"
#include "..\include\MyDataFile.h"
#include "..\include\DnFile.h"


C3DataFile _WDF[MAXDATAFILE];

C3_CORE_DLL_API
DWORD stringtoid ( const char* str )
{
	int i;
	unsigned int v;
	static unsigned m[70];
	strncpy((char *)m,str,256);
	for (i=0;i<256/4 && m[i];i++) ;
	m[i++]=0x9BE74448,m[i++]=0x66F42C48;
	v=0xF4FA8928;

	__asm {
		mov esi,0x37A8470E		;x0=0x37A8470E
		mov edi,0x7758B42B		;y0=0x7758B42B
		xor ecx,ecx
_loop:
		mov ebx,0x267B0B11		;w=0x267B0B11
		rol v,1
		lea eax,m
		xor ebx,v

		mov eax,[eax+ecx*4]
		mov edx,ebx
		xor esi,eax
		xor edi,eax

		add edx,edi
		or edx,0x2040801		;a=0x2040801
		and edx,0xBFEF7FDF		;c=0xBFEF7FDF

		mov eax,esi
		mul edx
		adc eax,edx
		mov edx,ebx
		adc eax,0

		add edx,esi
		or edx,0x804021			;b=0x804021
		and edx,0x7DFEFBFF		;d=0x7DFEFBFF

		mov esi,eax
		mov eax,edi
		mul edx

		add edx,edx
		adc eax,edx
		jnc _skip
		add eax,2
_skip:
		inc ecx;
		mov edi,eax
		cmp ecx,i
		jnz _loop
		xor esi,edi
		mov v,esi
	}
	return v;
}


DWORD string_id ( const char* filename )
{
	char buffer[256];
	int i;
	for (i=0;filename[i];i++) {
		if (filename[i]>='A' && filename[i]<='Z') buffer[i]=filename[i]+'a'-'A';
		else if (filename[i]=='\\') buffer[i]='/';
		else buffer[i]=filename[i];
	}
	buffer[i]=0;
	return stringtoid(buffer);
}

C3_CORE_DLL_API
void* MyDataFileLoad ( const char *filename, DWORD &size )
{
	if (!filename)
		return NULL;

	DWORD id = pack_name ( filename ) ;
	DWORD fid= real_name ( filename ) ;
	
	C3DataFileIndex* pf = NULL;
	// begin search the WFile 
	int i = 0;
	for ( i=0; i < MAXDATAFILE; i++ ) 
	{
		if ( DataFile_IsOpen ( &_WDF[i], id ) )
		{
			pf = DataFile_SearchFile ( &_WDF[i], fid ) ;
			if (pf == NULL) 
				return NULL;
			else
				break;
		}
	}

	if ( i == MAXDATAFILE ) 
		return NULL;

	HANDLE f = DataFile_GetFileHandle ( &_WDF[i] ) ;
	if (!f)
		return NULL;

	void *p = malloc ( pf->size ) ;
	if (!p)
		return NULL;

	SetFilePointer(f, pf->offset, 0, FILE_BEGIN);

	DWORD bytes = 0;
	if (ReadFile(f, p, pf->size, &bytes, 0) == 0)
	{
		free(p);

		size = 0;
		return NULL;
	}
	else
	{
		size = pf->size;
		return p;
	}
}

C3_CORE_DLL_API
void MyDataFileClose ( void )
{
	for (int i=0; i<MAXDATAFILE; i++ ) 
	{
		DataFile_Close( &_WDF[i] );
	}
}

C3_CORE_DLL_API
BOOL MyDataFileOpen ( const char *filename )
{
	int i;
	for ( i=0; i<MAXDATAFILE; i++ ) 
	{
		if ( !DataFile_IsValid( &_WDF[i] ) ) 
		{
			break ;
		}
	}

	if ( i == MAXDATAFILE )
	{
		i = 0 ;
		DataFile_Close ( &_WDF[i] ) ;
	}

	HANDLE f;
	(void *) f = CreateFile ( filename, 
					 GENERIC_READ, 
					 FILE_SHARE_READ, 
					 0, 
					 OPEN_EXISTING,
					 FILE_ATTRIBUTE_NORMAL,
					 0 ) ;

	if ( f == INVALID_HANDLE_VALUE )
		return false ;

	C3DataFileHeader header;
	DWORD bytes;
	if ( ReadFile ( f, &header, sizeof(header), &bytes, 0 ) == 0 ) 
	{
		CloseHandle ( f ) ;
		return false ;
	}

	_WDF[i].m_Index = ( C3DataFileIndex* ) malloc ( sizeof ( C3DataFileIndex ) * header.number ) ;
	if (!_WDF[i].m_Index)
	{
		CloseHandle ( f ) ;
		return false ;
	}

	SetFilePointer ( f, header.offset, 0, FILE_BEGIN ) ;
	if (ReadFile( f, _WDF[i].m_Index, sizeof (C3DataFileIndex)*header.number, &bytes, 0 ) == 0 ) 
	{
		CloseHandle ( f ) ;
		free( _WDF[i].m_Index ) ;
		return false ;
	}

	_WDF[i].m_Number = header.number ;
	_WDF[i].m_File = f ;
	_WDF[i].m_Id = string_id ( filename ) ;
	return true ;
}

C3_CORE_DLL_API
void DataFile_Close ( C3DataFile *lpDataFile )
{
	if ( lpDataFile->m_File != INVALID_HANDLE_VALUE ) 
	{
		CloseHandle ( lpDataFile->m_File ) ;
		lpDataFile->m_File = NULL;
	}

	if ( lpDataFile->m_Index ) 
	{
		free ( lpDataFile->m_Index ) ;
		lpDataFile->m_Index = NULL;
	}

	lpDataFile->m_Id = 0 ;
	lpDataFile->m_Number = 0;
}

C3_CORE_DLL_API	
C3DataFileIndex* DataFile_SearchFile ( C3DataFile *lpDataFile, DWORD id )
{
	int begin, end, middle ;
	begin = 0;
	end = lpDataFile->m_Number - 1 ;
	while ( begin<=end ) 
	{
		middle = ( begin + end ) / 2 ;
		if ( lpDataFile->m_Index[middle].uid == id ) 
			return &lpDataFile->m_Index[middle] ;
		else if ( lpDataFile->m_Index[middle].uid < id ) 
			begin = middle+1 ;
		else 
			end = middle-1 ;
	}
	return 0;
}

C3_CORE_DLL_API
void* DataFile_Load ( const char* pszFile, DWORD& dwSize )
{
	return MyDataFileLoad(pszFile, dwSize);
}


C3_CORE_DLL_API
BOOL DataFile_IsOpen ( C3DataFile *lpDataFile, DWORD id )
{
	return lpDataFile->m_Id == id ;
}

C3_CORE_DLL_API
BOOL DataFile_IsValid ( C3DataFile *lpDataFile ) 
{ 
	return lpDataFile->m_File != INVALID_HANDLE_VALUE ; 
}

C3_CORE_DLL_API
HANDLE DataFile_GetFileHandle( C3DataFile *lpDataFile )
{ 
	return lpDataFile->m_File ; 
}

C3_CORE_DLL_API
DWORD pack_name(const char *filename)
{
	static char buffer[256] ;

	int i ;
	for ( i=0; filename[i]; i++ ) 
	{
		if ( filename[i] == '/' ) 
		{
			memcpy ( buffer+i, ".wdf", 5 ) ;
			break ;
		}
		if ( filename[i] >= 'A' && filename[i] <= 'Z' ) 
			buffer[i] = filename[i] + 'a' - 'A' ;
		else 
			buffer[i] = filename[i] ;
	}
	if (i==0) return 0;
	return stringtoid( buffer ) ;
}

C3_CORE_DLL_API
DWORD real_name ( const char *filename )
{
/*	int i;
	for (i=0;filename[i];i++) 
	{
		if (filename[i]=='/') 
		{
			return string_id ( filename+i+1 ) ;
		}
	}
	if ( i == 0 ) return 0 ; */
	return string_id ( filename ) ;
}

C3_CORE_DLL_API
BOOL MyDnpFileOpen ( const char *filename )
{
	return g_objDnFile.OpenFile(filename);
}

C3_CORE_DLL_API
void* OpenFilePointer(const char* pszFile,unsigned long &usFileSize)
{
	return g_objDnFile.GetMPtr(pszFile,usFileSize);
}