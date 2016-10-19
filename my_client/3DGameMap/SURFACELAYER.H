//-------------------------------------------------------
// SurfaceLayer.h
//-------------------------------------------------------
#ifndef _SURFACE_LAYER
#define _SURFACE_LAYER
//-------------------------------------------------------
#include "MapLayer.h"
//-------------------------------------------------------
class CSurfaceLayer : public CMapLayer
{
public:
	CSurfaceLayer();
	~CSurfaceLayer();
public:
	void Show(CMyPos posShow);
	void Process(void* pInfo);
	int  GetType();
public:
	static CSurfaceLayer* CreateNew();
	
};
//-------------------------------------------------------
#endif