#ifndef _c3_camera_h_
#define _c3_camera_h_

#ifdef	C3_CORE_DLL_EXPORTS
#define C3_CORE_DLL_API __declspec ( dllexport )
#else
#define C3_CORE_DLL_API __declspec ( dllimport )
#endif

#include "c3_common.h"

/* 摄影机结构 */
typedef struct
{
	char				*lpName;		// 名字
	D3DXVECTOR3			*lpFrom;		// 位置
	D3DXVECTOR3			*lpTo;			// 目的
	float				fNear;			// 近平面
	float				fFar;			// 远平面
	float				fFov;			// 视域

	DWORD				dwFrameCount;
	int					nFrame;
} C3Camera;
C3_CORE_DLL_API
void Camera_Clear ( C3Camera *lpCamera );

C3_CORE_DLL_API
BOOL Camera_Load ( C3Camera **lpCamera,
				   char *lpName,
				   DWORD dwIndex );
C3_CORE_DLL_API
BOOL Camera_Save ( char *lpName, C3Camera *lpCamera, BOOL bNew );
C3_CORE_DLL_API
void Camera_Unload ( C3Camera **lpCamera );

C3_CORE_DLL_API
void Camera_NextFrame ( C3Camera *lpCamera, int nStep );

/* 
	建造视矩阵
	----------
	当摄影机的位置或目的改变后需要调用此函数
*/
C3_CORE_DLL_API
BOOL Camera_BuildView ( C3Camera *lpCamera, BOOL bSet = true );
/*
	建造投影矩阵
	------------
	当摄影机的远近平面或视域改变后需要调用此函数
*/
C3_CORE_DLL_API
BOOL Camera_BuildProject ( C3Camera *lpCamera, BOOL bSet = true );
/*
	建造平行投影矩阵
	------------
	当摄影机的远近平面或视域改变后需要调用此函数
*/
C3_CORE_DLL_API
BOOL Camera_BuildOrtho ( C3Camera *lpCamera,
						 float fWidth,
						 float fHeight,
						 BOOL bSet = true );

C3_CORE_DLL_API
void Camera_Process1stRotate ( C3Camera *lpCamera,
							   float fHRadian,
							   float fVRadian );
C3_CORE_DLL_API
void Camera_Process1stTranslate ( C3Camera *lpCamera,
								  float fDirRandian,
								  float fStep );
C3_CORE_DLL_API
void Camera_ProcessXYTranslate ( C3Camera *lpCamera,
								 float fDirRandian,
								 float fStep );

#endif