
//**********************************************************
// 代码编辑器
//**********************************************************

// define.h 全局类型和常量定义
// 仙剑修，2003.8.6
//////////////////////////////////////////////////////////////////////

#ifndef	DEFINE_H
#define	DEFINE_H

#include "common.h"
#include "T_SafePtr.h"

//////////////////////////////////////////////////////////////////////
// 通用常量
const int	IPSTR_SIZE			= 16;				// IP串空间
const int	_MAX_NAMESIZE		= 16;
const int	MAX_TALKSIZE		= 64;
const int	MAX_FIELDSIZE		= 256;				// 数据库中串域的长度


//////////////////////////////////////////////////////////////////////
// 类型
typedef	unsigned long	OBJID;

typedef	char			IPSTR[IPSTR_SIZE];
typedef	char			NAMESTR[_MAX_NAMESIZE];
typedef	char			SQLBUF[1024];
typedef	char			MSGBUF[1024];
typedef	char			TALKSTR[MAX_TALKSIZE];
typedef	char			FIELDBUF[MAX_FIELDSIZE];

//////////////////////////////////////////////////////////////////////
// 核心常量
const int	ID_NONE			= 0;

const bool	UPDATE_TRUE				= true;
const bool	UPDATE_FALSE			= false;
const bool	DELRECORD_TRUE			= true;
const bool	DELRECORD_FALSE			= false;
const bool	SYNCHRO_TRUE			= true;
const bool	SYNCHRO_FALSE			= false;

///////////////////////////////////////////////////////////////////////
// 行走方向
const int _DELTA_X[9] = { 0, -1, -1, -1, 0, 1, 1, 1, 0 };
const int _DELTA_Y[9] = { 1, 1, 0, -1, -1, -1, 0, 1, 0 };
const int MAX_DIRSIZE = 8;
inline int GetDirByPos(int nFromX, int nFromY, int nToX, int nToY)		// return MAX_DIRSIZE: error
{
    if (nFromX < nToX)
    {
        if (nFromY < nToY)
        {
            return 7;
        }
        else if (nFromY > nToY)
        {
            return 5;
        }
        else
        {
            return 6;
        }
    }
    else if (nFromX > nToX)
    {
        if (nFromY < nToY)
        {
            return 2;
        }
        else if (nFromY > nToY)
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }
    else // if (nFromX == nToX)
    {
        if (nFromY < nToY)
        {
            return 0;
        }
        else if (nFromY > nToY)
        {
            return 4;
        }
    }
    return MAX_DIRSIZE;
}



#endif // DEFINE_H
