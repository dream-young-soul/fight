//--------------------------------------------------------------------------------------
// File Name: DnFile.cpp
// Create by: Huang Yuhang
// Create on: 2003/11/16 20:13
//--------------------------------------------------------------------------------------
#pragma warning(disable:4786)
#include "../include/DnFile.h"
#include "Windows.h"
#include    <io.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const unsigned long DAWNFILE_BUFFERSIZE	=	1024*1024;
CRITICAL_SECTION	g_DnFileCriticalSection;
CDnFile g_objDnFile;
//--------------------------------------------------------------------------------------
CDnFile::CDnFile()
{
	m_pBuffer		= NULL;
	m_pExtendBuffer	= NULL;
	m_fpExtend		= NULL;
	this->Create();
	InitializeCriticalSection ( &g_DnFileCriticalSection );
}

CDnFile::~CDnFile()
{
	this->Destroy();
	LeaveCriticalSection ( &g_DnFileCriticalSection );
	DeleteCriticalSection ( &g_DnFileCriticalSection );
}
//--------------------------------------------------------------------------------------
void CDnFile::BeforeUseDnFile()
{
	EnterCriticalSection ( &g_DnFileCriticalSection );
}
//--------------------------------------------------------------------------------------
void CDnFile::AfterUseDnFile()
{
	LeaveCriticalSection ( &g_DnFileCriticalSection );
}
//--------------------------------------------------------------------------------------
FILE*	CDnFile::GetFPtr(const char* pszFile, unsigned long& usFileSize)
{
	this->ClearPtr();
	if(!pszFile)
		return false;
	char szFileCopy[512];
	strcpy(szFileCopy, pszFile);
	_strlwr(szFileCopy);
	unsigned long uFileNameLength = strlen(szFileCopy);
	for(unsigned long i = 0; i < uFileNameLength; i ++)
	{
		if(szFileCopy[i] == '/')
		{
			szFileCopy[i] = '\\';
		}
	}
	unsigned long idFile = this->GenerateID(szFileCopy);
	if(!this->CheckDisperseFile(idFile))
	{
		unsigned long uFileNameLength = strlen(szFileCopy);
		for(unsigned long i = 0; i < uFileNameLength; i ++)
		{
			if(szFileCopy[i] == '\\')
			{
				szFileCopy[i] = '\0';
			}
		}
		unsigned long idPack = this->GenerateID(szFileCopy);
		map<unsigned long, DnpInfo*>::iterator iter = m_mapDnp.find(idPack);
		if(iter != m_mapDnp.end())
		{
			DnpInfo* pInfoDnp = iter->second;
			if(pInfoDnp)
			{
				map<unsigned long, FileIndexInfo*>::iterator iterFile = pInfoDnp->mapIndex.find(idFile);
				if(iterFile != pInfoDnp->mapIndex.end())
				{
					fseek(pInfoDnp->fpDnp, iterFile->second->uOffset, SEEK_SET);
					usFileSize = iterFile->second->uSize;
					return pInfoDnp->fpDnp;
				}
			}
		}
	}
	m_fpExtend = fopen(pszFile, "rb");
	if(m_fpExtend)
		usFileSize = _filelength(_fileno(m_fpExtend));
	return 	m_fpExtend;	
}
//--------------------------------------------------------------------------------------
void*	CDnFile::GetMPtr(const char* pszFile, unsigned long& usFileSize)
{
	//优先从绝对路径加载 2013.9.15
	FILE* fp = fopen(pszFile,"rb");
	if(fp != NULL)
	{
		fseek(fp,0,SEEK_END);
		unsigned long nLen = ftell(fp);
		usFileSize = nLen;
		if(usFileSize > 0)
		{
			fseek(fp,0,SEEK_SET);
			m_pExtendBuffer = new unsigned char[nLen];
			fread(m_pExtendBuffer,nLen,1,fp);
		}else m_pExtendBuffer = NULL;
		fclose(fp);
		return m_pExtendBuffer;
	}
	this->ClearPtr();
	if(!pszFile)
		return false;
	char szFileCopy[512];
	strcpy(szFileCopy, pszFile);
	_strlwr(szFileCopy);
	unsigned long uFileNameLength = strlen(szFileCopy);
	for(unsigned long i = 0; i < uFileNameLength; i ++)
	{
		if(szFileCopy[i] == '/')
		{
			szFileCopy[i] = '\\';
		}
	}
	unsigned long idFile = this->GenerateID(szFileCopy);
	if(!this->CheckDisperseFile(idFile))
	{
		unsigned long uFileNameLength = strlen(szFileCopy);
		for(unsigned long i = 0; i < uFileNameLength; i ++)
		{
			if(szFileCopy[i] == '\\')
			{
				szFileCopy[i] = '\0';
			}
		}
		unsigned long idPack = this->GenerateID(szFileCopy);
		map<unsigned long, DnpInfo*>::iterator iter = m_mapDnp.find(idPack);
		if(iter != m_mapDnp.end())
		{
			DnpInfo* pInfoDnp = iter->second;
			if(pInfoDnp)
			{
				map<unsigned long, FileIndexInfo*>::iterator iterFile = pInfoDnp->mapIndex.find(idFile);
				if(iterFile != pInfoDnp->mapIndex.end())
				{
					FileIndexInfo* pInfo = iterFile->second;
					fseek(pInfoDnp->fpDnp, iterFile->second->uOffset, SEEK_SET);
					if(iterFile->second->uSize > DAWNFILE_BUFFERSIZE)
					{
						m_pExtendBuffer = new unsigned char[iterFile->second->uSize];
						if(!m_pExtendBuffer)
							return NULL;
						usFileSize = iterFile->second->uSize;
						fread(m_pExtendBuffer, sizeof(char), iterFile->second->uSize, pInfoDnp->fpDnp);
						return m_pExtendBuffer;
					}
					else
					{
						fread(m_pBuffer, sizeof(char), iterFile->second->uSize, pInfoDnp->fpDnp);
						usFileSize = iterFile->second->uSize;
						return m_pBuffer;
					}
				}
			}
		}
	}
	return 	NULL;	
}
//--------------------------------------------------------------------------------------
bool	CDnFile::CheckDisperseFile(const unsigned long uFileID)
{
	int nAmount = m_mapDisperseFiles.size();
	map<unsigned long, unsigned char>::iterator iter = m_mapDisperseFiles.find(uFileID);
	if(iter == m_mapDisperseFiles.end())
		return false;
	return true;
}
//--------------------------------------------------------------------------------------
bool	CDnFile::CheckDisperseFile(const char* pszFile)
{
	if(!pszFile)
		return false;
	char szFileCopy[512];
	strcpy(szFileCopy, pszFile);
	_strupr(szFileCopy);
	unsigned long idFile = this->GenerateID(pszFile);
	map<unsigned long, unsigned char>::iterator iter = m_mapDisperseFiles.find(idFile);
	if(iter == m_mapDisperseFiles.end())
		return false;
	return true;
}
//--------------------------------------------------------------------------------------
void	CDnFile::AddDisperseFile(const char* pszFile)
{
	if(!pszFile)
		return;
	char szFileCopy[512];
	strcpy(szFileCopy, pszFile);
	_strlwr(szFileCopy);
	unsigned long uFileNameLength = strlen(szFileCopy);
	for(unsigned long i = 0; i < uFileNameLength; i ++)
	{
		if(szFileCopy[i] == '/')
		{
			szFileCopy[i] = '\\';
		}
	}
	unsigned long idFile = this->GenerateID(szFileCopy);
	m_mapDisperseFiles[idFile] = 0;
}

void CDnFile::ProcessDir ( const char* pszDir)
{
	_finddata_t filestruct;
	int p = 0;
	int fn = 0;
	char szSearch[255];
	if (strlen(pszDir) == 0)
	{
		strcpy(szSearch , "*.*");
	}
	else
	{
		strcpy(szSearch , pszDir);
		strcat(szSearch , "/*.*");
	}
	
	int hnd = _findfirst(szSearch , &filestruct);
	if(hnd == -1) 
	{
		return;
	}
	do
	{ 
		char szFullName[MAX_PATH] = "";
		if (strlen(pszDir) > 0)
		{
			sprintf(szFullName , "%s/%s" , pszDir , filestruct.name);		
		}
		else
		{
			strcpy(szFullName , filestruct.name);
		}
		
		if (!(filestruct.attrib & _A_SUBDIR)) 
		{
			// Process the file name
			char szFileCopy[512];
			strcpy(szFileCopy, szFullName);
			_strlwr(szFileCopy);
			unsigned long uFileNameLength = strlen(szFileCopy);
			for(unsigned long i = 0; i < uFileNameLength; i ++)
			{
				if(szFileCopy[i] == '/')
				{
					szFileCopy[i] = '\\';
				}
			}
			DWORD id = this->GenerateID( szFileCopy ) ;
			m_mapDisperseFiles[id]=0;
		}
		else // Directory
		{
			if (strcmp(filestruct.name , "..") != 0 && strcmp(filestruct.name, ".") != 0)
			{
				ProcessDir(szFullName);
			}
		}
	}while(!_findnext(hnd , &filestruct)); 
}
//--------------------------------------------------------------------------------------
bool	CDnFile::OpenFile(const char* pszFile)
{

	if(!pszFile)
		return false;
	// 未打包目录中的文件列表预读, 优化未打包文件搜寻
	char* p = (char*)strchr ( pszFile, '.' ) ;
	if ( p )
	{
		char szDirName[MAX_PATH] = "";
		strncpy ( szDirName, pszFile, p - pszFile ) ;
		
		ProcessDir ( szDirName) ;
	}	
	char szFileCopy[512];
	strcpy(szFileCopy, pszFile);
	_strlwr(szFileCopy);

	if(strlen(pszFile) < 4)
		return false;
	int nFileNameLength = strlen(szFileCopy);
	if(szFileCopy[nFileNameLength-1] != 'p' || szFileCopy[nFileNameLength-2] != 'n' ||
		szFileCopy[nFileNameLength-3] != 'd' || szFileCopy[nFileNameLength-4] != '.')
		return false;
	szFileCopy[nFileNameLength-4] = '\0';

	unsigned long idFile = this->GenerateID(szFileCopy);
	map<unsigned long, DnpInfo*>::iterator iter = m_mapDnp.find(idFile);
	if(iter != m_mapDnp.end())
		return false;

	DnpInfo* pDnpInfo = new DnpInfo;
	char szTemp[100];
	sprintf(szTemp, "new 0x%x\n", pDnpInfo);
	//::OutputDebugString(szTemp);
	if(!pDnpInfo)
		return false;

	pDnpInfo->fpDnp = fopen(pszFile, "rb");
	if(!pDnpInfo->fpDnp)
	{
		delete(pDnpInfo);
		return false;
	}

	char szFileHeader[32]="";
	fread(szFileHeader, sizeof(char), 32, pDnpInfo->fpDnp);
	if(strcmp(szFileHeader, "DawnPack.TqDigital") != 0)
	{
		fclose(pDnpInfo->fpDnp);
		delete(pDnpInfo);
		return false;
	}

	unsigned long uVersion = 0;
	fread(&uVersion, sizeof(unsigned long), 1, pDnpInfo->fpDnp);
	if(uVersion != 1000)
	{
		fclose(pDnpInfo->fpDnp);
		delete(pDnpInfo);
		return false;
	}

	unsigned long uFileAmount = 0;
	fread(&uFileAmount, sizeof(unsigned long), 1, pDnpInfo->fpDnp);

	for(unsigned long uCounter = 0; uCounter < uFileAmount; uCounter++)
	{
		unsigned long idSubFile;
		unsigned long uFileOffset;
		unsigned long uFileSize;
		fread(&idSubFile, sizeof(unsigned long), 1, pDnpInfo->fpDnp);
		fread(&uFileSize, sizeof(unsigned long), 1, pDnpInfo->fpDnp);
		fread(&uFileOffset, sizeof(unsigned long), 1, pDnpInfo->fpDnp);

		FileIndexInfo* pinfoIndex = new FileIndexInfo;
		if(!pinfoIndex)
			return false;
		pinfoIndex->uOffset = uFileOffset;
		pinfoIndex->uSize = uFileSize;
		pDnpInfo->mapIndex[idSubFile] = pinfoIndex;
	}
	m_mapDnp[idFile] = pDnpInfo;
	return true;
}
//--------------------------------------------------------------------------------------
void	CDnFile::ClearPtr()
{
	if(m_pExtendBuffer)
	{
		delete[] m_pExtendBuffer;
		m_pExtendBuffer = NULL;
	}
	if(m_fpExtend)
	{
		fclose(m_fpExtend);
		m_fpExtend = NULL;
	}
}
//--------------------------------------------------------------------------------------
void	CDnFile::CloseFile(const char* pszFile)
{
	if(!pszFile)
		return;
	char szFileCopy[512];
	strcpy(szFileCopy, pszFile);
	_strupr(szFileCopy);
	unsigned long idFile = this->GenerateID(pszFile);
	map<unsigned long, DnpInfo*>::iterator iter = m_mapDnp.find(idFile);
	if(iter != m_mapDnp.end())
		return;
	DnpInfo* pDnp = iter->second;
	if(pDnp)
	{
		fclose(pDnp->fpDnp);
		pDnp->mapIndex.clear();
		delete(pDnp);
	}
	m_mapDnp.erase(iter);
}
//--------------------------------------------------------------------------------------
void	CDnFile::Destroy()
{
	map<unsigned long, DnpInfo*>::iterator iter = m_mapDnp.begin();
	while(iter != m_mapDnp.end())
	{
		DnpInfo* pDnp = iter->second;
		if(pDnp)
		{
			fclose(pDnp->fpDnp);
			map<unsigned long, FileIndexInfo*>::iterator iterIndex = pDnp->mapIndex.begin();
			while(iterIndex != pDnp->mapIndex.end())
			{
				FileIndexInfo* pInfo = iterIndex->second;
				if(pInfo)
					delete pInfo;
				iterIndex ++;
			}
			pDnp->mapIndex.clear();
			delete(pDnp);
			char szTemp[100];
			sprintf(szTemp, "delete 0x%x\n", pDnp);
			//::OutputDebugString(szTemp);
		}
		iter ++;
	}
	m_mapDnp.clear();
	if(m_pBuffer)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	if(m_pExtendBuffer)
	{
		delete[] m_pExtendBuffer;
		m_pExtendBuffer = NULL;
	}
	if(m_fpExtend)
	{
		fclose(m_fpExtend);
		m_fpExtend = NULL;
	}
	m_mapDisperseFiles.clear();
}
//--------------------------------------------------------------------------------------
void	CDnFile::Create()
{
	this->Destroy();
	m_pBuffer = new unsigned char[DAWNFILE_BUFFERSIZE];
}
//--------------------------------------------------------------------------------------
unsigned long	CDnFile::GenerateID(const char* str)
{
	if(!str)
		return 0;
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
//#ifdef _DEBUG
//	char szTemp[512];
//	sprintf(szTemp, "%s FileID: %u\n", str,v);
//	::OutputDebugString(szTemp);	
//#endif
	return v;
}
//--------------------------------------------------------------------------------------