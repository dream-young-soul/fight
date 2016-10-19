
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------//
// MapLayer.h
//----------------------------------------------------//

#ifndef _MAPLAYER_H
#define _MAPLAYER_H


//------------------------------------------------------
#include "BaseFunc.h"
//------------------------------------------------------
class CMapLayer
{
public:
    CMapLayer() {m_nMoveRateX = 100; m_nMoveRateY = 100;}
    virtual ~CMapLayer() {;}
public:
    // define layer type ...
    enum {LAYER_NONE = 0, LAYER_TERRAIN, LAYER_FLOOR, LAYER_INTERACTIVE, LAYER_SCENE, LAYER_SKY, LAYER_SURFACE};
public:
    virtual void Show(CMyPos posShow);
    virtual void Process(void* pInfo);
    virtual int  GetType();

public:
    void	SetMoveRateX(int nMoveRateX) {m_nMoveRateX = nMoveRateX;}
    void	SetMoveRateY(int nMoveRateY) {m_nMoveRateY = nMoveRateY;}
    int		GetMoveRateX() {return m_nMoveRateX;}
    int		GetMoveRateY() {return m_nMoveRateY;}


private:
    int	m_nMoveRateX;
    int	m_nMoveRateY;
};
//------------------------------------------------------

#endif
