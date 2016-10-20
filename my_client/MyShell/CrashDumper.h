//crash_dumper_w32.h

#ifndef _CRASH_DUMPER_H_

#define _CRASH_DUMPER_H_


#include <windows.h>

class CrashDumper

{

public:

	CrashDumper();

	~CrashDumper();

	static bool _PlaceHolder();

private:

	LPTOP_LEVEL_EXCEPTION_FILTER m_OriginalFilter;

	static LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);

};


namespace

{
	const bool bPlaceHolder = CrashDumper::_PlaceHolder();

}

#endif