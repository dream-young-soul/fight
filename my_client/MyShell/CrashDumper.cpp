#include "stdafx.h"
#include "CrashDumper.h"
#include <windows.h>
#include <tchar.h>
#include <dbghelp.h>
#include <string>


#ifdef UNICODE    
#     define tstring wstring     
#else       
#     define tstring string 
#endif


#pragma comment(lib, "dbghelp.lib")

CrashDumper dumper;

CrashDumper::CrashDumper()
{
	m_OriginalFilter = SetUnhandledExceptionFilter(ExceptionFilter);
}

CrashDumper::~CrashDumper()
{
	SetUnhandledExceptionFilter(m_OriginalFilter);
}

LONG WINAPI CrashDumper::ExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	bool bDumpOK = false;
	DWORD dwProcess = GetCurrentProcessId();
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcess);
	if (hProcess != INVALID_HANDLE_VALUE)
	{

		TCHAR szPath[MAX_PATH];
		if (GetModuleFileName(NULL, szPath, sizeof(szPath)))
		{
			std::tstring strDumpFileName = szPath;
			strDumpFileName += TEXT(".dmp");
			HANDLE hFile = CreateFile(strDumpFileName.c_str(), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, NULL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				MINIDUMP_EXCEPTION_INFORMATION exception_information;
				exception_information.ThreadId = GetCurrentThreadId();
				exception_information.ExceptionPointers = ExceptionInfo;
				exception_information.ClientPointers = TRUE;
				if (MiniDumpWriteDump(hProcess, dwProcess, hFile, MiniDumpNormal, &exception_information, NULL, NULL))
				{
					bDumpOK = true;
				}
				CloseHandle(hFile);

			}
		}
		CloseHandle(hProcess);
	}
	if (bDumpOK)
		MessageBox(NULL, TEXT("本程序遇到未处理的异常，MiniDump文件已经生成在程序的运行目录。"), TEXT("提示"), MB_OK);
	else
		MessageBox(NULL, TEXT("本程序遇到未处理的异常，生成MiniDump文件失败。"), TEXT("提示"), MB_OK);
	return EXCEPTION_EXECUTE_HANDLER;

}


bool CrashDumper::_PlaceHolder() {return true;}