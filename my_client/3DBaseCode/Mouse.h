
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef _MOUSE_H
#define _MOUSE_H

typedef struct
{
    int iPosX, iPosY;
    int iEvent;
    DWORD dwDownTime;
} CMouseInfo;

// mouse event define
const int _MOUSE_NONE		= 0;
const int _MOUSE_CLICK		= 10;
const int _MOUSE_DOWN		= 01;
const int _MOUSE_HOLD		= 11;
const int _MOUSE_RIGHTCLICK	= -10;
const int _MOUSE_RIGHTDOWN	= -01;
const int _MOUSE_RIGHTHOLD	= -11;
const int _MOUSE_ON			= 20;

#endif
