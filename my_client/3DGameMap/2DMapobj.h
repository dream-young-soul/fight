
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------
//2DMapObj.h
//----------------------------------------------------

#ifndef _2DMAPOBJ_H
#define _2DMAPOBJ_H
#include "MapObj.h"
#include "BaseFunc.h"
//----------------------------------------------------
class C2DMapObj : public CMapObj
{
public:
    C2DMapObj();
    ~C2DMapObj();
protected:
    CMyPos m_posCell;

    //------------------------------------------------------------
    // for mapobj
public:
    void GetWorldPos(CMyPos& posWorld);

    void GetPos(CMyPos& posCell);
    void SetPos(CMyPos posCell);

    virtual void Show(void* pInfo) {}
    virtual void Process(void* pInfo) {}

    virtual int GetObjType   () {return MAP_NONE;}
    virtual BOOL IsFocus() {return false;}
};

//----------------------------------------------------
#endif

