#include "..\include\c3_key.h"

C3_CORE_DLL_API
void Key_Clear ( C3Key *lpKey )
{
	lpKey->dwAlphas = 0;
	lpKey->lpAlphas = 0;

	lpKey->dwDraws = 0;
	lpKey->lpDraws = 0;

	lpKey->dwChangeTexs = 0;
	lpKey->lpChangeTexs = 0;
}

C3_CORE_DLL_API
BOOL Key_ProcessAlpha ( C3Key *lpKey, DWORD dwFrame, DWORD dwFrames, float* fReturn )
{
	// alpha
	int sindex = -1;
	int eindex = -1;
	for ( int n = 0; n < ( int )lpKey->dwAlphas; n++ )
	{
		if ( lpKey->lpAlphas[n].nFrame <= ( int )dwFrame )
		{
			if ( sindex == -1 || n > sindex)
				sindex = n;
		}
		if ( lpKey->lpAlphas[n].nFrame > ( int )dwFrame )
		{
			if ( eindex == -1 || n < eindex )
				eindex = n;
		}
	}
	if ( sindex == -1 && eindex > -1 )
	{
		*fReturn = lpKey->lpAlphas[eindex].fParam[0];
	}
	else
	if ( sindex > -1 && eindex == -1 )
	{
		*fReturn = lpKey->lpAlphas[sindex].fParam[0];
	}
	else
	if ( sindex > -1 && eindex > -1 )
	{
		*fReturn = lpKey->lpAlphas[sindex].fParam[0] + 
				   ( ( float )( dwFrame - lpKey->lpAlphas[sindex].nFrame ) / ( float )( lpKey->lpAlphas[eindex].nFrame - lpKey->lpAlphas[sindex].nFrame ) ) * 
					( lpKey->lpAlphas[eindex].fParam[0] - lpKey->lpAlphas[sindex].fParam[0] );
	}
	else
		return false;
	return true;
}
C3_CORE_DLL_API
BOOL Key_ProcessDraw ( C3Key *lpKey, DWORD dwFrame, BOOL* bReturn )
{
	// draw
	for ( int n = 0; n < ( int )lpKey->dwDraws; n++ )
	{
		if ( lpKey->lpDraws[n].nFrame == ( int )dwFrame )
		{
			*bReturn = lpKey->lpDraws[n].bParam[0];
			return true;
		}
	}
	return false;
}
C3_CORE_DLL_API
BOOL Key_ProcessChangeTex ( C3Key *lpKey, DWORD dwFrame, int* nReturn )
{
	// changetex
	for ( int n = 0; n < ( int )lpKey->dwChangeTexs; n++ )
	{
		if ( lpKey->lpChangeTexs[n].nFrame == ( int )dwFrame )
		{
			*nReturn = lpKey->lpChangeTexs[n].nParam[0];
			return true;
		}
	}
	return false;
}
