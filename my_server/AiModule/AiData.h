
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// AiCenter.cpp: implementation of the CAiCenter class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>

const int	FACT_ID_FIRST				= 1000000;			// 保留前面的为TEMP_FACT的ID，用于FactQueue。
//////////////////////////////////////////////////////////////////////
inline char*	SystemFunction(int idx)
{
    static char*	g_setSysFunc[] =
    {
        // debug or operator
        "dump",					// for debug, dump this rule
        "keep",					// (int), only for change mode, keep(0) for keep fact forever
        // function
        "now",					// (int), 当前时间
        "less",					// (left,right), 如果　left < right　则正确
        "less_equ",				// (left,right), 如果　left <= right　则正确
        "strstr",				// (str,substr), 如果str中包含有子串substr，正确
        "percent",				// (int), 0 ~ 100, 有n%的机会为TRUE。

        NULL
    };
    CHECKF(idx < sizeof(g_setSysFunc));
    return g_setSysFunc[idx];
}

enum
{
    SYS_FUNC_DUMP,			// "dump",					// for debug, dump this rule
    SYS_FUNC_KEEP,			// "keep",					// (int), only for change mode, keep(0) for keep fact forever

    SYS_FUNC_NOW,			// "now",					// (int), 当前时间
    SYS_FUNC_LESS,			// "less",					// (left,right), 如果　left < right　则正确
    SYS_FUNC_LESS_EQU,		// "less_equ",				// (left,right), 如果　left <= right　则正确
    SYS_FUNC_STRSTR,		// "strstr",				// (str,substr), 如果str中包含有子串substr，正确
    SYS_FUNC_PERCENT,		// "percent",				// (int), 0 ~ 100, 有n%的机会为TRUE。
};
