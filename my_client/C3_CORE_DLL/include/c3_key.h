#ifndef _c3_key_h_
#define _c3_key_h_

#include "c3_main.h"
#include "c3_common.h"

struct C3Frame
{
	int			nFrame;				// ¹Ø¼üÖ¡Î»ÖÃ
	float		fParam[1];			// float param
	BOOL		bParam[1];			// bool param
	int			nParam[1];			// int param
};
struct C3Key
{
	DWORD		dwAlphas;
	C3Frame*	lpAlphas;

	DWORD		dwDraws;
	C3Frame*	lpDraws;

	DWORD		dwChangeTexs;
	C3Frame*	lpChangeTexs;
};

C3_CORE_DLL_API
void Key_Clear ( C3Key *lpKey );
C3_CORE_DLL_API
BOOL Key_Load ( C3Key **lpKey, char *lpName, DWORD dwIndex );
C3_CORE_DLL_API
BOOL Key_Save ( char *lpName, C3Key *lpKey, BOOL bNew );
C3_CORE_DLL_API
void Key_Unload ( C3Key **lpKey );

C3_CORE_DLL_API
BOOL Key_ProcessAlpha ( C3Key *lpKey, DWORD dwFrame, DWORD dwFrames, float* fReturn );
C3_CORE_DLL_API
BOOL Key_ProcessDraw ( C3Key *lpKey, DWORD dwFrame, BOOL* bReturn );
C3_CORE_DLL_API
BOOL Key_ProcessChangeTex ( C3Key *lpKey, DWORD dwFrame, int* nReturn );

#endif
