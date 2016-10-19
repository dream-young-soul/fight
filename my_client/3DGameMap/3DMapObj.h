
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//----------------------------------------------------
//3DMapObj.h
//----------------------------------------------------
#ifndef _3DMAPOBJ_H
#define _3DMAPOBJ_H
//----------------------------------------------------
#include "MapObj.h"
//----------------------------------------------------
class C3DMapObj : public CMapObj
{
public:
    C3DMapObj();
    ~C3DMapObj();
protected:
    CMyPos m_posCell;

    //------------------------------------------------------------
    // for mapobj
public:
    virtual void GetPos(CMyPos& posCell);
    virtual void SetPos(CMyPos posCell);

    virtual void Show(void* pInfo) {;}
    virtual void Process(void* pInfo) {;}
    virtual int  GetObjType() {return MAP_3DOBJ;}
    virtual BOOL IsFocus() {return false;}
    virtual void GetWorldPos(CMyPos& posWorld);
};
//----------------------------------------------------
#endif
