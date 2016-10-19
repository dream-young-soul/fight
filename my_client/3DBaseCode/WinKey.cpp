
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
const int _MAX_KEYBUFFER	= 16;

int iAppendPt		= 0;
int iTakePt			= 0;

int iKeyAmount		= 0;
int iKeyBuffer[_MAX_KEYBUFFER];
//-----------------------------------------------------------------------------
void KeyClearBuffer(void)
{
    iAppendPt		= 0;
    iTakePt			= 0;
    iKeyAmount		= 0;
}

//-----------------------------------------------------------------------------
void KeyAppend(int iKeyPressed)
{
    if (iKeyAmount >= _MAX_KEYBUFFER)
    {
        return;
    }
    else
    {
        iKeyBuffer[iAppendPt]	= iKeyPressed;
        iAppendPt	= (iAppendPt + 1) % _MAX_KEYBUFFER;
        iKeyAmount++;
        return;
    }
}

//-----------------------------------------------------------------------------
int KeyGet(void)
{
    if (iKeyAmount == 0)
    {
        return 0;
    }
    int iReturnKey	= iKeyBuffer[iTakePt];
    iTakePt = (iTakePt + 1) % _MAX_KEYBUFFER;
    iKeyAmount--;
    return iReturnKey;
}

//-----------------------------------------------------------------------------
bool KeyUnGet(void)
{
    if (iKeyAmount >= _MAX_KEYBUFFER)
    {
        return false;
    }
    iTakePt	= (iTakePt + _MAX_KEYBUFFER - 1) % _MAX_KEYBUFFER;
    iKeyAmount++;
    return true;
}

//-----------------------------------------------------------------------------
