
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#ifndef		_M_MAP_H_
#define		_M_MAP_H_

#ifdef _BCB
#define sqrtf sqrt
#define	__max max
#endif

#include "c3_common.h"
#include "c3_main.h"
#include "c3_texture.h"
#include "c3_camera.h"
#include "nd_bitmap.h"

#define MAP_VERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
struct MAPVERTEX
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

class CElement
{
public:
    MAPVERTEX			m_Vec[4];
    int					m_nTex;
public:
    CElement ();
    virtual ~CElement ();

    BOOL Create ( LPCSTR lpFilename );
    void Release ( void );

    static void Prepare ( void );
    BOOL Draw ( int nOffsetX, int nOffsetY );

    void GetSize	(UINT& uWidth, UINT& uHeight);
};

class CMapBackground
{
public:
    DWORD				m_dwXBlock;
    DWORD				m_dwYBlock;
    POINT				m_Coordinate;
    CElement*			lpElement;
    C3Camera*			m_camera;

public:
    CMapBackground ();
    virtual ~CMapBackground ();

    BOOL Create		(const char* pszDir, DWORD dwXBlock, DWORD dwYBlock);
    void Release	(void);

    void SetViewport(int nX, int nY);
    void Draw		(void);
    void GetSize	(UINT& uWidth, UINT& uHeight);

    static void Prepare ( void );
};

#endif
