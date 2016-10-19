
//**********************************************************
// 代码编辑器
//**********************************************************

// Physics.cpp: implementation of the CPysics class.
//
//////////////////////////////////////////////////////////////////////

#include "Physics.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhysics::CPhysics()
{
}

CPhysics::~CPhysics()
{
}

//---------------------------------------------------------------------
DWORD CPhysics::S2DGetDistance(CMyPos posBegin, CMyPos posEnd)
{
    return (DWORD)sqrt(float(
                           (posBegin.x - posEnd.x) *
                           (posBegin.x - posEnd.x) +
                           (posBegin.y - posEnd.y) *
                           (posBegin.y - posEnd.y)
                       ));
}
//---------------------------------------------------------------------
BOOL CPhysics::S2DBMGetPos(int nV, float fA, float fT, CMyPos posBegin, CMyPos posEnd, CMyPos& posCurrent)
{
    double dwDistance = sqrt(float(
                                 (posBegin.x - posEnd.x) *
                                 (posBegin.x - posEnd.x) +
                                 (posBegin.y - posEnd.y) *
                                 (posBegin.y - posEnd.y)
                             ));
    if (dwDistance == 0)
    {
        posCurrent = posEnd;
        return true;
    }
    double nCompleteDistance = (int)(nV * fT + fA * fT * fT / 2);
    if (nCompleteDistance >= dwDistance)
    {
        posCurrent = posEnd;
        return true;
    }
    // Get Currentpos
    double dX = posBegin.x + (posEnd.x - posBegin.x) * nCompleteDistance / dwDistance;
    double dY = posBegin.y + (posEnd.y - posBegin.y) * nCompleteDistance / dwDistance;
    posCurrent.x = ::Double2Int(dX);
    posCurrent.y = ::Double2Int(dY);
    return false;
}
//---------------------------------------------------------------------
void CPhysics::S3Get3DDir(CMy3DPos posBegin, CMy3DPos posEnd, CMy3DDir& dir3D)
{
    // 计算平行角度
    double fPei = 3.14159;
    // 计算夹角先
    int nOffsetX = posEnd.x - posBegin.x;
    if (nOffsetX == 0)
    {
        nOffsetX = 1;
    }
    double fTan = (posEnd.y - posBegin.y) * 1.0 / nOffsetX;
    if (fTan < 0)
    {
        fTan = -fTan;
    }
    double fHorizontal = atan(fTan);
    // 判断在哪个象限，取附加数值
    int nX = posEnd.x - posBegin.x;
    int nY = posEnd.y - posBegin.y;
    if ((nX > 0) && (nY < 0))
    {
        // 第一象限
        dir3D.fHorizontal = (float)(fHorizontal + fPei / 2);
    }
    else if ((nX < 0) && (nY >= 0))
    {
        // 第二象限
        dir3D.fHorizontal = (float)(fHorizontal - fPei / 2);
    }
    else if ((nX < 0) && (nY < 0))
    {
        // 第三象限
        dir3D.fHorizontal = (float)(-fPei / 2 - fHorizontal);
    }
    else
    {
        // 第四象限
        dir3D.fHorizontal = (float)(fPei / 2 - fHorizontal);
    }
    // 计算垂直角度
    // 计算夹角先
    fTan = abs(posEnd.z - posBegin.z) / sqrt(float((posEnd.y - posBegin.y) * (posEnd.y - posBegin.y) + (posEnd.x - posBegin.x) * (posEnd.x - posBegin.x)));
    double fVertical = atan(fTan);
    // 判断在哪个象限，取附加数值
    int nZ = posEnd.z - posBegin.z;
    if (nZ >= 0 )
    {
        dir3D.fVertical = (float)fVertical;
    }
    else
    {
        dir3D.fVertical = (float)(-fVertical);
    }
}
//---------------------------------------------------------------------
BOOL CPhysics::IsPointInRect(CMyPos posPoint, CMyRect rect)
{
    if ((posPoint.x >= rect.l) &&
            (posPoint.x <= rect.r) &&
            (posPoint.y >= rect.b) &&
            (posPoint.y <= rect.t))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------
void CPhysics::S2DR45VP(CMyPos posTarget, CMyPos& posPoint)
{
    CMyPos posRule = {0, 32};
    // 真实长度在 32 和 16 之间
    // 先求真实长度RealDistance
    // 为求真实长度必须求和X轴夹角,用伪椭圆近似
    float fTag = 0.0;
    if (posTarget.x == 0)
    {
        fTag = 0.0;
    }
    else if (posTarget.y == 0)
    {
        fTag = 2.0;
    }
    else
    {
        fTag = (float)(1.0 * posTarget.x / posTarget.y);
    }
    if (fTag < 0)
    {
        fTag = -fTag;
    }
    if (fTag > 2.0)
    {
        fTag = 2.0;
    }
    float fDistance = (float)sqrt(float(posTarget.x * posTarget.x + posTarget.y * posTarget.y));
    // 靠近0为16， 靠近2.0为32
    float fRealDistance = (float)(16 + 16 * fTag * 1.0 / 2);
    CMyPos posRealTarget;
    // 求终点
    if ((posTarget.x == 0) && (posTarget.y != 0))
    {
        posRealTarget.x = 0;
        posRealTarget.y = (int)((posTarget.y / abs(posTarget.y)) * fRealDistance);
    }
    else if ((posTarget.y == 0) && (posTarget.x != 0))
    {
        posRealTarget.y = 0;
        posRealTarget.x = (int)((posTarget.x / abs(posTarget.x)) * fRealDistance);
    }
    else if ((posTarget.y == 0) && (posTarget.x == 0))
    {
        posRealTarget.x = 0;
        posRealTarget.y = 0;
    }
    else
    {
        posRealTarget.x = (int)(1.0 * posTarget.x * fRealDistance / fDistance);
        posRealTarget.y = (int)(1.0 * posTarget.y * fRealDistance / fDistance);
    }
    // 求映射点
    float fScale = (float)(posPoint.x * 1.0 / 32);
    if (fScale < 0)
    {
        fScale = -fScale;
    }
    posPoint.x = (int)(posRealTarget.x * fScale);
    posPoint.y = (int)(posRealTarget.y * fScale);
}
//---------------------------------------------------------------------
BOOL CPhysics::S2DCMGetPos(int nV, float fA, float fT, float fRadii, CMyPos posBegin, CMyPos posEnd, CMyPos& posCurrent, BOOL bSide)
{
    // Calculate the shortest distance from begin position to end position
    double dDistance = sqrt(float((posBegin.x - posEnd.x) * (posBegin.x - posEnd.x) + (posBegin.y - posEnd.y) * (posBegin.y - posEnd.y)));
    if ( dDistance == 0 )
    {
        posCurrent = posEnd;
        return true;
    }
    // Calculate the complete distance(The V&A are beeline, only the move track is curve)
    double dCompleteDistance = (int)( nV * fT + fA * fT * fT / 2 );
    if ( dCompleteDistance >= dDistance )
    {
        posCurrent = posEnd;
        return true;
    }
    // Calculate the curve offset from the shortest path
    double dLongRadii	= (int)( dDistance / 2 );
    double dShortRadii	= (int)( fRadii * dLongRadii );
    double dwCurveDistance = sqrt( dShortRadii * dShortRadii - ( dShortRadii * dShortRadii * ( dLongRadii - dCompleteDistance  ) * ( dLongRadii - dCompleteDistance ) ) / ( dLongRadii * dLongRadii ) );
    // The X axis and Y axis offset on the shortest path
    double dX = ( posEnd.x - posBegin.x ) * dCompleteDistance / dDistance;
    double dY = ( posEnd.y - posBegin.y ) * dCompleteDistance / dDistance;
    // Calculate the angle
    double dAngle = atan( dY / dX );
    if ( bSide )
    {
        dX -= sin(dAngle) * dwCurveDistance;
        dY += cos(dAngle) * dwCurveDistance;
    }
    else
    {
        dX += sin(dAngle) * dwCurveDistance;
        dY -= cos(dAngle) * dwCurveDistance;
    }
    dX += posBegin.x;
    dY += posBegin.y;
    posCurrent.x = ::Double2Int(dX);
    posCurrent.y = ::Double2Int(dY);
    return false;
}
//---------------------------------------------------------------------
