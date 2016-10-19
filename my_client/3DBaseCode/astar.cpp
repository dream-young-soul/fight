
//**********************************************************
// 代码编辑器
//**********************************************************


//		地图坐标原点在左上角

#include <string.h>
#include "astar.h"
#include "error_code.h"


#define NULL				0

const int CHECKSTEP			= 25;					//目标行走检测
const int CONTINUESTEP		= 15;					//追加搜索步

const int MAN_BODILY4_COORDINATE[ BODILY4 ][ 2 ] = { { 0, 0 }, { -1, 0 }, { 0, -1 }, { -1, -1 } };
//	   3 2
//	   1 0
const int COORDINATE_8WAY[ 8 ][ 2 ] = { { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },  };
//     3 4 5
//     2   6
//     1 0 7
const char DISTANCE2DIRECTION[ 16 ] =		{ -1, 0, -1, 4, 6, 7, -1, 5, -1, -1, -1, -1, 2, 1, -1, 3 };
const char DISTANCE2MIRRORDIRECTION[ 16 ] = { -1, 4, -1, 0, 2, 3, -1, 1, -1, -1, -1, -1, 6, 5, -1, 7 };
//const int StepLong[ 8 ] = { 2, 3, 2, 3, 2, 3, 2, 3 };
const int StepLong[ 8 ] = { 4, 6, 4, 6, 4, 6, 4, 6 };
const char MirrorTrasDirectTab[ 8 ] = { 4, 5, 6, 7, 0, 1, 2, 3 };

//-----------------------------------------------------------------------------

void ASTAR_8WAY::CHAINASN_O::Insert( ASTAR_NODE* pSuccessor )
{
    ASTAR_NODE* pTmp1, * pTmp2;
    int nF;
    pSuccessor->cWhere = INOPEN;
    if ( pNode == NULL )
    {
        pSuccessor->pNextNode = pNode;
        pNode = pSuccessor;
        return;
    }
    // insert into OPEN successor wrt f
    nF = pSuccessor->nF;
    pTmp1 = pNode;
    pTmp2 = pNode->pNextNode;
    while ( ( pTmp2 != NULL) && ( pTmp2->nF < nF) )
    {
        pTmp1 = pTmp2;
        pTmp2 = pTmp2->pNextNode;
    }
    pSuccessor->pNextNode = pTmp2;
    pTmp1->pNextNode = pSuccessor;
}

//-----------------------------------------------------------------------------

ASTAR_8WAY::ASTAR_8WAY( void )
{
    nMapHeight = nMapWidth = 0;
    pAStarIdx = NULL;
    unPathCount = 0;
}

//初始化
int ASTAR_8WAY::InitAStar( int nSizeX, int nSizeY )
{
    int i;
    nMapWidth = nSizeX;
    nMapHeight = nSizeY;
    if ( ( pAStarIdx = new ASTAR_IDX[ nMapWidth * nMapHeight ] ) == NULL )
    {
        return ERROR_OUTOFMEMORY;
    }
    ClearAstarIdx();
    FreeNodes();
    for( i = 0; i < BODILY4; i++ )
    {
        MAN_BODILY_OFFSET[ i ] = nMapWidth * MAN_BODILY4_COORDINATE[ i ][ 1 ] + MAN_BODILY4_COORDINATE[ i ][ 0 ];
    }
    MAN_BODILY_AROUND[ 0 ] = nMapWidth * COORDINATE_8WAY[ 0 ][ 1 ] + COORDINATE_8WAY[ 0 ][ 0 ];
    LINEOFF[ 0 ] = MAN_BODILY_AROUND[ 0 ];
    for( i = 1; i < 8; i++ )
    {
        MAN_BODILY_AROUND[ i ] = nMapWidth * COORDINATE_8WAY[ i ][ 1 ] + COORDINATE_8WAY[ i ][ 0 ];
        LINEOFF[ i ] = MAN_BODILY_AROUND[ i ] - MAN_BODILY_AROUND[ i - 1 ];
    }
    return ALL_OK;
}

inline void ASTAR_8WAY::ClearAstarIdx( void )
{
    memset( pAStarIdx, 0, sizeof( ASTAR_IDX ) * nMapWidth * nMapHeight );
}

void ASTAR_8WAY::SetSafelyZone( void )
{
    int i;
    for( i = 0; i < nMapWidth; i++ )
    {
        SetAStarAtr( i, 0, UNSTAY );
        SetAStarAtr( i, nMapHeight - 1, UNSTAY );
    }
    for( i = 0; i < nMapHeight; i++ )
    {
        SetAStarAtr( 0, i, UNSTAY );
        SetAStarAtr( nMapWidth - 1, i, UNSTAY );
    }
}

//释放
void ASTAR_8WAY::Release( void )
{
    if ( pAStarIdx ) { delete []pAStarIdx; }
    nMapHeight = nMapWidth = 0;
    pAStarIdx = NULL;
    unPathCount = 0;
}

//清除
void ASTAR_8WAY::FreeNodes( void )
{
    OpenChain.Clear();
    CloseChain.Clear();
    AstarStack.Clear();
    AstarNode.Clear();
}

//设置人占用
void ASTAR_8WAY::SetManInAStarAtr( int nCount, BODYSIZE cType )
{
    return;
    pAStarIdx[ nCount ].SetHasman();
    if ( cType == BODILY1 ) { return; }
    ASTAR_IDX* _AStarIdx = pAStarIdx + nCount;
    for( int i = 1; i < BODILY4; i++ )
    {
        _AStarIdx[ MAN_BODILY_OFFSET[ i ] ].SetHasman();
    }
}

//清除人占用
void ASTAR_8WAY::ClrManInAStarAtr( int nCount, BODYSIZE cType )
{
    pAStarIdx[ nCount ].SetNoman();
    if ( cType == BODILY1 ) { return; }
    ASTAR_IDX* _AStarIdx = pAStarIdx + nCount;
    for( int i = 1; i < BODILY4; i++ )
    {
        _AStarIdx[ MAN_BODILY_OFFSET[ i ] ].SetNoman();
    }
}

bool ASTAR_8WAY::TestStayHasMan( ASTAR_IDX* AStarIdx, BODYSIZE cType )
{
    if ( AStarIdx->IsUnstayCM() )
    {
        return false;
    }
    for( int i = 1; i < cType; i++ )
        if ( AStarIdx[ MAN_BODILY_OFFSET[ i ] ].IsUnstayCM() )
        {
            return false;
        }
    return true;
}

bool ASTAR_8WAY::TestStayNoMan( ASTAR_IDX* AStarIdx, BODYSIZE cType )
{
    if ( AStarIdx->IsUnstay() )
    {
        return false;
    }
    for( int i = 1; i < cType; i++ )
        if ( AStarIdx[ MAN_BODILY_OFFSET[ i ] ].IsUnstay() )
        {
            return false;
        }
    return true;
}

//寻路
int ASTAR_8WAY::FindPath( int nNowX, int nNowY, int nGoX, int nGoY, STEPDATA* pMan, int nFlags )
{
#ifndef _NOTUSETRY
    try
    {
#endif
        int nEndCode, nLimitStep;
        int nSX, nSY;
        if ( nGoX == nNowX && nGoY == nNowY )
        {
            return ATTARGET;
        }
        nEndCode = 0;
        pStepPtr = pMan;
        BSize = pMan->GetBodySize();
        if ( nFlags & CONTINUESEARCH ) 							//测试局部寻路
        {
            if ( ( CheckContinue( nNowX, nNowY, nGoX, nGoY ) ) == CONTINUESEARCH )
            {
                nFlags |= S_TO_E;								//不检测起点,强制正向搜索
                nEndCode |= CONTINUESEARCH;						//局部搜索
                nLimitStep = CONTINUESTEP * 3;
            }
            else
            {
                return NOENOUGHSTEP;    //剩余步数不足
            }
        }
        else if ( !( nFlags & NOCHECKFRONT ) )
        {
            nEndCode |= CheckFront( nNowX, nNowY, nGoX, nGoY, nFlags );		//目标封闭检测
            if ( nEndCode & ( HAVEPATH | NOPATH | UNWALKABLE ) )		//发现路线或被封死
            {
                return nEndCode;
            }
            nLimitStep = MAXSEARCHSTEP;
        }
        if ( nFlags & S_TO_E )
        {
            nSX = nNowX;	//正向搜索
            nSY = nNowY;
            nEndX = nGoX;
            nEndY = nGoY;
        }
        else
        {
            nSX = nGoX;	//反向搜索
            nSY = nGoY;
            nEndX = nNowX;
            nEndY = nNowY;
        }
        ASTAR_NODE* pBestNode, * pNode;
        int i, nEndN;
        FreeNodes();
        if ( ( pNode = AstarNode.CGetOne() ) == NULL )
        {
            goto error;
        }
        i = nMapWidth * nSY + nSX;										//检查目标点是否不可停留
        if ( nFlags & HAS_MAN )
        {
            ClrManInAStarAtr( nNowX, nNowY, BSize );
        }
        if ( !( nFlags & S_TO_E ) )
        {
            switch( BSize )
            {
            case BODILY1:
            default:
                if ( nFlags & HAS_MAN )
                {
                    if ( pAStarIdx[ i ].IsUnstayCM() )
                    {
                        SetManInAStarAtr( nNowX, nNowY, BODILY1 );
                        FreeNodes();
                        return nEndCode | TARGETNOPATH;
                    }
                }
                else if ( pAStarIdx[ i ].IsUnstay() )
                {
                    FreeNodes();
                    return nEndCode | TARGETNOPATH;
                }
                break;
            case BODILY4:
                if ( nFlags & HAS_MAN )
                {
                    if ( !TestStayHasMan( pAStarIdx + i, BODILY4 ) )
                    {
                        SetManInAStarAtr( nNowX, nNowY, BODILY4 );
                        FreeNodes();
                        return nEndCode | TARGETNOPATH;
                    }
                }
                else if ( !TestStayNoMan( pAStarIdx + i, BODILY4 ) )
                {
                    FreeNodes();
                    return nEndCode | TARGETNOPATH;
                }
                break;
            }
        }
        pNode->nG = 0;
        pNode->nH = Calculate( nSX, nSY, nEndX, nEndY );
        pNode->nF = pNode->nH;
        pNode->nX = nSX;
        pNode->nY = nSY;
        pNode->nNumber = i;
        pNode->cWhere = INOPEN;
        OpenChain.Insert( pNode );
        if ( ( unPathCount += 0x10 ) <= 0xf )
        {
            ClearAstarIdx();
            unPathCount = 0x10;
        }
        pAStarIdx[ i ].SetNode( pNode );
        pAStarIdx[ i ].SetCount( unPathCount );
        nEndN = nMapWidth * nEndY + nEndX;
        pEndASI = pAStarIdx + nEndN;
        if ( nFlags & HAS_MAN )
            for( i = 0; i < nLimitStep; i++ )
            {
                if ( ( pBestNode = ReturnBestNode() ) == NULL ) 	//不可到达的情况
                {
                    FreeNodes();
                    nEndCode |= ( nFlags & S_TO_E ) ? SOURCECLOSE : TARGETCLOSE;
                    goto error;
                }
                if ( pBestNode->nNumber == nEndN )					//到达
                {
                    break;
                }
                nEndCode |= GenerateSuccessorsHasMan( pBestNode );
            }
        else
        {
            for( i = 0; i < nLimitStep; i++ )
            {
                if ( ( pBestNode = ReturnBestNode() ) == NULL ) 	//不可到达的情况
                {
                    FreeNodes();
                    nEndCode |= ( nFlags & S_TO_E ) ? SOURCECLOSE : TARGETCLOSE;
                    goto error;
                }
                if ( pBestNode->nNumber == nEndN )					//到达
                {
                    break;
                }
                nEndCode |= GenerateSuccessorsNoMan( pBestNode );
            }
        }
        ASTAR_NODE* pTemp1, * pTemp2;
        char* pRecordWalkDirect;
        if ( nFlags & CONTINUESEARCH )
        {
            if ( i == nLimitStep ) 								//局部搜索路径满
            {
                if ( nFlags & HAS_MAN )
                {
                    SetManInAStarAtr( nNowX, nNowY, BSize );
                }
                FreeNodes();
                pStepPtr->AddStep( CONTINUESTEP );
                return NOFOUND | nEndCode;
            }
            else
            {
                pRecordWalkDirect = pStepPtr->GetStepArrayTail();
            }
        }
        else
        {
            if ( i == nLimitStep )
            {
                nEndCode |= NOFOUND;
            }
            pStepPtr->ClearStep();
            pRecordWalkDirect = pStepPtr->GetStepArray();
        }
        i = 0;
        pTemp1 = CloseChain.Get();
        if ( nFlags & S_TO_E )
        {
            while( pTemp1->pParent ) 								//记录路径,正向
            {
                pTemp2 = pTemp1->pParent;
                *pRecordWalkDirect = ( pTemp1->cDirect >= 0 ) ? pTemp1->cDirect : CalculateWay( pTemp1, pTemp2 );
                pTemp1 = pTemp2;
                pRecordWalkDirect++;
                i++;
            }
        }
        else
        {
            char MirrorPath[ MAXSEARCHSTEP ];
            pRecordWalkDirect = MirrorPath + MAXSEARCHSTEP - 1;		//记录路径,反向
            while( pTemp1->pParent )
            {
                pTemp2 = pTemp1->pParent;
                *pRecordWalkDirect = ( pTemp1->cDirect >= 0 ) ? MirrorTrasDirectTab[ pTemp1->cDirect ]
                : CalculateMirrWay( pTemp1, pTemp2 );
                pTemp1 = pTemp2;
                pRecordWalkDirect--;
                i++;
            }
            memcpy( pStepPtr->GetStepArrayTail(), pRecordWalkDirect + 1, i );
        }
        if ( nFlags & CONTINUESEARCH )
        {
            pStepPtr->AddStep( i );
        }
        else
        {
            pStepPtr->SetStep( i );
        }
        if ( nFlags & HAS_MAN )
        {
            SetManInAStarAtr( nNowX, nNowY, BSize );
        }
        FreeNodes();
        return ( HAVEPATH | RECORDPATH | nEndCode );
error:
        FreeNodes();
        if ( nFlags & CONTINUESEARCH )
        {
            pStepPtr->AddStep( CONTINUESTEP );
        }
        if ( nFlags & HAS_MAN )
        {
            SetManInAStarAtr( nNowX, nNowY, BSize );
        }
        return ( NOPATH | nEndCode );
#ifndef _NOTUSETRY
    }
    catch(...)
    {
        FreeNodes();
        return NOPATH;
    }
#endif
}

char ASTAR_8WAY::CalculateWay( ASTAR_NODE* pT1, ASTAR_NODE* pT2 )
{
    return DISTANCE2DIRECTION[ ( ( ( pT2->nX - pT1->nX ) & 0x3 ) << 2 ) | ( ( pT2->nY - pT1->nY ) & 0x3 ) ];
}

char ASTAR_8WAY::CalculateMirrWay( ASTAR_NODE* pT1, ASTAR_NODE* pT2 )
{
    return DISTANCE2MIRRORDIRECTION[ ( ( ( pT2->nX - pT1->nX ) & 0x3 ) << 2 ) | ( ( pT2->nY - pT1->nY ) & 0x3 ) ];
}

ASTAR_NODE* ASTAR_8WAY::ReturnBestNode( void )
{
    ASTAR_NODE* pTmp;
    if ( OpenChain.IsEmpty() )
    {
        return NULL;
    }
    pTmp = OpenChain.Get();							// point to first node on OPEN
    CloseChain.Insert( pTmp );
    return pTmp;
}

int ASTAR_8WAY::GenerateSuccessorsNoMan( ASTAR_NODE* pBestNode )
{
    int i;
    int nG, nH, nX, nY;
    ASTAR_IDX* pASI;
    ASTAR_NODE* pSuccessor, * pOld; //, ** ppTemp;
    pASI = pAStarIdx + pBestNode->nNumber;
    for( i = 0; i < 8; i++ )
    {
        pASI += LINEOFF[ i ];
        switch( BSize )
        {
        case BODILY1:
        default:
            if ( pASI->IsUnstay() )
            {
                continue;
            }
            break;
        case BODILY4:
            if ( !TestStayNoMan( pASI, BODILY4 ) )
            {
                continue;
            }
            break;
        }
        nG = StepLong[ i ] + pBestNode->nG;
        if ( pASI->IsAvail( unPathCount ) )
        {
            pOld = pASI->GetNode();
            pBestNode->pChild[ i ] = pOld;
            /*			ppTemp = pBestNode->pChild;

              #ifdef BORLANDCPPBUILD
              for( int c = 0; c < 8; c++, ppTemp++ )
              if ( ( *ppTemp ) == NULL )	// Add Old to the list of BestNode's Children (or Successors).
              break;
              ( *ppTemp ) = pOld;
              #else
              _asm{
              mov edi, ppTemp;
              mov ebx, pOld;
              mov ecx, 8;
              xor eax, eax;
              cld;
              repnz scasd;
              mov [ edi-4 ], ebx;
              }
            #endif*/
            if ( nG < pOld->nG )			// if our new g value is < Old's then reset Old's parent to point to BestNode
            {
                pOld->nG = nG;
                pOld->nF = nG + pOld->nH;
                pOld->pParent = pBestNode;
                pOld->cDirect = i;
                if ( pOld->cWhere == INCLOSE )
                {
                    PropagateDown( pOld );    // Since we changed the g value of Old, we need
                }
                // to propagate this new value downwards, i.e.
                // do a Depth-First traversal of the tree!
            }
        }
        else
        {
            if ( ( pSuccessor = AstarNode.CGetOne() ) == NULL )
            {
                break;
            }
            pASI->SetNode( pSuccessor );
            pASI->SetCount( unPathCount );
            nX = pBestNode->nX + COORDINATE_8WAY[ i ][ 0 ];
            nY = pBestNode->nY + COORDINATE_8WAY[ i ][ 1 ];
            pSuccessor->nG = nG;
            pSuccessor->nH = nH = Calculate( nX, nY, nEndX, nEndY );
            pSuccessor->nF = nG + nH;
            pSuccessor->nX = nX;
            pSuccessor->nY = nY;
            pSuccessor->nNumber = pBestNode->nNumber + MAN_BODILY_AROUND[ i ];
            pSuccessor->pParent = pBestNode;
            pSuccessor->cDirect = i;
            OpenChain.Insert( pSuccessor );				// Insert Successor on OPEN list wrt f
            pBestNode->pChild[ i ] = pSuccessor;
            /*			ppTemp = pBestNode->pChild;

              #ifdef BORLANDCPPBUILD
              for( int c =0; c < 8; c++, ppTemp++ )
              if ( ( *ppTemp ) == NULL )				// Add Old to the list of BestNode's Children (or Successors).
              break;
              ( *ppTemp) = pSuccessor;
              #else
              _asm{
              mov edi, ppTemp;
              mov ebx, pSuccessor;
              mov ecx, 8;
              xor eax, eax;
              cld;
              repnz scasd;
              mov [edi-4], ebx;
              }
            #endif*/
        }
        //if ( pASI == pEndASI ) break;
    }
    return 0;
}

int ASTAR_8WAY::GenerateSuccessorsHasMan( ASTAR_NODE* pBestNode )
{
    int i;
    int nG, nH, nX, nY;
    ASTAR_IDX* pASI;
    ASTAR_NODE* pSuccessor, * pOld; //, ** ppTemp;
    pASI = pAStarIdx + pBestNode->nNumber;
    for( i = 0; i < 8; i++ )
    {
        pASI += LINEOFF[ i ];
        switch( BSize )
        {
        case BODILY1:
        default:
            if ( pASI->IsUnstayCM() )
            {
                continue;
            }
            break;
        case BODILY4:
            if ( !TestStayHasMan( pASI, BODILY4 ) )
            {
                continue;
            }
            break;
        }
        nG = StepLong[ i ] + pBestNode->nG;
        if ( pASI->IsAvail( unPathCount ) )
        {
            pOld = pASI->GetNode();
            pBestNode->pChild[ i ] = pOld;
            /*			ppTemp = pBestNode->pChild;

              #ifdef BORLANDCPPBUILD
              for( int c = 0; c < 8; c++, ppTemp++ )
              if ( ( *ppTemp ) == NULL )	// Add Old to the list of BestNode's Children (or Successors).
              break;
              ( *ppTemp ) = pOld;
              #else
              _asm{
              mov edi, ppTemp;
              mov ebx, pOld;
              mov ecx, 8;
              xor eax, eax;
              cld;
              repnz scasd;
              mov [ edi-4 ], ebx;
              }
            #endif*/
            if ( nG < pOld->nG )			// if our new g value is < Old's then reset Old's parent to point to BestNode
            {
                pOld->nG = nG;
                pOld->nF = nG + pOld->nH;
                pOld->pParent = pBestNode;
                pOld->cDirect = i;
                if ( pOld->cWhere == INCLOSE )
                {
                    PropagateDown( pOld );    // Since we changed the g value of Old, we need
                }
                // to propagate this new value downwards, i.e.
                // do a Depth-First traversal of the tree!
            }
        }
        else
        {
            if ( ( pSuccessor = AstarNode.CGetOne() ) == NULL )
            {
                break;
            }
            pASI->SetNode( pSuccessor );
            pASI->SetCount( unPathCount );
            nX = pBestNode->nX + COORDINATE_8WAY[ i ][ 0 ];
            nY = pBestNode->nY + COORDINATE_8WAY[ i ][ 1 ];
            pSuccessor->nG = nG;
            pSuccessor->nH = nH = Calculate( nX, nY, nEndX, nEndY );
            pSuccessor->nF = nG + nH;
            pSuccessor->nX = nX;
            pSuccessor->nY = nY;
            pSuccessor->nNumber = pBestNode->nNumber + MAN_BODILY_AROUND[ i ];
            pSuccessor->pParent = pBestNode;
            pSuccessor->cDirect = i;
            OpenChain.Insert( pSuccessor );				// Insert Successor on OPEN list wrt f
            pBestNode->pChild[ i ] = pSuccessor;
            /*			ppTemp = pBestNode->pChild;

              #ifdef BORLANDCPPBUILD
              for( int c =0; c < 8; c++, ppTemp++ )
              if ( ( *ppTemp ) == NULL )				// Add Old to the list of BestNode's Children (or Successors).
              break;
              ( *ppTemp ) = pSuccessor;
              #else
              _asm{
              mov edi, ppTemp;
              mov ebx, pSuccessor;
              mov ecx, 8;
              xor eax, eax;
              cld;
              repnz scasd;
              mov [edi-4], ebx;
              }
            #endif*/
        }
        //if ( pASI == pEndASI ) break;
    }
    return 0;
}

void ASTAR_8WAY::PropagateDown( ASTAR_NODE* pOld )
{
    int				c, nG;
    ASTAR_NODE* 	pFather, ** ppTemp;
    int				tt = 0;
    AstarStack.Clear();
    nG = pOld->nG;								// alias.
    ppTemp = pOld->pChild;
    for ( c = 0; c < 8; c++, ppTemp++ )
    {
        if ( ( *ppTemp ) == NULL )				// create alias for faster access.
            //break;
        {
            continue;
        }
        if ( ( nG + StepLong[ c ] ) < (*ppTemp)->nG )
        {
            tt++;
            //(*ppTemp)->nG = nG;
            (*ppTemp)->nG = nG + StepLong[ c ];
            (*ppTemp)->nF =  + (*ppTemp)->nH;
            (*ppTemp)->pParent = pOld;			// reset parent to new path.
            //(*ppTemp)->cDirect = -1;
            (*ppTemp)->cDirect = c;
            AstarStack.Push( *ppTemp );			// Now the Child's branch need to be
        }										// checked out. Remember the new cost must be propagated down.
    }
    while ( !AstarStack.IsEmpty() )
    {
        pFather = AstarStack.Pop();
        ppTemp = pFather->pChild;
        nG = pFather->nG;
        for ( c = 0; c < 8; c++, ppTemp++ )
        {
            if ( (*ppTemp) == NULL )			// we may stop the propagation 2 ways: either
            {
                continue;
            }
            //break;
            if ( ( nG + StepLong[ c ] ) < (*ppTemp)->nG )			// there are no children, or that the g value of
            {
                // the child is equal or better than the cost we're propagating
                tt++;
                //(*ppTemp)->nG = nG;
                (*ppTemp)->nG = nG + StepLong[ c ];
                (*ppTemp)->nF = nG + (*ppTemp)->nH;
                (*ppTemp)->pParent = pFather;
                //(*ppTemp)->cDirect = -1;
                (*ppTemp)->cDirect = c;
                AstarStack.Push( *ppTemp );
            }
        }
    }
}

/*
void cpucount( unsigned int * h, unsigned int * l )
{
__asm {
cli;
_emit 0x0f
_emit 0x31
mov ebx,l;
mov [ebx],eax;
mov ebx,h;
mov [ebx],edx;
}
}*/

int ASTAR_8WAY::CheckContinue( int nNowX, int nNowY, int& nGoX, int& nGoY )
{
    int i;
    char* pRecordWalkDirect;
    i = pStepPtr->RemainSteps();
    if ( i >= ( CONTINUESTEP * 2 ) &&
    ( i + CONTINUESTEP * 3 ) < ( MAXSEARCHSTEP * 2 ) ) 		//更新当前一定范围行动路线
    {
        pRecordWalkDirect = pStepPtr->GetStepArrayTail() - 1;
        for( i = 0; i < CONTINUESTEP; i++, pRecordWalkDirect-- ) 	//计算局部搜索目标点
        {
            nNowX += COORDINATE_8WAY[ *pRecordWalkDirect ][ 0 ];
            nNowY += COORDINATE_8WAY[ *pRecordWalkDirect ][ 1 ];
        }
        pStepPtr->AddStep( -CONTINUESTEP );							//设置记录位置
        nGoX = nNowX;
        nGoY = nNowY;
        return CONTINUESEARCH;										//不检测起点,强制正向搜索
    }
    else
    {
        return NOENOUGHSTEP;
    }
}

/*bool TestContinue( short Sx, short Sy, short Dx, short Dy, char * RW, int Count )
{
int i;

  for( i = SManPtr->RecordWalk - 1; i >= 0; i-- ){
		Sx += ManWalkStep[ RW[ i ] ][ 0 ];
		Sy += ManWalkStep[ RW[ i ] ][ 1 ];
		}
		if ( Sx == SManPtr->GoX && Sy == SManPtr->GoY )
		return true;
		//if ( Sx == ggx && Sy == ggy )
		//	return true;
		return false;
}*/

int ASTAR_8WAY::CheckFront( int nNowX, int nNowY, int nGoX, int nGoY, int nFlags )
{
    int nEndCode;
    int nSX, nSY;
    nEndCode = 0;
    if ( nFlags & S_TO_E )
    {
        nSX = nGoX;	//反向搜索
        nSY = nGoY;
        nEndX = nNowX;
        nEndY = nNowY;
    }
    else
    {
        nSX = nNowX;	//正向搜索
        nSY = nNowY;
        nEndX = nGoX;
        nEndY = nGoY;
    }
    ASTAR_NODE* pBestNode, * pNode;
    int i, nEndN;
    FreeNodes();
    if ( ( pNode = AstarNode.CGetOne() ) == NULL )
    {
        goto error;
    }
    i = nMapWidth * nSY + nSX;										//检查目标点是否不可停留
    if ( nFlags & HAS_MAN )
    {
        ClrManInAStarAtr( nNowX, nNowY, BSize );
    }
    if ( nFlags & S_TO_E )
    {
        switch( BSize )
        {
        case BODILY1:
        default:
            if ( nFlags & HAS_MAN )
            {
                if ( pAStarIdx[ i ].IsUnstayCM() )
                {
                    SetManInAStarAtr( nNowX, nNowY, BODILY1 );
                    FreeNodes();
                    return nEndCode | TARGETNOPATH;
                }
            }
            else if ( pAStarIdx[ i ].IsUnstay() )
            {
                FreeNodes();
                return nEndCode | TARGETNOPATH;
            }
            break;
        case BODILY4:
            if ( nFlags & HAS_MAN )
            {
                if ( !TestStayHasMan( pAStarIdx + i, BODILY4 ) )
                {
                    SetManInAStarAtr( nNowX, nNowY, BODILY4 );
                    FreeNodes();
                    return nEndCode | TARGETNOPATH;
                }
            }
            else if ( !TestStayNoMan( pAStarIdx + i, BODILY4 ) )
            {
                FreeNodes();
                return nEndCode | TARGETNOPATH;
            }
            break;
        }
    }
    pNode->nG = 0;
    pNode->nH = Calculate( nSX, nSY, nEndX, nEndY );
    pNode->nF = pNode->nH;
    pNode->nX = nSX;
    pNode->nY = nSY;
    pNode->nNumber = i;
    pNode->cWhere = INOPEN;
    OpenChain.Insert( pNode );
    if ( ( unPathCount += 0x10 ) <= 0xf )
    {
        ClearAstarIdx();
        unPathCount = 0x10;
    }
    pAStarIdx[ i ].SetNode( pNode );
    pAStarIdx[ i ].SetCount( unPathCount );
    nEndN = nMapWidth * nEndY + nEndX;
    pEndASI = pAStarIdx + nEndN;
    if ( nFlags & HAS_MAN )
        for( i = 0; i < CHECKSTEP; i++ )
        {
            if ( ( pBestNode = ReturnBestNode() ) == NULL ) 	//不可到达的情况
            {
                FreeNodes();
                nEndCode |= ( nFlags & S_TO_E ) ? SOURCECLOSE : TARGETCLOSE;
                goto error;
            }
            if ( pBestNode->nNumber == nEndN )					//到达
            {
                break;
            }
            nEndCode |= GenerateSuccessorsHasMan( pBestNode );
        }
    else
        for( i = 0; i < CHECKSTEP; i++ )
        {
            if ( ( pBestNode = ReturnBestNode() ) == NULL ) 	//不可到达的情况
            {
                FreeNodes();
                nEndCode |= ( nFlags & S_TO_E ) ? SOURCECLOSE : TARGETCLOSE;
                goto error;
            }
            if ( pBestNode->nNumber == nEndN )					//到达
            {
                break;
            }
            nEndCode |= GenerateSuccessorsNoMan( pBestNode );
        }
    ASTAR_NODE* pTemp1, * pTemp2;
    char* pRecordWalkDirect;
    if ( i == CHECKSTEP ) 										//没找到,转完整搜索
    {
        if ( nFlags & HAS_MAN )
        {
            SetManInAStarAtr( nNowX, nNowY, BSize );
        }
        FreeNodes();
        return NOFOUND | nEndCode;
    }
    i = 0;
    pTemp1 = CloseChain.Get();
    if ( !( nFlags & S_TO_E ) )
    {
        pRecordWalkDirect = pStepPtr->GetStepArray();
        while( pTemp1->pParent ) 								//记录路径,正向
        {
            pTemp2 = pTemp1->pParent;
            *pRecordWalkDirect = ( pTemp1->cDirect >= 0 ) ? pTemp1->cDirect
            : CalculateWay( pTemp1, pTemp2 );
            pTemp1 = pTemp2;
            pRecordWalkDirect++;
            i++;
        }
    }
    else
    {
        char MirrorPath[ MAXSEARCHSTEP ];
        pRecordWalkDirect = MirrorPath + MAXSEARCHSTEP - 1;		//记录路径,反向
        while( pTemp1->pParent )
        {
            pTemp2 = pTemp1->pParent;
            *pRecordWalkDirect = ( pTemp1->cDirect >= 0 ) ? MirrorTrasDirectTab[ pTemp1->cDirect ] : CalculateMirrWay( pTemp1, pTemp2 );
            pTemp1 = pTemp2;
            pRecordWalkDirect--;
            i++;
        }
        memcpy( pStepPtr->GetStepArray(), pRecordWalkDirect + 1, i );
    }
    pStepPtr->SetStep( i );
    if ( nFlags & HAS_MAN )
    {
        SetManInAStarAtr( nNowX, nNowY, BSize );
    }
    FreeNodes();
    return ( HAVEPATH | RECORDPATH | nEndCode );
error:
    if ( nFlags & HAS_MAN )
    {
        SetManInAStarAtr( nNowX, nNowY, BSize );
    }
    FreeNodes();
    return ( NOPATH | nEndCode );
}

