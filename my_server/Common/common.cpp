
//**********************************************************
// ¥˙¬Î±‡º≠∆˜
//**********************************************************

// ≥£¡ø∫Õ≈‰÷√
// œ…Ω£–ﬁ£¨2002.9.14
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "common.h"

/////////////////////////////////////////////////////////////////////////////
// function
/////////////////////////////////////////////////////////////////////////////
bool	SafeCopy(char* pTarget, const char* pSource, int nBufLen /*= 0*/)
{
    try
    {
        if (pTarget)
        {
            pTarget[0] = 0;
            if (pSource)
            {
                if (nBufLen && (int)strlen(pSource) >= nBufLen)
                {
                    strncpy(pTarget, pSource, nBufLen - 1);
                    pTarget[nBufLen-1] = 0;
                    return false;
                }
                strcpy(pTarget, pSource);
                return true;
            }
        }
    }
    catch(...) { ::OutputDebugString("CATCH: *** SafeCopy() crash! ***\n"); }
    assert(!"SafeCopy()");
    return false;
}




