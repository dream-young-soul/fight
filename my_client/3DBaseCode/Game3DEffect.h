
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Game3DEffect.h: interface for the CGame3DEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME3DEFFECT_H__9B54FCE4_E536_41BB_97E2_0081DA71527B__INCLUDED_)
#define AFX_GAME3DEFFECT_H__9B54FCE4_E536_41BB_97E2_0081DA71527B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
struct ShapeBackupInfo;
class C3DRolePart;

class CGame3DEffect
{
public:
    CGame3DEffect();
    virtual ~CGame3DEffect();

    void ResetShape ();

protected:
    OBJID m_idTexture;
    OBJID m_idEffect;
    ShapeBackupInfo* m_pShapeInfoBuf;
    int m_nShapes;

    void BackupShapeInfo();

public:
    virtual void Clear() {m_idTexture = ID_NONE; m_idEffect = ID_NONE;}
    virtual void Draw(C3DRolePart* pPart);
    virtual void Rotate(float x, float y, float z);
    virtual void Scale(float x, float y, float z);
};

#include <deque>
using namespace std;
typedef deque<CGame3DEffect*>DEQUE_GAME3DEFFECT;

#endif // !defined(AFX_GAME3DEFFECT_H__9B54FCE4_E536_41BB_97E2_0081DA71527B__INCLUDED_)
